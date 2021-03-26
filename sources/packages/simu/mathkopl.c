/*******************************************************************************
GLEAM/AE                                                                  V1.2.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: MATHematica-KOPpLung
 
Package: simu                   File: mathkopl.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

Der Modult enthaelt die Anbindung an den externen "Mathematica"-Simulator.

Durch das "aufg"-Package wird unter SIMU_RUNS_PER_INIT_TXT in "tsk_ints" eine 
Grenze der Simulatorlaeufe pro Init vorgegeben. Bei Erreichen dieser Grenze 
initialisiert "do_gen_sim" den ext. Simulator neu. Dies ist z.B. dann erforder-
lich, wenn der ext. Simulator ein memory leak hat.

Die Routinen im einzelnen:

BOOL start_math_sim (char *mod_name);
          Die Routine setzt voraus, dass der externe Simulator mit Hilfe von
          "init_sim" (Modul "ext_koppl" in Package "sys") erfolgreich ("sim_ok" 
          ist TRUE) gestartet wurde.
          In diesem Falle erzeugt sie eine Startmeldung mit Uhrzeit und mit dem 
          "mod_name", sofern "mod_name" nicht leer ist. Die Meldung wird ueber 
	  den Textpuffer des Packages "fbhm" ausgegeben. Dann wartet die Routine
	  auf die Ausgaben des Simulators. Nach maximal START_SKIPS Zeilen muss 
          der Simulator mit einer Zeile antworten, die dem in INIT_OK_STRING 
          hinterlegten Text entspricht. Danach wird auf das erste Prompt ge-
          wartet. Wenn beides erfolgreich erhalten wurde, liefert die Routine
	  TRUE, sonst FALSE.

BOOL restart_math_sim (void);
          Es wird versucht, den externen Simulator durch Abwuergen des Child-
          Prozesses zu terminieren und neu zu starten; ihn also in einen Zu-
          stand zu bringen, wie er nach "start_math_sim" herrscht. 
          Wenn das glueckt, liefert die Routine TRUE sonst FALSE und "sim_up" 
          wird entsprechend gesetzt.

BOOL kleiner_math_restart (void);
          Nach Entgegennahme eventueller (Fehler-)Meldungen wird halb so lang  
          wie beim Simulatorstart auf den Erhalt des Simulator-Prompts gewartet. 
          Bei Ausbleiben eines Prompts wird mit kurzer Wartezeit versucht, durch 
          Senden von CR einen beliebigen Prompt zu erhalten ("check_for_prompt").
          Wenn der Simulator den Prompt ohne Fehlermeldung liefert, wird "sim_
          "up" TRUE, sonst FALSE. 
          Als Funktionsergebnis wird "sim_up" abgeliefert.

BOOL do_math_sim (DOUBLE *erg_werte,
                  INT    *i_params,
                  DOUBLE *r_params);
          Die Routine fuehrt einen Simulationslauf aus und liefert TRUE, wenn in
          "erg_werte" gueltige Werte stehen, d.h., die Simulation erfolgreich 
          durchgefuehrt werden konnte. 
          Die Eingangs- ("i_params" und "r_params") und Ausgangsparameter 
          ("erg_werte") sind bei der Routine "do_ext_sim" im Modul "ext_sim" be-
          schrieben.
	  Wenn die Routine FALSE liefert, kann die globale Variable "sim_up" 
          FALSE sein, sofern der Simulator sich nicht mehr mit "restart_math_
          sim" starten liess. Bei geloeschtem "sim_up" terminiert die Routine 
          sofort und liefert FALSE.

BOOL terminate_math_sim (BOOL  mit_quit);
          Bei gesetztem "mit_quit" wird der Simulator mit "Quit" terminiert u. 3
          mal maximal "RESTART_WAITS" Sekunden auf die Schlussmeldung gewartet.
          Wenn keine Schlussmeldung erhalten wurde oder "mit_quit" FALSE ist, 
          wird Mathematica per "kill_process" abgewuergt.
          Schliesst die Pipe-Streams. Liefert TRUE, wenn die Schlussmeldung er-
          halten wurde oder "kill_process" erfolgreich war.

BOOL make_math_startsim (void);
          Erzeugt unter Benutzung von "make_startsim" ("sys"-Package) das Mathe-
	  matica-spezifische "startsim"-File und liefert TRUE ab, wenn das File 
          angelegt werden konnte.

BOOL check_math_err_meld (void);
          Bestimmte Meldungen aus der Error-Pipe gelten nicht als Fehlermel-
          dungen. 

BOOL till_math_prompt (LONG  wcycles,
                       INT   max_skip,
                       BOOL  ignore_errors);
          Die Routine liest solange Zeilen (mit "wcycles" Versuchen) vom Simu-
          lator ein, bis entweder "max_skip" Zeilen oder eine Prompt-Zeile ge-
          lesen wurde(n). Wenn die Nummer im Prompt "prompt_ctr" entspricht, 
          liefert die Routine TRUE, sonst immer FALSE. Wenn "prompt_ctr" == -1 
          ist, wird die gelesene Prompt-Nr immer akzeptiert. Bei gesetztem 
          "ignore_errors" werden auch Fehlermeldungen ignoriert.
          Form des Prompts: In[n]:=, wobei n eine Integerzahl ist.

void set_math_data_waits (void);

BOOL init_math_koppl (void);
          Modulinitialisierung. Liefert z.Zt. immer TRUE.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Update von Mathematica Version 2.2 auf 3.0.1
 2. Statt "/opt/bin/tcsh" wird die "/usr/bin/csh" in "startsim" aufgerufen.
 3. Anpassungen an neue TSK-Datenhaltung.                            WJ 25.04.02
 4. "get_id_idx"-Aufrufe derart, dass Fehler gemeldet werden.        WJ 27.11.03
 5. Linux-Portierung: System-Includefile sys/conf.h nur für Solaris. WJ 12.05.05
 6. Erweiterung fuer die MatLab-Koppung: Streichung von GAMA.        WJ 22.08.07
 7. Entfernung von Code im Kontext der Schalter: BGI_MODE und 
    GLEAM_TCL. Streichung von GLEAM_C. Entfernung ueberfluessiger 
    Variable. Korrektur von Formatangaben. Vermeidung uninitiali-
    sierter Variable                                                 WJ 28.06.12
 8. Renovierung: Streichung des "link"-Includes. Anpassung der 
    "write_line"-Aufrufe.                                            WJ 22.04.16
 9. Anpassungen an die neue TSK-Datenverwaltung.                     WJ 21.11.16
10. Ersetzung von "akt_roh_erg_werte" durch "akt_anz_roh_erg_werte". WJ 29.05.17
11. Anpassungen an das neue Logging der Simulatorschnittstelle.      WJ 13.09.17
12. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
13. Include von "lgsw.h" anstelle von "chaindef.h".                  WJ 08.07.19
14. start_math_sim(): Avoiding buffer overflow of static strings.    WJ 18.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 18.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef BS_SOLARIS
  #include <sys/conf.h>
#endif /* Solaris */
#include <sys/stat.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "hmod.h"
#include "aufg.h"
#include "simuInt.h"
#include "simu.h"

#include GLOBAL_TEXTS
#include SIMU_TEXTS




/* ============================ Lokale Konstante: =========================== */
#define MATH_PACKAGE         "math3.0.1"
#define INIT_OK_STRING       " -- "
#define INIT_OK_STRING_LEN   4
#define NO_LICENCE_STRING    "General::pwnolic"    /* Mathematica Version 2.2 */
#define NO_LICENCE_STR_LEN   16
#define SIM_FIN_MELD         "Terminated"
#define SIM_FIN_MELD_LEN     10
#define MATH_WUERG_NAME      "MathKern"
#define SIMU_ABORTED_MELD    "SimuAborted"
#define START_SKIPS           5
#define ERR_SKIPS            15
#define KL_RESTART_SKIPS    200 

#ifdef ONLINE
#define START_IGN_WAITS       20 sec
#define INIT_PROMPT_WAITS      2 sec
#define SIMU_FIRST_WAITS      30 sec
#define SIMU_GADO_WAITS        1 sec
#define SKIP_ERR_WAITS      (0.5 sec)
#define TERM_WAITS             2 sec
#else /* ONLINE */
#define START_IGN_WAITS        3 
#define INIT_PROMPT_WAITS      2
#define SIMU_FIRST_WAITS       4
#define SIMU_GADO_WAITS        2
#define SKIP_ERR_WAITS         1
#define TERM_WAITS             2
#endif /* ONLINE */

#ifdef BRIEF_LOG
#define brief_log             TRUE
#else
#define brief_log             FALSE
#endif /* BRIEF_LOG */



/* ============================ Lokale Variable: ============================ */
static INT     simu_data_waits,   /* Wartezyklen nach 1.SimuDaten bis next    */
               run_ctr;           /* Zaehlt die Laeufe seit dem letzten Init. */
static BOOL    restart,           /* True, wenn Restart bei Simu-Fehlern      */
               simu_wdh;          /* True, wenn Simu zu wiederholen ist       */



/* ============================ Lokale Routinen: ============================ */
static BOOL start_simu_lauf      (INT    *i_params,
                                  DOUBLE *r_params);

static void simu_lauf_auswertung (DOUBLE *erg_werte);





/* ============================= start_math_sim ============================= */
BOOL start_math_sim (char *mod_name)
{
   STRING_TYPE  buffer;
   STR_TYPE     mbuf, dummy_buf;
   BUF_TYPE     zbuf;
   INT          ii;
   BOOL         tmo, ok, weiter;

   /* start_math_sim */
   sim_up = FALSE;
   if (sim_ok)
   { /* "mathematica" offenbar erfolgreich gestartet */
      get_time_stamp (dummy_buf, dummy_buf, dummy_buf, zbuf);
      if ((INT)(strlen (mod_name)) > 0)
	 sprintf (mbuf, SIMUSTART_TXT2, mod_name);
      else
	 strcpy (mbuf, "");
      sprintf (buffer, SIMUSTART_TXT1, zbuf, mbuf);
      write_buf (1, buffer);
      weiter = TRUE;
      ii     = 0;
      do
      { /* Warten auf Init-String */
	 if (ok = wait_for_reply (&tmo, START_IGN_WAITS))
	 { /* Zeile erhalten: analysieren */
	    if (strncmp (inbuffer, INIT_OK_STRING, INIT_OK_STRING_LEN) == 0)
	    { /* Init-String erkannt */
	       sim_up = TRUE;
	       weiter = FALSE;
	    } /* Init-String erkannt */
	    else                                   /* Mathematica Version 2.2 */
	       if (strncmp (inbuffer, NO_LICENCE_STRING, NO_LICENCE_STR_LEN) 
		   == 0)
	       { /* Keine Lizenz mehr verfügbar */
		  weiter = FALSE;
		  write_buf (get_line_anz()+1, NO_LICENCE_TXT);
	       } /* Keine Lizenz mehr verfügbar */
	 } /* Zeile erhalten: analysieren */
	 ii++;
	 if (!sim_up && (ii == 2) && !ok)        /* Mathematica Version 3.0.1 */
	 { /* Keine Lizenz mehr verfügbar */
	    weiter = FALSE;
	    write_buf (get_line_anz()+1, NO_LICENCE_TXT);
	 } /* Keine Lizenz mehr verfügbar */
      } /* Warten auf Init-String */
      while (weiter && ok && (ii < START_SKIPS));
      if (sim_up)
      { /* Init-String nach ignorierten Zeilen erhalten */
	 prompt_ctr = -1;                        /* akzeptiere jede Prompt-Nr */
	 sim_up     = till_math_prompt (INIT_PROMPT_WAITS, 5, STOP_ON_ERROR);
	 prompt_ctr++;
      } /* Init-String nach ignorierten Zeilen erhalten */
   } /* "mathematica" offenbar erfolgreich gestartet */
   run_ctr = 0;
   return (sim_up);
} /* start_math_sim */



/* ============================ restart_math_sim ============================= */
BOOL restart_math_sim (void)
{  

   /* restart_math_sim */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "%s RESTART_MATH_SIM %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   sim_up = FALSE;
#ifdef ONLINE
   if (kill_process (MATH_WUERG_NAME))
   { /* Mathematica abgewuergt */
      init_sim ();                              /* Mathematica erneut starten */
#ifdef IO_LOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "RESTART_MATH_SIM: Termination Done. "
		  "INIT_SIM: sim_ok =%d", sim_ok);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* IO_LOG */
      start_math_sim ("");
   } /* Mathematica abgewuergt */
#else /* ONLINE */
   tsim  = fopen ("tsim.txt", "w");
   fsim  = fopen ("fsim.txt", "r");
   efsim = fopen ("efsim.txt", "r");
   sim_ok = (fsim != NULL) && (efsim != NULL);
   sim_up = sim_ok;
#endif /* ONLINE */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "RESTART_MATH_SIM: sim_up=%d", sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   return (sim_up);
} /* restart_math_sim */



/* ========================== kleiner_math_restart ========================== */
BOOL kleiner_math_restart (void)
{
   /* kleiner_math_restart */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "%s KLEINER_MATH_RESTART %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   sim_up = FALSE;
   if (till_math_prompt((START_IGN_WAITS)/2, KL_RESTART_SKIPS, IGNORE_ERRORS))
   { /* Prompt erhalten */
      prompt_ctr++;
      sim_up = TRUE;
   } /* Prompt erhalten */
   else
      if (check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, EXPECT_NEW_PROMPT))
      { /* Prompt erhalten */
	 prompt_ctr++;
	 sim_up = TRUE;
      } /* Prompt erhalten */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "KLEINER_MATH_RESTART: sim_up=%d", sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   return (sim_up);
} /* kleiner_math_restart */



/* ============================== do_math_sim =============================== */
BOOL do_math_sim (DOUBLE *erg_werte,
                  INT    *i_params,
                  DOUBLE *r_params)
{
   STR_TYPE  zeit_buf;
   DOUBLE    simu_secs = 0.0;         /* Simu-Laufzeit in Sekunden            */
   INT       run_ctr_limit;
   INT       ii;

   /* do_math_sim */
#ifdef TEST_SIM
   ak_bewertet = TRUE;
   erg_werte[0] = frand() * 3000;
   erg_werte[1] = erg_werte[0];
   erg_werte[2] = frand() * 20;
   erg_werte[3] = erg_werte[2]; 
   erg_werte[4] = frand(); 
   printf (MSG_LINE_TXT, "Achtung: MATH-Koppl im Testmodus! KEIN Aufruf von mathematica!");
#else  /* --------------- Ankopplung des externen Simulators: --------------- */
   ak_bewertet = FALSE;
   if (!getIntVal(&run_ctr_limit, SIMU_RUNS_PER_INIT_IDX, GLEAM_OK))   /* xxyza */
      run_ctr_limit = 10000000;  /*No SIMU_RUNS_PER_INIT_TXT-Eintr.in TSK-File*/
   if (run_ctr > run_ctr_limit)
   { /* Simulator reinitialisieren */
      if (sim_up)
	 terminate_math_sim (TRUE);
      restart_math_sim ();
   } /* Simulator reinitialisieren */
   if (sim_up)
   { /* Simulator verfuegbar */
      restart   = FALSE;
      simu_wdh  = TRUE;
      simu_secs = 0.0;
      wait_ctr  = 0;
      run_ctr++;
      if (start_simu_lauf (i_params, r_params))
	 simu_lauf_auswertung (erg_werte);  /* Antw.i.d.Zeit ohne ErrPipeMldg */
      else
	 restart = TRUE;
      if (mit_rz)
      { /* Rechenzeitermittlung */
	 simu_secs = wait_ctr;
	 simu_secs = (simu_secs * WAIT_FOR_READ) / 1000;
	 sprintf (zeit_buf, SIMU_RZ_TXT, simu_secs);
	 printf (MSG_LINE_TXT, zeit_buf);
      } /* Rechenzeitermittlung */
#ifdef MITSIMULOG
      if (mit_simulog && (!brief_log || (brief_log && restart)))
      { /* Logging */
	 if (!simu_logfile_open)
	    simu_logfile = fopen(simu_logfilename, "at");
	 if (simu_logfile != NULL)
	 { /* logfile is open */
	    fprintf (simu_logfile, "Simu 1.Lauf: RZ=%6.2f  ak_bewert=%d  restart="
		     "%d  simu_wdh=%d  sim_up=%d\n", simu_secs, ak_bewertet, 
		     restart, simu_wdh, sim_up);
	    if (restart || !brief_log
#ifdef DETAILED_LOG
		|| ak_bewertet
#endif
		)
	    { /* Eingangs-Parameter ausgeben */
	       fprintf (simu_logfile, "             Params:");
	       for (ii = 0;  ii < i_par_ges_anz;  ii++)
		  fprintf (simu_logfile, " %d", i_params[ii]);
	       for (ii = 0;  ii < r_par_ges_anz;  ii++)
		  fprintf (simu_logfile, " %E", r_params[ii]);
	       fprintf (simu_logfile, "\n");
	    } /* Eingangs-Parameter ausgeben */
	    if (ak_bewertet)
	    { /* ausgeben */
	    fprintf (simu_logfile, "             Ergebn:");
	    for (ii = 0;  ii < akt_anz_roh_erg_werte;  ii++)
	       fprintf (simu_logfile, " %E", erg_werte[ii]);
	    fprintf (simu_logfile, "\n");
	    } /* Ergebniswerte ausgeben */
	    fclose(simu_logfile);
	    simu_logfile_open = FALSE;
	 } /* logfile is open */
	 else
	    meldung(SIMU_MATH_KOPPL, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
      } /* Logging */
#endif /* MITSIMULOG */

      /* --------------------  Restart und 2.Simu-Lauf: --------------------- */
      if (restart)
      { /* restart external simulator */
	 if (!kleiner_math_restart ())
	    restart_math_sim ();
      } /* restart external simulator */

      if (sim_up && simu_wdh)
      { /* Simulator (wieder) bereit: Lauf wiederholen */
	 restart   = FALSE;
	 simu_wdh  = FALSE;
	 simu_secs = 0.0;
	 wait_ctr  = 0;
	 if (start_simu_lauf (i_params, r_params))
	    simu_lauf_auswertung (erg_werte); /*Antw.i.d.Zeit ohne ErrPipeMldg*/
	 else
	    restart = TRUE;
	 if (mit_rz)
	 { /* Rechenzeitermittlung */
	    simu_secs = wait_ctr;
	    simu_secs = (simu_secs * WAIT_FOR_READ) / 1000;
	    sprintf (zeit_buf, SIMU_RZ_TXT, simu_secs);
	    printf (MSG_LINE_TXT, zeit_buf);
	 } /* Rechenzeitermittlung */
#ifdef MITSIMULOG
	 if (mit_simulog && (!brief_log || (brief_log && restart)))
	 { /* Logging */
	    if (!simu_logfile_open)
	       simu_logfile = fopen(simu_logfilename, "at");
	    if (simu_logfile != NULL)
	    { /* logfile is open */
	       fprintf (simu_logfile, "Simu 2.Lauf: RZ=%6.2f  ak_bewert=%d  "
			"restart=%d  simu_wdh=%d  sim_up=%d\n", simu_secs, 
			ak_bewertet, restart, simu_wdh, sim_up);
	       if (restart || !brief_log
#ifdef DETAILED_LOG
		   || ak_bewertet
#endif
		   )
	       { /* Eingangs-Parameter ausgeben */
		  fprintf (simu_logfile, "             Params:");
		  for (ii = 0;  ii < i_par_ges_anz;  ii++)
		     fprintf (simu_logfile, " %d", i_params[ii]);
		  for (ii = 0;  ii < r_par_ges_anz;  ii++)
		     fprintf (simu_logfile, " %E", r_params[ii]);
		  fprintf (simu_logfile, "\n");
	       } /* Eingangs-Parameter ausgeben */
	       if (ak_bewertet)
	       { /* ausgeben */
		  fprintf (simu_logfile, "             Ergebn:");
		  for (ii = 0;  ii < akt_anz_roh_erg_werte;  ii++)
		     fprintf (simu_logfile, " %E", erg_werte[ii]);
		  fprintf (simu_logfile, "\n");
	       } /* Ergebniswerte ausgeben */
	       fclose(simu_logfile);
	       simu_logfile_open = FALSE;
	    } /* logfile is open */
	    else
	       meldung(SIMU_MATH_KOPPL, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
	 } /* Logging */
#endif /* MITSIMULOG */
	 if (restart)
	 { /* restart external simulator */
	    if (!kleiner_math_restart ())
	       restart_math_sim ();
	 } /* restart external simulator */
      } /* Simulator (wieder) bereit: Lauf wiederholen */
   } /* Simulator verfuegbar */
#ifdef MITSIMULOG
   if (mit_simulog && (!brief_log || (brief_log && restart)))
   {
      sprintf(simuLogBuf, "%s%s%s\n", STRICH_15, STRICH_15, STRICH_15);	
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
#endif /* kein TEST_SIM */
   return (ak_bewertet);
} /* do_math_sim */



/* =========================== start_simu_lauf ============================== */
static BOOL start_simu_lauf (INT    *i_params,
                             DOUBLE *r_params)
/*----------------------------------------------------------------------------*/
/*   Startet mit den Eingabeparametern "i_params" und "r_params" einen Simu-  */
/*   latorlauf und wartet die Rueckmeldung ab. Liefert TRUE, wenn diese ohne  */
/*   Fehlermeldung der Error-Pipe innerhalb einer vorgegebenen Wartezeit er-  */
/*   halten wurde.                                                            */
/*   Die Parameter werden mit "erg_genauigkeit" Stellen im f-Format ueber-    */
/*   geben.                                                                   */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE         out_buf,
                       simModBuf;
   STR_TYPE            buf;
   ACT_DESCR_PTR_TYPE  act_descr;
   INT                 ii, ai, par_ctr;
   BOOL                dummy,
                       par_ok,
                       ok = TRUE;

   /*  start_simu_lauf */
   strcpy (out_buf, "");
   if (i_par_ges_anz > 0)
   { /* Es gibt Integer-Parameter */
      par_ctr = 0;
      par_ok  = TRUE;
      for (ai = 1;  (ai <= last_act) && ok;  ai++)
      { /* alle Integer-Parameter */
	 if (get_act_descr (&act_descr, ai, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
	 { /* Zugriff ok */
	    for (ii = 0;  (ii < act_descr->i_par_anz) && par_ok;  ii++)
	    { /* einen Integer-Parameter */
	       sprintf (buf, "%s=%d;", act_descr->i_par_descr[ii].par_name, 
			i_params[par_ctr]);
	       strcat (out_buf, buf);
	       par_ctr++;
	       par_ok = par_ctr < i_par_ges_anz;
	    } /* einen Integer-Parameter */
	 } /* Zugriff ok */
	 else
	    ok = FALSE;
      } /* alle Integer-Parameter */
      if (par_ctr != i_par_ges_anz)
      { /* I-Param-Anz stimmt nicht */
	 fatal (SIMU_MATH_KOPPL, 1, par_ctr, i_par_ges_anz, "", BAD_PAR_AMOUNT);
	 ok = FALSE;
      } /* I-Param-Anz stimmt nicht */
   } /* Es gibt Integer-Parameter */
   if (r_par_ges_anz > 0)
   { /* Es gibt Real-Parameter */
      par_ctr = 0;
      par_ok  = TRUE;
      for (ai = 1;  (ai <= last_act) && ok;  ai++)
      { /* alle Real-Parameter */
	 if (get_act_descr (&act_descr, ai, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
	 { /* Zugriff ok */
	    for (ii = 0;  (ii < act_descr->r_par_anz) && par_ok;  ii++)
	    { /* einen Real-Parameter */
	       /*
	       sprintf (buf, "%s=%.*E;", act_descr->r_par_descr[ii].par_name, 
			erg_genauigkeit-1, r_params[par_ctr]);
               */
	       sprintf (buf, "%s=%.*f;", act_descr->r_par_descr[ii].par_name, 
			erg_genauigkeit-1, r_params[par_ctr]);
	       strcat (out_buf, buf);
	       par_ctr++;
	       par_ok = par_ctr < r_par_ges_anz;
	    } /* einen Real-Parameter */
	 } /* Zugriff ok */
	 else
	    ok = FALSE;
      } /* alle Real-Parameter */
      if (par_ctr != r_par_ges_anz)
      { /* I-Param-Anz stimmt nicht */
	 fatal (SIMU_MATH_KOPPL, 2, par_ctr, r_par_ges_anz, "", BAD_PAR_AMOUNT);
	 ok = FALSE;
      } /* I-Param-Anz stimmt nicht */
   } /* Es gibt Real-Parameter */
   if (ok)
   { /* Start eines Simulatorlaufs */
      if (getStringVal(simModBuf, SIMU_MOD_NAME_IDX, GLEAM_ERROR))  /* xxyza */
      { /* SIMU_MOD_NAME_TXT-Eintrag existiert in den TSK-Daten */
	 fprintf (tsim, "%s<<%s.mat\n", out_buf, simModBuf);
	 fflush(tsim);
#ifdef IO_LOG
	 if (mit_simulog)
         { /* Logging */
	    sprintf(simuLogBuf, "ToSim: %s<<%s.mat", out_buf, simModBuf);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 } /* Logging */
#endif /* IO_LOG */
	 ok = wait_for_reply (&dummy, simu_first_waits); /*1.Rueckmldg abwart.*/
      } /* SIMU_MOD_NAME_TXT-Eintrag existiert in den TSK-Daten */
      else /* no data entry! */
	 ok = FALSE;
   } /* Start eines Simulatorlaufs */          
   return (ok);
} /*  start_simu_lauf */



/* ========================== simu_lauf_auswertung ========================== */
static void simu_lauf_auswertung (DOUBLE *erg_werte)
/*----------------------------------------------------------------------------*/
/*   Die Routine kommuniziert ueber "restart" und "ak_bewertet" mit "do_      */
/*   math_sim". Sie erwartet die Simulationsergebnisse zeilenweise und wertet */
/*   sie hinsichtlich Fehlermeldungen und Prompt-Nr aus. Positivenfalls wird  */
/*   "ak_bewertet" TRUE gesetzt und in "erg_werte" das Ergebnis hinterlegt.   */
/*----------------------------------------------------------------------------*/
{
   INT       ii;
   BOOL      werte_ok, prompt_ok;

   /* simu_lauf_auswertung */
   if (strcmp (inbuffer, SIMU_ABORTED_MELD) == 0)
   { /* "SimuAborted"-Meldung */
      simu_wdh  = FALSE;                                   /* Unbewertbare AK */
      prompt_ok = till_math_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				    STOP_ON_ERROR);
      if (!prompt_ok)
	 prompt_ok = check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, 
				       EXPECT_NEW_PROMPT);
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "*** Simu: \"%s\"-Meldung erhalten.  "
		 "Erwarteten Prompt %ld erhalten:%d", 
		 SIMU_ABORTED_MELD, prompt_ctr, prompt_ok);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
   } /* "SimuAborted"-Meldung */
   else
   { /* Moeglicherweise 1.Wert erhalten */
      if (test_and_read_val (&(erg_werte[0])))
      { /* 1.Wert erhalten */
	 werte_ok = TRUE;
	 for (ii = 1;  (ii < akt_anz_roh_erg_werte) && werte_ok;  ii++)
	 { /* weitere Werte lesen */
	    werte_ok = wait_for_reply (&prompt_ok,              /* next Rück- */
				       simu_data_waits);        /* meldung    */
	    if (werte_ok)
	       werte_ok = test_and_read_val (&(erg_werte[ii]));
	 } /* weitere Werte lesen */
	 prompt_ok = till_math_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				       STOP_ON_ERROR);
	 ak_bewertet = werte_ok && prompt_ok;
	 simu_wdh    = !ak_bewertet;
#ifdef MITSIMULOG
	 if (mit_simulog && simu_wdh)
	 {
	    sprintf(simuLogBuf, "*** Simu Werte- od.Promptfehler: Erwarteten Prompt %ld "
		    "erhalten:%d  Gelesene Werte: OK=%d Ist=%d Soll=%d", prompt_ctr, 
		    prompt_ok, werte_ok, ii, akt_anz_roh_erg_werte);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 }
#endif /* MITSIMULOG */
      } /* 1.Wert erhalten */
      else
      { /* Schrott erhalten */
	 prompt_ok = till_math_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				       IGNORE_ERRORS);
	 if (!prompt_ok)
	    prompt_ok = check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, 
					  EXPECT_NEW_PROMPT);
#ifdef MITSIMULOG
	 if (mit_simulog)
	 {
	    sprintf(simuLogBuf, "*** Simu: Fehler beim 1.Wert. Erwarteten"
		    " Prompt %ld erhalten:%d", prompt_ctr, prompt_ok);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 }
#endif /* MITSIMULOG */
      } /* Schrott erhalten */
   } /* Moeglicherweise 1.Wert erhalten */
   if (prompt_ok)
      prompt_ctr++;
   else
      restart = TRUE;                                        /* Prompt-Fehler */
} /* simu_lauf_auswertung */



/* =========================== terminate_math_sim =========================== */
BOOL terminate_math_sim (BOOL  mit_quit)
{
   BOOL  reply_received = FALSE, 
         out_of_time,
         math_tot,
         erg;

   /* terminate_math_sim */
   math_tot = FALSE;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "%s TERMINATE_MATH_SIM: %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
      if (mit_quit)
	 write_to_simulog("ToSim: Quit\n", CLOSE_LOG);
   }
#endif /* MITSIMULOG */
#ifdef ONLINE
   if (mit_quit)
   { /* Mathematica mit "quit" terminieren */
      fprintf (tsim, "Quit\n"); fflush(tsim);          /* terminate simulator */
      /* -------------- Versuche, auf SIM_FIN_MELD zu warten: --------------- */
      if (wait_for_reply (&out_of_time, TERM_WAITS))
      { /* 1.Antwort erhalten */
	 if (strlen (inbuffer) == 0)
	    wait_for_reply (&out_of_time, TERM_WAITS);     /* skip blank line */
	 if (strncmp (inbuffer, SIM_FIN_MELD, SIM_FIN_MELD_LEN) == 0)
	    reply_received = TRUE;
	 else
	 { /* Nicht erhalten: 2.Versuch */
	    if (wait_for_reply (&out_of_time, TERM_WAITS)) 
	       reply_received = (strncmp (inbuffer, SIM_FIN_MELD, 
					  SIM_FIN_MELD_LEN) == 0);
	 } /* Nicht erhalten: 2.Versuch */
      } /* 1.Antwort erhalten */
   } /* Mathematica mit "quit" terminieren */
   if (!reply_received)
   { /* Noch nicht tot ! */
      math_tot = kill_process (MATH_WUERG_NAME);
   } /* Noch nicht tot ! */
   close (pipe_p2s[1]);
   close (pipe_s2p[0]);
   close (pipe_se2p[0]);
   erg    = reply_received || math_tot;
   sim_up = !erg;
#else /* ONLINE */
   fclose (tsim);
   fclose (fsim);
   fclose (efsim);
   sim_up = FALSE;
   erg    = TRUE;
#endif /* Nicht ONLINE */
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "TERMINATE_MATH_SIM: reply_received=%d", reply_received);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
      if (reply_received) {
	 sprintf(simuLogBuf, "  math_tot=%d\n", math_tot);
         write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
   }
#endif /* MITSIMULOG */
   return (erg);
} /* terminate_math_sim */



/* =========================== make_math_startsim =========================== */
BOOL make_math_startsim (void)
{
   /*   return (make_startsim ("#!/usr/bin/csh", "", "", "echo Leerzeile", 
	"echo OK > /tmp/hygleam_work/ok.tmp", "")); */
   return (make_startsim ("#!/usr/bin/csh", MATH_PACKAGE, akt_sim_mod_path, "", "", "math")); 
} /* make_math_startsim */



/* ========================== check_math_err_meld =========================== */
BOOL check_math_err_meld (void)
{
   return (strlen(errbuffer) > 0);
} /* check_math_err_meld */



/* ============================ till_math_prompt ============================ */
BOOL till_math_prompt (LONG  wcycles,
                       INT   max_skip,
                       BOOL  ignore_errors)
{
   STR_TYPE  nr_buf;
   INT       len, akt_nr,
             skipped_lines = 0;
   size_t    pos;
   BOOL      weiter        = TRUE,
             ok            = FALSE,
             tmo;

   /* till_math_prompt */
   do
   { /* Hauptschleife */
      if (wait_for_reply (&tmo, wcycles))
      { /* Zeile erhalten: analysieren */
         len = strlen (inbuffer);
         if (len > 6)
	    if ((inbuffer[0] == 'I') && (inbuffer[1] == 'n') && (inbuffer[2] == '['))
            { /* "In[" erkannt. "]" suchen, Nummer lesen und pruefen */
	       pos = strcspn (inbuffer, "]");
               if (pos < len)
               { /* "]" gefunden */
                  weiter = FALSE;                         /* Prompt erkannt   */
	          strncpy (nr_buf, &(inbuffer[3]), pos - 3);
                  nr_buf [pos-3] = '\0';                  /* terminate string */
                  if (sscanf (nr_buf, "%d", &akt_nr) == 1)
		  { /* Prompt-Nr gelesen */
		     if (prompt_ctr < 0)
		     { /* Jede Prompt-Nr gilt und wird abgeliefert */
			ok         = TRUE;
			prompt_ctr = akt_nr;
		     } /* Jede Prompt-Nr gilt und wird abgeliefert */
		     else
		        ok = akt_nr == prompt_ctr;
		     if (ok)
			ok = (inbuffer[pos+1] == ':') && (inbuffer[pos+2] == '=');
		  } /* Prompt-Nr gelesen */
	       } /* "]" gefunden */
	    } /* "In[" erkannt. "]" suchen, Nummer lesen und pruefen */
      } /* Zeile erhalten: analysieren */
      else
	 weiter = ignore_errors && !tmo; /* FMeld von Simu oder keine Antwort */
      skipped_lines++;
   } /* Hauptschleife */
   while (weiter && (skipped_lines < max_skip));
   return (ok);
} /* till_math_prompt */



/* =========================== set_math_data_waits ========================== */
void set_math_data_waits (void)
{
   simu_data_waits = SIMU_GADO_WAITS;
} /* set_math_data_waits */



/* ============================ init_math_koppl ============================= */
BOOL init_math_koppl (void)
{
   wait_ctr         = 0;
   simu_first_waits = SIMU_FIRST_WAITS;
   set_math_data_waits ();
   return (TRUE);
} /* init_math_koppl */
