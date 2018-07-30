///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas RateChange.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "RateChange.h"
#include "RateChangeInternal.h"
#include "../include/LithUSS_Error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void atlaisvinti_atminti_ir_inicializuoti();

EXPORT void loadRateChange()
{
	atlaisvinti_atminti_ir_inicializuoti();
}

EXPORT void unloadRateChange()
{
	atlaisvinti_atminti_ir_inicializuoti();
}

/*********************************************************
 * Global\xFBs kintamieji
 ********************************************************/

// numeris pirmojo piko, esan\xE8io einamosios fonemos prad\xFEioje 
// (tiksliau, pirmojo piko, nepriklausan\xE8io prie\xF0 tai buvusiai fonemai. 
// Jis gali nepriklausyti ir einamajai, o kuriai nors tolimesnei).
int * pirmojo_piko_nr_fonemose = NULL;

// pik\xF8 skai\xE8ius fonemoje
int * piku_skaicius_fonemose = NULL;

// fonem\xF8 prad\xFEi\xF8 indeksai signalo masyve.
// Kai naudojami dinaminiam grei\xE8io keitimui, nereikia atlaisvinti atminties, nes ji i\xF0skiriama ne RateChange.dll'e.
// Kai naudojami statiniam grei\xE8io keitimui, atmintis i\xF0skiriama ir atlaisvinama change_DB_rate() funkcijoje.
long * fonemu_adresai = NULL;

// jei ne nulis, i\xF0veda papildom\xE0 informacij\xE0 apie programos darb\xE0, apie kiekvien\xE0 apdorojam\xE0 fonem\xE0
// jei > 100, i\xF0veda detali\xE0 informacij\xE0
// jei > 200, be proto daug detali\xF8
int debuginam = 101;

/*********************************************************
* atlaisvinti_atminti_ir_inicializuoti
********************************************************/
void atlaisvinti_atminti_ir_inicializuoti()
{
	if (signalas != NULL) { free(signalas); signalas = NULL; }
	signalo_ilgis = 0;
	if (fonemos != NULL) {
		for (unsigned int i = 0; i < fonemu_kiekis; i++) {
			if (fonemos[i] != NULL) { free(fonemos[i]); fonemos[i] = NULL; }
		}
		free(fonemos); fonemos = NULL;
	}
	fonemu_kiekis = 0;
	if (fonemu_ilgiai != NULL) { free(fonemu_ilgiai); fonemu_ilgiai = NULL; }
	if (pikai != NULL) { free(pikai); pikai = NULL; }
	piku_kiekis = 0;
	if (skirtingos_fonemos != NULL) { free(skirtingos_fonemos); skirtingos_fonemos = NULL; }
	if (vidutiniai_fonemu_ilgiai != NULL) { free(vidutiniai_fonemu_ilgiai); vidutiniai_fonemu_ilgiai = NULL; }
	skirtingu_fonemu_kiekis = 0;
	if (pirmojo_piko_nr_fonemose != NULL) { free(pirmojo_piko_nr_fonemose); pirmojo_piko_nr_fonemose = NULL; }
	if (piku_skaicius_fonemose != NULL) { free(piku_skaicius_fonemose); piku_skaicius_fonemose = NULL; }
}


/*********************************************************
 * DllMain
 ********************************************************/
/*BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
		// apsitvarkome
		atlaisvinti_atminti_ir_inicializuoti ();
	}
    else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		atlaisvinti_atminti_ir_inicializuoti ();
	}

    return TRUE;
}*/

/*********************************************************
 * spausdinti_loga
 ********************************************************/
void spausdinti_loga(const char* pranesimas)
{
	// Gintaras: prid\xEBjau laiko \xFEym\xE6
	time_t mytime = time(NULL);
	char * mytime_string = ctime(&mytime);
	FILE *stream;
	stream = fopen("LUSS_log.txt", "a+");
	fprintf(stream, "%s%s\n", mytime_string, pranesimas);
	printf("%s%s\n", mytime_string, pranesimas);
	fclose(stream);
}

/*********************************************************
 * Prie\xF0 kvie\xE8iant \xF0i\xE0 funkcij\xE0, nuskaityti duomenis ir apskai\xE8iuoti greitinimo_koef.
 * Gr\xE0\xFEina rekomenduojam\xE0 naujo signalo masyvo ilg\xE1 - \xF0iek tiek didesn\xE1, nei reikt\xF8 pagal greitinimo koeficient\xE0.
 ********************************************************/
unsigned int rekomenduoti_naujo_signalo_masyvo_ilgi (int greitis, int tono_aukscio_pokytis) // ivertinti_naujo_signalo_ilgi ?
{
	// TODO: tur\xEBt\xF8 atsi\xFEvelgti ir \xE1 tono_aukscio_pokytis (kaip?)

	if (greitis == 100 && tono_aukscio_pokytis == 100)
		// naujo signalo ilgis sutaps su seno
		return signalo_ilgis;

	double greitinimo_koef = (double) greitis/100;

	// d\xEBl visa ko dar kiek padidinkime koeficient\xE0
	double padidintas_koef = greitinimo_koef * NAUJO_SIGNALO_MASYVO_ILGIO_KOEF;

	// jei signal\xE0 reikia labai sutrumpinti, gali b\xFBti, kad tiek sutrumpinti nepavyks, ir signalo ilgis bus didesnis. 
	// Tokiu atveju d\xEBl visa ko geriau i\xF0skirkime daugiau atminties.
	if (greitis < 60)
		padidintas_koef *= NAUJO_SIGNALO_MASYVO_ILGIO_KOEF;

	return (unsigned int) (signalo_ilgis * padidintas_koef);
}

/*********************************************************
 * Konteksto suk\xFBrimas ir inicializavimas turi vykti tik \xF0ioje proced\xFBroje, 
 * kad pakeitus konteksto sandar\xE0 (prid\xEBjus nauj\xF8 lauk\xF8), 
 * u\xFEtekt\xF8 pakeisti kod\xE0 tik \xF0ioje funkcijoje.
 ********************************************************/
void init_konteksta (struct tkontekstas * kontekstas) {
	kontekstas->fonemos_nr = 0;
	kontekstas->fonemos_pradzia = 0;
	kontekstas->fonemos_pabaiga = 0;
	kontekstas->pirmojo_piko_nr = 0;
	kontekstas->piku_sk = 0;
	kontekstas->greitinimo_koef = 1.0;
	kontekstas->einamasis_postumis = 0;
	kontekstas->einamasis_signalo_nr = 0;
	kontekstas->naujas_signalas = NULL;
	kontekstas->naujo_signalo_masyvo_ilgis = 0;
	//kontekstas->naujo_signalo_ilgis = 0;
	kontekstas->einamasis_naujo_signalo_nr = 0;
	kontekstas->galima_pailginti_naujas_signalas = 0;
	kontekstas->tarpo_tarp_piku_didinimo_koef = 1.0;
	kontekstas->fonemos_klase = -1;
	kontekstas->keisti_tono_auksti = 0;
	kontekstas->keiciamu_burbulu_sk = 0;
}

/*********************************************************
 * Konteksto suk\xFBrimas ir inicializavimas turi vykti tik \xF0ioje proced\xFBroje, 
 * kad pakeitus konteksto sandar\xE0 (prid\xEBjus nauj\xF8 lauk\xF8), 
 * u\xFEtekt\xF8 pakeisti kod\xE0 tik \xF0ioje funkcijoje.
 ********************************************************/
void spausdinti_konteksta (struct tkontekstas * kontekstas) {
	char pranesimas [2048];
	sprintf (pranesimas, "Kontekstas: [fonemos_nr: %d][fonemos_pav: %s][fonemos_pradzia: %d][fonemos_pabaiga: %d][fonemos_ilgis: %d][pirmojo_piko_nr: %d][paskutiniojo_piko_nr: %d][piku_sk: %d][pirmojo_piko_vieta: %d][paskutiniojo_piko_vieta: %d][einamasis_postumis: %d][einamasis_signalo_nr: %d][naujo_signalo_masyvo_ilgis: %d][einamasis_naujo_signalo_nr: %d][galima_pailginti_naujas_signalas: %d][fonemos_klase: %d][keisti_tono_auksti: %d][keiciamu_burbulu_sk: %d][greitinimo_koef: %.3f][tarpo_tarp_piku_didinimo_koef: %.3f]",
		kontekstas->fonemos_nr, 
		fonemos [kontekstas->fonemos_nr],
		kontekstas->fonemos_pradzia, 
		kontekstas->fonemos_pabaiga, 
		kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia,
		kontekstas->pirmojo_piko_nr,
		kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1,
		kontekstas->piku_sk,
		(kontekstas->pirmojo_piko_nr<piku_kiekis)?pikai [kontekstas->pirmojo_piko_nr]:-1,
		(kontekstas->pirmojo_piko_nr<piku_kiekis && kontekstas->pirmojo_piko_nr + kontekstas->piku_sk > 0)?pikai [kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1]:-1,
		kontekstas->einamasis_postumis,
		kontekstas->einamasis_signalo_nr,
		kontekstas->naujo_signalo_masyvo_ilgis,
		kontekstas->einamasis_naujo_signalo_nr,
		kontekstas->galima_pailginti_naujas_signalas,
		kontekstas->fonemos_klase,
		kontekstas->keisti_tono_auksti,
		kontekstas->keiciamu_burbulu_sk,
		kontekstas->greitinimo_koef,
		kontekstas->tarpo_tarp_piku_didinimo_koef);
	spausdinti_loga (pranesimas);
}

/*********************************************************
Gr\xE0\xFEina fonemos klas\xEBs numer\xE1 pagal fonemos pavadinimo pirm\xE0j\xE0 raid\xE6:
0 - turintys pik\xF8 informacij\xE0 (skardieji priebalsiai, balsiai, t.y. visi, i\xF0skyrus x, f, p, t, k, s, S, _, r, R, z, Z, H)
1 - neturintys pik\xF8 informacijos (duslieji priebalsiai, t.y. x, f, p, t, k, s, S, _)
2 - gali tur\xEBti ar netur\xEBti pik\xF8 informacijos, tod\xEBl gali b\xFBti priskirti kuriai nors i\xF0 pirm\xF8j\xF8 dviej\xF8 klasi\xF8 - reikia papildomo tikrinimo (z, Z, h),
3 - neai\xF0ku, k\xE0 daryti (r, R).
*********************************************************/
int fonemosKlase (struct tkontekstas * kontekstas)
{

	switch (fonemos[kontekstas->fonemos_nr][0]) {

	case 'r':
	case 'R':
		return FONEMU_KLASE_RR;

	case 'x':
	case 'f':
	case 'p':
	case 't':
	case 'k':
	case 's':
	case 'S':
	case '_':
		return FONEMU_KLASE_DUSLIEJI;

	case 'z':
	case 'Z':
	case 'h':
		if (reguliarus_pikai (kontekstas))
			return FONEMU_KLASE_SKARDIEJI;
		else {

			if (debuginam) {
				char pranesimas [1024];
				sprintf (pranesimas, "RateChange.cpp: fonemosKlase(): nereguliarus garsas: %s, nr: %d, piku_sk: %d", 
					fonemos[kontekstas->fonemos_nr], kontekstas->fonemos_nr, kontekstas->piku_sk);
				spausdinti_loga (pranesimas);
			}

			return FONEMU_KLASE_DUSLIEJI;
		}

	default:
		return FONEMU_KLASE_SKARDIEJI;
	}

}

/*********************************************************
 * apskaiciuoti_pirmojo_piko_nr_fonemose
 ********************************************************/
int apskaiciuoti_pirmojo_piko_nr_fonemose ()
{
	// i\xF0skiriame atminties
	pirmojo_piko_nr_fonemose = (int *) malloc ((fonemu_kiekis+1) * sizeof(int));
	if(pirmojo_piko_nr_fonemose == NULL) 
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_INFO;
	piku_skaicius_fonemose = (int *) malloc ((fonemu_kiekis+1) * sizeof(int));
	if(pirmojo_piko_nr_fonemose == NULL) 
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_INFO;

	// einamosios fonemos prad\xFEia ir pabaiga
	unsigned int fonemos_pradzia = 0;
	unsigned int fonemos_pabaiga = 0;

	// numeris pirmojo piko, esan\xE8io einamosios fonemos prad\xFEioje 
	// (tiksliau, pirmojo piko, nepriklausan\xE8io prie\xF0 tai buvusiai fonemai. 
	// Jis gali nepriklausyti ir einamajai, o kuriai nors tolimesnei).
	int pirmojo_piko_nr = 0;

	// kiekvienai fonemai darome \xF0tai k\xE0:
	for (unsigned int fonemos_nr = 0; fonemos_nr < fonemu_kiekis; fonemos_nr++) {

		// randame fonemos pabaig\xE0
		fonemos_pabaiga = fonemos_pradzia + fonemu_ilgiai[fonemos_nr];

		// suskai\xE8iuojame, kiek pik\xF8 yra tarp prad\xFEios ir pabaigos
		int piku_sk = 0; 
		unsigned int i = pirmojo_piko_nr;
		while (i < piku_kiekis && pikai[i] < fonemos_pabaiga) 
			i++;
		piku_sk = (int) i - pirmojo_piko_nr;

		// \xE1simename
		pirmojo_piko_nr_fonemose [fonemos_nr] = pirmojo_piko_nr;
		piku_skaicius_fonemose [fonemos_nr] = piku_sk;

		// atnaujiname fonemos prad\xFEi\xE0 ir pirmojo piko nr
		fonemos_pradzia = fonemos_pabaiga;
		pirmojo_piko_nr += piku_sk;
	}

	return 0;
}

/*********************************************************
 * pakeiciam fonemos, kurios numeris yra fonemos_nr, greit\xE1 ir tono auk\xF0t\xE1.
 * Parametras "greitis" nurodo procentais, kiek pailginti fonem\xE0 (pavyzd\xFEiui, 120 rei\xF0kia pailginti 1,2 karto).
 * Parametras "tono_aukscio_pokytis" nurodo procentais, kiek paauk\xF0tinti pagrindin\xE1 ton\xE0 
 * (pavyzd\xFEiui, 120 rei\xF0kia paauk\xF0tinti 1,2 karto: jei pagrindinis tonas buvo 100 Hz, pasidarys 120 Hz).
 * Pat\xE1 nauj\xE0 signal\xE0 \xE1ra\xF0o \xE1 naujas_signalas masyv\xE0 (tiksliau, prie jo prisumuoja).
 * Laikome, kad "naujas_signalas" rodo \xE1 prie\xF0 tai buvusios (jei buvo) fonemos pabaig\xE0+1.
 *
 * Gr\xE0\xFEina naujo signalo ilg\xE1, jei pavyko, ir -1, jei nepavyko (jei masyve naujas_signalas neu\xFEteko vietos).
 ********************************************************/
int change_phoneme_rate_internal (int greitis, int tono_aukscio_pokytis, unsigned int fonemos_nr, 
						 short ** naujas_signalas, unsigned int * naujo_signalo_masyvo_ilgis, 
						 int galima_pailginti_naujas_signalas, unsigned int einamasis_naujo_signalo_nr)
{
	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): pradedame doroti fonema '%s\' (nr: %d), greitis %d, tono_aukscio_pokytis %d", fonemos[fonemos_nr], fonemos_nr, greitis, tono_aukscio_pokytis);
		spausdinti_loga (pranesimas);
	}

	// ------------------------- Inicializuojame parametrus --------------------------------- //

	// sukuriame kontekst\xE0
	struct tkontekstas kkontekstas;
	struct tkontekstas * kontekstas = &kkontekstas;
	init_konteksta (kontekstas);

	if (debuginam > 100) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): sukureme konteksta");
		spausdinti_konteksta (kontekstas);
	}

	kontekstas->fonemos_nr = fonemos_nr;

	// inicializuojame rezultat\xF8 masyv\xE0
	kontekstas->naujas_signalas = *naujas_signalas;
	kontekstas->naujo_signalo_masyvo_ilgis = *naujo_signalo_masyvo_ilgis;
	kontekstas->galima_pailginti_naujas_signalas = galima_pailginti_naujas_signalas;

	// gr\xE0\xFEiname rodykles \xE1 prad\xFEi\xE0
	kontekstas->einamasis_signalo_nr = fonemu_adresai [fonemos_nr];
	kontekstas->einamasis_naujo_signalo_nr = einamasis_naujo_signalo_nr;

	// inicializuojam einam\xE0j\xE1 post\xFBm\xE1
	kontekstas->einamasis_postumis = 0;
	
	// einamosios fonemos prad\xFEia ir pabaiga
	kontekstas->fonemos_pradzia = (int) fonemu_adresai [fonemos_nr];
	kontekstas->fonemos_pabaiga = kontekstas->fonemos_pradzia + fonemu_ilgiai[fonemos_nr];

	// numeris pirmojo piko, esan\xE8io einamosios fonemos prad\xFEioje 
	// (tiksliau, pirmojo piko, nepriklausan\xE8io prie\xF0 tai buvusiai fonemai. 
	// Jis gali nepriklausyti ir einamajai, o kuriai nors tolimesnei).
	kontekstas->pirmojo_piko_nr = pirmojo_piko_nr_fonemose [fonemos_nr];

	// kiek pik\xF8 yra tarp prad\xFEios ir pabaigos
	kontekstas->piku_sk = piku_skaicius_fonemose [fonemos_nr];
	
	// nustatome fonemos klas\xE6
	kontekstas->fonemos_klase = fonemosKlase (kontekstas);

	// ar keisti tono auk\xF0t\xE1
	kontekstas->keisti_tono_auksti =
		(kontekstas->fonemos_klase == FONEMU_KLASE_SKARDIEJI || kontekstas->fonemos_klase == FONEMU_KLASE_RR)
		&& (tono_aukscio_pokytis != 100)
		&& (kontekstas->piku_sk > 1);

	// nustatome tarpo tarp pik\xF8 keitimo koeficient\xE0
	if (kontekstas->keisti_tono_auksti)
		kontekstas->tarpo_tarp_piku_didinimo_koef = 100.0 / tono_aukscio_pokytis;
	else
		kontekstas->tarpo_tarp_piku_didinimo_koef = 1.0;

	// jei r, R, tai grei\xE8io nekei\xE8iame (nors tono auk\xF0t\xE1 galime keisti), t.y. neatsi\xFEvelgiame \xE1 nurodyt\xE0 greitinimo koeficiento reik\xF0m\xE6
	// (t.y. jei tono auk\xF0t\xE1 keisime, teks keisti ir greit\xE1, bet tik tiek, kad atstatytume fonemos ilg\xE1 \xE1 buvus\xE1).
	//if (kontekstas->fonemos_klase == FONEMU_KLASE_RR)
	//	greitis = 100;

	// apskai\xE8iuojame reikiam\xE0 greitinimo koeficient\xE0 pagal pateiktus greitinimo ir tono keitimo koeficientus
	//kontekstas->greitinimo_koef = (((double)greitis) / 100) / kontekstas->tarpo_tarp_piku_didinimo_koef;
		
	// apskai\xE8iuojame reikiam\xE0 greitinimo koeficient\xE0 pagal pateiktus greitinimo ir tono keitimo koeficientus
	if (kontekstas->fonemos_klase == FONEMU_KLASE_RR)
		// jei r, R, tai grei\xE8io nekei\xE8iame (nors tono auk\xF0t\xE1 galime keisti), t.y. neatsi\xFEvelgiame \xE1 nurodyt\xE0 greitinimo koeficiento reik\xF0m\xE6
		if (kontekstas->tarpo_tarp_piku_didinimo_koef < 1)
			// jei tono auk\xF0t\xE1 didinsime, teks keisti ir greit\xE1, bet tik tiek, kad atstatytume fonemos ilg\xE1 \xE1 buvus\xE1
			kontekstas->greitinimo_koef = 1 / kontekstas->tarpo_tarp_piku_didinimo_koef;
		else
			// jei tono auk\xF0t\xE1 ma\xFEinsime (ar jo nekeisime), grei\xE8io nekeisime (neatstatysime fonemos ilgio \xE1 buvus\xE1)
			// (t.y. jei fonemos r, R tono auk\xF0t\xE1 ma\xFEinsime, tai jos ilgis padid\xEBs)
			kontekstas->greitinimo_koef = 1;
	else
		// skard\xFEiosioms fonemoms
		kontekstas->greitinimo_koef = (((double)greitis) / 100) / kontekstas->tarpo_tarp_piku_didinimo_koef;

	// ------------------------- Euristika --------------------------------- //

	// kei\xE8iam\xF8 (\xF0alinam\xF8 ar dubliuojam\xF8) burbul\xF8 skai\xE8ius
	kontekstas->keiciamu_burbulu_sk = 0;
	
	if (debuginam) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): inicializavome parametrus");
		spausdinti_konteksta (kontekstas);
	}

	// euristi\xF0kai parinkti burbuliukus i\xF0metimui
	euristika (kontekstas);
	
	if (debuginam > 100) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): pritaikeme euristika");
		spausdinti_konteksta (kontekstas);
		spausdinti_burbulus (kontekstas->burbulai, kontekstas->keiciamu_burbulu_sk);
	}

	// ------------------------- Apdorojame signal\xE0 --------------------------------- //

	// jei pavyko == -1, visk\xE0 stabdome, nes nepavyko i\xF0skirti atminties
	int pavyko = 0;

	// tono auk\xF0\xE8io keitimas: apdorojame pus\xE6 pirmo burbulo, i\xF0lendan\xE8i\xE0 \xE1 prie\xF0 tai buvusi\xE0 fonem\xE0
	if (kontekstas->keisti_tono_auksti)
		pavyko = kopijuoti_signala_pradzioj (kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1)
		return DIDELIS_NEIGIAMAS_KLAIDOS_KODAS;

	if (debuginam > 100) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): nukopijavome signala, esanti ant fonemos pradzios ribos (jei keiciame tono auksti)");
		spausdinti_konteksta (kontekstas);
	}

	// i\xF0mesti parinktus burbuliukus, perskai\xE8iuoti masyvus
	if (kontekstas->greitinimo_koef<1)
		pavyko = trumpinti_fonema (kontekstas);
	else
		pavyko = ilginti_fonema (kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) {
		return DIDELIS_NEIGIAMAS_KLAIDOS_KODAS; 
	}

	if (debuginam > 100) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): sutrumpinome/pailginome fonema");
		spausdinti_konteksta (kontekstas);
	}

	// nustatome, iki kiek kopijuojame signal\xE0
	int iki = 0;
	if (kontekstas->keisti_tono_auksti)
		// tono auk\xF0\xE8io keitimas: kopijuosime iki paskutinio piko
		iki = pikai [kontekstas->pirmojo_piko_nr + kontekstas->piku_sk -1];
	else
		// jei nekeisime tono auk\xF0\xE8io, kopijuosime iki fonemos pabaigos
		iki = kontekstas->fonemos_pabaiga;

	// pabaigiame nukopijuoti signalo masyv\xE0
	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki signalo pabaigos
	// (negalime kopijuoti su memcpy, nes prarasime jau ten esan\xE8i\xE0 informacij\xE0).
	// Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes.
	pavyko = kopijuoti_signala (iki, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) {
		return DIDELIS_NEIGIAMAS_KLAIDOS_KODAS; 
	}

	if (debuginam > 100) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): nukopijavome likusi signala");
		spausdinti_konteksta (kontekstas);
	}

	// tono auk\xF0\xE8io keitimas: apdorojame pus\xE6 paskutinio burbulo, i\xF0lendan\xE8i\xE0 \xE1 po to einan\xE8i\xE0 fonem\xE0
	if (kontekstas->keisti_tono_auksti)
		pavyko = kopijuoti_signala_pabaigoj (kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1)
		return DIDELIS_NEIGIAMAS_KLAIDOS_KODAS;

	if (debuginam > 100) {
		spausdinti_loga ("RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): nukopijavome signala, esanti ant fonemos pabaigos ribos (jei keiciame tono auksti)");
		spausdinti_konteksta (kontekstas);
	}

	// apskai\xE8iuojame nauj\xE0j\xE1 fonemos ilg\xE1
	int naujas_fonemos_ilgis = fonemu_ilgiai[fonemos_nr] + kontekstas->einamasis_postumis;

	// atnaujiname reik\xF0mes
	*naujas_signalas = kontekstas->naujas_signalas;
	*naujo_signalo_masyvo_ilgis = kontekstas->naujo_signalo_masyvo_ilgis;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.dll: RateChange.cpp: change_phoneme_rate_internal(): sekmingai apdorota fonema '%s\' (nr: %d), buves ilgis=%d, naujas ilgis=%d, faktinis pailgejimas=%.3f", 
			fonemos[fonemos_nr], fonemos_nr, kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia, naujas_fonemos_ilgis, 
			(double)naujas_fonemos_ilgis/(kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia));
		spausdinti_loga (pranesimas);
		spausdinti_konteksta (kontekstas);
	}

	return naujas_fonemos_ilgis;
}

/*********************************************************
 * Nuskaitom BD is failu ir pakeiciam kalbejimo greiti
 ********************************************************/
EXPORT int change_DB_rate (char *katVardas, int greitis, int tono_aukscio_pokytis, char dbfv1[][4],
					int *dbilg1, long *dbadr1, short ** wholeinputt1)
{
	clock_t begin_time,end_time;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: change_DB_rate(): pradzia. Katalogas: %s, koef=%d", katVardas, greitis);
		spausdinti_loga (pranesimas);
		begin_time = clock ();
	}

	// ------------------------- Nuskaitome duomenis --------------------------------- //

	// apsitvarkome

	atlaisvinti_atminti_ir_inicializuoti ();

	// pasiruo\xF0imas duomen\xF8 nuskaitymui

	// sudarome gars\xF8 duomen\xF8 baz\xEBs failo pavadinim\xE0
	strcpy (signalo_failo_pavadinimas, katVardas);
	strcat (signalo_failo_pavadinimas, "db.raw");

	// sudarome fonem\xF8 failo pavadinim\xE0
	strcpy(fonemu_failo_pavadinimas, katVardas);
	strcat(fonemu_failo_pavadinimas, "db_fon_weights.txt");

	// sudarome pik\xF8 failo pavadinim\xE0
	strcpy(piku_failo_pavadinimas, katVardas);
	strcat(piku_failo_pavadinimas, "db_pik.txt");

	// nuskaitome duomenis is failu (u\xFEpildome duomen\xF8 masyvus)
	int a = nuskaityti_duomenis();
	if (a < 0) return a;

	if (debuginam) {
		end_time = clock ();
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: change_DB_rate(): duomenys nuskaityti per %.3f sec", ((double)(end_time-begin_time))/CLOCKS_PER_SEC);
		spausdinti_loga (pranesimas);
		begin_time = clock ();
	}

	// ------------------------- Inicializuojame parametrus --------------------------------- //

	// apskai\xE8iuojame pagalbinius masyvus darbui su pikais
	a = apskaiciuoti_pirmojo_piko_nr_fonemose ();
	if (a < 0) return a;

	// sukuriame kontekst\xE0
	//struct tkontekstas kkontekstas;
	//struct tkontekstas * kontekstas = &kkontekstas;
	//init_konteksta (kontekstas);

	// nustatome greitinimo koeficient\xE0
	//kontekstas->greitinimo_koef = ((double)greitis) / 100;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: change_DB_rate(): Greitinimo koeficientas: %d", greitis);
		spausdinti_loga (pranesimas);
	}

	// \xE1vertiname pailginto signalo masyvo ilg\xE1
	unsigned int naujo_signalo_masyvo_ilgis = rekomenduoti_naujo_signalo_masyvo_ilgi (greitis, tono_aukscio_pokytis);

	// pa\xFEymime, kad masyv\xE0 naujas_signalas galima ilginti, jei nety\xE8ia jam i\xF0skirta per ma\xFEai atminties
	int galima_pailginti_naujas_signalas = 1;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: change_DB_rate(): signalo ilgis: %d, naujo_signalo_masyvo_ilgis: %d", 
			signalo_ilgis, naujo_signalo_masyvo_ilgis);
		spausdinti_loga (pranesimas);
	}

	// inicializuojame rezultat\xF8 masyvus

	short * naujas_signalas = (short *) calloc (naujo_signalo_masyvo_ilgis, sizeof(short)); // kad u\xFEpildyt\xF8 nuliais
	if (naujas_signalas == NULL) {
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_CHANGED_RATE_DB; 
	}

	// susikuriame ir u\xFEpildome pagalbin\xE1 fonem\xF8 adres\xF8 masyv\xE0
	
	fonemu_adresai = (long *) calloc (fonemu_kiekis+1, sizeof(long));
	if (fonemu_adresai == NULL) {
		return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_CHANGED_RATE_DB; 
	}

	unsigned int l;
	fonemu_adresai[0]=0;
	for(l=0; l<fonemu_kiekis; l++) {
		fonemu_adresai[l+1] = fonemu_adresai[l] + fonemu_ilgiai[l];
	}

	// nauji_fonemu_ilgiai

	nauji_fonemu_ilgiai = dbilg1;

	if (debuginam) {
		spausdinti_loga ("RateChange.cpp: change_DB_rate(): pasiruoseme greicio keitimui");
	}

	// ------------------------- Skai\xE8iuojame --------------------------------- //

	unsigned int einamasis_naujo_signalo_nr = 0;

	for (unsigned int fonemos_nr = 0; fonemos_nr < fonemu_kiekis; fonemos_nr++) {
		
		// kiekvienai fonemai kvie\xE8iame change_phoneme_rate_internal() funkcij\xE0
		int naujas_fonemos_ilgis = change_phoneme_rate_internal (
			greitis, tono_aukscio_pokytis, fonemos_nr,
			&naujas_signalas, (unsigned int*) &naujo_signalo_masyvo_ilgis,
			galima_pailginti_naujas_signalas, einamasis_naujo_signalo_nr);
		// jei nepavyko, visk\xE0 stabdome
		if (naujas_fonemos_ilgis == DIDELIS_NEIGIAMAS_KLAIDOS_KODAS)
			return ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_CHANGED_RATE_DB;

		// perskai\xE8iuojame fonem\xF8 ilgi\xF8 masyv\xE0
		nauji_fonemu_ilgiai[fonemos_nr] = naujas_fonemos_ilgis;

		// atnaujiname einamasis_naujo_signalo_nr
		einamasis_naujo_signalo_nr += naujas_fonemos_ilgis;
		
	}

	if (debuginam) {
		end_time = clock ();
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: change_DB_rate(): greicio keitimas baigtas per %.3f sec\nFaktinis ilginimo koeficientas = %.3f", ((double)(end_time-begin_time))/CLOCKS_PER_SEC, (double)einamasis_naujo_signalo_nr/signalo_ilgis);
		spausdinti_loga (pranesimas);
		begin_time = clock ();
	}

	// ------------------------- pritaikome rezultatus LithUSS'ui --------------------------------- //

	*wholeinputt1 = naujas_signalas;

	nauji_fonemu_ilgiai[fonemu_kiekis]=0;

	dbadr1[0]=0;
	for(l=0; l<fonemu_kiekis; l++) {
		dbadr1[l+1] = dbadr1[l] + nauji_fonemu_ilgiai[l];
		dbfv1[l][0] = fonemos[l][0];
		dbfv1[l][1] = fonemos[l][1];
		if (dbfv1[l][1] == 0) dbfv1[l][2] = 0; else dbfv1[l][2] = fonemos[l][2];
		if (dbfv1[l][2] == 0) dbfv1[l][3] = 0; else dbfv1[l][3] = fonemos[l][3];
	}

	if (debuginam) {
		spausdinti_loga ("change_DB_rate: rezultatai paruosti");
	}

	// apsitvarkome
	atlaisvinti_atminti_ir_inicializuoti ();
	// turime atlaisvinti atminti \xF0ioje funkcijoje, nes \xE8ia suk\xFBr\xEBme, o dinaminiu re\xFEimu visai nereikia atlaisvinti
	if(fonemu_adresai != NULL) {free(fonemu_adresai); fonemu_adresai=NULL;}
	
	if (debuginam) {
		end_time = clock ();
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: change_DB_rate(): pabaiga per %.3f sec", ((double)(end_time-begin_time))/CLOCKS_PER_SEC);
		spausdinti_loga (pranesimas);
	}

	return NO_ERR;
}


EXPORT void getData(int * pIlgis, short ** ppData, int * pFonemuSkaicius, int ** ppFonemuIlgiai, char *** pppFonemos)
{
	*pIlgis = signalo_ilgis;
	*ppData = signalas;
	*pFonemuSkaicius = fonemu_kiekis;
	*ppFonemuIlgiai = fonemu_ilgiai;
	*pppFonemos = fonemos;
}

/*********************************************************
 * Nuskaitom BD is failu
 ********************************************************/
EXPORT int initRateChange (const char *katVardas, char dbfv1[][4], int *dbilg1, long *dbadr1, short ** wholeinput1)
{
	clock_t begin_time, end_time;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: initRateChange(): pradzia. Katalogas: %s", katVardas);
		spausdinti_loga (pranesimas);
		begin_time = clock ();
	}

	// apsitvarkome
	atlaisvinti_atminti_ir_inicializuoti ();
	
	// pasiruo\xF0imas duomen\xF8 nuskaitymui

	// sudarome gars\xF8 duomen\xF8 baz\xEBs failo pavadinim\xE0
	strcpy (signalo_failo_pavadinimas, katVardas);
	strcat (signalo_failo_pavadinimas, "db.raw");

	// sudarome fonem\xF8 failo pavadinim\xE0
	strcpy(fonemu_failo_pavadinimas, katVardas);
	strcat(fonemu_failo_pavadinimas, "db_fon_weights.txt");

	// sudarome pik\xF8 failo pavadinim\xE0
	strcpy(piku_failo_pavadinimas, katVardas);
	strcat(piku_failo_pavadinimas, "db_pik.txt");

	// nuskaitome duomenis is failu (u\xFEpildome duomen\xF8 masyvus)
	int a = nuskaityti_duomenis();
	if (a < 0) return a;

	if (debuginam) {
		end_time = clock ();
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: initRateChange(): duomenys nuskaityti per %.3f sec", ((double)(end_time-begin_time))/CLOCKS_PER_SEC);
		spausdinti_loga (pranesimas);
		begin_time = clock ();
	}

	// apskai\xE8iuojame pagalbinius masyvus darbui su pikais
	a = apskaiciuoti_pirmojo_piko_nr_fonemose ();
	if (a < 0) return a;

	// \xE1simename fonem\xF8 prad\xFEi\xF8 indeks\xF8 masyv\xE0. 
	// J\xE1 naudosime dinaminiam grei\xE8io keitimui.
	// (Gal pasidaryti kopij\xE0?)
	fonemu_adresai = dbadr1;

	// pritaikome duomenis Pijaus LithUSS'ui

	*wholeinput1 = signalas;

	dbilg1[fonemu_kiekis]=0;

	unsigned int l;
	fonemu_adresai[0]=0;
	for(l=0; l<fonemu_kiekis; l++) {
		dbilg1[l] = fonemu_ilgiai[l];
		fonemu_adresai[l+1] = fonemu_adresai[l] + fonemu_ilgiai[l];
		dbfv1[l][0] = fonemos[l][0];
		dbfv1[l][1] = fonemos[l][1];
		if (dbfv1[l][1] == 0) dbfv1[l][2] = 0; else dbfv1[l][2] = fonemos[l][2];
		if (dbfv1[l][2] == 0) dbfv1[l][3] = 0; else dbfv1[l][3] = fonemos[l][3];
	}

	if (debuginam) {
		end_time = clock ();
		char pranesimas [1024];
		sprintf (pranesimas, "RateChange.cpp: initRateChange(): pabaiga per %.3f sec", ((double)(end_time-begin_time))/CLOCKS_PER_SEC);
		spausdinti_loga (pranesimas);
	}
	return NO_ERR;
}

/*********************************************************
 * pakeiciam fonemos, kurios numeris yra fonemos_nr, greit\xE1 ir tono auk\xF0t\xE1.
 * Parametras "greitis" nurodo procentais, kiek pailginti fonem\xE0 (pavyzd\xFEiui, 120 rei\xF0kia pailginti 1,2 karto).
 * Parametras "tono_aukscio_pokytis" nurodo procentais, kiek paauk\xF0tinti pagrindin\xE1 ton\xE0 
 * (pavyzd\xFEiui, 120 rei\xF0kia paauk\xF0tinti 1,2 karto: jei pagrindinis tonas buvo 100 Hz, pasidarys 120 Hz).
 * Pat\xE1 nauj\xE0 signal\xE0 \xE1ra\xF0o \xE1 naujas_signalas masyv\xE0 (tiksliau, prie jo prisumuoja).
 * Laikome, kad "naujas_signalas" rodo \xE1 prie\xF0 tai buvusios (jei buvo) fonemos pabaig\xE0+1.
 *
 * Gr\xE0\xFEina naujo signalo ilg\xE1, jei pavyko, ir -1, jei nepavyko (jei masyve naujas_signalas neu\xFEteko vietos).
 ********************************************************/
EXPORT int change_phoneme_rate (int greitis, int tono_aukscio_pokytis, unsigned int fonemos_nr,
						 short * naujas_signalas, unsigned int naujo_signalo_masyvo_ilgis)
{
	int galima_pailginti_naujas_signalas = 0;
	unsigned int einamasis_naujo_signalo_nr = 0;
	int naujas_fonemos_ilgis = change_phoneme_rate_internal (greitis, tono_aukscio_pokytis, fonemos_nr, &naujas_signalas, &naujo_signalo_masyvo_ilgis, 
		galima_pailginti_naujas_signalas, einamasis_naujo_signalo_nr);
	// jei nepavyko, visk\xE0 stabdome
	if (naujas_fonemos_ilgis == DIDELIS_NEIGIAMAS_KLAIDOS_KODAS)
		return ERROR_RATECHANGE_SIGNAL_BUFFER_OVERFLOW;
	else
		return naujas_fonemos_ilgis;
}