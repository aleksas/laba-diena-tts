#ifndef TEXTNORMALIZATION_H_INCLUDED
#define TEXTNORMALIZATION_H_INCLUDED

#include "../include/common.h"

EXPORT int initTextNorm(char * rulesFilesDirectory, const char * rulesFileName);
EXPORT int normalizeText(char * buffer, char * retBuffer, int bufferSize, int * letPos);
EXPORT int spellText(char * buffer, char * retBuffer, int bufferSize, int * letPos);

EXPORT BOOL loadTextNorm(char*);
EXPORT void unloadTextNorm();

#endif
