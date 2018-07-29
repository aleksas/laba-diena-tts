
#ifndef LITHUSS_H_INCLUDED
#define LITHUSS_H_INCLUDED

#include "../include/common.h"

struct event {short Id; short phonviz; int charOffset; long signOffset;};

EXPORT int initLUSS(char *katDll, char *katVoice);
EXPORT int normalizeText1(char *text, char *normtextbuf, int bufsize, int * letPos);
EXPORT int stressTranscr(char *sakinys, char *TrSakinys, int bufsize, unsigned short* unitsR, unsigned short* unitsRNextSep, int* unitsLet, int* letPos);
EXPORT int findUnits(unsigned short *unitsRows, unsigned short *unitsRowsNextSeparators, int unitsRowsLength, unsigned int *retUnits, unsigned short *unitsDurr);
EXPORT int synthesizePhoneme(int greicioKoef, int tonas, unsigned short unitsDurr, unsigned int retUnit, short *phoneme, unsigned int naujo_signalo_masyvo_ilgis);
EXPORT const char* id2fv(unsigned short id);
EXPORT int synthesizeWholeText(char *tekstas, short *signbuf, unsigned int *signbufsize, event *evarr, int *evarrsize, int greitis, int tonas);
EXPORT int raw2wav(int, char * filename, short * pBuffer);

EXPORT BOOL loadLUSS();
EXPORT void unloadLUSS();

EXPORT const char* getLUSSErrorMessages(int err);

#endif //!LITHUSS_H_INCLUDED