///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas RateChange.h
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef RATECHANGEINTERNAL_H
#define RATECHANGEINTERNAL_H

/*********************************************************
 * Konstantos
 ********************************************************/

const double pi = 3.14159265359;

// Koks did\xFEiausias gali b\xFBti atstumas tarp pik\xF8 reguliariame garse.
// Jei atstumas tarp pik\xF8 arba tarp piko ir garso ribos yra didesnis, 
// tai ka\xFEkas negerai, tod\xEBl negalime tam garsui trumpinti naudoti pik\xF8
// Laikome, kad garsas negali b\xFBti \xFEemesnis u\xFE 50 Hz, 
// t.y atstumai tarp pik\xF8 negali b\xFBti didesni u\xFE 1/50 s, 
// t.y. atstumai tarp pik\xF8 indeks\xF8 garso masyve negali b\xFBti didesni u\xFE 22050/50=441.
#define MAX_ATSTUMAS_TARP_PIKU 441 

// koeficientas realloc funkcijai masyvo naujas_signalas pailginimui. 
// Tiek kart\xF8 bus pailgintas masyvas naujas_signalas.
// naujas_ilgis = REALLOC_COEFF * senas_ilgis, jei naujas_ilgis pakankamas
#define REALLOC_COEFF 1.2

// koeficientas, parodantis, kiek kart\xF8 daugiau reikia i\xF0skirti atminties 
// naujo signalo masyvui, negu apytikslis \xE1vertis (d\xEBl visa ko)
#define NAUJO_SIGNALO_MASYVO_ILGIO_KOEF 1.1f

// nurodo signalo masyvo indeks\xF8 skai\xE8iumi, kiek b\xFBtina palikti leistinosios srities,  
// trumpinant dusliuosius priebalsius.
// Gali b\xFBti nulis. Normalu - apie 50.
// Nerekomenduojama > 200, nes tada kai kuri\xF8 fonem\xF8 visai nesutrumpins.
#define ISSAUGOTI_GALIMOS_SRITIES_ILGIO 100

// jei n\xEBra pik\xF8, imam burbul\xE0 ne pagal pikus, o simetri\xF0k\xE0 2*PUSE_BURBULO_ILGIO ilgio burbul\xE0
// TODO: ar naudoti ka\xFEkoki\xE0 konstant\xE0, ar pabandyti apsiskai\xE8iuoti burbulo ilg\xE1 i\xF0 signalo?
//#define PUSE_BURBULO_ILGIO 240

// kadangi naujos fonemos ilgis gali b\xFBti ir neigiamas, klaidos kodui gr\xE0\xFEinti reikia didel\xEBs neigiamos reik\xF0m\xEBs
#define DIDELIS_NEIGIAMAS_KLAIDOS_KODAS -10000

/*********************************************************
Fonem\xF8 klasi\xF8 numeriai pagal fonemos pavadinimo pirm\xE0j\xE0 raid\xE6:
0 - turintys pik\xF8 informacij\xE0 (skardieji priebalsiai, balsiai, t.y. visi, i\xF0skyrus x, f, p, t, k, s, S, _, r, R, z, Z, H)
1 - neturintys pik\xF8 informacijos (duslieji priebalsiai, t.y. x, f, p, t, k, s, S, _)
//2 - gali tur\xEBti ar netur\xEBti pik\xF8 informacijos, tod\xEBl gali b\xFBti priskirti kuriai nors i\xF0 pirm\xF8j\xF8 dviej\xF8 klasi\xF8 - reikia papildomo tikrinimo (z, Z, h),
3 - neai\xF0ku, k\xE0 daryti (r, R).
*********************************************************/
#define FONEMU_KLASE_SKARDIEJI 0
#define FONEMU_KLASE_DUSLIEJI 1
//#define FONEMU_KLASE_ZZH 2
#define FONEMU_KLASE_RR 3

/*********************************************************
 * Burbulai
 ********************************************************/

struct burbulas
{
	// burbulo koordinat\xEBs
	unsigned int pradzia;
	unsigned int vidurys;
	unsigned int pabaiga;

	// kiek kart\xF8 dubliuoti burbul\xE0
	int kartai;

	// ar burbulas sudarytas pik\xF8 pagrindu, 
	// t.y. ar jo koordinat\xEBs sutampa su pik\xF8 koordinat\xEBmis.
	// Jei reik\xF0m\xEB nelygi nuliui, tai pik\xF8 pagrindu.
	//int pikai;
};

// nurodo, kiek burbul\xF8 vienai fonemai gali tekti keisti (pa\xF0alinti ar dubliuoti)
#define MAX_KEICIAMI_BURBULAI 500

// kad nesigaut\xF8 periodinis signalas, dusli\xF8j\xF8 priebalsi\xF8 burbulai, jei juos reik\xEBs kartoti > 1 kart\xE0, gaminami nesimetri\xF0ki. 
// \xD0is parametras nurodo, kurioje burbulo vietoje bus jo centras (vidurys). 
// Jei = 0.0, tai vidurys sutaps su pradzia, jei = 1.0, sutaps su pabaiga, jei = 0.5, bus lygiai per vidur\xE1 tarp pradzios ir pabaigos. 
// Kad burbulas b\xFBt\xF8 nesimetri\xF0kas, rinktis reik\xF0m\xE6 != 0.5. 
//#define BURBULO_CENTRO_POZICIJA 0.50

/*********************************************************
 * Kontekstas
 * 
 * Kad sintezavimas veikt\xF8 daugelio gij\xF8 re\xFEimu 
 * (kai pakrauta viena gars\xF8 baz\xEB, ta\xE8iau funkcija change_phoneme_rate() i\xF0 RateChange.cpp gali b\xFBti vienu metu kvie\xE8iama i\xF0 skirting\xF8 gij\xF8), 
 * neturi b\xFBti globali\xF8 kintam\xF8j\xF8, \xE1 kuriuos ra\xF0oma sintezavimo metu 
 * (pastaba: gali b\xFBti global\xFBs kintamieji, \xE1 kuriuos ra\xF0oma gars\xF8 baz\xEBs u\xFEkrovimo metu). 
 * Visus tokius globalius kintamuosius, \xE1 kuriuos iki \xF0iol buvo ra\xF0oma sintezavimo metu, nuo \xF0iol reikia perduoti kaip funkcij\xF8 parametrus. 
 * Kadangi j\xF8 nema\xFEai, juos sudedu \xE1 vien\xE0 strukt\xFBr\xE0, kuri\xE0 ir perduosiu.
 ********************************************************/

struct tkontekstas {
	// einamosios fonemos numeris
	unsigned int fonemos_nr;

	// einamosios fonemos prad\xFEia ir pabaiga
	unsigned int fonemos_pradzia;
	unsigned int fonemos_pabaiga;

	// numeris pirmojo piko, esan\xE8io einamosios fonemos prad\xFEioje 
	// (tiksliau, pirmojo piko, nepriklausan\xE8io prie\xF0 tai buvusiai fonemai. 
	// Jis gali nepriklausyti ir einamajai, o kuriai nors tolimesnei).
	unsigned int pirmojo_piko_nr;

	// kiek pik\xF8 yra tarp fonemos prad\xFEios ir pabaigos
	unsigned int piku_sk;
	
	// Greitinimo koeficientas (kiek kart\xF8 turi pailg\xEBti signalas)
	double greitinimo_koef;

	// Tarpo tarp pik\xF8 didinimo koeficientas (kiek kart\xF8 turi padid\xEBti tarpas tarp pik\xF8)
	double tarpo_tarp_piku_didinimo_koef;
	
	// skirtumas tarp to, koks plotis buvo panaudotas rezultatuose, ir koks duomenyse.
	// Bus neigiamas, jei signalo rezultatas sutrump\xEBjo, ir teigiamas, jei pailg\xEBjo.
	// Per tiek padid\xEBs fonem\xF8 ilgiai
	int einamasis_postumis;
	
	// einamasis garso signalo masyvo indeksas
	unsigned int einamasis_signalo_nr;
	
	// garso signalo masyvas, i\xF0skiriamas dinami\xF0kai
	short * naujas_signalas;
	
	// naujojo garso signalo masyvo ilgis (ne signalo ilgis, o kiek i\xF0skirta vietos masyvui. 
	// Paprastai masyvo ilgis didesnis u\xFE signalo ilg\xE1)
	unsigned int naujo_signalo_masyvo_ilgis;
	
	// garso signalo ilgis
	//unsigned int naujo_signalo_ilgis;
	
	// einamasis naujo garso signalo masyvo indeksas
	unsigned int einamasis_naujo_signalo_nr;

	// nurodo, ar galima pailginti masyv\xE0 naujas_signalas, jei per trumpas (0 - negalima, !=0 - galima)
	// (negalima - jei masyvas naujas_signalas gautas i\xF0 i\xF0or\xEBs (jei greitinama i\xF0 funkcijos change_phoneme_rate), 
	// galima - jei atmintis jam i\xF0skirta viduje (RateChange.dll'e) (jei greitinama i\xF0 funkcijos change_DB_rate))
	int galima_pailginti_naujas_signalas;
	
	// Euristika nustatys, kuriuos burbuliukus reikia pa\xF0alinti ar dubliuoti. 
	// J\xF8 s\xE0ra\xF0\xE0 pateiks \xF0iame kintamajame "burbulai". 
	// Kad nereik\xEBt\xF8 pastoviai i\xF0skirin\xEBti jam atminties, 
	// \xE8ia i\xF0skiriame vien\xE0 kart\xE0 ir pastoviai naudojame.
	// TODO: pastoviai tikrinti, ar nevir\xF0ijo MAX_BURBULAI. Vir\xF0ijus ka\xFEk\xE0 daryti.
	struct burbulas burbulai[MAX_KEICIAMI_BURBULAI];

	// kei\xE8iam\xF8 (\xF0alinam\xF8 ar dubliuojam\xF8) burbul\xF8 skai\xE8ius
	int keiciamu_burbulu_sk;
	
	// fonemos klas\xEB, nurodanti, ar ilginimui turime pik\xF8 informacij\xE0, ar ne 
	// (FONEMU_KLASE_SKARDIEJI - turime, FONEMU_KLASE_DUSLIEJI - ne, FONEMU_KLASE_RR - nieko nedarome)
	int fonemos_klase;

	// nurodo, ar keisti pagrindinio tono auk\xF0t\xE1 (0 - negalima, !=0 - galima)
	// Keisti, jei:
	// 1) fonema bals\xEB arba skardusis priebalsis
	// 2) fonema turi > 1 pik\xE0 (prie\xF0ingu atveju pagrindinis tonas n\xEBra patikimas, nes abiejose pus\xEBse gali b\xFBti fonemos be pik\xF8 
	// - kaip tokiu atveju rasti tarp\xE0 tarp pik\xF8?)
	// 3) funkcija change_phoneme_rate() i\xF0kviesta su parametru tono_aukscio_pokytis != 100
	int keisti_tono_auksti;
};

/*********************************************************
 * Greitis.cpp
 ********************************************************/

extern char signalo_failo_pavadinimas[];
extern short * signalas;
extern unsigned int signalo_ilgis;
extern char * naujo_signalo_failo_pavadinimas;
extern char fonemu_failo_pavadinimas[];
extern char ** fonemos;
extern int * fonemu_ilgiai;
extern unsigned int fonemu_kiekis;
extern char * naujo_fonemu_failo_pavadinimas;
extern int * nauji_fonemu_ilgiai;
extern char vidutiniu_ilgiu_fonemu_failo_pavadinimas[];
extern char ** skirtingos_fonemos;
extern int * vidutiniai_fonemu_ilgiai;
extern unsigned int skirtingu_fonemu_kiekis;
extern char piku_failo_pavadinimas[];
extern unsigned int * pikai;
extern unsigned int piku_kiekis;
extern const int scenarijus;
extern double scenarijaus5_koeficientas;

void klaida (char * klaidos_pranesimas);
void spausdinti_burbulus (struct burbulas * burbulai, int burbulu_sk);
int kopijuoti_signala_pradzioj (struct tkontekstas * kontekstas);
int kopijuoti_signala_pabaigoj (struct tkontekstas * kontekstas);
int kopijuoti_signala (unsigned int iki, struct tkontekstas * kontekstas);
int trumpinti_fonema (struct tkontekstas * kontekstas);
int ilginti_fonema (struct tkontekstas * kontekstas);
int vykdyti (int greitis, int tono_aukscio_pokytis, struct tkontekstas * kontekstas);

/*********************************************************
 * VeiksmaiSuFailais.cpp
 ********************************************************/

void sukurti_kataloga (char * katalogoVardas);
int failu_sarasas_is_katalogo (char * katalogoVardas, char ** failu_vardai);

int nuskaityti_anotacijas (char * fonemu_failo_pavadinimas, char *** fonemos1, int ** fonemu_ilgiai1, unsigned int * fonemu_kiekis1);
int nuskaityti_duomenis ();
int nuskaityti_ilginimo_koeficientus (char * failo_pavadinimas, float ** pateikti_koef1, float ** faktiniai_koef1, int * koef_skaicius);

int irasyti_anotacijas ();
int irasyti_duomenis (struct tkontekstas * kontekstas);

/*********************************************************
 * Euristika.cpp
 ********************************************************/

void euristika (struct tkontekstas * kontekstas);
bool reguliarus_pikai (struct tkontekstas * kontekstas);

/*********************************************************
 * RateChange.cpp
 ********************************************************/

extern int debuginam;

void spausdinti_loga(char* pranesimas);
void spausdinti_konteksta (struct tkontekstas * kontekstas);
int fonemosKlase (struct tkontekstas * kontekstas);

#endif //!RATECHANGEINTERNAL_H