/*******************************************************************************
HyGLEAM                                                                   V3.3.1

                                 H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: EVOlution
 
Package: evo                    File: evo.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul realisiert zusammen mit "evo_gsw" die Evolution einer Generation einer
Population in "evo_1gen" entsprechend dem Nachbarschaftsmodell. Dabei ist die
kleinste Verarbeitungseinheit, die nicht unterbrochen werden kann, eine Paarung.

Der Modul ist alternativ zu "parSimEvo.c" zu sehen, welcher die gleiche Schnitt-
stelle fuer die parallele Ausfuehrung von Simulations- und LSV-Auftraegen durch 
externe Optimierungsdienste implementiert. 
"evo.c" realisiert die Einknotenversion und die durch MPI_PAR_VERS definierte 
parallele Version fuer MPI-basierte Cluster mit Parallelisierung der Population.

**Veraltet:
**Die Unterbrechung erfolgt in der Einknotenversion (Compilerschalter PAR_VERS 
**nicht gesetzt) per Tastendruck. Die globale Variable "taste" erhaelt den Wert
**TRUE, um dies nach aussen anzuzeigen. In der Parallelversion (Compilerschalter 
**PAR_VERS gesetzt) erhaelt das Programm eine Nachricht und teilt dem Package 
**durch Setzen von "taste" den Unterbrechungswunsch mit.
   
Die Population wird als lineare Struktur dargestellt, wobei Paarungen nur
zwischen benachbarten, d.h. lokal nahen Individuen (deme) moeglich sind. 


Die von der uebergeordneten Software ("evo_steu") einstellbaren Parameter und 
lokalen Regelnsind:
     - das Populationsmodell: 2D (Ring, Leiter) oder 3D (Torus)  (z.Zt.nur 2D)
     - die Populationsgroesse, wird in Gen0 gesetzt
     - die Nachbarschaftsgroesse,
     - die Strategie fuer die Wahl des Partners:
         - Ranking mit Fitness des Besten: 1 <= max_fitness <= 2  (z.Zt.impl.)
         - proportionale Selektion mit Skalierung W=Groesse des 'Windows'
     - die Ueberlebensregel
         - akzeptiere immer den besten Nachkommen (accept_all)
         - akzeptiere den besten Nachkommen entsprechend der Note relativ
           zur Nachbarschaft, falls diese :
             - besser oder etwas schlechter ist als der lokal Schlechteste
               (worse), z.Zt. nicht implementiert,
             - besser ist als der lokal Schlechteste (local_least)
             - besser ist als der lokale Elternteil (better_parent)
         - Modifikation der Ueberlebensregel zu einer elitaeren Strategie (ES),
           dies erlaubt das lokal Beste und damit das global Beste zu
           erhalten. Die Regel 'better_parent' ist immer elitaer.
     
Voreingestellt ist das 2D Populationsmodell mit einer Nachbarschaftsgroesse von 
8. Der Partner wird mittels linear rank Selektion bestimmt. Die Wahrscheinlich-
keitsverteilung wird mit dem Parameter "max_fitness" eingestellt. Default ist 
eine mittlere Selektion mit max_fitness=1.4. Es sind alle Ueberlebensregeln mit
Ausnahme von "worse" und "worse_ES" implementiert. Die Regel "local_least" 
erlaubt eine breitere Suche, wodurch die Gefahr, an einem Suboptimum haengen zu 
bleiben, auf Kosten der Konvergenzgeschwindigkeit sinkt. Die Regel "better_parent" 
foerdert dagegen die Tiefensuche mit der Gefahr vorzeitiger Konvergenz.


Die Routinen im einzelnen:

BOOL   evo_1gen (void);
          Rechnet eine Generation durch und liefert die entsprechenden Daten der
          besten AK der Population in "gen_note" und "gen_len" ab. Darueber-
          hinaus wird in "bestJobNote" die Note des besten Individuums des 
          OptJobs gemerkt unabhaengig davon, ob es sich noch in der Population
          befindet.
          Bei Vorliegen einer Stoerung liefert die Routine FALSE ab, sonst TRUE.
          Es gibt folgende Stoerungen, die in nachstehendes (package-)globalen 
          Variablen vermerkt werden:
          - eine Taste wurde gedrueckt:      "taste" ist TRUE. (1-Knotenversion)
          - es trat ein Fehler auf:          "err_state" ist nicht GLEAM_OK.
          - der Speicherplatz ist alle:      "mem_out" ist TRUE.
          - Absturz des externen Simulators: "sim_up"  ist FALSE.

BOOL   pruef_und_speichere (CHAIN_ELEM_PTR_TYPE *c_ptr, 
                            INT                  mut_index);
          Die Routine fuehrt fuer den Nachkommen "c_ptr" den P-Test durch. Da-
          nach erfolgt bei gesetztem "lsv_all_children" die lokale Optimierung 
          durch "do_lsv_start" inkl. Bewertung. Andernfalls erfolgt die Bewer-
          tung durch Simulation, wobei AKs, die besser als der bisher erzeugte 
          Nachkomme sind, akzeptiert werden und die bisher erzeugte beste AK 
          ersetzen.              
          Unplausible Ketten loescht bereits "repair_1_chain" bzw. deren Routi-
          nen. Unbewertbare Ketten werden ebenfalls geloescht. 
          Alle evaluierten Nachkommen werden mitgezaehlt ("indiv_ctr"). Der 
          "mut_index" akzeptierter Nachkommen wird fuer statistische Zwecke in 
          der globalen Variable "mut_op" gespeichert.                                      
          Auftretende Fehler werden als Funktionsergebnis und in "err_state" 
          zurueckgeliefert.                                                           
          ADI: Bei "best" ("lsv_all_children" ist FALSE) bleibt alles unveraen-
               dert. Bei "all" ("lsv_all_children" ist TRUE) werden alle bewert-
               baren Nachkommen in "child_ak_list" gespeichert, wobei die beste
               Note in "best_child_note" gemerkt wird und in "best_child_idx" 
               der Index des besten Nachkommen in der "child_ak_list". 
               "best_child_ptr" zeigt auf das erste Kind, damit die Abfrage in 
               "evo_1gen", ob ueberhaupt Nachkommen erzeugt wurden, positiv aus-
               geht. Der Zeiger wird in "evo_1gen" auf NULL gesetzt, damit nicht 
               an anderer Stelle versucht wird, diese dann wahrscheinlich nicht 
               mehr existierende AK zu loeschen.
               Die Bewertung wird nicht gezeahlt, da dies in "evo_1gen" geregelt
               wird, damit bei LSV-verbesserten AKs nicht eine Bewertung zuviel 
               gezaehlt wird (Schwaeche der Implementierung: Bewertete Start-AKs  
               bewerten die LSVs noch einmal).
               Die Statistik-Erfassung in "mut_op" findet nicht statt.

BOOL   do_lsv (BOOL  lamarck,
               INT   znr);
          Fuehrt das durch die globale Variable "lsv_strat" vorgegebene LSV 
          (Index in "lsv") im Kontext eines Opt-Jobs als alleiniges Verfahren 
          mit der unter Index 0 im Popfeld gespeicherten AK aus und liefert 
          TRUE ab, wenn kein Fehler auftrat. Wenn die AK bzw. eine der AKs be-
          reits die "zielnote" hat, wird das LSV nicht ausgefuehrt. Bei gesetz- 
          tem "lamarck" wird die Ergebnis-AK an das gefundene lok. Optimum ange-
          passt, wenn dieses besser als die urspruengliche AK ist. Neu erzeugte 
          Ketten werden im AK-Mem gespeichert, bereits existierende bei gesetz-
          tem "lamarck" ggf. im AK-Mem umgehaengt. "lamarck" wird durchgereicht.
          Meldungen erfolgen in den Textpuffer ab "znr", sofern "znr" > 0. Bei 
          "znr" == 0 erfolgt eine Ergebnismeldung ins Logfile.

BOOL allocChrList (void);
          Allocates the "chrList". Dummy routine in this context. Returns always 
          TRUE.

void clearInitialChrList (void);
          Clears the "chrList" of the initial population. Dummy routine in this 
          context.

BOOL clearChrList (BOOL  delChr);
          Clears the "chrList". Dummy routine in this context. Returns always 
          TRUE.

BOOL evaluateStartPop (void);
          Evaluates the initial population. Dummy routine in this context. 
          Returns always TRUE.

--------------------------------------------------------------------------------
basiert auf C-Unit: evo.c
--------------------------------------------------------------------------------
Implementierungs-Status:
Implementierung vorbereitet aber noch nicht fertig und integriert:
   AK-Vorgenerierung ("gen0_gen") ist fertig, greift aber z.T.auf noch unfertige
     Routinen aus Modul "pre_gen" oder "appl_gen" zurueck.
   ReInit mit GEN und GEN_BEST ist nicht getestet!
   Sintflut-Optimierung ("rain_cycle").
Nicht implementiert: 
   Partnerwahl mit qualitaets-proportionaler Wahrscheinlichkeit.
   Torus-Population.
   Ueberlebensregeln WORSE und WORSE_ES.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0: 
 1. Strategieparameter des PSRO-ReInit ueberarbeitet.
 2. Integration des gen. Operators "ADD_NEW_SEGM"
 3. Integration der gen. Operatoren "PAR_CHANGE_SMALL" und "SEGM_CHANGE_SMALL".
 4. Externe Job-Terminierung bei GLEAM-Job-Variante.
 5. Fehler in "gen0_mix" beseitigt.
 6. "gen0_baks" nimmt immer die maximal ersten 3 BAKS.
 7. Bei permanent unbewertbaren oder unplausiblen AKs bricht "ch_gen" ab.
 8. Entfernung des Sintflut-Algorithmus und des PSRO. "ch_gen" erzeugt 
    Fehlermeldung bei MemOut. Anpassung von "gen0_gen" fuer LSV.     WJ 03.08.00
 9. Implizite Typkonversionen an g++-Compiler angepasst.             WJ 10.08.00
10. Aus "gen0_gen" "gen0_lsv" gemacht. "lsv_rosen", "gen0_all",
    "do_lsv_on_init_pop" eingefuehrt.                                WJ 25.09.00
11. "compute_results" angepasst.                                     WJ 04.10.00
14. Verbesserungen beim Logging von Jobs mit Nachoptimierung.        WJ 10.11.00
15. Default-Parameter fuer Nachoptimierung justiert.                 WJ 15.11.00
16. Umbenennung Baldwin -> Lamarck. Begrenzung der geretteten 
    Nischenrepraesentanten auf "max_repraes_anz". Steuerung ob eine 
    oder beide Bedingungen zum EvoAbbruch fuer Nachopt erfuellt 
    sein muessen. Verzoegertes LSV-Zuschalten nach Nischenbildung.   WJ 11.12.00
18. Integration des COMPLEX-Algorithmus.                             WJ 02.05.01
19. "do_lsv": LSV-Ergebnis wird bei existierenden AKs (NachOpt) in 
    "last_evo_erg" gespeichert.                                      WJ 11.06.02
20. "evo_1gen": Logfile-Meldungen bei externem Evo-Abbruch.
    "lsv_rosen" u. "lsv_complex" brechen bei Erreichen der 
    Zielnote und init_strat=ALL ab.                                  WJ 26.06.02
21. MIT_ADI: Anpassungen fuer die adaptive direkte Integration.      WJ 22.07.04
22. MIT_ADI: Anpassungen fuer neuen Aufruf von "lsv_erg_fuer_adi".   WJ 17.12.04
23. MIT_ADI: Anpassungen fuer die Adaption bei der all-Variante.     WJ 29.12.04
24. EARLY_FITNESS gesetzt: Abbruch der Generation, wenn "zielnote" 
    erreicht.                                                        WJ 17.01.05
25. Erweiterung fuer n LSVs und Heuristiken. "do_lsv_on_init_pop"
    in "gen0_lsv" integriert. Nachoptimierungs-Code im Zusammenhang 
    mit "ALL" und "ALL1COMPL", insbesondere "gen0_all" gestrichen.
    Entfernung von MIT_ADI, es gibt nur noch MIT_LSV.                WJ 29.08.06
26. "ch_gen": Verbesserte Fehlerbehandlung und nur noch nicht simu-
    lierte AKs werden simuliert.                                     WJ 27.10.06
27. Variable "farbe" in INDIVID_TYPE gestrichen.                     WJ 09.11.06
28. "init_strat" wird package-global.                                WJ 11.01.07
29. Neue Operatoren OX_XO, OX_RECO, PPX_XO und PPX_RECO.             WJ 29.01.07
30. Matlab-Version GLEAMKIT: Neuer Schalter BS_WINXP_GNU und Aufruf
    von "mlOnlineVisu" in "pruef_und_speichere", das auch zentral 
    die beste AK sowie deren Laenge merkt ("gen_note" u. "gen_len").
    Streichung von "calc_pop_best".                                  WJ 24.08.07
31. Reparatur von 30: Die zentrale Erfassung der besten AK wurde von
    "pruef_und_speichere" nach "evo_1gen" zur Nachkommensakzeptierung 
    verlagert: Gilt fuer alle OptVerfahren mit best- oder all-Verb. 
    Faktische Abschaltung des EvoAbbruchtests fuer interakt.Vers.
    Neuer Package-Export: "def_max_fit", "default_survival_rule".    WJ 02.11.07
32. CEC_MBF: Abbruch nach "maxFEs" FunktionsEvaluierungen und Er-
    fassung der FEs und des Fehlerwerts.                             WJ 07.11.07
33. Gorba-Spezifische Mutationen: In "pruef_und_speichere" wird der
    Simulator nur bei nicht gesetztem SIMULATED-Bit aufgerufen.      WJ 24.04.08
34. Aufteilung des "evo"-Moduls in "evo_gsw" und "evo" derart, dass
    die von einer Parallelisieung nicht betroffenen Teile in 
    "evo_gsw" sind.                                                  WJ 04.06.08
35. Aufteilung in "evo" und "evo_gsw", um von "evo" eine Variante
    fuer parallele Simulation anlegen zu koennen. "ch_gen" erhaelt
    jetzt nur den "indivIdx" und traegt die erzeugte AK ins 
    "pop_feld" ein. "accept" in "accepted" umbenannt.                WJ 24.06.08
36. Erweiterung der Parameterliste von "mlOnlineVisu" um "indivIdx",
    der hier NIX_IDX ist und Aufruf in "pruef_und_speichere".
    Evo-Abbruchtest der Engine korrigiert.                           WJ 20.08.08
37. GORBA: Sicherung von Daten des besten Indivs (GORBA_GLEAM).
    Derzeit nur fuer GLEAM und nicht fuer HyGLEAM implementiert!     WJ 18.12.08
38. Fuer GORBA-LSVs: "indiv_ctr" wird Package-Export. 
    Zaehlung der Bewertungen ("indiv_ctr") in "pruef-und_speichere" 
    im "all-Teil" korrigiert und im 2.Teil verbessert. 
    Punkt 37 rueckgaengig gemacht. Wird in der Engine erledigt.  
    Neue package-lokale Variable "gleamPur".   
    Bei gesetztem MIT_GORBA_LSVS: 
    1. "do_lsv" und "pruef_und_speichere": Vorbereitende Simulation 
       vor LSV-Start.
    2. "evo_1gen: Aufruf von "listenPlanDataToSave" bei AxMA/all.    WJ 20.11.09
39. "do_lsv" und "pruef_und_speichere": Bei RAS-LSV Aufruf von 
    "aktAJobPlanDataToSave" anstelle von "aktPlanDataToSave".        WJ 28.01.10
40. Entfernung von "mlOnlineVisu", "mitMatLabSimu" und "mitOnlVisu".
    Entfernung von Code im Kontext von GLEAM_TCL. Entfernung ueber-
    fluessiger Variable.                                             WJ 28.06.12
41. Der Pointer auf den Hammingabstand wird von "partnerwahl" mit-
    geliefert, wodurch der Aufruf von "get_ham_ptr" entfaellt.       WJ 28.09.12
42. Anpassungen des Variablenexports fuer die MPI-ParVers. "ch_gen"
    geht nach Modul "evo_gsw". "evo_1gen" wird bei MPI_PAR_VERS an-
    stelle von package-lokalem nunmehr globaler Export.
    Aufruf von "sendeAK" bei Offsping-Akzeptanz.                     WJ 21.11.12
43. Trennung der Erfassung des Generationsbesten in der Population 
    ("gen_note", "gen_len") von der besten erreichten Note eines 
    OptJobs ("bestJobNote").                                         WJ 30.11.12
44. Einbau eines Abbruchs bei Erreichen von "evalLimit". Aufruf von 
    "sendeAK" fuer akzeptierte AKs nur, wenn sie auch zu versenden 
    sind. ""gen_counter" wird Packageexport.                         WJ 14.02.13
45. GUI: Die Unterbrechbarkeit wird auf jeweils eine Paarung anstelle 
    eines GenOps beschraenkt. Letzteres steht wahrscheinlich im 
    Konflikt mit der dann wiederholten Partnerwahl. 
    "no_accept_gen_counter" und "no_deme_impr_gen_counter" werden 
    Package-Export.                                                  WJ 21.11.14
46. Renovierung: Streichung des "link"-Includes.                     WJ 18.04.16
47. Anpassung der "write_to_log"-Aufrufe. Hamming-Abstand muss  
    groesser und nicht nur groesser-gleich sein als Mindestwert. 
    Einfuehrung von "with_evo_term_tst" zum effizienteren Job-
    Abbruchtest der Commandlineversion.                              WJ 19.01.17
48. "do_lsv": Neben der Fitness wird nun auch die AK-Laenge erfasst. 
    Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".            WJ 30.06.17
22. Verlagerung allen Exports nach "evo_gsw.c", der nicht spezifisch 
    fuer diese Implementierung ("sequentiel und MPI_PAR_VERS) ist.   WJ 06.07.17
23. Fuer EXT_SIMU_SERV wurde eine alternative Implementierung dieses 
    Moduls in "parSimEvo" vorgenommen. Daher enthaelt diese Implemen-
    tierung folgende Routinen als leere Dummy-Implementierungen: 
    allocChrList(), clearChrList(), clearInitialChrList() und 
    evaluateStartPop().                                              WJ 15.11.17
24. Hammingabstand muss fuer XO/Reko nur groesser-gleich sein 
    als ein Mindestwert, damit ein Mindestwert von 0 bewirkt, dass 
    XO/Reko immer stattfinden.                                       WJ 24.01.18
25. Der VISU_MODE arbeitet mit Unterbrechungen durch simulierte Tas-
    tatureingaben (taste == TRUE), was nun beim Umgang mit"accepted" 
    und "accept_improved" beruecksichtigt wird, siehe Schalter.      WJ 02.07.18
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 17.07.2018
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
#include "hmod.h"
#include "aufg.h"
#include "simu.h"
#ifdef GORBA_GLEAM
  #include "xml.h"
  #include "gsim.h"
  #ifdef MIT_LSV
    #include "bew.h"
    #include "appl.h"
    #include "glsv.h"
  #endif /* MIT_LSV */
#endif /* GORBA_GLEAM */
#ifdef MPI_PAR_VERS
  #include "chio.h"
  #include "mPar.h"
#endif /* MPI_PAR_VERS */
#include "evo.h"
#include "evoInt.h"

#include EVO_TEXTS


#define EARLY_FITNESS   /* Generationsabbruch, wenn "zielnote" erreicht wird. */

#ifdef GLO_CHECKS
  #undef ADI_TST
  #undef ACCEPT_TST
#endif /* GLO_CHECKS */



/* ========================== Exportierte Variable: ========================= */
#ifdef CEC_MBF
  DOUBLE     cec_accuracy_val,     /* CEC-MBFs: Genauigkeitsgrenze            */
             cec_fopt;             /* CEC-MBFs: Funktionsoptimum              */
#endif /* CEC_MBF */
#ifdef MPI_PAR_VERS                /* sonst lokale Variable von "evo_1gen":   */
  BOOL       accepted,             /* TRUE = Offspr.wird in einer Gen.akzept. */
             accept_improved;      /* TRUE = Offspr. verbessert Deme          */
#endif /* MPI_PAR_VERS */



/* ======================== Package-globale Variable: ======================= */
#ifndef MPI_PAR_VERS
  BOOL    accepted,              /* TRUE = Offspr.wird in einer Gen.akzeptiert*/
          accept_improved;       /* TRUE = Offspr. verbessert Deme            */
#endif /* keine MPI_PAR_VERS */
#ifdef CEC_MBF
  LONG    aktFEsLim;             /* CEC: aktuelle FEs-UG zum Schreiben        */
  BOOL    fesT,                  /* CEC: #F-Zeile f.Tausend ungeschrieben     */
          fesZT,                 /* CEC: #F-Zeile f.ZehnTsd. ungeschrieben    */
          fesHT,                 /* CEC: #F-Zeile f.HundertT. ungeschrieben   */
          fesMax,                /* CEC: #F-Zeile f.maxFEs ungeschrieben      */
          sZeile;                /* CEC: #S-Zeile ungeschrieben               */
#endif /* CEC_MBF */



/* ============================ Lokale Konstante: =========================== */
#define PROPORTIONAL          0           /* Code fuer Partnerwahl-Strategie  */
#define RANK                  1           /* Code fuer Partnerwahl-Strategie  */
#define MATE_SELECTION     RANK           /* aktuelle Partnerwahl-Strategie   */
 


/* ============================ Lokale Variable: ============================ */
static DOUBLE               neue_lsv_note;   /* Durch das LSV ermittelte Note */
static INT                  next_indiv,      /* next individual to process    */
                            next_gen_op;     /* next genetic operation        */
#ifdef MIT_LSV
  static INT                best_child_idx;  /* adapt.all Idx d.besten offspr */
  static LONG               aufwand;         /* LSV-Aufwand in Evaluationen   */
#endif /* MIT_LSV */



/* ============================ Lokale Routinen: ============================ */




/* ================================ evo_1gen ================================ */
BOOL evo_1gen (void)
{
   DOUBLE *ham_ptr,
           local_weak_note,
           local_best_note,
           ketten_wert,
           fErrVal,              /* CEC: Funktionsfehler (Fakt - Fopt)        */
           f_alt,                /* alte Fitness vor dem LSV-Lauf             */
           fg;                   /* fitness gain                              */
   LONG    newAktFEsLim;         /* CEC: neue FEs-UG zum Schreiben            */
   INT     max_segm_anz,
           max_ch_len,
           partner,
           ii;
   BOOL    not_best_indiv, 
           replace,
           lamarck,
           unfit;                /* TRUE, solange "zielnote" oder bei CEC_MBF */
                                 /* auch "maxFEs" nicht erreicht.             */
#ifdef MIT_LSV
   DOUBLE  lsv_opt_p,            /* Wahrscheinlichkeit der LSV-Anwendungen    */
           lsv_anw_ist_rate,     /* Rate der tatsaechlichen LSV-Anwendungen   */
           best_evo_note;        /* Beste Note der Evo-Kinder                 */
   LONG    indiv_ctr_all_start,  /* indiv_ctr vor LSV-Opt. aller Nachkommen   */
           indiv_ctr_lsv_start;  /* indiv_ctr bei Beginn eines einzelnen LSVs */
   INT     best_lsv_child_idx,   /* Index des besten LSV-Kindes               */
           lsv_done_anz,         /* Anzahl tatsaechlich LSV-verbessert.Kinder */
           all_p_nkl,            /* Notenklasse zur all-Adaption              */
           akt_nkl;              /* Aktuelle Notenklasse der LSV-Adaption     */
   BOOL    apply_lsv;            /* TRUE, wenn LSV anzuwenden ist             */
   STR_TYPE  str1, str2;
#endif /* MIT_LSV */
  
   /* evo_1gen */
   unfit = TRUE;
   if (taste)
   { /* Unterbrechung durch Taste */
      akt_indiv         = next_indiv;
      akt_gen_op        = next_gen_op;
      taste             = FALSE;
      intrpt_at_gen_end = FALSE;
#ifdef VISU_MODE
      accepted          = FALSE;
      accept_improved   = FALSE;
#endif /* VISU_MODE */
   } /* Unterbrechung durch Taste */
   else
   { /* Keine Unterbrechung durch Taste */
      akt_gen_op      = 0;
#ifndef MPI_PAR_VERS
      accepted        = FALSE;
      accept_improved = FALSE;
      akt_indiv       = 0;
#endif /* keine MPI_PAR_VERS */
   } /* Keine Unterbrechung durch Taste */

#ifdef ACCEPT_TST
   dumpPopFeld (pop_size);
#endif /* ACCEPT_TST*/

   /* --- Bearbeite die Population sequentiell, bei Fehler/Taste "return": -- */
#ifndef MPI_PAR_VERS
   while (!taste && (akt_indiv <= max_pop_index) && unfit)
   { /* Paarbildung eines Individuums */
#endif /* keine MPI_PAR_VERS */
      akt_ptr = pop_feld[akt_indiv].chain_ptr;

      /* --------------------------- Partnerwahl: --------------------------- */
      switch (MATE_SELECTION) 
      {
         case PROPORTIONAL:                               /*not supported yet */
         err_state = fatal (EVO_EVO, 10, NO_INFO, NO_INFO, "", 
                            NOT_YET_IMPLEMENTED);
         return (FALSE);
         /* break;    aktivieren, wenn Implementierung! */

         default:             /* Waehle Partner mittels linear rank selection */
	    ham_ptr = partnerwahl (akt_indiv, &partner, &local_weak_note, &local_best_note);
         break;
      }
      partner_ptr = pop_feld[partner].chain_ptr;    

      /* ------------- Ermittlung des Platzbedarfs und MemTest: ------------- */
      /* Max.Platzbedarf eines XO-Operators plus 1 grosses BestChild (3AKs):  */
      /* 2 x Summe der Laengen beider Ketten plus 4 Headerelemente und        */
      /* 2 x Summe der Segmentanzahl beider Ketten plus SegmAnz pro Descrip-  */
      /*           tor wegen moeglichem Verschnitt                            */
      max_ch_len   = 2 * (partner_ptr->u_el.u_xel.u_hdr.chain_length +
                          akt_ptr->u_el.u_xel.u_hdr.chain_length      ) + 4;
      max_segm_anz = 2 * (partner_ptr->u_el.u_xel.u_hdr.segment_anz +
                          akt_ptr->u_el.u_xel.u_hdr.segment_anz) + SEGM_PTR_ANZ;
      if (!enough_free_mem (AK_HEADER, max_ch_len, max_segm_anz)) 
      { /* Speicher beschaffen */
         err_state = reclaim_memory (AK);
         if (!enough_free_mem (AK_HEADER, max_ch_len, max_segm_anz) || 
             (err_state != GLEAM_OK)) 
         { /* Kein Speicher beschaffbar oder Fehler: Abbruch */
            mem_out = TRUE;
            return (FALSE);
         } /* Kein Speicher beschaffbar oder Fehler: Abbruch */
      } /* Speicher beschaffen */

      /* ----- Ermittlung des Hammingabstands und der XO-Zulaessigkeit: ----- */
      if ((*ham_ptr) == NO_HAM)                /* Hamming-Abstand unbekannt ? */
         *ham_ptr = ham_dist (akt_ptr, partner_ptr);    /* HamAbst. bestimmen */
      do_xo = (xo_min_ham_dist <= *ham_ptr);    /* HamAbst ausreichend f. XO ? */

      /* ------------ Durchfuehrung der genetischen Operationen: ------------ */
      if (!do_g_operationen ())
         return (FALSE);   /* "melde_evo_crash" bereits in "do_g_operationen" */

#ifdef GLEAM_USR
      taste = kb_hit ();              /* Unterbrechbarkeit nach jeder Paarung */
#else
  #ifdef VISU_MODE                       /* Unterbrechung bei Generationsende */
      taste = (akt_indiv == max_pop_index);          /* zur Datenuebertragung */
  #endif /* VISU_MODE */
#endif /* GLEAM_USR */

      /* ---------- Bester Nachkomme ersetzt eventuell Elternteil: ---------- */
      if ((best_child_ptr != NULL) && (!taste || (akt_gen_op == go_anz)))  
      { /* Es gibt einen Offspring UND (Keine Taste ODER Paarung fertig) */
         /* --- Bestimme Note d.besten u.schwaechsten Individuums im Deme: -- */
         if (pop_feld[akt_indiv].note >= local_best_note)
         { /* aktuelles Individuum ist das Beste des demes */
            local_best_note = pop_feld[akt_indiv].note;
            not_best_indiv  = FALSE;
         } /* aktuelles Individuum ist das Beste des demes */
         else 
            not_best_indiv = TRUE;
         if (pop_feld[akt_indiv].note < local_weak_note)
            local_weak_note = pop_feld[akt_indiv].note;

#ifdef MIT_LSV
         if (lsv_best_child)
         { /* Besten Offspring mit LSV optimieren u.bewerten*/
            if (gleam_adi)
            { /* GLEAM mit ADI */
               if (gleam_amma)                          /* setzt "lsv_strat": */
                  set_actual_adi_lsv (&akt_nkl, best_child_note);
               else
                  akt_nkl = get_nklasse (best_child_note);            /* ASMA */
               aufwand    = indiv_ctr;
               err_state |= parameterise_lsv_for_best (akt_nkl);
            } /* GLEAM mit ADI */
            lamarck     = check_for_lamarck (lamarck_rate);
            ketten_wert = do_lsv_start (&best_child_ptr, lamarck, &err_state);
            if (ketten_wert != KEINE_NOTE)
            { /* kein Fehler */
               f_alt = best_child_note;
               if (ketten_wert > best_child_note)
               { /* Kind erfolgreich LSV-optimiert */
                  fg              = ketten_wert - f_alt;
                  best_child_note = ketten_wert;   
               } /* Kind erfolgreich LSV-optimiert */
               else
                  fg = 0.0;
               if (gleam_adi)
               { /* GLEAM mit ADI */
                  aufwand = indiv_ctr - aufwand;
                  lsv_erg_fuer_adi (akt_nkl, fg, f_alt, aufwand);
               } /* GLEAM mit ADI */
            } /* kein Fehler */
         } /* Besten Offspring mit LSV optimieren u.bewerten*/
         else
            if (gleam_adi)
            { /* Alle Offspring mit adaptiver Wahrscheinlichkeit optimieren */
               best_child_ptr = NULL; /* zeigt hier nur an, dass Offspr.exist.*/ 
               indiv_ctr_all_start = indiv_ctr;
               if (gleam_amma)
                  set_actual_adi_lsv (&all_p_nkl, best_child_note);
               else
                  all_p_nkl = get_nklasse (best_child_note);          /* ASMA */
               if (anz_children > 1)
                  lsv_opt_p = get_lsv_p (all_p_nkl);
               else
                  lsv_opt_p = 1.0;                  /* eigentlich ueberflussig */
               best_evo_note = best_child_note;
               lsv_done_anz  = 0;
               best_lsv_child_idx = best_child_idx;
  #ifdef ADI_TST
               sprintf (buf1, "evo_1gen  anz_children=%d  best_child_idx=%d "
                        "best_child_note=%g  lsv_strat=%d", 
                        anz_children, best_child_idx, best_child_note, lsv_strat);
               write_to_log (buf1, CLOSE_LOG);
               strcpy (str1, "evo_1gen  LSV-opt. childs (idx):");
  #endif /* ADI_TST */
               for (ii = 0;  ii < anz_children;  ii++)
               { /* alle Nachkommen */
                  if (ii == best_child_idx)
                     apply_lsv = TRUE;
                  else                                   /* Die Routine passt */
                     apply_lsv = check_for_lamarck (lsv_opt_p);  /* hier halt */
                  if (apply_lsv)
                  { /* Nachkomme lokal verbessern */
  #ifdef ADI_TST
                     sprintf (str2, " %d", ii);
                     strcat (str1, str2);
  #endif /* ADI_TST */
                     indiv_ctr_lsv_start = indiv_ctr;
                     lamarck = check_for_lamarck (lamarck_rate); /* diesmal wirklich*/
                     f_alt   = child_ak_list[ii]->u_el.u_xel.u_hdr.fitness_note;
                     err_state  |= parameterise_lsv_for_all (&akt_nkl, f_alt);
  #ifdef MIT_GORBA_LSVS
		     listenPlanDataToSave (ii);
  #endif /* MIT_GORBA_LSVS */
                     ketten_wert = do_lsv_start (&(child_ak_list[ii]), lamarck, &err_state);
                     if (ketten_wert != KEINE_NOTE)
                     { /* kein Fehler */
                        if (ketten_wert > f_alt)
                           fg = ketten_wert - f_alt;        /* Verbesserung ! */
                        else
                           fg = 0.0;
                        aufwand = indiv_ctr - indiv_ctr_lsv_start;
                        lsv_erg_fuer_adi (akt_nkl, fg, f_alt, aufwand);
                        if (ketten_wert > best_child_note)
                        { /* besser als bisheriger bester Nachkomme */
                           best_child_note    = ketten_wert;
                           best_lsv_child_idx = ii;
                        } /* besser als bisheriger bester Nachkomme */
                        lsv_done_anz++;
                     } /* kein Fehler */
                  } /* Nachkomme lokal verbessern */
               } /* alle Nachkommen */
               aufwand = indiv_ctr - indiv_ctr_all_start;
  #ifdef ADI_TST
               strcpy (str2, "");
  #endif /* ADI_TST */
               if (best_child_note > best_evo_note)
               { /* Verbesserung: LSV-Bestes ist besser als Evo-Bestes */
                  fg = best_child_note - best_evo_note;
                  if (best_child_idx != best_lsv_child_idx)
                  { /* Bonus: echte all-Verbesserung */
                     fg += best_child_note - 
                        child_ak_list[best_child_idx]->u_el.u_xel.u_hdr.fitness_note;
                     if (statistik_pro_gen)
                     { /* Bonus erfassen */
                        sprintf (buf3, "Fitness gain bonus bei all: %g",
                                 best_child_note - 
                                 child_ak_list[best_child_idx]->u_el.u_xel.u_hdr.fitness_note);
                        write_to_log (buf3, CLOSE_LOG);
                     } /* Bonus erfassen */
  #ifdef ADI_TST
                     sprintf (str2, "fg-Bonus=%g",  
                              best_child_note - 
                              child_ak_list[best_child_idx]->u_el.u_xel.u_hdr.fitness_note);
  #endif /* ADI_TST */
                  } /* Bonus: echte all-Verbesserung */
               } /* Verbesserung: LSV-Bestes ist besser als Evo-Bestes */  
               else
                  fg = 0.0;
  #ifdef ADI_TST
               sprintf (buf3, "%s  Erg: best_idx=%d note=%g fg=%g %s aufwand=%ld", 
                        str1, best_lsv_child_idx, best_child_note, fg, str2, aufwand);
               write_to_log (buf3, CLOSE_LOG);
  #endif /* ADI_TST */
               if (anz_children > 1)
               { /* LSV-Anwendungs-Ist-Rate bestimmen */
                  lsv_anw_ist_rate = lsv_done_anz - 1;   /* damit int -> real */
                  lsv_anw_ist_rate = lsv_anw_ist_rate / (anz_children - 1);
               } /* LSV-Anwendungs-Ist-Rate bestimmen */
               else
                  lsv_anw_ist_rate = 0.0;
               all_erg_fuer_adi (all_p_nkl, fg, best_evo_note, aufwand, lsv_anw_ist_rate);
               best_child_ptr = child_ak_list[best_lsv_child_idx];  /* Erg-AK */
               for (ii = 0;  ii < anz_children;  ii++)  /* Loeschung der      */
                  if (ii != best_lsv_child_idx)         /* nicht akzeptierten */
                     err_state |= delete_chain (&(child_ak_list[ii]));/*Offspr*/
            } /* Alle Offspring mit adaptiver Wahrscheinlichkeit optimieren */
#endif /* MIT_LSV */

         /* ------------------ Wende Ueberlebensregel an: ------------------- */
         /*  "..._ES" modifiziert die Ueberlebensregel zu einer elitaeren     */
         /*  Strategie: Erhalt des lokal Besten u. damit des global Besten.   */
         switch (act_survival_rule) 
         {
            case ALWAYS: 
               replace = TRUE;
            break;

            case ALWAYS_ES: 
               replace = not_best_indiv || (best_child_note > local_best_note);
            break;

            case LOCAL_LEAST: 
               replace = best_child_note > local_weak_note;
            break;

            case LOCAL_LEAST_ES: 
               replace = (best_child_note > local_weak_note) && 
                         (not_best_indiv || (best_child_note > local_best_note));
            break;

            case BETTER_PARENT: 
               replace = best_child_note > pop_feld[akt_indiv].note;
            break;
         } /* CASE */
#ifdef ACCEPT_TST
	 printf ("evo_1gen: Bester Nachkommen mit Note %g von Elter %d mit Note %g "
		 "wird akzeptiert: %d\n          Deme-Noten (Min, Max): %g %g  bes"
		 "ser=%d best=%d Elter nicht DemeBestes=%d\n", best_child_note,
		 akt_indiv, pop_feld[akt_indiv].note, replace, 
		 local_weak_note, local_best_note, best_child_note > local_weak_note, 
		 best_child_note > local_best_note, not_best_indiv);
#endif /* ACCEPT_TST*/

         if (replace)
         { /* Kind ersetzt Elternteil */
            err_state |= delete_chain (&(pop_feld[akt_indiv].chain_ptr)); 
            pop_feld[akt_indiv].chain_ptr  = best_child_ptr;
            pop_feld[akt_indiv].chain_ptr->u_el.u_xel.u_hdr.ref_counter = 1;
            pop_feld[akt_indiv].note       = best_child_note;
            pop_feld[akt_indiv].new_chain  = TRUE;           /* veränderte AK */
            berechne_guete (&(pop_feld[akt_indiv].guete), best_child_note);
            loe_deme_ham_werte (akt_indiv);
#ifdef MPI_PAR_VERS
	    if (nbKNrListLen[akt_indiv])      /* AK als Nachbar zu versenden? */
	       sendeAK (best_child_ptr, MsgS2S_UpdateNbAK, akt_indiv, 
			nbKNrListLen[akt_indiv], nbKNrList[akt_indiv]);
#endif /* MPI_PAR_VERS */
            if (best_child_note > local_best_note)
            { /* Kind besser als lokal Bestes */
               accept_improved = TRUE;
#ifdef EARLY_FITNESS
               unfit = (best_child_note < zielnote);
#endif /* EARLY_FITNESS */
            } /* Kind besser als lokal Bestes */
            if (best_child_note > gen_note)
	    { /* Kind besser als bisher bestes der Gen. und Pop. */
	       gen_note = best_child_note;
	       gen_len  = best_child_ptr->u_el.u_xel.u_hdr.chain_length;
	    } /* Kind besser als bisher bestes der Gen. und Pop. */
            best_child_ptr = NULL;                  /* Kind wurde uebernommen */
            accepted       = TRUE;
#ifdef MIT_OP_STATISTIK
            if (statistik_pro_gen)
               (mut_op_ctr[mut_op])++;     /* Statistik ueber gen. Operatoren */
#endif /* MIT_OP_STATISTIK */
         } /* Kind ersetzt Elternteil */
         else /* loesche abgewiesenes Kind */
	 { /* loesche abgewiesenes Kind u.pflege "gen_note" u."gen_len" */
            err_state |= delete_chain (&best_child_ptr);
	    if (gen_note < pop_feld[akt_indiv].note) { /* Elter ist das bisher beste */
	       gen_note = pop_feld[akt_indiv].note;
	       gen_len  = pop_feld[akt_indiv].chain_ptr->u_el.u_xel.u_hdr.chain_length;
	    } /* Elter ist das bisher beste */
	 } /* loesche abgewiesenes Kind u.pflege "gen_note" u."gen_len" */

         if (err_state != GLEAM_OK)
         { /* Evo-Abbruch wegen Fehler bei Offspring-Bearbeitung */
            meld_evo_crash (EVO_WUERG_OFFSPR_MELD, EVO_EVO, NO_INFO);
            return (FALSE);
         } /* Evo-Abbruch wegen Fehler bei Offspring-Bearbeitung */
      } /* Es gibt einen Offspring UND (Keine Taste ODER Paarung fertig) */
#if defined(CEC_MBF)
  #if defined(EARLY_FITNESS)
      unfit &= (indiv_ctr < maxFEs);
  #else
      unfit  = (indiv_ctr < maxFEs);
  #endif /* EARLY_FITNESS */
      if ((cec_best_fval <= cec_accuracy_val) && sZeile)
      { /* CEC: Funktionswert hinreichend genau erreicht: #S-Zeile */
         sprintf (buf3, "#S %ld %.12e", indiv_ctr, cec_best_fval);
         write_to_log (buf3, CLOSE_LOG);
	 sZeile = FALSE;
      } /* CEC: Funktionswert hinreichend genau erreicht: #S-Zeile */
      if (aktFEsLim < indiv_ctr)
      { /* aktFEsLim ueberschritten */
	 fErrVal = cec_best_fval - cec_fopt;
	 if ((1000 < indiv_ctr) && fesT) 
         { /* #F-Zeile fuer 1000 */
            sprintf (buf3, "#F 1000 %ld %.8e", indiv_ctr, fErrVal);
            write_to_log (buf3, KEEP_LOG_OPEN);
            fesT = FALSE;
	 } /* #F-Zeile fuer 1000 */
	 if ((10000 < indiv_ctr) && fesZT) 
         { /* #F-Zeile fuer 10000 */
            sprintf (buf3, "#F 10000 %ld %.8e", indiv_ctr, fErrVal);
            write_to_log (buf3, KEEP_LOG_OPEN);
            fesZT = FALSE;
	 } /* #F-Zeile fuer 10000 */
	 if ((100000 < indiv_ctr) && fesHT) 
         { /* #F-Zeile fuer 100000 */
            sprintf (buf3, "#F 100000 %ld %.8e", indiv_ctr, fErrVal);
            write_to_log (buf3, KEEP_LOG_OPEN);
            fesHT = FALSE;
	 } /* #F-Zeile fuer 100000 */
	 if ((maxFEs < indiv_ctr) && fesMax) 
         { /* #F-Zeile fuer maxFEs */
            sprintf (buf3, "#F %ld %ld %.8e", maxFEs, indiv_ctr, fErrVal);
            write_to_log (buf3, KEEP_LOG_OPEN);
            fesMax = FALSE;
	 } /* #F-Zeile fuer maxFEs */
         newAktFEsLim = (indiv_ctr / 500) * 500;
         sprintf (buf3, "#L %ld %ld %g", newAktFEsLim, indiv_ctr, fErrVal);
         write_to_log (buf3, CLOSE_LOG);
	 if (newAktFEsLim == aktFEsLim)
            aktFEsLim += 500;
	 else
            aktFEsLim = newAktFEsLim;
      } /* aktFEsLim ueberschritten */
#else /* kein CEC_MBF */
  #if defined(EARLY_FITNESS)
      unfit &= (indiv_ctr < evalLimit);
  #else
      unfit  = (indiv_ctr < evalLimit);
  #endif /* EARLY_FITNESS */
#endif /* kein CEC_MBF */

      if (!sim_up)
      { /* Evo-Abbruch wegen Crashs des ext. Simulators */
         meld_evo_crash (EVO_WUERG_SIMU_MELD, EVO_EVO, NO_INFO);
         return (FALSE);
      } /* Evo-Abbruch wegen Crashs des ext. Simulators */

#if !defined(GLEAM_USR) && !defined(MPI_PAR_VERS)   /* sequ. Engine-Version ? */
      externalTermRequest();    /* terminates act.opt.job on external request */
#endif /* sequentielle Engine-version */

#ifndef MPI_PAR_VERS
      if (taste)
      { /* Unterbrechung durch Taste */
         if (akt_gen_op == go_anz)
         { /* Unterbrechung bei Abschluss der gen. Operation */
            next_gen_op       = 0;
            intrpt_at_gen_end = akt_indiv == max_pop_index; /* GenerationsEnde*/
            if (intrpt_at_gen_end)
               next_indiv = 0;
            else
               next_indiv = akt_indiv + 1;
         } /* Unterbrechung bei Abschluss der gen. Operation */
         else
         { /* Unterbrechung wahrend einer gen. Operation */
            next_indiv  = akt_indiv;
            next_gen_op = akt_gen_op;
         } /* Unterbrechung wahrend einer gen. Operation */
      } /* Unterbrechung durch Taste */
      else
      { /* Keine Unterbrechung */ 
         akt_indiv++;                              /* Naechstes Individuum    */
         akt_gen_op = 0;                           /* 1. genetische Operation */
         intrpt_at_gen_end = FALSE;
      } /* Keine Unterbrechung */ 
   } /* Paarbildung eines Individuums */

   if (bestJobNote < gen_note)
      bestJobNote = gen_note;     /* Erfassung der besten Fitness des OptJobs */
  #ifndef VISU_MODE
   if (!taste || intrpt_at_gen_end)
   { /* Generation normal oder mit Unterbrechung bei letztem Indiv beendet */
  #endif /* no VISU_MODE */
      if (accepted)
      { /* Es wurde mindestens ein Kind der Generation akzeptiert */
         no_accept_gen_counter = 0;
         if (accept_improved)
            no_deme_impr_gen_counter = 0;
         else
            no_deme_impr_gen_counter++;
      } /* Es wurde mindestens ein Kind der Generation akzeptiert */
      else
      { /* Es wurde kein Kind der Generation akzeptiert */
         no_accept_gen_counter++;
         no_deme_impr_gen_counter++;
      } /* Es wurde kein Kind der Generation akzeptiert */
  #ifndef VISU_MODE
   } /* Generation normal oder mit Unterbrechung bei letztem Indiv beendet */
  #endif /* no VISU_MODE */
#endif /* keine MPI_PAR_VERS */

   return (!taste);
} /* evo_1gen */



/* ========================== pruef_und_speichere =========================== */
BOOL pruef_und_speichere (CHAIN_ELEM_PTR_TYPE *c_ptr, 
                          INT                  mut_index)
{
   DOUBLE  ketten_wert;
   BOOL    akzeptiere = FALSE,
           lamarck,
           teil2      = TRUE;
  
   /* pruef_und_speichere */
   if (repair_1_chain (c_ptr, &err_state))         /* Loescht unplausible AKs */
   { /* Plausible Kette */
#ifdef MIT_LSV
      if (lsv_all_children)
      { /* Verbesserung aller Nachkommen */
         if (gleam_adi)
         { /* ASMA/all oder AMMA/all (Gorba-HyGLEAM: Fall D) */
            teil2 = FALSE;
  #ifdef GORBA_GLEAM
	    vorGLSVstart = TRUE;
	    if ((*c_ptr)->u_el.u_xel.u_hdr.chain_status & SIMULATED)
	    { /* AK ist bereits simuliert */
	       ketten_wert = (*c_ptr)->u_el.u_xel.u_hdr.fitness_note;
	       do_appl_sim (simu_erg, *c_ptr, CALC_FITNESS, &err_state);
	    } /* AK ist bereits simuliert */
	    else { /* AK ist unsimuliert */
               err_state |= simulator (&ketten_wert, CALC_FITNESS, *c_ptr);
	       indiv_ctr++;
	    } /* AK ist unsimuliert */
  #else /* kein GORBA_GLEAM */
	    err_state |= simulator (&ketten_wert, CALC_FITNESS, *c_ptr);
	    indiv_ctr++;
  #endif /* kein GORBA_GLEAM */
            if (ketten_wert == KEINE_NOTE) 
                err_state |= delete_chain (c_ptr);      /* unbewertbare Kette */
            else 
            { /* bewertete Kette in "child_ak_list" eintragen */
               if (anz_children == 0)
               { /* ein Erstgeborenes */
                  best_child_note = ketten_wert;
                  best_child_idx  = 0;
                  best_child_ptr  = *c_ptr;  /*damit "evo_1gen" Offsprings sieht*/
               } /* ein Erstgeborenes */
               else /* ein Folgekind */
                  if (best_child_note < ketten_wert)
                  { /* besseres Kind merken */
                     best_child_note = ketten_wert;
                     best_child_idx  = anz_children;
                  } /* besseres Kind merken */
  #ifdef GORBA_GLEAM
	       aktPlanDataToListe (anz_children);
	       resetBelMat ();
  #endif /* kein GORBA_GLEAM */
               child_ak_list[anz_children++] = *c_ptr;      /* Kind eintragen */
            } /* bewertete Kette in "child_ak_list" eintragen */
  #ifdef GORBA_GLEAM
	    vorGLSVstart = FALSE;
  #endif /* kein GORBA_GLEAM */
  #ifdef ADI_TST
            sprintf (buf2, "pruef_und_speichere: adaptive all, anz_children=%d "
                     "best_child_note=%g", anz_children, best_child_note);
  #endif /* ADI_TST */
         } /* ASMA/all oder AMMA/all */
         else
         { /* SMA: AK gemaess "lsv_strat" optimieren und bewerten */
  #ifdef MIT_GORBA_LSVS  /* Gorba-HyGLEAM: Fall C */
	    vorGLSVstart = TRUE;
	    if (do_appl_sim (simu_erg, *c_ptr, CALC_FITNESS, &err_state)) 
            { /* AK wurde erfolgreich simuliert */
	       if (lsv_strat < LSV_RAS)
		  aktPlanDataToSave ();
	       else
		  aktAJobPlanDataToSave ();
	       vorGLSVstart = FALSE;      /* vor "do_lsv_start" notwendig! */
	       resetBelMat ();            /* vor "do_lsv_start" notwendig! */
	       ketten_wert = do_lsv_start (c_ptr, TRUE, &err_state);
	    } /* AK wurde erfolgreich simuliert */
	    else { /* AK nicht simulierbar */
	       resetBelMat ();
	       vorGLSVstart = FALSE;
	       ketten_wert  = KEINE_NOTE;
	    } /* AK nicht simulierbar */
  #else /* kein MIT_GORBA_LSVS */
	    lamarck     = check_for_lamarck (lamarck_rate);
            ketten_wert = do_lsv_start (c_ptr, lamarck, &err_state);
  #endif /* kein MIT_GORBA_LSVS */
  #ifdef ADI_TST
            sprintf (buf2, "pruef_und_speichere: old all");
  #endif /* ADI_TST */
         } /* SMA: AK gemaess "lsv_strat" optimieren und bewerten */
      } /* Verbesserung aller Nachkommen */
      else
#endif /* MIT_LSV */
      { /* Verbesserung des besten Nachkommen: AK bewerten (Gorba-HyGLEAM: Fall B) */
#ifdef GORBA_GLEAM
  #ifdef MIT_GORBA_LSVS
	 vorGLSVstart = !gleamPur;
  #endif /* MIT_GORBA_LSVS */
         if ((*c_ptr)->u_el.u_xel.u_hdr.chain_status & SIMULATED)
	 { /* AK ist bereits simuliert */
	    ketten_wert = (*c_ptr)->u_el.u_xel.u_hdr.fitness_note;
  #ifdef MIT_GORBA_LSVS
	    if (!gleamPur)
	       do_appl_sim (simu_erg, *c_ptr, CALC_FITNESS, &err_state);
  #endif /* MIT_GORBA_LSVS */
	 } /* AK ist bereits simuliert */
	 else { /* AK ist unsimuliert */
	    err_state |= simulator (&ketten_wert, CALC_FITNESS, *c_ptr);
	    indiv_ctr++;
	 } /* AK ist unsimuliert */
#else /* kein GORBA_GLEAM */
	 err_state |= simulator (&ketten_wert, CALC_FITNESS, *c_ptr);
         indiv_ctr++;
#endif /* kein GORBA_GLEAM */
#ifdef ADI_TST
         sprintf (buf2, "pruef_und_speichere: best");
#endif /* ADI_TST */
      } /* Verbesserung des besten Nachkommen: AK bewerten */

#ifdef ADI_TST
      sprintf (buf1, "%s teil2=%d", buf2, teil2);
      write_to_log (buf1, CLOSE_LOG);
#endif /* ADI_TST */
      if (teil2) 
      { /* GLEAM, SMA/all, SMA/best und AxMA/best */
         if (ketten_wert == KEINE_NOTE) 
            err_state |= delete_chain (c_ptr);          /* unbewertbare Kette */
         else 
         { /* bewertete Kette evtl. eintragen */
            if (best_child_ptr == NULL) 
               akzeptiere = TRUE;                               /* Neues Kind */
            else 
               if (ketten_wert > best_child_note) 
               { /* Besseres Kind ersetzt altes Kind */
                  err_state  |= delete_chain (&best_child_ptr);
                  akzeptiere  = TRUE;
               } /* Besseres Kind ersetzt altes Kind */
               else 
                  err_state = delete_chain (c_ptr); /*vernichte schlechteres Kind*/
            if (akzeptiere)
            { /* Akzeptiere Kind */
               best_child_ptr  = *c_ptr;
               best_child_note = ketten_wert;
#ifdef MIT_GORBA_LSVS
	       if (lsv_best_child) 
		  aktPlanDataToSave ();              /* d.h. nur bei xMA/best */
#endif /* MIT_GORBA_LSVS */
#ifdef MIT_OP_STATISTIK
               mut_op = mut_index;
#endif /* MIT_OP_STATISTIK */
            } /* Akzeptiere Kind */
         } /* bewertete Kette evtl. eintragen */
#ifdef MIT_GORBA_LSVS
	 if (lsv_best_child)                         /* d.h. nur bei xMA/best */
	    resetBelMat ();
	 vorGLSVstart = FALSE;
#endif /* MIT_GORBA_LSVS */
      } /* GLEAM, SMA/all, SMA/best und AxMA/best */
   } /* Plausible Kette */
   return (err_state == GLEAM_OK);
} /* pruef_und_speichere */



#ifdef MIT_LSV
/* ================================= do_lsv ================================= */
BOOL do_lsv (BOOL  lamarck,
             INT   znr)
{
   STR_TYPE  buf1, buf2;
   DOUBLE    alte_note,                /* Note der Original-AK                */
             start_pop_note;           /* Beste Note der Startpopulation      */
   INT       chrLen;                   /* Laenge des Chromosoms               */
   BOOL      lsv_ok;                   /* LSV lief fehlerfrei                 */
   
   /* do_lsv */
   alte_note = pop_feld[0].note;
   if (alte_note < zielnote) 
   { /* Zielnote ist noch nicht erreicht: LSV anwenden */
  #ifdef MIT_GORBA_LSVS   /* Gorba-HyGLEAM: Fall A */
      vorGLSVstart = TRUE;
      if (do_appl_sim (simu_erg, pop_feld[0].chain_ptr, CALC_FITNESS, &err_state)) 
       { /* AK wurde erfolgreich simuliert */
	  if (lsv_strat < LSV_RAS)
	     aktPlanDataToSave ();
	  else
	     aktAJobPlanDataToSave ();
	  resetBelMat ();                  /* vor "do_lsv_start" notwendig! */
	  vorGLSVstart = FALSE;            /* vor "do_lsv_start" notwendig! */
	  neue_lsv_note = do_lsv_start (&(pop_feld[0].chain_ptr), TRUE, &err_state);
       } /* AK wurde erfolgreich simuliert */
      else { /* AK nicht simulierbar */
	 resetBelMat ();
	 vorGLSVstart  = FALSE;
	 neue_lsv_note = KEINE_NOTE;
      } /* AK nicht simulierbar */
  #else /* kein MIT_GORBA_LSVS */
      neue_lsv_note = do_lsv_start (&(pop_feld[0].chain_ptr), lamarck, &err_state);
  #endif /* kein MIT_GORBA_LSVS */
   } /* Zielnote ist noch nicht erreicht: LSV anwenden */
   else
      neue_lsv_note = alte_note;

   /* ----------- Nachbehandlung der verbesserten AK im pop_feld: ----------- */
   lsv_ok = neue_lsv_note != KEINE_NOTE;
   chrLen = pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.chain_length;
   if (pop_feld[0].new_chain)
   { /* neugenerierte AK */               /* AK-Note und AK-Aktionen muessen  */
      if (lamarck && lsv_ok)              /* uebereinstimmen. Daher LSV-Note  */
      { /* Durch LSV veraenderte AK */    /* nur in AK, wenn lamarck==TRUE    */
         pop_feld[0].note = neue_lsv_note;
         berechne_guete (&(pop_feld[0].guete), neue_lsv_note);
         if (znr > 0)
            if (neue_lsv_note > alte_note)      /* Ausgaben nur, wenn 1 Indiv */
               write_buf (znr, AK_OPT_STORED);  /* lokal optimiert wird.      */
            else
               write_buf (znr, ORG_AK_STORED);
      } /* Durch LSV veraenderte AK */
      else
      { /* unveraenderte AK: Original-Bewertung wieder herstellen */
         err_state |= simulator (&alte_note, CALC_FITNESS, 
                                 pop_feld[0].chain_ptr);
         if (znr > 0)      /* Ausgaben nur, wenn 1 Indiv lokal optimiert wird */
            write_buf (znr, ORG_AK_STORED);
      } /* unveraenderte AK: Original-Bewertung wieder herstellen */
   } /* neugenerierte AK */
   else
   { /* AK stammt aus AK-Mem (Alt-AK) */
      if (lsv_ok && (neue_lsv_note > alte_note) && lamarck)
         behandle_neue_note (pop_feld[0].chain_ptr, znr, neue_lsv_note);  /* verbesserte Alt-AK updaten */
      else
      { /* Alt-AK bleibt unveraendert */
         pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.fitness_note = alte_note;
         if (znr > 0)      /* Ausgaben nur, wenn 1 Indiv lokal optimiert wird */
            write_buf (znr, AK_UNVERAENDERT);
      } /* Alt-AK bleibt unveraendert */
      store_in_evo_erg (pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.lfd_nr,
                        pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.guete,
                        pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.fitness_note, AK);
      pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.ref_counter = 1;   /* AK ist be-*/
      pop_feld[0].new_chain = FALSE;          /* reits im Mem. Nicht nochmal! */
      pop_feld[0].guete     = KEINE_GUETE;                   /* AK im         */
      pop_feld[0].note      = KEINE_NOTE;                    /* "pop_feld"    */
      pop_feld[0].chain_ptr = NULL;                          /* loeschen      */
   } /* AK stammt aus AK-Mem (Alt-AK) */
   if ((lsv_ok) && (neue_lsv_note > gen_note)) {
      gen_note = neue_lsv_note;
      gen_len  = chrLen;
   }
   return (lsv_ok);
} /* do_lsv */
#endif /* MIT_LSV */



/* =================================== allocChrList =================================== */
BOOL allocChrList ()
{
   return (TRUE);
} /* allocChrList */



/* ================================ clearInitialChrList =============================== */
void clearInitialChrList (void)
{
} /* clearInitialChrList */



/* =================================== clearChrList =================================== */
BOOL clearChrList (BOOL  delChr)
{
   return (TRUE);
} /* clearChrList */


/* ================================= evaluateStartPop ================================= */
BOOL evaluateStartPop (void)
{
   return (TRUE);
} /* evaluateStartPop */
