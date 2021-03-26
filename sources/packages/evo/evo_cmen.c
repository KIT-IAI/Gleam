/*******************************************************************************
HyGLEAM                                                                   V2.1.7

                                 H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: EVOlution C-MENue
 
Package: evo                    File: evo_cmen.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt das Optimierungsjob-Menue des interaktiven Parts in der 
TEXT_MODE-Variante. Mit dem Menue koennen
  - Jobs definiert, kopiert, veraendert, gelistet und geloescht werden,
  - die Jobbearbeitung gestartet werden,
  - die Ergebnisse des letzten Jobs gespeichert werden und
  - die Defaultwerte der Evolution veraendert werden.


Beim Loeschen von Optimierungsjobs gibt es nachstehende Faelle, wobei in der 
unteren Zeile Kuerzel fuer die notwendigen Anweisungen angegeben werden:

Evo ist aktiv:
---------------
1 Job in List |                        n Jobs in Liste                       |
              | 1.Job ausgew.  | x-ter Job  | alle war-   | alle Jobs ausgew.|
   1.Job      |    1.Job       | ausgewaehlt| tenden Jobs |     1.Job        |
fertig  Abbr. | fertig  Abbr.  |            | ausgew.     | fertig   Abbr.   | 
--------------+----------------+------------+-------------+------------------+
1Res   Abort1 | 1Res    Abort2 |  1Deakt    | 1Res        | LoeLi    Abort1  | 
              |                |            |             | 1Res             | 

Evo ist nicht aktiv:
--------------------
1 Job in |    n Jobs in Liste    |
  Liste  |                       |
         | x-ter Job | alle Jobs |
         |   ausgew. |  ausgew.  |  
---------+-----------+-----------+
 ResetLi |  1Deakt   | ResetLi   |


Zusammenfassung der Anweisungssequenzen mit deren (Haeufigkeit) und die Umsetzung 
durch eine Funktion bzw. durch direkte Codierung:

Res (3):      finishActJob():
evo_ws[first_evo_item].job_zeit = 0;
do_evo_jobs ();


Abort1 (2):              } abortJobList():
clearEvoJobIntrpt();     } clearEvoJobIntrpt();
first_evo_item = 0;      } resetJobList();
free_evo_index = 0;      }


Abort2 (1):   direkte Codierung:
clearEvoJobIntrpt();
evo_ws[index].deaktiviert = TRUE;


1Deakt (2):   direkte Codierung:
evo_ws[index].deaktiviert = TRUE;


LoeLi (2):    delWaitingJobs():
for (ii = first_evo_item + 1;  ii < free_evo_index;  ii++)
  evo_ws[ii].deaktiviert = TRUE;


ResetLi (2):  resetJobList():
first_evo_item = 0;
free_evo_index = 0;


Die Routinen im einzelnen:

void evo_cmenue (void);
        Evo/Opt-Menue: Menue zur Erstellung, Verwaltung und Durchfuehrung von 
        Optimierungsjobs und zur Vorgabe von Optimierungsparametern.

--------------------------------------------------------------------------------
basiert auf den C-Units: gado_gsw, gado_smen
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. PSRO-ReInit-Strategie und -Strategie-Parameter als EvoParams integriert.
 2. Populationsanzeige im BGI-Mode bei Funktion "ReteErgAK" integriert.
 3. Rankingparameter wird Bestandteil der Evo-Job-Daten.
 4. Verbesserung von "RetteErgAK" und "Lösche Job" (mit Ergebnisrettung).
 5. Ueberarbeitung der Menuesteuerung.                               WJ 22.12.98
 6. Entfernung des Sintflut-Algorithmus und des PSRO; 
    ParVers: Korrektur der Abfrage beim Jobstart, spezifischere
             JobDef-Dialoge.                                         WJ 23.08.00
 7. Erweiterungen fuer LSV.                                          WJ 19.09.00
 8. Neue LSV-Praezisionen X=(XL) und Y(=XXL)                         WJ 11.10.00
 9. Korrektur in "modif_evo_defaults".                               WJ 26.10.00
10. Default bei JobDef in "modif_ak_evojob" fuer NachOpt justiert.   WJ 15.11.00
11. Generationslimit und Limits f. GDV und GAk auf INT_MAX eroeht.   WJ 30.11.00
12. Umbenennung Baldwin -> Lamarck. Steuerung ob eine oder beide 
    Bedingungen zum EvoAbbruch fuer Nachopt erfuellt sein muessen.
    Verzoegertes LSV-Zuschalten nach Nischenbildung.                 WJ 11.12.00
13. Praezision "S" (Spezial) eingefuehrt.                            WJ 22.02.01
14. Separate GDV- und GAk-Limits fuer Nischencheck eingefuehrt.      WJ 22.03.01
15. Zielnotenvorgabe in DOUBLE-Genauigkeit, Anzeige als Int.         WJ 03.04.01
16. Im Rahmen der Integration des COMPLEX-Algorithmus Anpassungen
    einiger Job-Dialoge und -Defaultwerte.                           WJ 12.05.01
17. Kleine Korrektur f. die Zufallszahleninitialisierung bei GLEAM.  WJ 14.04.04
18. MIT_ADI: Anpassungen fuer die adaptive direkte Integration.      WJ 26.07.04
19. MIT_ADI: ADI nur bei gesetztem MIT_ADI als Verfahren waehlbar.   WJ 17.08.04
20. Loeschung einzelner Jobs korrigiert, so dass unterbrochener und
    fortzusetzender Job nicht neu startet (Zeilen 449 ff).           WJ 15.09.04
21. Complex-MaxIter fuer HyGLEAM einstellbar gemacht.                WJ 06.10.04
22. Anpassungen fuer ADI-Version 2.                                  WJ 25.12.04
23. Anpassungen beim Aufruf von "set_lsv_limits" fuer OPAL/V.        WJ 05.01.05
24. MIT_ADI: Anpassungen fuer "step_r" (step-Parameter des Rosenbr). WJ 07.01.05
25. Linux-Portierung: Aus "round" wird "l_round".                    WJ 12.05.05
26. Erweiterung fuer n LSVs und Heuristiken. Entfernung von MIT_ADI. WJ 30.08.06
27. Erweiterung fuer getrennte LSV- und Level-Adaptionsgeschw.       WJ 16.10.06
28. Korrektur in "modif_evo_defaults" und "modif_ak_evojob" wegen 
    GLEAM mit Vor-Initialisierung.                                   WJ 09.01.07
29. Korrektur in "modif_evo_defaults" beim Schalten d.PhaenoRepair.  WJ 13.02.07
30. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
31. Kleine Korrektur in "modif_ak_evojob" wegen MIT_ROCO.            WJ 04.11.09
32. Gesetztes MIT_GORBA_LSVS: "lamarck_rate" ist nicht veraenderbar. WJ 27.11.09
33. Entfernung von Code im Kontext der Schalter: BGI_MODE, BS_DOS. 
    Entfernung ueberfluessiger Variable. Initialisierung von "do_it"
    in "evo_cmenue".                                                 WJ 28.06.12
34. Einfuehrung von MIN_ANZ_NACHBARN.                                WJ 31.08.12
35. Einbau einer Abbruchschranke fuer die Anzahl der Evaluationen.   WJ 25.01.13
36. Korrektur in "evo_cmenue" beim Loeschen einzelner Jobs.          WJ 01.12.14
37. Renovierung: Korrektur bei Texten mit dem %-Zeichen. Streichung 
    des "link"-Includes. Anpassung des "write_text_buf"-Aufrufs.
    Formale Korrektur in "lsv_abfrage". Streichung des Scrollings bei
    der Joblistenanzeige. Anpassung folgender Aufrufe: "char_dialog", 
    "lies_...", "set_and_clear_menue_window", "select_menue_item", 
    Streichung der Aufrufe von: "clr_line", 
    Ersetzung der "write_line"- durch "printf"-Aufrufe. 
    Streichung der Zeilennummerbehandlung von "modif_ak_evojob" und
    "lsv_abfrage".                                                   WJ 04.05.16
38. Evo-Jobs werden mit "initZufGen" initialisiert.                  WJ 18.07.16
39. Akzeptanzregeln von ACCEPT_ALL(_ES) auf ALWAYS(_ES) umgestellt.
    Integration der neuen TSK-Datenverwaltung darunter neue Default-
    variable fuer entsprechende TSK-Daten. Anpassung der Vorgabe der 
    Sicherungsrate an INT_MAX.                                       WJ 15.12.16
40. Verbesserung der Abfrage zur LSV-Abschaltung in 
    "modif_evo_defaults". Verbesserung in "lsv_abfrage".             WJ 31.03.17
41. Einbindung von SIMU_TEXTS. Reunion of "evoIntg.h" and 
    "evoInts.h" in "evoInt.h". Deletion of old PAR_VERS code parts.  WJ 04.07.17
42. Ueberarbeitung des "Loesche Job"-Menuepunkts. Ersetzung von 
    "clear_evo_job_intrpt" durch "clearEvoJobIntrpt".                WJ 15.09.17
43. Vertauschung der Reihenfolge der Dialoge fuer GAk und GDV 
    in "modif_ak_evojob".                                            WJ 13.10.17
44. Vereinheitlichung der Abbruchmeldung: Aus NOLOAD_EVO_TXT wird 
    FKT_DISABLED_TXT. Neuer Parameter fuer "show_and_clear_fbuf".
    "modif_ak_evojob" erlaubt bei gesetztem EXT_SIMU_SERV fuer die 
    Initialisierungsstrategie NEU nur 0 als Strategieparameter.      WJ 15.11.17
45. Streichung des Includes von SIMU_TEXTS.                          WJ 10.01.18
46. Deletion of all code related to basic chains (BAKs).             WJ 18.06.18
47. Fehler und kein Joblistenstart, wenn mehr Bewertungskriterien 
    als Simulationsergebnisse spezifiziert wurden.                   WJ 14.02.19
48. modif_ak_evojob(): Avoiding buffer overflow of static strings. 
    Correction of an error at the initialization of LHC jobs.        WJ 08.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 08.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "appl.h"
#include "hmod.h"
#include "bew.h"
#include "simu.h"
#include "evo.h"
#include "evoInt.h"

#include GLOBAL_TEXTS
#include EVO_TEXTS



/* ============================ Lokale Routinen: ============================ */
#if defined(MIT_LSV)
  static INT  lsv_abfrage      (INT   vorgabe);
#endif /* MIT_LSV */

static BOOL modif_ak_evojob    (INT   index, 
                                BOOL  neu_def, 
                                BOOL  aktiv);

static void finishActJob       (void);

static void abortJobList       (void);

static void delWaitingJobs     (void);

static void resetJobList       (void);

static void modif_evo_defaults (void);



/* =============================== evo_cmenu ================================ */
void evo_cmenue (void)
{
   STR_TYPE  buffer, buf1;
   double    note;                    /* AK-Note                              */
   GSTATE    local_state = GLEAM_OK;  /* lokaler Return-Status                */
   INT       men_item,                /* Ausgewaehltes Menue-Item             */
             job_nr,                  /* Job-Nummer                           */
             index,                   /* allgemeine Index-Variable            */
             lfd_nr,                  /* Laufende Nummer (ab 1)               */
             max_anz,                 /* allgemeine Obergrenze                */
             anz,                     /* allgemeine Anzahl-Variable           */
             seq_anz,                 /* Jobanz einer Kopiersequenz           */
             guete,                   /* AK-Guete                             */
             ii, jj;
   char      ch;                      /* Eingabe                              */
   BOOL      sub_men_fin,             /* TRUE, wenn Evo-Menue zu beenden ist. */
             ausgeben,                /* Hilfsvariable zur Ausgabe-Steuerung  */
             do_it,                   /* Hilfsvariable f.Joblisten-Ausfuehrung*/
             fkt_done;                /* TRUE, wenn Funktion ausgefuehrt.     */

  /* evo_cmenu */
   sub_men_fin = FALSE;
   do_it       = FALSE;
   do 
   { /* Menue-Loop */
      set_and_clear_menue_window (2, EVO_MEN_TXT);
      write_menue_item ( 1, StndMenItem, JOB_DEF_MEN);
      write_menue_item ( 2, StndMenItem, JOB_AEND_MEN);
      write_menue_item ( 3, StndMenItem, OPT_START_MEN);
      write_menue_item ( 5, StndMenItem, ZEIG_JOBLI_MEN);
      write_menue_item ( 6, StndMenItem, COPY_JOB_MEN);
      write_menue_item ( 7, StndMenItem, LOE_JOB_MEN);
      write_menue_item ( 9, StndMenItem, RET_ERG_AK_MEN);
      write_menue_item (10, StndMenItem, RET_BEST_MEN);
      write_menue_item (11, StndMenItem, "ShowEvoPar");
      write_menue_item (12, StndMenItem, EVO_PARAM_MEN);
      write_menue_item (14, AttMenItem,  DEL_EP_MEN);
      menue_completion ();
      men_item = select_menue_item ();
      reset_text_buf ();
      switch (men_item) 
      {
         case 1: /* ----------------------- DefOptJob ----------------------- */
	    if (err_state == GLEAM_FATAL)
            { /* Abbruch */
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;                                              /* Abbruch */
	    } /* Abbruch */
#ifdef MIT_LSV
	    if (lsv_anz == 0)
	       sprintf (buffer, GJOB_DEF_HDR_TXT, free_evo_index+1);
	    else
	       sprintf (buffer, JOB_DEF_HDR_TXT, free_evo_index+1);
#else
            sprintf (buffer, GJOB_DEF_HDR_TXT, free_evo_index+1);
#endif /* MIT_LSV */
	    printf ("\n%s\n", buffer);
	    if (err_state != GLEAM_OK)
            { /* Fehlersituation */
	       printf ("%s\n", NIX_TUN_ERR_TXT);
	       break;                                              /* Abbruch */
	    } /* Fehlersituation */
	    /* ----- Default-Vorgaben: ----- */
	    evo_ws[free_evo_index].deaktiviert      = FALSE;
	    evo_ws[free_evo_index].ziel_note        = defTargetFitness;
	    evo_ws[free_evo_index].job_zeit         = defTimeLimit;
	    evo_ws[free_evo_index].strat_par        = defInitStratPar;
	    evo_ws[free_evo_index].ketten_art       =  AK;   
	    evo_ws[free_evo_index].popel_size       = def_psize;
	    evo_ws[free_evo_index].generationen     = defGenLimit;
	    evo_ws[free_evo_index].gen_no_accept    = defGAcLimit;
	    evo_ws[free_evo_index].genno_deme_impr  = defGDI_Limit;
	    evo_ws[free_evo_index].maxFEs           = defEvalLimit;
	    evo_ws[free_evo_index].anz_erg_ketten   = defResultChrs;
	    evo_ws[free_evo_index].save_rate        = save_rate_def;
	    evo_ws[free_evo_index].init_strat       = defInitStrat;
	    evo_ws[free_evo_index].xo_min_ham_dist  = def_xo_min_ham_dist;
	    evo_ws[free_evo_index].survival_rule    = default_survival_rule;
	    evo_ws[free_evo_index].rank_param       = def_max_fit;
	    evo_ws[free_evo_index].opt_strategy     = def_opt_strat;   
	    evo_ws[free_evo_index].z_init           = initZufGen;
	    evo_ws[free_evo_index].lamarck_rate     = def_lamarck_rate;
	    evo_ws[free_evo_index].lsv_strategy     = def_lsv_strat;
	    evo_ws[free_evo_index].lsv_all_offspr   = withAllImprovement;
	    evo_ws[free_evo_index].deaktiviert      = FALSE;
	    if (modif_ak_evojob (free_evo_index, TRUE, FALSE))
	       free_evo_index++;                 /* neu_def=TRUE, aktiv=FALSE */
	    else
	       printf ("\n%s\n", JOB_DEF_ABR_TXT); 
         break; /* DefOptJob */

         case 2: /* ---------------------- AendOptJob ----------------------- */
            if (err_state == GLEAM_FATAL)
            { /* Abbruch */
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;
	    } /* Abbruch */
	    printf ("\n%s\n\n", JOBAEND_HDR_TXT);
	    if (err_state != GLEAM_OK)
            { /* Fehlersituation */
	       printf ("%s\n", NIX_TUN_ERR_TXT);
	       break;                                              /* Abbruch */
	    } /* Fehlersituation */
	    if (first_evo_item == free_evo_index)
	       printf ("\n%s\n", JOBLI_LEER_TXT);
	    else
            { /* Jobliste nicht leer */
               if ((first_evo_item + 1) == free_evo_index)     /* nur 1 Job ? */
                  fkt_done = modif_ak_evojob (first_evo_item, FALSE, evo_aktiv);
               else
               { /* Es gibt mehrere Jobs */
		  sprintf (buffer, JOB_NR_QUERY, first_evo_item+1, free_evo_index);
		  if (lies_wert (&job_nr, buffer, first_evo_item + 1, 
				 free_evo_index, free_evo_index+1))
		     break;                                        /* Abbruch */
		  index = job_nr - 1;
		  fkt_done = modif_ak_evojob (index, FALSE, evo_aktiv && 
					      (index == first_evo_item));
	       } /* Es gibt mehrere Jobs */
	       if (!fkt_done)
		  printf ("\n%s\n", JOBAEND_ABR_TXT);
	    } /* Jobliste nicht leer */
         break; /* AendOptJob */

         case 3: /* ----------------------- StartJobLi ---------------------- */
            if (err_state == GLEAM_FATAL)
            { /* Abbruch */
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;
	    } /* Abbruch */
	    if (akt_anz_krit > akt_anz_roh_erg_werte)
            { /* Abbruch */
	       fehler(EVO_CMEN, 1, akt_anz_krit, akt_anz_roh_erg_werte, "", MORE_CRITERIA_THAN_RESULTS);
	       show_and_clear_fbuf(LOG_ALL_MSGS);
	       break;
	    } /* Abbruch */
	    printf ("\n%s\n", JLSTART_HDR_TXT);
	    if (err_state != GLEAM_OK)
            { /* Fehlersituation */
	       printf ("%s\n", NIX_TUN_ERR_TXT);
	       break;                                              /* Abbruch */
	    } /* Fehlersituation */
	    if (first_evo_item == free_evo_index)
	       printf ("\n%s\n", JOBLI_LEER_TXT);
	    else
            { /* Job-liste starten */
	       if (mit_ext_simu)
               { /* externe Simulation */
		  printf ("\n%s\n", EXT_SIMU_STARTUP_TXT);
		  do_it = ext_simu_hochfahren(FALSE); /*check/init Modell u.Simu*/
		  write_text_buf (CLEAR);
		  show_and_clear_fbuf(LOG_ALL_MSGS);  /*MeldgAusgabe+Loeschung*/
	       } /* externe Simulation */
	       else
		  do_it = TRUE;
	       if (do_it)
		  do_evo_jobs ();
	    } /* Job-liste starten */
         break; /* StartJobLi */

         case 5: /* ---------------------- ZeigeJobLi ----------------------- */
	    prepJobListDisplay ();
	    write_text_buf (CLEAR);
         break; /* ZeigeJobLi */

         case 6: /* ----------------------- KopiereJob ---------------------- */
            if (err_state == GLEAM_FATAL)
            { /* Abbruch */
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;
	    } /* Abbruch */
	    printf ("\n%s\n\n", JOBCOPY_HDR_TXT);
	    if (err_state != GLEAM_OK)
            { /* Fehlersituation */
	       printf ("%s\n", NIX_TUN_ERR_TXT);
	       break;                                               /* Abbruch */
	    } /* Fehlersituation */
	    if (first_evo_item == free_evo_index)
	       printf ("%s\n", JOBLI_LEER_TXT);
	    else 
            { /* Es gibt Jobs */
	       max_anz = MAX_EVO_WS - free_evo_index;
	       if (max_anz > 0) 
               { /* Es ist noch Platz da */
                  if ((first_evo_item + 1) == free_evo_index)
                  { /* Es gibt nur einen Job */
		     job_nr  = free_evo_index;
		     seq_anz = 1;
		  } /* Es gibt nur einen Job */         
		  else
                  { /* Es gibt mehrere Jobs */
		     sprintf (buffer, JOB_NR_QUERY, first_evo_item + 1, free_evo_index);
		     if (lies_wert (&job_nr, buffer, first_evo_item + 1, 
				    free_evo_index, free_evo_index+1))
			break;                                     /* Abbruch */
		     if (job_nr < free_evo_index)
                     { /* Sequenzgroesse erfragen */
			sprintf (buffer, JOB_SEQ_LEN_Q, free_evo_index - job_nr + 1);
			if (lies_wert (&seq_anz, buffer, 1, 
				       free_evo_index - job_nr + 1, 1))
			   break;                                  /* Abbruch */
		     } /* Sequenzgroesse erfragen */
		     else
			seq_anz = 1;
		  } /* Es gibt mehrere Jobs */
		  index   = job_nr - 1;
		  max_anz = max_anz / seq_anz;
		  sprintf (buf1, JOBCOPY_ANZ_TXT, max_anz);
		  if (lies_wert (&anz, buf1, 0, max_anz, 1))
		     break;                                        /* Abbruch */
		  for (ii = 1;  ii <= anz;  ii++) 
                  { /* Kopiere eine Job-Sequenz */
                     for (jj = 0;  jj < seq_anz;  jj++)
                     { /* Kopiere einen Job */
			evo_ws[free_evo_index] = evo_ws[index+jj];
			free_evo_index++;
		     } /* Kopiere einen Job */
		  } /* Kopiere eine Job-Sequenz */
		  sprintf (buf1, JOBCOPY_OK_TXT, anz * seq_anz);
		  printf ("%s\n", buf1);
	       } /* Es ist noch Platz da */
	       else 
		  printf ("%s\n", JOBCOPY_NIX_TXT);
	    } /* Es gibt Jobs */
         break; /* KopiereJob */

         case 7: /* --------------------------- Loesche Job --------------------------- */
            if (err_state == GLEAM_FATAL)
	    { /* Abbruch */
	       printf(FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;                                                       /* abortion */
	    } /* Abbruch */
	    printf("\n%s\n\n", JOBLOE_HDR_TXT);
	    if (err_state == GLEAM_ERROR)
	       printf("%s\n", FKT_STOER_ERR_TXT);                    /* Fehlersituation */
	    if (first_evo_item == free_evo_index)
	       printf("%s\n", JOBLI_LEER_TXT);                  /* job list is empty! */
	    else 
            { /* Es gibt mindestens einen Eintrag */
	       if (evo_aktiv)
	       { /* job list is currently being processed */
		  if (first_evo_item == (free_evo_index - 1))
		  { /* the current job is the only one */
		     ch = char_dialog(JOBLOE_AKT1_TXT);          /* delete current job? */
		     if ((ch == JA) || (ch == CR))
                     { /* delete job */
			ch = char_dialog(JOBLOE_AKT4_TXT);              /* finish job ? */
			if (ch == JA)                     
			   finishActJob();
			else
			   abortJobList();
		     } /* delete job */
		  } /* the current job is the only one */
		  else
		  { /* there are at least two jobs */
		     sprintf(buffer, JOBLOE_NR_QUERY, first_evo_item+1, free_evo_index);
		     if (lies_wert(&job_nr, buffer, 0, free_evo_index, free_evo_index+1))
			break;                                              /* abortion */
		     if (job_nr == 0) 
                     { /* deletion of the entire list */
			ch = char_dialog(JOBLOE_WAIT_QUERY);
			if (ch == JA)
			{ /* delete waiting jobs only */
			   delWaitingJobs();
			   printf("%s\n", JOBLOE_LOE2_TXT);
			} /* delete waiting jobs only */   
			else
			{ /* delete complete list */
                           ch = char_dialog(JOBLOE_AKT4_TXT);    /* finish actual job ? */
                           if (ch == JA)
			   { /* finish the first job and delete all others */
			      delWaitingJobs();
			      finishActJob();
			   } /* finish the first job and delete all others */
			   else
			   { /* abort the first job and delete all others */
			      abortJobList();
			      printf("%s\n", JOBLOE_LOE1_TXT);
			   } /* abort the first job and delete all others */
			} /* delete complete list */
		     } /* deletion of the entire list */
		     else 
		     { /* deletion of one particular job */
			index = job_nr - 1;
			if (index < first_evo_item)
			   printf(JOBLOE_LOE4_TXT, job_nr);    /* already processed job */
			else
			   if (index == first_evo_item)
			   { /* delete current job */
			      ch = char_dialog(JOBLOE_AKT3_TXT); /* delete current job? */
			      if (ch == JA)                     
                              { /* delete job */
				 ch = char_dialog(JOBLOE_AKT4_TXT);     /* finish job ? */
				 if (ch == JA)                     
				    finishActJob();
				 else
				 { /* abort current job so that processing can go on */
				    sprintf(logBuffer, JOBLOE_ABORT_TXT, first_evo_item + 1);
				    write_to_log(logBuffer, CLOSE_LOG);
				    clearEvoJobIntrpt();
				    evo_ws[index].deaktiviert = TRUE;  /* deactivate it */
				    printf(JOBLOE_LOE3_TXT, job_nr);
				 } /* abort current job so that processing can go on */
			      } /* delete job */
			   } /* delete current job */
			   else
			   { /* delete 1 waiting job */
			      evo_ws[index].deaktiviert = TRUE;  /* deactivate this job */
			      printf(JOBLOE_LOE3_TXT, job_nr);
			   } /* delete 1 waiting job */
		     } /* deletion of one particular job */
		  } /* there are at least two jobs */
	       } /* job list is currently being processed */
	       else
	       { /* job list is currently not processed */
		  if (first_evo_item == (free_evo_index - 1))
		     do_it = TRUE;                                /* delete entire list */
		  else
		  { /* there at least two jobs */
		     sprintf(buffer, JOBLOE_NR_QUERY, first_evo_item+1, free_evo_index);
		     if (lies_wert(&job_nr, buffer, 0, free_evo_index, free_evo_index+1))
			break;                                              /* abortion */
		     if (job_nr == 0) 
			do_it = TRUE;                             /* delete entire list */
		     else
                     { /* deletion of one particular job */
			do_it = FALSE;
			evo_ws[job_nr-1].deaktiviert = TRUE;     /* deactivate this job */
			printf(JOBLOE_LOE3_TXT, job_nr);
		     } /* deletion of one particular job */
		  } /* there at least two jobs */
		  if (do_it)
		  { /* deletion of the entire list */
		     resetJobList();
		     printf("%s\n", JOBLOE_LOE1_TXT);
		  } /* deletion of the entire list */
	       } /* job list is currently not processed */
	    } /* Es gibt mindestens einen Eintrag */
         break; /* Loesche Job */

         case 9: /* ---------------------- RetteErgAK ----------------------- */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    printf ("\n%s\n", AKSAV_HDR_TXT);
	    if (evo_aktiv) {
	       printf ("\n%s\n", SAV_FKT_NIX_TXT);
	       break;                                              /* Abbruch */
	    }
	    if (last_psize == 0)
	       printf ("\n%s\n", AKSAV_NIXDA_TXT);
	    else
            { /* ok */
	       printf ("\n%s\n", AKSAV_INFO1_TXT);
	       printf ("%s\n", AKSAV_INFO2_TXT);
	       ch = char_dialog (AKSAV_SCANQUERY);
	       if (ch == ESC)
		  break;                                           /* Abbruch */
	       anz = 0;
	       if ((ch == JA) || (ch == CR))
               { /* Population durchscannen */
		  sprintf (buffer, AKSAV_STRTQUERY, last_psize);
		  if (lies_wert (&lfd_nr, buffer, 1, last_psize, last_psize+1))
		     break;                                        /* Abbruch */
		  printf ("\n%s\n", AKSAV_INFO3_TXT);
		  printf ("%s\n", AKSAV_DONE_HDR);
		  index = lfd_nr - 1;
		  do
                  { /* scanning loop */
		     guete = get_guete (&note, index);
		     if (guete != KEINE_GUETE)
                     { /* es gibt diese AK */
			sprintf (buffer, AKSAV_AK_QUERY, 
				 note, guete, index+1, NEIN_JA_TXT);
			ch = char_dialog (buffer);
			if (ch == ESC)
			   break;                                  /* Abbruch */
			if (ch == JA)
                        { /* rette AK */
			   local_state = keep_result_chain (index, AK);
			   write_text_buf (CLEAR);
			   (ges_ak_stat_dat.res_ch_sum)++;
			} /* rette AK */
		     } /* es gibt diese AK */
		     index++;
		  } /* scanning loop */
		  while ((index < last_psize) && (local_state == GLEAM_OK));
		  if (ch == ESC)
		     printf ("%s\n", AKSAV_WUERG_TXT);
		  else
		     printf ("%s\n", AKSAV_ENDE_TXT);
		  err_state |= local_state;
	       } /* Population durchscannen */
	       else
               { /* AK per lfd.Nr. retten */
		  sprintf (buffer, AKSAV_IDX_QUERY, last_psize);
		  if (lies_wert (&lfd_nr, buffer, 1, last_psize, last_psize+1))
		     break;                                        /* Abbruch */
		  index = lfd_nr - 1;
		  err_state |= keep_result_chain (index, AK);
		  (ges_ak_stat_dat.res_ch_sum)++;
		  write_text_buf (CLEAR);
	       } /* AK per lfd.Nr. retten */
	    } /* ok */
         break; /* RetteErgAK */

         case 10: /* ---------------------- RetBestErg ---------------------- */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    printf ("\n%s\n", BESTSAV_HDR_TXT);
	    if (evo_aktiv) {
	       printf ("\n%s\n", SAV_FKT_NIX_TXT);
	       break;                                              /* Abbruch */
	    }  
	    if (last_psize < 2)
	       if (last_psize == 0)
		  printf ("\n%s\n", AKSAV_NIXDA_TXT);
	       else
		  printf ("\n%s\n", LSV_NIX_AK_SAV);
	    else
            { /* ok */
	       max_anz = last_psize / 2;
	       if (max_anz > 4)
		  ii = max_anz / 4;
	       else
		  ii = 1;
	       sprintf (buffer, BESTSAV_ANZ_TXT, max_anz, ii);
	       if (lies_wert (&anz, buffer, 1, max_anz, ii))
		  break;                                           /* Abbruch */
	       ii = get_best (anz, TRUE, &err_state);
	       (ges_ak_stat_dat.res_ch_sum) += ii;
	       sprintf (buffer, BESTSAV_ERG_TXT, ii);
	       printf ("\n%s\n", buffer);
	       write_text_buf (CLEAR);
	    } /* ok */
         break; /* RetBestErg */

         case 11: /* ---------------------- ShowEvoPar ---------------------- */
	    reset_text_buf ();
	    prep_evo_param_anzg ();
	    write_text_buf (CLEAR);
         break; /* ShowEvoPar */

         case 12: /* ---------------------- Evo Params ---------------------- */
            if (err_state == GLEAM_FATAL)
            { /* Abbruch */
	       printf (FMELD_LINE_TXT, ON_FATAL_NO_FKT);
	       break;
	    } /* Abbruch */
	    printf ("\n%s\n", EVOPAR_HDR_TXT);
	    if (err_state != GLEAM_OK)
            { /* Fehlersituation */
	       printf ("%s\n", NIX_TUN_ERR_TXT);
	       break;                                              /* Abbruch */
	    } /* Fehlersituation */
	    if (evo_aktiv)
	       printf ("\n%s\n", FKT_DISABLED_TXT);
	    else
	       modif_evo_defaults ();
         break; /* Evo Params */

         case 14: /* ----------------------- Loe.Popul. --------------------- */
	    if (err_state == GLEAM_FATAL)
	       printf (FMELD_LINE_TXT, FATAL_WARNUNG);
	    printf ("\n%s\n", EPOPLOE_HDR_TXT);
	    if (err_state != GLEAM_OK)
            { /* Fehlersituation */
	       printf ("%s\n", NIX_TUN_ERR_TXT);
	       break;                                              /* Abbruch */
	    } /* Fehlersituation */
	    if (evo_aktiv)
	       printf ("\n%s\n", EPOPLOE_NIX_TXT);
	    else {
	       err_state |= del_ep ();
	       printf ("\n%s\n", EPOPLOE_OK_TXT);
	    }
         break; /* Loe.Popul. */

         case ESC:
            sub_men_fin = TRUE;
         break;
      } /* case */
      show_and_clear_fbuf(LOG_ALL_MSGS);  /*Ausgabe evtl.Meldungen + Loeschung*/
   } /* Menue-Loop */
   while (!sub_men_fin);
} /* evo_cmenu */



#if defined(MIT_LSV)
/* =============================== lsv_abfrage ============================== */
static INT lsv_abfrage (INT  vorgabe)
/*----------------------------------------------------------------------------*/
/*  Abfrage des LSVs, wobei die "vorgabe" der Defaultwert ist und Rueckgabe   */
/*  des ausgewaehlten Index im "lsv"-Feld. Bei Abbruch oder keinen aktiven    */
/*  LSVs wird KEIN_LSV zurueckgegeben.                                        */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  buffer;
   INT       ii, erg;
   char      ch, ch_in;
   BOOL      eingabe_ok;

   /* lsv_abfrage */
   if (lsv_anz > 0) { /* es gibt aktive LSVs */
      sprintf (buffer, "%s:", OPTSTRAT_LSV);
      for (ii = 0;  ii < lsv_anz;  ii++) { /* Auflistung der aktiven LSVs */
         strcat (buffer, " ");
         strcat (buffer, lsv[aktive_lsv[ii]].lsvMenueName);
         if ((strlen (buffer) > 37) && (ii < lsv_anz-1)) { /* Zeile voll */
            printf ("%s\n", buffer);
            strcpy (buffer,"    ");
         } /* Zeile voll */
      } /* Auflistung der aktiven LSVs */
      if (vorgabe == KEIN_LSV)
         ch ='-';
      else
         ch = lsv[vorgabe].lsvCh;
      sprintf (buf2, "[%c]: ", ch);
      sprintf (buf1, "%*s", (int)(55-strlen(buffer)), buf2);
      strcat (buffer, buf1);
      do
      { /* Eingabe */
         ch_in = char_dialog (buffer);
         if (ch_in == CR)
            ch_in = ch;                                            /* default */
         if (ch_in == ESC)
            return (KEIN_LSV);                                     /* Abbruch */
         eingabe_ok = FALSE;
         for (ii = 0; (ii < lsv_anz) && !eingabe_ok;  ii++) 
            eingabe_ok = (ch_in == lsv[aktive_lsv[ii]].lsvCh);   /* LSV-Suche */
         if (eingabe_ok)
            erg = aktive_lsv[ii-1];
         else
            printf (FMELD_LINE_TXT, BAD_KEY);
      } /* Eingabe */
      while (!eingabe_ok);
   } /* es gibt aktive LSVs */
   else
      erg = KEIN_LSV;
   return (erg);
} /* lsv_abfrage */
#endif /* MIT_LSV */



/* ============================= modif_ak_evojob ============================ */
static BOOL modif_ak_evojob (INT   index, 
                             BOOL  neu_def, 
                             BOOL  aktiv)
/*----------------------------------------------------------------------------*/
/*  Veraendert den durch "index" bezeichneten Optimierungsjob und liefert     */
/*  TRUE ab, wenn die Dateneingabe durchgefuehrt wurde. Bei Abbruch dagegen   */
/*  FALSE.                                                                    */
/*  "neu_def" gibt an, dass der Job neu definiert wird und bei einigen Daten  */
/*  die entsprechenden Defaultwerte zu nehmen sind. "aktiv" gibt an, ob gera- */
/*  de eine Optimierung laeuft. In diesem Fall ist der erste Job nur einge-   */
/*  schraenkt aenderbar. Erst bei vollstaendig durchgefuehrter Dateneingabe   */
/*  werden die neuen Daten in den Job eingetragen.                            */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  chrPtr;  /* for testing the existance of a chromosome */
   GSTATE    ret_state;
   STR_TYPE  buffer;
   DOUBLE    r_wert,
             neu_rank_param,                                /* Neuer Job-Wert */
             neu_lamarck_rate,                              /* Neuer Job-Wert */
             neu_ziel_note,                                 /* Neuer Job-Wert */
             neu_xo_min_ham_dist,                           /* Neuer Job-Wert */
             neu_zeit_in_min;                               /* Neuer Job-Wert */
   LONG      neu_eval_lim,                                  /* Neuer Job-Wert */
             neu_strat_par;                                 /* Neuer Job-Wert */
   INT       neu_opt_strat,                                 /* Neuer Job-Wert */
             neu_p_size,                                    /* Neuer Job-Wert */
             neu_survival,                                  /* Neuer Job-Wert */
             neu_init_strat,                                /* Neuer Job-Wert */
             neu_save_rate,                                 /* Neuer Job-Wert */
             neu_anz_erg_ak,                                /* Neuer Job-Wert */
             neu_generat,                                   /* Neuer Job-Wert */
             neu_gen_no_acc,                                /* Neuer Job-Wert */
             neu_gen_no_deme,                               /* Neuer Job-Wert */
             neu_lsv_strat,                                 /* Neuer Job-Wert */
             rank,
             ii;
   BOOL      neu_z_init,                                    /* Neuer Job-Wert */
             neu_lsv_all_offspr,                            /* Neuer Job-Wert */
             eingabe_ok, evo_job, lsv_job,
             mit_lsv_ausw, 
             lsv_init_best;          /* TRUE, wenn bei Rosen/Compl Init= best */
   char      ch, ch_in;
  
   /* modif_ak_evojob */
   lsv_init_best       = FALSE;
   lsv_job             = FALSE;
   mit_lsv_ausw        = FALSE;
   ret_state           = GLEAM_OK;  /* must be ok, if passed to a function as return parameter */
   neu_init_strat      = evo_ws[index].init_strat;
   neu_strat_par       = evo_ws[index].strat_par;
   neu_survival        = evo_ws[index].survival_rule;
   neu_rank_param      = evo_ws[index].rank_param;
   neu_xo_min_ham_dist = evo_ws[index].xo_min_ham_dist;
   neu_save_rate       = evo_ws[index].save_rate;
   neu_lsv_all_offspr  = evo_ws[index].lsv_all_offspr;
   neu_lsv_strat       = evo_ws[index].lsv_strategy;
   neu_lamarck_rate    = evo_ws[index].lamarck_rate;
   neu_ziel_note       = evo_ws[index].ziel_note;
   neu_zeit_in_min     = evo_ws[index].job_zeit / 60.0;        /* sec --> min */
   neu_eval_lim        = evo_ws[index].maxFEs;
   printf ("\n");

   /* ------------------------ Optimierungsverfahren: ----------------------- */
#if defined(MIT_LSV)
   if (lsv_anz == 0) { /* Ohne aktivierte LSV ist nur GLEAM moeglich */
      neu_opt_strat = GLEAM;
      evo_job       = TRUE;
   } /* Ohne aktivierte LSV ist nur GLEAM moeglich */
   else
   { /* Mit aktivierten LSVs sind alle Jobarten moeglich */
      neu_opt_strat = evo_ws[index].opt_strategy;
      switch (neu_opt_strat) {
         case GLEAM      : ch = GLEAM_CHAR;      break;
         case GLEAM_SMA  : ch = GLEAM_SMA_CHAR;  break;
         case GLEAM_ASMA : ch = GLEAM_ASMA_CHAR; break;
         case GLEAM_AMMA : ch = GLEAM_AMMA_CHAR; break;
         case LSV        : ch = LSV_CHAR;        break;
         default:          ch = '?';
      }
      sprintf (buffer, OPT_VERF_TXT, ch);
      if (aktiv)
         printf ("%s\n", buffer);                              /* aktiver Job */
      else
      { /* nicht aktiver Job: Eingabe */
         strcat (buffer, ": ");
         do
         { /* Eingabe */
            ch_in      = char_dialog (buffer);
            eingabe_ok = TRUE;
            if (ch_in == CR)
               ch_in = ch;                                         /* default */
            switch (ch_in) { /* Zeichenauswertung */
               case GLEAM_CHAR: 
                  neu_opt_strat = GLEAM;
               break;
               case GLEAM_SMA_CHAR:
                  neu_opt_strat = GLEAM_SMA;
                  mit_lsv_ausw  = TRUE;
               break;
               case GLEAM_ASMA_CHAR:
                  neu_opt_strat = GLEAM_ASMA;
                  mit_lsv_ausw  = TRUE;
               break;
               case GLEAM_AMMA_CHAR:
                  neu_opt_strat = GLEAM_AMMA;
               break;
               case LSV_CHAR:
                  neu_opt_strat = LSV;
                  lsv_job       = TRUE;
                  mit_lsv_ausw  = TRUE;
               break;
               case ESC:
                  return (FALSE);                                  /* Abbruch */
               default:
                  eingabe_ok = FALSE;
                  printf (FMELD_LINE_TXT, BAD_KEY);
            } /* Zeichenauswertung */
         } /* Eingabe */
         while (!eingabe_ok);
      } /* nicht aktiver Job: Eingabe */   
      evo_job = neu_opt_strat <= GLEAM_AMMA;      /* GLEAM, SMA, ASMA, AMMA */

      if (mit_lsv_ausw)
      { /* Bestimmung des LSVs fuer SMA, ASMA und LSV */
         neu_lsv_strat = lsv_abfrage (neu_lsv_strat);
         if (neu_lsv_strat == KEIN_LSV)
            return (FALSE);                                        /* Abbruch */
      } /* Bestimmung des LSVs fuer SMA, ASMA und LSV */
   } /* Mit aktivierten LSVs sind alle Jobarten moeglich */
#else /* kein MIT_LSV: nur GLEAM */
   neu_opt_strat = GLEAM;
   evo_job       = TRUE;
#endif /* kein MIT_LSV: nur GLEAM */

   if (evo_job)
   { /* (GLEAM, SMA, ASMA, AMMA)-Job */
      /* ------------------------ Populationsgroesse: ----------------------- */
      neu_p_size = evo_ws[index].popel_size;
      sprintf (buffer, PSIZE_QUERY_TXT, anz_nachbarn, MAX_POPEL_SIZE, neu_p_size);
      if (aktiv)
         printf ("%s\n", buffer);                           /* aktiver Job */
      else
      { /* nicht aktiver Job: Eingabe */ 
         strcat (buffer, ": ");
         if (lies_wert (&neu_p_size, buffer, 
                        anz_nachbarn, MAX_POPEL_SIZE, evo_ws[index].popel_size))
            return (FALSE);                                        /* Abbruch */
      } /* nicht aktiver Job: Eingabe */ 
   } /* (GLEAM, SMA, ASMA, AMMA)-Job */
   else
   { /* nichts mit Evo */
      neu_generat     = 0;
      neu_gen_no_acc  = 0;
      neu_gen_no_deme = 0;
      neu_zeit_in_min = 1440.0;                                 /* 24 Stunden */
      neu_p_size      = 1;
   } /* nichts mit Evo */

   /* -------------- Initialisierungsstrategie samt Parameter: -------------- */
   switch (evo_ws[index].init_strat)
   {
      case NEU:       ch = NEU_INIT_CHAR;   break;
      case BEST:      ch = BEST_INIT_CHAR;  break;
      case MIX:       ch = MIX_INIT_CHAR;   break;
      case BEST_NEU:  ch = BN_INIT_CHAR;    break;
      case FROM_FILE: ch = FILE_INIT_CHAR;  break;
      case LSV_NEU:   ch = LSV_N_INIT_CHAR; break;
      case LSV_BEST:  ch = LSV_B_INIT_CHAR; break;
      case ONE_CHR:   ch = CHR_INIT_CHAR;   break;
   } /* CASE */
   
   if (evo_job)
      sprintf (buffer, INIT_STRAT_TXT, ch);         /* GLEAM, SMA, ASMA, AMMA */
   else
      sprintf (buffer, INIT_LSV_TXT, ch);        /* Allgemeines Init fuer LSV */
   if (aktiv)
      printf ("%s\n", buffer);                                 /* aktiver Job */
   else
   { /* nicht aktiver Job: Eingabe */
      strcat (buffer, ": ");
      do
      {
         eingabe_ok = TRUE;
         ch_in      = char_dialog (buffer);
         if (ch_in == CR)
            ch_in = ch;                                            /* default */
         switch (ch_in)
         { /* Zeichenauswertung */
            case NEU_INIT_CHAR: /* neu */
	       neu_init_strat = NEU;
#ifndef EXT_SIMU_SERV 
	       if (neu_strat_par > neu_p_size)
#endif /* no EXT_SIMU_SERV */
		  neu_strat_par = 0;
	       sprintf (buf1, AK_NGO_TXT, neu_strat_par);
#ifdef EXT_SIMU_SERV
	       printf("%s 0 %s\n", buf1, NO_ESS_PARAM_TXT); 
#else /* no EXT_SIMU_SERV */
	       if (lies_lwert (&neu_strat_par, buf1, 0, neu_p_size, neu_strat_par))
		  return (FALSE);                                  /* Abbruch */
#endif /* no EXT_SIMU_SERV */
            break;  /* neu */

            case BEST_INIT_CHAR:
	       neu_init_strat = BEST;
	       lsv_init_best  = lsv_job;
            break;

            case BN_INIT_CHAR: /* best_neu */
	       neu_init_strat = BEST_NEU;
	       if (neu_strat_par > neu_p_size)
		  neu_strat_par = neu_p_size;
	       sprintf (buf1, AK_BEST_ANZ_TXT, neu_strat_par);
	       if (lies_lwert (&neu_strat_par, buf1, 0, neu_p_size, neu_strat_par))
		  return (FALSE);                                  /* Abbruch */
            break;  /* best_neu */

            case MIX_INIT_CHAR: /* mix */
	       neu_init_strat = MIX;
	       if (neu_strat_par > INOTEN_MAX)
		  neu_strat_par = INOTEN_MAX;
	       sprintf (buf1, AK_MIND_N_TXT, neu_strat_par);
	       if (lies_lwert (&neu_strat_par, buf1, 0, INOTEN_MAX, neu_strat_par))
		  return (FALSE);                                  /* Abbruch */
            break;  /* mix */

#ifdef MIT_LSV
            case LSV_N_INIT_CHAR: /* LSV_NEU */
            case LSV_B_INIT_CHAR: /* LSV_BEST */
	       if ((neu_strat_par > neu_p_size) || (neu_strat_par == 0)) 
		  neu_strat_par = l_round(neu_p_size * 0.2);
	       if (ch_in == LSV_N_INIT_CHAR)
		  { /* LSV_NEU */
		     neu_init_strat = LSV_NEU;
		     sprintf (buf1, ANZ_LSV_N_TXT, neu_strat_par);
		  } /* LSV_NEU */
	       else
		  { /* LSV_BEST */
		     neu_init_strat = LSV_BEST;
		     sprintf (buf1, AK_BEST_ANZ_TXT, neu_strat_par);
		  } /* LSV_BEST */
	       if (lies_lwert (&neu_strat_par, buf1, 0, neu_p_size, neu_strat_par))
		  return (FALSE);                                  /* Abbruch */
            break;  /* neu, best */

            case CHR_INIT_CHAR: /* chromosome */
	       neu_init_strat = ONE_CHR;
	       if (lies_adr(&rank, &ii, AK_QUERY_TXT))
	       { /* chromosome address with valid rank part */
		  if (get_chain(&chrPtr, &ii, AK, rank, &rank, &ret_state))
		  { /* this chromosome address is valid */
		     neu_strat_par = rank;
		     evo_ws[index].chrNumber = ii;       /* only for lhc jobs */
		  } /* this chromosome address is valid */
		  else
		  { /* invalid chromosome address */
		     printf("%s\n", NIX_CH);
		     return (FALSE);                        /* abort function */
		  } /* invalid chromosome address */
	       } /* chromosome address with valid rank part */

            break;  /* chromosome */
#endif /* MIT_LSV */

            case FILE_INIT_CHAR:
               neu_init_strat = FROM_FILE;
            break;

            case ESC:
            return (FALSE);                                        /* Abbruch */

            default:
               eingabe_ok = FALSE;
               printf (FMELD_LINE_TXT, BAD_KEY);
         } /* CASE */
      } /* Zeichenauswertung */
      while (!eingabe_ok);
   } /* nicht aktiver Job: Eingabe */

   /* ------------ Initvorgabe fuer den Zufallszahlen-Generator: ------------ */
   if (lsv_init_best)                                  /* LSV mit Best-Init ? */
#ifdef MIT_ROCO
      neu_z_init = (neu_init_strat == BEST) || (neu_lsv_strat == LSV_COMPL);
#else
      neu_z_init = (neu_init_strat == BEST);
#endif /* MIT_ROCO */
   else
   { /* alles andere */
      strcpy (buf1, ZGEN_INIT_Q);
      if (evo_ws[index].z_init)
         strcat (buf1, JA_NEIN_TXT);
      else
         strcat (buf1, NEIN_JA_TXT);
      if (aktiv)
      { /* aktiver Job */
         strcpy (buf2, buf1);       
         buf2[strlen(buf1) - 2] = '\0';               /* Abschneiden von ": " */
         printf ("%s\n", buf2); 
         neu_z_init = evo_ws[index].z_init;                      /* neu = alt */
      } /* aktiver Job */
      else
      { /* nicht aktiver Job */
         ch = char_dialog (buf1);
         if (ch == ESC)
            return (FALSE);                                        /* Abbruch */
	 neu_z_init = (ch == JA) || ((ch == CR) && evo_ws[index].z_init);
      } /* nicht aktiver Job */
   } /* alles andere */

   if (evo_job)
   { /* GLEAM, SMA, ASMA, AMMA */
      /* ------------------------- SurvivalRule: ---------------------------- */
      switch (neu_survival)
      {
         case LOCAL_LEAST   : ch = '1'; break;
         case LOCAL_LEAST_ES: ch = '3'; break;
         case BETTER_PARENT : ch = '5'; break;
         case ALWAYS        : ch = '2'; break;
         case ALWAYS_ES     : ch = '4'; break;
      }
      sprintf (buffer, SURVIVAL_TXT, ch);
      if (aktiv)
         printf ("%s\n", buffer);                              /* aktiver Job */
      else
      { /* nicht aktiver Job: Eingabe */
         strcat (buffer, ": ");
	 do
         { /* Frage-Loop */
	    eingabe_ok = TRUE;
	    ch_in      = char_dialog (buffer);
	    if (ch_in == ESC)
	       return (FALSE);                                        /* Abbruch */
	    if (ch_in == CR)
	       ch_in = ch;
	    switch (ch_in) {
               case '1': neu_survival = LOCAL_LEAST;    break;
               case '3': neu_survival = LOCAL_LEAST_ES; break;
               case '5': neu_survival = BETTER_PARENT;  break;
               case '2': neu_survival = ALWAYS;         break;
               case '4': neu_survival = ALWAYS_ES;      break;
               default:
                  eingabe_ok = FALSE;
                  printf (FMELD_LINE_TXT, BAD_KEY);
	    } /* switch */
	 } /* Frage-Loop */
	 while (!eingabe_ok);
      } /* nicht aktiver Job: Eingabe */

      /* ---------------------- Ranking-Parameter: -------------------------- */
      sprintf (buffer, RANK_PAR_TXT, neu_rank_param);
      if (aktiv)
         printf ("%s\n", buffer);                              /* aktiver Job */
      else
      { /* nicht aktiver Job: Eingabe */
         strcat (buffer, ": ");
         if (lies_rwert(&neu_rank_param, buffer, 1.0, 2.0, neu_rank_param))
            return (FALSE);                                        /* Abbruch */
      } /* nicht aktiver Job: Eingabe */

      /* ------------------- XO-Mindesthammingabstand: ---------------------- */
      r_wert = neu_xo_min_ham_dist * 100.0;              /* Faktor -> Prozent */
      sprintf (buffer, XO_HD_TXT, r_wert);
      if (aktiv) 
         printf ("%s\n", buffer);                              /* aktiver Job */
      else { /* nicht aktiver Job: Eingabe */
         strcat (buffer, ": ");
         if (lies_rwert(&r_wert, buffer, 0.0, 100.0, r_wert))
            return (FALSE);                                        /* Abbruch */
         neu_xo_min_ham_dist = r_wert / 100.0;           /* Prozent -> Faktor */
      } /* nicht aktiver Job: Eingabe */

      /* ---------------------- Vorgabe der Save-Rate: ---------------------- */
      sprintf (buf1, EVO_SAVE_TXT, neu_save_rate);
      if (lies_wert (&neu_save_rate, buf1, 1, INT_MAX, neu_save_rate))
         return (FALSE);                                           /* Abbruch */

#if defined(MIT_LSV)
      if (neu_opt_strat != GLEAM)
      { /* ------------ Vorgabe von best/all bei SMA, ASMA, AMMA: ----------- */
         strcpy (buf1, LSV_ALL_OFFSPRQ);
         if (neu_lsv_all_offspr)
            strcat (buf1, JA_NEIN_TXT);
         else
            strcat (buf1, NEIN_JA_TXT);
	 if (aktiv)
         { /* aktiver Job */
	    buf1[strlen(buf1) - 2] = '\0';            /* Abschneiden von ": " */
	    printf ("%s\n", buf1);
	 } /* aktiver Job */
	 else
         { /* nicht aktiver Job: Eingabe */
	    ch = char_dialog (buf1);
	    if (ch == ESC)
	       return (FALSE);                                     /* Abbruch */
	    neu_lsv_all_offspr = (ch == JA) || ((ch == CR) && neu_lsv_all_offspr);
	 } /* nicht aktiver Job: Eingabe */
      } /* Vorgabe von best/all bei SMA, ASMA, AMMA */
      else /* GLEAM oder LSV */
	 neu_lsv_all_offspr = FALSE;
#else /* kein MIT_LSV: reines GLEAM */
      neu_lsv_all_offspr = FALSE;
#endif /* kein MIT_LSV */

#if defined(MIT_LSV)
      if (((neu_opt_strat == GLEAM)    || (neu_opt_strat == GLEAM_AMMA)) &&
          ((neu_init_strat == LSV_NEU) || (neu_init_strat == LSV_BEST))  )
      { /* ------------------ Auswahl des LSVs zur PopInit: ----------------- */
	 if (aktiv) /* aktiver Job? */
	    printf (POP_INIT_LSV_TXT, lsv[neu_lsv_strat].lsvMenueName);
	 else
         { /* nicht aktiver Job: Eingabe */
	    /* fuer SMA, ASMA und LSV ist LSV bereits ausgewaehlt */
	    neu_lsv_strat = lsv_abfrage (neu_lsv_strat);
	    if (neu_lsv_strat == KEIN_LSV) {
	       meldung(EVO_CMEN, NO_INFO, NO_INFO, "", REQUIRED_LHCS_NOT_AVAIL);
	       return (FALSE);                                        /* Abbruch */
	    }
	 } /* nicht aktiver Job: Eingabe */
      } /* Auswahl des LSVs zur PopInit */
#endif /* MIT_LSV */
   } /* GLEAM, SMA, ASMA, AMMA */

   if (evo_job && (neu_opt_strat != GLEAM))           /* SMA, ASMA oder AMMA? */
   { /* --------------------------- Lamarck-Rate: --------------------------- */
      r_wert = neu_lamarck_rate * 100.0;                 /* Faktor -> Prozent */
      sprintf (buffer, LAMARCK_PAR_TXT, r_wert);
#ifdef MIT_GORBA_LSVS
      printf ("%s\n", buffer);
#else /* kein MIT_GORBA_LSVS */
      if (aktiv)
         printf ("%s\n", buffer);                              /* aktiver Job */
      else { /* nicht aktiver Job: Eingabe */
         strcat (buffer, ": ");
         if (lies_rwert(&r_wert, buffer, 0.0, 100.0, r_wert))
            return (FALSE);                                        /* Abbruch */
         neu_lamarck_rate = r_wert / 100.0;              /* Prozent -> Faktor */
      } /* nicht aktiver Job: Eingabe */
#endif /* kein MIT_GORBA_LSVS */
   } /* Lamarck-Rate */

   /* ----------------- Vorgabe der Ergebnisketten-Anzahl: ------------------ */
   if (evo_job)                                /* GLEAM, SMA, ASMA oder AMMA? */
      neu_anz_erg_ak = neu_p_size/2;
   else
      neu_anz_erg_ak = 1;
   if (!lsv_init_best)
   { /* Ergebnis-AK-Anzahl festlegen */
      sprintf (buf1, ERG_AK_QUERY_TXT, neu_anz_erg_ak, evo_ws[index].anz_erg_ketten);
      if (lies_wert (&neu_anz_erg_ak, buf1, 
                     0, neu_anz_erg_ak, evo_ws[index].anz_erg_ketten))
     return (FALSE);                                               /* Abbruch */
   } /* Ergebnis-AK-Anzahl festlegen */

   /* -------------- Vorgaben zur Beendigung einer Population: -------------- */
   printf ("\n%s\n", OPT_ENDKRIT_TXT);             /* es gibt Abbruchvorgaben */

   /* ------------------------------ Ziel-Note: ----------------------------- */
   sprintf (buf1, ZNOTE_TXT, l_round(neu_ziel_note));
   if (lies_rwert (&neu_ziel_note, buf1, 0.0, RNOTEN_MAX,neu_ziel_note))
      return (FALSE);                                              /* Abbruch */
    
   if (evo_job)
   { /* GLEAM, SMA, ASMA, AMMA */
      /* --------------------- Anzahl der Generationen: --------------------- */
      sprintf (buf1, MAX_GEN_TXT, evo_ws[index].generationen);
      if (lies_wert (&neu_generat, buf1, 1, INT_MAX, evo_ws[index].generationen))
         return (FALSE);                                           /* Abbruch */
      if (evo_ws[index].genno_deme_impr > neu_generat)
         ii = neu_generat;
      else
         ii = evo_ws[index].genno_deme_impr;
      sprintf (buf1, MAX_NO_DEME_TXT, ii);
      if (lies_wert (&neu_gen_no_deme, buf1, 1, INT_MAX, ii))
         return (FALSE);                                           /* Abbruch */
      if (evo_ws[index].gen_no_accept > neu_generat)
         ii = neu_generat;
      else
         ii = evo_ws[index].gen_no_accept;
      sprintf (buf1, MAX_NO_ACC_TXT, ii);
      if (lies_wert (&neu_gen_no_acc, buf1, 1, INT_MAX, ii))
         return (FALSE);                                           /* Abbruch */

      /* ------------------ Evaluationsgrenze des Jobs: --------------------- */
      sprintf (buf1, EVAL_MAX_TXT, evo_ws[index].maxFEs);
      if (lies_lwert(&neu_eval_lim, buf1, 0, MAX_LONG_INT, neu_eval_lim))
         return (FALSE);                                           /* Abbruch */

      /* ------------------- Zeit des Optimierungsjobs: --------------------- */
      display_time (buf1, evo_ws[index].job_zeit, ADJUST);
      sprintf (buffer, JOB_ZEIT_TXT, buf1, neu_zeit_in_min);
      if (lies_rwert(&neu_zeit_in_min, buffer, 0.016667, MAX_JOB_ZEIT/60.0, neu_zeit_in_min))
         return (FALSE);                                           /* Abbruch */
   } /* GLEAM, SMA, ASMA, AMMA */
   
   /* ------------------- Kein Abbruch: Daten uebernehmen: ------------------ */
   evo_ws[index].opt_strategy   = neu_opt_strat;
   evo_ws[index].popel_size     = neu_p_size;
   evo_ws[index].survival_rule  = neu_survival;
   evo_ws[index].rank_param     = neu_rank_param;
   evo_ws[index].xo_min_ham_dist= neu_xo_min_ham_dist;
   evo_ws[index].init_strat     = neu_init_strat;
   evo_ws[index].strat_par      = neu_strat_par;
   evo_ws[index].save_rate      = neu_save_rate;
   evo_ws[index].anz_erg_ketten = neu_anz_erg_ak;
   evo_ws[index].z_init         = neu_z_init;
   evo_ws[index].ziel_note      = neu_ziel_note;
   evo_ws[index].generationen   = neu_generat;
   evo_ws[index].gen_no_accept  = neu_gen_no_acc;
   evo_ws[index].genno_deme_impr= neu_gen_no_deme;
   evo_ws[index].job_zeit       = l_round(neu_zeit_in_min * 60.0);
   evo_ws[index].maxFEs         = neu_eval_lim;
   evo_ws[index].lamarck_rate   = neu_lamarck_rate;
   evo_ws[index].lsv_strategy   = neu_lsv_strat;
   evo_ws[index].lsv_all_offspr = neu_lsv_all_offspr;

   return (TRUE);       /* Bei Benutzerabbruch wurde zuvor mit FALSE beendet! */
} /* modif_ak_evojob */ 



/* ============================== finishActJob ============================== */
static void finishActJob (void)
/*------------------------------------------------------------------------------
   The time budget of the actual optimization job is set to zero so that the job 
   is terminated after the end of the current generation. The processing of the 
   job list is restarted by a call of do_evo_jobs().
------------------------------------------------------------------------------*/
{
   evo_ws[first_evo_item].job_zeit = 0;
   sprintf(logBuffer, JOBLOE_FIN_TXT, first_evo_item + 1);
   write_to_log(logBuffer, CLOSE_LOG);
   do_evo_jobs();
} /* finishActJob */



/* =============================== abortJobList ============================= */
static void abortJobList (void)
/*------------------------------------------------------------------------------
   Resets the actual job interruption and terminates the actual job by a call of 
   clearEvoJobIntrpt(). Afterwards the job list is cleared and reset.
------------------------------------------------------------------------------*/
{
   sprintf(logBuffer, JOBLOE_ABORT_TXT, first_evo_item + 1);
   write_to_log(logBuffer, CLOSE_LOG);
   clearEvoJobIntrpt();
   resetJobList();
   write_text_buf(CLEAR);             /* output of results of job termination */
   printf("\n");
} /* abortJobList */



/* ============================= delWaitingJobs ============================= */
static void delWaitingJobs (void)
/*------------------------------------------------------------------------------
   All waiting jobs are deactivated.
------------------------------------------------------------------------------*/
{
   INT  ii;

   /* delWaitingJobs */
   for (ii = first_evo_item + 1;  ii < free_evo_index;  ii++)
      evo_ws[ii].deaktiviert = TRUE;
} /* delWaitingJobs */



/* ============================== resetJobList ============================== */
static void resetJobList (void)
/*------------------------------------------------------------------------------
   The optimization job list is cleared and reset.
------------------------------------------------------------------------------*/
{
   first_evo_item = 0;
   free_evo_index = 0;
} /* resetJobList */




/* =========================== modif_evo_defaults =========================== */
static void modif_evo_defaults (void)
/*----------------------------------------------------------------------------*/
/*   Erledigt das Submenue fuer                                               */
/*     - die Vorgabe von Defaultwerten fuer die Definition von Optimierungs-  */
/*       jobs                                                                 */
/*     - die Festlegung von Parametern der Evolution, der Adaption und/oder   */
/*       der LSVs.                                                            */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  buffer, buf1;            /* Hilfspuffer                          */
   DOUBLE    r_wert, rdef, ug, og;    /* Real-Puffer                          */
   INT       men_item,                /* Ausgewaehltes Menue-Item             */
             sub_men_item,            /* Ausgewaehltes Sub-Menue-Item         */
             i_wert,                  /* Integer-Puffer                       */
             ii, jj;
   char      ch; 
   BOOL      evo_menue_ende,          /* Menue-Steuerung                      */
             ok;

   /* modif_evo_defaults */
   evo_menue_ende = FALSE;
   do 
   { /* sub menue loop */
      set_and_clear_menue_window (3, EVO_PARAM_MEN);
#ifdef MIT_LSV
      if (lsv_anz > 0)
         write_menue_item ( 1, StndMenItem, EVOPARAM_OPT);
#else
         write_menue_item ( 1, DarkMenItem, EVOPARAM_OPT);
#endif
      if (def_opt_strat <= GLEAM_AMMA)
      { /* GLEAM, SMA, ASMA oder AMMA */
         write_menue_item ( 3, StndMenItem, EVOPARAM_DEME);
         write_menue_item ( 4, StndMenItem, EVOPARAM_SURV);
         write_menue_item ( 5, StndMenItem, EVOPARAM_RANK);
         write_menue_item ( 6, StndMenItem, EVOPARAM_HAM);
         if (mit_gen_rep_par)
            write_menue_item ( 7, StndMenItem, EVOPARAM_GENREPPAR);
         else
            write_menue_item ( 7, DarkMenItem, EVOPARAM_GENREPPAR);
         if (mitOptionPhaenoRep)
            write_menue_item ( 8, StndMenItem, EVOPARAM_PHEREPPAR);
         else
            write_menue_item ( 8, DarkMenItem, EVOPARAM_PHEREPPAR);
      } /* GLEAM, SMA, ASMA oder AMMA */
#ifdef MIT_LSV
      if (lsv_anz > 0)
      { /* present gene model includes LHCs */
	 if (((def_opt_strat == LSV) || (def_opt_strat == GLEAM_SMA) ||
	      (def_opt_strat == GLEAM)) && (def_lsv_strat != KEIN_LSV))
         { /* LSV, GLEAM (wg.VorInit) oder SMA UND aktive LSVs */
	    write_menue_item (10, StndMenItem, EVOPARAM_LSV_AUSW);
	    write_menue_item (11, StndMenItem, EVOPARAM_LSV_PARAM);
	 } /* LSV, GLEAM (wg.VorInit) oder SMA UND aktive LSVs */
	 if (def_opt_strat != GLEAM)
         { /* LSV, SMA, ASMA oder AMMA */
  #ifdef MIT_GORBA_LSVS
	    write_menue_item ( 9, DarkMenItem, EVOPARAM_LAMARCK);
  #else /* kein MIT_GORBA_LSVS */
	    write_menue_item ( 9, StndMenItem, EVOPARAM_LAMARCK);
  #endif /* kein MIT_GORBA_LSVS */
	    if ((def_opt_strat == GLEAM_ASMA) || (def_opt_strat == GLEAM_AMMA))
            { /* GLEAM_ASMA oder GLEAM_AMMA */
	       if (def_opt_strat == GLEAM_AMMA)
		  write_menue_item (10, StndMenItem, EVOPARAM_LSV_VERT);
	       else
		  write_menue_item (10, StndMenItem, EVOPARAM_LSV_AUSW);
	       write_menue_item (11, StndMenItem, EVOPARAM_ADA_SPEED);
	       write_menue_item (12, StndMenItem, EVOPARAM_N_KLASSE);
	       if (def_opt_strat == GLEAM_AMMA) {
		  write_menue_item (13, StndMenItem, EVOPARAM_ADAPT_ALT);
		  write_menue_item (14, StndMenItem, EVOPARAM_NIX_LSV);
	       }
	    } /* GLEAM_ASMA oder GLEAM_AMMA */
	 } /* LSV, SMA, ASMA oder AMMA */
      } /* present gene model includes LHCs */
#endif /* MIT_LSV */

      /* ------------------- Anzeige der aktuellen Werte: ------------------- */
      printf ("\n");                                /* erzeugt eine Leerzeile */
      reset_text_buf ();
      prep_evo_param_anzg ();
      write_text_buf (CLEAR);

      /* ---------------------------- Sub-Menue: ---------------------------- */
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item) 
      {
#if defined(MIT_LSV)
         case 1: /* ---------------- Optimierungs-strategie ----------------- */
	    printf ("\n%s\n", OPTSTRAT_HDR);
	    strcpy (buf1, OPTSTRAT_ANZG);
	    switch (def_opt_strat)
            {
               case GLEAM:      strcpy (buf2, OPTSTRAT_EVO);  break;
               case GLEAM_SMA:  strcpy (buf2, OPTSTRAT_SMA);  break;
               case GLEAM_ASMA: strcpy (buf2, OPTSTRAT_ASMA); break;
               case GLEAM_AMMA: strcpy (buf2, OPTSTRAT_AMMA); break;
               case LSV:        strcpy (buf2, OPTSTRAT_LSV);  break;
            }
	    strcat (buf1, buf2);
	    printf ("\n%s\n", buf1);
	    set_and_clear_menue_window (4, EVOPARAM_OPT);
	    write_menue_item (1, StndMenItem, OPTSTRAT_EVO);
	    write_menue_item (2, StndMenItem, OPTSTRAT_SMA);
	    write_menue_item (3, StndMenItem, OPTSTRAT_ASMA);
	    write_menue_item (4, StndMenItem, OPTSTRAT_AMMA);
	    write_menue_item (5, StndMenItem, OPTSTRAT_LSV);
	    menue_completion ();
	    sub_men_item = select_menue_item ();
	    switch (sub_men_item) 
	    {
	       case 1: def_opt_strat = GLEAM;      break;
	       case 2: def_opt_strat = GLEAM_SMA;  break;
	       case 3: def_opt_strat = GLEAM_ASMA; break;
	       case 4: def_opt_strat = GLEAM_AMMA; break;
	       case 5: def_opt_strat = LSV;        break;
	       case ESC:                           break;
	    } /* case */
         break; /* Optimierungs-strategie */
#endif /* MIT_LSV */

         case 3: /* ----------------- Nachbarschaftsanzahl ------------------ */
	    sprintf (buf1, EVOPAR_DSIZFRAG, MIN_ANZ_NACHBARN, MAX_ANZ_NACHBARN, 
		     anz_nachbarn);
	    do 
            { /* Einlesen bis gerader Wert */
               if (lies_wert (&i_wert, buf1, MIN_ANZ_NACHBARN, MAX_ANZ_NACHBARN, 
			      anz_nachbarn))
               { /* Abbruch */
		  i_wert = 0;
		  break;
	       } /* Abbruch */
	       if ((i_wert % 2) == 1)
		  printf (FMELD_LINE_TXT, EVOPAR_DSIZ_F1);
	    } /* Einlesen bis gerader Wert */
	    while ((i_wert % 2) == 1);
	    if (i_wert > 0)
	       anz_nachbarn = i_wert;
         break; /* Nachbarschaftsanzahl */
 
         case 4: /* --------------------- Survival-Rule --------------------- */
	    printf ("\n%s\n", EVOPAR_SURV_HDR);
	    strcpy (buf1, EVOPAR_SURVANZG);
	    switch (default_survival_rule) 
            {
               case LOCAL_LEAST_ES: strcat (buf1, EVOPAR_LL_ES);   break; 
               case LOCAL_LEAST   : strcat (buf1, EVOPAR_LL);      break;
               case ALWAYS        : strcat (buf1, EVOPAR_AA);      break;
               case ALWAYS_ES     : strcat (buf1, EVOPAR_AA_ES);   break;
               case BETTER_PARENT : strcat (buf1, EVOPAR_BP);      break;
	    }
	    printf ("\n%s\n", buf1);
	    set_and_clear_menue_window (4, EVOPARAM_SURV);
	    write_menue_item (1, StndMenItem, EVOPAR_SURV_BP);
	    write_menue_item (2, StndMenItem, EVOPAR_SURVLLES);
	    write_menue_item (3, StndMenItem, EVOPAR_SURVAAES);
	    write_menue_item (6, StndMenItem, EVOPAR_SURV_LL);
	    write_menue_item (7, StndMenItem, EVOPAR_SURV_AA);
	    menue_completion ();
	    sub_men_item = select_menue_item ();
	    switch (sub_men_item) 
            {
               case 1: default_survival_rule = BETTER_PARENT;  break;
               case 2: default_survival_rule = LOCAL_LEAST_ES; break;
               case 3: default_survival_rule = ALWAYS_ES;      break;
               case 6: default_survival_rule = LOCAL_LEAST;    break;
               case 7: default_survival_rule = ALWAYS;         break;
               case ESC:                                       break;
            } /* case */
         break; /* Survival-Rule */
 
         case 5: /* --------------------- Rank-Parameter -------------------- */
            sprintf (buf1, EVOPAR_RANKFRAG, def_max_fit);
            if (lies_rwert (&r_wert, buf1, 1.0, 2.0, def_max_fit))
               break;                                              /* Abbruch */
            def_max_fit = r_wert;
         break; /* Rank-Parameter */
   
         case 6:  /* ---------------- XO/Reko-Hamming-Dist. ----------------- */
	    r_wert = def_xo_min_ham_dist * 100;
	    sprintf (buf1, EVOPAR_XO_HAM_Q, r_wert);
	    if (lies_rwert (&r_wert, buf1, 0.0, 100.0, r_wert))
	       break;                                              /* Abbruch */
	    def_xo_min_ham_dist = r_wert / 100.0;
         break; /* XO/Reko-Hamming-Dist. */

         case 7: /* ------------ Genetic-Repair-Steuerparameter ------------- */
	    if (mitPhaenoRepair) 
	       printf (MSG_LINE_TXT, EVOPAR_GENO_PHENO);
	    sprintf (buf1, gen_rep_par_query, gen_rep_par_ug * 100.0, gen_rep_par_og * 100.0, 
		     gen_rep_par * 100.0);
	    if (lies_rwert (&r_wert, buf1, gen_rep_par_ug * 100.0, gen_rep_par_og * 100.0, 
			    gen_rep_par * 100.0))
	       break;                                              /* Abbruch */
	    gen_rep_par = r_wert / 100.0;
         break; /* Genetic-Repair-Steuerparameter */

         case 8: /* -------- Phaenotypisches-Repair-Steuerparameter --------- */
            if (gen_rep_par > 0.0)
	       printf (MSG_LINE_TXT, EVOPAR_GENO_PHENO);
	    sprintf (buf1, "%s? ", EVOPAR_PHAENO_Q);
	    if (mitPhaenoRepair)
	       strcat (buf1, JA_NEIN_TXT);
	    else
	       strcat (buf1, NEIN_JA_TXT);
	    ch = char_dialog (buf1);
	    if (ch == ESC)
	       break;                                              /* Abbruch */
	    mitPhaenoRepair = (ch == JA) || ((ch == CR) && mitPhaenoRepair);
         break; /* Phaenotypisches-Repair-Steuerparameter */
   
#ifdef MIT_LSV
  #ifndef MIT_GORBA_LSVS
         case 9:  /* ---------------- LSV: Lamarck-Parameter ---------------- */
	    r_wert = def_lamarck_rate * 100;
	    sprintf (buf1, EVOPAR_LAMA_Q, r_wert);
	    if (lies_rwert (&r_wert, buf1, 0.0, 100.0, r_wert))
	       break;                                              /* Abbruch */
	    def_lamarck_rate = r_wert / 100.0;
         break; /* LSV: Lamarck-Parameter */
  #endif /* kein MIT_GORBA_LSVS */

         case 10: /* ---------- LSV-Auswahl / LSV-Wahrscheinlichk. ---------- */
	    if (def_opt_strat == GLEAM_AMMA)
	    { /* AMMA: LSV-Wahrscheinlichkeiten */
	       printf ("\n%s\n", EVOPAR_AMMA_LSV_P);
	       ok  = TRUE;
	       og  = 1.0;
	       for (ii = 0;  (ii < lsv_anz)  && ok;  ii++)
               { /* Wahrscheinlichkeiten aller aktiven LSVs */
		  jj = aktive_lsv[ii];
		  if (og < lsv[jj].lsvStartP)
		     rdef = og;
		  else
		     rdef = lsv[jj].lsvStartP;
		  sprintf (buf1, "  %-30s (0..%6g) [%6g]: ", lsv[jj].lsvName, og, rdef);
		  ok = !lies_rwert (&(lsv[jj].neuLsvStartP), buf1, 0.0, og, rdef);
		  og -= lsv[jj].neuLsvStartP;
	       } /* Wahrscheinlichkeiten aller aktiven LSVs */
	       if (og == 0.0)
		  if (ok) 
                  { /* Kein Abbruch: Daten übernehmen */
		     for (ii = 0;  (ii < lsv_anz)  && ok;  ii++)
			lsv[aktive_lsv[ii]].lsvStartP = lsv[aktive_lsv[ii]].neuLsvStartP;
		     init_adi (aktLSVAdaptSpeed,           /* beteiligte LSVs */
			       aktLevelAdaptSpeed);        /* ermitteln ...   */
		     printf ("%s\n", EVOPAR_AMMA_NEU_P);
		  } /* Kein Abbruch: Daten übernehmen */
		  else 
		     printf ("%s\n", EVOPAR_AMMA_P_ERR1);
	       else {
		  sprintf (buf1, EVOPAR_AMMA_P_ERR2, 1.0 - og);
		  printf (FMELD_LINE_TXT, buf1);
	       }
	    } /* AMMA: LSV-Wahrscheinlichkeiten */
	    else
            { /* LSV, GLEAM (wg.VorInit), SMA, ASMA: LSV-Auswahl */
	       ii = lsv_abfrage (def_lsv_strat);
	       if (ii == KEIN_LSV)
		  break;                                           /* Abbruch */
	       def_lsv_strat = ii;
	    } /* LSV, GLEAM (wg.VorInit), SMA, ASMA: LSV-Auswahl */
         break; /* LSV, SMA, ASMA: LSV-Ausw */

         case 11: /* -------- LSV-Params / Adaptionsgeschwindigkeit --------- */
         if ((def_opt_strat == LSV) || (def_opt_strat == GLEAM_SMA) || 
             (def_opt_strat == GLEAM))
         { /* LSV, SMA, GLEAM (wg.VorInit): LSV-Params */
            if (lsv[def_lsv_strat].lsvParAnz > 0)
            { /* das Default-LSV hat Steuerparameter */
               sprintf (buffer, EVOPAR_AMMA_SPEED, lsv[def_lsv_strat].lsvName);
               printf ("\n%s\n", buffer);
               ok = TRUE;
               for (ii = 0;  (ii < lsv[def_lsv_strat].lsvParAnz) && ok;  ii++) 
               { /* Steuer-Parameter */
                  sprintf (buf1, "  %-37s [%7g]: ", lsv[def_lsv_strat].lsvParName[ii],
                           lsv[def_lsv_strat].lsvPar[ii]);
                  ok = !lies_rwert (&r_wert, buf1, DBL_MIN, DBL_MAX, 
                                    lsv[def_lsv_strat].lsvPar[ii]);
                  if (ok)
                     lsv[def_lsv_strat].lsvPar[ii] = r_wert;
               } /* Steuer-Parameter */
               if (!parameterise_lsv (def_lsv_strat))
                  printf ("%s\n", EVOPAR_AMMA_ERR1);
            } /* das Default-LSV hat Steuerparameter */
            else {
               sprintf (buf1, EVOPAR_AMMA_ERR2, lsv[def_lsv_strat].lsvName);
               printf ("%s\n", buf1);
            }
         } /* LSV, SMA, GLEAM (wg.VorInit): LSV-Params */
         else
         { /* ASMA, AMMA: Adaptionsgeschwindigkeit */
            do
            { /* Frage-Loop */
               ok = TRUE;
               sprintf (buffer, EVOPAR_AMMA_SPEED1, EVOPAR_A_FAST_CHAR, 
			EVOPAR_A_MED_CHAR, EVOPAR_A_SLOW_CHAR, EVOPAR_A_FAST_CHAR);
               ch = char_dialog (buffer);
               if (ch == ESC)
                  break;                                           /* Abbruch */
               if (ch == CR)
                  ch = EVOPAR_A_FAST_CHAR;
               switch (ch) {
                  case EVOPAR_A_FAST_CHAR: setLSVAdaptSpeed (ADAPT_SPEED_SCHNELL, 0); break;
                  case EVOPAR_A_MED_CHAR : setLSVAdaptSpeed (ADAPT_SPEED_MITTEL,  0); break;
                  case EVOPAR_A_SLOW_CHAR: setLSVAdaptSpeed (ADAPT_SPEED_LANGSAM, 0); break;
                  default: /* falsche Eingabe */
                     printf (FMELD_LINE_TXT, BAD_KEY);
                     ok = FALSE;
               } /* switch */
            } /* Frage-Loop */
            while (!ok);
            do
            { /* Frage-Loop */
               ok = TRUE;
               sprintf (buffer, EVOPAR_AMMA_SPEED2, EVOPAR_A_FAST_CHAR, 
			EVOPAR_A_MED_CHAR, EVOPAR_A_SLOW_CHAR, EVOPAR_A_FAST_CHAR);
               ch = char_dialog (buffer);
               if (ch == ESC)
                  break;                                           /* Abbruch */
               if (ch == CR)
                  ch = EVOPAR_A_FAST_CHAR;
               switch (ch) {
                  case EVOPAR_A_FAST_CHAR: setLevelAdaptSpeed (ADAPT_SPEED_SCHNELL); break;
                  case EVOPAR_A_MED_CHAR : setLevelAdaptSpeed (ADAPT_SPEED_MITTEL);  break;
                  case EVOPAR_A_SLOW_CHAR: setLevelAdaptSpeed (ADAPT_SPEED_LANGSAM); break;
                  default: /* falsche Eingabe */
                     printf (FMELD_LINE_TXT, BAD_KEY);
                     ok = FALSE;
               } /* switch */
            } /* Frage-Loop */
            while (!ok);
         } /* ASMA, AMMA: Adaptionsgeschwindigkeit */
         break; /* LSV, SMA: LSV-Params */

         case 12: /* ---------------- ASMA, AMMA: NotenKlass ---------------- */
         sprintf (buf1, EVOPAR_AMMA_NKL, nklassen_anz);
         if (lies_wert (&i_wert, buf1, 1, NKL_MAX, nklassen_anz))
            break;                                                 /* Abbruch */
         if ((i_wert == 2) || (5 < i_wert))
         { /* manuell */
            sprintf (buf1, EVOPAR_AMMA_NKLANZ, NKL_MAX, nklassen_anz);
            if (lies_wert (&i_wert, buf1, 1, NKL_MAX, nklassen_anz))
               break;                                              /* Abbruch */
            for (ii = 0;  ii < i_wert - 1;  ii++)
            { /* Notenwerte vorgeben */
               if (ii == 0)
                  ug = 0.0;
               else
                  ug = nklassen_feld_neu[ii-1];
               sprintf (buf1, EVOPAR_AMMA_NKLWRT, ii+1, ug, RNOTEN_MAX, 
			nklassen_feld_neu[ii]);
               if (lies_rwert (&r_wert, buf1, ug, RNOTEN_MAX, nklassen_feld_neu[ii]))
               { /* Abbruch */
                  i_wert = 0;
                  break;                    /* bricht nur die for-Schleife ab */
               } /* Abbruch */                               
               nklassen_feld_neu[ii] = r_wert;
            } /* Notenwerte vorgeben */
            if (i_wert != 0)
            { /* kein Abbruch: Werte uebernehmen */
               nklassen_anz = i_wert;
               for (ii = 0;  ii < nklassen_anz - 1;  ii++)
                  nklassen_feld[ii] = nklassen_feld_neu[ii];
               nklassen_feld[nklassen_anz-1] = RNOTEN_MAX + 0.001;
            } /* kein Abbruch: Werte uebernehmen */
         } /* manuell */
         else
            set_std_nkl (i_wert);                            /* Standardwerte */
         break; /* NotenKlass */

         case 13: /* ----------------- ASMA, AMMA: AdaptAlt ----------------- */
         sprintf (buf1, EVOPAR_AMMA_ALT, oldDistribFrac*100.0);
         if (lies_rwert (&r_wert, buf1, 0.0, 99.0, oldDistribFrac))
            break;                                                 /* Abbruch */
         oldDistribFrac = r_wert / 100.0;
         break; /* ASMA, AMMA: AdaptAlt */

         case 14: /* ------------------- AMMA: NixLSVLim -------------------- */
	 printf (EVOPAR_AMMA_NIXLIM);
	 printf (EVOPAR_LSVOFF_TXT1);
         printf (EVOPAR_LSVOFF_TXT2, startLsvPMin * 100.0, P_MIN * 100.0);
         printf (EVOPAR_LSVOFF_TXT3, lsv_nix_anwendung_limit);
         printf (EVOPAR_LSVOFF_TXT4);
         sprintf (buf1, EVOPAR_LSVOFF_QUERY, 10, lsv_nix_anwendung_limit);
         if (lies_wert (&i_wert, buf1, 1, 10, lsv_nix_anwendung_limit))
            break;                                                 /* Abbruch */
         lsv_nix_anwendung_limit = i_wert;
         break; /* AMMA: NixLSVLim */
#endif /* MIT_LSV */
 
         case ESC: 
            evo_menue_ende = TRUE;
         break;
      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING); /*Ausg.evtl. Meldungen + Loeschung*/
   } /* sub menue loop */
   while (!evo_menue_ende);
} /* modif_evo_defaults */
