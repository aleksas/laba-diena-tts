///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra�tija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas transcrLUSS.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TRANSCRLUSS_H__
#define __TRANSCRLUSS_H__

extern "C" {

int KircTranskr(char* eil, char* TrZodis, int TrEilIlg, unsigned short* unitsR,  unsigned short* unitsRNextSep, 
				int* unitsLet, int* letPos, int rules2use);
int initTranscrLUSS(const char *katVardas);

}

#endif
