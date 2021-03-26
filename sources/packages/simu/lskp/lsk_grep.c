/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator/LeSaKPlus
                        Module: LeSaK-Genetic-REPair
 
Package: simu/lskp              File: lsk_grep.c              Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
--------------------------------------------------------------------------------

Der Modul enthaelt Routinen zum Genetic-Repair der Anwendung LESAK/Plus ("appl_
code" = LESAK_APPL). Dabei handelt es sich um den alten Plausibilitaetstest 
(P-Test) von LESAK. 


Die Routinen im einzelnen:

BOOL lesak_p_test (CHAIN_ELEM_PTR_TYPE *ch_ptr,
		   GSTATE              *ret_state);
          Die Routine fuehrt das Genetic Repair fuer eine AK im LESAK-Kontext 
	  durch und liefert TRUE ab, wenn die AK korrigiert werden konnte und 
	  keine Fehler auftraten. Ansonsten FALSE, wobei die Kette geloescht und
	  "ch_ptr" NIL wird, sofern zuvor keine Fehler auftraten. Wenn beim Auf-
	  ruf "ret_state" bereits nicht GLEAM_OK ist, passiert gar nichts.
	  Eine AK ist unplausibel, wenn sie leer ist oder keinen Motor anschal-
	  tet. Das Genetic Repair betrifft die achsbezogenen Aktionen MOTOR_AUS,
	  GESCHW_VERGL und BESCHL_VERGL, die nur  bei eingeschaltetem Motor in
	  der Kette belassen werden. Desgleichen verbleibt ein UNVERAENDERT nur,
	  wenn mindestens ein Motor an ist und es nicht im Start-Block steht. 
	  Da ferner nur ungeschachtelte und abgeschlossene Startbloecke (START_
	  BLOCK_BEGINN, START_BLOCK_END) erlaubt sind, werden ueberfluessige 
	  START_BLOCK-Aktionen ebenfalls entfernt.

--------------------------------------------------------------------------------
basiert auf C-Unit: ptest_gen.c (LSKP).
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0 vom 05.12.1998:
 1. Include von "joints.h" hinzugefuegt.                             WJ 08.01.18
--------------------------------------------------------------------------------
Status: o.k.             Autoren: C.Blume / W.Jakob            Datum: 08.01.2018
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "joints.h"
#include "lsk_mod.h"



/* ============================ Lokale Konstante: =========================== */
#define MAX_SINNVOLL_LEN  2000               /* Laengere AKs sind unplausibel */


/* ======================== Lokale Typdeklarationen: ======================== */
typedef BOOL  MOTOR_EIN_TYP [MAXAXISANZ+1];




/* ============================== lesak_p_test ============================== */
BOOL lesak_p_test (CHAIN_ELEM_PTR_TYPE *ch_ptr,
		   GSTATE              *ret_state)
{
   MOTOR_EIN_TYP        motor_ein;          /* Merker fuer Motorzustand: "An" */
   CHAIN_ELEM_PTR_TYPE  pred_sbb_ptr,       /* Aktion vor dem StartBlockBeginn*/
                        destr_ptr,          /* Zeiger auf die zu loe. Aktion  */
                        pred_ptr,           /* Aktion vor der zu loeschenden  */
                        ptr;                /* die jeweils aktuelle Aktion    */
   SHORT                ii;                 /* Laufvariable                   */
   SHORT                ch_len;             /* Kettenlaenge                   */
   BOOL                 geloescht,          /* True, wenn was geloescht wurde */
                        in_startblock,      /* TRUE, wenn innerhalb Startblock*/
                        chain_ok,           /* TRUE, wenn Kettenpruefung ok   */
                        ein_motor_jetzt_an, /* f. lokalen Check b.UNVERAENDERT*/
                        ein_motor_an;       /* TRUE, wenn mind 1 Motor 1x an  */

   /* lesak_p_test */
   if (*ret_state != GLEAM_OK)
      return (FALSE);
   ch_len = (*ch_ptr)->u_el.u_xel.u_hdr.chain_length;
   if (ch_len > MAX_SINNVOLL_LEN)
      chain_ok = FALSE;
   else
   { /* Plausibel, wenn mindestens ein Motor angeschaltet wird */
      for (ii = 1;  ii <= MAXAXISANZ;  ii++) 
	 motor_ein[ii] = FALSE;                    /* Init: Kein Motor ist an */
      in_startblock = FALSE;
      pred_ptr      = *ch_ptr;
      ptr           = (*ch_ptr)->u_el.next_action;
      ein_motor_an  = FALSE;
      do 
      { /* Pruefe diese Aktion */
	 geloescht    = FALSE;
	 ein_motor_an = ein_motor_an || (SETZE_U_M == ptr->u_el.activity);
	 switch (ptr->u_el.activity) 
	 {
	    case SETZE_U_M:
	    motor_ein[ptr->u_el.u_xel.u_act.i_params[MOTOR_NR]] = TRUE;
	    break;

	    case MOTOR_AUS:
	    if (motor_ein[ptr->u_el.u_xel.u_act.i_params[MOTOR_NR]])
	       motor_ein[ptr->u_el.u_xel.u_act.i_params[MOTOR_NR]] = FALSE;
	    else /* loesche die Aktion: */
	       if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						   pred_ptr)) == GLEAM_OK)
	       { /* Entfernte Aktion loeschen */
		  *ret_state = delete_item (destr_ptr);
		  geloescht  = TRUE;
	       } /* Entfernte Aktion loeschen */
	    break;

	    case GESCHW_VERGL:
	    case BESCHL_VERGL:
	    if (!motor_ein[ptr->u_el.u_xel.u_act.i_params[GELENK_NR]])
	       if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						   pred_ptr)) == GLEAM_OK)
	       { /* Entfernte Aktion loeschen */
		  *ret_state = delete_item (destr_ptr);     /* loesche Aktion */
		  geloescht  = TRUE;
	       } /* Entfernte Aktion loeschen */
	    break;

	    case START_BLOCK_BEGIN:
	    if (in_startblock)
	    { /* START_BLOCK_BEGIN innerhalb eines Startblocks */ 
	       if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						   pred_ptr)) == GLEAM_OK)
	       { /* Entfernte Aktion loeschen */
		  *ret_state = delete_item (destr_ptr);     /* loesche Aktion */
		  geloescht  = TRUE;
	       } /* Entfernte Aktion loeschen */
	    } /* START_BLOCK_BEGIN innerhalb eines Startblocks */ 
	    else 
	    { /* Start_Block_Begin */
	       in_startblock = TRUE;
	       pred_sbb_ptr  = pred_ptr;
	    } /* Start_Block_Begin */
	    break;

	    case START_BLOCK_END:
	    if (in_startblock) 
	       in_startblock = FALSE;
	    else /* loesche die Aktion: */
	       if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						   pred_ptr)) == GLEAM_OK)
	       { /* Entfernte Aktion loeschen */
		  *ret_state = delete_item (destr_ptr);
		  geloescht  = TRUE;
	       } /* Entfernte Aktion loeschen */
	    break;

	    case UNVERAENDERT:   
	    if (in_startblock)
	    { /* UNVERAENDERT innerhalb eines Startblocks */ 
	       if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						   pred_ptr)) == GLEAM_OK)
	       { /* Entfernte Aktion loeschen */
		  *ret_state = delete_item (destr_ptr);     /* loesche Aktion */
		  geloescht  = TRUE;
	       } /* Entfernte Aktion loeschen */
	    } /* UNVERAENDERT innerhalb eines Startblocks */ 
	    else 
	    { /* Unveraendert */
               ii = 1;
	       do
	       {
		  ein_motor_jetzt_an = motor_ein[ii];
		  ii++;
	       }
	       while (!ein_motor_jetzt_an && (ii <= MAXAXISANZ));
	       if (!ein_motor_jetzt_an) 
		  if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						      pred_ptr)) == GLEAM_OK)
		  { /* Entfernte Aktion loeschen */
		     *ret_state = delete_item (destr_ptr);  /* loesche Aktion */
		     geloescht  = TRUE;
		  } /* Entfernte Aktion loeschen */
	    } /* Unveraendert */
	    break;
	 } /* CASE */
	 if (geloescht) 
	    ptr=pred_ptr->u_el.next_action;
	 else 
	 { /* normal weiterschalten */
	    pred_ptr = ptr;
	    ptr      = ptr->u_el.next_action;
	 } /* normal weiterschalten */
      } /* Pruefe diese Aktion */
      while ((ptr != NULL) && (*ret_state == GLEAM_OK));
      if  (*ret_state == GLEAM_OK)
      { /* Kein Status-Fehler */
	 if (in_startblock && ein_motor_an) 
	 { /* letztes "Start_Block_Beginn" loeschen */
	    pred_ptr = pred_sbb_ptr;
	    if ((*ret_state = remove_next_item (&destr_ptr, *ch_ptr,
						pred_ptr)) == GLEAM_OK)
	    { /* Entfernte Aktion loeschen */
	       *ret_state = delete_item (destr_ptr);
	       geloescht  = TRUE;
	    } /* Entfernte Aktion loeschen */
	 } /* letztes "Start_Block_Beginn" loeschen */
	 chain_ok = ein_motor_an;
      } /* Kein Status-Fehler */
      else
	 chain_ok = FALSE;
   } /* Plausibel, wenn mindestens ein Motor angeschaltet wird */

   if (!chain_ok)
      *ret_state |= delete_chain (ch_ptr);           /* Lösche unplausible AK */

   return (chain_ok);
} /* lesak_p_test */
