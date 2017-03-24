///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LIEPA.exe
// Failas LithUSS.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>

#ifndef __LITHUSS_H__
#define __LITHUSS_H__

struct event {short Id; short phonviz; int charOffset; long signOffset;}; 

typedef int (*PROCICC) (char*, char*);
typedef int (*PROCISSII)(char*, char*, int, int*);
typedef int (*PROCISSIUUII)(char*, char*, int, unsigned short*, unsigned short*, int*, int*);
typedef int (*PROCIUUIUU)(unsigned short*, unsigned short*, int, unsigned int*, unsigned short*);
typedef int (*PROCIIUUUS)(int, unsigned short, unsigned int, short**);
typedef char* (*PROCSU)(unsigned short);
typedef int (*PROCICSLEIII)(char*, short*, long*, event*, int*, int, int);

extern PROCICC		initLUSS;
extern PROCISSII	normalizeText1;
extern PROCISSIUUII	stressTranscr;
extern PROCIUUIUU	findUnits;
extern PROCIIUUUS	synthesizePhoneme;
extern PROCSU		id2fv;
extern PROCICSLEIII	synthesizeWholeText;

BOOL loadLUSS(char*);
void unloadLUSS();

#endif