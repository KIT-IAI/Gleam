/*******************************************************************************
GLEAM/AE                                                                  V1.1.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: HAndlungsMODell spezifische software
                        Module: HAMMINGabstand
 
Package: hmod                   File: hamming.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul stellt Routinen zur Berechnung des Hammingabstands zweier Aktionsket-
ten zur Verfuegung. Der Hammingabstand wird als Wert zwischen 0 und 1 gemessen, 
wobei 0 fuer identische Ketten steht und 1 den maximalen Abstand darstellt. Da 
aus Sicht der Verwendung des Hammingabstands nur die genaue Bestimmung geringer 
Kettenunterschiede interessiert, wird eine nicht exakte Erfassung von AKs vari-
abler Laenge und Aktionsreihenfolge wegen der damit verbundenen einfacheren Be-
rechnung in Kauf genommen.

Bei der Berechnung wird zwischen drei Faellen unterschieden:

AK-Laenge und Aktionsreihenfolge fix (FIX_LEN, FIX_ACT_FIX_GEN): 
================================================================
Die Aktionsreihenfolge ist in dem Sinne fix, dass sie bei der Bewertung keine 
Rolle spielt. Da es recht aufwaendig waere, die fixe Reihenfolge bei den gen. 
Operatoren zu erhalten, wird die fixe Aktionsreihenfolge derart interpretiert, 
dass die Reihenfolge bei der Bestimmung des Hammingabstands keine Rolle spielt, 
die Ketten sich aber in der Reihenfolge durchaus unterscheiden duerfen.
                                 
                   1             (|P(i,1) - P(i,2)|)
    par_ham = ----------- * SUM  (-----------------)
              ges_par_anz   P(i) (par_bereich(P(i)))

        mit:
             P(i,j)           : Wert der i-ten Paramters der Kette j
             par_bereich(P(i)): Wertebereich des i-ten Paramters
             ges_par_anz      : Anzahl aller Parameter aller Aktionen mit 
                                par_bereich (P(i)) > 0.0


AK-Laenge fix und Aktionsreihenfolge variabel (FIX_LEN, FIX_ACT_RAND_GEN): 
==========================================================================
In diesem Fall ist die Aktionsreihenfolge bedeutungstragend. Der gesamte 
Hammingabstand ist der Durchschnitt aus "par_ham" und "pos_ham", wobei 
"pos_ham" wie folgt definiert ist:

                  1
    pos_ham = -------- * SUM (PosAbst(A(i)))
              max_abst   A(i)

        mit:
             A(i)         : i-te Aktion
             PosAbst(A(i)): Positionsabstand der i-ten Aktion zweier Ketten
             max_abst     : Der maximale Abstand aller Aktionen bei gegebener
                            Kettenlaenge "len" und gleicher Aktionszusammen-
                            setzung berechnet sich wie folgt: 
                            
                            "len" gerade: 
                            =============
                                          (len-2)/2
                            max_abst = 2 *   SUM   (2i + 1) = SQR(len) / 2
                                             i=0
                            
                            "len" ungerade: 
                            ===============
                                          (len-1)/2
                            max_abst = 2 *   SUM   (2i)     = (SQR(len) - 1) / 2
                                             i=1


AK-Laenge und Aktionen samt Reihenfolge variabel (RAND_LEN, RAND_ACT_GEN): 
==========================================================================
Der gesamte Hammingabstand ist der Durchschnitt aus "par_ham", "pos_ham" und 
dem 3-fach bewerteten "diff_ham". "diff_ham" reflektiert die Uebereinstimmung 
der Aktionspraesenz in beiden Ketten und wird deshalb 3-fach gezaehlt, weil bei 
groesseren Differenzen "pos_ham" und "par_ham" stark abnehmen, da sie nur ueber 
der Menge der gemeinsamen Aktionen beider Ketten definiert sind.

AK(1) sei die laengere AK und A(gem) die Menge der gemeinsamen Aktionen beider 
AKs. A(AK(2)-AK(1))) sei die Menge derjenigen Aktionen von AK(2), die nicht in
AK(1) enthalten sind. 

Bei leerem A(gem) ist der Gesamt-Hammingabstand gleich 1.

"par_ham" ist ueber die A(gem) definiert. Eine weitergehendere Definition, z.B. 
ueber alle Aktionen der kuerzeren Kette, wobei in beiden Ketten nicht vorhandene
AKtionen mit maximalem Parameterabstand bewertet werden, ist insofern ueber-
fluessig, als dass dieser Umstand in Form der Nichtpraesenz von Aktionen bereits
in die Bewertung ("diff_ham") eingeht.

"pos_ham" wird nur ueber A(gem) definiert, wobei eine exakte Bestimmung von 
"max_abst" schwierig ist. Andererseits sind wir nur an einer exakten Erfassung 
kleiner Hammingabstaende interessiert. Daher reicht es aus, mit dem zuvor defi-
nierten "max_abst" der kuerzeren AK(2) so zu arbeiten, also ob AK(1) genauso 
lang waere. Der Fehler bei laengeren AK(1) bewirkt einen zu gross berechneten 
"pos_ham"-Wert, der die Obergrenze von 1.0 ueberschreiten kann und daher auf 
1.0 zu begrenzen ist. Ausserdem ist bei der Abstandsbestimmung zweier Aktioen 
zu beachten, dass Aktionen in jeder Kette mehrfach vorkommen koennen.

"diff_ham" reflektiert die Uebereinstimmung der Aktionspraesenz in beiden Ketten
und ist wie folgt definiert:

               Len(AK(1)) - Size(A(gem)) + Size(A(AK(2)-AK(1)))
    diff_ham = ------------------------------------------------ 
                              Len(AK(1)) + Len(AK(2))

                       2 * Size(A(gem))
             = 1 - -----------------------
                   Len(AK(1)) + Len(AK(2))

               mit:             
                    Len(AK(i)): Laenge der AK(i)
                    Size(A(i)): Groesse der Aktionen-Menge A(i)

Der Nenner kann maximal so gross werden wie der Zaehler, da die Menge gemein-
samer Aktionen leer sein kann. In diesem Fall ist "diff_ham" gleich 1.
    

Die Routinen im einzelnen:
    
DOUBLE ham_dist (CHAIN_ELEM_PTR_TYPE  ch1_ptr,
                 CHAIN_ELEM_PTR_TYPE  ch2_ptr)
          Berechnet den Hamminabstand der beiden Ketten entsprechend den aktuel-
          len Werten von "len_mode" und "akt_mode" gemaess obiger Formeln.

--------------------------------------------------------------------------------
basiert auf C-Unit: hamming.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
1. Der Aufruf von "ham_dist" mit Nil-Pointer(n) fuehrt nur noch zu einer Meldung
   und das auch nur, wenn "CHECKS" definiert ist.
2. Korrekturen wegen g++-Compiler.                                   WJ 08.08.00
3. Korrektur f.AKs ohne Parameter: par_ham = 0 statt Div durch 0! CB/WJ 15.04.05
4. Streichung der ueberfluessigen Deklaration von "calc_par_ham".    WJ 26.06.12
5. Anpassung der Aufrufe von get_act_descr().                        WJ 28.06.19
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 26.06.2012
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "schalter.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "hmodInt.h"

#ifdef GLO_CHECKS
  #undef CHECKS
#endif



/* ============================ Lokale Variable: ============================ */
static DOUBLE  ham_used_par_anz;  /* GesAnz d. Parameter mit Wertebereich > 0 */





/* ============================ calc_akt_par_ham ============================ */
static DOUBLE calc_akt_par_ham (CHAIN_ELEM_PTR_TYPE  akt1_ptr,
                                CHAIN_ELEM_PTR_TYPE  akt2_ptr)
/*----------------------------------------------------------------------------*/
/*  Liefert die Summe aller aktuellen Parameterabstaende bezogen auf den      */
/*  Maximalabstand der beiden Aktionen "akt1_ptr" und "akt2_ptr":             */
/*                                                                            */
/*             (|P(i,1) - P(i,2)|)     wobei die P(i) die Parameter der       */
/*        SUM  (-----------------)                    der Aktion sind.        */
/*        P(i) (par_bereich(P(i)))                                            */
/*                                                                            */
/*  Parameter mit einem Parameterbereich "par_bereich" == 0 werden ignoriert. */
/*  Parameter mit einem "par_bereich" != 0 werden in der modul-globalen       */
/*  Variable "ham_used_par_anz" mitgezaehlt.                                  */
/*----------------------------------------------------------------------------*/
{
   ACT_DESCR_PTR_TYPE  act_descr_ptr;
   DOUBLE              rbereich, delta_sum;
   INT                 ibereich, par_anz, pi;

   /* calc_akt_par_ham */
   get_act_descr  (&act_descr_ptr, akt1_ptr->u_el.activity, WITHOUT_GENECODE_CHECK);
   delta_sum = 0.0;
   par_anz   = act_descr_ptr->i_par_anz;
   for (pi = 0;  pi < par_anz;  pi++)
   { /* I-Params */
      ibereich = (INT)(act_descr_ptr->i_par_descr[pi].par_bereich);
      if (ibereich > 0)
      { /* Benutzter Parameter */
         delta_sum += fabs ((DOUBLE)(akt1_ptr->u_el.u_xel.u_act.i_params[pi] - 
                                     akt2_ptr->u_el.u_xel.u_act.i_params[pi]) /
                            ibereich);
         ham_used_par_anz++;
      } /* Benutzter Parameter */
   } /* I-Params */
   par_anz = act_descr_ptr->r_par_anz;
   for (pi = 0;  pi < par_anz;  pi++)
   { /* R-Params */
      rbereich = act_descr_ptr->r_par_descr[pi].par_bereich;
      if (rbereich > 0.0)
      { /* Benutzter Parameter */
         delta_sum += fabs ((akt1_ptr->u_el.u_xel.u_act.r_params[pi] - 
                             akt2_ptr->u_el.u_xel.u_act.r_params[pi]) /
                            rbereich);
         ham_used_par_anz++;
      } /* Benutzter Parameter */
   } /* R-Params */
   return (delta_sum);
} /* calc_akt_par_ham */



/* ================================ ham_dist ================================ */
DOUBLE ham_dist (CHAIN_ELEM_PTR_TYPE  ch1_ptr,
                 CHAIN_ELEM_PTR_TYPE  ch2_ptr)
{
   CHAIN_ELEM_PTR_TYPE  akt1_ptr, akt2_ptr;
   DOUBLE               ges_ham_dist, pos_ham, par_ham,
                        par_dist_sum, pos_abst_sum;
   INT                  ak1_len, ak2_len, pos1, pos2, 
                        size_AGem, tmp;
   BOOL                 akt_found;

   /* ham_dist */
   if ((ch1_ptr == NULL) || (ch2_ptr == NULL))
   {
#ifdef CHECKS
      meldung (HMOD_HAMMING, NO_INFO, NO_INFO, "", HAM_DIST_NIL_ERR);
#endif
      return (NO_HAM);
   }
   ham_used_par_anz = 0.0;
   par_dist_sum     = 0.0;
   pos_abst_sum     = 0.0;
   ak1_len          = ch1_ptr->u_el.u_xel.u_hdr.chain_length;
   ak2_len          = ch2_ptr->u_el.u_xel.u_hdr.chain_length;
   if (gen_len_mode == FIX_LEN)
   { /* fixe AK-Laenge */
      if (ak1_len != ak2_len)
      { /* Laengendifferenz trotz laengeninvarianter AKs ! */
         fehler (HMOD_HAMMING, 2, ch1_ptr->u_el.u_xel.u_hdr.chain_length, 
                 ch2_ptr->u_el.u_xel.u_hdr.chain_length, "", UNGLEICHE_CH_LEN);
         return (1.0);                            /* maximaler Hammingabstand */
      } /* Laengendifferenz trotz laengeninvarianter AKs ! */
      akt1_ptr = ch1_ptr->u_el.next_action;
      pos1     = 1;
      while (akt1_ptr != NULL)
      { /* all along these chains */
         akt2_ptr = ch2_ptr->u_el.next_action;
         pos2     = 1;
         do
         { /* Aktion suchen */
            akt_found = (akt1_ptr->u_el.activity == akt2_ptr->u_el.activity);
            if (!akt_found)
            {
               akt2_ptr = akt2_ptr->u_el.next_action;
               pos2++;
               if (akt2_ptr == NULL)
               { /* Aktion nicht gefunden */
                  fehler (HMOD_HAMMING, 3, akt1_ptr->u_el.activity, 
                          NO_INFO, "", NO_ITEM);
                  break;           /* item not found, terminate inner loop */
               } /* Aktion nicht gefunden */
            }
         } /* Aktion suchen */
         while (!akt_found);
         if (akt_found)
         { /* Aktion gefunden */
            par_dist_sum += calc_akt_par_ham (akt1_ptr, akt2_ptr);
            pos_abst_sum += abs (pos2 - pos1);
         } /* Aktion gefunden */
         else
         { /* abnormal termination */
            par_dist_sum = ham_used_par_anz;    /* forces return value of 1.0 */
            pos_abst_sum = ham_max_abst;        /* forces return value of 1.0 */
            break;                    /* item not found, terminate outer loop */
         } /* abnormal termination */
         akt1_ptr = akt1_ptr->u_el.next_action;
         pos1++;
      } /* all along these chains */
      if (ham_used_par_anz == 0.0)
         ges_ham_dist = 0.0;  /* AK ohne Parameter oder fruehe abnormal term. */
      else
         ges_ham_dist = par_dist_sum / ham_used_par_anz;   /* AK fixer Laenge */
      if (gen_akt_mode == FIX_ACT_RAND_GEN)
      { /* fixe Aktionen und variable Aktionsreihenfolge */
         ges_ham_dist = (ges_ham_dist + (pos_abst_sum / ham_max_abst)) / 2.0;
      } /* fixe Aktionen und variable Aktionsreihenfolge */
   } /* fixe AK-Laenge */
   else
   { /* variable AK-Laenge */
      if (ak1_len >= ak2_len)
         akt2_ptr = ch2_ptr->u_el.next_action;
      else
      { /* AKs vertauscht behandeln: AK1 soll die laengere AK sein */
         akt2_ptr = ch1_ptr->u_el.next_action;
         ch1_ptr  = ch2_ptr;
         tmp      = ak1_len;
         ak1_len  = ak2_len;
         ak2_len  = tmp;
      } /* AKs vertauscht behandeln: AK1 soll die laengere AK sein */
      size_AGem = 0;
      pos2      = 1;
      while (akt2_ptr != NULL)
      { /* all along these chains */
         akt1_ptr = ch1_ptr->u_el.next_action;
         pos1     = 1;
         do
         { /* Aktion suchen */
            akt_found = (akt1_ptr->u_el.activity == akt2_ptr->u_el.activity);
            if (akt_found)
            { /* Aktion gefunden */
               par_dist_sum += calc_akt_par_ham (akt1_ptr, akt2_ptr);
               pos_abst_sum += abs (pos2 - pos1);
               size_AGem++;
               akt1_ptr->u_el.activity = -(akt1_ptr->u_el.activity); /*marc it*/
            } /* Aktion gefunden */
            else
            { /* Aktion nicht gefunden */
               akt1_ptr = akt1_ptr->u_el.next_action;
               pos1++;
            } /* Aktion nicht gefunden */
         } /* Aktion suchen */
         while (!akt_found && (akt1_ptr != NULL));
         akt2_ptr = akt2_ptr->u_el.next_action;
         pos2++;
      } /* all along these chains */
      if (size_AGem > 0)
      { /* Es gibt ueberhaupt Gemeinsamkeiten */
         pos_ham = pos_abst_sum / calc_max_abst (ak2_len);
         if (pos_ham > 1.0)
            pos_ham = 1.0;
         if (ham_used_par_anz == 0.0)
            par_ham = 0.0;
         else
            par_ham = par_dist_sum / ham_used_par_anz;
         ges_ham_dist = (pos_ham + par_ham + 3.0 -
                         ((6.0 * size_AGem) / (ak1_len + ak2_len)))/5.0;
      } /* Es gibt ueberhaupt Gemeinsamkeiten */
      else
         ges_ham_dist = 1.0;
      /* ----------------------- Merker ruecksetzten: ----------------------- */
      akt1_ptr = ch1_ptr->u_el.next_action;
      while ((size_AGem > 0) && (akt1_ptr != NULL))
      { /* Aktion suchen */
         if (akt1_ptr->u_el.activity < 0)
         { /* Aktion gefunden */
            akt1_ptr->u_el.activity = -(akt1_ptr->u_el.activity); /*demarkieren*/
            size_AGem--;
         } /* Aktion gefunden */
         akt1_ptr = akt1_ptr->u_el.next_action;       /* Aktion weiterschalten */
      } /* Aktion suchen */
   } /* variable AK-Laenge */
   return (ges_ham_dist);
} /* ham_dist */
