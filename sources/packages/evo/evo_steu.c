/*******************************************************************************
HyGLEAM/A                                                                 V1.3.1

                                 H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: EVO-STEUerung
 
Package: evo                    File: evo_steu.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Der Modul enthaelt im wesentlichen die exportierte Routine "do_evo_jobs" zur Ab-
arbeitung der Jobliste und die lokale Routine "do_1_evo_job", die die Abarbei-
tung eines Optimierungsjobs mit Hilfe von "evo_1gen", "lsv_complex" oder 
"lsv_rosen" durchfuehrt.

"gen_note" und "gen_len" erfassen die Fitness und Laenge des Generationsbesten, 
der auch in der Population entahlten ist. Das bedeutet bei nicht-elitaeren 
Akzeptanzregeln, dass der beste Fitnesswert auch fallen!
Im Gegensatz dazu erfasst "bestJobNote" die Fitness des besten je erzeugten 
Individuums eines OptJobs undabhaengig davon, ob es am Ende des Laufs noch in 
der Population vorhanden ist.


Neben dem evolutionaeren Algorithmus GLEAM sind noch die lokalen Suchverfahren
als eigenstaendige Algorithmen und die folgenden Verfahrenkombinationen als
Memetische Algorithmen implementiert:


Eigenstaendiger Evo-Job GLEAM: 
  Initialisierung mit einer der im "evo"-Modul beschriebenen Initialsierungs- 
  strategien (ausser ALL), wobei bei gesetztem MIT_LSV auch LSV zur Anwendung 
  kommen koennen.

Eigenstaendige LSV-Jobs:
  Initialisierung mit einer der im "evo"-Modul beschriebenen Initialsierungs- 
  strategien NEU, BEST, BEST_NEU und ALL. Beim Complex wird bei dynamischen AKs
  und NEU, BEST oder BEST_NEU nur eine AK aus dem AK-Mem genommen und es werden
  die restlichen Startpunkte wie bei ALL zufaellig gewaehlt. Wenn im Job eine
  AK-Rettung vorgesehen ist, wird die beste ermittelt und gerettet.

Kombi-Job GLEAM_SMA (simple Memetic Algorithm oder Single-Meme-Algorithm):
  Initialisierung mit einer der im "evo"-Modul beschriebenen Initialsierungs- 
  strategien LSV_NEU oder LSV_BEST und direkte Integration mit einem der ge-
  maess Genmodell zulaessigen LSVs, wobei die Lamarckrate und die Steuer-
  parameter des LSVs gewaehlt werden koennen.

Kombi-Job GLEAM_ASMA (Adaptive Single-Meme-Algorithm):
  Initialisierung mit einer der im "evo"-Modul beschriebenen Initialsierungs- 
  strategien LSV_NEU oder LSV_BEST und adaptive direkte Integration eines der
  gemaess Genmodell zulaessigen LSVs. Neben der Lamarckrate kann auch der Adap-
  tionsanteil voergegeben werden. Letzteres bestimmt, mit welchen Anteil die 
  neu berechnete LSV- oder Level-Verteilung zusammen mit der alten Verteilung 
  in die neue eingeht.

Kombi-Job GLEAM_AMMA (Adaptive Multi-Meme-Algorithm):
  Initialisierung mit einer der im "evo"-Modul beschriebenen Initialsierungs- 
  strategien LSV_NEU oder LSV_BEST und adaptive direkte Integration eines der
  gemaess Genmodell zulaessigen LSVs. Neben der Lamarckrate kann auch der Adap-
  tionsanteil voergegeben werden, siehe GLEAM_ASMA. 


Die Routinen im einzelnen:


BOOL do_job_end (void);
          Nach Beendigung eines Jobs wird der naechste der Jobliste unter
          Ignorierung deaktivierter Jobs bereit gestellt. Ausserdem werden Ver-
          waltungsmassnahmen am Jobende durchgefuehrt. Bei Beendigung des letz-
          ten Jobs der Liste erfolgen Ausgaben ins Logfile. Die Routine liefert 
          TRUE, wenn es noch mindestens einen weiteren Job gibt, und nicht auf 
          Grund eines fatalen Fehlers oder von MemOut abgebrochen werden muss.
          Ansonsten FALSE. 

void do_evo_jobs (void);
          Die Routine bearbeitet alle Evo-WS-Eintraege in einer Schleife und er-
          stellt am Schluss die Gesamtstatistik der Jobliste.

          Terminierungsgruende und Variable:
          Terminierungsgrund        "evo_aktiv"  "taste"  "mem_out"  "err_state"
          ----------------------------------------------------------------------
          alle Jobs abgearbeitet       FALSE      FALSE     FALSE     GLEAM_OK
          Unterbrechungswunsch          TRUE       TRUE    (FALSE)   (GLEAM_OK)
          Speicher alle                FALSE     (FALSE)     TRUE    (GLEAM_OK)
          (fataler) Fehler             FALSE     (FALSE)   (FALSE)  != GLEAM_OK
          Einige Werte wurden geklammert, da die Variable beim gleichzeitigen 
          Auftreten der Ereignisse andere Werte annehmen koennen. Bei Fehler
          oder Kein-Speicher am Jobende wird ein etwaiger Unterbrechungswunsch
          ausgefiltert und ignoriert.

void   clearEvoJobIntrpt (void);
          Setzt eine Jobunterbrechung zurueck, so dass der naechste Job neu 
          gestartet wird.

void clear_evo_results (GES_STAT_TYPE *stat_dat);
          Loescht die durch "stat_dat" spezifizierte Gesamt-Job-Statistik.

BOOL init_evo_steu (void);
          Modulinitialsierung. Liefert FALSE, wenn der Speicherplatz nicht aus-
          reicht.

--------------------------------------------------------------------------------
basiert auf Modul: evo_steu.c  V2.0.1 vom 19.4.2002
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Anpassungen an Adaption der all-Variante.                        WJ 22.12.04
 2. Anpassungen beim Aufruf von "set_lsv_limits" fuer OPAL/V.        WJ 07.01.05
 3. "compl_erg_idx" in "do_1_evo_job" initialisiert.                 WJ 02.06.05
 4. Erweiterung f. n LSVs, neue Job-Struktur, 1 Population/Job.      WJ 25.08.06
 5. Ggf. Warten bei Jobende zur Vermeidung gleicher Zufallszahlen.   WJ 23.12.06
 6. "init_strat" wird package-global.                                WJ 11.01.07
 7. "gen_len" wird package-global und zusammen mit "gen_note"
    nicht mehr an "evo_1gen" als Parameter uebergeben.               WJ 24.08.07
 8. CEC: Abbruch nach "maxFEs" FunktionsEvaluierungen bei CEC_MBF.   WJ 07.11.07
 9. Matlab-Kopplung: Aufrufe von "mlOnlineVisu" in "do_1_evo_job".   WJ 28.01.08
10. Aufruf von "allocParListen" aus "par_simu" in "do_1_evo_job".    WJ 19.07.08
11. Korrektur des Aufrufs von "mlOnlineVisu".                        WJ.20.08.08
12. GORBA: Ruecksetzung von Daten des besten Indivs (GORBA_GLEAM).   WJ 18.12.08
13. Engine: Anzeige der "bestInitNote" nach dem Lauf.                WJ 26.02.09
14. Punkt 12 rueckgaengig gemacht. Wird in der Engine erledigt.      WJ 10.11.09
15. Neue Variable "gleamPur".                                        WJ 20.11.09
16. Aufruf von "check_for_lamarck" in "do_1_evo_job" statt Code.     WJ 27.11.09
17. Entfernung der Schalter BGI_MODE, PAR_SIMU, GLEAM_TCL samt dazu-
    gehoerigem Code und von "mlOnlineVisu", "mitMatLabSimu" und 
    "mitOnlVisu". Korrektur des Aufrufs von "zeige_gen_erg". Ent-
    fernung ueberfluessiger Variable.                                WJ 28.06.12
18. Erweiterung der Initialisierung des Zufallszahlengenerators um
    einen Offset-Parameter zur Vermeidung gleicher Startwerte.
    "first_evo_item" wird Package-Export. Einige Variable werden    
    package-global in der sequentiellen und global in der MPI-Vers.
    Streichung der Variablen "gak_max_halbe" und "gdv_max_halbe".    WJ 15.11.12
19. Trennung der Erfassung des Generationsbesten in der Population 
    ("gen_note", "gen_len") von der besten erreichten Note eines 
    OptJobs ("bestJobNote").                                         WJ 30.11.12
20. Einbau eines Abbruchs bei Erreichen von "evalLimit".
21. Anpassungen an den GUI_MODE: Export von "job_aktiv". Neue 
    Routine "do_job_end".                                            WJ 21.11.14
22. "do_job_end" gegen fehlerhafte Indices "free_evo_index" und 
    "firt_evo_item" abgesichert.                                     WJ 16.11.15
23. Renovierung: Ersetzung der "write_line"- durch "printf"-Aufrufe. 
    Aufruf von "kb_hit" nur bei gesetztem GLEAM_USR.                 WJ 10.05.16
24. Verschiebung von "def_gen_anz" als "defGenLimit" nach "evo_gsw.c" 
    im Rahmen der Integration der neuen TSK-Datenhaltung. Sonder-
    behandlung von "evalLimit" (0=Abschaltung) aufgehoben. Streichung
    von "evo_display", "cont_on_memout" und der Ueberwachung des 
    Logfileschreibens. Anpassung des "write_to_log"-Aufrufs. Ausgabe
    der besten Fitness der Startpopulation in allen Modi.            WJ 23.12.16
24. Uebernahme von "clear_evo_results" von Modul "men_gsw.c" aus
    Package "men" und Aufruf zur Initialisierung in "init_evo_steu". WJ 08.06.17
25. Anpassungen an das neue einheitliche Modul "evo_anzg". Streichung 
    der "log_bufs[]". Reunion of "evoIntg.h" and "evoInts.h" in 
    "evoInt.h".                                                      WJ 30.06.17
26. Die Semantik der Variable "taste", die ins "sys"-Package ver-
    schoben wurde, hat sich geaendert: Sie bleibt laenger gesetzt 
    und wird erst im Laufe des nachfolgenden Aufrufs von "do_1_evo_job" 
    zurueckgesetzt. "no_key" entfaellt in diesem Zusammenhang.       WJ 29.08.17
27. Das package-globale "evo_intrpt" wird modul-lokal. Ein Ueber-
    schreiten der Zielfitness vor der Optimierungsschleife kann zwei 
    Ursachen haben: Gute Initialisierung oder Herabsetzung der Ziel-
    fitness waehrend einer Unterbrechung. Beides fuehrt nun zu einer 
    neuen ausserplanmaessigem Jobterminierung.
    Vereinheitlichung der Routinen "clear_evo_job_intrpt" mit
    "clear_evo_intrpt" aus Modul "evo_gsw.c" zu "clearEvoJobIntrpt" 
    in diesem Modul.                                                 WJ 11.09.17
28. In "do_1_evo_job" wird bei gesetztem EXT_SIMU_SERV und 
    "mit_ext_simu" geprueft, ob "sim_up" FALSE ist und dann versucht,
    den Simulator hochzufahren. Wenn dies scheitert, wird die 
    Funktion mit FALSE abgebrochen. "clearEvoJobIntrpt" ruft bei 
    gesetztem EXT_SIMU_SERV abortJobOfWorkflow() auf.                WJ 27.10.17
29. Die Variable "job_aktiv" und die ansonsten statische Funktion 
    "do_1_evo_job" wird bei gesetztem GUI_MODE oder gesetztem 
    VISU_MODE exportiert.                                            WJ 13.03.18
30. do_1_evo_job(): Erfassung der Chromosomenlaenge des besten 
    Chromosoms der Startpopulation.                                  WJ 06.06.18
31. In do_1_evo_job() wird get_best() nur bei nicht gestztem 
    PAR_POP_VERS aufgerufen, da bei gesetztem PAR_POP_VERS die
    gesamte Population zur Rettung durch save_ep() benoetigt wird.   WJ 28.06.18
32. Streichung von "evo_done". "last_psize" wird Package-Export.     WJ 10.07.19
33. Transfer von "mit_statistik" zum Modul "tsk_data.c" aus "aufg".  WJ 11.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 11.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#ifdef EXT_SIMU_SERV
  #include "hmod.h"
  #include "webIO.h"
#endif /* EXT_SIMU_SERV */
#include "aufg.h"
#include "bew.h"
#include "simu.h"
#ifdef GORBA_GLEAM
  #include "xml.h"
  #include "gsim.h"
#endif /* GORBA_GLEAM */
#include "evo.h"
#include "evoInt.h"

#include EVO_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
  #undef INTRP_TST
  #undef ABLAUF_TST
#endif /* GLO_CHECKS */




/* ========================== Exportierte Variable: ========================= */
EVO_WS_TYPE   *evo_ws;              /* Optimierungs-Job-Warteschlange         */
JOB_STAT_TYPE  ak_jobli_stat_dat,   /* Gesamtstat. aller Jobs einer Jobliste  */
               rk_jobli_stat_dat;   /* Gesamtstat. aller Jobs einer Jobliste  */
GES_STAT_TYPE  ges_ak_stat_dat,     /* Gesamtstat. aller Jobs aller Joblisten */
               ges_rk_stat_dat;     /* Gesamtstat. aller Jobs aller Joblisten */
INT            free_evo_index,      /* Erstes freies Element in d.OptJobliste */
               first_evo_item,      /* Aktueller (erster) Job der OptJobliste */
               last_psize;          /* Populationsgroesse des letzten Jobs    */
BOOL           evo_aktiv,           /* TRUE, wenn Optimierung laeuft          */
               retteGorbaPlanData,  /* siehe Package glsv, Modul glsv_schieb  */
               job_ende_ton;        /* TRUE = Tonmeldung bei positivem Jobende*/

#if defined(GUI_MODE) || defined(VISU_MODE)
  BOOL         job_aktiv;           /* TRUE, solange keine JobEndeBed.erfuellt*/
#endif /* GUI_MODE or VISU_MODE*/

/* Package-global in sequentieller u. global in MPI-, GUI- oder VISU-Version: */
DOUBLE         bestJobNote,         /* Beste Note eines OptJobs               */
               gen_note;            /* Beste Note der Pop. nach einer Gen.    */
LONG           abgelaufene_zeit;    /* Verbrauchte Populationszeit in Sekunden*/



/* ======================== Package-globale Variable: ======================= */
JOB_STAT_TYPE  akjob_stat_dat,      /* Statistik eines AK-Jobs                */
               rkjob_stat_dat;      /* Statistik eines RK-Jobs                */
INT            gen_len;             /* Laenge der besten AK eines OptJobs     */



/* ============================ Lokale Konstante: =========================== */
#define JOB_ERG          FALSE    /* f."prep_job_erg_anzg": Anzg der JobLi-Erg*/
#define POPUL_ERG        TRUE     /* f."prep_job_erg_anzg": Anzg der aktJobErg*/
#define OPT_FIN          TRUE     /* f."zeige_gen_erg": Anzg bei OptLauf-Ende */
#define OPT_RUNNING      FALSE    /* f."zeige_gen_erg": Anzg waehrend OptLauf */


/* ======================== Lokale Typdeklarationen: ======================== */



/* ============================ Lokale Variable: ============================ */
static LONG           saved_time;
static BOOL           evo_intrpt;         /* Evo-Intrpt. ausser Popul-Wechsel */



/* ============================ Lokale Routinen: ============================ */
#if !defined(GUI_MODE) && !defined(VISU_MODE)
  static BOOL do_1_evo_job     (void);
#endif /* weder GUI_MODE noch VISU_MODE */

static void clear_job_stat_dat (JOB_STAT_TYPE *job_sdata);

BOOL do_job_end (void);




/* ============================== do_job_end ====-=========================== */
BOOL do_job_end (void)
{
   BOOL  weiterMachen;

   /* do_job_end */
#if defined(TEXT_MODE)
   printf ("\n");                                          /* eine Leerzeilen */
#endif
   first_evo_item++;                        /* Job beendet, auf zum naechsten */
   while ((first_evo_item < free_evo_index) && evo_ws[first_evo_item].deaktiviert)
      first_evo_item++;              /* Skip eventuelle leere Eintraege */
   if (first_evo_item >= free_evo_index)
   { /* letzten Job erledigt */
      first_evo_item = 0;
      free_evo_index = 0;
      weiterMachen   = FALSE;
      /* ---------------------------- Statistik: ---------------------------- */
      if (ak_jobli_stat_dat.best_ch_note > ges_ak_stat_dat.best_note)
	 ges_ak_stat_dat.best_note = ak_jobli_stat_dat.best_ch_note;
      (ges_ak_stat_dat.res_ch_sum) += ak_jobli_stat_dat.anz_res_chs;
#ifdef GLEAM_USR
      if (mit_statistik)
	 jobLiLog();  /* Statistik ueber d.akt.u. alle bisherigen Jobliste(n) */
#endif
      clear_job_stat_dat (&ak_jobli_stat_dat);
      clear_job_stat_dat (&rk_jobli_stat_dat);
#if defined(GLEAM_USR) 
      drei_klang (KURZ_TON);
#endif
   } /* letzten Job erledigt */
   else
   { /* Es gibt noch Jobs */
      weiterMachen = (err_state != GLEAM_FATAL) && !mem_out;
      if (weiterMachen && (err_state == GLEAM_ERROR))
	 clear_err_state ();
   } /* Es gibt noch Jobs */
   clear_mut_op_ctrs ();
   return (weiterMachen);
} /* do_job_end */



/* ============================== do_evo_jobs =============================== */
void do_evo_jobs (void)
{
   BOOL  job_done,     /* TRUE, wenn der Job - wie auch immer - erledigt ist. */
         loop_cont;    /* TRUE, solange die Evo-WS abgearbeitet werden soll.  */

   /* do_evo_jobs */
   evo_aktiv = FALSE;
   do
   { /* main loop on "evo_ws" */
      job_done = do_1_evo_job ();
      if (job_done || (err_state != GLEAM_OK) || mem_out)
	 loop_cont = do_job_end ();     /* Job normal oder mit Fehler beendet */
      else
      { /* Job unterbrochen */
         loop_cont = FALSE;
         evo_aktiv = TRUE;
      } /* Job unterbrochen */
   } /* main loop on "evo_ws" */
   while (loop_cont);
} /* do_evo_jobs */



#if !defined(GUI_MODE) && !defined(VISU_MODE)
  static
#endif /* weder GUI_MODE noch VISU_MODE */
/* ============================== do_1_evo_job ============================== */
BOOL do_1_evo_job (void)
/*----------------------------------------------------------------------------*/
/*  Die Routine bearbeitet einen Evo-WS-Eintrag und liefert TRUE, wenn dessen */
/*  Abarbeitung ohne Fehler beendet wurde. Die Routine liefert FALSE, wenn    */
/*  der "err_state" schlecht ist oder der Speicher ausging ("mem_out" ist     */
/*  TRUE) oder ein Unterbrechungswunsch vorliegt ("taste" ist TRUE).          */
/*  Der Job wird mit der vorgegebenen Individuenzahl ("popel_size") solange   */
/*  ausgefuehrt, bis eines der folgenden vier Kriterien zutrifft:             */
/*    1) Erreichen der Zielnote "ziel_note". Der Evo-Job ist damit fertig.    */
/*    2) Die "job_zeit" ist verbraucht und der Evo-Job damit erledigt.        */
/*    3) Es wurde eine Taste gedrueckt. Die Abarbeitung des EVO-Jobs wird     */
/*       damit unterbrochen. Bei der Weiterbearbeitung kommt es zu keiner     */
/*       Abkuerzung der Bearbeitung der aktuellen Population, da die Zeit-    */
/*       erfassung unterbrochen wird. Es wird ab der Unterbrechung ohne Ver-  */
/*       luste weitergearbeitet.                                              */
/*    4) Es trat ein Fehler auf (Mem-Out oder Fehler mit schlechtem Status).  */
/*----------------------------------------------------------------------------*/
{
   LONG    max_job_zeit;
   INT     ii,
           wdh_max, gen_no_accept_max, genno_deme_impr_max, psize;
   BOOL    init_ok, initIntrpt, lamarck,
           opt_err,     /* TRUE, wenn "mem_out" oder "err_state != GLEAM_OK"  */
#if !defined(GUI_MODE) && !defined(VISU_MODE)
           job_aktiv,   /* TRUE, solange keine JobEnde-Bedingung erfuellt ist */
#endif /* weder GUI_MODE noch VISU_MODE */
           opt_ok;      /* Loop-Control: False=Stop Loops aus vielen Gruenden */
   GSTATE  gstat = GLEAM_OK;
   DOUBLE  bestInitNote;
   
   /* do_1_evo_job */ 
   mem_out    = FALSE;
   initIntrpt = FALSE;
   err_state  = GLEAM_OK;

   if (!evo_ws[first_evo_item].deaktiviert)
   { /* Kein geloeschter Eintrag */
      psize               = evo_ws[first_evo_item].popel_size;
      act_survival_rule   = evo_ws[first_evo_item].survival_rule;
      max_fit             = evo_ws[first_evo_item].rank_param;
      lamarck_rate        = evo_ws[first_evo_item].lamarck_rate;
      act_opt_strategy    = evo_ws[first_evo_item].opt_strategy;
      wdh_max             = evo_ws[first_evo_item].generationen;
      gen_no_accept_max   = evo_ws[first_evo_item].gen_no_accept;
      genno_deme_impr_max = evo_ws[first_evo_item].genno_deme_impr;
      zielnote            = evo_ws[first_evo_item].ziel_note;
      xo_min_ham_dist     = evo_ws[first_evo_item].xo_min_ham_dist;
      lsv_strat           = evo_ws[first_evo_item].lsv_strategy;
      init_strat          = evo_ws[first_evo_item].init_strat;
      do_evo              = act_opt_strategy <= GLEAM_AMMA;
#ifdef CEC_MBF
      maxFEs    = evo_ws[first_evo_item].maxFEs;
#else /* kein CEC_MBF */
      evalLimit = evo_ws[first_evo_item].maxFEs;
#endif /* kein CEC_MBF */
#ifdef MIT_LSV
      gleamPur            = act_opt_strategy == GLEAM;
      gleam_asma          = act_opt_strategy == GLEAM_ASMA;
      gleam_amma          = act_opt_strategy == GLEAM_AMMA;
      gleam_adi           = gleam_asma || gleam_amma;
      if (((act_opt_strategy == GLEAM_SMA) || gleam_adi || (act_opt_strategy == LSV) || 
	   (init_strat == LSV_NEU) || (init_strat == LSV_BEST)) && (lsv_anz == 0))
      { /* Keine LSVs definiert aber durch den Job verlangt! */
	 err_state = fatal (EVO_STEU, 1, NO_INFO, NO_INFO, "", REQUIRED_LHCS_NOT_AVAIL);
	 return (FALSE);
      } /* Keine LSVs definiert aber durch den Job verlangt! */
      if ((act_opt_strategy == GLEAM_SMA) || gleam_adi)
      { /* SMA, ASMA oder AMMA */
         lsv_all_children = evo_ws[first_evo_item].lsv_all_offspr;
         lsv_best_child   = !lsv_all_children;
      } /* SMA, ASMA oder AMMA */
      else
#endif /* MIT_LSV */
      { /* GLEAM oder LSV */
         lsv_best_child   = FALSE;
         lsv_all_children = FALSE;
      } /* GLEAM oder LSV */
      job_aktiv    = TRUE;
      max_job_zeit = evo_ws[first_evo_item].job_zeit;
      if (evo_ws[first_evo_item].z_init && !evo_intrpt) 
      { /* init random generator only for started jobs */
	 if (rand_seed == time (NULL))  /* Wurde seed-Wert bereits verwendet? */
	    seed_rand (10 * first_evo_item);  /* Init. des ZufGen. mit Offset */
	 else
	    seed_rand (0);   /* Standardinitialisierung des Zufallszahlengen. */
      } /* init random generator only for started jobs */

#ifdef INTRP_TST
      printf ("do_1_evo_job: taste=%d job_aktiv=%d evo_intrpt=%d\n", taste, job_aktiv, evo_intrpt);
#endif /* INTRP_TST */

      /*-------------- Job-Start-Statistik und Start-Anzeigen: -------------- */
      if (evo_intrpt)
      { /* Unterbrochenen Opt-Job fortsetzen */
         abgelaufene_zeit = saved_time;      /* f.die Routine "zeige_gen_erg" */
         start_timer (saved_time);    /* Ermittle korrigierten Startzeitpunkt */
         evo_intrpt = FALSE;
	 if (jobInterruptPhase == POP_INIT_EVAL_PHASE)
	    init_ok = evaluateStartPop();
	 else
	    init_ok = TRUE;
         showActJobDataOnContinuation();
	 showOptRunHdr(SHOW_RESTART);
      } /* Unterbrochenen Opt-Job fortsetzen  */
      else
      { /* Job-Start */
         clear_job_stat_dat (&akjob_stat_dat);
         showActJobDataOnProcessingStart();
#ifdef MIT_LSV
         if (gleam_adi)
            reset_adi ();                                   /* ASMA oder AMMA */
#endif /* MIT_LSV */
#ifdef EXT_SIMU_SERV
	 if (mit_ext_simu && !sim_up)
	    if (!ext_simu_hochfahren(TRUE))   /*TRUE: incl.ext.sim.termination*/
	       return (FALSE);
#endif /* EXT_SIMU_SERV */
	 if (statistik_pro_gen)
	    startTimerMS();                    /* start timer in microseconds */
	 taste                    = FALSE;
	 bestJobNote              = 0.0;
         int_sim_data_ready       = FALSE;
         reset_simu_erg ();
         saved_time               = 0;
         gen_counter              = 0;
         no_accept_gen_counter    = 0;
         no_deme_impr_gen_counter = 0;
         gen_note                 = 0.0;
         gen_len                  = 0;
         indiv_ctr                = 0;
	 jobInterruptPhase        = POP_INIT_GEN_PHASE;
         start_timer (0);                          /* Ermittle Startzeitpunkt */
	 init_ok = init_ep (&gen_note, psize, evo_ws[first_evo_item].strat_par);
	 bestJobNote = gen_note;            /* beste Note der Startpopulation */
	 akjob_stat_dat.best_ch_note = gen_note;
	 akjob_stat_dat.best_ch_len  = gen_len;
         abgelaufene_zeit = calc_elapsed_time ();
	 showOptRunHdr(SHOW_START);
#if defined(GLEAM_USR) && defined(EXT_SIMU_SERV)
	 initIntrpt = taste && !init_ok && (err_state == GLEAM_OK);
#endif /* GLEAM_USR and EXT_SIMU_SERV */
	 bestInitNote = gen_note;
#if defined(CEC_MBF)
         aktFEsLim = 500;
         fesT      = TRUE;
         fesZT     = TRUE;
         fesHT     = TRUE;
         fesMax    = TRUE;
         sZeile    = TRUE;
         sprintf (buf3, "#L 0 %ld %g", indiv_ctr, cec_best_fval - cec_fopt);
         write_to_log (buf3, CLOSE_LOG);
#endif /* CEC_MBF */
      } /* Job-Start */

#if defined(INTRP_TST) || defined(ABLAUF_TST)
      printf ("do_1_evo_job: taste=%d init_ok=%d (gen_note < zielnote)=%d\n", 
	      taste, init_ok, (gen_note < zielnote)); 
#endif /* INTRP_TST or ABLAUF_TST */

      /* -------------------------- Job ausfuehren: ------------------------- */
      if (init_ok)
      { /* --------------------- Optimierungs-Schleife ---------------------- */
         if (gen_note < zielnote)
         { /* Zielnote nach Init (oder Unterbrechung) unerreicht */
            if (do_evo)
            { /* Evolution */
               do
               { /* Alle Generationen einer Population */
                  opt_ok = evo_1gen ();
                  abgelaufene_zeit = calc_elapsed_time (); 
                  if (opt_ok || intrpt_at_gen_end)
                  { /* Keine Fehler oder Unterbrechung bei Generationsende */
                     gen_counter++;
                     showGenResults();           /* Ergebn.d.letzt.Generation */
                     if ((gen_counter % evo_ws[first_evo_item].save_rate) == 0)
                        save_ep ();
                  } /* Keine Fehler oder Unterbrechung bei Generationsende */
                  job_aktiv = ((abgelaufene_zeit < max_job_zeit) &&
                               (gen_counter      < wdh_max)      &&
                               (bestJobNote      < zielnote)     &&
#ifdef CEC_MBF
                               (indiv_ctr        < maxFEs)       &&
#else /* kein CEC_MBF */
			       (indiv_ctr        < evalLimit)    &&
#endif /* kein CEC_MBF */
                               (no_accept_gen_counter    < gen_no_accept_max) &&
                               (no_deme_impr_gen_counter < genno_deme_impr_max));
               } /* Alle Generationen einer Population */
               while (job_aktiv && opt_ok);
#if defined(INTRP_TST) || defined(ABLAUF_TST)
	       printf ("do_1_evo_job: job_aktiv=%d opt_ok=%d\n", job_aktiv, opt_ok); 
#endif /* INTRP_TST or ABLAUF_TST */
            } /* Evolution */
#ifdef MIT_LSV
            else
            { /* Keine Evolution, also reines LSV */
	       lamarck = check_for_lamarck (lamarck_rate);
               if (!do_lsv(lamarck, 0))   /* znr=0: keine Textpuffer-Ausgaben */
		  err_state = fehler(EVO_STEU, 2, NO_INFO, NO_INFO, "", BAD_LSV_RUN);
               abgelaufene_zeit = calc_elapsed_time (); 
            } /* Keine Evolution, also reines LSV */
#endif /* MIT_LSV */
         } /* Zielnote nach Init (oder Unterbrechung) unerreicht */
	 else
	 { /* fitness goal achieved after init or reduced during interrupt */
	    job_aktiv = FALSE;                                 /* finish job! */
	    if ((jobInterruptPhase == POP_INIT_GEN_PHASE) || 
		(jobInterruptPhase == POP_INIT_EVAL_PHASE)  )
	       clearInitialChrList();
	    else
	       clearChrList(DEL_ALL_CHR_ENTRIES);
	 } /* fitness goal achieved after init or reduced during interrupt */
      } /* Optimierungs-Schleife */
      else 
	 if (!taste)
         { /* Fehler bei Initialisierung */
	    gen_note    = 0.0;
	    bestJobNote = 0.0;
	    gen_len     = 0;
	    job_aktiv   = FALSE;
	 } /* Fehler bei Initialisierung */
#ifdef ABLAUF_TST
      printf("do_1_evo_job: opt_ok=%d mem_out=%d taste=%d job_aktiv=%d  err_state=%d\n", 
	     opt_ok, mem_out, taste, job_aktiv, err_state);
#endif /* ABLAUF_TST */

      /* ------------------ Population beendet/unterbrochen: ---------------- */
      opt_err = mem_out || (err_state != GLEAM_OK) || !init_ok;
      if ((initIntrpt || init_ok) && taste && job_aktiv && !opt_err)
      { /* EvoJob-Unterbrechung und kein Fehlerabbruch */
         saved_time = abgelaufene_zeit;
         evo_intrpt = TRUE;
      } /* EvoJob-Unterbrechung und kein Fehlerabbruch */
      else
      { /* Population legal oder wg. Fehler beendet */
         if (gen_note > akjob_stat_dat.best_ch_note)
         { /* Erfasse interne Statistikdaten der Population */
            akjob_stat_dat.best_ch_note = gen_note;
            akjob_stat_dat.best_ch_len  = gen_len;
         } /* Erfasse interne Statistikdaten der Population */
         if (err_state != GLEAM_FATAL) 
         { /* Kein fataler Fehlerabbruch */
            if (init_ok && (evo_ws[first_evo_item].anz_erg_ketten > 0))
            { /* Opt (teilweise) gelaufen und Ergebnis-Kette(n) verlangt */
               if (do_evo)
               { /* rette Evo-Ergebnis-Kette(n) */ 
#ifndef PAR_POP_VERS
		  (akjob_stat_dat.anz_res_chs) +=                  /* FALSE = ohne Anzeige */
		     get_best (evo_ws[first_evo_item].anz_erg_ketten, FALSE, &gstat);
#endif /* PAR_POP_VERS */
                  err_state |= gstat;
               } /* rette Evo-Ergebnis-Kette(n) */ 
               else  /* sonstige Optimierung */
		  err_state |= store_res_ch (&ii, 0, AK);     /* Kette retten */
            } /* Opt (teilweise) gelaufen und Ergebnis-Kette(n) verlangt */
         } /* Kein fataler Fehlerabbruch */
#if defined(GLEAM_USR) 
         if ((bestJobNote >= zielnote) && job_ende_ton)
            drei_klang (LANG_TON);
#endif /* GLEAM_USR */
      } /* Population legal oder wg. Fehler beendet */

#if defined(INTRP_TST) || defined(ABLAUF_TST)
      printf ("do_1_evo_job: taste=%d job_aktiv=%d opt_err=%d\n", taste, job_aktiv, opt_err);
#endif /* INTRP_TST or ABLAUF_TST */

      /* Taste gilt nur bei aktivem Job und Fehlerfreiheit: */
      taste = taste && job_aktiv && !opt_err; 

      /* ------------------- Job beendet oder abgebrochen: ------------------ */
      if (!taste)
      { /* Job legal oder wegen Fehler beendet */
	 if ((akjob_stat_dat.best_ch_note > ak_jobli_stat_dat.best_ch_note) && 
	     (err_state != GLEAM_FATAL))
         { /* Erfasse Statistikdaten des Jobs */
	    ak_jobli_stat_dat.best_ch_note = akjob_stat_dat.best_ch_note;
            ak_jobli_stat_dat.best_ch_len  = akjob_stat_dat.best_ch_len;
	 } /* Erfasse Statistikdaten des Jobs */
	 ak_jobli_stat_dat.anz_res_chs += akjob_stat_dat.anz_res_chs;
	 if (opt_err)
	    write_to_log(JOB_TERMINATED_TXT, CLOSE_LOG);
	 showOptRunHdr(SHOW_END);
	 optJobEndStatistics();
	 jobInterruptPhase = NULL_PHASE;   /* phase of job execution finished */
	 ges_ak_stat_dat.job_anz++;                 /* Datenerf: Gesamt-Stat. */
	 if (do_evo)
	    ges_ak_stat_dat.popel_anz++;            /* Datenerf: Gesamt-Stat. */
	 if (init_ok && do_evo && (err_state == GLEAM_ERROR)) 
	    if (!clearChrList(DEL_ALL_CHR_ENTRIES))
	       err_state = GLEAM_FATAL;  /* errors while deleting chromosomes */
         if (evo_ws[first_evo_item].z_init && (rand_seed == time (NULL)))
            warte (1000);                    /* keine gleichen Zufallszahlen! */
      } /* Job legal oder wegen Fehler beendet */
   } /* Kein geloeschter Eintrag */
   else
   { /* Uebergehe geloeschten Evo-Job-Eintrag */
      init_ok = TRUE;
      opt_err = FALSE;
   } /* Uebergehe geloeschten Evo-Job-Eintrag */
 
   if (mem_out)
      meldung (EVO_STEU, NO_INFO, NO_INFO, "", NO_MEM);
#ifdef INTRP_TST
      printf ("do_1_evo_job: Ende! init_ok=%d taste=%d opt_err=%d err_state=%d job_aktiv=%d\n", 
	      init_ok, taste, opt_err, err_state, job_aktiv);
#endif /* INTRP_TST */
   return (init_ok && !(taste || opt_err));
} /* do_1_evo_job */ 



/* =========================== clear_job_stat_dat =========================== */
static void clear_job_stat_dat (JOB_STAT_TYPE *job_sdata)
/*----------------------------------------------------------------------------*/
/*  Loescht die Statistikdaten von Jobs.                                      */
/*----------------------------------------------------------------------------*/
{
  job_sdata->best_ch_len  = 0;
  job_sdata->best_ch_note = 0;
  job_sdata->anz_res_chs  = 0;
} /* clear_job_stat_dat */ 



/* ============================ clearEvoJobIntrpt =========================== */
void clearEvoJobIntrpt (void)
{
#ifdef EXT_SIMU_SERV
   abortJobOfWorkflow();                  /*  abort present SimuJob or LSVJob */
   if ((jobInterruptPhase == POP_INIT_GEN_PHASE) || (jobInterruptPhase == POP_INIT_EVAL_PHASE))
      clearInitialChrList();
   else
      clearChrList(DEL_ALL_CHR_ENTRIES);
#endif /* EXT_SIMU_SERV */
   evo_intrpt        = FALSE;
   evo_aktiv         = FALSE;
   intrpt_at_gen_end = FALSE;
   taste             = FALSE;
   jobInterruptPhase = NULL_PHASE;
   if (best_child_ptr != NULL)
      err_state |= delete_chain (&best_child_ptr);
} /* clearEvoJobIntrpt */



/* =========================== clear_evo_results ============================ */
void clear_evo_results (GES_STAT_TYPE *stat_dat)
{
   stat_dat->job_anz      = 0;
   stat_dat->best_note    = 0.0;
   stat_dat->res_ch_sum   = 0;
   stat_dat->popel_anz    = 0;
   stat_dat->gen_anz      = 0;
   stat_dat->zeit_sum     = 0;
   stat_dat->jobli_indivs = 0;
} /* clear_evo_results */ 



/* ============================= init_evo_steu ============================== */
BOOL init_evo_steu (void)
{
   /* init_evo_steu */   
   if ((evo_ws = (EVO_WS_TYPE*) malloc(MAX_EVO_WS * sizeof(EVO_WS_TYPE))) == NULL)
      return (FALSE);
   
   job_ende_ton            = FALSE;  /* aktuell exportiert aber nicht weiter behandelt */
   first_evo_item          = 0;
   free_evo_index          = 0;
   last_psize              = 0;
   retteGorbaPlanData      = FALSE;
   evo_aktiv               = FALSE;
   evo_intrpt              = FALSE;
   saved_time              = 0;
   clear_job_stat_dat (&ak_jobli_stat_dat);
   clear_job_stat_dat (&rk_jobli_stat_dat);
   clear_evo_results(&ges_ak_stat_dat);
   return (TRUE);
} /* init_evo_steu */
