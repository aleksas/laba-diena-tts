///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra�tija.lt)
// Sintezatoriaus komponentas LithUSS.dll
// Failas RateChange.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __RATECHANGE_H__
#define __RATECHANGE_H__

extern "C" {

int change_DB_rate (char *katVardas, int greitis, int tono_aukscio_pokytis, char dbfv1[][4], 
					int *dbilg1, long *dbadr1, short ** wholeinputt1);
int initRateChange (const char *katVardas, char dbfv1[][4], int *dbilg1, long *dbadr1, short ** wholeinput1);
int change_phoneme_rate (int greitis, int tono_aukscio_pokytis, unsigned int fonemos_nr, 
						 short * naujas_signalas, unsigned int naujo_signalo_masyvo_ilgis);

void loadRateChange();
void unloadRateChange();

}

#endif