/*******************************************************************************
GLEAM/AE                                                                  V1.1.4

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: ELDO-KOPpLung
 
Package: simu                   File: eldokopl.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

Der Modult enthaelt die Anbindung an den externen ELDO-Simulator ueber SIMPILOT. 

Fehlerverhalten:
================
Wenn ein Simulationslauf schief ging, wird ausser bei "Error = 1" versucht, ihn
noch einmal durchzufuehren. Wenn auch das schief geht, gilt die Kette als unbe-
wertbar. Es wird schliesslich versucht, den Simulator erneut zu re-starten. 
Wenn das schief geht, gilt er als nicht mehr verfuegbar und die globale Variable
"sim_up" wird "FALSE". Insgesamt gibt es folgende Fehlersituationen:

TimeOut und fehlgeschlagener kleiner Restart:
     Bei keiner Antwort innerhalb einer vorgegebenen Zeit (ueber keine der 
     Pipes) und fehlgeschlagenem kleinem Restart wird ein "Restart-Gross" ver-
     versucht (Routine "restart_eldo_sim"):
     Zunaechst wird versucht, den ELDO-Prozess zu terminieren. Danach wird auf 
     ein Prompt von SimPilot gewartet. Schliesslich wird mit "terminate_eldo_
     sim" durch Senden eines "quit" versucht, auch SimPilot zu terminieren. Wenn
     letzteres klappt, wird der Simlator mittels "init_sim" und "start_eldo_sim"
     komplett neu gestartet. 
     Start erfolgreich (sim_up = TRUE)?
        Ja:   Wiederhole gegebenenfalls den Lauf
        Nein: Crash
     Das Ergebnis schlaegt sich in "sim_up" nieder.

"Error = 2" oder "Error = ??" oder Fehlermeldungen der Simulator-Pipe oder 
Prompt-Fehler:
     Irgendwas ging bei der Simulation schief u. es erfolgt ein "Restart-Klein":
     Simulator mit "initscript"-Befehl neu initialisieren. 
     Das Ergebnis schlaegt sich in "sim_up" nieder.

Nicht alle Werte erhalten:
     Der Simulatorlauf wird noch einmal gestartet.

"Error = 1":
     Die AK gilt als unbewertbar. Der Simulatorlauf ist damit beendet.

Uebersicht:
   Situation:    | restart_klein | restart_gross | simu_wdh | ak_bewertet |
-----------------+---------------+---------------+----------+-------------+
  RestartKl-Fehl.|               |               |          |             |
  TimeOut        |      0        |       1       |     1    |      0      |
-----------------+---------------+---------------+----------+-------------+
  Error = 2      |               |               |          |             |
  Error = ??     |               |               |          |             |
  FMeld d.SimPipe|      1        |       0       |     1    |      0      |
  Prompt-Fehler  |               |               |          |             |
-----------------+---------------+---------------+----------+-------------+
  Werte-Fehler   |      0        |       0       |     1    |      0      |
-----------------+---------------+---------------+----------+-------------+
  Error = 1      |      0        |       0       |     0    |      0      |
-----------------+---------------+---------------+----------+-------------+
  Werte/Prompt ok|      0        |       0       |     0    |      1      |
-----------------+---------------+---------------+----------+-------------+
 
 
Die Routinen im einzelnen:

BOOL start_eldo_sim (char *mod_name);
          Die Routine setzt voraus, dass der externe Simulator mit Hilfe von
          "init_sim" (Modul "ext_koppl" in Package "sys") erfolgreich ("sim_ok" 
          ist TRUE) gestartet wurde.  
          In diesem Falle erzeugt sie eine Startmeldung mit Uhrzeit und mit dem 
          "mod_name", sofern "mod_name" nicht leer ist. Die Meldung wird ueber 
	  den Textpuffer des Packages "fbhm" ausgegeben. Dann wartet die Routine
	  auf das erste Prompt und laedt positivenfalls das "initscript".
	  Andernfalls oder wenn "initscript" nicht erfolgreich ausgefuehrt wer-
	  den konnte, ist das Funktionsergebnissie FALSE, sonst TRUE.

BOOL restart_eldo_sim (void);
          Es wird versucht, den externen Simulator zu terminieren und in einen 
          Zustand zu bringen, wie er nach "start_eldo_sim" herrscht. 
          Wenn das glueckt, liefert die Routine TRUE sonst FALSE und "sim_up" 
          wird entsprechend gesetzt.

void kleiner_eldo_restart (void);
          Nach der Entgegennahme eventueller Fehlermeldungen und Erhalt irgend-
          eines Prompts auf Grund von CR an den Simulator wird versucht, den ex-
          ternen Simulator mit "initscript" neu zu initialisieren. "sim_up" wird 
          TRUE, wenn sich der Simulator mit dem Init-Prompt ("Init:+") und 
          danach ohne Fehlermeldungen mit einem Prompt meldet.

BOOL do_eldo_sim (DOUBLE *erg_werte,
                  int    *i_params,
                  DOUBLE *r_params);
          Die Routine fuehrt einen Simulationslauf aus und liefert TRUE, wenn in
          "erg_werte" gueltige Werte stehen, d.h., die Simulation erfolgreich 
          durchgefuehrt werden konnte. 
          Die Eingangs- ("i_params" und "r_params") und Ausgangsparameter 
          ("erg_werte") sind bei der Routine "do_sim" im Modul "ext_sim" be-
          schrieben.
	  Wenn die Routine FALSE liefert, kann die globale Variable "sim_up" 
          FALSE sein, sofern der Simulator sich nicht mehr mit "restart_eldo_
          sim" starten liess. Bei geloeschtem "sim_up" terminiert die Routine 
          sofort und liefert FALSE.
 
BOOL make_eldo_startsim (void);
          Erzeugt unter Benutzung von "make_startsim" ("sys"-Package) das ELDO-
	  spezifische "startsim"-File und liefert TRUE ab, wenn das File ange-
          legt werden konnte.

BOOL terminate_eldo_sim (BOOL  mit_quit);
          Bei gesetztem "mit_quit" wird der Simulator mit "quit" terminiert u. 3 
          mal maximal "RESTART_WAITS" Sekunden auf die Schlussmeldung gewartet.
          Wenn keine Schlussmeldung erhalten wurde oder "mit_quit" FALSE ist, 
          werden ELDO und SIMPILOT per "kill_process" abgewuergt.
          Schliesst die Pipe-Streams. Liefert TRUE, wenn die Schlussmeldung er-
          halten wurde oder "kill_process" erfolgreich war.

BOOL check_eldo_err_meld (void);
          Meldungen aus der Error-Pipe gelten als Fehlermeldungen, wenn sie mit 
          "ERROR" beginnen oder lauten: "simpilot: Command not found". Zusaetz-
          lich gilt, dass eine Meldung, die mit "ERROR 1005: Unable to get 
          license for Aspire" beginnt, nicht als Fehlermeldung gilt.

BOOL till_eldo_prompt (LONG  wcycles,
                       INT   max_skip,
                       BOOL  ignore_errors);
          Die Routine liest solange Zeilen (mit "wcycles" Versuchen) vom Simu-
          lator ein, bis entweder "max_skip" Zeilen oder eine Prompt-Zeile ge-
          lesen wurde(n). Wenn die Nummer im Prompt "prompt_ctr" entspricht, 
          liefert die Routine TRUE, sonst immer FALSE. Wenn "prompt_ctr" == -1 
          ist, wird die gelesene Prompt-Nr immer akzeptiert. Bei gesetztem 
          "ignore_errors" werden auch Fehlermeldungen ignoriert.
          Form des Prompts: SPn>, wobei n eine Integerzahl ist.

void set_eldo_data_waits (void);
          Setzt die Wartezeit nach Empfang der ersten Daten bis zu den nachfol-
          genden.

BOOL init_eldo_koppl (void);
          Modulinitialisierung. Liefert FALSE bei Speichermangel.

--------------------------------------------------------------------------------
basiert auf C-Unit: eldo_koppl
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Erweiterung fuer die MatLab-Koppung: Streichung von GAMA.        WJ 22.08.07
 2. Entfernung von Code im Kontext der Schalter: BGI_MODE und 
    GLEAM_TCL. Streichung von GLEAM_C.                               WJ 22.06.12
 3. Explizite Klammern zur eindeutigen Zuordnung von else-Zweigen. 
    Entfernung ueberfluessiger Variable. Korrektur v. Formatangaben. WJ 28.06.12
 4. Renovierung: Streichung des "link"-Includes. Anpassung der 
    "write_line"-Aufrufe.                                            WJ 22.04.16
 5. Ersetzung von "akt_roh_erg_werte" durch "akt_anz_roh_erg_werte". WJ 29.05.17
 6. Anpassungen an das neue Logging der Simulatorschnittstelle.      WJ 13.09.17
 7. start_eldo_sim(): Avoiding buffer overflows of static strings.   WJ 16.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 16.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "aufg.h"
#include "simuInt.h"
#include "simu.h"

#include GLOBAL_TEXTS
#include SIMU_TEXTS



/* ============================ Lokale Konstante: =========================== */
#define ELDO_PACKAGE         "anacad4_4"
#define ELDO_WUERG_NAME      "eldo_fid"
#define SIMU_WUERG_NAME      "simpilot"
#define SIM_FIN_MELD         "End of SimPilot session"
#define SIM_FIN_MELD_LEN      23
#define INITSCR_PROMPT       "Init:"
#define INITSCR_PROMPT_LEN     5
#define START_SKIPS           14
#define INIT_SKIPS            12
#define ERR_SKIPS             15
#define RESTART_PROMPT_TRIALS  4
#ifdef ONLINE
  #define START_WAITS           30 sec
  #define INIT_WAITS            40 sec
  #define SIMU_FIRST_WAITS     300 sec
  #define SIMU_GADO_WAITS        2 sec
  #define RESTART_WAITS          (LONG)(0.3 sec)
  #define SKIP_ERR_WAITS         (LONG)(0.4 sec)
#else /* ONLINE */
  #define START_WAITS            4
  #define INIT_WAITS             4
  #define SIMU_FIRST_WAITS       4
  #define SIMU_GADO_WAITS        2
  #define RESTART_WAITS          2
  #define SKIP_ERR_WAITS         1
#endif /* ONLINE */
#ifdef BRIEF_LOG
  #define brief_log             TRUE
#else
  #define brief_log             FALSE
#endif /* BRIEF_LOG */


/* ============================ Lokale Variable: ============================ */
static DOUBLE  simu_secs = 0.0;       /* Simu-Laufzeit in Sekunden            */
static LONG    simu_data_waits;       /* Wartezyklen nach 1.SimuDaten bis next*/
static BOOL    restart_klein,         /* True, wenn kleiner Restart bei Simu-F*/
               restart_gross,         /* True, wenn grosser Restart bei Simu-F*/
               simu_wdh;              /* True, wenn Simu zu wiederholen ist   */
#ifdef MITSIMULOG
  static BOOL  error1;                /* TRUE bei "Error = 1"                 */
#endif




/* ============================ Lokale Routinen: ============================ */
static void perform_initscript    (void);

static BOOL start_simu_lauf       (BOOL   *time_out,
                                   INT    *i_params,
                                   DOUBLE *r_params);

static void simu_lauf_auswertung  (DOUBLE *erg_werte);





/* ========================== perform_initscript============================= */
static void perform_initscript (void)
{
   INT   skipped_lines = 0;
   BOOL  weiter        = TRUE,
         ok            = FALSE,
         dummy;

   /* perform_initscript */
   fprintf (tsim, "initscript\n"); fflush(tsim);
#ifdef IO_LOG
   if (mit_simulog)
      write_to_simulog("ToSim: initscript", CLOSE_LOG);
#endif /* IO_LOG */
   do
   { /* Warten auf Ende-Meldung des Initscripts */
      if (wait_for_reply (&dummy, INIT_WAITS))
      { /* Zeile erhalten: analysieren */
	 if (strlen (inbuffer) > (size_t)INITSCR_PROMPT_LEN)
	    if (strncmp (inbuffer, INITSCR_PROMPT, INITSCR_PROMPT_LEN) == 0)
	    { /* Init-Prompt erkannt */
	       ok = inbuffer [INITSCR_PROMPT_LEN] == '+';
	       weiter = FALSE;
	    } /* Init-Prompt erkannt */
      } /* Zeile erhalten: analysieren */
      skipped_lines++;
   } /* Warten auf Ende-Meldung des Initscripts */
   while (weiter && (skipped_lines < START_SKIPS));
   if (ok)
   { /* Intscript hat positiv quittiert */
      if (till_eldo_prompt ((1 sec), 1, STOP_ON_ERROR))
	 sim_up = TRUE;
      else
	 sim_up = check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, 
				    EXPECT_NEW_PROMPT);
   } /* Intscript hat positiv quittiert */
   else
      sim_up = FALSE;
   prompt_ctr++;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "PERFORM_INITSCRIPT: init_prompt=%d, sim_ok=%d, sim_up=%d", 
	      ok, sim_ok, sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
} /* perform_initscript */



/* ============================= start_eldo_sim ============================= */
BOOL start_eldo_sim (STR_TYPE  mod_name)
{
   STRING_TYPE  buf;
   STR_TYPE      mbuf, dummy_buf;
   BUF_TYPE      zbuf;

   /* start_eldo_sim */
   if (sim_ok)
   { /* SimPilot offenbar erfolgreich gestartet */
      get_time_stamp (dummy_buf, dummy_buf, dummy_buf, zbuf);
      if ((INT)(strlen (mod_name)) > 0)
	 sprintf (mbuf, SIMUSTART_TXT2, mod_name);
      else
	 strcpy (mbuf, "");
      sprintf (buf, SIMUSTART_TXT1, zbuf, mbuf);
      write_buf (1, buf);
      prompt_ctr = -1;                           /* akzeptiere jede Prompt-Nr */
      sim_up = till_eldo_prompt (START_WAITS, START_SKIPS, STOP_ON_ERROR); 
      sim_up = sim_up && sim_ok;
      if (sim_up)
      { /* SimPilot Startmeldung und 1.Prompt erhalten */
         prompt_ctr++;
	 perform_initscript ();
      } /* SimPilot Startmeldung und 1.Prompt erhalten */
   } /* SimPilot offenbar erfolgreich gestartet */

   return (sim_up);
} /* start_eldo_sim */



/* ============================ restart_eldo_sim ============================= */
BOOL restart_eldo_sim (void)
{  
   INT   prompt_trials = 0;
   BOOL  prompt_received;

   /* restart_eldo_sim */
   sim_up = FALSE;
#ifdef ONLINE
   if (kill_process (ELDO_WUERG_NAME))
   { /* ELDO abgewuergt */
      do
      { /* Auf Prompt von SimPilot warten */
	 prompt_ctr = -1;               /* akzeptiere jede Prompt-Nr */
	 prompt_received = till_eldo_prompt (RESTART_WAITS, START_SKIPS, 
					     IGNORE_ERRORS);
	 prompt_trials++;
      } /* Auf Prompt von SimPilot warten */
      while (!prompt_received && (prompt_trials < RESTART_PROMPT_TRIALS));
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "RESTART_ELDO_SIM: prompt_received=%d", prompt_received);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
      prompt_received = terminate_eldo_sim (TRUE);  /* Mit "quit" terminieren */
      if (prompt_received)
      { /* SimPilot komplett neu starten */
	 init_sim ();                      /* SimPilot erneut starten         */
#ifdef MITSIMULOG
         if (mit_simulog)
	 {
	    sprintf(simuLogBuf, "RESTART_ELDO_SIM: Termination Done. INIT_SIM: sim_ok =%d", sim_ok);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 }
#endif /* MITSIMULOG */
	 start_eldo_sim ("");
      } /* SimPilot komplett neu starten */
   } /* ELDO abgewuergt */
#else /* ONLINE */
   tsim  = fopen ("tsim.txt", "w");
   fsim  = fopen ("fsim.txt", "r");
   efsim = fopen ("efsim.txt", "r");
   sim_ok = (fsim != NULL) && (efsim != NULL);
   sim_up = sim_ok;
#endif /* ONLINE */
   return (sim_up);
} /* restart_eldo_sim */



/* ========================== kleiner_eldo_restart ========================== */
void kleiner_eldo_restart (void)
{
   /* kleiner_eldo_restart */
   prompt_ctr = -1;
   if (check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, EXPECT_NEW_PROMPT))
   { /* Prompt erhalten */
      prompt_ctr++;
      perform_initscript (); /* Initscript ausfuehren und aif Reaktion warten */
   } /* Prompt erhalten */
   else
      sim_up = FALSE;
} /* kleiner_eldo_restart */



/* ============================== do_eldo_sim =============================== */
BOOL do_eldo_sim (DOUBLE *erg_werte,
                  INT    *i_params,
                  DOUBLE *r_params)
{
   STR_TYPE  zeit_buf;
   INT       ii;

   /* do_eldo_sim */
#ifdef TEST_SIM
   ak_bewertet = TRUE;
   erg_werte[0] = frand() * 2000;
   erg_werte[1] = frand() * 0.8;
   erg_werte[2] = frand() * 0.4;
   erg_werte[3] = frand() *1000; 
   erg_werte[4] = frand() * 1.5;
   erg_werte[5] = frand() * 0.5;
   printf (MSG_LINE_TXT, "Achtung: ELDO-Koppl im Testmodus! KEIN Aufruf von simpilot!");
#else  /* --------------- Ankopplung des externen Simulators: --------------- */
   ak_bewertet = FALSE;
   if (sim_up)
   { /* Simulator verfuegbar */
      restart_klein = FALSE;
      simu_wdh      = TRUE;
      simu_secs     = 0.0;
      wait_ctr      = 0;
#ifdef MITSIMULOG
      error1        = FALSE;
#endif
      if (start_simu_lauf (&restart_gross, i_params, r_params))
	 simu_lauf_auswertung (erg_werte);  /* Antw.i.d.Zeit ohne ErrPipeMldg */
      else
	 restart_klein = !restart_gross;  /*restart_klein=TRUE: ErrPipe-Mldg! */
      if (mit_rz)
      { /* Rechenzeitermittlung */
	 simu_secs = wait_ctr;
	 simu_secs = (simu_secs * WAIT_FOR_READ) / 1000;
	 sprintf (zeit_buf, SIMU_RZ_TXT, simu_secs);
	 printf (MSG_LINE_TXT, zeit_buf);
      } /* Rechenzeitermittlung */
#ifdef MITSIMULOG
      if (mit_simulog && 
	  (!brief_log || (brief_log && (restart_gross || error1))))
      { /* Logging */
	 if (!simu_logfile_open)
	    simu_logfile = fopen(simu_logfilename, "at");
	 if (simu_logfile != NULL)
	 { /* logfile is open */
	    fprintf(simu_logfile, "Simu 1.Lauf: RZ=%6.2f  ak_bewert=%d  restart:"
		    " klein=%d gross=%d  simu_wdh=%d  sim_up=%d", simu_secs, 
		    ak_bewertet, restart_klein, restart_gross, simu_wdh, sim_up);
	    if (restart_gross || !brief_log || 
#ifdef DETAILED_LOG
		ak_bewertet   ||
#endif 
		error1)
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
	    meldung(SIMU_ELDO_KOPPL, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
      } /* Logging */
#endif /* MITSIMULOG */

      /* ---------------- eventuell Restart und 2.Simu-Lauf: ---------------- */
      if (restart_klein && sim_up)
	 kleiner_eldo_restart ();                          /* Kleiner Restart */
      if (restart_gross || (restart_klein && !sim_up))
	 restart_eldo_sim ();                              /* Grosser Restart */

      if (simu_wdh && sim_up)
      { /* Simu-Lauf wiederholen */
#ifdef MITSIMULOG
	 error1        = FALSE;
#endif
	 restart_klein = FALSE;
	 simu_secs     = 0.0;
	 wait_ctr      = 0;
	 if (start_simu_lauf (&restart_gross, i_params, r_params))
	    simu_lauf_auswertung (erg_werte); /*Antw.i.d.Zeit ohne ErrPipeMldg*/
	 else
	    sim_up = restart_klein = !restart_gross;  /*TimeOut-->clear sim_up*/
	 if (mit_rz)
	 { /* Rechenzeitermittlung */
	    simu_secs = wait_ctr;
	    simu_secs = (simu_secs * WAIT_FOR_READ) / 1000;
	    sprintf (zeit_buf, SIMU_RZ_TXT, simu_secs);
	    printf (MSG_LINE_TXT, zeit_buf);
	 } /* Rechenzeitermittlung */
#ifdef MITSIMULOG
	 if (mit_simulog)
	 { /* Logging */
	    if (!simu_logfile_open)
	       simu_logfile = fopen(simu_logfilename, "at");
	    if (simu_logfile != NULL)
	    { /* logfile is open */
	       fprintf (simu_logfile, "Simu 2.Lauf: RZ=%6.2f  ak_bewert=%d  "
			"restart: klein=%d gross=%d  simu_wdh=%d  sim_up=%d\n", 
			simu_secs, ak_bewertet, restart_klein, restart_gross, 
			simu_wdh, sim_up);
	       if (restart_gross || ak_bewertet || error1 || !brief_log)
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
	       meldung(SIMU_ELDO_KOPPL, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
	 } /* Logging */
#endif /* MITSIMULOG */
         /* ----------------------- eventuell Restart: ---------------------- */
         if (restart_klein && sim_up)
	    kleiner_eldo_restart ();                       /* Kleiner Restart */
         if (restart_gross || (restart_klein && !sim_up))
	    restart_eldo_sim ();                           /* Grosser Restart */
      } /* Simu-Lauf wiederholen */
   } /* Simulator verfuegbar */
#ifdef MITSIMULOG
   if (mit_simulog && (!brief_log || (brief_log && (restart_gross || error1))))
   {
      sprintf(simuLogBuf, "%s%s%s\n", STRICH_15, STRICH_15, STRICH_15);	
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
#endif /* Ankopplung des externen Simulators */
   return (ak_bewertet);
} /* do_eldo_sim */



/* =========================== start_simu_lauf ============================== */
static BOOL start_simu_lauf (BOOL   *time_out,
                             INT    *i_params,
                             DOUBLE *r_params)
/*----------------------------------------------------------------------------*/
/*   Startet mit den Eingabeparametern "i_params" und "r_params" einen Simu-  */
/*   latorlauf und wartet die 1. Rueckmeldung ab. Liefert TRUE, wenn diese    */
/*   ohne Fehlermeldung der Error-Pipe innerhalb einer vorgegebenen Wartezeit */
/*   erhalten wurde. Bei einem Time-Out ist "time_out" TRUE.                  */
/*   Die Parameter werden mit "erg_genauigkeit" Stellen uebergeben. Da das E- */
/*   Format genutzt wird, betraegt die Precission "erg_genauigkeit - 1".      */
/*----------------------------------------------------------------------------*/
{
   INT  ii;

   /*  start_simu_lauf */
   fprintf (tsim, "gesamt3script");
   for (ii = 0;  ii < i_par_ges_anz;  ii++)
      fprintf (tsim, " %d", i_params[ii]);
   for (ii = 0;  ii < r_par_ges_anz;  ii++)
      fprintf (tsim, " %.*E", erg_genauigkeit-1, r_params[ii]);
   fprintf (tsim, "\n");
   fflush(tsim);                                /* Start eines Simulatorlaufs */
#ifdef IO_LOG
   if (mit_simulog)
   { /* Logging */
      if (!simu_logfile_open)
	 simu_logfile = fopen(simu_logfilename, "at");
      if (simu_logfile != NULL)
      { /* logfile is open */
	 fprintf (simu_logfile, "ToSim: gesamt3script");
	 for (ii = 0;  ii < i_par_ges_anz;  ii++)
	    fprintf (simu_logfile, " %d", i_params[ii]);
	 for (ii = 0;  ii < r_par_ges_anz;  ii++)
	    fprintf (simu_logfile, " %.*E", erg_genauigkeit-1, r_params[ii]);
	 fprintf (simu_logfile, "\n");
	 fclose(simu_logfile);
	 simu_logfile_open = FALSE;
      } /* logfile is open */
      else
	 meldung(SIMU_ELDO_KOPPL, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
   } /* Logging */
#endif /* IO_LOG */
   return (wait_for_reply (time_out, simu_first_waits));  /*1.Rueckmldg warten*/
} /*  start_simu_lauf */



/* ======================== simu_lauf_auswertung ============================ */
static void simu_lauf_auswertung (DOUBLE *erg_werte)
/*----------------------------------------------------------------------------*/
/*   Die Routine kommuniziert ueber "restart_klein", "restart_gross" und      */
/*   "ak_bewertet" mit "do_sim". Sie liefert TRUE, wenn der Simu-Lauf wieder- */
/*   holt werden soll. Sie erwartet die Simulationsergebnisse und wertet sie  */
/*   hinsichtlich Fehlermeldungen und Prompt-Nr aus. Positivenfalls wird      */
/*   "ak_bewertet" TRUE gesetzt und in "erg_werte" das Ergebnis hinterlegt.   */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  nr_buf;
   INT       len, fnr, ii;
   BOOL      werte_ok, prompt_ok;

   /* simu_lauf_auswertung */
   if (strncmp (inbuffer, "Error =", 7) == 0)
   { /* "Error"-Meldung */
#ifdef MITSIMULOG
      fnr = -1;
#endif
      len = strlen (inbuffer);
      if (len > 7)
      { /* Nummer der "Error"-Meldung auswerten */
	 strncpy (nr_buf, &(inbuffer[7]), len - 7);
	 nr_buf [len-7] = '\0';                         /* terminate string */
	 if (sscanf (nr_buf, "%d", &fnr) == 1)
         { /* "fnr" gelesen */
	    if (fnr == 2)
	       restart_klein = TRUE;               /* Fehler beim Simu-Lauf */
	    else
	    {
	       if (fnr == 1)
                  simu_wdh = FALSE;                /* Unbewertbare AK       */
	       else
		  restart_klein = TRUE;            /* Unbekannte Fehlermldg */
	    }
	 } /* "fnr" gelesen */
#ifdef MITSIMULOG
	 error1 = fnr == 1;
#endif
      } /* Nummer der "Error"-Meldung auswerten */
      else
	 restart_klein = TRUE;                     /* Unbekannte Fehlermldg */
      prompt_ok = till_eldo_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				    STOP_ON_ERROR);
      if (!prompt_ok)
	 prompt_ok = check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, 
				       EXPECT_NEW_PROMPT);
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "*** Simu: Error = %d  restart_klein=%d  "
		 "simu_wdh=%d  Erwarteten Prompt %ld erhalten:%d", 
		 fnr, restart_klein, simu_wdh, prompt_ctr, prompt_ok);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
   } /* "Error"-Meldung */
   else
   { /* Moeglicherweise 1.Wert erhalten */
      if (test_and_read_val (&(erg_werte[0])))
      { /* 1.Wert erhalten */
         werte_ok = TRUE;
	 for (ii = 1;  (ii < akt_anz_roh_erg_werte) && werte_ok;  ii++)
	 { /* weitere Werte lesen */
           werte_ok = wait_for_reply (&prompt_ok,             /* next Rück- */
				      simu_data_waits);       /* meldung    */
	    if (werte_ok)
	       werte_ok = test_and_read_val (&(erg_werte[ii]));
	 } /* weitere Werte lesen */
         prompt_ok = till_eldo_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				       STOP_ON_ERROR);
	 if (!prompt_ok)
	    prompt_ok = check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, 
					  EXPECT_NEW_PROMPT);
         ak_bewertet = werte_ok && prompt_ok;
         simu_wdh    = !ak_bewertet;
#ifdef MITSIMULOG
         if (mit_simulog && simu_wdh)
	 {
	    sprintf(simuLogBuf, "*** Simu Werte- od.Promptfehler: Erwarteten Prompt "
		    "%ld erhalten:%d  Gelesene Werte: OK=%d Ist=%d Soll=%d", prompt_ctr, 
		    prompt_ok, werte_ok, ii, akt_anz_roh_erg_werte);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 }
#endif /* MITSIMULOG */
      } /* 1.Wert erhalten */
      else
      { /* Schrott erhalten */
         prompt_ok = till_eldo_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
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
      restart_klein = TRUE;                                  /* Prompt-Fehler */
} /* simu_lauf_auswertung */



/* =========================== make_eldo_startsim =========================== */
BOOL make_eldo_startsim (void)
{
   return (make_startsim ("#!/opt/bin/tcsh", ELDO_PACKAGE, akt_sim_mod_path, 
			  "setenv SHDEV /dev/null", "", "simpilot -i"));
} /* make_eldo_startsim */



/* =========================== terminate_eldo_sim =========================== */
BOOL terminate_eldo_sim (BOOL  mit_quit)
{
   BOOL  reply_received = FALSE, 
         out_of_time,
         eldo_tot       = TRUE,
         simpilot_tot   = TRUE,
         erg;

   /* terminate_eldo_sim */
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "%s TERMINATE_ELDO_SIM: %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
      if (mit_quit)
	 write_to_simulog("ToSim: quit", CLOSE_LOG);
   }
#endif /* MITSIMULOG */
#ifdef ONLINE
   if (mit_quit)
   { /* SIMPILOT mit "quit" terminieren */
      fprintf (tsim, "quit\n"); fflush(tsim);          /* terminate simulator */
      /* --- Versuche, auf "End of SimPilot session" zu warten: --- */
      if (wait_for_reply (&out_of_time, RESTART_WAITS))
      { /* 1.Antwort erhalten */
	 if (strlen (inbuffer) == 0)
	    wait_for_reply (&out_of_time, RESTART_WAITS);  /* skip blank line */
	 if (strncmp (inbuffer, SIM_FIN_MELD, SIM_FIN_MELD_LEN) == 0)
	    reply_received = TRUE;
	 else
	 { /* Nicht erhalten: 2.Versuch */
	    if (wait_for_reply (&out_of_time, RESTART_WAITS)) 
	       reply_received = (strncmp (inbuffer, SIM_FIN_MELD, 
					  SIM_FIN_MELD_LEN) == 0);
	 } /* Nicht erhalten: 2.Versuch */
      } /* 1.Antwort erhalten */
   } /* SIMPILOT mit "quit" terminieren */
   if (!reply_received)
   { /* Noch nicht tot ! */
      eldo_tot     = kill_process (ELDO_WUERG_NAME);
      simpilot_tot = kill_process (SIMU_WUERG_NAME);
   } /* Noch nicht tot ! */
   close (pipe_p2s[1]);
   close (pipe_s2p[0]);
   close (pipe_se2p[0]);
   erg    = reply_received || (eldo_tot && simpilot_tot);
   sim_up = !erg;
#endif /* ONLINE */
   fclose (tsim);
   fclose (fsim);
   fclose (efsim);
   sim_up = FALSE;
   sim_ok = FALSE;
   erg    = TRUE;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "TERMINATE_ELDO_SIM: reply_received=%d", reply_received);
      write_to_simulog(simuLogBuf, KEEP_LOG_OPEN);
      if (reply_received) {
	 sprintf(simuLogBuf, "  eldo_tot=%d simpilot_tot=%d\n", eldo_tot, simpilot_tot);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
   }
#endif /* MITSIMULOG */
   return (erg);
} /* terminate_eldo_sim */



/* ========================== check_eldo_err_meld =========================== */
BOOL check_eldo_err_meld (void)
{
   return ((strncmp(errbuffer, "simpilot: Command not found", 27) == 0) ||
	   ((strncmp (errbuffer, "ERROR", 5) == 0) &&
	    !(strncmp (errbuffer, "ERROR 1005: Unable to get license for "
			"Aspire", 44) == 0)));
} /* check_eldo_err_meld */



/* ============================ till_eldo_prompt ============================ */
BOOL till_eldo_prompt (LONG  wcycles,
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

   /* till_eldo_prompt */
   do
   { /* Hauptschleife */
      if (wait_for_reply (&tmo, wcycles))
      { /* Zeile erhalten: analysieren */
         len = strlen (inbuffer);
         if (len > 3)
	    if ((inbuffer[0] == 'S') && (inbuffer[1] == 'P'))
            { /* SP.. erkannt. ">" suchen, Nummer lesen und pruefen */
	       pos = strcspn (inbuffer, ">");
               if (pos < len)
               { /* ">" gefunden */
                  weiter = FALSE;                         /* Prompt erkannt   */
	          strncpy (nr_buf, &(inbuffer[2]), pos - 2);
                  nr_buf [pos-2] = '\0';                  /* terminate string */
                  if (sscanf (nr_buf, "%d", &akt_nr) == 1)
		  { /* Prompt-Nr gelesen */
		     if (prompt_ctr < 0)
		     { /* Jede Prompt-Nr gilt und wird abgeliefert */
			ok         = TRUE;
			prompt_ctr = akt_nr;
		     } /* Jede Prompt-Nr gilt und wird abgeliefert */
		     else
		        ok = akt_nr == prompt_ctr;
		  } /* Prompt-Nr gelesen */
	       } /* ">" gefunden */
	    } /* SP.. erkannt. ">" suchen, Nummer lesen und pruefen */
      } /* Zeile erhalten: analysieren */
      else
	 weiter = ignore_errors && !tmo; /* FMeld von Simu oder keine Antwort */
      skipped_lines++;
   } /* Hauptschleife */
   while (weiter && (skipped_lines < max_skip));

   return (ok);
} /* till_eldo_prompt */



/* =========================== set_eldo_data_waits ========================== */
void set_eldo_data_waits (void)
{
   simu_data_waits = SIMU_GADO_WAITS;
} /* set_eldo_data_waits */



/* ============================ init_eldo_koppl ============================= */
BOOL init_eldo_koppl (void)
{
   wait_ctr         = 0;
   simu_first_waits = SIMU_FIRST_WAITS;
   set_eldo_data_waits ();
   return (TRUE);
} /* init_eldo_koppl */

