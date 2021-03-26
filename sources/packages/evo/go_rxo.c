/*******************************************************************************
GLEAM/AE                                                                  V1.0.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: EVOlution
                        Module: Genetische Operatoren: Rekombination und X-Over
 
Package: evo                    File: go_rxo.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
 
Der Modul enthaelt alle genetischen Operatoren, die die Mechanismen der sexuel-
len Reproduktion nachbilden, also zwei Offspring basierend auf zwei Elternket-
ten erzeugen. Konkret sind dies Cross-Over- und Rekombinationsoperatoren. Die 
zugehoerigen Funktionen fuehren ihre Operation wahrscheinlichkeitsgesteuert aus 
und liefern TRUE, sofern sie veraenderte Ketten abliefern. Bei Auftreten eines 
Fehlers nimmt "ret_state" den Wert "GLEAM_FATAL" an. Einzige Ausnahme ist der 
Fehler zu vieler Abschnitte in "rekombination" mit dem Ergebnis "GLEAM_ERROR".
Das Bit "unmutierte_urkette" im "chain_status" wird nicht gepflegt.


Nachstehende neue XO/Reko-Varianten sind auf kombinatorische Aufgabestellungen 
bei fester AK-Laenge zugeschnitten. Sie nutzen die Segmentstruktur eines Elters
zur Ermittlung der Sequenzlaengen und Sequenzanzahl ("sequLen" und "sequAnz") 
und segmentieren die Kinder entsprechend der Segmentierung der Eltern. 

OX_XO und OX_RECO:
Beide Verfahren setzen ein Elter mit mindestens 3 Segmenten voraus. Die Sequenz-
anzahl liegt zwischen drei und der Segmentanzahl. XO ist eine 3-Sequenzvariante,
waehrend Reko die n-Sequenzvariante darstellt mit n>3.
Das "order crossover" macht die Reihenfolge beider Elternketten zur Grundlage 
der Reihenfolge beider Ergebnisketten. Es garantiert nicht, dass aus reihen-
folge-zulaessigen Eltern auch reihenfolge-zulaessige Kinder entstehen, macht es
aber wahrscheinlicher als die traditionellen XO- oder Reko-Operatoren. 
Algorithmus:
1. Kopiere die Aktionen der 2., 4., 6., ... Sequenz des 1.Elter in der vorhande-
   nen Reihefolge positionstreu in das Kind.
2. Fuelle die Luecken mit den noch nicht vorhandenen Aktionen des 2.Elter auf.

PPX_XO und PPX_RECO:
Das PPX-XO/Reko-Verfahren garantiert, dass aus reihenfolge-zulaessigen Eltern 
auch reihenfolge-zulaessigen Kinder entstehen. Es setzt mindestens 2 Segmente 
voraus. Die Sequenzanzahl liegt zwischen zwei und der Segmentanzahl. XO ist 
eine 2-Sequenzvariante, waehrend Reko die n-Sequenzvariante darstellt mit n>2.
Algorithmus:
FOR alle sequAnz Eintraege in sequLen DO
  Kopiere alternierend sequLen[ii] Aktionen in der jeweils vorhandenen Reihen-
  folge von Elter-AK1 und -AK2 soweit noch nicht kopiert.
END_FOR


Die Routinen im Einzelnen:
 
BOOL abschn_cross_over (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                        CHAIN_ELEM_PTR_TYPE *res_ptr2,
                        CHAIN_ELEM_PTR_TYPE  org_ptr1,
                        CHAIN_ELEM_PTR_TYPE  org_ptr2,
                        GSTATE              *ret_state);
          Die Routine operiert ueber Kopien der beiden Ketten "org_ptr1" und 
          "org_ptr2" und liefert die Ergebnisse in "res_ptr1" und "res_ptr2" ab.
          Sie vertauscht Abschnitt A1 der Kette K1 mit Abschnitt A2 der Kette 
          K2. Wenn beide Ketten weniger als zwei Abschnitte haben oder eine leer
          ist, liefert die Routine FALSE und leere Zeiger ab. Ansonsten TRUE und
          "res_ptr1" und "res_ptr2" zeigen auf die beiden neuen Ketten.
 
BOOL cross_over (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                 CHAIN_ELEM_PTR_TYPE *res_ptr2,
                 CHAIN_ELEM_PTR_TYPE  org_ptr1,
                 CHAIN_ELEM_PTR_TYPE  org_ptr2,
                 GSTATE              *ret_state);
          Die Routine operiert ueber Kopien der beiden Ketten "org_ptr1" (K1) 
          und "org_ptr2" (K2). Die Kette K1 habe mehr Abschnitte als K2. Wuerfle
          in beiden Ketten zwei Abschnitte AnK1 und AmK2 aus, wobei n=0,..,x und
          m=1,..,y+1 mit x=Abschnittanzahl(K1) und y=Abschnittanzahl(K2).
          Bilde zwei neue Ketten:
              X1 = A1K1,..,AnK1    ,AmK2    ,..,AyK2  und
              X2 = A1K2,..,A(m-1)K2,A(n+1)K1,..,AxK1
          aus Kopien der Teilketten. n=0 und m=y+1 bedeuten keinen Abschnitt aus
          K1 bzw. K2 fuer X1. Die beiden Faelle (n,m) = (0,1) und (x,y+1) lie-
          fern identische Ketten und werden daher ausgeschlossen. Die Faelle 
          (0,y+1) und (x,1) konkatenieren K1 und K2: es wird K1+K2 und eine 
          leere Kette abgeliefert.
          Wenn beide Ketten weniger als zwei Abschnitte haben oder eine leer 
          ist, liefert die Routine FALSE und leere Zeiger ab. Ansonsten TRUE und
          "res_ptr1" und "res_ptr2" zeigen auf die beiden neuen Ketten.
 
BOOL rekombination (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                    CHAIN_ELEM_PTR_TYPE *res_ptr2,
                    CHAIN_ELEM_PTR_TYPE  org_ptr1,
                    CHAIN_ELEM_PTR_TYPE  org_ptr2,
                    GSTATE              *ret_state);
          Die Routine erzeugt zwei neue Ketten aus zwei Elternketten "org_ptr1"
          und "org_ptr2" durch Zusammenfuegen der Kopie jeweils eines Eltern-
          abschnitts bzw einer Abschnittsgruppe zu einer neuen Kette. Der dabei
          verbleibende Rest wird ebenfalls kopiert und bildet die zweite Kette.
          Wenn beide Ketten weniger als zwei Abschnitte haben oder eine leer ist
          oder wahrscheinlichkeitsbedingt identische Ketten entstehen, liefert 
          die Routine FALSE und leere Zeiger ab. Ansonsten TRUE und "res_ptr1" 
          und "res_ptr2" zeigen auf die beiden neuen Ketten.

BOOL orderRekoXO (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                  CHAIN_ELEM_PTR_TYPE *res_ptr2,
                  CHAIN_ELEM_PTR_TYPE  org_ptr1,
                  CHAIN_ELEM_PTR_TYPE  org_ptr2,
                  INT                  genop,
                  BOOL                 xo_mode,
                  GSTATE              *ret_state);
          Die Routinen erzeugt zwei neue Ketten aus zwei Elternketten "org_ptr1"
          und "org_ptr2" derart, dass die Reihenfolge beider Elternketten zur
          Grundlage der Reihenfolge beider Ergebnisketten gemacht wird (order 
          crossover OX oder PPX). Damit soll erreicht werden, dass die Aktions-
          reihenfolge der Nachkommen entweder moeglichst (OX) oder definitiv 
          (PPX) zulaessig ist, sofern auch die Elternketten eine korrekte Rei-
          henfolge hatten. Die XO-Variante arbeitet im Gegensatz zu RECO mit
          der geringst-moeglichen Sequenzanzahl statt mit n Sequenzen. Ausgehend
          von den Abschnittsgrenzen des 1.Elter werden n Sequenzen unterschied-
          licher Länge bestimmt und zur Grundlage des Crossovers wie oben (siehe
          OX und PPX) beschrieben. Die resultierende AK wird genauso segmentiert 
          wie die AK des 1.Elter. Danach werden die Eltern vertauscht und der 2.
          Nachkomme erzeugt. Dieser erbt die Segmentierung des ehemals 2.Elters.
          Wenn die 1. Eltern-AK weniger als 3 (OX) bzw. 2 (PPX) Segmente hat, 
          wird die zweite zur ersten, sofern diese mindestens 3 bzw. 2 Segmente 
          hat. Andernfalls, liefert die Routine FALSE und leere Zeiger ab. An-
          sonsten TRUE und "res_ptr1" und "res_ptr2" zeigen auf die beiden neuen
          Ketten.

BOOL rxo_init (void);
          Aufbau der modul-lokalen Datenstruktur "gruppe" fuer "rekombination". 
          Liefert FALSE, wenn dazu der verfuegbare Speicher nicht ausreichen 
          sollte.

--------------------------------------------------------------------------------
basiert auf C-Unit: go_rxo
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Neue Operatoren OX_XO, OX_RECO, PPX_XO und PPX_RECO realisiert 
    durch "order_xoRekoXO".                                          WJ 30.01.07
 2. Entfernung eines Includes im Kontext von GLEAM_TCL. Entfernung 
    ueberfluessiger Variable.                                        WJ 28.06.12
 3. Renovierung: SEGM_ANZ_MAX aus "chaindef.h" hierher uebernommen. 
    Ersetzung von testweisen "write_line"-Ausgaben durch"printf"-
    Aufrufe und Anpassung der "lies_wert"-Aufrufe.                   WJ 04.05.16
 4. Anpassung des "write_to_log"-Aufrufs.                            WJ 19.12.16
 5. Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".            WJ 30.06.17
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 30.06.2017
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
#include "evo.h"
#include "evoInt.h"

#ifdef GLO_CHECKS
  #define CHECKS
#endif

#undef TEST
#undef MTEST
#undef ZTEST





/* ============================ Lokale Konstante: =========================== */
#define SEGM_ANZ_MAX   200     /* erwartete MaxAnz d.Segmente einer Kette als */
                               /* anfaengliche Groesse der Variable "gruppe". */



/* ============================ Lokale Variable: ============================ */
static CHAIN_ELEM_PTR_TYPE  dest1_ptr, dest2_ptr;       /* Fuer die Routinen  */
static REF_DESCR_PTR_TYPE   dest1_sd_ptr, dest2_sd_ptr; /* "abschn_cross_over"*/
static INT                  dest1_len, dest2_len,       /* "cross_over"       */
                            dest1_sd_index, dest2_sd_index, /* "rekombination"*/
                            last_gruppe_size,           /* akt.Gr.v."gruppe"  */
                            sequAnz;       /* Sequenzanzahl einer Eltern-AK   */
static INT                 *gruppe;        /* AbschnGruppen f."rekombination" */



/* ============================ Lokale Routinen: ============================ */
static GSTATE copy_segm_to_dest1 (REF_DESCR_PTR_TYPE  *src_sd_ptr,
                                  INT                 *src_sd_index);

static GSTATE copy_segm_to_dest2 (REF_DESCR_PTR_TYPE  *src_sd_ptr,
                                  INT                 *src_sd_index);

static BOOL   field2chain        (CHAIN_ELEM_PTR_TYPE *res_ptr, 
                                  CHAIN_ELEM_PTR_TYPE  org_ptr, 
                                  GSTATE              *ret_state);

static BOOL   make_OX_child      (CHAIN_ELEM_PTR_TYPE *res_ptr,
                                  CHAIN_ELEM_PTR_TYPE  org1_ptr,
                                  CHAIN_ELEM_PTR_TYPE  org2_ptr,
                                  GSTATE              *ret_state);

static BOOL   make_PPX_child     (CHAIN_ELEM_PTR_TYPE *res_ptr,
                                  CHAIN_ELEM_PTR_TYPE  org1_ptr,
                                  CHAIN_ELEM_PTR_TYPE  org2_ptr,
                                  GSTATE              *ret_state);



/* ============================ abschn_cross_over =========================== */
BOOL abschn_cross_over (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                        CHAIN_ELEM_PTR_TYPE *res_ptr2,
                        CHAIN_ELEM_PTR_TYPE  org_ptr1,
                        CHAIN_ELEM_PTR_TYPE  org_ptr2,
                        GSTATE              *ret_state)
{
   BOOL                 r_abschn_cross_over = FALSE;
   REF_DESCR_PTR_TYPE   sd1_ptr, sd2_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_end_ptr, seq_start_ptr, last1_ptr, pred1_ptr,
                        last2_ptr, pred2_ptr;
   INT    abschn1_anz, abschn1_len, abschn1_nr, abschn2_anz, abschn2_len,
          abschn2_nr, sd1_index, sd2_index;
   
   /* abschn_cross_over */
#ifdef CHECKS
   if (org_ptr1 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 1, NO_INFO, NO_INFO, "", NIL_PTR);
   if (org_ptr2 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 2, NO_INFO, NO_INFO, "", NIL_PTR);
   if (*ret_state != GLEAM_OK)
      return (FALSE);
#endif /* CHECKS */
   *res_ptr1 = NULL;
   *res_ptr2 = NULL;
   abschn1_anz = org_ptr1->u_el.u_xel.u_hdr.segment_anz;
   abschn2_anz = org_ptr2->u_el.u_xel.u_hdr.segment_anz;
   if (((abschn1_anz > 1) && (abschn2_anz > 0)) ||
       ((abschn1_anz > 0) && (abschn2_anz > 1))   )
   { /* Beide Ketten sind nicht leer u.mind. eine hat mehr als 1 Abschnitt */
      copy_chain (res_ptr1, org_ptr1, KEINE_URKETTE, NO_MEM_CHECK, ret_state);
      copy_chain (res_ptr2, org_ptr2, KEINE_URKETTE, NO_MEM_CHECK, ret_state);
#ifdef MTEST
      printf ("Abschn-Nummern [1..%1d] und [1..%1d]:", abschn1_anz, abschn2_anz);
      lies_wert (&abschn1_nr, "Abschn1-Nr: ", 1, abschn1_anz, abschn1_anz+1);
      lies_wert (&abschn2_nr, "Abschn2-Nr: ", 1, abschn2_anz, abschn2_anz+1);
#else /* Kein MTEST */
      abschn1_nr = irand (abschn1_anz) + 1;
      abschn2_nr = irand (abschn2_anz) + 1;
  #ifdef ZTEST
      printf ("SXO: Abschn-Nummern = %1d  %1d\n", abschn1_nr, abschn2_nr);
  #endif
#endif /* Kein MTEST */
      if (!ermittle_abschnitt (&pred1_ptr, &last1_ptr, &abschn1_len, &sd1_index,
                               &sd1_ptr, *res_ptr1, abschn1_nr, ret_state))
         *ret_state = fatal (EVO_GO_RXO, 3, abschn1_nr, NO_INFO, "", NO_ABSCHN);
      if (!ermittle_abschnitt (&pred2_ptr, &last2_ptr, &abschn2_len, &sd2_index,
                               &sd2_ptr, *res_ptr2, abschn2_nr, ret_state))
         *ret_state = fatal (EVO_GO_RXO, 4, abschn2_nr, NO_INFO, "", NO_ABSCHN);
      if (*ret_state != GLEAM_OK)
         return (FALSE);
      seq_end_ptr = last1_ptr;  /* Damit muss "remove_seq" nicht mehr suchen! */
      *ret_state  = remove_seq (&seq_start_ptr, &seq_end_ptr, &abschn1_len,
                                *res_ptr1, pred1_ptr);
      *ret_state |= insert_seq (*res_ptr2, last2_ptr, seq_start_ptr,
                                seq_end_ptr, abschn1_len);
      sd2_ptr->segm_ptrs  [sd2_index] = last1_ptr;
      sd2_ptr->segm_length[sd2_index] = abschn1_len;
      seq_end_ptr = last2_ptr;  /* Damit muss "remove_seq" nicht mehr suchen! */
      *ret_state |= remove_seq (&seq_start_ptr, &seq_end_ptr, &abschn2_len,
                                *res_ptr2, pred2_ptr);
      *ret_state |= insert_seq (*res_ptr1, pred1_ptr, seq_start_ptr,
                                seq_end_ptr, abschn2_len);
      sd1_ptr->segm_ptrs  [sd1_index] = last2_ptr;
      sd1_ptr->segm_length[sd1_index] = abschn2_len;
      r_abschn_cross_over = *ret_state == GLEAM_OK;
   } /* Beide Ketten sind nicht leer u.mind. eine hat mehr als 1 Abschnitt */
   return (r_abschn_cross_over);
} /* abschn_cross_over */ 



/* =========================== copy_segm_to_dest1 =========================== */
static GSTATE copy_segm_to_dest1 (REF_DESCR_PTR_TYPE *src_sd_ptr,
                                  INT                *src_sd_index)
/*----------------------------------------------------------------------------*/
/*  Kopiert das durch "src_sd_ptr" und "src_sd_index" angegebene Segment an   */
/*  das Ende der globalen Zielkette "dest1_...". Die Zielkette ist global,    */
/*  um die Uebergabe von 4 Parametern zu vermeiden. Die Routine wird von      */
/*  "cross_over" und "rekombination" benutzt. Bei auftretetenden Fehlern      */
/*  liefert sie GLEAM_FATAL.                                                  */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  start_ptr;
   INT                  segm_len;
   GSTATE               ret_state;
   
   /* copy_segm_to_dest1 */
   start_ptr = (*src_sd_ptr)->segm_ptrs[*src_sd_index]->u_el.next_action;
   inc_segm_nr (src_sd_ptr, src_sd_index);
   segm_len  = (*src_sd_ptr)->segm_length[*src_sd_index];
   ret_state = copy_seq (&(dest1_ptr->u_el.u_xel.u_hdr.last_action->u_el.
                           next_action),
                         &(dest1_ptr->u_el.u_xel.u_hdr.last_action),
                         start_ptr, segm_len);
   if ((dest1_sd_index+1) == SEGM_PTR_ANZ)
   {
      if (new_ref_descr (&(dest1_sd_ptr->next_ref_descr_ptr)) != GLEAM_OK)
         return (GLEAM_FATAL);
   }
   inc_segm_nr (&dest1_sd_ptr, &dest1_sd_index);
   dest1_sd_ptr->segm_ptrs  [dest1_sd_index] = dest1_ptr->u_el.u_xel.u_hdr.
                                               last_action;
   dest1_sd_ptr->segm_length[dest1_sd_index] = segm_len;
   dest1_len += segm_len;
   return (ret_state);
} /* copy_segm_to_dest1 */ 



/* =========================== copy_segm_to_dest2 =========================== */
static GSTATE copy_segm_to_dest2 (REF_DESCR_PTR_TYPE *src_sd_ptr,
                                  INT                *src_sd_index)
/*----------------------------------------------------------------------------*/
/*  Kopiert das durch "src_sd_ptr" und "src_sd_index" angegebene Segment an   */
/*  das Ende der globalen Zielkette "dest2_...". Die Zielkette ist global,    */
/*  um die Uebergabe von 4 Parametern zu vermeiden. Die Routine wird von      */
/*  "cross_over" und "rekombination" benutzt. Bei auftretetenden Fehlern      */
/*  liefert sie GLEAM_FATAL.                                                  */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  start_ptr;
   INT                  segm_len;
   GSTATE               ret_state;
   
   /* copy_segm_to_dest2 */
   start_ptr = (*src_sd_ptr)->segm_ptrs[*src_sd_index]->u_el.next_action;
   inc_segm_nr (src_sd_ptr, src_sd_index);
   segm_len  = (*src_sd_ptr)->segm_length[*src_sd_index];
   ret_state = copy_seq (&(dest2_ptr->u_el.u_xel.u_hdr.last_action->u_el.
                           next_action),
                         &(dest2_ptr->u_el.u_xel.u_hdr.last_action),
                         start_ptr, segm_len);
   if ((dest2_sd_index+1) == SEGM_PTR_ANZ)
   {
      if (new_ref_descr (&(dest2_sd_ptr->next_ref_descr_ptr)) != GLEAM_OK)
         return (GLEAM_FATAL);
   }
   inc_segm_nr (&dest2_sd_ptr, &dest2_sd_index);
   dest2_sd_ptr->segm_ptrs  [dest2_sd_index] = dest2_ptr->u_el.u_xel.u_hdr.
                                               last_action;
   dest2_sd_ptr->segm_length[dest2_sd_index] = segm_len;
   dest2_len += segm_len;
   return (ret_state);
} /* copy_segm_to_dest2 */ 



/* =============================== cross_over =============================== */
BOOL cross_over (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                 CHAIN_ELEM_PTR_TYPE *res_ptr2,
                 CHAIN_ELEM_PTR_TYPE  org_ptr1,
                 CHAIN_ELEM_PTR_TYPE  org_ptr2,
                 GSTATE              *ret_state)
{
   BOOL                 r_cross_over = FALSE;
   REF_DESCR_PTR_TYPE   src1_sd_ptr, src2_sd_ptr;
   CHAIN_ELEM_PTR_TYPE  tmp_ptr;
   INT     abschn1_anz, abschn1_nr, abschn2_anz, abschn2_nr, src1_sd_ind,
           src2_sd_ind, ii, ug, og_plus;
   
   /* CROSS_OVER */
#ifdef CHECKS
   if (org_ptr1 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 5, NO_INFO, NO_INFO, "", NIL_PTR);
   if (org_ptr2 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 6, NO_INFO, NO_INFO, "", NIL_PTR);
   if (*ret_state != GLEAM_OK)
      return (FALSE);
#endif /* CHECKS */
   *res_ptr1   = NULL;
   *res_ptr2   = NULL;
   abschn1_anz = org_ptr1->u_el.u_xel.u_hdr.segment_anz;
   abschn2_anz = org_ptr2->u_el.u_xel.u_hdr.segment_anz;
   if (abschn1_anz < abschn2_anz)
   { /* vertauschen */
      tmp_ptr     = org_ptr1;
      org_ptr1    = org_ptr2;
      org_ptr2    = tmp_ptr;
      ii          = abschn1_anz;
      abschn1_anz = abschn2_anz;
      abschn2_anz = ii;
   } /* vertauschen */
   if ((abschn1_anz > 1) && (abschn2_anz > 0))
   { /* Beide Ketten sind nicht leer u.mind. eine hat mehr als 1 Abschnitt */
#ifdef MTEST
      printf ("Abschn-Nummern [0..%d] und [1..%d]:", abschn1_anz, (abschn2_anz+1));
      lies_wert (&abschn1_nr, "Abschn1-Nr: ", 0, abschn1_anz,   abschn1_anz+1);
      lies_wert (&abschn2_nr, "Abschn2-Nr: ", 1, abschn2_anz+1, abschn2_anz+2);
#else /* Kein MTEST */
      ug         = 1;
      og_plus    = 1;
      abschn1_nr = irand (abschn1_anz+1);
      if (abschn1_nr == 0)
      { /* (min, min) verhindern */
         ug      = 2;
         og_plus = 0;
      } /* (min, min) verhindern */
      else {
         if (abschn1_nr == abschn1_anz)
            og_plus = 0;                             /* (max, max) verhindern */
      }
      abschn2_nr = irand (abschn2_anz + og_plus) + ug;
  #ifdef ZTEST
      printf ("XO: Abschn-Nummern = %d  %d", abschn1_nr, abschn2_nr);
  #endif
#endif /* Kein MTEST */
      src1_sd_ptr = org_ptr1->u_el.u_xel.u_hdr.segm_descr_ptr;
      src2_sd_ptr = org_ptr2->u_el.u_xel.u_hdr.segm_descr_ptr;
      src1_sd_ind = 0;
      src2_sd_ind = 0;
      dest1_sd_index = 0;
      dest2_sd_index = 0;
      dest1_len   = 0;
      dest2_len   = 0;
      *ret_state  = init_chain (&dest1_ptr, AK_HEADER);
      *ret_state |= new_ref_descr (&dest1_sd_ptr);
      if (*ret_state != GLEAM_OK)
         return (FALSE);
      dest1_sd_ptr->segm_ptrs[0]                 = dest1_ptr;
      dest1_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = dest1_sd_ptr;
      *ret_state  = init_chain (&dest2_ptr, AK_HEADER);
      *ret_state |= new_ref_descr (&dest2_sd_ptr);
      if (*ret_state != GLEAM_OK)
         return (FALSE);
      dest2_sd_ptr->segm_ptrs[0]                 = dest2_ptr;
      dest2_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = dest2_sd_ptr;
      for (ii = 1;  (ii <= abschn1_nr)   && (*ret_state == GLEAM_OK);  ii++)
         *ret_state = copy_segm_to_dest1 (&src1_sd_ptr,
                                          &src1_sd_ind);  /*A1K1..AnK1    ->D1*/
      for (ii = 1;  (ii <= abschn2_nr-1) && (*ret_state == GLEAM_OK);  ii++)
         *ret_state = copy_segm_to_dest2 (&src2_sd_ptr,
                                          &src2_sd_ind);  /*A1K2..A(m-1)K2->D2*/
      for (ii = abschn2_nr;  (ii <= abschn2_anz) &&
              (*ret_state == GLEAM_OK);  ii++)            /*AmK2..AxK2    ->D1*/
         *ret_state = copy_segm_to_dest1 (&src2_sd_ptr, &src2_sd_ind);
      for (ii = abschn1_nr + 1;  (ii <= abschn1_anz) &&
              (*ret_state == GLEAM_OK);  ii++)            /*A(n+1)K1..AyK1->D2*/
         *ret_state = copy_segm_to_dest2 (&src1_sd_ptr, &src1_sd_ind);
      dest1_ptr->u_el.u_xel.u_hdr.chain_length = dest1_len;
      dest2_ptr->u_el.u_xel.u_hdr.chain_length = dest2_len;
      dest1_ptr->u_el.u_xel.u_hdr.segment_anz  = abschn1_nr + abschn2_anz - 
                                                 abschn2_nr + 1;
      dest2_ptr->u_el.u_xel.u_hdr.segment_anz  = abschn2_nr + abschn1_anz - 
                                                 abschn1_nr - 1;
      *res_ptr1 = dest1_ptr;
      *res_ptr2 = dest2_ptr;
      r_cross_over = *ret_state == GLEAM_OK;
   } /* Beide Ketten sind nicht leer u.mind. eine hat mehr als 1 Abschnitt */
   return (r_cross_over);
} /* cross_over */ 



/* ============================== rekombination ============================= */
BOOL rekombination (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                    CHAIN_ELEM_PTR_TYPE *res_ptr2,
                    CHAIN_ELEM_PTR_TYPE  org_ptr1,
                    CHAIN_ELEM_PTR_TYPE  org_ptr2,
                    GSTATE              *ret_state)
{
   CHAIN_ELEM_PTR_TYPE  tmp_ptr;
   REF_DESCR_PTR_TYPE   src1_sd_ptr, src2_sd_ptr;
   INT     abschn1_anz, abschn2_anz, gruppen_anz, anz_in_gruppe, d1_abschn_anz,
           d2_abschn_anz, src1_sd_ind, src2_sd_ind, rest_abschn, abschn_pro_gruppe,
           p, gi, ii;
   BOOL    von1            = FALSE; 
   BOOL    von2            = FALSE;
   BOOL    r_rekombination = FALSE;
   
   /* REKOMBINATION */
#ifdef CHECKS
   if (org_ptr1 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 7, NO_INFO, NO_INFO, "", NIL_PTR);
   if (org_ptr2 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 8, NO_INFO, NO_INFO, "", NIL_PTR);
   if (*ret_state != GLEAM_OK)
      return (FALSE);
#endif /* CHECKS */
   *res_ptr1   = NULL;
   *res_ptr2   = NULL;
   abschn1_anz = org_ptr1->u_el.u_xel.u_hdr.segment_anz;
   abschn2_anz = org_ptr2->u_el.u_xel.u_hdr.segment_anz;
   if ((abschn1_anz > 1) && (abschn2_anz > 1))
   { /* Beide Ketten sind nicht leer u. haben mehr als 1 Abschnitt */
      if (abschn1_anz < abschn2_anz)
      { /* vertauschen */
         tmp_ptr     = org_ptr1;
         org_ptr1    = org_ptr2;
         org_ptr2    = tmp_ptr;
         ii          = abschn1_anz;
         abschn1_anz = abschn2_anz;
         abschn2_anz = ii;
      } /* vertauschen */
      
      /* Abschn d.K1 zu gleichviel Gruppen zusammenfassen, wie K2 Abschn hat: */
      gruppen_anz = abschn2_anz;
      if (gruppen_anz > last_gruppe_size)
      { /* Zu viel Abschnitte: Neu-Allokieren */
         free (gruppe);
         if ((gruppe = (INT*)malloc (gruppen_anz * sizeof (INT))) == NULL)
         { /* Speicher alle */
            *ret_state = fehler (EVO_GO_RXO, 9, gruppen_anz, last_gruppe_size, 
                                 "", ZU_VIEL_ABSCHN);
            mem_out    = TRUE;
            return (FALSE);
         } /* Speicher alle */
         else
            last_gruppe_size = gruppen_anz;        /* ok: neue Groesse merken */
      } /* Zu viel Abschnitte: Neu-Allokieren */
      abschn_pro_gruppe = abschn1_anz / gruppen_anz;
      rest_abschn       = abschn1_anz % gruppen_anz;
      for (ii = 0;  ii < gruppen_anz;  ii++)
         gruppe[ii] = abschn_pro_gruppe;
      if (rest_abschn > 0)
      { /* Es bleiben Abschnitte uebrig */
         if (rest_abschn <= trunc (gruppen_anz*0.6))
         { /* Es bleiben weniger als 60% der Abschnitte uebrig */
            for  (ii = 0;  ii < rest_abschn;  ii++)
            { /* restliche Abschnitte auf Gruppen verteilen */
               do
                  gi = irand (gruppen_anz);
               while (gruppe[gi] != abschn_pro_gruppe);
               (gruppe[gi])++;
            } /* restliche Abschnitte auf Gruppen verteilen */
         } /* Es bleiben weniger als 60% der Abschnitte uebrig */
         else
         { /* Es bleiben mehr als 60% der Abschnitte uebrig */
            abschn_pro_gruppe++;
            rest_abschn = gruppen_anz-rest_abschn;
            for (ii = 0;  ii < gruppen_anz;  ii++)
               gruppe[ii] = abschn_pro_gruppe;
            for (ii = 0;  ii < rest_abschn;  ii++)
            { /* restliche Abschnitte auf Gruppen verteilen */
               do
                  gi = irand (gruppen_anz);
               while (gruppe[gi] != abschn_pro_gruppe);
               (gruppe[gi])--;
            } /* restliche Abschnitte auf Gruppen verteilen */
         } /* Es bleiben mehr als 60% der Abschnitte uebrig */
      } /* Es bleiben Abschnitte uebrig */
#ifdef TEST
      sprintf (buf2, "%d Gruppen:", gruppen_anz);
      for (ii = 0;  ii < gruppen_anz;  ii++)
      {
         sprintf (buf1, "%d", gruppe[ii]);
         sprintf (buf2, "%s %s", buf2, buf1);
      }
      printf ("%s\n", buf2);
#endif /* TEST */
      src1_sd_ptr = org_ptr1->u_el.u_xel.u_hdr.segm_descr_ptr;
      src2_sd_ptr = org_ptr2->u_el.u_xel.u_hdr.segm_descr_ptr;
      src1_sd_ind    = 0;
      src2_sd_ind    = 0;
      dest1_sd_index = 0;
      dest2_sd_index = 0;
      dest1_len      = 0;
      dest2_len      = 0;
      d1_abschn_anz  = 0;
      d2_abschn_anz  = 0;
      *ret_state  = init_chain (&dest1_ptr, AK_HEADER);
      *ret_state |= new_ref_descr (&dest1_sd_ptr);
      if (*ret_state != GLEAM_OK)
         return (FALSE);
      dest1_sd_ptr->segm_ptrs[0]                 = dest1_ptr;
      dest1_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = dest1_sd_ptr;
      *ret_state  = init_chain (&dest2_ptr, AK_HEADER);
      *ret_state |= new_ref_descr (&dest2_sd_ptr);
      if (*ret_state != GLEAM_OK)
         return (FALSE);
      dest2_sd_ptr->segm_ptrs[0]                 = dest2_ptr;
      dest2_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = dest2_sd_ptr;
      
      /* Rekombination der neuen Ketten durch abschnittsweises Kopieren: */
      for (ii = 0;  (ii < gruppen_anz) && (*ret_state == GLEAM_OK);  ii++)
      { /* Rekombinieren der Abschnittsgruppe bzw. des Abschnitts */
#ifdef MTEST
         lies_wert (&p, "Quellkette eingeben (0,1) [0]: ", 0, 1, 0);
#else /* kein MTEST */
         p = irand (2);
  #ifdef ZTEST
         printf ("Reko: Quellkette = %d\n", p);
  #endif
#endif /* kein MTEST */
         if (p == 1)
         { /* Gruppe_K1 -> D1 und Abschnitt_K2 -> D2 */
            von1          = TRUE;
            anz_in_gruppe = gruppe[ii];
            for (gi = 0;  gi < anz_in_gruppe;  gi++)       /* Gruppe_K1 -> D1 */
               *ret_state |= copy_segm_to_dest1 (&src1_sd_ptr, &src1_sd_ind);
            d1_abschn_anz += anz_in_gruppe;
            *ret_state |= copy_segm_to_dest2 (&src2_sd_ptr, &src2_sd_ind);
            d2_abschn_anz++;                               /* Abschn_K2 -> D2 */
         } /* Gruppe_K1 -> D1 und Abschnitt_K2 -> D2 */
         else
         { /* Gruppe_K1 -> D2 und Abschnitt_K2 -> D1 */
            von2          = TRUE;
            anz_in_gruppe = gruppe[ii];
            for (gi = 0;  gi < anz_in_gruppe;  gi++)       /* Gruppe_K1 -> D2 */
               *ret_state |= copy_segm_to_dest2 (&src1_sd_ptr, &src1_sd_ind);
            d2_abschn_anz += anz_in_gruppe;
            *ret_state |= copy_segm_to_dest1 (&src2_sd_ptr, &src2_sd_ind);
            d1_abschn_anz++;                               /* Abschn_K2 -> D1 */
         } /* Gruppe_K1 -> D2 und Abschnitt_K2 -> D1 */
      } /* Rekombinieren der Abschnittsgruppe bzw. Abschnitts */
      dest1_ptr->u_el.u_xel.u_hdr.chain_length = dest1_len;
      dest2_ptr->u_el.u_xel.u_hdr.chain_length = dest2_len;
      dest1_ptr->u_el.u_xel.u_hdr.segment_anz  = d1_abschn_anz;
      dest2_ptr->u_el.u_xel.u_hdr.segment_anz  = d2_abschn_anz;
      
      /* Abschlusstest auf identische Ketten und Ergebnis abliefern: */
      if (von1 && von2)
      { /* D1 setzt sich aus Teilen von beiden "org_.."-AKs zusammen */
         r_rekombination = *ret_state == GLEAM_OK;
         *res_ptr1 = dest1_ptr;
         *res_ptr2 = dest2_ptr;
      } /* D1 setzt sich aus Teilen von beiden "org_.."-AKs zusammen */
      else
      { /* Identische Ketten vernichten */
         delete_chain (&dest1_ptr);
         delete_chain (&dest2_ptr);
      } /* Identische Ketten vernichten */
   } /* Beide Ketten sind nicht leer u. haben mehr als 1 Abschnitt */
   return (r_rekombination);
} /* rekombination */ 



/* =============================== field2chain ============================== */
static BOOL field2chain (CHAIN_ELEM_PTR_TYPE *res_ptr, 
                         CHAIN_ELEM_PTR_TYPE  org_ptr, 
                         GSTATE              *ret_state)
/*----------------------------------------------------------------------------*/
/*  Erzeugt in "res_ptr" eine AK mit den Aktionen des "aktionsFeld" und seg-  */
/*  mentiert sie so, wie in der durch "org_ptr" vorgegebenen Segmentstruktur. */
/*  Liefert TRUE und einen guten "ret_state" ab, wennd dazu genuegend Spei-   */
/*  cherplatz vorhanden ist.                                                  */
/*----------------------------------------------------------------------------*/
{
   REF_DESCR_PTR_TYPE  child_sd_ptr, org_sd_ptr, new_sd_ptr;;
   INT                 segm_anz, child_segm_idx, org_segm_idx, akt_segm_len, 
                       ii, jj;
   BOOL                erg = FALSE;

   /* field2chain */
   *ret_state  = init_chain (res_ptr, AK_HEADER);
   *ret_state |= new_ref_descr (&child_sd_ptr);
   if (*ret_state == GLEAM_OK)
   { /* AK-Kopf und 1.RefDescrBlock ok */
      /* --------------------------- AK aufbauen: --------------------------- */
      (*res_ptr)->u_el.next_action                = aktionsFeld[0];
      (*res_ptr)->u_el.u_xel.u_hdr.last_action    = aktionsFeld[last_act - 1];
      (*res_ptr)->u_el.u_xel.u_hdr.segm_descr_ptr = child_sd_ptr;
      for (ii = 1;  ii < last_act;  ii++)                    /* AK verzeigern */
         aktionsFeld[ii-1]->u_el.next_action =  aktionsFeld[ii];
      aktionsFeld[last_act - 1]->u_el.next_action = NULL;    /* AK-Abschluss  */
      (*res_ptr)->u_el.u_xel.u_hdr.chain_length = last_act;

      /* ------------------------- AK segmentieren: ------------------------- */
      child_segm_idx = 0;
      org_segm_idx   = 0;
      org_sd_ptr     = org_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
      segm_anz       = org_ptr->u_el.u_xel.u_hdr.segment_anz;
      erg            = TRUE;
      jj             = -1;     /* da Indizierung im aktionsFeld mit 0 beginnt */
      child_sd_ptr->segm_ptrs[0] = *res_ptr;
      for (ii = 0;  (ii < segm_anz) && erg;  ii++) 
      { /* Kopiere Segmentstruktur */
         if (inc_segm_nr (&org_sd_ptr, &org_segm_idx) == GLEAM_OK)
            if (org_segm_idx == 0) 
            { /* SegmDescrRecord-Wechsel, neuer fuers Kind */
#ifdef TEST
               sprintf (buf3, "field2chain: Segmentierung: Record-Wechsel beim Kopieren"
                       "des %d.Segm", ii);
               write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
               *ret_state = new_ref_descr (&new_sd_ptr);
               if (*ret_state == GLEAM_OK) {
                  child_sd_ptr->next_ref_descr_ptr = new_sd_ptr;
                  child_sd_ptr   = new_sd_ptr;
                  child_segm_idx = 0;
               }
               else
                  erg = FALSE;
            } /* SegmDescrRecord-Wechsel, neuer fuers Kind */
            else
               child_segm_idx++;
         else
            erg = FALSE;
         if (erg) { /* Segment-Record und Index gueltig */
            akt_segm_len = org_sd_ptr->segm_length[org_segm_idx];
            jj += akt_segm_len;
#ifdef TEST
            sprintf (buf3, "field2chain: Segmentierung: Kopieren des %d.Segm: "
                     "Idx: %d->%d, Len=%d, EndPos=%d", 
                     ii, org_segm_idx, child_segm_idx, akt_segm_len, jj);
            write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
            child_sd_ptr->segm_ptrs[child_segm_idx]   = aktionsFeld[jj];
            child_sd_ptr->segm_length[child_segm_idx] = akt_segm_len;
         } /* Segment-Record und Index gueltig */
      } /* Kopiere Segmentstruktur */
      child_sd_ptr->next_ref_descr_ptr = NULL;
      (*res_ptr)->u_el.u_xel.u_hdr.segment_anz = segm_anz;
   } /* AK-Kopf und 1.RefDescrBlock ok */
#ifdef TEST
   sprintf (buf3, "field2chain: erg=%d EndPos=%d EndAct=%d", erg, jj, 
            child_sd_ptr->segm_ptrs[child_segm_idx]->u_el.activity);
   write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
   return (erg);
} /* field2chain */



/* ============================== make_OX_child ============================= */
static BOOL make_OX_child (CHAIN_ELEM_PTR_TYPE *res_ptr,
                           CHAIN_ELEM_PTR_TYPE  org1_ptr,
                           CHAIN_ELEM_PTR_TYPE  org2_ptr,
                           GSTATE              *ret_state)
/*----------------------------------------------------------------------------*/
/*  Erzeugt in "res_ptr" einen Nachkommen gemaess OX-Crossover u.liefert TRUE */
/*  ab, wenn dies gelang. Ansonsten FALSE und einen schlechten "ret_state".   */
/*  Die Strukturibformationen sind in "sequLen" und "sequAnz" enthalten. Die  */
/*  Ergebnis-AK erhaelt die Sgementstruktur von "org1_ptr".                   */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  akt_ptr;
   INT                  pos,
                        ii, jj;
   BOOL                 erg = TRUE;

   /* make_OX_child */
   memset (akt_flags,   0, (last_act+1)*sizeof(BOOL));
   memset (aktionsFeld, 0,  last_act   *sizeof(CHAIN_ELEM_PTR_TYPE));
   akt_ptr = org1_ptr;
   pos     = 0;
   ii      = 0;
   while ((ii < sequAnz) && erg)
   { /* Kopier-Schleife */
#ifdef TEST
      sprintf (buf3, "make_OX_child: Copy-Loop %d.Sequ mit Len=%d ueberspringen", 
               ii, sequLen[ii]);
      write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
      for (jj = 0;  jj < sequLen[ii];  jj++)           /* 0., 2., 4., ... Se- */
         akt_ptr = akt_ptr->u_el.next_action;          /* quenz ueberspringen */
      pos += sequLen[ii];
      ii++;
#ifdef TEST
      sprintf (buf3, "make_OX_child: Copy-Loop %d.Sequ mit Len=%d nach Pos %d" 
              " kopieren", ii, sequLen[ii], pos);
      write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
      if (ii < sequAnz)
      { /* 1., 3., 5., ... Sequenz kopieren */
         for (jj = 0;  (jj < sequLen[ii]) && erg;  jj++) 
         { /* Sequenz kopieren */
            akt_ptr = akt_ptr->u_el.next_action;
            *ret_state = copy_act (&(aktionsFeld[pos++]), akt_ptr);
            akt_flags[akt_ptr->u_el.activity] = TRUE;
            erg = (*ret_state == GLEAM_OK);
         } /* Sequenz kopieren */
         ii++;
      } /* 1., 3., 5., ... Sequenz kopieren */
   } /* Kopier-Schleife */
   akt_ptr = org2_ptr;
   pos = 0;
   for (ii = 0;  (ii < last_act) && erg;  ii++) 
   { /* Auffuell-Schleife */
      akt_ptr = akt_ptr->u_el.next_action;
      if (!akt_flags[akt_ptr->u_el.activity]) { /* eine Aktion kopieren */
         while (aktionsFeld[pos] != NULL)
            pos++;               /* bereits kopierte Positionen ueberspringen */
         *ret_state = copy_act (&(aktionsFeld[pos++]), akt_ptr);
         akt_flags[akt_ptr->u_el.activity] = TRUE;
         erg = (*ret_state == GLEAM_OK);
      } /* eine Aktion kopieren */
   } /* Auffuell-Schleife */
#ifdef TEST
   sprintf (buf3, "make_OX_child: erg=%d pos=%d (nextPos in aktionsFeld)\n", 
            erg, pos);
   write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
   if (erg)
      erg = field2chain (res_ptr, org1_ptr, ret_state);
   return (erg);
} /* make_OX_child */



/* ============================= make_PPX_child ============================= */
static BOOL make_PPX_child (CHAIN_ELEM_PTR_TYPE *res_ptr,
                            CHAIN_ELEM_PTR_TYPE  org1_ptr,
                            CHAIN_ELEM_PTR_TYPE  org2_ptr,
                            GSTATE              *ret_state)
/*----------------------------------------------------------------------------*/
/*  Erzeugt in "res_ptr" einen Nachkommen gemaess PPX-Crossover und liefert   */
/*  TRUE ab, wenn dies gelang. Ansonsten FALSE und einen schlechten "ret_     */
/*  state". Die Strukturibformationen sind in "sequLen" und "sequAnz" enthal- */
/*  ten. Die Ergebnis-AK erhaelt die Sgementstruktur von "org1_ptr".          */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  akt1_ptr, akt2_ptr; 
   INT                  pos,
                        ii, jj;
   BOOL                 erg = TRUE;

   /* make_PPX_child */
   memset (akt_flags,   0, (last_act+1)*sizeof(BOOL));
   memset (aktionsFeld, 0,  last_act   *sizeof(CHAIN_ELEM_PTR_TYPE));
   akt1_ptr = org1_ptr;
   akt2_ptr = org2_ptr;
   pos     = 0;
   ii      = 0;
   while ((ii < sequAnz) && erg)
   { /* Kopier-Schleife */
#ifdef TEST
      sprintf (buf3, "make_PPX_child: Copy-Loop %d.Sequ mit Len=%d vom 1.Elter", 
               ii, sequLen[ii]);
      write_to_log (buf3, CLOSE_LOG);
      strcpy (buf3, "make_PPX_child: Copy-Loop Aktionen:");
#endif /* TEST */
      jj = 0;
      while ((jj < sequLen[ii]) && erg)
      { /* SequenzLen Aktionen vom 1.Elter kopieren */
         akt1_ptr = akt1_ptr->u_el.next_action;
         if (!akt_flags[akt1_ptr->u_el.activity]) { /* Aktion kopieren */
#ifdef TEST
            sprintf (buf2, " %d", akt1_ptr->u_el.activity);
            strcat (buf3, buf2);
#endif /* TEST */
            *ret_state = copy_act (&(aktionsFeld[pos++]), akt1_ptr);
            akt_flags[akt1_ptr->u_el.activity] = TRUE;
            erg = (*ret_state == GLEAM_OK);
            jj++;
         } /* Aktion kopieren */
      } /* SequenzLen Aktionen vom 1.Elter kopieren */
#ifdef TEST
      write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
      ii++;                                               /* naechste Sequenz */
      if (ii < sequAnz)
      { /* SequenzLen Aktionen vom 2.Elter kopieren */
#ifdef TEST
         sprintf (buf3, "make_PPX_child: Copy-Loop %d.Sequ mit Len=%d vom 2.Elter", 
               ii, sequLen[ii]);
         write_to_log (buf3, CLOSE_LOG);
         strcpy (buf3, "make_PPX_child: Copy-Loop Aktionen:");
#endif /* TEST */
         jj = 0;
         while ((jj < sequLen[ii]) && erg)
         { /* Aktionen vom 2.Elter kopieren */
            akt2_ptr = akt2_ptr->u_el.next_action;
            if (!akt_flags[akt2_ptr->u_el.activity]) { /* Aktion kopieren */
#ifdef TEST
               sprintf (buf2, " %d", akt2_ptr->u_el.activity);
               strcat (buf3, buf2);
#endif /* TEST */
               *ret_state = copy_act (&(aktionsFeld[pos++]), akt2_ptr);
               akt_flags[akt2_ptr->u_el.activity] = TRUE;
               erg = (*ret_state == GLEAM_OK);
               jj++;
            } /* Aktion kopieren */
         } /* Aktionen vom 2.Elter kopieren */
         ii++;
#ifdef TEST
         write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
      } /* SequenzLen Aktionen vom 2.Elter kopieren */
   } /* Kopier-Schleife */
#ifdef TEST
   sprintf (buf3, "make_PPX_child: erg=%d pos=%d (nextPos in aktionsFeld)\n", 
            erg, pos);
   write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
   if (erg)
      erg = field2chain (res_ptr, org1_ptr, ret_state);
   return (erg);
} /* make_PPX_child */



/* =============================== orderRekoXO ============================== */
BOOL orderRekoXO (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                  CHAIN_ELEM_PTR_TYPE *res_ptr2,
                  CHAIN_ELEM_PTR_TYPE  org_ptr1,
                  CHAIN_ELEM_PTR_TYPE  org_ptr2,
                  INT                  genop,
                  GSTATE              *ret_state)
{
   CHAIN_ELEM_PTR_TYPE  tmp_ptr;
   REF_DESCR_PTR_TYPE   sd_ptr;
   INT     abschn1_anz, abschn2_anz, abschn_anz, 
           restAbschnAnz,     /* Anzahl noch zusaetzlich moeglicher Abschnitte*/
           aktAbschnAnz,      /* aktuelle Anzahl der Abschnitte               */
           sd_idx,            /* Index im Segmentdescriptorblock              */
           abschnMinAnz,      /* Mindestanzahl der Abschnitte                 */
           ii, jj;
   BOOL    gop_OX,            /* TRUE, wenn OX_XO oder OX_RECO                */
           xo_variante,       /* TRUE, wenn OX_XO oder PPX_XO                 */
           erg = TRUE;

   /* orderRekoXO */
   *res_ptr1 = NULL;
   *res_ptr2 = NULL;
#ifdef CHECKS
   if (org_ptr1 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 10, NO_INFO, NO_INFO, "", NIL_PTR);
   if (org_ptr2 == NULL)
      *ret_state = fatal (EVO_GO_RXO, 11, NO_INFO, NO_INFO, "", NIL_PTR);
   if (*ret_state != GLEAM_OK)
      return (FALSE);
   if ((org_ptr1->u_el.u_xel.u_hdr.chain_length != last_act) || 
       (org_ptr2->u_el.u_xel.u_hdr.chain_length != last_act)   )
      *ret_state = fatal (EVO_GO_RXO, 12, 0, last_act, "", CH_LEN_ZU_KLEIN);
   if ((genop < OX_XO ) || (PPX_RECO < genop))
      *ret_state = fatal (EVO_GO_RXO, 13, genop, NO_INFO, "orderRekoXO", 
                          BAD_G_OP_CODE_ERR);
   if (*ret_state != GLEAM_OK)
      return (FALSE);
#endif /* CHECKS */
   if (dyn_aks) {
      *ret_state = fatal (EVO_GO_RXO, 14, NO_INFO, NO_INFO, "OrderXO/Reko", 
                          DYN_AKS_UNSUITED);
      return (FALSE);
   }
   abschn1_anz = org_ptr1->u_el.u_xel.u_hdr.segment_anz;
   abschn2_anz = org_ptr2->u_el.u_xel.u_hdr.segment_anz;
#ifdef TEST
   sprintf (buf3, "\norderRekoXo: op=%d ElternAbschnAnz: AK1=%d AK2=%d", 
            genop, abschn1_anz, abschn2_anz);
   write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
   if ((genop == OX_XO ) || (genop == OX_RECO)) {
      abschnMinAnz = 3;
      gop_OX       = TRUE;
   }
   else {
      abschnMinAnz = 2;
      gop_OX       = FALSE;
   }
   xo_variante = (genop == OX_XO ) || (genop == PPX_XO);
   if (abschn1_anz >= abschnMinAnz)
      abschn_anz = abschn1_anz;
   else
   { /* AK1 hat weniger als abschnMinAnz Abschnitte */
      if (abschn2_anz >= abschnMinAnz) 
      { /* AK2 hat mind. abschnMinAnz Abschnitte: AKs vertauschen */
         tmp_ptr  = org_ptr1;
         org_ptr1 = org_ptr2;
         org_ptr2 = tmp_ptr;
         abschn_anz = abschn2_anz;
      } /* AK2 hat mind. abschnMinAnz Abschnitte: AKs vertauschen */
      else
         erg = FALSE;  /* beide AKs haben weniger als abschnMinAnz Abschnitte */
   } /* AK1 hat weniger als abschnMinAnz Abschnitte */
   if (erg)
   { /* AK1 hat jetzt mind. abschnMinAnz Segmente */
      sd_ptr = org_ptr1->u_el.u_xel.u_hdr.segm_descr_ptr;
      if (abschn_anz == abschnMinAnz) 
      { /* (abschnMinAnz-1) XO-Punkte und abschnMinAnz Sequenzen=Abschnitte */
         sequAnz = abschnMinAnz;
         for (ii = 0;  ii < abschnMinAnz;  ii++)
            sequLen[ii] = sd_ptr->segm_length[ii+1];
      } /* (abschnMinAnz-1) XO-Punkte und abschnMinAnz Sequenzen=Abschnitte */
      else
      { /* Mehr als abschnMinAnz Segmente: Sequenz-Anzahl u.-laengen bestimmen*/
         if (xo_variante)                     /* abschnMinAnz-1 XO-Punkte und */
            sequAnz = abschnMinAnz;           /* abschnMinAnz   Sequenzen     */
         else
            sequAnz = abschnMinAnz + irand (abschn_anz - abschnMinAnz + 1);
         restAbschnAnz = abschn_anz - sequAnz;
         sd_idx        = 1;
         for (ii = 0;  ii < (sequAnz - 1);  ii++) 
         { /* Sequenzlaengen bestimmen */
            if (restAbschnAnz > 0) { /* aktuelle Abschnittanzahl bestimmen */
               aktAbschnAnz   = 1 + irand (restAbschnAnz + 1);
               restAbschnAnz -= (aktAbschnAnz - 1);
            } /* aktuelle Abschnittanzahl bestimmen */
            else
               aktAbschnAnz = 1;
            sequLen[ii] = sd_ptr->segm_length[sd_idx];
            for (jj = 1;  jj < aktAbschnAnz;  jj++) { /* berechne "sequLen" */
               inc_segm_nr (&sd_ptr, &sd_idx);
               sequLen[ii] += sd_ptr->segm_length[sd_idx];
            } /* berechne "sequLen" */
            inc_segm_nr (&sd_ptr, &sd_idx);
         } /* Sequenzlaengen bestimmen */
         aktAbschnAnz = 1 + restAbschnAnz;         /* letzte Sequenz: Restab- */
         sequLen[sequAnz-1] = sd_ptr->segm_length[sd_idx]; /* schnitte nutzen!*/
         for (jj = 1;  jj < aktAbschnAnz;  jj++) { /* berechne "sequLen" */
            inc_segm_nr (&sd_ptr, &sd_idx);
            sequLen[sequAnz-1] += sd_ptr->segm_length[sd_idx];
         } /* berechne "sequLen" */
      } /* Mehr als abschnMinAnz Segmente: Sequenz-Anzahl u.-laengen bestimmen*/
#ifdef TEST
      sprintf (buf3, "  %d Sequenzen Len:", sequAnz);
      for (ii = 0;  ii < sequAnz;  ii++) {
         sprintf (buf2, " %d", sequLen[ii]);
         strcat (buf3, buf2);
      }
      write_to_log (buf3, CLOSE_LOG);
#endif /* TEST */
      if (gop_OX)
         if (make_OX_child (res_ptr1, org_ptr1, org_ptr2, ret_state))
            erg = make_OX_child (res_ptr2, org_ptr2, org_ptr1, ret_state);
         else
            erg = FALSE;
      else
         if (make_PPX_child (res_ptr1, org_ptr1, org_ptr2, ret_state))
            erg = make_PPX_child (res_ptr2, org_ptr2, org_ptr1, ret_state);
         else
            erg = FALSE;
   } /* AK1 hat jetzt mind. abschnMinAnz Segmente */
#ifdef TEST
   write_to_log ("orderRekoXo: ----------------------------------------------", CLOSE_LOG);
#endif /* TEST */
   return (erg);
} /* orderRekoXO */



/* ================================ rxo_init ================================ */
BOOL rxo_init (void)
{
   last_gruppe_size = SEGM_ANZ_MAX;
   return ((gruppe = (INT*)malloc (SEGM_ANZ_MAX * sizeof (INT))) != NULL);
} /* rxo_init */ 
