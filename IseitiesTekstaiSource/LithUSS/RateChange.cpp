///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas RateChange.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RateChange.h"
#include <stdio.h>

HINSTANCE	hinstDllRC;
PROCICICILS change_DB_rate;
PROCICCILS initRateChange;
PROCIIIISI change_phoneme_rate;

BOOL loadRateChange(char *biblioteka)
{
	if (!(hinstDllRC=LoadLibrary(biblioteka))) //"RateChange.dll"
		return FALSE;

	change_DB_rate = (PROCICICILS) GetProcAddress (hinstDllRC, "change_DB_rate");
	initRateChange = (PROCICCILS) GetProcAddress (hinstDllRC, "initRateChange");
	change_phoneme_rate = (PROCIIIISI) GetProcAddress (hinstDllRC, "change_phoneme_rate");

	if ((!change_DB_rate) || (!initRateChange) || (!change_phoneme_rate))
	{
    	FreeLibrary (hinstDllRC); 
		hinstDllRC=NULL;
		return FALSE;
	}
	return TRUE;
}

void unloadRateChange()
{
	if (hinstDllRC)
	{
		FreeLibrary (hinstDllRC);
		hinstDllRC=NULL;
	}
}