///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas LIEPA.exe
// Failas getLUSSErrorMessages.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../include/LithUSS_Error.h"

#include <stdio.h>

char temp[200];

EXPORT char* getLUSSErrorMessages(int err)
{
	switch (err)
	{
//TextNormalization klaidu kodai
		case ERROR_TEXTNORMALIZATION_OPENING_RULES_FILE :
			return "Nepavyko atidaryti failo rules.txt.";
		case ERROR_TEXTNORMALIZATION_OPENING_FILE_SPECIFIED_IN_RULES_FILE :
			return "Nepavyko atidaryti failo, nurodyto faile rules.txt.";

//TranscrLUSS klaidu kodai
		case ERROR_TRANSCRLUSS_OPENING_TRANSCR_RULES_FILE :
			return "Nepavyko atidaryti failo transcr_rules.txt.";
		case ERROR_TRANSCRLUSS_READING_TRANSCR_RULES_FILE :
			return "Klaida nuskaitant duomenis i\xF0 failo transcr_rules.txt. Taisykli\xF8 skai\xE7ius neatitinka nurodyt\xF8 failo prad\xFEioje.";

//UnitSelection klaidu kodai
		case ERROR_UNITSELECTION_OPENING_SETTINGS_FILE:
			return "Nepavyko atidaryti failo UnitSelectionSettings.txt.";
		case ERROR_UNITSELECTION_READING_SETTINGS_FILE:
			return "Klaida skaitant fail\xE0 UnitSelectionSettings.txt.";
		case ERROR_UNITSELECTION_OPENING_JOINING_COSTS_FILE:
			return "Nepavyko atidaryti failo jungimo_kainos.txt.";
		case ERROR_UNITSELECTION_READING_JOINING_COSTS_FILE:
			return "Klaida skaitant fail\xE0 jungimo_kainos.txt.";
		case ERROR_UNITSELECTION_OPENING_LEFT_SUBSTITUTION_COSTS_FILE:
			return "Nepavyko atidaryti failo keitimo_kainos_KK.txt.";
		case ERROR_UNITSELECTION_READING_LEFT_SUBSTITUTION_COSTS_FILE:
			return "Klaida skaitant fail\xE0 keitimo_kainos_KK.txt.txt.";
		case ERROR_UNITSELECTION_OPENING_RIGHT_SUBSTITUTION_COSTS_FILE:
			return "Nepavyko atidaryti failo keitimo_kainos_DK.txt.";
		case ERROR_UNITSELECTION_READING_RIGHT_SUBSTITUTION_COSTS_FILE:
			return "Klaida skaitant fail\xE0 keitimo_kainos_KK.txt.txt.";
		case ERROR_UNITSELECTION_OPENING_DB_FON_FILE:
			return "Nepavyko atidaryti failo db_fon.txt.";
		case ERROR_UNITSELECTION_DB_FON_EXCEEDED_MAX_UNITS:
			return "Fonem\xF8 skai\xE8ius faile db_fon.txt vir\xF0ijo maksimal\xF8 leistin\xE0 kiek\xE1.";
		case ERROR_UNITSELECTION_READING_DB_FON_FILE:
			return "Klaida skaitant fail\xE0 db_fon.txt.";
		case ERROR_UNITSELECTION_OPENING_DB_FON_WEIGHTS_FILE:
			return "Nepavyko atidaryti failo db_fon_weights.txt.";
		case ERROR_UNITSELECTION_READING_DB_FON_WEIGHTS_FILE:
			return "Klaida skaitant fail\xE0 db_fon_weights.txt.";

//RateChange klaidu kodai
		case ERROR_RATECHANGE_OPENING_DB_FILE:
			return "Nepavyko atidaryti signalo failo db.raw skaitymui.";
		case ERROR_RATECHANGE_DETECTING_DB_FILE_SIZE:
			return "Nepavyko nustatyti signalo failo db.raw dyd\xFEio.";
		case ERROR_RATECHANGE_UNEVEN_DB_FILE_SIZE:
			return "Signalo failo db.raw dydis turi b\xFBti lyginis skai\xE8ius.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_DB:
			return "Nepavyko i\xF0skirti atminties gars\xF8 bazei db.raw.";
		case ERROR_RATECHANGE_READING_DB_FILE:
			return "Klaida skaitant signalo fail\xE0 db.raw.";
		case ERROR_RATECHANGE_UNFINISHED_READING_DB_FILE:
			return "Nepavyko nuskaityti signalo failo db.raw iki galo.";
		case ERROR_RATECHANGE_OPENING_DB_FON_WEIGHTS_FILE:
			return "Nepavyko atidaryti failo db_fon_weights.txt.";
		case ERROR_RATECHANGE_DETECTING_DB_FON_WEIGHTS_FILE_SIZE:
			return "Nepavyko nustatyti failo db_fon_weights.txt dyd\xFEio.";
		case ERROR_RATECHANGE_READING_DB_FON_WEIGHTS_FILE:
			return "Klaida skaitant fail\xE0 db_fon_weights.txt.";
		case ERROR_RATECHANGE_UNFINISHED_READING_DB_FON_WEIGHTS_FILE:
			return "Nepavyko nuskaityti failo db_fon_weights.txt iki galo.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST:
			return "Nepavyko i\xF0skirti atminties fonem\xF8 arba fonem\xF8 ilgi\xF8 s\xE0ra\xF0ui.";
		case ERROR_RATECHANGE_UNSPECIFIED_PHONEME_LENGTH:
			return "Klaida faile db_fon_weights.txt. Fonemai n\xEBra nurodytas jos ilgis.";
		case ERROR_RATECHANGE_NONNUMERICAL_PHONEME_LENGTH:
			return "Klaida faile db_fon_weights.txt: Nepavyko eilut\xEBs konvertuoti \xE1 fonemos ilg\xE1.";
		case ERROR_RATECHANGE_OPENING_DB_PIK_FILE:
			return "Nepavyko atidaryti failo db_pik.txt.";
		case ERROR_RATECHANGE_DETECTING_DB_PIK_FILE_SIZE:
			return "Nepavyko nustatyti failo db_pik.txt dyd\xFEio.";
		case ERROR_RATECHANGE_READING_DB_PIK_FILE:
			return "Klaida skaitant fail\xE0 db_pik.txt.";
		case ERROR_RATECHANGE_UNFINISHED_READING_DB_PIK_FILE:
			return "Nepavyko nuskaityti failo db_pik.txt iki galo.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_LIST:
			return "Nepavyko i\xF0skirti atminties pik\xF8 s\xE0ra\xF0ui.";
		case ERROR_RATECHANGE_NONNUMERICAL_PIK_VALUE:
			return "Klaida faile db_pik.txt. Nepavyko eilut\xEBs konvertuoti \xE1 skai\xE8i\xF8.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_INFO:
			return "Nepavyko i\xF0skirti atminties pagalbinei pik\xF8 informacijai.";

//LithUSS klaidu kodai
		case ERROR_LITHUSS_LOADING_TEXTNORMALIZATION_DLL:
			return "Nepavyko pakrauti TextNormalization.dll.";
		case ERROR_LITHUSS_LOADING_TRANSCRLUSS_DLL:
			return "Nepavyko pakrauti transcrLUSS.dll.";
		case ERROR_LITHUSS_OPENING_FACTORS_FILE:
			return "Nepavyko atidaryti failo faktoriai.txt.";
		case ERROR_LITHUSS_READING_FACTORS_FILE:
			return "Klaida skaitant fail\xE0 faktoriai.txt.";
		case ERROR_LITHUSS_LOADING_UNITSELECTION_DLL:
			return "Nepavyko pakrauti UnitSelection.dll.";
		case ERROR_LITHUSS_LOADING_RATECHANGE_DLL:
			return "Nepavyko pakrauti RateChange.dll.";
		
		default: sprintf(temp, "Kitas kodas %d", err); return temp;
	}
}