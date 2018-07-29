// Copyright (c) 2012 The Foundry Visionmongers Ltd.  All Rights Reserved.
// REFERENCE:
// https://learn.foundry.com/nuke/developers/7.0/ndkreference/examples/ffmpegReader.cpp
// https://stackoverflow.com/questions/19785254/play-a-video-from-memorystream-using-ffmpeg

#ifdef _WIN32
  #include <io.h>
#endif

#ifndef INT64_C
    #define INT64_C(c) (c ## LL)
    #define UINT64_C(c) (c ## ULL)
#endif

extern "C" {
    #include <errno.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libavutil/avutil.h>
    #include <libavutil/error.h>
    #include "libswresample/swresample.h"
}

#include <vector>
#include <string>
#include <memory>
#include <algorithm>

// Set non-zero to enable tracing of file information inspected while opening a file in FFmpegFile::FFmpegFile().
// Make sure this is disabled before checking-in this file.
#define TRACE_FILE_OPEN 0

// Set non-zero to enable tracing of FFmpegFile::decode() general processing (plus error reports).
// Make sure this is disabled before checking-in this file.
#define TRACE_DECODE_PROCESS 0

// Set non-zero to enable tracing of the first few bytes of each data block in the bitstream for each frame decoded. This
// assumes a 4-byte big-endian byte count at the start of each data block, followed by that many bytes of data. There may be
// multiple data blocks per frame. Each data block represents an atomic unit of data input to the decoder (e.g. an H.264 NALU).
// It works nicely for H.264 streams in .mov files but there's no guarantee for any other format. This can be useful if you
// need to see what kind of NALUs are being decoded.
// Make sure this is disabled before checking-in this file.
#define TRACE_DECODE_BITSTREAM 0

namespace
{

#define CHECK(x) \
{\
  int error = x;\
  if (error<0) {\
    setInternalError(error);\
    return;\
  }\
}\

  class FFmpegFile
  {
    struct Stream  
    {
      int _idx;                      // stream index
      AVStream* _avstream;           // video stream
      AVCodecContext* _codecContext; // video codec context
      AVCodec* _codec;
      AVFrame* _avFrame;             // decoding frame
      SwrContext *_swr;
      uint8_t * _swr_buffer;

      int _fpsNum;
      int _fpsDen;

      int64_t _startPTS;     // PTS of the first frame in the stream
      int64_t _frames;       // video duration in frames
      int64_t _samples;       // video duration in frames
      int64_t _bitsPerSample;       // video duration in frames
      int64_t _frame_size;       // video duration in frames

      bool _ptsSeen;                      // True if a read AVPacket has ever contained a valid PTS during this stream's decode,
                                          // indicating that this stream does contain PTSs.
      int64_t AVPacket::*_timestampField; // Pointer to member of AVPacket from which timestamps are to be retrieved. Enables
                                          // fallback to using DTSs for a stream if PTSs turn out not to be available.

      int _width;
      int _height;  
      double _aspect;

      int _decodeNextFrameIn; // The 0-based index of the next frame to be fed into decode. Negative before any
                              // frames have been decoded or when we've just seeked but not yet found a relevant frame. Equal to
                              // frames_ when all available frames have been fed into decode.

      int _decodeNextFrameOut; // The 0-based index of the next frame expected out of decode. Negative before
                               // any frames have been decoded or when we've just seeked but not yet found a relevant frame. Equal to
                               // frames_ when all available frames have been output from decode.

      int _accumDecodeLatency; // The number of frames that have been input without any frame being output so far in this stream
                               // since the last seek. This is part of a guard mechanism to detect when decode appears to have
                               // stalled and ensure that FFmpegFile::decode() does not loop indefinitely.

      Stream() 
        : _idx(0)
        , _avstream(NULL)
        , _codecContext(NULL)
        , _swr(NULL)
        , _swr_buffer(NULL)
        , _codec(NULL)
        , _avFrame(NULL)
        , _fpsNum(1)
        , _fpsDen(1)
        , _startPTS(0)
        , _frames(0)
        , _samples(0)
        , _bitsPerSample(0)
        , _ptsSeen(false)
        , _timestampField(&AVPacket::pts)
        , _width(0)
        , _height(0)
        , _aspect(1.0)
        , _decodeNextFrameIn(-1)
        , _decodeNextFrameOut(-1)
        , _accumDecodeLatency(0)
      {        
      }

      ~Stream()
      {

       if (_avFrame)
         av_free(_avFrame);

        if (_codecContext)
          avcodec_close(_codecContext);  

        if (_swr)
          swr_free(&_swr);    

        if (_swr_buffer){
          free(_swr_buffer); 
          _swr_buffer = NULL;
          }    
      }

      static void destroy(Stream* s)
      {
        delete(s);
      }

      int64_t frameToPts(int frame) const
      {
          int64_t pts = _startPTS;
        pts += ceil((double)(int64_t(frame) * _codecContext->frame_size *  _avstream->time_base.den) / 
                                    (int64_t(_codecContext->sample_rate) * _avstream->time_base.num));

        return pts;
      }

      int ptsToFrame(int64_t pts) const 
      {
        return ceil((double)(int64_t(pts - _startPTS) * _avstream->time_base.num *  _codecContext->sample_rate) / 
                                  (int64_t(_avstream->time_base.den) * _codecContext->frame_size));
      }

      // Return the number of input frames needed by this stream's codec before it can produce output. We expect to have to
      // wait this many frames to receive output; any more and a decode stall is detected.
      int getCodecDelay() const
      {
        return ((_codec->capabilities & CODEC_CAP_DELAY) ? _codecContext->delay : 0) + _codecContext->has_b_frames;
      }
    };

    // AV structure
    AVFormatContext* _context;
    AVInputFormat*   _format;

    // store all video streams available in the file
    std::vector<Stream*> _streams;

    // reader error state
    std::string _errorMsg;  // internal decoding error string
    bool _invalidState;     // true if the reader is in an invalid state

    AVPacket _avPacket;

    // set reader error
    void setError(const char* msg, const char* prefix = 0)
    {
      if (prefix) {
        _errorMsg = prefix;
        _errorMsg += msg;
      }
      else {
        _errorMsg = msg;
      }
      _invalidState = true;
    }

    // set FFmpeg library error
    void setInternalError(const int error, const char* prefix = 0) 
    {
      char errorBuf[1024];
      av_strerror(error, errorBuf, sizeof(errorBuf));
      setError(errorBuf, prefix);
    }

    // get stream start time
    int64_t getStreamStartTime(Stream& stream)
    {

      // Read from stream. If the value read isn't valid, get it from the first frame in the stream that provides such a
      // value.
      int64_t startPTS = stream._avstream->start_time;

      if (startPTS ==  int64_t(AV_NOPTS_VALUE)) {

        // Seek 1st key-frame in video stream.
        avcodec_flush_buffers(stream._codecContext);

        if (av_seek_frame(_context, stream._idx, 0, 0) >= 0) {
          av_init_packet(&_avPacket);

          // Read frames until we get one for the video stream that contains a valid PTS.
          do {
            if (av_read_frame(_context, &_avPacket) < 0)  {
              // Read error or EOF. Abort search for PTS.
              break;
            }
            if (_avPacket.stream_index == stream._idx) {
              // Packet read for video stream. Get its PTS. Loop will continue if the PTS is AV_NOPTS_VALUE.
              startPTS = _avPacket.pts;
            }

            av_free_packet(&_avPacket);
          } while (startPTS ==  int64_t(AV_NOPTS_VALUE));
        }

      }

      // If we still don't have a valid initial PTS, assume 0. (This really shouldn't happen for any real media file, as
      // it would make meaningful playback presentation timing and seeking impossible.)
      if (startPTS ==  int64_t(AV_NOPTS_VALUE)) {
        startPTS = 0;
      }


      return startPTS;
    }

    // Get the video stream duration in frames...
    int64_t getStreamFrames(Stream& stream)
    {

      // Obtain from stream's number of frames if specified. Will be 0 if unknown.
      int64_t frames = stream._avstream->nb_frames;

      // If number of frames still unknown, attempt to calculate from stream's duration, fps and timebase. This is
      // preferable to using AVFormatContext's duration as this value is exactly specified using rational numbers, so can
      // correctly represent durations that would be infinitely recurring when expressed in decimal form. By contrast,
      // durations in AVFormatContext are represented in units of AV_TIME_BASE (1000000), so may be imprecise, leading to
      // loss of an otherwise-available frame.
      if (!frames) {
        frames = ceil((double)(int64_t(stream._avstream->duration) * stream._avstream->time_base.num  * stream._codecContext->sample_rate) /
                                               (int64_t(stream._avstream->time_base.den) * stream._codecContext->frame_size));
      }

      // If the number of frames is still unknown, attempt to measure it from the last frame PTS for the stream in the
      // file relative to first (which we know from earlier).
      if (!frames) {

        int64_t maxPts = stream._startPTS;

        // Seek last key-frame.
        avcodec_flush_buffers(stream._codecContext);
        av_seek_frame(_context, stream._idx, stream.frameToPts(1<<29), AVSEEK_FLAG_BACKWARD);

        // Read up to last frame, extending max PTS for every valid PTS value found for the video stream.
        av_init_packet(&_avPacket);

        while (av_read_frame(_context, &_avPacket) >= 0) {
          if (_avPacket.stream_index == stream._idx && _avPacket.pts != int64_t(AV_NOPTS_VALUE) && _avPacket.pts > maxPts)
            maxPts = _avPacket.pts;
          av_free_packet(&_avPacket);
        }

        // Compute frame range from min to max PTS. Need to add 1 as both min and max are at starts of frames, so stream
        // extends for 1 frame beyond this.
        frames = 1 + stream.ptsToFrame(maxPts);
      }


      return frames;
    }

    // Get the video stream duration in frames...
    int64_t getStreamSamples(Stream& stream)
    {
        return getStreamFrames(stream) * stream._codecContext->frame_size;
    }

    int getBitsPerSample(AVSampleFormat sfmt)
    {
      int bits = -1;

      switch(sfmt){
          case AV_SAMPLE_FMT_U8:
          case AV_SAMPLE_FMT_U8P:
              bits = 8;
              break;
          case AV_SAMPLE_FMT_S16:
          case AV_SAMPLE_FMT_S16P:
              bits = 16;
              break;
          case AV_SAMPLE_FMT_S32:
          case AV_SAMPLE_FMT_S32P:
              bits = 32;
              break;
          case AV_SAMPLE_FMT_FLT:
          case AV_SAMPLE_FMT_FLTP:
              bits = 16;
              break;
      }

      return bits;

    }

  public:

    typedef std::auto_ptr<FFmpegFile> Ptr;

    // constructor
    FFmpegFile(const char* filename)
      : _context(NULL)
      , _format(NULL)
      , _invalidState(false)      
    {
      // FIXME_GC: shouldn't the plugin be passed the filename without the prefix?
      int offset = 0;
      if (std::string(filename).find("ffmpeg:") != std::string::npos)
        offset = 7;

      
      CHECK( avformat_open_input(&_context, filename + offset, _format, NULL) );
      CHECK( avformat_find_stream_info(_context, NULL) );


      // fill the array with all available video streams
      bool unsuported_codec = false;

      // find all streams that the library is able to decode
      for (unsigned i = 0; i < _context->nb_streams; ++i) {
        AVStream* avstream = _context->streams[i];

        // be sure to have a valid stream
        if (!avstream || !avstream->codec) {
          continue;
        }

        // considering only video streams, skipping audio
        if (avstream->codec->codec_type != AVMEDIA_TYPE_AUDIO) {
          continue;
        }

        // find the codec
        AVCodec* codec = avcodec_find_decoder(avstream->codec->codec_id);
        if (codec == NULL) {
          continue;
        }

        // skip if the codec can't be open
        if (avcodec_open2(avstream->codec, codec, NULL) < 0) {
          continue;
        }

        Stream* stream = new Stream();
        stream->_idx = i;
        stream->_avstream = avstream;
        stream->_codecContext = avstream->codec;
        stream->_codec = codec;
        stream->_avFrame = av_frame_alloc();

        stream->_swr = swr_alloc();
        av_opt_set_int(stream->_swr, "in_channel_layout",  avstream->codec->channel_layout, 0);
        av_opt_set_int(stream->_swr, "out_channel_layout", avstream->codec->channel_layout,  0);
        av_opt_set_int(stream->_swr, "in_sample_rate",     avstream->codec->sample_rate, 0);
        av_opt_set_int(stream->_swr, "out_sample_rate",    avstream->codec->sample_rate, 0);
        av_opt_set_sample_fmt(stream->_swr, "in_sample_fmt",  AV_SAMPLE_FMT_FLTP, 0);
        av_opt_set_sample_fmt(stream->_swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
        swr_init(stream->_swr);

        // If FPS is specified, record it. 
        // Otherwise assume 1 fps (default value).
        if ( avstream->r_frame_rate.num != 0 &&  avstream->r_frame_rate.den != 0 ) {
          stream->_fpsNum = avstream->r_frame_rate.num;
          stream->_fpsDen = avstream->r_frame_rate.den;
        } 

        stream->_width  = avstream->codec->width;
        stream->_height = avstream->codec->height;

        // set aspect ratio
        if (stream->_avstream->sample_aspect_ratio.num) {
          stream->_aspect = av_q2d(stream->_avstream->sample_aspect_ratio);
        }
        else if (stream->_codecContext->sample_aspect_ratio.num) {
          stream->_aspect = av_q2d(stream->_codecContext->sample_aspect_ratio);
        }

        // set stream start time and numbers of frames
        stream->_startPTS = getStreamStartTime(*stream);
        stream->_frames   = getStreamFrames(*stream);
        stream->_samples  = getStreamSamples(*stream);
        stream->_bitsPerSample = getBitsPerSample(stream->_codecContext->sample_fmt);
        stream->_frame_size = stream->_codecContext->frame_size;
          
        // save the stream
        _streams.push_back(stream);
      }

      if (_streams.empty())
        setError( unsuported_codec ? "unsupported codec..." : "unable to find video stream" );
    }

    // destructor
    ~FFmpegFile()
    {
      // force to close all resources needed for all streams
      std::for_each(_streams.begin(), _streams.end(), Stream::destroy);

      if (_context)
        avformat_close_input(&_context);
    }

    // get the internal error string
    const char* error() const
    {
      return _errorMsg.c_str();
    }

    // return true if the reader can't decode the frame
    bool invalid() const
    {
      return _invalidState;
    }

    // return the numbers of streams supported by the reader
    unsigned streams() const
    {
      return _streams.size();
    }

    int64_t decodeSamples(int64_t firstSample, int64_t sampleCount, int8_t * pBuffer, int64_t bufferSize, unsigned streamIdx = 0)
    {
        int64_t bytesWritten = 0;
        int64_t lastSample = firstSample + sampleCount;

        Stream* stream = _streams[streamIdx];

        if (streamIdx >= _streams.size()) return false;
        if (firstSample < 0) return false;
        if (lastSample >= stream->_samples) return false;

        int firstFrame = floor((double)firstSample / stream->_frame_size);
        int lastFrame = ceil((double)lastSample / stream->_frame_size);
        int _ltrim = (firstSample - firstFrame * stream->_frame_size) * stream->_bitsPerSample / 8;
        int _rtrim = (lastFrame * stream->_frame_size - lastSample) * stream->_bitsPerSample / 8;

        for (int f = firstFrame; f < lastFrame ; f++)
        {
          int64_t _bytesWritten = 0;

          int ltrim = (f == firstFrame) ? _ltrim : 0;
          int rtrim = (f == lastFrame) ? _rtrim : 0;

          _bytesWritten = decode(f, ltrim, rtrim, pBuffer + bytesWritten, bufferSize - bytesWritten);
          
          if (_bytesWritten >= 0) {
            bytesWritten += _bytesWritten;      
          } else {
            return _bytesWritten;
          }
        }

        return bytesWritten;
    }

    // return number if bytes written 
    int decode(unsigned frame, int ltrim, int rtrim, int8_t * pBuffer, int64_t bufferSize, unsigned streamIdx = 0)
    {
      if (streamIdx >= _streams.size())
        return -1;

      if (pBuffer == NULL)
        return -2;

      // get the stream
      Stream* stream = _streams[streamIdx];

      // Translate from the 1-based frames expected by Nuke to 0-based frame offsets for use in the rest of this code.
      int desiredFrame = frame;

      // Early-out if out-of-range frame requested.
      if (desiredFrame < 0 || desiredFrame >= stream->_frames)
        return -3;


      // Number of read retries remaining when decode stall is detected before we give up (in the case of post-seek stalls,
      // such retries are applied only after we've searched all the way back to the start of the file and failed to find a
      // successful start point for playback)..
      //
      // We have a rather annoying case with a small subset of media files in which decode latency (between input and output
      // frames) will exceed the maximum above which we detect decode stall at certain frames on the first pass through the
      // file but those same frames will decode succesfully on a second attempt. The root cause of this is not understood but
      // it appears to be some oddity of FFmpeg. While I don't really like it, retrying decode enables us to successfully
      // decode those files rather than having to fail the read.
      int retriesRemaining = 1;

      // Whether we have just performed a seek and are still awaiting the first decoded frame after that seek. This controls
      // how we respond when a decode stall is detected.
      //
      // One cause of such stalls is when a file contains incorrect information indicating that a frame is a key-frame when it
      // is not; a seek may land at such a frame but the decoder will not be able to start decoding until a real key-frame is
      // reached, which may be a long way in the future. Once a frame has been decoded, we will expect it to be the first frame
      // input to decode but it will actually be the next real key-frame found, leading to subsequent frames appearing as
      // earlier frame numbers and the movie ending earlier than it should. To handle such cases, when a stall is detected
      // immediately after a seek, we seek to the frame before the previous seek's landing frame, allowing us to search back
      // through the movie for a valid key frame from which decode commences correctly; if this search reaches the beginning of
      // the movie, we give up and fail the read, thus ensuring that this method will exit at some point.
      //
      // Stalls once seeking is complete and frames are being decoded are handled differently; these result in immediate read
      // failure.
      bool awaitingFirstDecodeAfterSeek = false;

      // If the frame we want is not the next one to be decoded, seek to the keyframe before/at our desired frame. Set the last
      // seeked frame to indicate that we need to synchronise frame indices once we've read the first frame of the video stream,
      // since we don't yet know which frame number the seek will land at. Also invalidate current indices, reset accumulated
      // decode latency and record that we're awaiting the first decoded frame after a seek.
      int lastSeekedFrame = -1; // 0-based index of the last frame to which we seeked when seek in progress / negative when no
                                // seek in progress,

      int64_t bytesWritten = 0;

      if (desiredFrame != stream->_decodeNextFrameOut) {

        lastSeekedFrame = desiredFrame;
        stream->_decodeNextFrameIn  = -1;
        stream->_decodeNextFrameOut = -1;
        stream->_accumDecodeLatency = 0;
        awaitingFirstDecodeAfterSeek = true;

        avcodec_flush_buffers(stream->_codecContext);
        int error = av_seek_frame(_context, stream->_idx, stream->frameToPts(desiredFrame), AVSEEK_FLAG_BACKWARD);
        if (error < 0) {
          // Seek error. Abort attempt to read and decode frames.
          setInternalError(error, "FFmpeg Reader failed to seek frame: ");
          return -4;
        }
      }

      av_init_packet(&_avPacket);

      //_avPacket.data = buf;
      //_avPacket.size = 2400;

      // Loop until the desired frame has been decoded. May also break from within loop on failure conditions where the
      // desired frame will never be decoded.
      bool hasFrame = false;
      do {
        bool decodeAttempted = false;
        int frameDecoded = 0;

        // If the next frame to decode is within range of frames (or negative implying invalid; we've just seeked), read
        // a new frame from the source file and feed it to the decoder if it's for the video stream.
        if (stream->_decodeNextFrameIn < stream->_frames) {

          int error = av_read_frame(_context, &_avPacket);
          if (error < 0) {
            // Read error. Abort attempt to read and decode frames.
            setInternalError(error, "FFmpeg Reader failed to read frame: ");
            break;
          }

          // If the packet read belongs to the video stream, synchronise frame indices from it if required and feed it
          // into the decoder.
          if (_avPacket.stream_index == stream->_idx) {

            // If the packet read has a valid PTS, record that we have seen a PTS for this stream.
            if (_avPacket.pts != int64_t(AV_NOPTS_VALUE))
              stream->_ptsSeen = true;

            // If a seek is in progress, we need to synchronise frame indices if we can...
            if (lastSeekedFrame >= 0) {

              // Determine which frame the seek landed at, using whichever kind of timestamp is currently selected for this
              // stream. If there's no timestamp available at that frame, we can't synchronise frame indices to know which
              // frame we're first going to decode, so we need to seek back to an earlier frame in hope of obtaining a
              // timestamp. Likewise, if the landing frame is after the seek target frame (this can happen, presumably a bug
              // in FFmpeg seeking), we need to seek back to an earlier frame so that we can start decoding at or before the
              // desired frame.
              int landingFrame;
              if (_avPacket.*stream->_timestampField == int64_t(AV_NOPTS_VALUE) ||
                  (landingFrame = stream->ptsToFrame(_avPacket.*stream->_timestampField)) > lastSeekedFrame) {

                // Wind back 1 frame from last seeked frame. If that takes us to before frame 0, we're never going to be
                // able to synchronise using the current timestamp source...
                if (--lastSeekedFrame < 0) {

                  // If we're currently using PTSs to determine the landing frame and we've never seen a valid PTS for any
                  // frame from this stream, switch to using DTSs and retry the read from the initial desired frame.
                  if (stream->_timestampField == &AVPacket::pts && !stream->_ptsSeen) {
                    stream->_timestampField = &AVPacket::dts;
                    lastSeekedFrame = desiredFrame;
                  }
                  // Otherwise, failure to find a landing point isn't caused by an absence of PTSs from the file or isn't
                  // recovered by using DTSs instead. Something is wrong with the file. Abort attempt to read and decode frames.
                  else {
                    setError("FFmpeg Reader failed to find timing reference frame, possible file corruption");
                    break;
                  }
                }

                // Seek to the new frame. By leaving the seek in progress, we will seek backwards frame by frame until we
                // either successfully synchronise frame indices or give up having reached the beginning of the stream.
                avcodec_flush_buffers(stream->_codecContext);
                error = av_seek_frame(_context, stream->_idx, stream->frameToPts(lastSeekedFrame), AVSEEK_FLAG_BACKWARD);
                if (error < 0) {
                  // Seek error. Abort attempt to read and decode frames.
                  setInternalError(error, "FFmpeg Reader failed to seek frame: ");
                  break;
                }
              }
              // Otherwise, we have a valid landing frame, so set that as the next frame into and out of decode and set
              // no seek in progress.
              else {
                stream->_decodeNextFrameOut = stream->_decodeNextFrameIn = landingFrame;
                lastSeekedFrame = -1;
              }
            }

            // If there's no seek in progress, feed this frame into the decoder.
            if (lastSeekedFrame < 0) {
              // Advance next frame to input.
              ++stream->_decodeNextFrameIn;

              // Decode the frame just read. frameDecoded indicates whether a decoded frame was output.
              decodeAttempted = true;
              error = avcodec_decode_audio4(stream->_codecContext, stream->_avFrame, &frameDecoded, &_avPacket);
              if (error < 0) {
                // Decode error. Abort attempt to read and decode frames.
                setInternalError(error, "FFmpeg Reader failed to decode frame: ");
                break;
              }
            }
          }
        }

        // If the next frame to decode is out of frame range, there's nothing more to read and the decoder will be fed
        // null input frames in order to obtain any remaining output.
        else {

          // Obtain remaining frames from the decoder. pkt_ contains NULL packet data pointer and size at this point,
          // required to pump out remaining frames with no more input. frameDecoded indicates whether a decoded frame
          // was output.
          decodeAttempted = true;
          int error = avcodec_decode_audio4(stream->_codecContext, stream->_avFrame, &frameDecoded, &_avPacket);
          if (error < 0) {
            // Decode error. Abort attempt to read and decode frames.
            setInternalError(error, "FFmpeg Reader failed to decode frame: ");
            break;
          }
        }

        // If a frame was decoded, ...
        if (frameDecoded) {
          // Now that we have had a frame decoded, we know that seek landed at a valid place to start decode. Any decode
          // stalls detected after this point will result in immediate decode failure.
          awaitingFirstDecodeAfterSeek = false;

          // If the frame just output from decode is the desired one, get the decoded picture from it and set that we
          // have a picture.
          if (stream->_decodeNextFrameOut == desiredFrame) {
            int data_size = stream->_avFrame->linesize[0];
            char * pbuf = (char *) stream->_avFrame->data[0];

            if (stream->_codecContext->sample_fmt == AV_SAMPLE_FMT_FLTP) {
              stream->_swr_buffer = (uint8_t*) realloc(stream->_swr_buffer, data_size);
              int converted = swr_convert(stream->_swr, &stream->_swr_buffer, stream->_avFrame->nb_samples, (const uint8_t **) stream->_avFrame->extended_data, stream->_avFrame->nb_samples);   
              if (converted >= 0) {
                pbuf = (char*) stream->_swr_buffer;
                data_size = converted * stream->_bitsPerSample / 8;
              } else {
                setError("Failed to cenvert data");
                break;
              }
            }

            if (bufferSize < data_size - ltrim - rtrim) 
            {
                setError("Buffer size too small.");
                return -5;
            } 

            memcpy(pBuffer, pbuf + ltrim, data_size - ltrim - rtrim);
            bytesWritten += data_size - ltrim - rtrim;
            hasFrame = true;
          }

          // Advance next output frame expected from decode.
          ++stream->_decodeNextFrameOut;
        }
        // If no frame was decoded but decode was attempted, determine whether this constitutes a decode stall and handle if so.
        else if (decodeAttempted) {
          // Failure to get an output frame for an input frame increases the accumulated decode latency for this stream.
          ++stream->_accumDecodeLatency;

          
          // If the accumulated decode latency exceeds the maximum delay permitted for this codec at this time (the delay can
          // change dynamically if the codec discovers B-frames mid-stream), we've detected a decode stall.
          if (stream->_accumDecodeLatency > stream->getCodecDelay()) {
            int seekTargetFrame; // Target frame for any seek we might perform to attempt decode stall recovery.
            
            // Handle a post-seek decode stall.
            if (awaitingFirstDecodeAfterSeek) {
              // If there's anywhere in the file to seek back to before the last seek's landing frame (which can be found in
              // stream->_decodeNextFrameOut, since we know we've not decoded any frames since landing), then set up a seek to
              // the frame before that landing point to try to find a valid decode start frame earlier in the file.
              if (stream->_decodeNextFrameOut > 0) {
                seekTargetFrame = stream->_decodeNextFrameOut - 1;
              }
              // Otherwise, there's nowhere to seek back. If we have any retries remaining, use one to attempt the read again,
              // starting from the desired frame.
              else if (retriesRemaining > 0) {
                --retriesRemaining;
                seekTargetFrame = desiredFrame;
              }
              // Otherwise, all we can do is to fail the read so that this method exits safely.
              else {
                setError("FFmpeg Reader failed to find decode reference frame, possible file corruption");
                break;
              }
            }
            // Handle a mid-decode stall. All we can do is to fail the read so that this method exits safely.
            else {
              // If we have any retries remaining, use one to attempt the read again, starting from the desired frame.
              if (retriesRemaining > 0) {
                --retriesRemaining;
                seekTargetFrame = desiredFrame;
              }
              // Otherwise, all we can do is to fail the read so that this method exits safely.
              else {
                setError("FFmpeg Reader detected decoding stall, possible file corruption");
                break;
              }
            }

            // If we reach here, seek to the target frame chosen above in an attempt to recover from the decode stall.
            lastSeekedFrame = seekTargetFrame;
            stream->_decodeNextFrameIn  = -1;
            stream->_decodeNextFrameOut = -1;
            stream->_accumDecodeLatency = 0;
            awaitingFirstDecodeAfterSeek = true;

            avcodec_flush_buffers(stream->_codecContext);
            int error = av_seek_frame(_context, stream->_idx, stream->frameToPts(seekTargetFrame), AVSEEK_FLAG_BACKWARD);
            if (error < 0) {
              // Seek error. Abort attempt to read and decode frames.
              setInternalError(error, "FFmpeg Reader failed to seek frame: ");
              break;
            }
          }
        }

        av_free_packet(&_avPacket);
      } while (!hasFrame);


      // If read failed, reset the next frame expected out so that we seek and restart decode on next read attempt. Also free
      // the AVPacket, since it won't have been freed at the end of the above loop (we reach here by a break from the main
      // loop when hasFrame is false).
      if (!hasFrame) {
        if (_avPacket.size > 0)
          av_free_packet(&_avPacket);
        stream->_decodeNextFrameOut = -1;
      } 

      return bytesWritten;
    }

    // get stream information
    bool info( int64_t & frames, int64_t & samplesPerFrame, int64_t & bitsPerSample, int & channels, int & sampleRate, unsigned streamIdx = 0)
    {
        if (streamIdx >= _streams.size())
            return false;

        // get the stream
        Stream* stream = _streams[streamIdx];
        frames = stream->_frames;
        samplesPerFrame = stream->_frame_size;
        bitsPerSample = stream->_bitsPerSample;
        channels = stream->_codecContext->channels;
        sampleRate = stream->_codecContext->sample_rate;

        return true;
    }
    
  };
}
