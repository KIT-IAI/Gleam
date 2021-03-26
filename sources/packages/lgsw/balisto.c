/*******************************************************************************
GLEAM/AE                                                                  V1.1.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: Listen-GrundSoftWare
                        Module: BAsic LIST Operations 
 
Package: lgsw                   File: balisto.c                 cc / Solaris 2.4
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt Routinen zur Administration von einfach verzeigerten Listen, 
wie sie die Aktionsketten (AKs) oder Chromosome darstellen. AKs koennen Unter-
ketten (hier: Unter-AKs) haben. Evolutionierbare Ketten (davon derzeit nur AKs) 
sind segmentiert, d.h. in Segmente oder Abschnitte unterteilt. Diese spielen 
fuer eine Reihe von Mutationsoperatoren sowie fuer das Crossover und die Rekom-
bination eine wichtige Rolle. Die Routinen koennen in folgende Funktionsgruppen 
eingeteilt werden: 

   - Manipulation einzelner Listenelemente
   - Manipulation von Elementsequenzen (Verzeigerte Elemente ohne Kopf und Seg-
     mentstruktur)
   - Manipulation von Ketten und Unterketten
   - Manipulation der Segmentstruktur

Ueber den Compilerschalter "CHECKS" koennen eine Reihe von Pruefungen der Para-
meter ein/ausgeschaltet werden, dabei gilt generell, dass die Uebergabe leerer 
Zeiger (ausser als Returnparameter) einen (fatalen) Fehler darstellt. Alle Rou-
tinen liefern GSTATE als Returnstatus ab (vgl. Package "fbhm"). Der Wert ist da-
bei entweder GLEAM_OK oder GLEAM_FATAL. Im Falle von GLEAM_FATAL wird ein von 
GSTATE verschiedenes Funktionsergebnis immer FALSE sein. Entsprechend sind als 
Returnparameter abgelieferte Ergebnisse ebenfalls FALSE, Null oder NIL. Es wird 
davon ausgegangen, dass genuegend Speicher zur Verfuegung steht (sonst fataler 
Fehler); d.h. dass dies durch die aufrufenden Routinen zu ueberpruefen ist.
 
Aktionsketten (AKs) haben einen Kopf, der den Referenzzaehler "ref_counter" ent-
haelt. Die Routinen des Units pflegen den Zaehler. Verweise von Pointervariablen
auf eine Kette sind von der anwendenden Software zu pflegen. Nicht dazu gehoeren
temporaere Variable, die z.B. benutzt werden, um eine Kette zu erzeugen, die 
dann als Unterkette in eine andere eingehaengt wird. Ferner steht im Kopf aller
Ketten die "chain_length", die von allen die Kettenlaenge veraendernden Routinen
gepflegt wird.
 
Zur Verwaltung der unterschiedlichen Ketten werden wiederum Ketten, naemlich 
Verwaltungsketten (VK) benutzt. Dazu werden keine speziellen Kettenelemente de-
finiert, sondern Handlungsaktionen ("activity" = "activity_act") benutzt, da 
diese Unterketten, naemlich die zu verwaltenden Ketten, haben duerfen. Damit 
sind alle BALISTO-Funktionen auch auf die Verwaltungsketten anwendbar.
 

Folgende Routinen pflegen die Segmentstruktur:
       append_item
       insert_item
       remove_next_item
"insert_seq", "remove_seq" und "delete_seq" lassen jedoch die Segmentstruktur 
unveraendert und ueberlassen diese Aufgabe den rufenden Routine. Dies ist sinn-
voll, da beide Routinen nur zur Manipulation kompletter Segmente benutzt werden
und die aufrufende Software sich sowieso mit der Segmentstruktur auseinander-
setzen muss.

Die Segmentstruktur ist folgendermassen organisiert:

Segmente: |          |                 |     |           |                   |
AK-Header->Akt1->Akt2->Akt3->Akt4->Akt5->Akt6->Akt7->Akt8->Akt9->Akt10->Akt11
  |    ^           ^                 ^     ^           ^                  ^
  V    |           |                 |     |           |                  |
Len Ptr|           |                 |     |           |                  |
 -   *-+           |                 |     |           |                  |
 2   *-------------+                 |     |           |                  |
 3   *-------------------------------+     |           |                  |
 1   *-------------------------------------+           |                  |
 2   *-------------------------------------------------+                  |
 3   *--------------------------------------------------------------------+



Die Routinen im einzelnen:

GSTATE find_predecessor (CHAIN_ELEM_PTR_TYPE *pred_item_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                         CHAIN_ELEM_PTR_TYPE  item_ptr);
          Ermittelt das Vorgaenger-Item zu "item_ptr" und liefert in "pred_item_
          ptr" einen Zeiger darauf ab. Wenn das Vorgaenger-Item nicht gefunden 
          wird, d.h. nicht in der Kette ist, wird NIL abgeliefert. 

BOOL   count_out_item (CHAIN_ELEM_PTR_TYPE *item_ptr, 
                       CHAIN_ELEM_PTR_TYPE  start_ptr, 
                       INT                  number,
                       GSTATE              *ret_status);
          Ermittelt den Zeiger ("item_ptr") auf das "number"-te Item ab dem 
          Item, auf das "start_ptr" zeigt. Wenn "number" 0 ist, ist der "item_
          ptr" gleich dem "start_ptr". Wenn die Kette zu kurz ist, wird NIL ab-
          geliefert und das Funktionsergebnis ist FALSE. Die Funktion wird nur 
          ausgefuehrt, wenn "ret_status" GLEAM_OK ist.

GSTATE append_item (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                    CHAIN_ELEM_PTR_TYPE  item_ptr);
          Das Item wird an das Ende der Kette "chain_ptr" angefuegt. Bei segmen-
          tierten Ketten werden die Segmentzeiger in den Segmentdescriptoren ge-
          pflegt. 

GSTATE insert_item (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                    CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                    CHAIN_ELEM_PTR_TYPE  item_ptr);
          Das Item wird hinter das Item, auf das "behind_ptr" zeigt, eingefuegt.
          Bei einer leeren Kette wird das Item eingefuegt, ohne dass der 
          "behind_ptr" eine Rolle spielt. Bei segmentierten Ketten werden die 
          Segmentzeiger in den Segmentdescriptoren folgendermassen gepflegt: 
          Wenn das Element zwischen zwei Abschnitte eingefuegt werden soll, 
          d.h., dass der "behind_ptr" auf das gleiche Kettenelement zeigt, wie 
          der "segm_ptrs" eines Segmentdescriptors, dann wird ausgewuerfelt, zu 
          welchem Abschnitt das Insert gehoeren soll; die Verteilung ist 50:50.

GSTATE remove_next_item (CHAIN_ELEM_PTR_TYPE *item_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                         CHAIN_ELEM_PTR_TYPE  behind_ptr);
          Das Item hinter dem Item, auf das "behind_ptr" zeigt, wird aus der 
          Kette "chain_ptr" entfernt und in "item_ptr" abgeliefert. Wenn 
          "behind_ptr" auf das letzte Item der Kette zeigt, wird NIL abgelie-
          fert. Bei segmentierten Ketten werden die Segmentzeiger in den Seg-
          mentdescriptoren gepflegt. 
    
GSTATE copy_act (CHAIN_ELEM_PTR_TYPE *copied_item_ptr, 
                 CHAIN_ELEM_PTR_TYPE  item_ptr);
          Das Item, auf das der "item_ptr" zeigt, wird kopiert und seine 
          Daten in einem neu erzeugten Item, auf das der Zeiger "copied_
          item_ptr" zeigt, abgeliefert.
          Wenn das Item bei gesetztem WITH_DYN_GENES eine Gen oder eine Aktion 
          ist, werden die beiden Parameterfelder von "copied_item_ptr" angelegt 
          und die Daten kopiert. 
          Wenn das zu kopierende Item eine Handlungsaktion ("activity_act") ist,
          wird entsprechend "ref_typ" wie folgt verfahren (Die Konstanten haben 
          negative Werte):
          ref_typ == SUB_AK_REF : Das Item wird samt Referenz kopiert und der 
                                  "ref_counter" der Unter-AK inkrementiert.
          ref_typ == VK_ITEM_REF: fataler Fehler!
          ref_typ >= 0          : Die Unterkette wird kopiert und die Kopie des 
                                  Items zeigt auf die kopierte Unterkette. Da-
                                  bei wird davon ausgegangen, dass fuer die Un-
                                  terkette genuegend Speicherplatz vorhanden ist.
          Wenn dazu nicht genügend Speicherplatz zur Verfügung steht erfolgt eine 
          fatale Fehlermeldung und es wird der entsprechende Status zurueck 
          geliefert.

GSTATE copy_item (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                  CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                  CHAIN_ELEM_PTR_TYPE  item_ptr);
          Eine Kopie des Items, auf das "item_ptr" zeigt, wird hinter dem Item, 
          auf das "behind_ptr" zeigt, eingefuegt. Die Zeiger "chain_ptr" und 
          "behind_ptr" muessen dabei zur selben Kette gehoeren. Die Kopie wird 
          mit der Funktion "copy_act" erzeugt; eingefuegt wird mit "insert_
          item", so dass bei segmentierten Ketten die Segmentstruktur gepflegt 
          wird.

GSTATE delete_item (CHAIN_ELEM_PTR_TYPE  item_ptr);
          Loescht die Aktion, auf die "item_ptr" zeigt. Wenn das Item ein Unter-
          kettenverweiss ("activity_act") ist, wird eine eventuelle Unterhand-
          lungskette per "delete_chain" versucht zu vernichten. Der Versuch, ein
          Header-Item zu loeschen, ist ein fataler Fehler.


GSTATE insert_seq (CHAIN_ELEM_PTR_TYPE  target_chain_ptr, 
                   CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                   CHAIN_ELEM_PTR_TYPE  anf_ptr, 
                   CHAIN_ELEM_PTR_TYPE  end_ptr, 
                   INT                  seq_len);
          Die mit "anf_ptr" und "end_ptr" beschriebene Kettensequenz wird hin-
          ter das Item, auf das "behind_ptr" in der "target_chain" zeigt, einge-
          fuegt. Bei segmentierten Ketten wird die Segmentstruktur nicht ge-
          pflegt. Die Kettenlaenge wird um die (ungepruefte!) "seq_len" inkre-
          mentiert.

GSTATE remove_seq (CHAIN_ELEM_PTR_TYPE *anf_ptr, 
                   CHAIN_ELEM_PTR_TYPE *end_ptr, 
                   INT                 *number, 
                   CHAIN_ELEM_PTR_TYPE  source_chain_ptr, 
                   CHAIN_ELEM_PTR_TYPE  behind_ptr);
          Eine Anzahl von "number" Items hinter dem Item, auf das "behind_ptr" 
          zeigt, wird aus der Kette "source_chain_ptr" entfernt. Diese Sequenz 
          kann auf zwei Arten vorgegeben werden: Wenn "end_ptr" ungleich NIL
          ist, wird sie mit "behind_ptr", "end_ptr" und "number" angegeben 
          ("number" wird nicht ueberprueft, muss aber stimmen!). Bei der zwei-
          ten Art erfolgt die Vorgabe nur durch "behind_ptr" u. "number", wobei 
          fuer den Fall, dass weniger als "number" Items vorhanden sind, der 
          Kettenrest genommen und "number" angepasst wird. Die Sequenz wird in 
          "anf_ptr" und "end_ptr" abgeliefert. Wenn "number" < 1 ist oder 
          "behind_ptr" auf das letzte Kettenelement zeigt, wird eine leere Se-
          quenz abgeliefert, d.h. "anf_ptr" und "end_ptr" sind NIL. "number" 
          wird gegebenfalls upgedatet. Bei segmentierten Ketten wird die Seg-
          mentstruktur NICHT gepflegt.

GSTATE copy_seq (CHAIN_ELEM_PTR_TYPE *start_ptr, 
                 CHAIN_ELEM_PTR_TYPE *end_ptr, 
                 CHAIN_ELEM_PTR_TYPE  first_ptr, 
                 INT                  number);
          Die Item-Sequenz ab "first_ptr" bestehend aus "number" Items (gedacht 
          ist an eine ganze Kette oder einen Abschnitt, daher wird "number" >= 1
          vorausgesetzt) wird kopiert und als Sequenz in "start_ptr" und "end_
          ptr" abgeliefert. Wenn die Originalsequenz kuerzer als "number" Items
          ist und der Compilerschalter "CHECKS" gesetzt ist, erfolgt ein fataler
          Fehler wegen Zugriffs auf NIL-Pointer. Kopiert wird mit "copy_act".

GSTATE delete_seq (CHAIN_ELEM_PTR_TYPE  start_ptr, 
                   CHAIN_ELEM_PTR_TYPE  end_ptr, 
                   INT                  seq_len);
          Die durch "start_ptr" und "end_ptr" bezeichnete Sequenz der Laenge 
          "seq_len" wird auf Unterketten durchsucht. Diese werden mit "delete_
          chain" geloescht. Danach wird die Sequenz mit "free_seq" freigegeben. 
          Die Routine prueft nicht, ob ihr eine Kette uebergeben wurde, d.h. ob
          "start_ptr" auf einen Header zeigt, oder ob die Laenge stimmt.
 
BOOL   copy_chain (CHAIN_ELEM_PTR_TYPE *new_chain_ptr, 
                   CHAIN_ELEM_PTR_TYPE  old_chain_ptr, 
                   BOOL                 urkette, 
                   BOOL                 mem_check,
                   GSTATE              *ret_status);
          Kopiert alle Ketten (d.h AKs) ausser VKs (fataler Fehler) und liefert 
          TRUE ab, wenn genuegend Speicher vorhanden ist, sonst FALSE und einen 
          leeren "new_chain_ptr".
          Bei gesetztem "mem_check" erfolgt die Platzpruefung fuer die gesamte 
          Kette VOR dem Kopieren, sonst wird blind kopiert. Die Abschaltmoeg-
          lichkeit ist vor allem fuer "copy_act" vorgesehen, um die Pruefung von
          Unterketten bereits gepruefter Ketten unterdruecken zu koennen. Bei
          gesetzter "urkette" ist im "chain_status" nur das Bit "unmutierte_
          urkette" gesetzt, ansonsten wird der "chain_status" der alten Kette 
          bis auf die Bits BASIS_AK, SIMULATED und RANDOMLY_GENERATED uebernom-
          men. Bei segmentierten Ketten (AKs) wird die Segmentstruktur mitko- 
          piert. Die restlichen Daten sind im Initialzustand wie bei "init_
          chain". Die Funktion wird nur ausgefuehrt, wenn "ret_status" GLEAM_OK 
          ist. 

GSTATE delete_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr);
          Zunaechst wird ueberprueft, ob es sich tatsaechlich um einen Ketten-
          kopf handelt; sonst fataler Fehler. Der "ref_counter" der Kette wird 
          dekrementiert und der "chain_ptr" auf NIL gesetzt. Wenn der "ref_
          counter" der Kette gleich oder kleiner als null ist, werden alle Items
          der Kette je nach Kettenart per "delete_seq" oder direkt per "free_
          seq" geloescht. Im ersten Fall werden auch alle Unterketten per 
          "delete_chain" versucht zu loeschen. Das Headerparameter-Item und 
          eventuelle Segmentdescriptor-Items werden vernichtet. 

GSTATE append_sub_chain (CHAIN_ELEM_PTR_TYPE  item_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Die Kette "chain_ptr" wird als Unterkette an das Item angefuegt, so-
          fern es sich dabei um ein "activity_act" handelt (sonst fataler Feh-
          ler; Pruefung nicht abschaltbar). Bei AKs wird der "ref_counter" in-
          krementiert. Wenn das Item bereits eine Unter-AK hat, wird diese ent-
          fernt und versucht, zu loeschen.

GSTATE remove_sub_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                         CHAIN_ELEM_PTR_TYPE  item_ptr);
          Die Unterkette des "activity_act" Item (sonst fataler Fehler) wird 
          entfernt und in "chain_ptr" abgeliefert. Bei AKs wird der "ref_coun-
          ter" dekrementiert. Wenn es keine Unterkette gibt, wird ein NIL-Zei-
          ger abgeliefert.

GSTATE inc_segm_nr (REF_DESCR_PTR_TYPE  *segm_descr_ptr, 
                    INT                 *index);
          Der "index" wird inkrementiert, sofern er kleiner als SEGM_PTR_ANZ 
          ist. Sonst zeigt "segm_descr_ptr" auf den naechsten Segmentdescriptor 
          und "index" ist 0.
  
BOOL   check_segm_ptr (REF_DESCR_PTR_TYPE  *segm_descr_ptr, 
                       INT                 *index, 
                       INT                  segm_anz, 
                       CHAIN_ELEM_PTR_TYPE  item_ptr,
                       GSTATE              *ret_status);
          "segm_anz" Eintraege in den linear verketteten Segmentdescriptoren 
          werden darauf ueberprueft, ob sie auf das gleiche Item zeigen wie 
          "item_ptr". Wenn ja, ist die Funktion TRUE und in "index" wird der 
          zugehoerige Feldindex des "segm_ptrs"-Arrays abgeliefert, das im 
          Descriptor steht, auf den "segm_descr_ptr" zeigt. "segm_anz" wird 
          als groesser null voraus gesetzt. Die Funktion wird nur ausgefuehrt, 
          wenn "ret_status" GLEAM_OK ist.

BOOL   ermittle_abschnitt (CHAIN_ELEM_PTR_TYPE *pred_ptr, 
                           CHAIN_ELEM_PTR_TYPE *last_ptr, 
                           INT                 *len, 
                           INT                 *index, 
                           REF_DESCR_PTR_TYPE  *sd_ptr, 
                           CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                           INT                  number,
                           GSTATE              *ret_status);
         In der Kette "chain_ptr" wird der Abschnitt mit der Nummer "number" ge-
         sucht. Wenn er gefunden wurde, wird folgendes abgeliefert: "pred_ptr" 
         zeigt auf das Item vor dem ersten Abschnitts-Item, "last_ptr" auf das
         letzte, "len" enthaelt die Abschnittslaenge und "index" und "sd_ptr" 
         bezeichnen den Segmentdescriptoreintrag. Wenn der Abschnitt nicht in 
         der Kette ist oder ein fataler Fehler auftrat, liefert die Funktion 
         FALSE ab und die Ergebnisparameter sind unbestimmt. Ansonsten TRUE. Die
         Funktion wird nur ausgefuehrt, wenn "ret_status" GLEAM_OK ist.

GSTATE insert_segm_entry (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                          REF_DESCR_PTR_TYPE   segm_d_ptr, 
                          INT                  index, 
                          CHAIN_ELEM_PTR_TYPE  last_ptr, 
                          INT                  seq_len, 
                          BOOL                 wohin);
         Ab dem durch "segm_d_ptr" und "index" bezeichneten Segmentdeskriptor-
         eintrag werden alle Eintraege um jeweils einen Indexwert verschoben. 
         Bei "wohin" = DAVOR einschliesslich des bezeichneten Eintrags und bei 
         "wohin" = DAHINTER ab dem folgenden. In den so vor oder hinter dem be-
         zeichneten Eintrag freigewordenen Deskriptoreintrag wird der Abschnitt
         "last_ptr", "seq_len" eingetragen und "segment_anz" inkrementiert. Die 
         Routine geht dabei davon aus, dass ein eventuell benoetigter neuer De-
         skriptorrecord zur Verfuegung steht. Der bezeichnete Eintrag muss exi-
         stieren; es darf der letzte sein. Existiert er nicht und ist der Com-
         pilerschalter "checks" gesetzt, wird ein fataler Fehler festgestellt.

GSTATE remove_segm_entry (REF_DESCR_PTR_TYPE   segm_d_ptr, 
                          INT                  index, 
                          CHAIN_ELEM_PTR_TYPE  chain_ptr);
         Der durch "segm_d_ptr" und "index" bezeichnete Segmentdeskriptoreintrag
         wird entfernt und alle weiteren Eintraege ruecken auf. "segment_anz" im
         Kopf der Kette "chain_ptr" wird dekrementiert.

GSTATE copy_segm_structure (CHAIN_ELEM_PTR_TYPE  new_ch_ptr, 
                            CHAIN_ELEM_PTR_TYPE  old_ch_ptr);
         Die Segmentstruktur von "old_ch_ptr" wird auf die von "new_ch_ptr" 
         uebertragen. Dabei wird davon ausgegangen, dass die neue Kette unseg-
         neue Kette unsegmentiert ist und dass beide gleich lang sind. Bei ge-
         setzter Compiler-Option "CHECKS" wird gefprueft, ob es sich um gleich-
         lange AKs handelt, sonst fataler Fehler.  Ferner wird die Uebertragung 
         der Segmentstruktur ueberwacht (nicht abschaltbarer fataler Fehler). Es
         wird ungeprueft vorausgesetzt, dass genuegend Speicher verfuegbar ist.

BOOL   teste_ak (CHAIN_ELEM_PTR_TYPE  ch_ptr);
         Folgende Kopfdaten der uebergebenen AK werden mit der Kette verglichen:
         AK-Laenge, Segmentanzahl und Segmentlaenge. Die Routine liefert TRUE, 
         wenn alles stimmt, sonst FALSE. Auftretende Fehler werden im Textpuffer
         vermerkt.

--------------------------------------------------------------------------------
basiert auf den C-Units: balsito.c, bacha.c
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Neue Routine "teste_ak".                                         WJ 18.08.99
 2. "copy_chain" kopiert das Statusbit RANDOMLY_GENERATED nicht mit. WJ 09.02.06
 3. Segment-Doku ergaenzt. "test_ak" prueft jetzt auch "last_action".WJ 26.01.07
 4. Verbesserung der Doku.                                           WJ 13.06.08
 5. Entfernung ueberfluessiger Variable. Initialisierung von 
    uninitialisierten Variablen.                                     WJ 26.06.12
 6. Renovierung: Streichung des "link"-Includes.                     WJ 18.04.16
 7. Korrektur der Pruefung des Kettentyps in "delete_chain".         WJ 13.11.17
 8. copy_act() und delete_chain(): Anpassungen an WITH_DYN_GENE.     WJ 08.07.19
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 08.07.2019
================================================================================ */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "lgsw.h"

#include LGSW_TEXTS
#include GLOBAL_TEXTS


#ifdef GLO_CHECKS
#define CHECKS
#endif



/* ============================ Lokale Konstante: =========================== */
#define MAX_SEGM_PTR_INDEX     (SEGM_PTR_ANZ - 1)


/* ============================ Lokale Routinen: ============================ */
static VOID   suche_segm          (REF_DESCR_PTR_TYPE  *segm_d_ptr,
                                   INT                 *index,
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr,
                                   CHAIN_ELEM_PTR_TYPE  such_ptr);

static VOID   del_ref_descr_chain (REF_DESCR_PTR_TYPE   ref_d_ptr);

static GSTATE umkopieren          (REF_DESCR_PTR_TYPE   segm_d_ptr, 
                                   INT                  index, 
                                   CHAIN_ELEM_PTR_TYPE  old_s_ptr, 
                                   INT                  old_s_len,
                                   INT                  s_anz_plus);




/* ===========================  find_predecessor ============================ */
GSTATE find_predecessor (CHAIN_ELEM_PTR_TYPE *pred_item_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                         CHAIN_ELEM_PTR_TYPE  item_ptr)
{
   CHAIN_ELEM_PTR_TYPE  search_ptr;
  
   /* find_predecessor */
#ifdef CHECKS
   *pred_item_ptr = NULL;
   if (chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 1, NO_INFO, NO_INFO, "", NIL_PTR));
   if (item_ptr  == NULL) 
      return (fatal (LGSW_BALISTO, 2, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   *pred_item_ptr = chain_ptr;
   search_ptr     = chain_ptr->u_el.next_action;
   while ((search_ptr != NULL) && (search_ptr != item_ptr)) 
   { /* search item on which item_ptr points */
      *pred_item_ptr = search_ptr;
      search_ptr     = (*pred_item_ptr)->u_el.next_action;
   } /* search item on which item_ptr points */
   if (search_ptr == NULL) 
      *pred_item_ptr = NULL;
   return (GLEAM_OK);
} /* find_predecessor */



/* ============================= count_out_item ============================= */
BOOL count_out_item (CHAIN_ELEM_PTR_TYPE *item_ptr, 
                     CHAIN_ELEM_PTR_TYPE  start_ptr, 
                     INT                  number,
                     GSTATE              *ret_status)
{
   BOOL  r_count_out_item = FALSE;
   INT   ii;
  
   /* count_out_item */
#ifdef CHECKS
   *item_ptr = NULL;
   if (start_ptr == NULL) 
      *ret_status |= fatal (LGSW_BALISTO, 3, NO_INFO, NO_INFO, "", NIL_PTR);
#endif
   if (*ret_status == GLEAM_OK)
   { /* nix fatal */
      r_count_out_item = TRUE;
      *item_ptr        = start_ptr;
      if (number > 0) 
      { /* count out */
         ii = 0;
         do 
         {
            ii++;
            *item_ptr = (*item_ptr)->u_el.next_action;
         } 
         while (!((ii == number) || (*item_ptr == NULL)));
         r_count_out_item = *item_ptr != NULL;
      } /* count out */
   } /* nix fatal */
   return (r_count_out_item);
} /* count_out_item */



/* =============================== append_item ============================== */
GSTATE append_item (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                    CHAIN_ELEM_PTR_TYPE  item_ptr)
{ 
   REF_DESCR_PTR_TYPE  this_segm_d_ptr;
   INT                 ii, index;
  
   /* append_item */
#ifdef CHECKS
   if (chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 4, NO_INFO, NO_INFO, "", NIL_PTR));
   if (item_ptr  == NULL) 
      return (fatal (LGSW_BALISTO, 5, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   item_ptr->u_el.next_action = NULL;
   if (chain_ptr->u_el.activity == AK_HEADER) 
   { /* segmentierbare Kette */
      if (chain_ptr->u_el.u_xel.u_hdr.segment_anz > 0) 
      { /* Update des Endes des letzten Segments */
         index = chain_ptr->u_el.u_xel.u_hdr.segment_anz % SEGM_PTR_ANZ;
         this_segm_d_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
         for (ii = 1;  
              ii <= (INT)(chain_ptr->u_el.u_xel.u_hdr.segment_anz / 
                          SEGM_PTR_ANZ);
              ii++)
            this_segm_d_ptr = this_segm_d_ptr->next_ref_descr_ptr;
         this_segm_d_ptr->segm_ptrs[index] = item_ptr;
         (this_segm_d_ptr->segm_length[index])++;
      } /* Update des Endes des letzten Segments */
   } /* segmentierbare Kette */
   chain_ptr->u_el.u_xel.u_hdr.last_action->u_el.next_action = item_ptr;
   chain_ptr->u_el.u_xel.u_hdr.last_action                   = item_ptr;
   (chain_ptr->u_el.u_xel.u_hdr.chain_length)++;
   return (GLEAM_OK);
} /* append_item */



/* ============================== insert_item =============================== */
GSTATE insert_item (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                    CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                    CHAIN_ELEM_PTR_TYPE  item_ptr)

{
   REF_DESCR_PTR_TYPE  this_segm_d_ptr;
   INT                 index;
   BOOL                last;
   GSTATE              ret_state = GLEAM_OK;
  
   /* insert_item */
#ifdef CHECKS
   if (chain_ptr  == NULL) 
      return (fatal (LGSW_BALISTO, 6, NO_INFO, NO_INFO, "", NIL_PTR));
   if (behind_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 7, NO_INFO, NO_INFO, "", NIL_PTR));
   if (item_ptr   == NULL) 
      return (fatal (LGSW_BALISTO, 8, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   item_ptr->u_el.next_action   = behind_ptr->u_el.next_action;
   behind_ptr->u_el.next_action = item_ptr;
   last = behind_ptr == chain_ptr->u_el.u_xel.u_hdr.last_action;
   if (last)                                  /* correct last_action pointer */
      chain_ptr->u_el.u_xel.u_hdr.last_action = item_ptr;
   (chain_ptr->u_el.u_xel.u_hdr.chain_length)++;
   if (chain_ptr->u_el.activity == AK_HEADER) 
   { /* segmentierbare Kette */
      if (chain_ptr->u_el.u_xel.u_hdr.segment_anz > 0) 
      { /* segmentierte Kette */
         this_segm_d_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
         if (check_segm_ptr (&this_segm_d_ptr, &index, 
                             chain_ptr->u_el.u_xel.u_hdr.segment_anz, 
                             behind_ptr, &ret_state)) 
         { /* Es wird zwischen zwei Segmenten eingefuegt */
            if (last || (irand(2) == 1)) 
            { /* Item gehoert zum Vorgaengersegment */
               this_segm_d_ptr->segm_ptrs[index] = item_ptr;
            } /* Item gehoert zum Vorgaengersegment */
            else 
            { /* Item gehoert zum Nachfolgersegment */
               if (index == MAX_SEGM_PTR_INDEX) 
               { /* letzter Eintrag im Segment-Descriptor */
                  this_segm_d_ptr = this_segm_d_ptr->next_ref_descr_ptr;
                  index           = 0;
               } /* letzter Eintrag im Segment-Descriptor */
               else 
                  index++;
            } /* Item gehoert zum Nachfolgersegment */
         } /* Es wird zwischen zwei Segmenten eingefuegt */
         else 
         { /* Einfuegen in das Innere eines Segments */
            if (ret_state == GLEAM_OK)
               suche_segm (&this_segm_d_ptr, &index, chain_ptr, behind_ptr);
            else
               return (ret_state);
         } /* Einfuegen in das Innere eines Segments */
         (this_segm_d_ptr->segm_length[index])++;
      } /* segmentierte Kette */
   } /* segmentierbare Kette */
   return (ret_state);
} /* insert_item */



/* ============================ remove_next_item ============================ */
GSTATE remove_next_item (CHAIN_ELEM_PTR_TYPE *item_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                         CHAIN_ELEM_PTR_TYPE  behind_ptr)
{
   REF_DESCR_PTR_TYPE  this_segm_d_ptr;
   INT                 segm_len, 
                       index;
   GSTATE              ret_state = GLEAM_OK;
  
   /* remove_next_item */
#ifdef CHECKS
   *item_ptr = NULL;
   if (chain_ptr  == NULL) 
      return (fatal (LGSW_BALISTO,  9, NO_INFO, NO_INFO, "", NIL_PTR));
   if (behind_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 10, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   *item_ptr = behind_ptr->u_el.next_action;
   if (*item_ptr != NULL) 
   { /* something to remove */
      (chain_ptr->u_el.u_xel.u_hdr.chain_length)--;
      if (chain_ptr->u_el.activity == AK_HEADER) 
      { /* segmentierbare Kette */
         if (chain_ptr->u_el.u_xel.u_hdr.segment_anz > 0) 
         { /* segmentierte Kette */
            this_segm_d_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
            if (check_segm_ptr (&this_segm_d_ptr, &index, 
                                chain_ptr->u_el.u_xel.u_hdr.segment_anz, 
                                *item_ptr, &ret_state)) 
            { /* Das Segmentende wird entfernt */
               this_segm_d_ptr->segm_ptrs[index] = behind_ptr;
            } /* Das Segmentende wird entfernt */
            else 
            { /* Entfernen aus dem Inneren eines Segments */
               if (ret_state == GLEAM_OK)
                  suche_segm (&this_segm_d_ptr, &index, chain_ptr, *item_ptr);
               else
                  return (ret_state);
            } /* Entfernen aus dem Inneren eines Segments */
            segm_len = this_segm_d_ptr->segm_length[index];
            segm_len--;
            if (segm_len == 0) 
               ret_state = remove_segm_entry (this_segm_d_ptr, index, 
                                              chain_ptr);
            else 
               this_segm_d_ptr->segm_length[index] = segm_len;
         } /* segmentierte Kette */
      } /* segmentierbare Kette */
      if (*item_ptr == chain_ptr->u_el.u_xel.u_hdr.last_action)
         chain_ptr->u_el.u_xel.u_hdr.last_action = behind_ptr; 
                                               /* correct last_action pointer */
      behind_ptr->u_el.next_action  = (*item_ptr)->u_el.next_action;
      (*item_ptr)->u_el.next_action = NULL;
   } /* something to remove */
   return (ret_state);
} /* remove_next_item */



/* ================================ copy_act ================================ */
GSTATE copy_act (CHAIN_ELEM_PTR_TYPE *copied_item_ptr, 
                 CHAIN_ELEM_PTR_TYPE  item_ptr)
{
   GSTATE  ret_state;
   INT     geneID, ii;

   /* copy_act */
#ifdef CHECKS
   *copied_item_ptr = NULL;
   if (item_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 11, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   if ((ret_state = new_chain_elem (copied_item_ptr)) == GLEAM_OK)
   { /* Speicher verfuegbar */
#ifdef WITH_DYN_GENES /* with genes of unlimited number of parameters */
      if ((ACTIVITY_ACT < item_ptr->u_el.activity) && (item_ptr->u_el.activity <= last_act))
      { /* valid gene id */
         geneID = item_ptr->u_el.activity;
	 (*copied_item_ptr)->u_el.activity = (SHORT)geneID;
	 if (int_pro_aktion[geneID] > 0) 
	 { /* this gene has integer parameters */
	    if (((*copied_item_ptr)->u_el.u_xel.u_act.i_params = 
		    (INT*)malloc(int_pro_aktion[geneID] * sizeof(INT))) == NULL)
	       ret_state = fatal(LGSW_BALISTO, 60, NO_INFO, NO_INFO, INT_PARAMS_TXT, NOT_ENOUGH_MEM);
	    else
               for (ii = 0;  ii < int_pro_aktion[geneID];  ii++)
		  (*copied_item_ptr)->u_el.u_xel.u_act.i_params[ii] = 
		     item_ptr->u_el.u_xel.u_act.i_params[ii];
	 } /* this gene has integer parameters */
	 if (real_pro_aktion[geneID] > 0) 
	 { /* this gene has real parameters */
	    if (((*copied_item_ptr)->u_el.u_xel.u_act.r_params = 
		    (DOUBLE*)malloc(real_pro_aktion[geneID] * sizeof(DOUBLE))) == NULL)
	       ret_state = fatal(LGSW_BALISTO, 61, NO_INFO, NO_INFO, REAL_PARAMS_TXT, NOT_ENOUGH_MEM);
	    else
               for (ii = 0;  ii < real_pro_aktion[geneID];  ii++)
		  (*copied_item_ptr)->u_el.u_xel.u_act.r_params[ii] = 
		     item_ptr->u_el.u_xel.u_act.r_params[ii];
	 } /* this gene has real parameters */
      } /* valid gene id */
#else /* with genes of fixed max. number of parameters */
      *(*copied_item_ptr) = *item_ptr;
#endif /* with genes of limited and fixed max. number of parameters */
      if ((*copied_item_ptr)->u_el.activity == ACTIVITY_ACT) 
      { /* activity_act */
         if ((*copied_item_ptr)->u_el.u_xel.u_sub.ref_typ  == SUB_AK_REF) 
         { /* incr sub_chain.ref_counter if there is a subchain */
            if ((*copied_item_ptr)->u_el.u_xel.u_sub.sub_chain != NULL)
               ((*copied_item_ptr)->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.
                u_hdr.ref_counter)++;
         } /* incr sub_chain.ref_counter if there is a subchain */
         else 
            if ((*copied_item_ptr)->u_el.u_xel.u_sub.ref_typ >= 0) 
            { /* lokale Unter-AK */
               copy_chain (&((*copied_item_ptr)->u_el.u_xel.u_sub.sub_chain), 
                           item_ptr->u_el.u_xel.u_sub.sub_chain, KEINE_URKETTE, 
                           NO_MEM_CHECK, &ret_state);
               if (ret_state == GLEAM_OK)
                  (*copied_item_ptr)->u_el.u_xel.u_sub.sub_chain->
                     u_el.u_xel.u_hdr.ref_counter = 1;
            } /* lokale Unter-AK */
            else 
               return (fatal (LGSW_BALISTO, 12, 
                              (*copied_item_ptr)->u_el.u_xel.u_sub.ref_typ, 
                              NO_INFO, "", BAD_REF_TYP));
      } /* activity_act */
   } /* Speicher verfuegbar */
   return (ret_state);
} /* copy_act */



/* =============================== copy_item ================================ */
GSTATE copy_item (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                  CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                  CHAIN_ELEM_PTR_TYPE  item_ptr)
{
   CHAIN_ELEM_PTR_TYPE  copied_item_ptr;
   GSTATE               ret_state;
  
   /* copy_item */
#ifdef CHECKS
   if (chain_ptr  == NULL) 
      return (fatal (LGSW_BALISTO, 13, NO_INFO, NO_INFO, "", NIL_PTR));
   if (behind_ptr == NULL)         
      return (fatal (LGSW_BALISTO, 14, NO_INFO, NO_INFO, "", NIL_PTR));
   if (item_ptr   == NULL) 
      return (fatal (LGSW_BALISTO, 15, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   ret_state  = copy_act (&copied_item_ptr, item_ptr);
   ret_state |= insert_item (chain_ptr, behind_ptr, copied_item_ptr);
   return (ret_state);
} /* copy_item */



/* ============================== delete_item =============================== */
GSTATE delete_item (CHAIN_ELEM_PTR_TYPE  item_ptr)
{
   GSTATE  ret_state = GLEAM_OK;

   /* delete_item */
#ifdef CHECKS
   if (item_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 16, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   if (item_ptr->u_el.activity == ACTIVITY_ACT) 
   { /* delete subchain */
      if (item_ptr->u_el.u_xel.u_sub.sub_chain != NULL)
         ret_state = delete_chain (&(item_ptr->u_el.u_xel.u_sub.sub_chain));
   } /* delete subchain */
   ret_state |= free_chain_elem (item_ptr);
   return (ret_state);
} /* delete_item */



/* =============================== insert_seq =============================== */
GSTATE insert_seq (CHAIN_ELEM_PTR_TYPE  target_chain_ptr, 
                   CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                   CHAIN_ELEM_PTR_TYPE  anf_ptr, 
                   CHAIN_ELEM_PTR_TYPE  end_ptr, 
                   INT                  seq_len)
{ /* insert_seq */
#ifdef CHECKS
   if (target_chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 17, NO_INFO, NO_INFO, "", NIL_PTR));
   if (behind_ptr       == NULL) 
      return (fatal (LGSW_BALISTO, 18, NO_INFO, NO_INFO, "", NIL_PTR));
   if (anf_ptr          == NULL)
      return (fatal (LGSW_BALISTO, 19, NO_INFO, NO_INFO, "", NIL_PTR));
   if (end_ptr          == NULL) 
      return (fatal (LGSW_BALISTO, 20, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   end_ptr->u_el.next_action    = behind_ptr->u_el.next_action;
   behind_ptr->u_el.next_action = anf_ptr;
   if (behind_ptr == target_chain_ptr->u_el.u_xel.u_hdr.last_action)
      target_chain_ptr->u_el.u_xel.u_hdr.last_action = end_ptr;
                                            /* correct last_action pointer */
   (target_chain_ptr->u_el.u_xel.u_hdr.chain_length) += seq_len;
   return (GLEAM_OK);
} /* insert_seq */



/* =============================== remove_seq =============================== */
GSTATE remove_seq (CHAIN_ELEM_PTR_TYPE *anf_ptr, 
                   CHAIN_ELEM_PTR_TYPE *end_ptr, 
                   INT                 *number, 
                   CHAIN_ELEM_PTR_TYPE  source_chain_ptr, 
                   CHAIN_ELEM_PTR_TYPE  behind_ptr)
{
   CHAIN_ELEM_PTR_TYPE  pred_end_ptr;
   INT                  ii;
  
   /* remove_seq */
#ifdef CHECKS
   ii       = *number;
   *number  = 0;
   *anf_ptr = NULL;
   *end_ptr = NULL;
   if (source_chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 21, NO_INFO, NO_INFO, "", NIL_PTR));
   if (behind_ptr       == NULL) 
      return (fatal (LGSW_BALISTO, 22, NO_INFO, NO_INFO, "", NIL_PTR));
   *number  = ii;
#endif
   *anf_ptr = behind_ptr->u_el.next_action;
   if ((*number > 0) && (*anf_ptr != NULL)) 
   { /* more than 0 items to be removed */
      if (*end_ptr == NULL) 
      { /* search for end of sequenz */
         pred_end_ptr = behind_ptr;
         *end_ptr     = *anf_ptr;
         ii = 1;
         while ((ii < *number) && (*end_ptr != NULL)) 
         { /* search last item to be removed */
            (ii)++;
            pred_end_ptr = *end_ptr;
            *end_ptr     = (*end_ptr)->u_el.next_action;
         } /* search last item to be removed */
         if (*end_ptr == NULL) 
         { /* not so many items there */
            *end_ptr = pred_end_ptr;
            *number  = ii-1;
         } /* not so many items there */
      } /* search for end of sequenz */
      if ((*end_ptr)->u_el.next_action == NULL)             /* last_removed ? */
         source_chain_ptr->u_el.u_xel.u_hdr.last_action = behind_ptr; 
                                                                /* is end of chain */
      behind_ptr->u_el.next_action = (*end_ptr)->u_el.next_action;
      (*end_ptr)->u_el.next_action = NULL;
      (source_chain_ptr->u_el.u_xel.u_hdr.chain_length) -= *number;
   } /* more than 0 items to be removed */
   else 
   { /* nothing to be removed */
      *number  = 0;
      *anf_ptr = NULL;
      *end_ptr = NULL;
   } /* nothing to be removed */
   return (GLEAM_OK);
} /* remove_seq */



/* ================================ copy_seq ================================ */
GSTATE copy_seq (CHAIN_ELEM_PTR_TYPE *start_ptr, 
                 CHAIN_ELEM_PTR_TYPE *end_ptr, 
                 CHAIN_ELEM_PTR_TYPE  first_ptr, 
                 INT                  number)
{
   CHAIN_ELEM_PTR_TYPE  new_ptr, 
                        pred_ptr;
   INT                  ii;
   GSTATE               ret_state;
  
   /* copy_seq */
   *end_ptr   = NULL;
#ifdef CHECKS
   *start_ptr = NULL;
   if (first_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 23, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   ret_state = copy_act (start_ptr, first_ptr);
   pred_ptr  = *start_ptr;
   for (ii = 2;  (ii <= number) && (ret_state == GLEAM_OK);  ii++) 
   { /* kopieren */
      first_ptr = first_ptr->u_el.next_action;
      ret_state = copy_act (&new_ptr, first_ptr);
      pred_ptr->u_el.next_action = new_ptr;
      pred_ptr                   = new_ptr;
   } /* kopieren */
   if (ret_state == GLEAM_OK)
   { /* nix fatal */
      pred_ptr->u_el.next_action = NULL;
      *end_ptr                   = pred_ptr;
   } /* nix fatal */
   return (ret_state);
} /* copy_seq */



/* =============================== delete_seq =============================== */
GSTATE delete_seq (CHAIN_ELEM_PTR_TYPE  start_ptr, 
                   CHAIN_ELEM_PTR_TYPE  end_ptr, 
                   INT                  seq_len)
{
   CHAIN_ELEM_PTR_TYPE  elem_ptr;
   GSTATE               ret_state = GLEAM_OK;

   /* delete_seq */
#ifdef CHECKS
   if (start_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 24, NO_INFO, NO_INFO, "", NIL_PTR));
   if (end_ptr   == NULL) 
      return (fatal (LGSW_BALISTO, 25, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   elem_ptr = start_ptr;
   while (elem_ptr != NULL) 
   { /* all along the sequence */
      if (elem_ptr->u_el.activity == ACTIVITY_ACT) 
      { /* delete subchain */
         if (elem_ptr->u_el.u_xel.u_sub.sub_chain != NULL)
            ret_state |= delete_chain (&(elem_ptr->u_el.u_xel.u_sub.sub_chain));
      } /* delete subchain */
      elem_ptr = elem_ptr->u_el.next_action;
   } /* all along the sequence */
   ret_state |= free_seq (start_ptr, end_ptr, seq_len);
   return (ret_state);
} /* delete_seq */



/* =============================== copy_chain =============================== */
BOOL copy_chain (CHAIN_ELEM_PTR_TYPE *new_chain_ptr, 
                 CHAIN_ELEM_PTR_TYPE  old_chain_ptr, 
                 BOOL                 urkette, 
                 BOOL                 mem_check,
                 GSTATE              *ret_status)
{
   BOOL    r_copy_chain;
   INT     item_anz, segm_anz;
   BOOL    mem_ok;
  
   /* copy_chain */
#ifdef CHECKS
   *new_chain_ptr = NULL;
   if (old_chain_ptr == NULL) 
      *ret_status |= fatal (LGSW_BALISTO, 26, NO_INFO, NO_INFO, "", NIL_PTR);
#endif
   if (*ret_status == GLEAM_OK)
   { /* nix fatal */
      if (mem_check) 
      { /* mem_check */
         if (old_chain_ptr->u_el.activity == AK_HEADER) 
         { /* AK */
            segm_anz = old_chain_ptr->u_el.u_xel.u_hdr.segment_anz;
            item_anz = old_chain_ptr->u_el.u_xel.u_hdr.chain_length;
	    mem_ok = enough_free_mem (old_chain_ptr->u_el.activity, item_anz, segm_anz);
         } /* AK */
         else 
            *ret_status = fatal (LGSW_BALISTO, 27, old_chain_ptr->u_el.activity,
                                 NO_INFO, "", CHAIN_NOT_COPIED);
      } /* mem_check */
      else 
         mem_ok = TRUE;

      if (mem_ok && (*ret_status == GLEAM_OK)) 
      { /* enough mem und nix fatal */
         *ret_status = init_chain (new_chain_ptr, old_chain_ptr->u_el.activity);

         if ((old_chain_ptr->u_el.u_xel.u_hdr.chain_length > 0) && 
             (*ret_status == GLEAM_OK))
         { /* Kette nicht leer und nix fatal */
            *ret_status = copy_seq (&((*new_chain_ptr)->u_el.next_action), 
                                    &((*new_chain_ptr)->u_el.u_xel.u_hdr.last_action), 
                                    old_chain_ptr->u_el.next_action, 
                                    old_chain_ptr->u_el.u_xel.u_hdr.chain_length);
            (*new_chain_ptr)->u_el.u_xel.u_hdr.chain_length = 
               old_chain_ptr->u_el.u_xel.u_hdr.chain_length;
            if (urkette)                                /* unmutierte_urkette */
               (*new_chain_ptr)->u_el.u_xel.u_hdr.chain_status |= 
                  UNMUTIERTE_URKETTE;
            else 
               (*new_chain_ptr)->u_el.u_xel.u_hdr.chain_status = 
                  old_chain_ptr->u_el.u_xel.u_hdr.chain_status & 
                  ~BASIS_AK & ~SIMULATED & ~RANDOMLY_GENERATED;
            if (old_chain_ptr->u_el.activity == AK_HEADER)
               *ret_status |= copy_segm_structure (*new_chain_ptr, 
                                                   old_chain_ptr);
            r_copy_chain = *ret_status == GLEAM_OK;
         } /* Kette nicht leer und nix fatal */
         else
            r_copy_chain = (old_chain_ptr->u_el.u_xel.u_hdr.chain_length == 0) 
                           && (*ret_status == GLEAM_OK);
      } /* enough mem und nix fatal */
      else 
         r_copy_chain = FALSE;                 /* not enough mem or bad state */
   } /* nix fatal */
   if ((*ret_status != GLEAM_OK) || !r_copy_chain)
   { /* Nicht ok */
      r_copy_chain   = FALSE;
      *new_chain_ptr = NULL;
   } /* Nicht ok */
   return (r_copy_chain);
} /* copy_chain */



/* ============================== delete_chain ============================== */
GSTATE delete_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr)
{
   GSTATE  ret_state = GLEAM_OK;
  
   /* delete_chain */
#ifdef CHECKS
   if (*chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 28, NO_INFO, NO_INFO, "", NIL_PTR));
   if (((*chain_ptr)->u_el.activity < VK_HEADER) || 
       (AK_HEADER < (*chain_ptr)->u_el.activity)) 
      return (fatal (LGSW_BALISTO, 29, (*chain_ptr)->u_el.activity, 
                     NO_INFO, "", BAD_HDR_CODE));
#endif
   ((*chain_ptr)->u_el.u_xel.u_hdr.ref_counter)--;
   if ((*chain_ptr)->u_el.u_xel.u_hdr.ref_counter <= 0) 
   { /* Kette loeschen */
      if ((*chain_ptr)->u_el.u_xel.u_hdr.chain_length > 0) 
      { /* delete all chain elements */
         ret_state = delete_seq ((*chain_ptr)->u_el.next_action, 
                                 (*chain_ptr)->u_el.u_xel.u_hdr.last_action, 
                                 (*chain_ptr)->u_el.u_xel.u_hdr.chain_length);
      } /* delete all chain elements */
      if ((*chain_ptr)->u_el.activity == AK_HEADER) 
      { /* AK-Header */
         del_ref_descr_chain ((*chain_ptr)->u_el.u_xel.u_hdr.segm_descr_ptr);
      } /* AK-Header */
      if ((*chain_ptr)->u_el.u_xel.u_hdr.header_par_ptr != NULL)
         ret_state |= freeHdrParElem((*chain_ptr)->u_el.u_xel.u_hdr.header_par_ptr);
      ret_state |= free_chain_elem (*chain_ptr);
   } /* Kette loeschen */
   *chain_ptr = NULL;
   return (ret_state);
} /* delete_chain */



/* ============================ append_sub_chain ============================ */
GSTATE append_sub_chain (CHAIN_ELEM_PTR_TYPE  item_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  old_chain_ptr;
   GSTATE               ret_state = GLEAM_OK;

   /* append_sub_chain */
#ifdef CHECKS
   if (chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 30, NO_INFO, NO_INFO, "", NIL_PTR));
   if (item_ptr  == NULL) 
      return (fatal (LGSW_BALISTO, 31, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   if (item_ptr->u_el.activity == ACTIVITY_ACT) 
   { /* append subchain */
      old_chain_ptr = item_ptr->u_el.u_xel.u_sub.sub_chain;
      if (old_chain_ptr != NULL) 
         ret_state = delete_chain (&old_chain_ptr);          /* try to delete */
      (chain_ptr->u_el.u_xel.u_hdr.ref_counter)++;
      item_ptr->u_el.u_xel.u_sub.sub_chain = chain_ptr;
   } /* append subchain */
   else 
      return (fatal (LGSW_BALISTO, 32, item_ptr->u_el.activity, 
                     NO_INFO, "", NO_ACTIVITY_ACT));
   return (ret_state);
} /* append_sub_chain */



/* ============================ remove_sub_chain ============================ */
GSTATE remove_sub_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                         CHAIN_ELEM_PTR_TYPE  item_ptr)

{ /* remove_sub_chain */
   *chain_ptr = NULL;
#ifdef CHECKS
   if (item_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 33, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   if (item_ptr->u_el.activity == ACTIVITY_ACT) 
   { /* try to remove subchain */
      *chain_ptr = item_ptr->u_el.u_xel.u_sub.sub_chain;
      if (*chain_ptr != NULL) 
      { /* remove subchain */
         ((*chain_ptr)->u_el.u_xel.u_hdr.ref_counter)--;
         item_ptr->u_el.u_xel.u_sub.sub_chain = NULL;
      } /* remove subchain */
      return (GLEAM_OK);
   } /* try to remove subchain */
   else 
      return (fatal (LGSW_BALISTO, 34, item_ptr->u_el.activity, 
                     NO_INFO, "", NO_ACTIVITY_ACT));
} /* remove_sub_chain */



/* =============================== inc_segm_nr ============================== */
GSTATE inc_segm_nr (REF_DESCR_PTR_TYPE  *segm_descr_ptr, 
                    INT                 *index)
{ /* inc_segm_nr */
#ifdef CHECKS
   if (*segm_descr_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 35, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   (*index)++;
   if (*index == SEGM_PTR_ANZ) 
   { /* weiter im naechsten Segmentdescriptor */
      *segm_descr_ptr = (*segm_descr_ptr)->next_ref_descr_ptr;
      *index          = 0;
   } /* weiter im naechsten Segmentdescriptor */
   return (GLEAM_OK);
} /* inc_segm_nr */


  
/* ============================= check_segm_ptr ============================= */
BOOL check_segm_ptr (REF_DESCR_PTR_TYPE  *segm_descr_ptr, 
                     INT                 *index, 
                     INT                  segm_anz, 
                     CHAIN_ELEM_PTR_TYPE  item_ptr,
                     GSTATE              *ret_status)
{ 
   INT     ii;
   BOOL    found = FALSE;
  
   /* check_segm_ptr */
#ifdef CHECKS
   if (*segm_descr_ptr == NULL) 
      *ret_status |= fatal (LGSW_BALISTO, 36, NO_INFO, NO_INFO, "", NIL_PTR);
   if (item_ptr == NULL) 
      *ret_status |= fatal (LGSW_BALISTO, 37, NO_INFO, NO_INFO, "", NIL_PTR);
#endif
   if (*ret_status == GLEAM_OK)
   { /* nix fatal */
      *index = 1;  /* Der 1.Eintrag zeigt aus Konsistensgruenden auf den Kopf */
      ii     = 0;
      do 
      {
         found = (*segm_descr_ptr)->segm_ptrs[*index] == item_ptr;
         if (!found) 
            *ret_status |= inc_segm_nr (segm_descr_ptr, index);
         ii++;
      } while (!(found || (ii == segm_anz)));
   } /* nix fatal */
   return (found);
} /* check_segm_ptr */



/* =========================== ermittle_abschnitt =========================== */
BOOL ermittle_abschnitt (CHAIN_ELEM_PTR_TYPE *pred_ptr, 
                         CHAIN_ELEM_PTR_TYPE *last_ptr, 
                         INT                 *len, 
                         INT                 *index, 
                         REF_DESCR_PTR_TYPE  *sd_ptr, 
                         CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                         INT                  number,
                         GSTATE              *ret_status)
{
   INT  ii, sd_index;

   /* ermittle_abschnitt */
#ifdef CHECKS
   if (chain_ptr == NULL) 
      *ret_status |= fatal (LGSW_BALISTO, 38, NO_INFO, NO_INFO, "", NIL_PTR);
   if (number < 1)        
      *ret_status |= fatal (LGSW_BALISTO, 39, number, NO_INFO, "", NO_ABSCHN);
#endif
   if (*ret_status != GLEAM_OK)
      return (FALSE);
   if (number <= (INT)chain_ptr->u_el.u_xel.u_hdr.segment_anz) 
   { /* es gibt dieses Segment */
      *sd_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
      sd_index =  (number-1) / SEGM_PTR_ANZ;                /* Ermittle       */
      *index   =  (number-1) % SEGM_PTR_ANZ;                /* Vorgaenger-    */
      for (ii = 1;  ii <= sd_index;  ii++)                  /* Segment wegen  */
         *sd_ptr = (*sd_ptr)->next_ref_descr_ptr;           /* Ermittlung des */
      *pred_ptr = (*sd_ptr)->segm_ptrs[*index];             /* "pred_ptr"     */
      inc_segm_nr (sd_ptr, index);
      *last_ptr = (*sd_ptr)->segm_ptrs[*index];
      *len      = (*sd_ptr)->segm_length[*index];
      return (TRUE);
   } /* es gibt dieses Segment */
   else 
      return (FALSE);                         /* es gibt dieses Segment nicht */
} /* ermittle_abschnitt */



/* =========================== insert_segm_entry ============================ */
GSTATE insert_segm_entry (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                          REF_DESCR_PTR_TYPE   segm_d_ptr, 
                          INT                  index, 
                          CHAIN_ELEM_PTR_TYPE  last_ptr, 
                          INT                  seq_len, 
                          BOOL                 wohin)
{
   REF_DESCR_PTR_TYPE  old_sd_ptr, new_sd_ptr;
   INT                 new_index;
   BOOL                do_copy;
   GSTATE              ret_state = GLEAM_OK;

   /* insert_segm_entry */
#ifdef CHECKS
   if (chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 40, NO_INFO, NO_INFO, "", NIL_PTR));
   if (segm_d_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 41, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   if (wohin) 
   { /* HINTER den angegebenen Eintrag */
      do_copy = chain_ptr->u_el.u_xel.u_hdr.last_action != 
         segm_d_ptr->segm_ptrs[index];
      old_sd_ptr = segm_d_ptr;
      ret_state  = inc_segm_nr (&segm_d_ptr, &index);
      if (segm_d_ptr == NULL) 
      { /* ein neuer Descriptor */
         ret_state |= new_ref_descr (&segm_d_ptr);
         old_sd_ptr->next_ref_descr_ptr = segm_d_ptr;
      } /* ein neuere Descriptor */
      do_copy = do_copy && (ret_state == GLEAM_OK);
   } /* HINTER den angegebenen Eintrag */
   else 
      do_copy = TRUE;                          /* VOR den angegebenen Eintrag */
   if (do_copy) 
   { /* Platz ab "segm_d_ptr"/"index" einschliesslich schaffen */
      new_sd_ptr = segm_d_ptr;
      new_index  = index;
      ret_state  = inc_segm_nr (&new_sd_ptr, &new_index);
      if (new_sd_ptr == NULL) 
      { /* ein neuere Descriptor */
         if ((ret_state |= new_ref_descr (&new_sd_ptr)) == GLEAM_OK)
            segm_d_ptr->next_ref_descr_ptr = new_sd_ptr;
      } /* ein neuere Descriptor */
      ret_state |= umkopieren (new_sd_ptr, new_index, 
                               segm_d_ptr->segm_ptrs[index], 
                               segm_d_ptr->segm_length[index],
                               chain_ptr->u_el.u_xel.u_hdr.segment_anz + 1);
   } /* Platz ab "segm_d_ptr"/"index" einschliesslich schaffen */
   if (ret_state != GLEAM_OK)
      return (ret_state);
   segm_d_ptr->segm_ptrs[index]   = last_ptr;
   segm_d_ptr->segm_length[index] = seq_len;
   (chain_ptr->u_el.u_xel.u_hdr.segment_anz)++;
   return (ret_state);
} /* insert_segm_entry */



/* ============================ remove_segm_entry =========================== */
GSTATE remove_segm_entry (REF_DESCR_PTR_TYPE   segm_d_ptr, 
                          INT                  index, 
                          CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   INT                 next_index;
   REF_DESCR_PTR_TYPE  next_sd_ptr;
   BOOL                weiter = FALSE;
  
   /* remove_segm_entry */
#ifdef CHECKS
   if (segm_d_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 42, NO_INFO, NO_INFO, "", NIL_PTR));
   if (chain_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 43, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   next_index  = index;
   next_sd_ptr = segm_d_ptr;
   inc_segm_nr (&next_sd_ptr, &next_index);  /*liefert GLEAM_OK wg.Vorpruefung*/
   if (next_sd_ptr != NULL) 
      weiter = next_sd_ptr->segm_ptrs[next_index] != NULL;
   while (weiter) 
   {
      segm_d_ptr->segm_ptrs  [index] = next_sd_ptr->segm_ptrs  [next_index];
      segm_d_ptr->segm_length[index] = next_sd_ptr->segm_length[next_index];
      index      = next_index;
      segm_d_ptr = next_sd_ptr;
      inc_segm_nr (&next_sd_ptr, &next_index); /*liefert GLEAM_OK wg.Vorchecks*/
      weiter     = next_sd_ptr != NULL;
      if (weiter) 
         weiter = next_sd_ptr->segm_ptrs[next_index] != NULL;
   }
   segm_d_ptr->segm_ptrs[index] = NULL;                /* del last segm entry */
   (chain_ptr->u_el.u_xel.u_hdr.segment_anz)--;
   return (GLEAM_OK);
} /* remove_segm_entry */



/* ========================== copy_segm_structure =========================== */
GSTATE copy_segm_structure (CHAIN_ELEM_PTR_TYPE  new_ch_ptr, 
                            CHAIN_ELEM_PTR_TYPE  old_ch_ptr)
{
   CHAIN_ELEM_PTR_TYPE  segm_end_ptr;
   REF_DESCR_PTR_TYPE   old_sd_ptr, 
                        new_sd_ptr;
   INT                  ii,       index, 
                        segm_len, segm_anz;
   GSTATE               ret_state = GLEAM_OK;
  
   /* copy_segm_structure */
#ifdef CHECKS
   if (old_ch_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 44, NO_INFO, NO_INFO, "", NIL_PTR));
   if (new_ch_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 45, NO_INFO, NO_INFO, "", NIL_PTR));
   if (new_ch_ptr->u_el.activity != AK_HEADER) 
      return (fatal (LGSW_BALISTO, 46, new_ch_ptr->u_el.activity, 
                     NO_INFO, "", BAD_HDR_CODE));
   if (old_ch_ptr->u_el.activity != AK_HEADER) 
      return (fatal (LGSW_BALISTO, 47, old_ch_ptr->u_el.activity, 
                     NO_INFO, "", BAD_HDR_CODE));
   if (new_ch_ptr->u_el.u_xel.u_hdr.chain_length != 
       old_ch_ptr->u_el.u_xel.u_hdr.chain_length) 
      return (fatal (LGSW_BALISTO, 48, new_ch_ptr->u_el.u_xel.u_hdr.chain_length,
                     old_ch_ptr->u_el.u_xel.u_hdr.chain_length, "", 
                     UNGLEICHE_CH_LEN));
#endif
   segm_anz = old_ch_ptr->u_el.u_xel.u_hdr.segment_anz;
   if (segm_anz > 0) 
   { /* segmentierte Kette */
      new_ch_ptr->u_el.u_xel.u_hdr.segment_anz = segm_anz;
      old_sd_ptr = old_ch_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
      if (new_ref_descr (&new_sd_ptr) != GLEAM_OK)
         return (GLEAM_FATAL);
      new_ch_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = new_sd_ptr;
      segm_end_ptr = new_ch_ptr;
      new_sd_ptr->segm_ptrs[0] = new_ch_ptr;
      index = 0;
      for (ii = 1;  (ii <= segm_anz) && (ret_state == GLEAM_OK);  ii++) 
      { /* Segment einrichten */
         inc_segm_nr (&old_sd_ptr, &index);
         if (index == 0) 
         { /* new segment descriptor */
            ret_state  = new_ref_descr (&(new_sd_ptr->next_ref_descr_ptr));
            new_sd_ptr = new_sd_ptr->next_ref_descr_ptr;
         } /* new segment descriptor */
         segm_len = old_sd_ptr->segm_length[index];
         if (count_out_item(&segm_end_ptr, segm_end_ptr, segm_len, &ret_state)) 
         { /* es gibt diesen Abschnitt */
            new_sd_ptr->segm_ptrs[index]   = segm_end_ptr;
            new_sd_ptr->segm_length[index] = segm_len;
         } /* es gibt diesen Abschnitt */
         else 
            return (fatal (LGSW_BALISTO, 49, NO_INFO, NO_INFO, "", 
                    NO_SEGM_END));
      } /* Segment einrichten */
      new_sd_ptr->next_ref_descr_ptr = NULL;
   } /* segmentierte Kette */
   return (ret_state);
} /* copy_segm_structure */



/* =============================== suche_segm =============================== */
static VOID suche_segm (REF_DESCR_PTR_TYPE  *segm_d_ptr,
                        INT                 *index,
                        CHAIN_ELEM_PTR_TYPE  chain_ptr,
                        CHAIN_ELEM_PTR_TYPE  such_ptr)  
/*----------------------------------------------------------------------------*/
/*  Ermittelt in "segm_d_ptr" und "index" den Segmentdescriptoreintrag des    */
/*  Segments, in dem das Element steht, auf das der "such_ptr" zeigt.         */
/*  Dieses muss in der Kette "chain_ptr" sein!                                */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  akt_ptr;
   INT                  sum, akt_nr;
  
   /* suche_segm */
   akt_nr  = 0;
   akt_ptr = chain_ptr;
   while (akt_ptr != such_ptr) 
   { /* Ermittle Ordnungszahl des Behind-Items */
      akt_nr++;
      akt_ptr = akt_ptr->u_el.next_action;
   } /* Ermittle Ordnungszahl des Behind-Items */
   *segm_d_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
   *index = 1;
   sum = (*segm_d_ptr)->segm_length[*index];
   while (sum < akt_nr) 
   { /* Suche Segment */
      inc_segm_nr (segm_d_ptr, index);
      sum += (*segm_d_ptr)->segm_length[*index];
   } /* Suche Segment */
} /* suche_segm */



/* ========================== del_ref_descr_chain =========================== */
static VOID del_ref_descr_chain (REF_DESCR_PTR_TYPE  ref_d_ptr)
/*----------------------------------------------------------------------------*/
/*  Loescht alle Ref-Descriptor-Elemente ab "ref_d_ptr" inklusive.            */
/*----------------------------------------------------------------------------*/
{
   REF_DESCR_PTR_TYPE  next_ref_d_ptr;
  
   /* del_ref_descr_chain */
   while (ref_d_ptr != NULL) 
   { /* Segmentdescriptoren freigeben */
      next_ref_d_ptr = ref_d_ptr->next_ref_descr_ptr;
      free_ref_descr (ref_d_ptr);
      ref_d_ptr      = next_ref_d_ptr;
   } /* Segmentdescriptoren freigeben */
} /* del_ref_descr_chain */



/* ==============================  umkopieren =============================== */
static GSTATE umkopieren (REF_DESCR_PTR_TYPE   segm_d_ptr, 
                          INT                  index, 
                          CHAIN_ELEM_PTR_TYPE  old_s_ptr, 
                          INT                  old_s_len,
                          INT                  s_anz_plus)
/*----------------------------------------------------------------------------*/
/*  Sucht den ersten freien Platz im durch "segm_d_ptr" bezeichneten Descrip- */
/*  tor-Record und kopiert alle Eintraege ab "index" um eine Position weiter. */
/*  Damit wird der "indext"-te Eintrag quasi frei fuer "old_s_ptr" und "old_  */
/*  s_len". Wenn im durch "segm_d_ptr" bezeichneten Segmentdescriptor kein    */
/*  freier Platz ist, sucht die Routine rekursiv die Deskriptorsequenz ab.    */
/*  Wenn der letzte Descriptor voll ist, wird ein neuer erzeugt und an die    */
/*  Sequenz gehaengt. Der gefundene Index wird ueberprueft (fataler Fehler).  */
/*----------------------------------------------------------------------------*/
{
   BOOL    weiter;
   INT     frei_ii, ii;
   GSTATE  ret_state = GLEAM_OK;

   /* umkopieren */
#ifdef CHECKS
   if (segm_d_ptr == NULL) 
      return (fatal (LGSW_BALISTO, 50, NO_INFO, NO_INFO, "", NIL_PTR));
   if (old_s_ptr  == NULL) 
      return (fatal (LGSW_BALISTO, 51, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   weiter  = TRUE;
   frei_ii = index;
   while ((segm_d_ptr->segm_ptrs[frei_ii] != NULL) && weiter) 
   { /* Suche 1. freien Platz */
      frei_ii++;
      if (frei_ii == SEGM_PTR_ANZ) 
      { /* Descriptor ist voll */
         frei_ii--;
         if (segm_d_ptr->next_ref_descr_ptr == NULL)
            new_ref_descr (&(segm_d_ptr->next_ref_descr_ptr));
         ret_state = umkopieren (segm_d_ptr->next_ref_descr_ptr, 0, 
                                 segm_d_ptr->segm_ptrs[frei_ii], 
                                 segm_d_ptr->segm_length[frei_ii], 
                                 s_anz_plus);
         weiter = FALSE;
      } /* Descriptor ist voll */
   } /* Suche 1. freien Platz */
#ifdef CHECKS
   if (weiter && (frei_ii != (s_anz_plus % SEGM_PTR_ANZ)))
      return (fatal (LGSW_BALISTO, 52, NO_INFO, NO_INFO, "", BAD_ABSCHNITT));
#endif
   for (ii = frei_ii;  ii > index;  ii--) 
   { /* umkopieren */
      segm_d_ptr->segm_ptrs[ii]   = segm_d_ptr->segm_ptrs[ii-1];
      segm_d_ptr->segm_length[ii] = segm_d_ptr->segm_length[ii-1];
   } /* umkopieren */
   segm_d_ptr->segm_ptrs[index]   = old_s_ptr;
   segm_d_ptr->segm_length[index] = old_s_len;
   return (ret_state);
} /* umkopieren */



/* ================================ teste_ak ================================ */
BOOL teste_ak (CHAIN_ELEM_PTR_TYPE  ch_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  elem_ptr, end_ptr;
   INT                  segm_soll_anz, segm_anz, 
                        segm_soll_len, segm_len, 
                        ak_soll_len,   ak_len, 
                        segm_elem_sum, sd_idx, znr;
   STR_TYPE             buf;
   BOOL                 erg           = TRUE,
                        segm_entry_da = TRUE;

   /* teste_ak */
   if (ch_ptr == NULL)
   { /* AK leer */
      write_buf (1, AK_NULL_TXT);
      return (FALSE);
   } /* AK leer */
   if (ch_ptr->u_el.activity != AK_HEADER)
   { /* keine AK */
      sprintf (buf, NO_AK_TXT, ch_ptr->u_el.activity);
      write_buf (1, buf);
      return (FALSE);
   } /* keine AK */
   ak_soll_len   = ch_ptr->u_el.u_xel.u_hdr.chain_length;
   segm_soll_anz = ch_ptr->u_el.u_xel.u_hdr.segment_anz;
   sd_ptr        = ch_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
   ak_len        = 0;
   znr           = 1;
   elem_ptr      = ch_ptr;
   while (elem_ptr->u_el.next_action != NULL)
   { /* Kettenende suchen */
      ak_len++;
      elem_ptr = elem_ptr->u_el.next_action;
   } /* Kettenende suchen */
   if (ak_soll_len == ak_len)
   { /* Ak-Laenge stimmt */
      if (elem_ptr != ch_ptr->u_el.u_xel.u_hdr.last_action) {
         write_buf (znr++, BAD_LASTACT_TXT);
         erg = FALSE;
      }
   } /* Ak-Laenge stimmt */
   else
   { /* Ak-Laenge stimmt nicht */
      sprintf (buf, BAD_AKLEN_TXT, ak_soll_len, ak_len);
      write_buf (znr++, buf);
      erg = FALSE;
   } /* Ak-Laenge stimmt nicht */
   if (segm_soll_anz > 0)
   { /* AK soll segmentiert sein */
      if (sd_ptr == NULL)
      { /* AK hat keinen Segmentdescriptor */
         write_buf (znr, NO_DESCR_TXT);
         erg = FALSE;
      } /* AK hat keinen Segmentdescriptor */
      else
      { /* Segmentstruktur pruefen */
         segm_elem_sum = 0;
         segm_anz      = 0;
         sd_idx        = 0;
         do
         { /* pruefe alle vorhandenen Segmente */
            elem_ptr      = sd_ptr->segm_ptrs[sd_idx];
            inc_segm_nr (&sd_ptr, &sd_idx);
            if (sd_ptr == NULL)
            { /* Keine Segmente mehr beschrieben */
               sprintf (buf, NO_MORE_SEGM, segm_soll_anz, segm_anz);
               write_buf (znr++, buf);
               erg           = FALSE;
               segm_entry_da = FALSE;
            } /* Keine Segmente mehr beschrieben */
            else
            { /* Segmenteintrag vorhanden */
               end_ptr       = sd_ptr->segm_ptrs[sd_idx];
               segm_soll_len = sd_ptr->segm_length[sd_idx];
               segm_anz++;
               if ((elem_ptr != NULL) && (end_ptr != NULL) && 
                   (segm_soll_len > 0))
               { /* Das Segment scheint zu existieren */
                  segm_len = 0;
                  while ((elem_ptr != NULL) && (elem_ptr != end_ptr))
                  { /* Segmentende suchen */
                     segm_len++;
                     elem_ptr = elem_ptr->u_el.next_action;
                  } /* Segmentende suchen */
                  if (elem_ptr == NULL)
                  { /* Segmentende nicht gefunden */
                     sprintf (buf, NO_SEGM_ENDE, segm_anz, segm_soll_len, 
                              segm_len-1);
                     write_buf (znr++, buf);
                     erg = FALSE;
                  } /* Segmentende nicht gefunden */
                  else
                  { /* Segmentende gefunden */
                     segm_elem_sum += segm_len;
                     if (segm_len != segm_soll_len)
                     { /* Segmentlaenge stimmt nicht */
                        sprintf (buf, SEGM_LEN_ERR, segm_anz, segm_soll_len, 
                                 segm_len);
                        write_buf (znr++, buf);
                        erg = FALSE;
                     } /* Segmentlaenge stimmt nicht */
                  } /* Segmentende gefunden */
               } /* Das Segment scheint zu existieren */
               else
               { /* Das Segment existiert nicht */
                  sprintf (buf, SEGM_NIX_DA, segm_anz, segm_soll_len);
                  if (elem_ptr != NULL)
                     strcat (buf, PRED_PTR_OK);
                  else
                     strcat (buf, PRED_PTR_NULL);
                  if (end_ptr != NULL)
                     strcat (buf, END_PTR_OK);
                  else
                     strcat (buf, END_PTR_NULL);
                  write_buf (znr++, buf);
                  erg = FALSE;
               } /* Das Segment existiert nicht */
            } /* Segmenteintrag vorhanden */
         }/* pruefe alle vorhandenen Segmente */
         while ((segm_anz < segm_soll_anz) && segm_entry_da);
         if (segm_entry_da)
            if (end_ptr != ch_ptr->u_el.u_xel.u_hdr.last_action)
            { /* AK-Ende nicht identisch mit dem Ende des letzten Segments */
               write_buf (znr++, NIX_GUT_ENDE);
               erg = FALSE;
            } /* AK-Ende nicht identisch mit dem Ende des letzten Segments */
         if (ak_len != segm_elem_sum)
         { /* AK-Laenge u. Elementanzahl der Segmente stimmen nicht ueberein! */
            sprintf (buf, LEN_NE_SUM, ak_len, segm_elem_sum);
            write_buf (znr++, buf);
            erg = FALSE;
         } /* AK-Laenge u. Elementanzahl der Segmente stimmen nicht ueberein! */
      } /* Segmentstruktur pruefen */
   } /* AK soll segmentiert sein */
   else
   { /* AK soll unsegmentiert sein */
      if (sd_ptr == NULL)
         write_buf (znr, AK_IST_UNSEGM);
      else
      { /* Unsegmentierte AK mit Segmentdescriptor */
         write_buf (znr, UNSEGM_REC);
         erg= FALSE;
      } /* Unsegmentierte AK mit Segmentdescriptor */
   } /* AK soll unsegmentiert sein */
   return (erg);
} /* teste_ak */
