///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas transcrLUSS.dll
// Failas Skiemen.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#define Kiek 138

void skiem(char eil[], char SkPb[])
{
static const char *SkirSkiem[Kiek]={
"AA","A\xC0","AE","A\xC6","A\xCB","AIE","AY","A\xC1","AO","AUO","A\xDB","A\xD8",
"\xC0""A","\xC0\xC0","\xC0""E","\xC0\xC6","\xC0\xCB","\xC0I","\xC0Y","\xC0\xC1","\xC0O","\xC0U","\xC0\xDB","\xC0\xD8",
"EA","E\xC0","EE","E\xC6","E\xCB","EIE","EY","E\xC1","EO","EUI","EUO","E\xDB","E\xD8",
"\xC6""A","\xC6\xC0","\xC6""E","\xC6\xC6","\xC6\xCB","\xC6I","\xC6Y","\xC6\xC1","\xC6O","\xC6U","\xC6\xDB","\xC6\xD8",
"\xCB""A","\xCB\xC0","\xCB""E","\xCB\xC6","\xCB\xCB","\xCBI","\xCBY","\xCB\xC1","\xCBO","\xCBU","\xCB\xDB","\xCB\xD8",
"IEI","I\xC6","I\xCB","II","IY","I\xC1",
"YA","Y\xC0","YE","Y\xC6","Y\xCB","YI","YY","Y\xC1","YO","YU","Y\xDB","Y\xD8",
"\xC1""A","\xC1\xC0","\xC1""E","\xC1\xC6","\xC1\xCB","\xC1I","\xC1Y","\xC1\xC1","\xC1O","\xC1U","\xC1\xDB","\xC1\xD8",
"OA","O\xC0","OE","O\xC6","O\xCB","OIE","OY","O\xC1","OO","OUO","O\xDB","O\xD8",
"UA","U\xC0","UE","U\xC6","U\xCB","UIE","UY","U\xC1","UU","U\xDB","U\xD8",
"\xDB""A","\xDB\xC0","\xDB""E","\xDB\xC6","\xDB\xCB","\xDBI","\xDBY","\xDB\xC1","\xDBO","\xDBU","\xDB\xDB","\xDB\xD8",
"\xD8""A","\xD8\xC0","\xD8""E","\xD8\xC6","\xD8\xCB","\xD8I","\xD8Y","\xD8\xC1","\xD8O","\xD8U","\xD8\xDB","\xD8\xD8"};

int i;
char* ek;

i=strlen(SkPb)-1;

if(SkPb[i] & 1) SkPb[i]++; //eilutes pabaiga
while(i>0)
	{
	while(!strchr("A\xC0""E\xC6\xCBIY\xC1OU\xDB\xD8 ",eil[i])&&(i>0)) i--;
	while(strchr("A\xC0""E\xC6\xCBIY\xC1OU\xDB\xD8",eil[i])&&(i>0)) i--;
	if((i>0)&&!(SkPb[i] & 2)&&strchr("JLMNRV",eil[i])) i--;
	if((i>1)&&!(SkPb[i] & 2)&&((eil[i]=='Z')&&(eil[i-1]=='D')
		||(eil[i]=='\xDE')&&(eil[i-1]=='D')
		||(eil[i]=='H')&&(eil[i-1]=='C'))) i-=2;
	else if((i>0)&&!(SkPb[i] & 2)&&strchr("BDGKPTC\xC8""FH",eil[i])) i--;
	if((i>0)&&!(SkPb[i] & 2)&&(strchr("S\xD0",eil[i])
		||strchr("Z\xDE",eil[i])&&(eil[i-1]!='D'))) i--;

	if(SkPb[i] & 1) SkPb[i]++;
	if(eil[i]==' ') {if(i>0) i--; if(SkPb[i] & 1) SkPb[i]++;}
	}

// negalimu balsiu kombinaciju priskyrimas skirtingiems skiemenims
for(i=0; i<Kiek; i++)
	{
	ek=eil-1;
	while(((ek=strstr(ek+1,SkirSkiem[i]))!=0)&&(SkPb[ek-eil] & 1))
		SkPb[ek-eil]++;
	}
}

