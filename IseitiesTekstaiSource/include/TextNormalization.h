///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra�tija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas TextNormalization.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTNORM_H__
#define __TEXTNORM_H__

#include "common.h"

extern "C" {

EXPORT int initTextNorm(const char * rulesFilesDirectory, const char * rulesFileName);
EXPORT int normalizeText(char * buffer, char * retBuffer, int bufferSize, int * letPos);
EXPORT int spellText(char * buffer, char * retBuffer, int bufferSize, int * letPos);

EXPORT void unloadTextNorm();

}

#endif