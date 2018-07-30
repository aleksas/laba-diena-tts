#ifndef RATECHANGE_H_INCLUDED
#define RATECHANGE_H_INCLUDED

#include "../include/common.h"

EXPORT int change_DB_rate(char *katVardas, int greitis, int tono_aukscio_pokytis, char dbfv1[][4], int *dbilg1, long *dbadr1, short ** wholeinputt1);
EXPORT int initRateChange(const char *katVardas, char dbfv1[][4], int *dbilg1, long *dbadr1, short ** wholeinput1);
EXPORT int change_phoneme_rate(int greitis, int tono_aukscio_pokytis, unsigned int fonemos_nr, short * naujas_signalas, unsigned int naujo_signalo_masyvo_ilgis);

EXPORT void loadRateChange();
EXPORT void unloadRateChange();
EXPORT void getData(int * pIlgis, short ** ppData, int * pFonemuSkaicius, int ** ppFonemuIlgiai, char *** pppFonemos);

#endif // !RATECHANGE_H_INCLUDED
