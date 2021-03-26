/*******************************************************************************
GLEAM/AE                                                                  V1.1.8

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: SIMUlator C-MENue
 
Package: simu                   File: simucmen.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Der Modul enthaelt das Simulator-Menue des interaktiven Parts in der TEXT_MODE-
Variante. Bei Aufrufe der Routine "simulator" wird der "interaktiv"-Parameter 
auf TRUE oder "SIMU_SHOW" gesetzt. Die Menue-Routine wird vom Hauptmenue aus 
aufgerufen und aus ihr wird der "simulator" aktiviert. 

Bei Anwendungen, bei denen der Simulator mit den aus der AK extrahierten Parame-
tern betrieben wird (z.B. GADO oder MatLab) zeigt die Anzeige der Bewertung 
eines Simulatorlaufs auch die Parameter mit denen simuliert wurde, an. Sie wer-
den in gleicher Reihenfolge angezeigt, wie sie der Zuordnung zu den Feldern 
"i_params" und "r_params" entspricht (siehe Modul "simu").


Die Routinen im einzelnen:

void simu_cmenue (GSTATE *ret_state);
              Realisiert das "simu_menue" und die Ergebnisanzeige im interakti-
              ven Modus. Beim Aufruf des externen Simulators wird geprueft, ob 
              er verfuegbar ist ("sim_up" = TRUE). Wenn dies nicht der Fall ist,
              wird versucht, den Simulator mittels "ext_sim_hochfahren" aus 
              "eldo_koppl" betriebsbereit zu machen. Das Simu-Menue gestattet 
              auch die Anzeige der Ergebniswerte der besten AK seit Aufruf von 
              "init_simu". 
              "ret_state" gibt den (globalen) Fehlerstatus an. Bei "GLEAM_FATAL"
              sind einige Funktionen gesperrt.


Status der Renovierungsmassnahmen vom April/Mai 2016:
Die Menuefunktion "Restart ExtSimu / Beende ExtSimu" wurde nicht getestet.

--------------------------------------------------------------------------------
basiert auf C-Unit: simu
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Anzeige des kettenspeichers beim Starten einer Simulation.       WJ 21.12.98
 2. Ueberarbeitung der Menuesteuerung.                               WJ 22.12.98
 3. Anpassungen einiger Ausgaben an SMALL_SCREEN.                    WJ 08.08.00
 4. Anpassungen beim Aufruf von "zeige_simu_bew" u. "save_simu_erg". WJ 07.05.02
 5. Anpassungen bei der Teminierung des ExtSim bei DOS.              WJ 14.03.03
 6. Linux-Portierung: Differenzierung zwischen Solaris und Linux.    WJ 11.05.05
 7. Entfernung ueberfluessiger Variable.                             WJ 28.06.12
 8. Anpassungen des Aufrufs von "zeige_evo_best_bew" und
    "save_simu_erg". Streichung der lokalen Routine "bew_ausgabe".   WJ 05.12.14
 9. Renovierung: Streichung des "link"-Includes. Anpassung der Auf-
    rufe von: "c_prep_gnu_plot", "c_gnu_hard_copy", "char_dialog", 
    "lies_...", "set_and_clear_menue_window", "erfrage_outfile_spec", 
    "lies_adr" und "select_menue_item". Streichung der Aufrufe von: 
    "clr_line" und "write_line".                                     WJ 04.05.16
10. Kleine Reparatur bei der AK-Mem-Anzeige von "SimuStart".         WJ 28.06.16
11. Einfuehrung von "reset_krit_gueltig".                            WJ 25.08.16
12. Neuer Parameter fuer "show_and_clear_fbuf". Bei gesetztem 
    EXT_SIMU_SERV: "taste" wird beim Simulatorstart sicherheitshalber 
    zurueckgesetzt und neues Menu-Item zur Auswahl eines Simulations-
    modells aus der Modellliste der externen Simulationsdienste.     WJ 28.11.17
13. Streichung des Includes von "evo.h".                             WJ 09.01.18
14. EXT_MATLAB_SIMU: Neue Variable "extractCaseFile".                WJ 09.01.18
15. Fehler und keine Simulation, wenn mehr Bewertungskriterien 
    als Simulationsergebnisse spezifiziert wurden.                   WJ 14.02.19
16. Avoiding buffer overflow of static strings: simu_cmenue().       WJ 18.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 18.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "bew.h"
#include "hmod.h"
#include "men.h"
#ifdef EXT_SIMU_SERV
  #include "webIO.h"
  #include "aufg.h"
#endif /* EXT_SIMU_SERV */
#include "simu.h"
#include "simuInt.h"

#if defined(BS_SOLARIS) || defined(BS_LINUX)
  #include <unistd.h>
#endif /* Solaris oder Linux */

#include GLOBAL_TEXTS
#include SIMU_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ============================ Lokale Variable: ============================ */
static BOOL  sub_men_fin;            /* TRUE, wenn Simu-Menue zu beenden ist  */



/* ============================ Lokale Routinen: ============================ */
#ifdef GNUPLOT
  static void plot_simu_erg (void);
#endif




/* =============================== simu_menu ================================ */
void simu_cmenue (GSTATE *ret_state)
{
   STRING_TYPE          buffer;
   STR_TYPE             buf1, buf2;
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   INT                  guete, lfd_nr, men_item, 
                        znr, lines, anz, ii;
   BOOL                 try_it, simu_oben, file_neu,
                        clr_simu_flag, do_it, must_simu;
   char                 ch;

   /* simu_menu */
   reset_krit_gueltig ();                                  /* Initialisierung */
   sub_men_fin = FALSE;
   do 
   {
      set_and_clear_menue_window (2, SIMU_MEN_TXT);
      write_menue_item (1, StndMenItem, SIM_MEN_START_TXT);
      if (int_sim_data_ready) {
	 write_menue_item (2, StndMenItem, SIM_MEN_BEW_TXT);
	 write_menue_item (4, StndMenItem, SIM_MEN_RES_TXT);
      } else {
	 write_menue_item (2, DarkMenItem, SIM_MEN_BEW_TXT);
	 write_menue_item (4, DarkMenItem, SIM_MEN_RES_TXT);
      }
#ifdef GNUPLOT
      if (int_sim_data_ready || evo_plot_data_ready)
	 write_menue_item (3, StndMenItem, SIM_MEN_PLOT_TXT);
      else
	 write_menue_item (3, DarkMenItem, SIM_MEN_PLOT_TXT);
#else
      write_menue_item (3, DarkMenItem, SIM_MEN_PLOT_TXT);
#endif /* GNUPLOT */
      if (best_ges_note != -1)
	 write_menue_item (5, StndMenItem, SIM_MEN_EVO_BEST_TXT);
      else
	 write_menue_item (5, DarkMenItem, SIM_MEN_EVO_BEST_TXT);
      write_menue_item (7, StndMenItem, SIM_MEN_STATUS_TXT);
      if (mit_ext_simu)
      { /* Mit externem Simulator */
	 if (sim_ok && sim_up)
	    write_menue_item (8, StndMenItem, SIM_MEN_FIN_TXT);
	 else
	    write_menue_item (8, StndMenItem, SIM_MEN_RESTART_TXT);
      } /* Mit externem Simulator */
      else
	 write_menue_item (8, DarkMenItem, SIM_MEN_RESTART_TXT);
#ifdef MITSIMULOG
      write_menue_item (9, StndMenItem, SIM_MEN_LOG_TXT);
#else
      write_menue_item (9, DarkMenItem, SIM_MEN_LOG_TXT);
#endif /* MITSIMULOG */
#ifdef EXT_SIMU_SERV
      write_menue_item (10, StndMenItem, SIM_MEN_MODELLIST_TXT);
#endif /* EXT_SIMU_SERV */
#ifdef EXT_MATLAB_SIMU
      write_menue_item (10, StndMenItem, SIM_MEN_CASE_FILE_TXT);
#endif /* EXT_MATLAB_SIMU */
      write_menue_item (11, StndMenItem, SIM_MEN_DEL_SIMVAL_TXT);
      write_menue_item (12, StndMenItem, SIM_MEN_NEU_BEW_TXT);
      menue_completion ();
      men_item = select_menue_item ();
      reset_text_buf ();
      switch (men_item) 
      {
	 case 1: /* -------------------- Simulator-Start -------------------- */
	    if (akt_anz_krit > akt_anz_roh_erg_werte)
            { /* Abbruch */
	       fehler(SIMU_CMEN, 1, akt_anz_krit, akt_anz_roh_erg_werte, "", 
		      MORE_CRITERIA_THAN_RESULTS);
	       show_and_clear_fbuf(LOG_ALL_MSGS);
	       break;
	    } /* Abbruch */
	    if (*ret_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    printf ("%s\n", SIMU_HDR_TXT);
	    gklassen (AK, FALSE);                          /* "alles" = FALSE */
	    write_text_buf (CLEAR);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT))
	    { /* Adresse mit gueltiger Guete */
	       if (get_ak_and_check_ak (&must_simu, lfd_nr, guete))
	       { /* Kette ok */
		  adr_to_text (buf1, guete, lfd_nr, FALSE);
		  sprintf (buffer, SIMU_HDR2_TXT, buf1);
		  printf ("\n%s\n", buffer);
		  if (must_simu)
	          { /* es wird simuliert */
		     if (mit_ext_simu)
		     { /* check/init Simu u. Modell */
			printf ("\n%s\n", EXT_SIMU_STARTUP_TXT);
			reset_text_buf();
			simu_oben = ext_simu_hochfahren (FALSE);  
			show_and_clear_fbuf(LIMITED_LOGGING); /* FMldgAusgabe */
			write_text_buf(CLEAR);     /* Ausgabe des Textpuffers */
#ifdef EXT_SIMU_SERV
			taste = FALSE;     /* to ensure that "taste" is reset */
#endif /* EXT_SIMU_SERV */
		     } /* check/init Simu u. Modell */
		     else
			simu_oben = TRUE; /* interner Simulator ist immer oben*/
		  } /* es wird simuliert */
		  else
		     simu_oben = TRUE;       /* wir simulieren nicht wirklich */
		  lines = get_line_anz ();
		  if (lines > 0)
	          { /* SimuHochfahrenMeldung ausgeben */
		     for (ii = 1;  ii <= lines;  ii++) {
			get_buf (ii, buffer);
			printf ("%s\n", buffer);
		     }
		     reset_text_buf ();
		  } /* SimuHochfahrenMeldung ausgeben */
		  perform_simu (simu_oben, 1);
		  write_text_buf (CLEAR);
	       } /* Kette ok */
	       else
		  printf ("%s\n", NIX_CH);
	    } /* Adresse mit gueltiger Guete */
	 break;  /* Simulator-Start */

	 case 2: /* -------------------- Zeige Bewertung -------------------- */
	    zeige_simu_bew (1, TRUE);                      /* mitIndex = TRUE */
	    write_text_buf (CLEAR);
	 break;  /* Zeige Bewertung */

#ifdef GNUPLOT
	 case 3: /* -------------------- Plot Bewertung --------------------- */
            if (int_sim_data_ready || evo_plot_data_ready)
	    { /* Plot eines Kriteriums */
	       if (evo_plot_data_ready)
		  printf ("%s\n", EVO_PLOT_HEADER_TXT);
	       else
		  printf ("%s\n", SIM_PLOT_HEADER_TXT);
               plot_simu_erg ();
	    } /* Plot eines Kriteriums */
	    else
	       printf ("%s\n", NIX_PLOT_DATA_TXT);
	    printf ("\n");
	 break;  /* Plot Bewertung */
#endif /* GNUPLOT */

	 case 4: /* --------------------- Rette SimErg  --------------------- */
	    printf ("\n%s\n", SAVE_ERG_HDR);
	    strcpy (buf2, "res");
	    ii = strlen (exp_file_spec) - 4;
	    if (ii > 0)
	    { /* es gibt eine EXP-File-Spec */
	       strcpy (buffer, exp_file_spec);
	       buffer[ii] = '\0';                   /* cut off file extension */
	    } /* es gibt eine EXP-File-Spec */
	    else
	       strcpy (buffer, "");
	    printf ("\n");
	    file_neu = FALSE;                     /* File muss nicht neu sein */
	    if (erfrage_outfile_spec (&file_neu, buf2, buffer, FKT_TERM_TXT))
	    { /* Der meint's ernst */
	       if (save_simu_erg (buf2, file_neu, FALSE)) /* mitIndex = FALSE */
		  printf ("%s\n", SAVE_OK_TXT);                 /* Rettung ok */
	       else
		  printf ("%s\n", SAVE_FEHL_TXT);
	    } /* Der meint's ernst */
	 break;  /* Rette SimErg */

	 case 5: /* -------------------- Zeig EvoBestBew -------------------- */
	    zeige_evo_best_bew (1, TRUE);                  /* mitIndex = TRUE */
	    write_text_buf (CLEAR);
	 break;  /* Zeig EvoBestBew */

	 case 7: /* -------------------- Zeige Status ----------------------- */
            prep_status_anzg ();
	    write_text_buf (CLEAR);
	 break;  /* Zeige Status */

	 case 8: /* ------------ Restart ExtSimu / Beende ExtSimu ----------- */
	    if (*ret_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    reset_text_buf();
#if defined(BS_SOLARIS) || defined(BS_LINUX)
	    if (sim_ok && sim_up)
	    { /* Beende ExtSimu */
	       printf ("\n%s\n", EXT_SIMU_FIN_HDR_TXT);
	       if (char_dialog (EXT_SIMU_FIN_QUERY_TXT) == JA)
		  terminate_ext_simu ();    /* externen Simulator terminieren */
	    } /* Beende ExtSimu */
	    else
	    { /* Restart ExtSimu */
	       printf ("\n%s\n", EXT_SIMU_RESTART_TXT);
	       if (sim_ok)
	       { /* abgeforkter Simu-Prozess laeuft noch */
	          if (sim_up)
	          { /* Wirklich re-starten ? */
		     printf ("\n%s\n", EXT_SIMU_STATUS_TXT);
		     ch = char_dialog (SIMU_RESTART_QUERY_TXT);
		     try_it = ch == JA;
		  } /* Wirklich re-starten ? */
		  else
		     try_it = TRUE;
		  if (try_it)
	          { /* der meint's ernst */
  #if defined(EXT_SIMU_SERV) || defined(EXT_MATLAB_SIMU)
		     ext_simu_hochfahren (FALSE);
  #else /* neither EXT_SIMU_SERV nor EXT_MATLAB_SIMU */
		     ch = char_dialog (EXT_SIMU_RESTART_QUERY);
		     if (ch == EXT_SIMU_KL_RESTART_CH)
			kleiner_ext_restart ();
		     else 
			if (ch == EXT_SIMU_GR_RESTART_CH)
			   restart_ext_sim ();
			else
			   if (ch == EXT_SIMU_RECOVERY_CH)
			      ext_simu_hochfahren (TRUE); /* Kleines Recovery */
			   else
			      printf ("%s\n", EXT_SIMU_NIX_TUN_TXT);
  #endif /* no EXT_SIMU_SERV */
		  } /* der meint's ernst */
	       } /* abgeforkter Simu-Prozess laeuft noch */
	       else
	       { /* Komplett Start */
		  printf ("\n%s\n", EXT_SIMU_DO_RESTRT_TXT);
		  sim_up = FALSE;
		  ext_simu_hochfahren (TRUE);
	       } /* Komplett Start */
               znr = get_line_anz () + 1;
	       if (sim_up)
		  printf("\n%s\n", EXT_SIMU_RESTART_DONE);
	       else
		  write_buf (znr, NIX_EXT_SIMU_TXT);
	    } /* Restart ExtSimu */
#else /* kein Solaris oder Linux */
	    if (mit_ext_simu)
	    { /* Es gibt einen ExtSimu */
	       if (sim_ok && sim_up)
	       { /* Beende ExtSimu */
		  printf ("\n%s\n", EXT_SIMU_FIN_HDR_TXT);
		  if (char_dialog (EXT_SIMU_FIN_QUERY_TXT) == JA)
		     terminate_ext_simu ();    /* externen Simulator terminieren */
	       } /* Beende ExtSimu */
	       else
	       { /* Restart ExtSimu */
		  printf ("\n%s\n", EXT_SIMU_RESTART_TXT);
		  ext_simu_hochfahren (TRUE);
	       } /* Restart ExtSimu */
	       znr = get_line_anz () + 1;
	       if (sim_up)
		  write_buf (znr, EXT_SIMU_RESTART_DONE);
	       else
		  write_buf (znr, NIX_EXT_SIMU_TXT);
	    } /* Es gibt einen ExtSimu */
	    else
	       printf ("\n%s\n", NIX_EXT_SIMU_TXT);
#endif /* kein Solaris oder Linux */
	    write_text_buf(CLEAR);                 /* Ausgabe des Textpuffers */
	    printf("\n");
	 break; /* Restart ExtSimu / Beende ExtSimu */

#ifdef MITSIMULOG
	 case 9: /* -------------------- SimuLog. an/aus -------------------- */
	    if (mit_simu_logfile)
	    { /* Simu-Logfile steht zur Verfuegung */
	       if (mit_simulog)
		  sprintf (buf1, "%s%s", SIMU_LOGFILE_QUERY_TXT, JA_NEIN_TXT);
	       else
		  sprintf (buf1, "%s%s", SIMU_LOGFILE_QUERY_TXT, NEIN_JA_TXT);
	       ch = char_dialog (buf1);
	       if ((ch != CR) && (ch != ESC))
		  mit_simulog = ch == JA;
	    } /* Simu-Logfile steht zur Verfuegung */
	    else
	       printf ("%s\n", NIX_LOG_TXT);
	 break;  /* SimuLog. an/aus */
#endif /* MITSIMULOG */

#ifdef EXT_SIMU_SERV
	 case 10: /* --------------------- ModellAusw: --------------------- */
	    printf ("\n%s\n", EXTSIM_MODLIST_HDR_TXT);
	    if (sim_ok) 
	    { /* communication to external simlation services available */
	       if (taskID == NIX_IDX)
		  getTaskID();                      /* sets taskID on success */
	       if (taskID != NIX_IDX)
	       { /* taskID available */
		  anz = getModelList(NUMBERED_MODEL_LIST);
		  if (anz > 0)
	          { /* display list and select a model */
		     write_text_buf(KEEP);
		     if (anz == 1)
			ii = 1;
		     else
	             { /* select item from list */
			sprintf(buf1, EXTSIM_MOD_AUSW_TXT, anz);
			if (lies_wert(&ii, buf1,  1, anz, anz+1)) {
			   printf(MSG_LINE_TXT, FKT_TERM_TXT);
			   ii = 0;
			}
		     } /* select item from list */
		     if (ii > 0)
                     { /* item selected */
			get_buf(ii, buf1);
			strcpy(extSimuModelName, &(buf1[6]));
			reset_text_buf();
			if (sim_up)
			   ext_simu_hochfahren(TRUE);  /*TRUE: with previous termination*/
			else
			{ /* no active workflow of "old" model */
			   if (startModel(extSimuModelName))
	                   { /* workflow of selected model is ready */
			      sim_up = TRUE;
			      strcpy(lastSimuModelName, extSimuModelName);   /* store model name */
			   } /* workflow of selected model is ready */
			   else
			   { /* error starting workflow of selected model */
			      sim_up = FALSE;
			      strcpy(lastSimuModelName, "");        /* clear "lastSimuModelName" */
			   } /* error starting workflow of selected model */
			   reportResultOfModelStart(buffer);
			   write_buf(get_line_anz()+1, buffer);
			} /* no active workflow of "old" model */
			if (sim_up)
			   setStringVal(extSimuModelName, SIMU_MOD_NAME_IDX);
			printf("\n");
			write_text_buf(CLEAR);
		     } /* item selected */
		  } /* display list and select a model */
		  reset_text_buf();
		  printf("\n");
	       } /* taskID available */
	       else
		  printf("%s.\n", EXTSIM_NO_COM_TXT);
	    } /* communication to external simlation services available */
	    else
	       printf ("%s!\n", EXTSIM_NO_COM_TXT);
	 break;  /* ModellAusw */
#endif /* EXT_SIMU_SERV */

#ifdef EXT_MATLAB_SIMU
	 case 10: /* ---------------------- case-File: ---------------------- */
	    printf ("\n%s\n", MATLAB_CASEF_HDR_TXT);
	    if (sim_ok && mit_ext_simu) 
	    { /* communication to MATLAB available */
	       ch = char_dialog (MATLAB_CASEF_QUERY_TXT);
	       if (ch != ESC)
		  extractCaseFile = ch == JA;
	       if (extractCaseFile)
		  printf(MATLAB_CASEF_ON_TXT);
	    } /* communication to MATLAB available */
	    else
	       printf ("%s!\n", NIX_EXT_SIMU_TXT);
	 break;  /* case-File */
#endif /* EXT_MATLAB_SIMU */

	 case 11: /* --------------------- LoeSimWerte: --------------------- */
	    if (*ret_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    printf ("\n%s\n", AK_SIM_LOE_HDR_TXT);
	    if (lies_adr (&guete, &lfd_nr, AK_QUERY_TXT))
	    { /* AK-Adresse OK */
	       if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, ret_state))
	       { /* Die Kette gibt es */
		  ak_ptr->u_el.u_xel.u_hdr.chain_status &= ~SIMULATED;
		  printf ("%s\n", SIM_VAL_DEL_TXT);
	       } /* Die Kette gibt es */
	       else
		  printf (FMELD_LINE_TXT, NIX_CH);
	    } /* AK-Adresse OK */
	 break;  /* LoeSimWerte */

	 case 12: /* ---------------------- NeuBewert: ---------------------- */
	    if (*ret_state == GLEAM_FATAL)
	    { /* Abbruch */
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;
	    } /* Abbruch */
	    if (akt_anz_krit > akt_anz_roh_erg_werte)
            { /* Abbruch */
	       fehler(SIMU_CMEN, 2, akt_anz_krit, akt_anz_roh_erg_werte, "", 
		      MORE_CRITERIA_THAN_RESULTS);
	       show_and_clear_fbuf(LOG_ALL_MSGS);
	       break;
	    } /* Abbruch */
	    printf ("\n%s\n", NEUBEWERT_HDR1);
	    printf ("%s\n", NEUBEWERT_HDR2);
	    ch = char_dialog (NEUBEW_SIM_LOE_QUERY);
	    clr_simu_flag = ((ch == CR) || (ch == JA));
	    if (ch == ESC)
	       break;                               /* Abbruch durch Benutzer */
	    if (clr_simu_flag && mit_ext_simu)
	    { /* externe Simulation */
	       printf ("\n%s\n", EXT_SIMU_STARTUP_TXT);
	       do_it = ext_simu_hochfahren (FALSE); /* check/init Modell u. Simu */
	       show_and_clear_fbuf(LIMITED_LOGGING); /* MeldgAusgabe + Loeschung */
	    } /* externe Simulation */
	    else
	       do_it = TRUE;
	    if (do_it)
	    { /* Ext. Simulator verfuegbar oder nicht benoetigt */
	       sprintf (buf1, NEUBEW_TO_DO_TXT,
			number_of_chains (1, MAX_GUETE, AK, ret_state));
	       printf ("\n%s\n", buf1);
	       *ret_state    |= ak_mem_neu_bewert (clr_simu_flag);
	       write_text_buf (CLEAR);                        /* "Puffer-Ausgabe */
	    } /* Ext. Simulator verfuegbar oder nicht benoetigt */
	    else
	       printf ("%s\n", NEUBEW_NIX_DONE_TXT);
	 break;  /* NeuBewert */

	 case ESC:
	    sub_men_fin = TRUE;
	 break;

      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);  /*Ausg.evtl.Meldungen + Loeschung*/
   } while (!sub_men_fin);
#ifdef GNUPLOT
   kill_all_plots ();
#endif
#ifdef EXT_MATLAB_SIMU
   extractCaseFile = FALSE;
#endif /* EXT_MATLAB_SIMU */
} /* simu_menu */



#ifdef GNUPLOT
/* ============================= plot_simu_erg ============================== */
static void plot_simu_erg ()
/*----------------------------------------------------------------------------*/
/*   Erfraegt den Index eines Kriteriums, erzeugt mt Hilfe von "c_prep_gnu_   */
/*   plot" einen Plot fuer dieses Kriterium und zeichnet mit Hilfe der Routi- */
/*   ne "plot_akt_wert" den Wert der letzten Simulation ein. Dabei wird ent-  */
/*   schieden, ob die Bewertungs- oder die Straffunktion anzuzeigen ist. Wenn */
/*   alles gut ging, kann abschliessend mit Hilfe von "gnu_hard_copy" ein     */
/*   Postscriptfile des Plots erzeugt werden.                                 */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  qbuf;
   INT       kk, offset;
   ssize_t   dummy;

   /* plot_simu_erg */
   if (last_krit == NIX_IDX)
   {
      if (lies_wert (&kk, KRIT_IND_QUERY_TXT, 1, akt_anz_krit, max_anz_kriterien+1))
	 return;                                    /* Abbruch durch Benutzer */
   }
   else
   {
      sprintf (qbuf, "%s%d]: ", KRIT_IND_QUERY2_TXT, last_krit);
      if (lies_wert (&kk, qbuf, 1, akt_anz_krit, last_krit))
	 return;                                    /* Abbruch durch Benutzer */
   }
   if (int_simValOutOfEvalRange[kk - 1])
      offset = max_anz_bew_krit;                  /* Zeige Straffunkttion     */
   else
      offset = 0;                                 /* Zeige Bewertungsfunktion */
   if (c_prep_gnu_plot (kk + offset, kk, SHOW_SIM_RES))
   { /* GnuPlot verfuegbar */
      plot_akt_wert (kk + offset);
      dummy = write (pipe_p2g[last_gnu_index][1], gnu_buffer, 
		     strlen (gnu_buffer));           /* returns -1, if failed */
      if (char_dialog (HARDCOPY_QUERY_TXT) == JA)
	 c_gnu_hard_copy ();
   } /* GnuPlot verfuegbar */
} /* plot_simu_erg */
#endif /* GNUPLOT */
