///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas TextNormalization.dll
// Failas TextNormalization.cpp
// Autorius dr. Tomas Anbinderis
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stringWithLetterPosition.h"

#include "TextNormalization.h"
#include "../include/LithUSS_Error.h"

#include <string.h>

char ***abbLists;
char ***abbListsSubstitutions;
unsigned short **abbListsIsWithSep;
int *abbSizes;
int totalFileBuffers = 0;

EXPORT BOOL loadTextNorm(char*)
{
	return TRUE;
}

EXPORT void unloadTextNorm()
{
	for (int i = 0; i<totalFileBuffers; i++)
	{
		for (int j = 0; j<2048; j++)
		{
			delete[] abbLists[i][j];
			delete[] abbListsSubstitutions[i][j];
		}
		delete[] abbLists[i];
		delete[] abbListsSubstitutions[i];
		delete[] abbListsIsWithSep[i];
	}
	delete[] abbLists;
	delete[] abbListsSubstitutions;
	delete[] abbListsIsWithSep;
	delete[] abbSizes;

}
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_DETACH && totalFileBuffers > 0)
	{
		for(int i=0; i<totalFileBuffers; i++)
		{
		   for(int j=0; j<2048; j++)
		   {
				delete[] abbLists[i][j];
				delete[] abbListsSubstitutions[i][j];
		   }
		   delete[] abbLists[i];
		   delete[] abbListsSubstitutions[i];
		   delete[] abbListsIsWithSep[i];
		}
		delete[] abbLists;
		delete[] abbListsSubstitutions;
		delete[] abbListsIsWithSep;
		delete[] abbSizes;
	}

    return TRUE;
}*/

#define MAX_RULES 4*1024
string rulesArray[MAX_RULES];
int totalRules;

const char* SimbPavad(char Simb)
{
	switch (Simb)
	{
		case '!' : return "\xD0""AUKTU`KAS";
		case '\"': return "KABU`T\xCBS";
		case '#' : return "NU`MERIS";
		case '$' : return "DO^LERIS";
		case '%' : return "PRO`CENTAS";
		case '&' : return "A^MPERSENDAS";
		case '\'': return "APOSTRO`FAS";
		case '(' : return "SKLIAU~STAI ATSIDA~RO";
		case ')' : return "SKLIAU~STAI U\xDESIDA~RO";
		case '*' : return "\xDEVAIG\xDE""DU`T\xCB";
		case '+' : return "PLIU`S";
		case ',' : return "KABLE~LIS";
		case '-' : return "MI`NUS";
		case '.' : return "TA~\xD0KAS";
		case '/' : return "\xC1\xDE""AMBU`S";
		case '0' : return "NU`LIS";
		case '1' : return "VI^ENAS";
		case '2' : return "DU`";
		case '3' : return "TRY~S";
		case '4' : return "KETURI`";
		case '5' : return "PENKI`";
		case '6' : return "\xD0""E\xD0I`";
		case '7' : return "SEPTYNI`";
		case '8' : return "A\xD0TUONI`";
		case '9' : return "DEVYNI`";
		case ':' : return "DVI`TA\xD0KIS";
		case ';' : return "KABLIA~TA\xD0KIS";
		case '<' : return "MA\xDEIAU~";
		case '=' : return "LY^GU";
		case '>' : return "DAUGIAU~";
		case '?' : return "KLAUSTU`KAS";
		case '@' : return "ETA`";
		case 'A' : return "\xC0~";
		case 'B' : return "B\xCB~";
		case 'C' : return "C\xCB~";
		case 'D' : return "D\xCB~";
		case 'E' : return "\xC6~";
		case 'F' : return "E`F";
		case 'G' : return "G\xCB~";
		case 'H' : return "H\xC0~";
		case 'I' : return "I`";
		case 'J' : return "JO`T";
		case 'K' : return "K\xC0~";
		case 'L' : return "E`L";
		case 'M' : return "E`M";
		case 'N' : return "E`N";
		case 'O' : return "O~";
		case 'P' : return "P\xCB~";
		case 'Q' : return "K\xDB~";
		case 'R' : return "E`R";
		case 'S' : return "E`S";
		case 'T' : return "T\xCB~";
		case 'U' : return "U`";
		case 'V' : return "V\xCB~";
		case 'W' : return "DVI`GUBA V\xCB~";
		case 'X' : return "I`KS";
		case 'Y' : return "Y~ ILGO^JI";
		case 'Z' : return "Z\xCB~";
		case '[' : return "LAU\xDETI`NIAI ATSIDA~RO";
		case '\\': return "A~TVIRK\xD0\xC8IAS \xC1\xDE""AMBU`S";
		case ']' : return "LAU\xDETI`NIAI U\xDESIDA~RO";
		case '^' : return "STOGE~LIS";
		case '_' : return "PABRAUKI`MAS";
		case '`' : return "KIR~TIS";
		case 'a' : return "\xC0~";
		case 'b' : return "B\xCB~";
		case 'c' : return "C\xCB~";
		case 'd' : return "D\xCB~";
		case 'e' : return "\xC6~";
		case 'f' : return "E`F";
		case 'g' : return "G\xCB~";
		case 'h' : return "H\xC0~";
		case 'i' : return "I`";
		case 'j' : return "JO`T";
		case 'k' : return "K\xC0~";
		case 'l' : return "E`L";
		case 'm' : return "E`M";
		case 'n' : return "E`N";
		case 'o' : return "O~";
		case 'p' : return "P\xCB~";
		case 'q' : return "K\xDB~";
		case 'r' : return "E`R";
		case 's' : return "E`S";
		case 't' : return "T\xCB~";
		case 'u' : return "U`";
		case 'v' : return "V\xCB~";
		case 'w' : return "DVI`GUBA V\xCB~";
		case 'x' : return "I`KS";
		case 'y' : return "Y~ ILGO^JI";
		case 'z' : return "Z\xCB~";
		case '{' : return "RIESTI`NIAI ATSIDA~RO";
		case '|' : return "VERTIKA~L\xCB";
		case '}' : return "RIESTI`NIAI U\xDESIDA~RO";
		case '~' : return "TIL~D\xCB";
		case '\xE0' : return "\xC0~ NO^SIN\xCB";
		case '\xE8' : return "\xC8\xCB~";
		case '\xE6' : return "\xC6~ NO^SIN\xCB";
		case '\xEB' : return "\xCB~";
		case '\xE1' : return "Y~ NO^SIN\xCB";
		case '\xF0' : return "E`\xD0";
		case '\xF8' : return "\xDB~ NO^SIN\xCB";
		case '\xFB' : return "\xDB~ ILGO^JI";
		case '\xFE' : return "\xDE\xCB~";
		case '\xC0' : return "\xC0~ NO^SIN\xCB";
		case '\xC8' : return "\xC8\xCB~";
		case '\xC6' : return "\xC6~ NO^SIN\xCB";
		case '\xCB' : return "\xCB~";
		case '\xC1' : return "Y~ NO^SIN\xCB";
		case '\xD0' : return "E`\xD0";
		case '\xD8' : return "\xDB~ NO^SIN\xCB";
		case '\xDB' : return "\xDB~ ILGO^JI";
		case '\xDE' : return "\xDE\xCB~";
		case '\x80' : return "EU~RAS";
		case '\x84' : return "KABU`T\xCBS ATSIDA~RO";
		case '\x8B' : return "LAU\xDETI`N\xCBS ATSIDA~RO";
		case '\x93' : return "KABU`T\xCBS U\xDESIDA~RO";
		case '\x96' : return "BR\xDBK\xD0NY~S";
		case '\x97' : return "I`LGAS BR\xDBK\xD0NY~S";
		case '\x9B' : return "LAU\xDETI`N\xCBS U\xDESIDA~RO";
		case '\xA2' : return "CEN~TAS";		 //�
		case '\xA3' : return "SVA~RAS";		 //�
		case '\xA4' : return "VALIUTA`";	 //�
		case '\xA5' : return "JENA`";
		case '\xA6' : return "VERTIKA~L\xCB SU TARPELIU`"; //�
		case '\xA7' : return "PARAGRA~FAS";	 //�
		case '\xA9' : return "A^UTORI\xD8 TE^IS\xCBS"; //�
		case '\xAB' : return "DVI`GUBOS LAU\xDETI`N\xCBS ATSIDA~RO";
		case '\xAE' : return "REGISTRU^OTA"; //�
		case '\xB0' : return "LA^IPSNIS";	 //�
		case '\xB1' : return "PLIU`S MI`NUS"; //�
		case '\xB6' : return "PASTRA^IPA";
		case '\xBB' : return "DVI`GUBOS LAU\xDETI`N\xCBS U\xDESIDA~RO";
		case '\xBC' : return "VIENA` KETVIRTO^JI";	//�
		case '\xBD' : return "VIENA` ANTRO^JI";		//�
		case '\xBE' : return "TRY~S KETVIR~TOSIOS"; //�
		case '\xD7' : return "DA^UGINTI";	 //�
		case '\xF7' : return "DALI`NTI";	 //�

		default  : return "";
	}
}

char* Skaitmuo(char Sk)
{
  switch (Sk)
    {
      case '0' : return " NU`LIS";
      case '1' : return " VI^ENAS";
      case '2' : return " DU`";
      case '3' : return " TRY~S";
      case '4' : return " KETURI`";
      case '5' : return " PENKI`";
      case '6' : return " \xD0""E\xD0I`";
      case '7' : return " SEPTYNI`";
      case '8' : return " A\xD0TUONI`";
      case '9' : return " DEVYNI`";
      default : return "";
    }
}

int TrizSk(char TrSk[], char Eil[])
  {
  int J;
  if((TrSk[0]=='0')&&(TrSk[1]=='0')&&(TrSk[2]=='0')) return -1;

  if((TrSk[0]=='0')&&(TrSk[1]=='0')&&(TrSk[2]=='1')) return 3; //1000 tukstantis
  if((TrSk[1]=='1')||(TrSk[2]=='0')) J=2;                      //12000, 20000 tukstanciu
  else if(TrSk[2]=='1') J=0;                                   //21000 tukstantis
  else J=1;                                                    //22000 tukstanciai

  if (TrSk[0] == '1')
    strcat(Eil, " \xD0IM~TAS");
  else
    if (TrSk[0] != '0')
      {
      strcat(Eil, Skaitmuo(TrSk[0]));
      strcat(Eil, " \xD0IMTAI~");
      }
  if (TrSk[1] == '1')
    {
    switch (TrSk[2])
      {
	  case '0' : { strcat(Eil, " DE~\xD0IMT");break;}
	  case '1' : { strcat(Eil, " VIENU^OLIKA");break;}
	  case '2' : { strcat(Eil, " DVY^LIKA");break;}
	  case '3' : { strcat(Eil, " TRY^LIKA");break;}
	  case '4' : { strcat(Eil, " KETURIO^LIKA");break;}
	  case '5' : { strcat(Eil, " PENKIO^LIKA");break;}
	  case '6' : { strcat(Eil, " \xD0""E\xD0IO^LIKA");break;}
	  case '7' : { strcat(Eil, " SEPTYNIO^LIKA");break;}
	  case '8' : { strcat(Eil, " A\xD0TUONIO^LIKA");break;}
	  case '9' : { strcat(Eil, " DEVYNIO^LIKA");}
	  }
    }
  else
    {
    switch (TrSk[1])
      {
	  case '2' : { strcat(Eil, " DVI`DE\xD0IMT");break;}
	  case '3' : { strcat(Eil, " TRI`SDE\xD0IMT");break;}
	  case '4' : { strcat(Eil, " KE~TURIASDE\xD0IMT");break;}
	  case '5' : { strcat(Eil, " PEN~KIASDE\xD0IMT");break;}
	  case '6' : { strcat(Eil, " \xD0""E~\xD0IASDE\xD0IMT");break;}
	  case '7' : { strcat(Eil, " SEPTY^NIASDE\xD0IMT");break;}
	  case '8' : { strcat(Eil, " A\xD0TU^ONIASDE\xD0IMT");break;}
	  case '9' : { strcat(Eil, " DEVY^NIASDE\xD0IMT");}
      }
    if (TrSk[2] != '0') strcat(Eil, Skaitmuo(TrSk[2]));
    }
return J;
}

int IsverstiSkEil(char Sk[], char ZodzEil[], int bufs)
{
  int J, Posl, PrIlgis, i;
  char Skaic[100];
  strcpy(&Skaic[2], Sk);
  Skaic[0] = '0';  Skaic[1] = '0';
  PrIlgis = strlen(Skaic);
  if (PrIlgis > 14)
    {
    for(i=2; i<PrIlgis; i++)
		if(strlen(ZodzEil)<bufs-20) {strcat(ZodzEil, Skaitmuo(Skaic[i]));}
		else {return 1;}
    switch (Skaic[PrIlgis-1])
      {
      case '0' : return 2;
      case '1' : return 0;
      default : return 1;
      }
    }
  Posl = PrIlgis % 3;

while (Posl < PrIlgis)
  {
  J = TrizSk(&Skaic[Posl], ZodzEil);
  switch (PrIlgis - Posl)
    {
      case 3 : {
       if(J==3) strcat(ZodzEil, " VI^ENAS");
		   if (ZodzEil[0] == ' ')
		     strcpy(ZodzEil, &ZodzEil[1]);
		 if(J==-1) return 2; else return J;
	       }
      case 6 : {
         switch (J)
           {
	       case 0 : case 3 : { strcat(ZodzEil, " T\xDB^KSTANTIS");break;}
	       case 1 : { strcat(ZodzEil, " T\xDB^KSTAN\xC8IAI");break;}
	       case 2 : { strcat(ZodzEil, " T\xDB^KSTAN\xC8I\xD8");}
           }
		 break;
	       }
      case 9 : {
         switch (J)
           {
	       case 0 : case 3 : { strcat(ZodzEil, " MILIJO~NAS");break;}
	       case 1 : { strcat(ZodzEil, " MILIJO~NAI");break;}
	       case 2 : { strcat(ZodzEil, " MILIJO~N\xD8");}
           }
		 break;
	       }
     case 12 : {
         switch (J)
           {
	       case 0 : case 3 : { strcat(ZodzEil, " MILIJA^RDAS");break;}
	       case 1 : { strcat(ZodzEil, " MILIJA^RDAI");break;}
	       case 2 : { strcat(ZodzEil, " MILIJA^RD\xD8");}
           }
	       }
    }
  Posl += 3;
  }
return 1;
}

int VisasSkaicius(char Sk[], char ZodzEil[], int bufsize)
{
  ZodzEil[0]=0;
  int i=0;
  int J, Jfinal=1;
  int ilg=strlen(Sk);
  char skaicius[100];
  do
  {
  if(Sk[i]=='+') {strcat(ZodzEil, " PLIUS"); i++;}
  else if(Sk[i]=='-') {if(isdigit(Sk[i+1])) strcat(ZodzEil, " MI`NUS"); i++;}

  while(Sk[i]=='0') {strcat(ZodzEil, " NU`LIS"); i++; if(Jfinal!=4) Jfinal=2;}
  int j=0;
  while(isdigit(Sk[i])) {skaicius[j]=Sk[i]; j++; i++;}
  if(j>0)
    {
    skaicius[j]=0;
    J=IsverstiSkEil(skaicius, ZodzEil, bufsize);
    if(Jfinal!=4) Jfinal=J;
    }
  if(Sk[i]==',') {strcat(ZodzEil, " KABLE~LIS"); i++; Jfinal=4;}
  else if(Sk[i]=='%')
	{
    switch (Jfinal)
      {
      case 0 : case 3 : { strcat(ZodzEil, " PRO`CENTAS");break;}
      case 2 : { strcat(ZodzEil, " PRO`CENT\xD8");break;}
      case 4 : { strcat(ZodzEil, " PRO`CENTO");break;}
      default : { strcat(ZodzEil, " PRO`CENTAI");}
      }
	i++;
    }
  else if(Sk[i]=='$')
	{
    switch (Jfinal)
      {
      case 0 : case 3 : { strcat(ZodzEil, " DO^LERIS");break;}
      case 2 : { strcat(ZodzEil, " DO^LERI\xD8");break;}
      case 4 : { strcat(ZodzEil, " DO^LERIO");break;}
      default : { strcat(ZodzEil, " DO^LERIAI");}
      }
	i++;
    }
  else if(Sk[i]=='\x80')
	{
    switch (Jfinal)
      {
      case 0 : case 3 : { strcat(ZodzEil, " EU~RAS");break;}
      case 2 : { strcat(ZodzEil, " EU~R\xD8");break;}
      case 4 : { strcat(ZodzEil, " EU~RO");break;}
      default : { strcat(ZodzEil, " EU~RAI");}
      }
	i++;
    }
  else if(Sk[i]=='\xA3')
	{
    switch (Jfinal)
      {
      case 0 : case 3 : { strcat(ZodzEil, " SVA~RAS");break;}
      case 2 : { strcat(ZodzEil, " SVA~R\xD8");break;}
      case 4 : { strcat(ZodzEil, " SVA~RO");break;}
      default : { strcat(ZodzEil, " SVA~RAI");}
      }
	i++;
    }
  if(Jfinal!=4) Jfinal=1;
  }
  while(i<ilg);

return strlen(ZodzEil);
}

	string years[] = {
		"PIRM\xD8~",
		"ANTR\xD8~",
		"TRE\xC8I\xD8~",
		"KETVIRT\xD8~",
		"PENKT\xD8~",
		"\xD0""E\xD0T\xD8~",
		"SEPTINT\xD8~",
		"A\xD0TUNT\xD8~",
		"DEVINT\xD8~",
		"DE\xD0IMT\xD8~",
		"VIENU^OLIKT\xD8",
		"DVY^LIKT\xD8",
		"TRY^LIKT\xD8",
		"KETURIO^LIKT\xD8",
		"PENKIO^LIKT\xD8",
		"\xD0""E\xD0IO^LKT\xD8",
		"SEPTYNIO^LIKT\xD8",
		"A\xD0TUONIO^LIKT\xD8",
		"DEVYNIO^LIKT\xD8"
	};
	string months[] = {
		"SAUSIO",
		"VASARIO",
		"KOVO",
		"BALAND\xDEIO",
		"GEGU\xDE\xCBS",
		"BIR\xDE""ELIO",
		"LIEPOS",
		"RUGPJ\xDB\xC8IO",
		"RUGS\xCBJO",
		"SPALIO",
		"LAPKRI\xC8IO",
		"GRUOD\xDEIO"
	};
	string days[] = {
		"PIRMA`",
		"ANTRA`",
		"TRE\xC8IA`",
		"KETVIRTA`",
		"PENKTA`",
		"\xD0""E\xD0TA`",
		"SEPTINTA`", 
		"A\xD0TUNTA`",
		"DEVINTA`", 
		"DE\xD0IMTA`",
		"VIENU^OLIKTA",
		"DVY^LIKTA",
		"TRY^LIKTA",
		"KETURIO^LIKTA",
		"PENKIO^LIKTA",
		"\xD0""E\xD0IO^LIKTA",
		"SEPTYNIO^LIKTA",
		"A\xD0TUONIO^LIKTA",
		"DEVYNIO^LIKTA",
		"DVIDE\xD0IMTA`",
		"DVI`DE\xD0IMT PIRMA`",
		"DVI`DE\xD0IMT ANTRA`",
		"DVI`DE\xD0IMT TRE\xC8IA`",
		"DVI`DE\xD0IMT KETVIRTA`",
		"DVI`DE\xD0IMT PENKTA`",
		"DVI`DE\xD0IMT \xD0""E\xD0TA`",
		"DVI`DE\xD0IMT SEPTINTA`", 
		"DVI`DE\xD0IMT A\xD0TUNTA`",
		"DVI`DE\xD0IMT DEVINTA`", 
		"TRISDE\xD0IMTA`",
		"TRI`SDE\xD0IMT PIRMA`"
	};
	

	
int expandDate(int yearNumber, int monthNumber, int dayNumber, int mode, char returnText[])
{
	char resTmp[256];
	char inputTmp[256];
	sprintf(inputTmp, "%d", yearNumber);
	VisasSkaicius(inputTmp, resTmp, 256);
	string yearsString = resTmp;

	int findResult = -1;

	if (yearNumber == 0)
	{
		yearsString = "NULINI\xD8";
	}
	else if ((yearNumber % 1000) == 0)
	{
		if (yearNumber == 1000)
		{
			findResult = yearsString.rfind("T\xDB^KSTANTIS");//1000
			if (findResult != -1) yearsString.replace(findResult, strlen("T\xDB^KSTANTIS"), "T\xDBKSTANT\xD8~J\xD8");
			else return -1;
		}
		else //2000,3000,... 
		{
			findResult = yearsString.rfind("T\xDB^KSTAN\xC8IAI");
			if (findResult != -1) yearsString.replace(findResult, strlen("T\xDB^KSTAN\xC8IAI"), "T\xDBKSTANT\xD8~J\xD8");
			else return -1;
		}
	}
	else if ((yearNumber % 100) == 0)
	{
		findResult = yearsString.rfind("\xD0IM~TAS"); //100,1100,2100,...
		if (findResult != -1) yearsString.replace(findResult, strlen("\xD0IM~TAS"), "\xD0IMT\xD8~J\xD8");
		else
		{
			findResult = yearsString.rfind("\xD0IMTAI~"); //200,300,...,800,900,1200,...,1800,1900,2200,...
			if (findResult != -1) yearsString.replace(findResult, strlen("\xD0IMTAI~"), "\xD0IMT\xD8~J\xD8");
			else return -1;
		}
	}
	else if ((yearNumber % 10) == 0)
	{
		findResult = yearsString.rfind("\xD0IMT"); //10,20,...,90,...,1980,1990,2010,2020,...
		if (findResult != -1) yearsString.replace(findResult, strlen("\xD0IMT"), "\xD0IMT\xD8~J\xD8");					
		else return -1;

	}
	else if ((yearNumber % 100) < 20) //...,1901,1902,...1919,2001,...
	{
		int SkTmp = yearNumber - (yearNumber % 100);
		sprintf(inputTmp, "%d", SkTmp);
		VisasSkaicius(inputTmp, resTmp, 256);
		string yearsString2 = resTmp;
		yearsString = yearsString2 + " " + years[(yearNumber % 100)-1];
	}
	else if ((yearNumber % 100) > 20) //...,1921,1922,...1999,2021,...
	{
		int SkTmp = yearNumber - (yearNumber % 10);
		sprintf(inputTmp, "%d", SkTmp);
		VisasSkaicius(inputTmp, resTmp, 256);
		string yearsString2 = resTmp;
		yearsString = yearsString2 + " " + years[(yearNumber % 10)-1];		
	}
	else return -1;

	string returnTextStringTmp;
	if (mode == 0)
		returnTextStringTmp = yearsString + " " + months[monthNumber-1] + " " + days[dayNumber-1];
	else 
		returnTextStringTmp = yearsString + " ME~T\xD8 " + months[monthNumber-1] + " " + days[dayNumber-1] + " DIENA`";
	sprintf(returnText, "%s", returnTextStringTmp.c_str());

	return 0;
}

EXPORT int initTextNorm(char * rulesFilesDirectory, const char * rulesFileName)
{
	char buffer_temp[1024];	

	totalRules = 0;
	totalFileBuffers = 0;
	sprintf(buffer_temp, "%s%s", rulesFilesDirectory, rulesFileName);
	FILE * file = fopen (buffer_temp, "r");
	if (file == NULL)
		return ERROR_TEXTNORMALIZATION_OPENING_RULES_FILE;
	while (fgets(buffer_temp , 1024 , file) != NULL)
	{
		if (buffer_temp[0] != '/')
		{
			rulesArray[totalRules].assign(buffer_temp);
			totalRules++;

			// File buffers
			if (strstr(buffer_temp, "AbbSeparateWord:") != NULL || strstr(buffer_temp, "ReplaceWithFile:") != NULL)
				totalFileBuffers++;
		}
	}
	fclose(file);
	
	// File buffers
	if (totalFileBuffers > 0)
	{
		abbLists = new char**[totalFileBuffers];
		abbListsSubstitutions = new char**[totalFileBuffers];
		abbListsIsWithSep = new unsigned short*[totalFileBuffers];
		abbSizes = new int[totalFileBuffers];
		for(int i=0; i<totalFileBuffers; i++)
		{
		   abbLists[i] = new char*[2048];
		   abbListsSubstitutions[i] = new char*[2048];
		   abbListsIsWithSep[i] = new unsigned short[2048];
		   for(int j=0; j<2048; j++)
		   {
			   abbLists[i][j] = new char[64];
			   abbListsSubstitutions[i][j] = new char[256];
		   }
		}

		int currentFileBuffer = 0;
		sprintf(buffer_temp, "%s%s", rulesFilesDirectory, rulesFileName);
		file = fopen (buffer_temp, "r");
		if (file == NULL) 
			return  ERROR_TEXTNORMALIZATION_OPENING_RULES_FILE;
		while (fgets(buffer_temp , 1024 , file) != NULL)
		{
			if (buffer_temp[0] != '/')
			{			
				if (strstr(buffer_temp, "AbbSeparateWord:") != NULL || strstr(buffer_temp, "ReplaceWithFile:") != NULL)
				{
					char * fileName = strstr(buffer_temp, ":") + 1;
					memset(strstr(fileName, "@"), 0, 1);
					abbSizes[currentFileBuffer] = 0;
					char fileNameTemp[128];	
					strcpy(fileNameTemp, fileName);	
					sprintf(buffer_temp, "%s%s", rulesFilesDirectory, fileNameTemp);
					FILE * fileAbb = fopen (buffer_temp, "r");
					if (fileAbb == NULL) 
					{
						fclose(file);
						return ERROR_TEXTNORMALIZATION_OPENING_FILE_SPECIFIED_IN_RULES_FILE;
					}
					while (fgets(buffer_temp, 1024, fileAbb) != NULL)
					{
						sscanf(buffer_temp, "%63[^@]@%255[^@]", 
							abbLists[currentFileBuffer][abbSizes[currentFileBuffer]], 
							abbListsSubstitutions[currentFileBuffer][abbSizes[currentFileBuffer]]);
						if (abbLists[currentFileBuffer][abbSizes[currentFileBuffer]][strlen(abbLists[currentFileBuffer][abbSizes[currentFileBuffer]])-1] == '.')
							abbListsIsWithSep[currentFileBuffer][abbSizes[currentFileBuffer]] = 1;
						else
							abbListsIsWithSep[currentFileBuffer][abbSizes[currentFileBuffer]] = 0;
						abbSizes[currentFileBuffer]++;
					}
					fclose(fileAbb);

					currentFileBuffer++;
				}
			}
		}		
	}
	fclose(file);

	return NO_ERR;
}

int applyPhrasesFilter(stringWithLetterPosition * bufferString)
{			
	char bruks = '�';
	string wordSeparatorsList = "\t [{(\"��']}).?!;:,-\r\n=";
	wordSeparatorsList.append(1, bruks);
	string phraseSeparatorsList = ".?!;:\r\n,";
	phraseSeparatorsList.append(1, bruks);
	string leadAndTrailSymbolsList = "[{()}]\"'";


	int phraseStartIndex = 0;
	int phraseEndIndex = bufferString->find_first_of(phraseSeparatorsList, phraseStartIndex);
	while (phraseEndIndex != -1)
	{
		if ( ((bufferString->at(phraseEndIndex) != bruks) && (bufferString->at(phraseEndIndex) != ',')) || 

			 ((bufferString->at(phraseEndIndex) == ',') && (((phraseEndIndex - phraseStartIndex) >= 25) && ((phraseEndIndex - phraseStartIndex) <= 120))) ||

			 ((bufferString->at(phraseEndIndex) == bruks) && 
			 (phraseEndIndex > 0) && 
			 (phraseEndIndex < (bufferString->length()-1)) && 
			 (bufferString->at(phraseEndIndex+1) == ' ') && 
			 (bufferString->at(phraseEndIndex-1) == ' ') && 
			 (((phraseEndIndex - phraseStartIndex) >= 25) && ((phraseEndIndex - phraseStartIndex) <= 120)))
			)
		{
			if ( (bufferString->at(phraseEndIndex) != '\n') && 
				 ((phraseEndIndex >= (bufferString->length()-1)) || (bufferString->at(phraseEndIndex+1) != '\n'))
				)
			{
				bufferString->insert(phraseEndIndex+1, "\n");
				phraseEndIndex+=1;					
			}
		}
		else
		{

			bufferString->erase(phraseEndIndex, 1);
			phraseEndIndex-=1;
		}

		if ((phraseEndIndex - phraseStartIndex) > 120)
		{
			string phraseSecondarySeparatorsList = "[{(]})\"'";
			int r = bufferString->find_first_of(phraseSecondarySeparatorsList, phraseStartIndex+25);
			if ((r != -1) && (r <= phraseStartIndex+120))
			{
				bufferString->insert(r+1, "\n");
				phraseEndIndex=r+1;		
			}
			else
			{
				string phraseTertiarySeparatorsList[] = {"ir","bei","ar","arba"};
				int r = -1;
				int bestR = bufferString->length();
				int bestI = -1;
				for (int i = 0; i < 4; i++)
				{
					r = bufferString->find(phraseTertiarySeparatorsList[i], phraseStartIndex+55);
					if ((r != -1) && (r < bestR) && (r <= phraseStartIndex+120))
					{
						bool isSeparateWord = true;

						if (((r-1) >= 0) && 
							(wordSeparatorsList.find(bufferString->at(r-1)) == -1))
								isSeparateWord = false;

						if (((r + phraseTertiarySeparatorsList[i].length()) <= (bufferString->length()-1)) && 
							(wordSeparatorsList.find(bufferString->at(r+phraseTertiarySeparatorsList[i].length())) == -1))
								isSeparateWord = false;

						if (isSeparateWord == true)
						{
							bestR = r;
							bestI = i;
						}
					}
				}
				if (bestR != bufferString->length())
				{
					bufferString->insert(bestR, "\n");
					phraseEndIndex=bestR;
				}
				else
				{
					bool isDone = false;
					int r = bufferString->rfind(' ', phraseStartIndex+120);
					while (r != -1)
					{
						if ( ( r > 0 ) && ( r < ( bufferString->length() - 1) ) && 
							 ( bufferString->letPos[r-1] != bufferString->letPos[r+1] ) )
						{
							bufferString->insert(r+1, "\n");
							phraseEndIndex=r+1;
							isDone = true;
							break;
						}
						r = bufferString->rfind(' ', r-1);
					}

					if ( ! isDone )
					{
						bufferString->insert(phraseStartIndex+120, "\n");
						phraseEndIndex=phraseStartIndex+120;	
					}
				}
			}				
		}

		phraseStartIndex = phraseEndIndex+1;
		phraseEndIndex = bufferString->find_first_of(phraseSeparatorsList, phraseStartIndex);
	}

	int findResult = bufferString->find_first_of(leadAndTrailSymbolsList, 0);
	while (findResult != -1)
	{
		bufferString->erase(findResult, 1);
		findResult = bufferString->find_first_of(leadAndTrailSymbolsList, 0);
	}

	return NO_ERR;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


EXPORT int spellText(char * buffer, char * retBuffer, int bufferSize, int * letPos)
{
	stringWithLetterPosition bufferString(buffer, letPos, bufferSize);
	if (bufferString.at(bufferString.length()-1) != '\n') bufferString.append("\n");

	// Spelling
	int currentIndex = 0;
	while (bufferString.at(currentIndex) != '\n')
	{
		const char * symbolText = SimbPavad(bufferString.at(currentIndex));
		int symbolTextLength = strlen(symbolText);
		bufferString.replace(currentIndex, 1, symbolText);
		currentIndex += symbolTextLength;
		if (bufferString.at(currentIndex) != ' ')
			bufferString.insert(currentIndex, 1, ' ');

		currentIndex += 1;
	}

	// Phrases
	applyPhrasesFilter(&bufferString);

	// Checking if buffer is OK
	int currentSize = bufferString.length();
	if(currentSize > bufferSize) return ERROR_TEXTNORMALIZATION_BUFFER_OVERFLOW;

	// Filling return buffer
	memcpy(retBuffer, bufferString.c_str(), currentSize);	
	retBuffer[currentSize] = '\0';

	return NO_ERR;
}

EXPORT int normalizeText(char * buffer, char * retBuffer, int bufferSize, int * letPos)
{
		stringWithLetterPosition bufferString(buffer, letPos, bufferSize);

		if (bufferString.at(bufferString.length()-1) != '\n') bufferString.append("\n");

		char bruks = '�';

		string wordSeparatorsList = "\t [{(\"��']}).?!;:,-\r\n=";
		wordSeparatorsList.append(1, bruks);
		string leadAndTrailSymbolsList = "[{()}]\"'";
		string digitsList = "1234567890";
		string smallLettersList = "a\xE0""bc\xE8""de\xE6\xEB""fghi\xE1yjklmnoprs\xF0tu\xF8\xFBvz\xFExwq";
		string capitalLettersList = "A\xC0""BC\xC8""DE\xC6\xCB""FGHI\xC1YJKLMNOPRS\xD0TU\xD8\xDBVZ\xDEXWQ";
		string lettersList = smallLettersList + capitalLettersList;
		string phraseSeparatorsList = ".?!;:\r\n,";
		phraseSeparatorsList.append(1, bruks);

		int currentFileBuffer = 0;

		for (int rn = 0; rn < totalRules; rn++)
		{
			string rulesBufferString = rulesArray[rn];

			int findResult = -1;

			findResult = rulesBufferString.find("\\r", 0);
			while (findResult != -1) 
			{
				rulesBufferString.replace(findResult, 2, "\r"); 
				findResult = rulesBufferString.find("\\r", 0); 
			}
			findResult = rulesBufferString.find("\\n", 0);
			while (findResult != -1) 
			{ 
				rulesBufferString.replace(findResult, 2, "\n"); 
				findResult = rulesBufferString.find("\\n", 0); 
			}

            findResult = rulesBufferString.find("\\t", 0);
            while (findResult != -1)
            {
            	rulesBufferString.replace(findResult, 2, "\t");
            	findResult = rulesBufferString.find("\\t", 0);
            }

			int paramCounter = 0;
			int functionNameIndex = rulesBufferString.find(':');
			string functionName = rulesBufferString.substr(0, functionNameIndex);
			int param1Index = rulesBufferString.find('@', functionNameIndex+1);
			int param2Index = rulesBufferString.find('@', param1Index+1);
			string param1Text, param2Text;
			if (param1Index != -1) {
				param1Text = rulesBufferString.substr(functionNameIndex+1, (param1Index-functionNameIndex-1));
				if (param1Text != "")
					paramCounter++;
			}
			if ((param1Index != -1) && (param2Index != -1)) {
				param2Text = rulesBufferString.substr(param1Index+1, (param2Index-param1Index-1));
				paramCounter++;
			}


			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (functionName == "Replace" && paramCounter == 2) 
			{			
				findResult = bufferString.find(param1Text);
				while (findResult != -1) 
				{
					bufferString.replace(findResult, param1Text.length(), param2Text); 
					findResult = bufferString.find(param1Text);
				}			
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (functionName == "ReplaceWithFile" && paramCounter == 1)
			{	
				for (int g = 0; g < abbSizes[currentFileBuffer]; g++)
				{					
					findResult = bufferString.find(abbLists[currentFileBuffer][g]);
					while (findResult != -1) 
					{
						bufferString.replace(findResult, strlen(abbLists[currentFileBuffer][g]), abbListsSubstitutions[currentFileBuffer][g]); 
						findResult = bufferString.find(abbLists[currentFileBuffer][g]);
					}		
					
				}
				currentFileBuffer++;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "RomanNumerals" && paramCounter == 2)
			{
				findResult = bufferString.find(param1Text);
				while (findResult != -1) 
				{
					int findResult2 = bufferString.find(param2Text);
					if (findResult2 != -1)
					{
						string strToReplace = "\nPIRMAS";
						int findResultDif = findResult2-findResult;
						if (param1Text[1] == 'V')
						{
							findResultDif = findResult-findResult2;
							strToReplace = "\nPENKTAS";
						}

						if ((findResultDif < 500) && (findResultDif > 0))
						{
							bufferString.replace(findResult, param1Text.length(), strToReplace); 
						}
					}
					findResult = bufferString.find(param1Text, findResult+1);
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "DigitsPattern" && (paramCounter == 1 || paramCounter == 2)) 
			{				
				findResult = param1Text.find('(');
				string patternSeed = param1Text.substr(0, findResult);				
				int patternSeedLength = patternSeed.length();
				int patternFinish = 0;
				int patternStart = bufferString.find(patternSeed, patternFinish);
				while (patternStart != -1)
				{					
					string patternString = param1Text.substr(patternSeedLength);
					patternFinish = patternStart+patternSeedLength;
					string filledPattern = "";
					//bool isPatternFits = true;
					int foundSymbolsNum = 0;
					for (int z = patternStart+patternSeedLength; z < bufferString.length(); z++)
					{
						if (patternString == "")
							break;

							int currentGroupFinish = patternString.find(')');
							string currentGroupString = patternString.substr(1, currentGroupFinish-1);
							patternString = patternString.substr(currentGroupFinish+1);
							int maxSymbolsNumStart = currentGroupString.find('[');
							bool isFound = false;
							for (int u = 0; u < maxSymbolsNumStart; u++)
							{
								if (((currentGroupString[u] == 'D') || (currentGroupString[u] == 'd')) && (digitsList.find(bufferString.at(z)) != -1))
								{
									isFound = true;
									filledPattern += currentGroupString[u];
									break;
								}
								else if (((currentGroupString[u] == 'L') || (currentGroupString[u] == 'l')) && (lettersList.find(bufferString.at(z)) != -1))
								{
									isFound = true;
									filledPattern += currentGroupString[u];
									break;
								}
								else if (bufferString.at(z) == currentGroupString[u])
								{
									isFound = true;
									filledPattern += currentGroupString[u];
									break;
								}
							}
							if (isFound == true)
							{
								foundSymbolsNum++;
								patternFinish = z;
								int minSymbolsNum, maxSymbolsNum;
								string tmpBuf = currentGroupString.substr(maxSymbolsNumStart+1, currentGroupString.length()-maxSymbolsNumStart-2);
								sscanf(tmpBuf.c_str(), "%d-%d", &minSymbolsNum, &maxSymbolsNum);
								maxSymbolsNum--;
								if (maxSymbolsNum > 0)
								{
									tmpBuf = currentGroupString.substr(0, maxSymbolsNumStart);
									char tmpBufChar[256];
									sprintf(tmpBufChar, "(%s[%d-%d])", tmpBuf.c_str(), minSymbolsNum, maxSymbolsNum);
									patternString = tmpBufChar + patternString;
								}
								else
									foundSymbolsNum = 0;
							}
							else
							{
								int minSymbolsNum, maxSymbolsNum;
								string tmpBuf = currentGroupString.substr(maxSymbolsNumStart+1, currentGroupString.length()-maxSymbolsNumStart-2);
								sscanf(tmpBuf.c_str(), "%d-%d", &minSymbolsNum, &maxSymbolsNum);

								if (foundSymbolsNum >= minSymbolsNum)
								{
									z--;
									continue;
								}
								else
									break;
							}
					}

					//Read by letter from patternStart to patternFinish
					if ((patternFinish - (patternStart+patternSeedLength)) > 0)
					{
						bool addPauses = false;
						
						if (param2Text == "addPauses")
							addPauses = true;

						int wordStartIndex = (patternStart+patternSeedLength);
						int wordEndIndex = patternFinish;
						int patternIndex = 0;
						for (int w = wordStartIndex; w <= wordEndIndex; w++)
						{								
							if ((addPauses == true) && (bufferString.at(w) == ' '))
							{
								bufferString.replace(w-1, 1, 1, '.');
							}

							if ((filledPattern[patternIndex] == 'L') || (filledPattern[patternIndex] == 'D'))
							{
								const char * symbolText = SimbPavad(bufferString.at(w));
								int symbolTextLength = strlen(symbolText);
								if (symbolTextLength != 0)
								{
									if ((w > 0) && (bufferString.at(w-1) != ' '))
									{
										bufferString.insert(w, 1, ' ');
										wordEndIndex++;
										w++;
									}
									bufferString.replace(w, 1, symbolText);
									wordEndIndex += (symbolTextLength-1);
									w += (symbolTextLength-1);
									if (w < (wordEndIndex-1))
									{
										bufferString.insert(w+1, " ");
										wordEndIndex++;
										w++;
									}
								}
								else
								{
									bufferString.erase(w, 1);
									wordEndIndex--;
									w--;
								}
							}
							patternIndex++;
						}							
					}

					patternStart = bufferString.find(patternSeed, patternFinish);
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "ReadByLetter" && (paramCounter == 1 || paramCounter == 2))
			{
				
				if (param1Text.find("Num[") != -1)
				{
					string patternString = param1Text;
					string patternStringParams = param1Text;
					int numParams = 0;
					int minRange[32];
					int maxRange[32];

					findResult = patternString.find("Num[");
					while (findResult != -1)
					{
						int findResultEnd = patternString.find("]", findResult);
						string range = patternString.substr(findResult+4, (findResultEnd-findResult-4));
						int digitsNum = range.find("-", 0);
						patternString.replace(findResult, (findResultEnd-findResult+1), digitsNum, 'D');
						patternStringParams.replace(findResult, (findResultEnd-findResult+1), digitsNum, numParams);

						minRange[numParams] = atoi(range.substr(0, digitsNum).c_str());
						maxRange[numParams] = atoi(range.substr(digitsNum+1, range.length()-digitsNum).c_str());
						numParams++;

						findResult = patternString.find("Num[");
					}

					for (int k = 0; k < bufferString.length(); k++)
					{
						int patternLength = patternString.length();
						string str = bufferString.substr(k, patternLength);
						bool is_pattern_found = true;
						string paramsText[32];
						for (int i = 0; i < patternString.length(); i++)
						{
							if (patternString[i] == 'D')
							{
								if ((str[i] < 0x30) || (str[i] > 0x39)) {is_pattern_found = false; break; }
								paramsText[patternStringParams[i]] += str[i];
							}
							else if (patternString[i] == '?')
							{
								if (lettersList.find(str[i]) == -1) {is_pattern_found = false; break; }
							}
							else if (patternString[i] == '*') {} //Do nothing 
							else if (patternString[i] != str[i]) {is_pattern_found = false; break; }
						}
						
						if (is_pattern_found == true)
						{
							for (int asd = 0; asd < numParams; asd++)
							{
								if (paramsText[asd] == "") {is_pattern_found = false; break; }

								int ppp = atoi(paramsText[asd].c_str());
								if (ppp < minRange[asd]) {is_pattern_found = false; break; }
								if (ppp > maxRange[asd]) {is_pattern_found = false; break; }
							}
						}

						if (is_pattern_found == true)
						{
							bool addPauses = false;
							
							if (param2Text == "addPauses")
								addPauses = true;


							int wordStartIndex = k;
							int wordEndIndex = k + patternLength;
							for (int w = wordStartIndex; w < wordEndIndex; w++)
							{								
								if ((addPauses == true) && (bufferString.at(w) == ' '))
								{
									bufferString.replace(w-1, 1, 1, '.');
									wordEndIndex++;
									continue;
								}


								if (digitsList.find(bufferString.at(w))!=-1)
								{
									const char * symbolText = SimbPavad(bufferString.at(w));
									int symbolTextLength = strlen(symbolText);
									if (symbolTextLength != 0)
									{
										if ((w > 0) && (bufferString.at(w-1) != ' '))
										{
											bufferString.insert(w, 1, ' ');
											w++;
										}
										bufferString.replace(w, 1, symbolText);
										wordEndIndex += (symbolTextLength-1);
										w += (symbolTextLength-1);
										if (w < (wordEndIndex-1))
										{
											bufferString.insert(w+1, " ");
											wordEndIndex++;
											w++;
										}
									}
									else
									{
										bufferString.erase(w, 1);
										wordEndIndex--;
										w--;
									}
								}
							}
						}
					}
				}
				else if ((param1Text[0] == '{') && (param1Text[param1Text.length()-1] == '}'))//Stress symbols (~^`)
				{					
					string readByLetterSymbolsList = param1Text.substr(1, param1Text.length()-2);//"~^`";
					findResult = bufferString.find_first_of(readByLetterSymbolsList, 0);
					while (findResult != -1)
					{
						bool isToBeSkipped = false;
						if (param2Text == "checkForNoSpaceAfter")
						{
							if (
								(findResult == (bufferString.length()-1)) || 
								(bufferString.at(findResult+1) == ' ') || 
								(bufferString.at(findResult+1) == '\r') || 
								(bufferString.at(findResult+1) == '\n')	
							   )
							{
								isToBeSkipped = true;
								findResult++;
							}
						}
						
						if (isToBeSkipped == false)
						{
							const char * symbolText = SimbPavad(bufferString.at(findResult));
							int symbolTextLength = strlen(symbolText);
												
							if ((findResult > 0) && (bufferString.at(findResult-1) != ' '))
							{
								bufferString.insert(findResult, " ");
								findResult++;
							}
							if ((findResult < (bufferString.length()-1)) && (bufferString.at(findResult+1) != ' '))
								bufferString.insert(findResult+1, " ");

							if ((param2Text == "") || (param2Text == "checkForNoSpaceAfter"))
							{
								if (symbolTextLength != 0)
								{
									bufferString.replace(findResult, 1, symbolText);
									findResult+=symbolTextLength;
								}
								else bufferString.erase(findResult, 1);
							}
							else if (param2Text == "LeaveOriginalDup")
							{
								if (symbolTextLength != 0)
								{
									bufferString.insert(findResult+1, (char*)symbolText);
									findResult++;
									findResult+=symbolTextLength;
								}
							}
						}

						findResult = bufferString.find_first_of(readByLetterSymbolsList, findResult);
					}
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				else if (param1Text == "WordNoVowels")
				{
					int wordStartIndex = 0;
					int wordEndIndex = bufferString.find_first_of(wordSeparatorsList, wordStartIndex);
					string vowelsList = "aeiyou\xE0\xE6\xEB\xE1\xF8\xFB""AEIYOU\xC0\xC6\xCB\xC1\xD8\xDB";

					while (wordEndIndex != -1)
					{
						int wordLength = (wordEndIndex-wordStartIndex);
						if (wordLength > 0)
						{
							bool isWordNoVowels = true;
							for (int w = wordStartIndex; w < wordEndIndex; w++)
							{
								if (vowelsList.find(bufferString.at(w)) != -1)
								{
									isWordNoVowels = false;
									break;
								}
							}

							if (isWordNoVowels == true)
							{
								for (int w = wordStartIndex; w < wordEndIndex; w++)
								{
									const char * symbolText = SimbPavad(bufferString.at(w));
									int symbolTextLength = strlen(symbolText);
									if (symbolTextLength != 0)
									{
										bufferString.replace(w, 1, symbolText);
										wordEndIndex += (symbolTextLength-1);
										w += (symbolTextLength-1);
										if (w < (wordEndIndex-1))
										{
											bufferString.insert(w+1, "-"); //santrumpas skaidom skiemenimis
											wordEndIndex++;
											w++;
										}
									}
									else
									{
										bufferString.erase(w, 1);
										wordEndIndex--;
										w--;
									}
								}
							}
						}
						wordStartIndex = wordEndIndex+1;
						wordEndIndex = bufferString.find_first_of(wordSeparatorsList, wordStartIndex);
					}
				}
			}			
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "Date" && paramCounter == 1)
			{
				int MonthVar = 1;
				if (param1Text.find("{MONTHS}") != -1)
				{
					MonthVar = 12;
				}
				for (int mv = 0; mv < MonthVar; mv++)
				{
					string patternString = param1Text;
					int zxc;
					if (MonthVar == 12)
					{
						zxc = patternString.find("{MONTHS}");
						if (zxc != -1)
							patternString.replace(zxc, 8, months[mv]); 
					}
					for (int k = 0; k < bufferString.length(); k++)
					{
						int patternLength = patternString.length();
						string str = bufferString.substr(k, patternLength);
						if (str.length() != patternLength) break;
								string strNormalized = str;
								if (MonthVar == 12)
								{
									for (int ss = zxc; ss < zxc+months[mv].length(); ss++)
									{
										switch (strNormalized[ss])
										{
										case '\xE0':
										case '\xE8':
										case '\xE6':
										case '\xEB':
										case '\xE1':
										case '\xF0':
										case '\xF8':
										case '\xFB':
										case '\xFE':
											strNormalized[ss]-=0x20;
											break;
										default:
											strNormalized[ss] = toupper(strNormalized[ss]);
											break;
										}
									}
								}
						bool is_pattern_found = true;
						string yearStr, monthStr, dayStr;
						for (int i = 0; i < patternString.length(); i++)
						{
							if (patternString[i] == 'Y')
							{
								if ((strNormalized[i] < 0x30) || (strNormalized[i] > 0x39)) {is_pattern_found = false; break; }
								yearStr += strNormalized[i];
							}
							else if (patternString[i] == 'M')
							{
								if ((strNormalized[i] < 0x30) || (strNormalized[i] > 0x39)) {is_pattern_found = false; break; }
								monthStr += strNormalized[i];
							}
							else if (patternString[i] == 'D')
							{
								if ((strNormalized[i] < 0x30) || (strNormalized[i] > 0x39)) {is_pattern_found = false; break; }
								dayStr += strNormalized[i];
							}
							else if (patternString[i] == '?')
							{
								if (isalpha(strNormalized[i]) == 0) {is_pattern_found = false; break; }
							}
							else if (patternString[i] == '*') {} //Do nothing 
							else if (patternString[i] != strNormalized[i]) {is_pattern_found = false; break; }
						}
						int y = 0; int m = 0; int d = 0;					
						if (is_pattern_found == true)
						{
							y = atoi(yearStr.c_str());
							if (MonthVar == 12)
							{
								m = mv+1;
							}
							else
								m = atoi(monthStr.c_str());
							d = atoi(dayStr.c_str());
							if (y <= 0) is_pattern_found = false;
							else if ((m <= 0) || (m >= 13)) is_pattern_found = false;
							else if ((d <= 0) || (d >= 32)) is_pattern_found = false;
						}
						if (is_pattern_found == true)
						{
							int mode = 0;
							if (str.find(" m.") != -1)
								mode = 1;
							char bufferTmp[256];
							if (expandDate(y,m,d, mode, bufferTmp) == -1)
							{
							}
							else
							{

								if ( (k > 0) && (wordSeparatorsList.find(bufferString.at(k-1)) == -1) )
								{
									bufferString.insert(k, " ");
									k++;
								}
								if ( ((k+patternLength) < bufferString.length()) && (wordSeparatorsList.find(bufferString.at((k+patternLength))) == -1) )
								{
									bufferString.insert((k+patternLength), " ");
								}

								bufferString.replace(k, patternLength, bufferTmp);
							}
						}
					}
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "AbbSeparateWord" && paramCounter == 1)
			{
				for (int g = 0; g < abbSizes[currentFileBuffer]; g++)
				{
					findResult = bufferString.find(abbLists[currentFileBuffer][g], 0);
					while (findResult != -1)
					{						
						bool isSeparateWord = true;

						if (((findResult-1) >= 0) && 
							(wordSeparatorsList.find(bufferString.at(findResult-1)) == -1))
								isSeparateWord = false;

						if (((findResult + strlen(abbLists[currentFileBuffer][g])) <= (bufferString.length()-1)) && 
							(wordSeparatorsList.find(bufferString.at(findResult+strlen(abbLists[currentFileBuffer][g]))) == -1))
								isSeparateWord = false;

						int abbLength = strlen(abbLists[currentFileBuffer][g]);
						if (isSeparateWord == true)
						{
							bool isLeaveSep = false;
							if (abbListsIsWithSep[currentFileBuffer][g] == 1)
							{
								if (((findResult+abbLength+1) < bufferString.length()) && 
									(bufferString.at(findResult+abbLength) == ' ') &&
									(capitalLettersList.find(bufferString.at(findResult+abbLength+1))!=-1))
								{
									isLeaveSep = true;
								}
							}

							if (isLeaveSep == true)
							{
								bufferString.replace(findResult, strlen(abbLists[currentFileBuffer][g])-1, abbListsSubstitutions[currentFileBuffer][g]);
								abbLength = strlen(abbListsSubstitutions[currentFileBuffer][g]) + 1;
							}
							else
							{
								bufferString.replace(findResult, strlen(abbLists[currentFileBuffer][g]), abbListsSubstitutions[currentFileBuffer][g]);
								abbLength = strlen(abbListsSubstitutions[currentFileBuffer][g]);
							}
						}
						findResult = bufferString.find(abbLists[currentFileBuffer][g], findResult+abbLength+1);				
					}			
				}
				currentFileBuffer++;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "Numbers" && paramCounter == 0)
			{
				//Digits	
//{+-}{1234567890}{,}{1234567890}{%$\x80\xA3} \x80 - euras, \xA3 - svaras
//ilgiausias skaicius +777777777777,777777777777% uzima 444 simb.
//ilgesni skaiciai sakomi po skaitmeni, o ilgiausio skaitmens ilgis 9. 
//440 turetu pakakti 44 zenklu skaiciui, imkim 512

				int lastDigitIndex = bufferString.find_first_of(digitsList, 0);
				while (lastDigitIndex != -1)
				{
					int digitsWordStartIndex = lastDigitIndex-1;
					int digitsWordEndIndex = lastDigitIndex+1;
					if ((digitsWordStartIndex >= 0) && ((bufferString.at(digitsWordStartIndex) == '+') || (bufferString.at(digitsWordStartIndex) == '+')))
						digitsWordStartIndex--;
					while ((digitsWordEndIndex < bufferString.length()) && (digitsList.find(bufferString.at(digitsWordEndIndex)) != -1))
						digitsWordEndIndex++;
					if ((digitsWordEndIndex < bufferString.length() - 1) && (bufferString.at(digitsWordEndIndex) == ',') && (digitsList.find(bufferString.at(digitsWordEndIndex + 1)) != -1))
						digitsWordEndIndex += 2;
					while ((digitsWordEndIndex < bufferString.length()) && (digitsList.find(bufferString.at(digitsWordEndIndex)) != -1))
						digitsWordEndIndex++;
					if ((digitsWordEndIndex < bufferString.length()) && ((bufferString.at(digitsWordEndIndex) == '%') || (bufferString.at(digitsWordEndIndex) == '$')
						|| (bufferString.at(digitsWordEndIndex) == '\x80') || (bufferString.at(digitsWordEndIndex) == '\xA3')))
						digitsWordEndIndex++;
					if(digitsWordEndIndex > digitsWordStartIndex + 44) digitsWordEndIndex = digitsWordStartIndex + 44; //apsauga kad tilptu

					string singleWord = bufferString.substr(digitsWordStartIndex+1, (digitsWordEndIndex-digitsWordStartIndex-1));

					char bufferTmp[512];
					VisasSkaicius(&singleWord[0], bufferTmp, 512);
					
					bufferString.replace(digitsWordStartIndex+1, (digitsWordEndIndex-digitsWordStartIndex-1), bufferTmp);
					digitsWordEndIndex += (strlen(bufferTmp)-(digitsWordEndIndex-digitsWordStartIndex-1));

					if ( (digitsWordStartIndex > 0) && (wordSeparatorsList.find(bufferString.at(digitsWordStartIndex)) == -1) )
					{
						bufferString.insert(digitsWordStartIndex+1, " ");
						digitsWordEndIndex++;
					}
					if ( (digitsWordEndIndex < (bufferString.length()-1)) && (wordSeparatorsList.find(bufferString.at(digitsWordEndIndex)) == -1) )
					{
						bufferString.insert(digitsWordEndIndex, " ");
						digitsWordEndIndex++;
					}
							
					lastDigitIndex = bufferString.find_first_of(digitsList, digitsWordEndIndex);
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "Phrases" && paramCounter == 0)//Phrase boundaries			
			{			
				

				int phraseStartIndex = 0;
				int phraseEndIndex = bufferString.find_first_of(phraseSeparatorsList, phraseStartIndex);
				while (phraseEndIndex != -1)
				{
					if ( ((bufferString.at(phraseEndIndex) != bruks) && (bufferString.at(phraseEndIndex) != ',')) || 

						 ((bufferString.at(phraseEndIndex) == ',') && (((phraseEndIndex - phraseStartIndex) >= 25) && ((phraseEndIndex - phraseStartIndex) <= 120))) ||

						 ((bufferString.at(phraseEndIndex) == bruks) && 
						 (phraseEndIndex > 0) && 
						 (phraseEndIndex < (bufferString.length()-1)) && 
						 (bufferString.at(phraseEndIndex+1) == ' ') && 
						 (bufferString.at(phraseEndIndex-1) == ' ') && 
						 (((phraseEndIndex - phraseStartIndex) >= 25) && ((phraseEndIndex - phraseStartIndex) <= 120)))
						)
					{
						if ( (bufferString.at(phraseEndIndex) != '\n') && 
							 ((phraseEndIndex >= (bufferString.length()-1)) || (bufferString.at(phraseEndIndex+1) != '\n'))
							)
						{
							bufferString.insert(phraseEndIndex+1, "\n");
							phraseEndIndex+=1;					
						}
					}
					else
					{

						bufferString.erase(phraseEndIndex, 1);
						phraseEndIndex-=1;
					}

					if ((phraseEndIndex - phraseStartIndex) >= 120)
					{
						string phraseSecondarySeparatorsList = "[{(]})\"'";
						int r = bufferString.find_first_of(phraseSecondarySeparatorsList, phraseStartIndex+25);
						if ((r != -1) && (r <= phraseStartIndex+120))
						{
							bufferString.insert(r+1, "\n");
							phraseEndIndex=r+1;		
						}
						else
						{
							string phraseTertiarySeparatorsList[] = {"ir","bei","ar","arba"};
							int r = -1;
							int bestR = bufferString.length();
							int bestI = -1;
							for (int i = 0; i < 4; i++)
							{
								r = bufferString.find(phraseTertiarySeparatorsList[i], phraseStartIndex+55);
								if ((r != -1) && (r < bestR) && (r <= phraseStartIndex+120))
								{
									bool isSeparateWord = true;

									if (((r-1) >= 0) && 
										(wordSeparatorsList.find(bufferString.at(r-1)) == -1))
											isSeparateWord = false;

									if (((r + phraseTertiarySeparatorsList[i].length()) <= (bufferString.length()-1)) && 
										(wordSeparatorsList.find(bufferString.at(r+phraseTertiarySeparatorsList[i].length())) == -1))
											isSeparateWord = false;

									if (isSeparateWord == true)
									{
										bestR = r;
										bestI = i;
									}
								}
							}
							if (bestR != bufferString.length())
							{
								bufferString.insert(bestR, "\n");
								phraseEndIndex=bestR; 
							}
							else
							{
								int r = bufferString.rfind(' ', phraseStartIndex+120);
								if (r != -1)
								{
									bufferString.insert(r+1, "\n");
									phraseEndIndex=r+1;	
								}
								else
								{
									bufferString.insert(phraseStartIndex+120, "\n");
									phraseEndIndex=phraseStartIndex+120;	
								}
							}
						}
					}

					phraseStartIndex = phraseEndIndex+1;
					phraseEndIndex = bufferString.find_first_of(phraseSeparatorsList, phraseStartIndex);
				}

				findResult = bufferString.find_first_of(leadAndTrailSymbolsList, 0);
				while (findResult != -1)
				{
					bufferString.erase(findResult, 1);
					findResult = bufferString.find_first_of(leadAndTrailSymbolsList, 0);
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (functionName == "ToUPPERCASE" && paramCounter == 0)
			{
				//ToUPPERCASE
				int s = bufferString.length();
				for (int g = 0; g < s; g++)
				{
					switch (bufferString.at(g))
					{
					case '\xE0':
					case '\xE8':
					case '\xE6':
					case '\xEB':
					case '\xE1':
					case '\xF0':
					case '\xF8':
					case '\xFB':
					case '\xFE':
						bufferString.set_at(g, bufferString.at(g)-0x20);
						break;
					default:
						bufferString.set_at(g, toupper(bufferString.at(g)));
						break;
					}
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		int retBufferSize = bufferString.length();

		if(retBufferSize > bufferSize) return ERROR_TEXTNORMALIZATION_BUFFER_OVERFLOW; 

		memcpy(retBuffer, bufferString.c_str(), retBufferSize);	
		retBuffer[retBufferSize] = '\0';
	
	return NO_ERR;
}

