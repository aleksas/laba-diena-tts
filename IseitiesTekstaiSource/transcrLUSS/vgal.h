///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas transcrLUSS.dll
// Failas vgal.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define VKS    1   //veiksmazodis
#define DAL    2   //dalyvis
#define PAD    3   //padalyvis
#define PSD    4   //pusdalyvis
#define BENDR  5   //bendratis
#define BUDN   6   //budinys
#define PRV    7   //prieveiksmis

#define ESAML  1   //esamasis laikas
#define BKL    2   //butasis kartinis laikas
#define BDL    3   //butasis dazninis laikas
#define BSL    4   //busimasis laikas
#define BTL    5   //butasis laikas

#define TIESN  1   //tiesiogine nuosaka
#define LIEPN  2   //liepiamoji nuosaka
#define TARN   3   //tariamoji nuosaka

#define VEIKR  1   //veikiamosios rusies
#define NEVR   2   //neveikiamosios rusies
#define REIK   3   //reikiamybes

#define VNS    1   //vienaskaita
#define DGS    2   //daugiskaita
#define V_D    3   //vienaskaita ir daugiskaita

#define VG     1   //vuriskoji gimine
#define MG     2   //moteriskoji gimine
#define VM     3   //vyriskoji arba moteriskoji gimine
#define BG     4   //bevarde gimine

#ifndef _VGAL_H_

// Asmenuociu kodai: A-O 1, A-\xCB 2, IA-O 4, IA-\xCB 8, I-O 16, O-O 32, O-\xCB 64,
// A-* 3, IA-* 12, O-* 96, A-*&O-* 99, *-O 53, *-\xCB 74,

struct VeiksmazodziuGalunes{const char *Galune; char KamTipas; char Sangrazine;
                            char Asmenuote; char KircRaide; char Priegaide;
	                        char KircTaisykle; char PriesdTE;
							char KalbDal; char Laikas; char NuosakaRusis;
							char AsmuoLinksnis; char Skaicius;
							char Gimine; char Apibreztumas;} GalMasV[1066] = {
// I\xF0 esamojo laiko kamieno darom\xF8 veiksma\xFEod\xFEio form\xF8 gal\xFBn\xEBs
// 1 Esamasis laikas ir sangraziniai
// A asmenuote
{"U",    0,0, 3, 0, 0,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"I",    0,0, 3, 0, 0,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"A",    0,0, 3,-1,-1,1,1, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"EMA",  0,0, 3,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"MA",   0,0, 3,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"ETA",  0,0, 3,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"TA",   0,0, 3,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"ISOU", 0,1, 3, 3, 1,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"SOU",  0,1, 3, 2, 1,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"ISEI", 0,1, 3, 3, 1,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"SEI",  0,1, 3, 2, 1,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"ISA",  0,1, 3,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"SA",   0,1, 3,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"S\xCBMA", 0,1, 3,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"S\xCBTA", 0,1, 3,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},

// IA asmenuote
{"UI",   0,0,12, 0, 0,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"I",    0,0,12, 0, 0,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"AI",   0,0,12,-1,-1,1,1, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"EMAI", 0,0,12,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"MAI",  0,0,12,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"ETAI", 0,0,12,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"TAI",  0,0,12,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"ISOUI",0,1,12, 3, 1,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"SOUI", 0,1,12, 2, 1,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"ISEI", 0,1,12, 3, 1,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"SEI",  0,1,12, 2, 1,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"ISAI", 0,1,12,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"SAI",  0,1,12,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"S\xCBMAI",0,1,12,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"S\xCBTAI",0,1,12,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},

// I asmenuote
{"UI",   0,0,16, 0, 0,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"I",    0,0,16, 0, 0,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"I",    0,0,16,-1,-1,1,1, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"EMI",  0,0,16,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"MI",   0,0,16,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"ETI",  0,0,16,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"TI",   0,0,16,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"ISOUI",0,1,16, 3, 1,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"SOUI", 0,1,16, 2, 1,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"ISEI", 0,1,16, 3, 1,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"SEI",  0,1,16, 2, 1,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"ISI",  0,1,16,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"SI",   0,1,16,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"S\xCBMI", 0,1,16,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"S\xCBTI", 0,1,16,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},

// O asmenuote
{"UA",   0,0,96, 0, 2,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"IA",   0,0,96, 0, 2,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"O",    0,0,96,-1,-1,1,1, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"EMO",  0,0,96,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"MO",   0,0,96,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"ETO",  0,0,96,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"TO",   0,0,96,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},
{"ISUA", 0,1,96, 2, 2,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"SUA",  0,1,96, 1, 2,0,0, VKS, ESAML, TIESN, 1, VNS, -1, -1},
{"ISIA", 0,1,96, 2, 2,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"SIA",  0,1,96, 1, 2,0,0, VKS, ESAML, TIESN, 2, VNS, -1, -1},
{"ISO",  0,1,96,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"SO",   0,1,96,-1,-1,1,0, VKS, ESAML, TIESN, 3, V_D, -1, -1},
{"S\xCBMO", 0,1,96,-1,-1,1,0, VKS, ESAML, TIESN, 1, DGS, -1, -1},
{"S\xCBTO", 0,1,96,-1,-1,1,0, VKS, ESAML, TIESN, 2, DGS, -1, -1},

// 2 Liepiamosios nuosakos 3 asmuo
{"EI",    0,0,31, 0, 2,2,1, VKS, -1, LIEPN, 3, V_D, -1, -1},
{"IA",    0,0,96,-1,-1,3,1, VKS, -1, LIEPN, 3, V_D, -1, -1},

// 3 Esamojo laiko padalyvis
{"TNA",   0,0,99,-1,-1,1,0, PAD, ESAML, -1, -1, -1, -1, -1},
{"SITNA", 0,1,99,-1,-1,1,0, PAD, ESAML, -1, -1, -1, -1, -1},
{"TNAI",  0,0,12,-1,-1,1,0, PAD, ESAML, -1, -1, -1, -1, -1},
{"SITNAI",0,1,12,-1,-1,1,0, PAD, ESAML, -1, -1, -1, -1, -1},
{"TNI",   0,0,16,-1,-1,1,0, PAD, ESAML, -1, -1, -1, -1, -1},
{"SITNI", 0,1,16,-1,-1,1,0, PAD, ESAML, -1, -1, -1, -1, -1},

// 4 Esamojo laiko veikiamasis dalyvis
// AO asmenuotes
{"S\xC0",          0,0,99, 1, 2,4,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"\xC0",           0,0,99, 0, 2,4,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},  // v.g.trumpos gal.

{"SIS\xC0",        0,1,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"IS\xC0",         0,1,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},  // sangrazines

{"SITNA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"OI\xC8NA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, VG, 0},    // v.g.
{"MAI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, VG, 0},
{"\xC1TNA",        0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, VG, 0},
{"UI\xC8NA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, VG, 0},
{"EMAI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, VG, 0},
{"SYTNA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},
{"\xD8I\xC8NA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 2, DGS, VG, 0},
{"SMEITNA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 0},
{"MEITNA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 0},
{"SUI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, VG, 0},
{"SIAI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, VG, 0},
{"ESOUI\xC8NA",    0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 0},
{"SOUI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 0},

{"SISYTNA",     0,0,99, 3, 2,4,0, DAL, ESAML, VEIKR, 1, VNS, VG, 1},
{"OJOI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, VG, 1},  // v.g.ivardziuot.
{"MAJAI\xC8NA",    0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1TNA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, VG, 1},
{"UJOUI\xC8NA",    0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, VG, 1},
{"EMAJAI\xC8NA",   0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, VG, 1},
{"IJEITNA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8I\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 2, DGS, VG, 1},
{"SMEISEITNA",  0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 1},
{"MEISEITNA",   0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 1},
{"SUISOUI\xC8NA",  0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, VG, 1},
{"SIAISIAI\xC8NA", 0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUI\xC8NA",0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 1},
{"SOUISOUI\xC8NA", 0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 1},

{"ITNA",        0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, MG, 0},
{"SOI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, MG, 0},    // m.g.
{"IAI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, MG, 0},
{"\xC0I\xC8NA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, MG, 0},
{"AI\xC8NA",       0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, MG, 0},
{"EJOI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 0},
{"JOI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 0},
{"SMOI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 0},
{"MOI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 0},
{"SAI\xC8NA",      0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, MG, 0},
{"SIMOI\xC8NA",    0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, MG, 0},
{"ESOI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, MG, 0},

{"IJOI\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, MG, 1},
{"SOISOI\xC8NA",   0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAI\xC8NA",    0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0I\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0I\xC8NA",     0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, MG, 1},
{"EJOJOI\xC8NA",   0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 1},
{"JOJOI\xC8NA",    0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 1},
{"SMOISOI\xC8NA",  0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 1},
{"MOISOI\xC8NA",   0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0I\xC8NA",   0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, MG, 1},
{"SIMOISOI\xC8NA", 0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, MG, 1},
{"ESOISOI\xC8NA",  0,0,99,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, MG, 1},

// IA asmenuote
{"S\xC0I",         0,0,12, 1, 2,4,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"\xC0I",          0,0,12, 0, 2,4,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},        // v.g.trumpos gal.

{"SIS\xC0I",       0,1,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"IS\xC0I",        0,1,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},  // sangrazines

{"SITNAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"OI\xC8NAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, VG, 0},    // v.g.
{"MAI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, VG, 0},
{"\xC1TNAI",       0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, VG, 0},
{"UI\xC8NAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, VG, 0},
{"EMAI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, VG, 0},
{"SYTNAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},
{"\xD8I\xC8NAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 2, DGS, VG, 0},
{"SMEITNAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 0},
{"MEITNAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 0},
{"SUI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, VG, 0},
{"SIAI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, VG, 0},
{"ESOUI\xC8NAI",   0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 0},
{"SOUI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 0},

{"SISYTNAI",    0,0,12, 3, 2,4,0, DAL, ESAML, VEIKR, 1, VNS, VG, 1},
{"OJOI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, VG, 1},  // v.g.ivardziuot.
{"MAJAI\xC8NAI",   0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1TNAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, VG, 1},
{"UJOUI\xC8NAI",   0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, VG, 1},
{"EMAJAI\xC8NAI",  0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, VG, 1},
{"IJEITNAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8I\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 2, DGS, VG, 1},
{"SMEISEITNAI", 0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 1},
{"MEISEITNAI",  0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 1},
{"SUISOUI\xC8NAI", 0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, VG, 1},
{"SIAISIAI\xC8NAI",0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUI\xC8NAI",0,0,12,-1,-1,1,0,DAL, ESAML, VEIKR, 6, DGS, VG, 1},
{"SOUISOUI\xC8NAI",0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 1},

{"ITNAI",       0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, MG, 0},
{"SOI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, MG, 0},    // m.g.
{"IAI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, MG, 0},
{"\xC0I\xC8NAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, MG, 0},
{"AI\xC8NAI",      0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, MG, 0},
{"EJOI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 0},
{"JOI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 0},
{"SMOI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 0},
{"MOI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 0},
{"SAI\xC8NAI",     0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, MG, 0},
{"SIMOI\xC8NAI",   0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, MG, 0},
{"ESOI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, MG, 0},

{"IJOI\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, MG, 1},
{"SOISOI\xC8NAI",  0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAI\xC8NAI",   0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0I\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0I\xC8NAI",    0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, MG, 1},
{"EJOJOI\xC8NAI",  0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 1},
{"JOJOI\xC8NAI",   0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 1},
{"SMOISOI\xC8NAI", 0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 1},
{"MOISOI\xC8NAI",  0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0I\xC8NAI",  0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, MG, 1},
{"SIMOISOI\xC8NAI",0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, MG, 1},
{"ESOISOI\xC8NAI", 0,0,12,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, MG, 1},

// I asmenuote
{"S\xC1",          0,0,16, 1, 2,4,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"\xC1",           0,0,16, 0, 2,4,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},        // v.g.trumpos gal.

{"SIS\xC1",        0,1,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"IS\xC1",         0,1,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},  // sangrazines

{"SITNI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, VG, 0},
{"OI\xC8NI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, VG, 0},    // v.g.
{"MAI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, VG, 0},
{"\xC1TNI",        0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, VG, 0},
{"UI\xC8NI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, VG, 0},
{"EMAI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, VG, 0},
{"SYTNI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 0},
{"\xD8I\xC8NI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 2, DGS, VG, 0},
{"SMEITNI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 0},
{"MEITNI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 0},
{"SUI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, VG, 0},
{"SIAI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, VG, 0},
{"ESOUI\xC8NI",    0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 0},
{"SOUI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 0},

{"SISYTNI",     0,0,16, 3, 2,4,0, DAL, ESAML, VEIKR, 1, VNS, VG, 1},
{"OJOI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, VG, 1},  // v.g.ivardziuot.
{"MAJAI\xC8NI",    0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1TNI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, VG, 1},
{"UJOUI\xC8NI",    0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, VG, 1},
{"EMAJAI\xC8NI",   0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, VG, 1},
{"IJEITNI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8I\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 2, DGS, VG, 1},
{"SMEISEITNI",  0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 1},
{"MEISEITNI",   0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, VG, 1},
{"SUISOUI\xC8NI",  0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, VG, 1},
{"SIAISIAI\xC8NI", 0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUI\xC8NI",0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 1},
{"SOUISOUI\xC8NI", 0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, VG, 1},

{"ITNI",        0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, MG, 0},
{"SOI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, MG, 0},    // m.g.
{"IAI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, MG, 0},
{"\xC0I\xC8NI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, MG, 0},
{"AI\xC8NI",       0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, MG, 0},
{"EJOI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 0},
{"JOI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 0},
{"SMOI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 0},
{"MOI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 0},
{"SAI\xC8NI",      0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, MG, 0},
{"SIMOI\xC8NI",    0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, MG, 0},
{"ESOI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, MG, 0},

{"IJOI\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 1, VNS, MG, 1},
{"SOISOI\xC8NI",   0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAI\xC8NI",    0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0I\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0I\xC8NI",     0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, VNS, MG, 1},
{"EJOJOI\xC8NI",   0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 1},
{"JOJOI\xC8NI",    0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, VNS, MG, 1},
{"SMOISOI\xC8NI",  0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 1},
{"MOISOI\xC8NI",   0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0I\xC8NI",   0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 4, DGS, MG, 1},
{"SIMOISOI\xC8NI", 0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 5, DGS, MG, 1},
{"ESOISOI\xC8NI",  0,0,16,-1,-1,1,0, DAL, ESAML, VEIKR, 6, DGS, MG, 1},

// 5 Esamojo laiko neveikiamasis dalyvis
// A asmenuote
{"SAMA",      0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 1, VNS, VG, 0},
{"OMA",       0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 0},    // v.g.
{"MAMA",      0,0,3, 1, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 0},
{"\xC0MA",       0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 0},
{"UMA",       0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, VG, 0},
{"EMAMA",     0,0,3, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 0},
{"IMA",       0,0,3, 0, 0,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 0},
{"\xD8MA",       0,0,3, 0, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 0},
{"SMEIMA",    0,0,3, 3, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"MEIMA",     0,0,3, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"SUMA",      0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, VG, 0},
{"SIAMA",     0,0,3, 1, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 0},
{"ESOUMA",    0,0,3, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},
{"SOUMA",     0,0,3, 1, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},

{"SISAMA",    0,0,3, 3, 0,5,0, DAL, ESAML, NEVR, 1, VNS, VG, 1},
{"OJOMA",     0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 1}, // v.g.ivardziuot.
{"MAJAMA",    0,0,3, 3, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0MA",     0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 1},
{"UJOUMA",    0,0,3, 3, 1,5,0, DAL, ESAML, NEVR, 5, VNS, VG, 1},
{"EMAJAMA",   0,0,3, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 1},
{"IJEIMA",    0,0,3, 3, 1,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8MA",     0,0,3, 2, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 1},
{"SMEISEIMA", 0,0,3, 6, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"MEISEIMA",  0,0,3, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"SUISOUMA",  0,0,3, 5, 1,5,0, DAL, ESAML, NEVR, 4, DGS, VG, 1},
{"SIAISIAMA", 0,0,3, 5, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 1},
{"ESOUISOUMA",0,0,3, 6, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},
{"SOUISOUMA", 0,0,3, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},

{"AMA",       0,0,3, 0, 0,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 0},
{"SOMA",      0,0,3, 1, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 0},    // m.g.
{"IAMA",      0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 0},
{"AMA",       0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, MG, 0},
{"EJOMA",     0,0,3, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"JOMA",      0,0,3, 1, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"SOMA",      0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 0},
{"SMOMA",     0,0,3, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"MOMA",      0,0,3, 1, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"SAMA",      0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, MG, 0},
{"SIMOMA",    0,0,3, 1, 0,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 0},
{"ESOMA",     0,0,3, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 0},

{"IJOMA",     0,0,3, 2, 1,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 1},
{"SOISOMA",   0,0,3, 4, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAMA",    0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0MA",     0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, MG, 1},
{"AJ\xC0MA",     0,0,3, 2, 1,5,0, DAL, ESAML, NEVR, 5, VNS, MG, 1},
{"EJOJOMA",   0,0,3, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"JOJOMA",    0,0,3, 3, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"SOISOMA",   0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 1},
{"SMOISOMA",  0,0,3, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"MOISOMA",   0,0,3, 4, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0MA",   0,0,3, 4, 1,5,0, DAL, ESAML, NEVR, 4, DGS, MG, 1},
{"SIMOISOMA", 0,0,3, 6, 2,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 1},
{"ESOISOMA",  0,0,3, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 1},

{"AMA",       0,0,3,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},
{"ISAMA",     0,1,3,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},     // bev.g.

// IA asmenuote
{"SAMAI",     0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 1, VNS, VG, 0},
{"OMAI",      0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 0},    // v.g.
{"MAMAI",     0,0,12, 1, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 0},
{"\xC0MAI",      0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 0},
{"UMAI",      0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, VG, 0},
{"EMAMAI",    0,0,12, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 0},
{"IMAI",      0,0,12, 0, 0,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 0},
{"\xD8MAI",      0,0,12, 0, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 0},
{"SMEIMAI",   0,0,12, 3, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"MEIMAI",    0,0,12, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"SUMAI",     0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, VG, 0},
{"SIAMAI",    0,0,12, 1, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 0},
{"ESOUMAI",   0,0,12, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},
{"SOUMAI",    0,0,12, 1, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},

{"SISAMAI",   0,0,12, 3, 0,5,0, DAL, ESAML, NEVR, 1, VNS, VG, 1},
{"OJOMAI",    0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 1}, // v.g.ivardziuot.
{"MAJAMAI",   0,0,12, 3, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0MAI",    0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 1},
{"UJOUMAI",   0,0,12, 3, 1,5,0, DAL, ESAML, NEVR, 5, VNS, VG, 1},
{"EMAJAMAI",  0,0,12, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 1},
{"IJEIMAI",   0,0,12, 3, 1,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8MAI",    0,0,12, 2, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 1},
{"SMEISEIMAI",0,0,12, 6, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"MEISEIMAI", 0,0,12, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"SUISOUMAI", 0,0,12, 5, 1,5,0, DAL, ESAML, NEVR, 4, DGS, VG, 1},
{"SIAISIAMAI",0,0,12, 5, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 1},
{"ESOUISOUMAI",0,0,12,6, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},
{"SOUISOUMAI",0,0,12, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},

{"AMAI",      0,0,12, 0, 0,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 0},
{"SOMAI",     0,0,12, 1, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 0},    // m.g.
{"IAMAI",     0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 0},
{"AMAI",      0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, MG, 0},
{"EJOMAI",    0,0,12, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"JOMAI",     0,0,12, 1, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"SOMAI",     0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 0},
{"SMOMAI",    0,0,12, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"MOMAI",     0,0,12, 1, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"SAMAI",     0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, MG, 0},
{"SIMOMAI",   0,0,12, 1, 0,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 0},
{"ESOMAI",    0,0,12, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 0},

{"IJOMAI",    0,0,12, 2, 1,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 1},
{"SOISOMAI",  0,0,12, 4, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAMAI",   0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0MAI",    0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, MG, 1},
{"AJ\xC0MAI",    0,0,12, 2, 1,5,0, DAL, ESAML, NEVR, 5, VNS, MG, 1},
{"EJOJOMAI",  0,0,12, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"JOJOMAI",   0,0,12, 3, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"SOISOMAI",  0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 1},
{"SMOISOMAI", 0,0,12, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"MOISOMAI",  0,0,12, 4, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0MAI",  0,0,12, 4, 1,5,0, DAL, ESAML, NEVR, 4, DGS, MG, 1},
{"SIMOISOMAI",0,0,12, 6, 2,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 1},
{"ESOISOMAI", 0,0,12, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 1},

{"AMAI",      0,0,12,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},
{"ISAMAI",    0,1,12,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},     // bev.g.

// I asmenuote
{"SAMI",      0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 1, VNS, VG, 0},
{"OMI",       0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 0},    // v.g.
{"MAMI",      0,0,16, 1, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 0},
{"\xC0MI",       0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 0},
{"UMI",       0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, VG, 0},
{"EMAMI",     0,0,16, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 0},
{"IMI",       0,0,16, 0, 0,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 0},
{"\xD8MI",       0,0,16, 0, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 0},
{"SMEIMI",    0,0,16, 3, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"MEIMI",     0,0,16, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"SUMI",      0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, VG, 0},
{"SIAMI",     0,0,16, 1, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 0},
{"ESOUMI",    0,0,16, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},
{"SOUMI",     0,0,16, 1, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},

{"SISAMI",    0,0,16, 3, 0,5,0, DAL, ESAML, NEVR, 1, VNS, VG, 1},
{"OJOMI",     0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 1}, // v.g.ivardziuot.
{"MAJAMI",    0,0,16, 3, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0MI",     0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 1},
{"UJOUMI",    0,0,16, 3, 1,5,0, DAL, ESAML, NEVR, 5, VNS, VG, 1},
{"EMAJAMI",   0,0,16, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 1},
{"IJEIMI",    0,0,16, 3, 1,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8MI",     0,0,16, 2, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 1},
{"SMEISEIMI", 0,0,16, 6, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"MEISEIMI",  0,0,16, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"SUISOUMI",  0,0,16, 5, 1,5,0, DAL, ESAML, NEVR, 4, DGS, VG, 1},
{"SIAISIAMI", 0,0,16, 5, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 1},
{"ESOUISOUMI",0,0,16, 6, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},
{"SOUISOUMI", 0,0,16, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},

{"AMI",       0,0,16, 0, 0,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 0}, 
{"SOMI",      0,0,16, 1, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 0},    // m.g.
{"IAMI",      0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 0},
{"AMI",       0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, MG, 0},
{"EJOMI",     0,0,16, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"JOMI",      0,0,16, 1, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"SOMI",      0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 0},
{"SMOMI",     0,0,16, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"MOMI",      0,0,16, 1, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"SAMI",      0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, MG, 0},
{"SIMOMI",    0,0,16, 1, 0,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 0},
{"ESOMI",     0,0,16, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 0},

{"IJOMI",     0,0,16, 2, 1,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 1},
{"SOISOMI",   0,0,16, 4, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAMI",    0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0MI",     0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, MG, 1},
{"AJ\xC0MI",     0,0,16, 2, 1,5,0, DAL, ESAML, NEVR, 5, VNS, MG, 1},
{"EJOJOMI",   0,0,16, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"JOJOMI",    0,0,16, 3, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"SOISOMI",   0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 1},
{"SMOISOMI",  0,0,16, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"MOISOMI",   0,0,16, 4, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0MI",   0,0,16, 4, 1,5,0, DAL, ESAML, NEVR, 4, DGS, MG, 1},
{"SIMOISOMI", 0,0,16, 6, 2,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 1},
{"ESOISOMI",  0,0,16, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 1},

{"AMI",       0,0,16,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},
{"ISAMI",     0,1,16,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},     // bev.g.

// O asmenuote
{"SAMO",      0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 1, VNS, VG, 0},
{"OMO",       0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 0},    // v.g.
{"MAMO",      0,0,96, 1, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 0},
{"\xC0MO",       0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 0},
{"UMO",       0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, VG, 0},
{"EMAMO",     0,0,96, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 0},
{"IMO",       0,0,96, 0, 0,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 0},
{"\xD8MO",       0,0,96, 0, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 0},
{"SMEIMO",    0,0,96, 3, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"MEIMO",     0,0,96, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 0},
{"SUMO",      0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, VG, 0},
{"SIAMO",     0,0,96, 1, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 0},
{"ESOUMO",    0,0,96, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},
{"SOUMO",     0,0,96, 1, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 0},

{"SISAMO",    0,0,96, 3, 0,5,0, DAL, ESAML, NEVR, 1, VNS, VG, 1},
{"OJOMO",     0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 2, VNS, VG, 1}, // v.g.ivardziuot.
{"MAJAMO",    0,0,96, 3, 1,5,0, DAL, ESAML, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0MO",     0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, VG, 1},
{"UJOUMO",    0,0,96, 3, 1,5,0, DAL, ESAML, NEVR, 5, VNS, VG, 1},
{"EMAJAMO",   0,0,96, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, VG, 1},
{"IJEIMO",    0,0,96, 3, 1,5,0, DAL, ESAML, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8MO",     0,0,96, 2, 2,5,0, DAL, ESAML, NEVR, 2, DGS, VG, 1},
{"SMEISEIMO", 0,0,96, 6, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"MEISEIMO",  0,0,96, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, VG, 1},
{"SUISOUMO",  0,0,96, 5, 1,5,0, DAL, ESAML, NEVR, 4, DGS, VG, 1},
{"SIAISIAMO", 0,0,96, 5, 2,5,0, DAL, ESAML, NEVR, 5, DGS, VG, 1},
{"ESOUISOUMO",0,0,96, 6, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},
{"SOUISOUMO", 0,0,96, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, VG, 1},

{"AMO",       0,0,96, 0, 0,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 0},
{"SOMO",      0,0,96, 1, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 0},    // m.g.
{"IAMO",      0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 0},
{"AMO",       0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 5, VNS, MG, 0},
{"EJOMO",     0,0,96, 0, 0,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"JOMO",      0,0,96, 1, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 0},
{"SOMO",      0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 0},
{"SMOMO",     0,0,96, 2, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"MOMO",      0,0,96, 1, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 0},
{"SAMO",      0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 4, DGS, MG, 0},
{"SIMOMO",    0,0,96, 1, 0,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 0},
{"ESOMO",     0,0,96, 0, 0,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 0},

{"IJOMO",     0,0,96, 2, 1,5,0, DAL, ESAML, NEVR, 1, VNS, MG, 1},
{"SOISOMO",   0,0,96, 4, 2,5,0, DAL, ESAML, NEVR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAMO",    0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0MO",     0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 4, VNS, MG, 1},
{"AJ\xC0MO",     0,0,96, 2, 1,5,0, DAL, ESAML, NEVR, 5, VNS, MG, 1},
{"EJOJOMO",   0,0,96, 4, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"JOJOMO",    0,0,96, 3, 2,5,0, DAL, ESAML, NEVR, 6, VNS, MG, 1},
{"SOISOMO",   0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, 1, DGS, MG, 1},
{"SMOISOMO",  0,0,96, 5, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"MOISOMO",   0,0,96, 4, 1,5,0, DAL, ESAML, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0MO",   0,0,96, 4, 1,5,0, DAL, ESAML, NEVR, 4, DGS, MG, 1},
{"SIMOISOMO", 0,0,96, 6, 2,5,0, DAL, ESAML, NEVR, 5, DGS, MG, 1},
{"ESOISOMO",  0,0,96, 5, 2,5,0, DAL, ESAML, NEVR, 6, DGS, MG, 1},

{"AMO",       0,0,96,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},
{"ISAMO",     0,1,96,-1,-1,1,0, DAL, ESAML, NEVR, -1, -1, BG, 0},     // bev.g.

// 6 Esamojo laiko veikiamasis prieveiksmis
{"IAI\xC8NA",    0,0,99,-1,-1,1,0},
{"IAI\xC8NAI",   0,0,12,-1,-1,1,0},
{"IAI\xC8NI",    0,0,16,-1,-1,1,0},

// 7 Esamojo laiko neveikiamasis prieveiksmis
{"IAMA",      0,0, 3, 0, 2,6,0},
{"IAMAI",     0,0,12, 0, 2,6,0},
{"IAMI",      0,0,16, 0, 2,6,0},
{"IAMO",      0,0,96, 0, 2,6,0},

//--------------------------------------------------------------------
// I\xF0 b\xFBtojo kartinio laiko kamieno darom\xF8 veiksma\xFEod\xFEio form\xF8 gal\xFBn\xEBs
// 8 B\xFBtasis kartinis laikas ir sangraziniai
// O asmenuote
{"UA",   1,0,53, 0, 2,7,0, VKS, BKL, TIESN, 1, VNS, -1, -1},
{"IA",   1,0,53, 0, 2,7,0, VKS, BKL, TIESN, 2, VNS, -1, -1},
{"O",    1,0,53,-1,-1,8,0, VKS, BKL, TIESN, 3, V_D, -1, -1},
{"EMO",  1,0,53,-1,-1,8,0, VKS, BKL, TIESN, 1, DGS, -1, -1},
{"MO",   1,0,53,-1,-1,8,0, VKS, BKL, TIESN, 1, DGS, -1, -1},
{"ETO",  1,0,53,-1,-1,8,0, VKS, BKL, TIESN, 2, DGS, -1, -1},
{"TO",   1,0,53,-1,-1,8,0, VKS, BKL, TIESN, 2, DGS, -1, -1},
{"ISUA", 1,1,53, 2, 2,7,0, VKS, BKL, TIESN, 1, VNS, -1, -1},
{"SUA",  1,1,53, 1, 2,7,0, VKS, BKL, TIESN, 1, VNS, -1, -1},
{"ISIA", 1,1,53, 2, 2,7,0, VKS, BKL, TIESN, 2, VNS, -1, -1},
{"SIA",  1,1,53, 1, 2,7,0, VKS, BKL, TIESN, 2, VNS, -1, -1},
{"ISO",  1,1,53,-1,-1,8,0, VKS, BKL, TIESN, 3, V_D, -1, -1},
{"SO",   1,1,53,-1,-1,8,0, VKS, BKL, TIESN, 3, V_D, -1, -1},
{"S\xCBMO", 1,1,53,-1,-1,8,0, VKS, BKL, TIESN, 1, DGS, -1, -1},
{"S\xCBTO", 1,1,53,-1,-1,8,0, VKS, BKL, TIESN, 2, DGS, -1, -1},

// \xCB asmenuote
{"UAI",  1,0,74, 0, 2,7,0, VKS, BKL, TIESN, 1, VNS, -1, -1},
{"IE",   1,0,74, 0, 2,7,0, VKS, BKL, TIESN, 2, VNS, -1, -1},
{"\xCB",    1,0,74,-1,-1,8,0, VKS, BKL, TIESN, 3, V_D, -1, -1},
{"EM\xCB",  1,0,74,-1,-1,8,0, VKS, BKL, TIESN, 1, DGS, -1, -1},
{"M\xCB",   1,0,74,-1,-1,8,0, VKS, BKL, TIESN, 1, DGS, -1, -1},
{"ET\xCB",  1,0,74,-1,-1,8,0, VKS, BKL, TIESN, 2, DGS, -1, -1},
{"T\xCB",   1,0,74,-1,-1,8,0, VKS, BKL, TIESN, 2, DGS, -1, -1},
{"ISUAI",1,1,74, 2, 2,7,0, VKS, BKL, TIESN, 1, VNS, -1, -1},
{"SUAI", 1,1,74, 1, 2,7,0, VKS, BKL, TIESN, 1, VNS, -1, -1},
{"ISIE", 1,1,74, 2, 2,7,0, VKS, BKL, TIESN, 2, VNS, -1, -1},
{"SIE",  1,1,74, 1, 2,7,0, VKS, BKL, TIESN, 2, VNS, -1, -1},
{"IS\xCB",  1,1,74,-1,-1,8,0, VKS, BKL, TIESN, 3, V_D, -1, -1},
{"S\xCB",   1,1,74,-1,-1,8,0, VKS, BKL, TIESN, 3, V_D, -1, -1},
{"S\xCBM\xCB", 1,1,74,-1,-1,8,0, VKS, BKL, TIESN, 1, DGS, -1, -1},
{"S\xCBT\xCB", 1,1,74,-1,-1,8,0, VKS, BKL, TIESN, 2, DGS, -1, -1},

// 9 B\xFBtojo kartinio laiko padalyvis
{"SU",   1,0,63,-1,-1,9,0, PAD, BKL, -1, -1, -1, -1, -1},
{"SISU", 1,1,63,-1,-1,9,0, PAD, BKL, -1, -1, -1, -1, -1},
{"SUI",  1,0,64,-1,-1,9,0, PAD, BKL, -1, -1, -1, -1, -1},
{"SISUI",1,1,64,-1,-1,9,0, PAD, BKL, -1, -1, -1, -1, -1},

// 10 B\xFBtojo kartinio laiko veikiamasis dalyvis
{"S\xC6",         1,0,127,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, VG, 0},
{"OISU",       1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, VG, 0},    // v.g.
{"MAISU",      1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, VG, 0},
{"\xC1SU",        1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, VG, 0},
{"UISU",       1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, VG, 0},
{"EMAISU",     1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, VG, 0},
{"\xC6",          1,0,127,-1,-1,9,0, DAL, BKL, VEIKR, 1, DGS, VG, 0},
{"\xD8ISU",       1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 2, DGS, VG, 0},
{"SMEISU",     1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 0},
{"MEISU",      1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 0},
{"SUISU",      1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, VG, 0},
{"SIAISU",     1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, VG, 0},
{"ESOUISU",    1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 0},
{"SOUISU",     1,0,63, -1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 0},

{"SISYSU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, VG, 1},
{"OJOISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, VG, 1}, // v.g.ivardziuot.
{"MAJAISU",    1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1SU",      1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, VG, 1},
{"UJOUISU",    1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, VG, 1},
{"EMAJAISU",   1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, VG, 1},
{"IJEISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8ISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 2, DGS, VG, 1},
{"SMEISEISU",  1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 1},
{"MEISEISU",   1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 1},
{"SUISOUISU",  1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, VG, 1},
{"SIAISIAISU", 1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUISU",1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 1},
{"SOUISOUISU", 1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 1},

{"SIS\xC6",       1,1,127,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, VG, 0},
{"IS\xC6",        1,1,127,-1,-1,9,0, DAL, BKL, VEIKR, 1, DGS, VG, 0},  // v.g.sangraziniai.

{"ISU",        1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, MG, 0},
{"SOISU",      1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, MG, 0},    // m.g.
{"IAISU",      1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, MG, 0},
{"\xC0ISU",       1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, MG, 0},
{"AISU",       1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, MG, 0},
{"EJOISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 0},
{"JOISU",      1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 0},
{"SMOISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 0},
{"MOISU",      1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 0},
{"SAISU",      1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, MG, 0},
{"SIMOISU",    1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, MG, 0},
{"ESOISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, MG, 0},

{"IJOISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, MG, 1},
{"SOISOISU",   1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAISU",    1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0ISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0ISU",     1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, MG, 1},
{"EJOJOISU",   1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 1},
{"JOJOISU",    1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 1},
{"SMOISOISU",  1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 1},
{"MOISOISU",   1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0ISU",   1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, MG, 1},
{"SIMOISOISU", 1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, MG, 1},
{"ESOISOISU",  1,0,63,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, MG, 1},

{"SISU",       1,1,63,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, MG, 0},
{"ISOISU",     1,1,63,-1,-1,9,0, DAL, BKL, VEIKR, 1, DGS, MG, 0},  // m.g.sangraziniai.

{"OISUI",      1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, VG, 0},    // v.g.
{"MAISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, VG, 0},
{"\xC1SUI",       1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, VG, 0},
{"UISUI",      1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, VG, 0},
{"EMAISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, VG, 0},
{"\xD8ISUI",      1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 2, DGS, VG, 0},
{"SMEISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 0},
{"MEISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 0},
{"SUISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, VG, 0},
{"SIAISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, VG, 0},
{"ESOUISUI",   1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 0},
{"SOUISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 0},

{"SISYSUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, VG, 1},
{"OJOISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, VG, 1}, // v.g.ivardziuot.
{"MAJAISUI",   1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1SUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, VG, 1},
{"UJOUISUI",   1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, VG, 1},
{"EMAJAISUI",  1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, VG, 1},
{"IJEISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8ISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 2, DGS, VG, 1},
{"SMEISEISUI", 1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 1},
{"MEISEISUI",  1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, VG, 1},
{"SUISOUISUI", 1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, VG, 1},
{"SIAISIAISUI",1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUISUI",1,0,64,-1,-1,9,0,DAL, BKL, VEIKR, 6, DGS, VG, 1},
{"SOUISOUISUI",1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, VG, 1},

{"ISUI",       1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, MG, 0},
{"SOISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, MG, 0},    // m.g.
{"IAISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, MG, 0},
{"\xC0ISUI",      1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, MG, 0},
{"AISUI",      1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, MG, 0},
{"EJOISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 0},
{"JOISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 0},
{"SMOISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 0},
{"MOISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 0},
{"SAISUI",     1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, MG, 0},
{"SIMOISUI",   1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, MG, 0},
{"ESOISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, MG, 0},

{"IJOISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, MG, 1},
{"SOISOISUI",  1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 2, VNS, MG, 1}, // m.g.ivardziuot.
{"IAJAISUI",   1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0ISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0ISUI",    1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, VNS, MG, 1},
{"EJOJOISUI",  1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 1},
{"JOJOISUI",   1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, VNS, MG, 1},
{"SMOISOISUI", 1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 1},
{"MOISOISUI",  1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0ISUI",  1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 4, DGS, MG, 1},
{"SIMOISOISUI",1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 5, DGS, MG, 1},
{"ESOISOISUI", 1,0,64,-1,-1,9,0, DAL, BKL, VEIKR, 6, DGS, MG, 1},

{"SISUI",      1,1,64,-1,-1,9,0, DAL, BKL, VEIKR, 1, VNS, MG, 0},
{"ISOISUI",    1,1,64,-1,-1,9,0, DAL, BKL, VEIKR, 1, DGS, MG, 0},  // m.g.sangraziniai.

// 11 B\xFBtojo kartinio laiko veikiamasis prieveiksmis
{"IAISU",  1,0,63,-1,-1,9,0},
{"IAISUI", 1,0,64,-1,-1,9,0},

// I\xF0 bendraties kamieno darom\xF8 veiksma\xFEod\xFEio form\xF8 gal\xFBn\xEBs
// 12 Bendratis
{"IT",     2,0,127,-1,-1,10,0, BENDR, -1, -1, -1, -1, -1, -1},
{"T",      2,0,127,-1,-1,10,0, BENDR, -1, -1, -1, -1, -1, -1},
{"SIT",    2,1,127,-1,-1,10,0, BENDR, -1, -1, -1, -1, -1, -1},

// 13 B\xFBtasis da\xFEninis laikas
{"UAVAD",  2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 1, VNS, -1, -1},
{"ISUAVAD",2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 1, VNS, -1, -1},
{"SUAVAD", 2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 1, VNS, -1, -1},
{"IAVAD",  2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 2, VNS, -1, -1},
{"ISIAVAD",2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 2, VNS, -1, -1},
{"SIAVAD", 2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 2, VNS, -1, -1},
{"OVAD",   2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 3, V_D, -1, -1},
{"ISOVAD", 2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 3, V_D, -1, -1},
{"SOVAD",  2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 3, V_D, -1, -1},
{"EMOVAD", 2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 1, DGS, -1, -1},
{"MOVAD",  2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 1, DGS, -1, -1},
{"S\xCBMOVAD",2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 1, DGS, -1, -1},
{"ETOVAD", 2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 2, DGS, -1, -1},
{"TOVAD",  2,0,127,-1,-1,10,0, VKS, BDL, TIESN, 2, DGS, -1, -1},
{"S\xCBTOVAD",2,1,127,-1,-1,10,0, VKS, BDL, TIESN, 2, DGS, -1, -1},

// 14 B\xFBsimasis laikas
{"UIS",    2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 1, VNS, -1, -1},
{"ISOUIS", 2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 1, VNS, -1, -1},
{"SOUIS",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 1, VNS, -1, -1},
{"IS",     2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 2, VNS, -1, -1},
{"ISEIS",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 2, VNS, -1, -1},
{"SEIS",   2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 2, VNS, -1, -1},
{"S",      2,0,127,-1,-1,14,1, VKS, BSL, TIESN, 3, V_D, -1, -1},
{"SIS",    2,1,127,-1,-1,14,0, VKS, BSL, TIESN, 3, V_D, -1, -1},
{"EMIS",   2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 1, DGS, -1, -1},
{"MIS",    2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 1, DGS, -1, -1},
{"S\xCBMIS",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 1, DGS, -1, -1},
{"ETIS",   2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 2, DGS, -1, -1},
{"TIS",    2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 2, DGS, -1, -1},
{"S\xCBTIS",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 2, DGS, -1, -1},

{"UI\xD0",    2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 1, VNS, -1, -1},
{"ISOUI\xD0", 2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 1, VNS, -1, -1},
{"SOUI\xD0",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 1, VNS, -1, -1},
{"I\xD0",     2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 2, VNS, -1, -1},
{"ISEI\xD0",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 2, VNS, -1, -1},
{"SEI\xD0",   2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 2, VNS, -1, -1},
{"\xD0",      2,0,127,-1,-1,14,1, VKS, BSL, TIESN, 3, V_D, -1, -1},
{"SI\xD0",    2,1,127,-1,-1,14,0, VKS, BSL, TIESN, 3, V_D, -1, -1},
{"EMI\xD0",   2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 1, DGS, -1, -1},
{"MI\xD0",    2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 1, DGS, -1, -1},
{"S\xCBMI\xD0",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 1, DGS, -1, -1},
{"ETI\xD0",   2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 2, DGS, -1, -1},
{"TI\xD0",    2,0,127,-1,-1,10,0, VKS, BSL, TIESN, 2, DGS, -1, -1},
{"S\xCBTI\xD0",  2,1,127,-1,-1,10,0, VKS, BSL, TIESN, 2, DGS, -1, -1},

// 15 Tariamoji nuosaka
{"UAI\xC8",   2,0,127,-1,-1,10,0, VKS, -1, TARN, 1, VNS, -1, -1},
{"ISUAI\xC8", 2,1,127,-1,-1,10,0, VKS, -1, TARN, 1, VNS, -1, -1},
{"SUAI\xC8",  2,1,127,-1,-1,10,0, VKS, -1, TARN, 1, VNS, -1, -1},
{"IEMUT",  2,0,127,-1,-1,10,0, VKS, -1, TARN, 2, VNS, -1, -1},
{"MUT",    2,0,127,-1,-1,10,0, VKS, -1, TARN, 2, VNS, -1, -1},
{"ISIEMUT",2,1,127,-1,-1,10,0, VKS, -1, TARN, 2, VNS, -1, -1},
{"SIEMUT", 2,1,127,-1,-1,10,0, VKS, -1, TARN, 2, VNS, -1, -1},
{"\xD8T",     2,0,127,-1,-1,10,0, VKS, -1, TARN, 3, V_D, -1, -1},
{"IS\xD8T",   2,1,127,-1,-1,10,0, VKS, -1, TARN, 3, V_D, -1, -1},
{"S\xD8T",    2,1,127,-1,-1,10,0, VKS, -1, TARN, 3, V_D, -1, -1},
{"EM\xCBMUT", 2,0,127,-1,-1,10,0, VKS, -1, TARN, 1, DGS, -1, -1},
{"M\xCBMUT",  2,0,127,-1,-1,10,0, VKS, -1, TARN, 1, DGS, -1, -1},
{"EMUT",   2,0,127,-1,-1,10,0, VKS, -1, TARN, 1, DGS, -1, -1},
{"S\xCBM\xCBMUT",2,1,127,-1,-1,10,0, VKS, -1, TARN, 1, DGS, -1, -1},
{"S\xCBMUT",  2,1,127,-1,-1,10,0, VKS, -1, TARN, 1, DGS, -1, -1},
{"ET\xCBMUT", 2,0,127,-1,-1,10,0, VKS, -1, TARN, 2, DGS, -1, -1},
{"T\xCBMUT",  2,0,127,-1,-1,10,0, VKS, -1, TARN, 2, DGS, -1, -1},
{"ETUT",   2,0,127,-1,-1,10,0, VKS, -1, TARN, 2, DGS, -1, -1},
{"S\xCBT\xCBMUT",2,1,127,-1,-1,10,0, VKS, -1, TARN, 2, DGS, -1, -1},
{"S\xCBTUT",  2,1,127,-1,-1,10,0, VKS, -1, TARN, 2, DGS, -1, -1},

// 16 Liepiamoji nuosaka
{"IK",     2,0,127,-1,-1,10,0, VKS, -1, LIEPN, 2, VNS, -1, -1},
{"K",      2,0,127,-1,-1,10,0, VKS, -1, LIEPN, 2, VNS, -1, -1},
{"SIK",    2,1,127,-1,-1,10,0, VKS, -1, LIEPN, 2, VNS, -1, -1},
{"EMIK",   2,0,127,-1,-1,10,0, VKS, -1, LIEPN, 1, DGS, -1, -1},
{"MIK",    2,0,127,-1,-1,10,0, VKS, -1, LIEPN, 1, DGS, -1, -1},
{"S\xCBMIK",  2,1,127,-1,-1,10,0, VKS, -1, LIEPN, 1, DGS, -1, -1},
{"ETIK",   2,0,127,-1,-1,10,0, VKS, -1, LIEPN, 2, DGS, -1, -1},
{"TIK",    2,0,127,-1,-1,10,0, VKS, -1, LIEPN, 2, DGS, -1, -1},
{"S\xCBTIK",  2,1,127,-1,-1,10,0, VKS, -1, LIEPN, 2, DGS, -1, -1},

// 17 Pusdalyvis
{"SAMAD",  2,0,127,-1,-1,10,0, PSD, -1, -1, -1, VNS, VG, -1},
{"AMAD",   2,0,127, 0, 0,11,0, PSD, -1, -1, -1, VNS, MG, -1},
{"SISAMAD",2,1,127,-1,-1,10,0, PSD, -1, -1, -1, VNS, VG, -1},
{"ISAMAD", 2,1,127,-1,-1,10,0, PSD, -1, -1, -1, VNS, MG, -1},
{"IMAD",   2,0,127, 0, 0,11,0, PSD, -1, -1, -1, DGS, VG, -1},
{"SOMAD",  2,0,127,-1,-1,10,0, PSD, -1, -1, -1, DGS, MG, -1},
{"ISEIMAD",2,1,127,-1,-1,10,0, PSD, -1, -1, -1, DGS, VG, -1},
{"ISOMAD", 2,1,127,-1,-1,10,0, PSD, -1, -1, -1, DGS, MG, -1},

// 18 B\xFBtojo da\xFEninio ir b\xFBsimojo laiko padalyvis
{"SUVAD",  2,0,127,-1,-1,10,0, PAD, BDL, -1, -1, -1, -1, -1},
{"SISUVAD",2,1,127,-1,-1,10,0, PAD, BDL, -1, -1, -1, -1, -1},

{"TNAIS",  2,0,127,-1,-1,10,0, PAD, BSL, -1, -1, -1, -1, -1},
{"SITNAIS",2,1,127,-1,-1,10,0, PAD, BSL, -1, -1, -1, -1, -1},
{"TNAI\xD0",  2,0,127,-1,-1,10,0, PAD, BSL, -1, -1, -1, -1, -1},
{"SITNAI\xD0",2,1,127,-1,-1,10,0, PAD, BSL, -1, -1, -1, -1, -1},

// 19 B\xFBtojo da\xFEninio laiko veikiamasis dalyvis
{"S\xC6VAD",     2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, VNS, VG, 0},
{"OISUVAD",   2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 2, VNS, VG, 0},     // v.g.
{"MAISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 3, VNS, VG, 0},
{"\xC1SUVAD",    2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 4, VNS, VG, 0},
{"UISUVAD",   2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 5, VNS, VG, 0},
{"EMAISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 6, VNS, VG, 0},
{"\xC6VAD",      2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, DGS, VG, 0},
{"\xD8ISUVAD",   2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 2, DGS, VG, 0},
{"SMEISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 3, DGS, VG, 0},
{"MEISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 3, DGS, VG, 0},
{"SUISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 4, DGS, VG, 0},
{"SIAISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 5, DGS, VG, 0},
{"ESOUISUVAD",2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 6, DGS, VG, 0},
{"SOUISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 6, DGS, VG, 0},

{"SIS\xC6VAD",   2,1,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, VNS, VG, 0},
{"IS\xC6VAD",    2,1,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, DGS, VG, 0},  // sangr.

{"ISUVAD",    2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, VNS, MG, 0},
{"SOISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 2, VNS, MG, 0},  // m.g.
{"IAISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 3, VNS, MG, 0},
{"\xC0ISUVAD",   2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 4, VNS, MG, 0},
{"AISUVAD",   2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 5, VNS, MG, 0},
{"EJOISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 6, VNS, MG, 0},
{"JOISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 6, VNS, MG, 0},
{"SOISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, DGS, MG, 0},
{"\xD8ISUVAD",   2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 2, DGS, MG, 0},
{"SMOISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 3, DGS, MG, 0},
{"MOISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 3, DGS, MG, 0},
{"SAISUVAD",  2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 4, DGS, MG, 0},
{"SIMOISUVAD",2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 5, DGS, MG, 0},
{"ESOISUVAD", 2,0,127,-1,-1,10,0, DAL, BDL, VEIKR, 6, DGS, MG, 0},

{"SISUVAD",   2,1,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, VNS, MG, 0},
{"ISOISUVAD", 2,1,127,-1,-1,10,0, DAL, BDL, VEIKR, 1, DGS, MG, 0},  // sangr.

// 20 B\xFBsimojo laiko veikiamasis dalyvis
{"S\xC0IS",         2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 0},
{"SITNAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 0},
{"OI\xC8NAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, VG, 0},         // v.g.
{"MAI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, VG, 0},
{"\xC1TNAIS",       2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, VG, 0},
{"UI\xC8NAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, VG, 0},
{"EMAI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, VG, 0},
{"\xC0IS",          2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 0},
{"SYTNAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 0},
{"\xD8I\xC8NAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, DGS, VG, 0},
{"SMEITNAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 0},
{"MEITNAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 0},
{"SUI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, VG, 0},
{"SIAI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, VG, 0},
{"ESOUI\xC8NAIS",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, VG, 0},
{"SOUI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, VG, 0},

{"SISYTNAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 1},
{"OJOI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, VG, 1},  // v.g.ivardz.
{"MAJAI\xC8NAIS",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1TNAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, VG, 1},
{"UJOUI\xC8NAIS",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, VG, 1},
{"EMAJAI\xC8NAIS",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, VG, 1},
{"IJEITNAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8I\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, DGS, VG, 1},
{"SMEISEITNAIS", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 1},
{"MEISEITNAIS",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 1},
{"SUISOUI\xC8NAIS", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, VG, 1},
{"SIAISIAI\xC8NAIS",2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUI\xC8NAIS",2,0,127,-1,-1,10,0,DAL, BSL, VEIKR, 6, DGS, VG, 1},
{"SOUISOUI\xC8NAIS",2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, VG, 1},

{"SIS\xC0IS",       2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 0},
{"IS\xC0IS",        2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 0},  // v.g.sangr.

{"ITNAIS",       2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, MG, 0},
{"SOI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, MG, 0},  // m.g.
{"IAI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, MG, 0},
{"\xC0I\xC8NAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, MG, 0},
{"AI\xC8NAIS",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, MG, 0},
{"EJOI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 0},
{"JOI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 0},
{"SMOI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 0},
{"MOI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 0},
{"SAI\xC8NAIS",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, MG, 0},
{"SIMOI\xC8NAIS",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, MG, 0},
{"ESOI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, MG, 0},

{"IJOI\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, MG, 1},
{"SOISOI\xC8NAIS",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, MG, 1},  // m.g.ivardz.
{"IAJAI\xC8NAIS",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0I\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0I\xC8NAIS",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, MG, 1},
{"EJOJOI\xC8NAIS",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 1},
{"JOJOI\xC8NAIS",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 1},
{"SMOISOI\xC8NAIS", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 1},
{"MOISOI\xC8NAIS",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0I\xC8NAIS",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, MG, 1},
{"SIMOISOI\xC8NAIS",2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, MG, 1},
{"ESOISOI\xC8NAIS", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, MG, 1},

{"SITNAIS",      2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, MG, 0},
{"ISOI\xC8NAIS",    2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, MG, 0},  // m.g.sangr.

{"S\xC0I\xD0",         2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 0},
{"SITNAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 0},
{"OI\xC8NAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, VG, 0},         // v.g.
{"MAI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, VG, 0},
{"\xC1TNAI\xD0",       2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, VG, 0},
{"UI\xC8NAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, VG, 0},
{"EMAI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, VG, 0},
{"\xC0I\xD0",          2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 0},
{"SYTNAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 0},
{"\xD8I\xC8NAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, DGS, VG, 0},
{"SMEITNAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 0},
{"MEITNAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 0},
{"SUI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, VG, 0},
{"SIAI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, VG, 0},
{"ESOUI\xC8NAI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, VG, 0},
{"SOUI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, VG, 0},

{"SISYTNAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 1},
{"OJOI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, VG, 1},  // v.g.ivardz.
{"MAJAI\xC8NAI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, VG, 1},
{"\xC1J\xC1TNAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, VG, 1},
{"UJOUI\xC8NAI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, VG, 1},
{"EMAJAI\xC8NAI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, VG, 1},
{"IJEITNAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 1},
{"\xD8J\xD8I\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, DGS, VG, 1},
{"SMEISEITNAI\xD0", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 1},
{"MEISEITNAI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, VG, 1},
{"SUISOUI\xC8NAI\xD0", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, VG, 1},
{"SIAISIAI\xC8NAI\xD0",2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, VG, 1},
{"ESOUISOUI\xC8NAI\xD0",2,0,127,-1,-1,10,0,DAL, BSL, VEIKR, 6, DGS, VG, 1},
{"SOUISOUI\xC8NAI\xD0",2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, VG, 1},

{"SIS\xC0I\xD0",       2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, VG, 0},
{"IS\xC0I\xD0",        2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, VG, 0},  // v.g.sangr.

{"ITNAI\xD0",       2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, MG, 0},
{"SOI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, MG, 0},     // m.g.
{"IAI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, MG, 0},
{"\xC0I\xC8NAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, MG, 0},
{"AI\xC8NAI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, MG, 0},
{"EJOI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 0},
{"JOI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 0},
{"SMOI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 0},
{"MOI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 0},
{"SAI\xC8NAI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, MG, 0},
{"SIMOI\xC8NAI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, MG, 0},
{"ESOI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, MG, 0},

{"IJOI\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, MG, 1},
{"SOISOI\xC8NAI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 2, VNS, MG, 1},  // m.g.ivardz.
{"IAJAI\xC8NAI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, VNS, MG, 1},
{"\xC0J\xC0I\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, VNS, MG, 1},
{"AJ\xC0I\xC8NAI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, VNS, MG, 1},
{"EJOJOI\xC8NAI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 1},
{"JOJOI\xC8NAI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, VNS, MG, 1},
{"SMOISOI\xC8NAI\xD0", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 1},
{"MOISOI\xC8NAI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 3, DGS, MG, 1},
{"SAIS\xC0I\xC8NAI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 4, DGS, MG, 1},
{"SIMOISOI\xC8NAI\xD0",2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 5, DGS, MG, 1},
{"ESOISOI\xC8NAI\xD0", 2,0,127,-1,-1,10,0, DAL, BSL, VEIKR, 6, DGS, MG, 1},

{"SITNAI\xD0",      2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, VNS, MG, 0},
{"ISOI\xC8NAI\xD0",    2,1,127,-1,-1,10,0, DAL, BSL, VEIKR, 1, DGS, MG, 0},  // m.g.sangr.

// 21 B\xFBtojo laiko neveikiamasis dalyvis
{"SAT",       2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 1, VNS, VG, 0},
{"OT",        2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 2, VNS, VG, 0},          // v.g.
{"MAT",       2,0,127, 1, 1,11,0, DAL, BTL, NEVR, 3, VNS, VG, 0},
{"\xC0T",        2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 4, VNS, VG, 0},
{"UT",        2,0,127, 0, 0,15,0, DAL, BTL, NEVR, 5, VNS, VG, 0},
{"EMAT",      2,0,127, 0, 0,11,0, DAL, BTL, NEVR, 6, VNS, VG, 0},
{"IT",        2,0,127, 0, 0,11,0, DAL, BTL, NEVR, 1, DGS, VG, 0},
{"\xD8T",        2,0,127, 0, 2,11,0, DAL, BTL, NEVR, 2, DGS, VG, 0},
{"SMEIT",     2,0,127, 3, 1,11,0, DAL, BTL, NEVR, 3, DGS, VG, 0},
{"MEIT",      2,0,127, 2, 1,11,0, DAL, BTL, NEVR, 3, DGS, VG, 0},
{"SUT",       2,0,127, 1, 0,15,0, DAL, BTL, NEVR, 4, DGS, VG, 0},
{"SIAT",      2,0,127, 1, 2,11,0, DAL, BTL, NEVR, 5, DGS, VG, 0},
{"ESOUT",     2,0,127, 0, 0,11,0, DAL, BTL, NEVR, 6, DGS, VG, 0},
{"SOUT",      2,0,127, 1, 2,11,0, DAL, BTL, NEVR, 6, DGS, VG, 0},

{"SISAT",     2,0,127, 3, 0,11,0, DAL, BTL, NEVR, 1, VNS, VG, 1},
{"OJOT",      2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 2, VNS, VG, 1},        // v.g.ivardz.
{"MAJAT",     2,0,127, 3, 1,11,0, DAL, BTL, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0T",      2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 4, VNS, VG, 1},
{"UJOUT",     2,0,127, 3, 1,11,0, DAL, BTL, NEVR, 5, VNS, VG, 1},
{"EMAJAT",    2,0,127, 4, 2,11,0, DAL, BTL, NEVR, 6, VNS, VG, 1},
{"IJEIT",     2,0,127, 3, 1,11,0, DAL, BTL, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8T",      2,0,127, 2, 2,11,0, DAL, BTL, NEVR, 2, DGS, VG, 1},
{"SMEISEIT",  2,0,127, 6, 1,11,0, DAL, BTL, NEVR, 3, DGS, VG, 1},
{"MEISEIT",   2,0,127, 5, 1,11,0, DAL, BTL, NEVR, 3, DGS, VG, 1},
{"SUISOUT",   2,0,127, 5, 1,11,0, DAL, BTL, NEVR, 4, DGS, VG, 1},
{"SIAISIAT",  2,0,127, 5, 2,11,0, DAL, BTL, NEVR, 5, DGS, VG, 1},
{"ESOUISOUT", 2,0,127, 6, 2,11,0, DAL, BTL, NEVR, 6, DGS, VG, 1},
{"SOUISOUT",  2,0,127, 5, 2,11,0, DAL, BTL, NEVR, 6, DGS, VG, 1},

{"AT",        2,0,127, 0, 0,11,0, DAL, BTL, NEVR, 1, VNS, MG, 0},
{"SOT",       2,0,127, 1, 2,11,0, DAL, BTL, NEVR, 2, VNS, MG, 0},              // m.g.
{"IAT",       2,0,127, 1, 1,12,0, DAL, BTL, NEVR, 3, VNS, MG, 0},
{"AT",        2,0,127, 0, 0,12,0, DAL, BTL, NEVR, 5, VNS, MG, 0},
{"EJOT",      2,0,127, 0, 0,11,0, DAL, BTL, NEVR, 6, VNS, MG, 0},
{"JOT",       2,0,127, 1, 2,11,0, DAL, BTL, NEVR, 6, VNS, MG, 0},
{"SOT",       2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 1, DGS, MG, 0},
{"SMOT",      2,0,127, 2, 1,11,0, DAL, BTL, NEVR, 3, DGS, MG, 0},
{"MOT",       2,0,127, 1, 1,11,0, DAL, BTL, NEVR, 3, DGS, MG, 0},
{"SAT",       2,0,127, 1, 0,15,0, DAL, BTL, NEVR, 4, DGS, MG, 0},
{"SIMOT",     2,0,127, 1, 1,11,0, DAL, BTL, NEVR, 5, DGS, MG, 0},
{"ESOT",      2,0,127, 0, 0,11,0, DAL, BTL, NEVR, 6, DGS, MG, 0},

{"IJOT",      2,0,127, 2, 1,11,0, DAL, BTL, NEVR, 1, VNS, MG, 1},
{"SOISOT",    2,0,127, 4, 2,11,0, DAL, BTL, NEVR, 2, VNS, MG, 1},         // m.g.ivardz.
{"IAJAT",     2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0T",      2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 4, VNS, MG, 1},
{"AJ\xC0T",      2,0,127, 2, 1,11,0, DAL, BTL, NEVR, 5, VNS, MG, 1},
{"EJOJOT",    2,0,127, 4, 2,11,0, DAL, BTL, NEVR, 6, VNS, MG, 1},
{"JOJOT",     2,0,127, 3, 2,11,0, DAL, BTL, NEVR, 6, VNS, MG, 1},
{"SOISOT",    2,0,127,-1,-1,13,0, DAL, BTL, NEVR, 1, DGS, MG, 1},
{"SMOISOT",   2,0,127, 5, 1,11,0, DAL, BTL, NEVR, 3, DGS, MG, 1},
{"MOISOT",    2,0,127, 4, 1,11,0, DAL, BTL, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0T",    2,0,127, 4, 1,11,0, DAL, BTL, NEVR, 4, DGS, MG, 1},
{"SIMOISOT",  2,0,127, 6, 2,11,0, DAL, BTL, NEVR, 5, DGS, MG, 1},
{"ESOISOT",   2,0,127, 5, 2,11,0, DAL, BTL, NEVR, 6, DGS, MG, 1},

{"AT",        2,0,127,-1,-1,13,0, DAL, BTL, NEVR, -1, -1, BG, 0},
{"ISAT",      2,0,127,-1,-1,13,0, DAL, BTL, NEVR, -1, -1, BG, 0},  // bev.g.

// 22 B\xFBsimojo laiko neveikiamasis dalyvis
{"SAMIS",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 1, VNS, VG, 0},
{"OMIS",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 2, VNS, VG, 0},      // v.g.
{"MAMIS",     2,0,127, 1, 1,11,0, DAL, BSL, NEVR, 3, VNS, VG, 0},
{"\xC0MIS",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, VNS, VG, 0},
{"UMIS",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 5, VNS, VG, 0},
{"EMAMIS",    2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, VNS, VG, 0},
{"IMIS",      2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 1, DGS, VG, 0},
{"\xD8MIS",      2,0,127, 0, 2,11,0, DAL, BSL, NEVR, 2, DGS, VG, 0},
{"SMEIMIS",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 0},
{"MEIMIS",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 0},
{"SUMIS",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, DGS, VG, 0},
{"SIAMIS",    2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 5, DGS, VG, 0},
{"ESOUMIS",   2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, DGS, VG, 0},
{"SOUMIS",    2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 6, DGS, VG, 0},

{"SISAMIS",   2,0,127, 3, 0,11,0, DAL, BSL, NEVR, 1, VNS, VG, 1},
{"OJOMIS",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 2, VNS, VG, 1},    // v.g.ivardz.
{"MAJAMIS",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0MIS",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, VNS, VG, 1},
{"UJOUMIS",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 5, VNS, VG, 1},
{"EMAJAMIS",  2,0,127, 4, 2,11,0, DAL, BSL, NEVR, 6, VNS, VG, 1},
{"IJEIMIS",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8MIS",    2,0,127, 2, 2,11,0, DAL, BSL, NEVR, 2, DGS, VG, 1},
{"SMEISEIMIS",2,0,127, 6, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 1},
{"MEISEIMIS", 2,0,127, 5, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 1},
{"SUISOUMIS", 2,0,127, 5, 1,11,0, DAL, BSL, NEVR, 4, DGS, VG, 1},
{"SIAISIAMIS",2,0,127, 5, 2,11,0, DAL, BSL, NEVR, 5, DGS, VG, 1},
{"ESOUISOUMIS",2,0,127,6, 2,11,0, DAL, BSL, NEVR, 6, DGS, VG, 1},
{"SOUISOUMIS",2,0,127, 5, 2,11,0, DAL, BSL, NEVR, 6, DGS, VG, 1},

{"AMIS",      2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 1, VNS, MG, 0},
{"SOMIS",     2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 2, VNS, MG, 0},          // m.g.
{"IAMIS",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 3, VNS, MG, 0},
{"AMIS",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 5, VNS, MG, 0},
{"EJOMIS",    2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, VNS, MG, 0},
{"JOMIS",     2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 6, VNS, MG, 0},
{"SOMIS",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 1, DGS, MG, 0},
{"SMOMIS",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 0},
{"MOMIS",     2,0,127, 1, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 0},
{"SAMIS",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, DGS, MG, 0},
{"SIMOMIS",   2,0,127, 1, 0,11,0, DAL, BSL, NEVR, 5, DGS, MG, 0},
{"ESOMIS",    2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, DGS, MG, 0},

{"IJOMIS",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 1, VNS, MG, 1},
{"SOISOMIS",  2,0,127, 4, 2,11,0, DAL, BSL, NEVR, 2, VNS, MG, 1},     // m.g.ivardz.
{"IAJAMIS",   2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0MIS",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, VNS, MG, 1},
{"AJ\xC0MIS",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 5, VNS, MG, 1},
{"EJOJOMIS",  2,0,127, 4, 2,11,0, DAL, BSL, NEVR, 6, VNS, MG, 1},
{"JOJOMIS",   2,0,127, 3, 2,11,0, DAL, BSL, NEVR, 6, VNS, MG, 1},
{"SOISOMIS",  2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 1, DGS, MG, 1},
{"SMOISOMIS", 2,0,127, 5, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 1},
{"MOISOMIS",  2,0,127, 4, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0MIS",  2,0,127, 4, 1,11,0, DAL, BSL, NEVR, 4, DGS, MG, 1},
{"SIMOISOMIS",2,0,127, 6, 2,11,0, DAL, BSL, NEVR, 5, DGS, MG, 1},
{"ESOISOMIS", 2,0,127, 5, 2,11,0, DAL, BSL, NEVR, 6, DGS, MG, 1},

{"AMIS",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, -1, -1, BG, 0},
{"ISAMIS",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, -1, -1, BG, 0},  // bev.g.

{"SAMI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 1, VNS, VG, 0},
{"OMI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 2, VNS, VG, 0},      // v.g.
{"MAMI\xD0",     2,0,127, 1, 1,11,0, DAL, BSL, NEVR, 3, VNS, VG, 0},
{"\xC0MI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, VNS, VG, 0},
{"UMI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 5, VNS, VG, 0},
{"EMAMI\xD0",    2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, VNS, VG, 0},
{"IMI\xD0",      2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 1, DGS, VG, 0},
{"\xD8MI\xD0",      2,0,127, 0, 2,11,0, DAL, BSL, NEVR, 2, DGS, VG, 0},
{"SMEIMI\xD0",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 0},
{"MEIMI\xD0",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 0},
{"SUMI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, DGS, VG, 0},
{"SIAMI\xD0",    2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 5, DGS, VG, 0},
{"ESOUMI\xD0",   2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, DGS, VG, 0},
{"SOUMI\xD0",    2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 6, DGS, VG, 0},

{"SISAMI\xD0",   2,0,127, 3, 0,11,0, DAL, BSL, NEVR, 1, VNS, VG, 1},
{"OJOMI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 2, VNS, VG, 1},    // v.g.ivardz.
{"MAJAMI\xD0",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 3, VNS, VG, 1},
{"\xC1J\xC0MI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, VNS, VG, 1},
{"UJOUMI\xD0",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 5, VNS, VG, 1},
{"EMAJAMI\xD0",  2,0,127, 4, 2,11,0, DAL, BSL, NEVR, 6, VNS, VG, 1},
{"IJEIMI\xD0",   2,0,127, 3, 1,11,0, DAL, BSL, NEVR, 1, DGS, VG, 1},
{"\xD8J\xD8MI\xD0",    2,0,127, 2, 2,11,0, DAL, BSL, NEVR, 2, DGS, VG, 1},
{"SMEISEIMI\xD0",2,0,127, 6, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 1},
{"MEISEIMI\xD0", 2,0,127, 5, 1,11,0, DAL, BSL, NEVR, 3, DGS, VG, 1},
{"SUISOUMI\xD0", 2,0,127, 5, 1,11,0, DAL, BSL, NEVR, 4, DGS, VG, 1},
{"SIAISIAMI\xD0",2,0,127, 5, 2,11,0, DAL, BSL, NEVR, 5, DGS, VG, 1},
{"ESOUISOUMI\xD0",2,0,127,6, 2,11,0, DAL, BSL, NEVR, 6, DGS, VG, 1},
{"SOUISOUMI\xD0",2,0,127, 5, 2,11,0, DAL, BSL, NEVR, 6, DGS, VG, 1},

{"AMI\xD0",      2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 1, VNS, MG, 0},
{"SOMI\xD0",     2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 2, VNS, MG, 0},          // m.g.
{"IAMI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 3, VNS, MG, 0},
{"AMI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 5, VNS, MG, 0},
{"EJOMI\xD0",    2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, VNS, MG, 0},
{"JOMI\xD0",     2,0,127, 1, 2,11,0, DAL, BSL, NEVR, 6, VNS, MG, 0},
{"SOMI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 1, DGS, MG, 0},
{"SMOMI\xD0",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 0},
{"MOMI\xD0",     2,0,127, 1, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 0},
{"SAMI\xD0",     2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, DGS, MG, 0},
{"SIMOMI\xD0",   2,0,127, 1, 0,11,0, DAL, BSL, NEVR, 5, DGS, MG, 0},
{"ESOMI\xD0",    2,0,127, 0, 0,11,0, DAL, BSL, NEVR, 6, DGS, MG, 0},

{"IJOMI\xD0",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 1, VNS, MG, 1},
{"SOISOMI\xD0",  2,0,127, 4, 2,11,0, DAL, BSL, NEVR, 2, VNS, MG, 1},     // m.g.ivardz.
{"IAJAMI\xD0",   2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 3, VNS, MG, 1},
{"\xC0J\xC0MI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 4, VNS, MG, 1},
{"AJ\xC0MI\xD0",    2,0,127, 2, 1,11,0, DAL, BSL, NEVR, 5, VNS, MG, 1},
{"EJOJOMI\xD0",  2,0,127, 4, 2,11,0, DAL, BSL, NEVR, 6, VNS, MG, 1},
{"JOJOMI\xD0",   2,0,127, 3, 2,11,0, DAL, BSL, NEVR, 6, VNS, MG, 1},
{"SOISOMI\xD0",  2,0,127,-1,-1,10,0, DAL, BSL, NEVR, 1, DGS, MG, 1},
{"SMOISOMI\xD0", 2,0,127, 5, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 1},
{"MOISOMI\xD0",  2,0,127, 4, 1,11,0, DAL, BSL, NEVR, 3, DGS, MG, 1},
{"SAIS\xC0MI\xD0",  2,0,127, 4, 1,11,0, DAL, BSL, NEVR, 4, DGS, MG, 1},
{"SIMOISOMI\xD0",2,0,127, 6, 2,11,0, DAL, BSL, NEVR, 5, DGS, MG, 1},
{"ESOISOMI\xD0", 2,0,127, 5, 2,11,0, DAL, BSL, NEVR, 6, DGS, MG, 1},

{"AMI\xD0",      2,0,127,-1,-1,10,0, DAL, BSL, NEVR, -1, -1, BG, 0},
{"ISAMI\xD0",    2,0,127,-1,-1,10,0, DAL, BSL, NEVR, -1, -1, BG, 0},  // bev.g.

// 23 Reikiamyb�s dalyvis
{"SANIT",     2,0,127,-1,-1,10,0, DAL, -1, REIK, 1, VNS, VG, 0},
{"ONIT",      2,0,127,-1,-1,10,0, DAL, -1, REIK, 2, VNS, VG, 0},  // v.g.
{"MANIT",     2,0,127, 1, 1,11,0, DAL, -1, REIK, 3, VNS, VG, 0},
{"\xC0NIT",      2,0,127,-1,-1,10,0, DAL, -1, REIK, 4, VNS, VG, 0},
{"UNIT",      2,0,127,-1,-1,10,0, DAL, -1, REIK, 5, VNS, VG, 0},
{"EMANIT",    2,0,127, 0, 0,11,0, DAL, -1, REIK, 6, VNS, VG, 0},
{"INIT",      2,0,127, 0, 0,11,0, DAL, -1, REIK, 1, DGS, VG, 0},
{"\xD8NIT",      2,0,127, 0, 2,11,0, DAL, -1, REIK, 2, DGS, VG, 0},
{"SMEINIT",   2,0,127, 3, 1,11,0, DAL, -1, REIK, 3, DGS, VG, 0},
{"MEINIT",    2,0,127, 2, 1,11,0, DAL, -1, REIK, 3, DGS, VG, 0},
{"SUNIT",     2,0,127,-1,-1,10,0, DAL, -1, REIK, 4, DGS, VG, 0},
{"SIANIT",    2,0,127, 1, 2,11,0, DAL, -1, REIK, 5, DGS, VG, 0},
{"ESOUNIT",   2,0,127, 0, 0,11,0, DAL, -1, REIK, 6, DGS, VG, 0},
{"SOUNIT",    2,0,127, 1, 2,11,0, DAL, -1, REIK, 6, DGS, VG, 0},

{"SISANIT",   2,0,127, 3, 0,11,0, DAL, -1, REIK, 1, VNS, VG, 1},
{"OJONIT",    2,0,127,-1,-1,13,0, DAL, -1, REIK, 2, VNS, VG, 1},  // v.g.ivardz.
{"MAJANIT",   2,0,127, 3, 1,11,0, DAL, -1, REIK, 3, VNS, VG, 1},
{"\xC1J\xC0NIT",    2,0,127,-1,-1,13,0, DAL, -1, REIK, 4, VNS, VG, 1},
{"UJOUNIT",   2,0,127, 3, 1,11,0, DAL, -1, REIK, 5, VNS, VG, 1},
{"EMAJANIT",  2,0,127, 4, 2,11,0, DAL, -1, REIK, 6, VNS, VG, 1},
{"IJEINIT",   2,0,127, 3, 1,11,0, DAL, -1, REIK, 1, DGS, VG, 1},
{"\xD8J\xD8NIT",    2,0,127, 2, 2,11,0, DAL, -1, REIK, 2, DGS, VG, 1},
{"SMEISEINIT",2,0,127, 6, 1,11,0, DAL, -1, REIK, 3, DGS, VG, 1},
{"MEISEINIT", 2,0,127, 5, 1,11,0, DAL, -1, REIK, 3, DGS, VG, 1},
{"SUISOUNIT", 2,0,127, 5, 1,11,0, DAL, -1, REIK, 4, DGS, VG, 1},
{"SIAISIANIT",2,0,127, 5, 2,11,0, DAL, -1, REIK, 5, DGS, VG, 1},
{"ESOUISOUNIT",2,0,127,6, 2,11,0, DAL, -1, REIK, 6, DGS, VG, 1},
{"SOUISOUNIT",2,0,127, 5, 2,11,0, DAL, -1, REIK, 6, DGS, VG, 1},

{"ANIT",      2,0,127, 0, 0,11,0, DAL, -1, REIK, 1, VNS, MG, 0},
{"SONIT",     2,0,127, 1, 2,11,0, DAL, -1, REIK, 2, VNS, MG, 0},              // m.g.
{"IANIT",     2,0,127,-1,-1,10,0, DAL, -1, REIK, 3, VNS, MG, 0},
{"ANIT",      2,0,127,-1,-1,10,0, DAL, -1, REIK, 5, VNS, MG, 0},
{"EJONIT",    2,0,127, 0, 0,11,0, DAL, -1, REIK, 6, VNS, MG, 0},
{"JONIT",     2,0,127, 1, 2,11,0, DAL, -1, REIK, 6, VNS, MG, 0},
{"SONIT",     2,0,127,-1,-1,10,0, DAL, -1, REIK, 1, DGS, MG, 0},
{"SMONIT",    2,0,127, 2, 1,11,0, DAL, -1, REIK, 3, DGS, MG, 0},
{"MONIT",     2,0,127, 1, 1,11,0, DAL, -1, REIK, 3, DGS, MG, 0},
{"SANIT",     2,0,127,-1,-1,10,0, DAL, -1, REIK, 4, DGS, MG, 0},
{"SIMONIT",   2,0,127, 1, 1,11,0, DAL, -1, REIK, 5, DGS, MG, 0},
{"ESONIT",    2,0,127, 0, 0,11,0, DAL, -1, REIK, 6, DGS, MG, 0},

{"IJONIT",    2,0,127, 2, 1,11,0, DAL, -1, REIK, 1, VNS, MG, 1},
{"SOISONIT",  2,0,127, 4, 2,11,0, DAL, -1, REIK, 2, VNS, MG, 1},         // m.g.ivardz.
{"IAJANIT",   2,0,127,-1,-1,13,0, DAL, -1, REIK, 3, VNS, MG, 1},
{"\xC0J\xC0NIT",    2,0,127,-1,-1,13,0, DAL, -1, REIK, 4, VNS, MG, 1},
{"AJ\xC0NIT",    2,0,127, 2, 1,11,0, DAL, -1, REIK, 5, VNS, MG, 1},
{"EJOJONIT",  2,0,127, 4, 2,11,0, DAL, -1, REIK, 6, VNS, MG, 1},
{"JOJONIT",   2,0,127, 3, 2,11,0, DAL, -1, REIK, 6, VNS, MG, 1},
{"SOISONIT",  2,0,127,-1,-1,13,0, DAL, -1, REIK, 1, DGS, MG, 1},
{"SMOISONIT", 2,0,127, 5, 1,11,0, DAL, -1, REIK, 3, DGS, MG, 1},
{"MOISONIT",  2,0,127, 4, 1,11,0, DAL, -1, REIK, 3, DGS, MG, 1},
{"SAIS\xC0NIT",  2,0,127, 4, 1,11,0, DAL, -1, REIK, 4, DGS, MG, 1},
{"SIMOISONIT",2,0,127, 6, 2,11,0, DAL, -1, REIK, 5, DGS, MG, 1},
{"ESOISONIT", 2,0,127, 5, 2,11,0, DAL, -1, REIK, 6, DGS, MG, 1},

{"ANIT",      2,0,127,-1,-1,10,0, DAL, -1, REIK, -1, -1, BG, 0}, // bev.g.

// 24 B\xFBdinys
{"ET",        2,0,127, 0, 0,11,0, BUDN, -1, -1, -1, -1, -1, -1},
{"IANIT",     2,0,127, 0, 2,11,0, BUDN, -1, -1, -1, -1, -1, -1},

// 25 B\xFBtojo laiko neveikiamasis prieveiksmis
{"IAT",       2,0,127, 0, 2,11,0}};

#define VPriesdSk 252

struct PriesdeliuGrupes{const char *Pavadinimas; char VisadKirc; char KircRaide;
                        char Priegaide;} PriesdGrup[VPriesdSk] = {
{"",0,0,0},
{"AP",0,0,0}, {"API",0,2,0}, {"AT",0,0,0}, {"ATI",0,2,0},
{"\xC1",0,0,2}, {"I\xD0",0,0,0}, {"NU",0,1,0}, {"PA",0,1,0},
{"PAR",0,2,2}, {"PRA",0,2,0}, {"PRI",0,2,0}, {"SU",0,1,0},
{"U\xDE",0,0,0}, {"NE",0,1,0}, {"NEBE",0,3,0}, {"TE",0,1,0},
{"TEBE",0,3,0}, {"BE",0,1,0}, {"PER",1,1,1},

{"NEAP",0,2,0}, {"NEAPI",0,4,0}, {"NEAT",0,2,0}, {"NEATI",0,4,0},
{"NE\xC1",0,2,2}, {"NEI\xD0",0,2,0}, {"NENU",0,3,0}, {"NEPA",0,3,0},
{"NEPAR",0,4,2}, {"NEPRA",0,4,0}, {"NEPRI",0,4,0}, {"NESU",0,3,0},
{"NEU\xDE",0,2,0}, {"NEPER",1,3,1},

{"BEAP",0,2,0}, {"BEAPI",0,4,0}, {"BEAT",0,2,0}, {"BEATI",0,4,0},
{"BE\xC1",0,2,2}, {"BEI\xD0",0,2,0}, {"BENU",0,3,0}, {"BEPA",0,3,0},
{"BEPAR",0,4,2}, {"BEPRA",0,4,0}, {"BEPRI",0,4,0}, {"BESU",0,3,0},
{"BEU\xDE",0,2,0}, {"BEPER",1,3,1},

{"NEBEAP",0,4,0}, {"NEBEAPI",0,6,0}, {"NEBEAT",0,4,0}, {"NEBEATI",0,6,0},
{"NEBE\xC1",0,4,2}, {"NEBEI\xD0",0,4,0}, {"NEBENU",0,5,0}, {"NEBEPA",0,5,0},
{"NEBEPAR",0,6,2}, {"NEBEPRA",0,6,0}, {"NEBEPRI",0,6,0}, {"NEBESU",0,5,0},
{"NEBEU\xDE",0,4,0}, {"NEBEPER",1,5,1},

{"TEAP",0,2,0}, {"TEAPI",0,4,0}, {"TEAT",0,2,0}, {"TEATI",0,4,0},
{"TE\xC1",0,2,2}, {"TEI\xD0",0,2,0}, {"TENU",0,3,0}, {"TEPA",0,3,0},
{"TEPAR",0,4,2}, {"TEPRA",0,4,0}, {"TEPRI",0,4,0}, {"TESU",0,3,0},
{"TEU\xDE",0,2,0}, {"TENE",0,3,0}, {"TENEBE",0,5,0}, {"TEPER",1,3,1},

{"TEBEAP",0,4,0}, {"TEBEAPI",0,6,0}, {"TEBEAT",0,4,0}, {"TEBEATI",0,6,0},
{"TEBE\xC1",0,4,2}, {"TEBEI\xD0",0,4,0}, {"TEBENU",0,5,0}, {"TEBEPA",0,5,0},
{"TEBEPAR",0,6,2}, {"TEBEPRA",0,6,0}, {"TEBEPRI",0,6,0}, {"TEBESU",0,5,0},
{"TEBEU\xDE",0,4,0}, {"TEBENE",0,5,0}, {"TEBEPER",1,5,1},

{"TENEAP",0,4,0}, {"TENEAPI",0,6,0}, {"TENEAT",0,4,0}, {"TENEATI",0,6,0},
{"TENE\xC1",0,4,2}, {"TENEI\xD0",0,4,0}, {"TENENU",0,5,0}, {"TENEPA",0,5,0},
{"TENEPAR",0,6,2}, {"TENEPRA",0,6,0}, {"TENEPRI",0,6,0}, {"TENESU",0,5,0},
{"TENEU\xDE",0,4,0}, {"TENEPER",1,5,1},

{"TENEBEAP",0,6,0}, {"TENEBEAPI",0,8,0}, {"TENEBEAT",0,6,0}, {"TENEBEATI",0,8,0},
{"TENEBE\xC1",0,6,2}, {"TENEBEI\xD0",0,6,0}, {"TENEBENU",0,7,0}, {"TENEBEPA",0,7,0},
{"TENEBEPAR",0,8,2}, {"TENEBEPRA",0,8,0}, {"TENEBEPRI",0,8,0}, {"TENEBESU",0,7,0},
{"TENEBEU\xDE",0,6,0}, {"TENEBEPER",1,7,1},

{"TEBENEAP",0,6,0}, {"TEBENEAPI",0,8,0}, {"TEBENEAT",0,6,0}, {"TEBENEATI",0,8,0},
{"TEBENE\xC1",0,6,2}, {"TEBENEI\xD0",0,6,0}, {"TEBENENU",0,7,0}, {"TEBENEPA",0,7,0},
{"TEBENEPAR",0,8,2}, {"TEBENEPRA",0,8,0}, {"TEBENEPRI",0,8,0}, {"TEBENESU",0,7,0},
{"TEBENEU\xDE",0,6,0}, {"TEBENEPER",1,7,1},

{"APSI",0,3,0}, {"ATSI",0,3,0}, {"\xC1SI",0,2,0}, {"I\xD0SI",0,3,0},
{"NUSI",0,3,0}, {"PASI",0,3,0}, {"PARSI",0,4,0}, {"PRASI",0,4,0},
{"PRISI",0,4,0}, {"SUSI",0,3,0}, {"U\xDESI",0,3,0}, {"NESI",0,3,0},
{"NEBESI",0,5,0}, {"TESI",0,3,0}, {"TEBESI",0,5,0}, {"PERSI",1,1,1},
{"BESI",0,3,0},//{"BNESI",0,3,0},

{"NEAPSI",0,5,0}, {"NEATSI",0,5,0}, {"NE\xC1SI",0,4,0}, {"NEI\xD0SI",0,5,0},
{"NENUSI",0,5,0}, {"NEPASI",0,5,0}, {"NEPARSI",0,6,0}, {"NEPRASI",0,6,0},
{"NEPRISI",0,6,0}, {"NESUSI",0,5,0}, {"NEU\xDESI",0,5,0}, {"NEPERSI",1,3,1},

{"BEAPSI",0,5,0}, {"BEATSI",0,5,0}, {"BE\xC1SI",0,4,0}, {"BEI\xD0SI",0,5,0},
{"BENUSI",0,5,0}, {"BEPASI",0,5,0}, {"BEPARSI",0,6,0}, {"BEPRASI",0,6,0},
{"BEPRISI",0,6,0}, {"BESUSI",0,5,0}, {"BEU\xDESI",0,5,0}, {"BEPERSI",1,3,1},

{"NEBEAPSI",0,7,0}, {"NEBEATSI",0,7,0}, {"NEBE\xC1SI",0,6,0}, {"NEBEI\xD0SI",0,7,0},
{"NEBENUSI",0,7,0}, {"NEBEPASI",0,7,0}, {"NEBEPARSI",0,8,0}, {"NEBEPRASI",0,8,0},
{"NEBEPRISI",0,8,0}, {"NEBESUSI",0,7,0}, {"NEBEU\xDESI",0,7,0}, {"NEBEPERSI",1,5,1},

{"TEAPSI",0,5,0}, {"TEATSI",0,5,0}, {"TE\xC1SI",0,4,0}, {"TEI\xD0SI",0,5,0},
{"TENUSI",0,5,0}, {"TEPASI",0,5,0}, {"TEPARSI",0,6,0}, {"TEPRASI",0,6,0},
{"TEPRISI",0,6,0}, {"TESUSI",0,5,0}, {"TEU\xDESI",0,5,0}, {"TENESI",0,5,0},
{"TENEBESI",0,7,0}, {"TEPERSI",1,3,1},

{"TEBEAPSI",0,7,0}, {"TEBEATSI",0,7,0}, {"TEBE\xC1SI",0,6,0}, {"TEBEI\xD0SI",0,7,0},
{"TEBENUSI",0,7,0}, {"TEBEPASI",0,7,0}, {"TEBEPARSI",0,8,0}, {"TEBEPRASI",0,8,0},
{"TEBEPRISI",0,8,0}, {"TEBESUSI",0,7,0}, {"TEBEU\xDESI",0,7,0}, {"TEBENESI",0,7,0},
{"TEBENEBESI",0,9,0}, {"TEBEPERSI",1,5,1},
						
{"TENEAPSI",0,7,0}, {"TENEATSI",0,7,0}, {"TENE\xC1SI",0,6,0}, {"TENEI\xD0SI",0,7,0},
{"TENENUSI",0,7,0}, {"TENEPASI",0,7,0}, {"TENEPARSI",0,8,0}, {"TENEPRASI",0,8,0},
{"TENEPRISI",0,8,0}, {"TENESUSI",0,7,0}, {"TENEU\xDESI",0,7,0}, {"TENEPERSI",1,5,1},

{"TENEBEAPSI",0,9,0}, {"TENEBEATSI",0,9,0}, {"TENEBE\xC1SI",0,8,0}, {"TENEBEI\xD0SI",0,9,0},
{"TENEBENUSI",0,9,0}, {"TENEBEPASI",0,9,0}, {"TENEBEPARSI",0,10,0}, {"TENEBEPRASI",0,10,0},
{"TENEBEPRISI",0,10,0}, {"TENEBESUSI",0,9,0}, {"TENEBEU\xDESI",0,9,0}, {"TENEBEPERSI",1,7,1},

{"TEBENEAPSI",0,9,0}, {"TEBENEATSI",0,9,0}, {"TEBENE\xC1SI",0,8,0}, {"TEBENEI\xD0SI",0,9,0},
{"TEBENENUSI",0,9,0}, {"TEBENEPASI",0,9,0}, {"TEBENEPARSI",0,10,0}, {"TEBENEPRASI",0,10,0},
{"TEBENEPRISI",0,10,0}, {"TEBENESUSI",0,9,0}, {"TEBENEU\xDESI",0,9,0}, {"TEBENEPERSI",1,7,1}}; 

#else
extern struct VeiksmazodziuGalunes{char *Galune; char KamTipas; char Sangrazine;
                            char Asmenuote; char KircRaide; char Priegaide;
	                        char KircTaisykle; char PriesdTE;
							char KalbDal; char Laikas; char NuosakaRusis;
							char AsmuoLinksnis; char Skaicius;
							char Gimine; char Apibreztumas;} GalMasV[1089];
extern struct PriesdeliuGrupes{char *Pavadinimas; char VisadKirc; char KircRaide;
                        char Priegaide;} PriesdGrup[252];
#endif
