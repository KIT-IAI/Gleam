/*******************************************************************************
GLEAM/AE                                                                  V1.1.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: EVOlution
                        Module: Genetische Operatoren: Aktions-MUTationen
 
Package: evo                    File: go_amut.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt alle aktionsbezogenen Mutationen soweit es sich nicht um die 
im Modul "go_pmut" enthaltenen Parametermutationen handelt. Die zugehoerigen 
Funktionen fuehren ihre Operation wahrscheinlichkeitsgesteuert mindestens ein-
mal fuer eine Aktion aus. 

Der Returnstatus wird GLEAM_FATAL bei auftretenden Fehlern. Das Bit "unmutierte_
urkette" im "chain_status" wird nicht gepflegt.

In den Schleifen zur Durchfuehrung der jeweiligen Mutation braucht kein Test
auf den Returnstatus zu erfolgen, da die Objektermittlungsroutine "count_out_
item" bei schlechtem Status FALSE liefert und damit die Schleife abbricht.


Die Routinen im einzelnen:

GSTATE plus_neue_aktion (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine oder mehrere neue Aktionen werden der Kette an zufaellig bestimm-
          ter Stelle unter Erweiterung des jeweiligen Segments hinzugefuegt. Bei
          einer leeren AK entsteht dabei ein neues Segment.

GSTATE aktions_austausch (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine oder mehrere zufaellig bestimmte Aktionen der Kette werden durch 
          neu generierte ausgetauscht.

GSTATE aktion_verdoppeln(CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine oder mehrere zufaellig bestimmte Aktionen der Kette werden ver-
          dopppelt. Die Kopie wird hinter dem Original unter Erweiterung des 
          jeweiligen Segments eingefuegt.

GSTATE aktion_loeschen (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine oder mehrere zufaellig bestimmte Aktionen der Kette werden ge-
          loescht. Falls dabei ein einelementiger Abschnitt geloescht wurde, hat
          die Kette einen Abschnitt weniger. Dieser Fall kann im Laufe der Evo-
          lution entstehen.

GSTATE aktions_transl (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Eine oder mehrere zufaellig bestimmte Aktionen der Kette werden inner-
          halb der Kette an zufaellig bestimmte Stellen verschoben. Dabei koen-
          nen Segmente verkleinert, geloescht und vergroessert werden.

--------------------------------------------------------------------------------
basiert auf C-Unit: xovr_mut.c von LESAK.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. "aktion_loeschen" wird nur bei Ketten mit mindestens 2 Elementen ausgefuehrt.
 2. Bei allen anderen Routinen entfaellt die Pruefung auf "nicht leer", da dies 
    "mutations_zahl" bereits erledigt.
 3. Entfernung eines Includes im Kontext von GLEAM_TCL.              WJ 22.06.12
 4. Renovierung: Ersetzung von testweisen "write_line"-Ausgaben durch
    "printf"-Aufrufe und Anpassung der "lies_wert"-Aufrufe.          WJ 04.05.16
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

#undef MTEST
#undef ZTEST



/* ============================ plus_neue_aktion ============================ */
GSTATE plus_neue_aktion (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  new_ptr, dummy_ptr, this_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* plus_neue_aktion */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_AMUT, 1, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   anzahl = mutations_zahl (ADD_NEW_ACT, len);
#ifdef ZTEST
   sprintf (buf2, "APlusNeu: %d von %d:", anzahl, len);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere eine Aktion */
#ifdef MTEST
      sprintf (buf1, "chain_index eingeben (0..%d): ", len);
      lies_wert (&c_index, buf1, 0, len, len+1);
#else /* kein MTEST */
      c_index = irand (len + 1);
  #ifdef ZTEST
      sprintf (buf1, " %d", c_index);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!count_out_item (&this_ptr, chain_ptr, c_index, &ret_state))
         return (fatal (EVO_GO_AMUT, 2, c_index, NO_INFO, "", NO_CH_ITEM));
      ret_state  = gen_act_seq (&new_ptr, &dummy_ptr, 1, 1);
      ret_state |= insert_item (chain_ptr, this_ptr, new_ptr);
      if (ret_state != GLEAM_OK)
         return (ret_state);
      if (len == 0)
      { /* ein neues Segment */
         sd_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
         if (sd_ptr == NULL)
         { /* Segmentdescriptor anlegen */
            if (new_ref_descr (&sd_ptr) != GLEAM_OK)
               return (GLEAM_FATAL);
            chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = sd_ptr;
            sd_ptr->segm_ptrs[0]                       = chain_ptr;
         } /* Segmentdescriptor anlegen */
         sd_ptr->segm_ptrs  [1] = new_ptr;
         sd_ptr->segm_length[1] = 1;
         chain_ptr->u_el.u_xel.u_hdr.segment_anz = 1;
      } /* ein neues Segment */
      len++;
   } /* mutiere eine Aktion */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* plus_neue_aktion */ 



/* ============================ aktions_austausch =========================== */
GSTATE aktions_austausch (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  new_ptr, dummy_ptr, this_ptr, del_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* aktions_austausch */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_AMUT, 3, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   anzahl = mutations_zahl (ACT_EXCHANGE, len);
#ifdef ZTEST
   sprintf (buf2, "AAust: %d von %d:", anzahl, len);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere eine Aktion */
#ifdef MTEST
      sprintf (buf1, "chain_index eingeben (0..%d):", len-1);
      lies_wert (&c_index, buf1, 0, len-1, len);
#else /* kein MTEST */
      c_index = irand (len);
  #ifdef ZTEST
      sprintf (buf1, " %d", c_index);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!count_out_item (&this_ptr, chain_ptr, c_index, &ret_state))
         return (fatal (EVO_GO_AMUT, 4, c_index, NO_INFO, "", NO_CH_ITEM));
      /* ----- Nachstehende Reihenfolge bewirkt, dass ein einelementiges ---- */
      /* ----- Segment meistens erhalten bleibt:                         ---- */
      if (gen_act_seq (&new_ptr, &dummy_ptr, 1, 1) != GLEAM_OK)
         return (ret_state);
      ret_state  = insert_item (chain_ptr, this_ptr, new_ptr);
      this_ptr   = this_ptr->u_el.next_action;
      ret_state |= remove_next_item (&del_ptr, chain_ptr, this_ptr);
      ret_state |= delete_item (del_ptr);
   } /* mutiere eine Aktion */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* aktions_austausch */ 



/* ============================ aktion_verdoppeln =========================== */
GSTATE aktion_verdoppeln (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  this_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* aktion_verdoppeln */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_AMUT, 5, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   anzahl = mutations_zahl (DOUBLE_ACT, len);
#ifdef ZTEST
   sprintf (buf2, "ADoppel: %d von %d:", anzahl, len);
#endif
   for (ii = 0;  ii < anzahl;  ii++)
   { /* mutiere eine Aktion */
#ifdef MTEST
      sprintf (buf1, "chain_index eingeben (1..%d):", len);
      lies_wert (&c_index, buf1, 1, len, len+1);
#else /* kein MTEST */
      c_index = irand (len) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %d", c_index);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!count_out_item (&this_ptr, chain_ptr, c_index, &ret_state))
         return (fatal (EVO_GO_AMUT, 6, c_index, NO_INFO, "", NO_CH_ITEM));
      ret_state = copy_item (chain_ptr, this_ptr, this_ptr);
      len++;
   } /* mutiere eine Aktion */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* aktion_verdoppeln */ 



/* ============================= aktion_loeschen ============================ */
GSTATE aktion_loeschen (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  old_ptr, pred_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* aktion_loeschen */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_AMUT, 7, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   len = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   if (len > 1)
   { /* Kette mit mind. 2 Elementen */
      anzahl = mutations_zahl (DELETE_ACT, len);
#ifdef ZTEST
      sprintf (buf2, "ALoe: %d von %d:", anzahl, len);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* mutiere eine Aktion */
#ifdef MTEST
         sprintf (buf1, "chain_index eingeben (0..%d):", (len-1));
         lies_wert (&c_index, buf1, 0, len-1, len);
#else /* kein MTEST */
         c_index = irand (len);
  #ifdef ZTEST
         sprintf (buf1, " %d", c_index);
         strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
         if (!count_out_item (&pred_ptr, chain_ptr, c_index, &ret_state))
            return (fatal (EVO_GO_AMUT, 8, c_index, NO_INFO, "", NO_CH_ITEM));
         if (remove_next_item (&old_ptr, chain_ptr, pred_ptr) == GLEAM_OK)
            ret_state = delete_item (old_ptr);
         len--;
      } /* mutiere eine Aktion */
   } /* Kette mit mind. 2 Elementen */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* aktion_loeschen */ 



/* ============================= aktions_transl ============================= */
GSTATE aktions_transl (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  nach_ptr, pred_von_ptr, von_ptr;
   INT                  von_index, nach_index, test_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* aktions_transl */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_AMUT, 9, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   len = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   if (len > 1)
   { /* Kette hat mindestens 2 Elemente */
      anzahl = mutations_zahl (ACT_TRANSLOCATION, len);
#ifdef ZTEST
      sprintf (buf2, "ATransl: %d von %d:", anzahl, len);
#endif
      for (ii = 0;  ii < anzahl;  ii++)
      { /* verschiebe eine Aktion */
#ifdef MTEST
         sprintf (buf2, "von_index (0..%d) nach_index (0..%d):", (len-1), len);
         printf ("%s\n", buf2);
         lies_wert (&von_index, "von_index:  ", 0, len-1, len);
         lies_wert (&nach_index, "nach_index: ", 0, len, len+1);
#else /* kein MTEST */
         if (len == 2)
         { /* trivial */
            von_index  = 1;
            nach_index = 0;
         } /* trivial */
         else
         { /* auswuerfeln */
            von_index  = irand (len);
            test_index = von_index + 1;
            do
               nach_index = irand (len + 1);
            while (!((nach_index != von_index) && (nach_index != test_index)));
         } /* auswuerfeln */
  #ifdef ZTEST
         sprintf (buf1, "  %d %d", von_index, nach_index);
         strcat (buf2, buf1);
  #endif /* ZTEST */
#endif /* kein MTEST */
         if (!count_out_item (&pred_von_ptr, chain_ptr, von_index, &ret_state))
            return (fatal (EVO_GO_AMUT, 10, von_index, NO_INFO, "", 
                           NO_CH_ITEM));
         if (!count_out_item (&nach_ptr, chain_ptr, nach_index, &ret_state))
            return (fatal (EVO_GO_AMUT, 11, nach_index, NO_INFO, "", 
                           NO_CH_ITEM));
         ret_state  = remove_next_item (&von_ptr, chain_ptr, pred_von_ptr);
         ret_state |= insert_item (chain_ptr, nach_ptr, von_ptr);
      } /* verschiebe eine Aktion */
   } /* Kette hat mindestens 2 Elemente */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* aktions_transl */ 
