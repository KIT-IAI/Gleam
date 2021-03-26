/*******************************************************************************
GLEAM/AE                                                                  V1.5.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: EXTerne SIMulatoren anbindung
 
Package: simu                   File: ext_sim.c   
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt das Interface zu den externen Simulatoren und die Basisrouti-
nen zu deren Ankopplung. 

Derzeit sind folgende externe Simulatoren integriert:
  - ELDO/Anacad   "eldokopl"  Eldo
  - mathematica   "mathkopl"  Mathematica
  - allg.ext.Simu "gen_kopl"  GenExtSim bzw. GenMLExtSim


Der externe Simulator kann mit unterschiedlichen Simulationsmodellen betrieben 
werden. Dazu wird ein Pfad genutzt, in dem das Simulationsmodell steht. In den 
Variablen "sim_root_dir_name", "extSimSubDir" und dem zu SIMU_MOD_NAME_IDX ge-
hoerigen Eintrag in der Programmparameterliste ("paramList" aus "aufg/tsk_data.c") 
stehen das Modell-Root-Directory, das optionale Simulator-Unterverzeichnis und 
das Modell-Directory. Der daraus zu bildende Pfad zum aktuellen Simulationsmodell 
wird in "akt_sim_mod_path" hinterlegt und hat folgenden Aufbau:
    <sim_root_dir_name>/<extSimSubDir>/<paramList entry>  
Webb der String "extSimSubDir" leer ist, entfaellt "<extSimSubDir>/".

Der Simulator wird nur noch bei Bedarf initialisiert und hochgefahren. Dies kann
bei interaktivem Start oder bei Verlassen des Evo-Menues, wenn Jobs vorliegen 
aber Evo nicht bereits aktiv ist, der Fall sein. Obige Namens-Variable koennen 
durch manuelle Aenderung oder durch Laden eines MOD-Files veraendert werden. 
Dazu folgende Tabelle:
                                  Neuer
Aktion:            |     Init   | Name  |   Simu-Bedarf    |Manuell hochfahren|
-------------------+------------+-------+------------------+------------------+
Name setzten       |aus Environ-|   -   |        -         |        -         |
                   |ment/Default|       |                  |                  |
Name pruefen       |ja/nur Meldg|   -   |        -         |        -         |
Name pruefen und   |            |       |                  |                  |
  setzen wenn ok   |     -      |  ja   |        -         |        -         |
                   |            |       |                  |                  |
"akt_sim_mod_path" |   clear    |   -   |bilden u.vgl      |bilden u.vgl      |
                   |            |       |  --> delta       |  --> delta       |
                   |            |       |                  |                  |
startsim-File      |     -      |   -   |wenn delta, neuen |wenn delta, neuen |
                   |            |       |Pfad bilden u.prue|Pfad bilden u.prue|
                   |            |       |fen u.wenn ok     |fen u.wenn ok     |
                   |            |       |File erzeugen     |File erzeugen     |
                   |            |       |                  |                  |
ExtSimu terminieren|     -      |   -   |wenn delta und ok |ja, soweit aktiv  |
                   |            |       |und Simu aktiv    |                  |
                   |            |       |                  |                  |
Prozess abforken   |     -      |   -   |}                 |}                 |
  (init_sim)       |            |       |} wenn            |} wenn            |
                   |            |       |} Simu            |} Simu            |
Modell-Init        |     -      |   -   |} down            |} down            |
  (start_xxxx_sim) |            |       |}                 |}                 |

Waehrend des Simulatorlaufs koennen (Fehler-)Meldungen entstehen, wenn Meldungen
ueber die Error-Pipe kommen oder bei gesetztem "mit_rz" (Rechenzeitmeldungen).

Ein bei Programmende noch aktiver externer Simulator wird durch die Exit-Routine
"terminate_ext_simu", die von "init_ext_koppl" aktiviert wird, terminiert.
 

Die Routinen im einzelnen:

Packages-Export der allgemeinen Schnittstelle zu externen Simulatoren:

BOOL do_ext_sim (DOUBLE *erg_werte,
                 INT    *i_params,
                 DOUBLE *r_params);
          Die Routine fuehrt einen Simulationslauf aus und liefert TRUE, wenn in
          "erg_werte" gueltige Werte stehen, d.h., die Simulation erfolgreich 
	  durchgefuehrt werden konnte. In "i_params" und "r_params" stehen die 
          (Eingabe-)Parameter des Simulationslaufs, deren Anzahl jeweils "i_par_
          ges_anz" bzw. "r_par_ges_anz" betraegt. Die von der aktuellen Anwendung 
          erwartete Anzahl der Ergebniswerte steht in "akt_anz_roh_erg_werte". 
	  Diese Mengenangaben werden beim Laden des MOD-Files durch Modul "mod_ 
          data" gesetzt. Wenn die Routine FALSE liefert, kann die globale Vari-
          able "sim_up" FALSE sein, sofern der Simulator sich nicht mehr mit 
          "restart_xxxx_sim" starten lies. Bei geloeschtem "sim_up" terminiert 
          die Routine sofort und liefert FALSE ab.

BOOL ext_simu_hochfahren (BOOL  mit_fin);
	  Fuehrt "prep_startsim" aus und terminiert den Simulator wenn entweder
	  "mit_fin" gesetzt ist oder ein neues SimuModell benutzt werden soll 
          (d.h., dass "prep_startsim" TRUE lieferte) und der externe Simulator 
	  aktiv ist. Danach wird der externe Simulator gestartet, sofern er 
	  nicht mehr aktiv ist. Die Routine liefert TRUE, wenn der externe Simu-
          lator wieder laueft.

void update_ext_simu_tmo (LONG  tmo_zeit_in_sec);
	  "sec" werden in interne Tics umgewandelt und in "simu_first_waits" 
	  gespeichert.

void terminate_ext_simu (void);
          Die Routine prueft, ob die aktuelle Anwendung einen externen Simulator
          benutzt und ob dieser hochgefahren ist. Wenn ja, wird er terminiert. 
          Ueber das Resultat der Terminierung erfolg eine entsprechende Meldung.
          Die Routine wird auch als Exithandler genutzt.


Package-lokale Routinen der allgemeinen Schnittstelle zu externen Simulatoren,
wobei "restart_ext_sim" und "kleiner_ext_restart" bei GUI_MODE zum Package-Export 
gehoeren:

void ext_simu_recovery (void);
          Die Routine fuehrt ein zyklisches Recovery des externen Simulators
          durch bis er entweder erfolgreich gestartet werden konnte oder 
	  RECOVER_TRIALS Versuche fehlschlugen. Dabei wird von Versuch zu Ver-
          such laenger gewartet, indem nach jedem erfolglosen Versuch RECOVER_
	  DELAY_INCR msec laenger gewartet wird.

BOOL restart_ext_sim (void);
          Es wird versucht, den externen Simulator zu terminieren und in einen 
          Zustand zu bringen, wie er nach "start_ext_sim" herrscht. Wenn das ge- 
          lingt, liefert die Routine TRUE sonst FALSE und "sim_up" wird entspre-
          chend gesetzt.

void kleiner_ext_restart (void);
          Es wird versucht, den externen Simulator in einen Zustand zu bringen, 
          wie er nach der Initialisierung durch "start_ext_sim" herrscht ohne 
          ihn zu terminieren. "sim_up" wird TRUE, wenn dies gelingt.


Package-lokale Basisroutinen zur Kommunikation mit den externen Simulatoren:

BOOL test_and_read_val (DOUBLE *wert);
          Testet, ob der sich im "inbuffer" befindliche String eine Zahl in Ex-
          ponentenschreibweise bzw. eine gueltige Real-Zahl ist und wandelt sie 
          in ein DOUBLE um. Liefert positivenfalls TRUE, sonst FALSE.

BOOL read_line (INT   in_pipe,         Online-Version
                char *buf);      
BOOL read_line (FILE *in_file,         Offline-Version
                char *buf);
          Liest eine Zeile aus der "in_pipe" und liefert sie in "buf" ohne CR
          als null-terminierter String ab. Die Routine liefert TRUE, wenn etwas 
          gelesen werden konnte. 
          Die Online-Version liest von den Kommunikationspipes, die Offline-
          Version aus Dateien.

BOOL wait_for_reply (BOOL *time_out,
                     LONG  wcycles);
          Die Routine liest je eine Zeile aus der Ausgabe- und der Error-Pipe
          des externen Simulators in die Puffer "inbuffer" und "errbuffer". Sie 
          terminiert, wenn die "wcycles" Wartezyklen aufgebraucht sind oder eine
          Antwort oder eine Fehlermeldung vorliegt. Sie liefert TRUE, wenn keine
          Fehlermeldung vorliegt und innerhalb von "wcycles" Lesezyklen eine 
          Zeile eingelesen werden konnte. 
          Je nach benutztem externen Simulator werden bestimmte Meldungen in der
          Error-Pipe nicht als Fehlermeldungen interpretiert.
          Alle Fehlermeldungen werden in die FMELD_LINE ausgegeben, alle anderen
          Meldungen der Error-Pipe in die EVENT_LINE. Antworten, die nur aus 
          einem Blank oder einer Leerzeile bestehen, werden ignoriert. 
          "time_out" wird TRUE, wenn innerhalb der Zeit nichts gelesen wurde.

BOOL check_for_prompt (LONG  wcycles,
                       INT   err_skips,
                       BOOL  ident_prompt);
          Die Routine versucht, durch Senden von CR ein Prompt entsprechend dem
          aktuellen "promt_ctr" zu erhalten. Wenn "ident_prompt" TRUE ist, wird
          die gleiche Promptnummer wie beim letzten Prompt erwartet, d.h. 
          "promt_ctr - 1", ansonsten wird die nachfolgende Promptnummer erwar-
          tet. Wenn das nicht klappt wird nach erneutem Senden eines CR jedes 
          Prompt akzeptiert. Fehlermeldungen werden ignoriert. Jedesmal werden 
          bis zu max. "err_skips" Zeilen ueberlesen. "wcycles" gibt die maximale
          Anzahl der Leseversuche pro Zeile an. 
          Die Routine liefert TRUE, wenn ein Prompt erhalten wurde.

BOOL check_for_pid (INT  *prog_pid,
                    char *prog_name);
          Ermittelt den zu "prog_name" gehoerigen PID und liefert ihn in "prog_
          pid" ab. In diesem Fall liefert die Routine TRUE. Wenn kein Programm 
          mit "prog_name" existiert, liefert sie FALSE. 

BOOL kill_process (char *prog_name);
          Versucht maximal 2 mal den durch "prog_name" bezeichneten Prozess zu 
          terminieren. Wenn das gut ging, liefert die Routine TRUE, sonst FALSE.

BOOL init_ext_koppl (BOOL *ext_sim_mod_ok);
          Initialisierung der Datenstrukturen der Ankopplung externer Simulato-
          ren unter Benutztung der simulator-spezifischen Initialisierungsrouti-
          nen "init_xxxx_koppl". Etablierung des Exithandlers "terminate_ext_
	  simu". Liefert TRUE, wenn alles gut ging.
          "ext_sim_mod_ok" wird auf TRUE gesetzt, wenn der Modellpfad fuer den
          externen Simulator korrekt ist oder wenn kein ext. Simulator benutzt 
          wird. Bei fehlerhaftem Modellpfad erfolgen Fehlermeldungen.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Integration der allgemeinen Schnittstelle zu externen Simulatoren.
 2. Fuer HyGLEAM die Deklarationen von "do_ext_sim" globalisiert.    WJ 11.08.00
 3. Schnittstelle fuer GEN_ML_EXT_SIMU erweitert.                    WJ 11.09.00
 4. Fehlernummern korrigiert.                                        WJ 04.10.00
 5. Anpassungen in "init_ext_koppl" an neue TSK-Datenhaltung.        WJ 02.07.02
 6. "get_id_idx"-Aufrufe derart, dass Fehler gemeldet werden.        WJ 27.11.03
 7. Linux-Portierung: Aus "round" wird "l_round". System-Includefile 
    sys/conf.h nur fuer Solaris.                                     WJ 12.05.05
 8. Erweiterung fuer die MatLab-Koppung: Streichung von GAMA.        WJ 22.08.07
 9. "check_mod_names" und "start_ext_sim" werden lokale Routinen.  
    Dummy-Routine "mlOnlineVisu" und Init von "mitOnlVisu".          WJ 24.08.07
10. Neue Kennungsvariable "mitMatLabSimu".                           WJ 02.11.07
11. Erweiterung der Parameterliste von "mlOnlineVisu".               WJ 28.07.08
12. Neue Routine "mlSaveOptRes" aus Kompatibilitaetsgruenden.        WJ 24.08.10
13. Entfernung der Matlab-Kopplung "mlSaveOptRes", "mlOnlineVisu", 
    "mitOnlVisu" und "mitMatLabSimu". Entfernung ueberfluessiger 
    Variable. Korrektur von Formatangaben. Explizite Klammern zur 
    eindeutigen Zuordnung von else-Zweigen.                          WJ 28.06.12
14. Renovierung: Korrektur der system-Aufrufe. Streichung des 
    "link"-Includes.                                                 WJ 10.05.16
15. "restart_ext_sim" und "kleiner_ext_restart" werden bei GUI_MODE 
    zum Package-Export.                               
16. Anpassungen an die neue TSK-Datenverwaltung.                     WJ 22.11.16
17. Nutzung der neue Variable "extSimuModelName", "extSimuRunLimit".
    Ersetzung von "sim_mod_art_name" durch "extSimSubDir". Der kann
    leer sein, was im Gegensatz zu GADO keine indirekte Steuerungs-
    funktion mehr hat. "check_for_pid" blockiert mittlerweile das 
    Program. Loesung: Erweiterung des "type"-Parameters bei popen() 
    um ein "e" und Entfernung von "pclose(qpipe)".                   WJ 15.03.17
18. Integration von matlab/matpoer: "doMatlabSim" als Dummy-Routine. WJ 02.05.17
19. Anpassungen an das neue Logging der Simulatorschnittstelle.      WJ 13.09.17
--------------------------------------------------------------------------------
Status: o.k.                    Autor: W.Jakob                 Datum: 13.09.2017
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
#include "chaindef.h"
#include "hmod.h"
#include "aufg.h"
#include "simuInt.h"
#include "simu.h"

#include SIMU_TEXTS



/* ======================== Package-globale Variable: ======================= */
char       *inbuffer;                /* Daten vom ExtSimu         [BUF_LEN+1] */
char       *errbuffer;               /* Fehlermeldungen d.ExtSimu [BUF_LEN+1] */
DOUBLE      to_tics          = 1;    /* Umrechnungsfaktor fuer Simu-Wartezeit */
LONG        prompt_ctr,              /* Nummer des nachsten erwarteten Prompt */
            wait_ctr,                /* Zaehlt Wartezyklen. Global f."mit_rz" */
            simu_first_waits = 4;    /* Wartezeit auf Simulatorantwort [tics] */
BOOL        ak_bewertet,             /* True, wenn AK erfolgreich bewertet    */
            detailed_iolog = FALSE;  /* Steuert Detailierungsgrad des IO_LOG  */



/* ============================ Lokale Konstante: =========================== */
#define PROC_CHECK_STRING    "ps -el | grep "
#define KILL_STRING          "kill -9"
#define BUF_LEN                120    /* Kommunikationspuffer zum extSimu     */
#define RECOVER_TRIALS          50    /* Achtung:                             */
#define RECOVER_DELAY_INCR    1000    /* RECOVER_DELAY_INCR * REC_GADO_START_ */
#define REC_GADO_START_DELAY   500    /* DELAY + REC_xxxx_START_DELAY < 65535 */
#define KILL_WAIT_MSEC         200




/* ============================ Lokale Variable: ============================ */
static char  e_ch = 'e';
static char  E_ch = 'E';



/* ============================ Lokale Routinen: ============================ */
static BOOL check_mod_names (char *sim_mod_path,
			     char *mod_dir_buf);

static BOOL start_ext_sim   (char *mod_name);

static BOOL prep_startsim   (BOOL *sim_mod_path_ok);




/* ============================== doMatlabSim =============================== */
BOOL doMatlabSim (DOUBLE              *erg_werte,
                  CHAIN_ELEM_PTR_TYPE  chrPtr)
{
   fatal (SIMU_EXT_SIMU, 1, NO_INFO, NO_INFO, MATLAB_SIMU_TXT, APPL_NOT_AVAILABLE);
   return (FALSE);
} /* doMatlabSim */



/* ============================== do_ext_sim ================================ */
BOOL do_ext_sim (DOUBLE *erg_werte,
                 INT    *i_params,
                 DOUBLE *r_params)
{
   BOOL ok = FALSE;

   /* do_ext_sim */
   switch (ext_simu_code)
   {
      case ELDO_SIMU:
      ok = do_eldo_sim (erg_werte, i_params, r_params);
      break;

      case M_MATICA_SIMU:
      ok = do_math_sim (erg_werte, i_params, r_params);
      break;

      case GEN_EXT_SIMU:
      ok = do_gen_sim (erg_werte, i_params, r_params, SINGLE_LINE);
      break;

      case GEN_ML_EXT_SIMU:
      ok = do_gen_sim (erg_werte, i_params, r_params, MULTI_LINE);
      break;

      default:
      fehler (SIMU_EXT_SIMU, 2, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      ok = FALSE;
   } /* CASE */
   return (ok);
} /* do_ext_sim */



/* ========================== ext_simu_hochfahren =========================== */
BOOL ext_simu_hochfahren (BOOL  mit_fin)
{
   INT   idx;
   BOOL  new_modell, 
         ok;
         
   /* ext_simu_hochfahren */
   new_modell = prep_startsim (&ok);          /* SimModell-Vgl. u.ggf. -Update*/
   if (!ok)
      return (FALSE);
   if (sim_ok && (mit_fin || new_modell))
      terminate_ext_simu ();                  /* Ext. Simulator terminieren   */
#ifdef ONLINE
   if (!(sim_ok && sim_up))
   { /* Simu hochfahren */
      init_sim ();                            /* SimPilot erneut starten      */
      start_ext_sim (extSimuModelName);       /* ext. Simulator hochfahren    */
   } /* Simu hochfahren */
   switch (ext_simu_code)
   {
      case ELDO_SIMU:
      set_eldo_data_waits ();
      break;

      case M_MATICA_SIMU:
      set_math_data_waits ();
      break;

      case GEN_EXT_SIMU:
      case GEN_ML_EXT_SIMU:
      set_gen_data_waits ();
      break;

      default:
      fehler (SIMU_EXT_SIMU, 3, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      ok = FALSE;
   } /* CASE */
#else /* not ONLINE */
   if (!(sim_ok && sim_up))
   { /* Simu "hochfahren" */
      tsim   = fopen ("tsim.txt", "w");
      fsim   = fopen ("fsim.txt", "r");
      efsim  = fopen ("efsim.txt", "r");
      sim_ok = (fsim != NULL) && (efsim != NULL);
      if (sim_ok)
	 start_ext_sim (extSimuModelName);
   } /* Simu "hochfahren" */
#endif /* not ONLINE */
   return (sim_ok && sim_up);
} /* ext_simu_hochfahren */



/* =========================== update_ext_simu_tmo ========================== */
void update_ext_simu_tmo (INT  tmo_zeit_in_sec)
{
   simu_first_waits = l_round (tmo_zeit_in_sec * to_tics);
} /* update_ext_simu_tmo */



/* ========================== terminate_ext_simu ============================ */
void terminate_ext_simu (void)
{
   BOOL ok = FALSE;

   /* terminate_ext_simu */
   if (sim_ok && mit_ext_simu)
   { /* ExtSimu aktiv */
      switch (ext_simu_code)
      {
	 case ELDO_SIMU:
	 ok = terminate_eldo_sim (sim_up);
	 break;

	 case M_MATICA_SIMU:
	 ok = terminate_math_sim (sim_up);
	 break;

         case GEN_ML_EXT_SIMU:
	 case GEN_EXT_SIMU:
	 ok = terminate_gen_sim (sim_up);
	 break;

	 default:
	 fehler (SIMU_EXT_SIMU, 4, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      } /* CASE */
#ifdef GLEAM_USR
      if (ok)
	 meldung (SIMU_EXT_SIMU, NO_INFO, NO_INFO, TERMINATED_TXT, EXT_SIMU_DONE);
      else
	 meldung (SIMU_EXT_SIMU, NO_INFO, NO_INFO, TERMINATED_TXT, EXT_SIMU_NIX_DONE);
#endif /* GLEAM_USR */
   } /* ExtSimu aktiv */
} /* terminate_ext_simu */



/* =========================== ext_simu_recovery ============================ */
void ext_simu_recovery (void)
{
   unsigned INT  rec_delay;       /* Aktuelle Wartezeit fuer Simu-Recover     */
   INT           trial_ctr,       /* Recovery-Zaehler                         */
                 idx;
#ifdef MITSIMULOG
   STR_TYPE      date_buf, time_buf, dummy;
#endif

   /* ext_simu_recovery */
   rec_delay = REC_GADO_START_DELAY;              /* GADO: kuerzer warten */
   trial_ctr = 0;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      get_time_stamp (dummy, date_buf, dummy, time_buf);
      sprintf(simuLogBuf, "SIMU-RECOVERY: Beginn am %s um %s  sim_up=%d sim_ok=%d", 
	      date_buf, time_buf, sim_up, sim_ok);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
   do
   { /* Warten und Starten */
      warte (rec_delay);                   /* Warte "rec_delay" Millisekunden */
      terminate_ext_simu ();               /* Externen Simulator terminieren  */
      init_sim ();                         /* Externen Simulator              */
      start_ext_sim (extSimuModelName);    /* erneut starten.                 */
      trial_ctr++;
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 get_time_stamp (dummy, date_buf, dummy, time_buf);
	 sprintf(simuLogBuf, "SIMU-RECOVERY: Time=%s %s  sim_up=%d sim_ok=%d Loop=%d Delay=%d msec", 
		 date_buf, time_buf, sim_up, sim_ok, trial_ctr, rec_delay);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
      rec_delay += RECOVER_DELAY_INCR;
   } /* Warten und Starten */
   while (!sim_up && (trial_ctr < RECOVER_TRIALS));
} /* ext_simu_recovery */



/* ============================ check_mod_names ============================= */
static BOOL check_mod_names (char *sim_mod_path,
			     char *mod_dir_buf)
/*----------------------------------------------------------------------------*/
/*  Baut den Pfad des Simulationsmodells in "sim_mod_path" unter Verwendung   */
/*  von "mod_dir_buf" und "extSimSubDir" auf, testet ihn und liefert das      */
/*  Ergebnis ab.                                                              */
/*----------------------------------------------------------------------------*/
{
   if (strlen(extSimSubDir) == 0)
      sprintf (sim_mod_path, "%s%s%s", sim_root_dir_name, DIR_DELIMITER, mod_dir_buf);
   else
      sprintf (sim_mod_path, "%s%s%s%s%s", sim_root_dir_name, DIR_DELIMITER,
	       extSimSubDir, DIR_DELIMITER, mod_dir_buf);
   return (check_dir (sim_mod_path));
} /* check_mod_names */



/* ============================ start_ext_sim =============================== */
static BOOL start_ext_sim (char *mod_name)
/*----------------------------------------------------------------------------*/
/*  Die Routine setzt voraus, dass der externe Simulator mit Hilfe von        */
/*  "init_sim" (Modul "ext_koppl" in Package "sys") erfolgreich gestartet     */
/*  wurde, "sim_ok" also TRUE ist.                                            */
/*  In diesem Falle erzeugt sie eine Startmeldung mit Uhrzeit und mit dem     */
/*  "mod_name", sofern "mod_name" nicht leer ist. Die Meldung wird ueber den  */
/*  Textpuffer des Packages "fbhm" ausgegeben. Dann wartet die Routine auf    */
/*  das erfolgreiche Hochfahren des Simulators und initialisiert ihn positi-  */
/*  venfalls, soweit das notwendig ist.                                       */
/*  Wenn alles gut ging, ist das Funktionsergebnissie FALSE, sonst TRUE.      */
/*----------------------------------------------------------------------------*/
{
   BOOL ok = FALSE;

   /* start_ext_sim */
   switch (ext_simu_code)
   {
      case ELDO_SIMU:
      ok = start_eldo_sim (mod_name);
      break;

      case M_MATICA_SIMU:
      ok = start_math_sim (mod_name);
      break;

      case GEN_ML_EXT_SIMU:
      case GEN_EXT_SIMU:
      ok = start_gen_sim (mod_name);
      break;

      default:
      fehler (SIMU_EXT_SIMU, 5, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      ok = FALSE;
   } /* CASE */
   return (ok);
} /* start_ext_sim */



/* ============================ restart_ext_sim ============================= */
BOOL restart_ext_sim (void)
{
   BOOL ok;

   /* restart_ext_sim */
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "\n%s RESTART_EXT_SIM: %s", STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
   switch (ext_simu_code)
   {
      case ELDO_SIMU:
      ok = restart_eldo_sim ();
      break;

      case M_MATICA_SIMU:
      ok = restart_math_sim ();
      break;

      case GEN_ML_EXT_SIMU:
      case GEN_EXT_SIMU:
      ok = restart_gen_sim ();
      break;

      default:
      fehler (SIMU_EXT_SIMU, 6, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      ok = FALSE;
   } /* CASE */
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "RESTART_EXT_SIM: Results: sim_ok=%d  sim_up=%d", sim_ok, sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
   return (ok);
} /* restart_ext_sim */



/* ========================== kleiner_ext_restart =========================== */
void kleiner_ext_restart (void)
{
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "\n%s KLEINER_EXT_RESTART: %s\n",  STRICH_15, STRICH_15);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif  /* MITSIMULOG */
   switch (ext_simu_code)
   {
      case ELDO_SIMU:
      kleiner_eldo_restart ();
      break;

      case M_MATICA_SIMU:
      kleiner_math_restart ();
      break;

      case GEN_ML_EXT_SIMU:
      case GEN_EXT_SIMU:
      meldung (SIMU_EXT_SIMU, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
/*      kleiner_gen_restart ();         funktioniert nicht !! */
      break;

      default:
      fehler (SIMU_EXT_SIMU, 7, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
   } /* CASE */
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "KLEINER_EXT_RESTART: sim_ok=%d  sim_up=%d\n", sim_ok, sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
} /* kleiner_ext_restart */



/* ============================= prep_startsim ============================== */
static BOOL prep_startsim (BOOL *sim_mod_path_ok)
/*----------------------------------------------------------------------------*/
/*  Baut den Pfad des Simulationsmodells lokal auf und vergleicht ihn mit     */
/*  "akt_sim_mod_path". Bei Abweichung wird der neue Pfad getestet. Wenn ok,  */
/*  wird er in "akt_sim_mod_path" hinterlegt und damit das "startsim"-File    */
/*  applikationsspezifisch neu erzeugt. Liefert TRUE, wenn ein neues "start-  */
/*  sim"-File erfolgreich angelegt wurde. "sim_mod_path_ok" ist TRUE, wenn    */
/*  der Modellpfad ok ist.                                                    */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE  local_path;
   INT          idx;
   BOOL         delta, 
                ok = FALSE;

   /* prep_startsim */
   *sim_mod_path_ok = check_mod_names (local_path, extSimuModelName);
   delta            = strcmp (akt_sim_mod_path, local_path) != 0;
   if (delta && *sim_mod_path_ok)
   { /* Neuer Pfad weicht ab und ist ok */
      strcpy (akt_sim_mod_path, local_path);
      switch (ext_simu_code)
      {
	 case ELDO_SIMU:
	 ok = make_eldo_startsim ();
	 break;

	 case M_MATICA_SIMU:
	 ok = make_math_startsim ();
	 break;

         case GEN_ML_EXT_SIMU:
	 case GEN_EXT_SIMU:
	 ok = make_gen_startsim ();
	 break;

	 default:
	 fehler (SIMU_EXT_SIMU, 8, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      } /* CASE */
   } /* Neuer Pfad weicht ab und ist ok */
   return (ok);
} /* prep_startsim */



/* ============================ test_and_read_val =========================== */
BOOL test_and_read_val (DOUBLE *wert)
{
   INT   len, pos;
   BOOL  wert_ok = FALSE;

   /* test_and_read_val */
   len     = strlen (inbuffer);
   wert_ok = sscanf (inbuffer, "%lf", wert) == 1;
   pos     = strcspn (inbuffer, &e_ch);
   if (pos >= len)
      pos = strcspn (inbuffer, &E_ch);
   if (pos < len)                         /* Zahl in Exponenten-Darstellung ? */
      wert_ok = wert_ok && (len >= 13) && ((len-pos) == 4);

   return (wert_ok);
} /* test_and_read_val */



#ifdef ONLINE
/* =============================== read_line ================================ */
BOOL read_line (INT   in_pipe,
                char *buf)
{
   INT  len,
        erg,
        inbytes;

   /* read_line */
   len = 0;
   do
   {
      /*                      alter Code, welcher mittlerweile stoert!
      erg = ioctl (in_pipe, I_NREAD, &inbytes);
      if (erg > 0)
	 erg = (read (in_pipe, &(buf[len]), 1) == 1);
      */

      erg = (read (in_pipe, &(buf[len]), 1) == 1);
      if (erg)
         len++;
   }
   while ((buf[len-1] != CR) && (len < BUF_LEN) && erg);
   if (len > 0)
      if (buf [len-1] == CR)
	 buf [len-1] = '\0';                              /* terminate string */
      else
	 buf [len] = '\0';                                /* terminate string */
   return (len > 0);
} /* read_line */
#else /* not ONLINE */
/* ============================== read_line ================================= */
BOOL read_line (FILE *in_file,
                char *buf)
{
   INT  len = 0;

   /* read_line */
   if (fgets (buf, BUF_LEN, in_file) == NULL)
      len = 0;
   else
      len = strlen (buf);
   if (len > 0)
     if (buf [len-1] == CR)
        buf [len-1] = '\0';                               /* terminate string */
   return (len > 0);
} /* read_line */
#endif /* not ONLINE */



/* ============================= wait_for_reply ============================= */
BOOL wait_for_reply (BOOL *time_out,
                     LONG  wcycles)
{
   LONG  loc_wait_ctr;
   BOOL  fertig = FALSE,     /* TRUE, wenn Antwort oder echte Fehlermeldung   */
         fmeld  = FALSE,     /* TRUE, wenn echte Fehlermeldung                */
         antw,               /* TRUE, wenn Antwort in der Ausgabe-Pipe        */
         err;                /* TRUE, wenn Meldung in der Error-Pipe          */

   /* wait_for_reply */
   strcpy (inbuffer, "");
   strcpy (errbuffer, "");
   antw         = FALSE;
   loc_wait_ctr = 0;
   do
   {
#ifdef ONLINE
      antw = read_line (pipe_s2p[0], inbuffer);
      err  = read_line (pipe_se2p[0], errbuffer);
#else /* not ONLINE */
      antw = read_line (fsim,  inbuffer);
      err  = read_line (efsim, errbuffer);
#endif /* not ONLINE */
#ifdef IO_LOG
      if (mit_simulog && (detailed_iolog || antw || err))
      {
	 sprintf(simuLogBuf, "WaitForReply%3ld: antw=%1d  in=\"%s\"", 
		 loc_wait_ctr, antw, inbuffer);
	 sprintf(simuLogBuf, "WaitForReply%3ld: err =%1d err=\"%s\"", 
		 loc_wait_ctr, err, errbuffer);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* IO_LOG */

      loc_wait_ctr++;
      if (antw)
      { /* Antwort erhalten: Leerzeilen und " "-Zeilen ignorieren */
	 antw = strlen (inbuffer) > (size_t)1;     /* ignore 1 char lines too */
      } /* Antwort erhalten: Leerzeilen und " "-Zeilen ignorieren */
      if (err)
      { /* Fehlersituation pruefen */
	 switch (ext_simu_code)
	 {
	    case ELDO_SIMU:
	    fmeld = check_eldo_err_meld ();
	    break;

	    case M_MATICA_SIMU:
	    fmeld = check_math_err_meld ();
	    break;

	    case GEN_ML_EXT_SIMU:
	    case GEN_EXT_SIMU:
	    fmeld = check_gen_err_meld ();
	    break;


	    default:
	    fehler (SIMU_EXT_SIMU, 9, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
	    fmeld = TRUE;
	 } /* CASE */
	 if (strlen (errbuffer) > 0)
	 { /* Fehler melden */
	    if (fmeld)
	       fehler (SIMU_EXT_SIMU, 11, NO_INFO, NO_INFO, errbuffer, EXT_SIMU_ERR_TXT);
	    else
	       meldung (SIMU_EXT_SIMU, NO_INFO, NO_INFO, errbuffer, EXT_SIMU_ERR_TXT);
	 } /* Fehler melden */
#ifdef MITSIMULOG
         if (mit_simulog)
	 {
            sprintf(simuLogBuf, "*** Sim-ErrorPipe: antw=%d, fmeld=%d  Text=\"%s\"", 
		    antw, fmeld, errbuffer);
	    write_to_simulog(simuLogBuf, CLOSE_LOG);
	 }
         if (!fmeld)
            strcpy (errbuffer, "");
#endif /* MITSIMULOG */
      } /* Fehlersituation pruefen */
      fertig = antw || fmeld;
      if (!fertig)
         warte (WAIT_FOR_READ);
   }
   while (!fertig && (loc_wait_ctr < wcycles));
   *time_out = !fertig;
   wait_ctr += loc_wait_ctr;
  
   return (antw && !fmeld);
} /* wait_for_reply */


/* ============================ check_for_prompt ============================ */
BOOL check_for_prompt (LONG  wcycles,
                       INT   err_skips,
                       BOOL  ident_prompt)
{
   BOOL  prompt_rec;

   /* check_for_prompt */
#ifdef MITSIMULOG
   write_to_simulog("-------------------- Check for Prompt: -----------------", CLOSE_LOG);
#endif /* MITSIMULOG */
   fprintf (tsim, "\n"); fflush(tsim);
#ifdef IO_LOG
   write_to_simulog("ToSim: CR\n", CLOSE_LOG);
#endif /* IO_LOG */
   if (ident_prompt)
      prompt_ctr--;
   switch (ext_simu_code)
   {
      case ELDO_SIMU:
      prompt_rec = till_eldo_prompt (wcycles, err_skips, IGNORE_ERRORS);
      break;

      case M_MATICA_SIMU:
      prompt_rec = till_math_prompt (wcycles, err_skips, IGNORE_ERRORS);
      break;

      case GEN_ML_EXT_SIMU:
      case GEN_EXT_SIMU:
      prompt_rec = till_gen_prompt (wcycles, err_skips, IGNORE_ERRORS);
      break;

      default:
      fehler (SIMU_EXT_SIMU, 10, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
      prompt_rec = FALSE;
   } /* CASE */
   if (!prompt_rec)
   { /* Kein Prompt */
      prompt_ctr = -1;                             /* Akzeptiere jeden Prompt */
      fprintf (tsim, "\n"); fflush(tsim);
#ifdef IO_LOG
      write_to_simulog("ToSim: CR", CLOSE_LOG);
#endif /* IO_LOG */
      switch (ext_simu_code)
      {
	 case ELDO_SIMU:
	 prompt_rec = till_eldo_prompt (wcycles, err_skips, IGNORE_ERRORS);
	 break;

	 case M_MATICA_SIMU:
	 prompt_rec = till_math_prompt (wcycles, err_skips, IGNORE_ERRORS);
	 break;

         case GEN_ML_EXT_SIMU:
	 case GEN_EXT_SIMU:
	 prompt_rec = till_gen_prompt (wcycles, err_skips, IGNORE_ERRORS);
	 break;

	 default:
	 fehler (SIMU_EXT_SIMU, 11, ext_simu_code, NO_INFO, "", BAD_EXT_SIMU_CODE);
	 prompt_rec = FALSE;
      } /* CASE */
   } /* Kein Prompt */
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "Check for Prompt: prompt_rec=%d sim_up=%d", 
	      prompt_rec, sim_up);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
   return (prompt_rec);
} /* check_for_prompt */



/* ============================= check_for_pid ============================== */
BOOL check_for_pid (INT  *prog_pid,
                    char *prog_name)
{
   STR_TYPE  buf, 
             dummy_buf;
   FILE     *qpipe;
   BOOL      erg = FALSE;

   /* check_for_pid */
   strcpy (buf, PROC_CHECK_STRING);
   strcat (buf, prog_name);
   qpipe = popen (buf, "re");
   if (qpipe != NULL)                           /* I/O stream der Pipe offen? */
      erg = fscanf (qpipe, "%s %s %s %d", dummy_buf, dummy_buf, dummy_buf, prog_pid) == 4;
   return (erg);
} /* check_for_pid */



/* ============================== kill_process ============================== */
BOOL kill_process (char *prog_name)
{
   STR_TYPE  wuerg_buf;
   INT       kill_pid,
             dummy;
   BOOL      done     = FALSE;
#ifdef MITSIMULOG
   BOOL      sofort_tot = FALSE,
             versuch_2  = FALSE;
#endif /* MITSIMULOG */

   /* kill_process */
   if (check_for_pid (&kill_pid, prog_name))
   { /* program is alive */
      sprintf (wuerg_buf, "%s %d", KILL_STRING, kill_pid);
      dummy = system (wuerg_buf);
      warte (KILL_WAIT_MSEC);
      if (check_for_pid (&kill_pid, prog_name))
      { /* program is still alive */
	 warte (KILL_WAIT_MSEC);
	 if (check_for_pid (&kill_pid, prog_name))
	 { /* program is still alive: 2nd try */
#ifdef MITSIMULOG
	    versuch_2 = TRUE;
#endif
	    dummy = system (wuerg_buf);
	    warte (KILL_WAIT_MSEC);
	    done = check_for_pid (&kill_pid, prog_name);
	 } /* program is still alive: 2nd try */
	 else
	    done = TRUE;                                /* Program terminated */
      } /* program is still alive */
      else
      {
	 done = TRUE;                                   /* Program terminated */
#ifdef MITSIMULOG
	 sofort_tot = TRUE;
#endif
      }
   } /* program is alive */
   else
      done = TRUE;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      sprintf(simuLogBuf, "KILL_PROCESS: \"%s\" Done=%d  Sofort_tot=%d  2.Versuch=%d",
	      prog_name, done, sofort_tot, versuch_2);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
   return (done);
} /* kill_process */



/* ============================= init_ext_koppl ============================= */
BOOL init_ext_koppl (BOOL *ext_sim_mod_ok)
{
   STRING_TYPE  buf;
   INT          intVal;
   BOOL         ok = TRUE;

   /* init_ext_koppl */
   atexit (terminate_ext_simu);       /* Etablierung des Exit-Handlers        */
   if (((inbuffer  = (char*)malloc((BUF_LEN+1)*sizeof(char))) == NULL) ||
       ((errbuffer = (char*)malloc((BUF_LEN+1)*sizeof(char))) == NULL)   )
      return (FALSE);
   strcpy (akt_sim_mod_path, "");     /* d.h., dass kein startsim-File exist. */
#ifdef IO_LOG
   detailed_iolog = DETAILED_IO_LOG;
#endif
   *ext_sim_mod_ok = TRUE;
   to_tics         = 1 sec;
   init_sim_mods (sim_root_dir_name);
#ifdef ONLINE
   if (mit_ext_simu)
      *ext_sim_mod_ok = check_mod_names (buf, extSimuModelName); 
#endif /* ONLINE */
   if (mit_ext_simu)
   { /* mit externen Simulator */
      if (!(*ext_sim_mod_ok))
      { /* Kein Zugriff auf Modelle des ext. Simu */
	 fehler  (SIMU_EXT_SIMU, 30, NO_INFO, NO_INFO, "", NO_EXT_SIM_MODELS);
	 fehler  (SIMU_EXT_SIMU, 31, NO_INFO, NO_INFO, "", BAD_SIMU_ROOT_DIR1);
	 sprintf (buf, "%s ", sim_root_dir_name);    /* never an empty string */
	 fehler  (SIMU_EXT_SIMU, 32, NO_INFO, NO_INFO, buf, BAD_SIMU_ROOT_DIR2);
	 get_env_var_name (buf);
	 meldung (SIMU_EXT_SIMU, NO_INFO, NO_INFO, buf, ENV_VAR_HINWEIS);
      } /* Kein Zugriff auf Modelle des ext. Simu */
   } /* mit externen Simulator */
   else
   { /* ohne externen Simulator */
      *ext_sim_mod_ok = TRUE;
      sim_up          = TRUE; /* using SW sees that it's possible to simulate */
   } /* ohne externen Simulator */
   ok = ok && init_eldo_koppl() && init_math_koppl() && init_gen_koppl();
   if (mit_ext_simu && ok)
   { /* TimeOut aus TSK-File erst nach Modul-Init setzen */
      if (getIntVal(&intVal, SIMU_MAX_TIME_IDX, GLEAM_ERROR))
	 update_ext_simu_tmo(intVal);
      else 
	 ok = FALSE;
   } /* TimeOut aus TSK-File erst nach Modul-Init setzen */
   return (ok);
} /* init_ext_koppl */

