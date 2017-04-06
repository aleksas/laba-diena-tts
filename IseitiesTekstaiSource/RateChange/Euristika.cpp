///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas Euristika.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// Euristin\xEBs funkcijos: 
// funkcijos, kurios parenka, kuriuos burbulus reik\xEBt\xF8 pa\xF0alinti ar dubliuoti. 
// Jos gali \xE1gyvendinti \xE1vairias strategijas.
//

#include "RateChangeInternal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*********************************************************
 *********************************************************
             PAGALBIN\xCBS FUNKCIJOS
 *********************************************************
 *********************************************************/

/*********************************************************
Nustato, ar duotoje fonemoje pikai pasiskirst\xE6 reguliariai. 
Tikrina tik tai, ar ne per retai, t.y. ar netr\xFBksta kokio piko.
*********************************************************/
bool reguliarus_pikai (struct tkontekstas * kontekstas)
{
	// jei i\xF0vis n\xEBra pik\xF8
	if (kontekstas->piku_sk <= 0)
		return false;
	
	// jei pernelyg didelis atstumas nuo fonemos prad\xFEios iki pirmojo piko
	if (pikai [kontekstas->pirmojo_piko_nr] - kontekstas->fonemos_pradzia > MAX_ATSTUMAS_TARP_PIKU)
		return false;
	
	// jei pernelyg didelis atstumas nuo paskutinio piko iki fonemos pabaigos
	if (kontekstas->fonemos_pabaiga - pikai [kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1] > MAX_ATSTUMAS_TARP_PIKU)
		return false;
	
	// jei pernelyg dideli atstumai tarp pik\xF8
	for (unsigned int i = 0; i < kontekstas->piku_sk - 1; i++) {
		if (pikai[kontekstas->pirmojo_piko_nr + i + 1] - pikai[kontekstas->pirmojo_piko_nr + i] > MAX_ATSTUMAS_TARP_PIKU)
			return false;
	}
	
	return true;
}

/*********************************************************
U\xFEpildo informacij\xE0 apie nurodytus burbulus.
Burbul\xF8 numeriai turi b\xFBti i\xF0reik\xF0ti viso signalo (ne vienos fonemos) pik\xF8 numeriais
*********************************************************/
void suformuoti_nurodytus_burbulus (int pirmojo_burbulo_nr, 
								    int paskutiniojo_burbulo_nr, int pakartojimu_skaicius, struct tkontekstas * kontekstas)
{
		int keiciamu_burbulu_sk = paskutiniojo_burbulo_nr - pirmojo_burbulo_nr + 1;
		
		if (kontekstas->keiciamu_burbulu_sk + keiciamu_burbulu_sk > MAX_KEICIAMI_BURBULAI)
		{
			// TODO: geriau ka\xFEk\xE0 daryti, padidinti burbulai masyv\xE0
			//fprintf (stderr, "Klaida: programa nesusidoroja, padidinti MAX_KEICIAMI_BURBULAI parametra");

			char klaidos_pranesimas [256];
			sprintf (klaidos_pranesimas, 
				"programa nesusidoroja, padidinti MAX_KEICIAMI_BURBULAI parametra: dabar yra %d, o turi buti bent %d", 
				MAX_KEICIAMI_BURBULAI, kontekstas->keiciamu_burbulu_sk + keiciamu_burbulu_sk);
			spausdinti_loga (klaidos_pranesimas);
			exit (1);
		}
		
		for (int burb_nr = 0; burb_nr < keiciamu_burbulu_sk; burb_nr++) {
			// suformuojame burbul\xE0
			int burbulo_vidurinio_piko_nr = pirmojo_burbulo_nr + burb_nr;
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].pradzia = pikai [burbulo_vidurinio_piko_nr - 1];
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].vidurys = pikai [burbulo_vidurinio_piko_nr];
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].pabaiga = pikai [burbulo_vidurinio_piko_nr + 1];
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].kartai = pakartojimu_skaicius;
		}

		kontekstas->keiciamu_burbulu_sk += keiciamu_burbulu_sk;
}

/*********************************************************
Koreguoja grei\xE8io koeficient\xE0, priklausomai nuo fonemos pavadinimo 
(balsiams nekei\xE8ia, kitiems suma\xFEina, sprogstamiesiems dar labiau suma\xFEina)
*********************************************************/
double koreguoti_greitinimo_koef_scenarijus4 (struct tkontekstas * kontekstas)
{
	// parodo, kiek patrump\xEBjimas/pailg\xEBjimas bus ma\xFEesnis u\xFE standartin\xE1
	// pvz. 0.5 rei\xF0kia, kad pailg\xEBs/patrump\xEBs 50% standartinio 
	double koregavimo_koeficientas = 1;

	switch (fonemos[kontekstas->fonemos_nr][0]) {
	// balsiams nekei\xE8iame	
	case 'i':
	case 'e':
	case 'a':
	case 'o':
	case 'u':
	case 'I':
	case 'E':
	case 'A':
	case 'O':
	case 'U':
		koregavimo_koeficientas = 1;
		break;
    // 
	case 'p':
	case 't':
	case 'k':
	case 'b':
	case 'd':
	case 'g':
		koregavimo_koeficientas = 0.2;
		break;
    // 
	case 's':
	case 'S':
	case 'z':
	case 'Z':
	case 'x':
	case 'h':
	case 'f':
		koregavimo_koeficientas = 0.5;
		break;
    // 
	case 'j':
	case 'J':
	case 'v':
	case 'w':
	case 'W':
	case 'l':
	case 'L':
	case 'm':
	case 'M':
	case 'n':
	case 'N':
		koregavimo_koeficientas = 0.5;
		break;
    // 
	case '_':
		koregavimo_koeficientas = 0.5;
		break;
    // 
	default:
		char klaidos_pranesimas [256];
		sprintf (klaidos_pranesimas, "nezinoma fonema: %s, nr: %d", fonemos[kontekstas->fonemos_nr], kontekstas->fonemos_nr);
		spausdinti_loga (klaidos_pranesimas);
		koregavimo_koeficientas = 0;
		break;
	}

	return 1 + koregavimo_koeficientas * (kontekstas->greitinimo_koef - 1);

}

/*********************************************************
Koreguoja grei\xE8io koeficient\xE0, priklausomai nuo fonemos ilgio skirtumo nuo vidutinio ilgio 
(patempia link vidutinio ilgio priklausomai nuo scenarijaus5_koeficientas reik\xF0m\xEBs)
*********************************************************/
double koreguoti_greitinimo_koef_scenarijus5 (int fonemos_ilgis, struct tkontekstas * kontekstas)
{
	// nustatykime, koks yra vidutinis tokios fonemos ilgis

	// ie\xF0kome einamosios fonemos skirting\xF8 fonem\xF8 masyve
	size_t fon_nr = 0;
	while (fon_nr < skirtingu_fonemu_kiekis && strcmp (fonemos[kontekstas->fonemos_nr], skirtingos_fonemos[fon_nr]) != 0)
		fon_nr++;

	// jei neradome, klaida
	if (fon_nr >= skirtingu_fonemu_kiekis) {
		char klaidos_pranesimas [256];
		sprintf (klaidos_pranesimas, "5 scenarijus: nezinomas fonemos \'%s\' (nr: %d) vidutinis ilgis", fonemos[kontekstas->fonemos_nr], kontekstas->fonemos_nr);
		spausdinti_loga (klaidos_pranesimas);
		exit(1);
	}

	// i\xF0siai\xF0kinkime, kiek reikia pailginti fonem\xE0

	int siekiamas_vidutinis_fonemos_ilgis = (int) (vidutiniai_fonemu_ilgiai[fon_nr] * kontekstas->greitinimo_koef);
	double siekiamas_fonemos_ilgis = fonemos_ilgis + scenarijaus5_koeficientas * (siekiamas_vidutinis_fonemos_ilgis - fonemos_ilgis);

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: koreguoti_greitinimo_koef_scenarijus5(): fonemos_ilgis: %d, vidutinis_fonemos_ilgis: %d, siekiamas_vidutinis_fonemos_ilgis: %d, siekiamas_fonemos_ilgis: %4.0f, koef: %f", 
			fonemos_ilgis, vidutiniai_fonemu_ilgiai[fon_nr], siekiamas_vidutinis_fonemos_ilgis, siekiamas_fonemos_ilgis, siekiamas_fonemos_ilgis/fonemos_ilgis);
		spausdinti_loga (pranesimas);
	}

	return siekiamas_fonemos_ilgis/fonemos_ilgis;
}

/*********************************************************
 *********************************************************
             ILGINIMAS: SKARDIEJI
 *********************************************************
 *********************************************************/

/*********************************************************
*********************************************************/
void ilginimo_euristika_skardieji_scenarijus1 (int pirmojo_burbulo_nr, 
								   int paskutiniojo_burbulo_nr, struct tkontekstas * kontekstas)
{
		// i\xF0renkame vidurin\xE1 burbul\xE0 dubliavimui
		
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// vidurinio burbulo numeris (numeruojant nuo 0, skai\xE8iuojant nuo pirmojo_burbulo_nr)
		int vidurinio_burbulo_nr = pirmojo_burbulo_nr 
			+ (paskutiniojo_burbulo_nr - pirmojo_burbulo_nr)/2;
		
		// suformuojame burbul\xE0
		int burbulo_vidurinio_piko_nr = kontekstas->pirmojo_piko_nr + vidurinio_burbulo_nr;
		kontekstas->burbulai -> pradzia = pikai [burbulo_vidurinio_piko_nr - 1];
		kontekstas->burbulai -> vidurys = pikai [burbulo_vidurinio_piko_nr];
		kontekstas->burbulai -> pabaiga = pikai [burbulo_vidurinio_piko_nr + 1];
		kontekstas->burbulai -> kartai = 1;	
}

/*********************************************************
// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
Greitinimo koeficientas paduodamas tam, 
kad b\xFBt\xF8 galima i\xF0kviesti 4 scenarijuje su skirtingais greitinimo koeficientais
*********************************************************/
void ilginimo_euristika_skardieji_scenarijus3 (int pirmojo_burbulo_nr, 
								   int paskutiniojo_burbulo_nr,
								   double greitinimo_koef, struct tkontekstas * kontekstas)
{
	// reikia 5 scenarijuje
	if (greitinimo_koef <= 1)
	{
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}

	// i\xF0siai\xF0kinkime, kiek reikia pailginti fonem\xE0

	int fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	int siekiamas_fonemos_ilgis = (int) (fonemos_ilgis * greitinimo_koef);
	int siekiamas_pailginimas = siekiamas_fonemos_ilgis - fonemos_ilgis;

	// Algoritmas: dubliuosime kiekvien\xE0 burbul\xE0 po ka\xFEkiek kart\xF8 (nul\xE1 arba daugiau), 
	// plius vidurinius (i\xF0 eil\xEBs einan\xE8ius) burbulus dar vienu kartu daugiau, kad pailgintume tiek, kiek reikia.
	// 1. Nustatome, kiek pailg\xEBt\xF8, jei panaudotume visus burbulus. 
	// 2. Paskai\xE8iuojame, kiek kart\xF8 reikia panaudoti kiekvien\xE0 burbul\xE0. 
	// 3. Paskai\xE8iuojame, kuriuos vidurinius burbulus reiks dubliuoti papildomai.
	// Kadangi gauti lygiai tokio pailginimo, kokio reikia, nepavyks, 
	// tai imame tok\xE1 pailginim\xE0, kad paklaida b\xFBt\xF8 ma\xFEiausia 
	// (arba \xF0iek tiek pritr\xFBksta iki reikiamo pailginimo, arba \xF0iek tiek vir\xF0ija).

	// 1. Nustatome, kiek pailg\xEBt\xF8, jei panaudotume visus burbulus. 

	// maksimalus pailginimas, jei kiekvien\xE0 burbul\xE0 kartosime ne daugiau kaip po 1 kart\xE0
	int maksimalus_pailginimas = (pikai [kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr] 
		+ pikai [kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr + 1] 
		- pikai [kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr] 
		- pikai [kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr - 1])/2;

	// 2. Paskai\xE8iuojame, kiek kart\xF8 reikia panaudoti kiekvien\xE0 burbul\xE0. 

	// kiek kart\xF8 reikia kartoti visus burbulus
	int pakartojimu_skaicius = siekiamas_pailginimas / maksimalus_pailginimas;
	// kiek dar lieka pailginti, pakartojus reikiam\xE0 kart\xF8 skai\xE8i\xF8 vis\xE0 leistin\xE0 srit\xE1
	int trukstamas_pailginimas = siekiamas_pailginimas % maksimalus_pailginimas;

	// 3. Paskai\xE8iuojame, kuriuos vidurinius burbulus reiks dubliuoti papildomai.

	// algoritmas: dubliavimui i\xF0rinksime tiek vidurini\xF8 i\xF0 eil\xEBs einan\xE8i\xF8 
	// (t.y. gretim\xF8) galim\xF8 burbul\xF8, kad b\xFBt\xF8 gautas tr\xFBkstamas pailginimas 
	// (gali b\xFBti, kad bus i\xF0rinkti visi).

	// vidurinio burbulo numeris, i\xF0reik\xF0tas viso signalo (ne vienos fonemos) pik\xF8 numeriais 
	int vidurinio_burbulo_nr = kontekstas->pirmojo_piko_nr + (pirmojo_burbulo_nr + paskutiniojo_burbulo_nr)/2;

	// maksimalus burbul\xF8, kuriuos galime dubliuoti, skai\xE8ius
	int max_burbulu_sk = paskutiniojo_burbulo_nr - pirmojo_burbulo_nr + 1;

	// ie\xF0komas burbul\xF8 skai\xE8ius
	int burbulu_sk = 0;

	// ie\xF0komos burbul\xF8 sekos pirmojo ir paskutinio burbul\xF8 numeriai
	// (burbulo numeris sutampa su jo vidurinio piko numeriu,
	// o pikai fonemoje numeruojami nuo nulio)
	// inicializuojame taip, kad burbul\xF8 seka b\xFBt\xF8 tu\xF0\xE8ia
	int burbulu_sekos_pradzia = vidurinio_burbulo_nr + 1;
	int burbulu_sekos_pabaiga = vidurinio_burbulo_nr;

	// kiek pailg\xEBt\xF8, jei dubliuotume toki\xE0 sek\xE0
	int busimas_pailginimas = 0;
	int busimas_pailginimas_old = 0; // \xE1simename paskutinio burbulo tikrinimui

	// i\xF0siai\xF0kinkime, kiek reikia dubliuoti burbul\xF8 tokiam pailginimui

	while (busimas_pailginimas < trukstamas_pailginimas && burbulu_sk < max_burbulu_sk)
	{
		burbulu_sk++;
		
		// nustatome burbul\xF8 sekos prad\xFEi\xE0
		burbulu_sekos_pradzia = vidurinio_burbulo_nr - (burbulu_sk - 1)/2; // teisinga tik tada, kai burbulu_sk > 0

		// nustatome burbul\xF8 sekos pabaig\xE0
		burbulu_sekos_pabaiga = vidurinio_burbulo_nr + burbulu_sk/2;

		// kiek pailg\xEBt\xF8, jei dubliuotume toki\xE0 sek\xE0
		busimas_pailginimas_old = busimas_pailginimas;
		busimas_pailginimas = (pikai [burbulu_sekos_pabaiga] + pikai [burbulu_sekos_pabaiga + 1] 
			- pikai [burbulu_sekos_pradzia] - pikai [burbulu_sekos_pradzia - 1])/2;
	}

	// i\xF0siai\xF0kinkime, ar paskutinis burbulas tikrai reikalingas: 
	// paskutin\xE1 burbul\xE0 imame tik tuo atveju, jei j\xE1 imant paklaida bus ma\xFEesn\xEB, negu neimant

	if (burbulu_sk != 0 && busimas_pailginimas - trukstamas_pailginimas > trukstamas_pailginimas - busimas_pailginimas_old) {
		// paskutinio burbulo atsisakome, perskai\xE8iuojame reik\xF0mes
		burbulu_sk--;
		if (burbulu_sk == 0)
			burbulu_sekos_pradzia = vidurinio_burbulo_nr + 1;
		else
			burbulu_sekos_pradzia = vidurinio_burbulo_nr - (burbulu_sk - 1)/2; // teisinga tik tada, kai burbulu_sk > 0
		burbulu_sekos_pabaiga = vidurinio_burbulo_nr + burbulu_sk/2;
		busimas_pailginimas = (pikai [burbulu_sekos_pabaiga] + pikai [burbulu_sekos_pabaiga + 1] 
			- pikai [burbulu_sekos_pradzia] - pikai [burbulu_sekos_pradzia - 1])/2;
	}

	// suformuojame nurodytus burbulus
	if (pakartojimu_skaicius == 0)
		// dubliuoti tik vidurinius burbulus po vien\xE0 kart\xE0
		suformuoti_nurodytus_burbulus (burbulu_sekos_pradzia, burbulu_sekos_pabaiga, 1, kontekstas);
	else {
		// dubliuoti visus burbulus po pakartojimu_skaicius kart\xF8
		suformuoti_nurodytus_burbulus (kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr, kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr, pakartojimu_skaicius, kontekstas);

		// vidurinius burbulus dubliuoti vienu kartu daugiau
		for (int burb_nr = 0; burb_nr < burbulu_sekos_pabaiga - burbulu_sekos_pradzia + 1; burb_nr++) {
			(kontekstas->burbulai[burbulu_sekos_pradzia - kontekstas->pirmojo_piko_nr - pirmojo_burbulo_nr + burb_nr].kartai)++;
		}
	}

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: ilginimo_euristika_skardieji_scenarijus3(): fonemos_ilgis: %d, siekiamas_fonemos_ilgis: %d, siekiamas_pailginimas: %d, maksimalus_pailginimas: %d, pakartojimu_skaicius: %d, trukstamas_pailginimas: %d, burbulu_sekos_pradzia: %d, burbulu_sekos_pabaiga: %d, busimas_pailginimas: %d", 
			fonemos_ilgis, siekiamas_fonemos_ilgis, siekiamas_pailginimas, maksimalus_pailginimas, pakartojimu_skaicius, trukstamas_pailginimas, burbulu_sekos_pradzia - kontekstas->pirmojo_piko_nr, burbulu_sekos_pabaiga - kontekstas->pirmojo_piko_nr, busimas_pailginimas);
		spausdinti_loga (pranesimas);
		//spausdinti_burbulus (kontekstas->burbulai, kontekstas->keiciamu_burbulu_sk);
	}

}

/*********************************************************
Skard\xFEi\xF8j\xF8 gars\xF8 ilginimas - i\xF0kvie\xE8iamas nurodytas scenarijus
*********************************************************/
void ilginimo_euristika_skardieji (struct tkontekstas * kontekstas)
{
	// jei tik 0, 1 ar 2 pikai fonemoje, n\xEBra burbulo dubliavimui
	if (kontekstas->piku_sk < 3) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	// nusistatome ribas - nuo kurio iki kurio burbulo galime dubliuoti
	int pirmojo_burbulo_nr = 1;
	int paskutiniojo_burbulo_nr = kontekstas->piku_sk - 2;
	
	// jei n\xEBra burbulo dubliavimui
	if (paskutiniojo_burbulo_nr < pirmojo_burbulo_nr) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	switch (scenarijus) {
		
	case 1:
	case 2:
		// i\xF0renkame vidurin\xE1 burbul\xE0 dubliavimui
		
		ilginimo_euristika_skardieji_scenarijus1 (pirmojo_burbulo_nr, paskutiniojo_burbulo_nr, kontekstas);
		
		break;
		
	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
		ilginimo_euristika_skardieji_scenarijus3 (pirmojo_burbulo_nr, paskutiniojo_burbulo_nr, kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus ma\xFEiau, sprogstamuosius dar ma\xFEiau, r nekeisti)
		ilginimo_euristika_skardieji_scenarijus3 (pirmojo_burbulo_nr, paskutiniojo_burbulo_nr,
			koreguoti_greitinimo_koef_scenarijus4 (kontekstas), kontekstas);
		break;
		
	case 5:
		// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas
		
		ilginimo_euristika_skardieji_scenarijus3 (pirmojo_burbulo_nr, paskutiniojo_burbulo_nr,
			koreguoti_greitinimo_koef_scenarijus5 (kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia, kontekstas), kontekstas);
		break;
		
	default:
		kontekstas->keiciamu_burbulu_sk = 0;
		break;
		
	}
}

/*********************************************************
 *********************************************************
             ILGINIMAS: DUSLIEJI
 *********************************************************
 *********************************************************/

/*********************************************************
Leistinos srities rib\xF8 dusli\xF8j\xF8 gars\xF8 ilginimui nustatymas
*********************************************************/
void leistinos_srities_ribos_dusliuju_ilginimui (size_t fonemos_nr, double * galimos_srities_pradzia, double * galimos_srities_pabaiga)
{
	switch (fonemos[fonemos_nr][0]) {
		
	case 's':
	case 'S':
		*galimos_srities_pradzia = 0.25;
		*galimos_srities_pabaiga = 0.65;
		break;
	case 'f':
		*galimos_srities_pradzia = 0.20;
		*galimos_srities_pabaiga = 0.70;
		break;
	case 'x':
		*galimos_srities_pradzia = 0.40;
		*galimos_srities_pabaiga = 0.70;
		break;
	case 'p':
		*galimos_srities_pradzia = 0.30;
		*galimos_srities_pabaiga = 0.55;
		break;
	case 't':
		if (fonemos[fonemos_nr][1] == 's' || fonemos[fonemos_nr][1] == 'S') {// ts arba tS
			*galimos_srities_pradzia = 0.40;
			*galimos_srities_pabaiga = 0.70;
		} else {// t
			*galimos_srities_pradzia = 0.20;
			*galimos_srities_pabaiga = 0.40;
		}
		break;
	case 'k':
		*galimos_srities_pradzia = 0.20;
		*galimos_srities_pabaiga = 0.45;
		break;
	case '_':
		*galimos_srities_pradzia = 0.10;
		*galimos_srities_pabaiga = 0.90;
		break;
	case 'z':
	case 'Z':
	case 'h':
		// TODO: tai Z duomenys. Kol kas z ir h nepasitaik\xEB visai. 
		// Kai pasitaikys, atitinkamai pakeisti galim\xE0 interval\xE0
		*galimos_srities_pradzia = 0.45;
		*galimos_srities_pabaiga = 0.70;
		break;
	default: // nekeiskim visai
		char klaidos_pranesimas [256];
		sprintf (klaidos_pranesimas, "nezinoma fonema: %s, nr: %d", fonemos[fonemos_nr], fonemos_nr);
		spausdinti_loga (klaidos_pranesimas);
		*galimos_srities_pradzia = 0.50;
		*galimos_srities_pabaiga = 0.50;
		break;
	}
	
}

/*********************************************************
// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
galimos_srities_pradzia - galimos srities prad\xFEios indeksas signalo masyve.
galimos_srities_pabaiga - galimos srities pabaigos indeksas signalo masyve.
Greitinimo koeficientas paduodamas tam, 
kad b\xFBt\xF8 galima i\xF0kviesti 4 scenarijuje su skirtingais greitinimo koeficientais
*********************************************************/
void ilginimo_euristika_duslieji_scenarijus3 (size_t galimos_srities_pradzia, 
											  size_t galimos_srities_pabaiga,
								              double greitinimo_koef, struct tkontekstas * kontekstas)
{
	// reikia 5 scenarijuje
	if (greitinimo_koef <= 1)
	{
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}

	// i\xF0siai\xF0kinkime, kiek reikia pailginti fonem\xE0

	size_t fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	size_t siekiamas_fonemos_ilgis = (size_t) (fonemos_ilgis * greitinimo_koef);
	int siekiamas_pailginimas = siekiamas_fonemos_ilgis - fonemos_ilgis;
	
	int burbulu_sk = 0;

	// ptk ilginame, imdami burbulus i\xF0 pirmos gars\xF8 baz\xEBs fonemos (pauz\xEBs), 
	// tod\xEBl parametrus galimos_srities_pradzia ir galimos_srities_pabaiga
	// nurodome ne einamosios fonemos, o pauz\xEBs,
	// bei kad nukopijuot\xF8 sen\xE0 signal\xE0 iki fonemos leistinos srities vidurio, 
	// suformuojame fiktyv\xF8 nulinio plo\xE8io burbul\xE0 toje vietoje
	
	if (fonemos[kontekstas->fonemos_nr][0] == 'p' || fonemos[kontekstas->fonemos_nr][0] == 'k' || 
		(fonemos[kontekstas->fonemos_nr][0] == 't' && fonemos[kontekstas->fonemos_nr][1] != 's' && fonemos[kontekstas->fonemos_nr][1] != 'S')) {
		
		// kad nukopijuot\xF8 sen\xE0 signal\xE0 iki fonemos leistinos srities vidurio, 
		// suformuojame fiktyv\xF8 nulinio plo\xE8io burbul\xE0 toje vietoje

		// suformuojame fiktyv\xF8 burbul\xE0
		kontekstas->burbulai[burbulu_sk].pradzia = (galimos_srities_pradzia + galimos_srities_pabaiga)/2;
		kontekstas->burbulai[burbulu_sk].pabaiga = kontekstas->burbulai[burbulu_sk].pradzia;
		kontekstas->burbulai[burbulu_sk].vidurys = kontekstas->burbulai[burbulu_sk].pradzia;
		kontekstas->burbulai[burbulu_sk].kartai = 0;

		// padidiname burbul\xF8 skaitliuk\xE0
		burbulu_sk++;

		// su\xFEinome pauz\xEBs galimos srities ribas
		double pauzes_galimos_srities_pradzia = 0;
		double pauzes_galimos_srities_pabaiga = 1;
		leistinos_srities_ribos_dusliuju_ilginimui (0, &pauzes_galimos_srities_pradzia, &pauzes_galimos_srities_pabaiga);
		
		// perskai\xE8iuojame parametrus galimos_srities_pradzia ir galimos_srities_pabaiga, 
		// kad atitikt\xF8 pirm\xE0 gars\xF8 baz\xEBs fonem\xE0 (pauz\xE6)
		galimos_srities_pradzia = (size_t) (pauzes_galimos_srities_pradzia * fonemu_ilgiai[0]);
		galimos_srities_pabaiga = (size_t) (pauzes_galimos_srities_pabaiga * fonemu_ilgiai[0]); 
	}
		
	// Algoritmas: burbulus parenkame atsitiktinai, kol pasiekiame norim\xE0 pailg\xEBjim\xE0. 
	// Paskutin\xE1 burbul\xE0, jei per didelis, suma\xFEiname iki reikiamo ilgio.

	int galimos_srities_ilgis = galimos_srities_pabaiga - galimos_srities_pradzia;
	// kiek dar liko pailginti
	int trukstamas_pailginimas = siekiamas_pailginimas;

	srand((unsigned int)time(NULL));

	while (trukstamas_pailginimas > 0) {
		// atsitiktinai parenkame burbulo ilg\xE1
		size_t burbulo_plotis = (size_t) ((((double)rand())/RAND_MAX)*galimos_srities_ilgis);

		// jei per ilgas burbulas, suma\xFEiname iki reikiamo ilgio
		if (burbulo_plotis/2 > (size_t)trukstamas_pailginimas)
			burbulo_plotis = trukstamas_pailginimas*2;

		// atsitiktinai parenkame burbulo prad\xFEios pozicij\xE0 leistinoje srityje
		size_t burbulo_pradzia = (size_t) ((((double)rand())/RAND_MAX)*(galimos_srities_ilgis-burbulo_plotis));

		// suformuojame burbul\xE0
		kontekstas->burbulai[burbulu_sk].pradzia = galimos_srities_pradzia + burbulo_pradzia;
		kontekstas->burbulai[burbulu_sk].pabaiga = kontekstas->burbulai[burbulu_sk].pradzia + burbulo_plotis;
		kontekstas->burbulai[burbulu_sk].vidurys 
			= (size_t) (kontekstas->burbulai[burbulu_sk].pradzia + kontekstas->burbulai[burbulu_sk].pabaiga)/2;
		kontekstas->burbulai[burbulu_sk].kartai = 1;

		if (debuginam > 100) {
			char pranesimas [1024];
			sprintf (pranesimas, "Euristika.cpp: ilginimo_euristika_duslieji_scenarijus3(): burbulo nr: %d, ilgis: %d/%d, pradzia: %d", burbulu_sk, burbulo_plotis, galimos_srities_ilgis, burbulo_pradzia);
			spausdinti_loga (pranesimas);
		}

		// padidiname burbul\xF8 skaitliuk\xE0
		burbulu_sk++;

		// perskai\xE8iuojame tr\xFBkstam\xE0 pailginim\xE0
		trukstamas_pailginimas -= burbulo_plotis/2;
	}
	
		// \xE1simename sudaryt\xF8 burbul\xF8 skai\xE8i\xF8
		kontekstas->keiciamu_burbulu_sk = burbulu_sk;
	
	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: ilginimo_euristika_duslieji_scenarijus3(): fonemos_ilgis: %d, siekiamas_fonemos_ilgis: %d, siekiamas_pailginimas: %d, keiciamu_burbulu_sk: %d", 
			fonemos_ilgis, siekiamas_fonemos_ilgis, siekiamas_pailginimas, kontekstas->keiciamu_burbulu_sk);
		spausdinti_loga (pranesimas);
		//spausdinti_burbulus (kontekstas->burbulai, kontekstas->keiciamu_burbulu_sk);
	}

}

/*********************************************************
Dusli\xF8j\xF8 gars\xF8 ilginimas - i\xF0kvie\xE8iamas nurodytas scenarijus
*********************************************************/
void ilginimo_euristika_duslieji (struct tkontekstas * kontekstas)
{
	// Duslieji garsai, kuri\xF8 trumpinimui nenaudojame pik\xF8 informacijos.
	// Sudarome 1 burbul\xE0, kur\xE1 reik\xEBs i\xF0kirpti.
	// TODO: Burbulo dydis ir pad\xEBtis turi priklausyti nuo:
	// 1) trumpinimo koeficiento,
	// 2) (galb\xFBt?) fonemos pavadinimo,
	// 3) fonemos rib\xF8 (pvz, burbulas negali prasid\xEBti ar\xE8iau fonemos prad\xFEios kaip 20% jos ilgio 
	//		ir negali pasibaigti ar\xE8iau fonemos pabaigos kaip 70% jos ilgio. 
	//		Ribos gali priklausyti nuo fonemos pavadinimo).
	
	// nusistatome ribas - nuo kur iki kur galime dubliuoti
	double galimos_srities_pradzia = 0;
	double galimos_srities_pabaiga = 1;
	
	leistinos_srities_ribos_dusliuju_ilginimui (kontekstas->fonemos_nr, &galimos_srities_pradzia, &galimos_srities_pabaiga);

	switch (scenarijus) {
		
	case 1:
	case 2:
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// suformuojame burbul\xE0
		kontekstas->burbulai[0].pradzia = kontekstas->fonemos_pradzia + (size_t) (0.30 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].vidurys = kontekstas->fonemos_pradzia + (size_t) (0.40 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].pabaiga = kontekstas->fonemos_pradzia + (size_t) (0.50 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].kartai = 1;
		//kontekstas->burbulai[0].pikai = 0;
		
		break;
		
	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
		ilginimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus ma\xFEiau, sprogstamuosius dar ma\xFEiau, r nekeisti)
		ilginimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			koreguoti_greitinimo_koef_scenarijus4 (kontekstas), kontekstas);
		break;
		
	case 5:
		// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas
		
		ilginimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			koreguoti_greitinimo_koef_scenarijus5 (kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia, kontekstas), kontekstas);
		break;
		
	default:
		kontekstas->keiciamu_burbulu_sk = 0;
		break;
	}
}

/*********************************************************
 *********************************************************
             TRUMPINIMAS: DUSLIEJI
 *********************************************************
 *********************************************************/

/*********************************************************
// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef).
galimos_srities_pradzia - galimos srities prad\xFEios indeksas signalo masyve.
galimos_srities_pabaiga - galimos srities pabaigos indeksas signalo masyve.
Greitinimo koeficientas paduodamas tam, 
kad b\xFBt\xF8 galima i\xF0kviesti 4 scenarijuje su skirtingais greitinimo koeficientais
*********************************************************/
void trumpinimo_euristika_duslieji_scenarijus3 (size_t galimos_srities_pradzia, 
											  size_t galimos_srities_pabaiga,
											  double greitinimo_koef, struct tkontekstas * kontekstas)
{
	// reikia 5 scenarijuje
	if (greitinimo_koef >= 1)
	{
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}

	// i\xF0siai\xF0kinkime, kiek reikia patrumpinti fonem\xE0

	size_t fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	size_t siekiamas_fonemos_ilgis = (size_t) (fonemos_ilgis * greitinimo_koef);
	size_t siekiamas_patrumpinimas = fonemos_ilgis - siekiamas_fonemos_ilgis;

	// randame leistinosios srities ilg\xE1

	size_t galimos_srities_centras = (galimos_srities_pradzia + galimos_srities_pabaiga)/2;
	size_t galimos_srities_ilgis = galimos_srities_pabaiga - galimos_srities_pradzia;

	// jei i\xF0saugoti reikia vis\xE0 leistin\xE0j\xE0 srit\xE1, nieko nedarome
	if (galimos_srities_ilgis <= ISSAUGOTI_GALIMOS_SRITIES_ILGIO) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}

	// Paskai\xE8iuokime, kiek galime patrumpinti fonem\xE0.

	size_t galimas_patrumpinimas = galimos_srities_ilgis - ISSAUGOTI_GALIMOS_SRITIES_ILGIO;

	size_t busimas_patrumpinimas = 0;
	if (siekiamas_patrumpinimas > galimas_patrumpinimas)
		busimas_patrumpinimas = galimas_patrumpinimas;
	else
		busimas_patrumpinimas = siekiamas_patrumpinimas;
	
	// Jei trumpinti nedaug, sudarysime vien\xE0 burbul\xE0.
	// Jei daugiau, sudarysime grandin\xE6 i\xF0 2 burbul\xF8.

	if (busimas_patrumpinimas <= ISSAUGOTI_GALIMOS_SRITIES_ILGIO) {
		
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// suformuojame burbul\xE0
		kontekstas->burbulai[0].pradzia = galimos_srities_centras - busimas_patrumpinimas;
		kontekstas->burbulai[0].vidurys = galimos_srities_centras;
		kontekstas->burbulai[0].pabaiga = galimos_srities_centras + busimas_patrumpinimas;
		kontekstas->burbulai[0].kartai = 1;
		
	} else {
		
		kontekstas->keiciamu_burbulu_sk = 2;

		size_t burbulu_srities_ilgis = busimas_patrumpinimas + ISSAUGOTI_GALIMOS_SRITIES_ILGIO;
		size_t burbulu_srities_pradzia = galimos_srities_centras - burbulu_srities_ilgis/2;
		
		// suformuojame burbulus

		// pirmojo burbulo kair\xEBs plotis = ISSAUGOTI_GALIMOS_SRITIES_ILGIO
		kontekstas->burbulai[0].pradzia = burbulu_srities_pradzia;
		kontekstas->burbulai[0].vidurys = burbulu_srities_pradzia + ISSAUGOTI_GALIMOS_SRITIES_ILGIO;
		kontekstas->burbulai[0].pabaiga = burbulu_srities_pradzia + busimas_patrumpinimas;
		kontekstas->burbulai[0].kartai = 1;
		
		// antrojo burbulo de\xF0in\xEBs plotis = ISSAUGOTI_GALIMOS_SRITIES_ILGIO
		kontekstas->burbulai[1].pradzia = kontekstas->burbulai[0].vidurys;
		kontekstas->burbulai[1].vidurys = kontekstas->burbulai[0].pabaiga;
		kontekstas->burbulai[1].pabaiga = burbulu_srities_pradzia + burbulu_srities_ilgis;
		kontekstas->burbulai[1].kartai = 1;
		
	}
}

/*********************************************************
Leistinos srities rib\xF8 dusli\xF8j\xF8 gars\xF8 trumpinimui nustatymas
*********************************************************/
void leistinos_srities_ribos_dusliuju_trumpinimui (size_t fonemos_nr, double * galimos_srities_pradzia, double * galimos_srities_pabaiga)
{
	// kol kas nesiskiria nuo leistinosios srities rib\xF8 dusli\xF8j\xF8 gars\xF8 ilginimui,
	// ta\xE8iau jei reik\xEBs, kad skirt\xF8si, galima bus pakeisti
	leistinos_srities_ribos_dusliuju_ilginimui (fonemos_nr, galimos_srities_pradzia, galimos_srities_pabaiga);
}

/*********************************************************
Dusli\xF8j\xF8 gars\xF8 trumpinimas - i\xF0kvie\xE8iamas nurodytas scenarijus
*********************************************************/
void trumpinimo_euristika_duslieji (struct tkontekstas * kontekstas)
{
	// Duslieji garsai, kuri\xF8 trumpinimui nenaudojame pik\xF8 informacijos.
	// Sudarome 1 burbul\xE0, kur\xE1 reik\xEBs i\xF0kirpti.
	// TODO: Burbulo dydis ir pad\xEBtis turi priklausyti nuo:
	// 1) trumpinimo koeficiento,
	// 2) (galb\xFBt?) fonemos pavadinimo,
	// 3) fonemos rib\xF8 (pvz, burbulas negali prasid\xEBti ar\xE8iau fonemos prad\xFEios kaip 20% jos ilgio 
	//		ir negali pasibaigti ar\xE8iau fonemos pabaigos kaip 70% jos ilgio. 
	//		Ribos gali priklausyti nuo fonemos pavadinimo).
	
	// nusistatome ribas - nuo kur iki kur galime "\xF0alinti"
	double galimos_srities_pradzia = 0;
	double galimos_srities_pabaiga = 1;
	
	leistinos_srities_ribos_dusliuju_trumpinimui (kontekstas->fonemos_nr, &galimos_srities_pradzia, &galimos_srities_pabaiga);

	switch (scenarijus) {
		
	case 1:
	case 2:
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// suformuojame burbul\xE0
		kontekstas->burbulai[0].pradzia = kontekstas->fonemos_pradzia + (size_t) (0.30 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].vidurys = kontekstas->fonemos_pradzia + (size_t) (0.40 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].pabaiga = kontekstas->fonemos_pradzia + (size_t) (0.50 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].kartai = 1;
		//kontekstas->burbulai[0].pikai = 0;
		
		break;

	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
		trumpinimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus ma\xFEiau, sprogstamuosius dar ma\xFEiau, r nekeisti)
		trumpinimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			koreguoti_greitinimo_koef_scenarijus4 (kontekstas), kontekstas);
		break;
		
	case 5:
		// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas
		
		trumpinimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			koreguoti_greitinimo_koef_scenarijus5 (kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia, kontekstas), kontekstas);
		break;
		
	default:
		kontekstas->keiciamu_burbulu_sk = 0;
		break;
		
	}
}

/*********************************************************
 *********************************************************
             TRUMPINIMAS: SKARDIEJI
 *********************************************************
 *********************************************************/

/*********************************************************
// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
Greitinimo koeficientas paduodamas tam, 
kad b\xFBt\xF8 galima i\xF0kviesti 4 scenarijuje su skirtingais greitinimo koeficientais
*********************************************************/
void trumpinimo_euristika_skardieji_scenarijus3 (int pirmojo_burbulo_nr, 
								   int paskutiniojo_burbulo_nr,
								   double greitinimo_koef, struct tkontekstas * kontekstas)
{
	// reikia 5 scenarijuje
	if (greitinimo_koef >= 1)
	{
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}

	// i\xF0siai\xF0kinkime, kiek reikia patrumpinti fonem\xE0

	int fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	int siekiamas_fonemos_ilgis = (int) (fonemos_ilgis * greitinimo_koef);
	int siekiamas_patrumpinimas = fonemos_ilgis - siekiamas_fonemos_ilgis;

	// Algoritmo tikslas: i\xF0mesti burbulus taip, kad net ir i\xF0metus nema\xFEai burbul\xF8, vis tiek likt\xF8 burbul\xF8 i\xF0 fonemos vidurio, ne tik i\xF0 kra\xF0t\xF8.
	//
	// Algoritmo id\xEBja: pradedant nuo vidurini\xF8 burbul\xF8, i\xF0metin\xEBti kas antr\xE0 burbul\xE0. Likusius i\xF0metin\xEBti, pradedant nuo kra\xF0tini\xF8.
	//
	// Detalesn\xEB algoritmo id\xEBja: 
	// pirmame etape i\xF0metin\xEBsime nelyginius burbulus nuo vidurini\xF8 link kra\xF0tini\xF8, 
	// antrame - lyginius nuo kra\xF0tini\xF8 link vidurini\xF8.
	// (laikome, kad pirmas burbulas yra pirmojo_burbulo_nr)
	//
	// Algoritmo realizacija:
	//
	// I etapas.
	//
	// Nelygini\xF8 skai\xE8i\xF8 sekos 1, 3, 5, ..., 2k-1 i\xF0rikiavim\xE0 nuo vidurini\xF8 nari\xF8 suvedame 
	// \xE1 skai\xE8i\xF8 sekos 1, 2, 3, ..., k i\xF0rikiavim\xE0 nuo vidurini\xF8 nari\xF8.
	// Pa\xFEym\xEBkime v:=[(k+1)/2] - vidurinis (arba vienas i\xF0 vidurini\xF8) sekos 1, 2, 3, ..., k narys. 
	// Tada vienas i\xF0 galim\xF8 i\xF0rikiavim\xF8 nuo vidurini\xF8 nari\xF8 b\xFBd\xF8 b\xFBt\xF8 toks:
	// v, v+1, v-1, v+2, v-2, ... (kol panaudosime visus k nari\xF8).
	// Tokiai sekai konstruoti sugalvojau tok\xE1 rekursyv\xF8 b\xFBd\xE0:
	// a_0 = v, a_i = a_{i-1} + b_i * i, kur
	// b_0 = -1; b_i = (-1) * b_{i-1}.
	//
	// II etapas.
	// 
	// Lygini\xF8 skai\xE8i\xF8 sekos 2, 4, 6, ..., 2k i\xF0rikiavim\xE0 nuo kra\xF0tini\xF8 nari\xF8 suvedame 
	// \xE1 skai\xE8i\xF8 sekos 1, 2, 3, ..., k i\xF0rikiavim\xE0 nuo kra\xF0tini\xF8 nari\xF8.
	// Tada vienas i\xF0 galim\xF8 i\xF0rikiavim\xF8 nuo kra\xF0tini\xF8 nari\xF8 b\xFBd\xF8 b\xFBt\xF8 toks:
	// 1, k, 2, k-1, ... (kol panaudosime visus k nari\xF8).
	// Tokiai sekai konstruoti sugalvojau tok\xE1 rekursyv\xF8 b\xFBd\xE0:
	// a_0 = 1, a_i = a_{i-1} + b_i * (k-i), kur
	// b_0 = -1; b_i = (-1) * b_{i-1}.
	// 
	// Bet mums neu\xFEtenka sudaryti burbul\xF8 sek\xE0, reikia dar, kad burbulai masyve b\xFBt\xF8 i\xF0rikiuoti nuo pirmiausio iki paskutinio.
	// Tai realizuosime taip: pirma \xFEym\xEBsim\xEBs \xF0alinam\xF8 burbul\xF8 numerius specialiame pagalbiniame masyve
	// (prad\xFEioje visi masyvo elementai = 0. Jei \xF0alinsime i-t\xE0j\xE1 burbul\xE0, i-1-\xE0j\xE1 masyvo element\xE0 kei\xE8iame \xE1 1), 
	// kol pasieksime reikiam\xE0 patrump\xEBjim\xE0. 
	// Tik tada suformuosime tuos burbulus burbul\xF8 masyve, kuri\xF8 reik\xF0m\xEBs specialiame masyve lygios 1.


	// maksimalus burbul\xF8, kuriuos galime "i\xF0mesti", skai\xE8ius
	int max_burbulu_sk = paskutiniojo_burbulo_nr - pirmojo_burbulo_nr + 1;

	// \xF0alinam\xF8 burbul\xF8 numeri\xF8 masyvas
	// (prad\xFEioje visi masyvo elementai = 0. Jei \xF0alinsime i-t\xE0j\xE1 burbul\xE0, i-1-\xE0j\xE1 masyvo element\xE0 kei\xE8iame \xE1 1)
	int * salinami_burbulai = (int *) calloc (max_burbulu_sk, sizeof(int));

	// kiek patrump\xEBt\xF8, jei "i\xF0mestume" tokius burbulus
	int busimas_patrumpinimas = 0;
	int busimas_patrumpinimas_old = 0; // \xE1simename paskutinio burbulo tikrinimui

	// \xE1simename paskutin\xE1 burbul\xE0, kad gal\xEBtume prireikus jo atsisakyti (numeruojame nuo 1 iki max_burbulu_sk)
	int burb = 0;
	int pik = 0;

	// I etapas

	// Nelyginio numerio burbul\xF8 skai\xE8ius (jei numeruojame juos nuo 1 iki max_burbulu_sk)
	int k = (max_burbulu_sk + 1)/2;

	// Pa\xFEym\xEBkime v:=[(k+1)/2] - vidurinis (arba vienas i\xF0 vidurini\xF8) sekos 1, 2, 3, ..., k narys. 
	int v = (k+1)/2;

	// i\xF0metin\xEBjame nelyginius burbulus nuo vidurinio, kol pasieksime siekiam\xE0 patrumpinim\xE0, arba kol panaudosime visus sekos narius

	int i = 0;
	int a = v;
	int b = -1;
	if (debuginam > 200) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): I etapas: max_burbulu_sk=%d, k=%d, i=%d, a=%d, b=%d, burb=%d, pik=%d, busimas_patrumpinimas=%d", 
			max_burbulu_sk, k, i, a, b, burb, pik, busimas_patrumpinimas);
		spausdinti_loga (pranesimas);
	}
	while (busimas_patrumpinimas < siekiamas_patrumpinimas && i < k)
	{
		a = a + b * i;
		b = -b;

		// \xF0alinsime burbul\xE0 2a-1
		//burb_old = burb;
		burb = 2*a - 1;
		salinami_burbulai [burb - 1] = 1;
		// burbulo numeris, i\xF0reik\xF0tas viso signalo (ne vienos fonemos) pik\xF8 numeriais 
		pik = kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr - 1 + burb;

		// kiek patrump\xEBt\xF8, jei "i\xF0mestume" tok\xE1 burbul\xE0
		busimas_patrumpinimas_old = busimas_patrumpinimas;
		busimas_patrumpinimas += pikai [pik+1] - pikai [pik-1] - (pikai [pik+1] - pikai [pik-1])/2;

		i++;

		if (debuginam > 200) {
			char pranesimas [1024];
			sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): I etapas: i=%d, a=%d, b=%d, burb=%d, pik=%d, busimas_patrumpinimas=%d", 
				i, a, b, burb, pik, busimas_patrumpinimas);
			spausdinti_loga (pranesimas);
		}
	}

	// II etapas

	// Lyginio numerio burbul\xF8 skai\xE8ius (jei numeruojame juos nuo 1 iki max_burbulu_sk)
	k = max_burbulu_sk/2;

	i = 0;
	a = k+1;
	b = -1;
	if (debuginam > 200) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): II etapas: max_burbulu_sk=%d, k=%d, i=%d, a=%d, b=%d, burb=%d, pik=%d, busimas_patrumpinimas=%d", 
			max_burbulu_sk, k, i, a, b, burb, pik, busimas_patrumpinimas);
		spausdinti_loga (pranesimas);
	}
	while (busimas_patrumpinimas < siekiamas_patrumpinimas && i < k)
	{
		a = a + b * (k-i);
		b = -b;

		// \xF0alinsime burbul\xE0 2a
		//burb_old = burb;
		burb = 2*a;
		salinami_burbulai [burb - 1] = 1;
		// burbulo numeris, i\xF0reik\xF0tas viso signalo (ne vienos fonemos) pik\xF8 numeriais 
		pik = kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr - 1 + burb;

		// kiek patrump\xEBt\xF8, jei "i\xF0mestume" tok\xE1 burbul\xE0
		busimas_patrumpinimas_old = busimas_patrumpinimas;
		busimas_patrumpinimas += pikai [pik+1] - pikai [pik-1] - (pikai [pik+1] - pikai [pik-1])/2; // tai da\xFEniausiai tikslus patrumpinimas, bet kartais gali skirtis per 1 ar -1, \xFEr. 2015-03-27 u\xFEra\xF0us 

		i++;

		if (debuginam > 200) {
			char pranesimas [1024];
			sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): II etapas: i=%d, a=%d, b=%d, burb=%d, pik=%d, busimas_patrumpinimas=%d", 
				i, a, b, burb, pik, busimas_patrumpinimas);
			spausdinti_loga (pranesimas);
		}
	}

	// i\xF0siai\xF0kinkime, ar paskutinis burbulas tikrai reikalingas: 
	// paskutin\xE1 burbul\xE0 imame tik tuo atveju, jei j\xE1 imant paklaida bus ma\xFEesn\xEB, negu neimant

	if (burb != 0 && busimas_patrumpinimas - siekiamas_patrumpinimas > siekiamas_patrumpinimas - busimas_patrumpinimas_old) {
		// paskutinio burbulo atsisakome, perskai\xE8iuojame reik\xF0mes
		salinami_burbulai [burb - 1] = 0;
		busimas_patrumpinimas -= pikai [pik+1] - pikai [pik-1] - (pikai [pik+1] - pikai [pik-1])/2;
		if (debuginam > 200) {
			char pranesimas [1024];
			sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): atsisakome paskutinio burbulo: burb=%d, pik=%d, busimas_patrumpinimas=%d", 
				burb, pik, busimas_patrumpinimas);
			spausdinti_loga (pranesimas);
		}
	}

	// suformuojame nurodytus burbulus
	for (i=0; i<max_burbulu_sk; i++)
		if (salinami_burbulai [i] == 1) {
			pik = kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr + i;
			suformuoti_nurodytus_burbulus (pik, pik, 1, kontekstas);
		}

	free (salinami_burbulai);

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): fonemos_ilgis: %d, siekiamas_fonemos_ilgis: %d, siekiamas_patrumpinimas: %d, busimas_patrumpinimas: %d", 
			fonemos_ilgis, siekiamas_fonemos_ilgis, siekiamas_patrumpinimas, busimas_patrumpinimas);
		spausdinti_loga (pranesimas);
	}

}

/*********************************************************
Skard\xFEi\xF8j\xF8 gars\xF8 trumpinimas - i\xF0kvie\xE8iamas nurodytas scenarijus
*********************************************************/
void trumpinimo_euristika_skardieji (struct tkontekstas * kontekstas)
{
	if (debuginam && !reguliarus_pikai (kontekstas)) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji(): !!! Nereguliarus pikai: %s, nr: %d, piku_sk: %d", 
			fonemos[kontekstas->fonemos_nr], kontekstas->fonemos_nr, kontekstas->piku_sk);
		spausdinti_loga (pranesimas);
	}
	
	// Skardieji garsai, kuri\xF8 trumpinimui naudojame pik\xF8 informacij\xE0.
	// Kol kas parinksime vidurin\xE1 burbul\xE0, kur\xE1 reik\xEBs i\xF0kirpti.
	// TODO: padaryti priklausomyb\xE6 nuo trumpinimo koeficiento.
	
	// jei tik 0, 1 ar 2 pikai fonemoje, n\xEBra burbulo \xF0alinimui
	if (kontekstas->piku_sk < 3) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	// randame pirmojo burbulo, kur\xE1 galima pa\xF0alinti, numer\xE1
	
	// numeris pirmojo burbulo fonemoje, kur\xE1 galima pa\xF0alinti. Numeruojama nuo nulio.
	// Burbulo numeris sutampa su burbulo vidurinio piko numeriu,
	// t.y. burbulas i yra tas, 
	// kurio vidurinis pikas yra pirmojo_piko_nr + i.
	// Nulinio burbulo vidurinis pikas yra pirmojo_piko_nr, 
	// tod\xEBl nulinis burbulas visada kirs fonemos rib\xE0 
	// ir jo d\xEBl to niekada negal\xEBsime \xF0alinti.
	int pirmojo_burbulo_nr = 0;
	
	// jei nuo pirmojo piko iki fonemos prad\xFEios lieka ma\xFEiau (arba lygu) 
	// kaip pus\xEB pirmojo periodo ilgio (atstumas tarp pirm\xF8j\xF8 pik\xF8), 
	// tai pirmojo burbulo negalime \xF0alinti, nes jo prad\xFEia labai arti fonemos prad\xFEios
	// (galime \xF0alinti nuo antrojo),
	// prie\xF0ingu atveju galime \xF0alinti jau ir pirm\xE0j\xE1.
	if (2 * (pikai [kontekstas->pirmojo_piko_nr] - kontekstas->fonemos_pradzia) <= 
		pikai [kontekstas->pirmojo_piko_nr+1] - pikai [kontekstas->pirmojo_piko_nr])
		pirmojo_burbulo_nr = 2;
	else
		pirmojo_burbulo_nr = 1;
	
	// analogi\xF0kai randame paskutiniojo burbulo, kur\xE1 galima pa\xF0alinti, numer\xE1
	
	int paskutiniojo_burbulo_nr = 0;
	int paskutiniojo_piko_nr = kontekstas->pirmojo_piko_nr+kontekstas->piku_sk-1;
	if (2 * (kontekstas->fonemos_pabaiga - pikai [paskutiniojo_piko_nr]) <= 
		pikai [paskutiniojo_piko_nr] - pikai [paskutiniojo_piko_nr-1])
		paskutiniojo_burbulo_nr = kontekstas->piku_sk - 3;
	else
		paskutiniojo_burbulo_nr = kontekstas->piku_sk - 2;
	
	// jei n\xEBra burbulo \xF0alinimui
	if (paskutiniojo_burbulo_nr < pirmojo_burbulo_nr) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	int vidurinio_burbulo_nr = 0;

	switch (scenarijus) {
		
	case 1:
		// i\xF0renkame vidurin\xE1 burbul\xE0 \xF0alinimui
		
		// vidurinio burbulo numeris (numeruojant nuo 0, skai\xE8iuojant nuo pirmojo_burbulo_nr)
		vidurinio_burbulo_nr = kontekstas->pirmojo_piko_nr 
			+ (pirmojo_burbulo_nr + paskutiniojo_burbulo_nr)/2;
		
		// suformuojame nurodytus burbulus
		suformuoti_nurodytus_burbulus (
			vidurinio_burbulo_nr, vidurinio_burbulo_nr, 1, kontekstas);

		break;
	case 2:
		// \xF0alinimui paskiriame visus galimus burbulus
		
		// suformuojame nurodytus burbulus
		suformuoti_nurodytus_burbulus (kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr, 
								   kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr, 1, kontekstas);

		break;
		
	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
		trumpinimo_euristika_skardieji_scenarijus3 (
			pirmojo_burbulo_nr, paskutiniojo_burbulo_nr, kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus ma\xFEiau, sprogstamuosius dar ma\xFEiau, r nekeisti)
		trumpinimo_euristika_skardieji_scenarijus3 (
			pirmojo_burbulo_nr, paskutiniojo_burbulo_nr,  
			koreguoti_greitinimo_koef_scenarijus4 (kontekstas), kontekstas);
		break;
		
	case 5:
		// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas
		
		trumpinimo_euristika_skardieji_scenarijus3 (
			pirmojo_burbulo_nr, paskutiniojo_burbulo_nr,
			koreguoti_greitinimo_koef_scenarijus5 (kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia, kontekstas), kontekstas);
		break;
		
	default:
		kontekstas->keiciamu_burbulu_sk = 0;
		break;
				
	}
	
}

/*********************************************************
 *********************************************************
             PAGRINDIN\xCBS FUNKCIJOS
 *********************************************************
 *********************************************************/

/*********************************************************
Dusli\xF8j\xF8 gars\xF8 apdorojimas - i\xF0kvie\xE8iama trumpinimo ar ilginimo funkcija, 
priklausomai nuo greitinimo koeficiento
*********************************************************/
void euristika_duslieji (struct tkontekstas * kontekstas)
{
	if (kontekstas->greitinimo_koef < 1)
		trumpinimo_euristika_duslieji (kontekstas);
	else if (kontekstas->greitinimo_koef == 1)
	{
		kontekstas->keiciamu_burbulu_sk = 0;
	} 
	else
		ilginimo_euristika_duslieji (kontekstas);
}

/*********************************************************
Skard\xFEi\xF8j\xF8 gars\xF8 apdorojimas - i\xF0kvie\xE8iama trumpinimo ar ilginimo funkcija, 
priklausomai nuo greitinimo koeficiento
*********************************************************/
void euristika_skardieji (struct tkontekstas * kontekstas)
{
	if (kontekstas->greitinimo_koef < 1)
		trumpinimo_euristika_skardieji (kontekstas);
	else if (kontekstas->greitinimo_koef == 1)
	{
		kontekstas->keiciamu_burbulu_sk = 0;
	} 
	else
		ilginimo_euristika_skardieji (kontekstas);
}

/*********************************************************
Pagrindin\xEB euristikos funkcija.
Parenka burbulus signalo trumpinimui (kalb\xEBjimo tempo greitinimui).
Bandomos \xE1vairios euristikos.
Svarbu: 
1) Gr\xE0\xFEina burbulus, i\xF0rikiuotus eil\xEBs tvarka (!!!).
2) Gr\xE0\xFEinti burbulai gali persidengti tik puse burbulo 
(kaip pik\xF8 pagrindu suformuot\xF8 burbul\xF8 atveju).
//3) Vis\xF8 gr\xE0\xFEint\xF8 burbul\xF8 reik\xF0m\xEBs "pikai" yra vienodos 
//(t.y. arba visi burbulai suformuoti pik\xF8 pagrindu, arba n\xEB vieno).
*********************************************************/
void euristika (struct tkontekstas * kontekstas)
{
	/* Algoritmas.
		Jei ne x, f, p, t, k, s, S, _, r, R, z, Z, H - daryti pagal pikus. 
			Tikrinti, ar yra i\xF0metam\xF8 burbul\xF8 (pagal pik\xF8 skai\xE8i\xF8 ir burbul\xF8 centrus).
			Jei yra - i\xF0mesti, jei n\xEBra - nedaryti nieko.
		Jei x, f, p, t, k, s, S, _ - daryti be pik\xF8 
			(parinkti ir i\xF0mesti gabaliuk\xE0 i\xF0 vidurio, link prad\xFEios).
		Jei r, R - nedaryti nieko.
		Jei z, Z, h - tikrinti, ar taisyklingai pasiskirst\xE6 pikai. 
			Jei taip, naudoti pikus, 
			jei tr\xFBksta tokio piko, pa\xE8iam prid\xEBti, 
			o jei ma\xFEai pik\xF8, tai daryti ne pagal pikus.*/
	
	switch (kontekstas->fonemos_klase) {

	case FONEMU_KLASE_SKARDIEJI:
		// Skardieji garsai, kuri\xF8 trumpinimui naudojame pik\xF8 informacij\xE0.
	case FONEMU_KLASE_RR:
		// Fonemas r, R, jei reikia, trumpiname/ilginame, naudodami pik\xF8 informacij\xE0 (jei jos yra pakankamai. Jei ne - fonemos nekei\xE8iame).
		euristika_skardieji (kontekstas);
		break;

	case FONEMU_KLASE_DUSLIEJI:
		// Duslieji garsai, kuri\xF8 trumpinimui nenaudojame pik\xF8 informacijos.
		euristika_duslieji (kontekstas);
		break;

//	case FONEMU_KLASE_RR:
		// Garso 'r' nemodifikuojame visai.
//		kontekstas->keiciamu_burbulu_sk = 0;
//		break;

	default:
		// Dar nenustatyta fonemos klas\xEB. Tokio atvejo netur\xEBt\xF8 b\xFBti, o jei yra, rei\xF0kia, ka\xFEkas negerai su realizacija, "internal error". K\xE0 daryti tokiu atveju? Kol kas nekei\xE8iame visai.
		kontekstas->keiciamu_burbulu_sk = 0;
		break;
	}
}

