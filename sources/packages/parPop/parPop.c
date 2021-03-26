/*****************************************************************************************
HyGLEAM/A                                                                           V1.0.0

                                       H y G L E A M
            Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                    Paralellisation based on distributed subpopulations
 
                                       File: parPop.c                 Linux, gcc/g++ 5.4.0
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------

There are two command line controlled components based on GLEAM, the "initializer" and the 
"islands":

initializer:
============
The tasks of the initializer are
- Checking the availability and correctness of all initialization files, which describe an
  optimization job and which will later configure the evolutionary engine of the islands
- Checking the data comming from the command line and the TSK-file describing the optimiza-
  tion job to be performed including corrections as far as possible.
- Performing the chromosome selection for all initialization methods different from "new",
  i.e. best, mix, bestNew, see documentation of module evo_gsw.c of package "evo". The 
  chromosomes are written to a chromosome file for later distribution among the islands.

islands:
========
The task of an island is to compute an epoch of a partial population based on the island 
population model. In detail:

1st epoch: 
----------
  a. Read in the initial chromosome file containing none or some start chromosomes.
  b. Create the start population based on the obtained start chromosomes and newly generated 
     ones. This is controlled by the command line parameters.
  c. Perform a number of generations until the limits of the  epoch are raeched.
  d. Write all chromosomes of the present population to a specific population chromosome 
     file. 
  e. Terminate.

n-th epoch:
-----------
  a. Read in the entire population from the population chromosome file. This file was pre-
     viously written by the island as a result of the preceeding epoch and may be changed 
     by the framework by substituting some individuals by migrants. This is controlled by 
     the command line parameters.
  c. Perform a number of generations until the limits of the epoch are reached.
  d. Write all chromosomes of the present population to a specific population chromosome 
     file. 
  e. Terminate.

last epoch:
-----------
The last epoch is an epoch run, which is either terminated by itself due to the attainment 
of epoch limits or it is terminated by the creation of the island termination file in the 
workspace of the running island (see value of EVO_ABBRUCH_FSPEC in schalter.h).
If the "-r<amount>"-option was used, the <amount> best result chromosomes are written to a 
chromosome file. This amount must not exceed the half of the subpopulation size. The file
name is the name of the experiment file followed by the contents of ISLAND_TXT and 
ISLAND_RES_TXT. If the option is not contained in the command line, a population chromosome 
file is written as usual.

The name of the population chromosome file is defined by EVO_SCRATCH_FILE, see module 
evo_gsw.c of package "evo".


Suitable population and deme sizes:
-----------------------------------
The population of an island is either a panmictic population or neighbourhood based one. 
In fact, in our case the panmictic one is a neighbourhood population with a deme size of 
mu - 1. As the deme size must be even and smaller than mu, the minimal subpopulation size
mu is 3 and the smallest deme is of size 2. The maximum deme size of a subpopulation can 
be calculated as follows:
  if ((subPopSize % 2) == 0)            // even subpopulation size ?
      maxDemeSize = subPopSize - 2;
   else
      maxDemeSize = subPopSize - 1;
These sizes are checked and corrected if necessary by the islands. 
Remark: Checks done by the initializer are based on the complete population and thus, not 
        really relevant for the islands.
The maximum sizes can be found in the schalter.h-File: MAX_POPEL_SIZE and MAX_ANZ_NACHBARN.


Interaction between initializer and islands:
--------------------------------------------
1. The initializer reads in all initialization files listed in the experiment file obtained 
   from the command line and the error text file(s). It may also read a specified chromosome 
   file, which may be used to seed the initial population. According to the command line 
   options a suitable set of initial chromosomes is selected and written to a file denoted 
   by INITIAL_CHR_FNAME. This file may be empty. The framework distributes these chromosomes 
   randomly among the islands avoiding multiple usage by writing them to a standard chromo-
   some file to the work space of each island. Such a file may be empty, if necessary. The 
   name of this file must be contained in the standard experiment file (EXP-file) of the 
   present application. 
   With the creation of the initial chromosome file the task of the initializer to pre-
   pare the start of the islands is done and it terminates returning 0. In case of an error 
   it returns 1 instead.
   After successful execution, the framework starts the islands with the "-Ibest" option, 
   if a none empty initial chromosome file was written. Otherwise, the "-Ineu" option 
   should be used.

2. Every island is started with its command line determing its population and deme sizes 
   among others. 
   The start of the first epoch must be done with "-Ibest" option or "-Ineu" option, if 
   no seeding of the start population is done at all (i.e. e.g. the initializer was also 
   called with "-Ineu"). In the latter case the chromosme file is either empty or ommited 
   at all. Every island reads in its chromosome file (if one is specified by the EXP-File) 
   and stores the chromosomes into the internal chromosome memory of GLEAM. The initial
    subpopulation is created as specified by the command line.
   The start of any subsequent epoch must be done with "-Ifile" option. 

3. Islands terminate themselves when they reach one of the given (epoch) termination 
   criteria: target fitness or limits for time, generations, evaluations or the two stag-
   nation indicators. The entire population is saved to the population chromosome file.
   Islands can also be stopen by writing a file denoted by EVO_ABBRUCH_FSPEC into their 
   work space.

4. The initializer and all islands create a logfile each. The initializer uses the logfile 
   name (and path) from its command line or, if absent, the EXP-file name (and path) is 
   used. The islands use the same name as prefix and add the string "_island-<island-ID>" 
   to the file name. These logfiles shall be managed by the framework and/or using an 
   appropriate tool. 
   The minimal content of such a logfile ist a line starting with "#t " containing the time 
   elapsed and the number of function evaluations used.


The following is performed by the framework:

1. Read in the experiment file and store it together with all files specified there and 
   the error text file(s) into the work space of the initializer. Then start the initiali-
   zer. If it completes successfully (i.e. returns 0), all the file are also written to 
   the workspaces of the islands, which can now be started with the command line options 
   of the first epoch.
   Remark: According to the application the simulation is done either by the external 
           simulation services (ESS) or by some internal simulators. In the latter case 
           additional initialization file as specified by the EXP-file must be contained 
           in the work space of each island.

2. After completion of an epoch the complete subpopulation is written to the population 
   chromosome file, which is processed by the frame work. The exchange of migrants is done 
   by replacing appropriate chromosomes of the receiving island during switching to the 
   next epoche. The resulting subpopulation is written to the population chromosome file,
   which is read by the island started afterwords for the n-th epoch.

3. The framework decides at least on the information received by epoch changes if the 
   optimization process has to be stopped.
   
4. After all islands have treminated the frame work does the postprocessing:
   - Extraction of the result chromosome(s) and their storage.
   - Collection if statistical relevant data of the run and its preparation.


Chromosomefiles written by the initializer and read or written by the islands are always 
text files. The chromosome file read by the initializer may be either a text or binary
chromosome file. This must be specified correctly by the TSK-file! By default, a binary 
file is expected.


List of exported functions:
---------------------------

BOOL initializer (void);
        If existing chromosomes are to be used for the initial population, the function 
        initializes the start population with them as specified by the TSK-fiel and/or the 
        command line options and extracts the selected chromosomes to a file denoted by 
        INITIAL_CHR_FNAME for further processing by the frame work. In no suited initial 
        chromosomes are available or no seeding of the start population is intended, an 
        empty chromosome file is written.
        The function returns TRUE on success and FALSE in case of an arror.
        Offen ???:
        The data of the optimization job are written to the logfile for documentation 
        purposes.

BOOL initParPop (void);
        Module initialization.
        In case of failure appropriate error messages are generated and FALSE is returned. 
        TRUE otherwise.


Package-local functions:
------------------------


------------------------------------------------------------------------------------------
List of changes:
------------------------------------------------------------------------------------------
Status: impl                 Author: W.Jakob / P.Ostheimer                Date: 06/20/2018
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "aufg.h"
#include "chio.h"
#include "evo.h"

#include "parPop.h"
#include "parPopInt.h"

#include GLOBAL_TEXTS
#include PAR_POP_TEXTS

#undef WITH_DBG



/* ================================ Exported Variables: =============================== */



/* ============================= Package-global Variables: ============================ */



/* ================================== Local Constants: ================================ */
#define INITIAL_CHR_FNAME    "initialChromosomeSet.aks"



/* ============================= Local Type Declarations: ============================= */



/* ================================== Local Variables: ================================ */
static STRING_TYPE  logBuf;   /* common buffer for log file entries or auxiliary buffer */



/* ================================== Local Functions: ================================ */




/* ==================================== initializer =================================== */
BOOL initializer (void)
{
   STR_TYPE     buf;               /* auxiliary buffer                                  */
   FILE        *dataFile;          /* chromosome file or cmd line list file             */
   DOUBLE       bestFitness;       /* fitness of the best initial chromosome            */
   INT          popSize,           /* size of the total population                      */
                initStratParam,    /* optional parameter for creating the inititial pop.*/
                markedChrs,        /* number of chromosomes marked for writing to file  */
                ii;                /* auxiliary variable                                */
   BOOL         result,            /* FALSE, if something went wrong and TRUE otherwise */
                writeEmptyFile,    /* TRUE if an empty chromosome file must be written  */
                best_mix_bestNeu,  /* TRUE if init. strategy is BEST or MIX or BEST_NEU */
                chrSaveOk;         /* TRUE if selected initial chromosomes were saved   */

   /* initializer */
   result         = TRUE;
   bestFitness    = 0.0;
   popSize        = evo_ws[first_evo_item].popel_size;
   init_strat     = evo_ws[first_evo_item].init_strat;
   initStratParam = evo_ws[first_evo_item].strat_par;

   /* ------ process init strategy for start pop. and select initial chromosomes: ----- */
   writeEmptyFile   = FALSE;
   best_mix_bestNeu = (init_strat == BEST) || (init_strat == MIX) || (init_strat == BEST_NEU);
   if (best_mix_bestNeu)
   { /* select chromsomes for seeding the start populations by the islands */
      strcpy(buf, INIT_PROC_BEST_TXT);
      prepareForEvo = FALSE;                         /* only select initial chromosomes */
      if (init_ep(&bestFitness, popSize, initStratParam))
      { /* selection processed successful */
#ifdef WITH_DBG
	 printf("initializer: bestFitness=%g selectedChrAmount=%d Details:\n",
		bestFitness, selectedChrAmount);
#endif /* WITH_DBG */
	 if (selectedChrAmount)
	 { /* set of initial chromosomes not empty */
	    chrSaveOk  = FALSE;
	    markedChrs = 0;
	    for (ii = 0;  ii < selectedChrAmount;  ii++)
	    { /* mark found chromosomes for save */
#ifdef WITH_DBG
	       printf("             %2d.AK: %2d/%2d", ii, 
		      pop_feld[ii].guete, pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.lfd_nr);
#endif /* WITH_DBG */
	       if (markiere_ak(pop_feld[ii].guete, 
			       pop_feld[ii].chain_ptr->u_el.u_xel.u_hdr.lfd_nr) == GLEAM_OK)
		  markedChrs++;
#ifdef WITH_DBG
	       printf(" markedChrs=%2d\n", markedChrs);
#endif /* WITH_DBG */
	    } /* mark found chromosomes for save */
	    if (markedChrs)
	    { /* at least one chromosome was marked */
	       if (open_ch_file(INITIAL_CHR_FNAME, 1, CH_WRITE, SELECTIVE_SAVE, FILE_TXT_IO))
               { /* file open */
		  chrSaveOk = save_marked_aks(FALSE);
		  close_ch_file();
		  if (detailed_log)
		     logTextBuffer(OHNE_TERM_AUSG);
		  if (chrSaveOk)
		  { /* log summary of successful chromosome output */
		     sprintf(logBuf, INIT_CHR_RES_TXT, selectedChrAmount, markedChrs, 
			     INITIAL_CHR_FNAME);
		     write_to_log(logBuf, CLOSE_LOG);
		  } /* log summary of successful chromosome output */
		  else
		     fatal(PAR_POP, 1, NO_INFO, NO_INFO, INITIAL_CHR_FNAME, WRITE_ERR);
	       } /* file open */
	       result = chrSaveOk;
	    } /* at least one chromosome was marked */
	    else
	       result = (fatal(PAR_POP, 2, NO_INFO, NO_INFO, "", NO_CHROMOSOME_ACCESS) == GLEAM_OK);
	 } /* set of initial chromosomes not empty */
	 else
	    writeEmptyFile = TRUE;                       /* no suited chromosomes found */
      } /* selection processed successful */
      else 
	 result = FALSE;                    /* init_ep() has already reported the error */
   } /* select chromsomes for seeding the start populations by the islands */
   if ((init_strat == NEU) || writeEmptyFile)
   { /* create an empty chromosome file */
      strcpy(buf, INIT_PROC_NEW_TXT);
      if (open_file(&dataFile, INITIAL_CHR_FNAME, "wt"))
	 fclose(dataFile);
      else
	 result = FALSE;                  /* open_file() has already reported the error */
   } /* create an empty chromosome file */
   else
      if (!best_mix_bestNeu)     /* not yet implemented or inappropriate init. strategy */
	 result = (fatal(PAR_POP, 3, init_strat, NO_INFO, "", BAD_STRATEGY) == GLEAM_OK);

   return (result);
} /* initializer */



/* =================================== performIsland ================================== */
void performIsland (INT  nodeNo)
{
   /* performIsland */
} /* performIsland */



/* ==================================== initParPop ==================================== */
BOOL initParPop (void)
{
   BOOL  result;                    /* result of package and pipe initialization        */

   /* initParPop */
   result = TRUE;

   /* some package initialization ... 
      the error message system can and should be used if FALSE is returned.    */

   return(result);
} /* initParPop */
