/*******************************************************************************
GLEAM/AE                                                                  V1.1.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: APPLication
                        Module: APPLication dependent SIMulator
 
Package: appl                   File: appl_sim.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

The module contains an application-specific simulator that can be added to 
HyGLEAM as an additional application.


The routines in detail:

BOOL   do_appl_sim (DOUBLE              *simu_erg_werte,
                    CHAIN_ELEM_PTR_TYPE  ch_ptr,
                    BOOL                 interaktiv,
                    GSTATE              *erg_state);
          Application-specific simulation of chromosomes. The function interprets 
          or simulates the chromosome passed in "ch_ptr" and returns the raw 
          values of the criteria in "simu_erg_werte". The order must correspond 
          to that which was determined by the weighted sum when defining the 
          evaluation. 
          The parameter "interactive" indicates whether the simulator was started 
          interactively as a user function, or whether it was called as a fitness 
          function in the context of evolution. In the former case, e.g. displays 
          can be made.
          If errors occur, the routine returns GLEAM_ERROR or GLEAM_FATAL in 
          "erg_state" and the function result is FALSE. Otherwise it is TRUE and 
          "erg_state" is GLEAM_OK.
          For further details see documentation of the module "simu.c" from 
          the "simu" package. 

GSTATE initApplSim (void);
          Gene model dependent initialization of the simulator. 
          In case of an error a corresponding message is generated and the 
          return status is set to GLEAM_FATAL. Otherwise, GLEAM_OK is returned.

BOOL   appl_init (BOOL  steu_par);
          Initialisierung des APPL-Packages und aller eingebundenen Anwendungs-
          Packages. Liefert TRUE, wenn alles gut ging.

--------------------------------------------------------------------------------
List of changes:
 1. Im Zuge der Erweiterung fuer n LSVs: Optionaler Steuerparameter
    fuer das Genetic-Repair (gen_rep_par_...).                       WJ 27.06.06
 2. Initialization of "gen_rep_par", which has become necessary in 
    the course of the new program data management.                   WJ 11.01.17
 3. New function initApplSim() for gene model dependent initializa-
    tion of the simulator.                                           WJ 06.06.18
--------------------------------------------------------------------------------
Status: o.k.                     Author: W.Jakob                Date: 06.06.2018
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
#include "simu.h"
#include "appl.h"
#include "applInt.h"


#include APPL_TEXTS



/* ======================== Package-global Variables: ======================= */



/* ============================= Local Constants: =========================== */



/* ======================== Local Type Declarations: ======================== */



/* ============================= Local Variables: =========================== */



/* ============================= Local Routines: ============================ */




/* ============================== do_appl_sim =============================== */
BOOL do_appl_sim (DOUBLE              *simu_erg_werte,
                  CHAIN_ELEM_PTR_TYPE  ch_ptr,
                  BOOL                 interaktiv,
                  GSTATE              *erg_state)
{
   BOOL  erg = FALSE;

   /* do_appl_sim */
   switch (add_appl_code)
   {
      case TSP_APPL:
      simu_erg_werte[0] = frand () * 200 + 50;                        /* test */
      erg        = TRUE;
      *erg_state = GLEAM_OK;
      /* fehler (APPL_SIM, 100, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED); */
      break; /* TSP_APPL */

      default:
      *erg_state = fatal (APPL_SIM, 1, add_appl_code,NO_INFO,"", BAD_APPL_CODE);
   } /* switch */
   return (erg);
} /* do_appl_sim */



/* ============================== initApplSim =============================== */
GSTATE initApplSim (void)
{
   GSTATE  retState;

   /* initApplSim */
   retState = GLEAM_OK;

   return (retState);
} /* initApplSim */



/* ================================ appl_init =============================== */
BOOL appl_init (BOOL  steu_par)
{
   BOOL  erg;

   /* appl_init */
   erg = (((gen_rep_par_query    = (char*)malloc(MAX_STR*sizeof(char))) != NULL) &&
          ((gen_rep_par_anzg     = (char*)malloc(MAX_STR*sizeof(char))) != NULL) &&
          ((gen_rep_par_kurzanzg = (char*)malloc(10     *sizeof(char))) != NULL)   );
   mit_gen_rep_par = FALSE;
   gen_rep_par     = 0.0;

   return (erg);
} /* appl_init */
