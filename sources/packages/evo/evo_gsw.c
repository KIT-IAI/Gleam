/*******************************************************************************
HyGLEAM                                                                   V1.3.0

                                 H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: EVOlutions-GrundSoftWare
 
Package: evo                    File: evo_gsw.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul realisiert zusammen mit "evo" die Evolution einer Generation ("evo_
_1gen") einer Population entsprechend dem Nachbarschaftsmodell. Dabei ist die
kleinste Verarbeitungseinheit, die nicht unterbrochen werden kann, eine Paarung.

Der Modul entstand als Ausgliederung der meisten Routinen aus "evo", um von 
"evo" eine Variante fuer die parallele Simulation anlegen zu koennen.

**Veraltet:
**Die Unterbrechung erfolgt in der Einknotenversion (Compilerschalter PAR_VERS 
**nicht gesetzt) per Tastendruck. Die globale Variable "taste" erhaelt den Wert
**TRUE, um dies nach aussen anzuzeigen. In der Parallelversion (Compilerschalter 
**PAR_VERS gesetzt) erhaelt das Programm eine Nachricht und teilt dem Package 
**durch Setzen von "taste" den Unterbrechungswunsch mit.

Verwaltung der Hammingabstaende:
Die Hammingabstaende der Mitglieder eines Demes werden gespeichert und gepflegt, 
da sie darueber entscheiden, ob ein Crossover stattfindet oder nicht. Die Ver-
waltung der Hammingabstaende erfolgt derart, dass jeder Abstand nur einmal 
gespeichert wird. Dazu enthaelt der Eintrag eines Individuums im "pop_feld" das 
Feld "deme_ham", dass die Abstaende des Individumms zu den "anz_nb_halbe" Nach-
barn mit geringerem Index im "pop_feld" (Vorgaenger) enthaelt. Die Abstaende zu 
den Nachfolgern stehen in den "deme_ham"-Feldern eben dieser Nachfolger.
Beispiel mit anz_nb_halbe = 3:

  "deme_ham"-Felder
  der Individuen 5 bis 8
  +---+---+---+
5 | 4 | 3 | 2 |    Hammingabstaende zu den Individuen 4, 3, 2 (Vorgaenger)
  +---+---+---+
6 | 5 |   |   |    Hammingabstand zwischen den Individuen 5 und 6 (Nachfolger)
  +---+---+---+
7 |   | 5 |   |    Hammingabstand zwischen den Individuen 5 und 7 (Nachfolger)
  +---+---+---+
8 |   |   | 5 |    Hammingabstand zwischen den Individuen 5 und 8 (Nachfolger)
  +---+---+---+


Die Nachbarschaft eines Individuums steht im Feld "nachbarn" im  Eintrag eines 
Individuums im "pop_feld". Das Feld enthaelt Pointer auf die entsprechenden 
"pop_feld"-Eintraege der Nachbarn. Da das Feld "nachbarn" von "partnerwahl"
permanent fuer das Ranking umsortiert wird, kann vom Feldindex nicht auf die 
Distanz zum Nachbarn geschlossen werden.


Die Prozedur "init_ep" erzeugt die Anfangspopulation Gen0 entsprechend einer 
vorgegebenen Strategie. Diese erlauben einen voelligen Neustart der Evolution 
oder aber die Einbeziehung von Vorwissen mittels frueher generierter Aktions-
ketten. Alle Strategien, die auf vorhandene AKs zurueckgreifen, mischen die
erzeugte Population:

  NEU:       Alle Individuen werden alle neu ausgewuerfelt, wobei "strat_par" 
             Individuen eine Note besser als Null haben muessen.
             Anmerkung: Dieser Strategieparameter wird derzeit nicht/kaum genutzt
                        und auf null gesetzt.
  BEST:      Generiere Anfangspopulation aus den besten vorhandenen Ketten der 
             Gueteklassen 2 bis MAX_GUETE unter Vermeidung von Mehrfachverwen-
             dungen. Wenn das nicht ausreicht, wird der Rest der Population mit 
             "gen0_neu" zufaellig initialisiert. 
  MIX:       Generiere Anfangspopulation aus zufaellig gewaehlten Ketten, deren 
             Note besser ist als "strat_par" unter Vermeidung von Mehrfachver-
             wendungen. Wenn die Anzahl vorhandener Ketten dazu nicht ausreicht,
             wir der Rest neu ausgewuerfelt.
  BEST_NEU:  Generiere die Anfangspopulation aus den "strat_par" besten AKs und
             fuelle den Rest mit zufaellig Erzeugten durch "gen0_neu" auf. Die 
             besten AKs werden gueteklassen-weise beginnend mit der hoechsten 
             Klasse genommen. Enthaelt dabei die letzte (d.h. schlechteste) 
             Klasse mehr Ketten, als benoetigt werden, werden von diesen nur die 
             Besten unter Vermeidung von Mehrfachverwendungen ausgewaehlt. 
  FROM_FILE: Initialisierung der Anfangspopulation mit den Ketten des EVO_
             SCRATCH_FILEs, wobei das File ausreichend viele Aks enthalten muss.
  LSV_NEU:   Initialisierung der Anfangspopulation durch "strat_par" zufaellig
             erzeugte und mit dem LSV des Jobs ("lsv_strategy") optimierter AKs. 
             Der Rest wird zufaellig erzeugt.
  LSV_BEST:  Initialisierung der Anfangspopulation gemaess BEST_NEU und optimie-
             re alle mit dem LSV des Jobs ("lsv_strategy").


Exportierte Routinen:
=====================

void   loesche_evo_erg  (void);
          Initialisiert das Evo-Ergebnisfeld in dem Adresse und Note der MAX_
          EVO_ERG letzten ErgebnisKetten gespeichert werden.

void   dumpPopFeld (const char *buf,
	 	    INT         popSize);
          Erzeugt eine Liste der Laenge "popSize" mit einigen Daten aus 
          "pop_feld".

DOUBLE getHamDist (INT  indivIdx,
                   INT  partnerIdx);
          Liefert den Hamming-Abstand zwischen den durch "indivIdx" und
          "partnerIdx" bezeichneten Individuen der aktuellen Population ab. Da-
          bei wird vorausgesetzt, dass sich die Individuen im gleichen Deme be-
          finden. Der Abstand wird bei Bedarf berechnet.
          Bei fehlerhaften Indices wird "NO_HAM" abgeliefert.


Package-lokale Routinen, die in der GUI-Version exportiert werden:
==================================================================

GSTATE del_ep (void);
          Loescht die Entwicklungspopulation und die Hammingabstaende.
 
GSTATE keep_result_chain (INT  index, 
                          INT  ketten_art);
          Speichert die Kette des durch "index" bezeichneten Individuums ab und 
          loescht sie aus der aktuellen Population, sofern sie veraendert und 
          noch nicht abgespeichert wurde. Liefert GLEAM_OK, wenn beim Speichern
          alles gut ging. In der ersten Zeile des Textpuffers steht das 
          Ergebnis (Nichts gespeichert oder Kettenadresse).

INT    get_guete (DOUBLE *note,
                  INT     index);
          Liefert Guete u. "note" des durch "index" bezeichneten Individuums ab.
          Wenn es fuer "index" kein Individuum gibt oder "index" groesser als
          die aktuelle Population ist, liefert die Funktion KEINE_GUETE ab.


Package-lokale Routinen, die in der MPI- und/oder der PAR_POP-Parallelversion exportiert werden:
================================================================================================

BOOL   init_ep (DOUBLE *best_note,
                INT     akt_pop_size,
                LONG    s_par);
          Initialisierung der Entwicklungspopulation je nach "init_strategy" aus
          dem EVO_SCRATCH_FILE oder durch Auswuerfeln und/oder Benutzung von AKs
          aus dem AK-Speicher und Aufbau der statischen Daten der Nachbarschaft.
          Das Funktionsergebnis ist TRUE, wenn alles gut ging. In diesem Fall 
          enthaelt "best_note" die Note der besten AK der Startpopulation.
          Wenn "from_file" gesetzt ist, wird die Entwicklungspopulation aus dem
          EVO_SCRATCH_FILE initialisiert, wobei "akt_pop_size" AKs erwartet wer-
          den. Bei fehlerhafter File-Initialisierung eine Fehlermeldung erzeugt. 
          "init_strategy", "s_par" und die LSV-Parameter haben keine Bedeutung.
          Wenn "from_file" nicht gesetzt ist, wird die Entwicklungspopulation 
          mit "akt_pop_size" Individuen mit der durch "init_strategy" vorgegebe-
          nen Strategie initialisiert. Sind noch Individuen in der Entwicklungs-
          population vorhanden, so werden diese geloescht. Die LSV-Parameter 
          "lsv_strat" und "lamarck_rate" spielen nur bei einer LSV-Voroptimie-
          rung der Population ("init_strategy" LSV_NEU und LSV_BEST) eine Rolle.
          Wenn bei der Initialisierung nicht genuegend Speicher vorhanden ist
          oder ein (fataler) Fehler auftritt, wird die Routine abgebrochen und
          die globalen Variablen "mem_out" und "err_state" werden entsprechend
          gesetzt.

void   save_ep (void);
          Oeffnet das EVO_SCRATCH_FILE und rettet alle Ketten der Entwicklungs-
          population in das File. Im Fehlerfall wird eine Meldung ausgegeben.


Lokale Routinen, die in der MPI-Parallelversion exportiert werden:
==================================================================

BOOL ch_gen (INT  indivIdx);
          IF (EXT_SIMU_SERV)
            Erzeugt ein plausibles Chromosom und traegt es in "pop_feld[indivIdx].
            chain_ptr" ein. Da die Bewertung erst nach Erstellung der Startpopu-
            lation erfolgen kann, wird als Fitnesswert 0.0 eingetragen. Die 
            Anzahl erzeugter Ketten wird in "gut_anz" mitgezaehlt, da die Ein-
            haltung einer Mindestnote im Kontext der externen Simulationsdienste 
            nicht moeglich ist. 
          ELSE
            Erzeugt eine Kette, deren Note mindestens gleich "mindest_note" ist 
            und traegt sie in "pop_feld[indivIdx].chain_ptr" ein. Die Note wird 
            in "pop_feld[indivIdx].note" eingetragen.
            Die Anzahl erzeugter Ketten mit einer Note besser als "mindest_note" 
            wird in "gut_anz" mitgezaehlt. 
          FI
          Wenn alles gut ging, ist das Funktionsergebnis TRUE, sonst FALSE und 
          weitere Fehlerinformationen in "mem_out" und "err_state".
          Bei unbewertbaren AKs oder permanent unplausiblen (z.B. wegen fehler-
          hafter Definition des Handlungsmodells) wird nach MAX_POPEL_SIZE Gene-  
          rierungsversuchen mit einer Fehlermeldung abgebrochen. 


Im Kontext der MPI-Parallelversion implementierte und exportierte Routinen:
===========================================================================

void initSubPop (INT     pSize,
                 INT     optVerf,
		 INT     survivalRule,
		 INT     lsvCode,
		 DOUBLE  rankParam,
		 BOOL    mitAllVerb);
          Nutzt die Parameter fuer Initialisierungen, die mit denen von 
          "do_1_evo_job" vergleichbar sind.


Im Kontext der MPI-Parallelversion implementierte und package-lokale Routinen:
==============================================================================

void   loe_deme_ham_werte (INT  indiv);
          Loescht die Hamming-Werte des Demes des Individuum "indiv" fuer die
          symmetrische Ringnachbarschaft. 
          Das Pendant fuer die MPI-Version steht in "mPar_gsw".


Package-lokale Routinen:
========================

BOOL   do_g_operationen (void);
          Fuehrt die genetischen Operationen fuer eine Paarung durch und lie-
          fert TRUE ab wenn, alles gut ging. Sonst FALSE und weitere Fehler-
          informationen in "mem_out" und "err_state". 
          Dabei wird mit folgenden modul-lokalen Variablen gearbeitet: 
          "akt_ptr" bezeichnet das aktuelle Individuum, "partner_ptr" seinen 
          Partner und "child_ptr" sowie "child2_ptr" die Offsprings. "do_xo" 
          steuert, ob der Hammingabstand Rekombination bzw. Crossover erlaubt.
 
GSTATE store_res_ch (INT *lfd_nr,
                     INT  idx,
                     INT  ketten_art);
          Rettet die Kette mit dem Index "idx" im "pop_feld", sofern "new_chain"
          gesetzt ist und zaehlt sie mit. 
          Liefert die "lfd_nr" im Kettenspeicher "ketten_art" ab, unter der die 
          AK gespeichert wurde. Funktionsergebnis ist GLEAM_OK, wenn alles gut 
          ging.

INT    get_best (INT     anz_erg_ketten,
                 BOOL    mit_anzg,
                 GSTATE *ret_state);
          Es werden maximal "anz_erg_ketten" unveranderte Ketten gerettet. Dazu
          wird versucht, moeglichst unterschiedliche Ketten aus verschiedenen 
          Nischen auszuwaehlen. Alle abgelieferten und alle unveraenderten Ket-
          ten werden am Schluss geloescht. Die Anzahl geretteter Ketten wird als
          Funktionsergebnis abgeliefert. Auftretende Fehler werden in "ret_
          state" zurueckgemeldet. Bei gesetztem "mit_anzg" werden pro geretteter
          AK Noten- und Adressiformationen im Textpbuffer gespeichert. "mit_anzg"
          darf nur bei Aufruf der Routine aus dem Menue heraus gesetzt werden! 
          Die Routine arbeitet auch auf teilweise bereits "abgeernteten" Ent-
          wicklungspopulationen.

void   store_in_evo_erg (INT     number, 
                         INT     guete,  
                         DOUBLE  note,
                         INT     ketten_art)
          Speichert die Ergebnisdaten einer Population ("note" und Adresse 
          ("guete", "number") im Feld "last_evo_erg". Das feld enthaelt jeweils 
          die Daten der MAX_EVO_ERG letzten ErgebnisKetten.


BOOL   check_for_lamarck (DOUBLE  lamarck_rate);
          Wuerfelt ausgehend von der "lamarck_rate" aus, ob Lamarcksche Evolu-
          tion stattfindet oder nicht und liefert das Ergebnis ab.

DOUBLE *partnerwahl (INT     no,
                     INT    *partnerIdx,
                     DOUBLE *weakest_note,
                     DOUBLE *best_note);
          Sortiert die Nachbarschaft des Populationsmitgliedes "no" und waehlt 
          einen Partner mittels linear rank selection, dessen Index in "partnerIdx" 
          abgeliefert wird. Zusaetzlich werden die Fitnesswerte des lokal schlech-
          testen und besten Individuums bestimmt ("weakest_note" und "best_note"). 
          Schliesslich wird die Adresse des im "pop_feld" gepeichert Hammingabstands  
          zum Partnerchromosom ermittelt und als Funktionsergebnis abgeliefert.

void   clear_mut_op_ctrs (void);
          Loescht die Zaehler im Feld "mut_op_ctr", die operatorbezogen die 
          Anzahl akzeptierter Offsprings zaehlen.

void   meld_evo_crash (INT         meld_nr,
                       const char *mod_id,
                       LONG        info);
          Erzeugt mit Hilfe von "meld_nr" und "mod_id" eine Meldung mit Time-
          Stampzum Abbruch der Evolution und loescht ein eventuell vorhandenes 
          bestes Kind. 

BOOL   externalTermRequest (void);
          Prueft bei gesetztem "with_evo_term_tst" die Existenz des Files 
          EVO_ABBRUCH_FSPEC und setzt positivenfalls die jeweiligen Schleifen-
          kontrollvariablen der Generations- und der Populationsschleife so, 
          dass die Engine abbricht. Bei Abbruch wird eine Meldung ins Logfile
          geschrieben und TRUE zurueckgeliefert. Andernfalls FALSE.

BOOL   evo_init (void);
          Die Modulinitialisierung ist beim Programmstart aufzurufen. Sie lie-
          fert TRUE, wenn die lokalen Datenstrukturen angelegt werden konnten.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0: 
 1. Korrektur einiger Fehlermeldungsaufrufe wg. Umstellung in fbhm.  WJ 13.08.08
 2. "genBestSimuLiIdx" ersetzt "parentIdx" wegen "mlOnlineVisu".     WJ 20.08.08
 3. "gen0_lsv": Vorbereitende Simulation vor LSV-Start bei gesetz-
    tem MIT_GORBA_LSVS. Tuning von "check_for_lamarck".              WJ 20.11.09
 4. Bei gesetztem MIT_GORBA_LSVS ist "def_lamarck_rate" immer 1.0.   WJ 27.11.09
 5. "gen0_lsv": Bei RAS-LSV Aufruf von "aktAJobPlanDataToSave" 
    anstelle von "aktPlanDataToSave".                                WJ 28.01.10
 6. "get_ham_ptr" etwas optimiert. Verwaltung der Hammingabstaende 
    dokumentiert. Testausgaben in "partnerwahl" verbessert.          WJ 16.12.11
 7. "meld_evo_crash": Deklaration von "mod_id", als "const".         WJ 20.04.12
 8. Entfernung von PAR_SIMU und GLEAM_TCL sowie dem dazugehoerigen
    Code und von "genBestSimuLiIdx". Entfernung ueberfluessiger 
    Variable.                                                        WJ 28.06.12
 9. Verlagerung von "ch_gen" als lokale Routine hierher.
    Umstellungen fuer die MPI-Parallelversion: Verlagerung des 
    Loeschens der Hammingabstaende von "init_ep" nach "del_ep". 
    "pop_feld" und "init_strat" sind in der MPI-Version Package-Export. 
    Integration der Funktionalitaet von "get_ham_ptr" in "partner_
    wahl". Ungenutztes "get_ham_dist" geloescht. 
    "anz_nb_halbe" wird Package-Export.
    In der Parallelversion gilt: "dumpPopFeld" wird Export.
      "init_ep": keine Generierung neuer AKs. 
      "ch_gen" ist Package-Export.                                   WJ 16.11.12
10. "do_g_operationen": Deaktivierung der Unterbrechbarkeit, wegen 
     moeglichem Widerspruch mit der dann wiederholten Partnerwahl.   WJ 20.11.14
11. GUI-Variante: Einfuehrung einer neuen Funktion "getHamDist", 
    Funktion "keep_result_chain" und Variable "akt_indiv" werden 
    exportiert.                                                      WJ 25.11.14
12. GUI-Variante: Die bisher package-globalen Variablen 
    "def_xo_min_ham_dist", "def_lamarck_rate", "def_opt_strat" und
    "def_lsv_strat" werden Package-Export.                           WJ 27.08.15
13. Renovierung: Streichung des "link"-Includes. Ersetzung der 
    testweisen "write_line"- durch "printf"-Aufrufe.                 WJ 04.05.16
14. Uebernahme von "initZufGen" von "mPar" / "engine_gsw".           WJ 14.07.16
15. Die Variablen "def_xo_min_ham_dist", "def_lamarck_rate", 
    "def_opt_strat" und "def_lsv_strat" werden Package-Export.       WJ 10.11.16
16. Uebernahme von "oldDistribFrac" von "adapt_di" und weiterer Vari-
    ablen, damit alle fuer TSK-Daten benoetigten Defaults und Zuwei-
    sungsmoeglichkeiten zur Verfuegung stehen.                       WJ 25.11.16
17. Integration der neuen TSK-Datenverwaltung. Neue Defaultvariable 
    fuer entsprechende TSK-Daten. Anpassung des "write_to_log"-
    Aufrufs.                                                         WJ 19.12.16
20. Einfuehrung von "with_evo_term_tst" zum effizienteren Job-
    Abbruchtest der Commandlineversion.                              WJ 19.01.17
21. Fehlermeldung NOT_ENOUGH_MEM korrigiert. Reunion of "evoIntg.h" 
    and "evoInts.h" in "evoInt.h".                                   WJ 30.06.17
22. Uebernahme allen Exports aus "evo.c", der nicht spezifisch fuer
    diese Implementierung ("sequentiel und MPI_PAR_VERS) ist. An-
    passung von ch_gen() und init_ep() an EXT_SIMU_SERV. Streichung 
    des "new_ch"-Parameters von "put_ch". Neustrukturierung des 
    Moduls. "indiv_ctr" zaehlt auch StartPopInit inkl. LSVs mit.     WJ 13.07.17
23. ExtSimuServ: "no_key" wird gestrichen und "taste an Modul 
    "sys_gsw" abgegeben. Vereinheitlichung der Routinen "clear_evo_
    intrpt" mit "clear_evo_job_intrpt" aus Modul "evo_steu.c" zu 
    "clearEvoJobIntrpt" in Modul "evo_gsw.c".                        WJ 06.09.17
24. "del_ep" loescht auch die Chromosomenliste bei ext.SimuDiensten. WJ 13.11.17
25. "anz_children" wird zu Beginn von "do_g_operationen" unabhaengig
    von Schaltern wie MIT_LSV oder EXT_MATLAB_SIMU zurueckgesetzt.   WJ 01.12.17
26. Neuer Include von GLOBAL_TEXTS.                                  WJ 10.01.18
27. Korrektur dreier Fehlermeldungen. Anpassung des Exports an den 
    neuen VISU_MODE.                                                 WJ 23.03.18
20. Bei der Chromosomenerzeugung wird die Laenge des besten Chromo-
    soms in "gen_len" gespeichert. PAR_POP_VERS: neue Variable 
    "selectedChrAmount" und "prepareForEvo", wobei letztere nur beim 
    initializer() FALSE ist. init_ep() erzeugt bei Rueckgabe von 
    FALSE eine Fehlermeldung. Korrektur am Returnwert von 
    gen0_mix(). Das EVO_SCRATCH_FILE wird in init_from_file() und 
    save_ep() bei gesetztem PAR_POP_VERS als Textfile geoeffnet und 
    sonst als Binaerfile. Bei Fehlerfreiheit erzeugt init_from_file() 
    einen Ergebniseintrag im Textpuffer.                             WJ 27.06.18
21. Die neue Funktion gen0_NBest() waehlt wirklich die besten Chromo-
    some aus und ersetzt gen0_best() und gen0_best_neu(). init_ep() 
    um Initialisierungsstrategie ONE_CHR fuer LSV-Jobs erweitert.    WJ 09.07.18
22. Streichung von "evo_fin_fspec" und stattdessen die direkte 
    Verwendung von EVO_ABBRUCH_FSPEC.                                WJ 21.08.18
23. "gen0_NBest": Liefert bei nicht gesetztem "prepareForEvo" die 
    Anzahl ausgewaehlter Chromosome in "selectedChrAmount" ab.       WJ 25.01.19
24. Avoiding buffer overflow of static strings: keep_result_chain(). WJ 19.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 19.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "chio.h"
#include "hmod.h"
#include "aufg.h"
#include "appl.h"
#include "simu.h"
#ifdef MIT_GORBA_LSVS
  #include "bew.h"
  #include "xml.h"
  #include "gsim.h"
  #include "glsv.h"
#endif /* MIT_GORBA_LSVS */
#ifdef MPI_PAR_VERS
  #include "mPar.h"
#endif /* MPI_PAR_VERS */
#include "evo.h"
#include "evoInt.h"

#include GLOBAL_TEXTS
#include EVO_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
  #undef DEME_TST
#endif /* GLO_CHECKS */

#undef GEN0_DBG




/* ========================== Exportierte Variable: ========================= */
EVO_ERG_TYP *last_evo_erg;         /* Ergebnisdaten der letzten EvoJobs       */
LONG         indiv_ctr,            /* Zaehlt d.Funtionsevaluationen inkl.Init */
             evalLimit;            /* Vorgabe eines Evaluationsbudgets        */
DOUBLE       xo_min_ham_dist,      /* akt.min.Hamming-Abst. fuer Reko/XO      */
             def_max_fit,          /* Default f. Ranking-Param "max_fit"      */
             def_xo_min_ham_dist,  /* Default min.Hamming-Abst. fuer Reko/XO  */
             defTargetFitness,     /* default for target fitness of a job     */
             def_lamarck_rate,     /* Default f.Lamarck-Param. lamarck_rate   */
             oldDistribFrac;       /* Anteil d.alten LSV-Verteilung an d.neuen*/
INT          evo_tst_frequ,        /* Frequenz der EvoAbbruchtests bei Jobvers*/
             anz_nachbarn,         /* aktuelle Nachbarschaftsgroesse          */
             anz_nb_halbe,         /* Haelfte von "anz_nachbarn"              */
             default_survival_rule,/* Default-Ueberlebensregel                */
             defInitStrat,         /* default strategy for start population   */
             defInitStratPar,      /* default for init. strategy parameter    */
             def_psize,            /* Default-Populationsgroesse              */
             defGenLimit,          /* default for generation limit            */
             defGAcLimit,          /* default for limit of gen. w/o acceptance*/
             defGDI_Limit,         /* default for limit of gen. w/o deme impr.*/
             defTimeLimit,         /* default for job time limit in seconds   */
             defEvalLimit,         /* default for limit of evaluations        */
             defResultChrs,        /* default number of result chromosomes    */
             def_opt_strat,        /* Default-Optimierungs-Verfahren          */
             def_lsv_strat,        /* Default-LSV solo und mit EVO            */
             save_rate_def,        /* Defaultwert fuer die Evo-Save-Rate      */
             selectedChrAmount,    /* PAR_POP_VERS: #initially selected chr.  */
             gen_counter;          /* Generationszaehler                      */
BOOL         with_evo_term_tst,    /* TRUE: Mit EvoAbbruchtests der Engine    */
             initZufGen,           /* TRUE: Init. des Zufallszahlengenerators */
             withAllImprovement,   /* TRUE: LHC-opt for all offspring         */
             prepareForEvo,        /* TRUE if not PAR_POP_VERS initializer    */
             mem_out = FALSE;      /* TRUE = Abbruch wegen Speichermangel     */

/* ---- Package-global in der sequentiellen Version und global in der  ------ */
/* ---- MPI-Version:                                                   ------ */
INDIVID_TYPE *pop_feld;            /* enthaelt die Population                 */
INT           init_strat,          /* Initialisierungs-Strategie              */
              max_nb_index;        /* Groesster Index: "anz_nachbarn"-1       */

/* ---- Package-global in der sequentiellen Version und global in der  ------ */
/* ---- MPI-Version ODER beim GUI- oder VISU-Mode:                     ------ */
INT           akt_indiv;           /* "pop_feld"-Index des akt. Individuums   */



/* ======================== Package-globale Variable: ======================= */
CHAIN_ELEM_PTR_TYPE  akt_ptr,         /* aktuelles Individuum                 */
                     best_child_ptr,  /* bester Nachkomme pro Paarung         */
                     partner_ptr,     /* Partner des aktuellen Individuums    */
                     child_ptr,       /* 1.Offspring (Mutationen od. XO/Reko) */
                     child2_ptr;      /* 2.Offspring (nur XO/Reko)            */
INT           mut_op_ctr [ANZ_PAAR_OPS]; /* Statistik: Zaehlt akzept. Offspr. */
DOUBLE        best_child_note,        /* Note dieses Nachkommens              */
              max_fit,                /* aktueller Ranking Parameter          */
              min_fitness,            /* f.Partnerwahl:  2.0-"max_fit"        */
              fitness_delta,          /*            "max_fit" - "min_fitness" */
              zielnote,               /* Vorgegebene Zielnote                 */
              lamarck_rate;           /* AK-Update-Rate fuer LSV              */
LONG          maxFEs;                 /* Abbruchlimit f. Funktionsberechnungen*/
INT           akt_gen_op,             /* Index der akt. genetischen Operation */
              pop_size,               /* aktuelle Groesse des "pop_feld"      */
              act_opt_strategy,       /* aktuelles Optimierungsverfahren      */
              act_survival_rule,      /* aktuelle Ueberlebensregel            */
              max_pop_index,          /* max.aktueller Index im "pop_feld"    */
              gut_anz,                /* Anz guter AKS in "gen0_neu"          */
              toBeEvalCount,          /* counts newly generated chromosomes   */
              jobInterruptPhase,      /* shows interrupted phase of job exec. */
              mut_op = 1,             /* Init. nur fuer NichtEvo relevant     */
              lsv_strat,              /* LSV-Strategie allein u.mit mit Evo   */
              chrFileIOMode,          /* IO mode of the population chrom.file */
              no_accept_gen_counter,  /* GenAnz ohne OffsprAkzeptanz } bei GUI*/
              no_deme_impr_gen_counter,/*GenAnz ohne DemeVerbesserung} Export */
              anz_children;           /* Anzahl der Nachkommen pro Paarung    */
BOOL          do_xo,                  /* TRUE = HammingAbst erlaubt XO/Reko   */
              intrpt_at_gen_end,      /* TRUE, wenn Taste bei Generations-Ende*/
              lsv_best_child,         /* TRUE, wenn LSV fuer besten Offspring */
              lsv_all_children;       /* TRUE, wenn LSV fuer alle Offsprings  */
#ifdef MIT_LSV
  BOOL          gleamPur,             /* TRUE: reiner GLEAM-Job               */
                gleam_adi,            /* TRUE: GLEAM_ASMA- oder GLEAM_AMMA-Job*/
                gleam_asma,           /* TRUE: GLEAM_ASMA-Job                 */
                gleam_amma;           /* TRUE: GLEAM_AMMA-Job                 */
  CHAIN_ELEM_PTR_TYPE *child_ak_list;   /* Liste der offspring                */
#endif /* MIT_LSV */



/* ============================ Lokale Konstante: =========================== */
#define EVO_SCRATCH_FILE  "evo_tmp.mem"   /* Zwischensicherungen der Evo.     */
#define EGAL              FALSE           /* Werte fuer Parameter "qualitaet" */
#define GUT                TRUE           /* Werte fuer Parameter "qualitaet" */
#define INIT_NEU_LSV      FALSE           /* Fuer "gen0_lsv"                  */
#define INIT_BEST_LSV      TRUE           /* Fuer "gen0_lsv"                  */



/* ============================ Lokale Variable: ============================ */
static LONG     strat_par;              /* Init-Strategieparameter            */
static DOUBLE  *init_best_note,         /* fuer "init_ep"                     */
                mindest_note;           /* MindNote gen.AKs in "gen0_neu"     */



/* ============================ Lokale Routinen: ============================ */
#ifndef MPI_PAR_VERS
  static BOOL  ch_gen          (INT     indivIdx);
#endif /* kein MPI_PAR_VERS */

static void    put_ch          (INT                  pop_index,
                                CHAIN_ELEM_PTR_TYPE  ch_ptr);

static BOOL    init_from_file  (INT     akt_size);

static void    mix_up_popul    (INT     anzAKs);

static BOOL    gen0_neu        (INT     anf,
                                INT     anzahl,
                                INT     local_strat_par,
                                BOOL    qualitaet);

static BOOL    gen0_NBest      (INT     targetAmount);

static BOOL    gen0_mix        (void);

#ifdef MIT_LSV
  static BOOL  gen0_lsv        (BOOL    best);
#endif /* MIT_LSV */

static BOOL    compute_results (CHAIN_ELEM_PTR_TYPE *c1_ptr, 
                                CHAIN_ELEM_PTR_TYPE *c2_ptr, 
                                INT                  mut_index);




/* ============================= loesche_evo_erg ============================ */
void loesche_evo_erg (void)
{
   INT  ii, jj;
 
   /* loesche_evo_erg */ 
   for (jj = 0;  jj < MAX_EVO_TYPEN;  jj++)
   { /* Loesche Evo-Ergebnis-Daten */
      for (ii = 0;  ii < MAX_EVO_ERG;  ii++) 
         last_evo_erg[jj].evo_erg_feld[ii].note = KEINE_NOTE; /* loesche evo_erg_feld */
      last_evo_erg[jj].last_evo_index = -1;
   } /* Loesche Evo-Ergebnis-Daten */
} /* loesche_evo_erg */



/* ============================== dumpPopFeld =============================== */
void dumpPopFeld (const char *buf,
		  INT         popSize)
{
   INT  ii;

   /* dumpPopFeld */
   printf ("\n%s: PopFeld mit %d Eintraegen", buf, popSize);
#ifdef MPI_PAR_VERS
   if (master)
      printf (":\n", popSize);
   else
      printf (", aktiver Bereich von %d bis %d:\n", firstIndIdx, lastIndIdx);
   printf ("  ii/PopIdx newCh bel. AKIdx Len stat guete pop_feld-Note    AK-Note\n");
   /*          xxx/xxx     x   xx   xxx  xxx  xxx   xxx 123456.12345 6 xxxxxxxxx  */
   for (ii = 0;  ii < popSize;  ii++) {
      printf ("   %3d/%3d     %1d   ", ii, pop_feld[ii].index, pop_feld[ii].new_chain);
      if (pop_feld[ii].chain_ptr == NULL)
	 printf ("--                   %3d %13.6f\n", pop_feld[ii].guete, pop_feld[ii].note);
      else
	 printf ("ja   %3d  %3d %4d   %3d %13.6f  %.9f\n", 
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.parIndIdx,
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.chain_length,
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.chain_status,
		 pop_feld[ii].guete, pop_feld[ii].note,
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.fitness_note);
   }
#else /* sequentielle Version */
   printf (":\n");
   printf ("  ii/PopIdx newCh bel. ref Len stat guete pop_feld-Note    AK-Note\n");
   /*          xxx/xxx     x   xx  xxx  xxx   xxx 123456.123456  xxxxxxxxx  */
   for (ii = 0;  ii < popSize;  ii++) {
      printf ("   %3d/%3d     %1d   ", ii, pop_feld[ii].index, pop_feld[ii].new_chain);
      if (pop_feld[ii].chain_ptr == NULL)
	 printf ("--                 %3d %13.6f\n", pop_feld[ii].guete, pop_feld[ii].note);
      else
	 printf ("ja  %3d %3d %4d   %3d %13.6f  %.9f\n", 
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.ref_counter,
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.chain_length,
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.chain_status,
		 pop_feld[ii].guete, pop_feld[ii].note,
		 pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.fitness_note);
   }
#endif /* sequentielle Version */
} /* dumpPopFeld */



/* =============================== getHamDist =============================== */
DOUBLE getHamDist (INT  indivIdx,
                   INT  partnerIdx)
{
   INT     delta;
   DOUBLE *hamWert;

   /* getHamDist */
   if ((indivIdx > max_pop_index) || (partnerIdx > max_pop_index) ||
       (indivIdx < 0) || (partnerIdx < 0))
      return (NO_HAM);
   else
   { /* gueltige Indices */
      delta = indivIdx - partnerIdx;
      if (delta == 0)
	 return (0.0);                     /* so 'ne Art Auto-Hamming-Abstand */
      else
      { /* zwei verschiedene Individuen im Deme */
	 if (abs(delta) > anz_nb_halbe) { /* Indivs liegen an den Tabellengrenzen */
	    if (indivIdx < anz_nb_halbe)
	       delta += pop_size;
	    else
	       delta -= pop_size;
	 } /* Indivs liegen an den Tabellengrenzen */
	 if (delta > 0)
	    hamWert = &(pop_feld[indivIdx].deme_ham[delta - 1]);
	 else
	    hamWert = &(pop_feld[partnerIdx].deme_ham[-delta - 1]);
	 if (*hamWert == NO_HAM)
	    *hamWert = ham_dist (pop_feld[indivIdx].chain_ptr, 
				 pop_feld[partnerIdx].chain_ptr);
      } /* zwei verschiedene Individuen im Deme */
   } /* gueltige Indices */
   return (*hamWert);
} /* getHamDist */



/* ================================= del_ep ================================= */
GSTATE del_ep (void)
{
   INT  hamLimit,                     /* number of stored Hamming distances   */
        popLimit,                     /* number of indiv-entries to be erased */
        ii, ij;

   /* del_ep  */
#ifdef MPI_PAR_VERS
   if (symRing)
      hamLimit = anz_nb_halbe;
   else
      hamLimit = anz_nachbarn;
#else /* keine MPI-Parallelversion */
   hamLimit = anz_nb_halbe;
#endif /* keine MPI-Parallelversion */
   if (last_psize > pop_size)
      popLimit = last_psize;
   else
      popLimit = pop_size;
   for (ii = 0;  ii < popLimit;  ii++) 
   { /* erase the old population and asure that all new entries are empty */
      if (pop_feld[ii].chain_ptr != NULL) 
	 if (delete_chain(&(pop_feld[ii].chain_ptr)) != GLEAM_OK)
	    return (GLEAM_FATAL);
      for (ij = 0;  ij < hamLimit;  ij++)
	 pop_feld[ii].deme_ham[ij] = NO_HAM;         /* Hamming-Wert loeschen */
   } /* erase the old population and asure that all new entries are empty */
   last_psize = 0;      /* Wo alles geloescht ist, ist nichts mehr zu retten! */
   return (GLEAM_OK);
} /* del_ep */
 


/* =========================== keep_result_chain ============================ */
GSTATE keep_result_chain (INT  index, 
                          INT  ketten_art)
{
   STR_TYPE  buf2;
   BUF_TYPE  buf1;
   INT       number, guete;
   GSTATE    erg_state = GLEAM_OK;

   /* KEEP_CHAIN_RESULT/EVO */
   if (!pop_feld[index].new_chain)
   { /* Kette offenbar schon gespeichert oder nicht neu */
      write_buf (1, AK_SCHON_GESP);
      erg_state = GLEAM_OK;
   } /* Kette offenbar schon gespeichert oder nicht neu */
   else
   { /* Rette Ergebniskette */
      guete     = pop_feld[index].guete;
      erg_state = store_res_ch (&number, index, ketten_art);
      adr_to_text (buf1, guete, number, NO_ADJUST);
      if (erg_state == GLEAM_OK)
      {
         sprintf (buf2, AK_GESPEICHERT, buf1);
         write_buf (get_line_anz () + 1, buf2);
      }
      else
         write_buf (1, AK_ERR_TXT);
   } /* Rette Ergebniskette */
   return (erg_state);
} /* KEEP_CHAIN_RESULT/EVO */



/* ================================ get_guete =============================== */
INT get_guete (DOUBLE *note,
               INT     index)
{
   /* get_guete */
   if (index > max_pop_index) 
      return (KEINE_GUETE);
   else
   {
      *note = pop_feld[index].note;
      return (pop_feld[index].guete);
   }
} /* get_guete */



/* ================================= init_ep ================================ */
BOOL init_ep (DOUBLE *best_note, 
              INT     akt_pop_size,
              LONG    s_par)
{
   CHAIN_ELEM_PTR_TYPE  chrPtr;
   INT   ii, ij, nb_index;
   BOOL  result;
  
   /* init_ep */
#ifdef CHECKS
   if (akt_pop_size > MAX_POPEL_SIZE)
      err_state = fehler (EVO_GSW, 20, akt_pop_size, MAX_POPEL_SIZE, "", POPEL_OVFL);
#endif
   *best_note = 0.0;
   pop_size   = akt_pop_size;
   if (best_child_ptr != NULL)    /* Loesche evtl. noch vorhandenen Offspring */
      err_state |= delete_chain (&best_child_ptr);
   err_state |= del_ep ();        /* loesche evtl. noch vorhandene EntwPopul. */
   result     = allocChrList();
   if ((err_state != GLEAM_OK) || !result)      /* memOut and/or other errors */
      return (FALSE);
   toBeEvalCount   = 0;
   init_best_note  = best_note;                          /* Pointer-Zuweisung */
   strat_par       = s_par;
   max_pop_index   = pop_size - 1;
   min_fitness     = 2.0 - max_fit;                 /* fuer "partnerwahl"     */
   fitness_delta   = max_fit - min_fitness;         /* fuer "partnerwahl"     */
   anz_nb_halbe    = trunc (anz_nachbarn / 2);      /* fuer "partnerwahl"     */
   max_nb_index    = anz_nachbarn - 1;              /* fuer "partnerwahl"     */

   for (ii = 0;  ii < pop_size; ii++)
   { /* ParVers: Plaetze neuer AKs markieren. Sonst: Nachbarschaft generieren */
#ifdef MPI_PAR_VERS
      pop_feld[ii].new_chain = TRUE;  /* d.h.: diese AK ist durch Slave zu generieren */ 
#else /* keine Parallelversion */
      for (ij = 1;  ij <= anz_nb_halbe;  ij++)     /* Generiere Nachbarschaft */
      { /* Eine Nachbarschaft anlegen */
         nb_index = (2 * ij) - 1;
         pop_feld[ii].nachbarn[nb_index].nbPtr   = &(pop_feld[(ii+ij)          % pop_size]);
         pop_feld[ii].nachbarn[nb_index-1].nbPtr = &(pop_feld[(ii+pop_size-ij) % pop_size]);
      } /* Eine Nachbarschaft anlegen */
#endif /* keine Parallelversion */
   } /*  ParVers: Plaetze neuer AKs markieren. Sonst: Nachbarschaft generieren */

   switch (init_strat) {
      case NEU:       result = gen0_neu      (0, pop_size, strat_par, GUT);
      break;

      case BEST:      result = gen0_NBest    (akt_pop_size);
      break;

      case MIX:       result = gen0_mix      ();
      break;

      case BEST_NEU:  result = gen0_NBest    (strat_par);
      break;

      case FROM_FILE: result = init_from_file (akt_pop_size);
      break;

#ifdef MIT_LSV
      case LSV_NEU:   result = gen0_lsv (INIT_NEU_LSV);
      break;

      case LSV_BEST:  result = gen0_lsv (INIT_BEST_LSV);
      break; 

      case ONE_CHR:
	 ii = evo_ws[first_evo_item].chrNumber;
	 ij = (INT)strat_par;
	 result = TRUE;
	 if (get_chain(&chrPtr, &ii, AK, ij, &ij, &err_state)) 
	    put_ch(0, chrPtr);
	 else {
	    result = FALSE;
	    adr_to_text(buf1, strat_par, ii, ADJUST);
	    err_state |= fehler(EVO_GSW, 22, NO_INFO, NO_INFO, buf1, NO_CHAIN);
	 }
	 break; /* ONE_CHR */
#endif /* MIT_LSV */

      default:
      err_state = fehler (EVO_GSW, 22, init_strat, NO_INFO, "", BAD_STRATEGY);
      result    = FALSE;
      break;
   } /* case */
   if (result) 
   { /* start population successfully generated */
      indiv_ctr += toBeEvalCount;
      if (toBeEvalCount > 0)
	 result = evaluateStartPop();     /* for parallel simu/eval by ext.sim.services */
      if (result)
	 last_psize = akt_pop_size;
   } /* start population successfully generated */
   else
      err_state = fehler (EVO_GSW, 23, NO_INFO, NO_INFO, "", START_POP_ERROR);
   return (result);
} /* init_ep */



/* ====================================== ch_gen ====================================== */
#ifdef MPI_PAR_VERS
         BOOL ch_gen (INT  indivIdx)
#else
  static BOOL ch_gen (INT  indivIdx)
#endif /* MPI_PAR_VERS */
{
   CHAIN_ELEM_PTR_TYPE  chain_ptr;
   DOUBLE               ak_note;
   INT                  bad_ch_ctr = 0;
   BOOL                 fertig     = FALSE;
  
   /* ch_gen */
   do 
   {
      if (!gen_chain(&chain_ptr, &err_state)) 
      { /* out of memory */
         reclaim_memory(AK);
         ak_note = 0.0;                         /* "vernuenftiger" Wert fuer "gen0_neu" */
         mem_out  = !gen_chain(&chain_ptr, &err_state);
      } /* out of memory */
      if (mem_out) 
      { /* report error and abandon loop */
         err_state |= fehler(EVO_GSW, 30, NO_INFO, NO_INFO, CHROMOSOM_TXT, NOT_ENOUGH_MEM);
         break;                                                         /* abandon loop */
      } /* report error and abandon loop */
      if (repair_1_chain(&chain_ptr, &err_state))                /* loescht unplaus.AKs */
      { /* plausible Kette */
#ifdef EXT_SIMU_SERV
	 ak_note = 0.0;
	 fertig  = TRUE;
#else /* no EXT_SIMU_SERV */
         if (chain_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED)
            ak_note = chain_ptr->u_el.u_xel.u_hdr.fitness_note;
         else 
            err_state |= simulator(&ak_note, CALC_FITNESS, chain_ptr);
         if (ak_note == KEINE_NOTE) 
            err_state |= delete_chain(&chain_ptr);                /* unbewertbare Kette */
         else 
            if (ak_note >= mindest_note) 
               fertig = TRUE;
            else 
               err_state |= delete_chain(&chain_ptr);
#endif /* no EXT_SIMU_SERV */
      } /* plausible Kette */
      if (!fertig)
      { /* unplausible oder unbewertbare Kette */
         bad_ch_ctr++;
         if (bad_ch_ctr > MAX_POPEL_SIZE)
            err_state |= fehler(EVO_GSW, 31, NO_INFO, NO_INFO, "", NO_AK_GEN);
      } /* unplausible oder unbewertbare Kette */
   } while (!fertig && (err_state == GLEAM_OK));
#ifndef EXT_SIMU_SERV                     /* Standard:     only chromosomes with a      */
   if (ak_note > mindest_note)            /*               fitness > 0 are counted.     */
#endif /* no EXT_SIMU_SERV */             /* EXT_SIM_SERV: as simulation is done later, */
      gut_anz++;                          /*               all chromosomes are "good".  */
   pop_feld[indivIdx].note      = ak_note;
   pop_feld[indivIdx].chain_ptr = chain_ptr;
   return (fertig);
} /* ch_gen */



/* ====================================== put_ch ====================================== */
static void put_ch (INT                  pop_index,
                    CHAIN_ELEM_PTR_TYPE  ch_ptr)
/*---------------------------------------------------------------------------------------
   Traegt, die durch "ch_ptr" angegebene Kette unter "pop_index" in das "pop_feld" ein 
   und setzt "new_chain" auf FALSE. "init_best_note" und "gen_len" werden aktualisiert, 
   sofern eine AK mit besserer Note eingetragen wird. 
----------------------------------------------------------------------------------------*/
{
   DOUBLE  note;

   /* put_ch */
   note = ch_ptr->u_el.u_xel.u_hdr.fitness_note;
   (ch_ptr->u_el.u_xel.u_hdr.ref_counter)++;
   pop_feld[pop_index].chain_ptr = ch_ptr;
   pop_feld[pop_index].note      = note;
   pop_feld[pop_index].new_chain = FALSE;
   berechne_guete (&(pop_feld[pop_index].guete), note);
   if (note > *init_best_note)
   { /* update best fitness and length of that chromosome */
      *init_best_note = note;
      gen_len         = ch_ptr->u_el.u_xel.u_hdr.chain_length;
   } /* update best fitness and length of that chromosome */
#ifdef GEN0_DBG
   printf("put_ch: Chr %d/%d fitness=%g\n", ch_ptr->u_el.u_xel.u_hdr.guete, 
	  ch_ptr->u_el.u_xel.u_hdr.lfd_nr, note); 
#endif /* GEN0_DBG */
} /* put_ch */



/* ============================= init_from_file ============================= */
static BOOL init_from_file (INT  akt_size)
/*------------------------------------------------------------------------------
   Oeffnet das EVO_SCRATCH_FILE und initialisiert die Entwicklungspopulation mit
   den Ketten dieses Files. Wenn bei der Kettenrestaurierung etwas schief ging 
   oder die im File enthaltenen Kettenzanzahl kleiner als "akt_size" ist, wird 
   eine Fehlermeldung erzeugt, die bereits aufgebaute Population geloescht und 
   FALSE abgeliefert. Andernfalls TRUE und es wird eine Erfolgsmeldung in den
   Textpuffer geschrieben.
------------------------------------------------------------------------------*/
{
   STR_TYPE             buf;
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   INT                  ak_anz;
   BOOL                 init_ok;

   /* init_from_file */
   init_ok = open_ch_file(EVO_SCRATCH_FILE, 1, CH_READ, GLOBAL_SAVE, chrFileIOMode);
   if (init_ok)
   { /* File offen */
      ak_anz = 0;
      while (ch_file_ok() && init_ok && (ak_anz < akt_size))
      { /* Eine Kette restaurieren und eintragen */
         init_ok = read_chain(&ch_ptr, CH_START);
         if (init_ok)
         { /* Kette erfolgreich restauriert */
            init_ok = restore_ch_ref(ch_ptr);
            pop_feld[ak_anz].chain_ptr = ch_ptr;
            pop_feld[ak_anz].note      = ch_ptr->u_el.u_xel.u_hdr.fitness_note;
            if (ch_ptr->u_el.u_xel.u_hdr.guete == KEINE_GUETE)
            { /* Frisch initialisierte oder durch Evo veraenderte AK */
               pop_feld[ak_anz].new_chain = TRUE;
               berechne_guete(&(pop_feld[ak_anz].guete), 
                               ch_ptr->u_el.u_xel.u_hdr.fitness_note);
            } /* Frisch initialisierte oder durch Evo veraenderte AK */
            else
            { /* Original-AK aus AK-Mem */
               pop_feld[ak_anz].new_chain = FALSE;
               pop_feld[ak_anz].guete     = ch_ptr->u_el.u_xel.u_hdr.guete;
            } /* Original-AK aus AK-Mem */
            ch_ptr->u_el.u_xel.u_hdr.ref_counter = 1;
            ch_ptr->u_el.u_xel.u_hdr.ak_index    = 0;
            if (pop_feld[ak_anz].note > *init_best_note) 
            { /* update best fitness and length of that chromosome */
               *init_best_note = pop_feld[ak_anz].note;
	       gen_len         = ch_ptr->u_el.u_xel.u_hdr.chain_length;
	    } /* update best fitness and length of that chromosome */
            ak_anz++;
         } /* Kette erfolgreich restauriert */
      } /* Eine Kette restaurieren und eintragen */
      close_ch_file();
   } /* File offen */
   init_ok = (akt_size == ak_anz) && init_ok;
   if (init_ok)
   { /* population successfully restored */
      sprintf(buf, REST_OK_TXT, "", ak_anz, *init_best_note);
      write_buf(get_line_anz()+1, buf);
   } /* population successfully restored */
   else
   { /* Fehlerbehandlung */
      err_state |= fehler(EVO_GSW, 35, ak_anz, akt_size, "", EVO_SCRATCH_REST_ERR);
      err_state |= del_ep();
   } /* Fehlerbehandlung */
   return (init_ok);
} /* init_from_file */



/* ============================== mix_up_popul ============================== */
static void mix_up_popul (INT  anzAKs)
/*----------------------------------------------------------------------------*/
/*  Die Routine mischt die Population, indem sie die erste Haelfte der Indi-  */
/*  viduen auf zufaellig bestimmte Plaetze verteilt. Wegen der generierten    */
/*  Nachbarschaft erfolgt das Vertauschen explizit.                           */
/*  Bei gesetztem MPI_PAR_VERS geschieht dies nur mit den ins "pop_feld" ein- */
/*  getragenen AKs (anzAKs), sofern diese weniger als die Haelfte ausmachen.  */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  tmp_ptr;
   DOUBLE               tmp_note;
   BOOL                 tmp_new_chain;
   INT                  tmp_guete, 
                        limit,            /* anzAKs aber maximal pop_size / 2 */
                        ii, jj;
 
   /* mix_up_popul */
   limit = (pop_size / 2) + 1;
#ifdef MPI_PAR_VERS
   if (anzAKs < limit)
      limit = anzAKs;
#endif /* MPI_PAR_VERS */
   for (ii = 0;  ii < limit;  ii++) 
   { /* Mischen der Population */
      jj = irand (pop_size);
      tmp_ptr       = pop_feld[ii].chain_ptr;
      tmp_guete     = pop_feld[ii].guete;
      tmp_note      = pop_feld[ii].note;
      tmp_new_chain = pop_feld[ii].new_chain;
      pop_feld[ii].chain_ptr = pop_feld[jj].chain_ptr;
      pop_feld[ii].guete     = pop_feld[jj].guete;
      pop_feld[ii].note      = pop_feld[jj].note;
      pop_feld[ii].new_chain = pop_feld[jj].new_chain;
      pop_feld[jj].chain_ptr = tmp_ptr;
      pop_feld[jj].guete     = tmp_guete;
      pop_feld[jj].note      = tmp_note;
      pop_feld[jj].new_chain = tmp_new_chain;
   } /* Mischen der Population */
} /* mix_up_popul */
 
 

/* ===================================== gen0_neu ===================================== */
static BOOL gen0_neu (INT   anf,
                      INT   anzahl,
                      INT   local_strat_par,
                      BOOL  qualitaet)   
/*----------------------------------------------------------------------------------------
   Generiere Anfangspopulation aus "anzahl" neuen Ketten beginnen ab Index "anf". Wenn 
   "qualitaet" GUT ist, muessen "local_strat_par" Ketten eine  Note > 0 haben. Dabei muss 
   "local_strat_par" kleiner als "anzahl" sein, was aber nicht geprueft wird. Der Ver-
   besserungsprozess bricht nach 10 * "local_strat_par" Versuchen ab. Fitnesswert und Laenge 
   des besten generierten Chromosoms werden in "init_best_note" und "gen_len" gespeichert.
   Liefert TRUE, wenn alles gut ging, sonst FALSE und weitere Fehlerinformationen in 
   "mem_out" und "err_state". 
   Bei gesetztem MPI_PAR_VERS tut die Routine nichts, da die Erzeugung neuer zufaelliger 
   AKs Sache der Slaves ist. 
   Die Anzahl neu erzeugter Chromosomoe wird bei gesetztem EXT_SIMU_SERV in "toBeEvalCount" 
   mitgezaehlt.
----------------------------------------------------------------------------------------*/
{
   INT   trialCtr, trialMax,              /* counter and maximum for improvement trials */
         ii, jj;                          /* inidices or ausxiliary variables           */
   BOOL  init_ok = TRUE;
  
   /* gen0_neu */
   gut_anz = 0;
#ifndef MPI_PAR_VERS                                  /* sequentielle Version */
   mindest_note = 0.0;
   ii = jj + anf;
   for (jj = 0;  (jj < anzahl) && init_ok;  jj++)
   { /* Erste Phase der Initialisierung */
      init_ok = ch_gen(ii);
      if (init_ok)
      { /* AK erfolgreich generiert */
         berechne_guete(&(pop_feld[ii].guete), pop_feld[ii].note);
         if (pop_feld[ii].note > *init_best_note) 
         { /* update best fitness and length of that chromosome */
            *init_best_note = pop_feld[ii].note;
	    gen_len         = pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.chain_length;
	 } /* update best fitness and length of that chromosome */
         pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.ref_counter = 1;
         pop_feld[ii].new_chain                               = TRUE;
	 toBeEvalCount++;
         ii++;
      } /* AK erfolgreich generiert */
   } /* Erste Phase der Initialisierung */

   if (qualitaet && init_ok && (gut_anz < local_strat_par)) 
   { /* Es sind zuwenig Ketten mit einer Note > 0 da */
      trialMax     = 10 * local_strat_par;
      trialCtr     = 0;
      mindest_note = 1.0;
      for (ii = gut_anz + 1;  (ii <= local_strat_par) && (trialCtr < trialMax);  ii++) 
      { /* Generiere eine "gute" Kette */
         do {
            jj = irand(anzahl) + anf;
	    trialCtr++;
	 }
         while ((pop_feld[jj].note != 0) && (trialCtr < trialMax));
         if (delete_chain(&(pop_feld[jj].chain_ptr)) == GLEAM_OK)
         { /* ok */
             if (!ch_gen(jj))
             { /* Error oder MemOut: abandon loop */
                init_ok = FALSE;
                break;
             } /* Error oder MemOut: abandon loop */
         } /* ok */
         else
         { /* error: abandon loop */
            err_state = GLEAM_FATAL;
            init_ok   = FALSE;
            break;
         } /* error: abandon loop */
         berechne_guete(&(pop_feld[jj].guete), pop_feld[jj].note);
         if (pop_feld[jj].note > *init_best_note)
         { /* update best fitness and length of that chromosome */
            *init_best_note = pop_feld[jj].note;
	    gen_len         = pop_feld[jj].chain_ptr->u_el.u_xel.u_hdr.chain_length;
	 } /* update best fitness and length of that chromosome */
         pop_feld[jj].chain_ptr->u_el.u_xel.u_hdr.ref_counter = 1;
         pop_feld[jj].new_chain                               = TRUE;
      } /* Generiere eine "gute" Kette */
   } /* Es sind zuwenig Ketten mit einer Note > 0 da */
#endif /* sequentiell Version */
   return (init_ok);
} /* gen0_neu */



/* ===================================== gen0_NBest =================================== */
static BOOL gen0_NBest (INT  targetAmount)
/*----------------------------------------------------------------------------------------
   Select the best "targetAmount" individuals from rank classes 2 and higher of the chro-
   mosome memory. If "targetAmount" is larger than the population size "pop_size" it is 
   limited to "pop_size". If there are  not enough chromosomes available, the rest of the
   initial population will be filled up with randomly generated ones. Finally, the popu-
   lation will be mixed. 
   Errors are reported and result in FALSE beeing returned.
   The function is used to implement the initialization strategies BEST and BEST_NEU.

   Pseudo code:
     IF not enough chromosomes in memory THEN
        take them all                            // implies that best is taken also
     ELSE                                        // more chr in memory than needed
        determine highest rank used --> bestRank
        remainingChrDemand = targetAmount
        actRank            = bestRank
        DO
           IF amount of chr of actRank <= remainingChrDemand THEN
              take them all                      // implies that best chr is taken also
              remainingChrDemand -= amount of chr of actRank
           ELSE // take the best of this rank only, implies that best chr is taken also
              take best remainingChrDemand chromosomes without selecting one multiply
              remainingChrDemand = 0
           FI
           actRank--;
        WHILE ((remainingChrDemand > 0) && (actRank > 1))
     FI
     IF not enough chromosomes for start population THEN
       generate rest new
     FI
     mix_up_popul();
----------------------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  chrPtr;             /* chromosome pointer                       */
   DOUBLE               bestFitness;        /* used for searching for best chromosomes  */
   INT                  chrMemSize,         /* number of chromosomes in memory          */
                        rankChrAmount,      /* amount of chromosomes of a rank class    */
                        actRank,            /* actual rank class                        */
                        bestRank,           /* highest rank with at least one chromosome*/
                        remainingChrDemand, /* remaining demand of crhomosomes          */
                        popIdx,             /* index of a chromosome within pop_feld[]  */
                        bestChrNo,          /* number of best chromosome in chr. mem.   */
                        ii, jj;             /* auxiliary variable                       */
   BOOL                 initOK = TRUE;

   /* gen0_NBest */
   if (targetAmount > pop_size)
      targetAmount = pop_size;

   popIdx = 0;                                      /* none stored in pop_feld[] so far */
   chrMemSize = number_of_chains(2, MAX_GUETE, AK, &err_state);
   if (chrMemSize <= targetAmount)
   { /* not or just enough chromosomes, so take them all */
      for (actRank = 2;  (actRank <= MAX_GUETE) && initOK;  actRank++) 
      { /* all rank classes larger than 1 */
	 rankChrAmount = number_of_chains(actRank, actRank, AK, &err_state);
	 for (ii = 1;  ii <= rankChrAmount;  ii++) 
         { /* all chromosomes of this rank class */
	    if (!get_chain(&chrPtr, &ii, AK, actRank, &actRank, &err_state))
            { /* chromosome not accessible */
	       adr_to_text(buf1, actRank, ii, NO_ADJUST);
	       err_state = fatal (EVO_GSW, 40, NO_INFO, NO_INFO, buf1, NO_CHAIN);
	       break;                                             /* abandon inner loop */
	    } /* chromosome not accessible */
            put_ch (popIdx++, chrPtr);
	 } /* all chromosomes of this rank class */
      } /* all rank classes larger than 1 */
   } /* not or just enough chromosomes, so take them all */
   else
   { /* more chromosomes in memory than needed: select targetAmount best ones */
      bestRank = MAX_GUETE;
      while ((number_of_chains(bestRank, bestRank, AK, &err_state) == 0) && (bestRank > 1))
	 bestRank--;
      if ((bestRank < 2) || (err_state != GLEAM_OK))  /* TRUE: something weird happened */
	 err_state = fatal(EVO_GSW, 41, NO_INFO, NO_INFO, "", AK_EXPECTED); 
      else
      { /* highest rank of chromosomes in memory determined */
	 remainingChrDemand = targetAmount;           /* amount to be taken from memory */
	 actRank            = bestRank;
	 do
	 { /* take all or select best chromosomes of actRank */
	    rankChrAmount = number_of_chains(actRank, actRank, AK, &err_state);
	    if (rankChrAmount <= remainingChrDemand)
	    { /* rank class contains less or equal number of chromosomes as requested */
	       for (ii = 1;  ii <= rankChrAmount;  ii++) 
               { /* take all chromosomes of this rank class */
		  if (!get_chain(&chrPtr, &ii, AK, actRank, &actRank, &err_state))
                  { /* chromosome not accessible */
		     adr_to_text(buf1, actRank, ii, NO_ADJUST);
		     err_state = fatal (EVO_GSW, 42, NO_INFO, NO_INFO, buf1, NO_CHAIN);
		     break;                                             /* abandon loop */
		  } /* chromosome not accessible */
		  put_ch (popIdx++, chrPtr);
	       } /* take all chromosomes of this rank class */
	       remainingChrDemand -= rankChrAmount;
	    } /* rank class contains less or equal number of chromosomes as requested */
	    else
	    { /* rank class contains more chromosomes than needed: select best ones */
	       for (ii = 1;  ii <= rankChrAmount;  ii++)
               { /* mark all chromosomes of this rank class as not selected */
		  if (!get_chain(&chrPtr, &ii, AK, actRank, &actRank, &err_state))
                  { /* chromosome not accessible */
		     adr_to_text(buf1, actRank, ii, NO_ADJUST);
		     err_state = fatal (EVO_GSW, 43, NO_INFO, NO_INFO, buf1, NO_CHAIN);
		     break;                                             /* abandon loop */
		  } /* chromosome not accessible */
		  chrPtr->u_el.u_xel.u_hdr.parIndIdx = NIX_IDX;   /* mark as unselected */
	       } /* mark all chromosomes of this rank class as not selected */
	       while ((remainingChrDemand > 0) && (err_state == GLEAM_OK))
	       { /* search for best chromosome not already taken */
		  bestFitness = 0.0;
		  for (ii = 1;  ii <= rankChrAmount;  ii++)
		  { /* search for better and not selected chromosome */
		     if (!get_chain(&chrPtr, &ii, AK, actRank, &actRank, &err_state))
                     { /* chromosome not accessible */
			adr_to_text(buf1, actRank, ii, NO_ADJUST);
			err_state = fatal (EVO_GSW, 43, NO_INFO, NO_INFO, buf1, NO_CHAIN);
			break;                                    /* abandon inner loop */
		     } /* chromosome not accessible */
		     if ((chrPtr->u_el.u_xel.u_hdr.fitness_note > bestFitness) && 
			 (chrPtr->u_el.u_xel.u_hdr.parIndIdx == NIX_IDX))
		     { /* a better unselected chromosome found */
			bestFitness = chrPtr->u_el.u_xel.u_hdr.fitness_note; /*remember */
			bestChrNo   = ii;                            /* best chr. found */
		     } /* a better unselected chromosome found */
		  } /* search for better and not selected chromosome */
		  if ((err_state == GLEAM_OK) && (bestFitness > 0.0))
		  { /* a better and unselected chromosome found: select it */
		     if (get_chain(&chrPtr, &bestChrNo, AK, actRank, &actRank, &err_state))
		     { /* select this chromosome and mark it */
			put_ch(popIdx++, chrPtr);               /* select chromosome   */
			chrPtr->u_el.u_xel.u_hdr.parIndIdx = 0; /* mark it as selected */
			remainingChrDemand--;
		     } /* select this chromosome and mark it */
		     else
                     { /* chromosome not accessible */
			adr_to_text(buf1, actRank, ii, NO_ADJUST);
			err_state = fatal (EVO_GSW, 44, NO_INFO, NO_INFO, buf1, NO_CHAIN);
		     } /* chromosome not accessible */
		  } /* a better and unselected chromosome found: select it */
	       } /* search for best chromosome not already taken */
	       if ((remainingChrDemand != 0) || (popIdx != targetAmount))
	       {
		  sprintf(buf1, "Error selecting chromosomes from memory: remaining demand: %d  "
			  "target amount: %d  amount stored: %d", remainingChrDemand, targetAmount, popIdx);
		  err_state = fatal(EVO_GSW, 45, NO_INFO, NO_INFO, buf1, GENERAL_MESSAGE);
	       }
	    } /* rank class contains more chromosomes than needed: select best ones */
	    actRank--;
	 } /* take all or select best chromosomes of actRank */
	 while ((remainingChrDemand > 0) && (actRank > 1) && (err_state == GLEAM_OK));
      } /* highest rank of chromosomes in memory determined */
   } /* more chromosomes in memory than needed: select targetAmount best ones */
#ifdef GEN0_DBG
   printf("gen0_NBest: chrMemSize=%d targetAmount=%d popIdx=%d remainingChrDemand=%d\n",
	  chrMemSize, targetAmount, popIdx, remainingChrDemand);
#endif /* GEN0_DBG */
   if (err_state != GLEAM_OK)
      initOK = FALSE;
   else
      remainingChrDemand = pop_size - popIdx;            /* rest to be created randomly */

#ifdef GEN0_DBG
   printf("gen0_NBest: initOK=%d remainingChrDemand=%d\n", initOK, remainingChrDemand); 
#endif /* GEN0_DBG */
 
   if (initOK && prepareForEvo)
   { /* create a possible remainder by chance and and shuffle the start population */
      if (remainingChrDemand > 0)       /* not enough chromosomes for start population? */
	 initOK = gen0_neu(popIdx, remainingChrDemand, 0, EGAL);
      if (initOK)
	 mix_up_popul(pop_size);
   } /* create a possible remainder by chance and and shuffle the start population */
   else
      selectedChrAmount = popIdx;                 /* only required if not prepareForEvo */
   return (initOK);
} /* gen0_NBest */



/* ===================================== gen0_mix ===================================== */
static BOOL gen0_mix (void)
/*----------------------------------------------------------------------------------------
   Generiere Anfangspopulation aus zufaellig gewaehlten Ketten, deren Fitness besser ist 
   als "strat_par" unter Vermeidung von Mehrfachverwendungen. Wenn die Anzahl vorhandener 
   Ketten dazu nicht ausreicht, wird der Rest mittels gen0_neu() frisch erzeugt. Am Ende 
   wird die Population durchgemischt. 
   Wenn alles gut ging, ist das Funktionsergebnis TRUE, sonst FALSE und weitere Fehler-
   informationen in "mem_out" und "err_state". 
----------------------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   BOOL                 gefunden, init_ok;
   INT                  ketten_anz, anf, ii, jj, kk, number, loop_end;
  
   /* gen0_mix */
   init_ok = TRUE;
   /* ------------ Bestimme die Anzahl der AKs mit Fitness >= "strat_par": ------------ */
   ketten_anz = 0;
   anf        = MAX_GUETE+1;
   do 
   { /* Such-Schleife */
      anf--;
      jj = number_of_chains(anf, anf, AK, &err_state);
      for (ii = 1; ii <= jj; ii++)     
      { /* durchsuche jede Gueteklasse */
         if (!get_chain(&ch_ptr, &ii, AK, anf, &anf, &err_state))
         { /* Kette nicht verfuegbar ! */
            adr_to_text(buf1, anf, ii, NO_ADJUST);
            err_state = fatal(EVO_GSW, 50, NO_INFO, NO_INFO, buf1, NO_CHAIN);
            return (FALSE);
         } /* Kette nicht verfuegbar ! */
         if (ch_ptr->u_el.u_xel.u_hdr.fitness_note >= strat_par) 
            ketten_anz++;
      } /* durchsuche jede Gueteklasse */
   }  /* Such-Schleife */
   while ((ketten_anz < pop_size) && (anf > 0));

   /* ------------------------- Initialisieren der Population: ------------------------ */
   if (ketten_anz <= pop_size) 
   { /* Nimm alle geeigneten AKs */
      ii = 0;
      if (ketten_anz > 0) 
      { /* es gibt AKs */
         for (jj = anf;  jj <= MAX_GUETE;  jj++) 
         { /* Alle Ketten dieser Guete */
            loop_end = number_of_chains (jj, jj, AK, &err_state); 
            for (kk = 1;  kk <= loop_end;  kk++)   
            { /* WITH */
               if (!get_chain (&ch_ptr, &kk, AK, jj, &jj, &err_state))
               { /* Kette nicht verfuegbar ! */
                  adr_to_text (buf1, jj, kk, NO_ADJUST);
                  err_state = fatal (EVO_GSW, 51, NO_INFO, NO_INFO, buf1, NO_CHAIN);
                  return (FALSE);
               } /* Kette nicht verfuegbar ! */
               if (ch_ptr->u_el.u_xel.u_hdr.fitness_note >= strat_par) 
               { /* Nimm diese AK */
                  put_ch (ii, ch_ptr);
                  ii++;                /* Naechstes Individuum initialisieren */
               } /* Nimm diese AK */
            } /* WITH */
         } /* Alle Ketten dieser Guete */
      } /* es gibt AKs */

      /* ------------ belege alle weiteren Individuen zufaellig: ------------ */
      if (prepareForEvo)
	 init_ok = gen0_neu (ii, pop_size-ii, 0, EGAL);
   } /* Nimm alle geeigneten AKs */ 
   else 
   { /* Waehle geeignete AK zufaellig aus */
      for (ii = 0;  ii <= max_pop_index;  ii++)
      { /* eine AK auswaehlen */
         do 
         { /* Auswaehlen */
            gefunden = TRUE;
            number   = 0;
            kk       = MAX_GUETE;
            if (!get_chain (&ch_ptr, &number, AK, anf, &kk, &err_state))
            { /* Kette nicht verfuegbar */
               sprintf (buf1, G_INTERVAL_TXT, anf, MAX_GUETE);
               err_state = fatal (EVO_GSW, 52, NO_INFO, NO_INFO, buf1, NO_CHAIN);
               return (FALSE);
            } /* Kette nicht verfuegbar */
            if (ch_ptr->u_el.u_xel.u_hdr.fitness_note >= strat_par) 
            { /* Note ok */
               if (ii>0)
               { /* teste, ob AK bereits ausgewaehlt wurde */
                  jj = ii;
                  do {
                     jj--;
                     if (pop_feld[jj].chain_ptr == ch_ptr)
                        gefunden = FALSE;
                  } while (jj != 0);
               } /* teste, ob AK bereits ausgewaehlt wurde */
            }  /* Note ok */
            else
               gefunden = FALSE;                      /* Note nicht gut genug */
         } /* Auswaehlen */
         while (!(gefunden));
         put_ch (ii, ch_ptr);
      } /* eine AK auswaehlen */
   } /* Waehle geeignete AK zufaellig aus */

   if (init_ok && prepareForEvo)
      mix_up_popul (ii);                            /* Mischen der Population */
   else
      selectedChrAmount = ii;           /* only required if not prepareForEvo */
   return (init_ok);
} /* gen0_mix */



#ifdef MIT_LSV
/* ================================ gen0_lsv ================================ */
static BOOL gen0_lsv (BOOL  best)
/*----------------------------------------------------------------------------*/
/*   Generiert die Anfangspopulation der Groesse "pop_size" gemaess "best"    */
/*   wie folgt:                                                               */
/*   Kein "best": Initialisierung der Anfangspopulation durch "strat_par" zu- */
/*                faellig erzeugter und mit dem LSV des Jobs ("lsv_strat")    */
/*                verbesserter AKs. Der Rest wird zufaellig erzeugt und die   */
/*                Population durchgemischt.                                   */
/*   Mit "best":  Initialisierung der Anfangspopulation gemaess BEST_NEU un-  */
/*                ter Verwendung des "strat_par" und Verbesserung aller AKs   */
/*                mit dem LSV des Jobs ("lsv_strat).                          */
/*   Die zufaellige Erzeugung erfolgt durch "gen0_neu". Die erzeugte Popula-  */
/*   tion wird entweder durch "gen0_NBest" oder in dieser Routine durchge-    */
/*   mischt.                                                                  */
/*   Die mit dem LSV verbesserten AKs werden mit einer Wahrscheinlichkeit     */
/*   entsprechend dem package-globalen Parameter "lamarck_rate" angepasst.    */
/*   Die Routine liefert TRUE, wenn alles gut ging, sonst FALSE und weitere   */
/*   Fehlerinformationen in "mem_out" und "err_state".                        */
/*   MPI_PAR_VERS: Die Routine ist noch nicht fit fuer die Parallelversion !! */
/*----------------------------------------------------------------------------*/
{
   DOUBLE    akt_note, alt_note; 
   INT       rest_anz,                     /* noch zu erzeugende AKs          */
             opt_anz,                      /* mit dem LSV zu optimierende AKs */
             ii;
   BOOL      ok = TRUE,
             lamarck,
             lamarck_bestimmen = FALSE;

   /* gen0_lsv */
   if (best)
   { /* Init mit "strat_par" besten AKs */
      ok       = gen0_NBest(strat_par);
      rest_anz = 0;
      opt_anz  = pop_size;
   } /* Init mit "strat_par" besten AKs */
   else
   { /* Init mit "strat_par" neuer und lokal-optimierter AKs */
      ok       = gen0_neu (0, strat_par, 0, EGAL); 
      rest_anz = pop_size - strat_par; 
      opt_anz  = strat_par;
   } /* Init mit "strat_par" neuer und lokal-optimierter AKs */
  #ifdef MIT_GORBA_LSVS
   lamarck = TRUE;
  #else /* kein MIT_GORBA_LSVS */
   if (lamarck_rate == 0.0)
      lamarck = FALSE;
   else
      if (lamarck_rate == 1.0)
         lamarck = TRUE;
      else
         lamarck_bestimmen = TRUE;
  #endif /* MIT_GORBA_LSVS */
   for (ii = 0;  (ii < opt_anz) && ok;  ii++)
   { /* Indiv der Start-Pop lokal optimieren */
      if (lamarck_bestimmen)
         lamarck = frand () < lamarck_rate;
      alt_note = pop_feld[ii].note;
  #ifdef MIT_GORBA_LSVS  /*  Gorba-HyGLEAM: Fall E */
      vorGLSVstart = TRUE;
      if (do_appl_sim (simu_erg, pop_feld[ii].chain_ptr, CALC_FITNESS, &err_state)) 
      { /* AK wurde erfolgreich simuliert */
	 if (lsv_strat < LSV_RAS)
	    aktPlanDataToSave ();
	 else
	    aktAJobPlanDataToSave ();
	 resetBelMat ();                  /* vor "do_lsv_start" notwendig! */
	 vorGLSVstart = FALSE;            /* vor "do_lsv_start" notwendig! */
	 akt_note = do_lsv_start (&(pop_feld[ii].chain_ptr), TRUE, &err_state);
      } /* AK wurde erfolgreich simuliert */
      else { /* AK nicht simulierbar */
	 resetBelMat ();
	 vorGLSVstart = FALSE;
	 akt_note     = KEINE_NOTE;
      } /* AK nicht simulierbar */
  #else /* kein MIT_GORBA_LSVS */
      akt_note = do_lsv_start (&(pop_feld[ii].chain_ptr), lamarck, &err_state);
  #endif /* kein MIT_GORBA_LSVS */
      ok = akt_note != KEINE_NOTE;
      if (ok && (akt_note > alt_note))
      { /* Kein Fehler und Verbesserung */
         pop_feld[ii].note = akt_note;
         berechne_guete (&(pop_feld[ii].guete), akt_note);
         if (akt_note > gen_note)
            gen_note = akt_note;                 /* Note der besten AK merken */
         sprintf (buf3, STARTPOP_LSV_OK, alt_note, akt_note);
      } /* Kein Fehler und Verbesserung */
      else
         sprintf (buf3, STARTPOP_LSVNIX, akt_note);
      if (statistik_pro_gen)
         write_to_log (buf3, CLOSE_LOG);
   } /* Indiv der Start-Pop lokal optimieren */
   if (ok && !best)
   { /* LSV_NEU: Rest generieren und mischen */
      ok = gen0_neu (strat_par, rest_anz, 0, EGAL);         /* Rest zufaellig */
      mix_up_popul (pop_size - rest_anz);           /* Mischen der Population */
   } /* LSV_NEU: Rest generieren und mischen */
   return (ok);
} /* gen0_lsv */
#endif /* MIT_LSV */



#ifdef MPI_PAR_VERS
/* =============================== initSubPop =============================== */
void initSubPop (INT     pSize,
                 INT     optVerf,
		 INT     survivalRule,
		 INT     lsvCode,
		 DOUBLE  rankParam,
		 BOOL    mitAllVerb)
{
   /* ------------------------ Parameteruebernahme: ------------------------- */
   pop_size          = pSize;            /* nur bei ACCEPT_TST in Modul "evo" */
   act_opt_strategy  = optVerf;          /* Optimierungsverfahren             */
   act_survival_rule = survivalRule;     /* Akzeptanzregel fuer d. Nachkommen */
   lsv_strat         = lsvCode;          /* LSV oder Meme. Index in lsv[]     */
   max_fit           = rankParam;        /* Ranking-Parameter                 */

   /* ----------------------- abgeleitete Parameter: ------------------------ */
   max_pop_index = pop_size - 1;
   min_fitness   = 2.0 - max_fit;                   /* fuer "partnerwahl"     */
   fitness_delta = max_fit - min_fitness;           /* fuer "partnerwahl"     */
  #ifdef MIT_LSV
   gleamPur      = act_opt_strategy == GLEAM;
   gleam_asma    = act_opt_strategy == GLEAM_ASMA;
   gleam_amma    = act_opt_strategy == GLEAM_AMMA;
   gleam_adi     = gleam_asma || gleam_amma;
   if ((act_opt_strategy == GLEAM_SMA) || gleam_adi)
   { /* SMA, ASMA oder AMMA */
      lsv_all_children = mitAllVerb;
      lsv_best_child   = !lsv_all_children;
   } /* SMA, ASMA oder AMMA */
   else
  #endif /* MIT_LSV */
   { /* GLEAM oder LSV */
      lsv_best_child   = FALSE;
      lsv_all_children = FALSE;
   } /* GLEAM oder LSV */
} /* initSubPop */
#endif /* MPI_PAR_VERS */



#ifndef MPI_PAR_VERS   /* sequentielle Version mit symmetr. Ringnachbarschaft */
/* =========================== loe_deme_ham_werte =========================== */
void loe_deme_ham_werte (INT  indiv)
{
   INT  ii;

   /* loe_deme_ham_werte  */
   for (ii = 0;  ii < anz_nb_halbe;  ii++)
   { /* Hamming-Werte zuruecksetzen */
      pop_feld[indiv].deme_ham[ii]                       = NO_HAM;
      pop_feld[(indiv + ii + 1) % pop_size].deme_ham[ii] = NO_HAM;
   } /* Hamming-Werte zuruecksetzen */
} /* loe_deme_ham_werte */
#endif /* sequentielle Version mit symmetrischer Ringnachbarschaft */



/* ============================ do_g_operationen ============================ */
BOOL do_g_operationen (void)
{ 
   INT   g_op_anz,      /* Anzahl genetischer Operatoren der gen. Operation   */
         g_op_index,    /* Index des aktuellen genetischen Operators          */
         g_op_code;     /* Code des aktuellen genetischen Operators           */
   BOOL  g_op_done,     /* TRUE, wenn mind.ein gen.Operator ausgefuehrt wurde */
         akt_gop_done,  /* TRUE, wenn aktueller gen.Operator ausgefuehrt wurde*/
         do_loop,       /* FALSE, wenn XO-Operator keine Offsprings liefert   */
         ein_kind,      /* TRUE, wenn Mutation (erzeugt einen Offspring)      */
         abbruch,       /* TRUE, wenn Fehler und Abbruch                      */
         bdummy;

   /* do_g_operationen */
   anz_children = 0;                          /* offspring amount per pairing */
   do
   { /* Schleife aller gen. Operationen */
      if ((g_op_anz = calc_p_go (&ein_kind, akt_gen_op, akt_ptr->u_el.u_xel.u_hdr.
                                 fitness_note)) > 0)
      { /* Gen. Operation ausfuehren */
         if (ein_kind)
         { /* Kind als Kopie erzeugen */
            if (!copy_chain (&child_ptr, akt_ptr, KEINE_URKETTE, NO_MEM_CHECK, &err_state))
            { /* Evo-Abbruch wegen Kopier-Fehler */
               meld_evo_crash (EVO_WUERG_COPY_MELD, EVO_GSW, akt_gen_op);
               return (FALSE);
            } /* Evo-Abbruch wegen Kopier-Fehler */
         } /* Kind als Kopie erzeugen */
         g_op_done = FALSE;
         do_loop   = TRUE;
         for (g_op_index = 0;  (g_op_index < g_op_anz) && do_loop;  g_op_index++)
         { /* Loop ueber alle gen. Operatoren der gen. Operation */
            if (calc_p_op (&g_op_code, g_op_index))
            { /* Operator ausfuehren */
               akt_gop_done = TRUE;
               if (g_op_code < 0)
               { /* applikationsspezifischer genetischer Operator */
                  if (ein_kind)
                     akt_gop_done = do_appl_mut (child_ptr, abs(g_op_code)-1, &err_state);
                  else
                  { /* applikationsspezifisches XO */
                     akt_gop_done = do_appl_xo (&child_ptr, &child2_ptr, &err_state,
                                                akt_ptr, partner_ptr, 
                                                abs(g_op_code)-1, do_xo);
                     do_loop      = akt_gop_done;
                  } /* applikationsspezifisches XO */
               } /* applikationsspezifischer genetischer Operator */
               else
                  if (g_op_code <= LAST_GO)
                     switch (g_op_code) { /* Verteiler fuer Mutationen */
                     case PAR_CHANGE_NEW:
                        err_state = akt_par_neu (child_ptr);
                        break;

                     case PAR_CHANGE_REL:
                        err_state = akt_par_aend (&akt_gop_done, CHANGE_REL, child_ptr);
                        break;

                     case ACT_NEW_PARAM:
                        err_state = akt_neu (child_ptr);
                        break;

                     case SEGM_CHANGE_REL:
                        err_state = segm_par_aend (&akt_gop_done, CHANGE_REL,child_ptr);
                        break;

                     case SEGM_NEW_PARAM:
                        err_state = segm_neu (child_ptr);
                        break;

                     case ACT_EXCHANGE:
                        err_state = aktions_austausch (child_ptr);
                        break;

                     case ADD_NEW_ACT:
                        err_state = plus_neue_aktion (child_ptr);
                        break;

                     case DOUBLE_ACT:
                        err_state = aktion_verdoppeln (child_ptr);
                        break;

                     case DELETE_ACT:
                        err_state = aktion_loeschen (child_ptr);
                        break;

                     case ACT_TRANSLOCATION:
                        err_state = aktions_transl (child_ptr);
                        break;

                     case SEGM_EXCHANGE:
                        err_state = abschn_austausch (child_ptr);
                        break;

                     case DOUBLE_SEGM:
                        err_state = abschn_verdoppeln (child_ptr);
                        break;

                     case DELETE_SEGM:
                        err_state = abschn_loeschen (child_ptr);
                        break;

                     case SEGM_TRANSL:
                        err_state = abschn_transl (child_ptr);
                        break;

                     case SEGM_INVERSION:
                        err_state = abschn_inversion (child_ptr);
                        break;

                     case INTEGR_NACHB_SEGM:
                        err_state    = nachb_segm_integr (child_ptr);
                        akt_gop_done = FALSE; /*Mut aendert nur die Segmentierung*/
                        break;

                     case INTEGR_SEGM:
                        err_state    = segm_integr (child_ptr);
                        break;

                     case SCHIEB_SEGM_GRENZ:
                        err_state    = schiebe_segm_grenze (&bdummy, child_ptr);
                        akt_gop_done = FALSE; /*Mut aendert nur die Segmentierung*/
                        break;

                     case TEILE_SEGM:
                        err_state    = segm_teilung (&bdummy, child_ptr);
                        akt_gop_done = FALSE; /*Mut aendert nur die Segmentierung*/
                        break;

                     case ADD_NEW_SEGM:
                        err_state = add_new_segm (child_ptr);
                        break;

                     case PAR_CHANGE_SMALL:
                        err_state = akt_par_aend (&akt_gop_done, CHANGE_SMALL, child_ptr);
                        break;

                     case SEGM_CHANGE_SMALL:
                        err_state = segm_par_aend (&akt_gop_done, CHANGE_SMALL,child_ptr);
                        break;

                     default:
                        sprintf (buf1, GEN_OP_ERR_TXT, akt_gen_op, g_op_index);
                        err_state = fatal (EVO_GSW, 1, g_op_code, NO_INFO, buf1, 
                                           BAD_G_OP_CODE_ERR);
                        meld_evo_crash (EVO_WUERG_OPC_MELD, EVO_GSW, NO_INFO);
                        return (FALSE);
                     }  /* Verteiler fuer Mutationen */
                  else { /* Reko und XO */
                     if (do_xo)
                        switch (g_op_code) { /* Verteiler fuer Reko und XO */
                        case SEGM_CROSS_OVER:
                           akt_gop_done = abschn_cross_over (&child_ptr, &child2_ptr,
                                                             akt_ptr, partner_ptr,
                                                             &err_state);
                           break; /* SEGM_CROSS_OVER */

                        case CROSSOVER:
                           akt_gop_done = cross_over (&child_ptr, &child2_ptr,
                                                      akt_ptr, partner_ptr, &err_state);
                           break; /* CROSSOVER */

                        case RECOMB:
                           akt_gop_done = rekombination (&child_ptr, &child2_ptr,
                                                         akt_ptr, partner_ptr, &err_state);
                           break; /* RECOMB */

                        case OX_XO:
                        case OX_RECO:
                        case PPX_XO:
                        case PPX_RECO:
                           akt_gop_done = orderRekoXO (&child_ptr, &child2_ptr,
                                                       akt_ptr, partner_ptr, 
                                                       g_op_code, &err_state);
                           break; /* OX_XO, OX_RECO, PPX_XO oder PPX_RECO */

                        default:
                           sprintf (buf1, GEN_OP_ERR_TXT, akt_gen_op, g_op_index);
                           err_state = fatal (EVO_GSW, 2, g_op_code, NO_INFO, buf1, 
                                              BAD_G_OP_CODE_ERR);
                           meld_evo_crash (EVO_WUERG_OPC_MELD, EVO_GSW, NO_INFO);
                           return (FALSE);
                        } /* Verteiler fuer Reko und XO */
                     else
                        akt_gop_done = FALSE;
                     do_loop = akt_gop_done;
                  } /* Reko und XO */
               g_op_done |= akt_gop_done;
               if (err_state != GLEAM_OK)
               { /* Evo-Abbruch wegen Fehler bei gen. Operator */
                  meldung (EVO_GSW, akt_gen_op, g_op_code, "", EVO_WUERG_INFO_MELD);
                  meld_evo_crash (EVO_WUERG_GOP_MELD, EVO_GSW, NO_INFO);
                  return (FALSE);
               } /* Evo-Abbruch wegen Fehler bei gen. Operator */
            } /* Operator ausfuehren */
         } /* Loop ueber alle gen. Operatoren der gen. Operation */
         if (g_op_done)
         { /* Mind. ein Operator wurde ausgefuehrt */
            if (ein_kind)
               abbruch = !pruef_und_speichere (&child_ptr, akt_gen_op);
            else
               abbruch = !compute_results (&child_ptr, &child2_ptr, akt_gen_op);
         } /* Mind. ein Operator wurde ausgefuehrt */
         else 
         { /* Nix getan */
            if (ein_kind)
               abbruch = ((err_state = delete_chain (&child_ptr)) != GLEAM_OK);
            else
               abbruch = FALSE;
         } /* Nix getan */
         if (abbruch)
         { /* Evo-Abbruch wegen Fehler bei Offspring-Bearbeitung */
            meld_evo_crash (EVO_WUERG_OFFSPR_MELD, EVO_GSW, akt_gen_op);
            return (FALSE);
         } /* Evo-Abbruch wegen Fehler bei Offspring-Bearbeitung */
      } /* Gen. Operation ausfuehren */
      akt_gen_op++;
/* ------ deaktiviert, siehe Doku zu Punkt 10 ----------------
#ifdef GLEAM_USR
      taste = kb_hit ();
#endif 
   } 
   while (!taste && (akt_gen_op < go_anz));
----------------------------------------------------------- */
   } /* Schleife aller gen. Operationen */
   while (akt_gen_op < go_anz);
   return (TRUE);
} /* do_g_operationen */



/* ============================= compute_results ============================ */
static BOOL compute_results (CHAIN_ELEM_PTR_TYPE *c1_ptr, 
                             CHAIN_ELEM_PTR_TYPE *c2_ptr, 
                             INT                  mut_index)
/*----------------------------------------------------------------------------*/
/*  Die Routine fuehrt bei Ketten fester Laenge fuer die Nachkommen "c1_ptr"  */
/*  und "c2_ptr" das Genetic Repair fuer 2 AKs durch bzw. verzweigt bei ADD_  */
/*  APPL nach "add_repair_2_chs". Danach erfolgt mit "pruef_und_speichere"    */
/*  der Plausibilitaetstest (PTest) mit Genetic Repair für eine AK und die    */
/*  Bewertung. Auftretende Fehler werden als Funktionsergebnis und in         */
/*  "err_state" zurueckgeliefert.                                             */
/*----------------------------------------------------------------------------*/
{
   /* compute_results */
   if (appl_code == ADD_APPL)
      err_state |= add_repair_2_chs (*c1_ptr, *c2_ptr);
   else
      if (!dyn_aks)
         err_state |= repair_2_aks (*c1_ptr, *c2_ptr);   /* AK fester Laenge */
   pruef_und_speichere (c1_ptr, mut_index);
   pruef_und_speichere (c2_ptr, mut_index);
   return (err_state == GLEAM_OK);
} /* compute_results */



/* ================================= save_ep ================================ */
void save_ep (void)
{
   BOOL  init_ok;
   INT   ii;

   /* save_ep */
   init_ok = open_ch_file (EVO_SCRATCH_FILE, 1, CH_WRITE, GLOBAL_SAVE, chrFileIOMode);
   if (init_ok)
   { /* File offen */
      ii = 0;
      while (init_ok && (ii < pop_size))
         init_ok = save_chain (pop_feld[ii++].chain_ptr, FALSE);
      close_ch_file ();
      sprintf(buf1, CHR_SAVE_TXT, pop_size, ii);
      write_buf(get_line_anz()+1, buf1);
   } /* File offen */
   if (!init_ok)
      meldung (EVO_GSW, NO_INFO, NO_INFO, "", EVO_SCRATCH_ERR_MELD);
} /* save_ep */



/* ============================== store_res_ch ============================== */
GSTATE store_res_ch (INT *lfd_nr,
                     INT  idx,
                     INT  ketten_art)
{
   GSTATE  ret_state;

   /* store_res_ch */
   if (pop_feld[idx].new_chain)
   { /* veränderte AK, also retten */
      (akjob_stat_dat.anz_res_chs)++;
      ret_state  = store_chain (lfd_nr, pop_feld[idx].chain_ptr, ketten_art,
                                pop_feld[idx].guete);
      ret_state |= delete_chain (&(pop_feld[idx].chain_ptr));
      store_in_evo_erg (*lfd_nr, pop_feld[idx].guete, pop_feld[idx].note, ketten_art);
      pop_feld[idx].new_chain = FALSE;
      pop_feld[idx].guete     = KEINE_GUETE;
      pop_feld[idx].note      = KEINE_NOTE;
   } /* veränderte AK, also retten */
   else
      ret_state = GLEAM_OK;
   return (ret_state);
} /* store_res_ch */



/* ================================ get_best ================================ */
INT get_best (INT     anz_erg_ketten,
              BOOL    mit_anzg,
              GSTATE *ret_state)
{
   INT  number, save_anz, offset, niche_size, j_index, 
        i_index, ii, jj, kk;
  
   /* get_best */
   if (mit_anzg)
      reset_text_buf ();
   save_anz = 0;                              /* noch keine Kette gespeichert */
   if (anz_erg_ketten > 0)
   { /* Es ist etwas abzuliefern */
      /* ------- Suche bestes veraendertes Individuum der Population: ------- */
      jj = 0;
      while (!pop_feld[jj].new_chain && (jj < pop_size))
         jj++;
      if (jj < pop_size)     
      { /* veraenderte Kette gefunden */ 
         /* ---------------- Suche bessere veraenderte Kette: --------------- */
         for (ii = jj+1;  ii <= (pop_size - 1);  ii++) 
            if ((pop_feld[ii].note > pop_feld[jj].note) && (pop_feld[ii].new_chain))
               jj = ii;
         *ret_state |= store_chain (&number, pop_feld[jj].chain_ptr, AK, pop_feld[jj].guete);
         store_in_evo_erg (number, pop_feld[jj].guete, pop_feld[jj].note, AK);
         pop_feld[jj].new_chain = FALSE;
         save_anz++;
         if (mit_anzg)
         { /* Melde AK im interaktiven Modus */
            sprintf (buf1, GET_BEST_SAVTXT, jj, pop_feld[jj].note, 
                     pop_feld[jj].guete, number);
            write_buf (save_anz, buf1);
         } /* Melde AK im interaktiven Modus */
      } /* veraenderte Kette gefunden */
      else
         return (0);               /* es gibt ueberhaupt keine veraenderte AK */

      /* --- Suche und speichere die besten Indivs der jeweiligen Nische: --- */
      niche_size = pop_size / anz_erg_ketten;
      offset     = jj + niche_size / 2;
      for (kk = 1;  kk <= (anz_erg_ketten-1);  kk++) 
      { /* Bearbeite die naechste Nische */
         jj = 0;
         while (!(pop_feld[(jj+offset) % pop_size].new_chain) && (jj < niche_size)) 
            jj++;
         if (jj < niche_size) 
         { /* veraenderte Kette gefunden */
            for (ii = jj+1;  ii <= (niche_size-1);  ii++) 
            { /* suche Nische */
               i_index = (ii+offset) % pop_size;
               if ((pop_feld[i_index].new_chain) && 
                   (pop_feld[i_index].note > pop_feld[(jj+offset) % pop_size].note))
                  jj = ii;
            } /* suche Nische */
            j_index = (jj+offset) % pop_size;
            if (pop_feld[j_index].new_chain)
            { /* Die AK gibt es */
               if ((*ret_state |= store_chain (&number, pop_feld[j_index].chain_ptr, 
                                               AK, pop_feld[j_index].guete))
                   != GLEAM_OK)
                  return (save_anz);                         /* Fehlerausgang */
               store_in_evo_erg (number, pop_feld[j_index].guete, 
                                 pop_feld[j_index].note, AK);
               pop_feld[j_index].new_chain = FALSE;
               save_anz++;
               if (mit_anzg)
               { /* Melde AK im interaktiven Modus */
                  sprintf (buf1, GET_BEST_SAVTXT, j_index, pop_feld[j_index].note, 
                           pop_feld[j_index].guete, number);
                  write_buf (save_anz, buf1);
               } /* Melde AK im interaktiven Modus */
            } /* Die AK gibt es */
            offset += niche_size;
         } /* veraenderte Kette gefunden */
      }  /* Bearbeite die naechste Nische */
   } /* Es ist etwas abzuliefern */

   for (ii = 0;  ii <= max_pop_index;  ii++)
      if ((!pop_feld[ii].new_chain) && (pop_feld[ii].chain_ptr != NULL))
      { /* Loesche alle alten und bereits gespeicherten Ketten */
         *ret_state |= delete_chain (&(pop_feld[ii].chain_ptr));
         pop_feld[ii].guete = KEINE_GUETE;
         pop_feld[ii].note  = KEINE_NOTE;
      } /* Loesche alle alten und bereits gespeicherten Ketten */

   return (save_anz);
} /* get_best */



/* ============================ store_in_evo_erg ============================ */
void store_in_evo_erg (INT     number, 
                       INT     guete,  
                       DOUBLE  note,
                       INT     ketten_art)
{
   INT  ind;

   /* store_in_evo_erg */
   ind = (last_evo_erg[ketten_art].last_evo_index + 1) % MAX_EVO_ERG;
   last_evo_erg[ketten_art].evo_erg_feld[ind].guete = guete; 
   last_evo_erg[ketten_art].evo_erg_feld[ind].note  = note; 
   last_evo_erg[ketten_art].evo_erg_feld[ind].lfdnr = number;
   last_evo_erg[ketten_art].last_evo_index = ind;
} /* store_in_evo_erg */



/* ============================ check_for_lamarck =========================== */
BOOL check_for_lamarck (DOUBLE  lamarck_rate)
{
   BOOL  lamarck;

   /* check_for_lamarck */
   if (lamarck_rate == 1.0)
      lamarck = TRUE;
   else
      if (lamarck_rate == 0.0)
         lamarck = FALSE;
      else
         lamarck = frand () < lamarck_rate;
   return (lamarck);
} /* check_for_lamarck */



/* =============================== partnerwahl ============================== */
DOUBLE *partnerwahl (INT     no,
                     INT    *partnerIdx,
                     DOUBLE *weakest_note,
                     DOUBLE *best_note)
{
   DEME_TYPE             tmp;
   INT                   delta, min, ii, jj;
   DOUBLE                z_zahl, sum, xpt,
                        *hamWert;
   struct DEME_TYPE     *nb_ptr;         /* Zeiger auf Deme eines Individuums */
  
   /* partnerwahl */
   nb_ptr = pop_feld[no].nachbarn;
#ifdef DEME_TST
  #ifdef MPI_PAR_VERS
   if (symRing)
      printf ("partnerwahl %s: pop_size=%d anz_nachbarn=%d max_nb_index=%d anz_nb_halbe=%d"
	      "  min_fitness=%g fitness_delta=%g\n", 
	      nodeBuf, pop_size, anz_nachbarn, max_nb_index, anz_nb_halbe, 
	      min_fitness, fitness_delta);
   else
      printf ("partnerwahl %s: pop_size=%d anz_nachbarn=%d max_nb_index=%d "
	      "  min_fitness=%g fitness_delta=%g\n", 
	      nodeBuf, pop_size, anz_nachbarn, max_nb_index, 
	      min_fitness, fitness_delta);
  #endif /* MPI_PAR_VERS */
   printf ("%s\n", "partnerwahl:");
   sprintf (buf1, "  Indiv=%d  Deme:", no);
   for (ii = 0; ii < anz_nachbarn;  ii++)
   {
      sprintf (buf2, " %d/%g", nb_ptr[ii].nbPtr->index, nb_ptr[ii].nbPtr->note);
      strcat (buf1, buf2);
   }
   printf ("%s\n", buf1);
#endif /* DEME_TST */   
      
   /* ------------------ Sortiere Nachbarschaft nach Note: ------------------ */
   for (ii = 0;  ii < max_nb_index;  ii++) 
   { /* Selection Sort */
      min = ii;
      for (jj = ii+1;  jj < anz_nachbarn;  jj++) 
         if (nb_ptr[jj].nbPtr->note < nb_ptr[min].nbPtr->note)
            min = jj;
      if (min != ii)
      { /* Vertauschen */
         tmp         = nb_ptr[min];
         nb_ptr[min] = nb_ptr[ii];
         nb_ptr[ii]  = tmp;
      } /* Vertauschen */
   } /* Selection Sort */

   /* ------------------------- Selektiere Partner: ------------------------- */
   z_zahl = frand ();
   ii     = 0;
   sum    = 0;
   xpt    = 0;

   do 
   { /* Ranking */
      xpt = min_fitness + (fitness_delta)*((double)(ii)/(double)max_nb_index);
      ii++;
   } /* Ranking */
   while ((sum += xpt/anz_nachbarn) < z_zahl);

   *weakest_note = nb_ptr[0].nbPtr->note;
   *best_note    = nb_ptr[max_nb_index].nbPtr->note;
   *partnerIdx   = nb_ptr[ii-1].nbPtr->index;
#ifdef MPI_PAR_VERS
   if (symRing)
#endif /* MPI_PAR_VERS */
   { /* symmetrische Ring-Topologie */
      delta = no - *partnerIdx;
      if (abs(delta) > anz_nb_halbe) {
	 if (no < anz_nb_halbe)
	    delta += pop_size;
	 else
	    delta -= pop_size;
      }
      if (delta > 0)
	 hamWert = &(pop_feld[no].deme_ham[delta - 1]);
      else
	 hamWert = &(pop_feld[*partnerIdx].deme_ham[-delta - 1]);
   } /* symmetrische Ring-Topologie */
#ifdef MPI_PAR_VERS
   else
   { /* Parallel-Version und allg. Topologie */
      if (*partnerIdx > no)
	 hamWert = &(pop_feld[no].deme_ham[nb_ptr[ii-1].hamDistIdx]);
      else
	 hamWert = &(pop_feld[nb_ptr[ii-1].nbPtr->index].deme_ham[nb_ptr[ii-1].hamDistIdx]);
   } /* Parallel-Version und allg. Topologie */
#endif /* MPI_PAR_VERS */
#ifdef DEME_TST
   sprintf (buf1, "  Partner=%d PartnerNote=%g Rang=%d/%d   "
	    "local_weak=%g local_best=%g  HamDist=%g",
            *partnerIdx, nb_ptr[ii-1].nbPtr->note, ii-1, max_nb_index,
	    *weakest_note, *best_note, *hamWert);
   printf ("%s\n", buf1);
   strcpy (buf1, "  Deme:");
   for (ii = 0; ii < anz_nachbarn;  ii++)
   {
      sprintf (buf2, " %d/%g", nb_ptr[ii].nbPtr->index, nb_ptr[ii].nbPtr->note);
      strcat (buf1, buf2);
   }
   printf ("%s\n", buf1);   
#endif /* DEME_TST */
   return (hamWert);
} /* partnerwahl */



/* ============================ clear_mut_op_ctr ============================ */
void clear_mut_op_ctrs (void)
{
   INT  ii;

   for (ii = 0;  ii < ANZ_PAAR_OPS;  ii++)
      mut_op_ctr[ii] = 0;
} /* clear_mut_op_ctrs */



/* ============================= meld_evo_crash ============================= */
void meld_evo_crash (INT         meld_nr,
                     const char *mod_id,
                     LONG        info)
{
   get_time_stamp (buf3, buf1, buf3, buf2);
   strncpy (buf1, buf1, 6);
   buf1[6] = '\0';
   sprintf (buf3, EVO_CRASH_TIME, buf1, buf2, akt_indiv);
   meldung (mod_id, info, NO_INFO, buf3, meld_nr);
   taste = FALSE;
   if (best_child_ptr != NULL)
      err_state |= delete_chain (&best_child_ptr);
} /* meld_evo_crash */



/* =========================== externalTermRequest ========================== */
BOOL externalTermRequest (void)
{
   BOOL  result;

   /* externalTermRequest */
   result = FALSE;
   if (with_evo_term_tst)
      if (((pop_size*gen_counter + akt_indiv) % evo_tst_frequ) == 0)
	 if (file_exists(EVO_ABBRUCH_FSPEC))
         { /* external termination for command line version */
	    sprintf (buf3, EXT_EVO_FIN_TXT, gen_counter, akt_indiv, evo_tst_frequ);
	    write_to_log (buf3, CLOSE_LOG);
	    result    = TRUE;
	    akt_indiv = max_pop_index;                         /* -> Gen-Ende */
	    accepted  = FALSE;                                 /* -> Pop-Ende */
	    zielnote  = 0;                                     /* -> Job-Ende */
	 } /* externnal termination for command line version */
   return (result);
} /* externalTermRequest */



/* ================================ evo_init ================================ */
BOOL evo_init (void)
{
   INT   gesAnz,
         ii, ij;
   LONG  indiv_type_size;
   BOOL  erg = TRUE;

   /* evo_init */
   indiv_type_size = sizeof(INDIVID_TYPE);
   if (((last_evo_erg = (EVO_ERG_TYP*)malloc(MAX_EVO_TYPEN * sizeof(EVO_ERG_TYP))) == NULL) ||
       ((pop_feld     = (INDIVID_TYPE*)malloc(MAX_POPEL_SIZE * indiv_type_size))   == NULL)   )
      return (FALSE);
#ifdef MIT_LSV
   if (((child_ak_list = (CHAIN_ELEM_PTR_TYPE*)malloc(ANZ_PAAR_OPS * 2 *
              sizeof(CHAIN_ELEM_PTR_TYPE))) == NULL))
      return (FALSE);
#endif /* MIT_LSV */
#ifdef MPI_PAR_VERS
   gesAnz = MAX_ANZ_NACHBARN;             /* alle Nachbarn, die es geben kann */
#else /* sequentielle Version mit symmetrischer Ringnachbarschaft */
   gesAnz = MAX_NB_HALBE;    /* die Haelfte aller Nachbarn, die es geben kann */
#endif /* sequentielle Version mit symmetrischer Ringnachbarschaft */

   for (ii = 0;  ii < MAX_POPEL_SIZE;  ii++)
   { /* Leerung des Populationsfelds und Hinterlegung des Index */
      pop_feld[ii].chain_ptr = NULL;
      pop_feld[ii].index     = ii;
      for (ij = 0;  ij < gesAnz;  ij++)
         pop_feld[ii].deme_ham[ij] = NO_HAM;         /* Hamming-Wert loeschen */
   } /* Leerung des Populationsfelds und Hinterlegung des Index */
   best_child_ptr    = NULL;        /* Es wurde noch kein Offspring erzeugt   */
   anz_nb_halbe      = 0;           /* Es gibt noch gar                       */
   pop_size          = 0;           /* keine Population                       */
   intrpt_at_gen_end = FALSE;       /* noch keine Unterbrechung eines OptJobs */
   jobInterruptPhase = NULL_PHASE;  /* no phase of job execution              */
   loesche_evo_erg ();
   clear_mut_op_ctrs ();


   /* ------------------------ Default-Einstellungen: ----------------------- */
   initZufGen            =  TRUE;          /* Jobweise Init.d.Zufallszahlengen*/
   withAllImprovement    = FALSE;          /* default                         */
   max_pop_index         =    -1;          /* Das Populationsfeld ist leer    */
   def_opt_strat         = GLEAM;          /* Default-Optimierungsverfahren   */
   act_opt_strategy      = GLEAM;          /* aktuelles Optimierungsverf.     */
   defInitStrat          =   NEU;          /* Defaultwerte der EvoJobs        */
   defInitStratPar       =     1;          /* Defaultwerte der EvoJobs        */
   def_psize             =   120;          /* Defaultwerte der EvoJobs        */
   anz_nachbarn          =     8;          /* Defaultwerte der Evolution      */
   defGenLimit           =  1000;          /* default for generation limit    */
   defGAcLimit           =   100;          /* def.:limit of gen.w/o acceptance*/
   defGDI_Limit          =   400;          /* def.:limit of gen.w/o deme impr.*/
   defTimeLimit          =  3600;          /* def.: job time limit seconds    */
   defEvalLimit          = INT_MAX;        /* def.: limit of evaluations      */
   defResultChrs         =     1;          /* Defaultwerte der Evolution      */
   xo_min_ham_dist       = 0.001;          /* aktuelle MinHamDist fuer XO/Reko*/
   defTargetFitness      = RNOTEN_MAX;     /* default for target fitness      */
   def_xo_min_ham_dist   = 0.001;          /* Defaultwerte der EvoJobs        */
   def_max_fit           = 1.4;            /* Defaultwerte der Evolution      */
   mindest_note          = 0.0;            /* akzeptiere jede plausible AK    */
   def_lamarck_rate      = 1.0;            /* alle Gorba-LSVs passen die AK an*/
   default_survival_rule = LOCAL_LEAST_ES; /* Defaultwerte der EvoJobs        */
   act_survival_rule     = LOCAL_LEAST_ES; /* aktuelle Ueberlebensregel       */
   def_lsv_strat         = KEIN_LSV;       /* Default-LSV in aktive_lsv       */
   save_rate_def         = INT_MAX;        /* i.e., backups are switched off  */
   with_evo_term_tst     = FALSE;          /* CLV: Default fuer Jobabbruchtest*/
   evo_tst_frequ         = 500;            /* CLV:Alle 500 Indivs EvoAbbrTests*/
   prepareForEvo         = TRUE;           /* TRUE: no PAR_POP_VERS initializ.*/
   selectedChrAmount     = 0;

#ifdef MIT_LSV                             /* CLV: Command Line Version       */
   if ((defInitStrat == NEU) || (defInitStrat == MIX))
      defInitStratPar = 0;   /* minimal fitness for init. strats. NEU and MIX */
   erg  = init_lsv_steu();
   erg &= init_adapt_di();
#else /* kein MIT_LSV */
   oldDistribFrac = 0.3;           /* die eigentliche Init. ist in "adapt_di" */
#endif /* kein MIT_LSV */

#ifdef EXT_SIMU_SERV
   chrList      = NULL;
#endif /* EXT_SIMU_SERV */
#if defined(PAR_POP_VERS)
   chrFileIOMode = FILE_TXT_IO;
#else /* no PAR_POP_VERS */
   chrFileIOMode = FILE_BIN_IO;
#endif /* no PAR_POP_VERS */

   return (erg);
} /* evo_init */
