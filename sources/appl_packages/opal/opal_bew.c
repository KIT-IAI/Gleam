/*******************************************************************************
GLEAM/AE                                                                  V1.1.0

			       G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

		       Package: OPAL/V
			Module: OPAL-BEWertung
 
Package: opal_v                File: opal_bew.c                  
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

The module contains chromosome interpretation and evaluation. It fulfills the 
corresponding interfaces of the APPL package of module "appl_sim".

The module also contains routines and declarations to formally satisfy the 
interfaces for LESAK and the math. benchmark functions.

The later added option of phenotypical repair addresses the phenomenon of hours 
without any production of batches, called "unused hours". These hours are ignored 
in so far, as they are deducted from the total time. The unused hours remain 
part of the returned assessment values, so that the can be used by the evaluation 
to calculate the fitness in oder to enable the evolution to reduce them.


Exported functions:

BOOL do_appl_sim (DOUBLE              *simu_erg_werte,
	          CHAIN_ELEM_PTR_TYPE  ch_ptr,
                  BOOL                 interaktiv,
                  GSTATE              *erg_state);
          Application-specific interpretation and assessment of the chromosome 
          passed in "ch_ptr" The function returns the raw values of the criteria 
          in "simu_erg_werte". The order must correspond to that which was 
          determined by the weighted sum when defining the evaluation. 
          The parameter "interactive" indicates whether the simulator was started 
          interactively as a user function, or whether it was called as a fitness 
          function in the context of evolution. In the former case, the result of 
          a possible phenotypic repair is shown. Furthermore, the hourly MA demand
          values are recorded in "schicht_profil" for test output.
          If errors occur, the function returns GLEAM_ERROR or GLEAM_FATAL in 
          "erg_state" and the function result is FALSE. Otherwise it is TRUE and 
          "erg_state" is GLEAM_OK.
          For further details see documentation of the module "simu.c" from 
          the "simu" package. 

GSTATE initApplSim (void);
          Gene model dependent initialization of the simulator. 
          Not required for this application.
          In case of an error a corresponding message is generated and the 
          return status is set to GLEAM_FATAL. Otherwise, GLEAM_OK is returned.

BOOL   appl_init (BOOL  steu_par);
          Initialization of the APPL package and all included application packages.
          Returns TRUE if everything went well.

--------------------------------------------------------------------------------
List of changes:
 1. Ausfilterung negativer Startzeiten wegen Rosenbrock.             WJ 12.03.01
 2. Die "null_stunden" werden von d. ermittelten ProdZeit abgezogen. WJ 16.03.01
 3. Bewertung: Nullstundenignorierung und Nullstundenabzug erst,
    wenn ProdZeit damit den Zielwert erreicht und SchichtspitzenMax 
    auf Zielwert ist. Beide Werte stammen aus dem TSK-File.          WJ 20.03.01
 4. Logging von 3. eingeschränkt und ins Simu-Logfile.               WJ 27.03.01
 5. Entfernung der Routine "write_to_simulog", die nun vom Package 
    "sys" zur Verfuegung gestellt wird.                              WJ 20.04.12
 6. Entfernung von BGI_MODE und des dazugehoerigen Codes.            WJ 21.06.12
 7. Streichung von durch 6. ueberfluessig gewordenen Konstanten.     WJ 24.05.15
 8. Anpassungen an die allg. Ueberarbeitung 2016/17.                 WJ 04.04.17
 9. Einfuehrung der Routinen "bewerte" und "do_opal_sim". Mit 
    letzterer kann auch durch die "chrDataList" simuliert werden.    WJ 13.04.17
10. Anpassung des Aufrufs von "write_to_simu_log".                   WJ 14.09.17
11. New switch MIT_NULL_STD_REP for phenotypical repair of unused 
    horurs as described above.                                       WJ 10.01.18
12. New switch OHNE_SIMU: Dummy-Version without real simulation.     WJ 29.01.18
13. New variables "factor" and "basicCycles" provided by "appl_if.c" 
    for experimental delay of the evaluation.                        WJ 09.02.18
14. New function initApplSim() for formal reasons.                   WJ 22.06.18
15. Introduction of interactive controllability of phenotypic repair.
    Removal of program parameter OPAL_MAX_PRODTIME. Usage of the TSK 
    file value as maximum shift peak, if specified.                  WJ 28.08.20
16. Deletion of everything that is not needed for the sample 
    implementation of a new application.                             WJ 23.10.20
--------------------------------------------------------------------------------
Status: o.k.                     Author: W.Jakob                Date: 23.10.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "appl.h"
#include "hmod.h"
#include "aufg.h"
#include "simu.h"
#include "opalInt.h"

#include ANW_TEXTS


#undef SIMU_TST
#undef LSV_TST
#undef BEW_TST



/* ========================== Exported Variables: ========================== */



/* ======================= Package-global Variables: ======================= */



/* =========================== Lokale Konstante: =========================== */
#define MAX_STUNDEN_ANZ      1800      /* 75 days, 225 shifts                */
#define MAX_SCHICHTEN_ANZ     225
#define MAX_ANLAGEN_ANZ         9
#define GES_ZEIT                0      /* indices of the evaluation criteria */
#define MAX_SP                  1
#define SP_ZUVIEL               2
#define DELAY                   3
#define MINDER_PROD             4
#define LUECKE                  5
#define START_ZEIT              0      /* indices of gene parameters         */
#define VERF_INDEX              1



/* ======================== Local Type Declarations: ======================== */
typedef struct BELEGUNGS_TYP
  {
     DOUBLE  ma_belegung;      /* staff allocation of facility in time unit   */
     INT     bel_verf_idx;     /* index of the product made in the facility   */
     BOOL    bel_beginn,       /* TRUE:  time unit (TU) is start of uccupancy */
	     bel_ende;         /* FALSE: time unit (TU) is end of uccupancy   */
  } BELEGUNGS_TYP;

typedef struct BELEGUNGS_TYP *BELEGUNGS_PTR_TYP;



/* =========================== Lokale Variable: ============================ */
static BELEGUNGS_PTR_TYP  belegung [MAX_ANLAGEN_ANZ];   /* occupancy matrix  */
static DOUBLE             akt_menge [MAX_ANLAGEN_ANZ];  /* act. produced qty */
static INT                verf_ende_zeit [MAX_VERFAHREN_ANZ];  /* act product*/
                                                               /* finish time*/
static INT               *beginn_flags,   /* # of product starts in this TU  */
                         *ende_flags;     /* # of product ends in this TU    */
static DOUBLE            *schicht_profil; /* profile of the shift peak values*/
static DOUBLE             schichtspitzen_max,     /* maximum shift peak      */
                          zuviel_schicht_spitzen, /* sum of exceedings of    */
                                                  /*shift peak limit         */
                          minder_prod;    /* missing production quantity     */
static INT                start_zeit,     /* TU of the first production start*/
                          ende_zeit,      /* TU of the last end of production*/
                          terminverzug,   /* TU sum of delayed batches       */
                          keine_belegung, /* counts failed allocations       */
                          null_stunden,   /* counts TUs without allocation   */
                          akt_schicht_anz; /* current number of shifts       */
static INT                std_feld_size;  /* size of line field in "belegung"*/



/* =========================== Local Routines: ============================= */
static BOOL bewerte          (DOUBLE *simu_erg_werte,
                              BOOL    interaktiv);

static void belege           (INT     start_ze,
                              INT     verf_idx);

static void reset_sim_data   (void);



/* ============================== do_appl_sim =============================== */
BOOL do_appl_sim (DOUBLE              *simu_erg_werte,
		  CHAIN_ELEM_PTR_TYPE  ch_ptr,
		  BOOL                 interaktiv,
		  GSTATE              *erg_state)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;      /* pointer for chromosome elements     */
   STR_TYPE  buf, buf2;
   INT       akt_ze;           /* present time unit (TU)                      */
   BOOL      erg_ok;           /* TRUE: Simu ok und Ergebnis ist zu bewerten  */

   /* do_appl_sim */
   *erg_state = GLEAM_OK;
   if (ch_ptr->u_el.u_xel.u_hdr.chain_length == 0)       /* empty chromosome! */
      return (FALSE);
   reset_sim_data ();

   /* ----------- process chromosome and create allocation matrix: ---------- */
   item_ptr = ch_ptr->u_el.next_action;
   while (item_ptr != NULL)
   { /* all along the chain */
#if defined(MITSIMULOG) && defined(SIMU_TST)
      write_to_simulog ("\n============== start of allocation: ==================", 
			KEEP_LOG_OPEN);
#endif /* MITSIMULOG und SIMU_TST */
      akt_ze = item_ptr->u_el.u_xel.u_act.i_params[START_ZEIT];
      if (akt_ze < 0)   /* Rosenbrock plant auch schon mal mit negativer Zeit */
      { /* neg. Zeit auf 0 setzen */
#if defined(MITSIMULOG) && defined(LSV_TST)
	 sprintf (buf, "LHC: negative start time (%d) set to 0.", akt_ze);
	 write_to_simulog (buf, KEEP_LOG_OPEN);
#endif /* MITSIMULOG und LSV_TST */
	 akt_ze = 0;
	 item_ptr->u_el.u_xel.u_act.i_params[START_ZEIT] = 0;
      } /* neg. Zeit auf 0 setzen */
      belege (akt_ze, item_ptr->u_el.u_xel.u_act.i_params[VERF_INDEX]);
      item_ptr = item_ptr->u_el.next_action;
   } /* all along the chain */
#if defined(MITSIMULOG) && defined(SIMU_TST)
   sprintf (buf, "\nNot assigned allocations: %d.", keine_belegung);
   write_to_simulog (buf, CLOSE_LOG);
#endif /* MITSIMULOG und SIMU_TST */
   erg_ok = bewerte(simu_erg_werte, interaktiv);
   return (erg_ok);
} /* do_appl_sim */



/* ================================= bewerte ================================ */
static BOOL bewerte (DOUBLE *simu_erg_werte,
                     BOOL    interaktiv)
{
   STRING_TYPE  buf;
   STR_TYPE     buf1, buf2;
   DOUBLE    transfer_menge,   /* current product quantity to be transferred  */
             akt_spitze,       /* for determining the current shift peak      */
             akt_bel_in_ze,    /* current employee assignment in a TU         */
             sollSSpitzenMax,  /* targeted shift peak maximum                 */
             delta;            /* auxiliary variable                          */
   INT       akt_ze,           /* present time unit (TU)                      */
             akt_verf_idx,     /* index of the present product                */
             akt_anl_idx,      /* index of the currently used facilty         */
             quell_verf_idx,   /* index of the predecessor prod. if prod.chain*/
             prod_zeit,        /* raw production time (ende_zeit-start_zeit)  */
             ii;               /* auxiliary variable                          */

   /* bewerte */
   /* Usage of the TSK file value as maximum shift peak, if specified. Other- */
   /* wise the value of the VL file is used. The evaluation settings should   */
   /* correspond with this value!                                             */
   if (!getDoubleVal(&sollSSpitzenMax, OPAL_MAX_SHIFTS_IDX, GLEAM_OK))
      sollSSpitzenMax = schichtspitzen_ug;           /* take value of VL file */

   /* ------------ determine material flow and product end time: ------------ */
   for (akt_ze = start_zeit;  akt_ze <= ende_zeit;  akt_ze++)
   { /* determine start/end times */
      if (beginn_flags[akt_ze] > 0)
	 for (akt_anl_idx = 0;  akt_anl_idx < MAX_ANLAGEN_ANZ;  akt_anl_idx++)
	 { /* search facility with starting product */
	    if (belegung[akt_anl_idx][akt_ze].bel_beginn)
	    { /* facility with starting product found */
	       akt_verf_idx = belegung[akt_anl_idx][akt_ze].bel_verf_idx;
	       if (verfahren[akt_verf_idx].mit_vorprod)
	       { /* pick up primary product */
		  quell_verf_idx = verfahren[akt_verf_idx].quell_verf_idx;
		  if (verfahren[quell_verf_idx].ges_menge >= 
		      verfahren[akt_verf_idx].max_kap)   /* enough pre-prod.? */
		     transfer_menge = verfahren[akt_verf_idx].max_kap;
		  else
		  { /* pre-product shortage */
		     transfer_menge = verfahren[quell_verf_idx].ges_menge;
#if defined(MITSIMULOG) && defined(SIMU_TST)
		     sprintf (buf, "VorProdMangel: Verfahren %2d fehlen %4.2f " 
			      "Chargen.", akt_verf_idx+1, 
			      verfahren[akt_verf_idx].max_kap - transfer_menge);
		     write_to_simulog (buf, KEEP_LOG_OPEN);
#endif /* MITSIMULOG und SIMU_TST */
		  } /* pre-product shortage */
		  akt_menge[akt_anl_idx]               = transfer_menge;
		  verfahren[quell_verf_idx].ges_menge -= transfer_menge;
	       } /* pick up primary product */
	       else
		  akt_menge[akt_anl_idx] = 1.0; /* Verf. od. VerfKetten-Start */
	    } /* facility with starting product found */
	 } /* search facility with starting product */
      if (ende_flags[akt_ze] > 0)
	 for (akt_anl_idx = 0;  akt_anl_idx < MAX_ANLAGEN_ANZ;  akt_anl_idx++)
	 { /* search facilities with ending product */
	    if (belegung[akt_anl_idx][akt_ze].bel_ende)
	    { /* facilities with ending product found */
	       akt_verf_idx = belegung[akt_anl_idx][akt_ze].bel_verf_idx;
	       verfahren[akt_verf_idx].ges_menge += akt_menge[akt_anl_idx];
	       if (verf_ende_zeit[akt_verf_idx] < akt_ze)   /* determine pro- */
		  verf_ende_zeit[akt_verf_idx] = akt_ze;    /* duct end time  */
	    } /* facilities with ending product found */
	 } /* search facilities with ending product */
   } /* determine start/end times */

   /* ---------------------- determine shift peaks: ---------------------- */
   akt_ze = start_zeit;
#if defined(MITSIMULOG) && defined(SIMU_TST)
   write_to_simulog ("\nShift profile of the first 80 TUs:", KEEP_LOG_OPEN);
#endif /* MITSIMULOG und SIMU_TST */
   while (akt_ze < ende_zeit)
   { /* determine peak allocation of all shifts */
      akt_spitze = 0.0;
      for (ii = 0;  ii < 8;  ii++)
      { /* determine peak allocation of one shift */
	 akt_bel_in_ze = 0.0;
	 for (akt_anl_idx = 0;  akt_anl_idx < MAX_ANLAGEN_ANZ;  akt_anl_idx++)
	 { /* determine employee allocation of TU */
	    akt_bel_in_ze += belegung[akt_anl_idx][akt_ze].ma_belegung;
	 } /* determine employee allocation of TU */
	 if (akt_bel_in_ze > akt_spitze)
	    akt_spitze  = akt_bel_in_ze;
	 else
	    if (akt_bel_in_ze == 0)
	       null_stunden++;                /* count TUs without allocation */
#if defined(MITSIMULOG) && defined(SIMU_TST)
	 if (akt_ze < 80)
	 {
	    sprintf (buf, "  employee allocation of TU %2d: %4.1f", 
		     akt_ze, akt_bel_in_ze);
	    write_to_simulog (buf, KEEP_LOG_OPEN);
	 }
#endif /* MITSIMULOG and SIMU_TST */
         akt_ze++;
      } /* determine peak allocation of one shift */
      if (interaktiv)                                     /* profile required */
	 schicht_profil[akt_schicht_anz++] = akt_spitze;  /* for display only */
      if (schichtspitzen_max < akt_spitze)
	 schichtspitzen_max = akt_spitze;
      delta = akt_spitze - sollSSpitzenMax;
      if (delta > 0)
	 zuviel_schicht_spitzen += delta;
   } /* determine peak allocation of all shifts */
#if defined(MITSIMULOG) && defined(SIMU_TST)
   write_to_simulog (" ", CLOSE_LOG);
#endif /* MITSIMULOG and SIMU_TST */

   /* -------------------------- determine results: ------------------------- */
   for (ii = 0;  ii < verf_anz;  ii++)
   { /* determine shortages and delays of deadlines */
      if (verfahren[ii].soll_menge > 0)
      { /* end of a product chain or stand-alone product */
	 minder_prod += (verfahren[ii].soll_menge - verfahren[ii].ges_menge);
#if defined(MITSIMULOG) && defined(SIMU_TST)
	 delta = verfahren[ii].soll_menge - verfahren[ii].ges_menge;
	 sprintf (buf, "Production shortfall: Product %2d is missing %4.1f batches.", 
		  ii+1, delta);
         if (delta > 0.0)
	    write_to_simulog (buf, CLOSE_LOG);
#endif /* MITSIMULOG and SIMU_TST */
      } /* end of a product chain or stand-alone product */
      if (verf_ende_zeit[ii] > verfahren[ii].soll_zeit) 
      { /* last batch finished delayed*/
	 terminverzug += (verf_ende_zeit[ii] - verfahren[ii].soll_zeit);
#if defined(MITSIMULOG) && defined(SIMU_TST)
	 sprintf (buf, "Delayed deadline : Product %2d delayed by %d TU.", 
		  ii+1, verf_ende_zeit[ii] - verfahren[ii].soll_zeit);
	 write_to_simulog (buf, CLOSE_LOG);
#endif /* MITSIMULOG and SIMU_TST */
      } /* last batch finished delayed */
   } /* determine shortages and delays of deadlines */
   if (minder_prod < 0.01)
      minder_prod = 0.0;                     /* death to the rounding errors! */
   prod_zeit = ende_zeit - start_zeit + 1;
   if (mitPhaenoRepair)
   { /* phenotypic repair of hours without any production */
      if (null_stunden > 0)
      { /* phenotypical repair: Ignoring unused hours */
	 sprintf(buf, PHAENO_REP_TXT, prod_zeit, null_stunden, prod_zeit - null_stunden);
	 prod_zeit = prod_zeit - null_stunden;
      } /* phenotypical repair: Ignoring unused hours */
      else
	 sprintf(buf, NO_PHAENO_REP_TXT);
  #if defined(MITSIMULOG) && defined(BEW_TST)
      write_to_simulog(buf, CLOSE_LOG);
  #endif /* MITSIMULOG and BEW_TST */
      if (interaktiv)
	 write_buf(get_line_anz()+1, buf);
   } /* phenotypic repair of hours without any production */
   simu_erg_werte[GES_ZEIT]    = prod_zeit;
   simu_erg_werte[MAX_SP]      = schichtspitzen_max;
   simu_erg_werte[SP_ZUVIEL]   = zuviel_schicht_spitzen;
   simu_erg_werte[DELAY]       = terminverzug;
   simu_erg_werte[MINDER_PROD] = minder_prod;
   simu_erg_werte[LUECKE]      = null_stunden;
#if defined(MITSIMULOG) && defined(SIMU_TST)
   if (interaktiv)
   { /* output of the shift peak profile */
      write_to_simulog ("Shift peak profile of the first 15 shifts:");
      strcpy (buf1, "");
      strcpy (buf2, "");
      for (ii = 0;  ii < 15;  ii++)
      {
	 sprintf (buf, "%4d ", ii+1);
	 strcat (buf1, buf);
	 sprintf (buf, "%4.1f ", schicht_profil[ii]);
	 strcat (buf2, buf);
      }
      write_to_simulog (buf1, KEEP_LOG_OPEN);
      write_to_simulog (buf2, KEEP_LOG_OPEN);
      write_to_simulog (" ", CLOSE_LOG);
   } /* output of the shift peak profile */
#endif /* MITSIMULOG and SIMU_TST */
   return (TRUE);
} /* bewerte */



/* ================================= belege ================================= */
static void belege (INT  start_ze,
                    INT  verf_idx)
{
   STR_TYPE  buf;
   DOUBLE  ma_bedarf;   /* employee requirements of a time interval           */
   INT     anl_idx,     /* index of a facility                                */
           ende_ze,     /* TU after the last TU of the current alloc. attempt */
           akt_ze,      /* current TU at allocation execution                 */
           interv_len,  /* length of a time interval                          */
           ii, jj;
   BOOL   frei;

   /* belege */
   anl_idx = verfahren[verf_idx].anl_idx;
#if defined(MITSIMULOG) && defined(SIMU_TST)
   sprintf (buf, "Intended allocation: From TU %d with batch of product %d using facility %d",
	    start_ze, verf_idx+1, anl_idx+1);
   write_to_simulog (buf, CLOSE_LOG);
#endif /* MITSIMULOG and SIMU_TST */
   while ((belegung[anl_idx][start_ze].ma_belegung != 0.0) && 
	  (start_ze < MAX_STUNDEN_ANZ))
      start_ze++;                         /* skip already existing allocation */
   if ((belegung[anl_idx][start_ze].ma_belegung == 0.0) &&
       ((MAX_STUNDEN_ANZ - start_ze) > verfahren[verf_idx].ges_dauer))
   { /* found unused TU with sufficient distance to MAX_STUNDEN_ANZ */
      ende_ze = start_ze + verfahren[verf_idx].ges_dauer; /*1TU more than EndTU*/
      frei    = TRUE;
#if defined(MITSIMULOG) && defined(SIMU_TST)
      sprintf (buf, "Allocation attempt from TU %d to TU %d", start_ze, ende_ze-1);
      write_to_simulog (buf, KEEP_LOG_OPEN);
#endif /* MITSIMULOG and SIMU_TST */
      for (ii = start_ze;  frei && (ii < ende_ze);  ii++)  /* check for      */
         frei = belegung[anl_idx][ii].ma_belegung == 0.0;  /* enough free TU */
      if (frei)
      { /* found an allocatable time slot */
	 akt_ze = start_ze;
         belegung[anl_idx][start_ze].bel_beginn = TRUE;          /* set begin */
	 beginn_flags[start_ze]++;                               /* flag      */
	 if (start_zeit > start_ze)
	    start_zeit = start_ze;            /* save the earliest start time */
	 for (ii = 0;  ii < verfahren[verf_idx].intervall_anz;  ii++)
	 { /* all time intervalls */
	    interv_len = verfahren[verf_idx].interv_dauer[ii];
	    ma_bedarf  = verfahren[verf_idx].interv_ma_bedarf[ii];
	    for (jj = 0;  jj < interv_len;  jj++)
	    { /* allocate all TU of the time interval */
	       belegung[anl_idx][akt_ze].ma_belegung  = ma_bedarf;
               belegung[anl_idx][akt_ze].bel_verf_idx = verf_idx;
	       akt_ze++;
	    } /* allocate all TU of the time interval */
	 } /* all time intervalls */
	 ende_ze--;                 /* correction: Now points to last used TU */
         belegung[anl_idx][ende_ze].bel_ende = TRUE;               /* set end */
	 ende_flags[ende_ze]++;                                    /* flag    */
	 if (ende_zeit < ende_ze)
	    ende_zeit = ende_ze;                      /* save latest end time */
#if defined(MITSIMULOG) && defined(SIMU_TST)
	 sprintf (buf, "Allocation from TU %d to TU %d", start_ze, ende_ze);
	 write_to_simulog (buf, CLOSE_LOG);
#endif /* MITSIMULOG und SIMU_TST */
      } /* found an allocatable time slot */
      else
      { /* freies Zeitfenster war zu klein */
#if defined(MITSIMULOG) && defined(SIMU_TST)
	 sprintf (buf, "Recursive allocation attempt starting from TU %d", ii);
	 write_to_simulog (buf, CLOSE_LOG);
#endif /* MITSIMULOG und SIMU_TST */
	 belege (ii, verf_idx);
      } /* freies Zeitfenster war zu klein */
   } /* found unused TU with sufficient distance to MAX_STUNDEN_ANZ */
   else
   { /* Belegung nicht moeglich */
      keine_belegung++;
#if defined(MITSIMULOG)
      write_to_simulog ("Allocation not possible.", CLOSE_LOG);
#endif /* MITSIMULOG */
   } /* Belegung nicht moeglich */
} /* belege */



/* ============================= reset_sim_data ============================= */
static void reset_sim_data (void)
{
   INT  ii;

   /* reset_sim_data */
   for (ii = 0;  ii < MAX_ANLAGEN_ANZ;  ii++)
      memset (belegung[ii], 0, std_feld_size);
   memset (akt_menge,      0, MAX_ANLAGEN_ANZ   * sizeof(DOUBLE));
   memset (beginn_flags,   0, MAX_STUNDEN_ANZ   * sizeof(INT));
   memset (ende_flags,     0, MAX_STUNDEN_ANZ   * sizeof(INT));
   memset (schicht_profil, 0, MAX_SCHICHTEN_ANZ * sizeof(DOUBLE));
   memset (verf_ende_zeit, 0, MAX_VERFAHREN_ANZ * sizeof(INT));
   for (ii = 0;  ii < verf_anz;  ii++)
      verfahren[ii].ges_menge = 0.0;
   schichtspitzen_max     = 0.0;
   zuviel_schicht_spitzen = 0.0;
   minder_prod            = 0.0;
   terminverzug           = 0;
   keine_belegung         = 0;
   start_zeit             = MAX_STUNDEN_ANZ;
   ende_zeit              = 0;
   null_stunden           = 0;
   akt_schicht_anz        = 0;
} /* reset_sim_data */



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
   INT   ii;
   BOOL  ok = TRUE;

   /* appl_init */
   std_feld_size = MAX_STUNDEN_ANZ * sizeof(BELEGUNGS_TYP);
   for (ii = 0;  (ii < MAX_ANLAGEN_ANZ) && ok;  ii++)
      ok = (belegung[ii] = (BELEGUNGS_TYP*)malloc(std_feld_size)) != NULL;
   if (ok)
   { /* allocation matrix created */
      beginn_flags   = (INT*)malloc(MAX_STUNDEN_ANZ*sizeof(INT));
      ende_flags     = (INT*)malloc(MAX_STUNDEN_ANZ*sizeof(INT));
      schicht_profil = (DOUBLE*)malloc(MAX_SCHICHTEN_ANZ*sizeof(DOUBLE));
      ok = (beginn_flags != NULL) && (ende_flags != NULL) && 
	   (schicht_profil != NULL);
   } /* allocation matrix created */

   if (ok)
   { /* stuff from appl_sim */ 
      ok = (((gen_rep_par_query    = (char*)malloc(MAX_STR*sizeof(char))) != NULL) &&
	    ((gen_rep_par_anzg     = (char*)malloc(MAX_STR*sizeof(char))) != NULL) &&
	    ((gen_rep_par_kurzanzg = (char*)malloc(10     *sizeof(char))) != NULL)   );
      mit_gen_rep_par = FALSE;
      gen_rep_par     = 0.0;
   } /* stuff from appl_sim */ 
   return (ok);
} /* appl_init */
