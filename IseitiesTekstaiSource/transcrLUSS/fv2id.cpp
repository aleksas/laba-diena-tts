///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas transcrLUSS.dll
// Failas fv2id.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "../include/strtokf.h"

#include "fv2id.h"

unsigned short fv2id(char *fpav)
{
for(int i=0; i<FonSk; i++)
	if(strcmp(fpav, FonV[i].fv)==0)
		return FonV[i].id;

return FonV[0].id; //pauze "_"
}

int trText2UnitList(char *TrSakinys, unsigned short *units, unsigned short *unitseparators)
{
	char temp[500], *pos, *newpos; //turetu pakakti 480
	strcpy(temp, TrSakinys);

	int i=0;
	pos=strtokf(temp, "+- ", &newpos);
	while(pos!=NULL)
		{
		units[i] = fv2id(pos);
		pos=strtokf(NULL, "+- ", &newpos);
		if(pos==NULL) unitseparators[i]='+';
		else unitseparators[i]=TrSakinys[pos-temp-1];
		i++;
		}

	return i;
}
