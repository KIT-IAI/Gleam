/*******************************************************************************
GLEAM/AE                                                                  V1.2.4

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: MENues
                        Module: Gleam/ae-C-MENues
 
Package: men                    File: g_cmen.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Modul enthaelt die Menues der TEXT_MODE-Variante.


Die Routinen im einzelnen:

void main_menue (void);
          Die Routine enthaelt das Hauptmenue.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. "hole_obs" statt "load_obs" u.Hinweis zur RobCode-Generierung.   CB 03.12.98
 2. Ueberarbeitung der Menuesteuerung.                               WJ 23.12.98
 3. hole_obs mit Kennung, ob interaktiver Aufruf (hier: FALSE)       CB 19.04.99
 4. Neue Funktion "Teste AK" im System-Submenue.                     WJ 18.08.99
 5. Einige Ausgaben werden wegen kleinem DOS-Fesster gescrollt.      WJ 08.08.00
 6. Neue Funktion im Sytem-Menue zum Testen gen. Operatoren. Neue
    gen. Operatoren OX_XO, OX_RECO, PPX_XO und PPX_RECO.             WJ 29.01.07
 7. Korrektur der Notenanzeige des Elters beim Mutationstest.        WJ 23.10.08
 8. Entfernung von BGI_MODE und des dazugehörigen Codes. Entfernung 
    ueberfluessiger Variable. Explizite Klammern zur eindeutigen 
    Zuordnung von else-Zweigen. Initialisierung von "old_appl" in 
    "load_menue" und von "del_bak" in "system_menue".                WJ 27.06.12
 9. LOAD_MOD_MEN durch LOAD_GMOD_MEN ersetzt.                        WJ 30.08.12
10. Renovierung: Streichung des "link"-Includes. Anpassung folgender
    Aufrufe: "write_text_buf", "set_and_clear_menue_window", 
    "char_dialog", "lies_...", "erfrage_outfile_spec", "lies_adr",
    "select_menue_item", "ladekinematik" und "hole_obs". Streichung 
    der Aufrufe von "clr_line". Layoutanpassungen. Ersetzung der 
    "write_line"-Ausgaben durch "printf"-Aufrufe.                    WJ 13.05.16
11. Integration der neuen TSK-Datenverwaltung. Ueberarbeitung der 
    File-I/O-Funktionen in "save_menue" und "load_menue".            WJ 07.12.16
12. Deletion of old PAR_VERS code parts.                             WJ 04.07.17
13. "Ketten pro Guete" um ref-Anzeige bei CHECKS erweitert.          WJ 31.07.17
14. Vereinheitlichung der Abbruchmeldung: Aus NOLOAD_EVO_TXT wird 
    FKT_DISABLED_TXT. Neuer Parameter fuer "show_and_clear_fbuf".    WJ 10.11.17
16. Ausgabe des Textpuffers nach erfolgreichem Laden einer TSK-
    Datei.                                                           WJ 28.11.17
17. Warnung, wenn mehr Bewertungskriterien als Simulationsergebnisse 
    spezifiziert wurden.                                             WJ 14.02.19
18. Steuerung der MEM-File-I/O durch "defaultChrFileMode":
    - Beim Experiment gemaess "defaultChrFileMode"
    - Bei Einzelfiles: 
      - bei "defaultChrFileMode" == FILE_TXT_MODE gilt der Wert
      - bei "defaultChrFileMode" == FILE_BIN_MODE wird nachgefragt   WJ 27.06.19
19. Bei gesetztem WITH_DYN_GENES und Chromosomen im Speicher und/oder 
    der letzten Population darf ein Genmodellwechsel durch Laden eines 
    EXP- oder MOD-File nur nach vorheriger Loeschung erfolgen.   
    Streichung von "evo_done".                                       WJ 10.07.19
20. Avoiding buffer overflow of static strings: system_menue(), 
    load_menue(), save_menue().                                      WJ 20.07.20
21. Reorganisation of the "info", "save", "load" and "system" menues 
    for the published version: Removal of basic chromosomes. 
    Extensions in the system menu for multilingualism.               WJ 29.09.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 29.09.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "chio.h"
#include "bew.h"
#include "appl.h"
#include "hmod.h"
#include "aufg.h"
#include "ched.h"
#include "simu.h"
#include "evo.h"
#include "men.h"
#include "menInt.h"

#include GLOBAL_TEXTS
#include MEN_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif

#undef WITH_B_CHRS      /* to be set only if basic chromosomes are used again */



/* ============================ Lokale Routinen: ============================ */
static void   info_menue    (void);

static BOOL   sel_chio_mode (void);

static void   load_menue    (void);

static void   save_menue    (void);

static void   system_menue  (void);



/* ============================ Lokale Variable: ============================ */
static INT   line_anz = 7;     /* Anzahl der Zeilen pro Mutationszahl-Anzeige */
static BOOL  neu_file       = TRUE,  /* Alle erfragten AusgFiles neu anlegen  */
             fatal_override = FALSE; /* TRUE: Weiter bei Fatal zu Testzwecken */




/* =============================== main_menue =============================== */
void main_menue (void)
{
   BOOL  fin = FALSE;
   INT   men_item;

   /* main_menue */
   do 
   { /* Menue-Loop */
      set_and_clear_menue_window (1, MAIN_MEN_TXT);
      write_menue_item ( 1,            StndMenItem, EVO_MEN_TXT);
      write_menue_item ( 2,            StndMenItem, SIMU_MEN_TXT);
      write_menue_item ( 3,            StndMenItem, INFO_MEN_TXT);
      write_menue_item ( 5,            StndMenItem, AUFG_MEN_TXT);
      write_menue_item ( 6,            StndMenItem, BEW_MEN_TXT);
      write_menue_item (LOAD_MEN_ITEM, StndMenItem, LOAD_MEN_TXT);
      write_menue_item (SAVE_MEN_ITEM, StndMenItem, SAVE_MEN_TXT);
      write_menue_item (SYS_MEN_ITEM,  AttMenItem,  SYS_MEN_TXT);
      write_menue_item (FIN_MEN_ITEM,  AttMenItem,  FIN_MEN_TXT);
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item) 
      {
	 case 1: /* Evo-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    if (!last_exp_ok)
	       printf (FMELD_LINE_TXT, LAST_EXP_FAIL);
	    if (akt_anz_krit > akt_anz_roh_erg_werte)
            { /* mehr Bewertungskriterien  als Simulationsergebnisse */
	       meldung(MEN_CMEN, akt_anz_krit, akt_anz_roh_erg_werte, "", 
		       MORE_CRITERIA_THAN_RESULTS);
	       show_and_clear_fbuf(LIMITED_LOGGING);
	    } /* mehr Bewertungskriterien  als Simulationsergebnisse */
	    if (file_data_ok (TRUE))                        /* for_evo = TRUE */
	       evo_cmenue ();
	    else 
	       printf (FMELD_LINE_TXT, BAD_DATA_FTXT);
	 break; /* Evo-Menue */

	 case 2: /* Simu-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    if (!last_exp_ok)
	       printf (FMELD_LINE_TXT, LAST_EXP_FAIL);
	    if (akt_anz_krit > akt_anz_roh_erg_werte)
            { /* mehr Bewertungskriterien  als Simulationsergebnisse */
	       meldung(MEN_CMEN, akt_anz_krit, akt_anz_roh_erg_werte, "", 
		       MORE_CRITERIA_THAN_RESULTS);
	       show_and_clear_fbuf(LIMITED_LOGGING);
	    } /* mehr Bewertungskriterien  als Simulationsergebnisse */
	    if (file_data_ok (FALSE))                      /* for_evo = FALSE */
	       simu_cmenue (&err_state);
	    else 
	       printf (FMELD_LINE_TXT, BAD_DATA_FTXT);
	 break; /* Simu-Menue */

	 case 3: /* Info-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    info_menue ();
	 break; /* Info-Menue */

	 case 5: /* ProgParams-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    tsk_aend = aufg_sub_men ();
	 break; /* ProgParams-Menue */

	 case 6: /* Bewertungs-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    bew_aend = bew_men ();
	 break; /* Bewertungs-Menue */

	 case LOAD_MEN_ITEM: /* Lade-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	    else
	       load_menue ();
	 break; /*Lade-Menue */

	 case SAVE_MEN_ITEM: /* Sicherungs-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    save_menue ();
	 break; /* Sicherungs-Menue */

	 case SYS_MEN_ITEM: /* System-Menue */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    system_menue ();
	 break; /* System-Menue */

	 case FIN_MEN_ITEM:
	 case ESC:
	    fin      = char_dialog (FIN_FRAG_TXT) == JA;
	 break;

      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);  /*Ausg.evtl.Meldungen + Loeschung*/
      if (!fatal_override)
	 get_fbuf_data (&err_state);   /* Ergebnis ist entweder OK oder Fatal */
   }  /* Menue-Loop */
   while (!fin);
} /* main_menue */



/* =============================== info_menue =============================== */
static void info_menue (void)
{
   STR_TYPE             buffer;
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   INT                  men_item, guete, lfd_nr;
   BOOL                 fin = FALSE;

   /* info_menue */
   do 
   { /* Menue-Loop */
      set_and_clear_menue_window (2, INFO_MEN_TXT);
      write_menue_item (1, StndMenItem, AK_MEM_MEN_TXT);
      write_menue_item (2, StndMenItem, AK_INFOMEN_TXT);
      write_menue_item (3, StndMenItem, AK_LEN_MEN_TXT);
      write_menue_item (4, StndMenItem, AK_AUSGMEN_TXT);
      write_menue_item (5, StndMenItem, OPTERG_MEN_TXT);
      write_menue_item (6, StndMenItem, LADSTATMEN_TXT);
      write_menue_item (7, StndMenItem, VERS_MEN_TXT);
#ifdef WITH_B_CHRS
      write_menue_item (8, StndMenItem, BAKS_MEN_TXT);
#endif /* WITH_B_CHRS */
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item) 
      {

	 case 1: /* ------------------------ AK-Mem ------------------------- */
	    gklassen (AK, TRUE);                            /* "alles" = TRUE */
	    write_text_buf (CLEAR);
	 break; /* AK-Mem */

	 case 2: /* ----------------------- AK/G Info ----------------------- */
	    printf ("%s\n\n", AK_INFO_HDR1);
	    sprintf (buffer, GUETE_QUERY, MAX_GUETE);
	    if (lies_wert (&guete, buffer, 1, MAX_GUETE, MAX_GUETE+1))
	       break;                                              /* Abbruch */
	    sprintf (buffer, AK_INFO_HDR2, guete);
	    printf ("\n%s\n", buffer);
#ifdef CHECKS
	    printf ("%s   Ref\n", AK_INFO_HDR3); /* Erweiterung f.Testausgaben*/
#else
	    printf ("%s\n", AK_INFO_HDR3);
#endif /* no CHECKS */
	    ketten_pro_guete (ZEIGE_CHS_DETAIL, guete, AK, &err_state);    
	    write_text_buf (CLEAR);
	 break; /* AK/G Info */

	 case 3: /* ----------------------- AK-Laengen ---------------------- */
	    zeige_k_laengen (AK, &err_state);
	    write_text_buf (CLEAR);
	 break; /* AK-Laengen */

	 case 4: /* ----------------------- AK-Ausgabe ---------------------- */
	    printf ("%s\n\n", AK_AUSG_HDR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT))
	    { /* Adresse mit gueltiger Guete */
	       if (get_chain (&ch_ptr, &lfd_nr, AK, guete, &guete, &err_state))
		  zeige_kette (ch_ptr);
	       else
		  printf ("%s\n", NIX_CH);
	    } /* Adresse mit gueltiger Guete */
	 break; /* AK-Ausgabe */

	 case 5: /* ----------------------- Opt-Ergebn ---------------------- */
	    printf ("%s\n\n", OPT_ERG_HDR1);
	    display_lsk_results (AK);
	    write_text_buf (CLEAR);
	    if (char_dialog (AK_ERG_QUERY) != 'N')
	    { /* AK-Ergebnisse darstellen */
	       strcpy (buffer, OPT_ERG_HDR2);
	       strcat (buffer, OPT_ERG_HDR1);
	       printf ("\n%s\n", buffer);
	       prep_opt_glob_erg (AK);
	       printf ("%s\n", BAKS_HDR2);
	       write_text_buf (CLEAR);
	    } /* AK-Ergebnisse darstellen */
	    if (ges_ak_stat_dat.gen_anz > 0)
	    { /* Zuruecksetzen der AK-Evo-Ergebnisse */
	       if (char_dialog (AKERGCLR_QUERY) == JA)
		{ /* loesche */
		   clear_evo_results (&ges_ak_stat_dat);
		   loesche_evo_erg ();
		} /* loesche */
	    } /* Zuruecksetzen der AK-Evo-Ergebnisse */
	 break; /* Opt-Ergebn */

	 case 6: /* ----------------------- LadeStatus ---------------------- */
	    prep_lade_status ();
	    write_text_buf (CLEAR);
	 break; /* LadeStatus */

	 case 7: /* ------------------------ Vers-Info ---------------------- */
	    prep_vers_info ();
	    write_text_buf (CLEAR);
	 break; /* Vers-Info */

#ifdef WITH_B_CHRS
	 case 8: /* ----------------------- Basis-AKs ---------------------- */
	    show_baks (&err_state);
	    write_text_buf (CLEAR);
	 break; /* Basis-AKs */
#endif /* WITH_B_CHRS */

	 case ESC:
	    fin = TRUE;
	 break;
      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);  /*Ausg.evtl.Meldungen + Loeschung*/
   } /* Menue-Loop */
   while (!fin);
} /* info_menue */



/* ============================= sel_chio_mode ============================== */
static BOOL sel_chio_mode (void)
{
   char  ch;

   /* sel_chio_mode */
   if (defaultChrFileMode == FILE_BIN_IO)
   { /* nachfragen */
      ch = char_dialog (CHIOMODE_QUERY);
      if ((ch == JA) || (ch == CR))
	 return (FILE_BIN_IO);
      else
	 return (FILE_TXT_IO);
   } /* nachfragen */
   else
      return (defaultChrFileMode);
} /* sel_chio_mode */



/* =============================== load_menue =============================== */
static void load_menue (void)
{
   char         buffer[2*MAX_STRING];
   STRING_TYPE  fspec_buf, buf1;
   INT          men_item, index, old_appl, ii;
   BOOL         do_load, load_ok, mit_ch_file, mit_anzg;
   BOOL         fin = FALSE;

   /* load_menue */
   do 
   { /* Menue-Loop */
      set_and_clear_menue_window (2, LOAD_MEN_TXT);
      write_menue_item ( 1, StndMenItem, IO_EXP_MEN);
      write_menue_item ( 2, StndMenItem, LOAD_GMOD_MEN);
      write_menue_item ( 3, StndMenItem, IO_BEW_MEN);
      write_menue_item ( 4, StndMenItem, IO_TSK_MEN);
      write_menue_item ( 5, StndMenItem, LOAD_EVO_MEN);
      write_menue_item ( 6, StndMenItem, IO_MEM_MEN);
      write_menue_item ( 7, StndMenItem, IO_AKS_MEN);
      for (ii = 0;  ii < appl_file_anz;  ii++)
	 if (appl_file_descr[ii].ladbar)
	    write_menue_item(9+ii, StndMenItem, appl_file_descr[ii].afile_men);
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item) 
      {
	 case 1: /* ----------------------- Experiment ---------------------- */
	    printf ("%s\n\n", LOAD_EXP_HDR1);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
	    do_load = TRUE;
#ifdef WITH_DYN_GENES
	    if (number_of_chains(1, MAX_GUETE, AK, &err_state) || (last_psize > 0))
	    { /* chromosomes in memory or in last population */
	       printf("%s\n", LOAD_MEM_CLR);  /* Genmodellwechsel nur bei leerem AK-Mem! */
	       do_load = char_dialog(DO_IT_NO_QUERY) == JA; 
	       if (do_load)
	       { /* Der meint's ernst */
		  err_state |= delete_ak_mem();
	          err_state |= del_ep();
	          if (err_state != GLEAM_OK)
		  { /* error while erasing chromosomes */
		     printf("%s\n", CHR_LOE_ERR);
		     do_load = FALSE;
		  } /* error while erasing chromosomes */
	       } /* Der meint's ernst */
	    } /* chromosomes in memory or in last population */
#else /* with genes of fixed max. number of parameters */
	    if (last_psize > 0)
	       do_load = char_dialog(LOAD_EXP_QUERY) == JA;
#endif /* with genes of limited and fixed max. number of parameters */
	    if (do_load)
  	    { /* wirklich laden! */
	       old_appl = appl_code;   /* load_exp veraendert evtl. appl_code */
	       if (erfrage_fspec (fspec_buf, "exp"))
	       { /* EXP soll geladen werden */
		  terminate_ext_simu ();   /* ggf. ext. Simulator terminieren */
                  getStringVal(memFileSpecSave, MEM_FILE_SPEC_IDX, GLEAM_OK);
		  load_ok = load_exp (&mit_ch_file, fspec_buf, FALSE);
		  printf ("\n%s\n", LOAD_EXP_HDR1);
		  sprintf (buffer, LOAD_EXP_HDR2, fspec_buf);
		  printf ("%s\n", buffer);
		  write_text_buf (CLEAR);
		  if (load_ok)
	          { /* Default-Files geladen */
		     if (mit_ch_file)
		     { /* Mit Kettenfile */
			if (getStringVal(fspec_buf, MEM_FILE_SPEC_IDX, GLEAM_ERROR))
			{ /* MEM file specified */
			   if (open_ch_file (fspec_buf, 1, CH_READ, GLOBAL_SAVE, defaultChrFileMode))
		           { /* Kettenfile offen */
			      mit_anzg = char_dialog (LOAD_MIT_ANZGQ) == JA;
#ifndef WITH_DYN_GENES
			      err_state |= delete_ak_mem ();
#endif /* with genes of limited and fixed max. number of parameters */
			      if (restore_chs (TRUE, mit_anzg))  /* MitBAKs=TRUE */
			      { /* ok */
				 setStringVal(fspec_buf, MEM_FILE_SPEC_IDX);
				 if (mit_anzg)
				    write_text_buf (CLEAR); /* detailierte Anzeige */
				 else
			         { /* summarische Anzeige */
				    get_buf (1, buf1);
				    sprintf (buffer, "  %s", buf1);
				    printf ("%s\n", buffer);
				    get_buf (2, buf1);
				    sprintf (buffer, "    %s", buf1);
				    printf ("%s\n", buffer);
				    reset_text_buf ();
				 } /* summarische Anzeige */
			      } /* ok */
			      close_ch_file ();
			   } /* Kettenfile offen */
			   else
			      printf ("%s\n", NO_CH_FILE_TXT);
			} /* MEM file specified */
			else
			   printf ("%s\n", NO_CH_FILE_TXT);
		     } /* Mit Kettenfile */
		     else
		     { /* Ohne Kettenfile */
			printf ("%s\n", NO_CH_FILE_TXT);
			if (old_appl != appl_code)
		        { /* Applikation wechselt, AK-Mem loeschen! */
			   err_state |= delete_ak_mem ();
			   printf ("%s\n", LOAD_EXP_MDEL);
			   setStringVal("", MEM_FILE_SPEC_IDX);
			} /* Applikation wechselt, AK-Mem loeschen! */
			else
			   setStringVal(memFileSpecSave, MEM_FILE_SPEC_IDX);
		     } /* Ohne Kettenfile */
		     if (last_exp_ok)
			printf ("%s\n", LOAD_EXPOK_TXT);
		     else
			printf ("%s\n", LOAD_EXP_F_TXT);
		  } /* Default-Files geladen */
		  else
		     printf ("%s\n", LOAD_EXP_F_TXT);
	       } /* EXP soll geladen werden */
	    } /* wirklich laden! */
	    else
	       printf ("%s\n", FKT_TERM_TXT);
	 break; /* Experiment */

	 case 2: /* ----------------------- Handl.Mod ----------------------- */
	    printf ("%s\n\n", LOAD_MOD_HDR);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
#ifdef WITH_DYN_GENES
	    if (number_of_chains(1, MAX_GUETE, AK, &err_state) || (last_psize > 0))
	    { /* chromosomes in memory or in last population */
	       printf ("%s\n", LOAD_MEM_CLR);  /* Genmodellwechsel nur bei leerem AK-Mem! */
	       if (char_dialog (DO_IT_NO_QUERY) == JA) 
	       { /* Der meint's ernst */
	          err_state |= delete_ak_mem();
	          err_state |= del_ep();
	          if (err_state != GLEAM_OK)
		     printf ("%s\n", FKT_UNVOLLST);
	       } /* Der meint's ernst */
	       else
	       { /* Abbruch */
	          printf ("%s\n", FKT_TERM_TXT);
	          break;                                           /* Abbruch */
	       } /* Abbruch */
	    } /* chromosomes in memory or in last population */
#endif /* WITH_DYN_GENES */
	    if (erfrage_fspec (fspec_buf, "mod"))
	    { /* MOD soll geladen werden */
	       terminate_ext_simu ();  /* ggf. externen Simulator terminieren */
	       if (load_mod_file (fspec_buf, FALSE))   /* "init_mode" = FALSE */
	       { /* File ok */
		  sprintf (buffer, LOAD_FILE_OK, fspec_buf);
		  printf ("\n%s\n", buffer);
	       } /* File ok */
	    } /* MOD soll geladen werden */
	 break; /* Handl.Mod */

	 case 3: /* ----------------------- Bewertung ----------------------- */
	    printf ("%s\n\n", LOAD_BEW_HDR);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
	    if (erfrage_fspec (fspec_buf, "bew"))
	    { /* BEW soll geladen werden */
	       if (load_bew_file (fspec_buf, FALSE))   /* "init_mode" = FALSE */
	       { /* File ok */
		  sprintf (buffer, LOAD_FILE_OK, fspec_buf);
		  printf ("\n%s\n", buffer);
	       } /* File ok */
	    } /* BEW soll geladen werden */
	 break; /* Bewertung */

	 case 4: /* ----------------------- ProgParams ---------------------- */
	    printf ("%s\n\n", LOAD_TSK_HDR);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
	    if (erfrage_fspec (fspec_buf, "tsk"))
	    { /* TSK soll geladen werden */
	       if (load_tsk_file (fspec_buf, FALSE))   /* "init_mode" = FALSE */
	       { /* File ok */
		  sprintf (buffer, LOAD_FILE_OK, fspec_buf);
		  printf ("\n%s\n", buffer);
		  write_text_buf(CLEAR);
	       } /* File ok */
	    } /* TSK soll geladen werden */
	 break; /* ProgParams */

	 case 5: /* ----------------------- EvoParams ----------------------- */
	    printf ("%s\n\n", LOAD_EVO_HDR);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
	    if (erfrage_fspec (fspec_buf, "evo"))
	    { /* EVO soll geladen werden */
	       if (load_evo_file (fspec_buf, FALSE))   /* "init_mode" = FALSE */
	       { /* File ok */
		  sprintf (buffer, LOAD_FILE_OK, fspec_buf);
		  printf ("\n%s\n", buffer);
	       } /* File ok */
	    } /* EVO soll geladen werden */
	 break; /* EvoParams */

	 case 6: /* ------------------------- AK-Mem ------------------------ */
	    printf("%s\n\n", LOAD_MEM_HDR);
	    if (number_of_chains(1, MAX_GUETE, AK, &err_state))
	    { /* chromosome memory is not empty */
	       printf("%s\n", LOAD_MEM_CLR);
	       if (char_dialog(DO_IT_NO_QUERY) == JA)
	       { /* Der meint's ernst */
                  err_state |= delete_ak_mem();
	          do_load = err_state == GLEAM_OK;
		  if (!do_load)
	             printf("%s\n", FKT_UNVOLLST);
               } /* Der meint's ernst */
	       else
	          printf("%s\n", FKT_TERM_TXT);
	    } /* chromosome memory is not empty */
	    else
	      do_load = TRUE;
	    if (do_load)
	    { /* MEM geloescht */
	       if (erfrage_fspec (fspec_buf, "mem"))
	       { /* MEM soll geladen werden */
		  printf ("\n%s\n", LOAD_MEM_HDR);
		  if (open_ch_file (fspec_buf, 1, CH_READ, GLOBAL_SAVE, sel_chio_mode ()))
		  { /* Kettenfile offen */
		     mit_anzg = char_dialog (LOAD_MIT_ANZGQ) == JA;
		     if (restore_chs (TRUE, mit_anzg))  /*MitBAKs = TRUE*/
		     { /* ok */
		        setStringVal(fspec_buf, MEM_FILE_SPEC_IDX);
		        write_text_buf (CLEAR);
		     } /* ok */
		     close_ch_file ();
		  } /* Kettenfile offen */
		  else
		     printf ("%s\n", NO_CH_FILE_TXT);
	       } /* MEM soll geladen werden */
	    } /* MEM geloescht */
	 break; /* AK-Mem */

	 case 7: /* ----------------------- Akt.Ketten ---------------------- */
	    printf ("%s\n\n", LOAD_AKS_HDR);
	    if (erfrage_fspec (fspec_buf, "aks"))
	    { /* AKS soll geladen werden */
	       printf ("\n%s\n", LOAD_AKS_HDR);
	       if (open_ch_file (fspec_buf, 1, CH_READ, SELECTIVE_SAVE, sel_chio_mode ()))
	       { /* Kettenfile offen */
		  mit_anzg = char_dialog (LOAD_MIT_ANZGQ) == JA;
		  if (restore_chs (TRUE, mit_anzg))         /* MitBAKs = TRUE */
	          { /* ok */
		     setStringVal(fspec_buf, MEM_FILE_SPEC_IDX);
		     write_text_buf (CLEAR);
		  } /* ok */
		  close_ch_file ();
	       } /* Kettenfile offen */
	       else
		  printf ("%s\n", NO_CH_FILE_TXT);
	    } /* AKS soll geladen werden */
	 break; /* Akt.Ketten */

	 default: /* --------------------- Add-Appl-File -------------------- */
	    index = men_item - 9; /* Idx ok, da nur legale Items selektierbar*/
	    sprintf (buffer, LOAD_APPLF_HDR, appl_file_descr[index].afile_bez);
	    printf ("%s\n\n", buffer);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
	    strcpy (buf1, appl_file_descr[index].afile_ext);   /* Copy, sonst */
	    if (erfrage_fspec (fspec_buf, buf1))     /* wird Extention gross! */
	    { /* File soll geladen werden */
	       printf ("\n%s\n", buffer);
	       load_ok = FALSE;
	       switch (appl_code)
	       {
	          case LESAK_APPL:
		     if (index == 0)
	             { /* Kinematik laden */
			if (ladekinematik (fspec_buf, 2))
		        { /* Kinematik-Datei gelesen */
			   strcpy (appl_file_descr[0].afile_spec, fspec_buf);
			   appl_file_descr[0].bad_data = FALSE;
			   load_ok = TRUE;
			} /* Kinematik-Datei gelesen */
			else
			   appl_file_descr[0].bad_data = TRUE;
		     } /* Kinematik laden */
		     if (index == 1)
	             { /* Hindernisse laden */
			if (hole_obs (fspec_buf, 2))
		        { /* Hindernis-Datei gelesen */
			   strcpy (appl_file_descr[1].afile_spec, fspec_buf);
			   appl_file_descr[1].bad_data = FALSE;
			   load_ok = TRUE;
			} /* Hindernis-Datei gelesen */
			else 
			   appl_file_descr[1].bad_data = TRUE;
	             } /* Hindernisse laden */
	          break; /* LESAK_APPL */

	          case ADD_APPL:
		     load_ok = add_appl_load_1_afile (index, fspec_buf);
	          break; /* ADD_APPL */

	          default:
	             printf ("%s\n", LOAD_NO_APPL);
	       } /* CASE  appl_code */
	       if (load_ok)
		  printf ("\n%s\n", LOAD_AFILE_OK);
	       write_text_buf (CLEAR); /* Ausg. eventueller Mldg. d. LadeProc */
	    } /* File soll geladen werden */
	 break; /* Add-Appl-File */

	 case ESC:
	    fin = TRUE;
	 break;
      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);  /*Ausg.evtl.Meldungen + Loeschung*/
   } /* Menue-Loop */
   while (!fin);
} /* load_menue */



/* =============================== save_menue =============================== */
static void save_menue (void)
{
   char         buffer[2*MAX_STRING];
   STRING_TYPE  fspec, exp_fspec, exp_fname;
   STR_TYPE     buf1;
   INT          men_item, index, lfd_nr, guete, znr, ii, ij;
   GSTATE       erg_state;
   char         ch;
   BOOL         save_ok, wuerg, abort, keep_file, 
                bew_save, tsk_save, mem_save,
                fin = FALSE;

   /* save_menue */
   do 
   { /* Menue-Loop */
      set_and_clear_menue_window (2, SAVE_MEN_TXT);
      write_menue_item ( 1, StndMenItem, IO_EXP_MEN);
      write_menue_item ( 3, StndMenItem, IO_BEW_MEN);
      write_menue_item ( 4, StndMenItem, IO_TSK_MEN);
      write_menue_item ( 6, StndMenItem, IO_MEM_MEN);
      write_menue_item ( 7, StndMenItem, IO_AKS_MEN);
#ifdef WITH_B_CHRS
      write_menue_item ( 8, StndMenItem, SAVE_BAKS_MEN);
#endif /* WITH_B_CHRS */
      for (ii = 0;  ii < appl_file_anz;  ii++)
	 if (appl_file_descr[ii].rettbar)
	    write_menue_item (9+ii, StndMenItem, appl_file_descr[ii].afile_men);
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item) 
      {
	 case 1: /* ----------------------- Experiment ---------------------- */
	    last_exp_ok = TRUE;      /* Benutzer haelt Daten fuer konsistent! */
	    bew_save    = FALSE;
	    tsk_save    = FALSE;
	    mem_save    = FALSE;
	    printf ("%s\n\n", SAVE_EXP_HDR1);
	    strcpy (exp_fspec, "exp");
	    if (erfrage_outfile_spec (&neu_file, exp_fspec, "", FKT_TERM_TXT))
	    { /* Der meint's ernst */
	       ij = strlen (exp_fspec) - 4;
	       strcpy (exp_fname, exp_fspec);
	       exp_fname[ij] = '\0';

	       /* ------------------------ BEW-File: ------------------------ */
	       getStringVal(fspec, BEW_FILE_SPEC_IDX, GLEAM_ERROR);
	       if (bew_aend)
	       { /* Bewertung evtl. veraendert */
		  printf ("%s\n", SAV_XBEW_TXT);
		  sprintf (buffer, SAV_XBEW_QUERY, fspec);
		  ch = char_dialog (buffer);
		  keep_file = ch  == JA;
	       } /* Bewertung evtl. veraendert */
	       else
	       { /* Bewertung unveraendert */
		  sprintf (buffer, SAV_XBEWQUERY2, fspec);
		  ch = char_dialog (buffer);
		  keep_file = ((ch == JA) || (ch == CR));
	       } /* Bewertung unveraendert */
	       if (ch == ESC)
	       { /* Funktionsabbruch */
		  printf ("%s\n", FKT_TERM_TXT);
		  break;
	       } /* Funktionsabbruch */
	       if (keep_file)
	       { /* Altes BEW-File soll weiterhin gelten */
		  bew_aend = FALSE;
		  bew_save = FALSE;
	       } /* Altes BEW-File soll weiterhin gelten */
	       else
	       { /* Neues BEW-File */
		  strcpy (tmp_bew_file_spec, "bew");
		  if (erfrage_outfile_spec (&neu_file, tmp_bew_file_spec, exp_fname, 
					    SAV_NOT_SAVED))
		     bew_save = TRUE;
	       } /* Neues BEW-File */

	       /* ------------------------ TSK-File: ------------------------ */
	       getStringVal(fspec, TSK_FILE_SPEC_IDX, GLEAM_ERROR);
	       if (tsk_aend)
	       { /* Aufgabenstellung evtl. veraendert */
		  printf ("%s\n", SAV_XTSK_TXT);
		  sprintf (buffer, SAV_XTSK_QUERY, fspec);
		  ch = char_dialog (buffer);
		  keep_file = ch  == JA;
	       } /* Aufgabenstellung evtl. veraendert */
	       else
	       { /* Aufgabenstellung unveraendert */
		  sprintf (buffer, SAV_XTSKQUERY2, fspec);
		  ch = char_dialog (buffer);
		  keep_file = ((ch == JA) || (ch == CR));
	       } /* Aufgabenstellung unveraendert */
	       if (ch == ESC)
	       { /* Funktionsabbruch */
		  printf ("%s\n", FKT_TERM_TXT);
		  break;
	       } /* Funktionsabbruch */
	       if (keep_file)
	       { /* Altes TSK-File soll weiterhin gelten */
		  tsk_aend = FALSE;
		  tsk_save = FALSE;
	       } /* Altes TSK-File soll weiterhin gelten */
	       else
	       { /* Neues TSK-File */
		  strcpy (tmp_tsk_file_spec, "tsk");
		  if (erfrage_outfile_spec (&neu_file, tmp_tsk_file_spec, exp_fname, 
					    SAV_NOT_SAVED))
		     tsk_save = TRUE;
	       } /* Neues TSK-File */

	       /* ------------------------ MEM-File: ------------------------ */
	       ch = char_dialog (SAV_MEM_QUERY);
	       if (ch == ESC)
	       { /* Funktionsabbruch */
		  printf ("%s\n", FKT_TERM_TXT);
		  break;
	       } /* Funktionsabbruch */
	       mem_save = ((ch == JA) || (ch == CR));

	       /* ----------------------- Appl-Files: ----------------------- */
	       for (ii = 0;  ii < appl_file_anz; ii++)
	       { /* appl-file loop */
		  if (appl_file_descr[ii].rettbar && appl_file_descr[ii].ladbar)
	          { /* rettbares Appl-File */
		     sprintf (buffer, SAV_APPL_QUERY, appl_file_descr[ii].afile_bez);
		     ch = char_dialog (buffer);
		     if (ch == ESC)
		     { /* Funktionsabbruch */
			printf ("%s\n", FKT_TERM_TXT);
			break;
		     } /* Funktionsabbruch */
		     appl_file_descr[ii].zu_retten = ((ch == JA) || (ch == CR));
		     if (appl_file_descr[ii].zu_retten)
		     { /* Appl-File retten! */
			strcpy (tmp_afile_spec[ii], appl_file_descr[ii].afile_ext);
			appl_file_descr[ii].zu_retten = 
			   erfrage_outfile_spec (&neu_file, tmp_afile_spec[ii], 
						 exp_fname, SAV_NOT_SAVED);
		     } /* Appl-File retten! */
		  } /* rettbares Appl-File */
		  else
		     appl_file_descr[ii].zu_retten = FALSE;
	       } /* appl-file loop */
	    
	       /* ---------------- Retten und Schlussmeldung: --------------- */
	       save_exp (exp_fspec, bew_save, tsk_save, mem_save);
	       printf ("\n%s\n", SAVE_EXP_HDR1);
	       write_text_buf (CLEAR);
	    } /* Der meint's ernst */
	 break; /* Experiment */

	 case 3: /* ----------------------- Bewertung ----------------------- */
	    printf("%s\n\n", SAVE_BEW_HDR);
	    if (getStringVal(fspec, BEW_FILE_SPEC_IDX, GLEAM_ERROR))
	    { /* BEW-file entry defined */
	       if ((strlen(fspec) > 3) && strcmp(fspec, BAD_DATA_TXT))
	       { /* BEW-file entry not empty and no spoiled data */
		  ij = strlen(fspec) - 4;
		  strcpy(buffer, fspec);
		  buffer[ij] = '\0';
	       } /* BEW-file entry not empty and no spoiled data */
	       else
		  strcpy(buffer, "");
	       strcpy(fspec, "bew");
	       if (erfrage_outfile_spec(&neu_file, fspec, buffer, FKT_TERM_TXT))
	       { /* Der meint's ernst */
		  if ((erg_state = save_bew(fspec)) == GLEAM_OK)
		  { /* Rettung ok */
		     printf("%s\n", SAVE_OK_TXT);
		     setStringVal(fspec, BEW_FILE_SPEC_IDX);
		     bew_aend = FALSE;
		  } /* Rettung ok */
		  else
		     printf("%s\n", SAVE_FEHL_TXT);
		  err_state |= erg_state;
	       } /* Der meint's ernst */
	    } /* BEW-file entry defined */
	 break; /* Bewertung */

	 case 4: /* ----------------------- ProgParams ---------------------- */
	    printf("%s\n\n", SAVE_TSK_HDR);
	    if (getStringVal(fspec, TSK_FILE_SPEC_IDX, GLEAM_ERROR))
	    { /* TSK-file entry defined */
	       if ((strlen(fspec) > 3) && strcmp(fspec, BAD_DATA_TXT))
	       { /* TSK-file entry not empty and no spoiled data */
		  ij = strlen(fspec) - 4;
		  strcpy(buffer, fspec);
		  buffer[ij] = '\0';
	       } /* TSK-file entry not empty and no spoiled data */
	       else
		  strcpy(buffer, "");
	       strcpy(fspec, "tsk");
	       if (erfrage_outfile_spec(&neu_file, fspec, buffer, FKT_TERM_TXT))
	       { /* Der meint's ernst */
		  if ((erg_state = saveTskFile (fspec)) == GLEAM_OK)
		  { /* Rettung ok */
		     printf("%s\n", SAVE_OK_TXT);
		     setStringVal(fspec, TSK_FILE_SPEC_IDX);
		     tsk_aend = FALSE;
		  } /* Rettung ok */
		  else
		     printf("%s\n", SAVE_FEHL_TXT);
		  err_state |= erg_state;
	       } /* Der meint's ernst */
	    } /* TSK-file entry defined */
	 break; /* ProgParams */

	 case 6: /* ------------------------ AK-Mem ------------------------- */
	    printf ("%s\n\n", SAVE_MEM_HDR);
	    strcpy (fspec, "mem");
	    if (erfrage_outfile_spec (&neu_file, fspec, "", FKT_TERM_TXT))
	    { /* Der meint's ernst */
	       open_ch_file (fspec, 1, CH_WRITE, GLOBAL_SAVE, sel_chio_mode ());
	       printf ("\n");
	       if (save_ch_mem ())
		  setStringVal(fspec, MEM_FILE_SPEC_IDX);       /* Rettung ok */
	       else
		  printf ("%s\n", SAVE_FEHL_TXT);
	       close_ch_file ();
	       write_text_buf (CLEAR);
	    } /* Der meint's ernst */
	 break; /* AK-Mem */

	 case 7: /* ----------------------- Akt.Ketten ---------------------- */
	    printf ("%s\n\n", SAVE_AKS_HDR);
	    strcpy (fspec, "aks");
	    if (erfrage_outfile_spec (&neu_file, fspec, "", FKT_TERM_TXT))
	    { /* Der meint's ernst */
	       gklassen (AK, FALSE);                         /* alles = FALSE */
	       printf ("\n");
	       write_text_buf (CLEAR);
	       printf ("\n%s\n", SAVE_AKS_HDR);
	       printf ("%s\n", SAVE_INFO1_TXT);
	       printf ("%s\n", SAVE_INFO2_TXT);
	       printf ("%s\n", SAVE_INFO3_TXT);
	       printf ("%s\n", SAVE_INFO4_TXT);
	       abort = FALSE;
	       do
	       { /* Query-Loop */
		  if (lies_wert (&guete, SAVE_AK_QUERY, 0, MAX_GUETE, MAX_GUETE+1))
	          { /* Abbruch bei "guete" */
		     wuerg = TRUE;
		     abort = TRUE;
		  } /* Abbruch bei "guete" */
		  else
	          { /* Guete-Wert gelesen */
		     if (guete > 0)
		     { /* Guete ok, "lfd_nr" lesen */
			strcpy (buffer, SAVE_AK_QUERY);
			sprintf (buf1, "%d/ ", guete);
			strcat (buffer, buf1);
			if (lies_wert (&lfd_nr, buffer, 0, INT_MAX-1, INT_MAX))
		        { /* Abbruch bei "lfd_nr" */
			   wuerg = TRUE;
			   abort = TRUE;
			} /* Abbruch bei "lfd_nr" */
			else
		        { /* Lfd-Nr-Wert gelesen */
			   if (lfd_nr == 0)
			   { /* Alle AKs der Guete markieren */
			      wuerg = (erg_state = markiere_aks (guete)) != GLEAM_OK; 
			      sprintf (buffer, AKS_MARKED_TXT, guete);
			   } /* Alle AKs der Guete markieren */
			   else
			   { /* AK markieren */
			      wuerg = (erg_state = markiere_ak (guete, lfd_nr)) != GLEAM_OK;
			      adr_to_text (buf1, guete, lfd_nr, NO_ADJUST);
			      sprintf (buffer, AK_MARKED_TXT, buf1);
			   } /* AK markieren */      
			   err_state |= erg_state;
			   printf ("%s\n", buffer);
			} /* Lfd-Nr-Wert gelesen */
		     } /* Guete ok, "lfd_nr" lesen */
		  } /* Guete-Wert gelesen */
	       } /* Query-Loop */
	       while (!wuerg && (guete > 0));
	       if (!wuerg && (guete == 0))
	       { /* Dialog beendet, markierte AKs retten */
		  printf ("\n");
		  if (open_ch_file (fspec, 1, CH_WRITE, SELECTIVE_SAVE, sel_chio_mode ()))
	          { /* File offen */
		     save_marked_aks (FALSE);
		     close_ch_file ();
		     printf ("\n%s\n", SAVE_AKS_HDR);
		     write_text_buf (CLEAR);
		  } /* File offen */
		  else
		     wuerg = TRUE;
	       } /* Dialog beendet, markierte AKs retten */
	       if (wuerg)
	       { /* Dialog abgebrochen oder Fehler, Markierungen zuruecksetzen */
		  clear_save_marks ();
		  if (abort)
		     printf ("%s\n", FKT_TERM_TXT);
		  else
		     printf ("%s\n", SAVE_FEHL_TXT);
	       } /* Dialog abgebrochen oder Fehler, Markierungen zuruecksetzen */
	    } /* Der meint's ernst */
	 break; /* Akt.Ketten */

#ifdef WITH_B_CHRS
	 case 8: /* ----------------------- Basis-AKs ----------------------- */
	    printf ("%s\n\n", SAVE_BAKS_HDR);
	    strcpy (fspec, "aks");
	    if (erfrage_outfile_spec (&neu_file, fspec, "", FKT_TERM_TXT))
	    { /* Der meint's ernst */
	       if ((erg_state = markiere_aks (BASIS_AK_GUETE)) == GLEAM_OK)
	       { /* Markierung ok */
		  if (open_ch_file (fspec, 1, CH_WRITE, SELECTIVE_SAVE, sel_chio_mode ()))
	          { /* File offen */
		     save_marked_aks (TRUE);
		     close_ch_file ();
		     printf ("\n");
		     write_text_buf (CLEAR);
		  } /* File offen */
		  else
		  { /* File nicht offen */
		     clear_save_marks ();
		     printf ("%s\n", SAVE_FEHL_TXT);
		  } /* File nicht offen */
	       } /* Markierung ok */
	       else
		  printf ("%s\n", SAVE_FEHL_TXT);
	       err_state |= erg_state;
	    } /* Der meint's ernst */
	 break; /* Basis-AKs */
#endif /* WITH_B_CHRS */

	 default: /* --------------------- Add-Appl-File -------------------- */
	    index = men_item - 9; /*Index ok, da nur legale Items selektierbar*/
	    sprintf (buffer, SAVE_APPLF_HDR, appl_file_descr[index].afile_bez);
	    printf ("%s\n\n", buffer);
	    strcpy (fspec, appl_file_descr[index].afile_ext);
	    ij = strlen (appl_file_descr[index].afile_spec) - 4;
	    if (ij < 0)
	       ij = 0;          /* Zur Sicherheit, falls "afile_spec" zu kurz */
	    strcpy (buffer, appl_file_descr[index].afile_spec);
	    buffer[ij] = '\0';
	    if (erfrage_outfile_spec (&neu_file, fspec, buffer, FKT_TERM_TXT))
	    { /* Der meint's ernst */
	       save_ok = FALSE;
	       switch (appl_code)
	       {
	          case LESAK_APPL:
	             if (index == 2)
			printf (FMELD_LINE_TXT, SAV_NIX_RCODE);
		     else
			printf ("%s\n", SAV_NOT_SAVED);
		  break; /* LESAK_APPL */

	          case ADD_APPL:
		     save_ok = add_appl_save_1_afile (index, fspec);
                  break; /* ADD_APPL */

	          default:
		     printf ("%s\n", LOAD_NO_APPL);
	       } /* CASE  appl_code */
	       if (save_ok)
		  printf ("%s\n", SAVE_OK_TXT);
	       else
		  printf ("%s\n", SAVE_FEHL_TXT);
	       write_text_buf (CLEAR);  /* Ausgabe eventueller Mldg. d. SaveProc */
	    } /* Der meint's ernst */
	 break; /* Add-Appl-File */

	 case ESC:
	    fin = TRUE;
	 break;
      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);  /*Ausg.evtl.Meldungen + Loeschung*/
   } /* Menue-Loop */
   while (!fin);
} /* save_menue */



/* ============================== system_menue ============================== */
static void system_menue (void)
{
   CHAIN_ELEM_PTR_TYPE  elter1_ptr, elter2_ptr, child1_ptr, child2_ptr;
   STR_TYPE  buffer;
   BUF_TYPE  buf2;
   DOUBLE    note, note2;
   INT       men_item, anz, go_idx, ii,
             guete, lfd_nr, guete2, lfd_nr2;
   GSTATE    res_state;
   char      ch;
   BOOL      ak_ok, ak_is_bak, done, del_bak,
             fin = FALSE;

   /* system_menue */
   do 
   { /* Menue-Loop */
      set_and_clear_menue_window (2, SYS_MEN_TXT);
      write_menue_item ( 1, StndMenItem, SYS_MINFO_MEN);
      write_menue_item ( 2, StndMenItem, SYS_AK_GEN_MEN);
      write_menue_item ( 3, StndMenItem, SYS_AKTEST_MEN);
      write_menue_item ( 4, StndMenItem, SYS_CHEDI_MEN);
      write_menue_item ( 5, StndMenItem, SYS_DEL_AK_MEN);
      write_menue_item ( 6, AttMenItem,  SYS_DEL_G_MEN);
      write_menue_item ( 7, AttMenItem,  SYS_DELMEM_MEN);
      write_menue_item ( 9, StndMenItem, SYS_HAM_MEN);
      write_menue_item (10, StndMenItem, SYS_ZEIG_MUTZ);
      write_menue_item (11, StndMenItem, SYS_GOPTST_MEN);
      write_menue_item (12, StndMenItem, SYS_GENREP_MEN);
      write_menue_item (13, AttMenItem,  IGN_FATAL_MEN);
#ifdef WITH_B_CHRS
      write_menue_item (15, StndMenItem, SYS_AK2BAK_MEN);
      write_menue_item (16, StndMenItem, SYS_AKG2BAKMEN);
      write_menue_item (17, StndMenItem, SYS_BAK2AK_MEN);
      write_menue_item (18, StndMenItem, SYS_BAKS2AKMEN);
      write_menue_item (19, AttMenItem,  SYS_DELBAKSMEN);
#endif /* WITH_B_CHRS */
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item) 
      {
	 case 1: /* ------------------------ MemInfo ------------------------ */
	    mem_info ();
	 break; /* MemInfo */

	 case 2: /* ---------------------- Erzeuge AK ---------------------- */
	    res_state = do_gen_ak ();
	    write_text_buf (CLEAR);
	 break; /* Erzeuge AK */

	 case 3: /* ----------------------- Teste AK ----------------------- */
	    printf ("%s\n\n", AK_TEST_HDR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
	    { /* AK-Adr ok */
	       reset_text_buf ();
	       if (do_teste_ak (guete, lfd_nr))
		  printf ("\n%s", AK_OK_TEXT);
	       printf ("\n");
	       write_text_buf (CLEAR);
	    } /* AK-Adr ok */
	 break; /* Teste AK */

	 case 4: /* ------------------------ K-Editor ----------------------- */
	    c_chain_edi ();
	 break; /* K-Editor */

	 case 5: /* ----------------------- Loesche AK ---------------------- */
	    printf ("%s\n\n", DEL_AK_HDR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
	    { /* AK-Adr ok */
	       err_state |= check_bak (&ak_ok, &ak_is_bak, guete, lfd_nr);
	       if (ak_ok)
	       { /* AK existiert */
		  del_bak = FALSE;
		  if (ak_is_bak)
	          { /* AK ist BAK */
		     ch = char_dialog (DEL_BAK_QUERY);
		     if (ch == ESC)
		     { /* Abbruch */
			printf ("%s\n", FKT_TERM_TXT);
			break;
		     } /* Abbruch */
		     del_bak = ch == JA;
		  } /* AK ist BAK */
		  err_state |= del_chain (&done, AK, guete, lfd_nr, del_bak);
		  if (done)
		     printf ("\n%s\n", DEL_AK_DONETXT);
		  else 
		     printf ("\n%s\n", DEL_AK_NIX_TXT);
	       } /* AK existiert */
	       else 
		  printf ("\n%s\n", NIX_CH);
	    } /* AK-Adr ok */
	 break; /* Loesche AK */

	 case 6: /* --------------------- Loe.AKs/Guete --------------------- */
	    printf ("%s\n\n", DEL_GCLASS_HDR);
	    sprintf (buffer, GUETE_QUERY, MAX_GUETE);
	    if (lies_wert (&guete, buffer, 0, MAX_GUETE, MAX_GUETE+1))
	       break;                                              /* Abbruch */
	    if ((res_state = clear_vk (AK, guete)) == GLEAM_OK)
	       printf ("\n%s\n", ERLEDIGT_TXT);
	    else
	       printf ("\n%s\n", FKT_UNVOLLST);
	    err_state |= res_state;               /* Fehlerstatus uebernehmen */
	 break; /* Loe.AKs/Guete */

	 case 7: /* ----------------------- Loe.AK-Mem ---------------------- */
	    printf ("%s\n\n", DEL_AKMEM_HDR);
	    if (evo_aktiv)
	    { /* Unzulaessig bei laufender Evolution */
	       printf ("\n%s\n", FKT_DISABLED_TXT);
	       break;                                              /* Abbruch */
	    } /* Unzulaessig bei laufender Evolution */
	    if (char_dialog (DO_IT_NO_QUERY) == JA) 
	    { /* Der meint's ernst */
	       err_state |= delete_ak_mem ();
	       if (err_state == GLEAM_OK)
		  printf ("\n%s\n", ERLEDIGT_TXT);
	       else
		  printf ("\n%s\n", FKT_UNVOLLST);
	    } /* Der meint's ernst */
	    else
	       printf ("%s\n", FKT_TERM_TXT);
	 break; /* Loe.AK-Mem */

	 case 9: /* ----------------------- HamDist ------------------------ */
	    printf ("%s\n\n", HAM_DIST_HDR);
	    if (lies_adr (&guete,  &lfd_nr,  AK_QUERY_TXT) &&
	     lies_adr (&guete2, &lfd_nr2, AK_QUERY_TXT)) 
	    { /* AK-Adr ok */
	       err_state |= do_ham_dist (buffer, guete, lfd_nr, guete2, lfd_nr2);
	       printf ("\n%s\n", buffer);
	    } /* AK-Adr ok */
	 break; /* HamDist */

	 case 10: /* ---------------------- MutOp-Vert ---------------------- */
	    printf ("%s\n\n", MZ_ANZG_HDR1);
	    anz = get_last_go_idx();
	    sprintf (buffer, MZ_ANZG_QUERY1, anz);
	    if (lies_wert (&ii, buffer, 0, anz, anz + 1))
	    { /* Abbruch */
	       printf ("%s\n", FKT_TERM_TXT);
	       break;
	    } /* Abbruch */   
	    sprintf (buffer, MZ_ANZG_QUERY2, line_anz);
	    if (lies_wert (&line_anz, buffer, 4, 30, line_anz))
	    { /* Abbruch */
	       printf ("%s\n", FKT_TERM_TXT);
	       return;
	    } /* Abbruch */   
	    display_mut_zahl (ii, line_anz);
	    sprintf (buffer, MZ_ANZG_HDR2, ii);
	    printf ("\n%s\n", buffer);
	    write_text_buf (CLEAR);
	 break; /* MutOp-Vert */

	 case 11: /* ---------------------- GenOp-Test ---------------------- */
	    printf ("%s\n\n", OP_TEST_HDR);
	    sprintf (buffer, OP_IDX_QUEWRY, LAST_XO+1, LAST_XO);
	    if (lies_wert (&go_idx, buffer, -LAST_XO-1, LAST_XO, LAST_XO+1))
	       break;                                              /* Abbruch */
	    res_state = GLEAM_OK;
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
	    { /* Adresse gelesen */
	       if (get_chain (&elter1_ptr, &lfd_nr, AK, guete, &guete, &res_state))
	       { /* 1.Elter ok */
		  if (abs(go_idx) <= LAST_GO)
	          { /* Mutation */
		     if (copy_chain (&child1_ptr, elter1_ptr, FALSE, FALSE, &res_state))
		     { /* Klone ok */
			if (go_idx < 0) /* applikationsspezifischer gen. Operator: */
			   done = do_appl_mut (child1_ptr, abs(go_idx)-1,  &err_state);
			else
		        { /* Standard gen. Operator */
			   done = TRUE;
			   switch (go_idx)
			   { /* Mutationsverteiler */
			      case PAR_CHANGE_NEW:
				 res_state = akt_par_neu (child1_ptr);
			      break;

			      case PAR_CHANGE_REL:
				 res_state = akt_par_aend (&done, CHANGE_REL, child1_ptr);
			      break;

			      case ACT_NEW_PARAM:
				 res_state = akt_neu (child1_ptr);
			      break;

			      case SEGM_CHANGE_REL:
				 res_state = segm_par_aend (&done, CHANGE_REL, child1_ptr);
			      break;

			      case SEGM_NEW_PARAM:
				 res_state = segm_neu (child1_ptr);
			      break;

			      case ACT_EXCHANGE:
				 res_state = aktions_austausch (child1_ptr);
			      break;

			      case ADD_NEW_ACT:
				 res_state = plus_neue_aktion (child1_ptr);
			      break;

			      case DOUBLE_ACT:
				 res_state = aktion_verdoppeln (child1_ptr);
			      break;

			      case DELETE_ACT:
				 res_state = aktion_loeschen (child1_ptr);
			      break;

			      case ACT_TRANSLOCATION:
				 res_state = aktions_transl (child1_ptr);
			      break;

			      case SEGM_EXCHANGE:
				 res_state = abschn_austausch (child1_ptr);
			      break;

			      case DOUBLE_SEGM:
				 res_state = abschn_verdoppeln (child1_ptr);
			      break;

			      case DELETE_SEGM:
				 res_state = abschn_loeschen (child1_ptr);
			      break;

			      case SEGM_TRANSL:
				 res_state = abschn_transl (child1_ptr);
			      break;

			      case SEGM_INVERSION:
				 res_state = abschn_inversion (child1_ptr);
			      break;

			      case INTEGR_NACHB_SEGM:
				 res_state = nachb_segm_integr (child1_ptr);
			      break;

			      case INTEGR_SEGM:
				 res_state = segm_integr (child1_ptr);
			      break;

			      case SCHIEB_SEGM_GRENZ:
				 res_state    = schiebe_segm_grenze (&done, child1_ptr);
			      break;

			      case TEILE_SEGM:
				 res_state    = segm_teilung (&done, child1_ptr);
			      break;

			      case ADD_NEW_SEGM:
				 res_state = add_new_segm (child1_ptr);
			      break;

			      case PAR_CHANGE_SMALL:
				 res_state = akt_par_aend (&done, CHANGE_SMALL, child1_ptr);
			      break;

			      case SEGM_CHANGE_SMALL:
				 res_state = segm_par_aend (&done, CHANGE_SMALL, child1_ptr);
			      break;
			   } /* Mutationsverteiler */
			} /* Standard gen. Operator */
			if (done)
		        { /* Mutation hat was gemacht */
			   simulator (&note, CALC_FITNESS, child1_ptr);
			   if (note != KEINE_NOTE) { /* Ak simulierbar */
			      sprintf (buffer, FITNESS_TXT, 
				       elter1_ptr->u_el.u_xel.u_hdr.fitness_note, note);
			      printf ("\n%s\n", buffer); 
			      if (berechne_guete (&guete, note)) {
				 store_chain (&lfd_nr, child1_ptr, AK, guete);
				 adr_to_text (buf2, guete, lfd_nr, NO_ADJUST);
				 sprintf (buffer, "%s %s%s", CHILD_STORED_1, buf2, CHILD_STORED_2);
				 printf ("%s\n", buffer); 
			      }
			   } /* Ak simulierbar */
			   else
			      printf ("\n%s\n", CHILD_NOT_SIM);
			} /* Mutation hat was gemacht */
			else {
			   printf ("\n%s\n", NO_MUT_DONE);
			   delete_chain (&child1_ptr);
			}
		     } /* Klone ok */
		     else 
			printf ("\n%s\n", NO_CHILD_TXT);
		  } /* Mutation */
		  else
		  { /* Reko/XO */
		     if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
		     { /* Adresse gelesen */
			if (get_chain (&elter2_ptr, &lfd_nr, AK, guete, &guete, &res_state))
	                { /* 2.Elter ok */
			   if (go_idx < 0) /* applikationsspezifischer gen. Operator: */
			      done = do_appl_xo (&child1_ptr, &child2_ptr, &err_state,
						 elter1_ptr, elter2_ptr,
						 abs(go_idx)-1, TRUE);
			   else
			   { /* Standard XO/Reko */
			      switch (go_idx)
			      { /* Verteiler */
			         case SEGM_CROSS_OVER:
				    done = abschn_cross_over (&child1_ptr, &child2_ptr,
							      elter1_ptr, elter2_ptr,
							      &err_state);
			         break; /* SEGM_CROSS_OVER */

			         case CROSSOVER:
				    done = cross_over (&child1_ptr, &child2_ptr,
						       elter1_ptr, elter2_ptr, &err_state);
			         break; /* CROSSOVER */

			         case RECOMB:
				    done = rekombination (&child1_ptr, &child2_ptr,
							  elter1_ptr, elter2_ptr,
							  &err_state);
			         break; /* RECOMB */

			         case OX_XO:
			         case OX_RECO:
			         case PPX_XO:
			         case PPX_RECO:
				    done = orderRekoXO (&child1_ptr, &child2_ptr,
							elter1_ptr, elter2_ptr, 
							go_idx, &err_state);
				 break; /* OX_XO, OX_RECO, PPX_XO oder PPX_RECO */
			      } /* Verteiler */
			   } /* Standard XO/Reko */
			   if (done)
		           { /* XO/Reko hat was gemacht */
			      if (!dyn_aks)     /* for chromosomes of fixed length only */
				 done = (repair_2_aks (child1_ptr, child2_ptr) == GLEAM_OK);
			      if (done)
			      { /* should now be possible to simulate the chromosomes */
				 sprintf (buffer, PARENT_FITN, 
					  elter1_ptr->u_el.u_xel.u_hdr.fitness_note,
					  elter2_ptr->u_el.u_xel.u_hdr.fitness_note);
				 printf ("\n%s\n", buffer); 
				 simulator (&note,  CALC_FITNESS, child1_ptr);
				 simulator (&note2, CALC_FITNESS, child2_ptr);
				 sprintf (buffer, OFFSPRING_FITN, note, note2);
				 printf ("%s\n", buffer); 
				 if (note != KEINE_NOTE) { /* Ak simulierbar */
				    if (berechne_guete (&guete, note)) 
				       store_chain (&lfd_nr, child1_ptr, AK, guete);
				    adr_to_text (buf2, guete, lfd_nr, NO_ADJUST);
				    sprintf (buffer, "%s%s %s%s", FIRST_TXT, CHILD_STORED_1, 
					     buf2, CHILD_STORED_2);
				    printf ("%s\n", buffer); 
				 } /* Ak simulierbar */
				 else
				    printf ("%s%s\n", FIRST_TXT, CHILD_NOT_SIM);
				 if (note2 != KEINE_NOTE) { /* Ak simulierbar */
				    if (berechne_guete (&guete, note2)) 
				       store_chain (&lfd_nr, child2_ptr, AK, guete);
				    adr_to_text (buf2, guete, lfd_nr, NO_ADJUST);
				    sprintf (buffer, "%s%s %s%s", SECOND_TXT, CHILD_STORED_1, 
					     buf2, CHILD_STORED_2);
				    printf ("%s\n", buffer); 
				 } /* Ak simulierbar */
				 else
				    printf ("%s%s\n", SECOND_TXT, CHILD_NOT_SIM);
			      } /* should now be possible to simulate the chromosomes */
			   } /* XO/Reko hat was gemacht */
			   else 
			      printf ("\n%s\n", NO_CHILDREN);
			} /* 2.Elter ok */
			else 
			   printf ("\n%s\n", NIX_CH);
		     } /* Adresse gelesen */
		  } /* Reko/XO */
	       } /* 1.Elter ok */
	       else 
		  printf ("\n%s\n", NIX_CH);
	    } /* Adresse gelesen */
	 break; /* GenOp-Test */

	 case 12: /* ---------------------- Gen.Repair ---------------------- */
	    printf ("%s\n\n", GEN_REPAIR_HDR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
	    { /* AK-Adr ok */
	       err_state |= do_gen_rep (buffer, guete, lfd_nr);
	       printf ("\n%s\n", buffer);
	    } /* AK-Adr ok */
	 break; /* Gen.Repair */

	 case 13: /* ---------------------- IgnorFatal ---------------------- */
	    printf ("%s\n\n", IGN_FATAL_HDR);
	    if (err_state == GLEAM_FATAL)
	    {
	       fatal_override = TRUE;
	       err_state      = GLEAM_OK;
	       printf ("%s\n", IGN_FATAL_TXT1);
	       printf ("%s\n", IGN_FATAL_TXT2);
	    }
	 break; /* IgnorFatal */

#ifdef WITH_B_CHRS
	 case 15: /* ---------------------- AK  -> BAK ---------------------- */
	    printf ("%s\n\n", AK2BAK_HDR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
	    { /* AK-Adr ok */
	       err_state |= ak_to_bak (buffer, guete, lfd_nr);
	       printf ("\n%s\n", buffer);
	    } /* AK-Adr ok */
	 break; /* AK  -> BAK */

	 case 16: /* ---------------------- AK/G-> BAK ---------------------- */
	    printf ("%s\n\n", AKG2BAK_HDR);
	    sprintf (buffer, GUETE_QUERY, MAX_GUETE);
	    if (lies_wert (&guete, buffer, 0, MAX_GUETE, MAX_GUETE+1))
	       break;                                              /* Abbruch */
	    anz = number_of_chains (guete, guete, AK, &err_state);
	    for (lfd_nr = 1;  (lfd_nr <= anz) && (err_state == GLEAM_OK); lfd_nr++)
	       err_state = ak_to_bak (buffer, guete, lfd_nr);
	    if (err_state == GLEAM_OK)
	       sprintf (buffer, XFER_ERG_TXT, anz, guete);
	    printf ("\n%s\n", buffer);
	 break; /* AK/G-> BAK */

	 case 17: /* ---------------------- BAK -> AK ----------------------- */
	    printf ("%s\n\n", BAK2AK_HDR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT)) 
  	    { /* AK-Adr ok */
	       err_state |= bak_to_ak (buffer, guete, lfd_nr);
	       printf ("\n%s\n", buffer);
	    } /* AK-Adr ok */
	 break; /* BAK -> AK */

	 case 18: /* ---------------------- BAKs-> AKs ---------------------- */
	    printf ("%s\n\n", BAKS2AKS_HDR);
	    if (char_dialog (DO_IT_NO_QUERY) == JA) 
	    { /* Der meint's ernst */
	       err_state |= baks_to_aks ();
	       printf ("\n");
	       write_text_buf (CLEAR);
	    } /* Der meint's ernst */
	    else
	       printf ("%s\n", FKT_TERM_TXT);
	 break; /* BAKs-> AKs */

	 case 19: /* ---------------------- LoeAlleBAKs ---------------------- */
	    printf ("%s\n\n", DEL_BAKS_HDR);
	    if (char_dialog (DO_IT_NO_QUERY) == JA) 
	    { /* Der meint's ernst */
	       err_state |= delete_baks ();
	       write_text_buf (CLEAR);
	    } /* Der meint's ernst */
	    else
	       printf ("%s\n", FKT_TERM_TXT);
	 break; /* LoeAlleBAKs */
#endif /* WITH_B_CHRS */

	 case ESC:
	    fin = TRUE;
	 break;
      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);  /*Ausg.evtl.Meldungen + Loeschung*/
   } /* Menue-Loop */
   while (!fin);
} /* system_menue */
