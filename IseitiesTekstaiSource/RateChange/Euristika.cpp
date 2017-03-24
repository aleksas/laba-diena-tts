///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas Euristika.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// Euristinës funkcijos: 
// funkcijos, kurios parenka, kuriuos burbulus reikëtø paðalinti ar dubliuoti. 
// Jos gali ágyvendinti ávairias strategijas.
//

#include "stdafx.h"

/*********************************************************
 *********************************************************
             PAGALBINËS FUNKCIJOS
 *********************************************************
 *********************************************************/

/*********************************************************
Nustato, ar duotoje fonemoje pikai pasiskirstæ reguliariai. 
Tikrina tik tai, ar ne per retai, t.y. ar netrûksta kokio piko.
*********************************************************/
bool reguliarus_pikai (struct tkontekstas * kontekstas)
{
	// jei iðvis nëra pikø
	if (kontekstas->piku_sk <= 0)
		return false;
	
	// jei pernelyg didelis atstumas nuo fonemos pradþios iki pirmojo piko
	if (pikai [kontekstas->pirmojo_piko_nr] - kontekstas->fonemos_pradzia > MAX_ATSTUMAS_TARP_PIKU)
		return false;
	
	// jei pernelyg didelis atstumas nuo paskutinio piko iki fonemos pabaigos
	if (kontekstas->fonemos_pabaiga - pikai [kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1] > MAX_ATSTUMAS_TARP_PIKU)
		return false;
	
	// jei pernelyg dideli atstumai tarp pikø
	for (unsigned int i = 0; i < kontekstas->piku_sk - 1; i++) {
		if (pikai[kontekstas->pirmojo_piko_nr + i + 1] - pikai[kontekstas->pirmojo_piko_nr + i] > MAX_ATSTUMAS_TARP_PIKU)
			return false;
	}
	
	return true;
}

/*********************************************************
Uþpildo informacijà apie nurodytus burbulus.
Burbulø numeriai turi bûti iðreikðti viso signalo (ne vienos fonemos) pikø numeriais
*********************************************************/
void suformuoti_nurodytus_burbulus (int pirmojo_burbulo_nr, 
								    int paskutiniojo_burbulo_nr, int pakartojimu_skaicius, struct tkontekstas * kontekstas)
{
		int keiciamu_burbulu_sk = paskutiniojo_burbulo_nr - pirmojo_burbulo_nr + 1;
		
		if (kontekstas->keiciamu_burbulu_sk + keiciamu_burbulu_sk > MAX_KEICIAMI_BURBULAI)
		{
			// TODO: geriau kaþkà daryti, padidinti burbulai masyvà
			//fprintf (stderr, "Klaida: programa nesusidoroja, padidinti MAX_KEICIAMI_BURBULAI parametra");

			char klaidos_pranesimas [256];
			sprintf (klaidos_pranesimas, 
				"programa nesusidoroja, padidinti MAX_KEICIAMI_BURBULAI parametra: dabar yra %d, o turi buti bent %d", 
				MAX_KEICIAMI_BURBULAI, kontekstas->keiciamu_burbulu_sk + keiciamu_burbulu_sk);
			spausdinti_loga (klaidos_pranesimas);
			exit (1);
		}
		
		for (int burb_nr = 0; burb_nr < keiciamu_burbulu_sk; burb_nr++) {
			// suformuojame burbulà
			int burbulo_vidurinio_piko_nr = pirmojo_burbulo_nr + burb_nr;
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].pradzia = pikai [burbulo_vidurinio_piko_nr - 1];
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].vidurys = pikai [burbulo_vidurinio_piko_nr];
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].pabaiga = pikai [burbulo_vidurinio_piko_nr + 1];
			kontekstas->burbulai[kontekstas->keiciamu_burbulu_sk + burb_nr].kartai = pakartojimu_skaicius;
		}

		kontekstas->keiciamu_burbulu_sk += keiciamu_burbulu_sk;
}

/*********************************************************
Koreguoja greièio koeficientà, priklausomai nuo fonemos pavadinimo 
(balsiams nekeièia, kitiems sumaþina, sprogstamiesiems dar labiau sumaþina)
*********************************************************/
double koreguoti_greitinimo_koef_scenarijus4 (struct tkontekstas * kontekstas)
{
	// parodo, kiek patrumpëjimas/pailgëjimas bus maþesnis uþ standartiná
	// pvz. 0.5 reiðkia, kad pailgës/patrumpës 50% standartinio 
	double koregavimo_koeficientas = 1;

	switch (fonemos[kontekstas->fonemos_nr][0]) {
	// balsiams nekeièiame	
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
Koreguoja greièio koeficientà, priklausomai nuo fonemos ilgio skirtumo nuo vidutinio ilgio 
(patempia link vidutinio ilgio priklausomai nuo scenarijaus5_koeficientas reikðmës)
*********************************************************/
double koreguoti_greitinimo_koef_scenarijus5 (int fonemos_ilgis, struct tkontekstas * kontekstas)
{
	// nustatykime, koks yra vidutinis tokios fonemos ilgis

	// ieðkome einamosios fonemos skirtingø fonemø masyve
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

	// iðsiaiðkinkime, kiek reikia pailginti fonemà

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
		// iðrenkame viduriná burbulà dubliavimui
		
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// vidurinio burbulo numeris (numeruojant nuo 0, skaièiuojant nuo pirmojo_burbulo_nr)
		int vidurinio_burbulo_nr = pirmojo_burbulo_nr 
			+ (paskutiniojo_burbulo_nr - pirmojo_burbulo_nr)/2;
		
		// suformuojame burbulà
		int burbulo_vidurinio_piko_nr = kontekstas->pirmojo_piko_nr + vidurinio_burbulo_nr;
		kontekstas->burbulai -> pradzia = pikai [burbulo_vidurinio_piko_nr - 1];
		kontekstas->burbulai -> vidurys = pikai [burbulo_vidurinio_piko_nr];
		kontekstas->burbulai -> pabaiga = pikai [burbulo_vidurinio_piko_nr + 1];
		kontekstas->burbulai -> kartai = 1;	
}

/*********************************************************
// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
Greitinimo koeficientas paduodamas tam, 
kad bûtø galima iðkviesti 4 scenarijuje su skirtingais greitinimo koeficientais
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

	// iðsiaiðkinkime, kiek reikia pailginti fonemà

	int fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	int siekiamas_fonemos_ilgis = (int) (fonemos_ilgis * greitinimo_koef);
	int siekiamas_pailginimas = siekiamas_fonemos_ilgis - fonemos_ilgis;

	// Algoritmas: dubliuosime kiekvienà burbulà po kaþkiek kartø (nulá arba daugiau), 
	// plius vidurinius (ið eilës einanèius) burbulus dar vienu kartu daugiau, kad pailgintume tiek, kiek reikia.
	// 1. Nustatome, kiek pailgëtø, jei panaudotume visus burbulus. 
	// 2. Paskaièiuojame, kiek kartø reikia panaudoti kiekvienà burbulà. 
	// 3. Paskaièiuojame, kuriuos vidurinius burbulus reiks dubliuoti papildomai.
	// Kadangi gauti lygiai tokio pailginimo, kokio reikia, nepavyks, 
	// tai imame toká pailginimà, kad paklaida bûtø maþiausia 
	// (arba ðiek tiek pritrûksta iki reikiamo pailginimo, arba ðiek tiek virðija).

	// 1. Nustatome, kiek pailgëtø, jei panaudotume visus burbulus. 

	// maksimalus pailginimas, jei kiekvienà burbulà kartosime ne daugiau kaip po 1 kartà
	int maksimalus_pailginimas = (pikai [kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr] 
		+ pikai [kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr + 1] 
		- pikai [kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr] 
		- pikai [kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr - 1])/2;

	// 2. Paskaièiuojame, kiek kartø reikia panaudoti kiekvienà burbulà. 

	// kiek kartø reikia kartoti visus burbulus
	int pakartojimu_skaicius = siekiamas_pailginimas / maksimalus_pailginimas;
	// kiek dar lieka pailginti, pakartojus reikiamà kartø skaièiø visà leistinà sritá
	int trukstamas_pailginimas = siekiamas_pailginimas % maksimalus_pailginimas;

	// 3. Paskaièiuojame, kuriuos vidurinius burbulus reiks dubliuoti papildomai.

	// algoritmas: dubliavimui iðrinksime tiek viduriniø ið eilës einanèiø 
	// (t.y. gretimø) galimø burbulø, kad bûtø gautas trûkstamas pailginimas 
	// (gali bûti, kad bus iðrinkti visi).

	// vidurinio burbulo numeris, iðreikðtas viso signalo (ne vienos fonemos) pikø numeriais 
	int vidurinio_burbulo_nr = kontekstas->pirmojo_piko_nr + (pirmojo_burbulo_nr + paskutiniojo_burbulo_nr)/2;

	// maksimalus burbulø, kuriuos galime dubliuoti, skaièius
	int max_burbulu_sk = paskutiniojo_burbulo_nr - pirmojo_burbulo_nr + 1;

	// ieðkomas burbulø skaièius
	int burbulu_sk = 0;

	// ieðkomos burbulø sekos pirmojo ir paskutinio burbulø numeriai
	// (burbulo numeris sutampa su jo vidurinio piko numeriu,
	// o pikai fonemoje numeruojami nuo nulio)
	// inicializuojame taip, kad burbulø seka bûtø tuðèia
	int burbulu_sekos_pradzia = vidurinio_burbulo_nr + 1;
	int burbulu_sekos_pabaiga = vidurinio_burbulo_nr;

	// kiek pailgëtø, jei dubliuotume tokià sekà
	int busimas_pailginimas = 0;
	int busimas_pailginimas_old = 0; // ásimename paskutinio burbulo tikrinimui

	// iðsiaiðkinkime, kiek reikia dubliuoti burbulø tokiam pailginimui

	while (busimas_pailginimas < trukstamas_pailginimas && burbulu_sk < max_burbulu_sk)
	{
		burbulu_sk++;
		
		// nustatome burbulø sekos pradþià
		burbulu_sekos_pradzia = vidurinio_burbulo_nr - (burbulu_sk - 1)/2; // teisinga tik tada, kai burbulu_sk > 0

		// nustatome burbulø sekos pabaigà
		burbulu_sekos_pabaiga = vidurinio_burbulo_nr + burbulu_sk/2;

		// kiek pailgëtø, jei dubliuotume tokià sekà
		busimas_pailginimas_old = busimas_pailginimas;
		busimas_pailginimas = (pikai [burbulu_sekos_pabaiga] + pikai [burbulu_sekos_pabaiga + 1] 
			- pikai [burbulu_sekos_pradzia] - pikai [burbulu_sekos_pradzia - 1])/2;
	}

	// iðsiaiðkinkime, ar paskutinis burbulas tikrai reikalingas: 
	// paskutiná burbulà imame tik tuo atveju, jei já imant paklaida bus maþesnë, negu neimant

	if (burbulu_sk != 0 && busimas_pailginimas - trukstamas_pailginimas > trukstamas_pailginimas - busimas_pailginimas_old) {
		// paskutinio burbulo atsisakome, perskaièiuojame reikðmes
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
		// dubliuoti tik vidurinius burbulus po vienà kartà
		suformuoti_nurodytus_burbulus (burbulu_sekos_pradzia, burbulu_sekos_pabaiga, 1, kontekstas);
	else {
		// dubliuoti visus burbulus po pakartojimu_skaicius kartø
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
Skardþiøjø garsø ilginimas - iðkvieèiamas nurodytas scenarijus
*********************************************************/
void ilginimo_euristika_skardieji (struct tkontekstas * kontekstas)
{
	// jei tik 0, 1 ar 2 pikai fonemoje, nëra burbulo dubliavimui
	if (kontekstas->piku_sk < 3) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	// nusistatome ribas - nuo kurio iki kurio burbulo galime dubliuoti
	int pirmojo_burbulo_nr = 1;
	int paskutiniojo_burbulo_nr = kontekstas->piku_sk - 2;
	
	// jei nëra burbulo dubliavimui
	if (paskutiniojo_burbulo_nr < pirmojo_burbulo_nr) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	switch (scenarijus) {
		
	case 1:
	case 2:
		// iðrenkame viduriná burbulà dubliavimui
		
		ilginimo_euristika_skardieji_scenarijus1 (pirmojo_burbulo_nr, paskutiniojo_burbulo_nr, kontekstas);
		
		break;
		
	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
		ilginimo_euristika_skardieji_scenarijus3 (pirmojo_burbulo_nr, paskutiniojo_burbulo_nr, kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus maþiau, sprogstamuosius dar maþiau, r nekeisti)
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
Leistinos srities ribø dusliøjø garsø ilginimui nustatymas
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
		// TODO: tai Z duomenys. Kol kas z ir h nepasitaikë visai. 
		// Kai pasitaikys, atitinkamai pakeisti galimà intervalà
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
// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
galimos_srities_pradzia - galimos srities pradþios indeksas signalo masyve.
galimos_srities_pabaiga - galimos srities pabaigos indeksas signalo masyve.
Greitinimo koeficientas paduodamas tam, 
kad bûtø galima iðkviesti 4 scenarijuje su skirtingais greitinimo koeficientais
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

	// iðsiaiðkinkime, kiek reikia pailginti fonemà

	size_t fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	size_t siekiamas_fonemos_ilgis = (size_t) (fonemos_ilgis * greitinimo_koef);
	int siekiamas_pailginimas = siekiamas_fonemos_ilgis - fonemos_ilgis;
	
	int burbulu_sk = 0;

	// ptk ilginame, imdami burbulus ið pirmos garsø bazës fonemos (pauzës), 
	// todël parametrus galimos_srities_pradzia ir galimos_srities_pabaiga
	// nurodome ne einamosios fonemos, o pauzës,
	// bei kad nukopijuotø senà signalà iki fonemos leistinos srities vidurio, 
	// suformuojame fiktyvø nulinio ploèio burbulà toje vietoje
	
	if (fonemos[kontekstas->fonemos_nr][0] == 'p' || fonemos[kontekstas->fonemos_nr][0] == 'k' || 
		(fonemos[kontekstas->fonemos_nr][0] == 't' && fonemos[kontekstas->fonemos_nr][1] != 's' && fonemos[kontekstas->fonemos_nr][1] != 'S')) {
		
		// kad nukopijuotø senà signalà iki fonemos leistinos srities vidurio, 
		// suformuojame fiktyvø nulinio ploèio burbulà toje vietoje

		// suformuojame fiktyvø burbulà
		kontekstas->burbulai[burbulu_sk].pradzia = (galimos_srities_pradzia + galimos_srities_pabaiga)/2;
		kontekstas->burbulai[burbulu_sk].pabaiga = kontekstas->burbulai[burbulu_sk].pradzia;
		kontekstas->burbulai[burbulu_sk].vidurys = kontekstas->burbulai[burbulu_sk].pradzia;
		kontekstas->burbulai[burbulu_sk].kartai = 0;

		// padidiname burbulø skaitliukà
		burbulu_sk++;

		// suþinome pauzës galimos srities ribas
		double pauzes_galimos_srities_pradzia = 0;
		double pauzes_galimos_srities_pabaiga = 1;
		leistinos_srities_ribos_dusliuju_ilginimui (0, &pauzes_galimos_srities_pradzia, &pauzes_galimos_srities_pabaiga);
		
		// perskaièiuojame parametrus galimos_srities_pradzia ir galimos_srities_pabaiga, 
		// kad atitiktø pirmà garsø bazës fonemà (pauzæ)
		galimos_srities_pradzia = (size_t) (pauzes_galimos_srities_pradzia * fonemu_ilgiai[0]);
		galimos_srities_pabaiga = (size_t) (pauzes_galimos_srities_pabaiga * fonemu_ilgiai[0]); 
	}
		
	// Algoritmas: burbulus parenkame atsitiktinai, kol pasiekiame norimà pailgëjimà. 
	// Paskutiná burbulà, jei per didelis, sumaþiname iki reikiamo ilgio.

	int galimos_srities_ilgis = galimos_srities_pabaiga - galimos_srities_pradzia;
	// kiek dar liko pailginti
	int trukstamas_pailginimas = siekiamas_pailginimas;

	srand((unsigned int)time(NULL));

	while (trukstamas_pailginimas > 0) {
		// atsitiktinai parenkame burbulo ilgá
		size_t burbulo_plotis = (size_t) ((((double)rand())/RAND_MAX)*galimos_srities_ilgis);

		// jei per ilgas burbulas, sumaþiname iki reikiamo ilgio
		if (burbulo_plotis/2 > (size_t)trukstamas_pailginimas)
			burbulo_plotis = trukstamas_pailginimas*2;

		// atsitiktinai parenkame burbulo pradþios pozicijà leistinoje srityje
		size_t burbulo_pradzia = (size_t) ((((double)rand())/RAND_MAX)*(galimos_srities_ilgis-burbulo_plotis));

		// suformuojame burbulà
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

		// padidiname burbulø skaitliukà
		burbulu_sk++;

		// perskaièiuojame trûkstamà pailginimà
		trukstamas_pailginimas -= burbulo_plotis/2;
	}
	
		// ásimename sudarytø burbulø skaièiø
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
Dusliøjø garsø ilginimas - iðkvieèiamas nurodytas scenarijus
*********************************************************/
void ilginimo_euristika_duslieji (struct tkontekstas * kontekstas)
{
	// Duslieji garsai, kuriø trumpinimui nenaudojame pikø informacijos.
	// Sudarome 1 burbulà, kurá reikës iðkirpti.
	// TODO: Burbulo dydis ir padëtis turi priklausyti nuo:
	// 1) trumpinimo koeficiento,
	// 2) (galbût?) fonemos pavadinimo,
	// 3) fonemos ribø (pvz, burbulas negali prasidëti arèiau fonemos pradþios kaip 20% jos ilgio 
	//		ir negali pasibaigti arèiau fonemos pabaigos kaip 70% jos ilgio. 
	//		Ribos gali priklausyti nuo fonemos pavadinimo).
	
	// nusistatome ribas - nuo kur iki kur galime dubliuoti
	double galimos_srities_pradzia = 0;
	double galimos_srities_pabaiga = 1;
	
	leistinos_srities_ribos_dusliuju_ilginimui (kontekstas->fonemos_nr, &galimos_srities_pradzia, &galimos_srities_pabaiga);

	switch (scenarijus) {
		
	case 1:
	case 2:
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// suformuojame burbulà
		kontekstas->burbulai[0].pradzia = kontekstas->fonemos_pradzia + (size_t) (0.30 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].vidurys = kontekstas->fonemos_pradzia + (size_t) (0.40 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].pabaiga = kontekstas->fonemos_pradzia + (size_t) (0.50 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].kartai = 1;
		//kontekstas->burbulai[0].pikai = 0;
		
		break;
		
	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
		ilginimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus maþiau, sprogstamuosius dar maþiau, r nekeisti)
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
// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef).
galimos_srities_pradzia - galimos srities pradþios indeksas signalo masyve.
galimos_srities_pabaiga - galimos srities pabaigos indeksas signalo masyve.
Greitinimo koeficientas paduodamas tam, 
kad bûtø galima iðkviesti 4 scenarijuje su skirtingais greitinimo koeficientais
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

	// iðsiaiðkinkime, kiek reikia patrumpinti fonemà

	size_t fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	size_t siekiamas_fonemos_ilgis = (size_t) (fonemos_ilgis * greitinimo_koef);
	size_t siekiamas_patrumpinimas = fonemos_ilgis - siekiamas_fonemos_ilgis;

	// randame leistinosios srities ilgá

	size_t galimos_srities_centras = (galimos_srities_pradzia + galimos_srities_pabaiga)/2;
	size_t galimos_srities_ilgis = galimos_srities_pabaiga - galimos_srities_pradzia;

	// jei iðsaugoti reikia visà leistinàjà sritá, nieko nedarome
	if (galimos_srities_ilgis <= ISSAUGOTI_GALIMOS_SRITIES_ILGIO) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}

	// Paskaièiuokime, kiek galime patrumpinti fonemà.

	size_t galimas_patrumpinimas = galimos_srities_ilgis - ISSAUGOTI_GALIMOS_SRITIES_ILGIO;

	size_t busimas_patrumpinimas = 0;
	if (siekiamas_patrumpinimas > galimas_patrumpinimas)
		busimas_patrumpinimas = galimas_patrumpinimas;
	else
		busimas_patrumpinimas = siekiamas_patrumpinimas;
	
	// Jei trumpinti nedaug, sudarysime vienà burbulà.
	// Jei daugiau, sudarysime grandinæ ið 2 burbulø.

	if (busimas_patrumpinimas <= ISSAUGOTI_GALIMOS_SRITIES_ILGIO) {
		
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// suformuojame burbulà
		kontekstas->burbulai[0].pradzia = galimos_srities_centras - busimas_patrumpinimas;
		kontekstas->burbulai[0].vidurys = galimos_srities_centras;
		kontekstas->burbulai[0].pabaiga = galimos_srities_centras + busimas_patrumpinimas;
		kontekstas->burbulai[0].kartai = 1;
		
	} else {
		
		kontekstas->keiciamu_burbulu_sk = 2;

		size_t burbulu_srities_ilgis = busimas_patrumpinimas + ISSAUGOTI_GALIMOS_SRITIES_ILGIO;
		size_t burbulu_srities_pradzia = galimos_srities_centras - burbulu_srities_ilgis/2;
		
		// suformuojame burbulus

		// pirmojo burbulo kairës plotis = ISSAUGOTI_GALIMOS_SRITIES_ILGIO
		kontekstas->burbulai[0].pradzia = burbulu_srities_pradzia;
		kontekstas->burbulai[0].vidurys = burbulu_srities_pradzia + ISSAUGOTI_GALIMOS_SRITIES_ILGIO;
		kontekstas->burbulai[0].pabaiga = burbulu_srities_pradzia + busimas_patrumpinimas;
		kontekstas->burbulai[0].kartai = 1;
		
		// antrojo burbulo deðinës plotis = ISSAUGOTI_GALIMOS_SRITIES_ILGIO
		kontekstas->burbulai[1].pradzia = kontekstas->burbulai[0].vidurys;
		kontekstas->burbulai[1].vidurys = kontekstas->burbulai[0].pabaiga;
		kontekstas->burbulai[1].pabaiga = burbulu_srities_pradzia + burbulu_srities_ilgis;
		kontekstas->burbulai[1].kartai = 1;
		
	}
}

/*********************************************************
Leistinos srities ribø dusliøjø garsø trumpinimui nustatymas
*********************************************************/
void leistinos_srities_ribos_dusliuju_trumpinimui (size_t fonemos_nr, double * galimos_srities_pradzia, double * galimos_srities_pabaiga)
{
	// kol kas nesiskiria nuo leistinosios srities ribø dusliøjø garsø ilginimui,
	// taèiau jei reikës, kad skirtøsi, galima bus pakeisti
	leistinos_srities_ribos_dusliuju_ilginimui (fonemos_nr, galimos_srities_pradzia, galimos_srities_pabaiga);
}

/*********************************************************
Dusliøjø garsø trumpinimas - iðkvieèiamas nurodytas scenarijus
*********************************************************/
void trumpinimo_euristika_duslieji (struct tkontekstas * kontekstas)
{
	// Duslieji garsai, kuriø trumpinimui nenaudojame pikø informacijos.
	// Sudarome 1 burbulà, kurá reikës iðkirpti.
	// TODO: Burbulo dydis ir padëtis turi priklausyti nuo:
	// 1) trumpinimo koeficiento,
	// 2) (galbût?) fonemos pavadinimo,
	// 3) fonemos ribø (pvz, burbulas negali prasidëti arèiau fonemos pradþios kaip 20% jos ilgio 
	//		ir negali pasibaigti arèiau fonemos pabaigos kaip 70% jos ilgio. 
	//		Ribos gali priklausyti nuo fonemos pavadinimo).
	
	// nusistatome ribas - nuo kur iki kur galime "ðalinti"
	double galimos_srities_pradzia = 0;
	double galimos_srities_pabaiga = 1;
	
	leistinos_srities_ribos_dusliuju_trumpinimui (kontekstas->fonemos_nr, &galimos_srities_pradzia, &galimos_srities_pabaiga);

	switch (scenarijus) {
		
	case 1:
	case 2:
		kontekstas->keiciamu_burbulu_sk = 1;
		
		// suformuojame burbulà
		kontekstas->burbulai[0].pradzia = kontekstas->fonemos_pradzia + (size_t) (0.30 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].vidurys = kontekstas->fonemos_pradzia + (size_t) (0.40 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].pabaiga = kontekstas->fonemos_pradzia + (size_t) (0.50 * fonemu_ilgiai[kontekstas->fonemos_nr]);
		kontekstas->burbulai[0].kartai = 1;
		//kontekstas->burbulai[0].pikai = 0;
		
		break;

	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
		trumpinimo_euristika_duslieji_scenarijus3 (
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pradzia * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->fonemos_pradzia + (size_t) (galimos_srities_pabaiga * fonemu_ilgiai[kontekstas->fonemos_nr]), 
			kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus maþiau, sprogstamuosius dar maþiau, r nekeisti)
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
// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
Greitinimo koeficientas paduodamas tam, 
kad bûtø galima iðkviesti 4 scenarijuje su skirtingais greitinimo koeficientais
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

	// iðsiaiðkinkime, kiek reikia patrumpinti fonemà

	int fonemos_ilgis = kontekstas->fonemos_pabaiga - kontekstas->fonemos_pradzia;
	int siekiamas_fonemos_ilgis = (int) (fonemos_ilgis * greitinimo_koef);
	int siekiamas_patrumpinimas = fonemos_ilgis - siekiamas_fonemos_ilgis;

	// Algoritmo tikslas: iðmesti burbulus taip, kad net ir iðmetus nemaþai burbulø, vis tiek liktø burbulø ið fonemos vidurio, ne tik ið kraðtø.
	//
	// Algoritmo idëja: pradedant nuo viduriniø burbulø, iðmetinëti kas antrà burbulà. Likusius iðmetinëti, pradedant nuo kraðtiniø.
	//
	// Detalesnë algoritmo idëja: 
	// pirmame etape iðmetinësime nelyginius burbulus nuo viduriniø link kraðtiniø, 
	// antrame - lyginius nuo kraðtiniø link viduriniø.
	// (laikome, kad pirmas burbulas yra pirmojo_burbulo_nr)
	//
	// Algoritmo realizacija:
	//
	// I etapas.
	//
	// Nelyginiø skaièiø sekos 1, 3, 5, ..., 2k-1 iðrikiavimà nuo viduriniø nariø suvedame 
	// á skaièiø sekos 1, 2, 3, ..., k iðrikiavimà nuo viduriniø nariø.
	// Paþymëkime v:=[(k+1)/2] - vidurinis (arba vienas ið viduriniø) sekos 1, 2, 3, ..., k narys. 
	// Tada vienas ið galimø iðrikiavimø nuo viduriniø nariø bûdø bûtø toks:
	// v, v+1, v-1, v+2, v-2, ... (kol panaudosime visus k nariø).
	// Tokiai sekai konstruoti sugalvojau toká rekursyvø bûdà:
	// a_0 = v, a_i = a_{i-1} + b_i * i, kur
	// b_0 = -1; b_i = (-1) * b_{i-1}.
	//
	// II etapas.
	// 
	// Lyginiø skaièiø sekos 2, 4, 6, ..., 2k iðrikiavimà nuo kraðtiniø nariø suvedame 
	// á skaièiø sekos 1, 2, 3, ..., k iðrikiavimà nuo kraðtiniø nariø.
	// Tada vienas ið galimø iðrikiavimø nuo kraðtiniø nariø bûdø bûtø toks:
	// 1, k, 2, k-1, ... (kol panaudosime visus k nariø).
	// Tokiai sekai konstruoti sugalvojau toká rekursyvø bûdà:
	// a_0 = 1, a_i = a_{i-1} + b_i * (k-i), kur
	// b_0 = -1; b_i = (-1) * b_{i-1}.
	// 
	// Bet mums neuþtenka sudaryti burbulø sekà, reikia dar, kad burbulai masyve bûtø iðrikiuoti nuo pirmiausio iki paskutinio.
	// Tai realizuosime taip: pirma þymësimës ðalinamø burbulø numerius specialiame pagalbiniame masyve
	// (pradþioje visi masyvo elementai = 0. Jei ðalinsime i-tàjá burbulà, i-1-àjá masyvo elementà keièiame á 1), 
	// kol pasieksime reikiamà patrumpëjimà. 
	// Tik tada suformuosime tuos burbulus burbulø masyve, kuriø reikðmës specialiame masyve lygios 1.


	// maksimalus burbulø, kuriuos galime "iðmesti", skaièius
	int max_burbulu_sk = paskutiniojo_burbulo_nr - pirmojo_burbulo_nr + 1;

	// ðalinamø burbulø numeriø masyvas
	// (pradþioje visi masyvo elementai = 0. Jei ðalinsime i-tàjá burbulà, i-1-àjá masyvo elementà keièiame á 1)
	int * salinami_burbulai = (int *) calloc (max_burbulu_sk, sizeof(int));

	// kiek patrumpëtø, jei "iðmestume" tokius burbulus
	int busimas_patrumpinimas = 0;
	int busimas_patrumpinimas_old = 0; // ásimename paskutinio burbulo tikrinimui

	// ásimename paskutiná burbulà, kad galëtume prireikus jo atsisakyti (numeruojame nuo 1 iki max_burbulu_sk)
	int burb = 0;
	int pik = 0;

	// I etapas

	// Nelyginio numerio burbulø skaièius (jei numeruojame juos nuo 1 iki max_burbulu_sk)
	int k = (max_burbulu_sk + 1)/2;

	// Paþymëkime v:=[(k+1)/2] - vidurinis (arba vienas ið viduriniø) sekos 1, 2, 3, ..., k narys. 
	int v = (k+1)/2;

	// iðmetinëjame nelyginius burbulus nuo vidurinio, kol pasieksime siekiamà patrumpinimà, arba kol panaudosime visus sekos narius

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

		// ðalinsime burbulà 2a-1
		//burb_old = burb;
		burb = 2*a - 1;
		salinami_burbulai [burb - 1] = 1;
		// burbulo numeris, iðreikðtas viso signalo (ne vienos fonemos) pikø numeriais 
		pik = kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr - 1 + burb;

		// kiek patrumpëtø, jei "iðmestume" toká burbulà
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

	// Lyginio numerio burbulø skaièius (jei numeruojame juos nuo 1 iki max_burbulu_sk)
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

		// ðalinsime burbulà 2a
		//burb_old = burb;
		burb = 2*a;
		salinami_burbulai [burb - 1] = 1;
		// burbulo numeris, iðreikðtas viso signalo (ne vienos fonemos) pikø numeriais 
		pik = kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr - 1 + burb;

		// kiek patrumpëtø, jei "iðmestume" toká burbulà
		busimas_patrumpinimas_old = busimas_patrumpinimas;
		busimas_patrumpinimas += pikai [pik+1] - pikai [pik-1] - (pikai [pik+1] - pikai [pik-1])/2; // tai daþniausiai tikslus patrumpinimas, bet kartais gali skirtis per 1 ar -1, þr. 2015-03-27 uþraðus 

		i++;

		if (debuginam > 200) {
			char pranesimas [1024];
			sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji_scenarijus3(): II etapas: i=%d, a=%d, b=%d, burb=%d, pik=%d, busimas_patrumpinimas=%d", 
				i, a, b, burb, pik, busimas_patrumpinimas);
			spausdinti_loga (pranesimas);
		}
	}

	// iðsiaiðkinkime, ar paskutinis burbulas tikrai reikalingas: 
	// paskutiná burbulà imame tik tuo atveju, jei já imant paklaida bus maþesnë, negu neimant

	if (burb != 0 && busimas_patrumpinimas - siekiamas_patrumpinimas > siekiamas_patrumpinimas - busimas_patrumpinimas_old) {
		// paskutinio burbulo atsisakome, perskaièiuojame reikðmes
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
Skardþiøjø garsø trumpinimas - iðkvieèiamas nurodytas scenarijus
*********************************************************/
void trumpinimo_euristika_skardieji (struct tkontekstas * kontekstas)
{
	if (debuginam && !reguliarus_pikai (kontekstas)) {
		char pranesimas [1024];
		sprintf (pranesimas, "Euristika.cpp: trumpinimo_euristika_skardieji(): !!! Nereguliarus pikai: %s, nr: %d, piku_sk: %d", 
			fonemos[kontekstas->fonemos_nr], kontekstas->fonemos_nr, kontekstas->piku_sk);
		spausdinti_loga (pranesimas);
	}
	
	// Skardieji garsai, kuriø trumpinimui naudojame pikø informacijà.
	// Kol kas parinksime viduriná burbulà, kurá reikës iðkirpti.
	// TODO: padaryti priklausomybæ nuo trumpinimo koeficiento.
	
	// jei tik 0, 1 ar 2 pikai fonemoje, nëra burbulo ðalinimui
	if (kontekstas->piku_sk < 3) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	// randame pirmojo burbulo, kurá galima paðalinti, numerá
	
	// numeris pirmojo burbulo fonemoje, kurá galima paðalinti. Numeruojama nuo nulio.
	// Burbulo numeris sutampa su burbulo vidurinio piko numeriu,
	// t.y. burbulas i yra tas, 
	// kurio vidurinis pikas yra pirmojo_piko_nr + i.
	// Nulinio burbulo vidurinis pikas yra pirmojo_piko_nr, 
	// todël nulinis burbulas visada kirs fonemos ribà 
	// ir jo dël to niekada negalësime ðalinti.
	int pirmojo_burbulo_nr = 0;
	
	// jei nuo pirmojo piko iki fonemos pradþios lieka maþiau (arba lygu) 
	// kaip pusë pirmojo periodo ilgio (atstumas tarp pirmøjø pikø), 
	// tai pirmojo burbulo negalime ðalinti, nes jo pradþia labai arti fonemos pradþios
	// (galime ðalinti nuo antrojo),
	// prieðingu atveju galime ðalinti jau ir pirmàjá.
	if (2 * (pikai [kontekstas->pirmojo_piko_nr] - kontekstas->fonemos_pradzia) <= 
		pikai [kontekstas->pirmojo_piko_nr+1] - pikai [kontekstas->pirmojo_piko_nr])
		pirmojo_burbulo_nr = 2;
	else
		pirmojo_burbulo_nr = 1;
	
	// analogiðkai randame paskutiniojo burbulo, kurá galima paðalinti, numerá
	
	int paskutiniojo_burbulo_nr = 0;
	int paskutiniojo_piko_nr = kontekstas->pirmojo_piko_nr+kontekstas->piku_sk-1;
	if (2 * (kontekstas->fonemos_pabaiga - pikai [paskutiniojo_piko_nr]) <= 
		pikai [paskutiniojo_piko_nr] - pikai [paskutiniojo_piko_nr-1])
		paskutiniojo_burbulo_nr = kontekstas->piku_sk - 3;
	else
		paskutiniojo_burbulo_nr = kontekstas->piku_sk - 2;
	
	// jei nëra burbulo ðalinimui
	if (paskutiniojo_burbulo_nr < pirmojo_burbulo_nr) {
		kontekstas->keiciamu_burbulu_sk = 0;
		return;
	}
	
	int vidurinio_burbulo_nr = 0;

	switch (scenarijus) {
		
	case 1:
		// iðrenkame viduriná burbulà ðalinimui
		
		// vidurinio burbulo numeris (numeruojant nuo 0, skaièiuojant nuo pirmojo_burbulo_nr)
		vidurinio_burbulo_nr = kontekstas->pirmojo_piko_nr 
			+ (pirmojo_burbulo_nr + paskutiniojo_burbulo_nr)/2;
		
		// suformuojame nurodytus burbulus
		suformuoti_nurodytus_burbulus (
			vidurinio_burbulo_nr, vidurinio_burbulo_nr, 1, kontekstas);

		break;
	case 2:
		// ðalinimui paskiriame visus galimus burbulus
		
		// suformuojame nurodytus burbulus
		suformuoti_nurodytus_burbulus (kontekstas->pirmojo_piko_nr + pirmojo_burbulo_nr, 
								   kontekstas->pirmojo_piko_nr + paskutiniojo_burbulo_nr, 1, kontekstas);

		break;
		
	case 3:
		// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
		trumpinimo_euristika_skardieji_scenarijus3 (
			pirmojo_burbulo_nr, paskutiniojo_burbulo_nr, kontekstas->greitinimo_koef, kontekstas);
		break;
		
	case 4:
		// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
		// (balsius labiausiai, kitus maþiau, sprogstamuosius dar maþiau, r nekeisti)
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
             PAGRINDINËS FUNKCIJOS
 *********************************************************
 *********************************************************/

/*********************************************************
Dusliøjø garsø apdorojimas - iðkvieèiama trumpinimo ar ilginimo funkcija, 
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
Skardþiøjø garsø apdorojimas - iðkvieèiama trumpinimo ar ilginimo funkcija, 
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
Pagrindinë euristikos funkcija.
Parenka burbulus signalo trumpinimui (kalbëjimo tempo greitinimui).
Bandomos ávairios euristikos.
Svarbu: 
1) Gràþina burbulus, iðrikiuotus eilës tvarka (!!!).
2) Gràþinti burbulai gali persidengti tik puse burbulo 
(kaip pikø pagrindu suformuotø burbulø atveju).
//3) Visø gràþintø burbulø reikðmës "pikai" yra vienodos 
//(t.y. arba visi burbulai suformuoti pikø pagrindu, arba në vieno).
*********************************************************/
void euristika (struct tkontekstas * kontekstas)
{
	/* Algoritmas.
		Jei ne x, f, p, t, k, s, S, _, r, R, z, Z, H - daryti pagal pikus. 
			Tikrinti, ar yra iðmetamø burbulø (pagal pikø skaièiø ir burbulø centrus).
			Jei yra - iðmesti, jei nëra - nedaryti nieko.
		Jei x, f, p, t, k, s, S, _ - daryti be pikø 
			(parinkti ir iðmesti gabaliukà ið vidurio, link pradþios).
		Jei r, R - nedaryti nieko.
		Jei z, Z, h - tikrinti, ar taisyklingai pasiskirstæ pikai. 
			Jei taip, naudoti pikus, 
			jei trûksta tokio piko, paèiam pridëti, 
			o jei maþai pikø, tai daryti ne pagal pikus.*/
	
	switch (kontekstas->fonemos_klase) {

	case FONEMU_KLASE_SKARDIEJI:
		// Skardieji garsai, kuriø trumpinimui naudojame pikø informacijà.
	case FONEMU_KLASE_RR:
		// Fonemas r, R, jei reikia, trumpiname/ilginame, naudodami pikø informacijà (jei jos yra pakankamai. Jei ne - fonemos nekeièiame).
		euristika_skardieji (kontekstas);
		break;

	case FONEMU_KLASE_DUSLIEJI:
		// Duslieji garsai, kuriø trumpinimui nenaudojame pikø informacijos.
		euristika_duslieji (kontekstas);
		break;

//	case FONEMU_KLASE_RR:
		// Garso 'r' nemodifikuojame visai.
//		kontekstas->keiciamu_burbulu_sk = 0;
//		break;

	default:
		// Dar nenustatyta fonemos klasë. Tokio atvejo neturëtø bûti, o jei yra, reiðkia, kaþkas negerai su realizacija, "internal error". Kà daryti tokiu atveju? Kol kas nekeièiame visai.
		kontekstas->keiciamu_burbulu_sk = 0;
		break;
	}
}

