///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas TextNormalization.dll
// Failas stringWithLetterPosition.h
// Autorius dr. Tomas Anbinderis
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRING_WITH_LETTER_POSITION_H_INCLUDED
#define STRING_WITH_LETTER_POSITION_H_INCLUDED

#include <string>
using namespace std;

class stringWithLetterPosition
{
public:
	stringWithLetterPosition(char* initialBuffer, int* pLetPos, int letPosBufferMaxSize);
	~stringWithLetterPosition(void);
	string buffer;
	int letPosMaxBuffer;
	int* letPos;
	char at(int p);
	int length();
	void append(char* str);
	int find(string str);
	int find(string str, int p);
	void replace(int p, int l, string str);
	void replace(int p, int l, int s, char c);
	void insert(int p, int l, char c);
	void insert(int p, char* str);
	void erase(int p, int l);
	string substr(int p, int l);
	int find_first_of(string str, int p);
	int rfind(char c, int p);
	void set_at(int p, char c);
	const char* c_str();
private:
	void replaceLetPos(int * letPos, int pra, int ilgo, int ilgn);
};

#endif // STRING_WITH_LETTER_POSITION_H_INCLUDED
