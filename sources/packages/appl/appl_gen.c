/*******************************************************************************
GLEAM/AE                                                                  V1.1.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: APPLication
                        Module: APPLication dependent ak-GENeration
 
Package: appl                   File: appl_gen.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

This module contains application-specific functions for chromosome and gene sequence 
generation, provided that a corresponding additional application is linked to GLEAM.

The present implementation of the module acts as a kind of "blind plug" and 
must always be included if no additional application is to be included OR 
as a distributor for the applications to be included (see also module "appl_if").


The routines in detail:

BOOL   add_anw_ch_gen (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                       GSTATE              *ret_state);
          Non-standard chromosome generation. Returns TRUE if the passed "ret_state" 
          is GLEAM_OK and the chromosome could be generated successfully.
          Otherwise FALSE and an empty "ch_ptr".

GSTATE add_gen_act_seq (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                        CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                        INT                  number);
          The function generates a gene sequence of the length "number" andreturns 
          it in "seq_ptr" and "seq_end_ptr". If "number" is equal to zero, empty 
          pointers are returned and the result of the function is GLEAM_OK. If 
          errors occur during generation, the corresponding error status and a 
          correctly linked gene sequence are returned in the two pointers.

--------------------------------------------------------------------------------
List of changes:
1. Deletion of "add_appl_pre_gen".                                   WJ 02.08.00
--------------------------------------------------------------------------------
Status: Impl                     Author: W.Jakob                Date: 02.08.2000
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




/* ============================= add_anw_ch_gen ============================= */
BOOL add_anw_ch_gen (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                     GSTATE              *ret_state)
{
   *ret_state |= fehler (APPL_GEN, 101, NO_INFO, NO_INFO, "", 
                         NOT_YET_IMPLEMENTED);
   return (FALSE);
} /* add_anw_ch_gen */



/* ============================= add_gen_act_seq ============================ */
GSTATE add_gen_act_seq (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                        CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                        INT                  number)
{
   return (fehler (APPL_GEN, 102, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED));
} /* add_gen_act_seq */


