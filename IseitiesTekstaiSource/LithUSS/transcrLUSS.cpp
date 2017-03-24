///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas transcrLUSS.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "transcrLUSS.h"

HINSTANCE		hinstDlltranscrLUSS;
PROCICCISSIII	KircTranskr;
PROCAIC			initTranscrLUSS;

BOOL loadTranscrLUSS(char *biblioteka)
{
	if (!(hinstDlltranscrLUSS=LoadLibrary(biblioteka))) //"transcrLUSS.dll"
		return FALSE;
	KircTranskr		=(PROCICCISSIII) GetProcAddress(hinstDlltranscrLUSS,"KircTranskr");
	initTranscrLUSS	=(PROCAIC) GetProcAddress(hinstDlltranscrLUSS,"initTranscrLUSS");

	if ((!KircTranskr) || (!initTranscrLUSS))
	{
    	FreeLibrary (hinstDlltranscrLUSS); 
		hinstDlltranscrLUSS=NULL;
		return FALSE;
	}
	return TRUE;
}

void unloadTranscrLUSS()
{
	if (hinstDlltranscrLUSS)
	{
		FreeLibrary (hinstDlltranscrLUSS);
		hinstDlltranscrLUSS=NULL;
	}
}
