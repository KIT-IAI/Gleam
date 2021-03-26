/*******************************************************************************
GLEAM/AE                                                                  V4.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                            Package: BEWertung
                             Module: BEWERTung
 
Package: bew                   File: bewert.c                     
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt alle Routinen zur BEW-File-I/O und zur Bewertung. Er ist 
sowohl fuer die reine algorithmische Bewertung als auch fuer die verschiedenen
Anzeigefunktionen des "simu"-Packages ausgelegt. Die meisten exportierten Vari-
ablen sind fuer diese Anzeigen vorgesehen. 

Zur algorithmischen Bewertung kann wie folgt vorgegangen werden:
  Initialisierungsphase:
    bew_init ();                        BOOLsche Modul-Initialisierungsroutine.
    restore _bew (...);                 BEW-File laden (liefert GSTATE !).
    std_simu_interface = TRUE;          Initialisierung.
    record_evo_best    = FALSE;         Initialisierung mit TRUE.
  Anwendungsphase. Pro Anwendung:
    simu_erg[ii] = ... ;                Belegung des "simu_erg"-Feldes.
    note = ak_bewerten (FALSE);         Berechnung der Endnote.

Der Modul benoetigt die beiden Packages "sys" und "fbhm". Letzteres kann durch 
einen Dummy-Modul nachgebildet werden, wobei auf eine entsprechende Fehlerbe-
handlung zu achten ist, siehe die Beschreibungen der benutzten Routinen.


Das BEW-File ist ein Textfile und hat folgenden Aufbau:

  1.Zeile:  Header
  2.Zeile:  Header
  3.Zeile:  <Anzahl d.aktiven Krits>  <Anzahl d.benutzten Krits>    INT
  (4(i-1)+4).Zeile:  bezeichnung                                    STRING[19] 
  (4(i-1)+6).Zeile:  einheit                                        STRING[11] 
  (4(i-1)+7).Zeile:  klasse, max_note, erfuellungs_wert             INT, DOUBLE,...
  (4(i-1)+8).Zeile:  bew_alg, nmin, ns1, ns2, nmax,  nmax_cont      DOUBLE,...,BOOL
  (4(i-1)+9).Zeile:           xmin, xs1, xs2, xmax,  xdrittel       DOUBLE,...

Der String "einheit" enthaelt neben dem Bezeichner (max 10 Zeichen) noch folgen-
de Informationen: 

Im Index-Bereich 0.."max_anz_bew_krit" enthaelt der String den Bezeichnernamen 
der verwendeten Mass-Einheit. Ausgegeben werden die Werte im "%G"-Format, es 
sei denn, dass es sich bei dem Bezeichner um das "%"-Zeichen handelt. In diesem 
Fall wird auf %-Ausgabe normiert: xxx.xx %. 

Im Index-Bereich "max_anz_bew_krit".."max_anz_kriterien" steuert der "einheit"-
String die 2.Ausgabe des Werts bei Bereichsueber/unterschreitung. Er hat fol-
gendes Format: 
       <flag><blank><bezeichner>. 

<flag> hat folgende Bedeutung:
       !: Keine 2.Ausgabe.
       <: 2.Ausgabe des 1000-fachen des Werts im "%G"-Format, wenn der Wert 
          kleiner als 1/1000 ist.
       >: 2.Ausgabe des 1/1000 des Werts im "%G"-Format, wenn der Wert groesser-
          gleich 1000 ist.
       %: 2.Ausgabe des Werts im "%G"-Format, wenn der Wert kleiner als 1.0 oder 
          groesser-gleich 100000 ist.

Der Boolsche Parameter "nmax_cont" regelt, ob bei steigenden Bewertungsfunktionen 
fuer x > xmax weiterhin RNOTEN_MAX abgeliefert wird oder nicht bzw. entsprechend 
bei fallender Bewertung. Dies dient der Bildung von Bewertungsfunktionen mit an- 
und absteigender Flanken durch Zusammensetzung zweier Standard-Bewertungs-
funktionen. Um eine Ueberschneidung beim "xmax"- und "xmin"-Wert zu vermeiden, 
gilt folgendes Gesamtintervall:
                        [xmin..xmax)[xmin..xmax]
                         steigender  fallender
                            Teil        Teil

Fuer die Bewertung stehen 6 Standard-Bewertungsfunktionen zur Verfuegung:
linear, exponentiel und gemischt linear-exponentiell jeweils in den Varianten
monoton steigend und monoton fallend. Fitness- (oder Noten-)werte sind aus dem 
Interval [0.0, RNOTEN_MAX]. Die Funktionen im einzelnen:


LIN_MINCR:
==========
         ^
   nend -+ - - - - - - - - - - **       y = a*x + b
         |                   ** :       
    ns2 -+ - - - - - - - -  *   :       mit:
         |                 *:   :       xstart < xs1 < xs2 < xend
         |                * :   :       
         |               *  :   :            ns2 - ns1                beispiel-
         |              *   :   :       a = -----------               haft
         |             *    :   :            xs2 - xs1                fuer das
    ns1 -+ - - - -  ***     :   :                                     mittlere
         |       ***  :     :   :           ns1 * xs2 - ns2 * x_xs1   Geraden-
 nstart -+----***-----+-----+---+--->   b = -----------------------   segment
              |       |     |   |                  xs2 - xs1
           xstart    xs1   xs2  xend    
                                        Vorgegeben werden die Stuetzpunkte:
                                        (xstart, nstart =0 ), (xs1, ns1), (xs2, ns2), 
                                        (xend, nend = RNOTEN_MAX)
                                        
LIN_MDECR:
==========
         ^
 nstart -+ - -***                       y = a*x + b
         |    :  ***                             
    ns1 -+ - -:- - -*                   mit:
         |    :     :*                  xmin < xs1 < xs2 < xmax
         |    :     : *                   
         |    :     :  *                Berechnung von a und b siehe LIN_MINCR.
         |    :     :   *               
         |    :     :    *              
    ns2 -+ - -:- - -:- - -*             Vorgegeben werden die Stuetzpunkte:
         |    :     :     :***          (xstart, nstart = RNOTEN_MAX), (xs1, ns1),
   nend -+----+-----+-----+---***--->   (xs2, ns2), (xend, nend = 0)
              |     |     |     |       
           xstart  xs1   xs2    xend    
                                        

EXP_MINCR:
==========
          ^                                   xend - x + b
    nend -+ - - - - - - - - *                 ------------ * a
          |                *:                     xend        
          |               * :            y = e
          |              *  :
          |            **   :            mit:
          |          **     :            xdrittel < xend
  nmax/3 -+ - - - ***       :
          |   ****:         :                xend * (ln (nend/3) - ln (nend))
         *****    :         :            a = --------------------------------
     0.0 -+-------+---------+--->                   xend - xdrittel
                  |         |
               xdrittel    xend              ln (nend) * (xend - xdrittel)
                                         b = -----------------------------
                                                ln (nend/3) - ln (nend)

                                          Vorgegeben werden: xend, xdrittel

EXP_MDECR:
==========
          ^                                      x - xstart + b
  nstart -+ - -*                               - -------------- * a
          |    :*                                    xstart
          |    : *                        y = e
          |    :  *
          |    :   **                     mit:
          |    :     **                   xmin < xdrittel
  nmax/3 -+ - -:- - - -***
          |    :       :  ****                xstart * (ln (nstart) - ln (nstart/3))
          |    :       :      *****       a = --------------------------------------
     0.0 -+----+-------+------------->                  xdrittel - xstart
               |       |
            xstart   xdrittel                 ln (nstart) * (xdrittel - xstart)
                                          b = ---------------------------------
                                                 ln (nstart/3) - ln (nstart)

                                          Vorgegeben werden: xstart, xdrittel



MIXED_MINCR:
============                                         xs2 - x + b
          ^                                          ----------- * a
    nend -+ - - - - - - - - - - - - - - - **             xs2
          |                             ** :    y = e 
          |                           **   :
     ns2 -+ - - - - - - - - - - - - -*     :
          |                         *:     :    mit:
          |                        * :     :    xstart < xs1 < xdrittel < xs2 < xend
          |                      **  :     :       ns2 > nend/3
          |                    **    :     :
  nend/3 -+                 ***      :     :        xs2 * (ln(nend/3) - ln(ns2)
          |             ****  :      :     :    a = ---------------------------
     ns1 -+ - - - -*****      :      :     :              xs2 - xdrittel
          |      **:          :      :     :
          |    **  :          :      :     :        ln(ns2) * (xs2 - xdrittel)
  nstart -+--**----+----------+------+-----+->  b = --------------------------
             |     |          |      |     |            ln(nend/3) - ln(ns2)
          xstart  xs1     xdrittel  xs2   xend
                                                Vorgegeben werden:
             \     /\                /\    /    xstart, xs1, xdrittel, xs2, xend u. ns2.
              --v--  --------v-------  --v-     ns1 wird aus diesen Daten berechnet.
              linear    exponentiel    linear   


MIXED_MDECR:
============                                           x - xs1 + b
          ^                                          - ----------- * a
  nstart -+ - **                                           xs1
          |   : **                              y = e
          |   :   **
     ns1 -+ - : - - *
          |   :     :*                          mit
          |   :     : *                         xstart < xs1 < xdrittel < xs2 < xend
          |   :     :  **                          ns1 > nstart/3
          |   :     :    **                     
nstart/3 -+   :     :      ***                      xs1 * (ln(ns1) - ln(nstart/3))
          |   :     :      :  ****              a = ----------------------------
     ns2 -+ - : - - : - - -:- - - *****                   xdrittel - xs1
          |   :     :      :          :**
          |   :     :      :          :  **         ln(ns1) * (xdrittel - xs1)
    nend -+---+-----+------+----------+----**-> b = --------------------------
              |     |      |          |   |             ln(nstart/3) - ln(ns2)
           xstart  xs1  xdrittel     xs2 xend
              \     /\                /\    /   Vorgegeben werden:
               --v--  --------v-------  --v-    xstart, xs1, xdrittel, xs2, xend u. ns1.
               linear    exponentiel    linear  ns2 wird aus diesen Daten berechnet.
                                              


Die Daten der Kriterien werden im Feld "kriterien" gespeichert. Es ist folgen-
dermassen organisiert:

  +------+
  |      |
  | Be-  |
  | wer- | < akt_anz_krit
  | tung | < akt_anz_used_krit  (immer >= akt_anz_krit)
  |      |
  |      |
  |      | 
  +------+ < max_anz_bew_krit   (initial: MIN_ANZ_BEW_KRIT)
  |      | 
  |Straf-|
  |funk- | < krit_og            ( == max_anz_bew_krit + akt_anz_krit)
  |tionen| < used_krit_og       ( == max_anz_bew_krit + akt_anz_used_krit)
  |      |
  |      |
  |      | 
  +------+ < max_anz_kriterien  ( == 2 * max_anz_bew_krit)

Initial wird Platz fuer AK_ERG_WERTE_MAX (derzeit 16) Bewertungsfunktionen und 
den dazugehoerigen Straffunktionen angelegt. Innerhalb dieser Grenzen koennen
1. interaktiv neue Kriterien definiert werden und 2. die Ergebniswerte in den 
AKs gespeichert werden. Die Vorgabe von mehr Kriterien durch das BEW-File ist 
zulaessig.

Straffunktionen werden aktiviert, wenn der x-Wert nicht im bewertbaren Bereich 
liegt und die Fitness bei Standardbewertungsfunktionen null ist. Dies ist der
Fall, wenn bei 
   steigenden linearen oder gemischt-linearen Funktionen gilt:  Xsim < Xmin
   fallenden  linearen oder gemischt-linearen Funktionen gilt:  Xmax < Xsim
Bei rein exponentiellen Funktionen (EXP_MINCR und EXP_MDECR) ist dies hingegen
nie der Fall, sodass Straffunktionen an dieser Stelle sinnlos sind.


Die Routinen im einzelnen:

DOUBLE ak_bewerten (BOOL  interaktiv);
        Bewertet die AK ausgehend von den "simu_erg"-Werten und liefert die
        Daten in folgenden Variablen ab: "ungew_note", "gew_note", "straf_fkt"
        und "krit_gueltig". In letzterem sind in der unteren Haelfte diejenigen
        Kriterien vermerkt, die zur Notenbildung beigetragen haben und - sofern 
        "interaktiv" gesetzt ist - in der oberen alle angewandten Straffunktio-
        nen. Die Gesamtnote wird als Funktionsergebnis abgeliefert. Bei auftre-
        tenden Fehlern in "calc_ungew_note" wird KEINE_NOTE abgeliefert und 
        obige Zwischenwerte sind (z.T.) undefiniert.
        Bei zusammengesetzten Funktionen wird ein Kriterium zur Pruefung der 
        Klassenerfuellung nur herangezogen, wenn es eine Note > 0 hat. Das be-
        wirkt, dass bei X-Werten, die ausserhalb des Bewertungsintervalls lie-
        gen (falls eines definiert ist), das Kriterium faelschlicherweise als 
        erfuellt angesehen wird. Der Grund liegt darin, dass die Datenstrukturen
        keine Beziehung zwischen den Bewertungsfunktionen zusammengesetzter Kri-
        terien abbilden. Da zusammengesetzte Funktionen eher selten sind und das
        Problem durch geeignete Wahl des Bewertungsintervalls umgangen werden
        kann, bleibt es zunaechst dabei.

DOUBLE calc_phaen_dist (DOUBLE *simu_erg1,
                        DOUBLE *simu_erg2);
        Berechnet ausgehend von den beiden "simu_erg"-Feldern die phaenotypische
        Distanz basierend auf den gewichteten Noten und liefert sie als Ergebnis
        ab. 
        Die Routine erwartet in ihren Parametern die Simu-Ergebnisdaten. Diese 
        werden beim Aufruf aus der AK genommen, was nur solange geht, wie mit
        nicht mehr als AK_ERG_WERTE_MAX Kriterien bewertet wird.

DOUBLE calc_ungew_note (INT     indx,
                        DOUBLE  xwert);
        Berechnet entsprechend den in den "kriterien" hinterlegten Daten fuer 
        das durch "indx" bezeichnete Kriterium ausgehend vom "xwert" die unge-
        wichtete Note als Funktionsergebnis. Sollten, was sehr unwahrscheinlich 
        ist, Fehler auftreten, liefert die Routine "KEINE_NOTE" als Funktions-
        ergebnis ab und loest einen fatalen Fehler aus, der aber nicht als 
        Return-Status ausgegeben wird.
        Bei noch nicht definiertem Algorithmus (bew_alg == NIX_IDX) liefert die
        Routine KEINE_NOTE ab und erstattet Meldung.

GSTATE save_bew (char *file_name);
        Die Daten der Bewertung werden in das File "file_name" gerettet. Falls
        das File nicht geoeffnet werden konnte liefert die Routine GLEAM_ERROR.

GSTATE restore_bew (char *file_name, 
                    BOOL *bad_data);
        Die Bewertungsdaten werdeb aus dem File "file_name" restauriert. Bei 
        einem I/O-Fehler erfolgt Fehlermeldung (GLEAM_ERROR). Die Daten werden 
        mit Hilfe "gewichten" und "prepare_eval" geprueft und aufbereitet. Der 
        Status von "prepare_eval" wird abgeliefert. "bad_data" ist TRUE, wenn 
        bei fehlerhaftem Returnstatus die internen Bewertungs-Daten bereits 
        veraendert wurden.
 
GSTATE prepare_eval (void);
        Die Routine prueft die Zulaessigkeit der vorgegebenen Werte entsprechend
        dem gewaehlten Bewertungsalgorithmus (siehe obige Angaben). Bei Unzu-
        laessigkeit erfolgt eine Fehlermeldung (GLEAM_ERROR) u. eine Abbruch der
        Routine. Andernfalls werden die a- u. b-Werte aller Kriterien berechnet.
        Bei noch nicht definiertem Algorithmus (bew_alg == NIX_IDX) geschieht 
        nichts.

void   gewichten (void);
        Stellt die Notengewichte ("noten_gewicht" in "kriterien") ein und ermit-
        telt den groessten Klassen-Index in der globalen Variable "max_klasse".

void   reset_krit_gueltig (void);
        Setzt alle Eintraege in "krit_gueltig" auf FALSE.

void   resetSimValOutOfEvalRange (void);
        Setzt alle Eintraege in "simValOutOfEvalRange" auf FALSE.

BOOL   bew_init (void);
        Legt die modul-internen Datenstrukturen an und liefert TRUE, wenn dafuer
        genuegend Speicherplatz zur Verfuegung steht, sonst FALSE.

--------------------------------------------------------------------------------
basiert auf den C-Units: aufg_def, aufgabe.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
1. Neue Datum "nmax_cont" in den KRITERIUMS_TYP-Daten: Regelt, ob RNOTEN_MAX bei
   Ueber- bzw. Unterschreitung von "xmax" bzw. "xmin" beibehalten wird oder 
   nicht. 
2. Erweiterung des Konzepts auf beliebig viele Kriterien und Dynamisierung der
   Datenstruktur. Dabei erfolgt nur bei Handlungsmodellen mit maximal AK_ERG_
   WERTE_MAX Ergebniswerten deren Speicherung in der AK. Sonst muss IMMER 
   simuliert werden.
3. Verbesserung der Plotausgabe bei zusammengesetzten Funktionen.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 2.0.0:
Bereinigung des Bewertungs-Packages mit dem Ziel, alle Bewertungsroutinen darin
zusammenzufassen und einen Kern bereit zustellen, der die reine Algorithmik der 
Bewertung und das File-I/O abdeckt. Massnahmen:
 1. Transfer von "compress_crit_name" und der GNU-Plott-Routinen "prep_gnu_plot"
    und "gnu_hard_copy" nach Modul "bew_gsw".
 2. Uebernahme der Routine "ak_bewerten" vom Package "simu".
 3. In "restore_bew" alle Kriterien initial deaktiviert, wenn keine neue Daten-
    struktur angelegt wird.
 4. Neue Routine "calc_phaen_dist" fuer Nachoptimierung mit LSV.     
 5. "save_bew" schreibt im Format "%.16e" statt "%12.6e".            WJ 30.10.07
 6. CEC: "prepare_eval" wird Package-Export.                         WJ 07.11.07
 7. Korrektur einiger Fehlermeldungsaufrufe wg. Umstellung in fbhm.  WJ 13.08.08
 8. Aus Kompatibiltaetsgruenden wird OLD_NIX_IDX beim Einlesen in 
    NIX_IDX umgewandelt.                                             WJ 03.09.08
 9. Korrektur der Doku von MIXED_MDECR: 
    aus "xs1 - xdrittel" wird "xdrittel - xs1".                      WJ 15.07.10
10. Korrektur im Dokuteil von "save_bew".                            WJ 26.06.12
11. "calc_a", "calc_b" und "x_test" sowie einige lokale Konstanten 
    aus "berwert.c" werden bei gesetztem GUI_MODE zu Package-Export 
    und sind ansonsten nur package-lokal.                            WJ 27.07.15
12. Renovierung: Punkt 11 wird rueckgaengig gemmacht. Aus EIN_DRITTEL 
    wird NOTEN_DRITTEL. Ueberarbeitung der Dokumentation der Bewertungs-
    funktion und deren Konstanten, siehe "bew.h". Aus "prop" wird "steigend". 
    Korrektur des fgets-Aufrufs. Streichung der xtics und von "kurz_name" 
    in KRITERIUMS_TYP und in den Datenzeilen im bew-File.            WJ 07.04.16
13. Neu: "critLine2Index" fuer die GUI-Anbindung. Einige bisher 
    package-lokale Variable werden Package-Export. Neue Variable: 
    "simValOutOfEvalRange", "int_simValOutOfEvalRange" und 
    "best_simValOutOfEvalRange". Ueberarbeitung der Behandlung von 
    Straffunktionen in "ak_bewerten". Verbesserung der Doku und der
    Behandlung von unzulaessigen Funktionstypen.                     WJ 18.08.16
14. Anpassung an ueberarbeitete Bewertung: "ak_bewerten", 
    "calc_ungew_note" und "prepare_eval".                            WJ 22.08.16
15. save_bew(): Extension through English documentation in the file. WJ 27.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 27.07.2020
================================================================================ */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <ctype.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "chaindef.h"
#include "bewInt.h"
#include "bew.h"

#include BEW_TEXTS


#ifdef GLO_CHECKS
#define CHECKS
#endif



/* ========================== Exportierte Variable: ========================= */
KRITERIUMS_TYP *kriterien;        /* Bewertungskriterien.                     */
DOUBLE         *simu_erg,         /* Aufbereitete Daten eines SimuLaufs       */
               *int_simu_erg,     /* Aufb.Daten d.letzten interaktiv.SimuLaufs*/
               *best_simu_erg,    /* Aufb.Daten d.BestAK seit "reset_simu_erg"*/
               *ungew_note,       /* Ungew.Noten aller Krits[max_anz_bew_krit]*/
               *int_ungew_note,   /* Erg.d.letzt.interakt.SimuLaufs           */
               *best_ug_note,     /* UngewNote d.BestAK seit "reset_simu_erg" */
               *gew_note,         /* Gewichtete Noten aller Kriterien         */
               *int_gew_note,     /* GewNote d.letzten interaktiven SimuLaufs */
               *best_g_note,      /* GewNote der BestAK seit "reset_simu_erg" */
               *straf_fkt,        /* angewandte Straffaktoren                 */
               *int_straf_fkt,    /* Erg. des letzten interaktiven SimuLaufs  */
               *best_str_fkt,     /* StrafFkts. d.BestAK seit "reset_simu_erg"*/
                last_x_start,     /* Start des X-Bereichs der GnuPlot-Anzeige */
                last_delta,       /* Zwischenschritte der GnuPlot-Anzeige     */
                gnu_x_max;        /* Ende des X-Bereichs der GnuPlot-Anzeige  */
INT             max_klasse,       /* Maximaler Klassenwert                    */
                akt_anz_krit,     /* maximale Anzahl der aktiven Kriterien    */
                max_anz_bew_krit, /* maximale Kriterienanzahl im Feld         */
                krit_og,          /* OG d.aktiven Krit. im "kriterien"-Feld   */
                max_anz_kriterien,/* MaxAnz der Kriterien u. StrafFunktionen  */
                last_krit = NIX_IDX, /* Zuletzt bearbeitetes Kriterium        */
                                  /* Zum Testen der Bewertungsfunktion in der */
                                  /* TEXT_MODE-Variante als Zahlenwerte und   */
                                  /* bei beiden Varianten mit dem GnuPlot.    */
                x_anz;            /* Anzahl d.Zwischenschritte d.GnuPlot-Anzg.*/
BOOL           *krit_gueltig,     /* Mark.gueltige Krits. [max_anz_kriterien] */
               *int_krit_gueltig, /* Ergebnis d.letzten interaktiven SimuLaufs*/
               *best_krit_g,      /* GueltKrit.d.BestAK seit "reset_simu_erg" */
               *simValOutOfEvalRange,   /* SimuWert im Bereich der Bestrafung */
               *int_simValOutOfEvalRange, /* sim... der letzten interakt. Simu*/
               *best_simValOutOfEvalRange,/* der bestAK  seit "reset_simu_erg"*/
                record_evo_best,        /* TRUE: SimuDaten von EvoBest sammeln*/
                std_simu_interface,     /* TRUE, wenn Simu-Standard-Schnittst.*/
                bew_aend_sim = FALSE;   /* TRUE: Neubew.wg.Bew-Änderung notw. */
#ifdef GUI_MODE
  INT         *critLine2Index;    /* Krit-Indices in d.Darstellungsreihenfolge*/
#endif /* GUI_MODE */


/* ------------- Im GUI_MODE Export, sonst nur package-global: -------------- */
INT       akt_anz_used_krit,  /* aktuelle Anzahl der genutzten Kriterien      */
          used_krit_og;       /* OG d.benutzten Krit in "kriterien"           */



/* ======================== Package-globale Variable: ======================= */
char     *bez_len_str,        /* "xxx..xxx"-Schablone f.Krit-Bezeichner       */
         *unit_len_str;       /* "xxx..xxx"-Schablone f.Krit-Einheitsbez.     */
BOOL      baddata;            /* bad_data fuer TCL-Version                    */



/* =========================== Lokale Konstanten: =========================== */
#define OLD_NIX_IDX    99                        /* Abwaertskompatibilitaet   */
#define LN_DRITTEL     10.41431318               /* ln (RNOTEN_MAX / 3)       */
#define LN_NOTEN_MAX   11.51292547               /* ln (RNOTEN_MAX)           */
#define LN_DIFF        (-1.098612289)            /* LN_DRITTEL - LN_NOTEN_MAX */



/* ============================ Lokale Variable: ============================ */
size_t  bigBArrayLen,         /* Bool-Feldlaenge des ganzen Feldes in Bytes   */
        smallBArrayLen;       /* Bool-Feldlaenge des halben Feldes in Bytes   */




/* ============================ Lokale Routinen ============================= */ 
static BOOL   alloc_bewkrit_data (INT  anz);

static DOUBLE calc_a (DOUBLE  n_ug,
                      DOUBLE  n_og,
                      DOUBLE  x_ug,
                      DOUBLE  x_og);

static DOUBLE calc_b (DOUBLE  n_ug,
                      DOUBLE  n_og,
                      DOUBLE  x_ug,
                      DOUBLE  x_og);

static BOOL   x_test (INT     index);



/* ============================== ak_bewerten =============================== */
DOUBLE ak_bewerten (BOOL  interaktiv)
{
   DOUBLE  straf_fakt, ges_note;
   INT     diese_klasse, 
           max_gueltige_klasse,
           krit_index,
           krit_alg,
           ii;
   BOOL    keine_erfuellung,
           bestrafen;

   /* ak_bewerten */
   reset_krit_gueltig ();                                  /* Initialisierung */
   resetSimValOutOfEvalRange ();                           /* Initialisierung */
   for (ii = 0;  ii < akt_anz_krit;  ii++)
   {
      if (std_simu_interface)
         if ((ungew_note[ii] = calc_ungew_note (ii, simu_erg[ii])) == KEINE_NOTE)
            return (KEINE_NOTE); /* Abbruch, da "calc_ungew_note" Fehler liefert */
      gew_note [ii]    = ungew_note[ii] * kriterien[ii].noten_gewicht;
      krit_gueltig[ii] = kriterien[ii].klasse == 1;        /* Initialisierung */
   }
   
   /* ----------------- Ermittlung der gueltigen Kriterien: ----------------- */
   keine_erfuellung    = FALSE;
   diese_klasse        = 1;
   max_gueltige_klasse = 1;
   do
   { /* Gueltige Kriterien suchen */
      for (ii = 0;  ii < akt_anz_krit;  ii++)
      { /* Pruefen, ob alle Kriterien der Klasse "diese_klasse" erfuellt sind */
         if (kriterien[ii].klasse == diese_klasse)   /* Krit.der akt.Klasse ? */
            if ((gew_note[ii] < kriterien[ii].erfuellungs_note) && 
                (kriterien[ii].nmax_cont || (gew_note[ii] > 0.0)))
               keine_erfuellung = TRUE;
      } /* Pruefen, ob alle Kriterien der Klasse "diese_klasse" erfuellt sind */
      if (!keine_erfuellung)
      { /* Erfuellung: Naechst hoehere Klasse "gueltig" markieren */
         max_gueltige_klasse = diese_klasse + 1;
         for (ii = 0;  ii < akt_anz_krit;  ii++)
            if (kriterien[ii].klasse == max_gueltige_klasse)
               krit_gueltig[ii] = TRUE;
      } /* Erfuellung: Naechst hoehere Klasse "gueltig" markieren */
      diese_klasse++;
   } /* Gueltige Kriterien suchen */
   while (!(keine_erfuellung || (diese_klasse >= max_klasse)));
  
   /* ------- Gewichtete gueltige Noten zur Endnote zusammenzaehlen: -------- */
   ges_note = 0.0;
   for (ii = 0;  ii < akt_anz_krit;  ii++)
      if (krit_gueltig[ii])
         ges_note += gew_note[ii];
  
   /* --------- Straffunktion testen und gegebenenfalls ausfuehren: --------- */
   for (ii = max_anz_bew_krit;  ii < krit_og;  ii++)
   { /* Alle Straffunktionen pruefen und ggf. anwenden */
      if (kriterien[ii].klasse != KRITERIUM_AUS)
      { /* fuer definierte Straffunktionen */
         krit_index = ii - max_anz_bew_krit;
         krit_alg   = kriterien[krit_index].bew_alg;
	 switch (krit_alg) {
   	    case LIN_MINCR:
	    case MIXED_MINCR:
	       simValOutOfEvalRange[krit_index] = simu_erg[krit_index] < kriterien[krit_index].xstart;
	       break; /*  /* LIN or MIXED increasing */

	    case LIN_MDECR:
	    case MIXED_MDECR:
	       simValOutOfEvalRange[krit_index] = simu_erg[krit_index] > kriterien[krit_index].xend;	    
	       break; /*  /* LIN or MIXED decreasing */

	    /* Bei EXP_M* wird immer auf FALSE gesetzt, was aber der initiale Wert ist. */
	 } /* switch */
	 if (simValOutOfEvalRange[krit_index])
	 { /* SimuWert im Strafbereich */
            straf_fakt = calc_ungew_note (ii, simu_erg[krit_index]) / RNOTEN_MAX;
	    if (kriterien[ii].klasse <= max_gueltige_klasse)   /* Prio erfuellt ? */
	       ges_note = straf_fakt * ges_note;        /* Straffunktion anwenden */
            if (record_evo_best || interaktiv)
            { /* Daten fuer Anzeige merken: Straffaktor und Prio-erfuellt */
               straf_fkt[krit_index] = straf_fakt; /* Straffaktor merken          */
               krit_gueltig[ii]      = kriterien[ii].klasse <= max_gueltige_klasse;
            } /* Daten fuer Anzeige merken: Straffaktor und Prio-erfuellt */
         } /* SimuWert im Strafbereich */
      } /* fuer definierte Straffunktion */
   } /* Alle Straffunktionen pruefen und ggf. anwenden */
   return (ges_note);
} /* ak_bewerten */



/* ============================= calc_phaen_dist ============================ */
DOUBLE calc_phaen_dist (DOUBLE *simu_erg1,
                        DOUBLE *simu_erg2)
{
   DOUBLE  ungew_note1, ungew_note2,
           phaen_dist;
   INT     ii;

   /* calc_phaen_dist */
   phaen_dist = 0.0;
   for (ii = 0;  ii < akt_anz_krit;  ii++)
   { /* Berechnung des phaenotypischen Abstands */
      if ((ungew_note1 = calc_ungew_note (ii,simu_erg1[ii])) == KEINE_NOTE)
         return (1.0);        /* Abbruch, da "calc_ungew_note" Fehler liefert */
      if ((ungew_note2 = calc_ungew_note (ii,simu_erg2[ii])) == KEINE_NOTE)
         return (1.0);        /* Abbruch, da "calc_ungew_note" Fehler liefert */
      phaen_dist += fabs (ungew_note1 - ungew_note2) / RNOTEN_MAX;
   } /* Berechnung des phaenotypischen Abstands */
   phaen_dist = phaen_dist / akt_anz_krit;
   return (phaen_dist);
} /* calc_phaen_dist */



/* ============================= calc_ungew_note ============================ */
DOUBLE calc_ungew_note (INT     indx,
                        DOUBLE  xwert)
{
   DOUBLE  ungew_note;
   BOOL    steigend;
   
   /* calc_ungew_note */
#ifdef CHECKS
   if (indx >= max_anz_kriterien)
   { /* Mist */
      fatal (BEW_BEWERTUNG, 1, indx+1, NO_INFO, "", BAD_KRIT_INDX);
      return (KEINE_NOTE);
   } /* Mist */
#endif /* CHECKS */
   switch (kriterien[indx].bew_alg)
   {
      case LIN_MINCR:
         if (kriterien[indx].xs1 <= xwert)
         { /* xs1 <= x */
            if (xwert <= kriterien[indx].xs2)            /*  xs1 <= x <= xs2: */
               ungew_note = kriterien[indx].a2 * xwert + kriterien[indx].b2;
            else
            { /* xs2 < x */
               if (kriterien[indx].xend <= xwert)
		  ungew_note = RNOTEN_MAX;                       /* xend <= x */
               else                                        /* xs2 < x < xend: */
                  ungew_note = kriterien[indx].a3 * xwert + kriterien[indx].b3;
            } /* xs2 < x */
         } /* xs1 <= x */
         else
         { /* x < xs1 */
            if (xwert <= kriterien[indx].xstart) 
	       ungew_note = 0.0;                                /* x < xstart */
            else                                         /* xstart < x < xs1: */
               ungew_note = kriterien[indx].a1 * xwert + kriterien[indx].b1;
         } /* x < xs1 */
         break; /* LIN_MINCR */

      case LIN_MDECR:
         if (kriterien[indx].xs1 <= xwert)
         { /* xs1 <= x */
            if (xwert <= kriterien[indx].xs2)             /* xs1 <= x <= xs2: */
               ungew_note = kriterien[indx].a2 * xwert + kriterien[indx].b2;
            else
            { /* xs2 < x */
               if (kriterien[indx].xend <= xwert)
		  ungew_note = 0.0;                              /* xend <= x */
               else                                        /* xs2 < x < xend: */
                  ungew_note = kriterien[indx].a3 * xwert + kriterien[indx].b3;
            } /* xs2 < x */
         } /* xs1 <= x */
         else
         { /* x < xs1 */
            if (xwert <= kriterien[indx].xstart)
	       ungew_note = RNOTEN_MAX;                         /* x < xstart */
            else                                         /* xstart < x < xs1: */
               ungew_note = kriterien[indx].a1 * xwert + kriterien[indx].b1;
         } /* x < xs1 */
         break; /* LIN_MDECR */

      case EXP_MINCR:
         if (kriterien[indx].xend <= xwert)
            ungew_note = RNOTEN_MAX;
         else
         { /* x < xend */
            ungew_note = exp((kriterien[indx].xend - xwert + kriterien[indx].b1) *
                             kriterien[indx].a1 / kriterien[indx].xend);
         } /* x < xend */
         break; /* EXP_MINCR */
         
      case EXP_MDECR:
         if (xwert <= kriterien[indx].xstart)
            ungew_note = RNOTEN_MAX;
         else
         { /* xstart < x */
            ungew_note = exp(-((xwert - kriterien[indx].xstart + kriterien[indx].b1) *
                               kriterien[indx].a1 / kriterien[indx].xstart));
         } /* xstart < x */
         break; /* EXP_MDECR */
         
      case MIXED_MINCR:
         if (kriterien[indx].xs1 <= xwert)
         { /* xs1 <= x */
            if (xwert <= kriterien[indx].xs2)            /*  xs1 <= x <= xs2: */
               ungew_note = exp ((kriterien[indx].b2 - xwert) *
                                 kriterien[indx].a2 / kriterien[indx].xs2);
            else
            { /* xs2 < x */
               if (kriterien[indx].xend <= xwert)
                  ungew_note = RNOTEN_MAX;                       /* xend <= x */
               else                                        /* xs2 < x < xend: */
                  ungew_note = kriterien[indx].a3 * xwert + kriterien[indx].b3;
            } /* xs2 < x */
         } /* xs1 <= x */
         else
         { /* x < xs1 */
            if (xwert <= kriterien[indx].xstart)
               ungew_note = 0.0;                                /* x < xstart */
            else                                         /* xstart < x < xs1: */
               ungew_note = kriterien[indx].a1 * xwert + kriterien[indx].b1;
         } /* x < xs1 */
         break; /* MIXED_MINCR */
         
      case MIXED_MDECR:
         if (kriterien[indx].xs1 <= xwert)
         { /* xs1 <= x */
            if (xwert <= kriterien[indx].xs2)            /*  xs1 <= x <= xs2: */
               ungew_note = exp (-((xwert + kriterien[indx].b2) *
                                   kriterien[indx].a2 / kriterien[indx].xs1));
            else
            { /* xs2 < x */
               if (kriterien[indx].xend <= xwert)
                  ungew_note = 0.0;                              /* xend <= x */
               else                                        /* xs2 < x < xend: */
                  ungew_note = kriterien[indx].a3 * xwert + kriterien[indx].b3;
            } /* xs2 < x */
         } /* xs1 <= x */
         else
         { /* x < xs1 */
            if (xwert <= kriterien[indx].xstart)
               ungew_note = RNOTEN_MAX;                         /* x < xstart */
            else                                         /* xstart < x < xs1: */
               ungew_note = kriterien[indx].a1 * xwert + kriterien[indx].b1;
         } /* x < xs1 */
         break; /* MIXED_MDECR */

      case NIX_IDX:
         ungew_note = KEINE_NOTE;
         meldung (BEW_BEWERTUNG, NO_INFO, NO_INFO, "", UNDEF_BEW_STRF_FKT);
         break; /* noch nicht definierte Funktion */
         
      default:
	 if (indx < max_anz_bew_krit)
	    fatal (BEW_BEWERTUNG, 2, indx+1, kriterien[indx].bew_alg, 
		   BEW_FKT_TXT, BAD_EVAL_FKT2);
	 else
	    fatal (BEW_BEWERTUNG, 2, indx - max_anz_bew_krit + 1, 
		   kriterien[indx].bew_alg, STR_FKT_TXT, BAD_EVAL_FKT2);
         ungew_note = KEINE_NOTE;
   } /* switch */

   if (!kriterien[indx].nmax_cont)
   { /* Note = 0.0, wenn ausserhalb des X-Bereichs */
      steigend = (kriterien[indx].bew_alg == LIN_MINCR) ||
	         (kriterien[indx].bew_alg == EXP_MINCR) ||
          	 (kriterien[indx].bew_alg == MIXED_MINCR);
      if ((ungew_note != KEINE_NOTE) && 
          (( steigend && (kriterien[indx].xend   <= xwert)) || 
           (!steigend && (kriterien[indx].xstart >  xwert))))
         ungew_note = 0.0;
   } /* Note = 0.0, wenn ausserhalb des X-Bereichs */
   return (ungew_note);
} /* calc_ungew_note */ 



/* ================================ save_bew ================================ */
GSTATE save_bew (char *file_name)
{
   INT          idx, ii;
   STRING_TYPE  buf, tag_buf, datums_buf, zeit_buf;
   FILE        *bew_file;
   
   /* save_bew */
   if (open_file (&bew_file, file_name, "wt"))
   { /* File offen */
      get_time_stamp (buf, datums_buf, tag_buf, zeit_buf);
      fprintf (bew_file, BEW_FILE_HDR_1, buf);
      fprintf (bew_file, "===================================================="
               "=======================\n");
      fprintf (bew_file, BEW_FILE_HDR_2, akt_anz_krit, akt_anz_used_krit, max_anz_bew_krit);
      idx = 0;
      for (ii = 0;  ii < (2 * akt_anz_used_krit);  ii++)
      { /* Retten */
         fprintf (bew_file, "%s\n", kriterien[idx].bezeichnung);
         fprintf (bew_file, "%s\n", kriterien[idx].einheit);
         fprintf (bew_file, "%2d  %.12e  %.12e\n",
                  kriterien[idx].klasse, kriterien[idx].max_note,
                  kriterien[idx].erfuellungs_wert);
         fprintf (bew_file, "%1d  %.12e %.12e %.12e %.12e   %d\n",
                  kriterien[idx].bew_alg,
                  kriterien[idx].nstart, kriterien[idx].ns1,
                  kriterien[idx].ns2,    kriterien[idx].nend,
                  kriterien[idx].nmax_cont);
         fprintf (bew_file,  "   %.12e %.12e %.12e %.12e   %.12e\n",
                  kriterien[idx].xstart, kriterien[idx].xs1,
                  kriterien[idx].xs2,    kriterien[idx].xend,
                  kriterien[idx].xdrittel);
         if ((ii + 1) == akt_anz_used_krit)
         { /* Ausgabe der Straffunktionen */
            idx = max_anz_bew_krit;
            fprintf (bew_file, BEW_FILE_STRFF);
         } /* Ausgabe der Straffunktionen */
         else
            idx++;
      } /* Retten */
      fprintf (bew_file,
               "=============== Do not remove this line! =================\n");
      fprintf (bew_file, BEW_FILE_END_1);
      fprintf (bew_file, BEW_FILE_END_2, KRIT_BEZ_LEN);
      fprintf (bew_file, BEW_FILE_END_3);
      fprintf (bew_file, BEW_FILE_END_4);
      fprintf (bew_file, BEW_FILE_END_5);
      fprintf (bew_file, BEW_FILE_END_6);
      fprintf (bew_file, BEW_FILE_END_7);
      fprintf (bew_file, BEW_FILE_END_8);
      fprintf (bew_file, BEW_FILE_END_9);
      fprintf (bew_file, BEW_FILE_END10);
      fprintf (bew_file, BEW_FILE_END11);
      fprintf (bew_file, BEW_FILE_END12);
      fclose (bew_file);
      return (GLEAM_OK);
   } /* File offen */
   else
      return (GLEAM_ERROR);
} /* save_bew */ 



/* ============================== restore_bew =============================== */
GSTATE restore_bew (char *file_name,
                    BOOL *bad_data)
{
   INT          idx, ii, tmp1, tmp2, io_res;
   STRING_TYPE  buffer;
   GSTATE       ret_state;
   FILE        *bew_file;
   char        *ch_tmp;
   
   *bad_data = FALSE;
   if (!open_file (&bew_file, file_name, "rt"))
      return (GLEAM_ERROR);
   ch_tmp = fgets (buffer, MAX_STRING+1, bew_file);            /* Skip Header */
   ch_tmp = fgets (buffer, MAX_STRING+1, bew_file);            /* Skip Header */
   io_res = fscanf (bew_file, "%d %d", &tmp1, &tmp2);
   ch_tmp = fgets (buffer, MAX_STRING+1, bew_file);      /* Skip rest of line */
   if (io_res != 2)
      return (fehler (BEW_BEWERTUNG, 3, io_res, 2, "", BEW_IO_ERROR));
   if (tmp1 > tmp2)
      return (fehler (BEW_BEWERTUNG, 4, tmp1, tmp2, "",
                      MORE_ACTIVE_THAN_USED));
   if (tmp2 > max_anz_bew_krit)
   {
      if (!alloc_bewkrit_data (tmp2 * 2))
         return (GLEAM_FATAL); /* Fehlermeldung erledigt "alloc_bewkrit_data" */
   }
   else
      for (ii = 0;  ii < max_anz_bew_krit;  ii++)
      { /* Initialisierungen */
         kriterien[ii].klasse = KRITERIUM_AUS;
      } /* Initialisierungen */
   *bad_data         = TRUE;
   akt_anz_krit      = tmp1;
   akt_anz_used_krit = tmp2;
   krit_og           = max_anz_bew_krit + akt_anz_krit;
   used_krit_og      = max_anz_bew_krit + akt_anz_used_krit;
   idx               = 0;
   for (ii = 0;  ii < (akt_anz_used_krit * 2);  ii++)
   { /* Einlesen */
      io_res = fscanf (bew_file, "%[^\n]s", kriterien[idx].bezeichnung);
      ch_tmp = fgets (buffer, MAX_STRING+1, bew_file);   /* Skip rest of line */
      if (io_res != 1)
        return (fehler (BEW_BEWERTUNG, 1+((ii+1)*100), io_res, 1, "", 
                        BEW_IO_ERROR));
      kriterien[idx].bezeichnung[KRIT_BEZ_LEN+1] = '\0';
      io_res = fscanf (bew_file, "%[^\n]s", kriterien[idx].einheit);
      ch_tmp = fgets (buffer, MAX_STRING+1, bew_file);   /* Skip rest of line */
      if (io_res != 1)
         return (fehler (BEW_BEWERTUNG, 3+((ii+1)*100), io_res, 1, "", 
                         BEW_IO_ERROR));
      kriterien[idx].einheit[ENAME_LEN] = '\0';
      io_res = fscanf (bew_file, "%d%lf%lf", &(kriterien[idx].klasse),
                       &(kriterien[idx].max_note), 
                       &(kriterien[idx].erfuellungs_wert));
      ch_tmp = fgets (buffer,  MAX_STRING+1, bew_file);  /* Skip rest of line */
      if (io_res != 3)
         return (fehler (BEW_BEWERTUNG, 4+((ii+1)*100), io_res, 3, "", 
                         BEW_IO_ERROR));
      io_res = fscanf (bew_file, "%d%lf%lf%lf%lf%d",
                       &(kriterien[idx].bew_alg),
                       &(kriterien[idx].nstart), &(kriterien[idx].ns1),
                       &(kriterien[idx].ns2),    &(kriterien[idx].nend),
                       &(kriterien[idx].nmax_cont));
      ch_tmp = fgets (buffer,  MAX_STRING+1, bew_file);  /* Skip rest of line */
      if (io_res != 6)
         return (fehler (BEW_BEWERTUNG, 5+((ii+1)*100), io_res, 6, "", 
                         BEW_IO_ERROR));
      if (kriterien[idx].bew_alg == OLD_NIX_IDX)       /* wg. Kompatibilitaet */
	 kriterien[idx].bew_alg = NIX_IDX;             /* zu alten BEW-Files. */
      io_res = fscanf (bew_file, "%lf%lf%lf%lf%lf",
                       &(kriterien[idx].xstart), &(kriterien[idx].xs1),
                       &(kriterien[idx].xs2),    &(kriterien[idx].xend),
                       &(kriterien[idx].xdrittel));
      ch_tmp = fgets (buffer,  MAX_STRING+1, bew_file);  /* Skip rest of line */
      if (io_res != 5)
         return (fehler (BEW_BEWERTUNG, 6+((ii+1)*100), io_res, 5, "", 
                         BEW_IO_ERROR));
      if ((ii + 1) == akt_anz_used_krit)
      { /* Einlesen der Straffunktionen */
         idx = max_anz_bew_krit;
         ch_tmp = fgets (buffer, MAX_STRING+1, bew_file);   /* skip seperator */
      } /* Einlesen der Straffunktionen */
      else
         idx++;
   } /* Einlesen */
   gewichten ();
   if ((ret_state = prepare_eval ()) == GLEAM_OK)
   { /* OK */
      for (ii = 0;  ii < akt_anz_used_krit;  ii++)
         kriterien[ii].erfuellungs_note =
             calc_ungew_note (ii, kriterien[ii].erfuellungs_wert) *
             kriterien[ii].noten_gewicht;
      bew_aend_sim = TRUE;
      *bad_data    = FALSE;
      last_krit    = NIX_IDX;   /* erzwingt notw.Neuberechnungen b.BewFktAnzg */
   } /* OK */
   return (ret_state);
} /* restore_bew */ 



/* ================================= calc_a ================================= */
static DOUBLE calc_a (DOUBLE  n_ug,
                      DOUBLE  n_og,
                      DOUBLE  x_ug,
                      DOUBLE  x_og)
/*----------------------------------------------------------------------------*/
/*  Liefert als Funktionsergebnis den "a"-Wert der durch die Parameter be-    */
/*  zeichneten Gerade y = a * x + b ab.                                       */
/*----------------------------------------------------------------------------*/
{
   return ((n_og - n_ug) / (x_og - x_ug));
} /* calc_a */ 



/* ================================= calc_b ================================= */
static DOUBLE calc_b (DOUBLE  n_ug,
                      DOUBLE  n_og,
                      DOUBLE  x_ug,
                      DOUBLE  x_og)
/*----------------------------------------------------------------------------*/
/*  Liefert als Funktionsergebnis den "b"-Wert der durch die Parameter be-    */
/*  zeichneten Gerade y = a * x + b ab.                                       */
/*----------------------------------------------------------------------------*/
{
   return (((n_ug * x_og) - (n_og * x_ug)) / (x_og - x_ug));
} /* calc_b */ 



/* ================================= x_test ================================= */
static BOOL x_test (INT  index)
/*----------------------------------------------------------------------------*/
/*  Prueft, ob die Werte "xmin", "xs1", "xs2" und "xmax" des durch "index"    */
/*  bezeichneten Kriteriums in streng steigender Wertefolge sind und liefert  */
/*  das Ergebnis als Funktionsergebnis ab.                                    */
/*----------------------------------------------------------------------------*/
{
   return ((kriterien[index].xstart <  kriterien[index].xs1)  &&
           (kriterien[index].xs1    <  kriterien[index].xs2)  &&
           (kriterien[index].xs2    <  kriterien[index].xend));
} /* x_test */ 



/* ============================== prepare_eval ============================== */
GSTATE prepare_eval (void)
{
   INT     idx, ii;
   BOOL    ok;
   
   /* prepare_eval */
   ok  = TRUE;
   idx = 0;
   for (ii = 0;  ii < (2 * akt_anz_used_krit);  ii++)
   { /* Ein Kriterium pruefen */
      switch (kriterien[idx].bew_alg)
      {
         case LIN_MINCR:
            ok = x_test (idx) && ok &&
                 ((kriterien[idx].nstart <= kriterien[idx].ns1)  &&
		  (kriterien[idx].ns1    <= kriterien[idx].ns2)  &&
		  (kriterien[idx].ns2    <= kriterien[idx].nend)   );
            break; /* LIN_MINCR */
            
         case LIN_MDECR:
            ok = x_test (idx) && ok &&
                 ((kriterien[idx].nend <= kriterien[idx].ns2)    &&
		  (kriterien[idx].ns2  <= kriterien[idx].ns1)    &&
                  (kriterien[idx].ns1  <= kriterien[idx].nstart)   );
            break; /* LIN_MDECR */
            
         case EXP_MINCR:
            ok = (kriterien[idx].xdrittel < kriterien[idx].xend) && ok;
            break; /* EXP_MINCR */
            
         case EXP_MDECR:
            ok = (kriterien[idx].xstart < kriterien[idx].xdrittel) && ok;
            break; /* EXP_MDECR */
            
         case MIXED_MINCR:
            ok = x_test (idx) && ok &&
                 (kriterien[idx].xs1      < kriterien[idx].xdrittel) &&
                 (kriterien[idx].xdrittel < kriterien[idx].xs2)      &&
                 (kriterien[idx].ns2      > NOTEN_DRITTEL);
            break; /* MIXED_INCR */
            
         case MIXED_MDECR:
            ok = x_test (idx) && ok &&
                 (kriterien[idx].xs1      < kriterien[idx].xdrittel) &&
                 (kriterien[idx].xdrittel < kriterien[idx].xs2)      &&
                 (kriterien[idx].ns1      > NOTEN_DRITTEL);
            break; /*MIXED_MDECR */
            
         case NIX_IDX:
            ok = TRUE;
            break; /* noch nicht definierte Funktion */
         
         default:
	    if (idx < max_anz_bew_krit)
	       fehler (BEW_BEWERTUNG, 5, idx+1, kriterien[idx].bew_alg, 
		       BEW_FKT_TXT, BAD_EVAL_FKT);
	    else
	       fehler (BEW_BEWERTUNG, 5, idx - max_anz_bew_krit + 1, 
		       kriterien[idx].bew_alg, STR_FKT_TXT, BAD_EVAL_FKT);
	    kriterien[idx].bew_alg = NIX_IDX;
            return (GLEAM_ERROR);
      } /* CASE */
      if (!ok)
      {
         if (idx >= max_anz_bew_krit)
            return (fehler (BEW_BEWERTUNG, 6, idx-max_anz_bew_krit+1, NO_INFO, 
                    STR_FKT_TXT, BAD_BEW_DATA));
         else
            return (fehler (BEW_BEWERTUNG, 7, idx + 1, NO_INFO, 
                    BEW_FKT_TXT, BAD_BEW_DATA));
      }
      if ((ii + 1) == akt_anz_used_krit)
         idx = max_anz_bew_krit;           /* Wechsel zu den  Straffunktionen */
      else
         idx++;
   } /* Ein Kriterium pruefen */
   
   idx = 0;
   for (ii = 0;  ii < (2 * akt_anz_used_krit);  ii++)
   { /* Bewertung eines Kriteriums vorbereiten */
      switch (kriterien[idx].bew_alg)
      {
         case LIN_MINCR:
         case LIN_MDECR:
            kriterien[idx].a1 = calc_a(kriterien[idx].nstart,kriterien[idx].ns1,
                                       kriterien[idx].xstart,kriterien[idx].xs1);
            kriterien[idx].a2 = calc_a(kriterien[idx].ns1, kriterien[idx].ns2,
                                       kriterien[idx].xs1, kriterien[idx].xs2);
            kriterien[idx].a3 = calc_a(kriterien[idx].ns2, kriterien[idx].nend,
                                       kriterien[idx].xs2, kriterien[idx].xend);
            kriterien[idx].b1 = calc_b(kriterien[idx].nstart,kriterien[idx].ns1,
                                       kriterien[idx].xstart,kriterien[idx].xs1);
            kriterien[idx].b2 = calc_b(kriterien[idx].ns1, kriterien[idx].ns2,
                                       kriterien[idx].xs1, kriterien[idx].xs2);
            kriterien[idx].b3 = calc_b(kriterien[idx].ns2, kriterien[idx].nend,
                                       kriterien[idx].xs2, kriterien[idx].xend);
            break; /* LIN_MINCR oder LIN_MDECR */
            
         case EXP_MINCR:
            kriterien[idx].a1 = (LN_DIFF * kriterien[idx].xend) /
                               (kriterien[idx].xend - kriterien[idx].xdrittel);
            kriterien[idx].b1 = ((kriterien[idx].xend - kriterien[idx].xdrittel) *
                                LN_NOTEN_MAX) / LN_DIFF;
            kriterien[idx].nend = RNOTEN_MAX;
            break; /* EXP_MINCR */
            
         case EXP_MDECR:
            kriterien[idx].a1 = ((-LN_DIFF) * kriterien[idx].xstart) /
                               (kriterien[idx].xdrittel - kriterien[idx].xstart);
            kriterien[idx].b1 = ((kriterien[idx].xdrittel - kriterien[idx].xstart) *
                                LN_NOTEN_MAX) / LN_DIFF;
            kriterien[idx].nend = RNOTEN_MAX;
            break; /* EXP_MDECR */
            
         case MIXED_MINCR:
         case MIXED_MDECR:
            /* --------- Exponential-Anteil: --------- */
	    if (kriterien[idx].bew_alg == MIXED_MINCR)
	    { /* mixed + increasing */
	       kriterien[idx].a2 = (kriterien[idx].xs2 *
				    (LN_DRITTEL - log (kriterien[idx].ns2))) /
		                   (kriterien[idx].xs2 - kriterien[idx].xdrittel);
	       kriterien[idx].b2 = ((log (kriterien[idx].ns2) *
				     (kriterien[idx].xs2-kriterien[idx].xdrittel))
				    / (LN_DRITTEL - log (kriterien[idx].ns2)))
	             	           + kriterien[idx].xs2;
	       kriterien[idx].ns1  = exp(((kriterien[idx].b2-kriterien[idx].xs1) *
					  kriterien[idx].a2) / kriterien[idx].xs2);
	       kriterien[idx].nstart = 0.0;
	       kriterien[idx].nend   = RNOTEN_MAX;
	    } /* mixed + increasing */
	    else
	    { /* mixed + decreasing */
	       kriterien[idx].a2 = (kriterien[idx].xs1 *
				    (log (kriterien[idx].ns1) - LN_DRITTEL)) /
		                   (kriterien[idx].xdrittel - kriterien[idx].xs1);
	       kriterien[idx].b2 = ((log (kriterien[idx].ns1) *
				     (kriterien[idx].xdrittel-kriterien[idx].xs1))/
				    (LN_DRITTEL - log (kriterien[idx].ns1)))
		                   - kriterien[idx].xs1;
	       kriterien[idx].ns2= exp(-((kriterien[idx].b2 + kriterien[idx].xs2) *
					 kriterien[idx].a2) / kriterien[idx].xs1);
	       kriterien[idx].nstart = RNOTEN_MAX;
	       kriterien[idx].nend   = 0.0;
	    } /* mixed + decreasing */
            
            /* ---------- 1.Linear-Anteil: ----------- */
            kriterien[idx].a1 = calc_a(kriterien[idx].nstart, kriterien[idx].ns1,
                                       kriterien[idx].xstart, kriterien[idx].xs1);
            kriterien[idx].b1 = calc_b(kriterien[idx].nstart, kriterien[idx].ns1,
                                       kriterien[idx].xstart, kriterien[idx].xs1);
            
            /* ---------- 2.Linear-Anteil: ----------- */
            kriterien[idx].a3 = calc_a(kriterien[idx].ns2, kriterien[idx].nend,
                                       kriterien[idx].xs2, kriterien[idx].xend);
            
            kriterien[idx].b3 = calc_b(kriterien[idx].ns2, kriterien[idx].nend,
                                       kriterien[idx].xs2, kriterien[idx].xend);
            break; /* MIXED_MINCR oder MIXED_MDECR */
      } /* CASE */
      if ((ii + 1) == akt_anz_used_krit)
         idx = max_anz_bew_krit;           /* Wechsel zu den  Straffunktionen */
      else
         idx++;
   } /* Bewertung eines Kriteriums vorbereiten */
   return (GLEAM_OK);
} /* prepare_eval */ 



/* ================================ gewichten =============================== */
void gewichten (void)
{
   INT  ii;
   
   /* gewichten */
   max_klasse = 0;
   for (ii = 0;  ii < used_krit_og;  ii++)
      if ((ii < akt_anz_used_krit) || (max_anz_bew_krit <= ii))
      { /* Alle benutzten Kriterien */
         kriterien[ii].noten_gewicht = kriterien[ii].max_note / RNOTEN_MAX;
         if (kriterien[ii].klasse > max_klasse)
            max_klasse = kriterien[ii].klasse;
      } /* Alle benutzten Kriterien */
} /* gewichten */ 



/* ============================ reset_krit_gueltig ========================== */
void reset_krit_gueltig (void)
{
   memset (krit_gueltig, 0, bigBArrayLen);
} /* reset_krit_gueltig */



/* ======================== resetSimValOutOfEvalRange ======================= */
void resetSimValOutOfEvalRange (void)
{
   memset (simValOutOfEvalRange, 0, smallBArrayLen);
} /* resetSimValOutOfEvalRange */



/*============================ alloc_bewkrit_data ============================*/
static BOOL alloc_bewkrit_data (INT  anz)
/*----------------------------------------------------------------------------*/
/*  Die Routine allokiert das "kriterien"- Feld mit "anz" Eintraegen, sofern  */
/*  das bereits vorhandene Feld kleiner ist. Ausserdem werden noch folgende,  */
/*  mit der Feldgroesse "anz" in Zusammenhang stehende Datenobjekte (neu) an- */
/*  gelegt: "ungew_note", "int_ungew_note", "best_ug_note", "gew_note",       */
/*  "int_gew_note", "best_g_note", "straf_fkt", "int_straf_fkt", "best_str_   */
/*  fkt", "krit_gueltig", "int_krit_gueltig" und "best_krit_g".               */
/*  "anz" entspricht der Anzahl der Kriterien und der Straffunktionen und     */
/*  MUSS daher ein gerader Wert sein.                                         */
/*  Die Funktion liefert FALSE und erzeugt einen fatalen Fehler, wenn dazu    */
/*  der Speicher nicht ausreicht.                                             */
/*----------------------------------------------------------------------------*/
{
   size_t  ksize = sizeof (KRITERIUMS_TYP);
   size_t  dsize = sizeof (DOUBLE);
   size_t  bsize = sizeof (BOOL);
   INT     anz_halbe, ii;
   BOOL    erg;

   /* alloc_bewkrit_data */
   if ((kriterien != NULL) && (max_anz_kriterien < anz))
   { /* deallocate "kriterien" and related data objects */
      free (kriterien);
      kriterien = NULL;
      free (simu_erg);
      free (int_simu_erg);
      free (best_simu_erg);
      free (ungew_note);
      free (int_ungew_note);
      free (best_ug_note);
      free (gew_note);
      free (int_gew_note);
      free (best_g_note);
      free (straf_fkt);
      free (int_straf_fkt);
      free (best_str_fkt);
      free (krit_gueltig);
      free (int_krit_gueltig);
      free (simValOutOfEvalRange);
      free (int_simValOutOfEvalRange);
      free (best_simValOutOfEvalRange);
      free (best_krit_g);
#ifdef GUI_MODE
      free (critLine2Index);
#endif /* GUI_MODE */
   } /* deallocate "kriterien" and related data objects */
   anz_halbe      = anz / 2;
   bigBArrayLen   = anz * bsize;
   smallBArrayLen = anz_halbe * bsize;
   if (kriterien == NULL)
   { /* allocate "kriterien" and related data objects */
      erg = ((kriterien     = (KRITERIUMS_TYP*)malloc(anz * ksize)) != NULL) &&
            ((simu_erg        = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((int_simu_erg    = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((best_simu_erg   = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((ungew_note      = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((int_ungew_note  = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((best_ug_note    = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((gew_note        = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((int_gew_note    = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((best_g_note     = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((straf_fkt       = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((int_straf_fkt   = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((best_str_fkt    = (DOUBLE*)malloc(anz_halbe * dsize)) != NULL) &&
            ((krit_gueltig    = (BOOL*)  malloc(bigBArrayLen)) != NULL) &&
            ((int_krit_gueltig= (BOOL*)  malloc(bigBArrayLen)) != NULL) &&
            ((best_krit_g     = (BOOL*)  malloc(bigBArrayLen)) != NULL) &&
	    ((simValOutOfEvalRange      = (BOOL*)malloc(smallBArrayLen)) != NULL) &&
	    ((int_simValOutOfEvalRange  = (BOOL*)malloc(smallBArrayLen)) != NULL) &&
            ((best_simValOutOfEvalRange = (BOOL*)malloc(smallBArrayLen)) != NULL);

#ifdef GUI_MODE
      erg &= ((critLine2Index  = (INT*)malloc(anz_halbe * sizeof(INT))) != NULL);
#endif /* GUI_MODE */
      max_anz_bew_krit  = anz_halbe;
      max_anz_kriterien = anz;
   } /* allocate "kriterien" and related data objects */
   else
      erg = TRUE;
   if (erg) {
      for (ii = 0;  ii < anz;  ii++)
      { /* Initialisierungen */
         memset (&(kriterien[ii]), 0, sizeof(KRITERIUMS_TYP));
         memset (kriterien[ii].bezeichnung, ' ', KRIT_BEZ_LEN);
         kriterien[ii].bezeichnung[KRIT_BEZ_LEN] = ':';
         kriterien[ii].einheit[0]           = ' ';
         kriterien[ii].klasse               = KRITERIUM_AUS;
      } /* Initialisierungen */
      reset_krit_gueltig ();
      resetSimValOutOfEvalRange ();
   }
   else
      fatal (BEW_BEWERTUNG, 8, anz_halbe, NO_INFO, "", BAD_KRIT_AMOUNT);
   return (erg);
} /* alloc_bewkrit_data */



/*================================ bew_init ==================================*/
BOOL bew_init (void)
{
   BOOL  erg;

   /* bew_init */
   kriterien         = NULL;
   max_anz_bew_krit  = MIN_ANZ_BEW_KRIT;
   max_anz_kriterien = MIN_ANZ_BEW_KRIT * 2;
   erg       = alloc_bewkrit_data(max_anz_kriterien) &&
               ((bez_len_str  = (char*)malloc(KRIT_BEZ_LEN+1)) != NULL) &&
               ((unit_len_str = (char*)malloc(ENAME_LEN+1))    != NULL)
#ifdef GNUPLOT
               && ((gnu_buffer = (char*)malloc(400))           != NULL)
               && ((gnu_fname  = (char*)malloc(MAX_STR+1))     != NULL)
#endif /* GNUPLOT */
   ;
   if (erg)
   { /* init strings */
      memset (bez_len_str, 'x', KRIT_BEZ_LEN);
      bez_len_str[KRIT_BEZ_LEN] = '\0';
      memset (unit_len_str, 'x', ENAME_LEN);
      unit_len_str[ENAME_LEN] = '\0';
      record_evo_best = TRUE;
   } /* init strings */
   return (erg);
} /* bew_init */
