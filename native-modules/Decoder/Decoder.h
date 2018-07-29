#ifndef DECODER_H_INCLUDED
#define DECODER_H_INCLUDED

#include <stdlib.h>

typedef void (*init_playback_callback)(int bits, int channels, int sample_rate, void * callbackData);
typedef void (*play_callback)(char * buffer, int bufferSize, void * callbackData);

typedef void* DecoderHandle;

int CreateDecoder(const char * szFilename, DecoderHandle * phDecoder);
void FreeDecoder(DecoderHandle hDecoder);

int DecodeSamples(DecoderHandle hDecoder, int64_t firstSample, int64_t sampleCount, int8_t * pBuffer, int64_t bufferSize, int64_t * pBytesWritten);

const char * GetError(DecoderHandle hDecoder);

#endif // DECODER_H_INCLUDED
