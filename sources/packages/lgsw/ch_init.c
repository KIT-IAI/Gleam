/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: Listen-GrundSoftWare
                        Module: CHain INITialisation
 
Package: lgsw                   File: ch_init.c                 cc / Solaris 2.4
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt die Routine "init_chain" zur Erzeugung und Initialisierung von 
Ketten. Diese Funktion wurde vom Rest des Packages isoliert, um anwendungsspezi-
fische Anpassungen zu vereinfachen.

Die Routinen im einzelnen:

GSTATE init_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                   SHORT                ch_typ);

          Es wird der Kopf einer leeren Kette vom Typ "ch_typ" erzeugt und in 
	  "chain_ptr" abgeliefert. Die "chain_length" und bei AKs der "ref_counter" 
	  sind null, "next_action" gleich NIL und "last_action" zeigt auf den Kopf. 
	  Bei evolutionierbaren Ketten ist die "segment_anz" gleich null und der 
	  "segm_descr_ptr" NIL. "ch_typ" kann die Werte der ".._HEADER"-Codierungen 
	  von "activity" des Files "chaindef.h" annehmen. Ein ungueltiger "ch_typ"-
	  Wert ist ein fataler Fehler ebenso wie nicht ausreichender Speicher. Die 
	  aufrufenden Routinen muessen sicherstellen, dass genuegend Speicher fuer 
	  die Kette zur Verfuegung steht (siehe "enough_free_mem" aus "mmsw").

--------------------------------------------------------------------------------
basiert auf C-Unit: balisto.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 03.01.1997
================================================================================ */
#include <stdio.h>
#include <stdlib.h>

#include "schalter.h"
#include "fbhm.h"
#include "lgsw.h"
#include "lgswInt.h"



/* =============================== init_chain =============================== */
GSTATE init_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                   SHORT                ch_typ)
{
   /* INIT_CHAIN/BALISTO */
   if (new_chain_elem (chain_ptr) != GLEAM_OK)
      return (GLEAM_FATAL);
   (*chain_ptr)->u_el.activity                   = ch_typ;
   (*chain_ptr)->u_el.next_action                = NULL;
   (*chain_ptr)->u_el.u_xel.u_hdr.chain_length   = 0;
   (*chain_ptr)->u_el.u_xel.u_hdr.ref_counter    = 0;
   (*chain_ptr)->u_el.u_xel.u_hdr.last_action    = *chain_ptr;
   (*chain_ptr)->u_el.u_xel.u_hdr.guete          = KEINE_GUETE;
   (*chain_ptr)->u_el.u_xel.u_hdr.lfd_nr         = 0;
   (*chain_ptr)->u_el.u_xel.u_hdr.fitness_note   = 0.0;
   (*chain_ptr)->u_el.u_xel.u_hdr.chain_status   = 0;
   (*chain_ptr)->u_el.u_xel.u_hdr.segment_anz    = 0;
   (*chain_ptr)->u_el.u_xel.u_hdr.segm_descr_ptr = NULL;
   if (ch_typ < AK_HEADER) 
      (*chain_ptr)->u_el.u_xel.u_hdr.header_par_ptr = NULL;             /* VK */
   else 
   { /* AK,  */
      if (new_chain_elem (&((*chain_ptr)->u_el.u_xel.u_hdr.header_par_ptr)) != GLEAM_OK)
	 return (GLEAM_FATAL);
      if (ch_typ == AK_HEADER) 
      { /* AK-Header */
	 (*chain_ptr)->u_el.u_xel.u_hdr.ak_index = 0;
      } /* AK-Header */
      else 
	 return (fatal (LGSW_CH_INIT, 1, ch_typ, NO_INFO, "", BAD_HDR_CODE));
   } /* AK, */
   return (GLEAM_OK);
} /* init_chain */



/* ================================ lgswInit ================================ */
GSTATE lgswInit (void)
{
   GSTATE  erg_state;

   /* lgswInit */
   if ((erg_state = mmsw_init ()) == GLEAM_OK)
      bacha_init ();
   return (erg_state);
} /* lgswInit */
