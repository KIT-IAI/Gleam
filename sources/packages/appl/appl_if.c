/*******************************************************************************
GLEAM/AE                                                                  V1.1.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: APPLication
                        Module: APPLication InterFace
 
Package: appl                   File: appl_if.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

Preliminary remark:
The extension of GLEAM or HyGLEAM by one or more new applications can basically 
be done in two ways:

1. Alternatively to each other and in addition to the standard applications (robot 
   path planning and math. benchmark functions (MBFs):
   All other modules of this package - if their routines are needed - have to 
   be replaced by application specific modules with the same interface. The replacing 
   routines must check if they are used in the correct application context.

2. In each case additive and additional to the standard applications:
   The routines of the other modules of this package have to be extended to inter-
   faces to the integrated applications. Depending on the application, they branch 
   to application-specific routines in additional packages.

The first method is described in the documentation "Applications Included in GLEAM 
and Integration of New Ones". The second one has never been tried so far, but is 
basically possible.

This interface module contains two routines for integrating additional applications, 
which must always be included. These must be extended for each new application.


The routines in detail:

BOOL read_add_appl (INT  *appl_id_code,
                    char *appl_id_str);
          Checks the "appl_id_str" for permissibility and sets "appl_id_code" to 
          ADD_APPL and the global variable "add_appl_code" to the corresponding 
          application ID in the positive case. In case of an error the routine 
          returns FALSE and the application variables remain unchanged.

GSTATE set_appl_texts (BOOL *std_ak_anwndg,
                       BOOL *std_segm_gen_appl);
          Sets the application strings for additional applications and the two 
          variables "mit_ext_simu" and "std_simu_interface" according to the value 
          of "add_appl_code". In addition, TRUE is returned in "std_ak_anwndg" and 
          in "std_segm_gen_appl" if standard chromosome generation or standard gene 
          modification and segment generation can be applied. If "add_appl_code" 
          is not valid, the same values are set as by the read_mod_file() function
          from module "mod_data.c" of the "hmod" package.
          In the SGS application initApplSim() is called and its return status is 
          returned. Otherwise the result is always GLEAM_OK.

--------------------------------------------------------------------------------
List of changes:
 1. Erweiterung um OPAL/V (OPAL_KENNUNG).                            WJ 09.03.01
 2. "i/r_par_ges_anz" werden bei OPAL/V nicht mehr auf 0 gesetzt!     
    "sim_mod_art_name" muss wg. ext. Sim. leer sein.                 WJ 12.03.01
 3. Integration des Drucksensor-Simulators vom ITEM.                 WJ 21.06.01
 4. Integration von GORBA, kombinatorische und permutatiosnbasierte
    Handlungsmodell fuer Standard-GLEAM.                             WJ 17.10.05
 4. GORBA: erweitertes kombinatorische Handlungsmodell f. Standard-
    GLEAM (Aktion zur ResAuswahlStrategie, keine Aktions-ResParams.  WJ 10.05.06
 5. Im Zuge der Erweiterung fuer n LSVs: Optionaler Steuerparameter
    fuer das genotypsche Repair (gen_rep_par_...) und fuer das phae-
    notypische (...PhaenoRep).                                       WJ 28.08.06
 4. Entfernung von "add_appl_hdr".                                   WJ 21.06.12
 5. Entfernung von  "save_rate_def".                                 WJ 13.12.16
 6. Ersetzung von "sim_mod_art_name" durch "extSimSubDir". Dieses 
    darf leer sein.                                                  WJ 10.03.17
 7. Einfuehrung von "simulateChromosome": direkte AK-Simulation.     WJ 27.04.17
 8. "extractChrParams" wird in "read_mod_file" oder "set_appl_texts" 
    direkt gesetzt, wenn die Parameter fuer die Simulation eines 
    OptJobs zu extrahieren sind. "displayableParams" gibt an, ob die
    Parameter eines Chromosoms bei der Ergebnisdarstellung eines 
    OptJobs angezeigt werden koennen.                                WJ 14.08.17
 9. New variables "factor" and "basicCycles" for experimental delay 
    of the evaluation.                                               WJ 09.02.18
10. Erweiterung um die SmartGridScheduling-Anwendung (SGS). Die Funk-
    tion set_appl_texts() ruft die neue Routine initApplSim() auf 
    und liefert deren Status ab.                                     WJ 06.06.18
10. Erweiterung um die Anwendung Leerfahrtenminimierung beim Projekt
    "Printed Materials and Systems" (PMS).                           WJ 30.10.19
11. OPAL/V: Introduction of interactive controllability 
    of phenotypic repair.                                            WJ 27.08.20
16. Deletion of everything that is not needed for the sample 
    implementation of a new application.                             WJ 23.10.20
--------------------------------------------------------------------------------
Status: Impl                     Author: W.Jakob                Date: 23.10.2020
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
#include "bew.h"
#include "appl.h"
#include "applInt.h"


#include APPL_TEXTS


/* =========================== Exported Variables: ========================== */
INT        add_appl_code = 0;    /* ID of the additional application          */
BOOL       mit_gen_rep_par,      /* TRUE: with control param.f.genetic repair */
           mitOptionPhaenoRep,   /* TRUE: with option for phenotypic repair   */
           mitPhaenoRepair;      /* TRUE: with phenotypic repair              */
DOUBLE     gen_rep_par,          /* control parameter for genetic repair      */
           gen_rep_par_ug,       /* lower limit of gen.repair control paramm. */
           gen_rep_par_og;       /* upper limit of gen.repair control paramm. */
char      *gen_rep_par_query,    /* query text for gen.repair control paramm. */
          *gen_rep_par_anzg,     /* output text for gen.repair control paramm.*/
          *gen_rep_par_kurzanzg; /* logfile output of gen.rep.control paramm. */


/* ======================== Package-global Variables: ======================= */



/* ============================= Local Constants: =========================== */



/* ======================== Local Type Declarations: ======================== */



/* ============================= Local Variables: =========================== */



/* ============================= Local Routines: ============================ */





/* ============================== read_add_appl ============================= */
BOOL read_add_appl (INT  *appl_id_code,
                    char *appl_id_str)
{
   BOOL  erg = TRUE;

   /* read_add_appl */
   if (strcmp (appl_id_str, TSP_KENNUNG) == 0)
      add_appl_code = TSP_APPL;                                        /* POP */
   else
      if (strcmp (appl_id_str, OPAL_KENNUNG) == 0)
         add_appl_code = OPAL_APPL;                                   /* OPAL */
      else
         if (strcmp (appl_id_str, ITEM_KENNUNG) == 0)
            add_appl_code = ITEM_APPL;                                /* ITEM */
         else
            if (strcmp (appl_id_str, GORBA_K_KENNUNG) == 0)
               add_appl_code = GORBA_KOMB_APPL;                    /* GORBA/K */
            else
               if (strcmp (appl_id_str, GORBA_P_KENNUNG) == 0)
                  add_appl_code = GORBA_PERM_APPL;                 /* GORBA/P */
               else
                  if (strcmp (appl_id_str, GORBA_K2_KENNUNG) == 0)
                     add_appl_code = GORBA_KOMB2_APPL;            /* GORBA/K2 */
                  else
		     if (strcmp (appl_id_str, SGS_KENNUNG) == 0)
			add_appl_code = SGS_APPL;                      /* SGS */
		     else
		        if (strcmp (appl_id_str, PMS_KENNUNG) == 0)
		 	   add_appl_code = PMS_APPL;                   /* PMS */
		        else
			   erg = FALSE;
   if (erg)
      *appl_id_code = ADD_APPL;
   return (erg);
} /* read_add_appl */



/* ============================= set_appl_texts ============================= */
GSTATE set_appl_texts (BOOL *std_ak_anwndg,
                       BOOL *std_segm_gen_appl)
{
   GSTATE  retState;

   /* set_appl_texts */
   retState = GLEAM_OK;
   switch (add_appl_code)
   {
      case TSP_APPL:                      /* old travelling salesman appl.    */
      strcpy (appl_name,      TSP_NAME);
      strcpy (appl_text,      TSP_TEXT);
      strcpy (extSimSubDir,   TSP_SUB_DIR);
      strcpy (opt_param_name, "");
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = FALSE;         /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = TRUE;          /* T: standard chromosome generation*/
      *std_segm_gen_appl = TRUE;          /* T: Standard chr.gen. and modific.*/
      i_par_ges_anz      =  0;            /* TSP application does not use     */
      r_par_ges_anz      =  0;            /* this kind of simulator interface */
      mit_gen_rep_par    = FALSE;         /* T: uses gen.repair control param */
      mitOptionPhaenoRep = FALSE;         /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = FALSE;         /* T: phenotypic repair active      */
      break; /* TSP_APPL */

      case OPAL_APPL:            /* here the simplified OPAL/V implementation */
      strcpy (appl_name,      OPAL_NAME); /* see appl_gb.h file               */
      strcpy (appl_text,      OPAL_TEXT); /* see appl_gb.h file.No longer used*/
      strcpy (extSimSubDir,   "");        /* sub directory for external simu. */
      strcpy (opt_param_name, "");        /* for display of decision variables*/
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = TRUE;          /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = TRUE;          /* T: standard chromosome generation*/
      *std_segm_gen_appl = TRUE;          /* T: Standard chr.gen. and modific.*/
      mit_gen_rep_par    = FALSE;         /* T: uses gen.repair control param */
      mitOptionPhaenoRep = TRUE;          /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = TRUE;          /* T: phenotypic repair active      */
      break; /* OPAL_APPL */

      case ITEM_APPL:                     /* worn out application             */
      strcpy (appl_name,      ITEM_NAME);
      strcpy (appl_text,      ITEM_TEXT);
      strcpy (extSimSubDir,   ""); 
      strcpy (opt_param_name, "");
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = FALSE;         /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = TRUE;          /* T: standard chromosome generation*/
      *std_segm_gen_appl = TRUE;          /* T: Standard chr.gen. and modific.*/
      mit_gen_rep_par    = FALSE;         /* T: uses gen.repair control param */
      mitOptionPhaenoRep = FALSE;         /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = FALSE;         /* T: phenotypic repair active      */
      break; /* ITEM_APPL */

      case GORBA_KOMB2_APPL:              /* GORBA-Implementation for combi-  */
      case GORBA_KOMB_APPL:               /* natorial and permutation based   */
      case GORBA_PERM_APPL:               /* genemodels for standard GLEAM    */
      strcpy (appl_name,      GORBA_NAME);
      strcpy (appl_text,      GORBA_TEXT);
      strcpy (extSimSubDir,   "");
      strcpy (opt_param_name, "");
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = FALSE;         /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = (add_appl_code != GORBA_KOMB2_APPL);
      *std_segm_gen_appl = TRUE;          /* T: Standard chr.gen. and modific.*/
      mit_gen_rep_par    = TRUE;          /* T: uses gen.repair control param */
      gen_rep_par        = 0.2;           /* default for genetic repair rate  */
      gen_rep_par_ug     = 0.0;           /* lower limit of the gen.rep. rate */
      gen_rep_par_og     = 1.0;           /* upper limit of the gen.rep. rate */
      strcpy (gen_rep_par_query, EVOPAR_GENO_QUERY);
      strcpy (gen_rep_par_anzg,  EVOPAR_GENO_ANZG);
      strcpy (gen_rep_par_kurzanzg, EVOPAR_GENO_HDR);
      mitOptionPhaenoRep = TRUE;          /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = FALSE;         /* T: phenotypic repair active      */
      break; /* GORBA_APPL */

      case SGS_APPL:                   /* SmartGridScheduling-Anwendung (SGS) */
      strcpy (appl_name,      SGS_NAME);
      strcpy (appl_text,      SGS_TEXT);
      strcpy (extSimSubDir,   ""); 
      strcpy (opt_param_name, "");
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = FALSE;         /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = TRUE;          /* T: standard chromosome generation*/
      *std_segm_gen_appl = TRUE;          /* T: Standard chr.gen. and modific.*/
      mit_gen_rep_par    = FALSE;         /* T: uses gen.repair control param */
      mitOptionPhaenoRep = FALSE;         /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = FALSE;         /* T: phenotypic repair active      */
      retState = initApplSim();
      break; /* SGS_APPL */

      case PMS_APPL:                      /*  */
      strcpy (appl_name,      PMS_NAME);
      strcpy (appl_text,      PMS_TEXT);
      strcpy (extSimSubDir,   ""); 
      strcpy (opt_param_name, "");
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = FALSE;         /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = TRUE;          /* T: standard chromosome generation*/
      *std_segm_gen_appl = TRUE;          /* T: Standard chr.gen. and modific.*/
      mit_gen_rep_par    = FALSE;         /* T: uses gen.repair control param */
      mitOptionPhaenoRep = FALSE;         /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = FALSE;         /* T: phenotypic repair active      */
      retState = initApplSim();
      break; /* PMS_APPL */

      default:
      strcpy (appl_name,      UNDEF_TXT);
      strcpy (appl_text,      "");
      strcpy (extSimSubDir,   "");
      strcpy (opt_param_name, "");
      mit_ext_simu       = FALSE;         /* T: external simulator is used    */
      displayableParams  = FALSE;         /* T: displayable decision variables*/
      extractChrParams   = FALSE;         /* T: extract dec.vars for ext. simu*/
      sim_up             = TRUE;          /* T: "simulator" ready to be used  */
      std_simu_interface = TRUE;          /* T: standard simulator interface  */
      always_sim         = FALSE;         /* T: simu regardless of chr.-flag  */
      *std_ak_anwndg     = FALSE;         /* T: standard chromosome generation*/
      *std_segm_gen_appl = FALSE;         /* T: Standard chr.gen. and modific.*/
      i_par_ges_anz      = 0;
      r_par_ges_anz      = 0;
      mit_gen_rep_par    = FALSE;         /* T: uses gen.repair control param */
      mitOptionPhaenoRep = FALSE;         /* T: with opt.of phenotypic repair */
      mitPhaenoRepair    = FALSE;         /* T: phenotypic repair active      */
   } /* switch */
   return (retState);
} /* set_appl_texts */

