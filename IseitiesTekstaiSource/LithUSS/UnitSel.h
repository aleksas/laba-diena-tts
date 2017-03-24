///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas UnitSel.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>

#ifndef __UNITSEL_H__
#define __UNITSEL_H__

typedef int (*PROCBIC)(char *);
typedef int (*PROCBISSSIII)(unsigned short[],unsigned short[],unsigned short[],int,unsigned int[],unsigned int*);

extern PROCBIC initUnitSel;
extern PROCBISSSIII selectUnits;

BOOL loadUnitSel(char*);
void unloadUnitSel();

#endif