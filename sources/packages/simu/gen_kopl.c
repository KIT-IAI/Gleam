/*******************************************************************************
GLEAM/AE                                                                  V1.1.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: GENerelle-KOPpLung
 
Package: simu                   File: gen_kopl.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

Der Modult enthaelt die allgemeine Anbindung an externe Simulatoren.

Durch das "aufg"-Package wird unter SIMU_RUNS_PER_INIT_TXT in "tsk_ints" eine 
Grenze der Simulatorlaeufe pro Init vorgegeben. Bei Erreichen dieser Grenze 
initialisiert "do_gen_sim" den ext. Simulator neu. Dies ist z.B. dann erforder-
lich, wenn der ext. Simulator ein memory leak hat.


Die Pipe-Schnittstelle zum externen Simulator sieht folgendermassen aus:

  Initialisierung:
  ================
  stdout:
    Init <simu_modell>

  stdin:
    Initialized
    <init_prompt>


  Simulatoraufruf:
  ================
  stdout:
    Bei GenExtSim (multi_line_call ist FALSE):
      <simu_modell> <param_1>=<wert> <param_2>=<wert> ... <param_n>=<wert>

    Bei GenLMExtSim (multi_line_call ist TRUE):
      <simu_modell> 
      <param_1>=<wert> 
      <param_2>=<wert> 
        ... 
      <param_n>=<wert>
      StartSimu

  stdin:
    <wert_1>
    <wert_2>
       ...
    <wert_m>
    <prompt>

    SimuAborted
    <prompt>

  stderr:
    <beliebige Meldung>


  Terminierung:
  =============
  stdout:
    Quit

  stdin:
    Terminated


Die Parameter kommen in der Reihenfolge der Gendefinitionen im Genmodell,
wobei zuerst alle Integer- und dann alle Realparameter ausgegeben werden. Bei 
einem Multiline-Call wird das "StartSimu"-Kommando durch ein Prompt (siehe weiter 
unten) bestaetigt. Ausserdem jede Parameterzeile, sofern PROMPT_EVERY_ML_LINE 
gesetzt ist. 
Bei einem Singleline-Call wird die Aufrufzeile in jedem Fall durch ein Prompt 
bestätigt. 
Die Terminierung wird nicht durch erfolgt ohne Prompts.

Bei dynamischen AKs wird die Parameteruebergabe des obigen Ablaufschema wie folgt
modifiziert: Zwischen <simu_modell> und den Parametern werden die Gencodes in
der Reihenfolge der Gene/Aktionen in der AK eingeschoben und die Parameter-
benennungen werden um die Gen/Aktionsnummern (<nr>) erweitert. Die Parameter 
kommen in der Reihenfolge der Gene/Aktionen in der AK und pro Gen/Aktion in der 
Reihenfolge, in der sie im jeweiligen Gentyp im Genmodell definiert wurden. Bei 
einem Multiline-Call sieht das dann folgendermassen aus (auch die Gen/Aktions-
codes werden mit einem Prompt bestaetigt):

  <simu_modell>
  Akt#<nr>=<code>
    ... 
  Akt#<nr>=<code>
  <param_1>#<nr>=<wert> 
  <param_2>#<nr>=<wert> 
    ... 
  <param_n>#<nr>=<wert>
  StartSimu

Dabei ist <nr> die laufende Nummer der Aktion in der AK. Bei einem Multiline-
Call werden jede Gen/Aktionscodezeile und jede Parameterzeile durch ein Prompt 
bestaetigt, sofern PROMPT_EVERY_ML_LINE gesetzt ist.


Die beiden Eingangspipes "stdin" und "stderr" werden zyklisch zusammen abge-
fragt. Bei Vorliegen eines beliebigen Text in "stderr" gilt die Simulation als
fehlerhaft beendet.
Nach der Initialisierung wird auf den initialen Prompt als Zeichen eines er-
folgreichen Starts des externen Simulators gewartet.
Jeder Prompt hat die Form "<nr>>". Mit der Nummer des initialen Prompts
wird die Schnittstelle initialisiert. Alle folgenden Promptnummern sind jeweils 
um 1 inkrementiert. Dies wird geprueft, um sicherzustellen, dass die Antworten 
des Simulators zu den von SIMOT gestellten Fragen passen.
Der einzeilige Aufruf besteht aus dem Modelnamen gefolgt von der Parameterliste,
also erst alle Integer-, dann alle Real-Parameter. Der mehrzeilige besteht aus je 
einer Zeile fuer den Modellnamen und die Parameterliste plus einer abschliessen-
den "StartSimu"-Zeile.
Die Antwort besteht aus den Werten, jeweils ein Wert pro Zeile. Die Werterueck-
gabe wird durch einen Prompt abgeschlossen. Leerzeilen oder Zeilen mit einem 
Zeichen werden ignoriert. Statt der Werte kann auch ein "SimuAborted" kommen.


Die Routinen im einzelnen:

BOOL start_gen_sim (char *mod_name);
          Die Routine setzt voraus, dass der externe Simulator mit Hilfe von
          "init_sim" (Modul "ext_koppl" in Package "sys") erfolgreich ("sim_ok" 
          ist TRUE) gestartet wurde.
          In diesem Falle erzeugt sie mit dem Modellnamen "mod_name" einen Init-
          Befehl und schickt ihn zum Simulator. Danach erzeugt sie eine Start-
          meldung mit Uhrzeit und mit dem "mod_name", sofern "mod_name" nicht 
          leer ist. Die Meldung wird ueber den Textpuffer des Packages "fbhm" 
	  ausgegeben. Dann wartet die Routine auf die Ausgaben des Simulators. 
	  Nach maximal START_SKIPS Zeilen muss der Simulator mit einer Zeile 
          antworten, die dem in INIT_OK_STRING hinterlegten Text entspricht, 
          sofern dieser String nicht leer ist. Danach wird auf das erste Prompt 
          gewartet. Wenn beides erfolgreich erhalten wurde, liefert die Routine
          TRUE, sonst FALSE.
          Die Routine wertet auch Meldungen (NO_LICENCE_STRING) ueber fehlende 
          Lizenzen aus, sofern ueberhaupt auf Strings gewartet wird, d.h.
          START_SKIPS > 0 ist.   

BOOL restart_gen_sim (void);
          Es wird versucht, den externen Simulator durch Abwuergen des Child-
          Prozesses zu terminieren und neu zu starten; ihn also in einen Zu-
          stand zu bringen, wie er nach "start_gen_sim" herrscht. 
          Wenn das glueckt, liefert die Routine TRUE sonst FALSE und "sim_up" 
          wird entsprechend gesetzt.

BOOL kleiner_gen_restart (void);
          Nach Entgegennahme eventueller (Fehler-)Meldungen wird halb so lang  
          wie beim Simulatorstart auf den Erhalt des Simulator-Prompts gewartet. 
          Bei Ausbleiben eines Prompts wird mit kurzer Wartezeit versucht, durch 
          Senden von CR einen beliebigen Prompt zu erhalten ("check_for_prompt").
          Wenn der Simulator den Prompt ohne Fehlermeldung liefert, wird "sim_
          up" TRUE, sonst FALSE. 
          Als Funktionsergebnis wird "sim_up" abgeliefert.
          ACHTUNG: Diese Routine funktioniert mit dem perlsim.pl-Script nicht, da 
                   es keine CR verarbeiten kann!

BOOL do_gen_sim (DOUBLE *erg_werte,
                  INT    *i_params,
                  DOUBLE *r_params,
		  BOOL    multi_line_call);
          Die Routine fuehrt einen Simulationslauf aus und liefert TRUE, wenn in
          "erg_werte" gueltige Werte stehen, d.h., die Simulation erfolgreich 
          durchgefuehrt werden konnte. 
          Die Eingangs- ("i_params" und "r_params") und Ausgangsparameter 
          ("erg_werte") sind bei der Routine "do_ext_sim" im Modul "ext_sim" be-
          schrieben. Die Bedeutung von "multi_line_call" wird im Modulkopf er-
          klaert.
	  Wenn die Routine FALSE liefert, kann die globale Variable "sim_up" 
          FALSE sein, sofern der Simulator sich nicht mehr mit "restart_gen_
          sim" starten liess. Bei geloeschtem "sim_up" terminiert die Routine 
          sofort und liefert FALSE.

BOOL terminate_gen_sim (BOOL  mit_quit);
          Bei gesetztem "mit_quit" wird der Simulator mit "Quit" terminiert u. 3
          mal maximal "RESTART_WAITS" Sekunden auf die Schlussmeldung gewartet.
          Wenn keine Schlussmeldung erhalten wurde oder "mit_quit" FALSE ist, 
          werden ?? und ?? per "kill_process" abgewuergt.
          Schliesst die Pipe-Streams. Liefert TRUE, wenn die Schlussmeldung er-
          halten wurde oder "kill_process" erfolgreich war.

BOOL make_gen_startsim (void);
          Erzeugt unter Benutzung von "make_startsim" ("sys"-Package) das 
	  "startsim"-File und liefert TRUE ab, wenn das File angelegt werden 
          konnte.

BOOL check_gen_err_meld (void);
          Bestimmte Meldungen aus der Error-Pipe gelten nicht als Fehlermel-
          dungen. 

BOOL till_gen_prompt (LONG  wcycles,
                      INT   max_skip,
                      BOOL  ignore_errors);
          Die Routine liest solange Zeilen (mit "wcycles" Versuchen) vom Simu-
          lator ein, bis entweder "max_skip" Zeilen oder eine Prompt-Zeile ge-
          lesen wurde(n). Wenn die Nummer im Prompt "prompt_ctr" entspricht, 
          liefert die Routine TRUE, sonst immer FALSE. Wenn "prompt_ctr" == -1 
          ist, wird die gelesene Prompt-Nr immer akzeptiert. Bei gesetztem 
          "ignore_errors" werden auch Fehlermeldungen ignoriert.
          Form des Prompts: "n>", wobei n eine Integerzahl ist.

void set_gen_data_waits (void);

BOOL init_gen_koppl (void);
          Modulinitialisierung. Liefert z.Zt. immer TRUE.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. EXTSIM_WUERG_NAME darf nur max. 8 Zeichen haben!
 2. "out_buf" vergroessert.
 3. Statt "/opt/bin/tcsh" wird die "/usr/bin/csh" in "startsim" aufgerufen.
 4. Schnittstelle fuer GEN_ML_EXT_SIMU erweitert.                    WJ 11.09.00
 5. Spezialanbindung dynamischer AKs eines Aktionstyps f.ext.Simu.   WJ 29.09.00
 6. Verallgemeinerung der Spezialanbindung fuer dynamische AKs.      WJ 21.02.01
 7. Konstante PARAM_WAITS auch im OFFLINE-Mode definiert.            WJ 09.03.01
 8. Trennzeichen bei MultilineCall dynamischer AKs von "_" nach "#". WJ 03.07.01
 9. Anpassungen an neue TSK-Datenhaltung.                            WJ 25.04.02
10. Logfilemeldung bei ExtSimu-Timeout in "start_simu_lauf".         WJ 02.07.02
11. "get_id_idx"-Aufrufe derart, dass Fehler gemeldet werden.        WJ 27.11.03
12. Linux-Portierung: System-Includefile sys/conf.h nur für Solaris. WJ 12.05.05
13. Erweiterung fuer die MatLab-Koppung: Streichung von GAMA.        WJ 22.08.07
14. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
15. Entfernung von Code im Kontext der Schalter: BGI_MODE und 
    GLEAM_TCL. Streichung von GLEAM_C. Entfernung ueberfluessiger 
    Variable. Korrektur von Formatangaben. Vermeidung uninitiali-
    sierter Variable.                                                WJ 28.06.12
16. Renovierung: Streichung des "link"-Includes. Anpassung der 
    "write_line"-Aufrufe.                                            WJ 22.04.16
17. Anpassungen an die neue TSK-Datenverwaltung.                     WJ 21.11.16
18. Nutzung der neue Variable "extSimuModelName", "extSimuRunLimit".
    Fehler in "start_simu_lauf", statische AKs, beseitigt. Schalter
    PROMPT_EVERY_ML_LINE eingefuehrt.                                WJ 16.03.17
19. Ersetzung von "akt_roh_erg_werte" durch "akt_anz_roh_erg_werte". WJ 29.05.17
20. Anpassungen an das neue Logging der Simulatorschnittstelle.      WJ 13.09.17
21. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
22. Include von "lgsw.h" anstelle von "chaindef.h".                  WJ 08.07.19
23. New big string buffer "sbuf" to avoid overflow warnings.         WJ 13.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 13.07.2020
================================================================================ */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef BS_SOLARIS
  #include <sys/types.h>
  #include <sys/conf.h>
  #include <sys/stat.h>
  #include <stropts.h>
#endif /* Solaris */

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


#undef PROMPT_EVERY_ML_LINE    /* set: prompt expected after every sent line */



/* ============================ Lokale Konstante: =========================== */
#define OUT_BUF_LEN          1024
#define MATH_PACKAGE         "" 
#define INIT_OK_STRING       "Initialized" 
#define INIT_OK_STRING_LEN   11
#define NO_LICENCE_STRING    ""
#define NO_LICENCE_STR_LEN   0
#define SIM_FIN_MELD         "Terminated"  
#define SIM_FIN_MELD_LEN     10
#define EXTSIM_WUERG_NAME    "extSimFak"                  /* max. 8 Zeichen !! alt: "perlsim." */
#define START_SIMU_STR       "StartSimu"
#define SIMU_ABORTED_MELD    "SimuAborted"
#define INIT_STR             "Init"
#define QUIT_STR             "Quit"
#define EXT_SIM_CALL         "extSimFake"                /* alt: "perlsim.pl" */
#define START_SKIPS           2
#define ERR_SKIPS            15
#define KL_RESTART_SKIPS    200 

#ifdef ONLINE
#define START_IGN_WAITS        5 sec /* 45, 10 sec */
#define INIT_PROMPT_WAITS      6 sec /* 45, 2 sec */
#define SIMU_FIRST_WAITS       7 sec /* 20 sec */
#define PARAM_WAITS            1 sec /* 10 sec */
#define SIMU_GADO_WAITS        2 sec
#define SKIP_ERR_WAITS      (0.5 sec)
#define TERM_WAITS             2 sec
#else /* not ONLINE */
#define START_IGN_WAITS        3 
#define INIT_PROMPT_WAITS      2
#define SIMU_FIRST_WAITS       4
#define PARAM_WAITS            2
#define SIMU_GADO_WAITS        2
#define SKIP_ERR_WAITS         1
#define TERM_WAITS             2
#endif /* not ONLINE */

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
static char   *out_buf;           /* Ausgabe-Puffer                           */



/* ============================ Lokale Routinen: ============================ */
static BOOL start_simu_lauf      (INT        *i_params,
                                  DOUBLE     *r_params,
		                  BOOL        multi_line_call);

static void write_to_sim         (const char *buf); 

static void simu_lauf_auswertung (DOUBLE     *erg_werte);





/* ============================= start_gen_sim ============================== */
BOOL start_gen_sim (char *mod_name)
{
   STRING_TYPE  sbuf;
   STR_TYPE     buf, zbuf, mbuf, dummy_buf;
   INT          ii;
   BOOL         tmo, ok, weiter;

   /* start_gen_sim */
   sim_up = FALSE;
   if (sim_ok)
   { /* Ext.Sim offenbar erfolgreich gestartet */
      sprintf (buf, "%s %s", INIT_STR, mod_name); 
      fprintf (tsim, "%s\n", buf); fflush(tsim);            /* init simulator */
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "ToSim: %s", buf);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
      get_time_stamp (dummy_buf, dummy_buf, dummy_buf, zbuf);
      if ((INT)(strlen (mod_name)) > 0)
	 sprintf (mbuf, SIMUSTART_TXT2, mod_name);
      else
	 strcpy (mbuf, "");
      sprintf (sbuf, SIMUSTART_TXT1, zbuf, mbuf);
      write_buf (1, sbuf);
      weiter = TRUE;
      ok     = TRUE;
      ii     = 0;
      while (weiter && ok && (ii < START_SKIPS))
      { /* Warten auf Init-String */
	 if (ok = wait_for_reply (&tmo, START_IGN_WAITS))
	 { /* Zeile erhalten: analysieren */
	    if (INIT_OK_STRING_LEN > 0)
	       if (strncmp (inbuffer, INIT_OK_STRING, INIT_OK_STRING_LEN) == 0)
	       { /* Init-String erkannt */
		  sim_up = TRUE;
		  weiter = FALSE;
	       } /* Init-String erkannt */
	    if (NO_LICENCE_STR_LEN > 0)
	       if (strncmp (inbuffer, NO_LICENCE_STRING, 
			    NO_LICENCE_STR_LEN) == 0)
	       { /* Keine Lizenz mehr verfügbar */
		  weiter = FALSE;
		  write_buf (get_line_anz()+1, NO_LICENCE_TXT);
	       } /* Keine Lizenz mehr verfügbar */
	 } /* Zeile erhalten: analysieren */
	 ii++;
      } /* Warten auf Init-String */
      if (sim_up || (START_SKIPS == 0))
      { /* Init-String nach ignorierten Zeilen erhalten od. keine Initstrings */
	 prompt_ctr = -1;                        /* akzeptiere jede Prompt-Nr */
	 sim_up     = till_gen_prompt (INIT_PROMPT_WAITS, 5, STOP_ON_ERROR);
	 prompt_ctr++;
      } /* Init-String nach ignorierten Zeilen erhalten od. keine Initstrings */
   } /* Ext.Sim offenbar erfolgreich gestartet */
   run_ctr = 0;
   return (sim_up);
} /* start_gen_sim */



/* ============================ restart_gen_sim ============================= */
BOOL restart_gen_sim (void)
{  

   /* restart_gen_sim */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "%s RESTART_GEN_SIM %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   sim_up = FALSE;
#ifdef ONLINE
   if (kill_process (EXTSIM_WUERG_NAME))
   { /* ExtSim abgewuergt */
      init_sim ();                           /* ext. Simulator erneut starten */
  #ifdef IO_LOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "RESTART_GEN_SIM: Termination Done. "
		 "INIT_SIM: sim_ok =%d\n", sim_ok);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
  #endif /* IO_LOG */
      start_gen_sim ("");
   } /* ExtSim abgewuergt */
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
      sprintf(simuLogBuf, "RESTART_GEN_SIM: sim_up=%d", sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   return (sim_up);
} /* restart_gen_sim */



/* ========================== kleiner_gen_restart =========================== */
BOOL kleiner_gen_restart (void)
{
   /* kleiner_gen_restart */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "%s KLEINER_GEN_RESTART %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   sim_up = FALSE;
   if (till_gen_prompt((START_IGN_WAITS)/2, KL_RESTART_SKIPS, IGNORE_ERRORS))
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
      sprintf(simuLogBuf, "KLEINER_GEN_RESTART: sim_up=%d", sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* MITSIMULOG */
   return (sim_up);
} /* kleiner_gen_restart */



/* =============================== do_gen_sim =============================== */
BOOL do_gen_sim (DOUBLE *erg_werte,
                 INT    *i_params,
                 DOUBLE *r_params,
	         BOOL    multi_line_call)
{
   STR_TYPE  zeit_buf;
   DOUBLE    simu_secs = 0.0;                    /* Simu-Laufzeit in Sekunden */
   INT       ii;

   /* do_gen_sim */
#ifdef TEST_SIM
   ak_bewertet = TRUE;
   erg_werte[0] = frand() * 3000;
   erg_werte[1] = erg_werte[0];
   erg_werte[2] = frand() * 20;
   erg_werte[3] = erg_werte[2]; 
   erg_werte[4] = frand(); 
   printf (MSG_LINE_TXT, "Achtung: ExtSim-Koppl im Testmodus! KEIN Aufruf des Ext.Sim.!");
#else  /* --------------- Ankopplung des externen Simulators: --------------- */
   ak_bewertet = FALSE;
   if (run_ctr > extSimuRunLimit)
   { /* Simulator reinitialisieren */
      if (sim_up)
	 terminate_gen_sim (TRUE);
      restart_gen_sim ();
   } /* Simulator reinitialisieren */
   if (sim_up)
   { /* Simulator verfuegbar */
      restart   = FALSE;
      simu_wdh  = TRUE;
      simu_secs = 0.0;
      wait_ctr  = 0;
      run_ctr++;
      if (start_simu_lauf (i_params, r_params, multi_line_call))
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
	    fprintf(simu_logfile, "Simu 1.Lauf: RZ=%6.2f  ak_bewert=%d  restart=%d  simu_wdh=%d"
		    "  sim_up=%d\n", simu_secs, ak_bewertet, restart, simu_wdh, sim_up);
	    if (restart || !brief_log
#ifdef DETAILED_LOG
		|| ak_bewertet
#endif
		)
	    { /* Eingangs-Parameter ausgeben */
	       fprintf (simu_logfile, "             Params:");
	       for (ii = i_par_anf;  ii < all_iwerte_anz;  ii++)
		  fprintf (simu_logfile, " %d", i_params[ii]);
	       for (ii = 0;  ii < akt_rpar_anz;  ii++)
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
	    meldung(SIMU_GENEXTSIM, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
      } /* Logging */
#endif /* MITSIMULOG */

      /* --------------------  Restart und 2.Simu-Lauf: --------------------- */
      if (restart)
      { /* restart external simulator */
/*         if (!kleiner_gen_restart ())   kleiner_gen_restart geht nicht */
	    restart_gen_sim ();
      } /* restart external simulator */

      if (sim_up && simu_wdh)
      { /* Simulator (wieder) bereit: Lauf wiederholen */
	 restart   = FALSE;
	 simu_wdh  = FALSE;
	 simu_secs = 0.0;
	 wait_ctr  = 0;
	 if (start_simu_lauf (i_params, r_params, multi_line_call))
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
	       fprintf(simu_logfile, "Simu 2.Lauf: RZ=%6.2f  ak_bewert=%d  "
		       "restart=%d  simu_wdh=%d  sim_up=%d\n", simu_secs, 
		       ak_bewertet, restart, simu_wdh, sim_up);
	       if (restart || !brief_log
#ifdef DETAILED_LOG
		   || ak_bewertet
#endif
		   )
	       { /* Eingangs-Parameter ausgeben */
		  fprintf (simu_logfile, "             Params:");
		  for (ii = i_par_anf;  ii < all_iwerte_anz;  ii++)
		     fprintf (simu_logfile, " %d", i_params[ii]);
		  for (ii = 0;  ii < akt_rpar_anz;  ii++)
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
	       meldung(SIMU_GENEXTSIM, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
	 } /* Logging */
#endif /* MITSIMULOG */
	 if (restart)
	 { /* restart external simulator */
/*	    if (!kleiner_gen_restart ())   "kleiner_gen_restart" geht nicht!! */
	       restart_gen_sim ();
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
} /* do_gen_sim */



/* ============================ start_simu_lauf ============================= */
static BOOL start_simu_lauf (INT    *i_params,
                             DOUBLE *r_params,
                             BOOL    multi_line_call)
/*----------------------------------------------------------------------------*/
/*   Startet mit den Eingabeparametern "i_params" und "r_params" einen Simu-  */
/*   latorlauf und wartet die Rueckmeldung ab. Liefert TRUE, wenn diese ohne  */
/*   Fehlermeldung der Error-Pipe innerhalb einer vorgegebenen Wartezeit er-  */
/*   halten wurde.                                                            */
/*   Bei gesetztem "multi_line_call" werden die Werte pro Zeile uebergeben,   */
/*   Details siehe Modulkopf.                                                 */
/*   Die Parameter werden mit "erg_genauigkeit" Stellen im f-Format ueber-    */
/*   geben.                                                                   */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE            buf;
   ACT_DESCR_PTR_TYPE  act_descr;
   INT                 ii, jj, ai, par_ctr, rpar_ctr, akt_anz;
   BOOL                dummy,
                       ok = TRUE;

   /*  start_simu_lauf */
   strcpy (out_buf, extSimuModelName);
   strcat (out_buf, " ");
   if (multi_line_call)
      write_to_sim (out_buf);
   if (dyn_aks)
   { /* dynamische AKs: Int/Real-Params, Aktions-Reihenfolge in AK */
      akt_anz = i_params[0];
      for (ii = 1;  (ii <= akt_anz) && ok;  ii++)
      { /* Aktionscodes schreiben */
	 sprintf (buf, "Akt#%1d=%d ", ii, i_params[ii]);
	 if (multi_line_call)
	 { /* mehrzeilige Paraemteruebergabe */
	    write_to_sim (buf);
#ifdef PROMPT_EVERY_ML_LINE
	    ok = till_gen_prompt (PARAM_WAITS, 0, STOP_ON_ERROR);
	    prompt_ctr++;
#endif /* PROMPT_EVERY_ML_LINE */
	 } /* mehrzeilige Paraemteruebergabe */
	 else
	    strcat (out_buf, buf);
      } /* Aktionscodes schreiben */
      par_ctr  = i_par_anf;
      rpar_ctr = 0;
      for (ii = 1;  (ii <= akt_anz) && ok;  ii++)
      { /* Aktionsparameter schreiben */
	 ai = i_params[ii];
	 if (get_act_descr (&act_descr, (SHORT)(ai), WITHOUT_GENECODE_CHECK) == GLEAM_OK)
	 { /* Zugriff ok */
	    for (jj = 0;  (jj < int_pro_aktion[ai]) && ok;  jj++)
	    { /* Alle Ints dieser Aktion */
	       sprintf (buf, "%s#%1d=%d ", 
			act_descr->i_par_descr[jj].par_name, ii,
			i_params[par_ctr++]);
	       if (multi_line_call)
	       { /* mehrzeilige Paraemteruebergabe */
		  write_to_sim (buf);
#ifdef PROMPT_EVERY_ML_LINE
		  ok = till_gen_prompt (PARAM_WAITS, 0, STOP_ON_ERROR);
		  prompt_ctr++;
#endif /* PROMPT_EVERY_ML_LINE */
	       } /* mehrzeilige Paraemteruebergabe */
	       else
		  strcat (out_buf, buf);
	    } /* Alle Ints dieser Aktion */
	    for (jj = 0;  (jj < real_pro_aktion[ai]) && ok;  jj++)
	    { /* Alle Reals dieser Aktion */
	       sprintf (buf, "%s#%1d=%.*f ", 
			act_descr->r_par_descr[jj].par_name, ii,
			erg_genauigkeit-1, r_params[rpar_ctr++]);
	       if (multi_line_call)
	       { /* mehrzeilige Paraemteruebergabe */
		  write_to_sim (buf);
#ifdef PROMPT_EVERY_ML_LINE
		  ok = till_gen_prompt (PARAM_WAITS, 0, STOP_ON_ERROR);
		  prompt_ctr++;
#endif /* PROMPT_EVERY_ML_LINE */
	       } /* mehrzeilige Paraemteruebergabe */
	       else
		  strcat (out_buf, buf);
	    } /* Alle Reals dieser Aktion */
	 } /* Zugriff ok */
	 else
	    ok = FALSE;
      } /* Aktionsparameter schreiben */
   } /* dynamische AKs: Int/Real-Params, Aktions-Reihenfolge in AK */
   else
   { /* statische AKs: erst Ints, dann Reals in der AktDefReihenfolge */
      par_ctr = 0;
      if (i_par_ges_anz > 0)
      { /* Es gibt Integer-Parameter */
	 for (ai = 1;  (ai <= last_act) && ok;  ai++)
         { /* alle Integer-Parameter */
	    if (get_act_descr (&act_descr, ai, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
	    { /* Zugriff ok */
	       for (ii = 0;  (ii < act_descr->i_par_anz) && ok;  ii++)
	       { /* einen Integer-Parameter */
		  sprintf (buf, "%s=%d ", act_descr->i_par_descr[ii].par_name, 
			   i_params[par_ctr]);
		  if (multi_line_call)
	          { /* mehrzeilige Paraemteruebergabe */
		     write_to_sim (buf);
#ifdef PROMPT_EVERY_ML_LINE
		     ok = till_gen_prompt (PARAM_WAITS, 0, STOP_ON_ERROR);
		     prompt_ctr++;
#endif /* PROMPT_EVERY_ML_LINE */
		  } /* mehrzeilige Paraemteruebergabe */
		  else
		     strcat (out_buf, buf);
		  par_ctr++;
		  ok &= par_ctr <= i_par_ges_anz;
	       } /* einen Integer-Parameter */
	    } /* Zugriff ok */
	    else
	       ok = FALSE;
	 } /* alle Integer-Parameter */
	 if (par_ctr != i_par_ges_anz)
         { /* I-Param-Anz stimmt nicht */
	    fatal (SIMU_GENEXTSIM, 2, par_ctr, i_par_ges_anz, "", BAD_PAR_AMOUNT);
	    ok = FALSE;
	 } /* I-Param-Anz stimmt nicht */
      } /* Es gibt Integer-Parameter */
      if ((r_par_ges_anz > 0) && ok)
      { /* Es gibt Real-Parameter */
	 par_ctr = 0;
	 for (ai = 1;  (ai <= last_act) && ok;  ai++)
	 { /* alle Real-Parameter */
	    if (get_act_descr (&act_descr, ai, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
	    { /* Zugriff ok */
	       for (ii = 0;  (ii < act_descr->r_par_anz) && ok;  ii++)
	       { /* einen Real-Parameter */
		  sprintf (buf, "%s=%.*f ", act_descr->r_par_descr[ii].par_name, 
			   erg_genauigkeit-1, r_params[par_ctr]);
		  if (multi_line_call)
	          { /* mehrzeilige Paraemteruebergabe */
		     write_to_sim (buf);
#ifdef PROMPT_EVERY_ML_LINE
		     ok = till_gen_prompt (PARAM_WAITS, 0, STOP_ON_ERROR);
		     prompt_ctr++;
#endif /* PROMPT_EVERY_ML_LINE */
		  } /* mehrzeilige Paraemteruebergabe */
		  else
		     strcat (out_buf, buf);
		  par_ctr++;
		  ok &= par_ctr <= r_par_ges_anz;
	       } /* einen Real-Parameter */
	    } /* Zugriff ok */
	    else
	       ok = FALSE;
	 } /* alle Real-Parameter */
	 if (par_ctr != r_par_ges_anz)
         { /* R-Param-Anz stimmt nicht */
	    fatal (SIMU_GENEXTSIM, 3, par_ctr, r_par_ges_anz, "", BAD_PAR_AMOUNT);
	    ok = FALSE;
	 } /* R-Param-Anz stimmt nicht */
      } /* Es gibt Real-Parameter */
   } /* statische AKs: erst Ints, dann Reals in der AktDefReihenfolge */

   if (ok)
   { /* Start eines Simulatorlaufs */
      if (multi_line_call)
	 write_to_sim (START_SIMU_STR);
      else
	 write_to_sim (out_buf);
      ok = wait_for_reply (&dummy, simu_first_waits); /* 1.Rueckmldg abwarten */

#ifdef MITSIMULOG
      if (!ok && mit_simulog) {
	 sprintf(simuLogBuf, "*** ExtSimuError. Present timeout value: %ld sec", 
		 (LONG)(simu_first_waits / (1 sec)));
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
   } /* Start eines Simulatorlaufs */          
   return (ok);
} /*  start_simu_lauf */



/* ========================== simu_lauf_auswertung ========================== */
static void write_to_sim (const char *buf)
/*----------------------------------------------------------------------------*/
/*   Schreibt den "buf" in die tsim-Pipe.                                     */
/*----------------------------------------------------------------------------*/
{ /* write_to_sim */
   fprintf (tsim, "%s\n", buf);
   fflush(tsim);
#ifdef IO_LOG
   if (mit_simulog)
   { /* Logging */
      sprintf(simuLogBuf, "ToSim: %s", buf);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   } /* Logging */
#endif /* IO_LOG */
} /*  write_to_sim */



/* ========================== simu_lauf_auswertung ========================== */
static void simu_lauf_auswertung (DOUBLE *erg_werte)
/*----------------------------------------------------------------------------*/
/*   Die Routine kommuniziert ueber "restart" und "ak_bewertet" mit "do_      */
/*   ext_sim". Sie erwartet die Simulationsergebnisse zeilenweise und wertet */
/*   sie hinsichtlich Fehlermeldungen und Prompt-Nr aus. Positivenfalls wird  */
/*   "ak_bewertet" TRUE gesetzt und in "erg_werte" das Ergebnis hinterlegt.   */
/*----------------------------------------------------------------------------*/
{
   BOOL  werte_ok, prompt_ok;
   INT   ii;

   /* simu_lauf_auswertung */
   if (strcmp (inbuffer, SIMU_ABORTED_MELD) == 0)
   { /* "SimuAborted"-Meldung */
      simu_wdh  = FALSE;                                   /* Unbewertbare AK */
      prompt_ok = till_gen_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				   STOP_ON_ERROR);
      if (!prompt_ok)
	 prompt_ok = check_for_prompt (SKIP_ERR_WAITS, ERR_SKIPS, 
				       EXPECT_NEW_PROMPT);
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 sprintf(simuLogBuf, "*** Simu: \"%s\"-Meldung erhalten.  "
		  "Erwarteten Prompt %ld erhalten: %d", 
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
	 prompt_ok = till_gen_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
				      STOP_ON_ERROR);
	 ak_bewertet = werte_ok && prompt_ok;
	 simu_wdh    = !ak_bewertet;
#ifdef MITSIMULOG
	 if (mit_simulog && simu_wdh)
	 {
	    sprintf(simuLogBuf, "*** Simu Werte- od.Promptfehler: Erwarte"
		     "ten Prompt %ld erhalten:%d  Gelesene Werte: OK=%d Ist=%d" 
		     " Soll=%d", prompt_ctr, prompt_ok, werte_ok, ii, 
		     akt_anz_roh_erg_werte);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 }
#endif /* MITSIMULOG */
      } /* 1.Wert erhalten */
      else
      { /* Schrott erhalten */
	 prompt_ok = till_gen_prompt (simu_data_waits, akt_anz_roh_erg_werte, 
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



/* =========================== terminate_gen_sim ============================ */
BOOL terminate_gen_sim (BOOL  mit_quit)
{
   BOOL  reply_received = FALSE, 
         out_of_time,
         ext_sim_tot,
         erg;

   /* terminate_gen_sim */
   ext_sim_tot = FALSE;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "%s TERMINATE_GEN_SIM: %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
      if (mit_quit) {
	 sprintf(simuLogBuf, "ToSim: %s", QUIT_STR);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
   }
#endif /* MITSIMULOG */
#ifdef ONLINE
   if (mit_quit)
   { /* ExtSim mit "quit" terminieren */
      fprintf (tsim, "%s\n", QUIT_STR); fflush(tsim);  /* terminate simulator */
      /* -------------- Versuche, auf SIM_FIN_MELD zu warten: --------------- */
      if (wait_for_reply (&out_of_time, TERM_WAITS))
      { /* 1.Antwort erhalten */
	 if (strlen (inbuffer) == 0)
	    wait_for_reply (&out_of_time, TERM_WAITS);  /* skip blank line */
	 if (strncmp (inbuffer, SIM_FIN_MELD, SIM_FIN_MELD_LEN) == 0)
	    reply_received = TRUE;
	 else
	 { /* Nicht erhalten: 2.Versuch */
	    if (wait_for_reply (&out_of_time, TERM_WAITS)) 
	       reply_received = (strncmp (inbuffer, SIM_FIN_MELD, 
					  SIM_FIN_MELD_LEN) == 0);
	 } /* Nicht erhalten: 2.Versuch */
      } /* 1.Antwort erhalten */
   } /* ExtSim mit "quit" terminieren */
   if (!reply_received)
   { /* Noch nicht tot ! */
      ext_sim_tot = kill_process (EXTSIM_WUERG_NAME);
   } /* Noch nicht tot ! */
   close (pipe_p2s[1]);
   close (pipe_s2p[0]);
   close (pipe_se2p[0]);
   erg    = reply_received || ext_sim_tot;
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
      sprintf(simuLogBuf, "TERMINATE_GEN_SIM: reply_received=%d", reply_received);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
      if (reply_received) {
	 sprintf(simuLogBuf, "  ext_sim_tot=%d\n", ext_sim_tot);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
   }
#endif /* MITSIMULOG */
   return (erg);
} /* terminate_gen_sim */



/* =========================== make_gen_startsim ============================ */
BOOL make_gen_startsim (void)
{
   return (make_startsim ("#!/bin/bash", "", akt_sim_mod_path, 
			  "", "", EXT_SIM_CALL));
} /* make_gen_startsim */



/* =========================== check_gen_err_meld =========================== */
BOOL check_gen_err_meld (void)
{
   return (strlen(errbuffer) > 0);
} /* check_gen_err_meld */



/* ============================ till_gen_prompt ============================= */
BOOL till_gen_prompt (LONG  wcycles,
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

   /* till_gen_prompt */
   do
   { /* Hauptschleife */
      if (wait_for_reply (&tmo, wcycles))
      { /* Zeile erhalten: analysieren */
         len = strlen (inbuffer);
         if (len > 1)
	 { /* ">" suchen, Nummer lesen und pruefen */
	    pos = strcspn (inbuffer, ">");
	    if (pos < len)
	    { /* ">" gefunden */
	       weiter = FALSE;                         /* Prompt erkannt   */
	       strncpy (nr_buf, inbuffer, pos);
	       nr_buf [pos] = '\0';                    /* terminate string */
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
	 } /* ">" suchen, Nummer lesen und pruefen */
      } /* Zeile erhalten: analysieren */
      else
	 weiter = ignore_errors && !tmo; /* FMeld von Simu oder keine Antwort */
      skipped_lines++;
   } /* Hauptschleife */
   while (weiter && (skipped_lines < max_skip));
   return (ok);
} /* till_gen_prompt */



/* =========================== set_gen_data_waits =========================== */
void set_gen_data_waits (void)
{
   simu_data_waits = SIMU_GADO_WAITS;
} /* set_gen_data_waits */



/* ============================= init_gen_koppl ============================= */
BOOL init_gen_koppl (void)
{
   wait_ctr         = 0;
   simu_first_waits = SIMU_FIRST_WAITS;
   set_gen_data_waits ();
   return (NULL != (out_buf = (char*)malloc(OUT_BUF_LEN * sizeof(char))));
} /* init_gen_koppl */
