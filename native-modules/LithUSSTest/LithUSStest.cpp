///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas LithUSStest.exe
// Failas LithUSStest.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "../RateChange/RateChange.h"
#include "../include/LithUSS_Error.h"
#include "../LithUSS/LithUSS.h"

const char* getLUSSErrorMessages(int);

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

CallbackReturnType TextToSound(void * lpParam)
{
	PSYNTHDATA pData = (PSYNTHDATA)lpParam;
	if (pData == NULL) return (CallbackReturnType) 7;
	if (pData->largebf == NULL) return (CallbackReturnType) 8;
	if (pData->evsz == NULL) return (CallbackReturnType) 9;
	int hr = -synthesizeWholeText(pData->text, //pakeiciam neigiamus i teigiamus
		pData->largebf,
		(unsigned int*)pData->largebfsize, //TODO: check consistency
		pData->evar,
		pData->evsz,
		pData->greitis,
		pData->tonas);
	return (CallbackReturnType) hr;
}

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

	char *katvardai[4] = { "." PS "Regina" PS, "." PS "Edvardas" PS, "." PS "Aiste" PS, "." PS "Vladas" PS };
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
/*
		for (z = 0; z < fonemuSkaicius; z++) {
			ilgis += fonemuIlgiai[z];
			sprintf(transcrBuf, "%s\r\n%s", transcrBuf, ppFonemos[z]);

			if (ppFonemos[z][0] == '_' && ppFonemos[z][1] == '\0' && ilgis > fonemuIlgiai[z]) {

				sprintf(transcrFname, "." PS "%s_" PS "%d.txt", vardai[ii], w);
				sprintf(wavFname, "." PS "%s" PS "%d.wav", vardai[ii], w);

				FILE *f = fopen(transcrFname, "w");
				fprintf(f, transcrBuf);
				fclose(f);

				raw2wav(ilgis, wavFname, (short*)(pData + pradzia));

				w++;
				pradzia += ilgis;
				ilgis = 0;
				sprintf(transcrBuf, "");
			}
		}*/


		//sintezavimas pastraipomis
		//struct event {short Id; short phonviz; int charOffset; long signOffset;}; 

		int k;
		int greitis = 100;
		int tonas = 100;

		if (hr == 0)
		{
			for (k = 0; k < pastrSk; k++)
			{
				pDataArray[k] = (PSYNTHDATA) malloc(sizeof(SYNTHDATA)); // HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SYNTHDATA));
				if (pDataArray[k] != NULL)
				{
					pDataArray[k]->text = eil[k];

					int eillen = strlen(eil[k]) * 2;
					if (eillen < PAGE_SIZE) eillen = PAGE_SIZE;

					if (greitis >= 100)
					{
						largebufsize[k] = eillen * 1250 * greitis / 100;
					}
					else //greitis < 100)
					{
						largebufsize[k] = eillen * 1250 * greitis / 150;
					}
					pDataArray[k]->largebfsize = &largebufsize[k];
					pDataArray[k]->largebf = (short*)calloc(largebufsize[k], sizeof(short)); //butina uznulinti, todel calloc

					evsize[k] = eillen;
					pDataArray[k]->evsz = &evsize[k];
					pDataArray[k]->evar = (event*)malloc(evsize[k] * sizeof(event));

					pDataArray[k]->greitis = greitis;
					pDataArray[k]->tonas = tonas;
				}
			}
		}

		ThreadHandle  hThreadArray[MAX_PASTR_SK];
		unsigned long hrM[MAX_PASTR_SK];
		if (hr == 0)
		{
			for (k = 0; k < pastrSk; k++)
			{
				CreateNewThread(&hThreadArray[k], TextToSound, pDataArray[k]);
				if (hThreadArray[k] == NULL) exit(3);
			}


			for (k = 0; k < pastrSk; k++)
			{
				int thcod;
				CallbackReturnType ret;
				JoinThread(hThreadArray[k], &ret, &thcod);
				hrM[k] = (unsigned long) ret;

				if (thcod != 0) hrM[k] = 2; //kazkoks klaidos kodas
			}
		}

		//rezultatu isvedimas
		char temp[100], fvwav[100], fvev[100];
		if (hr == 0)
		{
			int res = 0;
			FILE *fr;
			FILE *fe;
			char *evvardai[4] = { "Sak", "Zod", "Skm", "Fon" };

			for (k = 0; k < pastrSk; k++)
			{
				printf("Gija %d kodas %d\n", k, hrM[k]);
				if (hrM[k] == 0)
				{
					sprintf(temp, "%d_%d", ii, k);

					strcpy(fvwav, "pastr_");
					strcat(fvwav, temp);
					strcat(fvwav, ".wav");

					res = raw2wav(*(pDataArray[k]->largebfsize), fvwav, pDataArray[k]->largebf);
					if (res != 0)
					{
						spausdinti_loga("Nepavyko atidaryti wav failo");
						continue;
					}

					strcpy(fvev, "pastr_");
					strcat(fvev, temp);
					strcat(fvev, ".txt");

					if ((fe = fopen(fvev, "w")) == 0)
					{
						spausdinti_loga("Nepavyko atidaryti txt failo");
						continue;
					}

					for (int i = 0; i < *(pDataArray[k]->evsz); i++)
					{
						fprintf(fe, "%d\t%s\t%d\t%s\t%d\t%c\t%d\n", i,
							evvardai[pDataArray[k]->evar[i].Id - 1],
							pDataArray[k]->evar[i].phonviz,
							id2fv(pDataArray[k]->evar[i].phonviz),
							pDataArray[k]->evar[i].charOffset,
							eil[k][pDataArray[k]->evar[i].charOffset],
							pDataArray[k]->evar[i].signOffset);
					}
					fclose(fe);
				}
			}

			//Atlaisvinami resursai
			for (k = 0; k < pastrSk; k++)
			{
				CloseThread(hThreadArray[k]);
				if (pDataArray[k] != NULL)
				{
					if (pDataArray[k]->largebf != NULL) free(pDataArray[k]->largebf);
					if (pDataArray[k]->evar != NULL) free(pDataArray[k]->evar);
					free(pDataArray[k]);
					pDataArray[k] = NULL;
				}
			}
		}
		//fragmento pabaiga
	}

	unloadLUSS();

	return hr;
}

