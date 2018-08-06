///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra�tija.lt)
// Sintezatoriaus komponentas LithUSStest.exe
// Failas LithUSS_Error.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LITHUSS_H__
#define __LITHUSS_H__

#include "common.h"

extern "C" {

struct event {short Id; short phonviz; int charOffset; long signOffset;}; 

EXPORT int initLUSS(const char *katDll, const char *katVoice);

EXPORT int normalizeText1(char *text, char *normtextbuf, int bufsize, int *letPos);
EXPORT int stressTranscr(char *sakinys, char *TrSakinys, int bufsize, unsigned short *unitsR, unsigned short *unitsRNextSep,
				  int *unitsLet, int *letPos);
EXPORT int findUnits(unsigned short *unitsRows, unsigned short *unitsRowsNextSeparators,
			  int unitsRowsLength, int *retUnits, unsigned short *unitsDurr);
EXPORT int synthesizePhoneme(int greicioKoef, int tonas, unsigned short unitsDurr, unsigned int retUnit, short *phoneme, unsigned int naujo_signalo_masyvo_ilgis);
EXPORT const char* id2fv(unsigned short id);
EXPORT int synthesizeWholeText(char *tekstas, short *signbuf, unsigned int *signbufsize, event *evarr, int *evarrsize, int greitis, int tonas);

}

#endif