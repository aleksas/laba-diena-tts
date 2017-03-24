///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas transcrLUSS.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>

#ifndef __TRANSCRLUSS_H__
#define __TRANSCRLUSS_H__

typedef int  (*PROCICCISSIII) (char*, char*, int, unsigned short*, unsigned short*, int*, int*, int);
typedef int  (*PROCAIC) (char*);

extern PROCICCISSIII	KircTranskr;
extern PROCAIC			initTranscrLUSS;

BOOL loadTranscrLUSS(char*);
void unloadTranscrLUSS();

#endif
