/*******************************************************************************
GLEAM/AE                                                                  V1.1.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: AUFGabenstellung
                        Module: AUFGabe-C-MENue
 
Package: aufg                   File: aufgcmen.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul realisiert das Aufgabensubmenue in der TEXT_MODE-Variante. Es besteht 
aus einem Standard- und einem applikationsspezifischen Teil.

Die Routinen im einzelnen:

BOOL aufg_sub_men (void);
          Die Routine realisiert das Aufgabensubmenue. Sie liefert TRUE, wenn
          an der Aufgabenstellung (wahrscheinlich) etwas geaendert wurde.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. "erg_genauigkeit" wird bei Aenderung der Aufgabendaten unab-
    haengig von der Applikation uebernommen.
 2. Ueberarbeitung der Menuesteuerung.                               WJ 22.12.98
 3. Einbinden der Hindernisverwaltung                                CB 23.04.99
 4. Korrektur Display Menue                                          CB 24.04.99
 5. Includierung von "rob_int" entfernt.                             WJ 21.06.99
 6. Anpassungen an neue TSK-Datenhaltung.                            WJ 25.04.02
 7. Optionale Rotation der KoordAchsen gesteuert durch TSK-Daten.    WJ 24.04.03
 8. "get_id_idx"-Aufrufe derart, dass Fehler gemeldet werden.        WJ 27.11.03
 9. Erweiterung fuer die MatLab-Koppung: GAMA komplett gestrichen.   WJ 22.08.07
10. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).      WJ 26.10.07
11. Entfernung ueberfluessiger Variable.                             WJ 26.06.12
12. Renovierung: Streichung des "link"-Includes und von ueberfluessigen 
    Aufrufen aus dem "ctio"-Package. Anpassung der Aufrufe von
    "write_text_buf", "set_and_clear_menue_window", "dialog", 
    "char_dialog", "lies_..." und "select_menue_item".               WJ 13.05.16
13. Anpassungen an die neue TSK-Datenverwaltung. Erweiterung des
    Menues um "Alle Param". Warnhinweis bei Werteaenderungen.        WJ 23.12.16
14. Ersetzung von "sim_mod_art_name" durch "extSimSubDir".           WJ 10.03.17
15. Programmparameteraenderung bei aktivem OptJob nicht mehr moeg-
    lich. Neuer Parameter fuer "show_and_clear_fbuf".                WJ 10.11.17
16. Ausgabe des Textpuffers bei Beendigung des Menues und 
    geaenderten Daten.                                               WJ 28.11.17
17. aufg_sub_men(): Avoiding buffer overflow of static strings.      WJ 18.07.20
--------------------------------------------------------------------------------
Status: o.k.                 Autoren: W.Jakob / C. Blume       Datum: 18.02.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "simu.h"
#include "evo.h"
#include "aufg.h"
#include "aufgInt.h"

#include AUFG_TEXTS
#include GLOBAL_TEXTS


#ifdef GLO_CHECKS
#define CHECKS
#endif




/* ============================== aufg_sub_men ============================== */
BOOL aufg_sub_men (void)
{
   char         bigBuf [2*MAX_STRING];
   char         buf3   [MAX_STR+MAX_BUF];
   STRING_TYPE  modDirBuf;
   STR_TYPE     buf, buf1, buf2,
                parName;
   DOUBLE       rwert,
                doubleVal,
                lowerRealLim, upperRealLim;
   INT          men_item,
                intVal,
                lowerIntLim, upperIntLim,
                dataType,
                parIdx,
                mod_anz, idx, ii;
   const char **enumNames;
   char         ch;
   BOOL         noVal;
   BOOL         data_aend   = FALSE,
                sub_men_fin = FALSE;
   
   /* aufg_sub_men */
   sprintf (buf, PARAM_ANZ_TXT, PARAM_LIST_LENGTH - FIRST_TSK_DATA_IDX);
   printf ("%s\n", buf);
   do
   { /* Menue-Loop */
      set_and_clear_menue_window (2, AUFG_MEN_TXT);
      write_menue_item (1, StndMenItem, AUFG_MEN1);             /* ZeigParam  */
      write_menue_item (2, StndMenItem, AUFG_MEN2);             /* AendParam  */
      write_menue_item (3, StndMenItem, AUFG_MEN3);             /* Alle Param */
      if (appl_code == GADO_APPL) {
	 write_menue_item (5, StndMenItem, AUFG_MEN5_SIMOT);    /* ZeigModell */
	 write_menue_item (6, StndMenItem, AUFG_MEN6_SIMOT);    /* AendModell */
      }
      if (appl_code == LESAK_APPL) {
	 write_menue_item (5, StndMenItem, AUFG_MEN5_LESAK);    /* ZeigStartZ */
	 write_menue_item (6, StndMenItem, AUFG_MEN6_LESAK);    /* Teach-in   */
	 write_menue_item (8, StndMenItem, AUFG_MEN8_LESAK);    /* Hindernis  */
      }
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item)
      {
         case 1: /* ---------------------------- ZeigParam ---------------------------- */
            prepareTskDataDisplay(WITHOUT_INDICES, SHOW_EDITABLE_PARAMS);
            printf ("%s\n\n", AUFG_PAR_TITLE);
            printf ("%s\n", AUFG_PAR_HDR);
            write_text_buf (CLEAR);
         break; /* ZeigParam */
            
         case 2: /* ---------------------------- AendParam ---------------------------- */
            printf("%s\n\n", AUFG_PAR_XTITLE);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);          
	       break;                                                        /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
            prepareTskDataDisplay(WITH_INDICES, SHOW_EDITABLE_PARAMS);
            printf("%s\n", AUFG_PAR_XHDR);
            write_text_buf(CLEAR);
            sprintf(buf, AUFG_PIDX_QUERY, 1, paramListActEntryAmount);
            if (lies_wert(&idx, buf, 1, paramListActEntryAmount, PARAM_LIST_LENGTH))
               break;                                                /* aborted by user */
	    parIdx = paramListActEntries[idx - 1];
	    if (getParamData(parName, &dataType, &enumNames, parIdx))  /* errMsg issued by function */
	    { /* parameter exists and data obtained */
	       switch (dataType) 
	       {
	          case BOOL_DATA:
		     sprintf (bigBuf, "%s %s", parName, NEIN_JA_TXT);  /* default if not defined */
		     if (getBoolVal(&intVal, parIdx, GLEAM_OK))
		     { /* value obtained */
			noVal = FALSE;
			if (intVal)
			   sprintf (bigBuf, "%s %s", parName, JA_NEIN_TXT);
		     } /* value obtained */
		     else
			noVal = TRUE;
		     ch = char_dialog (bigBuf);
		     if (ch != ESC)
                     { /* Kein Abbruch */
			if (ch == JA)
			   setBoolVal(TRUE, parIdx);
			else if (ch == NEIN)
			   setBoolVal(FALSE, parIdx);
			else if (ch == CR) {
			   if (noVal)
			      setBoolVal(FALSE, parIdx);
			}
			else
			   printf (FMELD_LINE_TXT, BAD_KEY);
			data_aend = TRUE;
		     } /* Kein Abbruch */
		  break; /* BOOL_DATA */

	          case INT_DATA:
		     getIntLimits(&lowerIntLim, &upperIntLim, parIdx);
		     sprintf(bigBuf, "%s (%d .. %d)", parName, lowerIntLim, upperIntLim);
		     if (getIntVal(&intVal, parIdx, GLEAM_OK)) 
			sprintf(buf1, " [%d]: ", intVal);
		     else { /* no value obtained */
			sprintf(buf1, ": ");
			intVal = upperIntLim + 1;              /* no value, no default! */
		     } /* no value obtained */
		     strcat(bigBuf, buf1);
		     if (lies_wert (&ii, bigBuf, lowerIntLim, upperIntLim, intVal))
			break;                                       /* aborted by user */
		     setIntVal(ii, parIdx);
		     data_aend = TRUE;
		  break; /* INT_DATA */

	          case ENUM_DATA:
		     getIntLimits(&lowerIntLim, &upperIntLim, parIdx);
		     sprintf(bigBuf, "%s (", parName);
		     for (ii = lowerIntLim;  ii <= upperIntLim; ii++) {
			strcat (bigBuf, enumNames[ii]);
			if (ii < upperIntLim)
			   strcat(bigBuf, ", ");
		     }
		     if (getEnumText(buf2, parIdx, GLEAM_OK))
			sprintf(buf3, ") [%s]: ", buf2);
		     else
			strcpy(buf3, "): ");
		     strcat (bigBuf, buf3);
		     strcpy(buf1, "");
		     if (dialog(bigBuf, buf1))
			break;                                       /* aborted by user */
		     if (strlen(buf1) > 0)
			data_aend = setEnumVal(buf1, parIdx);    /* some input obtained */
		  break; /* ENUM_DATA */

	          case DOUBLE_DATA:
		     getDoubleLimits(&lowerRealLim, &upperRealLim, parIdx);
		     sprintf(bigBuf, "%s (%g .. %g)", parName, lowerRealLim, upperRealLim);
		     if (getDoubleVal(&doubleVal, parIdx, GLEAM_OK)) 
			sprintf(buf1, " [%g]: ", doubleVal);
		     else { /* no value obtained */
			sprintf(buf1, ": ");
			doubleVal = upperRealLim + 1;           /*no value, no default! */
		     } /* no value obtained */
		     strcat(bigBuf, buf1);
		     if (lies_rwert (&rwert, bigBuf, lowerRealLim, upperRealLim, doubleVal))
			break;                                       /* aborted by user */
		     setDoubleVal(rwert, parIdx);
		     data_aend = TRUE;
		  break; /* DOUBLE_DATA */

	          case STRING_DATA:
		     sprintf(bigBuf, "%s: ", parName);
		     if (dialog (bigBuf, buf1))
			break;                                       /* aborted by user */
		     data_aend = setStringVal(buf1, parIdx);
		  break; /* STRING_DATA */
	       } /* switch */
	    } /* parameter exists and data obtained */

            prepareTskDataDisplay(WITH_INDICES, SHOW_EDITABLE_PARAMS);
            printf("%s\n\n", AUFG_PAR_XTITLE);
            printf("%s\n", AUFG_PAR_XHDR);
            write_text_buf(CLEAR);
         break; /* AendParam */
            
         case 3: /* ----------------------- Alle Param ----------------------- */
            prepareTskDataDisplay(WITHOUT_INDICES, SHOW_ALL_PARAMS);
            printf ("%s\n\n", AUFG_ALL_PAR_TITLE);
            printf ("%s\n", AUFG_PAR_HDR);
            write_text_buf (CLEAR);
         break; /* Alle Param */
            
         case 5: /* ---------- Applikationsspezifische Zeigefunktion -------- */
            switch (appl_code)
            {
               case GADO_APPL: /* ------------------ SIMOT ------------------ */
                  printf ("\n%s\n", AKT_MOD_HDR_TXT);
		  if (getStringVal(modDirBuf, SIMU_MOD_NAME_IDX, GLEAM_OK))
                  { /* SIMU_MOD_NAME_IDX-Eintrag hat einen Wert */
                     sprintf (bigBuf, "%s%s%s%s%s", sim_root_dir_name, 
                              DIR_DELIMITER, extSimSubDir, DIR_DELIMITER, modDirBuf);
                     if (check_dir (bigBuf))
                        sprintf (bigBuf, AKT_MODELL_TXT, modDirBuf, SIM_MOD_OK_TXT);
                     else
                        sprintf (bigBuf, AKT_MODELL_TXT, modDirBuf, NO_SIM_MOD_TXT);
                     printf ("%s\n", bigBuf);
                  } /* SIMU_MOD_NAME_IDX-Eintrag hat einen Wert */
		  else
		     fehler(AUFG_CMEN, 1, NO_INFO, NO_INFO, SIMU_MOD_NAME_TXT, 
			    PARAM_HAS_NO_VALUE);
               break; /* SIMOT */
                  
               case LESAK_APPL: /* ----------------- LESAK ------------------ */
                  write_buf (2, START_ZIEL_HDR);
                  zeige_start_ziel (4, TRUE);
                  write_text_buf (CLEAR);
               break; /* LESAK */
            } /* switch (appl_code) */
         break; /* Applikationsspezifische Zeigefunktion */

         case 6: /* --------- Applikationsspezifische Update-Funktion ------- */
            switch (appl_code)
            {
               case GADO_APPL: /* ------------------ SIMOT ------------------ */
                  printf ("%s\n", NEU_MOD_HDR_TXT);
		  if (getStringVal(modDirBuf, SIMU_MOD_NAME_IDX, GLEAM_ERROR))
                  { /* SIMU_MOD_NAME_IDX-Eintrag hat einen Wert */
                     sprintf (buf1, "%s%s%s", sim_root_dir_name, DIR_DELIMITER,
                              extSimSubDir);
                     sprintf (bigBuf, "%s%s%s", buf1, DIR_DELIMITER, modDirBuf);
                     if (check_dir (bigBuf))
                        sprintf (bigBuf, AKT_MODELL_TXT, modDirBuf, SIM_MOD_OK_TXT);
                     else
                        sprintf (bigBuf, AKT_MODELL_TXT, modDirBuf, NO_SIM_MOD_TXT);
                     printf ("%s\n", bigBuf);
                     if ((mod_anz = show_sim_mods (buf1, 1, TRUE)) > 0)
                     { /* Directory in "bigBuf" existiert u.es gibt darin Dirs */
                        write_text_buf (KEEP);               /* SimuModAusg */
                        sprintf (buf2, MOD_INDEX_QUERY, mod_anz);
                        if (lies_wert (&ii, buf2, 1, mod_anz, mod_anz + 1))
                        { /* Abbruch durch Benutzer */
                           reset_text_buf ();
                           printf ("%s\n", NIX_NEU_MOD_TXT);
                        } /* Abbruch durch Benutzer */
                        else
                        { /* Index auswerten */
                           get_buf (ii+1, buf2);
                           strcpy (buf, &(buf2[6]));
                           sprintf (bigBuf, "%s%s%s", buf1, DIR_DELIMITER, buf);
                           if (check_dir (bigBuf))
                           { /* Modell ok */
			      if (setStringVal(buf, SIMU_MOD_NAME_IDX)) {
				 sprintf (bigBuf, NEU_MOD_ANZEIGE, buf);
				 printf ("%s\n", bigBuf);
			      }
                           } /* Modell ok */
                           else
                              mod_dir_mecker (buf);
                           reset_text_buf ();
                        } /* Index auswerten */
                     } /* Directory in "bigBuf" existiert u.es gibt darin Dirs */
                     else /* Dir in "buf" existiert nicht oder enthaelt keine Dirs */
                        printf ("%s\n", KEIN_MODELL_TXT);
                  } /* SIMU_MOD_NAME_IDX-Eintrag hat einen Wert */
               break; /* SIMOT */
                  
               case LESAK_APPL: /* ----------------- LESAK ------------------ */
                  data_aend = teach_in ();
               break; /* LESAK */
            } /* switch (appl_code) */
         break; /* Applikationsspezifische Update-Funktion */
         
         case 8: /* ----------------------- Hindernisse --------------------- */
            if (appl_code == LESAK_APPL)  /* -------- Falls LESAK: ---------- */
               hindernismanagement();
         break; /* Hindernisse */
            
         case ESC: /* ----------------------- End/Quit ---------------------- */
            sub_men_fin = TRUE;
         break; /* End/Quit */
            
      } /* switch (men_item) */
      show_and_clear_fbuf(LIMITED_LOGGING);     /* eventuelle Fehler anzeigen */
   } /* Menue-Loop */
   while (!(sub_men_fin));

   if (data_aend) { /* something changed */
      processReadTskData(PARAM_FROM_MENU);
      printf(CHANGE_WARNING_TXT);
      write_text_buf(CLEAR);                       /* Ausgabe des Textpuffers */
      printf("\n");
   } /* something changed */
   show_and_clear_fbuf(LIMITED_LOGGING);        /* eventuelle Fehler anzeigen */
   
   return (data_aend);
} /* aufg_sub_men */ 
