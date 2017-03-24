///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSStest.exe
// Failas getLUSSErrorMessages.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LithUSS_Error.h"

char temp[200];

char* getLUSSErrorMessages(int err)
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
			return "Klaida nuskaitant duomenis ið failo transcr_rules.txt. Taisykli¸ skai?ius neatitinka nurodyt? failo prad?ioje.";

//UnitSelection klaidu kodai
		case ERROR_UNITSELECTION_OPENING_SETTINGS_FILE:
			return "Nepavyko atidaryti failo UnitSelectionSettings.txt.";
		case ERROR_UNITSELECTION_READING_SETTINGS_FILE:
			return "Klaida skaitant failà UnitSelectionSettings.txt.";
		case ERROR_UNITSELECTION_OPENING_JOINING_COSTS_FILE:
			return "Nepavyko atidaryti failo jungimo_kainos.txt.";
		case ERROR_UNITSELECTION_READING_JOINING_COSTS_FILE:
			return "Klaida skaitant failà jungimo_kainos.txt.";
		case ERROR_UNITSELECTION_OPENING_LEFT_SUBSTITUTION_COSTS_FILE:
			return "Nepavyko atidaryti failo keitimo_kainos_KK.txt.";
		case ERROR_UNITSELECTION_READING_LEFT_SUBSTITUTION_COSTS_FILE:
			return "Klaida skaitant failà keitimo_kainos_KK.txt.txt.";
		case ERROR_UNITSELECTION_OPENING_RIGHT_SUBSTITUTION_COSTS_FILE:
			return "Nepavyko atidaryti failo keitimo_kainos_DK.txt.";
		case ERROR_UNITSELECTION_READING_RIGHT_SUBSTITUTION_COSTS_FILE:
			return "Klaida skaitant failà keitimo_kainos_KK.txt.txt.";
		case ERROR_UNITSELECTION_OPENING_DB_FON_FILE:
			return "Nepavyko atidaryti failo db_fon.txt.";
		case ERROR_UNITSELECTION_DB_FON_EXCEEDED_MAX_UNITS:
			return "Fonemø skaièius faile db_fon.txt virðijo maksimalø leistinà kieká.";
		case ERROR_UNITSELECTION_READING_DB_FON_FILE:
			return "Klaida skaitant failà db_fon.txt.";
		case ERROR_UNITSELECTION_OPENING_DB_FON_WEIGHTS_FILE:
			return "Nepavyko atidaryti failo db_fon_weights.txt.";
		case ERROR_UNITSELECTION_READING_DB_FON_WEIGHTS_FILE:
			return "Klaida skaitant failà db_fon_weights.txt.";

//RateChange klaidu kodai
		case ERROR_RATECHANGE_OPENING_DB_FILE:
			return "Nepavyko atidaryti signalo failo db.raw skaitymui.";
		case ERROR_RATECHANGE_DETECTING_DB_FILE_SIZE:
			return "Nepavyko nustatyti signalo failo db.raw dydþio.";
		case ERROR_RATECHANGE_UNEVEN_DB_FILE_SIZE:
			return "Signalo failo db.raw dydis turi bûti lyginis skaièius.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_DB:
			return "Nepavyko iðskirti atminties garsø bazei db.raw.";
		case ERROR_RATECHANGE_READING_DB_FILE:
			return "Klaida skaitant signalo failà db.raw.";
		case ERROR_RATECHANGE_UNFINISHED_READING_DB_FILE:
			return "Nepavyko nuskaityti signalo failo db.raw iki galo.";
		case ERROR_RATECHANGE_OPENING_DB_FON_WEIGHTS_FILE:
			return "Nepavyko atidaryti failo db_fon_weights.txt.";
		case ERROR_RATECHANGE_DETECTING_DB_FON_WEIGHTS_FILE_SIZE:
			return "Nepavyko nustatyti failo db_fon_weights.txt dydþio.";
		case ERROR_RATECHANGE_READING_DB_FON_WEIGHTS_FILE:
			return "Klaida skaitant failà db_fon_weights.txt.";
		case ERROR_RATECHANGE_UNFINISHED_READING_DB_FON_WEIGHTS_FILE:
			return "Nepavyko nuskaityti failo db_fon_weights.txt iki galo.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST:
			return "Nepavyko iðskirti atminties fonemø arba fonemø ilgiø sàraðui.";
		case ERROR_RATECHANGE_UNSPECIFIED_PHONEME_LENGTH:
			return "Klaida faile db_fon_weights.txt. Fonemai nëra nurodytas jos ilgis.";
		case ERROR_RATECHANGE_NONNUMERICAL_PHONEME_LENGTH:
			return "Klaida faile db_fon_weights.txt: Nepavyko eilutës konvertuoti á fonemos ilgá.";
		case ERROR_RATECHANGE_OPENING_DB_PIK_FILE:
			return "Nepavyko atidaryti failo db_pik.txt.";
		case ERROR_RATECHANGE_DETECTING_DB_PIK_FILE_SIZE:
			return "Nepavyko nustatyti failo db_pik.txt dydþio.";
		case ERROR_RATECHANGE_READING_DB_PIK_FILE:
			return "Klaida skaitant failà db_pik.txt.";
		case ERROR_RATECHANGE_UNFINISHED_READING_DB_PIK_FILE:
			return "Nepavyko nuskaityti failo db_pik.txt iki galo.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_LIST:
			return "Nepavyko iðskirti atminties pikø sàraðui.";
		case ERROR_RATECHANGE_NONNUMERICAL_PIK_VALUE:
			return "Klaida faile db_pik.txt. Nepavyko eilutës konvertuoti á skaièiø.";
		case ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_INFO:
			return "Nepavyko iðskirti atminties pagalbinei pikø informacijai.";

//LithUSS klaidu kodai
		case ERROR_LITHUSS_LOADING_TEXTNORMALIZATION_DLL:
			return "Nepavyko pakrauti TextNormalization.dll.";
		case ERROR_LITHUSS_LOADING_TRANSCRLUSS_DLL:
			return "Nepavyko pakrauti transcrLUSS.dll.";
		case ERROR_LITHUSS_OPENING_FACTORS_FILE:
			return "Nepavyko atidaryti failo faktoriai.txt.";
		case ERROR_LITHUSS_READING_FACTORS_FILE:
			return "Klaida skaitant failà faktoriai.txt.";
		case ERROR_LITHUSS_LOADING_UNITSELECTION_DLL:
			return "Nepavyko pakrauti UnitSelection.dll.";
		case ERROR_LITHUSS_LOADING_RATECHANGE_DLL:
			return "Nepavyko pakrauti RateChange.dll.";
		
		default: sprintf(temp, "Kitas kodas %d", err); return temp;
	}
}