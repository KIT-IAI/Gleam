/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: APPLication
                        Module: APPLication dependent Genetic Operators
 
Package: appl                   File: appl_go.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

This module contains application-specific genetic operators, provided that a 
corresponding additional application is linked to GLEAM.

The present implementation of the module acts as a kind of "blind plug" and 
must always be included if no additional application is to be included OR 
as a distributor for the applications to be included (see also module "appl_if").


The routines in detail:

BOOL do_appl_xo (CHAIN_ELEM_PTR_TYPE *child1_ptr,
                 CHAIN_ELEM_PTR_TYPE *child2_ptr,
                 GSTATE              *ret_state,
                 CHAIN_ELEM_PTR_TYPE  indiv_ptr,
                 CHAIN_ELEM_PTR_TYPE  mate_ptr,
                 INT                  op_code,
                 BOOL                 do_xo);
          Application-specific crossover operator. Returns TRUE if changed 
          chromosomes are returned so that their fitness must be calculated. 
          Occurring errors are reported in "ret_state" and the function result 
          is then FALSE.
          From the two remaining unchanged input chromosomes "indiv_ptr" and 
          "mate_ptr" two children "child1_ptr" and "child2_ptr" are generated 
          by means of a crossover operator specified by "op_code". "do_xo" is 
          TRUE if the parents are sufficiently different (Hamming distance).
          It depends on the application whether a crossover is performed anyway 
          or nothing happens and FALSE is returned.

BOOL do_appl_mut (CHAIN_ELEM_PTR_TYPE  child_ptr,
                  INT                  op_code,
                  GSTATE              *ret_state);
          Application-specific mutation operator. Returns TRUE if the chromosome 
          has been altered in such a way that its fitness has to be calculated.
          Occurring errors are reported in "ret_state" and the function result 
          is then FALSE.
          The chromosome "child_ptr" is a possibly already altered copy of the 
          parent chromosome and is directly altered by the mutation operator 
          indicated by "op_code".

--------------------------------------------------------------------------------
List of changes:
--------------------------------------------------------------------------------
Status: o.k.                     Author: W.Jakob                Date: 18.02.1997
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




/* =============================== do_appl_xo =============================== */
BOOL do_appl_xo (CHAIN_ELEM_PTR_TYPE *child1_ptr,
                 CHAIN_ELEM_PTR_TYPE *child2_ptr,
                 GSTATE              *ret_state,
                 CHAIN_ELEM_PTR_TYPE  indiv_ptr,
                 CHAIN_ELEM_PTR_TYPE  mate_ptr,
                 INT                  op_code,
                 BOOL                 do_xo)
{
   BOOL  erg = FALSE;

   /* do_appl_xo */
   switch (add_appl_code)
   {
      case TSP_APPL:
      /*
      *ret_state = fehler (APPL_GO, 100, NO_INFO, NO_INFO, "", 
                           NOT_YET_IMPLEMENTED);
      */
      break; /* TSP_APPL */

      default:
      *ret_state = fatal (APPL_GO, 1, add_appl_code, NO_INFO, "", 
                          BAD_APPL_CODE);
   } /* switch */
   return (erg);
} /* do_appl_xo */



/* ============================== do_appl_mut =============================== */
BOOL do_appl_mut (CHAIN_ELEM_PTR_TYPE  child_ptr,
                  INT                  op_code,
                  GSTATE              *ret_state)
{
   BOOL  erg = FALSE;

   /* do_appl_mut */
   switch (add_appl_code)
   {
      case TSP_APPL:
      /*
      *ret_state = fehler (APPL_GO, 101, NO_INFO, NO_INFO, "", 
                           NOT_YET_IMPLEMENTED);
      */
      erg = TRUE;
      break; /* TSP_APPL */

      default:
      *ret_state = fatal (APPL_GO, 2, add_appl_code, NO_INFO, "", 
                          BAD_APPL_CODE);
   } /* switch */
   return (erg);
} /* do_appl_mut */

