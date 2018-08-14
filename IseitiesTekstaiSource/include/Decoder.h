#ifndef DECODER_H_INCLUDED
#define DECODER_H_INCLUDED

#include <stdlib.h>

extern "C"
{

typedef void * DecoderHandle;

int  DecoderCreate(const char * szFilename, DecoderHandle * phDecoder);
void DecoderFree(DecoderHandle hDecoder);

int  DecoderDecodeSamples(DecoderHandle hDecoder, int64_t firstSample, int64_t sampleCount, int8_t * pBuffer, int64_t bufferSize, int64_t * pBytesWritten);
int  DecoderGetSampleCount(DecoderHandle hDecoder, int64_t * pValue);

const char * GetError(DecoderHandle hDecoder);

}
#endif // DECODER_H_INCLUDED
