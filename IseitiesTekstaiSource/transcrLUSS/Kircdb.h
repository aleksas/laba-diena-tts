///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas transcrLUSS.dll
// Failas KircDB.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct struct_variantas {int Zodynas;
                  int GramForma;
                  int KamNr;
                  int KirtRaide;
                  int Priegaide;
                  int Prioritetas;
                  char Skiem[136];
                  char Tarpt;} variantas;

int Kirciuoti(char *Zodis, char *SkPb, variantas *Variant);
void initKircLUSS();
