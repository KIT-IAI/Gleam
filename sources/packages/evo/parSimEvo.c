/*****************************************************************************************
GLEAM/AE                                                                            V1.0.0

                                    G L E A M  /  A E
       General Learning Evolutionary Algorithm and Method / Application Environment

                        Package: EVOlution
                         Module: parallel SIMulation based EVOlution
 
Package: evo                                                             File: parSimEvo.c
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
------------------------------------------------------------------------------------------

This module implements an assessment of all offspring of one generation at one step 
and thus, allows for a parallel execution of the necessary simulations. It replaces 
the module "evo.c" and has therefore the same interface, which include the following 
functions: evo_1gen(), pruef_und_speichere() und do_lsv().

The external simulation service (ESS) interface allows 
- the parallel simulation of a set of chromosomes stored in a chromosome list as imple-
  mented by the package "webIO", called a SimuJob consisting of a set of simulator runs.
- the parallel execution of an LHCJob consisting of a set of LHC runs. (expansion step, 
  see phases 1.b and 2.b below)


Phases of an optimization job of a population of m individuals producing l offspring per 
generation:
1.   initialisation of the start population                    m simulator runs
1.b) local improvement of a fraction of the start population   n LHC runs       (n <= m/5)

2.   assessment of the offspring of one generation             l simulator runs
2.b) local improvement of a fraction of these offspring        k LHC runs    (m <= k <= l)


The processing of a generation or of the initialisation of the start population is orga-
nized in passes as follows:

Pass: Initialisation of the Start Population:    Per Generation:
----------------------------------------------+-------------------------------------------
 1st  Selection of available individuals as   |  Creation of all offspring by genetic 
      requested and creation of the rest.     |  operators.
                  Transmission as a SimuJob chromsosome list to the ESS.

 2nd             Wait with TMO till reception of the corresponding result list. 
                            This may be interrupted by the user.

 3rd  Assessment of all newly created indi-   |  Assessment of all offspring based on the 
      viduals based on the result list.       |  result list. 
                                              |
 4th  Selection of the individuals to be      |  Selection of the offpring to be locally 
      locally improved.                       |  improved. 
                  Transmission as an LHCJob chromosome list to the ESS.

 5th             Wait with TMO till reception of the corresponding result list. 
                            This may be interrupted by the user.

 6th          Update of the data of all improved offspring based on the result list.

 7th                      -                   |  Processing of the offspring acceptance
                                              |  resulting in the next population.

The passes 4 to 6 belong to the expansion step introducing LHC impromevement, which is 
implemented later.


Relationship between "pop_feld" and "chrList":
The "chrList" contains pointers to chromosomes, the contents of which are to be copied to 
the chromosome list to be sent to the external simulation services. The result list entries 
use the same indices as used in the chromsosme list and in "chrList". Entries within the 
"chrList" may be NULL, which means that there is no chromosome for this index (index pair) 
to be evaluated.
In the initialization phase only those chromosomes from the "pop_feld" are copied to the
"chrList", which are newly generated and must be evaluated. As the indices in both lists 
are the same, the "chrList" will have empty fields (NULL) in the end. Only the first column 
of the "chrList" is used in this phase.
In the phase of processing a generation the "chrList" contains the offspring per pairing
in the row of the parent's index. The amount of these offspring varies from pairing to 
pairing, so that the rest of a row may be unused (NULL).

The dimension of the "chrList" is actRowsOfChrList x actColsOfChrList, which is adapted, 
if needed by a larger population or by a new EVO-file, which may generate more offspring 
per pairing. From the "chrList" matrix a subset of pop_size x maxOffsprAmount is used for 
a particular optimization job.

The planned extension of LHCJobs means that some chromosomes of the "chrList" will be 
improved by LHCs in a second pass after the assessment pass. If a chromosome of the 
"chrList" is selected for LHC treatment, its data are entered into a newly generated chro-
mosome list together with the LHC index and the LHC parameters such as an iteration limit. 
As the amount of chromosomes to undergo LHC treatment (i.e. the length of the chromosome 
list) is known only after the selection, this information is stored in the header of the 
chromosome list last, see updateChrListHeader() of module "listIO" of package "webIO".

The waiting for results may be interrupted by the user. After possible changes to the job 
list or jobs the job list is restarted for further processing. The variable "jobInterruptPhase" 
is used to manage the various phases of processing.


Exported routines in detail:
============================

Package-global routines in detail:

BOOL allocChrList (void);
        Deallocates an existing "chrList", if it is to samll and allocates a new one of 
        size "pop_size" x "maxOffsprAmount". The content of the "offspringList"-component 
        is set to null.
        Returns TRUE if there is sufficient memory available and FALSE otherwise (fatal 
        error). 
        The new sizes are stored in actRowsOfChrList" and "actColsOfChrList". The "chrList" 
        is too small, if at least one of the stored sizes is smaller than the the actual 
        size.

void clearInitialChrList (void);
        Clears the "chrList" of the initial population by setting for each column the 
        "offspringAmount" to zero and the first chromosome pointer to NULL.

BOOL clearChrList (BOOL  delChr);
        Clears the "chrList" by setting all "offspringAmount" entries to zero. 
        If "delChr" is set, all stored chromosomes are deleted, so that the resulting 
        "chrList" contains NULL-pointers only. Errors when deleting chromosomes cause an 
        error message and FALSE to be returned. TRUE is returned otherwise. 
        If "delChr" is not set, all pointers in the "offspringList" of "chrList" must be 
        NULL, which is checked. If they are not, a fatal error is reported and FALSE is 
        returned. TRUE otherwise.
        The function checks the existence of "chrList" and its actual sizes.

BOOL evaluateStartPop (void);
        The function implements passes 1 to 3 and 7 of the above table: The newly generated 
        part of the initial population contained in "pop_feld" is copied to "chrList", from
        which a chromosome list message is created and send to the simulation service. After 
        successful transmission, the function waits for the response (result list). This 
        is an active waiting (polling) of WAIT_FOR_READ msec per wait cycle and it may be 
        interrupted by the user. The time out of the waiting is controlled by the TSK-file 
        entry "timeout for ext. simulator [sec]".
        After successfull receipt of the result list it is first checked whether there are 
        as many results as expected. If so, all chromosomes referenced in the list are
        evaluated, updated, and deleted from the "chrList". A references to an empty entry 
        in the "chrList" is an error. Errors are usually reported as a simple GLEAM_ERROR. 
        If there unevaluated chromosomes in the result list their amount is reported in the 
        logfile. It is an error, if their share of the list exceeds a rate given by 
        ALLOWABLE_BAD_EVAL_FRACTION.
        The function returns TRUE, if the received results were all processed correctly.
        Otherwise, FALSE ist returned and the global variable "taste" indicates, whether 
        this is due to an error or because of a user interrupt. In case of an error, the 
        global variable "err_state" contains the actual error status. Fatal errors occur,
        if there is not enough memory or the function is called with an inappropriate value 
        of "jobInterruptPhase".

BOOL  evo_1gen (void);
        The function implements passes 1 to 3 and 7 of the above table: The newly generated 
        offspring of all individuals of the population contained in "pop_feld" are stored
        in the "chrList", from which a chromosome list message is created and send to the 
        simulation service. After successful transmission, the function waits for the 
        response (result list). This is an active waiting (polling) of WAIT_FOR_READ msec 
        per wait cycle and it may be interrupted by the user. The time out of the waiting 
        is controlled by the TSK-file entry "timeout for ext. simulator [sec]". 
        After successfull receipt of the result list it is first checked whether there are 
        as many results as expected. If so, all chromosomes referenced in the list are
        identified, evaluated, and their header is updated accordingly. Otherwise, an error 
        is reported. If there unevaluated chromosomes in the result list their amount is 
        reported in the logfile. If their share of the list exceeds a rate given by 
        ALLOWABLE_BAD_EVAL_FRACTION, an error is reported.  Errors are usually treated as 
        simple GLEAM_ERROR.  
        According to the actual acceptance rule, an offspring is either accepted and replaces 
        its parent or it is rejected and deleted.
        The function returns TRUE, if the received results were all processed correctly.
        Otherwise, FALSE ist returned and the global variable "taste" indicates, whether 
        this is due to an error or because of a user interrupt. In case of an error, the 
        global variable "err_state" contains the actual error status. Fatal errors occur,
        if there is not enough memory or the function is called with an inappropriate value 
        of "jobInterruptPhase".

BOOL  pruef_und_speichere (CHAIN_ELEM_PTR_TYPE *c_ptr, 
                           INT                  mut_index);
        The routine performs an application dependent genetic repair by a call to "repair_1_chain"
        for the chromosome "c_ptr". For all applications empty chromosome are deleted and for 
        the simulation service application nothing else is done. In case of the robot applica- 
        tion the LESAK plausibility test is performed. 
        After successful repair, the chromosome is stored in the "chrList" and its content is 
        written to the chromosome list message buffer.
        If no error occurs, TRUE is returned and FALSE otherwise. 

BOOL  do_lsv (BOOL  lamarck,
              INT   znr);
        Not yet implemented!

------------------------------------------------------------------------------------------
Changes with respect to version 1.0.0:
 1. Hammingabstand muss fuer XO/Reko nur groesser-gleich als ein Mindestwert sein, 
    damit ein Mindestwert von 0 bewirkt, dass XO/Reko immer stattfinden.       WJ 01/24/18
 2. "aufg.h" wird zum Include hinzugefuegt.                                    WJ 08/23/18
------------------------------------------------------------------------------------------
Status: Impl                          Author: W.Jakob                     Date: 08/23/2018
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "hmod.h"
#include "aufg.h"
#include "bew.h"
#include "webIO.h"
#include "simu.h"
#include "evo.h"
#include "evoInt.h"

#include GLOBAL_TEXTS
#include EVO_TEXTS


#undef PERF_TST
#ifdef GLO_CHECKS
  #undef   ALL_RESULTS_TST
  #undef   INTRP_TST
  #undef   ACCEPT_TST  
  #undef   ADI_TST                  /*  required ??? */
#endif /* GLO_CHECKS */




/* =============================== Exportierte Variable: ============================== */


/* ============================= Package-globale Variable: ============================ */
CHR_LIST_TYPE *chrList;      /* list of initial or offspring individuals per generation */
BOOL     accepted,           /* TRUE = mind. 1 Offspring wird in einer Gen. akzeptiert  */
         accept_improved;    /* TRUE = mind. 1 Offspring verbessert Deme                */



/* ================================= Lokale Konstante: ================================ */
#define ALLOWABLE_BAD_EVAL_FRACTION    0.05

#define PROPORTIONAL          0           /* Code fuer Partnerwahl-Strategie  */
#define RANK                  1           /* Code fuer Partnerwahl-Strategie  */
#define MATE_SELECTION     RANK           /* aktuelle Partnerwahl-Strategie   */



/* ============================= Lokale Typdeklarationen: ============================= */



/* ================================= Lokale Variable: ================================= */
static INT     actRowsOfChrList = 0,  /* actual population size, 1st dim. of chrList    */
               actColsOfChrList = 0;  /* actual max. offspring amount, 2nd dim. chrList */
static INT     lastWaitCount,         /* saves "waitCtr" of at job interruption         */
               chrPerGenAmount;       /* amount of offspring per generation             */
                            

static DOUBLE               neue_lsv_note;   /* Durch das LSV ermittelte Note */
#ifdef MIT_LSV
  static INT                best_child_idx;  /* adapt.all Idx d.besten offspr */
  static LONG               aufwand;         /* LSV-Aufwand in Evaluationen   */
#endif /* MIT_LSV */



/* ================================= Lokale Routinen: ================================= */




/* =================================== allocChrList =================================== */
BOOL allocChrList (void)
{
   INT    ii;
   BOOL   completeNew,             /* TRUE, if chrList must be allocated completely new */
          res;                     /* TRUE, if new chrList was allocated successfully   */

   /* allocChrList */
   res = TRUE;
   if ((actRowsOfChrList < pop_size) || (actColsOfChrList < maxOffsprAmount)) 
   { /* the actual "chrList" is too small */
      if (chrList != NULL)
      { /* deallocation */
	 for (ii = 0;  ii < actRowsOfChrList;  ii++) 
         { /* free allocated rows */
	    free(chrList[ii].offspringList);
	    chrList[ii].offspringList = NULL;
	 } /* free allocated rows */
	 if (actRowsOfChrList < pop_size)
         { /* free allocated column */
	    free(chrList);
	    chrList = NULL;
	 } /* free allocated column */
      } /* deallocation */
      if (chrList == NULL)
      { /* allocate new column */
	 chrList = (CHR_LIST_TYPE*)malloc(pop_size * sizeof(CHR_LIST_TYPE));
	 if (chrList == NULL)
	    res = FALSE;
	 else
	    actRowsOfChrList = pop_size;
      } /* allocate new column */
      if (res)
      { /* new column successfully allocated */
	 for (ii = 0;  (ii < pop_size) && res;  ii++) {
	    chrList[ii].offspringList = (CHAIN_ELEM_PTR_TYPE*)malloc(maxOffsprAmount * 
								     sizeof(CHAIN_ELEM_PTR_TYPE));
	    res = (chrList[ii].offspringList != NULL);
	    if (res)
	       memset(chrList[ii].offspringList, 0, maxOffsprAmount * sizeof(CHAIN_ELEM_PTR_TYPE));
	 }
	 if (res)
	    actColsOfChrList = maxOffsprAmount;
      } /* new column successfully allocated */
      if (!res)
      { /* not enough memory */
	 actRowsOfChrList = 0;
	 actColsOfChrList = 0;
	 chrList          = NULL;
	 err_state = fatal(EVO_PAR_SIM_EVO, 1, NO_INFO, NO_INFO, "chrList", NOT_ENOUGH_MEM);
      } /* not enough memory */
   } /* the actual "chrList" is too small */
   return (res);
} /* allocChrList */



/* ================================ clearInitialChrList =============================== */
void clearInitialChrList (void)
{
   INT  row;

   /* clearInitialChrList */
   if (chrList != NULL)
   { /* "chrList" exists */
      for (row = 0;  row < actRowsOfChrList;  row++)
      { /* all rows: first and only chromosome is newly generated */
	 chrList[row].offspringAmount  = 0;
	 chrList[row].offspringList[0] = NULL;
      } /* all rows: first and only chromosome is newly generated */
   } /* "chrList" exists */
} /* clearInitialChrList */



/* =================================== clearChrList =================================== */
BOOL clearChrList (BOOL  delChr)
{
   INT     row, col,
           rowLimit, colLimit,
           nonNullCtr;           /* counts all non null pointers in all "offspringList" */
   GSTATE  retState;             /* local return state of called function               */
   BOOL    res;

   /* clearChrList */
   nonNullCtr = 0;
   res        = TRUE;
   if (chrList != NULL)
   { /* "chrList" exists */
      if (pop_size <= actRowsOfChrList)
	 rowLimit = pop_size;
      else
	 rowLimit = actRowsOfChrList;
      if (maxOffsprAmount <= actColsOfChrList)
	 colLimit = maxOffsprAmount;
      else
	 colLimit = actColsOfChrList;

      for (row = 0;  (row < rowLimit) && res;  row++)
      { /* all rows or individuals */
	 chrList[row].offspringAmount = 0;
	 for (col = 0;  (col < colLimit) && res;  col++)
	    if (chrList[row].offspringList[col] != NULL)
	       if (delChr) 
	       { /* delete stored chromosomes */
		  retState = delete_chain(&(chrList[row].offspringList[col]));
		  if (retState != GLEAM_OK) 
                  { /* chromosome ill structured and not deletable*/
		     res        = FALSE;
		     err_state |= retState;
		  } /* chromosome ill structured and not deletable*/
	       } /* delete stored chromosomes */
	       else
		  nonNullCtr++;
      } /* all rows or individuals */
      if (nonNullCtr != 0) {
	 err_state = fatal(EVO_PAR_SIM_EVO, 5, nonNullCtr, 0, CHR_PTR_TXT, TOO_MANY_ITEMS_ERR);
	 res = FALSE;
      }
      else
	 if (!res)
	    err_state = fatal(EVO_PAR_SIM_EVO, 6, NO_INFO, NO_INFO, NO_LIST_RESET_TXT, CHROMOSOME_LIST_ERR);
   } /* "chrList" exists */
   return (res);
} /* clearChrList */



/* ================================= evaluateStartPop ================================= */
BOOL evaluateStartPop (void)
{
   STRING_TYPE  buffer;
   STR_TYPE     buf;
   DOUBLE  badEvalShare,    /* share of unsuccessfull evaluations                       */
           fitness;         /* fitness of the actual processed chromosome               */
   INT     indivIdx,        /* common index of a chromosome in "pop_feld" and "chrList" */
           resListStatus,   /* 0=ok,  >0: number of bad evaluations,  0<: error         */
           resultAmount,    /* amount of result list entries (should be ="toBeEvalCount"*/
           unevalChrCount,  /* counts the number of unevaluated chromosomes (final test)*/
           ii, ij, idummy;
   BOOL    result;          /* TRUE as long as no errors occur. Final function result   */

   /* evaluateStartPop */
   taste  = FALSE;                                                       /* reset taste */
   result = FALSE;
#ifdef INTRP_TST
   printf("evaluateStartPop: Phase=%d --- Start ---\n", jobInterruptPhase); 
#endif /* INTRP_TST */
   if (jobInterruptPhase == POP_INIT_GEN_PHASE)
   { /* no interrupt */
      lastWaitCount = 0;                                          /* reset wait counter */
      clearInitialChrList();
      deleteOldResList();     /* file test version: delete a possibly existing old file */

      /* ------------- copy all chromosomes to be evaluated to chrList ---------------- */
      result = writeChrListHeader(toBeEvalCount);
      for (indivIdx = 0;  (indivIdx < pop_size) && result;  indivIdx++)
	 if (pop_feld[indivIdx].new_chain) {
	    chrList[indivIdx].offspringList[0] = pop_feld[indivIdx].chain_ptr;
	    result = writeChrToList(chrList[indivIdx].offspringList[0], indivIdx, 0);
	 }
#ifdef INTRP_TST
      printf("evaluateStartPop: Phase=%d not interupted. result=%d lastWaitCount=%d\n", 
	     jobInterruptPhase, result, lastWaitCount);
#endif /* INTRP_TST */

      /* ------------------- send chromosome list and wait for reply ------------------ */
      if (result)
	 if (sendChrList2OptServ())
	    if (!waitForResultList(&lastWaitCount)) 
	    { /* interrupt or error */
#ifdef INTRP_TST
	       printf("evaluateStartPop: Phase=%d Intrpt! taste=%d lastWaitCount=%d\n", 
		      jobInterruptPhase, taste, lastWaitCount); 
#endif /* INTRP_TST */
               result = FALSE;
	       if (taste)
		  jobInterruptPhase = POP_INIT_EVAL_PHASE;
	       else /* error! */
		  err_state |= fehler(EVO_PAR_SIM_EVO, 10, 
				      (LONG)((simuResultWaitCycles * WAIT_FOR_READ) / 1000), 
				      NO_INFO, RES_LIST_TXT, TMO_READ_ERR);
	    } /* interrupt or error */
   } /* no interrupt */
   else /* interrupted */
      if (jobInterruptPhase == POP_INIT_EVAL_PHASE)
      { /* interrupted while evaluating start pop */
#ifdef INTRP_TST
	 printf("evaluateStartPop: Phase=%d  Interupted! lastWaitCount=%d\n", jobInterruptPhase, lastWaitCount);
#endif /* INTRP_TST */
	 if (waitForResultList(&lastWaitCount))
	    result = TRUE;
	 else 
	 { /* interrupt or error */
	    if (!taste)
	       err_state |= fehler(EVO_PAR_SIM_EVO, 12, (LONG)((simuResultWaitCycles * WAIT_FOR_READ) / 1000), 
				   NO_INFO, RES_LIST_TXT, TMO_READ_ERR);
	 } /* interrupt or error */
      } /* interrupted while evaluating start pop */
      else
	 err_state = fatal(EVO_PAR_SIM_EVO, 13, jobInterruptPhase, NO_INFO, "", BAD_PHASE_OF_OPTJOB_PROC);

#ifdef INTRP_TST
   printf("evaluateStartPop: Phase=%d --- taste=%d result=%d lastWaitCount=%d  err_state=%d\n", 
	  jobInterruptPhase, taste, result, lastWaitCount, err_state); 
#endif /* INTRP_TST */

   /* -------------------------- check and pocess result list ------------------------- */
   if (result)
   { /* result list received */
      if (getResultListHeader(&resListStatus, &resultAmount)) 
      { /* Result list header successfully read and status ok */
	 if (resultAmount == toBeEvalCount)
	 { /* returned result list contains all required result entries */
	    if (resListStatus > 0) 
	    { /* report bad number of entries */
	       badEvalShare = (DOUBLE)resListStatus;
	       result = (badEvalShare / (DOUBLE)toBeEvalCount) < ALLOWABLE_BAD_EVAL_FRACTION;
	       sprintf(buf, BAD_SIMU_RUNS_TXT, resultAmount, resListStatus);
	       write_to_log(buf, CLOSE_LOG);
	       if (!result)                  /* too many results of faulty simulations? */
		  err_state |= fehler(EVO_PAR_SIM_EVO, 14, resListStatus, 
				      l_round(ALLOWABLE_BAD_EVAL_FRACTION * toBeEvalCount), 
				      BAD_RES_ITEMS_TXT, TOO_MANY_ITEMS_ERR);
	    } /* report bad simulator runs */
	    for (ii = 0;  (ii < toBeEvalCount) && result;  ii++)
	    { /* process returned result list entries */
	       result = getNextResults(simu_erg, &indivIdx, &idummy);
	       if (result)
		  if (chrList[indivIdx].offspringList[0] == NULL) {
		     sprintf(buf, "%d", indivIdx);
		     err_state |= fehler(EVO_PAR_SIM_EVO, 16, NO_INFO, NO_INFO, buf, NO_CHAIN);
		     result     = FALSE;
		  }
	       if (result)
	       { /* process this result entry */
		  fitness = ak_bewerten (CALC_FITNESS);
		  for (ij = 0;  ij < akt_anz_krit;  ij++) 
		     chrList[indivIdx].offspringList[0]->u_el.u_xel.u_hdr.header_par_ptr->sim_erg_werte[ij] = 
			simu_erg[ij];
		  chrList[indivIdx].offspringList[0]->u_el.u_xel.u_hdr.fitness_note  = fitness;
		  chrList[indivIdx].offspringList[0]->u_el.u_xel.u_hdr.chain_status |= SIMULATED;
		  chrList[indivIdx].offspringList[0] = NULL;  /* chr also stored in "pop_feld" */
		  pop_feld[indivIdx].note = fitness;
		  berechne_guete(&(pop_feld[indivIdx].guete), fitness);
		  if (record_evo_best && (fitness > best_ges_note))
                  { /* record best of evolution and there was an improvement */
		     if (statistik_pro_gen)
                     { /* with statistics per generation: log best indiv */
			sprintf(buffer, "#* %e %13.6f ", calcElapsedTimeMS(), fitness);
			for (ij = 0;  ij < akt_anz_krit;  ij++) {
			   sprintf(buf, " %.*g", erg_genauigkeit, simu_erg[ii]);
			   strcat(buffer, buf);
			}
			write_to_log(buffer, CLOSE_LOG);
		     } /* with statistics per generation: log best indiv */
#ifdef GLEAM_USR
		     bewert_data_to_best_data(fitness); /* data of the best chr. so far */
#else /* no GLEAM_USR */
		     best_ges_note = fitness;
		     bewert_data_to_int_data (fitness);      /* for results in RES-file */
		     save_simu_erg (res_fspec, TRUE, FALSE); /* ..., new file, with Idx */
#endif /* no GLEAM_USR */
		  } /* record best of evolution and there was an improvement */
	       } /* process this result entry */
	    } /* process returned result list entries */
#ifdef ALL_RESULTS_TST
	    unevalChrCount = 0;
	    for (indivIdx = 0;  indivIdx < pop_size;  indivIdx++)
	       if (!(pop_feld[indivIdx].chain_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED))
		  unevalChrCount++;
	    if (unevalChrCount) 
            { /* there are unevaluated chromosomes! */
	       result = FALSE;
	       sprintf(buf, "*** There are %d unevaluated chromosomes in the start population!", 
		       unevalChrCount);
	       write_to_log(buf, CLOSE_LOG);
	    } /* there are unevaluated chromosomes! */
#endif /* ALL_RESULTS_TST */
	    if (jobInterruptPhase == POP_INIT_EVAL_PHASE)
	       last_psize = pop_size;
	 } /* returned result list contains all required result entries */
	 else {
	    err_state |= fehler(EVO_PAR_SIM_EVO, 17, toBeEvalCount, resultAmount, 
				BAD_RES_AMOUNT_TXT, BAD_VALUE_ERR);
	    result     = FALSE;
	 }
      } /* Result list header successfully read and status ok */
      else
	 result = FALSE;
   } /* result list received */
#ifdef INTRP_TST
   printf("evaluateStartPop: Phase=%d --- Ende ---  taste=%d result=%d err_state=%d\n", 
	  jobInterruptPhase, taste, result, err_state);
#endif /* INTRP_TST */
   return (result);
} /* evaluateStartPop */



/* ===================================== evo_1gen ===================================== */
BOOL evo_1gen (void)
{
   DOUBLE *ham_ptr,              /* points to the hamming value stored in pop_feld[]    */
           fitness,              /* fitness of an offspring                             */
           bestChildFitn,        /* fitness of best offspring of a pairing              */
           badEvalShare;         /* share of unsuccessfull evaluations                  */
   INT     max_segm_anz,         /* for calculating the maximum of required memory for  */
           max_ch_len,           /* the generation of 2 offspring by cross-over         */
           partner,              /* index of mate in pop_feld[], cf. "partner_ptr"      */
           resListStatus,        /* 0=ok,  >0: number of bad evaluations,  0<: error    */
           resultAmount,         /* amount of result list entries                       */
           indivIdx,             /* parent index of received result of child            */
           childIdx,             /* child index of received result of child             */
           bestChildIdx,         /* index of best child in offspringList[] of chrList[] */
           ii, ij;
   BOOL    parentIsNotDemeBest,  /* parent is not the best individual of the deme       */
           replace,              /* TRUE, if child replaces parent                      */
           genOK;                /* TRUE, if the generation was processed successfully  */
#ifdef MIT_LSV
   DOUBLE  lsv_opt_p,            /* Wahrscheinlichkeit der LSV-Anwendungen    */
           lsv_anw_ist_rate,     /* Rate der tatsaechlichen LSV-Anwendungen   */
           best_evo_note,        /* Beste Note der Evo-Kinder                 */
           f_alt,                /* alte Fitness vor dem LSV-Lauf             */
           fg;                   /* fitness gain                              */
   LONG    indiv_ctr_all_start,  /* indiv_ctr vor LSV-Opt. aller Nachkommen   */
           indiv_ctr_lsv_start;  /* indiv_ctr bei Beginn eines einzelnen LSVs */
   INT     best_lsv_child_idx,   /* Index des besten LSV-Kindes               */
           lsv_done_anz,         /* Anzahl tatsaechlich LSV-verbessert.Kinder */
           all_p_nkl,            /* Notenklasse zur all-Adaption              */
           akt_nkl;              /* Aktuelle Notenklasse der LSV-Adaption     */
   BOOL    apply_lsv,            /* TRUE, wenn LSV anzuwenden ist             */
           lamarck;              /*  */
   STR_TYPE  str1, str2;
#endif /* MIT_LSV */
  
   /* evo_1gen */
   accepted        = FALSE;
   accept_improved = FALSE;
   genOK           = FALSE;
   akt_indiv       = 0;
   akt_gen_op      = 0;
#if defined(INTRP_TST) || defined(PERF_TST)
   printf("evo_1gen: Phase=%d --- Start ---  taste=%d\n", jobInterruptPhase, taste); fflush(stdout);
#endif /* INTRP_TST or PERF_TST */

   if (taste)
      taste = FALSE;                                       /* Unterbrechung durch Taste */
   else
   { /* Keine Unterbrechung durch Taste */
      lastWaitCount     = 0;            /* reset wait counter as there was no interrupt */
      jobInterruptPhase = OFFSPRING_GEN_PHASE;
      if (!clearChrList(REPORT_CHR_ENTRIES))
	 return (FALSE);
      if (!writeChrListHeader(1))    /* the number of chromosomes will be updated later */
      { /* not enough memory */
	 mem_out = TRUE;
	 return (FALSE);
      } /* not enough memory */
   } /* Keine Unterbrechung durch Taste */

#ifdef ACCEPT_TST
   dumpPopFeld ("evo_1gen", pop_size);
#endif /* ACCEPT_TST*/
#ifdef INTRP_TST
   printf("evo_1gen: Phase=%d  taste=%d\n", jobInterruptPhase, taste); 
#endif /* INTRP_TST */

   if (jobInterruptPhase == OFFSPRING_GEN_PHASE)
   { /* phase of offspring generation */
      chrPerGenAmount = 0;
      /* ---------------- Erzeuge alle Nachkommen, bei Fehler return(): --------------- */
      while (akt_indiv <= max_pop_index)
      { /* offspring generation of all individuals */
	 akt_ptr = pop_feld[akt_indiv].chain_ptr;

	 /* ------------------------------- Partnerwahl: ------------------------------ */
	 switch (MATE_SELECTION) 
         {
            case PROPORTIONAL:                                      /*not supported yet */
	       err_state = fatal (EVO_PAR_SIM_EVO, 10, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
            return (FALSE);
            /* break;    aktivieren, wenn Implementierung! */

            default:                    /* Waehle Partner mittels linear rank selection */
	       ham_ptr = partnerwahl (akt_indiv, &partner, &(chrList[akt_indiv].worstDemeFitness), 
				      &(chrList[akt_indiv].bestDemeFitness));
            break;
         }
	 partner_ptr = pop_feld[partner].chain_ptr;    

	 /* ----------------- Ermittlung des Platzbedarfs und MemTest: ---------------- */
	 /* Maximaler Platzbedarf eines XO-Operators plus 1 grosses BestChild (3AKs):   */
	 /* 2 x Summe der Laengen beider Ketten plus 4 Headerelemente und               */
	 /* 2 x Summe der Segmentanzahl beider Ketten plus SegmAnz pro Descriptor wegen */
	 /*           moeglichem Verschnitt                                             */
	 max_ch_len   = 2 * (partner_ptr->u_el.u_xel.u_hdr.chain_length +
			     akt_ptr->u_el.u_xel.u_hdr.chain_length      ) + 4;
	 max_segm_anz = 2 * (partner_ptr->u_el.u_xel.u_hdr.segment_anz +
			     akt_ptr->u_el.u_xel.u_hdr.segment_anz) + SEGM_PTR_ANZ;
	 if (!enough_free_mem (AK_HEADER, max_ch_len, max_segm_anz)) 
         { /* Speicher beschaffen */
	    err_state = reclaim_memory (AK);
	    if (!enough_free_mem (AK_HEADER, max_ch_len, max_segm_anz) || (err_state != GLEAM_OK)) 
	    { /* Kein Speicher beschaffbar oder Fehler: Abbruch */
	       mem_out = TRUE;
	       return (FALSE);
	    } /* Kein Speicher beschaffbar oder Fehler: Abbruch */
	 } /* Speicher beschaffen */

	 /* --------- Ermittlung des Hammingabstands und der XO-Zulaessigkeit: -------- */
	 if ((*ham_ptr) == NO_HAM)                       /* Hamming-Abstand unbekannt ? */
	    *ham_ptr = ham_dist (akt_ptr, partner_ptr);  /* Hamming-Abstand bestimmen!  */
	 do_xo = (xo_min_ham_dist <= *ham_ptr); /* Hamming-Abstand ausreichend fuer XO? */

	 /* ---------------- Durchfuehrung der genetischen Operationen: --------------- */
	 if (!do_g_operationen ())
	    return (FALSE);          /* "melde_evo_crash" bereits in "do_g_operationen" */
	 chrList[akt_indiv].offspringAmount = anz_children;  /* offspr amount per indiv */
	 akt_indiv++;                                          /* next chromosome       */
	 akt_gen_op = 0;                                       /* 1st genetic operation */
	 chrPerGenAmount += anz_children;
      } /* offspring generation of all individuals */
#if defined (INTRP_TST) || defined(PERF_TST)
      printf("evo_1gen: Phase=%d --- offspring generated --- not interrupted chrPerGenAmount=%d\n", 
	     jobInterruptPhase, chrPerGenAmount); fflush(stdout);
#endif /* INTRP_TST or PERF_TST */

      /* ------- send chromosome list to simulation services and wait for reply: ------ */
      if (updateChrListHeader(chrPerGenAmount))
	 if (sendChrList2OptServ())
	    if (waitForResultList(&lastWaitCount))
	       genOK = TRUE;
	    else 
            { /* interrupt or error */
#ifdef INTRP_TST
	       printf("evo_1gen: Phase=%d Intrpt! lastWaitCount=%d taste=%d\n", 
		      jobInterruptPhase, lastWaitCount, taste);
#endif /* INTRP_TST */
	       if (taste)
		  jobInterruptPhase = GEN_OFFSPRING_EVAL_PHASE; /* interrupt during eval*/
	       else
		  err_state |= fehler(EVO_PAR_SIM_EVO, 20, 
				      (LONG)((simuResultWaitCycles * WAIT_FOR_READ) / 1000), 
				      NO_INFO, RES_LIST_TXT, TMO_READ_ERR);
	    } /* interrupt or error */
   } /* phase of offspring generation */
   else 
   { /* continue interrupted waiting for result list */
#ifdef INTRP_TST
      printf("evo_1gen: Phase=%d Interrupted! lastWaitCount=%d\n", jobInterruptPhase, lastWaitCount);
#endif /* INTRP_TST */
      if (jobInterruptPhase == GEN_OFFSPRING_EVAL_PHASE) 
      { /* interrupted while evaluating offspring */
	 if (waitForResultList(&lastWaitCount))
	    genOK = TRUE;
	 else /* interrupt or error */
	    if (!taste)
	       err_state |= fehler(EVO_PAR_SIM_EVO, 21, (LONG)((simuResultWaitCycles * WAIT_FOR_READ) / 1000), 
				   NO_INFO, RES_LIST_TXT, TMO_READ_ERR);
      } /* interrupted while evaluating offspring */
      else
	 err_state = fatal(EVO_PAR_SIM_EVO, 22, jobInterruptPhase, NO_INFO, "", BAD_PHASE_OF_OPTJOB_PROC);
   } /* continue interrupted waiting for result list */

#if defined(INTRP_TST) || defined(PERF_TST)
   printf("evo_1gen: Phase=%d --- start processing result list --- taste=%d genOK=%d lastWaitCount=%d\n", 
	  jobInterruptPhase, taste, genOK, lastWaitCount); fflush(stdout); 
#endif /* INTRP_TST or PERF_TST */

   /* ----------------- process result list and apply acceptance rule: ---------------- */
   if (genOK)
   { /* process received result list */
      if (getResultListHeader(&resListStatus, &resultAmount)) 
      { /* result list header successfully read */
	 if (resultAmount == chrPerGenAmount)
	 { /* returned result list contains all required result entries */
	    if (resListStatus > 0) 
	    { /* report bad number of entries */
	       badEvalShare = (DOUBLE)resListStatus;
	       genOK = (badEvalShare / (DOUBLE)chrPerGenAmount) < ALLOWABLE_BAD_EVAL_FRACTION;
	       sprintf(buf3, BAD_SIMU_RUNS_TXT, resultAmount, resListStatus);
	       write_to_log(buf3, CLOSE_LOG);
	       if (!genOK)                  /* too many results of faulty simulations? */
		  err_state |= fehler(EVO_PAR_SIM_EVO, 23, resListStatus, 
				      l_round(ALLOWABLE_BAD_EVAL_FRACTION * chrPerGenAmount), 
				      BAD_RES_ITEMS_TXT, TOO_MANY_ITEMS_ERR);
	    } /* report bad simulator runs */

#if defined(INTRP_TST) || defined(PERF_TST)
	    printf("evo_1gen: Phase=%d --- start of offspring acceptance --- genOK=%d\n", 
		   jobInterruptPhase, genOK); fflush(stdout); 
#endif /* INTRP_TST or PERF_TST */

	    /* --------------------- process result list entries: --------------------- */
	    for (ii = 0;  (ii < chrPerGenAmount) && genOK;  ii++)
	    { /* process returned result list entries */
	       genOK = getNextResults(simu_erg, &indivIdx, &childIdx);
	       if (genOK)
	       { /* result successfully read */
		  if (chrList[indivIdx].offspringList[childIdx] == NULL) 
		  { /* this chromosome does not exist */
		     sprintf(buf3, "of child (%d/%d)", indivIdx, childIdx);
		     err_state |= fehler(EVO_PAR_SIM_EVO, 24, NO_INFO, NO_INFO, buf3, NO_CHAIN);
		     genOK      = FALSE;
		  } /* this chromosome does not exist */
		  else
		     if (chrList[indivIdx].offspringList[childIdx]->u_el.u_xel.u_hdr.chain_status & SIMULATED)
		     { /* the existing chromosome is already assessed! */
			err_state |= fehler(EVO_PAR_SIM_EVO, 25, indivIdx, childIdx, "", 
					    RESULTS_ALREADY_RECEIVED);
			genOK      = FALSE;
		     } /* the existing chromosome is already assessed! */
	       } /* result successfully read */
	       if (genOK)
	       { /* process this result entry */
		  fitness = ak_bewerten (CALC_FITNESS);
		  for (ij = 0;  ij < akt_anz_krit;  ij++) 
		     chrList[indivIdx].offspringList[childIdx]->u_el.u_xel.u_hdr.header_par_ptr->sim_erg_werte[ij] = 
			simu_erg[ij];
		  chrList[indivIdx].offspringList[childIdx]->u_el.u_xel.u_hdr.fitness_note  = fitness;
		  chrList[indivIdx].offspringList[childIdx]->u_el.u_xel.u_hdr.chain_status |= SIMULATED;
	       } /* process this result entry */
	    } /* process returned result list entries */

	    /* -------- determine the best offspring and apply acceptance rule: ------- */
	    for (akt_indiv = 0;  (akt_indiv <= max_pop_index) && genOK;  akt_indiv++)
	    { /* process the offspring of all inividuals */
	       if (chrList[akt_indiv].offspringAmount > 0)
	       { /* there is at least one offspring of this individual */
		  /* ------ update fitness values of worst and best deme members: ----- */
		  if (pop_feld[akt_indiv].note >= chrList[akt_indiv].bestDemeFitness)
                  { /* parent is best of deme */
		     chrList[akt_indiv].bestDemeFitness = pop_feld[akt_indiv].note;
		     parentIsNotDemeBest = FALSE;
		  } /* parent is best of deme */
		  else 
		     parentIsNotDemeBest = TRUE;
		  if (pop_feld[akt_indiv].note < chrList[akt_indiv].worstDemeFitness)
		     chrList[akt_indiv].worstDemeFitness = pop_feld[akt_indiv].note;

 		  /* ------------------ determine the best offspring: ----------------- */
		  bestChildIdx  = 0;
		  bestChildFitn = chrList[akt_indiv].offspringList[bestChildIdx]->u_el.u_xel.u_hdr.fitness_note;
		  for (childIdx = 1;  (childIdx < chrList[akt_indiv].offspringAmount) && genOK;  childIdx++)
		     if (bestChildFitn < chrList[akt_indiv].offspringList[childIdx]->u_el.u_xel.u_hdr.fitness_note) 
		     { /* store data of better offspring and delete substituted child */
			genOK = delete_chain(&(chrList[akt_indiv].offspringList[bestChildIdx])) == GLEAM_OK;
			bestChildFitn = chrList[akt_indiv].offspringList[childIdx]->u_el.u_xel.u_hdr.fitness_note;
			bestChildIdx  = childIdx;
		     } /* store data of better offspring and delete substituted child */
		     else
			genOK = delete_chain(&(chrList[akt_indiv].offspringList[childIdx])) == GLEAM_OK;

		  /* ---------------------- apply acceptance rule: -------------------- */
                  switch (act_survival_rule) 
		  {
		     case ALWAYS: 
			replace = TRUE;
			break;

		     case ALWAYS_ES: 
			replace = parentIsNotDemeBest || (bestChildFitn > chrList[akt_indiv].bestDemeFitness);
			break;

		     case LOCAL_LEAST: 
			replace = bestChildFitn > chrList[akt_indiv].worstDemeFitness;
			break;

		     case LOCAL_LEAST_ES: 
			replace = (bestChildFitn > chrList[akt_indiv].worstDemeFitness) && 
			          (parentIsNotDemeBest || (bestChildFitn > chrList[akt_indiv].bestDemeFitness));
			break;

		     case BETTER_PARENT: 
			replace = bestChildFitn > pop_feld[akt_indiv].note;
			break;
		  } /* switch */
#ifdef ACCEPT_TST
		  printf ("evo_1gen: Bester Nachkommen mit Note %g von Elter %d mit Note %g wird akzeptiert: %d\n"
			  "          Deme-Noten (Min, Max): %g %g  besser als Schlechtestes=%d besser als Bestes=%d "
			  "Elter nicht DemeBestes=%d\n", bestChildFitn, akt_indiv, pop_feld[akt_indiv].note, 
			  replace, chrList[akt_indiv].worstDemeFitness, chrList[akt_indiv].bestDemeFitness,
			  bestChildFitn > chrList[akt_indiv].worstDemeFitness,
			  bestChildFitn > chrList[akt_indiv].bestDemeFitness, parentIsNotDemeBest);
#endif /* ACCEPT_TST*/
		  if (replace)
		  { /* accepted child replaces parent */
		     genOK &= delete_chain(&(pop_feld[akt_indiv].chain_ptr)) == GLEAM_OK;
		     pop_feld[akt_indiv].chain_ptr  = chrList[akt_indiv].offspringList[bestChildIdx];
		     pop_feld[akt_indiv].note       = bestChildFitn;
		     pop_feld[akt_indiv].new_chain  = TRUE;       /* mutated chromosome */
		     pop_feld[akt_indiv].chain_ptr->u_el.u_xel.u_hdr.ref_counter = 1;
		     chrList[akt_indiv].offspringList[bestChildIdx] = NULL; 
		     berechne_guete (&(pop_feld[akt_indiv].guete), bestChildFitn);
		     loe_deme_ham_werte (akt_indiv);    /* they ar not valid any longer */
		     if (bestChildFitn > chrList[akt_indiv].bestDemeFitness)
			accept_improved = TRUE;                  /* child improves deme */
		     if (bestChildFitn > gen_note)
		     { /* child is better than best of generation and population */
			gen_note = bestChildFitn;
			gen_len  = pop_feld[akt_indiv].chain_ptr->u_el.u_xel.u_hdr.chain_length;
		     } /* child is better than best of generation and population */
		     accepted = TRUE;   /* at least 1 accepted child in this generation */
		  } /* accepted child replaces parent */
		  else
		  { /* delete rejected child */
		     genOK &= delete_chain(&(chrList[akt_indiv].offspringList[bestChildIdx])) == GLEAM_OK;
		     if (gen_note < pop_feld[akt_indiv].note) 
		     { /* parent is the best of this generation so far */
			gen_note = pop_feld[akt_indiv].note;
			gen_len  = pop_feld[akt_indiv].chain_ptr->u_el.u_xel.u_hdr.chain_length;
		     } /* parent is the best of this generation so far */
		  } /* delete rejected child */
	       } /* there is at least one offspring of this individual */
	    } /* process the offspring of all inividuals */
 	 } /* returned result list contains all required result entries */
	 else {
	    err_state |= fehler(EVO_PAR_SIM_EVO, 28, chrPerGenAmount, resultAmount, 
				BAD_RES_AMOUNT_TXT, BAD_VALUE_ERR);
	    genOK      = FALSE;
	 }
      } /* result list header successfully read */
      else
	 genOK = FALSE;                                          /* no results received */
   } /* process received result list */
   if (!sim_up)
   { /* Evo-Abbruch wegen Crashs des ext. Simulators */
      meld_evo_crash (EVO_WUERG_SIMU_MELD, EVO_EVO, NO_INFO);
      return (FALSE);
   } /* Evo-Abbruch wegen Crashs des ext. Simulators */

#if !defined(GLEAM_USR) /* command line version */
   externalTermRequest();    /* terminates actual job on appearance of termination file */
#endif /* command line version */

#if defined(INTRP_TST) || defined(PERF_TST)
   printf("evo_1gen: Phase=%d --- Ende ---  taste=%d genOK=%d\n",
	  jobInterruptPhase, taste, genOK);  fflush(stdout);
#endif /* INTRP_TST or PERF_TST */

   if (bestJobNote < gen_note)
      bestJobNote = gen_note;               /* Erfassung der besten Fitness des OptJobs */
   if (!taste && genOK)
   { /* Generation normal beendet */
      if (accepted)
      { /* Es wurde mindestens ein Kind der Generation akzeptiert */
         no_accept_gen_counter = 0;
         if (accept_improved)
            no_deme_impr_gen_counter = 0;
         else
            no_deme_impr_gen_counter++;
      } /* Es wurde mindestens ein Kind der Generation akzeptiert */
      else
      { /* Es wurde kein Kind der Generation akzeptiert */
         no_accept_gen_counter++;
         no_deme_impr_gen_counter++;
      } /* Es wurde kein Kind der Generation akzeptiert */
   } /* Generation normal beendet */
   return (!taste && genOK);
} /* evo_1gen */



/* =============================== pruef_und_speichere ================================ */
BOOL pruef_und_speichere (CHAIN_ELEM_PTR_TYPE *c_ptr, 
                          INT                  mut_index)
{
 
   /* pruef_und_speichere */
   if (repair_1_chain (c_ptr, &err_state))            /* deletes implausible chromosome */
   { /* Plausibles Chromosom */
      indiv_ctr++;
      chrList[akt_indiv].offspringList[anz_children] = *c_ptr;
      writeChrToList(*c_ptr, akt_indiv, anz_children++);   /* sets "err_state" if error */
   } /* Plausibles Chromosom */
   return (err_state == GLEAM_OK);
} /* pruef_und_speichere */




/* ===================================================================================== */
/* ================================= Baustelle: ======================================== */

#ifdef MIT_LSV
/* ================================= do_lsv ================================= */
BOOL do_lsv (BOOL  lamarck,
             INT   znr)
{
   STR_TYPE  buf1, buf2;
   DOUBLE    alte_note,                /* Note der Original-AK                */
             start_pop_note;           /* Beste Note der Startpopulation      */
   INT       chrLen;                   /* Laenge des Chromosoms               */
   BOOL      lsv_ok;                   /* LSV lief fehlerfrei                 */
   
   /* do_lsv */
   alte_note = pop_feld[0].note;
   if (alte_note < zielnote) 
   { /* Zielnote ist noch nicht erreicht: LSV anwenden */
  #ifdef MIT_GORBA_LSVS   /* Gorba-HyGLEAM: Fall A */
      vorGLSVstart = TRUE;
      if (do_appl_sim (simu_erg, pop_feld[0].chain_ptr, CALC_FITNESS, &err_state)) 
       { /* AK wurde erfolgreich simuliert */
	  if (lsv_strat < LSV_RAS)
	     aktPlanDataToSave ();
	  else
	     aktAJobPlanDataToSave ();
	  resetBelMat ();                  /* vor "do_lsv_start" notwendig! */
	  vorGLSVstart = FALSE;            /* vor "do_lsv_start" notwendig! */
	  neue_lsv_note = do_lsv_start (&(pop_feld[0].chain_ptr), TRUE, &err_state);
       } /* AK wurde erfolgreich simuliert */
      else { /* AK nicht simulierbar */
	 resetBelMat ();
	 vorGLSVstart  = FALSE;
	 neue_lsv_note = KEINE_NOTE;
      } /* AK nicht simulierbar */
  #else /* kein MIT_GORBA_LSVS */
      neue_lsv_note = do_lsv_start (&(pop_feld[0].chain_ptr), lamarck, &err_state);
  #endif /* kein MIT_GORBA_LSVS */
   } /* Zielnote ist noch nicht erreicht: LSV anwenden */
   else
      neue_lsv_note = alte_note;

   /* ----------- Nachbehandlung der verbesserten AK im pop_feld: ----------- */
   lsv_ok = neue_lsv_note != KEINE_NOTE;
   chrLen = pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.chain_length;
   if (pop_feld[0].new_chain)
   { /* neugenerierte AK */               /* AK-Note und AK-Aktionen muessen  */
      if (lamarck && lsv_ok)              /* uebereinstimmen. Daher LSV-Note  */
      { /* Durch LSV veraenderte AK */    /* nur in AK, wenn lamarck==TRUE    */
         pop_feld[0].note = neue_lsv_note;
         berechne_guete (&(pop_feld[0].guete), neue_lsv_note);
         if (znr > 0)
            if (neue_lsv_note > alte_note)      /* Ausgaben nur, wenn 1 Indiv */
               write_buf (znr, AK_OPT_STORED);  /* lokal optimiert wird.      */
            else
               write_buf (znr, ORG_AK_STORED);
      } /* Durch LSV veraenderte AK */
      else
      { /* unveraenderte AK: Original-Bewertung wieder herstellen */
         err_state |= simulator (&alte_note, CALC_FITNESS, 
                                 pop_feld[0].chain_ptr);
         if (znr > 0)      /* Ausgaben nur, wenn 1 Indiv lokal optimiert wird */
            write_buf (znr, ORG_AK_STORED);
      } /* unveraenderte AK: Original-Bewertung wieder herstellen */
   } /* neugenerierte AK */
   else
   { /* AK stammt aus AK-Mem (Alt-AK) */
      if (lsv_ok && (neue_lsv_note > alte_note) && lamarck)
         behandle_neue_note (pop_feld[0].chain_ptr, znr, neue_lsv_note);  /* verbesserte Alt-AK updaten */
      else
      { /* Alt-AK bleibt unveraendert */
         pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.fitness_note = alte_note;
         if (znr > 0)      /* Ausgaben nur, wenn 1 Indiv lokal optimiert wird */
            write_buf (znr, AK_UNVERAENDERT);
      } /* Alt-AK bleibt unveraendert */
      store_in_evo_erg (pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.lfd_nr,
                        pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.guete,
                        pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.fitness_note, AK);
      pop_feld[0].chain_ptr->u_el.u_xel.u_hdr.ref_counter = 1;   /* AK ist be-*/
      pop_feld[0].new_chain = FALSE;          /* reits im Mem. Nicht nochmal! */
      pop_feld[0].guete     = KEINE_GUETE;                   /* AK im         */
      pop_feld[0].note      = KEINE_NOTE;                    /* "pop_feld"    */
      pop_feld[0].chain_ptr = NULL;                          /* loeschen      */
   } /* AK stammt aus AK-Mem (Alt-AK) */
   if ((lsv_ok) && (neue_lsv_note > gen_note)) {
      gen_note = neue_lsv_note;
      gen_len  = chrLen;
   }
   return (lsv_ok);
} /* do_lsv */
#endif /* MIT_LSV */


