///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas LithUSStest.exe
// Failas LithUSStest.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../RateChange/RateChange.h"
#include "../include/LithUSS_Error.h"
#include "../LithUSS/LithUSS.h"

#include <malloc.h>
#include <stdio.h>

#define MAX_PASTR_SK 10
#define MAX_PASTR_ILG 1000
#define PAGE_SIZE 1600

void spausdinti_loga(const char* pranesimas)
{
	FILE *stream;
	stream = fopen("LUSS_log.txt", "a+");
	fprintf(stream, "%s\n", pranesimas);
	printf("%s\n", pranesimas);
	fclose(stream);
}

typedef struct SynthData {
	char *text;
	short *largebf;
	long *largebfsize;
	event *evar;
	int *evsz;
	int greitis;
	int tonas;
} SYNTHDATA, *PSYNTHDATA;

int main(int argc, char* argv[])
{
	//tekstiniu duomenu nuskaitymas pastraipomis
	FILE *df;
	char eil[MAX_PASTR_SK][MAX_PASTR_ILG];
	int pastrSk;

	int hr = 0;
	if ((df = fopen("duom.txt", "rb")) == NULL)
	{
		spausdinti_loga("Nepavyko atidaryti duom.txt failo");
		hr = -1;
	}

	if (hr == 0)
	{
		int i = 0, j = 0, ch;
		do
		{
			ch = fgetc(df);
			if ((ch == '\n') || (ch == '\r') || (ch == EOF))
			{
				eil[i][j] = 0;
				if (j > 0) { if (i < MAX_PASTR_SK - 1) i++; }
				j = 0;
			}
			else
			{
				eil[i][j] = ch;
				if (j < MAX_PASTR_ILG - 1) j++;
			}
		} while (ch != EOF);
		fclose(df);
		pastrSk = i;
	}

	//dll pakrovimas ir inicializavimas
	if (hr == 0)
	{
		if (loadLUSS() == FALSE)
		{
			spausdinti_loga("Klaida pakraunant LithUSS.dll");
			hr = -3;
		}
	}

	int evsize[MAX_PASTR_SK];
	long largebufsize[MAX_PASTR_SK];
	PSYNTHDATA pDataArray[MAX_PASTR_SK];

	char *katvardai[4] = { "./Regina/", "./Edvardas/", "./Aiste/", "./Vladas/" };
	char *vardai[4] = { "Regina", "Edvardas", "Aiste", "Vladas" };
	int ilgis, fonemuSkaicius;
	int * fonemuIlgiai = NULL;
	short * pData = NULL;
	char ** ppFonemos = NULL;
	char wavFname[256] = { 0 };
	char transcrFname[256] = { 0 };
	char transcrBuf[256] = { 0 };
	int bendrasIlgis = 0;

	for (int ii = 0; ii < 4; ii++)
	{
		int z, w = 0, pradzia = 0, ilgis = 0;

		// inicializuojantis, rasantis, skaitantis fragmentas
		if ((hr == 0) || (hr == -4))
		{
			int res = initLUSS("", katvardai[ii]);
			if (res != NO_ERR)
			{
				spausdinti_loga(getLUSSErrorMessages(res));
				hr = -4;
			}
		}

		printf(katvardai[ii]);
		getData(&bendrasIlgis, &pData, &fonemuSkaicius, &fonemuIlgiai, &ppFonemos);

		sprintf(wavFname, "./%s.wav", vardai[ii], w);
		raw2wav(bendrasIlgis, wavFname, (short*)pData);
	}

	unloadLUSS();

	return hr;
}
