///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas VeiksmaiSuFailais.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// reikia, norint gauti kataloge esanèiø failø sàraðà
#include <windows.h>
#include "stdafx.h"

/*********************************************************
 * Grazina failo fp dydi arba -1, jei ivyksta klaida.
 * Jei klaidu neivyko, einamoji failo pozicija lieka nepakeista.
 * Korektiskai veikia tik failams, kuriu dydis iki LONG_MAX = 2147483647 baitu.
 ********************************************************/
long failo_dydis(FILE *fp)
{
    // isimename einamaja failo pozicija
	long buvusi_pozicija = ftell(fp); 
	if (buvusi_pozicija == -1L) return -1L;

    // nuvaziuojame i failo pabaiga
	if (fseek(fp, 0L, SEEK_END) != 0) 
	{
		//jei klaida: bandome grizti i pradine padeti
		fseek(fp, buvusi_pozicija, SEEK_SET); 
		return -1L; 
	}

    // isimename paskutine failo pozicija (failo dydi)
	int dydis = ftell(fp); 

    //griztame i pradine padeti
	if (fseek(fp, buvusi_pozicija, SEEK_SET) != 0) return -1L; 

    return dydis;
}

/*********************************************************
 * Nuskaito duomenis is signalo failo.
 * Grazina 0, jei nebuvo klaidos, ir < 0, jei buvo klaida.
 ********************************************************/
int nuskaityti_signala()
{
	FILE *signalo_failas;
	
	// atidarome binarini signalo faila skaitymui
	if((signalo_failas=fopen(signalo_failo_pavadinimas, "rb"))==NULL)
	{
		return ERROR_RATECHANGE_OPENING_DB_FILE;
	}

	// nustatome jo dydi
	long signalo_failo_dydis = failo_dydis (signalo_failas);
	if (signalo_failo_dydis == -1L)
	{
		fclose(signalo_failas);
		return ERROR_RATECHANGE_DETECTING_DB_FILE_SIZE;
	}
	if (signalo_failo_dydis%2 != 0L)
	{
		fclose(signalo_failas);
		return ERROR_RATECHANGE_UNEVEN_DB_FILE_SIZE;
	}
	
	signalo_ilgis = (size_t) (signalo_failo_dydis/2);
	
	// iskiriame atminties signalo masyvui
	signalas = (short *) malloc ((signalo_ilgis+1) * sizeof(short));
	if(signalas == NULL)
	{
		fclose(signalo_failas);
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_DB;
	}

	// nuskaitome signalo masyva is failo
	size_t nuskaityta=fread(signalas, sizeof(short), signalo_ilgis+1, signalo_failas);
	if (ferror (signalo_failas))
	{
		fclose(signalo_failas);
		return ERROR_RATECHANGE_READING_DB_FILE;
	}
	if (! feof (signalo_failas) || nuskaityta != signalo_ilgis)
	{
		fclose(signalo_failas);
		return ERROR_RATECHANGE_UNFINISHED_READING_DB_FILE;
	}

	// uzdarome signalo faila
	fclose(signalo_failas);

	return 0;
}

/*********************************************************
 * Nuskaito duomenis ið nurodyto tekstinio failo ir suraðo á parametrà turinys. 
 * Á parametrà turinio_ilgis áraðo suraðytø duomenø kieká 
 * (reikðminiø duomenø, t.y. neskaièiuojant '\0').
 * Grazina 0, jei nebuvo klaidos, ir < 0, jei buvo klaida.
 ********************************************************/
int nuskaityti_faila (char * failo_pavadinimas, char ** turinys, size_t * turinio_ilgis)
{
	FILE *failas;
	
	// atidarome tekstiná faila skaitymui
	if((failas=fopen(failo_pavadinimas, "r"))==NULL)
	{
		return -1;
	}

	// nustatome failo dydi
	long failo_dydis_long = failo_dydis (failas);
	if (failo_dydis_long == -1L)
	{
		fclose(failas);
		return -2;
	}

	size_t failo_dydis = (size_t) failo_dydis_long;

	// iskiriame atminties failo turiniui
	char * failo_turinys = (char *) malloc ((failo_dydis+1) * sizeof(char));
	if(failo_turinys == NULL) 
		return -5;

	// nuskaitome failà
	size_t nuskaityta_zenklu = fread (failo_turinys, sizeof(char), failo_dydis+1, failas);
	if (ferror (failas))
	{
		free(failo_turinys);
		fclose(failas);
		return -3;
	}
	if (! feof (failas))
	{
		free(failo_turinys);
		fclose(failas);
		return -4;
	}

	// uzdarome faila
	fclose(failas);

	// paþymime eilutës pabaigà
	failo_turinys[nuskaityta_zenklu] = '\0';

	*turinys = failo_turinys;
	*turinio_ilgis = nuskaityta_zenklu;

	return 0;
}

/*********************************************************
 * Nuskaito duomenis is anotacijø failo.
 * Grazina 0, jei nebuvo klaidos, ir < 0, jei buvo klaida.
 ********************************************************/
int nuskaityti_anotacijas (char * fonemu_failo_pavadinimas, char *** fonemos1, int ** fonemu_ilgiai1, size_t * fonemu_kiekis1)
{
	// nuskaitome failo turiná
	char * fonemu_failo_turinys = NULL;
	size_t nuskaityta_zenklu = 0;
	int nepavyko = nuskaityti_faila (fonemu_failo_pavadinimas, &fonemu_failo_turinys, &nuskaityta_zenklu);
	switch (nepavyko)
		{
		case -1 : return ERROR_RATECHANGE_OPENING_DB_FON_WEIGHTS_FILE;
		case -2 : return ERROR_RATECHANGE_DETECTING_DB_FON_WEIGHTS_FILE_SIZE;
		case -3 : return ERROR_RATECHANGE_READING_DB_FON_WEIGHTS_FILE;
		case -4 : return ERROR_RATECHANGE_UNFINISHED_READING_DB_FON_WEIGHTS_FILE;
		case -5 : return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST;
		default : ;
		}
	
	// susiskaièiuojame, kiek fonemu failo duomenyse yra eiluèiø
	int eiluciu_skaicius = 0;
	for (size_t i=0; i<nuskaityta_zenklu; i++) {
		if (fonemu_failo_turinys[i] == '\n')
			eiluciu_skaicius++;
	}

	char ** fonemos = NULL;
	int * fonemu_ilgiai = NULL;
	size_t fonemu_kiekis = 0;

	// iskiriame atminties fonemu ir ju ilgiu masyvams
	// (dël visa ko pridedam vienetà, nes paskutinë eilutë galëjo neturëti '\n')
	fonemos = (char **) malloc ((eiluciu_skaicius+1) * sizeof(char *));
	if(fonemos == NULL) 
	{
		free (fonemu_failo_turinys);
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST;
	}
	fonemu_ilgiai = (int *) malloc ((eiluciu_skaicius+1) * sizeof(int));
	if(fonemu_ilgiai == NULL)
	{
		free (fonemu_failo_turinys);
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST;
	}
	
	// analizuojame fonemu failo duomenis
	// duomenys eina poromis: fonemos pavadinimas ir ilgis
	char * eilute;
	char * skirtukai = "\t \n";
	int i = 0;
	// nuskaitome fonemos pavadinima
	eilute = strtok (fonemu_failo_turinys, skirtukai); // fonemos pavadinimas
	// kol yra fonemu
	while (eilute != NULL)
	{
		// isimename fonemos pavadinima
		fonemos[i] = (char *) malloc ((strlen(eilute)+1) * sizeof(char));// strlen nepriskaièiuoja '\0', todël +1
		if(fonemos[i] == NULL)
			{
			free (fonemu_failo_turinys);
			return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST;
			}
		strcpy (fonemos[i], eilute);

		// nuskaitome fonemos ilgi
		eilute = strtok (NULL, skirtukai); // fonemos ilgis
		if (eilute == NULL) {
			free (fonemu_failo_turinys);
			return ERROR_RATECHANGE_UNSPECIFIED_PHONEME_LENGTH;
		}
		
		// konvertuojame fonemos ilgá á int
		int n = sscanf (eilute, "%d", &fonemu_ilgiai[i]);
		if (n != 1) {
			free (fonemu_failo_turinys);
			return ERROR_RATECHANGE_NONNUMERICAL_PHONEME_LENGTH;
		}

		// nuskaitome fonemos svorá. Jo mums nereikia, todël nieko su juo nedarome
		eilute = strtok (NULL, skirtukai); // fonemos svoris

		// nuskaitome nauja fonemos pavadinima
		eilute = strtok (NULL, skirtukai); // fonemos pavadinimas

		// padidiname skaitliukà
		i++;
	}

	// isimename nuskaitytu fonemu kieki
	fonemu_kiekis = i;

	free (fonemu_failo_turinys);

	*fonemos1 = fonemos;
	*fonemu_ilgiai1 = fonemu_ilgiai;
	*fonemu_kiekis1 = fonemu_kiekis;

	return 0;
}

/*********************************************************
 * Nuskaito duomenis is pikø failo.
 * Grazina 0, jei nebuvo klaidos, ir < 0, jei buvo klaida.
 ********************************************************/
int nuskaityti_pikus ()
{
	// nuskaitome failo turiná
	char * piku_failo_turinys = NULL;
	size_t nuskaityta_zenklu = 0;
	int nepavyko = nuskaityti_faila (piku_failo_pavadinimas, &piku_failo_turinys, &nuskaityta_zenklu);
	switch (nepavyko)
		{
		case -1 : return ERROR_RATECHANGE_OPENING_DB_PIK_FILE;
		case -2 : return ERROR_RATECHANGE_DETECTING_DB_PIK_FILE_SIZE;
		case -3 : return ERROR_RATECHANGE_READING_DB_PIK_FILE;
		case -4 : return ERROR_RATECHANGE_UNFINISHED_READING_DB_PIK_FILE;
		case -5 : return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_LIST;
		default : ;
		}
	
	// susiskaièiuojame, kiek piku failo duomenyse yra eiluèiø
	int eiluciu_skaicius = 0;
	for (size_t i=0; i<nuskaityta_zenklu; i++) {
		if (piku_failo_turinys[i] == '\n')
			eiluciu_skaicius++;
	}

	// iskiriame atminties piku masyvui
	// (dël visa ko pridedam vienetà, nes paskutinë eilutë galëjo neturëti '\n')
	pikai = (unsigned int *) malloc ((eiluciu_skaicius+1) * sizeof(int));
	if(pikai == NULL)
	{
		free (piku_failo_turinys);
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_LIST;
	}
	
	// analizuojame piku failo duomenis
	char * eilute;
	char * skirtukai = " \n";
	int i = 0;
	// nuskaitome piko reikðmæ
	eilute = strtok (piku_failo_turinys, skirtukai); // piko reikðmë
	// kol yra pikø
	while (eilute != NULL)
	{
		// konvertuojame piko reikðmæ á int
		int n = sscanf (eilute, "%d", &pikai[i]);
		if (n != 1) {
			free (piku_failo_turinys);
			return ERROR_RATECHANGE_NONNUMERICAL_PIK_VALUE;
		}
		// nuskaitome nauja piko reikðmæ
		eilute = strtok (NULL, skirtukai);
		// padidiname skaitliukà
		i++;
	}

	// isimename pikø kieki
	piku_kiekis = i;

	free (piku_failo_turinys);

	return 0;
}

/*********************************************************
 * Nuskaito reikiamus duomenis is failu.
 * Grazina 0, jei nebuvo klaidos, ir -1, jei buvo klaida.
 ********************************************************/
int nuskaityti_duomenis()
{
	// nuskaitome duomenis is signalo failo
	int a = nuskaityti_signala ();
	if (a < 0) return a;

	// nuskaitome duomenis ið anotacijø failo
	a = nuskaityti_anotacijas (fonemu_failo_pavadinimas, &fonemos, &fonemu_ilgiai, &fonemu_kiekis);
	if (a < 0) return a;

	// nuskaitome duomenis ið pikø failo
	a = nuskaityti_pikus ();
	if (a < 0) return a;

	return 0;
}
