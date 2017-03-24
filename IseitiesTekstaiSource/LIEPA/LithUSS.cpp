///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LIEPA.exe
// Failas LithUSS.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LithUSS.h"

HINSTANCE	hinstDllLUSS;
PROCICC		initLUSS;
PROCISSII	normalizeText1;
PROCISSIUUII	stressTranscr;
PROCIUUIUU	findUnits;
PROCIIUUUS	synthesizePhoneme;
PROCSU		id2fv;
PROCICSLEIII	synthesizeWholeText;

BOOL loadLUSS(char *biblioteka)
{
	if (!(hinstDllLUSS=LoadLibrary(biblioteka))) //"LithUSS.dll"
		return FALSE;
	initLUSS			=(PROCICC) GetProcAddress(hinstDllLUSS,"initLUSS");
	normalizeText1		=(PROCISSII) GetProcAddress(hinstDllLUSS,"normalizeText1");
	stressTranscr		=(PROCISSIUUII) GetProcAddress(hinstDllLUSS,"stressTranscr");
	findUnits			=(PROCIUUIUU) GetProcAddress(hinstDllLUSS,"findUnits");
	synthesizePhoneme	=(PROCIIUUUS) GetProcAddress(hinstDllLUSS,"synthesizePhoneme");
	id2fv				=(PROCSU) GetProcAddress(hinstDllLUSS,"id2fv");
	synthesizeWholeText	=(PROCICSLEIII) GetProcAddress(hinstDllLUSS,"synthesizeWholeText");

	if ((!initLUSS)
		|| (!normalizeText1)
		|| (!stressTranscr)
		|| (!findUnits)
		|| (!synthesizePhoneme)
		|| (!id2fv)
		|| (!synthesizeWholeText))
	{
    	FreeLibrary (hinstDllLUSS); 
		hinstDllLUSS=NULL;
		return FALSE;
	}
	return TRUE;
}

void unloadLUSS()
{
	if (hinstDllLUSS)
	{
		FreeLibrary (hinstDllLUSS);
		hinstDllLUSS=NULL;
	}
}