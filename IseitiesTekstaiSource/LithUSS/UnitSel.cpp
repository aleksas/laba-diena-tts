///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas UnitSel.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "UnitSel.h"

HMODULE dll_handle;
PROCBIC initUnitSel;
PROCBISSSIII selectUnits;


BOOL loadUnitSel(char *biblioteka)
{
	if (!(dll_handle=LoadLibrary(biblioteka))) //"UnitSelection.dll"
		return FALSE;
	initUnitSel = (PROCBIC)GetProcAddress(dll_handle, "initUnitSel");
	selectUnits = (PROCBISSSIII)GetProcAddress(dll_handle, "selectUnits");

	if ((!initUnitSel)	|| (!selectUnits))
	{
    	FreeLibrary (dll_handle); 
		dll_handle=NULL;
		return FALSE;
	}
	return TRUE;
}

void unloadUnitSel()
{
	if (dll_handle)
	{
		FreeLibrary (dll_handle);
		dll_handle=NULL;
	}
}