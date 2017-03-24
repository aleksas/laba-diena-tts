///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas TextNormalization.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>

#ifndef __TEXTNORM_H__
#define __TEXTNORM_H__

typedef int (*PROCICC) (char *, char *);
typedef int (*PROCISSII)(char *, char*, int, int*);

extern PROCICC		initTextNorm;
extern PROCISSII	normalizeText;
extern PROCISSII	spellText;

BOOL loadTextNorm(char*);
void unloadTextNorm();

#endif