/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: APPLication
                        Module: APPLication dependent genetic REPair
 
Package: appl                   File: appl_rep.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

This module contains application-specific functions for a genetic repair, provided 
that a corresponding additional application is linked to GLEAM.

The present implementation of the module acts as a kind of "blind plug" and 
must always be included if no additional application is to be included OR 
as a distributor for the applications to be included (see also module "appl_if").


The routines in detail:

BOOL   add_appl_repair_1ch (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                            GSTATE              *ret_state);
          The function performs a genetic repair for one chromosome und returns 
          TRUE if it could be corrected and no errors occurred. Otherwise FALSE, 
          whereby the chromosome is deleted and "ch_ptr" becomes NULL, provided 
          that no errors have occurred before. Empty chains are always irreparable.

GSTATE add_repair_2_chs (CHAIN_ELEM_PTR_TYPE  ch1_ptr,
                         CHAIN_ELEM_PTR_TYPE  ch2_ptr);
          The function performs a Genetic Repair for two chromosomes that were 
          subjected to a recombination or crossover. There are always two correct 
          chromosomes to be returned by changing the passed chromosomes. Other-
          wise or if the "add_appl_code" is not allowed or if software errors 
          have occurred due to illegal input chromosomes, the return status will 
          be bad.


--------------------------------------------------------------------------------
List of changes:
1. With OPAL/V, the genetic repair consists of doing nothing for one chromosome 
   and calling "repair_2_aks" for two.                               WJ 07.03.01
2. For the ITEM application, the same applies.                       WJ 21.06.01
3. For the GORBA application, the same applies.                      WJ 27.10.05
4. For the SGS application genetic repair consists of doing nothing 
   for one or two chromosomes.                                       WJ 05.06.18
5. With PMS, the genetic repair consists of doing nothing for one 
   chromosome and calling "repair_2_aks" for two.                    WJ 31.10.19
--------------------------------------------------------------------------------
Status: Impl                     Author: W.Jakob                Date: 31.10.2019
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "hmod.h"
#include "appl.h"
#include "applInt.h"




/* ======================== Package-global Variables: ======================= */



/* ============================= Local Constants: =========================== */



/* ======================== Local Type Declarations: ======================== */



/* ============================= Local Variables: =========================== */



/* ============================= Local Routines: ============================ */




/* ============================= add_appl_repair ============================ */
BOOL add_appl_repair_1ch (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                          GSTATE              *ret_state)
{  
   BOOL  erg = FALSE;

   /* add_appl_repair_1ch */
   switch (add_appl_code)
   {
      case TSP_APPL:
      erg = TRUE;
      break; /* TSP_APPL */

      case GORBA_KOMB_APPL:
      case GORBA_PERM_APPL:
      case ITEM_APPL:
      case OPAL_APPL:
      case SGS_APPL:
      case PMS_APPL:
      erg = TRUE;
      break; /* ..._APPL */

      default:
      *ret_state = fatal (APPL_REP, 1, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   } /* switch */

   return (erg);
} /* add_appl_repair_1ch */



/* ============================ add_repair_2_aks ============================ */
GSTATE add_repair_2_chs (CHAIN_ELEM_PTR_TYPE  ch1_ptr,
                         CHAIN_ELEM_PTR_TYPE  ch2_ptr)
{
   GSTATE  ret_state = GLEAM_OK;

   /* add_repair_2_aks */
   switch (add_appl_code)
   {
      case TSP_APPL:
      /*
      ret_state = fatal(APPL_REP, 101, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      */
      break; /* TSP_APPL */

      case SGS_APPL:
      break; /* SGS_APPL */

      case GORBA_KOMB_APPL:
      case GORBA_PERM_APPL:
      case ITEM_APPL:
      case OPAL_APPL:
      case PMS_APPL:
      ret_state = repair_2_aks (ch1_ptr, ch2_ptr);
      break; /* OPAL_APPL ... */

      default:
      ret_state = fatal (APPL_REP, 2, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   } /* switch */
   return (ret_state);
} /* add_repair_2_aks */



