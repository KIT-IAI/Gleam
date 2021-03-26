/*****************************************************************************************
HyGLEAM/A                                                                           V1.1.0

                                       H y G L E A M
            Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                Mainprogram of the Commandline Version (Engine), Version 2
 
                                  File: hy_gleam_engine.c             Linux, gcc/g++ 4.8.4
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
------------------------------------------------------------------------------------------

Mainprogram of the commandline version of GLEAM and HyGLEAM for performing one optimization 
job. According to the switch MIT_LSV (see file s"schalter.h") either GLEAM (switch not set 
and makefile) or HyGLEAM (switch set and hyGleamEngine2.mak) is generated.

Call:   
  hyGleamEngine2 [OPTION]... EXP_FILE [LOG_FILE]
    -A<text>      : acceptance rule for offspring: always, localLeast, betterParent, 
                    always-ES, localLeast-ES
    -B(+|-)       : +: best-improvement,  -: all-improvement
    -C(+|-)       : Mit/Ohne Erzeugung eines case-Files bei einer Ergebnissimulation. 
                    Nur fuer MATPOWER-Anwendungen!
    -D<demeSize>  : neighbourhood size, must be even and smaller than popSize. (4 - 32)
    -E<number>    : limit for function evaluations (0 or not specified: termination condition 
                    switched off)
    -F<fitness>   : fitness limit (termination condition) (0.0 - 100000.0)
    -G<gen>       : generation limit (termination condition)
    -h -H --help  : online help (this text)
    -I<text>      : initialization strategy: new, best, mix, bestNew, lhcNew, lhcBest
    -i<value>     : initialization parameter: mix:minimal fitness,  bestNew:amount of best,  
                    lhcBest:amount of best
    -L(+|-)       : Lamarcksche evolution: +:on,  -:off (Baldwin evolution)
    -l<frac>      : learning rate: proportion of old LHC distribution used for the new one 
                    in % (0.0 - 100.0)
    -M<value>     : maximum iterations per LHC run (termination threshold)
    -N<text>      : domain name of the external simulation services (ESS)
    -n<islandNo>  : 0: initializer, >0: island number, used by PAR_POP program variants
    -O<text>      : optimization procedure: gleam, sma, asma, amma, lhc
    -P<popSize>   : population size (4 - 20000)
    -p(+|-)       : logging: +:detailed,  -:minimal
    -R<value>     : ranking parameter, from minimal to maximal selective pressure (1.0 - 2.0)
    -r<number>    : number of results. Default: 1,  max: popSize/2,  0: no simulation or 
                    save of results possible.
    -S<index>     : Searcher: LHC index for LHC, SMA, ASMA or GLEAM with LHC-initialization 
                    (lhcNew oder lhcBest)
    -T<value>     : time limit: maximal job run time in seconds (termination condition)
    -t<value>     : threshold for LHCs (optional 2nd LHC termination threshold)
    -W<nr>:<val>  : New weight of criterion <nr> expressed as its maximum fitness <val>.
    -X<val>:<val> : undocumented switch for experimental purposes.

The above list of options is based on some parameters from the "schalter.h" file, which 
provide limits for the following options: -D, -F, and -P.

If one or more -W options are given, the weights of the criteria are adjusted and checked 
to sum up to 100% before processing the optimization job.

The program returns 0, if it was either called with the help option, or if no experiment 
file (EXP_FILE, .exp) was provided or if the specified optimization job was performed 
successfully, i.e. without any errors. Otherwise a value of 1 is returned.

Error messages can be found in the logfile (.log). If it is ommitted, the name of the expe-
riment file is used instead. Logfiles can be written in a short or in a detailed form, 
whichcan be controlled by the -p option. Error messages will always be written to the 
logfile.


The program consists of six major parts:
1. First analysis of the command line, i.e. search for file names and the options -h and -p.
   if no file parameter is given or the h/H-option is found, the help text is displayed and 
   the program terminates. As the p-option controls logging it is searched for in such an 
   early phase. Finally, the program initialization (gen_init()) is done, which must be 
   successful to avoid program termination.

2. Reading in the experiment file and all files specified therein. All errors are treated 
   as fatal errors and lead to program termination.

3. First checks of the task file parameter with respect to the selected optimization pro-
   cedure.  All errors are treated as fatal errors and lead to program termination.

4. Reading all program options not processed so far. Errors may be treated as fatal ones
   depending on the program variant (GLEAM or HyGLEAM), then application, and the selected 
   optimization procedure. As before, fatal errors lead to program termination. Simple 
   errors are reported and the error status is reset afterwards.

5. Based on the actual optimization procedure all necessary program parameters are checked 
   for consistency and semantic correctness as far as it is possible. Again, all errors are 
   treated as fatal ones causing program termination.

6. Finally, the optimization job is prepared, executed, and its result is processed. 


The experiment file contains additional file specifications that describe the experiment, 
among them the task file (.tsk). An optimization job can be controlled by parameters given 
in a task file and/or by program options. Options always override task file parameters. 
Options or parameters, which are not required for the selected optimization procedure, are 
ignored. Missing but required job specifications result in an error and the job is not 
performed.
 
A further file, which is optional and may be provided by the experiment file is the chromo-
some file (.mem) for the initialization of the chromosome memory. Accordimg to the appli-
cation scenario some or all chromosomes may be unevaluated or the original evaluation does 
not fit to the actual context. In these cases a simulation and evaluation calculating a 
new fitness value must be performed. This is done by the restore_chs() procedure for chro-
mosomes with an unset SIMULATED-flag. To avoid possible unnecessary result processing the 
variable "record_evo_best" is set to FALSE while reading the chromosome file.

------------------------------------------------------------------------------------------
List of changes:
 1. CLV_MAIN_TEXTS ersetzt ENG2_MAIN_TEXTS.                                    WJ 12/22/17
 2. New "-W"-switch added.                                                     WJ 01/19/18
 3. New "-N"-switch added for ESS version. The start of the external simulator 
    and the reading of the optional chromosome file are shifted after the 
    processing of the program switsches, so that the switches can affect the 
    simulator start ("-N"-option in particular). The simulator may be needed 
    for the assessment of possible unevaluated chromosomes read from file.     WJ 01/30/18
 4. Handling of logfile name corrected.                                        WJ 02/05/18
 5. New "-X"-switch added.                                                     WJ 02/09/18
 6. VISU_MODE: Call of doVisuJob() from package "webIO" added, which is 
    called instead of do_evo_jobs().                                           WJ 03/27/18
 7. New variable "initialChrFileMode", which controls the file format of an 
    initial chromosome file.                                                   WJ 04/18/18
 8. Integration of the new package visuIO, the software of which was formerly 
    part of the webIO package.                                                 WJ 05/28/18
 9. Extensions for the PAR_POP_VERS variants.                                  WJ 06/22/18
10. Use of new timer functions startTimerMS() and calcElapsedTimeMS(). The 
    data of the best individual is only stored when statistical data is 
    collected generation by generation. This enables improvements to be 
    recorded over time.                                                        WJ 07/18/18
11. Deletion of "evo_fin_fspec". EVO_ABBRUCH_FSPEC is used instead directly.   WJ 08/21/18
12. New "-C"-switch for MATPOWER applications added.                           WJ 10/29/18
13. EXT_MATLAB_SIMU: If chromosomes are to be saved to a file, their SIMULATED 
    state is reset.                                                            WJ 02/06/19
14. Am 21.2.19 wurde die Verarbeitung von Optimierungsergebnissen undokumen-
    tiert veraendert. Vermutlich von Andi im Kontext der MatPower/Matlab-An-
    bindung fuer die SG-Resilience-Anwendungen. Diese Aenderungen sind jetzt nur 
    bei gesetztem EXT_MATLAB_SIMU aktiv. Ansonsten gilt wieder die alte SW.    WJ 10/31/19
15. "nodeNumber" is declared here, if switch PAR_POP_VERS is set and switch
    EXT_SIMU_SERV is unset, as in this case the parallel island-based version 
    of gleam does not use parallelized ESS. Thus, the variable "nodeNumber" is
    used in the main program only locally.                                     WJ 03/18/20
16. New big string buffer "buffer" and other schanges to avoid buffer overflow 
    warnings. A chromosome file (MEM file) that may need to be loaded is 
    searched for first in the present working directory and if it does not 
    exist there also in the directory given by the environ ment variable, the 
    name of which is contained in GLEAM_ROOT_ENV string constant.              WJ 08/08/20
17. Deletion of experimental delay stuff that is not needed any more.          WJ 23.10.20
------------------------------------------------------------------------------------------
Status: o.k.                          Author: W.Jakob                     Date: 23/10/2020
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <float.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "chio.h"
#include "hmod.h"
#include "bew.h"
#include "simu.h"
#include "aufg.h"
#include "appl.h"
#include "evo.h"
#include "men.h"
#if defined(EXT_SIMU_SERV)
  #include "webIO.h"
#endif /* EXT_SIMU_SERV */
#if defined(VISU_MODE)
  #include "visuIO.h"
#endif /* VISU_MODE */
#ifdef PAR_POP_VERS
  #include "parPop.h"
#endif /* PAR_POP_VERS */

#include AUFG_TEXTS
#include EVO_TEXTS
#include MAIN_TEXTS
#include CLV_MAIN_TEXTS
#include GLOBAL_TEXTS

#define SCHLUSS_MLDG



/* ================================== Local Constants: ================================ */
#define NIX_WERT         -1
#define OK_RET_CODE       0
#define ERR_RET_CODE      1



/* ================================= Local Variables: ================================= */
static GSTATE  errLevel;        /* error levels: GLEAM_OK, GLEAM_ERROR, and GLEAM_FATAL */



/* ================================= Lokale Routinen: ================================= */
static BOOL readDVal  (DOUBLE     *val,

                       const char *buf);

static BOOL readIVal  (INT        *val,
                       const char *buf);

static void showHelp  (void);




/* ===================================== readDVal ===================================== */
static BOOL readDVal (DOUBLE     *val,
                      const char *buf)
/*--------------------------------------------------------------------------------------*/
/*   The contents of the buffer "buf" is converted to double. The conversion is checked */
/*   for under/overflow, conversion errors, no digits and remaining characters after    */
/*   the number. In case of one of these errors, FALSE is returned and "val" remains    */
/*   unchanged. Otherwise, the function returns TRUE and the converted number is re-    */
/*   turned in "val".                                                                   */
/*--------------------------------------------------------------------------------------*/
{
   DOUBLE  dVal;
   BOOL    result;
   char   *endPtr;

   /* readDVal */
   errno = 0;                              /* to distinguish success/failure after call */
   dVal = strtod(buf, &endPtr);
   /*    under/overflow  OR      bad conversion         OR    no digits    OR chars after number */
   if ((errno == ERANGE) || (errno != 0 && dVal == 0.0) || (endPtr == buf) || (*endPtr != '\0')) 
      result = FALSE;                                 /* something of the above occured */
   else { /* successful conversion and nothing after digits */
      result = TRUE;
      *val   = dVal;
   } /* successful conversion and nothing after digits */
   return (result);
} /* readDVal */



/* ===================================== readIVal ===================================== */
static BOOL readIVal (INT        *val,
                      const char *buf)
/*--------------------------------------------------------------------------------------*/
/*   The contents of the buffer "buf" is converted to double by "readDVal()". If        */
/*   successful, it is checked whether the obtained double value is integer, i.e., does  /
/*   not contain decimal places. In this case, the truncated value is returned in "val" */
/*   and the function returns TRUE. Otherwise, an error message is issued, FALSE is     */
/*   returned and "val" remains unchanged.                                              */
/*--------------------------------------------------------------------------------------*/
{
   STR_TYPE  buffer;
   DOUBLE    dVal;
   BOOL      result;

   /* readIVal */
   result = FALSE;
   if (readDVal(&dVal, buf))
      if(((INT)dVal - dVal) == 0.0)
      { /* the read number is an integer value */
	 result = TRUE;
	 *val   = (INT)dVal;
      } /* the read number is an integer value */
      else {
	 sprintf (buffer, "%g", dVal);
	 fehler(PROG_NAME, 300, NO_INFO, NO_INFO, buffer, NO_INT_FOUND);
      }
   return (result);
} /* readIVal */



/* ===================================== showHelp ===================================== */
static void showHelp (void)
/*--------------------------------------------------------------------------------------*/
/*   Displays some help information for program call syntax and options.                */
/*--------------------------------------------------------------------------------------*/
{
   STRING_TYPE  buffer;

   /* showHelp */   
   sprintf (buffer, PROG_ID_STR, PROGRAM_NAME, VERSION, STAND);
   printf ("%s\n\n  %s %s [OPTIONS]... EXP_FILE [LOG_FILE]\n\n",
           buffer, CALL_MAIN_TXT, EXE_NAME);
   printf (CALL_EXP_TXT);
   printf (CALL_LOG_TXT);
   printf (CALL_OPT_TXT);
   printf (CALL_PAR_A_TXT);
#ifdef MIT_LSV
   printf (CALL_PAR_B_TXT);
#endif /* MIT_LSV */
#ifdef EXT_MATLAB_SIMU
   printf (CALL_PAR_C_TXT);
#endif /* EXT_MATLAB_SIMU */
   printf (CALL_PAR_D_TXT, MIN_ANZ_NACHBARN, MAX_ANZ_NACHBARN);
   printf (CALL_PAR_E_TXT);
   printf (CALL_PAR_F_TXT, RNOTEN_MAX);
   printf (CALL_PAR_G_TXT);
   printf (CALL_PAR_H_TXT);
#ifdef MIT_LSV
   printf (CALL_PAR_Ih_TXT);
   printf (CALL_PAR_ih_TXT);
   printf (CALL_PAR_L_TXT);
   printf (CALL_PAR_l_TXT);
   printf (CALL_PAR_M_TXT);
  #ifdef EXT_SIMU_SERV
   printf (CALL_PAR_N_TXT);
  #endif /* EXT_SIMU_SERV */
  #ifdef PAR_POP_VERS
   printf (CALL_PAR_n_TXT);
  #endif /* PAR_POP_VERS */
   printf (CALL_PAR_Oh_TXT);
#else /* no LS: GLEAM */
   printf (CALL_PAR_Ig_TXT);
   printf (CALL_PAR_ig_TXT);
  #ifdef EXT_SIMU_SERV
   printf (CALL_PAR_N_TXT);
  #endif /* EXT_SIMU_SERV */
  #ifdef PAR_POP_VERS
   printf (CALL_PAR_n_TXT);
  #endif /* PAR_POP_VERS */
   printf (CALL_PAR_Og_TXT);
#endif /* no LS: GLEAM */
   printf (CALL_PAR_P_TXT, MAX_POPEL_SIZE);
   printf (CALL_PAR_p_TXT);
   printf (CALL_PAR_R_TXT);
   printf (CALL_PAR_r_TXT);
#ifdef MIT_LSV
   printf (CALL_PAR_S_TXT);
#endif /* MIT_LSV */
   printf (CALL_PAR_T_TXT);
#ifdef MIT_LSV
   printf (CALL_PAR_t_TXT);
#endif /* MIT_LSV */
   printf (CALL_PAR_W_TXT);
   printf (CALL_PAR_X_TXT);
   printf ("\n");
} /* showHelp */



/* ======================================= main ======================================= */
INT main (int argc, char *argv[])
{
   CHAIN_ELEM_PTR_TYPE  bestChrPtr,    /* pointer to the best result chromosome         */
                        chrPtr;        /* pointer to some result chromosome             */
   char         bigBuf[2*MAX_STRING],  /* buffer for file spec plus content of env.var. */
                buffer[MAX_STRING+MAX_BUF],  /* big string buffer                       */
               *envVar;                /* ponter to content of environment variable     */
   STRING_TYPE  expFName,              /* expFSpec without file extension ".exp"        */
                logFName,              /* logFSpec without file extension ".log"        */
                badParamBuffer,        /* collects superfluous program parameters       */
                expFSpec,              /* file specification EXPeriment file            */
                logFSpec,              /* file specification LOGfile                    */
                chrFileSpec;           /* file specification chromosome file            */
   STR_TYPE     buf1, buf2;            /* buffer for temporary data                     */
   DOUBLE       rParam, r1, r2,        /* auxiliary variables                           */
                oldDistrPerCent,       /* "oldDistribFrac" in percent                   */
                usedTime,              /* job time in sec. with microsec. resolution    */
                bestChrFitness;        /* fitness of the best result chromosome         */
   LONG         liParam;               /* auxiliary variable                            */
   INT          bestChrFitnClass,      /* address (fitness class and number within this */
                bestChrNumber,         /* class) of the best result chromosome          */
                bestChrIdx,            /* index of best result in "last_evo_erg[]"      */
                iParam, ii, jj,        /* auxiliary variables                           */
                len,                   /* string length                                 */
                intVal,                /* local buffer for integer values               */
                tskFileIdx,            /* index of tssk file (TSK) entry                */
                fileCounter,           /* counts the file parameters found              */
                classNumber,           /* number of fitn.classes for separate adaptation*/
                lhcAdaptSpeed,         /* adaptation speed for LHC selection: 0, 1, 2   */
                resChrAmount,          /* number of result chromosomes marked for save  */
                levelAdaptSpeed,       /* adaptation speed for param. levels: 0, 1, 2   */
                retCode;               /* programm return code                          */
   BOOL         withoutExt,            /* TRUE: without file extension                  */
                helpOptionFound,       /* TRUE: -h or -H detected                       */
                logOptionFound,        /* TRUE: "-p"-option controlling logging found   */
                withDetailedLog,       /* TRUE: "-p+" found; detailed logging           */
                withSimuLog,           /* TRUE: with log of extern. simulator interface */
                withResChrSimu,        /* TRUE: simulate result chromosome              */
                withResChrSave,        /* TRUE: save result chromosome(s)               */
                chrMarked,             /* TRUE while chromosomes are marked successfully*/
                evoOrMAWithLHCInit,    /* TRUE: GLEAM or MA with LSV_NEU or LSV_BEST    */
                popBasedOptProc,       /* population based are GLEAM, SMA, ASMA, or AMMA*/
                withChrFile,           /* TRUE: EXP-file contains chrFileSpec           */
                expFileExists,         /* TRUE: EXP-file exists                         */
                expLoadOk,             /* TRUE: all experiment files loaded             */
                newWeights,            /* TRUE: at least 1 -W-option successfully done  */
                pnOptionError,         /* TRUE: there was an error with p- or n-option  */
                newPN_Error,           /* TRUE if a new error in p- or n-option found   */
                cSwitchValue,          /* value of the C-switch to be used later        */
                xOption;               /* TRUE: experimental X-option instead of W-opt. */
   char         ch;                    /* auxiliary variable                            */
   char        *retBuf;                /* auxiliary buffer for parameter conversion     */
#if defined(PAR_POP_VERS) && !defined(EXT_SIMU_SERV)
   INT          nodeNumber;            /* 0 = initializer, >0: number of island node    */
#endif /* PAR_POP_VERS aber nicht EXT_SIMU_SERV */

   /* ------------------------------------ job data ----------------------------------- */
   DOUBLE       targetFitness,         /* target fitness value                          */
                minHamXO,              /* minimal Hamming-Distance for XO, Recoombin.   */
                lamarckRate,           /* Lamarck rate for LHC- or any MA-Job           */
                lhcTerminationParam,   /* termination parameter for LHCs                */
                rankingParam;          /* ranking parameter [1.0, 2.0]                  */
   INT          optMethod,             /* optimization method                           */
                lhcID,                 /* Local Hill Climber identification             */
                popSize,               /* population size                               */
                demeSize,              /* size of the neighbourhood (deme)              */
                initStrat,             /* initialisation strategy                       */
                initStratPar,          /* parameter of initialisation strategy          */
                acceptanceRule,        /* rule for acceptance of best child             */
                resChrNumber,          /* number of requestet result chromosomes        */
                backupRate,            /* rate in generations for pop-backup            */
                genLimit,              /* generation limit                              */
                gakLimit,              /* limit: generations w/o acceptance in deme     */
                gdvLimit,              /* limit: generations w/o improvement of deme    */
                timeLimit,             /* limit for job execution time in seconds       */
                localEvalLimit,        /* limit for function evaluations                */
                lhcIterLimit;          /* limit of LHC iterations                       */
   BOOL         withAllImprovement;    /* TRUE: LHC-opt for all offspring               */


   /* main */
   strcpy (logFSpec, LOGFILE_NAME);         /* use default logfile name                 */
   strcpy (expFSpec, "");                   /* never an uninitialized string            */
   strcpy (badParamBuffer, "");             /* never an uninitialized string            */
   retBuf          = (char*)malloc(MAX_STR);
   retCode         = OK_RET_CODE;
   errLevel        = GLEAM_OK;
   expFileExists   = FALSE;
   expLoadOk       = FALSE;
   withDetailedLog = TRUE;
   logOptionFound  = FALSE;
   withSimuLog     = FALSE;
   optMethod       = NIX_WERT; /* "-Oxxx" is looked for before processing the task file */
   popBasedOptProc = FALSE;
   pnOptionError   = FALSE;
   newPN_Error     = FALSE;
   newWeights      = FALSE;
   fileCounter     = 0;                     /* counts the file parameters found         */
   helpOptionFound = FALSE;
   cSwitchValue    = FALSE;
#ifdef PAR_POP_VERS
   nodeNumber      = NIX_WERT;              /* role is initially unknown                */
#endif /* PAR_POP_VERS */

   /* ------------------ processing of program parameters, part 1: -------------------- */
   strcpy(buf2, "");                         /* collects bad program parameters, if any */
   for (ii = 1;  (ii < argc) && !helpOptionFound;  ii++)
   { /* search for file parameters and the "-p", "-n" and all help options */
      if (argv[ii][0] == '-')
      { /* option found */
	 if (strlen(argv[ii]) > 1)
	 { /* there is an option character */
	    if ((toupper(argv[ii][1]) == 'H') || (strcasecmp (argv[ii], "--help") == 0)) 
	       helpOptionFound = TRUE;           /* options: "-h", "-H", --help, --HELP */
	    else
	    { /* some other option */
	       strcpy (buf1, &(argv[ii][2]));                   /* value of that option */
               switch (argv[ii][1])
	       { /* processing of some options */
	          case 'p': /* -p(+|-): protocol logging +:detailed ------------------- */
		     if (strlen(buf1) == 1) 
		     { /* option length ok */
			ch = argv[ii][2];
			if ((ch == '+') || (ch == '-')) {
			   withDetailedLog = (ch == '+');
			   logOptionFound  = TRUE;
			} else
			   newPN_Error = TRUE;
		     } /* option length ok */
		     else
			newPN_Error = TRUE;
		     break; /* -p(+|-): protocol logging +:detailed */

	          case 'n': /* -n<islandNo> number of island, 0=initializer ----------- */
#ifdef PAR_POP_VERS
		     if (strlen(buf1) >= 1) 
		     { /* option length ok */
			if (readDVal(&r1, buf1))
		        { /* double value successfully read */
			   if(((INT)r1 - r1) == 0.0)
			      nodeNumber = (INT)r1;  /* read number is an integer value */
			   else 
			      newPN_Error = TRUE;
			} /* double value successfully read */
		        else
			   newPN_Error = TRUE;
		     } /* option length ok */
		     else
			newPN_Error = TRUE;
#endif /* PAR_POP_VERS */
		     break; /* -n<islandNo> number of island, 0=initializer */
	       } /* processing of some options */
	    } /* some other option */
	 } /* there is an option character */
	 if (newPN_Error)
	 { /* error in p- or n-option */
	    strcat(buf2, argv[ii]);                        /* collect error information */
	    strcat(buf2, " ");                             /* because package "fbhm" is */
	    pnOptionError = TRUE;                          /* not yet initialized!      */
	    newPN_Error   = FALSE;
	 } /* error in p- or n-option */
      } /* option found */
      else
      { /* file parameter found */
	 fileCounter++;
	 if (fileCounter == 1)
	 { /* first file parameter is taken as exp file specification */
	    len = strlen (argv[ii]);
	    if (len > 4)
	       withoutExt = strcasecmp (&(argv[ii][len-4]), ".exp") != 0;
	    else
	       withoutExt = TRUE;
	    strcpy (expFSpec, argv[ii]);
	    if (withoutExt) {
	       strcat (expFSpec, ".exp");
	       strcpy (expFName, argv[ii]);
	    } else {
	       strcpy (expFName, argv[ii]);
	       expFName[len-4] = '\0';
	    }
	    expFileExists = file_exists (expFSpec);
	 } /* first file parameter is taken as exp file specification */
	 if (fileCounter == 2)
	 { /* second file parameter is taken as log file specification */
	    len = strlen (argv[ii]);
	    if (len > 4)
	       withoutExt = strcasecmp (&(argv[ii][len-4]), ".log") != 0;
	    else
	       withoutExt = TRUE;
	    strcpy (logFSpec, argv[ii]);
	    if (withoutExt) {
	       strcat (logFSpec, ".log");
	       strcpy (logFName, argv[ii]);
	    } else {
	       strcpy (logFName, argv[ii]);
	       logFName[len-4] = '\0';
	    }
	 } /* second file parameter is taken as log file specification */
	 if (fileCounter > 2) { /* unrecognized parameter found */
	    strcat (badParamBuffer, argv[ii]);
	    strcat (badParamBuffer, " ");
	 } /* unrecognized parameter found */
      } /* file parameter found */
   } /* search for file parameters and the "-p", "-n", and all help options */
   if ((fileCounter == 1) && expFileExists)
   { /* use exp file name for log file */
      strcpy (logFSpec, expFName);
      strcat (logFSpec, ".log");
      strcpy (logFName, expFName);
   } /* use exp file name for log file */
#ifdef PAR_POP_VERS
   if (nodeNumber > 0)
   { /* extend logfile name according to the island number */
      sprintf(buf1, ISLAND_TXT, nodeNumber);
      sprintf(logFSpec, "%s%s.log", logFName, buf1);
   } /* extend logfile name according to the island number */
#endif /* PAR_POP_VERS */

   if (helpOptionFound || (fileCounter == 0)) { /* terminate program */
      showHelp ();
      if (!helpOptionFound)
	 retCode = ERR_RET_CODE;
   } /* terminate program */
   else
   { /* no help option found and at least one file parameter detected */
      /* -------------------------- package initialization: --------------------------- */
      if (gen_init (FALSE, TRUE, logFSpec))           /* no chr-I/O log, with error log */
      { /* package initialization ok */
	 if (fileCounter > 2)
	    errLevel = fatal(PROG_NAME, 1, fileCounter-2, NO_INFO, badParamBuffer, UNKNOWN_PARAMS);
	 if (pnOptionError) {
	    meldung (PROG_NAME, NO_INFO, NO_INFO, buf2, BAD_PARAM_TXT_VALUE);
	    withDetailedLog = TRUE;     /* it is better to document call parameters now */
	 }
	 if (withDetailedLog || (errLevel != GLEAM_OK))
	 { /* log all program parameters */
	    strcpy (buffer, AUFRUF_DOKU);
	    for (ii = 1;  ii < argc;  ii++) {
	       strcat (buffer, argv[ii]);
	       strcat (buffer, " ");
	    }
	    write_to_log (buffer, CLOSE_LOG);
	    detailed_log = withDetailedLog;   /* for involved packages which load files */
	 } /* log all program parameters */
	 if (expFileExists)
	 { /* exp file exists */
	    /* ------------------------- load experiment file ------------------------- */
	    expLoadOk = load_exp(&withChrFile, expFSpec, TRUE);         /* TRUE=Startup */
	    sprintf (buffer, EXP_FILE_TXT, expFSpec);
	    write_to_log (buffer, CLOSE_LOG);
	    if (withDetailedLog) { /* detailed logging */
	       write_to_log (INIT_HDR_1, KEEP_LOG_OPEN);
	       write_to_log (INIT_HDR_2, CLOSE_LOG);
	       logTextBuffer (OHNE_TERM_AUSG);                 /* log possible messages */
	    } /* detailed logging */
	    else
	       reset_text_buf();                            /* delete possible messages */

	    if (expLoadOk && (appl_file_anz == exp_afile_anz))
	    { /* all necessary initialization files loaded */
	       if (errLevel == GLEAM_OK)
               { /* ok */
		  if (withDetailedLog) 
		     write_to_log (INIT_FIN_TXT, CLOSE_LOG);

                  /* ----------- processing of program parameters, part 2: ------------ */
		  for (ii = 1;  ii < argc;  ii++)    /* search for "-O"- or "-o"-option */
		     if ((argv[ii][0] == '-') && (toupper(argv[ii][1]) == 'O'))
			if (getEnumIndexFromParIdx(&intVal, &(argv[ii][2]), OPT_PROCEDURE_IDX))
			   optMethod = intVal;
			else
			   errLevel |= fatal(PROG_NAME, 4, NO_INFO, NO_INFO, &(argv[ii][2]), 
					     UNKNOWN_OPT_PROC_ERR);
	       } /* ok */
	    } /* all necessary initialization files loaded */
	    else
	       errLevel = fatal(PROG_NAME, 5, NO_INFO, NO_INFO, "", NO_INI);
	    log_msgs ();                                /* log possible (error)messages */

	    if (errLevel == GLEAM_OK)
	    { /* all initialization files loaded and (ext.) simu ok */
               /* ----------------------- initialize job data: ------------------------ */
	       targetFitness       = NIX_WERT;
	       minHamXO            = NIX_WERT;
	       lamarckRate         = 100.0;     /* Lamarkian evolution for memetic alg. */
	       oldDistrPerCent     = oldDistribFrac * 100.0;
	       lhcTerminationParam = NIX_WERT;
	       localEvalLimit      = INT_MAX;   /* i.e. disabled termination  condition */
	       timeLimit           = NIX_WERT;
	       lhcID               = NIX_WERT;
	       popSize             = NIX_WERT;  /* population size                      */
	       demeSize            = NIX_WERT;  /* size of the neighbourhood (deme)     */
	       initStrat           = NIX_WERT;  /* initialisation strategy              */
	       initStratPar        = NIX_WERT;  /* parameter of initialisation strategy */
	       resChrNumber        = defResultChrs;         /* number of requestet result chromos.  */
	       backupRate          = NIX_WERT;  /* rate in generations for pop-backup   */
	       genLimit            = NIX_WERT;  /* generation limit                     */
	       gakLimit            = NIX_WERT;  /* limit: gen. w/o acceptance in deme   */
	       gdvLimit            = NIX_WERT;  /* limit: gen. w/o improvement of deme  */
	       lhcIterLimit        = NIX_WERT;  /* limit of LHC iterations              */
               classNumber         = NIX_WERT;  /* fitn.classes for separate adaptation */
               lhcAdaptSpeed       = NIX_WERT;  /* adapt.speed for LHC selection: 0,1,2 */
               levelAdaptSpeed     = NIX_WERT;  /* adapt.speed for param .levels: 0,1,2 */
	       withAllImprovement  = FALSE;     /* default: switched off                */
	       mitPhaenoRepair     = FALSE;     /* default: switched off                */
	       withResChrSave      = FALSE;     /* default: switched off                */
	       withResChrSimu      = FALSE;     /* default: switched off                */
	       acceptanceRule      = default_survival_rule;  /* default from module evo */
	       rankingParam        = def_max_fit;            /* default from module evo */

               /* -------------- read logging parameter from TSK-file: ---------------- */
	       if (!logOptionFound)       /* TSK file only relevant, if no "-p"-option: */
		  getBoolVal(&withDetailedLog, WITH_DETAILED_LOG_IDX, GLEAM_ERROR);

               /* ------------ read optimization procedure from TSK-file: ------------- */
#ifdef MIT_LSV
	       if (optMethod != NIX_WERT) 
	       { /* optimization method given by "-O"-option */
		  if (getIntVal(&intVal, OPT_PROCEDURE_IDX, GLEAM_OK))
		     if ((optMethod != intVal) && (intVal != def_opt_strat)) {
			getEnumText(buf1, OPT_PROCEDURE_IDX, GLEAM_OK);
			meldung (PROG_NAME, NO_INFO, NO_INFO, buf1, OPT_PROC_IGNORED_MLDG);
		     }
	       } /* optimization method given by "-O"-option */
	       else
	       { /* no "-O"-option */
		  if (!getIntVal(&optMethod, OPT_PROCEDURE_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 6, NO_INFO, NO_INFO, OPT_PROCEDURE_TXT, NOT_IN_TSK_FILE);
	       } /* no "-O"-option */
	       if ((lsv_anz == 0) && (optMethod != GLEAM) && (errLevel == GLEAM_OK))  
		  errLevel = fatal(PROG_NAME, 7, NO_INFO, NO_INFO,  /* no LHCs although */
				   "", REQUIRED_LHCS_NOT_AVAIL);    /* required by job  */
#else /* no MIT_LSV: pure GLEAM */
	       if (optMethod != NIX_WERT) 
	       { /* optimization method given by "-O"-option */
		  if (optMethod != GLEAM) 
		      errLevel = fatal(PROG_NAME, 8, optMethod, NO_INFO, IMPROPER_TXT, 
				       UNKNOWN_OPT_VERF);
		  else
		  { /* GLEAM specified by "-O"-option */
		     if (getIntVal(&intVal, OPT_PROCEDURE_IDX, GLEAM_OK))
			if (intVal != GLEAM) {
			   getEnumText(buf1, OPT_PROCEDURE_IDX, GLEAM_OK);
			   meldung (PROG_NAME, NO_INFO, NO_INFO, buf1, OPT_PROC_IGNORED_MLDG);
			}
		  } /* GLEAM specified by "-O"-option */
		  optMethod = GLEAM;
	       } /* optimization method given by "-O"-option */
	       else
	       { /* no "-O"-option */
		  if (getIntVal(&intVal, OPT_PROCEDURE_IDX, GLEAM_OK))
		     if (intVal != GLEAM)
			errLevel = fatal(PROG_NAME, 9, NO_INFO, NO_INFO, "", ONLY_GLEAM);
		  optMethod = GLEAM;
	       } /* no "-O"-option */
#endif /* no MIT_LSV: pure GLEAM */
	       popBasedOptProc = (GLEAM <= optMethod) && (optMethod <= GLEAM_AMMA);

               /* --- read TSK-file param.required for population based opt. proc.: --- */
               if (popBasedOptProc)
	       { /* population based are at present GLEAM, SMA, ASMA, and AMMA */
		  if (!getIntVal(&popSize, POP_SIZE_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 10, NO_INFO, NO_INFO, POP_SIZE_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&demeSize, DEME_SIZE_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 11, NO_INFO, NO_INFO, DEME_SIZE_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&timeLimit, RUNTIME_LIMIT_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 12, NO_INFO, NO_INFO, RUNTIME_LIMIT_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&genLimit, GEN_LIMIT_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 13, NO_INFO, NO_INFO, GEN_LIMIT_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&gakLimit, GAC_LIMIT_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 14, NO_INFO, NO_INFO, GAC_LIMIT_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&gdvLimit, GDI_LIMIT_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 15, NO_INFO, NO_INFO, GDI_LIMIT_TXT, NOT_IN_TSK_FILE);
		  if (!getDoubleVal(&minHamXO, XO_HAMDIST_IDX, GLEAM_OK))
		     errLevel = fatal(PROG_NAME, 16, NO_INFO, NO_INFO, XO_HAMDIST_TXT, NOT_IN_TSK_FILE);
	       } /* population based are at present GLEAM, SMA, ASMA, and AMMA */

               /* -------------- read always required TSK-file parameter: ------------- */
	       if (!getDoubleVal(&targetFitness, TARGET_FITNESS_IDX, GLEAM_OK))
		  errLevel = fatal(PROG_NAME, 17, NO_INFO, NO_INFO, TARGET_FITNESS_TXT, NOT_IN_TSK_FILE);
	       if (!getIntVal(&initStrat, INIT_STRATEGY_IDX, GLEAM_OK))
		  errLevel = fatal(PROG_NAME, 18, NO_INFO, NO_INFO, INIT_STRATEGY_TXT, NOT_IN_TSK_FILE);
	       if (!getIntVal(&initStratPar, INIT_SPAR_IDX, GLEAM_OK))
		  if ((initStrat == MIX)     || (initStrat == BEST_NEU) || 
		      (initStrat == LSV_NEU) || (initStrat == LSV_BEST)   )
		     errLevel = fatal(PROG_NAME, 19, NO_INFO, NO_INFO, INIT_SPAR_TXT, NOT_IN_TSK_FILE);

#ifdef CEC_MBF
	       if (!getIntVal(&localEvalLimit, CEC_FES_LIMIT_IDX, GLEAM_OK))
		  errLevel = fatal(PROG_NAME, 20, NO_INFO, NO_INFO, CEC_FES_LIMIT_TXT, NOT_IN_TSK_FILE);
#else /* no CEC_MBF */
	       getIntVal(&localEvalLimit, EVAL_LIMIT_IDX, GLEAM_OK);
#endif /* no CEC_MBF */

               /* ----------------- read optional TSK-file parameter: ----------------- */
	       getBoolVal(&withResChrSimu, WITH_RES_CHR_SIMU_IDX, GLEAM_OK);
               if (mitOptionPhaenoRep) 
	       { /* with phenotypic repair option */
		  if (!getBoolVal(&mitPhaenoRepair, WITH_PHAENO_REP_IDX, GLEAM_OK))
                     meldung (PROG_NAME, NO_INFO,NO_INFO, WITH_PHAENO_REP_TXT, NOT_IN_TSK_FILE);
	       } /* with phenotypic repair option */
	       else
		  if (getBoolVal(&mitPhaenoRepair, WITH_PHAENO_REP_IDX, GLEAM_OK))
                     meldung (PROG_NAME, NO_INFO,NO_INFO, EVOPAR_PHAENOKLTXT, REPAIR_NOT_IMPL);
               if (mit_gen_rep_par) 
               { /* with genetic repair */
		  if (getDoubleVal(&rParam, GEN_REP_RATE_IDX, GLEAM_OK))
		  { /* parameter for genetic repair provided */
		     rParam = rParam / 100.0;
		     if ((gen_rep_par_ug <= rParam) && (rParam <= gen_rep_par_og))
			 gen_rep_par = rParam;
		     else
			errLevel = fatal(PROG_NAME, 21, l_round(rParam * 100.0), NO_INFO, 
					 GEN_REP_RATE_TXT, BAD_PARAM_VALUE);
		  } /* parameter for genetic repair provided */
                  else 
                     meldung (PROG_NAME, NO_INFO,NO_INFO, GEN_REP_RATE_TXT, NOT_IN_TSK_FILE);
	       } /* with genetic repair */
	       else
		  if(getDoubleVal(&rParam, GEN_REP_RATE_IDX, GLEAM_OK))
		     meldung (PROG_NAME, NO_INFO,NO_INFO, EVOPAR_GENO_KL_TXT, REPAIR_NOT_IMPL);
	       getBoolVal(&initZufGen, WITH_RANDGEN_INIT_IDX, GLEAM_OK);
	       getBoolVal(&withResChrSave, WITH_RES_CHR_SAVE_IDX, GLEAM_OK);
	       getBoolVal(&withSimuLog, WITH_SIMU_LOG_IDX, GLEAM_OK);
	       getIntVal(&acceptanceRule, ACCEPTANCE_RULE_IDX, GLEAM_OK);
	       getIntVal(&backupRate, BACKUP_RATE_IDX, GLEAM_OK);
	       getBoolVal(&with_evo_term_tst, WITH_TERM_TEST_IDX, GLEAM_OK);
	       if (with_evo_term_tst)
	       { /* allow opt.jobs to be stopped externally */
		  if (optMethod != LSV)       /* only population based opt.jobs can be stopped */
		     getIntVal(&evo_tst_frequ, TERM_TEST_FREQ_IDX, GLEAM_OK);   /*  externally */
		  else
		     meldung (PROG_NAME, NO_INFO, NO_INFO, NO_EXT_TERM_MLDG, GENERAL_MESSAGE);
	       } /* allow opt.jobs to be stopped externally */
	       getIntVal(&resChrNumber, NUMBER_OF_RESULTS_IDX, GLEAM_OK);
	       getDoubleVal(&rankingParam, RANKING_PAR_IDX, GLEAM_OK);
	       getIntVal(&lhcID, LHC_INDEX_IDX, GLEAM_OK);
	       getIntVal(&lhcIterLimit, LHC_ITER_LIMIT_IDX, GLEAM_OK);
	       getDoubleVal(&lhcTerminationParam, LHC_TERM_LIMIT_IDX, GLEAM_OK);
	       if (optMethod != GLEAM)
	       { /*  any MA- or LHC-job */
		  if (getDoubleVal(&rParam, LAMARCK_RATE_IDX, GLEAM_OK))
	          { /* must ba a value within its limits and this must be checked here */
		     getDoubleLimits(&r1, &r2, LAMARCK_RATE_IDX); /* works as previos call worked */
		     if ((r1 <= rParam) && (rParam <= r2))
			lamarckRate = rParam;
		     else
			errLevel = fatal(PROG_NAME, 22, rParam, NO_INFO, LAMARCK_RATE_TXT, BAD_PARAM_VALUE);
		  } /* must ba a value within its limits and this must be checked here */
	       } /*  any MA- or LHC-job */
	       getBoolVal(&withAllImprovement, MA_WITH_ALL_IMPR_IDX, GLEAM_OK);
	       getDoubleVal(&oldDistrPerCent, OLD_DISTR_FRAC_IDX, GLEAM_OK);

               if ((optMethod == GLEAM_ASMA) || (optMethod == GLEAM_AMMA))
               { /* ASMA or AMMA */
		  if (!getIntVal(&classNumber, NUMB_OF_FCLASSES_IDX, GLEAM_OK))
		     meldung (PROG_NAME, NO_INFO, NO_INFO, NUMB_OF_FCLASSES_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&lhcAdaptSpeed, LHC_ADAPT_SPEED_IDX, GLEAM_OK))
		     meldung (PROG_NAME, NO_INFO, NO_INFO, LHC_ADAPT_SPEED_TXT, NOT_IN_TSK_FILE);
		  if (!getIntVal(&levelAdaptSpeed, LEVEL_ADAPT_SPEED_IDX, GLEAM_OK))
		     meldung (PROG_NAME, NO_INFO, NO_INFO, LEVEL_ADAPT_SPEED_TXT, NOT_IN_TSK_FILE);
	       } /* ASMA or AMMA */

               /* ------------- processing of program parameters, part 3: ----------------------- */
	       if (errLevel != GLEAM_FATAL)
	       { /* no fatal errors occurred */
		  for (ii = 1;  ii < argc;  ii++)
		  { /* search for options */
		     if  (argv[ii][0] == '-')
		     { /* option found */
			if (strlen(argv[ii]) > 1)
			   strcpy (buf1, &(argv[ii][2]));                 /* value of that option */
			else
			   strcpy(buf1, "");                               /* no option character */
			if (strlen(buf1) > 0)
			{ /* there is something after the option character */
			   switch (argv[ii][1])
			   { /* processing of options */
			   case 'A': /* -A<text> acceptance rule -------------------------------- */
			   case 'a':
			      if (getEnumIndexFromParIdx(&intVal, buf1, ACCEPTANCE_RULE_IDX))
				 acceptanceRule = intVal;
			      else
				 errLevel = fatal(PROG_NAME, 30, NO_INFO, NO_INFO, buf1, 
						  UNKNOWN_ACCEPT_RULE_ERR);
			      break; /* -A<text> acceptance rule */

			   case 'B': /* -B(+|-): best improvement ------------------------------- */
			   case 'b':
#ifdef MIT_LSV
			      if (strlen(argv[ii]) == 3) 
			      { /* option length ok */
				 ch = argv[ii][2];
				 if ((ch == '+') || (ch == '-'))
				    withAllImprovement = (ch == '-');
				 else
				    errLevel = fatal(PROG_NAME, 31, NO_INFO, NO_INFO, argv[ii], 
						     BAD_PARAM_TXT_VALUE);
			      } else
				 errLevel = fatal(PROG_NAME, 32, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
#else /* no MIT_LSV: pure GLEAM */
			      errLevel |= fehler(PROG_NAME, 33, NO_INFO, NO_INFO, BAD_MA_SW_TXT, ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
			      break; /* -B(+|-): best improvement */

#ifdef EXT_MATLAB_SIMU
			   case 'C': /* -C(+|-): case file generation --------------------------- */
			   case 'c':
			      if (strlen(argv[ii]) == 3) 
			      { /* option length ok */
				 ch = argv[ii][2];
				 if ((ch == '+') || (ch == '-'))
				 { /* valid character */
				    if (ch == '+') 
				       if (withResChrSimu && (resChrNumber > 0))
					  cSwitchValue = TRUE;
				       else
					  write_to_log (NO_SIM_NO_CASE_FILE, CLOSE_LOG); 
				    else
				       cSwitchValue = FALSE;
				 } /* valid character */
				 else
				    errLevel = fatal(PROG_NAME, 63, NO_INFO, NO_INFO, argv[ii], 
						     BAD_PARAM_TXT_VALUE);
			      } else
				 errLevel = fatal(PROG_NAME, 64, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -C(+|-): case file generation */
#endif /* EXT_MATLAB_SIMU */

			   case 'D': /* -D<demeSize> -------------------------------------------- */
			   case 'd':
			      if (!readIVal(&demeSize, buf1))
				 errLevel = fatal(PROG_NAME, 34, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -D<demeSize> */

			   case 'E': /* -E<number> evaluation limit (opt.term.cond.) ------------ */
			   case 'e':
			      if (!readIVal(&localEvalLimit, buf1))
				 errLevel = fatal(PROG_NAME, 35, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -E<number> evaluation limit (opt.term.cond.) */

			   case 'F': /* -F<fitness> (termination condition) --------------------- */
			   case 'f':
			      if (!readDVal(&targetFitness, buf1))
				 errLevel = fatal(PROG_NAME, 36, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -F<fitness> termination condition */

			   case 'G': /* -G<gen> generation limit, termination condition */
			   case 'g':
			      if (!readIVal(&genLimit, buf1))
				 errLevel = fatal(PROG_NAME, 37, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -G<gen> generation limit (termination condition) */

			   case 'I': /* -I<text> initialization procedure ----------------------- */
			      if (getEnumIndexFromParIdx(&intVal, buf1, INIT_STRATEGY_IDX))
				 initStrat = intVal;
			      else
				 errLevel = fatal(PROG_NAME, 38, NO_INFO, NO_INFO, buf1, 
						  UNKNOWN_INIT_PROC_ERR);
			      break; /* -I<text> initialization procedure */

			   case 'i': /* -i<value> initialization parameter ---------------------- */
			      if (!readIVal(&initStratPar, buf1))
				 errLevel = fatal(PROG_NAME, 39, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -i<value> initialization parameter */

			   case 'L': /* -L(+|-): Lamarckian evolution --------------------------- */
#ifdef MIT_LSV
			      if (strlen(argv[ii]) == 3) 
			      { /* option length ok */
				 ch = argv[ii][2];
				 if (ch == '+')
				    lamarckRate = 100.0;
				 else
				    if (ch == '-')
				       lamarckRate = 0.0;
				    else
				       errLevel = fatal(PROG_NAME, 40, NO_INFO, NO_INFO, argv[ii], 
							BAD_PARAM_TXT_VALUE);
			      } else
				 errLevel = fatal(PROG_NAME, 41, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
#else /* no MIT_LSV: pure GLEAM */
			      errLevel |= fehler(PROG_NAME, 42, NO_INFO, NO_INFO, BAD_MA_SW_TXT, ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
			      break; /* -L(+|-): Lamarckian evolution */

			   case 'l': /* -l<frac> learning rate ---------------------------------- */
#ifdef MIT_LSV
			      if (!readDVal(&oldDistrPerCent, buf1))
				 errLevel = fatal(PROG_NAME, 43, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
#else /* no MIT_LSV: pure GLEAM */
			      errLevel |= fehler(PROG_NAME, 44, NO_INFO, NO_INFO, BAD_MA_SW_TXT, ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
			      break; /* -l<frac> learning rate */

			   case 'M': /* -M<value> maximum of iterations per LHC run ------------- */
			   case 'm':
#ifdef MIT_LSV
			      if (!readIVal(&lhcIterLimit, buf1))
				 errLevel = fatal(PROG_NAME, 45, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
#else /* no MIT_LSV: pure GLEAM */
			      errLevel |= fehler(PROG_NAME, 46, NO_INFO, NO_INFO, BAD_LSV_SW_TXT, ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
			   break; /* -M<value> maximum of iterations per LHC run */

			   case 'N': /* -N<text> domain name of the ESS ------------------------- */
#ifdef EXT_SIMU_SERV 
			      /*
                              printf("HP: -N anf: sim_ok=%d sim_up=%d atProgStart=%d domain=\"%s\" model=\"%s\"\n", 
				     sim_ok, sim_up, atProgStart, extSimServDomainName, extSimuModelName);
			      */
			      strcpy(extSimServDomainName, buf1);
			      setStringVal(extSimServDomainName, SIMU_DOMAIN_NAME_IDX);
			      setServerPraeFix();  /* sets or updates URL start in "serverBuffer" */
#else /* no EXT_SIMU_SERV */
			      errLevel |= fehler(PROG_NAME, 61, NO_INFO, NO_INFO, argv[ii], UNKNOWN_PARAM);
#endif /* no EXT_SIMU_SERV */
			      break; /* -N<text> domain name of the ESS */

			   case 'n': /* -n<islandNo> number of node or island ------------------- */
#if defined(PAR_POP_VERS)
			      setServerPraeFix();  /* sets or updates URL start in "serverBuffer" */
#else  /* no PAR_POP_VERS */
			      errLevel |= fehler(PROG_NAME, 62, NO_INFO, NO_INFO, argv[ii], UNKNOWN_PARAM);
#endif /* no PAR_POP_VERS */
			      break; /* -n<islandNo> number of node or island, see part 1 */

			   case 'O': /* -O<text>: optimization method, see part 2 --------------- */
			   case 'o':
			      break; /* -O<text>: optimization method, see part 2 */

			   case 'P': /* -P<popSize> population size ----------------------------- */
			      if (!readIVal(&popSize, buf1))
				 errLevel = fatal(PROG_NAME, 47, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -P<popSize> population size */

			   case 'p': /* -p(+|-): protocol logging +:detailed -------------------- */
			      break; /* -p(+|-): protocol logging +:detailed, see part 1 */

			   case 'R': /* -R<value> ranking parameter ----------------------------- */
			      if (!readDVal(&rankingParam, buf1))
				 errLevel = fatal(PROG_NAME, 48, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -R<value> ranking parameter */

			   case 'r': /* -r<number> number of results ---------------------------- */
			      if (!readIVal(&resChrNumber, buf1))
				 errLevel = fatal(PROG_NAME, 49, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      break; /* -r<number> number of results */

			   case 'S': /* -S<index> Searcher: LHC index --------------------------- */
			   case 's':
#ifdef MIT_LSV
			      if (!readIVal(&lhcID, buf1))
				 errLevel = fatal(PROG_NAME, 50, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
#else /* no MIT_LSV: pure GLEAM */
			      errLevel |= fehler(PROG_NAME, 51, NO_INFO, NO_INFO, BAD_LSV_SW_TXT, ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
			      break; /* -S<index> Searcher: LHC index */

			   case 'T': /* -T<value> time limit, termination condition ------------- */
			      if (!readIVal(&iParam, buf1))
				 errLevel = fatal(PROG_NAME, 52, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
			      else
				 timeLimit = (LONG)iParam;
			      break; /* -T<value> time limit, termination condition */

			   case 't': /* -t<value> threshold for LHC termination ----------------- */
#ifdef MIT_LSV
			      if (!readDVal(&lhcTerminationParam, buf1))
				 errLevel = fatal(PROG_NAME, 53, NO_INFO, NO_INFO, argv[ii], 
						  BAD_PARAM_TXT_VALUE);
#else /* no MIT_LSV: pure GLEAM */
			      errLevel |= fehler(PROG_NAME, 54, NO_INFO, NO_INFO, BAD_LSV_SW_TXT, ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
			      break; /* -t<value> threshold for LHC termination */

			   case 'W': /* -W<nr>:<val> new weigt of crterion <nr> ----------------- */
			   case 'w':
			   case 'X': /* -X<val>:<val> two values for experiments ---------------- */
			   case 'x':
			      xOption = (argv[ii][1] == 'X') || (argv[ii][1] == 'x');
#ifdef PAR_POP_VERS
			      if (xOption)
			      { /* "-X"-option */
				 if (readIVal(&delayMSec, buf1))
				 { /* integer value obtained */
				    if (delayMSec < 0) 
				       errLevel = fatal(PROG_NAME, 55, delayMSec, NO_INFO, argv[ii], 
							ACT_VAL_OUT_OF_RANGE);
				 } /* integer value obtained */
				 else
				    errLevel = fatal(PROG_NAME, 55, NO_INFO, NO_INFO, argv[ii], 
						     BAD_PARAM_TXT_VALUE);
			      } /* "-X"-option */
			      else
				 errLevel = fatal(PROG_NAME, 55, NO_INFO, NO_INFO, argv[ii], UNKNOWN_PARAM);
#else /* no PAR_POP_VERS */
			      jj  = 0;
			      len = strlen(buf1);
			      while ((buf1[jj] != ':') && (jj < len)) /* search for ':' */
				 jj++;
			      if (jj < len)
			      { /* ':' found */
				 strncpy(buf2, buf1, (size_t)jj);
				 buf2[jj] = '\0';
				 if (!readIVal(&intVal, buf2))
				    errLevel = fatal(PROG_NAME, 56, NO_INFO, NO_INFO, argv[ii], 
						     BAD_PARAM_TXT_VALUE);
				 else
				 { /* first value successfully read */
				    strcpy(buf2, &buf1[jj+1]);
				    if (!readDVal(&rParam, buf2))
				       errLevel = fatal(PROG_NAME, 57, NO_INFO, NO_INFO, argv[ii], 
							BAD_PARAM_TXT_VALUE);
				    else
				    { /* 2nd value successfully read */
				       if ((0 < intVal) && (intVal <= akt_anz_krit))
				       { /* intVal is a valid criteria number */
				          if ((0 <= rParam) && (rParam <= RNOTEN_MAX))
					  { /* <val> is a valid fitness value */
					     kriterien[intVal-1].max_note = rParam;
					     newWeights = TRUE;
					  } /* <val> is a valid fitness value */
					  else
					     errLevel = fatal(PROG_NAME, 60, i_round(rParam), NO_INFO, 
							      "-W", BAD_PARAM_VALUE); 
				       } /* intVal is a valid criteria number */
				       else
					  errLevel = fatal(PROG_NAME, 61, intVal, NO_INFO, "", BAD_KRIT_INDX); 
				    } /* 2nd value successfully read */
				 } /* first value successfully read */
			      } /* ':' found */
			      else
				 errLevel = fatal(PROG_NAME, 68, NO_INFO, NO_INFO, argv[ii], UNKNOWN_PARAM);
#endif /* no PAR_POP_VERS */
			      break; /* -W<nr>:<val> new weigt of crterion <nr> / -X... */

			   default:
			      meldung (PROG_NAME, NO_INFO, NO_INFO, argv[ii], UNKNOWN_PARAM);
			   } /* processing of options */
			} /* there is something after the option character */
			else
			   errLevel = fatal(PROG_NAME, 69, NO_INFO, NO_INFO, argv[ii], 
					    MISSING_DATA_OF_OPTION_ERR);
		     } /* option found */
		  } /* search options */
	       } /* no fatal errors occurred */

               /* ----------------- semantic check of all parameters: ----------------- */
	       if (errLevel != GLEAM_FATAL)
	       { /* TSK-file and options successfully read */
		  if ((initStrat != NEU) && (initStrat != BEST) && (initStrat != MIX) && 
#ifdef MIT_LSV
		      (initStrat != LSV_NEU) && (initStrat != LSV_BEST) &&
#endif /* MIT_LSV */
		      (initStrat != BEST_NEU) && (initStrat != FROM_FILE))
		     errLevel = fatal(PROG_NAME, 70, initStrat, NO_INFO, "", BAD_STRATEGY);
		  evoOrMAWithLHCInit = (optMethod != LSV) && 
		                       ((initStrat == LSV_NEU) ||(initStrat == LSV_BEST));
                  if (mitOptionPhaenoRep && mit_gen_rep_par && mitPhaenoRepair && (gen_rep_par > 0.0))
                     meldung (PROG_NAME, NO_INFO, NO_INFO, "", NO_GEN_AND_PHAENO_REP);
#ifdef MIT_LSV
                  if ((GLEAM <= optMethod) && (optMethod <= LAST_OPT_VERF))
#else /* no MIT_LSV: pure GLEAM */
                  if (optMethod == GLEAM)
#endif /* no MIT_LSV: pure GLEAM */
                  { /* valid optimization procedure */
#ifdef MIT_LSV
                     if ((optMethod == LSV)        || (optMethod == GLEAM_SMA) ||
                         (optMethod == GLEAM_ASMA) || evoOrMAWithLHCInit) 
                     { /* LSV, SMA, ASMA, or LHC-init: a valid LHC must be specified! */
                        if (lhcID == NIX_WERT)
                           errLevel = fatal(PROG_NAME, 71, NO_INFO, NO_INFO, LHC_INDEX_TXT, NO_PARAM_VALUE);
                        else 
                           if ((0 <= lhcID) && (lhcID < LSV_ANZ)) {
                              if (!lsv[lhcID].aktiv)
                                 errLevel = fatal(PROG_NAME, 72, lhcID, NO_INFO, LHC_INDEX_TXT, BAD_PARAM_VALUE);
                           }
                           else
                              errLevel = fatal(PROG_NAME, 73, lhcID, NO_INFO, "", UNKNOWN_LSV);
			if ((optMethod != GLEAM_ASMA) || evoOrMAWithLHCInit)
			{ /*  no ASMA or LHC-init:  LHC paramerter must be specified! */
			   if (lhcIterLimit == NIX_WERT)
			      errLevel = fatal(PROG_NAME, 74, NO_INFO, NO_INFO, LHC_ITER_LIMIT_TXT, 
						NO_PARAM_VALUE);
			   else
			      if (lhcIterLimit < 1)
				 errLevel = fatal(PROG_NAME, 75, lhcIterLimit, NO_INFO, 
						   LHC_ITER_LIMIT_TXT, BAD_PARAM_VALUE);
  #ifdef MIT_ROCO
			   if ((lhcTerminationParam == NIX_WERT) && (lhcID == LSV_ROSEN))
			      errLevel = fatal(PROG_NAME, 76, NO_INFO, NO_INFO, LHC_TERM_LIMIT_TXT, 
						NO_PARAM_VALUE);
			   else
			      if ((lhcTerminationParam <= 0) || (1.0 <= lhcTerminationParam))
				 errLevel = fatal(PROG_NAME, 77, lhcTerminationParam, NO_INFO, 
						   LHC_ITER_LIMIT_TXT, BAD_PARAM_VALUE);
  #endif /* MIT_ROCO */
			} /*  no ASMA or LHC-init:  LHC paramerter must be specified! */
                     } /* LSV, SMA, ASMA, or LHC-init: a valid LHC must be specified! */
                     if (optMethod == LSV)
		     { /* LHC run only */
			if (popSize == NIX_WERT)
			    popSize = 1;
			if (popSize > 1) {
                           meldung(PROG_NAME, NO_INFO, NO_INFO, "", POP_SIZE_FOR_LHC_MSG);
			   popSize = 1;
			}
			genLimit  = 0;
			gdvLimit  = 0;
			gakLimit  = 0;
			timeLimit = 86400;         /* seconds. For display purpose only */
			if ((initStrat != NEU) && (initStrat != BEST))
			   errLevel = fatal(PROG_NAME, 79, initStrat, NO_INFO, "", BAD_STRATEGY);
		     } /* LHC run only */
#endif /* MIT_LSV */
		     if (optMethod != LSV)
                     { /* population based optimization procedures */
			if ((demeSize < MIN_ANZ_NACHBARN) || (MAX_ANZ_NACHBARN < demeSize))
			   errLevel = fatal(PROG_NAME, 80, demeSize, NO_INFO, DEME_SIZE2_TXT, 
					    BAD_PARAM_VALUE);
                        if ((popSize < 4) || (MAX_POPEL_SIZE < popSize))
                           errLevel = fatal(PROG_NAME, 81, popSize, NO_INFO, PSIZE_TXT, BAD_PARAM_VALUE);
                        else 
                        { /* valid population size */
			   if (((demeSize % 2) == 1) || (demeSize > popSize))     /* demeSize must be   */
			      errLevel = fatal(PROG_NAME, 82, demeSize, NO_INFO,  /* even and not larger*/
					       DEME_SIZE2_TXT, BAD_PARAM_VALUE);  /* than popSize       */
                           if (popSize/2 < resChrNumber)      /* additional check below */
                           { /* reduce number of result chromosomes */
			      sprintf (buffer, REDUZ_TXT, resChrNumber, popSize/2);
			      write_to_log (buffer, CLOSE_LOG);
			      resChrNumber = popSize/2;
			   } /* reduce number of result chromosomes */
                        } /* valid population size */
			if ((initStrat == BEST_NEU) || (initStrat == MIX)     || 
			    (initStrat == LSV_NEU)  || (initStrat == LSV_BEST)  )
			{ /* initStratPar required */
			   if (initStratPar == NIX_WERT)
			      errLevel = fatal(PROG_NAME, 83, NO_INFO, NO_INFO, INIT_SPAR_TXT, 
						NO_PARAM_VALUE);
			   else
			      if (initStrat == MIX)
			      { /* MIX: value must be a minimal fitness */
				 if ((initStratPar < 0) || (INOTEN_MAX <= initStratPar))
				    errLevel = fatal(PROG_NAME, 84, initStratPar, NO_INFO, 
						      INIT_SPAR_TXT, BAD_PARAM_VALUE);
			      } /* MIX: value must be a minimal fitness */
			      else
			      { /* BEST_NEU, LSV_NEU, or LSV_BEST: number of improved individuals */
				 if ((initStratPar < 1) || (popSize < initStratPar))
				    errLevel = fatal(PROG_NAME, 85, initStratPar, NO_INFO, 
						      INIT_SPAR_TXT, BAD_PARAM_VALUE);
			      } /* BEST_NEU, LSV_NEU, or LSV_BEST: number of improved individuals */
			} /* initStratPar required */
			else
			   initStratPar = 0;        /* required for init.strategy "new" */
			if (minHamXO == NIX_WERT)
			   errLevel = fatal(PROG_NAME, 86, NO_INFO, NO_INFO, 
					     MIN_HAM_XO_TXT, NO_PARAM_VALUE);
			else
			{ /* check parameter limits */
			   if (getDoubleLimits(&r1, &r2, XO_HAMDIST_IDX))
			   { /* there is a data entry */
			      if ((minHamXO < r1) || (r2 < minHamXO))
				 errLevel = fatal(PROG_NAME, 87, minHamXO, NO_INFO, 
						   MIN_HAM_XO_TXT, BAD_PARAM_VALUE);
			      else 
				 if (10.0 < minHamXO)  /* hamming distance limit too large? */
				    meldung (PROG_NAME, minHamXO, NO_INFO, "", HAMMING_DIST_TOO_BIG_MSG);
			   } /* there is a data entry */
			   else
			      errLevel = fatal(PROG_NAME, 88, NO_INFO, NO_INFO, MIN_HAM_XO_TXT, 
						PARAM_HAS_NO_VALUE);
			} /* check parameter limits */
			if (timeLimit < 1)
			   errLevel = fatal(PROG_NAME, 89, timeLimit, NO_INFO, RUNTIME_LIMIT_TXT, 
					     BAD_PARAM_VALUE);
			if (genLimit < 0)
			   errLevel = fatal(PROG_NAME, 90, genLimit, NO_INFO, GEN_LIMIT_TXT, BAD_PARAM_VALUE);
			if (gdvLimit < 0)
			   errLevel = fatal(PROG_NAME, 91, gdvLimit, NO_INFO, GDI_LIMIT_TXT, BAD_PARAM_VALUE);
			if (gakLimit < 0)
			   errLevel = fatal(PROG_NAME, 92, gakLimit, NO_INFO, GAC_LIMIT_TXT, BAD_PARAM_VALUE);
			if (localEvalLimit < 1)
			   errLevel = fatal(PROG_NAME, 93, localEvalLimit, NO_INFO, EVAL_LIMIT_TXT, 
					     BAD_PARAM_VALUE);
			if ((acceptanceRule < ALWAYS) || (LOCAL_LEAST_ES < acceptanceRule))
			   errLevel = fatal(PROG_NAME, 94, acceptanceRule, NO_INFO, ACCEPTANCE_RULE_TXT, 
					     BAD_PARAM_VALUE);
			if ((rankingParam < 1.0) || (2.0 < rankingParam))
			   errLevel = fatal(PROG_NAME, 95, rankingParam, NO_INFO, RANKING_PAR_TXT, 
					     BAD_PARAM_VALUE);
                     } /* population based optimization procedures */
		     if (resChrNumber < 0)  /* also relevant for not population based opt. */
			errLevel = fatal(PROG_NAME, 96, resChrNumber, NO_INFO, RES_ANZ_TXT, 
					  BAD_PARAM_VALUE);
		     else
			if ((resChrNumber == 0) && ((withResChrSimu || withResChrSave)))
			   write_to_log (NO_SAVE_NO_SIM, CLOSE_LOG); 
                     if ((optMethod == GLEAM_ASMA) || (optMethod == GLEAM_AMMA))
                     { /* adaptive MA */
                        if (classNumber == NIX_WERT)
                           errLevel = fatal(PROG_NAME, 97, NO_INFO, NO_INFO, NUMB_OF_FCLASSES_TXT, 
					     NO_PARAM_VALUE);
                        else                    /* check number of adaptation classes: */
                           if ((classNumber != 1) && ((classNumber < 3) || (5 < classNumber)))
                              errLevel = fatal(PROG_NAME, 98, classNumber, NO_INFO, NUMB_OF_FCLASSES_TXT, 
						BAD_PARAM_VALUE);
                        if (levelAdaptSpeed == NIX_WERT)
                           errLevel = fatal(PROG_NAME, 99, NO_INFO, NO_INFO, LEV_ADA_SPEED_TXT,
					     NO_PARAM_VALUE);
                        else       /* check speed index for parameter level adaptation: */
                           if ((levelAdaptSpeed < 0) || (2 < levelAdaptSpeed))
                              errLevel = fatal(PROG_NAME, 100, levelAdaptSpeed, NO_INFO, 
						LEV_ADA_SPEED_TXT, BAD_PARAM_VALUE);
                        if (optMethod == GLEAM_AMMA) 
                        { /* AMMA */
			   if (lhcAdaptSpeed == NIX_WERT)
			      errLevel = fatal(PROG_NAME, 101, NO_INFO, NO_INFO, LSV_ADA_SPEED_TXT,
						NO_PARAM_VALUE);
			   else                /* check speed index for LHC adaptation: */
			      if ((lhcAdaptSpeed < 0) || (2 < lhcAdaptSpeed))
				 errLevel = fatal(PROG_NAME, 102, lhcAdaptSpeed, NO_INFO, 
						   LSV_ADA_SPEED_TXT, BAD_PARAM_VALUE);
			   if (getDoubleLimits(&r1, &r2, OLD_DISTR_FRAC_IDX))
			   { /* there is a data entry */
			      if ((oldDistrPerCent < r1) || (r2 < oldDistrPerCent))
				 errLevel = fatal(PROG_NAME, 103, (INT)(oldDistribFrac*100), 
						   NO_INFO, OLD_FRAC_TXT, BAD_PARAM_VALUE);
			      else
				 oldDistribFrac = oldDistrPerCent / 100.0;
			   } /* there is a data entry */
			   else
			      errLevel = fatal(PROG_NAME, 104, NO_INFO, NO_INFO, OLD_FRAC_TXT, 
						PARAM_HAS_NO_VALUE);
			} /* AMMA */
                     } /* adaptive MA */
		     if ((initStrat == NEU) && ((initStratPar < 0) || (popSize < initStratPar))) 
                     { /* correction of the initialization parameter for "new" */
			sprintf(buffer, INIT_SPAR_CORR_MSG, initStratPar, popSize / 2);
			meldung(PROG_NAME, NO_INFO, NO_INFO, buffer, GENERAL_MESSAGE);
			initStratPar = popSize / 2;
		     } /* correction of the initialization parameter for "new" */
		     if (targetFitness  == NIX_WERT)
			errLevel = fatal(PROG_NAME, 105, NO_INFO, NO_INFO, FITNESS_TXT, NO_PARAM_VALUE);
		     else
			if ((targetFitness < 0.0) || (RNOTEN_MAX < targetFitness))
			   errLevel = fatal(PROG_NAME, 106, l_round(targetFitness), NO_INFO, 
					     FITNESS_TXT, BAD_PARAM_VALUE);
                  } /* valid optimization procedure */
                  else
#ifdef MIT_LSV
                     errLevel = fatal(PROG_NAME, 107, optMethod, NO_INFO, UNKNOWN_TXT, UNKNOWN_OPT_VERF);
#else /* no MIT_LSV: pure GLEAM */
                     errLevel = fatal(PROG_NAME, 107, NO_INFO, NO_INFO, "", ONLY_GLEAM);
#endif /* no MIT_LSV: pure GLEAM */
		  if (newWeights)
	          { /* weighting of some criteria changed */
		     liParam = addiere_noten();
		     if (liParam != INOTEN_MAX) {
			sprintf(buf1, "%g%%", liParam/1000.0);
			errLevel = fatal(PROG_NAME, 108, NO_INFO, NO_INFO, buf1, FITNESS_WEIGHTS_ERROR);
		     }
		     else
		     { /* the weights are ok */
			gewichten();
			write_to_log (WEIGHTS_ADJUSTED_TXT, CLOSE_LOG);
		     } /* the weights are ok */
		  } /* weighting of some criteria changed */
	       } /* TSK-file and options successfully read */
#ifdef MIT_LSV
	       if (errLevel != GLEAM_FATAL)
	       { /* no fatal error(s) so far, process some HyGLEAM parameters */
		  if ((optMethod == GLEAM_ASMA) || (optMethod == GLEAM_AMMA))
                  { /* adaptive MA */
		     set_std_nkl (classNumber);
		     setLevelAdaptSpeed (levelAdaptSpeed);
		     if (optMethod == GLEAM_AMMA)
			setLSVAdaptSpeed (lhcAdaptSpeed, 0);
		  } /* adaptive MA */
		  if ((optMethod == LSV) || (optMethod == GLEAM_SMA) || evoOrMAWithLHCInit)
		  { /* some MA or GLEAM mit LSV-Init, or SMA, or LHC */
		     lsv[lhcID].lsvPar[0] = lhcIterLimit;
		     lsv[lhcID].lsvPar[1] = lhcTerminationParam;
		     if (!parameterise_lsv(lhcID))
			errLevel = GLEAM_FATAL;  /* msg issued by "parameterise_lsv" */
		  } /* LSV oder SMA oder GLEAM mit LSV-Init */
	       } /* no fatal error(s) so far, process some HyGLEAM parameters */
#endif /* MIT_LSV */
	       log_msgs ();                 /* log possible (error) messages to logfile */
#ifdef PAR_POP_VERS
	       if (nodeNumber == NIX_WERT)
		  errLevel = fatal(PROG_NAME, 110, NO_INFO, NO_INFO, "-n", PARAM_HAS_NO_VALUE);
	       else
		  if (nodeNumber < 0)
		     errLevel = fatal(PROG_NAME, 111, nodeNumber, NO_INFO, "-n", BAD_PARAM_VALUE);
#endif /* PAR_POP_VERS */

	       if (errLevel != GLEAM_FATAL)
	       { /* no fatal error(s) so far, start ext.simu and read opt. chr.file */
		  if (mit_ext_simu)
		     if (!ext_simu_hochfahren(FALSE)) /* check/init model and simulator */
			errLevel = fatal(PROG_NAME, 2, NO_INFO, NO_INFO, "", NO_EXT_SIMU);
		  if (errLevel == GLEAM_OK)
                  { /* ok */
		     /* ---------------- load optional chromosome file ---------------- */
		     if (withChrFile)
		     { /* chromosome file provided */
			getStringVal(chrFileSpec, MEM_FILE_SPEC_IDX, GLEAM_OK);
			strcpy(bigBuf, chrFileSpec);
			envVar= getenv(GLEAM_ROOT_ENV);     /* get GLEAM_ROOT contents  */
	                if (!file_exists(chrFileSpec) && (envVar != NULL))  /* check GLEAM_ROOT */
			   sprintf(bigBuf, "%s/%s", envVar, chrFileSpec);   /* and chrFileSpec  */
			if (open_ch_file(bigBuf, 1, CH_READ, GLOBAL_SAVE, initialChrFileMode))
		        { /* chromosome file open */
#ifdef MITSIMULOG
			   mit_simulog     = FALSE;        /* for possible simu-calls   */
#endif                                          
			   record_evo_best = FALSE;        /* while reading chromosomes */
			   if (!restore_chs(TRUE, withDetailedLog))
			      errLevel = fatal(PROG_NAME, 3, NO_INFO, NO_INFO, "", CH_REST_ERR_BAD_RECOV);
			   close_ch_file();
			   if (withDetailedLog)                  /* detailed logging ?  */
			      logTextBuffer(OHNE_TERM_AUSG);     /* messages to logfile */
			   else
			   { /* log only summary line of chromosome loading */
			      if (get_buf(get_line_anz(), buf1))
				 write_to_log(buf1, CLOSE_LOG);
			      reset_text_buf();                  /* clear text buffer   */
			   } /* log only summary line of chromosome loading */
			} /* chromosome file open */
			else {
			   write_to_log(NO_CH_FILE_TXT, CLOSE_LOG);
			   errLevel = GLEAM_FATAL;
			}
		     } /* chromosome file provided */
		     else
			if (withDetailedLog) 
			   write_to_log(NO_CH_FILE_TXT, CLOSE_LOG);
		  } /* ok */
	       } /* no fatal error(s) so far, start ext.simu and read opt. chr.file */
	       if (errLevel == GLEAM_ERROR) 
               { /* only errors and no fatal errors */
		  errLevel = GLEAM_OK;             /* reset errLevel and err_state,     */
		  clear_fbuf();                    /* if no fatal errors occured so far */
	       } /* only errors and no fatal errors */
	       log_msgs();                              /* log possible (error)messages */

               /* ------- main part: job preparation, definition, and execution ------- */
	       if (errLevel == GLEAM_OK)
	       { /* no fatal error(s) so far, execute main part */
		  anz_nachbarn = demeSize;
		  detailed_log = withDetailedLog;              /* for involved packages */
                  if (backupRate == NIX_WERT)
                     backupRate = genLimit + 1;     /* i.e. no backups during evolution */
#ifdef MITSIMULOG
                  mit_simulog = withSimuLog;
#else /* no MITSIMULOG */
                  if (withSimuLog)
                     write_to_log (NO_SIMU_LOG_TXT, CLOSE_LOG);
#endif /* no MITSIMULOG */
#ifdef CEC_MBF
                  cec_accuracy_val  = kriterien[0].xmin;
                  cec_fopt          = d_round (kriterien[0].xmin);
                  kriterien[0].xmin = cec_fopt +  1.0e-8;
                  if (prepare_eval() != GLEAM_OK) 
                     write_to_log (CEC_BEW_DATA_ER, CLOSE_LOG);
#endif /* CEC_MBF */

                  /* ------------------------- job definition: ------------------------ */
                  evo_ws[free_evo_index].opt_strategy   = optMethod;
                  evo_ws[free_evo_index].lsv_strategy   = lhcID;        /* HyGLEAM only */
                  evo_ws[free_evo_index].popel_size     = popSize;
                  evo_ws[free_evo_index].init_strat     = initStrat;
                  evo_ws[free_evo_index].strat_par      = initStratPar;
                  evo_ws[free_evo_index].z_init         = initZufGen; 
                  evo_ws[free_evo_index].ketten_art     = AK;       /* currently unused */
                  evo_ws[free_evo_index].save_rate      = backupRate;
                  evo_ws[free_evo_index].xo_min_ham_dist= minHamXO / 100.0;
                  evo_ws[free_evo_index].survival_rule  = acceptanceRule;
                  evo_ws[free_evo_index].rank_param     = rankingParam;
                  evo_ws[free_evo_index].lsv_all_offspr = withAllImprovement;  /*HyGLEAM*/
                  evo_ws[free_evo_index].lamarck_rate   = lamarckRate / 100.0; /* only  */

                  evo_ws[free_evo_index].anz_erg_ketten = resChrNumber;
                  evo_ws[free_evo_index].ziel_note      = targetFitness;
                  evo_ws[free_evo_index].job_zeit       = timeLimit;
                  evo_ws[free_evo_index].generationen   = genLimit;
                  evo_ws[free_evo_index].gen_no_accept  = gakLimit;
                  evo_ws[free_evo_index].genno_deme_impr= gdvLimit;
                  evo_ws[free_evo_index].maxFEs         = localEvalLimit;
                  free_evo_index++;

                  /* ------------------------- job execution: ------------------------- */
		  startTimerMS();                        /* start timer in microseconds */
#ifdef PAR_POP_VERS
		  if (delayMSec > 0) {
		     sprintf(buffer, MBF_DELAY_TXT, delayMSec); 
		     write_to_log (buffer, CLOSE_LOG);
		  }
		  if (nodeNumber == 0)
		  { /* perform initializer */
		     if (!initializer())
			retCode = ERR_RET_CODE;
		     withDetailedLog = FALSE;      /* no further output to logfile and  */
		     resChrNumber    = 0;          /* no post processing by initializer */
		  } /* perform initializer */
		  else
#endif /* PAR_POP_VERS */
		  { /* execute that job */
#ifdef VISU_MODE
		     record_evo_best = TRUE;
		     doVisuJob();
#else /* no VISU_MODE: standard CLV */
		     record_evo_best = statistik_pro_gen; /*only for data entry per gen.*/
		     do_evo_jobs();
#endif /* no VISU_MODE: standard CLV */
		  } /* execute that job */
		  usedTime = calcElapsedTimeMS();

                  if (withDetailedLog)
                  { /* detailed logging */
                     logTextBuffer(OHNE_TERM_AUSG);     /* possible messages from exec. */
                     sprintf (buffer, RESOURCES_TXT, ges_ak_stat_dat.zeit_sum/60, 
                              ges_ak_stat_dat.zeit_sum % 60,
                              ges_ak_stat_dat.jobli_indivs);
                     write_to_log (buffer, CLOSE_LOG);
                  } /* detailed logging */
                  else
                     reset_text_buf();                      /* delete possible messages */
		  log_msgs ();                /* log possible error messages to logfile */

		  /* -------- performance data based on microsecond accuracy: --------- */
		  if ((usedTime > 0.0000001) && (gen_counter > 0))
		     sprintf (buffer, "#t %.6f %ld  %g %ld  %g  # sec Eval  Eval/sec Eval/Gen  sec/Eval", 
			      usedTime, indiv_ctr, indiv_ctr/usedTime, indiv_ctr/gen_counter, usedTime/indiv_ctr);
		  else { /* time to short and/or no generations */
		     if ((usedTime <= 0.0000001) || (gen_counter <= 0)) {
			if (usedTime > 0.0000001)
			   sprintf (buffer, "#t %.6f %ld  %g   # sec Eval  Eval/sec %s", 
				    usedTime, indiv_ctr, indiv_ctr/usedTime, NO_GENERATIONS);
			else
			   sprintf (buffer, "#t %.6f %ld  %ld   # sec Eval/Gen %s", 
				    usedTime,  indiv_ctr, indiv_ctr/gen_counter, NO_TIME_ELAPSED);
		     }
                     else
			strcpy (buffer, NO_JOB_TIME_GEN);
		  }
		  write_to_log (buffer, CLOSE_LOG);             /* log performance data */

                  /* ------------------- processing of job results: ------------------- */
#ifdef PAR_POP_VERS
		  if (nodeNumber > 0)
		  { /* end of an epoch */            /* no reports about this kind of   */
		     resChrNumber = 0;               /* results during epochs           */
		     save_ep();                      /* save population for next epoch  */
		     withResChrSave = FALSE;   /* do not save single result chromosomes */
		  } /* end of an epoch */
#endif /* PAR_POP_VERS */
		  if (resChrNumber > 0)
		  { /* result chromosomes available */
                     bestChrIdx     = 0;

#ifdef EXT_MATLAB_SIMU  /* vermutlich von Andi geaenderter Code */
                     bestChrFitness = 1000000;
                     //~ CHAIN_ELEM_PTR_TYPE best_results[resChrNumber];
                     int best_chr_fit_class[resChrNumber];  /* addresses (fitness classes and number within these */
                     double best_chr_fit[resChrNumber];	    /* classes) of the best result chromosomes          */
                     int best_chr_number[resChrNumber];
                     
                     for(int res_count = 0; res_count <= last_evo_erg[AK].last_evo_index;  res_count++){
			printf("The %d. individuum is the one with id #%d/%d\n", res_count+1,
			       last_evo_erg[AK].evo_erg_feld[res_count].guete,
			       last_evo_erg[AK].evo_erg_feld[res_count].lfdnr); 
		     }
		     for(int res_count = 0; res_count <= last_evo_erg[AK].last_evo_index;  res_count++){
			double min_distance = FLT_MAX; 
			for (ii = 0;  ii <= last_evo_erg[AK].last_evo_index;  ii++){
			   const double current_distance = bestChrFitness - last_evo_erg[AK].evo_erg_feld[ii].note;
			   if (current_distance <= min_distance && current_distance > 0.0)
                           { /* search for best result chromosome */
			      bestChrIdx = ii;
			      min_distance = current_distance;
			      best_chr_fit[res_count] = last_evo_erg[AK].evo_erg_feld[ii].note;
			   } /* search for best result chromosome */
			}   
			
			bestChrFitness = best_chr_fit[res_count];
			best_chr_fit[res_count] = last_evo_erg[AK].evo_erg_feld[bestChrIdx].note;
			best_chr_fit_class[res_count] = last_evo_erg[AK].evo_erg_feld[bestChrIdx].guete;
			best_chr_number[res_count]    = last_evo_erg[AK].evo_erg_feld[bestChrIdx].lfdnr;
			printf("The %d. best result is ak with id #%d/%d\n", res_count+1, 
			       best_chr_fit_class[res_count], best_chr_number[res_count]);
			sprintf (buf2, ERG_AK_TXT, last_evo_erg[AK].last_evo_index+1, bestChrIdx+1, 
				 best_chr_fit_class[res_count], best_chr_number[res_count]);
		     }
                     for(int res_count = 0;res_count<resChrNumber; res_count++){
			if (get_chain (&bestChrPtr, &best_chr_number[res_count], AK, best_chr_fit_class[res_count], 
				       &best_chr_fit_class[res_count], 
				       &errLevel))   /* only done, if errLevel == GLEAM_OK */
			{ /* successful access to that chromosome */
			   if (withDetailedLog) 
                           { /* detailed logging */
			      sprintf (buffer, NOTE_TXT, buf2, bestChrPtr->u_el.u_xel.u_hdr.fitness_note);
			      write_to_log (buffer, CLOSE_LOG);
			   } /* detailed logging */

			   /* ------------------- re-simulate result: -------------------- */
			   if (withResChrSimu)
                           { /* re-simulate and save results to <logfilename>.res */
			      bestChrPtr->u_el.u_xel.u_hdr.chain_status &= ~SIMULATED; /* clear flag */
			      reset_simu_erg ();                                   /* prepare saving */
			      record_evo_best = TRUE;                              /* simu results   */
			      sprintf (res_fspec, "%s.res", logFName);             /* to .res-file   */
/* alt  #ifdef EXT_MATLAB_SIMU */
			      extractCaseFile = cSwitchValue;     /* for MatPower applications only! */
/* alt  #endif /* EXT_MATLAB_SIMU */
			      simulator (&rParam, SIMU_SHOW, bestChrPtr);
			      if (fabs(rParam - best_chr_fit[res_count]) > 0.01)
				 write_to_log (NOTEN_DIFF_TXT, KEEP_LOG_OPEN);  /*fitn.values differ!*/
			      sprintf (buffer, SIMU_ERG_TXT, rParam);
			      write_to_log (buffer, CLOSE_LOG);
			   } /* re-simulate and save results to <logfilename>.res */
			} /* successful access to that chromosome */
			else 
			{ /* no access to result chromosome */
			   sprintf (buffer, NO_ACCESS_TXT, buf2);
			   write_to_log (buffer, CLOSE_LOG);
			} /* no access to result chromosome */
		     }
#else /* urspruenglicher Code */
                     bestChrFitness = 0.0;
                     for (ii = 0;  ii <= last_evo_erg[AK].last_evo_index;  ii++)
                        if (bestChrFitness < last_evo_erg[AK].evo_erg_feld[ii].note)
                        { /* search for best result chromosome */
                           bestChrIdx     = ii;
                           bestChrFitness = last_evo_erg[AK].evo_erg_feld[ii].note;
                        } /* search for best result chromosome */
                     bestChrFitnClass = last_evo_erg[AK].evo_erg_feld[bestChrIdx].guete;
                     bestChrNumber    = last_evo_erg[AK].evo_erg_feld[bestChrIdx].lfdnr;
                     sprintf (buf2, ERG_AK_TXT, last_evo_erg[AK].last_evo_index+1, bestChrIdx+1, 
                              bestChrFitnClass, bestChrNumber);
                     if (get_chain (&bestChrPtr, &bestChrNumber, AK, bestChrFitnClass, &bestChrFitnClass, 
				    &errLevel))   /* only done, if errLevel == GLEAM_OK */
                     { /* successful access to that chromosome */
                        if (withDetailedLog) 
                        { /* detailed logging */
                           sprintf (buffer, NOTE_TXT, buf2, bestChrPtr->u_el.u_xel.u_hdr.fitness_note);
                           write_to_log (buffer, CLOSE_LOG);
                        } /* detailed logging */

                        /* ------------------- re-simulate result: -------------------- */
                        if (withResChrSimu)
                        { /* re-simulate and save results to <logfilename>.res */
			   bestChrPtr->u_el.u_xel.u_hdr.chain_status &= ~SIMULATED; /* clear flag */
			   reset_simu_erg ();                                   /* prepare saving */
			   record_evo_best = TRUE;                              /* simu results   */
			   sprintf (res_fspec, "%s.res", logFName);             /* to .res-file   */
  #ifdef EXT_MATLAB_SIMU
			   extractCaseFile = cSwitchValue;     /* for MatPower applications only! */
  #endif /* EXT_MATLAB_SIMU */
                           simulator (&rParam, SIMU_SHOW, bestChrPtr);
                           if (fabs(rParam - bestChrFitness) > 0.01)
                              write_to_log (NOTEN_DIFF_TXT, KEEP_LOG_OPEN);  /*fitn.values differ!*/
                           sprintf (buffer, SIMU_ERG_TXT, rParam);
                           write_to_log (buffer, CLOSE_LOG);
                        } /* re-simulate and save results to <logfilename>.res */
                     } /* successful access to that chromosome */
                     else 
                     { /* no access to result chromosome */
                        sprintf (buffer, NO_ACCESS_TXT, buf2);
                        write_to_log (buffer, CLOSE_LOG);
                     } /* no access to result chromosome */
#endif /* urspruenglicher Code */

		     /* ------------- save all result chromosomes to file: ------------ */
		     if (withResChrSave)
                     { /* save to file <logfilename>_res.aks */
			resChrAmount = 0;
			chrMarked    = TRUE;
			for (ii = 0;  ii <= last_evo_erg[AK].last_evo_index;  ii++)
			   if (markiere_ak (last_evo_erg[AK].evo_erg_feld[ii].guete,
					    last_evo_erg[AK].evo_erg_feld[ii].lfdnr) == GLEAM_OK)
		           { /* chromosome exists and was marked */
			      resChrAmount++;
#ifdef EXT_MATLAB_SIMU
			      jj     = last_evo_erg[AK].evo_erg_feld[ii].guete;
			      intVal = last_evo_erg[AK].evo_erg_feld[ii].lfdnr;
			      if (get_chain (&chrPtr, &intVal, AK, jj, &jj, &err_state))
				  chrPtr->u_el.u_xel.u_hdr.chain_status &= ~SIMULATED;
#endif /* EXT_MATLAB_SIMU */
			   } /* chromosome exists and was marked */
			   else
			      chrMarked = FALSE;
			if (!chrMarked && resChrAmount)
			   write_to_log (NICHT_ALLE_TXT, CLOSE_LOG);
			if (resChrAmount)
                        { /* at least one result chromosome was marked */
			   strcpy (buffer, logFName);
			   strcat (buffer, RES_FILE_TXT);      /* <logfilename>_res.aks */
			   if (open_ch_file (buffer, 1, CH_WRITE, SELECTIVE_SAVE, FILE_TXT_IO))
                           { /* file open */
			      save_marked_aks(FALSE);
			      close_ch_file();
			   } /* file open */
			   else
			      meldung (PROG_NAME, NO_INFO, NO_INFO, buffer, FILE_NOT_OPEN);
			} /* at least one result chromosome was marked */
			else
			   write_to_log (NO_SAVE_TXT, CLOSE_LOG);
		     } /* save to file <logfilename>_res.aks */
		  } /* result chromosomes available */
		  if (err_state != GLEAM_OK)
		     errLevel |= err_state;                 /* something went wrong ... */
		  for (ii = 0;  (rand_seed == time (NULL)) && (ii < 10);  ii++)
                     warte (100);          /* avoid the same seeds in case of fast runs */
	       } /* no fatal error(s) so far, execute main part */
	    } /* all initialization files loaded and (ext.) simu ok */
	 } /* exp file exists */
	 else
	    errLevel = fatal(PROG_NAME, 199, NO_INFO, NO_INFO, expFSpec, FILE_NOT_EXISTS);
         log_msgs();                                    /* log possible (error)messages */
         if (mit_ext_simu)
	    terminate_ext_simu ();         /* only by exit handler, unsecure for MatLab */
      } /* package initialization ok */
      else
	 retCode = ERR_RET_CODE;
      if (textBufOK)
      { /* possible messages into logfile */
	 logTextBuffer(OHNE_TERM_AUSG);             
	 write_to_log("", CLOSE_LOG);       
      } /* possible messages into logfile */

      /* ------------------------------- finalization: -------------------------------- */
      if ((errLevel != GLEAM_OK) || (err_state != GLEAM_OK))
	 retCode = ERR_RET_CODE;
#ifdef SCHLUSS_MLDG
      if (withDetailedLog)
	 if (retCode == OK_RET_CODE)
	    printf (OK_FIN_MLDG_TXT);
	 else 
	    printf (BAD_FIN_MLDG_TXT, logfilename);
#endif /* SCHLUSS_MLDG */
   } /* no help option found and at least one file parameter detected */
   return (retCode);
} /* main */



/* ---------------------------------------------------------------------------------------
Copyright (c) 2003 .. 2016  Christian Blume, Wilfried Jakob

Das Urheberrecht (Copyright) fuer GLEAM (General Learning Evolutionary Algorithm 
and Method, frueher: Genetic Learning Algorithm and Method) liegt bei 
Prof. Dr. Christian Blume.

Das Urheberrecht (Copyright) fuer HyGLEAM (Hybrid General purpose Evolutionary 
Algorithm and Method), einem auf GLEAM beruhenden Werk, liegt bei
Dr. Wilfried Jakob, Karlsruher Institut fuer Technologie (KIT).

Diese Datei ist Teil von GLEAM und HyGLEAM. GLEAM und HyGLEAM sind Freie Software: 
Sie koennen sie unter den Bedingungen der GNU Lesser General Public License, wie 
von der Free Software Foundation, Version 3 der Lizenz oder (nach Ihrer Wahl) 
jeder spaeteren veroeffentlichten Version, weiterverbreiten und/oder modifizieren.

GLEAM und HyGLEAM werden in der Hoffnung, dass sie nützlich sein werden, aber
OHNE JEDE GEWAEHRLEISTUNG, bereitgestellt; sogar ohne die implizite Gewaehr-
leistung der MARKTFAEHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die 
GNU Lesser General Public License fuer weitere Details.

In allen Publikationen, welche unter Verwendung von GLEAM, HyGLEAM oder darauf 
beruhender Werke entstanden sind, ist auf die Urheber von GLEAM und gegebenen-
falls auch von HyGLEAM hinzuweisen. Dies kann durch nachstehende Literaturver-
weise geschehen:

  C. Blume: GLEAM - A System for Intuitive Learning. In: Schwefel, H.P., 
  Maenner, R. (eds.): Proc. of PPSN I, LNCS 496, Springer, Berlin, S.48-54, 1990.

  C. Blume, W. Jakob:  GLEAM - ein Evolutionärer Algorithmus und seine 
  Anwendungen. KIT Scientific Publishing, Schriftenreihe des Instituts fuer 
  Angewandte Informatik / Automatisierungstechnik (AIA), Band 32, 
  ISBN 978-3-86644-436-2, 2009. 

  W. Jakob: A general cost-benefit-based adaptation framework for multimeme 
  algorithms. Memetic Computing, 2(2010), S.201-218. 

------------------------------------------------------------------------------------------
Copyright (c) 2003 .. 2016  Christian Blume, Wilfried Jakob

The copyright of GLEAM (General Learning Evolutionary Algorithm and Method, 
former: Genetic Learning Algorithm and Method) is owned by Prof. Dr. Christian 
Blume.

The copyright of HyGLEAM (Hybrid General purpose Evolutionary Algorithm and 
Method),  a software based on GLEAM, is owned by Dr. Wilfried Jakob, Karlsruhe 
Institute of Technology (KIT).

This file is part of GLEAM and HyGLEAM. GLEAM and HyGLEAM are free software: 
you can redistribute them and/or modify them under the terms of the GNU Lesser 
General Public License as published by the Free Software Foundation, either 
version 3 of the License, or (at your option) any later version.

GLEAM and HyGLEAM are distributed in the hope that they will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License 
for more details.

In all publications which refer to GLEAM, HyGLEAM or works based on it or to 
results obtained by the usage of GLEAM, HyGLEAM or works based on it, a
reference to the Author(s) of GLEAM and HyGLEAM, if applicable, shall be 
includeded. This can be done by the following literature references:

  C. Blume: GLEAM - A System for Intuitive Learning. In: Schwefel, H.P., 
  Maenner, R. (eds.): Proc. of PPSN I, LNCS 496, Springer, Berlin, S.48-54, 1990.

  W. Jakob:  A general cost-benefit-based adaptation framework for multimeme 
  algorithms. Memetic Computing, 2(2010), S.201-218. 

-----------------------------------------------------------------------------------------

GNU LESSER GENERAL PUBLIC LICENSE

Version 3, 29 June 2007

Copyright © 2007 Free Software Foundation, Inc. <http://fsf.org/>

Everyone is permitted to copy and distribute verbatim copies of this license document, 
but changing it is not allowed.

This version of the GNU Lesser General Public License incorporates the terms and conditions 
of version 3 of the GNU General Public License, supplemented by the additional permissions 
listed below.


0. Additional Definitions.

As used herein, "this License" refers to version 3 of the GNU Lesser General Public License, 
and the "GNU GPL" refers to version 3 of the GNU General Public License.

"The Library" refers to a covered work governed by this License, other than an Application or 
a Combined Work as defined below.

An "Application" is any work that makes use of an interface provided by the Library, but 
which is not otherwise based on the Library. Defining a subclass of a class defined by the 
Library is deemed a mode of using an interface provided by the Library.

A "Combined Work" is a work produced by combining or linking an Application with the Library. 
The particular version of the Library with which the Combined Work was made is also called 
the "Linked Version".

The "Minimal Corresponding Source" for a Combined Work means the Corresponding Source for 
the Combined Work, excluding any source code for portions of the Combined Work that, 
considered in isolation, are based on the Application, and not on the Linked Version.

The "Corresponding Application Code" for a Combined Work means the object code and/or source 
code for the Application, including any data and utility programs needed for reproducing the 
Combined Work from the Application, but excluding the System Libraries of the Combined Work.


1. Exception to Section 3 of the GNU GPL.

You may convey a covered work under sections 3 and 4 of this License without being bound by 
section 3 of the GNU GPL.


2. Conveying Modified Versions.

If you modify a copy of the Library, and, in your modifications, a facility refers to a 
function or data to be supplied by an Application that uses the facility (other than as an 
argument passed when the facility is invoked), then you may convey a copy of the modified 
version:

    a) under this License, provided that you make a good faith effort to ensure that, in 
       the event an Application does not supply the function or data, the facility still 
       operates, and performs whatever part of its purpose remains meaningful, or
    b) under the GNU GPL, with none of the additional permissions of this License applicable 
       to that copy.


3. Object Code Incorporating Material from Library Header Files.

The object code form of an Application may incorporate material from a header file that is 
part of the Library. You may convey such object code under terms of your choice, provided 
that, if the incorporated material is not limited to numerical parameters, data structure 
layouts and accessors, or small macros, inline functions and templates (ten or fewer lines 
in length), you do both of the following:

    a) Give prominent notice with each copy of the object code that the Library is used in 
       it and that the Library and its use are covered by this License.
    b) Accompany the object code with a copy of the GNU GPL and this license document.


4. Combined Works.

You may convey a Combined Work under terms of your choice that, taken together, effectively do 
not restrict modification of the portions of the Library contained in the Combined Work and 
reverse engineering for debugging such modifications, if you also do each of the following:

    a) Give prominent notice with each copy of the Combined Work that the Library is used in 
       it and that the Library and its use are covered by this License.
    b) Accompany the Combined Work with a copy of the GNU GPL and this license document.
    c) For a Combined Work that displays copyright notices during execution, include the 
       copyright notice for the Library among these notices, as well as a reference directing 
       the user to the copies of the GNU GPL and this license document.
    d) Do one of the following:
        0) Convey the Minimal Corresponding Source under the terms of this License, and the 
           Corresponding Application Code in a form suitable for, and under terms that permit, 
           the user to recombine or relink the Application with a modified version of the Linked 
           Version to produce a modified Combined Work, in the manner specified by section 6 of 
           the GNU GPL for conveying Corresponding Source.
        1) Use a suitable shared library mechanism for linking with the Library. A suitable 
           mechanism is one that (a) uses at run time a copy of the Library already present 
           on the user's computer system, and (b) will operate properly with a modified 
           version of the Library that is interface-compatible with the Linked Version.
    e) Provide Installation Information, but only if you would otherwise be required to provide 
       such information under section 6 of the GNU GPL, and only to the extent that such 
       information is necessary to install and execute a modified version of the Combined Work 
       produced by recombining or relinking the Application with a modified version of the 
       Linked Version. (If you use option 4d0, the Installation Information must accompany 
       the Minimal Corresponding Source and Corresponding Application Code. If you use option 
       4d1, you must provide the Installation Information in the manner specified by section 6 
       of the GNU GPL for conveying Corresponding Source.)


5. Combined Libraries.

You may place library facilities that are a work based on the Library side by side in a single 
library together with other library facilities that are not Applications and are not covered 
by this License, and convey such a combined library under terms of your choice, if you do both 
of the following:

    a) Accompany the combined library with a copy of the same work based on the Library, 
       uncombined with any other library facilities, conveyed under the terms of this License.
    b) Give prominent notice with the combined library that part of it is a work based on 
       the Library, and explaining where to find the accompanying uncombined form of the same 
       work.


6. Revised Versions of the GNU Lesser General Public License.

The Free Software Foundation may publish revised and/or new versions of the GNU Lesser 
General Public License from time to time. Such new versions will be similar in spirit to 
the present version, but may differ in detail to address new problems or concerns.

Each version is given a distinguishing version number. If the Library as you received it 
specifies that a certain numbered version of the GNU Lesser General Public License "or any 
later version" applies to it, you have the option of following the terms and conditions 
either of that published version or of any later version published by the Free Software 
Foundation. If the Library as you received it does not specify a version number of the 
GNU Lesser General Public License, you may choose any version of the GNU Lesser General 
Public License ever published by the Free Software Foundation.

If the Library as you received it specifies that a proxy can decide whether future 
versions of the GNU Lesser General Public License shall apply, that proxy's public 
statement of acceptance of any version is permanent authorization for you to choose 
that version for the Library.

--------------------------------------------------------------------------------------- */
