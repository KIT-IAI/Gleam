/*******************************************************************************
HyGLEAM/A                                                                 V3.2.2

                                  H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: ADAPTive Direkte Integration
 
Package: evo                    File: adapt_di.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

In der Version 2 wird eine relative "fitness gain" und notenklassen-abhaengige 
Adaption eingefuehrt. 
Beim relativen Fitnesszuwachs "fg_rel" wird der absolute Zuwachs "fg" in Rela-
tion zum Moeglichen gesetzt:
                              fg_rel = fg / (RNOTEN_MAX - f_alt)
Dabei ist "f_alt", die AK-Note vor der lokalen Optimierung.
Die Idee der notenklassen-abhaengigen Adaption beruht darauf, dass das, was 
fuer schlechte AKs gut ist, nicht gut sein muss fuer gute AKs und umgekehrt.

In der Version 3 wird die Implementierung von 2 LSVs auf n LSVs erweitert. Das
aktuell verwendete LSV steht in der package-globalen Variable "lsv_strat" und 
wird nicht mehr als Parameter herumgereicht. Die Variable wird zu Jobbeginn mit 
dem Wert der "lsv_strategy" aus den Jobdaten initialisiert.
Im Kontext der Erweiterung werden folgende Begriffe eingefuehrt:
  aktives LSV:     implementiertes LSV, das gemaess Genmodell verwendet wird.
                   Anzahl steht in "lsv_anz".
  beteiligtes LSV: aktives LSV, d.h. mit p(LSV)>0 u. lsv[idx]lsvOn ist TRUE.
  benutztes LSV:   beteiligtes LSV, dessen lsvCtr > 0 ist.

Die Aktivierung und Parametrierung der LSVs erfolgt ueber das MOD-File. Ausser-
dem wird die LSV-Auswahl dahingehend veraendert, dass ein Anteil von OLD_DISTR_
FRAC der alten Verteilung zusammen mit einem Anteil von (1 - OLD_DISTR_FRAC) der
neu berechneten die endgueltig neue Verteilung bildet. Die Zaehler-Limits zur 
LSV-Auswahl und fuer die Level-Anpassung werden in Abhaengigkeit der beteiligten 
LSVs definiert ind dynamisch angepasst.

Anfaenglich sind nur die LSV-Wahrscheinlichkeiten und die Level der schlechte-
sten AK-Klasse (Notenklasse 0) vorbesetzt. Sobald eine AK auf eine Klasse ohne 
Werte trifft, wird die naechstschlechteste Klasse mit Werten gesucht und deren 
Werte als Startwerte uebernommen. Dabei erfolgt der Update fuer die LSV-Vertei-
lung getrennt von den LSV-spezifischen Updates fuer die Level (LSV-Parameter 
und allP).

Moeglichkeiten zur (teilweisen) Abschaltung der Adaption:
1.: Ein  LSVs kann abgeschaltet werden, indem man ihm eine Wahrscheinlichkeit
    von 0.0 gibt.
2.: Ein Level kann fixiert werden, indem man ihm die Wahrscheinlichkeit 1.0 
    gibt. Das muss nicht notwendig der 1.Level sein, wenn man Werte am Ende der
    Skala nutzen will. Es ist aber geringfuegig schneller.


all/best fuer Version 2:
best: Die Optimierung des besten Nachkommen einer Paarungen ist geblieben wie 
      bisher.
all:  Bei der Optimierung aller Nachkommen einer Paarungen wird die Wahrschein-
      lichkeit, mit der die Nachkommen per LSV verbessert werden, adaptiv ge-
      staltet, wobei generell der beste Nachkomme immer LSV-verbessert wird.
      Die Adaption erfolgt fuer das Ergebnis einer Paarung getrennt nach ver-
      wendetem LSV, wobei das gewaehlte LSV fuer alle Nachkommen einer Paarung
      gleich ist. Die adaptiven LSV-Parameter werden fuer jeden Nachkommen ent-
      sprechend seiner Note neu bestimmt. Es fallen somit hinsichtlich der
      LSV-Wahl weniger Adaptionen an als bei der LSV-Parameteradaption. 
      Die Fitness-Verbesserung "fg" ergibt sich aus der Differerenz zwischen
      der besten LSV-Note und der besten Evo-Note. Wenn das LSV-Beste nicht 
      auch das Evo-Beste ist, die "all"-Regel also zu einer Verbesserung gegen-
      ueber reinem "best" gefuehrt hat, wird diese Fitnessdifferenz dem "fg" 
      als Bonus zugeschlagen. Die zugehoerigen Variable beginnen mit "allP".
      Das Level 0.0 entspricht der bisherigen best-Optimierung und das Level
      1.0 der alten all-Optimierung.

Aufrufsequenzen bei best und all, ASMA und AMMA:
  best:  IF (ASMA)
            akt_nkl = get_nklasse (best_child_note)  // lsv_strat aus Jobdaten
         ELSE
            set_actual_adi_lsv (&akt_nkl, best_child_note)
         parameterise_lsv_for_best (akt_nkl)
         do_lsv_start (&best_child_ptr, lamarck)
         lsv_erg_fuer_adi (akt_nkl, fg, f_alt, aufwand)

  all:   IF (ASMA)
            all_p_nkl = get_nklasse (best_child_note)  // lsv_strat aus Jobdaten
         ELSE
            set_actual_adi_lsv (&all_p_nkl, best_child_note)
         get_lsv_p (all_p_nkl)
         LOOP alle gemaess all-P zu verbessernden Nachkommen
            parameterise_lsv_for_all (&akt_nkl, f_alt)
            do_lsv_start (&(child_ak_list[ii]), lamarck)
            lsv_erg_fuer_adi (akt_nkl, fg, f_alt, aufwand)
         END_LOOP
         all_erg_fuer_adi (all_p_nkl, fg, best_evo_note, aufwand, 
                           lsv_anw_ist_rate)


Die Routinen im einzelnen:

void   reset_adi (void);
          Setzt die Zaehler und die internen Datenstrukturen der adaptiven 
          direkten Integration fuer einen neuen Lauf zurueck. Dabei werden fol-
          gende Initialwerte aus den lsv[]-Daten benutzt:
            parLevelAnz[parIdx]      Anfaengliches kleinstes Level } eines
            parIniP1stLev[parIdx]    } Anfaenglicher Wert des      } LSV-
            parIniP2ndLev[parIdx]    } 1. und 2. Levels            } Parameters
            allPStartLevel           Anfaengliches kleinstes Level } fuer LSV-
            allPIniP1stLev           } Anfaenglicher Wert des      } Wahrsch.bei
            allPIniP2ndLev           } 1. und 2. Levels            } all-Optim.
            lsvStart                 Anfaengliche Wahrscheinlichkeit eines LSVs

INT    get_nklasse (DOUBLE  note);
          Liefert den Index der zur "note" gehoerigen Notenklasse ab.

INT    getActNKlAnz (void);
          Liefert den aktuellen Wert von "nklassen_anz" ab.

void   set_actual_adi_lsv (INT    *this_nkl,
                           DOUBLE  akt_note);
          Hinterlegt den Code (Index in lsv[]) fuer das aktuell ausgewaehlte LSV
          ausgehend von der aktuellen Note "akt_note" in "lsv_strat". Ausserdem 
          wird die zu "akt_note" gehoerige allg. Notenklasse in "this_nkl" abge-
          liefert. Wenn dabei auf eine unbenutzte Notenklasse gestossen wird, 
          werden die Werte der naechstniedrigen uebernommen. Steuerung ueber 
          die allg. Notenklassen (nklasse_valid).

GSTATE parameterise_lsv_for_all (INT    *this_nkl,
                                 DOUBLE  akt_note);
          Aus "akt_note" wird die aktuelle Notenklasse bestimmt und in "this_
          nkl" abgeliefert. Mit dieser Notenklasse wird "parameterise_lsv_for_
          best" aufgerufen und dessen Returnstatus zurueckgeliefert.
          Die neu bestimmte Notenklasse kann unbenutztet sein: Durch den vor-
          herigen Aufruf von "get_lsv_p" ist lediglich sichergestellt, dass 
          die zur "best_child_note" gehoerige Notenklasse aktiviert ist. 
          "akt_note" kann nur geringer sein und zu einer noch nicht aktivierten 
          (Luecke). Daher muss "lsv[].nklValid" abgefragt werden, siehe 
          "parameterise_lsv_for_best".

GSTATE parameterise_lsv_for_best (INT  akt_nkl);
          Die Routine setzt die Strategieparameterwerte des durch "lsv_strat" 
          bezeichneten LSVs, die zu "akt_nkl" gehoeren, neu, wenn sich die neuen
          Werte gegenueber der letzten Verwendung des betreffenden LSVs veraen-
          dert haben.
          Die durch den vorherigen Aufruf von "set_actual_adi_lsv" erhaltene
          Notenklasse "this_nkl" kann im LSV-Par-/all-P-Kontext noch nicht
          aktiviert sein und muss gegebenenfalls aktiviert werden.
          Der zurueckgelieferte Statuswert wird bei auftretenden Problemen 
          schlecht.
          Wenn es sich bei "lsv_strat" um das Rosenbrockverfahren oder den Com-
          plex-Algorithmus handelt, erfolgt die Neuparametrierung mit Hilfe von 
          "set_rc_limits", deren Statuswert bei I/O-Problemen schlecht wird.

void   lsv_erg_fuer_adi (INT     akt_nkl,
                         DOUBLE  fg,
                         DOUBLE  f_alt,
                         LONG    aufwand);
          Das durch "lsv_strat" bezeichnete aktuelle LSV hat den Fitnesszuwachs 
          "fg" mit dem Aufwand "aufwand" erreicht. Der relative Fitnesszuwachs 
          wird berechnet durch:
              fg_rel = fg / (RNOTEN_MAX - f_alt)
          wobei "f_alt" die Fitnes vor der lokalen Verbesserung ist. Die neue
          Verteilung ergibt sich zu einem Anteil von OLD_DISTR_FRAC aus der
          alten und zu einem Anteil von 1-OLD_DISTR_FRAC aus der neu berechne-
          ten.
          Es erfolgt ein Update der internen Zaehler und eine adaptive Anpassung 
          der LSV-Verteilung und/oder der Level, falls folgendes zutrifft:
          LSV-Verteilung: 
            Eine Adaption erfolgt, wenn jedes LSV mindestens "lsv_einzel_
            limit[akt_nkl]" mal angewandt wurde oder beide mindestens insgesamt 
            "lsv_gesamt_limit[akt_nkl]" mal angewandt wurden UND die Summe aller 
            Verbesserungen groesser als 0 ist. Wenn ein oder mehrere LSVs keine 
            Anwendungen hatten, werden nur die Wahrscheinlichkeiten der verblei-
            benden angepasst. Wenn dabei die Wahrscheinlichkeit eines LSVs erst-
            mals unter "lsvPMin[akt_nkl]" sinkt, wird dies mitgezaehlt und bei 
            zusaetzlicher Unterschreitung von P_MIN auf P_MIN angehoben. Wenn 
            sie "lsv_nix_anwendung_limit" mal hintereinander unter "lsvPMin[akt_
            nkl]" liegt, wird das LSV abgeschaltet.
            Die Parametrierung der LSV-Adaptionsgeschwindigkeit ist abhaengig von
            der Anzahl der beteiligten LSVs. Da diese in den verschiedenen Noten-
            klassen variieren kann, werden auch die Grenzen pro Notenklasse ge-
            speichert.
          Level-Adaption:
            Eine Adaption erfolgt, wenn entweder insgesamt mindestens "level_
            gesamt_limit" Anwendungen stattfanden oder zwei Levels mindestens
            je "level_einzel_limit" Anwendungen hatten. Wenn ein Level keine 
            Anwendung hatte, werden nur die verbleibenden Level angepasst. Kein 
            Level darf unter einen Anteilvon MIN_LEVEL_P sinken.
            Die Level-Adaptionsgeschwindgkeit ist unabhaengig von der Anzahl der
            LSV und damit der Notenklasse.

DOUBLE get_lsv_p (INT  akt_nkl);
          Ausgehend von der aktuellen Notenklasse "akt_nkl" und dem zu "lsv_
          strat" gehoerigem Level-Feld wird die Wahrscheinlichkeit zur lokalen 
          Verbesserung der Nachkommen bei adaptivem "all" bestimmt und abge-
          liefert.

void   all_erg_fuer_adi (INT     akt_nkl,
                         DOUBLE  fg,
                         DOUBLE  f_alt,
                         LONG    aufwand,
                         DOUBLE  lsv_ist_rate);
          Bestimmung des relativen Fitnesszuwachses und Update der internen 
          Zaehler "lsv[].allPCtr" und "lsv[].allPAllCtr" des durch "lsv_strat" 
          bezeichneten LSVs. Der Level-Index des upzudatenden Levels wird an 
          Hand der tatsaechlichen LSV-Anwendungsrate "lsv_ist_rate" im Rahmen 
          der aktiven Level bestimmt. 
          Eine Level-Anpassung erfolgt, wenn entweder insgesamt mindestens 
          "level_gesamt_limit" Anwendungen stattfanden oder zwei Levels mindes-
          tens je "level_einzel_limit" Anwendungen hatten. Wenn ein Level keine
          Anwendung hatte, werden nur die verbleibenden Level angepasst. Kein
          Level darf unter einen Anteil von MIN_LEVEL_P sinken.

void   init_adi (INT  lsvAdaptSpeed,
                 INT  levelAdaptSpeed);
          Berechnet die Anzahl beteiligter LSVs in "initialBeteilLsvAnz" und
          initialisiert "aktuellBeteilLsvAnz[0]" damit. Berechnet "startLsvPMin" 
          und setzt durch Aufruf von "setAdaptSpeed" die LSV-Adaptionsgeschwin-
          digkeit fuer die Notenklasse 0 auf "adaptSpeed" und durch Aufruf
          von "setLevelAdaptSpeed" die Level-Adaptionsgeschwindigkeit ebenfalls
          auf "adaptSpeed".

void   setLSVAdaptSpeed (INT  adaptSpeed,
                         INT  aktNkl);
          Setzt die Variablen "lsv_einzel_limit" und "lsv_gesamt_limit" ent-
          sprechend der Anzahl der beteiligten LSVs "aktuelBeteilLsvAnz", der 
          Notenklasse "aktNkl" u. der gewuenschten Geschwindigkeit "adaptSpeed".
          Die neue Geschwindigkeit wird in "aktLSVAdaptSpeed" und als Text in 
          "lsv_adapt_speed_txt" hinterlegt.

void   setLevelAdaptSpeed (INT  adaptSpeed);
          Setzt die Variablen "level_einzel_limit" und "level_gesamt_limit" ent-
          sprechend der gewuenschten Geschwindigkeit "adaptSpeed". Die neue 
          Geschwindigkeit wird in "aktLevelAdaptSpeed" und als Text in "level_
          adapt_speed_txt" hinterlegt.

void   getAdaptSpeeds (INT *lsvAdaptSpeed,
                       INT *levelAdaptSpeed);
       Liefert die aktuellen Werte der beiden Adaptionsgeschwindigkeiten 
       "aktLSVAdaptSpeed" und "aktLevelAdaptSpeed" ab.

void   get_adi_statistik_string (char *str);
          Die Routine liefert einen String als Ergänzung der #-Meldung mit 
          folgenden Werten in folgender Reihenfolge:
             Gesamtanzahl aller Updates, 
             Gesamtanzahl der allP-Updates, 
             hoechste aktive Notenklasse
             dann fuer diese Klasse folgende Werte: 
               Anzahl der Updates von p(LSV) und LSV-Parameter aber ohne all-P, 
               dann pro aktives LSV:
                 p(LSV)
                 allP: den Level mit der groessten Wahrscheinlichkeit
                 dann pro LSV-Steuerparameter:
                   den Level mit der groessten Wahrscheinlichkeit
          Format:
          ges_upd all_p_upd nkl_max  upd_anz[  p(LSV) bestLev(all-P)[ bestLev(LSV-Par)]*]+

void   log_adi_ges_statistik (void);
          Erzeugt die Daten fuer ein Balkendiagramm der End-ADI-Parameter aller
          Jobs seit Programmstart. Die LSV-Wahrscheinlichkeiten werden in 10 
          Klassen aufsummiert und bei den Levels werden die Klassen mit
          der groessten Wahrscheinlichkeit erfasst. 
          Zeilenkennzeichnung und Daten pro LSV und Notenklasse "nkl":
            *xP nkl  Histogramm von p(LSV)               JobAnz Schnitt
            *xi nkl  Level-Histogramm der LSV-Parameter  JobAnz Schnitt
            *xA nkl  Level-Histogramm von allP des LSVs  JobAnz Schnitt
          Dabei gilt:
            x               : LSV-Zeichen (lsvCh)
            i               : Index des LSV-Parammeters

void   set_std_nkl (INT  new_nkl_anz);
          Setzt eine der drei durch "new_nkl_anz" vorgegebenen Standardparame-
          trierungen fuer die Notenklasse. "new_nkl_anz" darf die Werte 1, 3, 4
          oder 5 haben, was nicht ueberprueft wird (alles andere ergibt NKL5)!
          Bei 1 wird eine Notenklasse benutzt und damit die Adaption nach Noten-
          klassen abgeschaltet.

BOOL   init_adapt_di (void);
          Package-Initialisierung.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. "allg_max_iter" wegen Compl-max_iter nach "lsvkoppl" verschoben. WJ 06.10.04

Aenderung gegenueber Version 1.0.1:
 1. Version 2: Einfuehrung von relativem "fitness gain" und noten-
    klassenabhaengiger Adaption.                                     WJ 21.12.04
 2. Anpassungen fuer die Adaption bei der all-Variante.              WJ 29.12.04
 3. Veraenderte Level-Werte fuer "error_goal" und beide "max_iter" 
    sowie "error_goal_start_level"-Werte fuer OPAL/V schaltbar 
    durch OPAL_V.                                                    WJ 05.01.05
 4. Adaption fuer die initiale Rosenbrock Schrittweite "step".       WJ 07.01.05
 5. Erkennung unbenutzter N-Klassen "get_best_level_idx" korrigiert. 
    Gesamt-Statistik "log_adi_ges_statistik" fuer all-P korrigiert.  WJ 10.01.05
 6. Aenderung 3 auch fuer LSKP schaltbar durch LSKP.                 WJ 12.01.05

Aenderung gegenueber Version 2.0.0:
 1. Version 3: Erweiterung fuer n LSVs.                              WJ 21.08.06
 2. Beseitigung folgenden Fehlers, der auch Version 2 betrifft: Wenn
    bei allen LSV-Anwendungen seit der letzten Adaption kein Fitness-
    gewinn erzielt werden konnte, darf nicht adaptiert werden, da bei
    der Neuberechnung sonst durch 0 dividiert wird. Bei schlecht per-
    formanten LSVs wie dem Complex kann das auch passieren!
 3. Es wird mit packageglobalen Variable "lsv_strat" gearbeitet an-
    statt das LSV per Parameter durchzureichen.                      WJ 29.08.06
 4. Getrennte Adaptionsgeschwindigkeiten f. LSV- u. Level-Adaption.  WJ 16.10.06
 5. ROS_3PARAM: Erweiterung fuer 3. Rosenbrock-Parameter f. OPAL/V.  WJ 20.11.06
 6. Auswertung d. Schalters MIT_ROCO in "parameterise_lsv_for_best". WJ 18.12.06
 7. Korrektur von "get_adi_statistik_string" fuer ASMA.              WJ 08.01.07
 8. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
 9. Entfernung von LSV_2OPT und LSV_3OPT. NOT_YET_IMPLEMENTED 
    ersetzt durch NOT_IN_THIS_VERSION.                               WJ 02.01.08
10. Erweiterungen fuer die GORBA-LSVs (MIT_GORBA_LSVS).              WJ 04.11.09
11. Beim AMMA darf ein abzuschaltende LSV nicht das aktuell Benutzte 
    sein.In diesem Fall erfolgt gesteuert ueber "lsv[].abzuschalten" 
    eine verzoegerte Abschaltung. Neue Routine "lsvAbschaltung".     WJ 15.04.10
12. Entfernung der Schalter und des dazugehoerigen Includes:
    BS_DOS, BS_WINXP_GNU.                                            WJ 21.06.12
13. Verschiebung von "oldDistribFrac" nach "evo_gsw", damit alle fuer
    TSK-Daten benoetigten Defaults auch in GLEAM verfuegbar sind. Neue
    Routine "getActNKlAnz". Anpassung des "write_to_log"-Aufrufs.    WJ 19.12.16
14. Neue Funktion "getAdaptSpeeds()". Verschiebung von P_MIN nach 
    "evoIntg.h"                                                      WJ 25.01.17
15. Fehlermeldung NOT_ENOUGH_MEM korrigiert. Reunion of "evoIntg.h" 
    and "evoInts.h" in "evoInt.h".                                   WJ 30.06.17
16. get_adi_statistik_string(): Enlargement of string buffer "buf" 
    to avoid overflow warnings.                                      WJ 10.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 10.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "lgsw.h"
#include "aufg.h"
#include "simu.h"
#ifdef MIT_GORBA_LSVS
  #include "glsv.h"
#endif /* MIT_GORBA_LSVS */
#include "evo.h"
#include "evoInt.h"

#include EVO_TEXTS


#ifdef GLO_CHECKS
  #define SW_CHECKS
#endif

#undef LSV_SEL_LOGS
#undef LEVEL_LOGS
#undef ADI_LOGS



/* ========================== Exportierte Variable: ========================= */
INT     lsv_einzel_limit  [NKL_MAX],   /* Mindestanwendungen einzelnes LSV    */
        lsv_gesamt_limit  [NKL_MAX],   /* Mindestanwendungen aller LSVs       */
        lsv_nix_anwendung_limit,       /* MaxAnzAdapt.eines LSVs bis Abschalt.*/
        level_einzel_limit,            /* Mindestanwendungen einzelnes Level  */
        level_gesamt_limit;            /* Mindestanwendungen alle Level       */



/* ======================== Package-globale Variable: ======================= */
char    lsv_adapt_speed_txt[10],      /* Text zur LSV-Adaptionsgeschwindigkeit*/
        level_adapt_speed_txt[10];    /* Text zur Level-Adaptionsgeschwindigk.*/
INT     nklassen_anz = 1,             /* Anzahl der Notenklassen              */
        aktLSVAdaptSpeed,             /* aktuelle LSV-Adaptionsgeschwindigkeit*/
        aktLevelAdaptSpeed,           /* aktuelle Level-Adaptionsgeschw.      */
        singleLsv          [NKL_MAX], /*nach Wettbew.verbliebenes/einziges LSV*/
        aktuellBeteilLsvAnz[NKL_MAX]; /* Anzahl aktuell noch beteiligter LSVs */
DOUBLE  nklassen_feld      [NKL_MAX], /* Notenklassen-Feld                    */
        nklassen_feld_neu  [NKL_MAX], /* neues Feld fuer "evo_cmen"           */
        startLsvPMin;                 /* Initiale P-Untergrenze f.LSV-Abschalt*/
BOOL    lsv_adapt_aktiv    [NKL_MAX]; /* TRUE: aktive LSV konkurrieren noch   */



/* ============================ Lokale Konstante: =========================== */
#define MIN_LEVEL_P      0.1      /* Mindestwahrscheinlickeit eines Levels    */
#define LSV_PMIN_FRAC    0.2      /* Prozent d.durchschn.p(LSV) f.Min(p(LSV)) */
#define OLD_DISTR_FRAC   0.33333  /* Initialer Ant.d.alten Vert.an der neuen  */
#define LSV_EINTR_ANZ    9        /* Eintrage aktiver LSVs f.Adaptionsgeschw. */



/* ======================== Lokale Typdeklarationen: ======================== */



/* ============================ Lokale Variable: ============================ */
static DOUBLE  verkleinern_limit =0.5,/* Limit f.Levelverschieben nach links  */
               vergroessern_limit=0.5,/* Limit f.Levelverschieben nach rechts */
              *lsvPMin        = NULL; /* Aktuelle P-Untergrenze f.LSV-Abschalt*/
static INT    *lsv_anw_ctr    = NULL, /* Zaehler der LSV-Anwendungen          */
              *update_ctr     = NULL, /* LSV- u.Par-Updates pro Job u.NKlasse */
              *allP_update_ctr= NULL, /* all-P-Updates pro Job und NKlasse    */
               initialBeteilLsvAnz,   /* Anzahl nach Genmodel beteiligter LSVs*/
               update_ges_ctr,        /* Zaehler aller Updates pro Job        */
               allP_upd_ges_ctr;      /* GesUpd pro Job fuer all-P aller LSVs */
static BOOL   *nklasse_valid   =NULL, /* TRUE: lsvP dieser NKLasse gueltig    */
               mitAltPAnteil;         /* TRUE: oldDistribFrac > 0.0           */

static int     lsvEinzelLimit[3][LSV_EINTR_ANZ] = {{3, 3, 3,  3, 3, 2,  2, 2, 2},
                                                   {5, 4, 4,  4, 4, 3,  3, 3, 3},
                                                   {8, 6, 6,  5, 5, 5,  4, 4, 4}};
static INT     lsvGesamtLimit[3][LSV_EINTR_ANZ] = {{15, 18, 24,  28, 32, 28,  32, 34, 36},
                                                   {25, 24, 32,  37, 43, 42,  48, 51, 54},
                                                   {40, 36, 48,  46, 54, 70,  64, 68, 72}};
static size_t  dsize = sizeof(DOUBLE);
static size_t  lsize = sizeof(LONG);
static size_t  isize = sizeof(INT);
static size_t  bsize = sizeof(BOOL);



/* ============================ Lokale Routinen: ============================ */
static void   reset_lsv_ctr              (INT     nkl);

static void   reset_level_ctr            (INT     lsvIdx,
                                          INT     parIdx,
                                          INT     nkl);

static void   reset_allP_level_ctr       (INT     lsvIdx,
                                          INT     nkl);

static void   lsvPSel_bestimmen          (INT     aktNkl);

static void   lsvP_und_lsvPSel_bestimmen (INT     aktNkl,
                                          DOUBLE  delta);

static void   single_lsv_bestimmen       (INT     aktNkl);

static void   activate_nkl               (INT     akt_nkl);

static BOOL   level_adapter              (DOUBLE *levels,
                                          DOUBLE *fg_sum,
                                          LONG   *eval_sum,
                                          INT    *ctr,
                                          INT    *all_ctr,
                                          INT    *level1,
                                          DOUBLE *secondLevPSum,
                                          INT     level_max_anz,
                                          INT     akt_nkl,
                                          INT    *upd_ctr);

static void   lsvAbschaltung             (INT     lsvIdx, 
                                          INT     akt_nkl);

static INT    get_level_from_fraction    (DOUBLE  fraction,
                                          INT     level1);

static INT    get_best_level_idx         (DOUBLE *levels,
                                          INT     level1,
                                          BOOL    nkl_valid);

static INT    get_max_nkl                (void);

static void   setPMinLSV                 (INT     aktNkl);

static void   log_adi_statistik          (INT     akt_nkl);



/* ============================== reset_lsv_ctr ============================= */
static void reset_lsv_ctr (INT  nkl)
/*----------------------------------------------------------------------------*/
/*  Setzt Zaehler der adaptiven direkten Integration fuer die LSV-Auswahl     */
/*  der Notenklasse "nkl" nach einer Adaption wieder zurueck.                 */
/*----------------------------------------------------------------------------*/
{
   INT  ii;

   /* reset_lsv_ctr */
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if (lsv[ii].lsvOn)
      { /* alle aktiven LSVs */
	 lsv[ii].lsvFgSum[nkl]   = 0.0;
	 lsv[ii].lsvEvalSum[nkl] = 0;
	 lsv[ii].lsvCtr[nkl]     = 0;
      } /* alle aktiven LSVs */
   lsv_anw_ctr[nkl] = 0;
} /* reset_lsv_ctr */



/* ============================= reset_level_ctr ============================ */
static void reset_level_ctr (INT  lsvIdx,
                             INT  parIdx,
                             INT  nkl)
/*----------------------------------------------------------------------------*/
/*  Setzt Zaehler der adaptiven direkten Integration fuer das durch "lsvIdx"  */
/*  bezeicchnete LSV und den durch "parIdx" bezeichneten Steuerparameter der  */
/*  Notenklasse "nkl" (nach einer Adaption wieder) zurueck.                   */
/*----------------------------------------------------------------------------*/
{
   INT  levelAnz;

   /* reset_r_mi_level_ctr */
   levelAnz = lsv[lsvIdx].parLevelAnz[parIdx];
   memset (lsv[lsvIdx].parFgSum[parIdx][nkl],   0, dsize*levelAnz);
   memset (lsv[lsvIdx].parEvalSum[parIdx][nkl], 0, lsize*levelAnz);
   memset (lsv[lsvIdx].parCtr[parIdx][nkl],     0, isize*levelAnz);
   lsv[lsvIdx].parAllCtr[parIdx][nkl] = 0;
} /* reset_r_mi_level_ctr */



/* ========================== reset_allP_level_ctr ========================== */
static void reset_allP_level_ctr (INT  lsvIdx,
                                  INT  nkl)
/*----------------------------------------------------------------------------*/
/*  Setzt Zaehler der adaptiven direkten Integration fuer die Wahrscheinlich- */
/*  keit der Verbesserung durch das mit "lsvIdx" bezeichnete LSV bei "all-    */
/*  Verbessrung" der Notenklasse "nkl" nach einer Adaption wieder zurueck.    */
/*----------------------------------------------------------------------------*/
{
   memset (lsv[lsvIdx].allPFgSum[nkl],   0, dsize*allPLevelAnz);
   memset (lsv[lsvIdx].allPEvalSum[nkl], 0, lsize*allPLevelAnz);
   memset (lsv[lsvIdx].allPCtr[nkl],     0, isize*allPLevelAnz);
   lsv[lsvIdx].allPAllCtr[nkl] = 0;
} /* reset_allP_level_ctr */



/* ============================ lsvPSel_bestimmen =========================== */
static void lsvPSel_bestimmen (INT  aktNkl)
/*----------------------------------------------------------------------------*/
/*  Berechnet fuer "aktNkl" die summierten Wahrscheinlichkeiten pro beteilig- */
/*  tem LSV und hinterlegt diese in lsvPSel. Bei gesetztem SW_CHECKS wird ge- */
/*  prueft, ob die am Schluss die P-Summe gleich 1.0 ist. Wenn nicht, erfolgt */
/*  eine Fehlermeldung.                                                       */
/*----------------------------------------------------------------------------*/
{
   DOUBLE  lastLsvPSel;
   INT     ii;
   BOOL    first_lsv;

   /* lsvPSel_bestimmen */
   first_lsv = TRUE;
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if (lsv[ii].lsvOn[aktNkl])
      { /* alle beteiligten LSVs */
        if (first_lsv) { /* erstes beteiligtes LSV */
           first_lsv   = FALSE;
           lastLsvPSel = lsv[ii].lsvP[aktNkl];
        } /* erstes beteiligtes LSV */
        else 
           lastLsvPSel += lsv[ii].lsvP[aktNkl];
        lsv[ii].lsvPSel[aktNkl] = lastLsvPSel;
      } /* alle beteiligten LSVs */
#ifdef SW_CHECKS
   if (fabs(lastLsvPSel - 1.0) > (100 * DBL_EPSILON)) {
      sprintf (buf3, "lsvPSel_bestimmen: P-Summe=%g statt 1.0 !", lastLsvPSel);
      err_state = fatal (EVO_ADI, 50, NO_INFO, NO_INFO, buf3, SIMU_RZ_MELD);
      sprintf (buf2, "***** %s", buf3);
      write_to_log (buf2, CLOSE_LOG);
   }
  #ifdef LSV_SEL_LOGS
   sprintf (buf1, "ADI: lsvPSel_bestimmen: aktNkl=%d lsvPSel =", aktNkl);
   for (ii = 0;  ii < LSV_ANZ;  ii++) {
      sprintf (buf2, " %6g", lsv[ii].lsvPSel[aktNkl]);
      strcat (buf1, buf2);
   }
   write_to_log (buf1, CLOSE_LOG);
  #endif /* LSV_SEL_LOGS */
#endif /* SW_CHECKS */
} /* lsvPSel_bestimmen */



/* ======================= lsvP_und_lsvPSel_bestimmen ======================= */
static void lsvP_und_lsvPSel_bestimmen (INT     aktNkl,
                                        DOUBLE  delta)
/*----------------------------------------------------------------------------*/
/*  Korrigiert fuer "aktNkl" die LSV-Wahrscheinlichkeiten "lsvP" pro betei-   */
/*  ligtem LSV um delta/aktuellBeteilLsvAnz und berechnet danach die summier- */
/*  ten Wahrscheinlichkeiten in lsvPSel. Bei gesetztem SW_CHECKS wird ge-     */
/*  prueft, ob am Schluss die P-Summe gleich 1.0 ist. Wenn nicht, erfolgt     */
/*  eine Fehlermeldung.                                                       */
/*----------------------------------------------------------------------------*/
{
   DOUBLE  lastLsvPSel;
   INT     ii;
   BOOL    first_lsv;

   /* lsvP_und_lsvPSel_bestimmen */
   first_lsv = TRUE;
   delta     = delta / aktuellBeteilLsvAnz[aktNkl];
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if (lsv[ii].lsvOn[aktNkl])
      { /* alle beteiligten LSVs */
         lsv[ii].lsvP[aktNkl] += delta;
         if (first_lsv) { /* erstes beteiligtes LSV */
            first_lsv   = FALSE;
            lastLsvPSel = lsv[ii].lsvP[aktNkl];
         } /* erstes beteiligtes LSV */
         else 
            lastLsvPSel += lsv[ii].lsvP[aktNkl];
         lsv[ii].lsvPSel[aktNkl] = lastLsvPSel;
      } /* alle beteiligten LSVs */
#ifdef SW_CHECKS
   if (fabs(lastLsvPSel - 1.0) > (100 * DBL_EPSILON)) {
      sprintf (buf3, "lsvP_und_lsvPSel_bestimmen: P-Summe=%g statt 1.0 !", lastLsvPSel);
      err_state = fatal (EVO_ADI, 51, NO_INFO, NO_INFO, buf3, SIMU_RZ_MELD);
      sprintf (buf2, "***** %s", buf3);
      write_to_log (buf2, CLOSE_LOG);
   }
  #ifdef LSV_SEL_LOGS
   sprintf (buf1, "ADI: lsvP_und_lsvPSel_bestimmen: aktNkl=%d delta/LSV=%6.4f "
            "lsvP/lsvPSel: ",aktNkl, delta);
   for (ii = 0;  ii < LSV_ANZ;  ii++) {
      sprintf (buf2, " %6.4f", lsv[ii].lsvPSel[aktNkl]);
      strcat (buf1, buf2);
   }
   write_to_log (buf1, CLOSE_LOG);
  #endif /* LSV_SEL_LOGS */
#endif /* SW_CHECKS */
} /* lsvP_und_lsvPSel_bestimmen */



/* ========================== single_lsv_bestimmen ========================== */
static void single_lsv_bestimmen (INT  aktNkl)
/*----------------------------------------------------------------------------*/
/*  Bestimmt fuer "aktNkl" das letzte verbliebene beteiligte LSV und hinter-  */
/*  legt es in "singleLsv". Bei gesetztem SW_CHECKS erfolgt eine Fehlermel-   */
/*  dung, wenn es nicht genau ein beteiligte LSV gibt.                        */
/*----------------------------------------------------------------------------*/
{
   INT   ii, jj;
   BOOL  not_found;

   /* single_lsv_bestimmen */
   not_found = TRUE;
   for (ii = 0;  (ii < LSV_ANZ) && not_found;  ii++)
      if (lsv[ii].aktiv)
         not_found = !lsv[ii].lsvOn[aktNkl];
   if (not_found) {
      singleLsv[aktNkl] = KEIN_LSV;
#ifdef SW_CHECKS
      write_to_log ("***** single_lsv_bestimmen: Kein beteiligtes LSV gefunden!", CLOSE_LOG);
      err_state = fatal (EVO_ADI, 52, NO_INFO, NO_INFO, "single_lsv_bestimmen: Kein "
             "beteiligtes LSV gefunden!", SIMU_RZ_MELD);
#endif /* SW_CHECKS */
   }
   else
   { /* einzig verbliebenes beteiligtes LSV ermittelt */
      singleLsv[aktNkl]      = ii - 1;
      lsv[ii-1].lsvP[aktNkl] = 1.0;                     /* fuer die Statistik */
   } /* einzig verbliebenes beteiligtes LSV ermittelt */
#ifdef SW_CHECKS
   for (jj = ii+1;  jj < LSV_ANZ;  jj++)
      if (lsv[ii].aktiv && lsv[ii].lsvOn[aktNkl]) {
         sprintf (buf3, "*****single_lsv_bestimmen: Ein weiteres beteiligtes LSV "
                  "gefunden: %d!", jj);
         write_to_log (buf3, CLOSE_LOG);
         err_state = fatal (EVO_ADI, 53, NO_INFO, NO_INFO, buf3, SIMU_RZ_MELD);
      }
#endif /* SW_CHECKS */
#ifdef LSV_SEL_LOGS
   sprintf (buf3, "ADI: single_lsv_bestimmen: aktNkl=%d singleLsv=%d (%s) lsvOn =", 
            aktNkl, singleLsv[aktNkl], lsv[singleLsv[aktNkl]].lsvMenueName);
   for (ii = 0;  ii < LSV_ANZ;  ii++) {
      sprintf (buf2, " %d", lsv[ii].lsvOn[aktNkl]);
      strcat (buf3, buf2);
   }
   write_to_log (buf3, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
} /* single_lsv_bestimmen */



/* ================================ reset_adi =============================== */
void reset_adi (void)
{
   INT   lev, ii, pp, kk;

   /* reset_adi */
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if (lsv[ii].aktiv)
      { /* alle aktiven LSVs */
         for (pp = 0;  pp < lsv[ii].lsvParAnz;  pp++)
         { /* alle Parameter eines LSVs */
            for (kk = 0;  kk < nklassen_anz;  kk++)
            { /* alle NKlassen eines LSVs */
               reset_level_ctr (ii, pp, kk);
               memset (lsv[ii].parLevelP[pp][kk], 0, dsize * lsv[ii].parLevelAnz[pp]);
            } /* alle NKlassen eines LSVs */
            memset (lsv[ii].par2ndLevPSum[pp], 0, dsize * nklassen_anz);
            memset (lsv[ii].par1stActLev[pp],  0, isize * nklassen_anz);
            lsv[ii].par1stActLev[pp][0] = lsv[ii].parStartLev[pp];
            lev = lsv[ii].par1stActLev[pp][0];
            lsv[ii].parLevelP[pp][0][lev++] = lsv[ii].parIniP1stLev[pp];
            lsv[ii].parLevelP[pp][0][lev++] = lsv[ii].parIniP2ndLev[pp];
            lsv[ii].parLevelP[pp][0][lev]   = 1.0 - (lsv[ii].parIniP1stLev[pp] + 
                                                     lsv[ii].parIniP2ndLev[pp]  );
            lsv[ii].par2ndLevPSum[pp][0] = lsv[ii].parIniP1stLev[pp] + 
               lsv[ii].parIniP2ndLev[pp];
            lsv[ii].parLastLevel[pp] = -1;
         } /* alle Parameter eines LSVs */
         for (kk = 0;  kk < nklassen_anz;  kk++)
         { /* alle NKlassen */
            reset_allP_level_ctr (ii, kk);
            memset (lsv[ii].allPLevelP[kk], 0, dsize * allPLevelAnz);
         } /* alle NKlassen */
         memset (lsv[ii].lsvOn,          0, bsize * nklassen_anz);
         memset (lsv[ii].abzuschalten,   0, bsize * nklassen_anz);
         memset (lsv[ii].nklValid,       0, bsize * nklassen_anz);
         memset (lsv[ii].allP1stActLev,  0, isize * nklassen_anz);
         memset (lsv[ii].allP2ndLevPSum, 0, dsize * nklassen_anz);
         lsv[ii].nklValid[0]      = TRUE;
         lsv[ii].allP1stActLev[0] = lsv[ii].allPStartLevel;
         lsv[ii].allPLevelP[0][lsv[ii].allPStartLevel]   = lsv[ii].allPIniP1stLev;
         lsv[ii].allPLevelP[0][lsv[ii].allPStartLevel+1] = lsv[ii].allPIniP2ndLev;
         lsv[ii].allPLevelP[0][lsv[ii].allPStartLevel+2] = 1 - (lsv[ii].allPIniP1stLev +
                                                                lsv[ii].allPIniP2ndLev);
         lsv[ii].allP2ndLevPSum[0] = lsv[ii].allPIniP1stLev + lsv[ii].allPIniP2ndLev;
         if (gleam_amma) 
         { /* AMMA */
            memset (lsv[ii].lsvFinCtr, 0, isize * nklassen_anz);
            lsv[ii].lsvP[0]    = lsv[ii].lsvStartP;
            lsv[ii].lsvPSel[0] = 0.0;
            lsv[ii].lsvOn[0]   = lsv[ii].lsvStartP > 0.0;
         } /* AMMA */
      } /* alle aktiven LSVs */
   lsv_adapt_aktiv[0] = FALSE;    /* ASMA oder AMMA und nur ein LSV beteiligt */
   if (gleam_amma) 
   { /* AMMA */
      memset (nklasse_valid, 0, bsize * nklassen_anz);
      nklasse_valid[0]       = TRUE;
      aktuellBeteilLsvAnz[0] = initialBeteilLsvAnz;
      setLSVAdaptSpeed (aktLSVAdaptSpeed, 0); /* benutzt "aktuellBeteilLsvAnz"*/
      if (initialBeteilLsvAnz == 1) 
         single_lsv_bestimmen (0);                   /* nur ein LSV beteiligt */
      else 
      { /* LSV-Konkurrenz */
         lsv_adapt_aktiv[0] = TRUE;                  /* mehrere LSV beteiligt */
         lsvPMin[0] = startLsvPMin;  /* reset aktuelle P-UG f.LSV-Abschaltung */
         lsvPSel_bestimmen (0);
         for (ii = 0;  ii < nklassen_anz;  ii++)
            reset_lsv_ctr (ii);
      } /* LSV-Konkurrenz */
   } /* AMMA */
   if (gleam_asma) 
   { /* ASMA */
      lsv[lsv_strat].lsvOn[0] = TRUE;
      aktuellBeteilLsvAnz[0]  = 1;
      setLSVAdaptSpeed (aktLSVAdaptSpeed, 0);
   } /* ASMA */
   memset (update_ctr,      0, isize * nklassen_anz);
   memset (allP_update_ctr, 0, isize * nklassen_anz);
   update_ges_ctr     = 0;
   allP_upd_ges_ctr   = 0;
   mitAltPAnteil      = oldDistribFrac > 0.0;
   log_adi_statistik (0);             /* logge Anfangswerte der 1. Notenklasse */
#if defined (LSV_SEL_LOGS) || defined (LEVEL_LOGS)
   sprintf (buf1, "reset_adi: gleam_amma=%d gleam_asma=%d initialBeteilLsvAnz=%d "
            "lsv_adapt_aktiv[0]=%d\n", 
            gleam_amma, gleam_asma, initialBeteilLsvAnz, lsv_adapt_aktiv[0]);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS oder LEVEL_LOGS */
} /* reset_adi */



/* ================================ get_nklasse ============================= */
INT get_nklasse (DOUBLE  note)
{
   INT  ii;

   /* get_nklasse */
   ii = 0;
   if (nklassen_anz > 1)
      while (nklassen_feld[ii] < note)
         ii++;
#ifdef LSV_SEL_LOGS
   if (gleam_asma) {
      sprintf (buf1, "\nADI: get_nklasse  note=%g nkl=%d ", note, ii);
      write_to_log (buf1, CLOSE_LOG);
   }
#endif /* LSV_SEL_LOGS */
   return (ii);
} /* get_nklasse */



/* =================================== getActNKlAnz =================================== */
INT getActNKlAnz (void)
{
   return (nklassen_anz);
} /* getActNKlAnz */



/* ================================ activate_nkl ============================= */
static void activate_nkl (INT  aktNkl)
/*-----------------------------------------------------------------------------*/
/*  Uebernimmt fuer das LSV "lsv_strat" und die Notenklasse "aktNkl" die       */
/*  Levelwerte der naechstniedrigen Notenklasse. Diese betrifft die die LSV-   */
/*  Parametrierung und die all-P-Wahrscheinlichkeiten.                         */
/*-----------------------------------------------------------------------------*/
{
   INT  lev, ii, pp, kk;
   INT  lastValidNkl, idx;

   /* activate_nkl */
   lastValidNkl = aktNkl - 1;
   while (!lsv[lsv_strat].nklValid[lastValidNkl])        /* naechstniedrige    */
      lastValidNkl--;                                    /* Notenklasse suchen */
#if defined (LSV_SEL_LOGS) || defined (LEVEL_LOGS)
   sprintf (buf2, " mit NKL%1d-Werten", lastValidNkl);
#endif /* LSV_SEL_LOGS oder LEVEL_LOGS */
   for (pp = 0;  pp < lsv[lsv_strat].lsvParAnz;  pp++)
   { /* alle Parameter eines LSVs */
      idx = lsv[lsv_strat].par1stActLev[pp][lastValidNkl];
      lsv[lsv_strat].par1stActLev[pp][aktNkl]    = idx;
      lsv[lsv_strat].parLevelP[pp][aktNkl][idx]  =
         lsv[lsv_strat].parLevelP[pp][lastValidNkl][idx];
      lsv[lsv_strat].parLevelP[pp][aktNkl][idx+1]=
         lsv[lsv_strat].parLevelP[pp][lastValidNkl][idx+1];
      lsv[lsv_strat].parLevelP[pp][aktNkl][idx+2]=
         lsv[lsv_strat].parLevelP[pp][lastValidNkl][idx+2];
      lsv[lsv_strat].par2ndLevPSum[pp][aktNkl]   =
         lsv[lsv_strat].par2ndLevPSum[pp][lastValidNkl];
   } /* alle Parameter eines LSVs */
   idx = lsv[lsv_strat].allP1stActLev[lastValidNkl];      /* 1. aktiver Level */
   lsv[lsv_strat].allP1stActLev[aktNkl]    = idx;
   lsv[lsv_strat].allPLevelP[aktNkl][idx]  = lsv[lsv_strat].allPLevelP[lastValidNkl][idx];
   lsv[lsv_strat].allPLevelP[aktNkl][idx+1]= lsv[lsv_strat].allPLevelP[lastValidNkl][idx+1];
   lsv[lsv_strat].allPLevelP[aktNkl][idx+2]= lsv[lsv_strat].allPLevelP[lastValidNkl][idx+2];
   lsv[lsv_strat].allP2ndLevPSum[aktNkl]   = lsv[lsv_strat].allP2ndLevPSum[lastValidNkl];
   if (gleam_asma)
      lsv[lsv_strat].lsvOn[aktNkl] = lsv[lsv_strat].lsvOn[lastValidNkl];
   lsv[lsv_strat].nklValid[aktNkl] = TRUE;           /* Aktivierung vermerken */
   log_adi_statistik (aktNkl);     /* logge Anfangswerte der akt. Notenklasse */
} /* activate_nkl */



/* =========================== set_actual_adi_lsv =========================== */
void set_actual_adi_lsv (INT    *this_nkl,
                         DOUBLE  akt_note)
{
   DOUBLE  pw;
   INT     lastValidNkl, aktNkl, aktLsv, ii;
   BOOL    not_found;

   /* set_actual_adi_lsv */
   aktNkl = get_nklasse (akt_note);
#ifdef LSV_SEL_LOGS
   sprintf (buf1, "\nADI: set_actual_adi_lsv  akt_note=%g nkl=%d nkl_valid=%d"
            " lsv_adapt_aktiv=%d", akt_note, aktNkl, 
            nklasse_valid[aktNkl], lsv_adapt_aktiv[aktNkl]);
   write_to_log (buf1, CLOSE_LOG);
   strcpy (buf2, "");
#endif /* LSV_SEL_LOGS */
   *this_nkl = aktNkl;                                 /* Wert zurueckliefern */
   if (!nklasse_valid[aktNkl])
   { /* Notenklasse aktivieren */
      lastValidNkl = aktNkl - 1;
      while (!nklasse_valid[lastValidNkl])              /* naechstniedrige    */
         lastValidNkl--;                                /* Notenklasse suchen */
#if defined (LSV_SEL_LOGS) || defined (LEVEL_LOGS)
      sprintf (buf2, " mit NKL%1d-Werten", lastValidNkl);
#endif /* LSV_SEL_LOGS oder LEVEL_LOGS */
      lsv_adapt_aktiv[aktNkl] = lsv_adapt_aktiv[lastValidNkl];
      if (lsv_adapt_aktiv[aktNkl])
      { /* Werte der niedrigeren Klasse uebernehmen, da noch LSV-Konkurrenz */
         for (ii = 0;  ii < LSV_ANZ;  ii++)
            if (lsv[ii].aktiv)
               { /* alle aktiven LSVs */
                  lsv[ii].lsvP[aktNkl]    = lsv[ii].lsvP[lastValidNkl];
                  lsv[ii].lsvPSel[aktNkl] = lsv[ii].lsvPSel[lastValidNkl];
                  lsv[ii].lsvOn[aktNkl]   = lsv[ii].lsvOn[lastValidNkl];
               } /* alle aktiven LSVs */
         lsv_einzel_limit[aktNkl]    = lsv_einzel_limit[lastValidNkl];
         lsv_gesamt_limit[aktNkl]    = lsv_gesamt_limit[lastValidNkl];
         lsvPMin[aktNkl]             = lsvPMin[lastValidNkl];
         aktuellBeteilLsvAnz[aktNkl] = aktuellBeteilLsvAnz[lastValidNkl];
      } /* Werte der niedrigeren Klasse uebernehmen, da noch LSV-Konkurrenz */
      else 
      { /* nur noch 1 LSV und lsvP uebernehmen wg. #-Job-Statistik-Zeile */
         singleLsv[aktNkl] = singleLsv[lastValidNkl];     /* nur noch 1 LSV */
         for (ii = 0;  ii < LSV_ANZ;  ii++)    /* wegen Job-Statistik-Zeile */
            if (lsv[ii].aktiv)
               lsv[ii].lsvP[aktNkl] = lsv[ii].lsvP[lastValidNkl];
      } /* nur noch 1 LSV und lsvP uebernehmen wg. #-Job-Statistik-Zeile */
      nklasse_valid[aktNkl] = TRUE;
   } /* Notenklasse aktivieren */

   if (lsv_adapt_aktiv[aktNkl])
   { /* mindesetens 2 LSVs konkurrieren noch fuer diese Notenklasse */
      pw = frand();
      not_found = TRUE;
      for (aktLsv = 0;  (aktLsv < LSV_ANZ) && not_found;  aktLsv++)
         not_found = pw > lsv[aktLsv].lsvPSel[aktNkl];
      aktLsv--;                  /* Wenn gefunden, ist "aktLsv" um 1 zu gross */
#ifdef SW_CHECKS
      if (!not_found)
         not_found = !lsv[aktLsv].aktiv;
      if (not_found) {
         err_state = fatal (EVO_ADI, 54, NO_INFO, NO_INFO, 
                            "Fehler bei LSV-Auswahl!", SIMU_RZ_MELD);
         aktLsv = aktive_lsv[0];
         write_to_log ("***** set_actual_adi_lsv: Fehler bei LSV-Auswahl!", CLOSE_LOG);
      }
#endif /* SW_CHECKS */
#ifdef LSV_SEL_LOGS
      sprintf (buf1, "                        %s nkl_valid=%d"
               " lsvOn=%d pSel(LSV)=%g pw=%g  LSV=%d (%s)", buf2, 
               nklasse_valid[aktNkl], lsv[aktLsv].lsvOn[aktNkl], 
               lsv[aktLsv].lsvPSel[aktNkl], pw, aktLsv, lsv[aktLsv].lsvMenueName);
      write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
   } /* mindesetens 2 LSVs konkurrieren noch fuer diese Notenklasse */
   else 
   { /* nur noch ein LSV fuer diese Notenklasse */
      aktLsv = singleLsv[aktNkl];
#ifdef LSV_SEL_LOGS
      sprintf (buf1, "                         nkl_valid=%d lsvOn=%d "
               "lsv_adapt_aktiv=%d  LSV=%s", nklasse_valid[aktNkl], 
               lsv[aktLsv].lsvOn[aktNkl], lsv_adapt_aktiv[aktNkl], 
               lsv[aktLsv].lsvMenueName);
      write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
   } /* nur noch ein LSV fuer diese Notenklasse */
   lsv_strat = aktLsv;      /* package-globale Variable fuer das aktuelle LSV */
} /* set_actual_adi_lsv */



/* ======================== parameterise_lsv_for_all ======================== */
GSTATE parameterise_lsv_for_all (INT    *this_nkl,
                                 DOUBLE  akt_note)
{
   *this_nkl = get_nklasse (akt_note);
#ifdef LEVEL_LOGS
   sprintf (buf1, "ADI: parameterise_lsv_for_all  akt_note=%g nkl=%d",
        akt_note, *this_nkl);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */
   return (parameterise_lsv_for_best (*this_nkl));
} /* parameterise_lsv_for_all */



/* ======================== parameterise_lsv_for_best ======================= */
GSTATE parameterise_lsv_for_best (INT  akt_nkl)
{
   INT     aktParLevel[LSV_PAR_MAX],
           last_valid_nkl,
           akt_level,
           idx;
   DOUBLE  pw;
   GSTATE  ret_state = GLEAM_OK;
   BOOL    lsvParChanged = FALSE;

   /* parameterise_lsv_for_best */
#ifdef LEVEL_LOGS
   strcpy (buf2, "");
#endif /* LEVEL_LOGS */
   if (!lsv[lsv_strat].nklValid[akt_nkl])
      activate_nkl (akt_nkl);
#ifdef LEVEL_LOGS
   sprintf (buf1, "ADI: parameterise_lsv_for_best %s nkl=%d nkl_valid=%d "
            "LSV=%s:", buf2, akt_nkl, lsv[lsv_strat].nklValid[akt_nkl], 
            lsv[lsv_strat].lsvJobLiName);
#endif /* LEVEL_LOGS */
   for (idx = 0;  idx < lsv[lsv_strat].lsvParAnz;  idx++)
   { /* alle Steuerparameter des LSVs */
      pw        = frand();
      akt_level = lsv[lsv_strat].par1stActLev[idx][akt_nkl];
      if (pw < lsv[lsv_strat].parLevelP[idx][akt_nkl][akt_level])
         aktParLevel[idx] = akt_level;
      else
         if (pw < lsv[lsv_strat].par2ndLevPSum[idx][akt_nkl])
            aktParLevel[idx] = akt_level + 1;
         else
            aktParLevel[idx] = akt_level + 2;
      lsvParChanged |= (aktParLevel[idx] != lsv[lsv_strat].parLastLevel[idx]);
#ifdef LEVEL_LOGS
      sprintf (buf2, " %s=%d/%d (%5g)", lsv[lsv_strat].lsvParName[idx],
               aktParLevel[idx], lsv[lsv_strat].parLastLevel[idx], pw);
      strcat (buf1, buf2);
#endif /* LEVEL_LOGS */
   } /* alle Steuerparameter des LSVs */
#ifdef LEVEL_LOGS
   sprintf (buf2, "  lsvParChanged=%d", lsvParChanged);
   strcat (buf1, buf2);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */

   if (lsvParChanged)
      switch (lsv_strat) { /* LSV-spezifische Steuerparameteranpassung */

#ifdef MIT_ROCO  /* ================ Anpassung der ROCO-LSVs ================ */
      case LSV_ROSEN:
  #ifdef ROS_3PARAM
         ret_state = set_rc_limits (0, 
                                    (INT)lsv[lsv_strat].parLevelVal[0][aktParLevel[0]],
                                    lsv[lsv_strat].parLevelVal[1][aktParLevel[1]],
                                    lsv[lsv_strat].parLevelVal[2][aktParLevel[2]]);
         lsv[lsv_strat].parLastLevel[2] = aktParLevel[2];
  #else /* standard (no ROS_3PARAM) */
         ret_state = set_rc_limits (0, 
                                    (INT)lsv[lsv_strat].parLevelVal[0][aktParLevel[0]],
                                    lsv[lsv_strat].parLevelVal[1][aktParLevel[1]],
                                    ROSEN_DEF_STEP_SIZE);
  #endif /* standard (no ROS_3PARAM) */
         lsv[lsv_strat].parLastLevel[0] = aktParLevel[0];
         lsv[lsv_strat].parLastLevel[1] = aktParLevel[1];
         break; /* LSV_ROSEN */

      case LSV_COMPL: 
         ret_state = set_rc_limits ((INT)lsv[lsv_strat].parLevelVal[0][aktParLevel[0]],
                                    0, 0.0, 0.0);
         lsv[lsv_strat].parLastLevel[0] = aktParLevel[0];
         break; /* LSV_COMPL */
#endif /* MIT_ROCO */

#ifdef MIT_GORBA_LSVS  /* ============= Anpassung der GORBA-LSVs ============ */
      case LSV_LVKG: 
         setParamLVKG (lsv[lsv_strat].parLevelVal[0][aktParLevel[0]],
		       lsv[lsv_strat].parLevelVal[1][aktParLevel[1]]);
         lsv[lsv_strat].parLastLevel[0] = aktParLevel[0];
         lsv[lsv_strat].parLastLevel[1] = aktParLevel[1];
         break; /* LSV_LVKG */

      case LSV_LVGK: 
         setParamLVGK (lsv[lsv_strat].parLevelVal[0][aktParLevel[0]],
		       lsv[lsv_strat].parLevelVal[1][aktParLevel[1]]);
         lsv[lsv_strat].parLastLevel[0] = aktParLevel[0];
         lsv[lsv_strat].parLastLevel[1] = aktParLevel[1];
         break; /* LSV_LVGK */

      case LSV_RV: 
         setParamRV (lsv[lsv_strat].parLevelVal[0][aktParLevel[0]]);
         lsv[lsv_strat].parLastLevel[0] = aktParLevel[0];
         break; /* LSV_RV */

      case LSV_RAS: 
         break; /* LSV_RAS */
#endif /* MIT_GORBA_LSVS */

      default:
         ret_state = fatal (EVO_ADI, 55, lsv_strat, NO_INFO, "", UNKNOWN_LSV);
      } /* LSV-spezifische Steuerparameteranpassung */

   return (ret_state);
} /* parameterise_lsv_for_best */



/* ============================= level_adapter ============================== */
static BOOL level_adapter (DOUBLE *levels,
                           DOUBLE *fg_sum,
                           LONG   *eval_sum,
                           INT    *ctr,
                           INT    *all_ctr,
                           INT    *level1,
                           DOUBLE *secondLevPSum,
                           INT     level_max_anz,
                           INT     aktNkl,
                           INT    *upd_ctr)
/*----------------------------------------------------------------------------*/
/*  Inkrementiert den Gesamtzaehler des Levels "all_ctr". Eine Adaption der   */
/*  "levels" und gegebenenfalls des Index des ersten aktiven Levels "level1"  */
/*  erfolgt, wenn mindestens insgesamt "level_gesamt_limit" Anwendungen       */
/*  ("all_ctr") oder zwei Levels mindestens je "level_einzel_limit" Anwen-    */
/*  dungen hatten. Wenn ein Level keine Anwendung hatte, werden nur die ver-  */
/*  bleibenden Level angepasst. Kein Level darf unter einen Anteil von        */
/*  MIN_LEVEL_P sinken.                                                       */
/*----------------------------------------------------------------------------*/
{
   DOUBLE  ql1, ql2, ql3,   /* Quotienten aus fg und eval pro Level           */
           qsum,            /* Summe der beteiligten Quotienten               */
           alt_p,           /* Altes P beider Level bei 2-Level-Adaption      */
           ausgleich;       /* Ausgleich bei zu kleinem Level                 */
   INT     l_kl[3], l_gr[3];/* Hilfsindices (nur die 1.beiden werden benutzt) */
   INT     l1, l2,          /* Die zu adaptierenden Level bei 2 angew. Levels */
           l3,              /* Hilfsindex                                     */
           angew_level_anz, /* Anzahl der angewandten Levels (ctr > 0)        */
           ok_level_anz,    /* Anzahl der Levels mit ctr > level_einzel_limit */
           small_level_anz, /* Anzahl der Levels mit p < MIN_LEVEL_P          */
           old_level;       /* nur fuer SW-Test                               */
   BOOL    update_done;     /* TRUE, wenn mindestens 2 Levels angepasst wurden*/

   /* level_adapter */
#ifdef LEVEL_LOGS
   old_level = *level1;
   sprintf (buf1, "ADI: level_adapter: alt: akt.Level=%d (%g, %g, %g)", 
        *level1, levels[*level1], levels[*level1 + 1],
        levels[*level1 + 2]); 
   write_to_log (buf1, CLOSE_LOG);
   qsum = 1.0;                        /* sonst falsche "alle fg Null"-Anzeige */
#endif /* LEVEL_LOGS */

   update_done     = FALSE;
   angew_level_anz = 0;
   ok_level_anz    = 0;
   if (ctr[*level1] > 0) 
   { /* niedrigster Level wurde angewandt */
      angew_level_anz++;
      l1 = *level1;
      if (ctr[*level1] >= level_einzel_limit)
         ok_level_anz++;
   } /* niedrigster Level wurde angewandt */
   if (ctr[*level1 + 1] > 0) 
   { /* mittlerer Level wurde angewandt */
      if (angew_level_anz == 1)
         l2 = *level1 + 1;
      else
         l1 = *level1 + 1;
      angew_level_anz++;
      if (ctr[*level1 + 1] >= level_einzel_limit)
     ok_level_anz++;
   } /* mittlerer Level wurde angewandt */
   if (ctr[*level1 + 2] > 0) 
   { /* hoechster Level wurde angewandt */
      angew_level_anz++;
      l2 = *level1 + 2;          /* egal, wenn alle 3 Levels angewandt wurden */
      if (ctr[*level1 + 2] >= level_einzel_limit)
     ok_level_anz++;
   } /* hoechster Level wurde angewandt */

   if ((ok_level_anz >= 2) || (*all_ctr >= level_gesamt_limit))
   { /* potentielle Update-Situation */
      if (angew_level_anz == 3)
      { /* all drei Level anpassen */
         l1   = *level1;                                      /* Hilfsindices */
         l2   = l1 + 1;                                       /* Hilfsindices */
         l3   = l2 + 1;                                       /* Hilfsindices */
         ql1  = fg_sum[l1] / eval_sum[l1];
         ql2  = fg_sum[l2] / eval_sum[l2];
         ql3  = fg_sum[l3] / eval_sum[l3];
         qsum = ql1 + ql2 + ql3;
         if (qsum > 0.0) { /* Update ausfuehren */
            levels[l1]  = ql1 / qsum;
            levels[l2]  = ql2 / qsum;
            levels[l3]  = ql3 / qsum;
            update_done = TRUE;
         } /* Update ausfuehren */
      } /* all drei Level anpassen */
      else
         if (angew_level_anz == 2)
         { /* 2 Level (l1 und l2) anpassen */
        ql1  = fg_sum[l1] / eval_sum[l1];
        ql2  = fg_sum[l2] / eval_sum[l2];
        qsum = ql1 + ql2;
        if (qsum > 0.0) { /* Update ausfuehren */
           alt_p       = levels[l1] + levels[l2];
           levels[l1]  = (ql1 * alt_p) / qsum;
           levels[l2]  = (ql2 * alt_p) / qsum;
           update_done = TRUE;
        } /* Update ausfuehren */
     } /* 2 Level (l1 und l2) anpassen */
   } /* potentielle Update-Situation */

   if (update_done)
   { /* Nachbearbeitung des Updates */
      /* ---- Pruefen, ob Level-Verschiebung notwendig und durchfuehren: ---- */
      if (update_done)
      { /* Anpassung wurde ausgefuehrt */
#ifdef LEVEL_LOGS
         sprintf (buf1, "                    neu: akt.Level=%d (%g, %g, %g)", 
                  *level1, levels[*level1], levels[*level1 + 1],
                  levels[*level1 + 2]); 
         write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */
         if ((levels[*level1] > verkleinern_limit) && (*level1 > 0))
         { /* Linken (kleineren) Level hinzunehmen */
            levels[*level1 + 1] += levels[*level1 + 2];
            levels[*level1 + 2]  = 0.0;
            ql1                  = levels[*level1] * 0.2;
            levels[*level1]     -= ql1;
            ql2                  = levels[*level1 + 1] * 0.2;
            levels[*level1 + 1] -= ql2;
            (*level1)--;                           /* neues niedrigstes Level */
            levels[*level1]      = ql1 + ql2;
         } /* Linken (kleineren) Level hinzunehmen */
         else
            if ((levels[*level1 + 2] > vergroessern_limit) && 
                (*level1 < (level_max_anz - 3)))   /*3: 2 used + 1 free level */
            { /* Rechten (groesseren) Level hinzunehmen */
               levels[*level1 + 1] += levels[*level1];
               levels[*level1]      = 0.0;
               (*level1)++;                        /* neues niedrigstes Level */
               ql1                  = levels[*level1] * 0.2;
               levels[*level1]     -= ql1;
               ql2                  = levels[*level1 + 1] * 0.2;
               levels[*level1 + 1] -= ql2;
               levels[*level1 + 2]  = ql1 + ql2;
            } /* Rechten (groesseren) Level hinzunehmen */
#ifdef LEVEL_LOGS
         if (old_level != *level1) {
            sprintf (buf1, "                    akt.Level: %d (%g, %g, %g) nach "
                     "Verschiebung", *level1, levels[*level1], levels[*level1 + 1],
                     levels[*level1 + 2], update_done); 
            write_to_log (buf1, CLOSE_LOG);
         }
#endif /* LEVEL_LOGS */

         /* --------- Level mit weniger als MIN_LEVEL_P ausgleichen: -------- */
         small_level_anz = 0;
         if (levels[*level1] < MIN_LEVEL_P)
         { /* Level mit zu geringem P gefunden */
            l_kl[small_level_anz] = *level1;
            small_level_anz++;
         } /* Level mit zu geringem P gefunden */
         else
            l_gr[0] = *level1;
         if (levels[*level1 + 1] < MIN_LEVEL_P)
         { /* Level mit zu geringem P gefunden */
            l_kl[small_level_anz] = *level1 + 1;
            small_level_anz++;
         } /* Level mit zu geringem P gefunden */
         else
            if (small_level_anz == 1)
               l_gr[0] = *level1 + 1;
            else
               l_gr[1] = *level1 + 1;
         if (levels[*level1 + 2] < MIN_LEVEL_P)
         { /* Level mit zu geringem P gefunden */
            l_kl[small_level_anz] = *level1 + 2;
            small_level_anz++;
         } /* Level mit zu geringem P gefunden */
         else
            if (small_level_anz == 1)
               l_gr[1] = *level1 + 2;
            else
               l_gr[0] = *level1 + 2;
#ifdef LEVEL_LOGS
         if (small_level_anz > 0) {
            sprintf (buf1, "                    small_level_anz=%d  l_kl: %d %d  l_gr:"
                     " %d %d", small_level_anz, l_kl[0], l_kl[1], l_gr[0], l_gr[1]);
            write_to_log (buf1, CLOSE_LOG);
         }
#endif /* LEVEL_LOGS */

         if (small_level_anz == 1)
         { /* ein armes Level gefunden, Ausgleich verteilen */
            ausgleich        = MIN_LEVEL_P - levels[l_kl[0]];
            levels[l_kl[0]]  = MIN_LEVEL_P;
            qsum             = levels[l_gr[0]] + levels[l_gr[1]];
            ql1              = (levels[l_gr[0]] * ausgleich) / qsum;
            levels[l_gr[0]] -= ql1; 
            levels[l_gr[1]] -= (ausgleich - ql1);
         } /* ein armes Level gefunden, Ausgleich verteilen */
         else
            if (small_level_anz == 2)
            { /* zwei arme Level gefunden */
               ausgleich = 2 * MIN_LEVEL_P - levels[l_kl[0]] -levels[l_kl[1]];
               levels[l_kl[0]]  = MIN_LEVEL_P;
               levels[l_kl[1]]  = MIN_LEVEL_P;
               levels[l_gr[0]] -= ausgleich;
            } /* zwei arme Level gefunden */
         *secondLevPSum = levels[*level1] + levels[(*level1) + 1];

#if  defined (LEVEL_LOGS) || defined (ADI_LOGS)
  #if !defined (LEVEL_LOGS) && defined (ADI_LOGS)
         write_to_log ("\n----------------------------------------------------------", CLOSE_LOG);
  #endif /* ADI_LOGS Aber keine LEVEL_LOGS */
         sprintf (buf1, "ADI: level_adapter: Ergebnis fuer Nkl=%d ", aktNkl);
         write_to_log (buf1);
         for (l1 = 0;  l1 < LSV_ANZ;  l1++) 
            if (lsv[l1].lsvOn[aktNkl]) 
            { /* alle aktiven LSVs */
               for (l2 = 0;  l2 < lsv[l1].lsvParAnz;  l2++) { /* alle Params eines LSVs */
                  sprintf(buf1, "                    %s %16s: 1.Level=%d (%g, %g, %g) 2nd" 
                          "LevPSum =%g", lsv[l1].lsvJobLiName, lsv[l1].lsvParName[l2],
                          lsv[l1].par1stActLev[l2][aktNkl], 
                          lsv[l1].parLevelP[l2][aktNkl][lsv[l1].par1stActLev[l2][aktNkl]],
                          lsv[l1].parLevelP[l2][aktNkl][lsv[l1].par1stActLev[l2][aktNkl]+1],
                          lsv[l1].parLevelP[l2][aktNkl][lsv[l1].par1stActLev[l2][aktNkl]+2],
                          lsv[l1].par2ndLevPSum[l2][aktNkl]);
                  write_to_log (buf1, KEEP_LOG_OPEN);
               } /* alle Params eines LSVs */
               if (lsv_all_children) {
                  sprintf(buf1, "                    %s allP            : 1.Level=%d "
                          "(%g, %g, %g) 2ndLevPSum =%g", lsv[l1].lsvJobLiName, 
                          lsv[l1].allP1stActLev[aktNkl],
                          lsv[l1].allPLevelP[aktNkl][lsv[l1].allP1stActLev[aktNkl]],
                          lsv[l1].allPLevelP[aktNkl][lsv[l1].allP1stActLev[aktNkl]+1],
                          lsv[l1].allPLevelP[aktNkl][lsv[l1].allP1stActLev[aktNkl]+2],
                          lsv[l1].allP2ndLevPSum[aktNkl]);
                  write_to_log (buf1, CLOSE_LOG);
               }
            } /* alle aktiven LSVs */
#endif /* LEVEL_LOGS oder ADI_LOGS*/
         upd_ctr[aktNkl]++;
         update_ges_ctr++;
      } /* Anpassung wurde ausgefuehrt */
   }  /* Nachbearbeitung des Updates */
#ifdef LEVEL_LOGS
   else
      if (qsum == 0.0)
         write_to_log ("                    Kein Level-Update, da alle fg Null !", CLOSE_LOG);
#endif /* LEVEL_LOGS */

   return (update_done);
} /* level_adapter */




/* ============================= lsvAbschaltung ============================= */
static void lsvAbschaltung (INT  lsvIdx, 
                            INT  akt_nkl)
/*----------------------------------------------------------------------------*/
/*  Fuehrt die Abschaltung des durch "lsvIdx" bezeichneten LSVs fuer die      */
/*  Notenklasse "akt_nkl" durch. Die verbliebene Wahrscheinlichkeit des LSVs  */
/*  ist vor Aufruf der Routine zu verarbeiten.                                */
/*----------------------------------------------------------------------------*/
{
#ifdef SW_CHECKS
   if (!lsv[lsvIdx].lsvOn[akt_nkl]) { /* LSV ist bereits abgeschaltet */ 
      sprintf (buf3, "lsvAbschaltung: LSV %d:%s ist bereits abgeschaltet!", 
	       lsvIdx, lsv[lsvIdx].lsvMenueName);
      err_state = fatal (EVO_ADI, 56, NO_INFO, NO_INFO, buf3, SIMU_RZ_MELD);
      sprintf (buf2, "***** %s", buf3);
      write_to_log (buf2, CLOSE_LOG);
   } /* LSV ist bereits abgeschaltet */
#endif /* SW_CHECKS */
   lsv[lsvIdx].lsvOn[akt_nkl]   = FALSE;
   lsv[lsvIdx].lsvP[akt_nkl]    = 0.0;
   lsv[lsvIdx].lsvPSel[akt_nkl] = 0.0;
   lsv[lsvIdx].abzuschalten[akt_nkl] = FALSE;
   aktuellBeteilLsvAnz[akt_nkl]--;
   setLSVAdaptSpeed (aktLSVAdaptSpeed, akt_nkl);
   if (aktuellBeteilLsvAnz[akt_nkl] == 1) 
   { /* nur noch 1 LSV */
      lsv_adapt_aktiv[akt_nkl] = FALSE;             /* LSV-Auswahl abschalten */
      single_lsv_bestimmen (akt_nkl);
   } /* nur noch 1 LSV */
#if defined (LSV_SEL_LOGS)
   if (lsv_adapt_aktiv[akt_nkl])
      strcpy (buf2, "TRUE");
   else
      strcpy (buf2, "FALSE");
   sprintf (buf3, "                       lsvAbschaltung fuer LSV %d:%s und NKl=%d  "
	    "lsv_adapt_aktiv[]=%s", lsvIdx, lsv[lsvIdx].lsvMenueName, akt_nkl, buf2);
   write_to_log (buf3, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
} /* lsvAbschaltung */



/* ============================ lsv_erg_fuer_adi ============================ */
void lsv_erg_fuer_adi (INT     akt_nkl,
                       DOUBLE  fg,
                       DOUBLE  f_alt,
                       LONG    aufwand)
{
   DOUBLE  quotient[LSV_ANZ],  /* Quotient pro benutztem LSV: FgSum / EvalSum */
           neuLsvP[LSV_ANZ],   /* neue Wahrscheinlichkeiten der LSVs          */
           qsum,               /* Summe von "quotient" aller beteiligten LSVs */
           deltaP,             /* zu verteilende Restwahrscheinlichkeit       */
           minderP,            /* wg.P_MIN einzuziehende Wahrscheinlichkeit   */
           minderPAnteil,      /* pro LSV mit p(LSV)>lsvPLimit einzuziehend.P */
           lsvPLimit,          /* p(LSV)-Grenze damit nach Abzug P_MIN uebrig */
           fg_rel,             /* relativer Fitnessgewinn                     */
           einzelLimitOkAnz,   /* Anzahl LSVs mit lsvCtr >= lsv_einzel_limit  */
           altPSum,            /* Summe der alten p(LSV) der benutzten LSVs   */
           nenner;             /* Hilfsvariable                               */
   INT     lastLevel,          /* letzter benutzter Level: Daten fuer diesen  */
           benutzteLsvAnz,     /* Anzahl benutzter LSV, d.h. lsvCtr > 0       */
           anzPMinLsv,         /* Anzahl auf auf P_MIN angehobener LSVs       */
           lsvPBleibtAnz,      /* Anz. LSVs mit: P_MIN <= p(LSV) <= lsvPLimit */
           pp, firstLevel, ii, jj;
   BOOL    update_done,
           benutztesLsv[LSV_ANZ], /* TRUE: beteiligtes LSV  mit lsvCtr > 0    */
           alleLsvAnpassen;    /* TRUE: Wahrsch.aller beteiligten LSVs anpass.*/

   /* lsv_erg_fuer_adi */
   update_done = FALSE;
   nenner      = RNOTEN_MAX - f_alt;
   if (nenner < 0.001)
      fg_rel = 0.0;
   else
      fg_rel = fg / nenner;
#if defined (LSV_SEL_LOGS) || defined (LEVEL_LOGS)
   sprintf (buf1, "ADI: lsv_erg_fuer_adi: f_alt=%g fg=%g fg_rel=%g aufwand=%ld nkl=%d", 
            f_alt, fg, fg_rel, aufwand, akt_nkl);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS oder LEVEL_LOGS */

   for (pp = 0;  pp < lsv[lsv_strat].lsvParAnz;  pp++)
   { /* alle Parameter eines LSVs */
      /* ------------------------- Datenerfassung: -------------------------- */
      lastLevel = lsv[lsv_strat].parLastLevel[pp];
      lsv[lsv_strat].parFgSum[pp][akt_nkl][lastLevel]   += fg_rel;
      lsv[lsv_strat].parEvalSum[pp][akt_nkl][lastLevel] += aufwand;
      lsv[lsv_strat].parCtr[pp][akt_nkl][lastLevel]++;
      lsv[lsv_strat].parAllCtr[pp][akt_nkl]++;
#ifdef LEVEL_LOGS
      firstLevel = lsv[lsv_strat].par1stActLev[pp][akt_nkl];
      sprintf (buf1, "ADI: lsv_erg_fuer_adi: %s %s  ctr: %d (%d %d %d)  "
               "fg: %g %g %g  eval: %ld %ld %ld", 
               lsv[lsv_strat].lsvJobLiName, lsv[lsv_strat].lsvParName[pp], 
               lsv[lsv_strat].parAllCtr[pp][akt_nkl],
               lsv[lsv_strat].parCtr[pp][akt_nkl][firstLevel],
               lsv[lsv_strat].parCtr[pp][akt_nkl][firstLevel+1],
               lsv[lsv_strat].parCtr[pp][akt_nkl][firstLevel+2],
               lsv[lsv_strat].parFgSum[pp][akt_nkl][firstLevel],
               lsv[lsv_strat].parFgSum[pp][akt_nkl][firstLevel+1],
               lsv[lsv_strat].parFgSum[pp][akt_nkl][firstLevel+2],
               lsv[lsv_strat].parEvalSum[pp][akt_nkl][firstLevel],
               lsv[lsv_strat].parEvalSum[pp][akt_nkl][firstLevel+1],
               lsv[lsv_strat].parEvalSum[pp][akt_nkl][firstLevel+2]);
      write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */

      /* -------------------------- Level-Updates: ------------------------- */
      if (level_adapter (lsv[lsv_strat].parLevelP[pp][akt_nkl],
                         lsv[lsv_strat].parFgSum[pp][akt_nkl],
                         lsv[lsv_strat].parEvalSum[pp][akt_nkl],
                         lsv[lsv_strat].parCtr[pp][akt_nkl],
                         &(lsv[lsv_strat].parAllCtr[pp][akt_nkl]),
                         &(lsv[lsv_strat].par1stActLev[pp][akt_nkl]),
                         &(lsv[lsv_strat].par2ndLevPSum[pp][akt_nkl]),
                         lsv[lsv_strat].parLevelAnz[pp],
                         akt_nkl, update_ctr)) {
         reset_level_ctr (lsv_strat, pp, akt_nkl);
         update_done = TRUE;
      }
   } /* alle Parameter eines LSVs */

   if (lsv_adapt_aktiv[akt_nkl])
   { /* mindesetens 2 LSVs konkurrieren noch fuer diese Notenklasse */
      /* ------------------------- Datenerfassung: -------------------------- */
      lsv[lsv_strat].lsvCtr[akt_nkl]++;
      lsv[lsv_strat].lsvEvalSum[akt_nkl] += aufwand;
      lsv[lsv_strat].lsvFgSum[akt_nkl]   += fg_rel;
      lsv_anw_ctr[akt_nkl]++;
#ifdef LSV_SEL_LOGS
      sprintf (buf1, "ADI: lsv_erg_fuer_adi: LSV-Ausw: nkl=%d LSV=%s: ctr=%d fg=%g eval=%ld"
	       "  lsv_einzel_limit=%d lsv_gesamt_limit=%d",
               akt_nkl, lsv[lsv_strat].lsvMenueName, lsv[lsv_strat].lsvCtr[akt_nkl],
               lsv[lsv_strat].lsvFgSum[akt_nkl], lsv[lsv_strat].lsvEvalSum[akt_nkl],
	       lsv_einzel_limit[akt_nkl], lsv_gesamt_limit[akt_nkl]);
      write_to_log (buf1, CLOSE_LOG);
      sprintf (buf1, "                       LSV-Ausw: (LSV: lsvCtr/benutztesLSV):");
#endif /* LSV_SEL_LOGS */

      /* --------------------------- LSV-Update: ---------------------------- */
      benutzteLsvAnz   = 0;
      einzelLimitOkAnz = 0;
      memset(benutztesLsv, 0, bsize * LSV_ANZ);
      for (ii = 0;  ii < LSV_ANZ;  ii++)
         if (lsv[ii].lsvOn)                            /* LSV mit p(LSV)>=0 ? */
         { /* alle aktiven LSVs mit p(LSV)>0 (beteiligtes LSV): Update? */
            if (lsv[ii].lsvCtr[akt_nkl] >= lsv_einzel_limit[akt_nkl])
               einzelLimitOkAnz++;
            if (lsv[ii].lsvCtr[akt_nkl] > 0) { /* benutztes LSV */
               benutztesLsv[ii] = TRUE;
               benutzteLsvAnz++;
            } /* benutztes LSV */
#ifdef LSV_SEL_LOGS
	    sprintf (buf2, " (%d: %d/%d)", ii, lsv[ii].lsvCtr[akt_nkl], benutztesLsv[ii]);
	    strcat (buf1, buf2);
#endif /* LSV_SEL_LOGS */
         } /* alle aktiven LSVs mit p(LSV)>0 (beteiligtes LSV): Update? */
#ifdef LSV_SEL_LOGS
      write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
      if (((lsv_anw_ctr[akt_nkl] >= lsv_gesamt_limit[akt_nkl]) ||     /* Li-  */
           (einzelLimitOkAnz == aktuellBeteilLsvAnz[akt_nkl])    ) && /* mits */
          (benutzteLsvAnz > 1))        /* mind. 2 benutzte LSV (lsv(Ctr) > 0) */
      { /* Ein LSV-Update steht an */
         alleLsvAnpassen = (benutzteLsvAnz == aktuellBeteilLsvAnz[akt_nkl]);
         qsum    = 0.0;
         altPSum = 0.0;
         for (ii = 0;  ii < LSV_ANZ;  ii++)
            if (benutztesLsv[ii])
            { /* alle benutzten LSVs: Quotienten u. deren Summe */
               quotient[ii] = lsv[ii].lsvFgSum[akt_nkl] / lsv[ii].lsvEvalSum[akt_nkl];
               qsum        += quotient[ii];
               if (!alleLsvAnpassen)
                  altPSum += lsv[ii].lsvP[akt_nkl];
            } /* alle benutzten LSVs: Quotienten u. deren Summe */
         if (qsum > 0.0)
         { /* Update ausfuehren */
            anzPMinLsv = 0;
            minderP    = 0.0;
            deltaP     = 0.0;  /*summiert nachtraeglich zu verechnendes p(LSV)*/
#if defined (LSV_SEL_LOGS)
            sprintf (buf1, "                       LSV-Ausw: benutzteLsvAnz=%d "
                     "aktuellBeteilLsvAnz=%d (LSV: neuLsvP): ", benutzteLsvAnz, 
                     aktuellBeteilLsvAnz[akt_nkl]);
#endif /* LSV_SEL_LOGS */
            for (ii = 0;  ii < LSV_ANZ;  ii++)
               if (benutztesLsv[ii])
               { /* alle benutzten LSVs: neues Roh-P berechn.u.Abschaltung pruefen */
                  if (alleLsvAnpassen)
                     neuLsvP[ii] = quotient[ii] / qsum;
                  else
                     neuLsvP[ii] = (quotient[ii] * altPSum) / qsum;
                  if (mitAltPAnteil)
                     neuLsvP[ii] = (oldDistribFrac * lsv[ii].lsvP[akt_nkl]) +
                        ((1.0 - oldDistribFrac) * neuLsvP[ii])     ;
#if defined (LSV_SEL_LOGS)
                  sprintf (buf2, " (%d: %6.4f)", ii, neuLsvP[ii]);
                  strcat (buf1, buf2);
		  if (lsv[ii].abzuschalten[akt_nkl])
		     write_to_log ("                       abzuschalteneds LSV ist "
				   "benutztes/aktuelles !!!", CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
                  if (neuLsvP[ii] < lsvPMin[akt_nkl]) 
                  { /* minimales p(LSV) unterschritten */
                     lsv[ii].lsvFinCtr[akt_nkl]++;
                     if (lsv[ii].lsvFinCtr[akt_nkl] > lsv_nix_anwendung_limit)
                     { /* LSV muesste abgeschaltet werden */
#if defined (LSV_SEL_LOGS)
			sprintf (buf2, "                       LSV-Ausw: abzuschaltendes LSV %d:%s"
				 ", aktuelles LSV: %d:%s  aktuellBeteilLsvAnz=%d  ", 
				 ii, lsv[ii].lsvMenueName, lsv_strat, 
				 lsv[lsv_strat].lsvMenueName, aktuellBeteilLsvAnz[akt_nkl]);
			if ((lsv_strat != ii) || (aktuellBeteilLsvAnz[akt_nkl] == 2))
			   sprintf (buf3, "wird abgeschaltet!");
			else
			   sprintf (buf3, "Abschaltung vorgemerkt!");
                        strcat (buf2, buf3);
			write_to_log (buf2, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
			if ((lsv_strat != ii) || (aktuellBeteilLsvAnz[akt_nkl] == 2))
			/* abzuschaltendes LSV ist nicht das aktuelle und nicht das vorletzte */
                        { /* LSV abschalten */
			   deltaP += neuLsvP[ii];      /* zu verteilendes p(LSV) */
			   benutztesLsv[ii] = FALSE;
			   benutzteLsvAnz--;
			   lsvAbschaltung (ii, akt_nkl);
			} /* LSV abschalten */
			else { /* Abschaltung vormerken */
			   lsv[ii].abzuschalten[akt_nkl] = TRUE;
			   if (neuLsvP[ii] < P_MIN) 
			   { /* einzuziehendes P berechnen, da es noch nicht abgeschaltet ist */
			      minderP += P_MIN - neuLsvP[ii];
			      anzPMinLsv++;
			      neuLsvP[ii] = P_MIN;  
			   } /* einzuziehendes P berechnen, da es noch nicht abgeschaltet ist */
			} /* Abschaltung vormerken */
		     } /* LSV muesste abgeschaltet werden */
                     else
                        if (neuLsvP[ii] < P_MIN) 
                        { /* Rueckkehrchance: einzuziehendes P berechnen */
                           minderP += P_MIN - neuLsvP[ii];
                           anzPMinLsv++;
                           neuLsvP[ii] = P_MIN;  
                        } /* Rueckkehrchance: einzuziehendes P berechnen */
                  } /* minimales p(LSV) unterschritten */
                  else { /* neu p(LSV) >= min(p(LSV)) */
                     lsv[ii].lsvFinCtr[akt_nkl]    = 0;
		     lsv[ii].abzuschalten[akt_nkl] = FALSE;
		  } /* neu p(LSV) >= min(p(LSV)) */
               } /* alle benutzten LSVs: neues Roh-P berechn.u.Abschaltung pruefen */
	       else
		  if (lsv[ii].abzuschalten[akt_nkl])
		  { /* unbenutzes LSV ist abzuschalten */
		     deltaP += lsv[ii].lsvP[akt_nkl]; 
		     lsvAbschaltung (ii, akt_nkl);
		  } /* unbenutzes LSV ist abzuschalten */
#if defined (LSV_SEL_LOGS)
            write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
            if (lsv_adapt_aktiv[akt_nkl])
            { /* LSV-Konkurrenz besteht weiter */
               if (anzPMinLsv > 0)
               { /* minderP anteilig abziehen */
                  nenner = benutzteLsvAnz - anzPMinLsv;
#ifdef SW_CHECKS
                  if (nenner <= 0) {
                     sprintf (buf2, "lsv_erg_fuer_adi: nenner %g <= 0 !", nenner);
                     err_state = fatal (EVO_ADI, 57, NO_INFO, NO_INFO, buf2, SIMU_RZ_MELD);
                     sprintf (buf3, "***** %s", buf2);
                     write_to_log (buf3, CLOSE_LOG);
                     nenner = 1;
                  }
#endif /* SW_CHECKS */
                  minderPAnteil = minderP / nenner;
                  lsvPLimit     = P_MIN + minderPAnteil;
#if defined (LSV_SEL_LOGS)
                  sprintf (buf3, "                       LSV-Ausw: Ausgleich: benutzte"
                           "LsvAnz=%d anzPMinLsv=%d  minderP=%6.4f minderPAnteil=%6.4f"
                           " lsvPLimit=%6.4f", benutzteLsvAnz, anzPMinLsv, minderP, 
                           minderPAnteil, lsvPLimit);
                  write_to_log (buf3, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
                  lsvPBleibtAnz = 0;
                  for (ii = 0;  ii < LSV_ANZ;  ii++)
                     if (benutztesLsv[ii]) 
                        if ((P_MIN < neuLsvP[ii]) && (neuLsvP[ii] <= lsvPLimit))
                           lsvPBleibtAnz++;
                  if (lsvPBleibtAnz > 0) 
                  { /* LSVs mit etwas grosserer lsvP als P_MIN bleiben unveraendert */
                     nenner -= lsvPBleibtAnz;
#ifdef SW_CHECKS
                     if (nenner <= 0) {
                        sprintf (buf2, "lsv_erg_fuer_adi: angepasster nenner %g <= 0 !", 
                                 nenner);
                        err_state = fatal (EVO_ADI, 58, NO_INFO, NO_INFO, buf2, 
                                           SIMU_RZ_MELD);
                        sprintf (buf3, "***** %s", buf2);
                        write_to_log (buf3, CLOSE_LOG);
                        nenner = 1;
                     }
#endif /* SW_CHECKS */
#if defined (LSV_SEL_LOGS)
                     sprintf (buf3, "                       LSV-Ausw: Ausgleich: "
                              "lsvPBleibtAnz=%d korrigierter minderPAnteil=%6.4f", 
                              lsvPBleibtAnz, minderP/nenner);
                     write_to_log (buf3, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
                     minderPAnteil = minderP / nenner;  /* Neuberechnung */
                  } /* LSVs mit etwas grosserer lsvP als P_MIN bleiben unveraendert */
                  for (ii = 0;  ii < LSV_ANZ;  ii++)
                     if (benutztesLsv[ii])
                        if (neuLsvP[ii] > lsvPLimit)
                           lsv[ii].lsvP[akt_nkl] = neuLsvP[ii] - minderPAnteil;
                        else
                           lsv[ii].lsvP[akt_nkl] = neuLsvP[ii];
               } /* minderP anteilig abziehen */
               else
               { /* keine P_MIN-Faelle */
                  for (ii = 0;  ii < LSV_ANZ;  ii++)
                     if (benutztesLsv[ii])
                        lsv[ii].lsvP[akt_nkl] = neuLsvP[ii];
               } /* keine P_MIN-Faelle */
#if defined (LSV_SEL_LOGS) || defined (ADI_LOGS)
               sprintf (buf1, "                       LSV-Ausw. nkl=%d benutzteLsvAnz=%d"
                        " aktuellBeteilLsvAnz=%d lsv_adapt_aktiv=%d deltaP=%g", akt_nkl, 
                        benutzteLsvAnz, aktuellBeteilLsvAnz[akt_nkl], 
                        lsv_adapt_aktiv[akt_nkl], deltaP);
               write_to_log (buf1, CLOSE_LOG);
               for (ii = 0;  ii < LSV_ANZ;  ii++)
                  if (lsv[ii].aktiv) { /* LSV gemaess Genmodell aktiv */
                     sprintf (buf1, "                       %11s: p=%6.4f "
                              "lsvOn=%d FinCtr=%d abzuschalten=%d", lsv[ii].lsvMenueName,  
                              lsv[ii].lsvP[akt_nkl], lsv[ii].lsvOn[akt_nkl], 
                              lsv[ii].lsvFinCtr[akt_nkl], lsv[ii].abzuschalten[akt_nkl]);
                     write_to_log (buf1, CLOSE_LOG);
                  } /* LSV gemaess Genmodell aktiv */
#endif /* LSV_SEL_LOGS oder ADI_LOGS */
               lsvP_und_lsvPSel_bestimmen (akt_nkl, deltaP);
            } /* LSV-Konkurrenz besteht weiter */
#if defined (LSV_SEL_LOGS) || defined (ADI_LOGS)
            else {
               sprintf (buf1, "                       LSV-Ausw. nkl=%d abgeschaltet: "
                        "aktuellBeteilLsvAnz=%d lsv_adapt_aktiv=%d LSV=%d:%s", akt_nkl, 
                        aktuellBeteilLsvAnz[akt_nkl], lsv_adapt_aktiv[akt_nkl], 
                        singleLsv[akt_nkl], lsv[singleLsv[akt_nkl]].lsvMenueName);
               write_to_log (buf1, CLOSE_LOG);
            }
#endif /* LSV_SEL_LOGS oder ADI_LOGS */
            update_done = TRUE;
            update_ges_ctr++;
            update_ctr[akt_nkl]++;
            reset_lsv_ctr (akt_nkl);
         } /* Update ausfuehren */
#if defined (LSV_SEL_LOGS)
         else 
            write_to_log ("                       LSV-Ausw. keine Anpassung der "
                          "LSV-Verteilung, da alle fg Null !", CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
      } /* Ein LSV-Update steht an */
   } /* mindesetens 2 LSVs konkurrieren noch fuer diese Notenklasse */
#if defined (LSV_SEL_LOGS) || defined (LEVEL_LOGS)
   write_to_log ("---------------------------------------------------------", CLOSE_LOG);
#endif /* LSV_SEL_LOGS oder LEVEL_LOGS */
   if (update_done)
      log_adi_statistik (akt_nkl);
} /* lsv_erg_fuer_adi */



/* ================================ get_lsv_p =============================== */
DOUBLE get_lsv_p (INT  akt_nkl)
{
   DOUBLE  p_wert, pw;
   INT     idx, akt_level;

   /* get_lsv_p */
#ifdef LSV_SEL_LOGS
   strcpy (buf2, "");
#endif /* LSV_SEL_LOGS */
   if (!lsv[lsv_strat].nklValid[akt_nkl])
      activate_nkl (akt_nkl);
   idx = lsv[lsv_strat].allP1stActLev[akt_nkl];             /* 1. aktiver Level */
   pw = frand();
   if (pw < lsv[lsv_strat].allPLevelP[akt_nkl][idx])
      akt_level = idx;
   else
      if (pw < lsv[lsv_strat].allP2ndLevPSum[akt_nkl])
         akt_level = idx + 1;
      else
         akt_level = idx + 2;
   p_wert = allPLevelVal[akt_level];

#ifdef LSV_SEL_LOGS
   sprintf (buf1, "ADI: get_lsv_p: %s akt_nkl=%d akt_level=%d p_wert=%g", buf2,
            akt_nkl, akt_level, p_wert);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LSV_SEL_LOGS */
   return (p_wert);
} /* get_lsv_p */



/* ========================= get_level_from_fraction ======================== */
static INT get_level_from_fraction (DOUBLE  fraction,
                                    INT     level1)
/*----------------------------------------------------------------------------*/
/*  Ermittelt den  Level-Index an Hand der tatsaechlichen LSV-Anwendungsrate  */
/*  "fraction" in "allPLevelVal" ab "level1" im Rahmen der aktiven Level und  */
/*  liefert den Index ab.                                                     */
/*----------------------------------------------------------------------------*/
{
   INT  level_idx;              /* Level-Index gemaess der "fraction"        */

   /* get_level_from_fraction */
   for (level_idx = level1;  
        (level_idx < allPLevelAnz - 1) && (allPLevelVal[level_idx] < fraction);  
        level_idx++)
      ;
   if (level_idx > level1 + 2)
      level_idx = level1 + 2;
   return (level_idx);
} /* get_level_from_fraction */



/* ============================ all_erg_fuer_adi ============================ */
void all_erg_fuer_adi (INT     akt_nkl,
                       DOUBLE  fg,
                       DOUBLE  f_alt,
                       LONG    aufwand,
                       DOUBLE  lsv_ist_rate)
{
   DOUBLE  nenner,               /* Nenner fuer die Bildung von fg_rel        */
           fg_rel;               /* relativer Fitnessgewinn                   */
   INT     level_idx,            /* Level-Index gemaess der tatsaechl.Anwndg. */
           level1;               /* Level1 der akt. Notenklasse               */
   BOOL    update_done = FALSE;  /* TRUE bei Level-Anpassung                  */

   /* all_erg_fuer_adi */
   nenner = RNOTEN_MAX - f_alt;
   if (nenner < 0.001)
      fg_rel = 0.0;
   else
      fg_rel = fg / nenner;
#ifdef LEVEL_LOGS
   sprintf (buf1, "ADI: all_erg_fuer_adi: f_alt=%g fg=%g fg_rel=%g  aufwand=%ld"
            "  nkl=%d  ist_rate=%g ", 
            f_alt, fg, fg_rel, aufwand, akt_nkl, lsv_ist_rate);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */

   level1    = lsv[lsv_strat].allP1stActLev[akt_nkl];
   level_idx = get_level_from_fraction (lsv_ist_rate, level1);
#ifdef LEVEL_LOGS
   sprintf (buf1, "                       %s: level1=%d level_idx=%d",
        lsv[lsv_strat].lsvMenueName, level1, level_idx);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */
   lsv[lsv_strat].allPFgSum[akt_nkl][level_idx]   += fg_rel;
   lsv[lsv_strat].allPEvalSum[akt_nkl][level_idx] += aufwand;
   lsv[lsv_strat].allPCtr[akt_nkl][level_idx]++;
   lsv[lsv_strat].allPAllCtr[akt_nkl]++;
#ifdef LEVEL_LOGS
   sprintf (buf1, "ADI: all_erg_fuer_adi: %s allP ctr: %d (%d %d %d)  "
            "fg: %g %g %g  eval: %ld %ld %ld", lsv[lsv_strat].lsvMenueName,
            lsv[lsv_strat].allPAllCtr[akt_nkl],
            lsv[lsv_strat].allPCtr[akt_nkl][level1],
            lsv[lsv_strat].allPCtr[akt_nkl][level1+1],
            lsv[lsv_strat].allPCtr[akt_nkl][level1+2],
            lsv[lsv_strat].allPFgSum[akt_nkl][level1],
            lsv[lsv_strat].allPFgSum[akt_nkl][level1+1],
            lsv[lsv_strat].allPFgSum[akt_nkl][level1+2],
            lsv[lsv_strat].allPEvalSum[akt_nkl][level1],
            lsv[lsv_strat].allPEvalSum[akt_nkl][level1+1],
            lsv[lsv_strat].allPEvalSum[akt_nkl][level1+2]);
   write_to_log (buf1, CLOSE_LOG);
#endif /* LEVEL_LOGS */
   if (level_adapter (lsv[lsv_strat].allPLevelP[akt_nkl], 
                      lsv[lsv_strat].allPFgSum[akt_nkl], 
                      lsv[lsv_strat].allPEvalSum[akt_nkl], 
                      lsv[lsv_strat].allPCtr[akt_nkl], 
                      &(lsv[lsv_strat].allPAllCtr[akt_nkl]), 
                      &(lsv[lsv_strat].allP1stActLev[akt_nkl]),
                      &(lsv[lsv_strat].allP2ndLevPSum[akt_nkl]),
                      allPLevelAnz, akt_nkl, allP_update_ctr)) {
      reset_allP_level_ctr (lsv_strat, akt_nkl);
      allP_upd_ges_ctr++;      /* Summe aller allP-Updates aller LSVs pro Job */
      update_done = TRUE;
   }
#ifdef LEVEL_LOGS
   write_to_log ("------------------------------------------------------------"
                 "------------------------------------------\n", CLOSE_LOG);
#endif /* LEVEL_LOGS */
   if (update_done) 
      log_adi_statistik (akt_nkl);
} /* all_erg_fuer_adi */



/* =========================== get_best_level_idx =========================== */
static INT get_best_level_idx (DOUBLE *levels,
                               INT     level1,
                               BOOL    nkl_valid)
/*----------------------------------------------------------------------------*/
/*  Ermittelt den besten Level in "levels" ab "level1" und liefert den Index  */
/*  ab, wenn "nkl_valid" gesetzt ist. Ansonsten ist das Level-Feld der ueber- */
/*  gebenen Notenklasse unbenutzt und es wird "level1" abgeliefert.           */
/*----------------------------------------------------------------------------*/
{
   INT  lbest;

   /* get_best_level_idx */
   if (nkl_valid)
   { /* Level-Feld der uebergebenen Notenklasse ist benutzt */
      lbest = level1 + 1;
      if (levels[lbest] < levels[level1])
         lbest = level1;
      if (levels[lbest] < levels[level1 + 2])
         lbest = level1 + 2;
   } /* Level-Feld der uebergebenen Notenklasse ist benutzt */
   else
      lbest = level1;
   return (lbest);
} /* get_best_level_idx */



/* =============================== get_max_nkl ============================== */
static INT get_max_nkl (void)
/*----------------------------------------------------------------------------*/
/*  Ermittelt die hoechste aktive Notenklasse und liefert deren Index ab.     */
/*----------------------------------------------------------------------------*/
{
   INT  max_nkl, max_lsv_nkl, ii;

   /* get_max_nkl */
   if (gleam_amma) 
   { /* Hoechste aktive Notenklasse aus "nklasse_valid" bestimmen */
      max_nkl = nklassen_anz - 1;
      while (!nklasse_valid[max_nkl])
         max_nkl--;
   } /* Hoechste aktive Notenklasse aus "nklasse_valid" bestimmen */
   else
   { /* Hoechste aktive Notenklasse aus "nklValid" der LSVs bestimmen */
      max_nkl = 0;
      for (ii = 0;  ii < LSV_ANZ;  ii++)
         if (lsv[ii].aktiv) {
            max_lsv_nkl = nklassen_anz - 1;
            while (!lsv[ii].nklValid[max_lsv_nkl])
               max_lsv_nkl--;
            if (max_lsv_nkl > max_nkl)
               max_nkl = max_lsv_nkl;
         }
   } /* Hoechste aktive Notenklasse aus "nklValid" der LSVs bestimmen */
   return (max_nkl);
} /* get_max_nkl */



/* ================================ init_adi ================================ */
void init_adi (INT  lsvAdaptSpeed,
               INT  levelAdaptSpeed)
{
   INT  ii;

   /* init_adi */
   initialBeteilLsvAnz = 0;
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if (lsv[ii].aktiv)
         if (lsv[ii].lsvStartP > 0.0)
            initialBeteilLsvAnz++;
   aktuellBeteilLsvAnz[0] = initialBeteilLsvAnz;
   startLsvPMin           = LSV_PMIN_FRAC / initialBeteilLsvAnz;
   setLSVAdaptSpeed (lsvAdaptSpeed, 0);
   setLevelAdaptSpeed (levelAdaptSpeed);
} /* init_adi */



/* =============================== setPMinLSV =============================== */
static void setPMinLSV (INT  aktNkl)
/*----------------------------------------------------------------------------*/
/*  Setzt "lsvPMin" auf 20% des Anteils eines LSVs, der sich bei gleicher     */
/*  Wahrscheinlichkeit aller beteiligten LSVs der Notenklasse "aktNkl"        */
/*  ergibt.                                                                   */
/*----------------------------------------------------------------------------*/
{
   lsvPMin[aktNkl] = LSV_PMIN_FRAC / aktuellBeteilLsvAnz[aktNkl];
} /* setPMinLSV */



/* ============================ setLSVAdaptSpeed ============================ */
void setLSVAdaptSpeed (INT  adaptSpeed,
                       INT  aktNkl)
{
   INT  lsvAnzIdx;

   /* setLSVAdaptSpeed */
   aktLSVAdaptSpeed = adaptSpeed;          /* gesetzte Geschwindigkeit merken */
   if (aktuellBeteilLsvAnz[aktNkl] == 1)
      lsvAnzIdx = 0;
   else
      if (aktuellBeteilLsvAnz[aktNkl] > (LSV_EINTR_ANZ + 1))
      { /* zu viel aktive LSVs */
         lsvAnzIdx = LSV_EINTR_ANZ - 1;
         meldung (EVO_ADI, LSV_EINTR_ANZ + 1, aktuellBeteilLsvAnz[aktNkl], "", 
                  TOO_MANY_ACT_LSV);
      } /* zu viel aktive LSVs */
      else
         lsvAnzIdx = aktuellBeteilLsvAnz[aktNkl] - 2;
   lsv_einzel_limit[aktNkl] = lsvEinzelLimit[adaptSpeed][lsvAnzIdx];
   lsv_gesamt_limit[aktNkl] = lsvGesamtLimit[adaptSpeed][lsvAnzIdx];
   switch (adaptSpeed) {
   case ADAPT_SPEED_SCHNELL:
      strcpy (lsv_adapt_speed_txt, ADI_SCHNELL_TXT);
      break; /* ADAPT_SPEED_SCHNELL */

   case ADAPT_SPEED_MITTEL:
      strcpy (lsv_adapt_speed_txt, ADI_MITTEL_TXT);
      break; /* ADAPT_SPEED_SCHNELL */

   case ADAPT_SPEED_LANGSAM:
      strcpy (lsv_adapt_speed_txt, ADI_LANGSAM_TXT);
      break; /* ADAPT_SPEED_SCHNELL */
   } /* switch */
} /* setLSVAdaptSpeed */



/* =========================== setLevelAdaptSpeed =========================== */
void setLevelAdaptSpeed (INT  adaptSpeed)
{
   /* setLevelAdaptSpeed */
   aktLevelAdaptSpeed = adaptSpeed;        /* gesetzte Geschwindigkeit merken */
   switch (adaptSpeed) {
   case ADAPT_SPEED_SCHNELL:
      level_einzel_limit =  3;             /* Mindestanw.eines einzeln.Lev.*/
      level_gesamt_limit = 12;             /* Mindestanw. aller Levels     */
      strcpy (level_adapt_speed_txt, ADI_SCHNELL_TXT);
      break; /* ADAPT_SPEED_SCHNELL */

   case ADAPT_SPEED_MITTEL:
      level_einzel_limit =  4;             /* Mindestanw.eines einzeln.Lev.*/
      level_gesamt_limit = 15;             /* Mindestanw. aller Levels     */
      strcpy (level_adapt_speed_txt, ADI_MITTEL_TXT);
      break; /* ADAPT_SPEED_SCHNELL */

   case ADAPT_SPEED_LANGSAM:
      level_einzel_limit =  7;             /* Mindestanw.eines einzeln.Lev.*/
      level_gesamt_limit = 25;             /* Mindestanw. aller Levels     */
      strcpy (level_adapt_speed_txt, ADI_LANGSAM_TXT);
      break; /* ADAPT_SPEED_SCHNELL */
   } /* switch */
} /* setLevelAdaptSpeed */



/* ============================= getAdaptSpeeds ============================= */
void getAdaptSpeeds (INT *lsvAdaptSpeed,
                     INT *levelAdaptSpeed)
{
   *lsvAdaptSpeed   = aktLSVAdaptSpeed;
   *levelAdaptSpeed = aktLevelAdaptSpeed;
} /* getAdaptSpeeds */



/* ======================== get_adi_statistik_string ======================== */
void get_adi_statistik_string (char *str)
{
   INT   best_level_idx, max_nkl,
         ii, jj, kk;
   BOOL  not_found;
   char  buf[30], buf2[10];

   /* get_adi_statistik_string */
   max_nkl = get_max_nkl ();
   sprintf (str, " %d %d %d  %d", 
        update_ges_ctr, allP_upd_ges_ctr, max_nkl + 1, update_ctr[max_nkl]);
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if ((gleam_amma && lsv[ii].aktiv) || (gleam_asma && (lsv_strat == ii)))
      { /* alle aktiven LSVs bzw. das benutzte LSV */
         best_level_idx = get_best_level_idx(lsv[ii].allPLevelP[max_nkl], 
                                             lsv[ii].allP1stActLev[max_nkl],
                                             lsv[ii].nklValid[max_nkl]);
         if (gleam_amma)
            sprintf (buf2, "%6.4f", lsv[ii].lsvP[max_nkl]);
         else
            strcpy (buf2, "1.0000");
         sprintf (buf, "  %s %d", buf2, best_level_idx + 1);
         strcat (str, buf);
         for (jj = 0;  jj < lsv[ii].lsvParAnz;  jj++)
         { /* alle Parameter eines LSVs */
            best_level_idx = get_best_level_idx (lsv[ii].parLevelP[jj][max_nkl],
                                                 lsv[ii].par1stActLev[jj][max_nkl], 
                                                 lsv[ii].nklValid[max_nkl]);
            sprintf (buf, " %d", best_level_idx + 1);
            strcat (str, buf);
         } /* alle Parameter eines LSVs */
      } /* alle aktiven LSVs bzw. das benutzte LSV */

   for (kk = 0;  kk <= max_nkl;  kk++)
   { /* Erfassung der Gesamtstatistik fuer alle Notenklassen */
      if (gleam_amma)                         /* AMMA: LSV-Auswahl-Statistik: */
         if (nklasse_valid[kk])
         { /* Notenklasse wurde fuer LSV-Wahl verwendet */
            for (ii = 0;  ii < LSV_ANZ;  ii++)
               if (lsv[ii].aktiv) 
               { /* alle aktiven LSVs */
                  lsv[ii].statLsvPNklSum[kk] += lsv[ii].lsvP[kk];
                  not_found = TRUE;
                  for (jj = 0;  (jj < 10) && not_found;  jj++)
                     if (lsv[ii].lsvP[kk] <= (0.1 * (jj + 1))) {
                        not_found = FALSE;
                        lsv[ii].statLsvPNklHisto[kk][jj]++;
                     }
               } /* alle aktiven LSVs */
         } /* Notenklasse wurde fuer LSV-Wahl verwendet */
      for (ii = 0;  ii < LSV_ANZ;  ii++)
         if (lsv[ii].aktiv && lsv[ii].nklValid[kk])
         { /* alle aktiven LSVs, deren NKL f.LSV-Par oder all-P benutzt wurde */
            for (jj = 0;  jj < lsv[ii].lsvParAnz;  jj++)
            { /* alle Parameter eines LSVs */
               best_level_idx = get_best_level_idx (lsv[ii].parLevelP[jj][kk],
                                                    lsv[ii].par1stActLev[jj][kk], TRUE);
               lsv[ii].statParBestLSum[jj][kk][best_level_idx]++;
            } /* alle Parameter eines LSVs */
            best_level_idx = get_best_level_idx(lsv[ii].allPLevelP[kk], 
                                                lsv[ii].allP1stActLev[kk], TRUE);
            lsv[ii].statAllPBestLSum[kk][best_level_idx]++;
            lsv[ii].statAllPJobAnz[kk]++;
         } /* alle aktiven LSVs */
   } /* Erfassung der Gesamtstatistik fuer alle Notenklassen */
} /* get_adi_statistik_string */



/* =========================== log_adi_statistik ============================ */
static void log_adi_statistik (INT  akt_nkl)
/*----------------------------------------------------------------------------*/
/*  Erzeugt bei eingeschalteter generationsweiser Statistik ("statistik_pro_  */
/*  gen") nach jeder Adaption Ausgaben fuer die Notenklasse "akt_nkl"ins Log- */
/*  file zur Darstellung mit dem Gnuplot fuer den Verlauf folgender Groessen  */
/*  in einer Kopfzeile und dann pro LSV:                                      */
/*    **p  nkl  upd allPupd  Liste aller p(LSV)                               */       
/*    **xP nkl  p(LSV)  bestLevel(Params)  bestLevel(AllP)                    */
/*    **xi nkl  Level-Werte pro LSV-Param                                     */
/*    **xA nkl  Level-Werte fuer AllP                                         */
/*                                                                            */
/*  Dabei gilt:                                                               */
/*  x               : LSV-Zeichen (lsvCh)                                     */
/*  i               : Index des LSV-Parammeters                               */
/*  upd             : update_ctr                                              */
/*  allPupd         : allP_update_ctr                                         */
/*  bestLevel(Param): Die Levelnummern der besten Parameter-Level des LSVs    */
/*  bestLevel(AllP) : Die Levelnummern der besten allP-Level des LSVs         */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE  buf;
   STR_TYPE     ibuf;
   INT          ii, jj, ll;
   BOOL         nkl_valid;

   /* log_adi_statistik */
   if (statistik_pro_gen)
   { /* sonst mache mer nix */
      sprintf (buf, "**p  %d  %2d %2d  ", akt_nkl + 1, 
               update_ctr[akt_nkl], allP_update_ctr[akt_nkl]);
      for (ii = 0;  ii < LSV_ANZ;  ii++)
         if (lsv[ii].aktiv)
         { /* alle aktiven LSVs */
            sprintf (ibuf, " %5.3f", lsv[ii].lsvP[akt_nkl]);
            strcat (buf, ibuf);
         } /* alle aktiven LSVs */
      write_to_log (buf, KEEP_LOG_OPEN);

      for (ii = 0;  ii < LSV_ANZ;  ii++)
         if (lsv[ii].aktiv)
         { /* alle aktiven LSVs */
            nkl_valid = lsv[ii].nklValid[akt_nkl];
            sprintf (buf, "**%cP %d  %g ", lsv[ii].lsvCh, akt_nkl + 1, 
                     lsv[ii].lsvP[akt_nkl]);
            for (jj = 0;  jj < lsv[ii].lsvParAnz;  jj++)
            { /* alle Parameter eines LSVs */
               sprintf (ibuf, " %d", get_best_level_idx(lsv[ii].parLevelP[jj][akt_nkl],
                                                        lsv[ii].par1stActLev[jj][akt_nkl], 
                                                        nkl_valid) + 1);
               strcat (buf, ibuf);
            } /* alle Parameter eines LSVs */
            sprintf (ibuf, "  %d", get_best_level_idx(lsv[ii].allPLevelP[akt_nkl], 
                                                      lsv[ii].allP1stActLev[akt_nkl], 
                                                      nkl_valid) + 1);
            strcat (buf, ibuf);
            write_to_log (buf, KEEP_LOG_OPEN);
            for (jj = 0;  jj < lsv[ii].lsvParAnz;  jj++)
            { /* alle Parameter eines LSVs */
               sprintf (buf, "**%c%1d %d  %d  ", lsv[ii].lsvCh, jj, akt_nkl + 1, 
                        update_ctr[akt_nkl]);
               for (ll = 0;  ll < lsv[ii].parLevelAnz[0];  ll++) {
                  sprintf (ibuf, " %g", lsv[ii].parLevelP[jj][akt_nkl][ll]);
                  strcat (buf, ibuf);
               }
               write_to_log (buf, KEEP_LOG_OPEN);
            } /* alle Parameter eines LSVs */
            sprintf (buf, "**%cA %d  %d  ", lsv[ii].lsvCh, akt_nkl + 1, 
                     allP_update_ctr[akt_nkl]);
            for (ll = 0;  ll < allPLevelAnz;  ll++) {
               sprintf (ibuf, " %g", lsv[ii].allPLevelP[akt_nkl][ll]);
               strcat (buf, ibuf);
            }
            write_to_log (buf, KEEP_LOG_OPEN);
         } /* alle aktiven LSVs */
      write_to_log ("", CLOSE_LOG);                              /* Leerzeile */
   } /* sonst mache mer nix */
} /* log_adi_statistik */



/* ========================= log_adi_ges_statistik ========================== */
void   log_adi_ges_statistik (void)
{
   STRING_TYPE  buf;
   STR_TYPE     ibuf;
   DOUBLE       sum;
   INT          max_nkl, nkl, job_anz, ii, jj, ll;

   /* log_adi_ges_statistik */
   for (nkl = 0;  nkl < nklassen_anz;  nkl++)
   { /* Schleife ueber alle Notenklassen */
      for (ii = 0;  ii < LSV_ANZ;  ii++)
         if (lsv[ii].aktiv)
         { /* alle aktiven LSVs */
            job_anz = 0;
            sprintf (buf, "*%cp %d ", lsv[ii].lsvCh, nkl+1);  /* p(LSV)-Zeile */
            for (jj = 0;  jj < 10;  jj++) {
               sprintf (ibuf, " %3d", lsv[ii].statLsvPNklHisto[nkl][jj]);
               strcat (buf, ibuf);
               job_anz += lsv[ii].statLsvPNklHisto[nkl][jj];
            }
            sprintf (ibuf, "   %3d", job_anz);
            strcat (buf, ibuf);
            if (job_anz != 0)
               sprintf (ibuf, " %g", lsv[ii].statLsvPNklSum[nkl]/job_anz);
            else
               sprintf (ibuf, "  -");
            strcat (buf, ibuf);
            write_to_log (buf, KEEP_LOG_OPEN);

            for (jj = 0;  jj < lsv[ii].lsvParAnz;  jj++) /* LSV-SteuParZeilen */
            { /* alle Parameter eines LSVs */
               sum     = 0.0;
               job_anz = 0;
               sprintf (buf, "*%c%1d %d ", lsv[ii].lsvCh, jj, nkl+1);
               for (ll = 0;  ll <  lsv[ii].parLevelAnz[jj];  ll++) {
                  sprintf (ibuf, " %3d", lsv[ii].statParBestLSum[jj][nkl][ll]);
                  strcat (buf, ibuf);
                  sum     += (ll + 1) * lsv[ii].statParBestLSum[jj][nkl][ll];
                  job_anz += lsv[ii].statParBestLSum[jj][nkl][ll];
               }
               for (ll = lsv[ii].parLevelAnz[jj];  ll < LSV_LEVEL_ANZ_MAX;  ll++) 
                  strcat (buf, "    ");
               sprintf (ibuf, "   %3d", job_anz);
               strcat (buf, ibuf);
               if (job_anz != 0)
                  sprintf (ibuf, " %g", sum/job_anz);
               else
                  sprintf (ibuf, "  -");
               strcat (buf, ibuf);
               write_to_log (buf, KEEP_LOG_OPEN);
            } /* alle Parameter eines LSVs */
            sum = 0.0;                                 /* AllP-Zeile des LSVs */
            sprintf (buf, "*%cA %d ", lsv[ii].lsvCh, nkl+1);
            for (ll = 0;  ll < allPLevelAnz;  ll++) {
               sprintf (ibuf, " %3d", lsv[ii].statAllPBestLSum[nkl][ll]);
               strcat (buf, ibuf);
               sum += (ll + 1) * lsv[ii].statAllPBestLSum[nkl][ll];
            }
            sprintf (ibuf, "                   %3d", lsv[ii].statAllPJobAnz[nkl]);
            strcat (buf, ibuf);
            if (lsv[ii].statAllPJobAnz[nkl] != 0)
               sprintf (ibuf, " %g", sum/lsv[ii].statAllPJobAnz[nkl]);
            else
               sprintf (ibuf, "  -");
            strcat (buf, ibuf);
            write_to_log (buf, KEEP_LOG_OPEN);
         } /* alle aktiven LSVs */
      write_to_log ("", CLOSE_LOG);                              /* Leerzeile */

   } /* Schleife ueber alle Notenklassen */
} /* log_adi_ges_statistik */



/* ============================== set_std_nkl ================================ */
void set_std_nkl (INT  new_nkl_anz)
{
   if (new_nkl_anz == 3)
   { /* NKL 3 */
      nklassen_feld[0] =  40000.0;
      nklassen_feld[1] =  75000.0;
   } /* NKL 3 */
   else
      if (new_nkl_anz == 4)
      { /* NKL 4 */
         nklassen_feld[0] =  35000.0;
         nklassen_feld[1] =  65000.0;
         nklassen_feld[2] =  85000.0;
      } /* NKL 4 */
      else
      { /* NKL 5 */
         nklassen_feld[0] =  30000.0;
         nklassen_feld[1] =  55000.0;
         nklassen_feld[2] =  75000.0;
         nklassen_feld[3] =  90000.0;
      } /* NKL 5 */
   nklassen_anz                  = new_nkl_anz;
   nklassen_feld[nklassen_anz-1] = RNOTEN_MAX + 0.001;
} /* set_std_nkl */



/* ============================= init_adapt_di =============================== */
BOOL init_adapt_di (void)
{
   INT   ii;
   BOOL  ok = TRUE;

   /* ------------ 1-dim Datenstrukturen und Hauptfelder anlegen: ------------ */
   if (((lsv_anw_ctr        = (INT*)   malloc(NKL_MAX*isize)) == NULL) ||
       ((update_ctr         = (INT*)   malloc(NKL_MAX*isize)) == NULL) ||
       ((allP_update_ctr    = (INT*)   malloc(NKL_MAX*isize)) == NULL) ||
       ((lsvPMin            = (DOUBLE*)malloc(NKL_MAX*dsize)) == NULL) ||
       ((nklasse_valid      = (BOOL*)  malloc(NKL_MAX*bsize)) == NULL)   )
   { /* Mem out */
      err_state = fatal (EVO_ADI, 59, NO_INFO, NO_INFO, " ", NOT_ENOUGH_MEM);
      ok = FALSE;
   } /* Mem out */

   if (ok)
   { /* Felder erfolgreich neu angelegt */
      set_std_nkl (3);
   } /* Felder erfolgreich neu angelegt */
   lsv_nix_anwendung_limit = 3;  /* MaxAnz Adaptionen eines LSVs zur Abschalt.*/
   oldDistribFrac          = OLD_DISTR_FRAC;

   return (ok);
} /* init_adapt_di */

