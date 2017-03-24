///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas TextNormalization.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TextNormalization.h"

HINSTANCE	hinstDllTN;
PROCICC		initTextNorm;
PROCISSII	normalizeText;
PROCISSII	spellText;

BOOL loadTextNorm(char *biblioteka)
{
	if (!(hinstDllTN=LoadLibrary(biblioteka))) //"TextNormalization.dll"
		return FALSE;
	initTextNorm	=(PROCICC) GetProcAddress(hinstDllTN,"initTextNorm");
	normalizeText	=(PROCISSII) GetProcAddress(hinstDllTN,"normalizeText");
	spellText		=(PROCISSII) GetProcAddress(hinstDllTN,"spellText");

	if ((!initTextNorm) || (!normalizeText) || (!spellText))
	{
    	FreeLibrary (hinstDllTN); 
		hinstDllTN=NULL;
		return FALSE;
	}
	return TRUE;
}

void unloadTextNorm()
{
	if (hinstDllTN)
	{
		FreeLibrary (hinstDllTN);
		hinstDllTN=NULL;
	}
}