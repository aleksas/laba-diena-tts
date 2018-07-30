///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas ilgiai.cpp
// Autorius dr. Margarita Beniu\xF0\xEB
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "ilgiai.h"
#include "fv2id.h"

#include "../include/LithUSS_Error.h"

#include <stdio.h>
#include <string.h>

#define FonSk 92
#define KoefSk 36
#define BalsiaiStartId 1
#define PriebalsiaiStartId 35

static struct FonIlgiai{char fv[4]; unsigned short id; unsigned short vid; unsigned short min; double koef[KoefSk];} FonIlg[FonSk] ;

// + \xFEod\xFEio riba, - skiemens riba, tarpas fonemos riba,
// sakinio prad\xFEioje/pabaigoje yra pauz\xEB, t.y. pabraukimo simbolis.

bool isBalsis(unsigned short id) {
	if( BalsiaiStartId <= id && id < PriebalsiaiStartId )
		return true;
	else
		return false;
}

bool isPriebalsis(unsigned short id) {
	if( id >= PriebalsiaiStartId )
		return true;
	else
		return false;
}

unsigned short id2ilg(unsigned short *l, unsigned short len, unsigned short i)
{
	if(l[i] == FonIlg[0].id) {
		return FonIlg[0].vid; // pauze "_"
	}
	else if(isBalsis(l[i]) || isPriebalsis(l[i]))
	{
		unsigned short startId, endId;
		unsigned short f[KoefSk]; // visi 36 faktoriai 1.1 1.2 1.3 2.1 2.2 ... 12.1 12.2 13.1 13.2

		for(int j = 0; j < KoefSk; j++) {
			f[j] = 0;						// 0 - netaikom faktoriaus, 1 taikom faktoriu
		}

		if(isBalsis(l[i])) { // jei BALSIS
			startId = BalsiaiStartId;
			endId = PriebalsiaiStartId;

			//--1 faktoriu grupe
			if (i+1 < len) {
				if (l[i+1] == 0) {  // l[i+1] == 0 sakinio pabaiga
					f[0] = 1;  // 1.1 Balsis sakinio gale
				} else {
					int ii = i+1;
					while (ii < len && isPriebalsis(l[ii]))
						ii++;

					if (ii < len && l[ii] == 0)  // l[ii] == 0 - sakinio pabaiga
						f[1] = 1;  // 1.2 Balsis sakinio gale pries priebalsius
					else
						f[2] = 1;  // 1.3 Balsis ne sakinio gale
				}
			}  //--1

		} else if (isPriebalsis(l[i])) { // jei PRIEBALSIS
			startId = PriebalsiaiStartId;
			endId = FonSk;

			//--2 faktoriu grupe
			if(i+1 < len) {
				if (l[i+1] == 0)
					f[3] = 1; //2.1 Priebalsis sakinio gale
				else
					f[4] = 1; //2.2 Priebalsis ne sakinio gale
			} //--2

			//--4 faktoriu grupe
			if (i == 0)
				f[8] = 1;  //4.1 Priebalsis sakinio pradzioje
			else if (i > 0)
			{
				if ( l[i-1] == 0 )
					f[8] = 1;  //4.1 Priebalsis sakinio pradzioje
				else
					f[9] = 1;  //4.2 Priebalsis ne sakinio pradzioje
			} //--4

			//--12 faktoriu grupe
			if ( (i-1 >= 0 && isPriebalsis(l[i-1])) || (i+1 < len && isPriebalsis(l[i+1])) )
				f[32] = 1; //12.1 Priebalsis priebalsiu grupeje
			else
				f[33] = 1; //12.2 Priebalsis ne priebalsiu grupeje
			//--12
		}

		// Pradedam skaiciuoti bendra daugikli f1*f2*... ir atitinkama fonemos ilgi:
		double fFinal = 1;  // visu veikianciu faktoriu sandauga

		for(int k = startId; k < endId; k++) {

			if(l[i] == FonIlg[k].id) { // vietoj sito for()+if() galima padaryti binary search, jei reikia

				for(int j = 0; j < KoefSk; j++) {
					if(f[j] == 1)
						fFinal *= FonIlg[k].koef[j];
				}

				return (unsigned short)(FonIlg[k].min + (FonIlg[k].vid - FonIlg[k].min) * fFinal);
			}
		}
	} //else {
		// cia reikia grazinti kazkokia reiksme, pvz. 0, jei pasitaike fonema, kurios nera duomenu bazeje FonIlg[FonSk], kad programa neuzluztu
		//return 0;
	//}
	return 0;
}

void ilgiai(unsigned short *units, unsigned short *unitseparators, int unitscount, unsigned short *unitslen)
{
	for(int i=0; i < unitscount; i++)
	{
		unitslen[i] = id2ilg(units, unitscount, i);
	}
}

int initFaktoriai(const char *dirVardas)
{
	char duomenuByla[200];
	strcpy(duomenuByla, dirVardas);
	strcat(duomenuByla, "faktoriai.txt");

	FILE * pFile = fopen (duomenuByla, "r");
	if (pFile == NULL) return ERROR_LITHUSS_OPENING_FACTORS_FILE; //nepavyko atidaryti failo faktoriai.txt

	int ll, j = 0;
	do
		{
		ll = fscanf(pFile, "%s %hd %hd %hd", FonIlg[j].fv, &FonIlg[j].id, &FonIlg[j].vid, &FonIlg[j].min);
		for(int k = 0; k < KoefSk; k++)
			ll += fscanf(pFile, "%lf", &FonIlg[j].koef[k]);
		fscanf(pFile, "\n");
		j++;
		}
	while((j < FonSk) && (ll == KoefSk+4));

	if((j < FonSk) || (ll < KoefSk+4)) return ERROR_LITHUSS_READING_FACTORS_FILE; //klaida nuskaitant duomenis is failo faktoriai.txt. Duomenys neatitinka reikalaujamo formato

	return NO_ERR;
}

