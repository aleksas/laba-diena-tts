///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas transcrLUSS.dll
// Failas Transkr.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include "Kircdb.h"
#include "fv2id.h"
#include "../include/LithUSS_Error.h"

#define ILGIS1 500	//analizuojamos tekstines eilute ilgis
#define MAX_RULES 250 //maksimalus transkribavimo taisykliu skaicius

int rkiek;
// Transkribavimo taisykles
struct taisykles
{
	char KKont[15];
	char ES;
	char DKont1[15];
	char DKont2[15];
	unsigned char Kirt;
	unsigned char Minkst;
	unsigned char SkDu;
	unsigned char SkPab;
	char FonV[5];
	char PoslR;
	char PoslT;
} Taisykl[MAX_RULES];
/*
Kirt 1-nek,2-tr,4-des,8-gal,16-kirtis 1 s i des .
Mink 1-mink,2-kiet,3-betkoks.
SkDu 1-skardus,2-duslus,3-betkoks.
SkPb 1-kitur,2-einam,4-1ikair.
*/

int initTranscrLUSS(const char *katVardas)
{
	FILE *df = NULL;
	int j, n;
	char laik[200];

	int hr = NO_ERR;
	strcpy(laik, katVardas);
	strcat(laik, "transcr_rules.txt");

	if ((df = fopen(laik, "r")) == NULL)
	{
		hr = ERROR_TRANSCRLUSS_OPENING_TRANSCR_RULES_FILE;
	}

	if (hr == NO_ERR)
	{
		n = fscanf(df, "%d%[^\n]", &rkiek, laik);

		j = 0;
		do
		{
			n = fscanf(df, "%s %c %s %s %hhu %hhu %hhu %hhu %s %hhd %hhd%[^\n]", Taisykl[j].KKont, &Taisykl[j].ES, Taisykl[j].DKont1, Taisykl[j].DKont2,
					   &Taisykl[j].Kirt, &Taisykl[j].Minkst, &Taisykl[j].SkDu, &Taisykl[j].SkPab,
					   Taisykl[j].FonV, &Taisykl[j].PoslR, &Taisykl[j].PoslT, laik);
			
			if ((n >= 11) && (j < MAX_RULES - 1)) j++;

		} while (!feof(df));

		if (j != rkiek) hr = ERROR_TRANSCRLUSS_READING_TRANSCR_RULES_FILE;
	}

	if (hr == NO_ERR)
	{
		for (j = 0; j < rkiek; j++)
		{
			if (Taisykl[j].KKont[0] == '*')
				Taisykl[j].KKont[0] = 0;
			if (Taisykl[j].DKont1[0] == '*')
				Taisykl[j].DKont1[0] = 0;
			if (Taisykl[j].DKont2[0] == '*')
				Taisykl[j].DKont2[0] = 0;
			if (Taisykl[j].FonV[0] == '*')
				Taisykl[j].FonV[0] = 0;
		}
		initKircLUSS();
	}

	if (df != 0)
		fclose(df);

	return hr;
}
//-------------------------------------------------------------------------------------------
int transkr(char *eil, char *Kirt, char *SkPb, char *Trmp, char *TrEil, int TrEilIlg,
			unsigned short *unitsR, unsigned short *unitsRNextSep, int *unitsLet, int *letPos)
{
	char Mn[ILGIS1];
	char SkDu[ILGIS1];

	int i, j, ilg, KietMin, BalPrieb, SkD, PriebNe;

	TrEil[0] = 0;
	strcat(TrEil, "_+");
	unitsR[0] = 0;
	unitsRNextSep[0] = '+';
	unitsLet[0] = letPos[0];
	int fnr = 1;

	ilg = strlen(SkPb);

	// Kietumo-minkstumo nustatymas
	KietMin = 2;  //1-minksta, 2-kieta
	BalPrieb = 1; //0-priebalse, 1-balse
	for (i = ilg - 1; i > 0; i--)
	{
		if (strchr("AÀEÆËIYÁOUÛØ", eil[i]) && (eil[i] != 0))
			BalPrieb = 1;
		else if ((BalPrieb == 1) || (eil[i] == 'J'))
		{
			KietMin = (strchr("EÆËIYÁ", eil[i + 1]) && (eil[i + 1] != 0) || (eil[i] == 'J')) ? 1 : 2;
			BalPrieb = 0;
		}
		Mn[i] = KietMin;
	}

	// Skardumo-duslumo nustatymas
	SkD = 2;	 //1-skardus, 2-duslus
	PriebNe = 0; //0-trankusis priebalsis, 1-ne
	for (i = ilg - 1; i > 0; i--)
	{
		if (strchr("BDGPTKSÐZÞCÈ", eil[i]))
		{
			if ((PriebNe == 1) && (eil[i] != 0))
			{
				SkD = strchr("BDGZÞ", eil[i]) ? 1 : 2;
				PriebNe = 0;
			}
		}
		else
			PriebNe = 1;
		SkDu[i] = SkD;
	}

	eil[0] = '_';
	eil[ilg] = 0;
	eil[ilg + 1] = '_';
	eil[ilg + 2] = '_';
	eil[ilg + 3] = 0;
	// Taisykliu paieska
	int k;
	i = 1;
	j = 0;
	while (i < ilg)
	{
		if (Taisykl[j].ES != eil[i])
			j += Taisykl[j].PoslT;
		else
		{
			if (eil[i + 1] == 0)
				k = 2;
			else
				k = 1;
			if (((Taisykl[j].KKont[0] != 0) ? strchr(Taisykl[j].KKont, eil[i - 1]) : (void *)1) && ((Taisykl[j].DKont1[0] != 0) ? strchr(Taisykl[j].DKont1, eil[i + k]) : (void *)1) && ((Taisykl[j].DKont2[0] != 0) ? strchr(Taisykl[j].DKont2, eil[i + k + 1]) : (void *)1) && (Taisykl[j].Kirt & Kirt[i]) && (Taisykl[j].Minkst & 3 & Mn[i]) && (Taisykl[j].SkDu & SkDu[i]) && (((Taisykl[j].SkPab & SkPb[i] & 7) == (SkPb[i] & 7)) || ((Taisykl[j].SkPab & SkPb[i] & 7) == Taisykl[j].SkPab) || ((Taisykl[j].SkPab == 8) && (SkPb[i] & 8))) && (((Taisykl[j].Minkst & 4) == 0) || Trmp[i]))
			{
				unitsLet[fnr] = letPos[i - 1];
				i += Taisykl[j].PoslR;
				if (strlen(TrEil) < TrEilIlg - 8) //fv ilgis yra iki 4 raidziu ir \n+\n_
				{
					if (Taisykl[j].PoslR > 1)
					{
						int ii = strlen(TrEil) - 1;
						if (SkPb[i - 2] & 8)
						{
							TrEil[ii] = '+';
							unitsRNextSep[fnr - 1] = '+';
						}
						else if (SkPb[i - 2] & 2)
						{
							if (TrEil[ii] == ' ')
							{
								TrEil[ii] = '-';
								unitsRNextSep[fnr - 1] = '-';
							}
						}
						if (eil[i - 1] == 0)
							i++;
					}
					if (Taisykl[j].FonV[0] != 0)
					{
						strcat(TrEil, Taisykl[j].FonV);
						unitsR[fnr] = fv2id(Taisykl[j].FonV);
						if (SkPb[i - 1] & 8)
						{
							strcat(TrEil, "+");
							unitsRNextSep[fnr] = '+';
						}
						else if ((SkPb[i - 1] & 2) && (Taisykl[j].PoslR > 0))
						{
							strcat(TrEil, "-");
							unitsRNextSep[fnr] = '-';
						}
						else
						{
							strcat(TrEil, " ");
							unitsRNextSep[fnr] = ' ';
						}
						fnr++;
					}
					else
					{
						int ii = strlen(TrEil) - 1;
						if (SkPb[i - 1] & 8)
						{
							TrEil[ii] = '+';
							unitsRNextSep[fnr - 1] = '+';
						}
						else if (SkPb[i - 1] & 2)
						{
							if (TrEil[ii] == ' ')
							{
								TrEil[ii] = '-';
								unitsRNextSep[fnr - 1] = '-';
							}
						}
					}
				}
				else
					return -1;
				if (Taisykl[j].PoslR == 0)
				{
					j += Taisykl[j].PoslT;
				}
				else
				{
					j = 0;
				}
			}
			else
				j++;
		}
		if (j > rkiek - 1)
		{
			j = 0;
			i++;
		}
	}
	TrEil[strlen(TrEil) - 1] = 0; //atmetam paskutine skiemens pabaiga
	strcat(TrEil, "+_\n");
	unitsRNextSep[fnr - 1] = '+';
	unitsR[fnr] = 0;
	unitsRNextSep[fnr] = '+';
	unitsLet[fnr] = letPos[i - 1];

	return fnr + 1;
}
