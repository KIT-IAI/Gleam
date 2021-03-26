/*******************************************************************************
GLEAM/AE                                                                  V1.1.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: EVOlution
                        Module: Genetische Operatoren: Parameter-MUTationen
 
Package: evo                    File: go_pmut.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
 
Der Modul enthaelt alle genetischen Operatoren, die die Parameter einer einzel-
nen Aktion oder der Aktionen eines ganzen Segments veraendern. 

Die zugehoerigen Funktionen fuehren ihre Operation wahrscheinlichkeitsgesteuert 
mindestens einmal fuer eine Aktion bzw. Segment aus. Dabei werden "activity_
act"-Aktionen ignoriert. Aktionen und Segmente, die nur aus "activity_act"-
Aktionen bestehen, werden ausser bei den beiden ".._par_aend"-Funktionen nicht 
mitgezaehlt. Daher muessen Ketten mindestens eine echte Aktion enthalten; sie 
sollten mehr echte Aktionen enthalten, als der groesste Wert, den "mutations_
zahl" liefern kann.

Der Returnstatus wird GLEAM_FATAL bei auftretenden Fehlern. Das Bit "unmutierte_
urkette" im "chain_status" wird nicht gepflegt.

In den Schleifen zur Durchfuehrung der jeweiligen Mutation braucht kein Test
auf den Returnstatus zu erfolgen, da die Objektermittlungsroutinen "count_out_
item" und "ermittle_abschnitt" bei schlechtem Status FALSE liefern und damit 
die Schleife abbrechen.

 
Die Routinen im einzelnen:

GSTATE akt_par_aend (BOOL                *mut_done,
                     INT                  mod_kind,
                     CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein oder mehrere Parameter einer ausgewuerfelten Anzahl von Aktionen 
          werden veraendert, wobei die Paramterunter- und -obergrenzen eingehal-
          ten werden. Wenn nichts veraendert wird, liefert die Routine in "mut_
          done" FALSE ab, sonst TRUE.
          Die Aenderung wird von "mod_kind" gesteuert und geht vom aktuellen 
          Wert aus. Bei CHANGE_REL erstreckt sich der Aenderungsbereich von der
          Unter- bis zur Obergrenze. Bei CHANGE_SMALL findet die Aenderung nur
          innerhalb eines kleinen Bruchteils (siehe SMALL_CHANGE_FRAC aus Modul 
          "mod_data" im Package "hmod") des gesamten Werteintervalls statt. Ge-
          aendert wird mit Hilfe der Routine "modify_param" aus Modul "segm_
          gen", Package "hmod".

GSTATE akt_par_neu (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Jeweils ein Parameter einer ausgewuerfelten Anzahl von Aktionen wird 
          neu bestimmt, wobei alle Werte des Wertebereichs gleich wahrscheinlich
          sind.

GSTATE akt_neu (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Alle Parameter einer ausgewuerfelten Anzahl von Aktionen werden neu 
          bestimmt, wobei alle Werte des Wertebereichs gleich wahrscheinlich 
          sind.

GSTATE segm_par_aend (BOOL                *mut_done,
                      CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Ein oder mehrere Parameter aller Aktionen einer (od. mehrerer) zufael- 
          lig bestimmter Segmente werden veraendert, wobei die Paramterunter- 
          und -obergrenzen eingehalten werden. Wenn nichts veraendert wird, lie-
          fert die Routine in "mut_done" FALSE ab, sonst TRUE.
          Die Art der Aenderung erfolgt wie in "akt_spar_aend" beschrieben.
          
GSTATE segm_neu (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Alle Parameter aller Aktionen einer (oder mehrerer) zufaellig bestimm-
          ter Segmente werden neu bestimmt, wobei alle Werte des Wertebereichs 
          gleich wahrscheinlich sind. 
    
--------------------------------------------------------------------------------
basiert auf C-Unit: go_pmut
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Erweiterung der Routinen "akt_par_aend" und "segm_par_aend".
 2. Entfernung eines Includes im Kontext von GLEAM_TCL.              WJ 22.06.12
 3. Renovierung: Ersetzung von testweisen "write_line"-Ausgaben durch
    "printf"-Aufrufe und Anpassung der "lies_wert"-Aufrufe.          WJ 04.05.16
 4. Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".            WJ 30.06.17
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




/* ============================== akt_par_aend ============================== */
GSTATE akt_par_aend (BOOL                *mut_done,
                     INT                  mod_kind,
                     CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  this_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   BOOL                 loc_mut   = FALSE;
   
   /* akt_par_aend */
   *mut_done = FALSE;
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_PMUT, 1, 0, 0, "", NIL_PTR));
#endif
   len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   anzahl = mutations_zahl (PAR_CHANGE_REL, len);
#ifdef ZTEST
   sprintf (buf2, "AParAend: %d von %d:", anzahl, len);
#endif
   for (ii = 1;  ii <= anzahl;  ii++)
   { /* mutiere eine Aktion */
#ifdef MTEST
      sprintf (buf1, "chain_index eingeben [1..%1u]:", len);
      printf ("%s\n", buf1);
      lies_wert (&c_index, "Index: ", 1, len, len+1);
#else /* kein MTEST */
      c_index = irand (len) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %d", c_index);
      strcat (buf2, buf1);
  #endif
#endif /* kein MTEST */
      if (!count_out_item (&this_ptr, chain_ptr, c_index, &ret_state))
         return (fatal (EVO_GO_PMUT, 2, c_index, 0, "", NO_CH_ITEM));
      loc_mut   = modify_param (this_ptr, mod_kind, &ret_state);
      *mut_done = *mut_done || loc_mut;
   } /* mutiere eine Aktion */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* akt_par_aend */ 



/* =============================== akt_par_neu ============================== */
GSTATE akt_par_neu (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  this_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* akt_par_neu */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_PMUT, 3, 0, 0, "", NIL_PTR));
#endif
   len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   anzahl = mutations_zahl (PAR_CHANGE_NEW, len);
#ifdef ZTEST
   sprintf (buf2, "AParNeu: %d von %d:", anzahl, len);
#endif
   for (ii = 1;  ii <= anzahl;  ii++)
   { /* mutiere eine Aktion */
#ifdef MTEST
      sprintf (buf1, "chain_index eingeben [1..%1u]:", len);
      printf ("%s\n", buf1);
      lies_wert (&c_index, "Index: ", 1, len, len+1);
#else /* kein MTEST */
      c_index = irand (len) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %1u", c_index);
      strcat (buf2, buf1);
  #endif /* ZTEST */
#endif /* kein MTEST */
      if (!count_out_item (&this_ptr, chain_ptr, c_index, &ret_state))
         fatal (EVO_GO_PMUT, 4, c_index, 0, "", NO_CH_ITEM);
      if (!modify_param (this_ptr, CHANGE_NEW, &ret_state))
         ii--;                             /* "activity_act" nicht mitzaehlen */
   } /* mutiere eine Aktion */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* akt_par_neu */ 



/* ================================= akt_neu ================================ */
GSTATE akt_neu (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  this_ptr;
   INT                  c_index, anzahl, len, ii;
   GSTATE               ret_state = GLEAM_OK;
   
   /* akt_neu */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_PMUT, 5, 0, 0, "", NIL_PTR));
#endif
   len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
   anzahl = mutations_zahl (ACT_NEW_PARAM, len);
#ifdef ZTEST
   sprintf (buf2, "ANeu: %d von %d:", anzahl, len);
#endif
   for (ii = 1;  ii <= anzahl;  ii++)
   { /* mutiere eine Aktion */
#ifdef MTEST
      sprintf (buf1, "chain_index eingeben [1..%1u]:", len);
      printf ("%s\n", buf1);
      lies_wert (&c_index, "Index: ", 1, len, len+1);
#else /* kein MTEST */
      c_index = irand (len) + 1;
  #ifdef ZTEST
      sprintf (buf1, " %1u", c_index);
      strcat (buf2, buf1);
  #endif /* ZTEST */
#endif /* kein MTEST */
      if (!count_out_item (&this_ptr, chain_ptr, c_index, &ret_state))
         fatal (EVO_GO_PMUT, 6, c_index, 0, "", NO_CH_ITEM);
      if (!parameterize (this_ptr, &ret_state))
         ii--;                             /* "activity_act" nicht mitzaehlen */
   } /* mutiere eine Aktion */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* akt_neu */ 



/* ============================== segm_par_aend ============================= */
GSTATE segm_par_aend (BOOL                *mut_done,
                      INT                  mod_kind,
                      CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  act_ptr, seq_end_ptr, pred_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index,
                        ii, jj;
   GSTATE               ret_state = GLEAM_OK;
   BOOL                 loc_mut;
   
   /* segm_par_aend */
   *mut_done = FALSE;
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_PMUT, 7, 0, 0, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (SEGM_CHANGE_REL, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "SParAend: %d von %d:", anzahl, abschn_anz);
#endif
   for (ii = 1;  ii <= anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben [1..%1u]:", abschn_anz);
      printf ("%s\n", buf1);
      lies_wert (&abschn_nr, "Nr: ", 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
      sprintf (buf1, "%1u", abschn_nr);
      sprintf (buf2, "%s %s", buf2, buf1);
  #endif /* ZTEST */
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, &sd_index,
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_PMUT, 8, abschn_nr, 0, "", NO_ABSCHN));
      act_ptr = pred_ptr->u_el.next_action;
      for (jj = 1;  (jj <= abschn_len) && (ret_state == GLEAM_OK);  jj++)
      { /* jede Aktion des Abschnitts */
         loc_mut   = modify_param (act_ptr, mod_kind, &ret_state);
         *mut_done = *mut_done || loc_mut;
         act_ptr   = act_ptr->u_el.next_action;
      } /* jede Aktion des Abschnitts */
   } /* mutiere einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* segm_par_aend */ 



/* ================================ segm_neu ================================ */
GSTATE segm_neu (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   REF_DESCR_PTR_TYPE   sd_ptr;
   CHAIN_ELEM_PTR_TYPE  act_ptr, seq_end_ptr, pred_ptr;
   INT                  abschn_anz, abschn_len, abschn_nr, anzahl, sd_index,
                        ii, jj;
   GSTATE               ret_state = GLEAM_OK;
   BOOL                 loc_mut,
                        mut_done  = FALSE;
   
   /* segm_neu */
#ifdef CHECKS
   if (chain_ptr == NULL)
      return (fatal (EVO_GO_PMUT, 9, 0, 0, "", NIL_PTR));
#endif
   abschn_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
   anzahl     = mutations_zahl (SEGM_NEW_PARAM, abschn_anz);
#ifdef ZTEST
   sprintf (buf2, "SNeu: %d von %d:", anzahl, abschn_anz);
#endif
   for (ii = 1;  ii <= anzahl;  ii++)
   { /* mutiere einen Abschnitt */
#ifdef MTEST
      sprintf (buf1, "Abschnitt-Nr eingeben [1..%1u]:", abschn_anz);
      printf ("%s\n", buf1);
      lies_wert (&abschn_nr, "Nr: ", 1, abschn_anz, abschn_anz+1);
#else /* kein MTEST */
      abschn_nr = irand (abschn_anz) + 1;
  #ifdef ZTEST
        sprintf (buf1, "%1u", abschn_nr);
      sprintf (buf2, "%s %s", buf2, buf1);
  #endif /* ZTEST */
#endif /* kein MTEST */
      if (!ermittle_abschnitt (&pred_ptr, &seq_end_ptr, &abschn_len, &sd_index,
                               &sd_ptr, chain_ptr, abschn_nr, &ret_state))
         return (fatal (EVO_GO_PMUT, 10, abschn_nr, 0, "", NO_ABSCHN));
      act_ptr = pred_ptr->u_el.next_action;
      for (jj = 1;  (jj <= abschn_len) && (ret_state == GLEAM_OK);  jj++)
      { /* jede Aktion des Abschnitts */
         loc_mut  = parameterize (act_ptr, &ret_state);
         mut_done = mut_done || loc_mut;
         act_ptr  = act_ptr->u_el.next_action;
      } /* jede Aktion des Abschnitts */
      if (mut_done)
         mut_done = FALSE;
      else
         ii--;             /* Abschnitte ohne echte Aktionen nicht mitzaehlen */
   } /* mutiere einen Abschnitt */
#ifdef ZTEST
   printf ("%s\n", buf2);
#endif
   return (ret_state);
} /* segm_neu */ 
