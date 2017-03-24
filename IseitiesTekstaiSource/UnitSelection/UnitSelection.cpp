///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas UnitSelection.dll
// Failas UnitSelection.cpp
// Autorius dr. Tomas Anbinderis
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LithUSS_Error.h"
#include <string>
using namespace std;

#define MAX_UNITS 200000
#define MAX_DIFFERENT_UNITS 92
#define MAX_SAME_UNIT 32*1024

char* strtokf(char*, const char*, char**);

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	return TRUE;
}

unsigned short unitsDB_ID[MAX_UNITS];
int unitsDB_E1[MAX_UNITS];
int unitsDB_E2[MAX_UNITS];
char unitsDB_NextSep[MAX_UNITS];
int n_total_DBunits;
int unitsWeights[MAX_UNITS];
int unitsLengths[MAX_UNITS];

int E1_koef;
int E2_koef;
float JK_koef1;
float JK_koef2;
int keitimoKainuSvoris;
int jungimoKainuSvoris;
int unitsWeightsKoef;
int unitsLengthsKoef;

int unit_segments_lengths[MAX_DIFFERENT_UNITS];
int unit_segments[MAX_DIFFERENT_UNITS][MAX_SAME_UNIT];
int jungimo_kainos_precalculated[MAX_DIFFERENT_UNITS][MAX_DIFFERENT_UNITS];
int keitimo_kaina_DK_precalculated[MAX_DIFFERENT_UNITS][MAX_DIFFERENT_UNITS][MAX_DIFFERENT_UNITS];
int keitimo_kaina_KK_precalculated[MAX_DIFFERENT_UNITS][MAX_DIFFERENT_UNITS][MAX_DIFFERENT_UNITS];

const int FonSk = 92;
static struct FonVardai{ char *fv; unsigned short id; } FonV[FonSk] = {
	{ "_", 0 },
	{ "i", 1 },
	{ "e", 2 },
	{ "a", 3 },
	{ "o", 4 },
	{ "u", 5 },
	{ "I", 6 },
	{ "E", 7 },
	{ "A", 8 },
	{ "O", 9 },
	{ "U", 10 },
	{ "ii", 11 },
	{ "Ii", 12 },
	{ "iI", 13 },
	{ "ie", 14 },
	{ "Ie", 15 },
	{ "iE", 16 },
	{ "ee", 17 },
	{ "Ee", 18 },
	{ "eE", 19 },
	{ "ea", 20 },
	{ "Ea", 21 },
	{ "eA", 22 },
	{ "aa", 23 },
	{ "Aa", 24 },
	{ "aA", 25 },
	{ "oo", 26 },
	{ "Oo", 27 },
	{ "oO", 28 },
	{ "uo", 29 },
	{ "Uo", 30 },
	{ "uO", 31 },
	{ "uu", 32 },
	{ "Uu", 33 },
	{ "uU", 34 },
	{ "p", 35 },
	{ "p\'", 36 },
	{ "b", 37 },
	{ "b\'", 38 },
	{ "t", 39 },
	{ "t\'", 40 },
	{ "d", 41 },
	{ "d\'", 42 },
	{ "k", 43 },
	{ "k\'", 44 },
	{ "g", 45 },
	{ "g\'", 46 },
	{ "ts", 47 },
	{ "ts\'", 48 },
	{ "dz", 49 },
	{ "dz\'", 50 },
	{ "tS", 51 },
	{ "tS\'", 52 },
	{ "dZ", 53 },
	{ "dZ\'", 54 },
	{ "s", 55 },
	{ "s\'", 56 },
	{ "z", 57 },
	{ "z\'", 58 },
	{ "S", 59 },
	{ "S\'", 60 },
	{ "Z", 61 },
	{ "Z\'", 62 },
	{ "x", 63 },
	{ "x\'", 64 },
	{ "h", 65 },
	{ "h\'", 66 },
	{ "f", 67 },
	{ "f\'", 68 },
	{ "j\'", 69 },
	{ "j", 70 },
	{ "J", 71 },
	{ "v", 72 },
	{ "v\'", 73 },
	{ "w", 74 },
	{ "W", 75 },
	{ "l", 76 },
	{ "l\'", 77 },
	{ "L", 78 },
	{ "L\'", 79 },
	{ "r", 80 },
	{ "r\'", 81 },
	{ "R", 82 },
	{ "R\'", 83 },
	{ "m", 84 },
	{ "m\'", 85 },
	{ "M", 86 },
	{ "M\'", 87 },
	{ "n", 88 },
	{ "n\'", 89 },
	{ "N", 90 },
	{ "N\'", 91 }};

unsigned short fv2id(char *fpav)
{
	for (int i = 0; i<FonSk; i++)
	if (strcmp(fpav, FonV[i].fv) == 0)
		return FonV[i].id;

	return FonV[0].id; //pauze "_"
}

int getDBFromFile(char * dataBaseFileName)
{
	FILE * pFile;
	pFile = fopen(dataBaseFileName, "r");
	if (pFile == NULL) return ERROR_UNITSELECTION_OPENING_DB_FON_FILE;

	unsigned short PAUSE_UNIT_ID;
	PAUSE_UNIT_ID = fv2id("_");

	char laikinasBuferis[1024];
	char visaEilute[1024];
	char wordBufferChar[128];

	while (fgets(visaEilute, 1024, pFile) != NULL)
	{
		int totalSylsNum;
		int currentSylNum;		

		int totalWordsNum = 0;
		int ii = 0;
		while ((visaEilute[ii] != '\n' ) && (visaEilute[ii] != '\r' ) && (visaEilute[ii] != 0 ) && (ii < 1024))
		{
			if(visaEilute[ii] == '+') totalWordsNum++;
			ii++;
		}
		totalWordsNum--;

		int eilutesIlgis = ii;
		if (eilutesIlgis < 4 || visaEilute[0] != '_' || visaEilute[1] != '+' || visaEilute[eilutesIlgis-2] != '+' || visaEilute[eilutesIlgis-1] != '_')
		{
			fclose(pFile);
			return ERROR_UNITSELECTION_READING_DB_FON_FILE;
		}

		int currentWordNum = 0;

		unitsDB_ID[n_total_DBunits] = PAUSE_UNIT_ID;
		unitsDB_NextSep[n_total_DBunits] = '+';
		n_total_DBunits++;
		if (n_total_DBunits == MAX_UNITS) { fclose(pFile); return ERROR_UNITSELECTION_DB_FON_EXCEEDED_MAX_UNITS; }

		int last_sep = -1;
		char last_sep_ch;
		int index = 0;
		while ((visaEilute[index] != '\n') && (visaEilute[index] != '\r') && (visaEilute[index] != 0) && (index < 1024))
		{
			if ((visaEilute[index] == '-') || (visaEilute[index] == '+') || (visaEilute[index] == ' '))
			{
				int sep = index;
				if (last_sep >= 0)
				{
					if (unitsDB_NextSep[n_total_DBunits - 1] == '+')
					{
						currentWordNum++;
						currentSylNum = 1;

						int wordEndNum = (int)(strchr(visaEilute + last_sep + 1, '+') - visaEilute);
						memcpy(wordBufferChar, visaEilute + last_sep + 1, (wordEndNum - last_sep - 1));
						wordBufferChar[(wordEndNum - last_sep - 1)] = '\0';

						totalSylsNum = 1;
						int ii = 0;
						while ((wordBufferChar[ii] != 0) && (ii < 1024))
						{
							if (wordBufferChar[ii] == '-') totalSylsNum++;
							ii++;
						}
					}

					if (unitsDB_NextSep[n_total_DBunits - 1] == '-')
					{
						currentSylNum++;
					}

					memcpy(laikinasBuferis, visaEilute + last_sep + 1, (sep - last_sep - 1));
					laikinasBuferis[(sep - last_sep - 1)] = '\0';
					unitsDB_ID[n_total_DBunits] = fv2id(laikinasBuferis);

					unitsDB_NextSep[n_total_DBunits] = visaEilute[sep];

					unitsDB_E1[n_total_DBunits] = 0;
					unitsDB_E2[n_total_DBunits] = 0;

					if (totalWordsNum == 1)
						unitsDB_E1[n_total_DBunits] = 5;
					else if (currentWordNum == 1)
						unitsDB_E1[n_total_DBunits] = 1;
					else if (currentWordNum == totalWordsNum)
						unitsDB_E1[n_total_DBunits] = 4;
					else
						unitsDB_E1[n_total_DBunits] = 2;

					if (totalSylsNum == 1)
						unitsDB_E2[n_total_DBunits] = 5;
					else if (currentSylNum == 1)
						unitsDB_E2[n_total_DBunits] = 1;
					else if (currentSylNum == totalSylsNum)
						unitsDB_E2[n_total_DBunits] = 4;
					else
						unitsDB_E2[n_total_DBunits] = 2;

					n_total_DBunits++;
					if (n_total_DBunits == MAX_UNITS) { fclose(pFile); return ERROR_UNITSELECTION_DB_FON_EXCEEDED_MAX_UNITS; }
				}

				last_sep = sep;
				last_sep_ch = visaEilute[sep];
			}
			index++;
		}

		unitsDB_ID[n_total_DBunits] = PAUSE_UNIT_ID;
		n_total_DBunits++;
		if (n_total_DBunits == MAX_UNITS) { fclose(pFile); return ERROR_UNITSELECTION_DB_FON_EXCEEDED_MAX_UNITS; }
	}
	fclose(pFile);

	if (n_total_DBunits == 0)
		return ERROR_UNITSELECTION_READING_DB_FON_FILE;

	int x, z;
	for (x = 0; x < n_total_DBunits; x++)
	{
		z = unitsDB_ID[x];
		unit_segments[z][unit_segments_lengths[z]] = x;
		unit_segments_lengths[z]++;
	}

	return NO_ERR;
}

int initCosts(char * dataBaseDirName)
{
	int i, j, k;

	FILE * file;
	char buffer[10 * 1024];

	char laikKat[200];
	strcpy(laikKat, dataBaseDirName);
	strcat(laikKat, "jungimo_kainos.txt");

	file = fopen(laikKat, "r");
	if (file == NULL) return ERROR_UNITSELECTION_OPENING_JOINING_COSTS_FILE;
	else
	{
		i = 0;
		while (!feof(file))
		{
			if (fgets(buffer, 10 * 1024, file) != NULL)
			{
				string bufferString(buffer);
				int lastSep = 0;
				int nextSep = bufferString.find(' ', lastSep);
				j = 0;
				while (nextSep != -1)
				{
					string singleValue = bufferString.substr(lastSep, (nextSep - lastSep));
					jungimo_kainos_precalculated[i][j] = atoi(singleValue.c_str());
					lastSep = nextSep + 1;
					nextSep = bufferString.find(' ', lastSep);
					j++;
				}
				if ( j != 0 )
				{
					if ( j != FonSk )
					{
						fclose(file);
						return ERROR_UNITSELECTION_READING_JOINING_COSTS_FILE;
					}
					i++;
				}				
			}			
		}
		fclose(file);
		if ( i != FonSk )
		{
			return ERROR_UNITSELECTION_READING_JOINING_COSTS_FILE;
		}
	}

	strcpy(laikKat, dataBaseDirName);
	strcat(laikKat, "keitimo_kainos_KK.txt");

	file = fopen(laikKat, "r");
	if (file == NULL) return ERROR_UNITSELECTION_OPENING_LEFT_SUBSTITUTION_COSTS_FILE;
	else
	{
		k = 0;
		i = 0;
		while (!feof(file))
		{
			if (fgets(buffer, 10 * 1024, file) != NULL)
			{
				string bufferString(buffer);
				int lastSep = 0;
				int nextSep = bufferString.find(' ', lastSep);
				j = 0;
				while (nextSep != -1)
				{
					string singleValue = bufferString.substr(lastSep, (nextSep - lastSep));
					keitimo_kaina_KK_precalculated[k][i][j] = atoi(singleValue.c_str());
					lastSep = nextSep + 1;
					nextSep = bufferString.find(' ', lastSep);
					j++;
				}

				if ( j != 0 )
				{
					if ( j != FonSk )
					{
						fclose(file);						
						return ERROR_UNITSELECTION_READING_LEFT_SUBSTITUTION_COSTS_FILE;
					}
					i++;
				}
			}
			if (i == FonSk)
			{
				i = 0;
				k++;
			}
		}
		fclose(file);
		if ( k != FonSk )
		{
			return ERROR_UNITSELECTION_READING_LEFT_SUBSTITUTION_COSTS_FILE;
		}
	}

	strcpy(laikKat, dataBaseDirName);
	strcat(laikKat, "keitimo_kainos_DK.txt");

	file = fopen(laikKat, "r");
	if (file == NULL) return ERROR_UNITSELECTION_OPENING_RIGHT_SUBSTITUTION_COSTS_FILE;
	else
	{
		k = 0;
		i = 0;
		while (!feof(file))
		{
			if (fgets(buffer, 10 * 1024, file) != NULL)
			{
				string bufferString(buffer);
				int lastSep = 0;
				int nextSep = bufferString.find(' ', lastSep);
				j = 0;
				while (nextSep != -1)
				{
					string singleValue = bufferString.substr(lastSep, (nextSep - lastSep));
					keitimo_kaina_DK_precalculated[k][i][j] = atoi(singleValue.c_str());
					lastSep = nextSep + 1;
					nextSep = bufferString.find(' ', lastSep);
					j++;
				}
				if ( j != 0 )
				{
					if ( j != FonSk )
					{
						fclose(file);						
						return ERROR_UNITSELECTION_READING_RIGHT_SUBSTITUTION_COSTS_FILE;
					}
					i++;
				}
			}
			if (i == FonSk)
			{
				i = 0;
				k++;
			}
		}
		fclose(file);
		if ( k != FonSk )
		{
			return ERROR_UNITSELECTION_READING_RIGHT_SUBSTITUTION_COSTS_FILE;
		}
	}

	return NO_ERR;
}

int getDBWeightsAndLengthsFromFile(char * dataBaseFileName)
{
	for (int i = 0; i < n_total_DBunits; i++)
	{
		unitsLengths[i] = 0;
		unitsWeights[i] = 0;
	}
	FILE * pFile;
	pFile = fopen(dataBaseFileName, "r");
	if (pFile == NULL) return ERROR_UNITSELECTION_OPENING_DB_FON_WEIGHTS_FILE;
	string tmpBuffer;
	char tmpCharBuffer[1024];
	int n_total_DBunits_tmp = 0;
	while (fgets(tmpCharBuffer, 1024, pFile) != NULL)
	{
		char *pch, *newpch;
		pch = strtokf(tmpCharBuffer, "\t", &newpch);
		int column = 0;
		while (pch != NULL)
		{
			int sscanfRes = 0;
			switch (column)
			{
			case 1: sscanfRes = sscanf(pch, "%d", &unitsLengths[n_total_DBunits_tmp]); break;
			case 2: sscanfRes = sscanf(pch, "%d", &unitsWeights[n_total_DBunits_tmp]); n_total_DBunits_tmp++; break;
			}
			if (column > 0 && sscanfRes != 1)
				return ERROR_UNITSELECTION_READING_DB_FON_WEIGHTS_FILE;
			column++;

			pch = strtokf(NULL, "\t", &newpch);
		}
	}
	fclose(pFile);
	if (n_total_DBunits_tmp != n_total_DBunits)
	{
		return ERROR_UNITSELECTION_READING_DB_FON_WEIGHTS_FILE;
	}
	return NO_ERR;
}

int initUnitSel(char * dataBaseDirName)
{
	char laikKat[200];
		
	// Nuskaitome nustatymus is UnitSelectionSettings.txt
	strcpy(laikKat, dataBaseDirName);
	strcat(laikKat, "UnitSelectionSettings.txt");
	FILE * pFile = fopen(laikKat, "r");
	if(pFile == NULL) return ERROR_UNITSELECTION_OPENING_SETTINGS_FILE;
	int lineNum = 0;
	char tmpBuffer[128];
	int sscanf_res;
	while (fgets(tmpBuffer, 128, pFile) != NULL)
	{	
		sscanf_res = 0;
		switch (lineNum)
		{
		case 0: sscanf_res = sscanf(tmpBuffer, "%d", &E1_koef); break;
		case 1: sscanf_res = sscanf(tmpBuffer, "%d", &E2_koef); break;
		case 2: sscanf_res = sscanf(tmpBuffer, "%f", &JK_koef1); break;
		case 3: sscanf_res = sscanf(tmpBuffer, "%f", &JK_koef2); break;
		case 4: sscanf_res = sscanf(tmpBuffer, "%d", &keitimoKainuSvoris); break;
		case 5: sscanf_res = sscanf(tmpBuffer, "%d", &jungimoKainuSvoris); break;
		case 6: sscanf_res = sscanf(tmpBuffer, "%d", &unitsWeightsKoef); break;
		case 7: sscanf_res = sscanf(tmpBuffer, "%d", &unitsLengthsKoef); break;
		}
		lineNum++;
		if (sscanf_res != 1)
		{
			fclose(pFile);	
			return ERROR_UNITSELECTION_READING_SETTINGS_FILE;
		}
	}
	fclose(pFile);	
	if (lineNum < 8)
		return ERROR_UNITSELECTION_READING_SETTINGS_FILE;

	int res;
	// Pradines reiksmes (init)
	n_total_DBunits = 0;
	for (int i = 0; i < MAX_DIFFERENT_UNITS; i++)
		unit_segments_lengths[i] = 0;
	if ((res=initCosts(dataBaseDirName)) != 0) return res;

	// Nuskaitome DB is db_fon.txt
	strcpy(laikKat, dataBaseDirName);
	strcat(laikKat, "db_fon.txt");
	if ((res=getDBFromFile(laikKat)) != 0) return res;
	
	// Nuskaitome svorius is db_fon_weights.txt
	strcpy(laikKat, dataBaseDirName);
	strcat(laikKat, "db_fon_weights.txt");
	if ((res=getDBWeightsAndLengthsFromFile(laikKat)) != 0) return res;
		
	return NO_ERR;//SUCCESS;
}

int selectUnits(unsigned short unitsRow[], unsigned short unitsRowNextSeparators[], unsigned short unitsRowDurr[], int unitsRowLength, int retUnits[], int * retCost)
{
	int MAX_SEQUENCES = unitsRowLength * 800;

	int n_total_units_seq = 0;
	int seq_index = 0;

	int *p_units_sequences_arrayunit_num_in_DB = (int*)malloc(MAX_SEQUENCES * sizeof(int));
	if (p_units_sequences_arrayunit_num_in_DB == NULL)
	{
		return ERROR_UNITSELECTION_MEMORY_ALLOCATION;
	}

	int *p_units_sequences_arraylast_seq_index = (int*)malloc(MAX_SEQUENCES * sizeof(int));
	if (p_units_sequences_arraylast_seq_index == NULL)
	{
		free(p_units_sequences_arrayunit_num_in_DB);
		return ERROR_UNITSELECTION_MEMORY_ALLOCATION;
	}

	int *p_units_sequences_arraycost = (int*)malloc(MAX_SEQUENCES * sizeof(int));
	if (p_units_sequences_arraycost == NULL)
	{
		free(p_units_sequences_arrayunit_num_in_DB);
		free(p_units_sequences_arraylast_seq_index);
		return ERROR_UNITSELECTION_MEMORY_ALLOCATION;
	}

	int JK = 0;
	int K = 0;
	int KK = 0;
	int DK = 0;


	int seq_at_start;
	bool b_unit_found;

	int best_non_seq__unit_seq_cost;
	int best_non_seq__unit_cost;
	int best_non_seq__unit_num;
	int best_non_seq__seq_num;

	int sel_seq;

	int z;
	int unit_segment_length;

	int i, x, m;

	int new_seq_cost;
	int seq_num_tmp;

	int n_same_seq;

	int newBestCost;

	int totalWordsNum = 0;
	for (int b = 0; b < unitsRowLength; b++)
	{
		if (unitsRowNextSeparators[b] == '+')
			totalWordsNum++;
	}
	totalWordsNum -= 2;

	int currentWordNum = 0;
	int totalSylsNum, currentSylNum;
	int s;

	for (i = 1; i < (unitsRowLength - 1); i++)
	{
		seq_at_start = n_total_units_seq;
		b_unit_found = false;

		JK = jungimo_kainos_precalculated[unitsRow[i - 1]][unitsRow[i]];
		if (unitsRowNextSeparators[i - 1] == '+') JK = int(float(JK)*JK_koef1);
		else if (unitsRowNextSeparators[i - 1] == '-') JK = int(float(JK)*JK_koef2);

		JK = int(JK * float(jungimoKainuSvoris / 100.0f));

		if (unitsRowNextSeparators[i - 1] == '+')
		{
			currentWordNum++;

			currentSylNum = 1;

			totalSylsNum = 1;
			for (int b = i; b < unitsRowLength; b++)
			{
				if (unitsRowNextSeparators[b] == '+')
					break;

				if (unitsRowNextSeparators[b] == '-')
					totalSylsNum++;
			}
		}

		if (unitsRowNextSeparators[i - 1] == '-')
		{
			currentSylNum++;
		}

		int E1 = 0;
		int E2 = 0;

		if (totalWordsNum == 1)
			E1 = 5;
		else if (currentWordNum == 1)
			E1 = 1;
		else if (currentWordNum == totalWordsNum)
			E1 = 4;
		else
			E1 = 2;

		if (totalSylsNum == 1)
			E2 = 5;
		else if (currentSylNum == 1)
			E2 = 1;
		else if (currentSylNum == totalSylsNum)
			E2 = 4;
		else
			E2 = 2;

		best_non_seq__unit_seq_cost = 999999;
		best_non_seq__unit_cost = 0;
		best_non_seq__unit_num = -1;
		best_non_seq__seq_num = -1;

		int tmp_sel_seq = -1;
		int smallest_cost = 999999;
		for (s = seq_index; s < n_total_units_seq; s++)
		{
			if (p_units_sequences_arraycost[s] <= smallest_cost)
			{
				smallest_cost = p_units_sequences_arraycost[s];
				tmp_sel_seq = s;
			}
		}
		sel_seq = tmp_sel_seq;

		newBestCost = 999999;

		z = unitsRow[i];
		unit_segment_length = unit_segments_lengths[z];

		for (x = 0; x < unit_segment_length; x++)
		{
			m = unit_segments[z][x];
			b_unit_found = true;

			////////////////////////
			// Kainos skaiciavimas
			KK = keitimo_kaina_KK_precalculated[unitsRow[i]][unitsRow[i - 1]][unitsDB_ID[m - 1]];
			DK = keitimo_kaina_DK_precalculated[unitsRow[i]][unitsRow[i + 1]][unitsDB_ID[m + 1]];
			K = DK + KK;

			K = int(K * float(keitimoKainuSvoris / 100.0f));

			K += int(unitsWeights[m] * float(unitsWeightsKoef / 100.0f));
			if ((unitsLengths[m] > 0) && (unitsRowDurr[i] > 0)) K += int(abs(unitsLengths[m] - unitsRowDurr[i]*22.05f)*float(unitsLengthsKoef / 100.0f));

			if ((unitsDB_E1[m] & E1) == false) K += E1_koef;
			if ((unitsDB_E2[m] & E2) == false) K += E2_koef;
			////////////////////////

			if (unitsRow[i + 1] == unitsDB_ID[m + 1])
			{
				if (i == 1)
				{
					p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = m; //first_unit_num_in_DB;
					p_units_sequences_arraylast_seq_index[n_total_units_seq] = -1;
					p_units_sequences_arraycost[n_total_units_seq] = K; //cost;
					n_total_units_seq++;

					if (K < newBestCost) newBestCost = K;
				}
				else
				{
					if (m <= 0)
						n_same_seq = -1;
					else
					{
						for (s = seq_index; s < n_total_units_seq; s++)
						if (p_units_sequences_arrayunit_num_in_DB[s] == (m - 1))
						{
							n_same_seq = s; break;
						}

						if (s == n_total_units_seq) n_same_seq = -1;
					}

					//Add to best
					if (sel_seq >= 0)
					{
						if (n_same_seq != -1)
						{
							if ((p_units_sequences_arraycost[sel_seq] + JK) < p_units_sequences_arraycost[n_same_seq])
							{
								p_units_sequences_arraylast_seq_index[n_total_units_seq] = sel_seq; //old_seq;
								p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = m; //new_unit_num;
								p_units_sequences_arraycost[n_total_units_seq] = p_units_sequences_arraycost[sel_seq] + (K + JK); //cost_to_add;
								n_total_units_seq++;
								if ((p_units_sequences_arraycost[sel_seq] + JK + K) < newBestCost) newBestCost = (p_units_sequences_arraycost[sel_seq] + JK + K);
							}
							else
							{
								p_units_sequences_arraylast_seq_index[n_total_units_seq] = n_same_seq; //old_seq;
								p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = m; //new_unit_num;
								p_units_sequences_arraycost[n_total_units_seq] = p_units_sequences_arraycost[n_same_seq] + K; //cost_to_add;
								n_total_units_seq++;
								if ((p_units_sequences_arraycost[n_same_seq] + K) < newBestCost) newBestCost = (p_units_sequences_arraycost[n_same_seq] + K);
							}
						}
						else
						{
							p_units_sequences_arraylast_seq_index[n_total_units_seq] = sel_seq; //old_seq;
							p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = m; //new_unit_num;
							p_units_sequences_arraycost[n_total_units_seq] = p_units_sequences_arraycost[sel_seq] + (K + JK); //cost_to_add;
							n_total_units_seq++;
							if ((p_units_sequences_arraycost[sel_seq] + JK + K) < newBestCost) newBestCost = (p_units_sequences_arraycost[sel_seq] + JK + K);
						}
					}
					else
					{
						return ERROR_UNITSELECTION_UNEXPECTED_1;
					}
				}
			}
			else
			{
				if ((K < newBestCost) && (K < best_non_seq__unit_seq_cost))
				{
					new_seq_cost = K;
					seq_num_tmp = -1;

					if (i == 1)
					{
					}
					else
					{
						if (m <= 0)
							n_same_seq = -1;
						else
						{
							for (s = seq_index; s < n_total_units_seq; s++)
							if (p_units_sequences_arrayunit_num_in_DB[s] == (m - 1))
							{
								n_same_seq = s; break;
							}

							if (s == n_total_units_seq) n_same_seq = -1;
						}

						//Add to best
						if (sel_seq >= 0)
						{
							if (n_same_seq != -1)
							{
								if ((p_units_sequences_arraycost[sel_seq] + JK) < p_units_sequences_arraycost[n_same_seq])
								{
									new_seq_cost += p_units_sequences_arraycost[sel_seq] + JK;
									seq_num_tmp = sel_seq;
								}
								else
								{
									new_seq_cost += p_units_sequences_arraycost[n_same_seq];
									seq_num_tmp = n_same_seq;
								}
							}
							else
							{
								new_seq_cost += p_units_sequences_arraycost[sel_seq] + JK;
								seq_num_tmp = sel_seq;

							}
						}
						else
							return ERROR_UNITSELECTION_UNEXPECTED_1;
					}



					if (new_seq_cost < best_non_seq__unit_seq_cost)
					{
						best_non_seq__unit_seq_cost = new_seq_cost;
						best_non_seq__unit_cost = K;
						best_non_seq__unit_num = m;
						best_non_seq__seq_num = seq_num_tmp;
					}
				}
			}
		}


		if (b_unit_found == false)
		{
			free(p_units_sequences_arrayunit_num_in_DB);
			free(p_units_sequences_arraylast_seq_index);
			free(p_units_sequences_arraycost);

			return ERROR_UNITSELECTION_UNIT_NOT_FOUND;
		}

		if (best_non_seq__unit_num != -1)
		{
			if (best_non_seq__seq_num >= 0)
			{
				if (p_units_sequences_arrayunit_num_in_DB[best_non_seq__seq_num] == (best_non_seq__unit_num - 1))
				{
					p_units_sequences_arraylast_seq_index[n_total_units_seq] = best_non_seq__seq_num; //old_seq;
					p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = best_non_seq__unit_num; //new_unit_num;
					p_units_sequences_arraycost[n_total_units_seq] = p_units_sequences_arraycost[best_non_seq__seq_num] + best_non_seq__unit_cost; //cost_to_add;
					n_total_units_seq++;
				}
				else
				{
					p_units_sequences_arraylast_seq_index[n_total_units_seq] = best_non_seq__seq_num; //old_seq;
					p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = best_non_seq__unit_num; //new_unit_num;
					p_units_sequences_arraycost[n_total_units_seq] = p_units_sequences_arraycost[best_non_seq__seq_num] + (best_non_seq__unit_cost + JK); //cost_to_add;
					n_total_units_seq++;
				}
			}
			else
			{
				if (i == 1)
				{
					p_units_sequences_arrayunit_num_in_DB[n_total_units_seq] = best_non_seq__unit_num; //first_unit_num_in_DB;
					p_units_sequences_arraylast_seq_index[n_total_units_seq] = -1;
					p_units_sequences_arraycost[n_total_units_seq] = best_non_seq__unit_cost; //cost;
					n_total_units_seq++;
				}
				else
					return ERROR_UNITSELECTION_UNEXPECTED_2;
			}
		}

		seq_index = seq_at_start;
	}

	/////////////////////////////////////////////////////////////////
	//Results
	int tmp_sel_seq = -1;
	int smallest_cost = 999999;
	for (s = seq_index; s < n_total_units_seq; s++)
	{
		if (p_units_sequences_arraycost[s] <= smallest_cost)
		{
			smallest_cost = p_units_sequences_arraycost[s];
			tmp_sel_seq = s;
		}
	}

	int best_seq = tmp_sel_seq;

	if (best_seq == -1)
		return ERROR_UNITSELECTION_UNEXPECTED_3;

	int last_index = best_seq;
	int bs = unitsRowLength - 2;
	while (last_index != -1)
	{
		if (bs < 0)
		{
			free(p_units_sequences_arrayunit_num_in_DB);
			free(p_units_sequences_arraylast_seq_index);
			free(p_units_sequences_arraycost);

			return ERROR_UNITSELECTION_UNEXPECTED_4;
		}

		retUnits[bs] = p_units_sequences_arrayunit_num_in_DB[last_index];
		last_index = p_units_sequences_arraylast_seq_index[last_index];
		bs--;
	}


	if (unitsDB_ID[retUnits[1] - 1] == 0)
		retUnits[0] = retUnits[1] - 1;
	else
		retUnits[0] = 0;

	if (unitsDB_ID[retUnits[unitsRowLength - 2] + 1] == 0)
		retUnits[unitsRowLength - 1] = retUnits[unitsRowLength - 2] + 1;
	else
		retUnits[unitsRowLength - 1] = n_total_DBunits - 1;

	memcpy(retCost, &p_units_sequences_arraycost[best_seq], sizeof(int));

	free(p_units_sequences_arrayunit_num_in_DB);
	free(p_units_sequences_arraylast_seq_index);
	free(p_units_sequences_arraycost);

	return NO_ERR;
}
