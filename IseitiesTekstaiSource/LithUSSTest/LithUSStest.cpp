///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSStest.exe
// Failas LithUSStest.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LithUSS.h"
#include "raw2wav.h"
#include <malloc.h>
#include <malloc.h>
#include "LithUSS_Error.h"

char* getLUSSErrorMessages(int);

#define MAX_PASTR_SK 10
#define MAX_PASTR_ILG 1000
#define PAGE_SIZE 1600

void spausdinti_loga(char* pranesimas)
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

DWORD WINAPI TextToSound(LPVOID lpParam)
{
PSYNTHDATA pData = (PSYNTHDATA) lpParam;
if(pData == NULL) return 7;
if(pData->largebf == NULL) return 8;
if(pData->evsz == NULL) return 9;
int hr = -synthesizeWholeText(pData->text, //pakeiciam neigiamus i teigiamus
							 pData->largebf,
							 pData->largebfsize,
							 pData->evar,
							 pData->evsz,
							 pData->greitis,
							 pData->tonas);
return (DWORD)hr;
}

int main(int argc, char* argv[])
{
//tekstiniu duomenu nuskaitymas pastraipomis
FILE *df;
char eil[MAX_PASTR_SK][MAX_PASTR_ILG];
int pastrSk;

int hr=0;
	if((df = fopen( "duom.txt", "rb" )) == NULL)
		{
		spausdinti_loga("Nepavyko atidaryti duom.txt failo");
		hr = -1;
		}

if(hr == 0)
	{
	int i=0, j=0, ch;
	do
		{
		ch = fgetc(df);
		if((ch=='\n') || (ch=='\r') || (ch==EOF))
			{
			eil[i][j]=0;
			if(j>0) {if(i<MAX_PASTR_SK-1) i++;}
			j=0;
			}
		else
			{
			eil[i][j]=ch;
			if(j<MAX_PASTR_ILG-1) j++;
			}
		}
	while(ch != EOF);
	fclose(df);
	pastrSk = i;
	}

//dll pakrovimas ir inicializavimas
if(hr == 0)
	{
	if(loadLUSS("LithUSS.dll")==FALSE)
		{
		spausdinti_loga("Klaida pakraunant LithUSS.dll");
		hr = -3;
		}
	}

int evsize[MAX_PASTR_SK];
long largebufsize[MAX_PASTR_SK];
PSYNTHDATA pDataArray[MAX_PASTR_SK];

char *katvardai[4] = {".\\Regina\\", ".\\Edvardas\\", ".\\Aiste\\", ".\\Vladas\\"};

for(int ii=0; ii<4; ii++)
{
// inicializuojantis, rasantis, skaitantis fragmentas
if((hr == 0) || (hr == -4))
	{
	int res = initLUSS("", katvardai[ii]);
	if(res != NO_ERR)
		{
		spausdinti_loga(getLUSSErrorMessages(res));
		hr = -4;
		}
	}

//sintezavimas pastraipomis
//struct event {short Id; short phonviz; int charOffset; long signOffset;}; 

int k;
int greitis = 100;
int tonas = 100;

if(hr == 0)
	{
	for(k=0; k<pastrSk; k++)
		{
		pDataArray[k] = (PSYNTHDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SYNTHDATA));
		if(pDataArray[k] != NULL)
			{
			pDataArray[k]->text = eil[k];

			int eillen = strlen(eil[k]) * 2;
			if(eillen < PAGE_SIZE) eillen = PAGE_SIZE;

			if(greitis >= 100)
				{largebufsize[k] = eillen * 1250 * greitis / 100;}
			else //greitis < 100)
				{largebufsize[k] = eillen * 1250 * greitis / 150;}
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

DWORD   dwThreadIdArray[MAX_PASTR_SK];
HANDLE  hThreadArray[MAX_PASTR_SK]; 
unsigned long hrM[MAX_PASTR_SK];
if(hr == 0)
	{
	for(k=0; k<pastrSk; k++)
		{
		hThreadArray[k] = CreateThread(NULL, 0, TextToSound, pDataArray[k], 0, &dwThreadIdArray[k]);
		if (hThreadArray[k] == NULL) ExitProcess(3);
		}

	DWORD thret = WaitForMultipleObjects(pastrSk, hThreadArray, TRUE, INFINITE);
	DWORD err = GetLastError(); 
	for(k=0; k<pastrSk; k++)
		if(thret == WAIT_OBJECT_0)
			{
			BOOL thcod = GetExitCodeThread(hThreadArray[k], &hrM[k]);
			if(thcod == FALSE) hrM[k]=2; //kazkoks klaidos kodas
			}
		else 
			{
			hrM[k]=3; //kazkoks klaidos kodas
			}
	}

//rezultatu isvedimas
char temp[100], fvwav[100], fvev[100];
if(hr == 0)
	{
	int n;
	FILE *fr;
	FILE *fe;
	char *evvardai[4] = {"Sak", "Zod", "Skm", "Fon"};

	for(k=0; k<pastrSk; k++)
		{
printf("Gija %d kodas %d\n", k, hrM[k]);
		if(hrM[k] == 0)
			{
			sprintf(temp, "%d_%d", ii, k);

			strcpy(fvwav, "pastr_");
			strcat(fvwav, temp);
			strcat(fvwav, ".wav");

			if((fr = fopen(fvwav, "wb"))==0)
				{
				spausdinti_loga("Nepavyko atidaryti wav failo");
				continue; 
				}

			n=raw2wav(*(pDataArray[k]->largebfsize), fr, pDataArray[k]->largebf);
			fclose(fr);

			strcpy(fvev, "pastr_");
			strcat(fvev, temp);
			strcat(fvev, ".txt");

			if((fe = fopen(fvev, "w"))==0)
				{
				spausdinti_loga("Nepavyko atidaryti txt failo");
				continue;
				}

			for(int i=0; i<*(pDataArray[k]->evsz); i++)
				{
				fprintf(fe, "%d\t%s\t%d\t%s\t%d\t%c\t%d\n", i, 
					evvardai[pDataArray[k]->evar[i].Id-1], 
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
	for(k=0; k<pastrSk; k++)
		{
		CloseHandle(hThreadArray[k]);
		if(pDataArray[k] != NULL)
			{ 
			if(pDataArray[k]->largebf != NULL) free(pDataArray[k]->largebf);
			if(pDataArray[k]->evar != NULL) free(pDataArray[k]->evar);
			HeapFree(GetProcessHeap(), 0, pDataArray[k]);
			pDataArray[k] = NULL;
			}
		}
	}
//fragmento pabaiga
}

	unloadLUSS();

	return hr;
}

