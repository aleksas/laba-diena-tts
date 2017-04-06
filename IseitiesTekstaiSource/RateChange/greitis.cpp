///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas greitis.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "RateChangeInternal.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*********************************************************
 * Global\xFBs kiti kintamieji
 ********************************************************/

// Greitinimo koeficientas (kiek kart\xF8 turi pailg\xEBti signalas)
// TODO: nuskaityti is programos komandines eilutes parametru
//double greitinimo_koef = 1.49;

// numeris, \xE1einantis \xE1 naujai sukuriam\xF8 signalo ir anotacij\xF8 fail\xF8 pavadinimus
int bandymo_numeris = 102;

// Scenarijaus numeris
// TODO: nuskaityti is programos komandines eilutes parametru
// 1. I\xF0mesti/dubliuoti: skardiems vidurin\xE1 burbul\xE0, dusliems interval\xE0 tarp 30 ir 50% garso.
// 2. Kaip 1, bet skard\xFEiuosius sutrumpina maksimaliai
// 3. Visus trumpinti/ilginti vienodai (tiek kart\xF8, kiek liepia greitinimo_koef)
// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
// (balsius labiausiai, kitus ma\xFEiau, sprogstamuosius dar ma\xFEiau, r nekeisti)
// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas 
// [2015-01-12: nuo 7 versijos 5 scenarijus nebeveikia, nes funkcija nuskaityti_anotacijas() pakeista d\xEBl to, kad pasikeit\xEB anotacij\xF8 failo formatas]
const int scenarijus = 3;

// Scenarijaus 5 koeficientas i\xF0 intervalo [0,1]: 
// jei didelis (arti vieneto), tai stengsis patrumpinti/pailginti iki vidutinio ilgio, 
// jei ma\xFEas (arti nulio) - ma\xFEiau stengsis pasiekti vidutin\xE1 ilg\xE1
// TODO: nuskaityti is programos komandines eilutes parametru
double scenarijaus5_koeficientas = 1;

// Tai veiksmas, kur\xE1 programa atlieka. Kitaip tariant, programos veikimo re\xFEimas. 
// \xD0iuo metu realizuoti keturi re\xFEimai.
// 1. Atlieka duoto signalo failo (signalo_failo_pavadinimas ir kt.) l\xEBtinim\xE0/greitinim\xE0 
// pagal nurodyt\xE0 scenarij\xF8 su nurodytu greitinimo koeficientu
// 2. Atlieka duoto signalo failo (signalo_failo_pavadinimas ir kt.) l\xEBtinim\xE0/greitinim\xE0
// pagal nurodyt\xE0 scenarij\xF8 su \xE1vairiais greitinimo koeficientais 
// (j\xF8 pradin\xE6 ir galutin\xE6 reik\xF0mes bei \xFEingsn\xE1 galima nustatyti funkcijoje testas, 
// rezultat\xF8 \xE1 failus ne\xE1ra\xF0o, tik i\xF0veda \xE1 ekran\xE0 gaut\xE0 faktin\xE1 sul\xEBt\xEBjim\xE0/pagreit\xEBjim\xE0)
// 3. Randa fonem\xF8 ilgi\xF8 vidurkius 
// (fonem\xF8 ilgi\xF8 katalogas nurodomas kintamajame "katalogas" funkcijoje rasti_fonemu_ilgiu_vidurkius ())
// 4. Konvertuoja visus nurodytus signalo failus su nurodytais greitinimo koeficientais. 
const int veiksmas = 2;

// skirtumas tarp to, koks plotis buvo panaudotas rezultatuose, ir koks duomenyse.
// Bus neigiamas, jei signalo rezultatas sutrump\xEBjo, ir teigiamas, jei pailg\xEBjo.
// Per tiek padid\xEBs fonem\xF8 ilgiai
//int einamasis_postumis = 0;

//size_t fonemos_nr_debug = 0;

// jei ne nulis, i\xF0veda papildom\xE0 informacij\xE0 apie programos darb\xE0
// jei > 100, i\xF0veda detali\xE0 informacij\xE0
//int debuginam = 101;

/*********************************************************
 * Global\xFBs signalo (duomen\xF8) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char signalo_failo_pavadinimas[256] = "db.raw";

// garso signalo masyvas, i\xF0skiriamas dinami\xF0kai
short * signalas = NULL;

// garso signalo masyvo ilgis
size_t signalo_ilgis = 0;

/*********************************************************
 * Global\xFBs naujojo signalo (rezultato) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char * naujo_signalo_failo_pavadinimas = "10m2e-new.16le";
char * naujo_signalo_failo_pavadinimo_pradzia = "rez\\10m2e-new";
char * naujo_signalo_failo_pavadinimo_pabaiga = ".16le";

/*********************************************************
 * Global\xFBs fonem\xF8 (duomen\xF8) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char fonemu_failo_pavadinimas[256] = "db_fon_weights.txt";

// fonem\xF8 pavadinim\xF8 masyvas
char ** fonemos = NULL;

// fonem\xF8 ilgi\xF8 masyvas
int * fonemu_ilgiai = NULL;

// fonem\xF8 kiekis (fonem\xF8 masyvo ir fonem\xF8 ilgi\xF8 masyvo ilgis)
size_t fonemu_kiekis = 0;

/*********************************************************
 * Global\xFBs naujieji fonem\xF8 (rezultat\xF8) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char * naujo_fonemu_failo_pavadinimas = "10m3_ilg-new.txt";
char * naujo_fonemu_failo_pavadinimo_pradzia = "rez\\10m3_ilg-new";
char * naujo_fonemu_failo_pavadinimo_pabaiga = ".txt";

// fonem\xF8 ilgi\xF8 masyvas
int * nauji_fonemu_ilgiai = NULL;

/*********************************************************
 * Global\xFBs duomen\xF8 kintamieji, susij\xE6 su vidutiniu fonem\xF8 ilgiu (reikalingi kai kuriems scenarijams)
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru?
char vidutiniu_ilgiu_fonemu_failo_pavadinimas[256] = "vidutiniai_fonemu_ilgiai.txt";

// fonem\xF8 pavadinim\xF8 masyvas
char ** skirtingos_fonemos = NULL;

// fonem\xF8 ilgi\xF8 masyvas
int * vidutiniai_fonemu_ilgiai = NULL;

// fonem\xF8 kiekis (fonem\xF8 masyvo ir fonem\xF8 ilgi\xF8 masyvo ilgis)
size_t skirtingu_fonemu_kiekis = 0;
		
/*********************************************************
 * Global\xFBs pik\xF8 (duomen\xF8) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char piku_failo_pavadinimas[256] = "db_pik.txt";

// pik\xF8 masyvas (pikai[0] yra pirmojo piko vieta "signalas" masyve, t.y. masyvo "signalas" indeksas)
unsigned int * pikai = NULL;

// pik\xF8 kiekis (pik\xF8 masyvo ilgis)
size_t piku_kiekis = 0;

/*********************************************************
 * Burbulai
 ********************************************************/

// Euristika nustatys, kuriuos burbuliukus reikia pa\xF0alinti ar dubliuoti. 
// J\xF8 s\xE0ra\xF0\xE0 pateiks \xF0iame kintamajame "burbulai". 
// Kad nereik\xEBt\xF8 pastoviai i\xF0skirin\xEBti jam atminties, 
// \xE8ia i\xF0skiriame vien\xE0 kart\xE0 ir pastoviai naudojame.
// TODO: pastoviai tikrinti, ar nevir\xF0ijo MAX_BURBULAI. Vir\xF0ijus ka\xFEk\xE0 daryti.
//struct burbulas burbulai[MAX_KEICIAMI_BURBULAI];

/*********************************************************
I\xF0veda klaidos prane\xF0im\xE0 \xE1 stderr.
 ********************************************************/
void klaida (char * klaidos_pranesimas)
{
	char pranesimas [1024];
	sprintf (pranesimas, "Klaida: %s", klaidos_pranesimas);
	spausdinti_loga (pranesimas);
}

/*********************************************************
U\xFEtikrina, kad masyvo naujas_signalas ilgis yra ne ma\xFEesnis, 
nei reikiamas_ilgis.

Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int pailginti_masyva_naujas_signalas (size_t reikiamas_ilgis, struct tkontekstas * kontekstas)
{
	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): reikia pailginti nuo %d iki %d", 
			kontekstas->naujo_signalo_masyvo_ilgis, reikiamas_ilgis);
		spausdinti_loga (pranesimas);
	}

	// jei negalima ilginti - baigiam darb\xE0
	if (kontekstas->galima_pailginti_naujas_signalas == 0)
		return -1;

	// siulomas_naujas_ilgis - naujas ilgis, jei pailgintume pagal REALLOC_COEFF
	size_t siulomas_naujas_ilgis = (size_t) (kontekstas->naujo_signalo_masyvo_ilgis * REALLOC_COEFF);

	// galutinis naujas ilgis bus didesnysis i\xF0 siulomas_naujas_ilgis ir reikiamas_ilgis
	size_t naujas_ilgis;
	if (siulomas_naujas_ilgis < reikiamas_ilgis)
		naujas_ilgis = reikiamas_ilgis;
	else
		naujas_ilgis = siulomas_naujas_ilgis;

	// i\xF0skiriame atminties
	short * naujas_naujas_signalas 
		= (short *) realloc ((void *)kontekstas->naujas_signalas, naujas_ilgis*sizeof(short));

	// jei nepavyko i\xF0skirti atminties, baigiam darb\xE0
	if (naujas_naujas_signalas == NULL) {
		if (debuginam) {
			char pranesimas [1024];
			sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): nepavyko pailginti signalo nuo %d iki %d", 
				kontekstas->naujo_signalo_masyvo_ilgis, naujas_ilgis);
			klaida (pranesimas);
		}
		free (kontekstas->naujas_signalas);
		kontekstas->naujas_signalas = NULL;
		return -1;
	}

	// u\xFEnuliname naujai i\xF0skirt\xE0 (papildom\xE0) atmint\xE1
	for (size_t i = kontekstas->naujo_signalo_masyvo_ilgis; i < naujas_ilgis; i++)
		naujas_naujas_signalas[i] = 0;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): sekmingai pailginta nuo %d iki %d", 
			kontekstas->naujo_signalo_masyvo_ilgis, naujas_ilgis);
		spausdinti_loga (pranesimas);
	}

	// \xE1simename nauj\xE0j\xE1 masyv\xE0 ir jo ilg\xE1
	kontekstas->naujas_signalas = naujas_naujas_signalas;
	kontekstas->naujo_signalo_masyvo_ilgis = naujas_ilgis;

	return 0;
}

/*********************************************************
 * Hann lango kair\xEB pus\xEB (did\xEBjanti).
 * Nukopijuoja i\xF0 signalo masyvo nuo pradzia
 * \xE1 nauj\xE0j\xE1 signalo masyv\xE0 nuo nauja_pradzia
 * burbulo kairi\xE0j\xE0 pus\xE6 plo\xE8io lango_plotis.
 *
 * nauja_pradzia gali b\xFBti ir neigiamas, nes tono keitimo atveju galime ra\xF0yti prie\xF0 masyvo prad\xFEi\xE0.
 *
 * Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
********************************************************/
int Hann_lango_kaire_puse (int pradzia, int nauja_pradzia, int lango_plotis, struct tkontekstas * kontekstas)
{
	// taikome Hann lang\xE0 tik tuo atveju, kai lango_plotis > 1 (kad neb\xFBt\xF8 dalybos i\xF0 nulio ir kitoki\xF8 problem\xF8)
	if (lango_plotis <= 1)
		return 0;

	// tikriname, ar reikia ilginti naujo signalo masyv\xE0

	// nustatome, kokio ilgio tur\xEBt\xF8 b\xFBti masyvas naujas_signalas
	// (gali b\xFBti ir neigiamas, jei nauja_pradzia < 0 ir |nauja_pradzia| > lango_plotis)
	int reikiamas_ilgis = (size_t) (nauja_pradzia + lango_plotis);

	// jei reikiamas ilgis vir\xF0ija dabartin\xE1
	if (reikiamas_ilgis > (int) kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyv\xE0
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// \xFEingsnio ilgis
	// TODO: i\xF0siai\xF0kinti: gal zingsnis = pi / lango_plotis; ?
	double zingsnis = pi / (lango_plotis - 1);

	// taikome Hann lang\xE0
	for (int i=0; i < lango_plotis; i++)
		kontekstas->naujas_signalas [nauja_pradzia + i] += (short)
			(signalas [pradzia + i] * 0.5 * (1 - cos (zingsnis * i)));

	return 0;
}

/*********************************************************
 * Hann lango de\xF0in\xEB pus\xEB (ma\xFE\xEBjanti).
 * Nukopijuoja i\xF0 signalo masyvo nuo pradzia
 * \xE1 nauj\xE0j\xE1 signalo masyv\xE0 nuo nauja_pradzia
 * burbulo de\xF0in\xE6 pus\xE6 plo\xE8io lango_plotis
 *
 * Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int Hann_lango_desine_puse (int pradzia, int nauja_pradzia, int lango_plotis, struct tkontekstas * kontekstas)
{
	// taikome Hann lang\xE0 tik tuo atveju, kai lango_plotis > 1 (kad neb\xFBt\xF8 dalybos i\xF0 nulio ir kitoki\xF8 problem\xF8)
	if (lango_plotis <= 1)
		return 0;

	// tikriname, ar reikia ilginti naujo signalo masyv\xE0

	// nustatome, kokio ilgio tur\xEBt\xF8 b\xFBti masyvas naujas_signalas
	size_t reikiamas_ilgis = (size_t) (nauja_pradzia + lango_plotis);

	// jei reikiamas ilgis vir\xF0ija dabartin\xE1
	if (reikiamas_ilgis > kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyv\xE0
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// \xFEingsnio ilgis
	// TODO: i\xF0siai\xF0kinti: gal zingsnis = pi / lango_plotis; ?
	double zingsnis = pi / (lango_plotis - 1);

	// taikome Hann lang\xE0
	for (int i=0; i < lango_plotis; i++)
		kontekstas->naujas_signalas [nauja_pradzia + i] += (short)
			(signalas [pradzia + i] * 0.5 * (1 + cos (zingsnis * i)));

	return 0;
}

/*********************************************************
 * Spausdinti informacij\xE0 apie duotus burbulus
 ********************************************************/
void spausdinti_burbulus (struct burbulas * burbulai, int burbulu_sk)
{
	for (int i=0; i< burbulu_sk; i++, burbulai++)
	{
		char pranesimas [1024];
		sprintf (pranesimas, "%d:[pr: %d, vid: %d, pab: %d, kartai: %d][plotis: %d, kaire: %d, desine: %d]",
			i, burbulai->pradzia, burbulai->vidurys, burbulai->pabaiga, burbulai->kartai, 
			burbulai->pabaiga - burbulai->pradzia, burbulai->vidurys - burbulai->pradzia, burbulai->pabaiga - burbulai->vidurys);
		spausdinti_loga (pranesimas);
	}
}

/*********************************************************
 * tono auk\xF0\xE8io keitimas: apdorojame pus\xE6 pirmo burbulo (dal\xE1, kertan\xE8i\xE0 fonemos rib\xE0).
 * Laikoma, kad fonema skard\xFEioji, turi pakankamai pik\xF8 (piku_sk > 1) ir tono auk\xF0t\xE1 keisti reikia.
 ********************************************************/
int kopijuoti_signala_pradzioj (struct tkontekstas * kontekstas)
{
	// randame pirmojo burbulo kairiosios dalies plot\xE1.

	// pirmojo burbulo de\xF0iniosios dalies plotis - kairioji dalis bus ne platesn\xEB
	unsigned int burbulo_desines_plotis = pikai [kontekstas->pirmojo_piko_nr + 1] - pikai [kontekstas->pirmojo_piko_nr];
	
	// Formuojant ant fonemos ribos esant\xE1 burbul\xE0, tur\xEBti omenyje, kad u\xFE ribos gali visai neb\xFBti pik\xF8. 
	// Algoritmas b\xFBt\xF8 toks: tegu X yra burbulo kitos dalies plotis. 
	// Jei atstumu <= X nuo burbulo centro yra kitas pikas (kitoje fonemoje), tai jis ir bus burbulo kra\xF0tas, 
	// o jei ne, tai formuoti simetri\xF0k\xE0 burbul\xE0 (t.y. imti burbulo kra\xF0t\xE0 atstumu X nuo centro).

	// jei prie\xF0 tai yra pikas, ir jis nelabai toli nuo pirmojo fonemos piko, tai ir bus burbulo prad\xFEia. 
	// Jei ne, burbulo prad\xFEia bus nutolusi nuo burbulo centro (pirmojo fonemos piko) tokiu pat atstumu, kaip pabaiga.
	int burbulo_pradzia = 0;
	if (kontekstas->pirmojo_piko_nr > 0 && pikai [kontekstas->pirmojo_piko_nr]-pikai[kontekstas->pirmojo_piko_nr-1] <= burbulo_desines_plotis)
		burbulo_pradzia = pikai[kontekstas->pirmojo_piko_nr-1];
	else
		burbulo_pradzia = pikai[kontekstas->pirmojo_piko_nr] - burbulo_desines_plotis;

	// pirmojo burbulo kairiosios dalies plotis
	int burbulo_kaires_plotis = pikai[kontekstas->pirmojo_piko_nr] - burbulo_pradzia;
	
	// tono auk\xF0\xE8io keitimas: apskai\xE8iuojame, kiek d\xEBl tono auk\xF0\xE8io keitimo turi pasislinkti pikas
	// (jei neigiama reik\xF0m\xEB, slinksis \xE1 kair\xE6, jei teigiama, \xE1 de\xF0in\xE6).
	// Kitaip tariant, tiek pasikeis naujo signalo ilgis originalaus signalo ilgio at\xFEvilgiu
	int einamasis_postumis = (int) (burbulo_kaires_plotis * (kontekstas->tarpo_tarp_piku_didinimo_koef - 1));
	
	// jei pavyko == -1, visk\xE0 stabdome, nes nepavyko i\xF0skirti atminties
	int pavyko = 0;
	// Hann lango kair\xEB pus\xEB - prisumuojame pirmojo burbulo kairi\xE0j\xE0 dal\xE1
	pavyko = Hann_lango_kaire_puse (burbulo_pradzia, 
		kontekstas->einamasis_naujo_signalo_nr - (kontekstas->fonemos_pradzia - burbulo_pradzia) + einamasis_postumis, 
		burbulo_kaires_plotis, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) return -1;
	
	// atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes
	kontekstas->einamasis_naujo_signalo_nr += + burbulo_kaires_plotis + einamasis_postumis - (kontekstas->fonemos_pradzia - burbulo_pradzia);
	kontekstas->einamasis_signalo_nr = pikai [kontekstas->pirmojo_piko_nr];

	// perskai\xE8iuojame einam\xE0j\xE1 post\xFBm\xE1
	kontekstas->einamasis_postumis += einamasis_postumis;

	return 0;
}

/*********************************************************
 * tono auk\xF0\xE8io keitimas: apdorojame pus\xE6 paskutinio burbulo (dal\xE1, kertan\xE8i\xE0 fonemos rib\xE0).
 * Laikoma, kad fonema skard\xFEioji, turi pakankamai pik\xF8 (piku_sk > 1) ir tono auk\xF0t\xE1 keisti reikia.
 ********************************************************/
int kopijuoti_signala_pabaigoj (struct tkontekstas * kontekstas)
{
	// randame paskutiniojo burbulo de\xF0iniosios dalies plot\xE1.

	// paskutiniojo piko numeris
	size_t paskutinio_piko_nr = kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1;

	// paskutiniojo burbulo kairiosios dalies plotis - de\xF0inioji dalis bus ne platesn\xEB
	unsigned int burbulo_kaires_plotis = pikai [paskutinio_piko_nr] - pikai [paskutinio_piko_nr - 1];
	
	// Formuojant ant fonemos ribos esant\xE1 burbul\xE0, tur\xEBti omenyje, kad u\xFE ribos gali visai neb\xFBti pik\xF8. 
	// Algoritmas b\xFBt\xF8 toks: tegu X yra burbulo kitos dalies plotis. 
	// Jei atstumu <= X nuo burbulo centro yra kitas pikas (kitoje fonemoje), tai jis ir bus burbulo kra\xF0tas, 
	// o jei ne, tai formuoti simetri\xF0k\xE0 burbul\xE0 (t.y. imti burbulo kra\xF0t\xE0 atstumu X nuo centro).

	// jei paskui yra pikas, ir jis nelabai toli nuo paskutinio fonemos piko, tai ir bus burbulo pabaiga. 
	// Jei ne, burbulo pabaiga bus nutolusi nuo burbulo centro (paskutinio fonemos piko) tokiu pat atstumu, kaip prad\xFEia.
	int burbulo_pabaiga = 0;
	if (paskutinio_piko_nr + 1 < piku_kiekis && pikai [paskutinio_piko_nr + 1]-pikai [paskutinio_piko_nr] <= burbulo_kaires_plotis)
		burbulo_pabaiga = pikai [paskutinio_piko_nr + 1];
	else
		burbulo_pabaiga = pikai [paskutinio_piko_nr] + burbulo_kaires_plotis;

	// paskutiniojo burbulo de\xF0iniosios dalies plotis
	int burbulo_desines_plotis = burbulo_pabaiga - pikai [paskutinio_piko_nr];
	
	// jei pavyko == -1, visk\xE0 stabdome, nes nepavyko i\xF0skirti atminties
	int pavyko = 0;
	// Hann lango de\xF0in\xEB pus\xEB - prisumuojame paskutiniojo burbulo de\xF0ini\xE0j\xE0 dal\xE1
	pavyko = Hann_lango_desine_puse (pikai [paskutinio_piko_nr], 
		kontekstas->einamasis_naujo_signalo_nr, 
		burbulo_desines_plotis, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) return -1;
	
	// atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes
	kontekstas->einamasis_naujo_signalo_nr += kontekstas->fonemos_pabaiga - pikai [paskutinio_piko_nr];
	kontekstas->einamasis_signalo_nr = kontekstas->fonemos_pabaiga;

	// perskai\xE8iuojame einam\xE0j\xE1 post\xFBm\xE1 (\xF0iuo atveju - nepasikei\xE8ia)
	kontekstas->einamasis_postumis += 0;

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 indeks\xF8 reik\xF0mes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala_keiciant_tono_auksti (size_t iki, struct tkontekstas * kontekstas)
{
	// nieko nedarome, jei n\xEBra k\xE0 kopijuoti
	if (iki <= kontekstas->einamasis_signalo_nr)
		return 0;

	// surandame einamojo piko (nuo kurio reik\xEBs visk\xE0 kopijuoti) numer\xE1
	unsigned int pradzios_piko_nr = kontekstas->pirmojo_piko_nr;
	while (pradzios_piko_nr < kontekstas->pirmojo_piko_nr + kontekstas->piku_sk && pikai [pradzios_piko_nr] < kontekstas->einamasis_signalo_nr) 
		pradzios_piko_nr++;
	if (pikai [pradzios_piko_nr] != kontekstas->einamasis_signalo_nr) {
		// taip negali b\xFBti
		if (debuginam) {
			spausdinti_loga ("!!! Internal error 1 in function kopijuoti_signala_keiciant_tono_auksti() !!!");
			spausdinti_konteksta (kontekstas);
		}
	}

	// surandame paskutinio piko, iki kurio reik\xEBs visk\xE0 kopijuoti, numer\xE1
	unsigned int pabaigos_piko_nr = pradzios_piko_nr;
	while (pabaigos_piko_nr < kontekstas->pirmojo_piko_nr + kontekstas->piku_sk && pikai [pabaigos_piko_nr] < iki) 
		pabaigos_piko_nr++;
	if (pikai [pabaigos_piko_nr] != iki) {
		// taip negali b\xFBti
		if (debuginam) {
			spausdinti_loga ("!!! Internal error 2 in function kopijuoti_signala_keiciant_tono_auksti() !!!");
			spausdinti_konteksta (kontekstas);
		}
	}

	// kopijuojame tarpus tarp vis\xF8 pik\xF8 nuo pradzios_piko_nr iki pabaigos_piko_nr

	for (unsigned int piko_nr = pradzios_piko_nr; piko_nr < pabaigos_piko_nr; piko_nr++) {
		
		// jei pavyko == -1, visk\xE0 stabdome, nes nepavyko i\xF0skirti atminties
		int pavyko = 0;

		// tarpas tarp piku piko_nr ir piko_nr+1
		int tarpas_tarp_piku = pikai [piko_nr+1] - pikai [piko_nr];

		// Hann lango de\xF0in\xEB pus\xEB
		pavyko = Hann_lango_desine_puse (pikai [piko_nr], kontekstas->einamasis_naujo_signalo_nr, 
			tarpas_tarp_piku, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko == -1) return -1;
		
		// tono auk\xF0\xE8io keitimas: apskai\xE8iuojame, kiek d\xEBl tono auk\xF0\xE8io keitimo turi pasislinkti pikas
		// (jei neigiama reik\xF0m\xEB, slinksis \xE1 kair\xE6, jei teigiama, \xE1 de\xF0in\xE6).
		// Kitaip tariant, tiek pasikeis naujo signalo ilgis originalaus signalo ilgio at\xFEvilgiu
		int einamasis_postumis = (int) (tarpas_tarp_piku * (kontekstas->tarpo_tarp_piku_didinimo_koef - 1));
		
		// Hann lango kair\xEB pus\xEB
		pavyko = Hann_lango_kaire_puse (pikai [piko_nr], 
			kontekstas->einamasis_naujo_signalo_nr + einamasis_postumis, 
			tarpas_tarp_piku, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko == -1) return -1;
		
		// atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes
		kontekstas->einamasis_naujo_signalo_nr += tarpas_tarp_piku + einamasis_postumis;
		kontekstas->einamasis_signalo_nr = pikai [piko_nr+1];
		
		// perskai\xE8iuojame einam\xE0j\xE1 post\xFBm\xE1
		kontekstas->einamasis_postumis += einamasis_postumis;
		
	}

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 indeks\xF8 reik\xF0mes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala_nekeiciant_tono_aukscio (size_t iki, struct tkontekstas * kontekstas)
{
	// tikriname, ar reikia ilginti naujo signalo masyv\xE0

	// nustatome, kokio ilgio tur\xEBt\xF8 b\xFBti masyvas naujas_signalas
	size_t reikiamas_ilgis = kontekstas->einamasis_naujo_signalo_nr;
	if (iki > kontekstas->einamasis_signalo_nr)
		reikiamas_ilgis += iki - kontekstas->einamasis_signalo_nr;

	// jei reikiamas ilgis vir\xF0ija dabartin\xE1
	if (reikiamas_ilgis > kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyv\xE0
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo prad\xFEios
	// (negalime kopijuoti su memcpy, nes prarasime jau ten esan\xE8i\xE0 informacij\xE0).
	// Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes.
	for (; kontekstas->einamasis_signalo_nr < iki; 
			kontekstas->einamasis_signalo_nr++, kontekstas->einamasis_naujo_signalo_nr++)
		kontekstas->naujas_signalas[kontekstas->einamasis_naujo_signalo_nr] += signalas[kontekstas->einamasis_signalo_nr];
	//memcpy (kontekstas->naujas_signalas + kontekstas->einamasis_naujo_signalo_nr, 
	//	signalas + kontekstas->einamasis_signalo_nr, 
	//	(pirmas_burbulas->pradzia - kontekstas->einamasis_signalo_nr) * sizeof (short));

	// atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes
	//kontekstas->einamasis_naujo_signalo_nr += pirmas_burbulas->pradzia - kontekstas->einamasis_signalo_nr;
	//kontekstas->einamasis_signalo_nr = pirmas_burbulas->pradzia;

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 indeks\xF8 reik\xF0mes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala (size_t iki, struct tkontekstas * kontekstas)
{
	if (kontekstas->keisti_tono_auksti)
		// tono auk\xF0\xE8io keitimas: kopijuojame, keisdami tono auk\xF0t\xE1
		return kopijuoti_signala_keiciant_tono_auksti (iki, kontekstas);
	else
		// tiesiog kopijuojame (prisumuojame signal\xE0)
		return kopijuoti_signala_nekeiciant_tono_aukscio (iki, kontekstas);
}

/*********************************************************
 * I\xF0meta burbul\xF8 grandin\xE6. 
 * Tiksliau, nukopijuoja duomenis i\xF0 signalo masyvo \xE1 nauj\xE0 masyv\xE0, 
 * "i\xF0mesdamas" (nenukopijuodamas) visus grandin\xEBs burbulus.
 *
 * Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int ismesti_burbulu_grandine (struct burbulas * pirmas_burbulas,
							   struct burbulas * paskutinis_burbulas, struct tkontekstas * kontekstas)
{
	// jei pavyko == -1, visk\xE0 stabdome, nes nepavyko i\xF0skirti atminties
	int pavyko = 0;

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo prad\xFEios
	// Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes.
	pavyko = kopijuoti_signala (pirmas_burbulas->pradzia, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) return -1;

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: ismesti_burbulu_grandine(): einamasis_signalo_nr: %d, einamasis_naujo_signalo_nr: %d", 
			kontekstas->einamasis_signalo_nr, kontekstas->einamasis_naujo_signalo_nr);
		spausdinti_loga (pranesimas);
	}

	// pirmojo burbulo kair\xEBs dalies plotis
	int pirmojo_burbulo_kaires_plotis 
		= pirmas_burbulas->vidurys - pirmas_burbulas->pradzia;

	// paskutiniojo burbulo de\xF0in\xEBs dalies plotis
	int paskutinio_burbulo_desines_plotis 
		= paskutinis_burbulas->pabaiga - paskutinis_burbulas->vidurys;

	// apdorojame i\xF0mest\xE0j\xE0 dal\xE1 - tai bus dviej\xF8 Hann lang\xF8 suma

	// Hann lango de\xF0in\xEB pus\xEB - prisumuojame pirmojo burbulo kairi\xE0j\xE0 dal\xE1
	pavyko = Hann_lango_desine_puse (pirmas_burbulas->pradzia, kontekstas->einamasis_naujo_signalo_nr, 
		pirmojo_burbulo_kaires_plotis, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) return -1;

	// apskai\xE8iuojame, koks bus "apdorojamos dalies" plotis
	// (t.y. tos dalies, kur sumuojame Hann langus.
	// Jei yra pik\xF8, tai tas plotis parodo, kur bus kitas pikas).
	// TODO: \xD0iuo metu plo\xE8iu laikau pirmojo burbulo kairiosios dalies ir 
	// paskutiniojo burbulo de\xF0iniosios dalies plo\xE8i\xF8 vidurk\xE1.
	int einamasis_plotis = (pirmojo_burbulo_kaires_plotis + paskutinio_burbulo_desines_plotis)/2;

	// tono auk\xF0\xE8io keitimas: kei\xE8iame viet\xE0, kur bus kitas pikas (t.y. einamasis_plotis), 
	// priklausomai nuo tarpo tarp pik\xF8 didinimo koeficiento
	einamasis_plotis = (int) (einamasis_plotis * kontekstas->tarpo_tarp_piku_didinimo_koef);
		
	// Hann lango kair\xEB pus\xEB - prisumuojame paskutiniojo burbulo de\xF0ini\xE0j\xE0 dal\xE1
	pavyko = Hann_lango_kaire_puse (paskutinis_burbulas->vidurys, 
		kontekstas->einamasis_naujo_signalo_nr + einamasis_plotis - paskutinio_burbulo_desines_plotis, 
		paskutinio_burbulo_desines_plotis, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) return -1;

	// atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes
	kontekstas->einamasis_naujo_signalo_nr += einamasis_plotis;
	kontekstas->einamasis_signalo_nr = paskutinis_burbulas->pabaiga;

	// perskai\xE8iuojame einam\xE0j\xE1 post\xFBm\xE1
	kontekstas->einamasis_postumis += einamasis_plotis 
		- (paskutinis_burbulas->pabaiga - pirmas_burbulas->pradzia);

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: ismesti_burbulu_grandine(): einamasis_postumis: %d", kontekstas->einamasis_postumis);
		spausdinti_loga (pranesimas);
	}

	return 0;
}

/*********************************************************
 * Trumpina signal\xE0, perskai\xE8iuodamas visus reikiamus masyvus.
 *
 * Burbulai i\xF0metimui turi b\xFBti pateikti sur\xFB\xF0iuoti i\xF0 eil\xEBs.
 *
 * Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int trumpinti_fonema (struct tkontekstas * kontekstas)
{
	// burbul\xF8 grandin\xEBs (i\xF0 eil\xEBs einan\xE8i\xF8 burbul\xF8 sekos) apdorojimas 
	// skiriasi nuo pavieni\xF8 burbul\xF8 apdorojimo, 
	// tod\xEBl turime rasti tokias grandines. 
	// Tiksliau, apdorosime duomenis burbul\xF8 grandin\xEBmis, 
	// kurios gali b\xFBti ir tik vieno burbulo ilgio.

	// einamojo burbulo numeris
	int burbulo_nr = 0;

	// kol turime neapdorot\xF8 burbul\xF8
	while (burbulo_nr < kontekstas->keiciamu_burbulu_sk)
	{
		// i\xF0saugome pradin\xE1 grandin\xEBs burbul\xE0
		struct burbulas * pirmas_burbulas = kontekstas->burbulai+burbulo_nr;

		// ie\xF0kome paskutinio grandin\xEBs burbulo.

		// Burbulas nebus paskutinis burbul\xF8 grandin\xEBs burbulas, jei:
		// 1) jis nebus i\xF0vis paskutinis burbulas, ir
		while (burbulo_nr < kontekstas->keiciamu_burbulu_sk - 1 
		// 2) kitas burbulas eina i\xF0kart u\xFE jo
		// (Burbulas B eina i\xF0kart u\xFE burbulo A, jei B.pradzia == A.vidurys)
		// TODO: gal dar reikia tikrinti, ar B.vidurys == A.pabaiga? Ir k\xE0 daryti, jei ne?
			&& kontekstas->burbulai[burbulo_nr].vidurys == kontekstas->burbulai[burbulo_nr+1].pradzia)
			burbulo_nr++;

		// i\xF0saugome paskutin\xE1 grandin\xEBs burbul\xE0
		struct burbulas * paskutinis_burbulas = kontekstas->burbulai+burbulo_nr;

		// i\xF0metame burbul\xF8 grandin\xE6
		int pavyko = ismesti_burbulu_grandine (pirmas_burbulas, paskutinis_burbulas, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko == -1) return -1;

		// einame prie naujo burbulo
		burbulo_nr++;
	}

	return 0;
}

/*********************************************************
 * Iterpia duot\xE0 burbul\xE0 tiek kart\xF8, kiek jame nurodyta. 
 * Tiksliau, nukopijuoja duomenis i\xF0 signalo masyvo \xE1 nauj\xE0 masyv\xE0, 
 * dubliuodamas duot\xE0 burbul\xE0 tiek kart\xF8, kiek jame nurodyta.
 *
 * Gali b\xFBti, kad \xE1terpiamo burbulo plotis = 0, ir kad j\xE1 \xE1terpti reikia 0 kart\xF8 (dummy burbulas). 
 * Tokiu atveju vienintelis efektas bus toks, 
 * kad nukopijuos signalo duomenis iki pirmojo burbulo vidurio 
 * ir atnaujins einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes (to reikia ptk ilginimui).
 *
 * Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int iterpti_burbula (struct burbulas * burbulas, struct tkontekstas * kontekstas)
{
	// jei pavyko == -1, visk\xE0 stabdome, nes nepavyko i\xF0skirti atminties
	int pavyko = 0;

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo vidurio
	// Tuo pa\xE8iu ir atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes.
	pavyko = kopijuoti_signala (burbulas->vidurys, kontekstas);
	// jei nepavyko, visk\xE0 stabdome
	if (pavyko == -1) return -1;

	// burbulo kair\xEBs dalies plotis
	int burbulo_kaires_plotis 
		= burbulas->vidurys - burbulas->pradzia;

	// burbulo de\xF0in\xEBs dalies plotis
	int burbulo_desines_plotis 
		= burbulas->pabaiga - burbulas->vidurys;

	// tiek kart\xF8, kiek nurodyta burbule, nukopijuojame j\xE1 \xE1 nauj\xE0 signal\xE0
	for (int i = 0; i < burbulas->kartai; i++)
	{
		// apskai\xE8iuojame, koks bus "apdorojamos dalies" plotis
		// (t.y. tos dalies, kur sumuojame Hann langus.
		// Jei yra pik\xF8, tai tas plotis parodo, kur bus kitas pikas).
		// Jis tolygiai kinta nuo burbulo_kaires_plotis iki burbulo_desines_plotis.
		int einamasis_plotis 
			= ((burbulas->kartai - i) * burbulo_kaires_plotis 
			+ (i + 1) * burbulo_desines_plotis) / (burbulas->kartai + 1);

		// tono auk\xF0\xE8io keitimas: kei\xE8iame viet\xE0, kur bus kitas pikas (t.y. einamasis_plotis), 
		// priklausomai nuo tarpo tarp pik\xF8 didinimo koeficiento
		einamasis_plotis = (int) (einamasis_plotis * kontekstas->tarpo_tarp_piku_didinimo_koef);
		
		// apdorojame \xE1terpt\xE0j\xE0 dal\xE1 - tai bus dviej\xF8 Hann lang\xF8 suma
		
		// Hann lango de\xF0in\xEB pus\xEB - prisumuojame pirmojo burbulo kairi\xE0j\xE0 dal\xE1
		pavyko = Hann_lango_desine_puse (burbulas->vidurys, kontekstas->einamasis_naujo_signalo_nr, 
			burbulo_desines_plotis, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko == -1) return -1;
		
		// Hann lango kair\xEB pus\xEB - prisumuojame paskutiniojo burbulo de\xF0ini\xE0j\xE0 dal\xE1
		pavyko = Hann_lango_kaire_puse (burbulas->pradzia, 
			kontekstas->einamasis_naujo_signalo_nr + einamasis_plotis - burbulo_kaires_plotis, 
			burbulo_kaires_plotis, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko == -1) return -1;
		
		// atnaujiname einam\xE0sias signal\xF8 masyv\xF8 reik\xF0mes
		kontekstas->einamasis_naujo_signalo_nr += einamasis_plotis;
		//kontekstas->einamasis_signalo_nr += 0; // nepasikei\xE8ia
		
		// perskai\xE8iuojame einam\xE0j\xE1 post\xFBm\xE1
		kontekstas->einamasis_postumis += einamasis_plotis;
	}

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: iterpti_burbula(): einamasis_postumis: %d", kontekstas->einamasis_postumis);
		spausdinti_loga (pranesimas);
	}

	return 0;
}

/*********************************************************
 * Ilgina signal\xE0, perskai\xE8iuodamas visus reikiamus masyvus.
 *
 * Gr\xE0\xFEina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int ilginti_fonema (struct tkontekstas * kontekstas)
{
	for (int burbulo_nr=0; burbulo_nr < kontekstas->keiciamu_burbulu_sk; burbulo_nr++) {
		int pavyko = iterpti_burbula (kontekstas->burbulai+burbulo_nr, kontekstas);
		// jei nepavyko, visk\xE0 stabdome
		if (pavyko == -1) return -1;
	}

	return 0;
}
