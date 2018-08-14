///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra�tija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas fv2id.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "../transcrLUSS/fv2id.h"

extern "C" {

EXPORT const char* id2fv(unsigned short id)
{
for(int i=0; i<FonSk; i++)
	if(id == FonV[i].id)
		return FonV[i].fv;

return FonV[0].fv; //pauze "_"
}

}