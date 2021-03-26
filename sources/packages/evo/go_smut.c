/*******************************************************************************
GLEAM/AE                                                                  V1.1.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: EVOlution
                        Module: Genetische Operatoren: Segment-MUTationen
 
Package: evo                    File: go_smut.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt alle segmentbezogenen Mutationen soweit es sich nicht um die 
im Modul "go_pmut" enthaltenen Parametermutationen handelt. Die zugehoerigen 
Funktionen fuehren ihre Operation wahrscheinlichkeitsgesteuert mindestens ein-
mal fuer ein Segment aus. 

Der Returnstatus wird GLEAM_FATAL bei auftretenden Fehlern. Das Bit "unmutierte_
urkette" im "chain_status" wird nicht gepflegt.

In den Schleifen zur Durchfuehrung der jeweiligen Mutation braucht kein Test
auf den Returnstatus zu erfolgen, da die Objektermittlungsroutine "ermittle_
abschnitt" bei schlechtem Status FALSE liefert und damit die Schleife abbricht.


Die Routinen im einzelnen:

GSTATE abschn_austausch (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein neuer Abschnitt gleicher Laenge ersetzt einen zufaellig bestimmten
          Abschnitt.

GSTATE abschn_verdoppeln (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein zufaellig bestimmter Abschnitt wird verdoppelt und als neuer Ab-
          schnitt hinter seinem Original eingefuegt.

GSTATE abschn_loeschen (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein zufaellig bestimmter Abschnitt wird geloescht.

GSTATE abschn_transl (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein zufaellig bestimmter Abschnitt wird innerhalb der Kette hinter
          einen zufaellig bestimmten anderen Abschnitt verschoben.

GSTATE abschn_inversion (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Die Aktionen eines zufaellig bestimmten Abschnitts werden in umgekehr-
          ter Reihenfolge angeordnet.

GSTATE nachb_segm_integr (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine per Zufall bestimmte Anzahl zufaellig bestimmter Segmente wird 
          mit dem jeweils nachfolgenden Segment verschmolzen. 

GSTATE segm_integr (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine per Zufall bestimmte Anzahl zufaellig bestimmter nicht notwendig
          benachbarter Segmentpaare wird verschmolzen. Dazu wird das ausgewahl-
          te Segment vor das Zielsegment geschoben und mit ihm verschmolzen.

GSTATE schiebe_segm_grenze (BOOL                *mut_done,
                            CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine per Zufall bestimmte Anzahl zufaellig bestimmter Segmentgrenzen 
          wird um eine zufaellig bestimmte Anzahl Kettelemente nach rechts/links
          verschoben. Bei Erreichen der naechsten Segmentgrenze wird mit dem
          Verschieben aufgehoert. Das ganze kann also jeweils eine Segmentver-
          schmelzung implizieren. Dazu muss die Kette ueber mindestens 2 Segmen-
          te verfuegen. Wird kein Segment veraendert, liefert die Routine in 
          "mut_done" FALSE ab, sonst TRUE.

GSTATE segm_teilung (BOOL                *mut_done,
                     CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine per Zufall bestimmte Anzahl Segmente wird an einer zufaellig ge-
          waehlten Stelle geteilt, sofern das Segment aus mehr als 2 Elementen
          besteht. Wird kein Segment veraendert, liefert die Routine in "mut_
          _done" FALSE ab, sonst TRUE.

GSTATE add_new_segm (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein neuer zufaellig generierter Abschnitt wird hinter einem zufaellig
          bestimmten Abschnitt eingefuegt.

--------------------------------------------------------------------------------
basiert auf den C-Units: xovr_mut.c von LESAK und xovr_mut.c von OPAL.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. "abschn_loeschen" wird nur bei Ketten mit mindestens 2 Segmenten ausgefuehrt.
 2. Bei allen anderen Routinen entfaellt die Pruefung auf "nicht leer", da dies 
    "mutations_zahl" bereits erledigt.
 3. Entfernung eines Includes im Kontext von GLEAM_TCL. Entfernung 
    ueberfluessiger Variable.                                        WJ 28.06.12
 4. Renovierung: Ersetzung von testweisen "write_line"-Ausgaben durch
    "printf"-Aufrufe und Anpassung der "lies_wert"-Aufrufe.          WJ 04.05.16
 5. Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".            WJ 30.06.17
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 30.06.2016
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

#undef MTEST
#undef ZTEST



/* ============================ abschn_austausch ============================ */
GSTATE abschn_austausch (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_start_ptr, seq_end_ptr, pred_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* abschn_austausch */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 1, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (SEGM_EXCHANGE, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "SAustausch: %d von %d:", anzahl, abschn_anz);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
      lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %d", abschn_nr);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, &sd_index, 
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_SMUT, 2, abschn_nr, NO_INFO, "", NO_ABSCHN));
      if (remove_seq (&seq_start_ptr, &seq_end_ptr, &abschn_len, chain_ptr, 
                      pred_ptr) != GLEAM_OK)
         return (GLEAM_FATAL);
      ret_state  = delete_seq  (seq_start_ptr, seq_end_ptr, abschn_len);
      ret_state |= gen_act_seq (&seq_start_ptr, &seq_end_ptr, abschn_len, 1);
      if (ret_state != GLEAM_OK)
         return (ret_state);
      ret_state = insert_seq (chain_ptr, pred_ptr, seq_start_ptr, 
                              seq_end_ptr, abschn_len);
      sd_ptr->segm_ptrs[sd_index] = seq_end_ptr;
   } /* mutiere einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* abschn_austausch */ 



/* ============================ abschn_verdoppeln =========================== */
GSTATE abschn_verdoppeln (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_start_ptr, seq_end_ptr, last_ptr, pred_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* abschn_verdoppeln */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 3, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (DOUBLE_SEGM, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "SDoppel: %d von %d:", anzahl, abschn_anz);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
      lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %d", abschn_nr);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &last_ptr, &abschn_len, &sd_index, 
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_SMUT, 4, abschn_nr, NO_INFO, "", NO_ABSCHN));
      if (copy_seq (&seq_start_ptr, &seq_end_ptr, pred_ptr->u_el.next_action,
                    abschn_len) != GLEAM_OK)
          return (GLEAM_FATAL);
      ret_state  = insert_segm_entry (chain_ptr, sd_ptr, sd_index, 
                                      seq_end_ptr, abschn_len, DAHINTER);
      ret_state |= insert_seq (chain_ptr, last_ptr, seq_start_ptr, 
                               seq_end_ptr, abschn_len);
   } /* mutiere einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* abschn_verdoppeln */ 



/* ============================= abschn_loeschen ============================ */
GSTATE abschn_loeschen (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_start_ptr, seq_end_ptr, pred_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* abschn_loeschen */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 5, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   if (abschn_anz > 1)
   { /* Kette hat mind. 2 Abschnitte */
      anzahl = mutations_zahl (DELETE_SEGM, abschn_anz);
#ifdef ZTEST
      sprintf (buf2, "SLoe: %d von %d:", anzahl, abschn_anz);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* mutiere einen Abschnitt */
#ifdef MTEST
         sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
         lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
         abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
         sprintf (buf1, " %d", abschn_nr);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, 
                                  &sd_index, &sd_ptr, chain_ptr, abschn_nr, 
                                  &ret_state))
            return (fatal (EVO_GO_SMUT, 6, abschn_nr, NO_INFO, "", NO_ABSCHN));
         ret_state  = remove_seq (&seq_start_ptr, &seq_end_ptr, &abschn_len, 
                                  chain_ptr, pred_ptr);
         ret_state |= delete_seq (seq_start_ptr, seq_end_ptr, abschn_len);
         ret_state |= remove_segm_entry (sd_ptr, sd_index, chain_ptr);
         abschn_anz--;
      } /* mutiere einen Abschnitt */
   } /* Kette hat mind. 2 Abschnitte */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* abschn_loeschen */ 



/* ============================== abschn_transl ============================= */
GSTATE abschn_transl (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_start_ptr, seq_end_ptr, last_ptr, pred_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, nach_abschn_nr,
                        anzahl, sd_index, wdummy, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* abschn_transl */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 7, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   if (abschn_anz > 1)
   { /* Kette hat mehr als einen Abschnitt */
      anzahl = mutations_zahl (SEGM_TRANSL, abschn_anz);
#ifdef ZTEST
      sprintf (buf2, "STransl: %d von %d:", anzahl, abschn_anz);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* mutiere einen Abschnitt */
#ifdef MTEST
         sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
         lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
         if (abschn_anz == 2)
            abschn_nr = 1;
         else
            abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
         sprintf (buf1, " %d", abschn_nr);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, 
                                  &sd_index, &sd_ptr, chain_ptr, abschn_nr, 
                                  &ret_state))
            return (fatal (EVO_GO_SMUT, 8, abschn_nr, NO_INFO, "", NO_ABSCHN));
         ret_state  = remove_seq (&seq_start_ptr, &seq_end_ptr, &abschn_len, 
                                  chain_ptr, pred_ptr);
         ret_state |= remove_segm_entry (sd_ptr, sd_index, chain_ptr);
         if (ret_state != GLEAM_OK)
            return (ret_state);
         abschn_anz--;
#ifdef MTEST
         sprintf (buf1, "Ziel-Abschnitt-Nr eingeben (0..%d):", abschn_anz);
         lies_wert (&nach_abschn_nr, buf1, 0, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
         if (abschn_anz == 1)
            nach_abschn_nr = 1;
         else
         {
            abschn_nr--;
            do
               nach_abschn_nr = irand (abschn_anz+1);
            while (nach_abschn_nr == abschn_nr);
         }
  #ifdef ZTEST
         sprintf (buf1, ", %d", nach_abschn_nr);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         if (nach_abschn_nr == 0)
         { /* vor den ersten Abschnitt */
            ret_state  = insert_segm_entry (chain_ptr,
                               chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr, 1, 
                               seq_end_ptr, abschn_len, DAVOR);
            ret_state |= insert_seq (chain_ptr, chain_ptr, seq_start_ptr, 
                                     seq_end_ptr, abschn_len);
         } /* vor den ersten Abschnitt */
         else
         { /* hinter irgendeinen Abschnitt */
            if (!ermittle_abschnitt (&pred_ptr, &last_ptr, &wdummy, &sd_index,
                                     &sd_ptr, chain_ptr, nach_abschn_nr, 
                                     &ret_state))
               return (fatal (EVO_GO_SMUT, 9, nach_abschn_nr, NO_INFO, "", 
                       NO_ABSCHN));
            ret_state  = insert_segm_entry (chain_ptr, sd_ptr, sd_index, 
                                            seq_end_ptr, abschn_len, DAHINTER);
            ret_state |= insert_seq (chain_ptr, last_ptr, seq_start_ptr, 
                                     seq_end_ptr, abschn_len);
         } /* hinter irgendeinen Abschnitt */
         abschn_anz++;
      } /* mutiere einen Abschnitt */
   } /* Kette hat mehr als einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* abschn_transl */ 



/* ============================ abschn_inversion ============================ */
GSTATE abschn_inversion (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  rem_ptr, last_ptr, pred_ptr, seq_end_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index,
                        ii, jj;
   GSTATE               ret_state = GLEAM_OK;
   
   /* abschn_inversion */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 10, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (SEGM_INVERSION, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "SInvers: %d von %d:", anzahl, abschn_anz);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
      lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %d", abschn_nr);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &last_ptr, &abschn_len, &sd_index,
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_SMUT, 11, abschn_nr, NO_INFO, "", NO_ABSCHN));
      /*  dirty trick: Unterdrueckung d.Segmentpflege beim Aktionsumhaengen:  */
      seq_end_ptr = pred_ptr->u_el.next_action;
      chain_ptr->u_el.u_xel.u_hdr.segment_anz = 0;     /* perform dirty trick */
      for (jj = 2;  jj <= abschn_len;  jj++)
      { /* invertieren */
         ret_state  = remove_next_item (&rem_ptr, chain_ptr, pred_ptr);
         ret_state |= insert_item (chain_ptr, last_ptr, rem_ptr);
         if (ret_state != GLEAM_OK)
            break;                                            /* abandon loop */
      } /* invertieren */
      sd_ptr->segm_ptrs[sd_index] = seq_end_ptr;  /*update segment end pointer*/
      chain_ptr->u_el.u_xel.u_hdr.segment_anz = abschn_anz;   /* repair trick */
   } /* mutiere einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* abschn_inversion */ 



/* ============================ nachb_segm_integr =========================== */
GSTATE nachb_segm_integr (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_end_ptr,    pred_ptr;
   INT                  abschn_anz,     abschn_len, abschn_nr,
                        anzahl,         sd_index,   ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* nachb_segm_integr */
#ifdef CHECK
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 12, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   if (abschn_anz >= 2)
   { /* Kette hat mindestens zwei Abschnitte */
      anzahl = mutations_zahl (INTEGR_NACHB_SEGM, abschn_anz-1);
#ifdef ZTEST
      sprintf (buf2, "NBSegmIntegr: %d von %d:", anzahl, abschn_anz);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* mutiere einen Abschnitt */
#ifdef MTEST
         sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", (abschn_anz-1));
         lies_wert (&abschn_nr, buf1, 1, abschn_anz-1, abschn_anz);
#else /* kein MTEST */
         if (abschn_anz == 2)
            abschn_nr = 1;                                 /* trivial         */
         else
            abschn_nr = irand (abschn_anz - 1) + 1;        /* 1..abschn_anz-1 */
  #ifdef ZTEST
         sprintf (buf1, " %d", abschn_nr);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         
         /* ---------- Abschnitt mit Folgeabschnitt verschmelzen: ----------- */
         if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, 
                                  &sd_index,&sd_ptr, chain_ptr, abschn_nr, 
                                  &ret_state))
            return (fatal (EVO_GO_SMUT, 13, abschn_nr, NO_INFO, "", NO_ABSCHN));
         ret_state = remove_segm_entry (sd_ptr, sd_index, chain_ptr);
         (sd_ptr-> segm_length[sd_index]) += abschn_len;
         abschn_anz--;
      } /* mutiere einen Abschnitt */
   } /* Kette hat mindestens zwei Abschnitte */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* nachb_segm_integr */ 



/* =============================== segm_integr ============================== */
GSTATE segm_integr (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_start_ptr,  seq_end_ptr,
                        last_ptr,       pred_ptr;
   INT                  abschn_anz,     abschn_len,
                        abschn_nr,      nach_abschn_nr,
                        anzahl,         sd_index,
                        dummy,          ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* segm_integr */
#ifdef CHECK
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 14, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   if (abschn_anz >= 2)
   { /* Kette hat mindestens zwei Abschnitte */
      anzahl = mutations_zahl (INTEGR_SEGM, abschn_anz-1);
#ifdef ZTEST
      sprintf (buf2, "SegmIntegr: %d von %d:", anzahl, abschn_anz);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* mutiere einen Abschnitt */
#ifdef MTEST
         sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
         lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz + 1);
         sprintf (buf1, "Ziel-Abschnitt-Nr eingeben (0..%d):", abschn_anz-2);
         lies_wert (&nach_abschn_nr, buf1, 0, abschn_anz-2, abschn_anz);
#else /* kein MTEST */
         if (abschn_anz == 2)
         { /* trivial */
            abschn_nr      = 2;
            nach_abschn_nr = 0;
         } /* trivial */
         else
         { /* auswuerfeln */
            abschn_nr      = irand (abschn_anz) + 1;       /* 1..abschn_anz   */
            nach_abschn_nr = irand (abschn_anz-1);         /* 0..abschn_anz-2 */
         } /* auswuerfeln */
  #ifdef ZTEST
         sprintf (buf1, " %d,%d", abschn_nr, nach_abschn_nr);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */

         /* --------------------- Abschnitt entfernen: ---------------------- */
         if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, 
                                  &sd_index, &sd_ptr, chain_ptr, abschn_nr, 
                                  &ret_state))
            return (fatal (EVO_GO_SMUT, 15, abschn_nr, NO_INFO, "", NO_ABSCHN));
         ret_state  = remove_seq (&seq_start_ptr, &seq_end_ptr, &abschn_len, 
                                  chain_ptr, pred_ptr);
         ret_state |= remove_segm_entry (sd_ptr, sd_index, chain_ptr);
         
         /* -- Abschn an den Anfang eines Abschnitts in diesen integrieren: - */
         if (nach_abschn_nr == 0)
         { /* vor den ersten Abschnitt */
            last_ptr = chain_ptr;
            sd_ptr   = chain_ptr-> u_el.u_xel.u_hdr.segm_descr_ptr;
            sd_index = 0;
         } /* vor den ersten Abschnitt */
         else if (!ermittle_abschnitt (&pred_ptr, &last_ptr, &dummy, &sd_index,
                                       &sd_ptr, chain_ptr, nach_abschn_nr, 
                                       &ret_state))
            return (fatal (EVO_GO_SMUT, 16, nach_abschn_nr, NO_INFO, "", 
                           NO_ABSCHN));
         ret_state  = insert_seq (chain_ptr, last_ptr, seq_start_ptr, 
                                  seq_end_ptr, abschn_len);
         ret_state |= inc_segm_nr (&sd_ptr, &sd_index);
         (sd_ptr->segm_length[sd_index]) += abschn_len;
         abschn_anz--;
      } /* mutiere einen Abschnitt */
   } /* Kette hat mindestens zwei Abschnitte */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* segm_integr */ 



/* =========================== schiebe_segm_grenze ========================== */
GSTATE schiebe_segm_grenze (BOOL                *mut_done,
                            CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr,         sd2_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_end_ptr,    pred_ptr;
   INT                  abschn_anz,     abschn_len, abschn_nr,
                        anzahl,
                        sd_index,       sd2_index,
                        schieb_anz,     kor_schieb_anz,
                        pred_segm_len,  ii, ij;
   BOOL                 rueckwaerts;
   GSTATE               ret_state = GLEAM_OK;
   BOOL                 done      = FALSE;
   
   /* schiebe_segm_grenze */
   *mut_done = FALSE;
#ifdef CHECK
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 17, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   if (abschn_anz >= 2)
   { /* Kette hat mindestens zwei Abschnitte */
      anzahl = mutations_zahl (SCHIEB_SEGM_GRENZ, abschn_anz-1);
#ifdef ZTEST
      sprintf (buf2, "SGrenzSchieb: %d von %d:", anzahl, abschn_anz);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* mutiere einen Abschnitt */
#ifdef MTEST
         sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz-1);
         lies_wert (&abschn_nr, buf1, 1, abschn_anz-1, abschn_anz);
#else /* kein MTEST */
         if (abschn_anz == 2)
            abschn_nr = 1;                               /* trivial           */
         else
            abschn_nr = irand (abschn_anz - 1) + 1;      /* 1 .. abschn_anz-1 */
  #ifdef ZTEST
         sprintf (buf1, " %d", abschn_nr);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         
         /* -------- Abschnittsgrenze nach rechts/links verschieben: -------- */
         if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len,
                                  &sd_index, &sd_ptr, chain_ptr, abschn_nr,
                                  &ret_state))
            return (fatal (EVO_GO_SMUT, 18, abschn_nr, NO_INFO, "", NO_ABSCHN));
         sd2_ptr   = sd_ptr;
         sd2_index = sd_index;
         inc_segm_nr (&sd2_ptr, &sd2_index);    /* geht gut, da max.vorl.Segm */
#ifdef MTEST
         lies_wert (&ij, "Rueckw/Vorw (0/1) [1]: ", 0, 1, 1);
#else /* kein MTEST */
         ij = irand (2);
#endif /* kein MTEST */
         if (ij == 1)
         { /* Schiebe vorwaerts */
            rueckwaerts   = FALSE;
            pred_segm_len = abschn_len;
            abschn_len    = sd2_ptr-> segm_length[sd2_index];
            pred_ptr      = seq_end_ptr;
#ifdef MTEST
            sprintf (buf1, "Schieb-Anz (1..%d): ", abschn_len);
            lies_wert (&schieb_anz, buf1, 1, abschn_len, abschn_len + 1);
#else /* kein MTEST */
            schieb_anz = irand (abschn_len) + 1;           /* 1 .. abschn_len */
#endif /* kein MTEST */
            kor_schieb_anz = schieb_anz;
#ifdef ZTEST
            strcat (buf2, "/V: ");
#endif
         } /* Schiebe vorwaerts */
         else
         { /* Schiebe rueckwaerts */
            rueckwaerts = TRUE;
#ifdef MTEST
            sprintf (buf1, "Schieb-Anz (1..%d): ", abschn_len);
            lies_wert (&schieb_anz, buf1, 1, abschn_len, abschn_len + 1);
#else /* kein MTEST */
            schieb_anz = irand (abschn_len) + 1;           /* 1 .. abschn_len */
#endif /* kein MTEST */
            kor_schieb_anz = abschn_len - schieb_anz;
#ifdef ZTEST
            strcat (buf2, "/R: ");
#endif
         } /* Schiebe rueckwaerts */
         if (schieb_anz == abschn_len)
         { /* rechte oder linke Segmentgrenze erreicht */
            ret_state = remove_segm_entry (sd_ptr, sd_index, chain_ptr);
            if (!rueckwaerts)
               schieb_anz = pred_segm_len;
            (sd_ptr-> segm_length[sd_index]) += schieb_anz;
            abschn_anz--;
#ifdef ZTEST
            strcat (buf2, "G,");
#endif
         } /* rechte oder linke Segmentgrenze erreicht */
         else
         { /* Schieben */
            for (ij = 1;  ij <= kor_schieb_anz;  ij++)
               pred_ptr = pred_ptr-> u_el.next_action;
            sd_ptr-> segm_ptrs[sd_index] = pred_ptr;
            if (rueckwaerts)
            {
               (sd_ptr-> segm_length[sd_index])   -= schieb_anz;
               (sd2_ptr-> segm_length[sd2_index]) += schieb_anz;
            }
            else
            { /* vorwaerts */
               (sd_ptr-> segm_length[sd_index])   += schieb_anz;
               (sd2_ptr-> segm_length[sd2_index]) -= schieb_anz;
            } /* vorwaerts */
#ifdef ZTEST
            sprintf (buf1, "%d,", schieb_anz);
            strcat (buf2, buf1);
#endif
         } /* Schieben */
         done = TRUE;
      } /* mutiere einen Abschnitt */
      *mut_done = done && (ret_state == GLEAM_OK);
   } /* Kette hat mindestens zwei Abschnitte */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* schiebe_segm_grenze */ 



/* ============================== segm_teilung ============================== */
GSTATE segm_teilung (BOOL                *mut_done,
                     CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  last_ptr,       pred_ptr,
                        teil1_end_ptr;
   INT                  abschn_anz,     abschn_len,
                        abschn_nr,      teil1_len,
                        anzahl,         sd_index,
                        ii;
   GSTATE               ret_state = GLEAM_OK;
   BOOL                 done      = FALSE;
   
   /* segm_teilung */
   *mut_done = FALSE;
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 19, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (SEGM_INVERSION, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "STeil: %d von %d:", anzahl, abschn_anz);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
      lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;                  /* 1 .. abschn_anz */
  #ifdef ZTEST
      sprintf (buf1, " %d", abschn_nr);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &last_ptr, &abschn_len, &sd_index,
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_SMUT, 20, abschn_nr, NO_INFO, "", NO_ABSCHN));
      if (abschn_len > 1)
      { /* Abschnitt gross genug zum Teilen */
#ifdef MTEST
         sprintf (buf1, "Teile hinter Elem (1..%d):", abschn_len - 1);
         lies_wert (&teil1_len, buf1, 1, abschn_len - 1, abschn_len);
#else /* kein MTEST */
         if (abschn_len == 2)
            teil1_len = 1;                               /* trivial           */
         else 
            teil1_len = irand (abschn_len - 1) + 1;      /* 1 .. abschn_len-1 */
  #ifdef ZTEST
         sprintf (buf1, "(%d,%d)", teil1_len, abschn_len - teil1_len);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         if (!count_out_item (&teil1_end_ptr, pred_ptr, teil1_len, &ret_state))
            return (fatal (EVO_GO_SMUT, 21, teil1_len, NO_INFO, "",NO_CH_ITEM));
         sd_ptr->segm_length[sd_index] = abschn_len - teil1_len;
         ret_state = insert_segm_entry (chain_ptr, sd_ptr, sd_index, 
                                        teil1_end_ptr, teil1_len, DAVOR);
         done = TRUE;
      } /* Abschnitt gross genug zum Teilen */
   } /* mutiere einen Abschnitt */
   *mut_done = done && (ret_state == GLEAM_OK);
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* segm_teilung */ 



/* ============================== add_new_segm ============================== */
GSTATE add_new_segm (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  seq_start_ptr, seq_end_ptr, pred_ptr, last_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index, ii;
   GSTATE               ret_state = GLEAM_OK;

   /* add_new_segm */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_SMUT, 22, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (ADD_NEW_SEGM, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "AddNewSegm: Ist:%d Neu:%d:", abschn_anz, anzahl);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben (1..%d):", abschn_anz);
      lies_wert (&abschn_nr, buf1, 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %d", abschn_nr);
      strcat (buf2, buf1);
  #endif /* ZTEST */
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &last_ptr, &abschn_len, &sd_index, 
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_SMUT, 23, abschn_nr, NO_INFO, "", NO_ABSCHN));
      abschn_len = calc_segm_len (ak_max_segm_len+1);
      ret_state |= gen_act_seq (&seq_start_ptr, &seq_end_ptr, abschn_len, 1);
      if (ret_state != GLEAM_OK)
         return (ret_state);
      ret_state  = insert_segm_entry (chain_ptr, sd_ptr, sd_index, 
                                      seq_end_ptr, abschn_len, DAHINTER);
      ret_state |= insert_seq (chain_ptr, last_ptr, seq_start_ptr, 
                               seq_end_ptr, abschn_len);
   } /* mutiere einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* add_new_segm */
