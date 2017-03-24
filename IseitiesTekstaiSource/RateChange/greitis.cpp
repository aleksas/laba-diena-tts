///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas greitis.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

/*********************************************************
 * Globalûs kiti kintamieji
 ********************************************************/

// Greitinimo koeficientas (kiek kartø turi pailgëti signalas)
// TODO: nuskaityti is programos komandines eilutes parametru
//double greitinimo_koef = 1.49;

// numeris, áeinantis á naujai sukuriamø signalo ir anotacijø failø pavadinimus
int bandymo_numeris = 102;

// Scenarijaus numeris
// TODO: nuskaityti is programos komandines eilutes parametru
// 1. Iðmesti/dubliuoti: skardiems viduriná burbulà, dusliems intervalà tarp 30 ir 50% garso.
// 2. Kaip 1, bet skardþiuosius sutrumpina maksimaliai
// 3. Visus trumpinti/ilginti vienodai (tiek kartø, kiek liepia greitinimo_koef)
// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
// (balsius labiausiai, kitus maþiau, sprogstamuosius dar maþiau, r nekeisti)
// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas 
// [2015-01-12: nuo 7 versijos 5 scenarijus nebeveikia, nes funkcija nuskaityti_anotacijas() pakeista dël to, kad pasikeitë anotacijø failo formatas]
const int scenarijus = 3;

// Scenarijaus 5 koeficientas ið intervalo [0,1]: 
// jei didelis (arti vieneto), tai stengsis patrumpinti/pailginti iki vidutinio ilgio, 
// jei maþas (arti nulio) - maþiau stengsis pasiekti vidutiná ilgá
// TODO: nuskaityti is programos komandines eilutes parametru
double scenarijaus5_koeficientas = 1;

// Tai veiksmas, kurá programa atlieka. Kitaip tariant, programos veikimo reþimas. 
// Ðiuo metu realizuoti keturi reþimai.
// 1. Atlieka duoto signalo failo (signalo_failo_pavadinimas ir kt.) lëtinimà/greitinimà 
// pagal nurodytà scenarijø su nurodytu greitinimo koeficientu
// 2. Atlieka duoto signalo failo (signalo_failo_pavadinimas ir kt.) lëtinimà/greitinimà
// pagal nurodytà scenarijø su ávairiais greitinimo koeficientais 
// (jø pradinæ ir galutinæ reikðmes bei þingsná galima nustatyti funkcijoje testas, 
// rezultatø á failus neáraðo, tik iðveda á ekranà gautà faktiná sulëtëjimà/pagreitëjimà)
// 3. Randa fonemø ilgiø vidurkius 
// (fonemø ilgiø katalogas nurodomas kintamajame "katalogas" funkcijoje rasti_fonemu_ilgiu_vidurkius ())
// 4. Konvertuoja visus nurodytus signalo failus su nurodytais greitinimo koeficientais. 
const int veiksmas = 2;

// skirtumas tarp to, koks plotis buvo panaudotas rezultatuose, ir koks duomenyse.
// Bus neigiamas, jei signalo rezultatas sutrumpëjo, ir teigiamas, jei pailgëjo.
// Per tiek padidës fonemø ilgiai
//int einamasis_postumis = 0;

//size_t fonemos_nr_debug = 0;

// jei ne nulis, iðveda papildomà informacijà apie programos darbà
// jei > 100, iðveda detalià informacijà
//int debuginam = 101;

/*********************************************************
 * Globalûs signalo (duomenø) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char signalo_failo_pavadinimas[256] = "db.raw";

// garso signalo masyvas, iðskiriamas dinamiðkai
short * signalas = NULL;

// garso signalo masyvo ilgis
size_t signalo_ilgis = 0;

/*********************************************************
 * Globalûs naujojo signalo (rezultato) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char * naujo_signalo_failo_pavadinimas = "10m2e-new.16le";
char * naujo_signalo_failo_pavadinimo_pradzia = "rez\\10m2e-new";
char * naujo_signalo_failo_pavadinimo_pabaiga = ".16le";

/*********************************************************
 * Globalûs fonemø (duomenø) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char fonemu_failo_pavadinimas[256] = "db_fon_weights.txt";

// fonemø pavadinimø masyvas
char ** fonemos = NULL;

// fonemø ilgiø masyvas
int * fonemu_ilgiai = NULL;

// fonemø kiekis (fonemø masyvo ir fonemø ilgiø masyvo ilgis)
size_t fonemu_kiekis = 0;

/*********************************************************
 * Globalûs naujieji fonemø (rezultatø) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char * naujo_fonemu_failo_pavadinimas = "10m3_ilg-new.txt";
char * naujo_fonemu_failo_pavadinimo_pradzia = "rez\\10m3_ilg-new";
char * naujo_fonemu_failo_pavadinimo_pabaiga = ".txt";

// fonemø ilgiø masyvas
int * nauji_fonemu_ilgiai = NULL;

/*********************************************************
 * Globalûs duomenø kintamieji, susijæ su vidutiniu fonemø ilgiu (reikalingi kai kuriems scenarijams)
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru?
char vidutiniu_ilgiu_fonemu_failo_pavadinimas[256] = "vidutiniai_fonemu_ilgiai.txt";

// fonemø pavadinimø masyvas
char ** skirtingos_fonemos = NULL;

// fonemø ilgiø masyvas
int * vidutiniai_fonemu_ilgiai = NULL;

// fonemø kiekis (fonemø masyvo ir fonemø ilgiø masyvo ilgis)
size_t skirtingu_fonemu_kiekis = 0;
		
/*********************************************************
 * Globalûs pikø (duomenø) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char piku_failo_pavadinimas[256] = "db_pik.txt";

// pikø masyvas (pikai[0] yra pirmojo piko vieta "signalas" masyve, t.y. masyvo "signalas" indeksas)
unsigned int * pikai = NULL;

// pikø kiekis (pikø masyvo ilgis)
size_t piku_kiekis = 0;

/*********************************************************
 * Burbulai
 ********************************************************/

// Euristika nustatys, kuriuos burbuliukus reikia paðalinti ar dubliuoti. 
// Jø sàraðà pateiks ðiame kintamajame "burbulai". 
// Kad nereikëtø pastoviai iðskirinëti jam atminties, 
// èia iðskiriame vienà kartà ir pastoviai naudojame.
// TODO: pastoviai tikrinti, ar nevirðijo MAX_BURBULAI. Virðijus kaþkà daryti.
//struct burbulas burbulai[MAX_KEICIAMI_BURBULAI];

/*********************************************************
Iðveda klaidos praneðimà á stderr.
 ********************************************************/
void klaida (char * klaidos_pranesimas)
{
	char pranesimas [1024];
	sprintf (pranesimas, "Klaida: %s", klaidos_pranesimas);
	spausdinti_loga (pranesimas);
}

/*********************************************************
Uþtikrina, kad masyvo naujas_signalas ilgis yra ne maþesnis, 
nei reikiamas_ilgis.

Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int pailginti_masyva_naujas_signalas (size_t reikiamas_ilgis, struct tkontekstas * kontekstas)
{
	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): reikia pailginti nuo %d iki %d", 
			kontekstas->naujo_signalo_masyvo_ilgis, reikiamas_ilgis);
		spausdinti_loga (pranesimas);
	}

	// jei negalima ilginti - baigiam darbà
	if (kontekstas->galima_pailginti_naujas_signalas == 0)
		return -1;

	// siulomas_naujas_ilgis - naujas ilgis, jei pailgintume pagal REALLOC_COEFF
	size_t siulomas_naujas_ilgis = (size_t) (kontekstas->naujo_signalo_masyvo_ilgis * REALLOC_COEFF);

	// galutinis naujas ilgis bus didesnysis ið siulomas_naujas_ilgis ir reikiamas_ilgis
	size_t naujas_ilgis;
	if (siulomas_naujas_ilgis < reikiamas_ilgis)
		naujas_ilgis = reikiamas_ilgis;
	else
		naujas_ilgis = siulomas_naujas_ilgis;

	// iðskiriame atminties
	short * naujas_naujas_signalas 
		= (short *) realloc ((void *)kontekstas->naujas_signalas, naujas_ilgis*sizeof(short));

	// jei nepavyko iðskirti atminties, baigiam darbà
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

	// uþnuliname naujai iðskirtà (papildomà) atmintá
	for (size_t i = kontekstas->naujo_signalo_masyvo_ilgis; i < naujas_ilgis; i++)
		naujas_naujas_signalas[i] = 0;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): sekmingai pailginta nuo %d iki %d", 
			kontekstas->naujo_signalo_masyvo_ilgis, naujas_ilgis);
		spausdinti_loga (pranesimas);
	}

	// ásimename naujàjá masyvà ir jo ilgá
	kontekstas->naujas_signalas = naujas_naujas_signalas;
	kontekstas->naujo_signalo_masyvo_ilgis = naujas_ilgis;

	return 0;
}

/*********************************************************
 * Hann lango kairë pusë (didëjanti).
 * Nukopijuoja ið signalo masyvo nuo pradzia
 * á naujàjá signalo masyvà nuo nauja_pradzia
 * burbulo kairiàjà pusæ ploèio lango_plotis.
 *
 * nauja_pradzia gali bûti ir neigiamas, nes tono keitimo atveju galime raðyti prieð masyvo pradþià.
 *
 * Gràþina 0, jei pavyko, ir -1, jei nepavyko.
********************************************************/
int Hann_lango_kaire_puse (int pradzia, int nauja_pradzia, int lango_plotis, struct tkontekstas * kontekstas)
{
	// taikome Hann langà tik tuo atveju, kai lango_plotis > 1 (kad nebûtø dalybos ið nulio ir kitokiø problemø)
	if (lango_plotis <= 1)
		return 0;

	// tikriname, ar reikia ilginti naujo signalo masyvà

	// nustatome, kokio ilgio turëtø bûti masyvas naujas_signalas
	// (gali bûti ir neigiamas, jei nauja_pradzia < 0 ir |nauja_pradzia| > lango_plotis)
	int reikiamas_ilgis = (size_t) (nauja_pradzia + lango_plotis);

	// jei reikiamas ilgis virðija dabartiná
	if (reikiamas_ilgis > (int) kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyvà
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// þingsnio ilgis
	// TODO: iðsiaiðkinti: gal zingsnis = pi / lango_plotis; ?
	double zingsnis = pi / (lango_plotis - 1);

	// taikome Hann langà
	for (int i=0; i < lango_plotis; i++)
		kontekstas->naujas_signalas [nauja_pradzia + i] += (short)
			(signalas [pradzia + i] * 0.5 * (1 - cos (zingsnis * i)));

	return 0;
}

/*********************************************************
 * Hann lango deðinë pusë (maþëjanti).
 * Nukopijuoja ið signalo masyvo nuo pradzia
 * á naujàjá signalo masyvà nuo nauja_pradzia
 * burbulo deðinæ pusæ ploèio lango_plotis
 *
 * Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int Hann_lango_desine_puse (int pradzia, int nauja_pradzia, int lango_plotis, struct tkontekstas * kontekstas)
{
	// taikome Hann langà tik tuo atveju, kai lango_plotis > 1 (kad nebûtø dalybos ið nulio ir kitokiø problemø)
	if (lango_plotis <= 1)
		return 0;

	// tikriname, ar reikia ilginti naujo signalo masyvà

	// nustatome, kokio ilgio turëtø bûti masyvas naujas_signalas
	size_t reikiamas_ilgis = (size_t) (nauja_pradzia + lango_plotis);

	// jei reikiamas ilgis virðija dabartiná
	if (reikiamas_ilgis > kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyvà
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// þingsnio ilgis
	// TODO: iðsiaiðkinti: gal zingsnis = pi / lango_plotis; ?
	double zingsnis = pi / (lango_plotis - 1);

	// taikome Hann langà
	for (int i=0; i < lango_plotis; i++)
		kontekstas->naujas_signalas [nauja_pradzia + i] += (short)
			(signalas [pradzia + i] * 0.5 * (1 + cos (zingsnis * i)));

	return 0;
}

/*********************************************************
 * Spausdinti informacijà apie duotus burbulus
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
 * tono aukðèio keitimas: apdorojame pusæ pirmo burbulo (dalá, kertanèià fonemos ribà).
 * Laikoma, kad fonema skardþioji, turi pakankamai pikø (piku_sk > 1) ir tono aukðtá keisti reikia.
 ********************************************************/
int kopijuoti_signala_pradzioj (struct tkontekstas * kontekstas)
{
	// randame pirmojo burbulo kairiosios dalies plotá.

	// pirmojo burbulo deðiniosios dalies plotis - kairioji dalis bus ne platesnë
	unsigned int burbulo_desines_plotis = pikai [kontekstas->pirmojo_piko_nr + 1] - pikai [kontekstas->pirmojo_piko_nr];
	
	// Formuojant ant fonemos ribos esantá burbulà, turëti omenyje, kad uþ ribos gali visai nebûti pikø. 
	// Algoritmas bûtø toks: tegu X yra burbulo kitos dalies plotis. 
	// Jei atstumu <= X nuo burbulo centro yra kitas pikas (kitoje fonemoje), tai jis ir bus burbulo kraðtas, 
	// o jei ne, tai formuoti simetriðkà burbulà (t.y. imti burbulo kraðtà atstumu X nuo centro).

	// jei prieð tai yra pikas, ir jis nelabai toli nuo pirmojo fonemos piko, tai ir bus burbulo pradþia. 
	// Jei ne, burbulo pradþia bus nutolusi nuo burbulo centro (pirmojo fonemos piko) tokiu pat atstumu, kaip pabaiga.
	int burbulo_pradzia = 0;
	if (kontekstas->pirmojo_piko_nr > 0 && pikai [kontekstas->pirmojo_piko_nr]-pikai[kontekstas->pirmojo_piko_nr-1] <= burbulo_desines_plotis)
		burbulo_pradzia = pikai[kontekstas->pirmojo_piko_nr-1];
	else
		burbulo_pradzia = pikai[kontekstas->pirmojo_piko_nr] - burbulo_desines_plotis;

	// pirmojo burbulo kairiosios dalies plotis
	int burbulo_kaires_plotis = pikai[kontekstas->pirmojo_piko_nr] - burbulo_pradzia;
	
	// tono aukðèio keitimas: apskaièiuojame, kiek dël tono aukðèio keitimo turi pasislinkti pikas
	// (jei neigiama reikðmë, slinksis á kairæ, jei teigiama, á deðinæ).
	// Kitaip tariant, tiek pasikeis naujo signalo ilgis originalaus signalo ilgio atþvilgiu
	int einamasis_postumis = (int) (burbulo_kaires_plotis * (kontekstas->tarpo_tarp_piku_didinimo_koef - 1));
	
	// jei pavyko == -1, viskà stabdome, nes nepavyko iðskirti atminties
	int pavyko = 0;
	// Hann lango kairë pusë - prisumuojame pirmojo burbulo kairiàjà dalá
	pavyko = Hann_lango_kaire_puse (burbulo_pradzia, 
		kontekstas->einamasis_naujo_signalo_nr - (kontekstas->fonemos_pradzia - burbulo_pradzia) + einamasis_postumis, 
		burbulo_kaires_plotis, kontekstas);
	// jei nepavyko, viskà stabdome
	if (pavyko == -1) return -1;
	
	// atnaujiname einamàsias signalø masyvø reikðmes
	kontekstas->einamasis_naujo_signalo_nr += + burbulo_kaires_plotis + einamasis_postumis - (kontekstas->fonemos_pradzia - burbulo_pradzia);
	kontekstas->einamasis_signalo_nr = pikai [kontekstas->pirmojo_piko_nr];

	// perskaièiuojame einamàjá postûmá
	kontekstas->einamasis_postumis += einamasis_postumis;

	return 0;
}

/*********************************************************
 * tono aukðèio keitimas: apdorojame pusæ paskutinio burbulo (dalá, kertanèià fonemos ribà).
 * Laikoma, kad fonema skardþioji, turi pakankamai pikø (piku_sk > 1) ir tono aukðtá keisti reikia.
 ********************************************************/
int kopijuoti_signala_pabaigoj (struct tkontekstas * kontekstas)
{
	// randame paskutiniojo burbulo deðiniosios dalies plotá.

	// paskutiniojo piko numeris
	size_t paskutinio_piko_nr = kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1;

	// paskutiniojo burbulo kairiosios dalies plotis - deðinioji dalis bus ne platesnë
	unsigned int burbulo_kaires_plotis = pikai [paskutinio_piko_nr] - pikai [paskutinio_piko_nr - 1];
	
	// Formuojant ant fonemos ribos esantá burbulà, turëti omenyje, kad uþ ribos gali visai nebûti pikø. 
	// Algoritmas bûtø toks: tegu X yra burbulo kitos dalies plotis. 
	// Jei atstumu <= X nuo burbulo centro yra kitas pikas (kitoje fonemoje), tai jis ir bus burbulo kraðtas, 
	// o jei ne, tai formuoti simetriðkà burbulà (t.y. imti burbulo kraðtà atstumu X nuo centro).

	// jei paskui yra pikas, ir jis nelabai toli nuo paskutinio fonemos piko, tai ir bus burbulo pabaiga. 
	// Jei ne, burbulo pabaiga bus nutolusi nuo burbulo centro (paskutinio fonemos piko) tokiu pat atstumu, kaip pradþia.
	int burbulo_pabaiga = 0;
	if (paskutinio_piko_nr + 1 < piku_kiekis && pikai [paskutinio_piko_nr + 1]-pikai [paskutinio_piko_nr] <= burbulo_kaires_plotis)
		burbulo_pabaiga = pikai [paskutinio_piko_nr + 1];
	else
		burbulo_pabaiga = pikai [paskutinio_piko_nr] + burbulo_kaires_plotis;

	// paskutiniojo burbulo deðiniosios dalies plotis
	int burbulo_desines_plotis = burbulo_pabaiga - pikai [paskutinio_piko_nr];
	
	// jei pavyko == -1, viskà stabdome, nes nepavyko iðskirti atminties
	int pavyko = 0;
	// Hann lango deðinë pusë - prisumuojame paskutiniojo burbulo deðiniàjà dalá
	pavyko = Hann_lango_desine_puse (pikai [paskutinio_piko_nr], 
		kontekstas->einamasis_naujo_signalo_nr, 
		burbulo_desines_plotis, kontekstas);
	// jei nepavyko, viskà stabdome
	if (pavyko == -1) return -1;
	
	// atnaujiname einamàsias signalø masyvø reikðmes
	kontekstas->einamasis_naujo_signalo_nr += kontekstas->fonemos_pabaiga - pikai [paskutinio_piko_nr];
	kontekstas->einamasis_signalo_nr = kontekstas->fonemos_pabaiga;

	// perskaièiuojame einamàjá postûmá (ðiuo atveju - nepasikeièia)
	kontekstas->einamasis_postumis += 0;

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo paèiu ir atnaujiname einamàsias signalø masyvø indeksø reikðmes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala_keiciant_tono_auksti (size_t iki, struct tkontekstas * kontekstas)
{
	// nieko nedarome, jei nëra kà kopijuoti
	if (iki <= kontekstas->einamasis_signalo_nr)
		return 0;

	// surandame einamojo piko (nuo kurio reikës viskà kopijuoti) numerá
	unsigned int pradzios_piko_nr = kontekstas->pirmojo_piko_nr;
	while (pradzios_piko_nr < kontekstas->pirmojo_piko_nr + kontekstas->piku_sk && pikai [pradzios_piko_nr] < kontekstas->einamasis_signalo_nr) 
		pradzios_piko_nr++;
	if (pikai [pradzios_piko_nr] != kontekstas->einamasis_signalo_nr) {
		// taip negali bûti
		if (debuginam) {
			spausdinti_loga ("!!! Internal error 1 in function kopijuoti_signala_keiciant_tono_auksti() !!!");
			spausdinti_konteksta (kontekstas);
		}
	}

	// surandame paskutinio piko, iki kurio reikës viskà kopijuoti, numerá
	unsigned int pabaigos_piko_nr = pradzios_piko_nr;
	while (pabaigos_piko_nr < kontekstas->pirmojo_piko_nr + kontekstas->piku_sk && pikai [pabaigos_piko_nr] < iki) 
		pabaigos_piko_nr++;
	if (pikai [pabaigos_piko_nr] != iki) {
		// taip negali bûti
		if (debuginam) {
			spausdinti_loga ("!!! Internal error 2 in function kopijuoti_signala_keiciant_tono_auksti() !!!");
			spausdinti_konteksta (kontekstas);
		}
	}

	// kopijuojame tarpus tarp visø pikø nuo pradzios_piko_nr iki pabaigos_piko_nr

	for (unsigned int piko_nr = pradzios_piko_nr; piko_nr < pabaigos_piko_nr; piko_nr++) {
		
		// jei pavyko == -1, viskà stabdome, nes nepavyko iðskirti atminties
		int pavyko = 0;

		// tarpas tarp piku piko_nr ir piko_nr+1
		int tarpas_tarp_piku = pikai [piko_nr+1] - pikai [piko_nr];

		// Hann lango deðinë pusë
		pavyko = Hann_lango_desine_puse (pikai [piko_nr], kontekstas->einamasis_naujo_signalo_nr, 
			tarpas_tarp_piku, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko == -1) return -1;
		
		// tono aukðèio keitimas: apskaièiuojame, kiek dël tono aukðèio keitimo turi pasislinkti pikas
		// (jei neigiama reikðmë, slinksis á kairæ, jei teigiama, á deðinæ).
		// Kitaip tariant, tiek pasikeis naujo signalo ilgis originalaus signalo ilgio atþvilgiu
		int einamasis_postumis = (int) (tarpas_tarp_piku * (kontekstas->tarpo_tarp_piku_didinimo_koef - 1));
		
		// Hann lango kairë pusë
		pavyko = Hann_lango_kaire_puse (pikai [piko_nr], 
			kontekstas->einamasis_naujo_signalo_nr + einamasis_postumis, 
			tarpas_tarp_piku, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko == -1) return -1;
		
		// atnaujiname einamàsias signalø masyvø reikðmes
		kontekstas->einamasis_naujo_signalo_nr += tarpas_tarp_piku + einamasis_postumis;
		kontekstas->einamasis_signalo_nr = pikai [piko_nr+1];
		
		// perskaièiuojame einamàjá postûmá
		kontekstas->einamasis_postumis += einamasis_postumis;
		
	}

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo paèiu ir atnaujiname einamàsias signalø masyvø indeksø reikðmes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala_nekeiciant_tono_aukscio (size_t iki, struct tkontekstas * kontekstas)
{
	// tikriname, ar reikia ilginti naujo signalo masyvà

	// nustatome, kokio ilgio turëtø bûti masyvas naujas_signalas
	size_t reikiamas_ilgis = kontekstas->einamasis_naujo_signalo_nr;
	if (iki > kontekstas->einamasis_signalo_nr)
		reikiamas_ilgis += iki - kontekstas->einamasis_signalo_nr;

	// jei reikiamas ilgis virðija dabartiná
	if (reikiamas_ilgis > kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyvà
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo pradþios
	// (negalime kopijuoti su memcpy, nes prarasime jau ten esanèià informacijà).
	// Tuo paèiu ir atnaujiname einamàsias signalø masyvø reikðmes.
	for (; kontekstas->einamasis_signalo_nr < iki; 
			kontekstas->einamasis_signalo_nr++, kontekstas->einamasis_naujo_signalo_nr++)
		kontekstas->naujas_signalas[kontekstas->einamasis_naujo_signalo_nr] += signalas[kontekstas->einamasis_signalo_nr];
	//memcpy (kontekstas->naujas_signalas + kontekstas->einamasis_naujo_signalo_nr, 
	//	signalas + kontekstas->einamasis_signalo_nr, 
	//	(pirmas_burbulas->pradzia - kontekstas->einamasis_signalo_nr) * sizeof (short));

	// atnaujiname einamàsias signalø masyvø reikðmes
	//kontekstas->einamasis_naujo_signalo_nr += pirmas_burbulas->pradzia - kontekstas->einamasis_signalo_nr;
	//kontekstas->einamasis_signalo_nr = pirmas_burbulas->pradzia;

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo paèiu ir atnaujiname einamàsias signalø masyvø indeksø reikðmes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala (size_t iki, struct tkontekstas * kontekstas)
{
	if (kontekstas->keisti_tono_auksti)
		// tono aukðèio keitimas: kopijuojame, keisdami tono aukðtá
		return kopijuoti_signala_keiciant_tono_auksti (iki, kontekstas);
	else
		// tiesiog kopijuojame (prisumuojame signalà)
		return kopijuoti_signala_nekeiciant_tono_aukscio (iki, kontekstas);
}

/*********************************************************
 * Iðmeta burbulø grandinæ. 
 * Tiksliau, nukopijuoja duomenis ið signalo masyvo á naujà masyvà, 
 * "iðmesdamas" (nenukopijuodamas) visus grandinës burbulus.
 *
 * Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int ismesti_burbulu_grandine (struct burbulas * pirmas_burbulas,
							   struct burbulas * paskutinis_burbulas, struct tkontekstas * kontekstas)
{
	// jei pavyko == -1, viskà stabdome, nes nepavyko iðskirti atminties
	int pavyko = 0;

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo pradþios
	// Tuo paèiu ir atnaujiname einamàsias signalø masyvø reikðmes.
	pavyko = kopijuoti_signala (pirmas_burbulas->pradzia, kontekstas);
	// jei nepavyko, viskà stabdome
	if (pavyko == -1) return -1;

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: ismesti_burbulu_grandine(): einamasis_signalo_nr: %d, einamasis_naujo_signalo_nr: %d", 
			kontekstas->einamasis_signalo_nr, kontekstas->einamasis_naujo_signalo_nr);
		spausdinti_loga (pranesimas);
	}

	// pirmojo burbulo kairës dalies plotis
	int pirmojo_burbulo_kaires_plotis 
		= pirmas_burbulas->vidurys - pirmas_burbulas->pradzia;

	// paskutiniojo burbulo deðinës dalies plotis
	int paskutinio_burbulo_desines_plotis 
		= paskutinis_burbulas->pabaiga - paskutinis_burbulas->vidurys;

	// apdorojame iðmestàjà dalá - tai bus dviejø Hann langø suma

	// Hann lango deðinë pusë - prisumuojame pirmojo burbulo kairiàjà dalá
	pavyko = Hann_lango_desine_puse (pirmas_burbulas->pradzia, kontekstas->einamasis_naujo_signalo_nr, 
		pirmojo_burbulo_kaires_plotis, kontekstas);
	// jei nepavyko, viskà stabdome
	if (pavyko == -1) return -1;

	// apskaièiuojame, koks bus "apdorojamos dalies" plotis
	// (t.y. tos dalies, kur sumuojame Hann langus.
	// Jei yra pikø, tai tas plotis parodo, kur bus kitas pikas).
	// TODO: Ðiuo metu ploèiu laikau pirmojo burbulo kairiosios dalies ir 
	// paskutiniojo burbulo deðiniosios dalies ploèiø vidurká.
	int einamasis_plotis = (pirmojo_burbulo_kaires_plotis + paskutinio_burbulo_desines_plotis)/2;

	// tono aukðèio keitimas: keièiame vietà, kur bus kitas pikas (t.y. einamasis_plotis), 
	// priklausomai nuo tarpo tarp pikø didinimo koeficiento
	einamasis_plotis = (int) (einamasis_plotis * kontekstas->tarpo_tarp_piku_didinimo_koef);
		
	// Hann lango kairë pusë - prisumuojame paskutiniojo burbulo deðiniàjà dalá
	pavyko = Hann_lango_kaire_puse (paskutinis_burbulas->vidurys, 
		kontekstas->einamasis_naujo_signalo_nr + einamasis_plotis - paskutinio_burbulo_desines_plotis, 
		paskutinio_burbulo_desines_plotis, kontekstas);
	// jei nepavyko, viskà stabdome
	if (pavyko == -1) return -1;

	// atnaujiname einamàsias signalø masyvø reikðmes
	kontekstas->einamasis_naujo_signalo_nr += einamasis_plotis;
	kontekstas->einamasis_signalo_nr = paskutinis_burbulas->pabaiga;

	// perskaièiuojame einamàjá postûmá
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
 * Trumpina signalà, perskaièiuodamas visus reikiamus masyvus.
 *
 * Burbulai iðmetimui turi bûti pateikti surûðiuoti ið eilës.
 *
 * Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int trumpinti_fonema (struct tkontekstas * kontekstas)
{
	// burbulø grandinës (ið eilës einanèiø burbulø sekos) apdorojimas 
	// skiriasi nuo pavieniø burbulø apdorojimo, 
	// todël turime rasti tokias grandines. 
	// Tiksliau, apdorosime duomenis burbulø grandinëmis, 
	// kurios gali bûti ir tik vieno burbulo ilgio.

	// einamojo burbulo numeris
	int burbulo_nr = 0;

	// kol turime neapdorotø burbulø
	while (burbulo_nr < kontekstas->keiciamu_burbulu_sk)
	{
		// iðsaugome pradiná grandinës burbulà
		struct burbulas * pirmas_burbulas = kontekstas->burbulai+burbulo_nr;

		// ieðkome paskutinio grandinës burbulo.

		// Burbulas nebus paskutinis burbulø grandinës burbulas, jei:
		// 1) jis nebus iðvis paskutinis burbulas, ir
		while (burbulo_nr < kontekstas->keiciamu_burbulu_sk - 1 
		// 2) kitas burbulas eina iðkart uþ jo
		// (Burbulas B eina iðkart uþ burbulo A, jei B.pradzia == A.vidurys)
		// TODO: gal dar reikia tikrinti, ar B.vidurys == A.pabaiga? Ir kà daryti, jei ne?
			&& kontekstas->burbulai[burbulo_nr].vidurys == kontekstas->burbulai[burbulo_nr+1].pradzia)
			burbulo_nr++;

		// iðsaugome paskutiná grandinës burbulà
		struct burbulas * paskutinis_burbulas = kontekstas->burbulai+burbulo_nr;

		// iðmetame burbulø grandinæ
		int pavyko = ismesti_burbulu_grandine (pirmas_burbulas, paskutinis_burbulas, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko == -1) return -1;

		// einame prie naujo burbulo
		burbulo_nr++;
	}

	return 0;
}

/*********************************************************
 * Iterpia duotà burbulà tiek kartø, kiek jame nurodyta. 
 * Tiksliau, nukopijuoja duomenis ið signalo masyvo á naujà masyvà, 
 * dubliuodamas duotà burbulà tiek kartø, kiek jame nurodyta.
 *
 * Gali bûti, kad áterpiamo burbulo plotis = 0, ir kad já áterpti reikia 0 kartø (dummy burbulas). 
 * Tokiu atveju vienintelis efektas bus toks, 
 * kad nukopijuos signalo duomenis iki pirmojo burbulo vidurio 
 * ir atnaujins einamàsias signalø masyvø reikðmes (to reikia ptk ilginimui).
 *
 * Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int iterpti_burbula (struct burbulas * burbulas, struct tkontekstas * kontekstas)
{
	// jei pavyko == -1, viskà stabdome, nes nepavyko iðskirti atminties
	int pavyko = 0;

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo vidurio
	// Tuo paèiu ir atnaujiname einamàsias signalø masyvø reikðmes.
	pavyko = kopijuoti_signala (burbulas->vidurys, kontekstas);
	// jei nepavyko, viskà stabdome
	if (pavyko == -1) return -1;

	// burbulo kairës dalies plotis
	int burbulo_kaires_plotis 
		= burbulas->vidurys - burbulas->pradzia;

	// burbulo deðinës dalies plotis
	int burbulo_desines_plotis 
		= burbulas->pabaiga - burbulas->vidurys;

	// tiek kartø, kiek nurodyta burbule, nukopijuojame já á naujà signalà
	for (int i = 0; i < burbulas->kartai; i++)
	{
		// apskaièiuojame, koks bus "apdorojamos dalies" plotis
		// (t.y. tos dalies, kur sumuojame Hann langus.
		// Jei yra pikø, tai tas plotis parodo, kur bus kitas pikas).
		// Jis tolygiai kinta nuo burbulo_kaires_plotis iki burbulo_desines_plotis.
		int einamasis_plotis 
			= ((burbulas->kartai - i) * burbulo_kaires_plotis 
			+ (i + 1) * burbulo_desines_plotis) / (burbulas->kartai + 1);

		// tono aukðèio keitimas: keièiame vietà, kur bus kitas pikas (t.y. einamasis_plotis), 
		// priklausomai nuo tarpo tarp pikø didinimo koeficiento
		einamasis_plotis = (int) (einamasis_plotis * kontekstas->tarpo_tarp_piku_didinimo_koef);
		
		// apdorojame áterptàjà dalá - tai bus dviejø Hann langø suma
		
		// Hann lango deðinë pusë - prisumuojame pirmojo burbulo kairiàjà dalá
		pavyko = Hann_lango_desine_puse (burbulas->vidurys, kontekstas->einamasis_naujo_signalo_nr, 
			burbulo_desines_plotis, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko == -1) return -1;
		
		// Hann lango kairë pusë - prisumuojame paskutiniojo burbulo deðiniàjà dalá
		pavyko = Hann_lango_kaire_puse (burbulas->pradzia, 
			kontekstas->einamasis_naujo_signalo_nr + einamasis_plotis - burbulo_kaires_plotis, 
			burbulo_kaires_plotis, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko == -1) return -1;
		
		// atnaujiname einamàsias signalø masyvø reikðmes
		kontekstas->einamasis_naujo_signalo_nr += einamasis_plotis;
		//kontekstas->einamasis_signalo_nr += 0; // nepasikeièia
		
		// perskaièiuojame einamàjá postûmá
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
 * Ilgina signalà, perskaièiuodamas visus reikiamus masyvus.
 *
 * Gràþina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int ilginti_fonema (struct tkontekstas * kontekstas)
{
	for (int burbulo_nr=0; burbulo_nr < kontekstas->keiciamu_burbulu_sk; burbulo_nr++) {
		int pavyko = iterpti_burbula (kontekstas->burbulai+burbulo_nr, kontekstas);
		// jei nepavyko, viskà stabdome
		if (pavyko == -1) return -1;
	}

	return 0;
}
