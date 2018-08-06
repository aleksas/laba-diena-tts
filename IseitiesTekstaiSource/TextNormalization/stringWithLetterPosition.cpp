///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas TextNormalization.dll
// Failas stringWithLetterPosition.cpp
// Autorius dr. Tomas Anbinderis
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "stringWithLetterPosition.h"

stringWithLetterPosition::stringWithLetterPosition(char* initialBuffer, int* pLetPos, int letPosBufferMaxSize)
{
	buffer.assign(initialBuffer);
	letPos = pLetPos;
	letPosMaxBuffer = letPosBufferMaxSize;
}

stringWithLetterPosition::~stringWithLetterPosition(void)
{
}

void stringWithLetterPosition::set_at(int p, char c)
{
	buffer[p] = c;
}

char stringWithLetterPosition::at(int p)
{
	return buffer.at(p);
}

int stringWithLetterPosition::length()
{
	return buffer.length();
}

int stringWithLetterPosition::find(string str)
{
	return buffer.find(str);
}

int stringWithLetterPosition::find(string str, int p)
{
	return buffer.find(str, p);
}

int stringWithLetterPosition::find_first_of(string str, int p)
{
	return buffer.find_first_of(str, p);
}

int stringWithLetterPosition::rfind(char c, int p)
{
	return buffer.rfind(c, p);
}

string stringWithLetterPosition::substr(int p, int l)
{
	return buffer.substr(p, l);
}

const char* stringWithLetterPosition::c_str()
{
	return buffer.c_str();
}

void stringWithLetterPosition::replaceLetPos(int * letPos, int pra, int ilgo, int ilgn)
{
	int ilg_skirt = (ilgn-ilgo);
	
	int letPosPab = buffer.length();

	int i;
	if (ilg_skirt < 0)
	{
		for(i = pra+ilgn; i < letPosPab; i++)
			letPos[i] = letPos[i-ilg_skirt];
	}
	else if (ilg_skirt > 0)
	{
		if (letPosPab + ilg_skirt >= letPosMaxBuffer)
			return;

		for(i = letPosPab+ilg_skirt; i >= pra+ilgo+ilg_skirt; i--)
			letPos[i] = letPos[i-ilg_skirt];
	}

	for(i = pra+1; i < pra+ilgn; i++)
		letPos[i] = letPos[pra];
}

void stringWithLetterPosition::replace(int p, int l, string str)
{
	replaceLetPos(letPos, p, l, str.length());	
	buffer.replace(p, l, str);
}

void stringWithLetterPosition::replace(int p, int l, int s, char c)
{
	replaceLetPos(letPos, p, l, s);	
	buffer.replace(p, l, s, c);
}

void stringWithLetterPosition::insert(int p, char* str)
{
	replaceLetPos(letPos, p, 0, strlen(str));
	buffer.insert(p, str);
}

void stringWithLetterPosition::insert(int p, int l, char c)
{
	replaceLetPos(letPos, p, 0, l);
	buffer.insert(p, l, c);
}

void stringWithLetterPosition::erase(int p, int l)
{
	int letPosPab = 0;
	letPosPab = buffer.length();
	for(int i = p; i < letPosPab; i++)
		letPos[i] = letPos[i+l];

	buffer.erase(p, l);
}

void stringWithLetterPosition::append(char* str)
{
	int letPosPab = buffer.length();
	int pridetiIlgis = strlen(str);

	if (letPosPab + pridetiIlgis >= letPosMaxBuffer)
		return;

	for(int i = 0; i < pridetiIlgis; i++)
	{
		letPos[letPosPab] = letPos[letPosPab-1];
		letPosPab++;
	}

	buffer.append(str);
}
