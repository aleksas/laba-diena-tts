#ifndef TRANSCRLUSS_H_INCLUDED
#define TRANSCRLUSS_H_INCLUDED

#include "../include/common.h"

EXPORT int KircTranskr(char* eil, char* TrZodis, int TrEilIlg, unsigned short* unitsR, unsigned short* unitsRNextSep, int* unitsLet, int* letPos, int rules2use);
EXPORT int initTranscrLUSS(const char *katVardas);

#endif //!TRANSCRLUSS_H_INCLUDED