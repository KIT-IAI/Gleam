/*****************************************************************************************
HyGLEAM                                                                             V1.0.3

                                      H y G L E A M
            Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                            Package: EVOlution
                             Module: EVOlutions-ANZeiGen und -statistik
 
Package: evo                         File: evo_anzg.c
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------

This module provides functions for                | 1: actJobHdrBuf1/2, 
- collection of statistical data of opt.jobs      |    actJobDataBuf, actJobDataSaveBuf1/2
  and opt.job list execution                      | 2: actJobLimitsBuf
- displaying general parameters or defaults       +---------------------------------------
  for opt.jobs
- displaying opt.jobs in the job list:       basic + limits (+ par)     text buffer of fbhm
- displaying an executed opt.job on screen:  basics (+ par)             repeated on interrupt  (1)
                                             limits                     repeated every 10 gen. (2)
                                             counters                   per generation
                                             results                    limits + final counters
- writing an executed opt.job to logfile:    basics (+par) +add.param.  once on execution start
  (the logfile must document all relevant    readable result data       once on execution end
   parameters of an optimization job)        result data for further
                                             processing                 once on execution end

Data of the different types of optimization jobs:
-------------------------------------------------
LI: LHC based Initialization of the start population

OptJobType:  Job List:                Execution:                 Logfile: 
GLEAM:       <basic><limits>          <basic><evoBasic>          <exec>
GLEAM + LI:  <basic><limits>          <basic><evoBasic><actLHC>  <basic><evoBasic><iLHC><lhcPar>

LSV:         <basic><lhcLim><lhcPar>  <basic><lhcPar>            <exec>

SMA (+ LI):  <basic><limits><actLHC>  <basic><evoBasic><actLHC>  <exec><lhcPar><maBasic>

ASMA:        <basic><limits><actLHC>  <basic><evoBasic><actLHC>  <exec><maBasic><ama>
ASMA + LI:   <basic><limits><actLHC>  <basic><evoBasic><actLHC>  <exec><maBasic><ama><iLHC><lhcPar>
AMMA:        <basic><limits><actLHC>  <basic><evoBasic><actLHC>  <exec><maBasic><ama>
AMMA + LI:   <basic><limits><actLHC>  <basic><evoBasic><actLHC>  <exec><maBasic><ama><iLHC><lhcPar>


<basic>:                                   <limits>:                              <actLHC>: 
---Job-- --Sizes- -------Init------- Res   -----------Target Values------------    active 
 No OptP Pop/Deme  RNG     Pop  Spar Chr   Fitness hh:mm   Gen  GDI GAc MaxEval      LHCs 
xxx xxxx xxxxx/xx xxxx xxxxxxx xxxxx xxx    xxxxxx xx:xx xxxxx xxxx xxx xxxxxxx   abcdefg

<lhcPar>:                                  <ama>:                                  <iLHC>:
   Max- Rosenbr ...                        Fitness AdaptationSpeed Learn            | Init-: 
   Iter   Limit ...                        Classes  Param/LHC-Sel.  Rate            |  LSV :
 xxxxxx x.xe-xx                                  x xxxxxxx/xxxxxxx xx.xx              xxxx

<evoBasic>:                                <maBasic>:                 <exec>:
HamDist Rank AcceptanceRule                UpdRate  Offspring         data displayed as header 
    [%]  Par  for Offspring                    [%]  Selection         when opt. job is executed.
 xx.xxx x.xx xxxxxx14xxxxxx                    xxx xxxxxxxxxx


Job List:
The two head line of a job list are determined by the type of the first job. The following 
table illustrates this and shows the displayed data of subsequent jobs other than the first 
one. In case of LHC jobs the LHC of the first job determines the header for LHC parameters,
so that the parameters of subsequent LHC jobs with different LHCs cannot be displayed. In 
all cases of left out data this is indicated by "...".

1st job type:   |      GLEAM          |           LHC           |         any MA          |
----------------+---------------------+-------------------------+-------------------------+
general header: | <basic><limits>     | <basic><lhcLim><lhcPar> | <basic><limits><actLHC> |
subsequent job: |                     |                         |                         |
  GLEAM         | <basic><limits>     | <basic> ...             | <basic><limits>         |
  LSV           | <basic><lhcLim> ... | <basic><lhcLim><lhcPar> | <basic><lhcLim> ...     |
  any MA        | <basic><limits> ... | <basic> ...             | <basic><limits><actLHC> |


The limits and current values displayed during the execution of an optimization job and at 
its end are as follows. If the program parameter (cf. TSK file) "with recording of 
statistical data" is not set the "Job List Results" section on the right is ommited.

-----------------------------------------------+
       Fitness  Runtime   Gen  GDI GAc MaxEval |
 Limit: 100000 00:10:00 12345 1234 123 1234567 |
-----------------------------------------------+
ActVal:                                        |

-----------------------------------------------+----------------------------------
       Fitness  Runtime   Gen  GDI GAc MaxEval |         Job List Results     
 Limit: 100000 00:10:00 12345 1234 123 1234567 | Number BestChrom.: Fitness Length
ActVal:                                        |   1234              123456  12345
-----------------------------------------------+----------------------------------


For every job a line starting with "# " is written to the logfile, which contains result 
and performance data of the finished job for further statistical processing and analysis.
The data for evolution based and LHC based optimization jobs have the same basic entries, 
with some data fields of LHC jobs remain unused and may contain 0 or a default value:

Data item:                                                                        LHC job:
- acceptance rule for offspring (0=ALWAYS, 1=LOCAL_LEAST, 2=BETTER_PARENT,        unused
       3=ALWAYS_ES, 4=LOCAL_LEAST_ES)
- population size                                                                   used
- last amount of generations without deme improvement (GDI)                       unused 0
- last amount of generations without offspring acceptance per deme (GAc)          unused 0
- used time in seconds                                                              used
- processed generations                                                           unused 0
- fitness of the best chromosome                                                    used
- length of the best chromosome                                                     used
- number of fitness evaluations / assessed chromosomes                              used

In case of ASMA- and AMMA-jobs the line is supplemented by data regarding the adaptation 
as described in the documentation of function "get_adi_statistik_string" of module 
"adapt_di.c".

If Matlab is used as an external simulator (EXT_MATLAB_SIMU is set), an additional line 
beginning with "##" is written to the logfile containing the number of non-converged 
Matlab runs per optimization job.


The functions of the package-export in detail:
==============================================

void prep_evo_param_anzg (void);
          The display of actual parameters and defaults of (memetic) evolution, adaptation 
          and other optimization procedures is prepared by writing the corresponding texts 
          to the text buffer of the "fbhm"-package.
          This function belongs to the package export in GUI_MODE, otherwise it is 
          package-local.

void prepJobListDisplay (void);
          The display of the job list is prepared by writing the header lines corresponding 
          to the first optimization job to the text buffer of the "fbhm"-package. All jobs 
          are written to the buffer as described above.
          This function is implemented, if GLEAM_USR is set (i.e. not for commandline vari-
          ants). If implemented, it belongs to the package export in GUI_MODE, otherwise it 
          is package-local.

void showActJobDataOnProcessingStart (void);
          The complete data of the actual optimization job is written to the logfile as 
          specified above. If GLEAM_USR is set, a subset of the data as specified above 
          is also stored in the buffers "actJobHdrBuf1/2" for displaying it, if an 
          interruptioned job is continued. 
          In TEXT_MODE the buffers are written to the terminal. In GUI_MODE the buffers 
          are used by the GUI.
          The target limits of the actual optimization job are stored in "actJobLimitsBuf"
          using ACTIVE_JOB_HDR2_TXT. 

void showActJobDataOnContinuation (void);
          The data subset of the actual optimization job is written to the termianl based 
          on the buffers "actJobHdrBuf1/2".

void showGenResults (void); 
          Displays the data of the counters of termination limits at the end of a gene-
          ration, whereby ACTIVE_JOB_VAL is used. The header is displayed every 10 gene-
          rations by a call of showOptRunHdr(SHOW_RUNNING).
          Package-Export nur bei gesetztem GUI_MODE, sonst package-lokal.

void optJobEndStatistics (void);
          Records some data for the overall statistics and writes job data for further 
          statistical processing as described above into the logfile.


The package-global functions in detail:
=======================================

void showOptRunHdr (INT  showMode);
          Displays the header of the results per generation when processing an optimization 
          job. The header limit data string is taken from "actJobLimitsBuf". This is the 
          standard behaviour and corresponds to a "showMode" value of SHOW_RUNNING.
          If "showMode" equals to SHOW_START, the results from the initialiszation of the 
          population is shown additionally.
          If "showMode" is SHOW_END, the updated overall job list results are shown in 
          addition using ACT_JOB_END_VAL and ACT_JOB_END_HDR1/2_TXT. 
          If "mit_statistik" is set, the lines displayed for the "showMode" values of 
          SHOW_START and SHOW_END are also written to the logfile. 

void jobLiLog (void);
          If adaptive MA-jobs (ASMA or AMMA) have been processed, cumulated data of the 
          adaptation are written to the logfile for further processing.
          Writes the overall statistics of the just finished job list and the the cumulated
          statistics of all processed job lists since program start to the logfile in a 
          readable form.

BOOL init_evo_anzg (void);
          Modul-Initialisierung.

------------------------------------------------------------------------------------------
New implementation on the basis of the old module V2.2.2 from 06.04.2017. From this were 
taken over:
- display of the parameters of optimization jobs: "prep_evo_param_anzg", "prep_adi_param_anzg"
- cumulated data of job list execution abd adaptation: "jobLiLog", former "evo_jobs_log"

------------------------------------------------------------------------------------------
Change log since version 1.0.0:
 1. Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".                      WJ 30.06.17
 2. Elimination of reversed output of GDI and GAc counters and limits.         WJ 13.10.17
 3. "logBuffer" wird Package global.                                           WJ 14.11.17
 4. Puffer in showActJobDataOnProcessingStart vergroessert.                    WJ 09.04.18
 5. EXT_MATLAB_SIMU: optJobEndStatistics() shows the amount of unvonverged 
    matalab runs in an extra line starting wit "## ".                          WJ 27.04.18
 6. Seletion of no longer suported BAC chromosomes.                            WJ 18.06.18
 7. The "# "-line now contains the last values of GDI and GAc counters instead 
    of the selected procedure for the creation of the initial population and
    its parameter.                                                             WJ 28.06.18
 8. Avoiding buffer overflow of static strings: prepOptJobLimits(),
    prep_evo_param_anzg(), showActJobDataOnProcessingStart(), prepOptJobAma(), 
    and prepOptJobMABasic().
    Include of "aufg.h" because of transfer of "mit_statistik". The "Job List 
    Results" section" at the end of an optimization job is omitted if the 
    program parameter "with recording of statistical data" is not set.         WJ 11.08.20
------------------------------------------------------------------------------------------
Status: o.k.                          Autor: W.Jakob                     Datum: 11.08.2020
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "aufg.h"
#include "appl.h"
#include "simu.h"
#include "evo.h"
#include "evoInt.h"

#include GLOBAL_TEXTS 
#include EVO_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif /* GLO_CHECKS */




/* =============================== Exportierte Variable: ============================== */
BOOL  initFileReloaded;     /* TRUE, if at minimum one initialization file was reloaded */
BOOL  detailed_log;         /* TRUE: mit detailierten Logfile-Eintraegen, Engine-Export */


/* ============================= Package-globale Variable: ============================ */
char *logBuffer;            /* big buffer for logfile output                            */



/* ================================= Lokale Konstante: ================================ */
#define SHORT_FORMAT     TRUE               /* for "time2String"                        */
#define LONG_FORMAT      FALSE              /* for "time2String"                        */
#define GLEAM_JOB_HDR    0                  /* job types for job list display header    */
#define LHC_JOB_HDR      1                  /* job types for job list display header    */
#define MA_JOB_HDR       2                  /* job types for job list display header    */



/* ================================= Lokale Variablen: ================================ */
static char *actJobHdrBuf1,       /* 1st header line } for data of actual processed job */
            *actJobHdrBuf2,       /* 2nd header line }   - when started or resumed      */
            *actJobDataBuf,       /* job data line   }   - when started                 */
            *actJobDataSaveBuf1,  /* job data line   }   - when resumed, basic part     */
            *actJobDataSaveBuf2,  /* job data line   }   - when resumed, remaining part */
            *actJobLimitsBuf,     /* 2nd header line for limit values of actual job     */
            *longSeperatorLine;   /* string of 190 "-" characters                       */
          


 
/* ================================= Lokale Routinen: ================================= */
#ifdef MIT_LSV
  static void   prep_adi_param_anzg  (INT   znr,
                                      BOOL  mitLsvSel);
#endif /* MIT_LSV */

static void time2String              (char *timeBuf,
			              LONG  timeInSec,
                                      BOOL  shortFormat);

static void prepOptJobProcessingData (char *resultBuf);

static void prepOptJobBasics         (char *resultBuf,
                                      INT   jobIdx);

static void prepOptJobEvoBasics      (char *resultBuf,
                                      INT   jobIdx);

static void prepOptJobActLHC         (char *resultBuf,
                                      INT   jobIdx);

static void prepOptJobLHCPar         (char *resBuf1, 
                                      char *resBuf2, 
                                      char *resBuf3, 
                                      INT   jobIdx);

static void prepOptJobInitLHC        (char *resBuf1, 
                                      char *resBuf2, 
                                      char *resBuf3, 
                                      INT   jobIdx);

static void prepOptJobMABasic        (char *resBuf1, 
                                      char *resBuf2, 
                                      char *resBuf3, 
                                      INT   jobIdx);

static void prepOptJobAma            (char *resBuf1, 
                                      char *resBuf2, 
                                      char *resBuf3);

#ifdef GLEAM_USR
  static void prepOptJobLimits       (char *resultBuf,
                                      INT   jobIdx);

  static void prepOptJobLhcLim       (char *resultBuf,
                                      INT   jobIdx);
#endif /* GLEAM_USR */

static void prepActJobLimitsBuf      (void);




/* =============================== prep_evo_param_anzg ================================ */
void prep_evo_param_anzg (void)
{
   STRING_TYPE  buffer;
   STR_TYPE     buf1;
   BUF_TYPE     sbuf;
   INT          idx, znr, ii, jj,
                localOptStrat,
                localLHCStrat;
   BOOL         gleamMitLSVInit;

   /* prep_evo_param_anzg */
#ifdef GLEAM_USR
   localOptStrat = def_opt_strat;         /* default optimization procedure   */
   localLHCStrat = def_lsv_strat;         /* default local hill climber (LHC) */
#else /* no GLEAM_USR: command line version */
   localOptStrat = act_opt_strategy;      /* actual optimization procedure    */
   localLHCStrat = lsv_strat;             /* actual local hill climber (LHC)  */
#endif /* no GLEAM_USR: command line version */
   if (localOptStrat < LSV)
   { /* GLEAM, SMA, ASMA oder AMMA  */
      switch (localOptStrat) {
        case GLEAM:      sprintf (buf1, EVOPAR_HDR1, OPTSTRAT_EVO);  break;
        case GLEAM_SMA:  sprintf (buf1, EVOPAR_HDR1, OPTSTRAT_SMA);  break;
        case GLEAM_ASMA: sprintf (buf1, EVOPAR_HDR1, OPTSTRAT_ASMA); break;
        case GLEAM_AMMA: sprintf (buf1, EVOPAR_HDR1, OPTSTRAT_AMMA); break;
        default:    sprintf (buf1, EVOPAR_HDR1, "????");             break;
      } /* case */
      write_buf (1, buf1);
      strcpy (buffer, EVOPAR_HDR2);
      jj = strlen (buf1) - strlen (EVOPAR_HDR2);
      for (ii = 0;  ii < jj;  ii++)
         strcat (buffer, "=");
      write_buf (2, buffer);
      sprintf (buffer, EVOPAR_DEMESIZE, anz_nachbarn);
      znr = 4;
      write_buf (znr++, buffer);
      switch (default_survival_rule) {
         case LOCAL_LEAST_ES: strcpy (buf1, EVOPAR_LL_ES);    break;
         case LOCAL_LEAST   : strcpy (buf1, EVOPAR_LL);       break;
         case ALWAYS        : strcpy (buf1, EVOPAR_AA);       break;
         case ALWAYS_ES     : strcpy (buf1, EVOPAR_AA_ES);    break;
         case BETTER_PARENT : strcpy (buf1, EVOPAR_BP);       break;
         default:             strcpy (buf1, "???");
      } /* case */
      strcpy (buffer, EVOPAR_SURVRULE);
      strcat (buffer, buf1);
      write_buf (znr++, buffer);
      sprintf (buffer, EVOPAR_RANK, def_max_fit);
      write_buf (znr++, buffer);
      sprintf (buffer, EVOPAR_XO_HAM, def_xo_min_ham_dist * 100);
      write_buf (znr++, buffer);
      if (mit_gen_rep_par) 
      { /* mit Genetic-Repair-Steuerparameter */
	 strcpy (buf1, EVOPAR_GENO_KL_TXT);
	 if (mitOptionPhaenoRep) {
	     strcat (buf1, ", ");
	     strcat (buf1, EVOPAR_PHAENOKLTXT);
	 }
      } /* mit Genetic-Repair-Steuerparameter */
      else
	 if (mitOptionPhaenoRep)
	    strcpy (sbuf, EVOPAR_PHAENOKLTXT);
	 else
	    strcpy (sbuf, "-");
      sprintf (buffer, "%s%s", EVOPAR_REPAIR, sbuf);
      write_buf (znr++, buffer);
      if (mit_gen_rep_par) 
      { /* mit Genetic-Repair-Steuerparameter */
         sprintf (buffer, gen_rep_par_anzg, gen_rep_par * 100.0);
         write_buf (znr++, buffer);
      } /* mit Genetic-Repair-Steuerparameter */
      if (mitOptionPhaenoRep) 
      { /* mit Phaeno-Repair-Steuerparameter */
         strcpy (buffer, EVOPAR_PHAENO);
         if (mitPhaenoRepair)
            strcat (buffer, JA_FORMATIERT_TXT);
         else
            strcat (buffer, NO_FORMATIERT_TXT);
         write_buf (znr++, buffer);
      } /* mit Phaeno-Repair-Steuerparameter */
   } /* GLEAM, SMA, ASMA oder AMMA */
#ifdef MIT_LSV
   else
   { /* Reine LSV-Optimierung */
      if ((0 <= localLHCStrat) && (localLHCStrat < LSV_ANZ))
         sprintf (buf1, OPTPAR_LSV_TXT, lsv[localLHCStrat].lsvMenueName);
      else
         if (localLHCStrat == KEIN_LSV)
            sprintf (buf1, OPTPAR_LSV_TXT, "-");
         else
            sprintf (buf1, OPTPAR_LSV_TXT, "???");
      write_buf (2, OPTPAR_HDR1);
      write_buf (3, OPTPAR_HDR2);
      write_buf (4, buf1);
      znr = 5;
   } /* Reine LSV-Optimierung */

   if (lsv_anz > 0)
   { /* present gene model includes LHCs */
      if (localOptStrat != GLEAM) { /* Kein reines GLEAM */
	 sprintf (buffer, EVOPAR_LAMARCK, def_lamarck_rate * 100.0);
	 write_buf (znr++, buffer);
      } /* Kein reines GLEAM */
  #ifdef GLEAM_USR 
      if (localOptStrat != GLEAM_AMMA)
      { /* GLEAM (wg.VorInit), SMA, ASMA oder LSV: Default-LSV anzeigen */
	 if (localLHCStrat == KEIN_LSV)
	    strcpy (buf1, "-");
	 else
	    strcpy (buf1, lsv[localLHCStrat].lsvName);
	 if (localOptStrat == GLEAM)
	    sprintf (buffer, AKT_INIT_LSV_TXT, buf1);
	 else
	    sprintf (buffer, AKT_LSV_TXT, buf1);
	 write_buf (znr++, buffer);
      } /* GLEAM (wg.VorInit), SMA, ASMA oder LSV: Default-LSV anzeigen */
      if (((localOptStrat == GLEAM) || (localOptStrat == GLEAM_SMA) || 
	   (localOptStrat == LSV)) && (localLHCStrat != KEIN_LSV))
      { /* GLEAM (wg.VorInit), SMA oder LSV: feste Steuerparameter anzeigen */
	 znr++;
	 write_buf (znr++, LSV_LIST_TXT);
	 for (ii = 0;  ii < lsv_anz;  ii++)
         { /* Anzeige aller LSVs samt Steuer-Parameter  */
	    idx = aktive_lsv[ii];
	    if (lsv[idx].lsvParAnz > 0)
            { /* LSV mit Steuerparameter */
	       sprintf (buffer, STEU_PARAM_TXT, lsv[idx].lsvName);
	       write_buf (znr++, buffer);
	       for (jj = 0;  jj < lsv[idx].lsvParAnz;  jj++)
               { /* Anzeige aller Steuer-Parameter eines LSVs */
		  sprintf (buffer, "    %-28s: %g", lsv[idx].lsvParName[jj], 
			   lsv[idx].lsvPar[jj]);
		  write_buf (znr++, buffer);
	       } /* Anzeige aller Steuer-Parameter eines LSVs */
	    } /* LSV mit Steuerparameter */
	    else
            { /* LSV ohne Steuerparameter */
	       sprintf (buffer, NO_STEU_PARAM_TXT, lsv[idx].lsvName);
	       write_buf (znr++, buffer);
	    } /* LSV ohne Steuerparameter */
	 } /* Anzeige aller LSVs samt Steuer-Parameter */
      } /* GLEAM (wg.VorInit), SMA oder LSV: feste Steuerparameter anzeigen */
      if ((localOptStrat == GLEAM_ASMA) || (localOptStrat == GLEAM_AMMA))
	 prep_adi_param_anzg (znr, localOptStrat == GLEAM_AMMA);
  #endif /* GLEAM_USR */
   } /* present gene model includes LHCs */
#endif /* MIT_LSV */
} /* prep_evo_param_anzg */



#ifdef GLEAM_USR
/* ================================ prepJobListDisplay ================================ */
void prepJobListDisplay (void)
{
   STRING_TYPE  hdrLine1, hdrLine2, /* the two header lines, later auxiliary buffer     */
                dataLine;           /* used for the seperator line and for job data     */
   INT          headerJobType,      /* job type of the first job determining the header */
                headerLHC,          /* LHC of the first LHC job determining the header  */
                lineCtr,            /* line counter                                     */
                actJobOptProc,      /* optimization procedure of actual job             */
                jobIdx;             /* optimization job index                           */

   /* prepJobListDisplay */
   reset_text_buf ();
   write_buf(1, JOBLI_HDR_TXT);
   headerLHC = NIX_IDX;
   if (first_evo_item == free_evo_index)
      write_buf(3, JOBLI_LEER_TXT);
   else
   { /* job list not empty */
      /* -------------------- compilation of the two header lines: -------------------- */
      strcpy(hdrLine1, JOBLI_BASIC_HDR1_TXT);                                  /* basic */
      strcpy(hdrLine2, JOBLI_BASIC_HDR2_TXT);                                  /* basic */
      if (evo_ws[first_evo_item].opt_strategy == GLEAM)
      { /* GLEAM job */
	 headerJobType = GLEAM_JOB_HDR;
	 strcat(hdrLine1, JOBLI_LIMITS_HDR1_TXT);                             /* limits */
	 strcat(hdrLine2, JOBLI_LIMITS_HDR2_TXT);                             /* limits */
      } /* GLEAM job */
      else
	 if (evo_ws[first_evo_item].opt_strategy == LSV) 
         { /* LHC_JOB */
	    headerJobType = LHC_JOB_HDR;
	    headerLHC = evo_ws[first_evo_item].lsv_strategy; 
	    strcat(hdrLine1, JOBLI_LHC_LIM_HDR1_TXT);                         /* lhcLim */
	    strcat(hdrLine2, JOBLI_LHC_LIM_HDR2_TXT);                         /* lhcLim */
	    strcpy(dataLine, "");
	    prepOptJobLHCPar(hdrLine1, hdrLine2, dataLine, first_evo_item);   /* lhcPar */
	 } /* LHC_JOB */
	 else
	 { /* any MA: SMA, ASMA, or AMMA */
	    headerJobType = MA_JOB_HDR;
	    strcat(hdrLine1, JOBLI_LIMITS_HDR1_TXT);                          /* limits */
	    strcat(hdrLine2, JOBLI_LIMITS_HDR2_TXT);                          /* limits */
	    strcat(hdrLine1, JOBLI_ACT_LHC_HDR1_TXT);                         /* actLHC */
	    strcat(hdrLine2, JOBLI_ACT_LHC_HDR2_TXT);                         /* actLHC */
	 } /* any MA: SMA, ASMA, or AMMA */
      strncpy(dataLine, longSeperatorLine, strlen(hdrLine2));
      dataLine[strlen(hdrLine2)] = '\0';
      write_buf(3, hdrLine1);
      write_buf(4, hdrLine2);
      write_buf(5, dataLine);
      strcpy(hdrLine1, "");                                  /* reset these buffers, as */
      strcpy(hdrLine2, "");                                  /* they may be used later  */
      lineCtr = 6;

      /* -------------------- compilation of a data line per job: --------------------- */
      for (jobIdx = first_evo_item;  jobIdx < free_evo_index;  jobIdx++)
      { /* compile data of one job */
	 prepOptJobBasics(dataLine, jobIdx);                                   /* basic */
	 if (evo_ws[jobIdx].deaktiviert)
	    strcat (dataLine, JOB_DEAKTIV_TXT);                      /* job deactivated */
	 else
	 { /* valid job */
	    actJobOptProc = evo_ws[jobIdx].opt_strategy;
	    switch (headerJobType) {
	    case GLEAM_JOB_HDR: /* --------------- GLEAM job header: ------------------ */
	       if (actJobOptProc == LSV)
               { /* LHC job */
		  prepOptJobLhcLim(dataLine, jobIdx);                         /* lhcLim */
#ifdef MIT_LSV
		  if (lsv[evo_ws[jobIdx].lsv_strategy].lsvParAnz > 0)
		     strcat(dataLine, " ...");
#endif /* MIT_LSV */
	       } /* LHC job */
	       else
   	       { /* GLEAM or any MA */
		  prepOptJobLimits(dataLine, jobIdx);                         /* limits */
		  if (actJobOptProc != GLEAM)
		     strcat(dataLine, " ...");                                /* any MA */
	       } /* GLEAM or any MA */
	       break; /* GLEAM job header */

	    case LHC_JOB_HDR: /* ------------------ LHC job header: ------------------- */
	       if (actJobOptProc == LSV)
               { /* LHC job */
		  prepOptJobLhcLim(dataLine, jobIdx);                         /* lhcLim */
#ifdef MIT_LSV
  #ifdef MIT_ROCO
		  if ((headerLHC == evo_ws[jobIdx].lsv_strategy) ||
		      (headerLHC == LSV_ROSEN))
  #else /* other LHCs than Rosenbrock and Complex */
		  if (headerLHC == evo_ws[jobIdx].lsv_strategy)
  #endif /* other LHCs than Rosenbrock and Complex */
		     prepOptJobLHCPar(hdrLine1, hdrLine2, dataLine, jobIdx);  /* lhcPar */
		  else
		     if (lsv[evo_ws[jobIdx].lsv_strategy].lsvParAnz > 0)
			strcat(dataLine, " ...");
#endif /* MIT_LSV */
	       } /* LHC job */
	       else
		  strcat(dataLine, " ...");                          /* GLEAM or any MA */
	       break; /* LHC job header */

	    case MA_JOB_HDR:  /* ------------------- MA job header: ------------------- */
	       if (actJobOptProc == LSV)
               { /* LHC job */
		  prepOptJobLhcLim(dataLine, jobIdx);                         /* lhcLim */
#ifdef MIT_LSV
		  if (lsv[evo_ws[jobIdx].opt_strategy].lsvParAnz > 0)
		     strcat(dataLine, " ...");
#endif /* MIT_LSV */
	       } /* LHC job */
	       else
	       { /* GLEAM or any MA */
		  prepOptJobLimits(dataLine, jobIdx);                         /* limits */
		  if (actJobOptProc != GLEAM)
	          { /* any MA */
		     prepOptJobActLHC(hdrLine1, jobIdx);                      /* actLHC */
		     strcat(dataLine, hdrLine1);
		  } /* any MA */
	       } /* GLEAM or any MA */
	       break; /* MA job header */
	    } /* switch (headerJobType) */
	 } /* valid job */
	 write_buf(lineCtr++, dataLine);
      } /* compile data of one job */
   } /* job list not empty */
} /* prepJobListDisplay */
#endif /* GLEAM_USR */



/* ========================== showActJobDataOnProcessingStart ========================= */
void showActJobDataOnProcessingStart (void)
{
   STRING_TYPE  buf1;                /* auxiliary buffer                                */
   STRING_TYPE  execBuf1, execBuf2,  /* } part of the execution display that            */
                execBuf3;            /* } is also used and extended for logging         */
   INT          len,                 /* actual length of output buffer                  */
                actLHCLen;           /* actual length of "actLHC" string                */
   BOOL         evoJob,              /* TRUE, if optimization job is evolution based    */
                lhcBasedPopInit,     /* TRUE, if init. of start pop. uses some LHCs     */
                smaJob,              /* TRUE, if it is an SMA optimization job          */
                maJob;               /* TRUE, if optimization job uses any MA           */

   /* showActJobDataOnProcessingStart */
   if (first_evo_item >= free_evo_index)
   { /* opt job index out of range */
      sprintf(buf1, BAD_JOB_INDEX_TXT, first_evo_item, free_evo_index);
      write_to_log(buf1, CLOSE_LOG);
#ifdef TEXT_MODE
      printf("%s\n", buf1);
#endif /* TEXT_MODE */
   } /* opt job index out of range */
   else
   { /* opt job index valid */
      actLHCLen = 0;

      /* ------------------ show initialization files if reloaded: -------------------- */
      if (initFileReloaded) 
      { /* write loaded file names to logfile */
	 if (!logfile_open)
	    logfile = fopen (logfilename, "at");
	 logfile_open = logfile != NULL;
	 if (logfile_open) {
	    fprintf(logfile, "\n%s%s\n", STRICH_40_TXT, STRICH_40_TXT);
	    fprintf(logfile, INIT_FILES_RELOADED_TXT);
	    fprintf(logfile, "%s%s\n", STRICH_40_TXT, STRICH_40_TXT);
	    print_exp_data(logfile);
	    fclose (logfile);
	    logfile_open = FALSE;
	 }
	 initFileReloaded = FALSE;
      } /* write loaded file names to logfile */

      /* --------- prepare job data for display on execution and for logging: --------- */
      evoJob = (evo_ws[first_evo_item].opt_strategy <= GLEAM_AMMA);
      maJob  = evoJob && (evo_ws[first_evo_item].opt_strategy != GLEAM);
      smaJob = (evo_ws[first_evo_item].opt_strategy == GLEAM_SMA);
      sprintf(actJobHdrBuf1, "\n%s", JOBLI_BASIC_HDR1_TXT);                    /* basic */
      strcpy(actJobHdrBuf2, JOBLI_BASIC_HDR2_TXT);
      prepOptJobBasics(actJobDataBuf, first_evo_item);

      // xxyzabc      strncpy(actJobDataSaveBuf1, actJobDataBuf, strlen(actJobDataBuf) - 3);

      strcpy(actJobDataSaveBuf1, actJobDataBuf);
      actJobDataSaveBuf1[strlen(actJobDataBuf) - 3] = '\0';

      if (evoJob)
      { /* opt.job based on evolution */
	 lhcBasedPopInit = ((evo_ws[first_evo_item].init_strat == LSV_NEU) || 
			    (evo_ws[first_evo_item].init_strat == LSV_BEST)  );
	 strcat(actJobHdrBuf1, JOBLOG_EVO_BASIC_HDR1_TXT);                  /* evoBasic */
	 strcat(actJobHdrBuf2, JOBLOG_EVO_BASIC_HDR2_TXT);
	 prepOptJobEvoBasics(actJobDataSaveBuf2, first_evo_item);
	 strcat(actJobDataBuf, actJobDataSaveBuf2);
	 if (lhcBasedPopInit || maJob)
	 { /* show LHC used for initialization of the start population */
	    if (!maJob)
	       actLHCLen = strlen(JOBLI_ACT_LHC_HDR2_TXT);      /* evo + LHC pop. init. */
	    strcat(actJobHdrBuf1, JOBLI_ACT_LHC_HDR1_TXT);                    /* actLHC */
	    strcat(actJobHdrBuf2, JOBLI_ACT_LHC_HDR2_TXT);
	    prepOptJobActLHC(buf1, first_evo_item);
	    strcat(actJobDataBuf, buf1);
	    strcat(actJobDataSaveBuf2, buf1);
	 } /* show LHC used for initialization of the start population */
      } /* opt.job based on evolution */
      else
      { /* opt.job based on LHC */
	 lhcBasedPopInit = FALSE;                                             /* lhcPar */
	 strcpy(actJobDataSaveBuf2, "");
	 prepOptJobLHCPar(actJobHdrBuf1, actJobHdrBuf2, actJobDataSaveBuf2, first_evo_item);
	 strcat(actJobDataBuf, actJobDataSaveBuf2);
      } /* opt.job based on LHC */

      /* ----------------- display job data on start of job execution: ---------------- */
#ifdef TEXT_MODE
      printf("%s\n", actJobHdrBuf1);
      printf("%s\n", actJobHdrBuf2);
      printf("%s\n", actJobDataBuf);
#endif /* TEXT_MODE */

      /* ---- continue preparation of job data for logging and write them to file: ---- */
      strcpy(execBuf1, actJobHdrBuf1);
      strcpy(execBuf2, actJobHdrBuf2);
      strcpy(execBuf3, actJobDataBuf);
      execBuf1[(INT)strlen(actJobHdrBuf1) - actLHCLen] = '\0';    /* remove possible    */
      execBuf2[(INT)strlen(actJobHdrBuf2) - actLHCLen] = '\0';    /* actLHC-entry       */
      execBuf3[(INT)strlen(actJobDataBuf) - actLHCLen] = '\0';    /* for logging        */
      if (maJob)
      { /* memetic evolution: maBasic */
	 if (smaJob)
	    prepOptJobLHCPar(execBuf1, execBuf2, execBuf3, first_evo_item);  /* lhcPar */
	 prepOptJobMABasic(execBuf1, execBuf2, execBuf3, first_evo_item);    /* maBasic */
	 if (!smaJob)                                                  /* ASMA or AMMA? */
	    prepOptJobAma(execBuf1, execBuf2, execBuf3);                         /* ama */
      } /* memetic evolution: maBasic */
      if (lhcBasedPopInit && !smaJob)
      { /* evo, ASMA, or AMMA with LHC pop. init. */
	 prepOptJobInitLHC(execBuf1, execBuf2, execBuf3, first_evo_item);     /* iLHC   */
	 prepOptJobLHCPar(execBuf1, execBuf2, execBuf3, first_evo_item);      /* lhcPar */
      } /* evo, ASMA, or AMMA with LHC pop. init. */
      write_to_log(execBuf1, KEEP_LOG_OPEN);
      write_to_log(execBuf2, KEEP_LOG_OPEN);
      write_to_log(execBuf3, KEEP_LOG_OPEN);
      len = strlen(execBuf3);
      strncpy(buf1, longSeperatorLine, len);
      buf1[len] = '\0';
      write_to_log(buf1, CLOSE_LOG);
#ifdef MIT_LSV
      if (gleam_adi && detailed_log && mit_statistik)
      { /* ASMA oder AMMA */
	 prep_adi_param_anzg (1, gleam_amma);
	 logTextBuffer(OHNE_TERM_AUSG);                  /* also resets the text buffer */
	 write_to_log(JOBLI_STRICH_LANG, CLOSE_LOG);
      } /* ASMA oder AMMA */
#endif  /* MIT_LSV */

      prepActJobLimitsBuf();       /* prepares "actJobLimitsBuf" for repetitive display */
   } /* opt job index valid */
} /* showActJobDataOnProcessingStart */



/* ============================ showActJobDataOnContinuation ========================== */
void showActJobDataOnContinuation (void)
{
   STR_TYPE buf1, buf2;              /* auxiliary buffers                               */

   /* showActJobDataOnContinuation */
   if (first_evo_item >= free_evo_index)
   { /* opt job index out of range */
      sprintf(buf1, BAD_JOB_INDEX_TXT, first_evo_item, free_evo_index);
      write_to_log(buf1, CLOSE_LOG);
#ifdef TEXT_MODE
      printf("%s\n", buf1);
#endif /* TEXT_MODE */
   } /* opt job index out of range */
   else
   { /* opt job index valid */
#ifdef TEXT_MODE
      printf("%s\n", actJobHdrBuf1);
      printf("%s\n", actJobHdrBuf2);
      printf("%s%3d%s\n", actJobDataSaveBuf1, evo_ws[first_evo_item].anz_erg_ketten, 
	     actJobDataSaveBuf2);
#endif /* TEXT_MODE */
      prepActJobLimitsBuf();
   } /* opt job index valid */
} /* showActJobDataOnContinuation */



/* ================================== showGenResults ================================== */
void showGenResults (void)
{
   STR_TYPE  buf;

   /* showGenResults */
   if ((gen_counter % 10) == 0)
      showOptRunHdr(SHOW_RUNNING);
   prepOptJobProcessingData(buf);
#ifdef TEXT_MODE
   printf("%s: %s\n", ACTIVE_JOB_ACT_VAL_TXT, buf);
#endif /* TEXT_MODE */
} /* showGenResults */



/* =============================== optJobEndStatistics ================================ */
void optJobEndStatistics (void)
{
   STR_TYPE  buf;

   /* optJobEndStatistics */
   ges_ak_stat_dat.gen_anz      += (LONG)gen_counter;             /* data recording     */
   ges_ak_stat_dat.zeit_sum     += abgelaufene_zeit;              /* for                */
   ges_ak_stat_dat.jobli_indivs += indiv_ctr;                     /* overall statistics */

   if (mem_out)
      write_to_log(EVOPAR_ABORTION, KEEP_LOG_OPEN);
   strcpy (buf, "");
#ifdef MIT_LSV
   if (gleam_adi)
      get_adi_statistik_string(buf);
#endif /* MIT_LSV */
   sprintf(logBuffer, "# %1d %d %1d %d  %ld %d %.16G %1d %ld  %s", 
	   evo_ws[first_evo_item].survival_rule,
	   evo_ws[first_evo_item].popel_size, 
	   no_deme_impr_gen_counter, no_accept_gen_counter,
	   abgelaufene_zeit, gen_counter, gen_note, gen_len, indiv_ctr, buf);
   write_to_log (logBuffer, KEEP_LOG_OPEN);
#ifdef EXT_MATLAB_SIMU
   sprintf(logBuffer, "## %d  # non coverged matlab runs of this job", matlabNoConvCtr);
   write_to_log (logBuffer, KEEP_LOG_OPEN); 
   matlabNoConvCtr = 0;                                  /* Reset to count per opt. job */
#endif /* EXT_MATLAB_SIMU */
   write_to_log ("", CLOSE_LOG);                         /* just an empty line          */
} /* optJobEndStatistics */



/* =================================== showOptRunHdr ================================== */
void showOptRunHdr (INT  showMode)
{
   STR_TYPE  buf;
   INT       len;

   /* showOptRunHdr */
   if (showMode != SHOW_END)
   { /* show start or running */
#ifdef TEXT_MODE
      if ((showMode == SHOW_START) || (showMode == SHOW_RESTART))
      { /* write long seperator line to screen */
	 len = strlen(actJobDataBuf) - (INT)strlen(ACTIVE_JOB_HDR_LINE);
	 strcpy(buf, ACTIVE_JOB_HDR_LINE);
	 if (len > 0) 
	    strncat(buf, longSeperatorLine, len);
	 printf("%s\n", buf);
      } /* write long seperator line to screen */
      else
	 printf("%s\n", ACTIVE_JOB_HDR_LINE);
      printf("%s\n", ACTIVE_JOB_HDR1_TXT);
      printf("%s\n", actJobLimitsBuf);
      if (do_evo) 
	 printf("%s\n", ACTIVE_JOB_HDR_LINE);
#endif /* TEXT_MODE */
      if (showMode == SHOW_START) 
      { /* display data of initialization */
	 prepOptJobProcessingData(buf);
	 sprintf(logBuffer, "%s: %s", ACTIVE_JOB_INIT_TXT, buf);
	 if (mit_statistik) {
	    write_to_log(ACTIVE_JOB_HDR1_TXT, KEEP_LOG_OPEN);
	    write_to_log(actJobLimitsBuf, KEEP_LOG_OPEN);
	    write_to_log(logBuffer, CLOSE_LOG);
	 }
#ifdef TEXT_MODE
	 printf("%s\n", logBuffer);
#endif /* TEXT_MODE */
      } /* display data of initialization */
   } /* show start or running */
   else
   { /* show end */
      strcpy(logBuffer, ACTIVE_JOB_HDR_LINE);     /* delimiting line                    */
      if (mit_statistik) {
	 strcat(logBuffer, ACT_JOB_END_HDR_LINE);
	 write_to_log(logBuffer, KEEP_LOG_OPEN);
      }
#ifdef TEXT_MODE
      printf("%s\n", logBuffer);
#endif /* TEXT_MODE */
      strcpy(logBuffer, ACTIVE_JOB_HDR1_TXT);     /* 1st header line with column titles */
      if (mit_statistik)
	 strcat(logBuffer, ACT_JOB_END_HDR1_TXT);
      write_to_log(logBuffer, KEEP_LOG_OPEN);
#ifdef TEXT_MODE
      printf("%s\n", logBuffer);
#endif /* TEXT_MODE */
      strcpy(logBuffer, actJobLimitsBuf);         /* 2nd header line with target limits */
      if (mit_statistik)
	 strcat(logBuffer, ACT_JOB_END_HDR2_TXT);
      write_to_log(logBuffer, KEEP_LOG_OPEN);
#ifdef TEXT_MODE
      printf("%s\n", logBuffer);
#endif /* TEXT_MODE */
      prepOptJobProcessingData(buf);            /* final job fitness and counter values */
      if (mit_statistik)
	 sprintf(logBuffer, ACT_JOB_END_VAL_TXT, ACTIVE_JOB_ACT_VAL_TXT, 
		 buf, ak_jobli_stat_dat.anz_res_chs, l_round(ak_jobli_stat_dat.best_ch_note), 
		 ak_jobli_stat_dat.best_ch_len);
      else
	 sprintf(logBuffer, "%s: %s", ACTIVE_JOB_ACT_VAL_TXT, buf);
      write_to_log(logBuffer, KEEP_LOG_OPEN);
#ifdef TEXT_MODE
      printf("%s\n", logBuffer);
#endif /* TEXT_MODE */
      strcpy(logBuffer, ACTIVE_JOB_HDR_LINE);     /* delimiting line                    */
      if (mit_statistik)
	 strcat(logBuffer, ACT_JOB_END_HDR_LINE);
      write_to_log(logBuffer, CLOSE_LOG);
#ifdef TEXT_MODE
      printf("%s\n", logBuffer);
#endif /* TEXT_MODE */
   } /* show end */
} /* showOptRunHdr */



/* ===================================== jobLiLog ===================================== */
void jobLiLog (void)
{
#ifdef MIT_LSV
   if (gleam_adi)
      log_adi_ges_statistik();
#endif /* MIT_LSV */
   sprintf(logBuffer, "\n%s%s", STRICH_40_TXT, STRICH_40_TXT);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   write_to_log(ENDSTAT_HDR, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_NOTE, ak_jobli_stat_dat.best_ch_note);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_LEN, ak_jobli_stat_dat.best_ch_len);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_ERG_ANZ, ak_jobli_stat_dat.anz_res_chs);
   write_to_log(logBuffer, KEEP_LOG_OPEN);

   sprintf(logBuffer, "%s%s", STRICH_40_TXT, STRICH_40_TXT);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   write_to_log(ENDSTAT_HDR2, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_NOTE, ges_ak_stat_dat.best_note);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_ALL_ERG, ges_ak_stat_dat.res_ch_sum);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_JOB_ANZ, ges_ak_stat_dat.job_anz);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_POP_ANZ, ges_ak_stat_dat.popel_anz);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_GEN_ANZ, ges_ak_stat_dat.gen_anz);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, ENDSTAT_OFFSPR, ges_ak_stat_dat.jobli_indivs);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   display_time(buf1, ges_ak_stat_dat.zeit_sum, ADJUST);
   sprintf(logBuffer, ENDSTAT_TIME, buf1);
   write_to_log(logBuffer, KEEP_LOG_OPEN);
   sprintf(logBuffer, "%s%s\n", STRICH_40_TXT, STRICH_40_TXT);
   write_to_log(logBuffer, CLOSE_LOG);
   return;
} /* jobLiLog */



/* ================================ prep_adi_param_anzg =============================== */
#ifdef MIT_LSV
static void prep_adi_param_anzg (INT   znr,
                                 BOOL  mitLsvSel)
/* ---------------------------------------------------------------------------------------
   Erzeugt im Textpuffer des "fbhm"-Moduls ab der Zeilennummer "znr" eine Ausgabe der im 
   Genmodell enthaltenen Steuerparameter der Adaption. Bei gesetztem "mitLsvSel" werden 
   auch Daten zur LSV-Auswahl ausgegeben.
--------------------------------------------------------------------------------------- */
{
   INT  idx, ii, jj, kk;

   /* prep_adi_param_anzg */
   znr++;
#ifdef GLEAM_USR
   write_buf (znr++, ADAPAR_INTRAKT_HDR);
#else /* no GLEAM_USR, i.e. command line version */
   write_buf (znr++, ADAPAR_HMOD_HDR);
#endif /* no GLEAM_USR, i.e. command line version */
   if (mitLsvSel) 
   { /* AMMA: LSV-Wahrscheinlichkeiten anzeigen */
      strcpy (buf1, ADAPAR_INIT_P_TXT);
      for (ii = 0;  ii < lsv_anz;  ii++)
      { /* Anzeige der initialen Wahrscheinlichkeiten aller aktiven LSVs */
         idx = aktive_lsv[ii];
         sprintf (buf2, " %c=%g", lsv[idx].lsvCh, lsv[idx].lsvStartP);
         strcat (buf1, buf2);
      } /* Anzeige der initialen Wahrscheinlichkeiten aller aktiven LSVs */
      write_buf (znr++, buf1);
   } /* AMMA: LSV-Wahrscheinlichkeiten anzeigen */
   sprintf (buf1, ADAPAR_SPEED_TXT, lsv_adapt_speed_txt, level_adapt_speed_txt);
   write_buf (znr++, buf1);
   sprintf (buf1, ADAPAR_ANW_LSV_TXT, lsv_einzel_limit[0], lsv_gesamt_limit[0]);
   write_buf (znr++, buf1);
   sprintf (buf1, ADAPAR_ANW_LEV_TXT, level_einzel_limit, level_gesamt_limit);
   write_buf (znr++, buf1);
   sprintf (buf1, ADAPAR_NKL_TXT, nklassen_anz);
   for (ii = 0;  ii < nklassen_anz - 1;  ii++) {
      sprintf (buf2, "%g, ", nklassen_feld[ii] / 1000.0);
      strcat (buf1, buf2);
   }
   sprintf (buf2, "%g)", nklassen_feld[nklassen_anz-1] / 1000.0);
   strcat (buf1, buf2);
   write_buf (znr++, buf1);
   if (mitLsvSel) 
   { /* AMMA: Nix-LSV-Limit anzeigen */
      sprintf (buf1, ADAPAR_OLDFRAC_TXT, oldDistribFrac * 100);
      write_buf (znr++, buf1);
      sprintf (buf1, ADAPAR_NIX_LSV_TXT, lsv_nix_anwendung_limit);
      write_buf (znr++, buf1);
      sprintf (buf1, ADAPAR_LSV_MIN_TXT, startLsvPMin * 100);
      write_buf (znr++, buf1);
   } /* AMMA: Nix-LSV-Limit anzeigen */

   znr++;
#ifdef GLEAM_USR
   write_buf (znr++, ADAPAR_HMOD_HDR);
#endif /* GLEAM_USR */
   sprintf (buf1, ADAPAR_ALLPLEV_TXT, allPLevelAnz);
   for (jj = 0;  jj < allPLevelAnz;  jj++) {
      sprintf (buf2, " %g", allPLevelVal[jj]);
      strcat (buf1, buf2);
      if (((jj+1) % 5) == 0)
         strcat (buf1, " ");
   }
   write_buf (znr++, buf1);
   for (ii = 0;  ii < lsv_anz;  ii++)
   { /* Anzeige der Steuerparameter aller aktiven LSVs */
      idx = aktive_lsv[ii];
      sprintf (buf1, ADAPAR_LSVNAME_TXT, lsv[idx].lsvName);
      write_buf (znr++, buf1);
      sprintf (buf1, ADAPAR_ALLPINI_TXT, lsv[idx].allPStartLevel, 
               lsv[idx].allPIniP1stLev * 100.0, lsv[idx].allPIniP2ndLev * 100.0, 
               (1.0 - lsv[idx].allPIniP1stLev - lsv[idx].allPIniP2ndLev) * 100.0);
      write_buf (znr++, buf1);

      for (kk = 0;  kk < lsv[idx].lsvParAnz;  kk++) { /* alle LSV-Steuer-Params */
         sprintf (buf1, ADAPAR_PAR_NAM_TXT, lsv[idx].lsvParName[kk]);
         write_buf (znr++, buf1);
         sprintf (buf1, ADAPAR_PAR_LEV_TXT,lsv[idx].parStartLev[kk], 
                  lsv[idx].parIniP1stLev[kk] * 100,
                  lsv[idx].parIniP2ndLev[kk] * 100, 
                  (1.0 - lsv[idx].parIniP1stLev[kk] - lsv[idx].parIniP2ndLev[kk]) * 100);
         write_buf (znr++, buf1);
         sprintf (buf1, ADAPAR_PAR_VAL_TXT, lsv[idx].parLevelAnz[kk]);
         for (jj = 0;  jj < lsv[idx].parLevelAnz[kk];  jj++) {
            sprintf (buf2, " %g", lsv[idx].parLevelVal[kk][jj]);
            strcat (buf1, buf2);
            if (((jj+1) % 5) == 0)
               strcat (buf1, " ");
         }
         write_buf (znr++, buf1);
      } /* alle LSV-Steuer-Params */
   } /* Anzeige der Steuerparameter aller aktiven LSVs */
} /* prep_adi_param_anzg */
#endif /* MIT_LSV */



/* ==================================== time2String =================================== */
static void time2String (char *timeBuf,
			 LONG  timeInSec,
                         BOOL  shortFormat)
/* ---------------------------------------------------------------------------------------
   Converts "timeInSec" to a string whose formatting depends on "shortFormat" and the size
   of the time value:
   shortFormat == TRUE:   "hh:mm",    if timeInSec >= 100, beeing rounded up for more than 
                                      29 remaining seconds
                          "xxsec",    if timeInSec <  100      
   shortFormat == FALSE:  "hh:mm:ss"  for time values less than 100 hours
                          "hhh:mm:0"  for time values greater than or equal to 100 hours
--------------------------------------------------------------------------------------- */
{
   INT  std, min, restSec;

   /* time2String */
   std     = (INT)(timeInSec / 3600L);
   restSec = (INT)(timeInSec % 3600L);
   min     = restSec / 60;
   restSec = restSec % 60;
   if (shortFormat)
   { /* "hh:mm" oder "xxsec" */
      if (timeInSec < 100)
	 sprintf(timeBuf, "%2ldsec", timeInSec);
      else 
      { /* "hh:mm" */
	 if (restSec > 29)
	 { /* round up to complete minutes */
	    timeInSec += 30;
	    std     = (INT)(timeInSec / 3600L);
	    restSec = (INT)(timeInSec % 3600L);
	    min     = restSec / 60;
	 } /* round up to complete minutes */
	 if (std < 100)
	    sprintf(timeBuf, "%2d:%02d", std, min);
	 else
	    sprintf(timeBuf, "%4dh", std);
      } /* "hh:mm" */
   } /* "hh:mm" oder "xxsec" */
   else
   { /* "hh:mm:ss" oder "hhhh:mm " */
      if (std < 100)
	 sprintf(timeBuf, "%2d:%02d:%02d", std, min, restSec);
      else
	 sprintf(timeBuf, "%3d:%02d:0", std, min);
   } /* "hh:mm:ss" oder "hhhh:mm " */
} /* time2String */



/* =========================== prepOptJobProcessingDisplay ============================ */
static void prepOptJobProcessingData (char *resultBuf)
/* ---------------------------------------------------------------------------------------
   Write the actual counters to "resultBuf" using ACTIVE_JOB_VAL or ACTIVE_LHC_JOB_VAL resp.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf;

   /* prepOptJobProcessingDisplay */
   time2String(buf, abgelaufene_zeit, LONG_FORMAT);
   if (do_evo)
      sprintf(resultBuf, ACTIVE_JOB_VAL_TXT, l_round(gen_note), buf, gen_counter, 
	      no_deme_impr_gen_counter, no_accept_gen_counter, indiv_ctr);
   else /* LHC job */
      sprintf(resultBuf, ACTIVE_LHC_JOB_VAL_TXT, l_round(gen_note), buf, indiv_ctr);
} /* prepOptJobProcessingDisplay */



/* ================================= prepOptJobBasics ================================= */
static void prepOptJobBasics (char *resultBuf,
                              INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The basic data of the optimization job indicated by "jobIdx" are written to the 
   "resultBuf" according to JOBLI_BASIC_JOB_VAL_TXT. "jobIdx" is checked by the calling 
   function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  jobBuf, demeBuf, rngBuf, initBuf, iniParBuf;

   /* prepOptJobBasics */
   switch (evo_ws[jobIdx].opt_strategy) 
   { /* Optimierungsstrategie */
      case GLEAM:      
         strcpy(jobBuf, EVO_TXT); 
         break;
#ifdef MIT_LSV
      case GLEAM_ASMA: 
         strcpy(jobBuf, EVO_ASMA_TXT); 
         break;
      case GLEAM_AMMA: 
         strcpy(jobBuf, EVO_AMMA_TXT); 
         break;
      case GLEAM_SMA:  
         strcpy(jobBuf, EVO_SMA_TXT);  
         break;
      case LSV:
         strcpy(jobBuf, lsv[evo_ws[jobIdx].lsv_strategy].lsvJobLiName);
         break;
#endif /* MIT_LSV */
      default: strcpy(jobBuf, " ???");
   } /* Optimierungsstrategie */
   if (evo_ws[jobIdx].opt_strategy == LSV)
      strcpy(demeBuf, "- ");
   else
      sprintf(demeBuf, "%-2d", anz_nachbarn);
   if (evo_ws[jobIdx].z_init)
      sprintf(rngBuf, "%4s", JA_TXT);
   else
      sprintf(rngBuf, "%4s", NEIN_TXT);
   switch (evo_ws[jobIdx].init_strat) {
      case NEU:       strcpy(initBuf, INIT_NEU_TXT);     break;
      case BEST:      strcpy(initBuf, INIT_BEST_TXT);    break;
      case MIX:       strcpy(initBuf, INIT_MIX_TXT);     break;
      case BEST_NEU:  strcpy(initBuf, INIT_NEU_BEST);    break;
      case FROM_FILE: strcpy(initBuf, INIT_FILE_TXT);    break;
#ifdef MIT_LSV
      case LSV_NEU:   strcpy(initBuf, INIT_LSV_N_TXT);   break;
      case LSV_BEST:  strcpy(initBuf, INIT_LSV_B_TXT);   break;
      case ONE_CHR:   strcpy(initBuf, INIT_ONE_CHR_TXT); break;
#endif /* MIT_LSV */
      default:        strcpy(initBuf, " ??????");
   } /* CASE */
   if ((evo_ws[jobIdx].init_strat == BEST)      || 
       (evo_ws[jobIdx].init_strat == FROM_FILE)   )
      strcpy  (iniParBuf, "    -");
   else
      if (evo_ws[jobIdx].init_strat == ONE_CHR)
	 adr_to_text(iniParBuf, evo_ws[jobIdx].strat_par, evo_ws[jobIdx].chrNumber, ADJUST);
      else
	 sprintf(iniParBuf, "%5ld", evo_ws[jobIdx].strat_par);
   sprintf(resultBuf, JOBLI_BASIC_JOB_VAL_TXT, jobIdx + 1, jobBuf, evo_ws[jobIdx].popel_size, 
	   demeBuf, rngBuf, initBuf, iniParBuf, evo_ws[jobIdx].anz_erg_ketten);
} /* prepOptJobBasics */



/* =============================== prepOptJobEvoBasics ================================ */
static void prepOptJobEvoBasics (char *resultBuf,
                                 INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The evoBasic data of the optimization job indicated by "jobIdx" are written to the 
   "resultBuf" according to JOBLOG_EVO_BASIC_VAL_TXT. "jobIdx" is checked by the calling 
   function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf;

   /* prepOptJobEvoBasics */
   switch (evo_ws[jobIdx].survival_rule) {
      case LOCAL_LEAST    : strcpy(buf, LL_STRAT_TXT);     break;
      case LOCAL_LEAST_ES : strcpy(buf, LL_ES_STRAT_TXT);  break;
      case BETTER_PARENT  : strcpy(buf, BP_STRAT_TXT);     break;
      case ALWAYS_ES      : strcpy(buf, AA_ES_STRAT_TXT);  break;
      case ALWAYS         : strcpy(buf, AA_STRAT_TXT);     break;
      default             : strcpy(buf, NO_STRAT_TXT);
   } /* switch */
   sprintf(resultBuf, JOBLOG_EVO_BASIC_VAL_TXT, evo_ws[jobIdx].xo_min_ham_dist * 100.0,
	   evo_ws[jobIdx].rank_param, buf);
} /* prepOptJobEvoBasics */



/* ================================= prepOptJobActLHC ================================= */
static void prepOptJobActLHC (char *resultBuf,
                              INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The actLHC data of the optimization job indicated by "jobIdx" are written to the 
   "resultBuf" according to JOBLI_ACT_LHC_JOB_VAL_TXT. "jobIdx" is checked by the calling 
   function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf;
   INT       ii;

   /* prepOptJobActLHC */
#ifdef MIT_LSV
   if (evo_ws[jobIdx].opt_strategy == GLEAM_AMMA) 
   { /* list of active LHCs */
      strcpy(buf, "");
      for (ii = 0;  ii < lsv_anz;  ii++)                              /* assemble lhc   */
	 buf[ii] = lsv[aktive_lsv[ii]].lsvCh;                         /* characters     */
      buf[lsv_anz] = '\0';                                            /* to a string    */
      sprintf(resultBuf, JOBLI_ACT_LHC_JOB_VAL_TXT, buf);
   } /* list of active LHCs */
   else
      sprintf(resultBuf, JOBLI_ACT_LHC_JOB_VAL_TXT, lsv[evo_ws[jobIdx].lsv_strategy].lsvJobLiName);
#else /* no MIT_LSV */
   sprintf(resultBuf, JOBLI_ACT_LHC_JOB_VAL_TXT, "???");
#endif /* no MIT_LSV */
} /* prepOptJobActLHC */



/* ================================= prepOptJobLHCPar ================================= */
static void prepOptJobLHCPar (char *resBuf1, 
                              char *resBuf2, 
                              char *resBuf3, 
                              INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The lhcPar data of the optimization job indicated by "jobIdx" are appended to the 
   result buffers "resBuf?". The output is limited to 4 parameter. "jobIdx" is checked 
   by the calling  function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf;
   DOUBLE    rVal;
   INT       lhcIdx, anz, ii;
   BOOL      zu_viel;

   /* prepOptJobLHCPar */
#ifdef MIT_LSV
   lhcIdx  = evo_ws[jobIdx].lsv_strategy;
   zu_viel = (lsv[lhcIdx].lsvParAnz > 4);
   if (lsv[lhcIdx].lsvParAnz > 4)
      anz = 4;
   else
      anz = lsv[lhcIdx].lsvParAnz;
   for (ii = 0;  ii < anz;  ii++) 
   { /* for all parameters of that LHC */
      strcat (resBuf1, lsv[lhcIdx].lsvParJL1Name[ii]);
      strcat (resBuf2, lsv[lhcIdx].lsvParJL2Name[ii]);
      rVal = lsv[lhcIdx].lsvPar[ii];
      if (rVal < 1.0)
	 sprintf (buf, " %7.2g", rVal);
      else
	 sprintf (buf, " %7g", rVal);
      strcat (resBuf3, buf);
   } /* for all parameters of that LHC */
   if (zu_viel) {
      strcat (resBuf1, " ...");
      strcat (resBuf2, " ...");
   }
#else /* no MIT_LSV */
   strcat(resBuf1, "  no");
   strcat(resBuf2, " LHC");
   strcat(resBuf3, " ???");
#endif /* no MIT_LSV */
} /* prepOptJobLHCPar */



/* ================================= prepOptJobInitLHC ================================ */
static void prepOptJobInitLHC (char *resBuf1, 
                               char *resBuf2, 
                               char *resBuf3, 
                               INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The job list name of the LHC of the optimization job indicated by "jobIdx" is appended 
   to the result buffers "resBuf?" according to JOBLOG_INIT_LHC_VAL_TXT. "jobIdx" is 
   checked  by the calling function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf;

   /* prepOptJobInitLHC */
#ifdef MIT_LSV
   strcat (resBuf1, JOBLOG_INIT_LHC_HDR1_TXT);
   strcat (resBuf2, JOBLOG_INIT_LHC_HDR2_TXT);
   sprintf(buf, JOBLOG_INIT_LHC_VAL_TXT, lsv[evo_ws[jobIdx].lsv_strategy].lsvJobLiName);
   strcat (resBuf3, buf);
#else /* no MIT_LSV */
   strcat(resBuf1, " |  no");
   strcat(resBuf2, " | LHC");
   strcat(resBuf3, "   ???");
#endif /* no MIT_LSV */
} /* prepOptJobInitLHC */



/* ================================= prepOptJobMABasic ================================ */
static void prepOptJobMABasic (char *resBuf1, 
                               char *resBuf2, 
                               char *resBuf3, 
                               INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   Some basic MA related data of the optimization job indicated by "jobIdx" are appended 
   to the result buffers "resBuf?" according to JOBLOG_MA_BASIC_VAL_TXT. "jobIdx" is 
   checked by the calling function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buffer;
   BUF_TYPE  buf1;

   /* prepOptJobMABasic */
#ifdef MIT_LSV
   strcat (resBuf1, JOBLOG_MA_BASIC_HDR1_TXT);
   strcat (resBuf2, JOBLOG_MA_BASIC_HDR2_TXT);
   if (evo_ws[jobIdx].lsv_all_offspr)
      if (evo_ws[jobIdx].opt_strategy == GLEAM_SMA)
	 strcpy(buf1, MA_ALL_OFFSPRING_TXT);
      else
	 strcpy(buf1, MA_ADAPT_ALL_OFFSPR_TXT);
   else
      strcpy(buf1, MA_BEST_OFFSPRING_TXT);
   sprintf(buffer, JOBLOG_MA_BASIC_VAL_TXT, i_round(evo_ws[jobIdx].lamarck_rate * 100.0), buf1);
   strcat (resBuf3, buffer);
#else /* no MIT_LSV */
   strcat(resBuf1, " no LHCs,");
   strcat(resBuf2, "   no MA!");
   strcat(resBuf3, "      ???");
#endif /* no MIT_LSV */
} /* prepOptJobMABasic */



/* =================================== prepOptJobAma ================================== */
static void prepOptJobAma (char *resBuf1, 
                           char *resBuf2, 
                           char *resBuf3)
/* ---------------------------------------------------------------------------------------
   The parameters of the adaptive control are appended to the result buffers "resBuf?" 
   according to JOBLOG_AMA_VAL_TXT. 
--------------------------------------------------------------------------------------- */
{
   STRING_TYPE  buffer;
   BUF_TYPE     buf1, buf2;

   /* prepOptJobAma */
#ifdef MIT_LSV
   strcat (resBuf1, JOBLOG_AMA_HDR1_TXT);
   strcat (resBuf2, JOBLOG_AMA_HDR2_TXT);
   switch (aktLSVAdaptSpeed) {
      case ADAPT_SPEED_SCHNELL: strcpy(buf1, ADI_SCHNELL_TXT); break;
      case ADAPT_SPEED_MITTEL:  strcpy(buf1, ADI_MITTEL_TXT);  break;
      case ADAPT_SPEED_LANGSAM: strcpy(buf1, ADI_LANGSAM_TXT); break;
   }
   switch (aktLevelAdaptSpeed) {
      case ADAPT_SPEED_SCHNELL: strcpy(buf2, ADI_SCHNELL_TXT); break;
      case ADAPT_SPEED_MITTEL:  strcpy(buf2, ADI_MITTEL_TXT);  break;
      case ADAPT_SPEED_LANGSAM: strcpy(buf2, ADI_LANGSAM_TXT); break;
   }
   sprintf(buffer, JOBLOG_AMA_VAL_TXT, nklassen_anz, buf1, buf2, oldDistribFrac * 100);
   strcat (resBuf3, buffer);
#else /* no MIT_LSV */
   strcat(resBuf1, " no LHCs,");
   strcat(resBuf2, "  no AMA!");
   strcat(resBuf3, "    ?????");
#endif /* no MIT_LSV */
} /* prepOptJobAma */


#ifdef GLEAM_USR
/* ================================= prepOptJobLimits ================================= */
static void prepOptJobLimits (char *resultBuf,
                              INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The limit and target data of the optimization job indicated by "jobIdx" are appended 
   to the "resultBuf" according to JOBLI_LIMITS_JOB_VAL_TXT. "jobIdx" is checked by the  
   calling function.
--------------------------------------------------------------------------------------- */
{
   STRING_TYPE  buf3;
   BUF_TYPE     buf1, buf2;

   /* prepOptJobLimits */
   time2String(buf1, evo_ws[jobIdx].job_zeit, SHORT_FORMAT);
   if (evo_ws[jobIdx].maxFEs == INT_MAX)
      strcpy(buf2, "      -");
   else
      if (evo_ws[jobIdx].maxFEs < 10000000)
	 sprintf(buf2, "%7ld", evo_ws[jobIdx].maxFEs);
      else
	 strcpy(buf2, JOBLI_TO_BIG_FES_TXT);
   sprintf(buf3, JOBLI_LIMITS_JOB_VAL_TXT, l_round(evo_ws[jobIdx].ziel_note), buf1,
	   evo_ws[jobIdx].generationen,  evo_ws[jobIdx].genno_deme_impr, 
	   evo_ws[jobIdx].gen_no_accept, buf2);
   strcat(resultBuf, buf3);
} /* prepOptJobLimits */



/* ================================= prepOptJobLhcLim ================================= */
static void prepOptJobLhcLim (char *resultBuf,
                              INT   jobIdx)
/* ---------------------------------------------------------------------------------------
   The target fitness value of the optimization job indicated by "jobIdx" is appended to 
   the  "resultBuf" according to JOBLI__JOB_VAL_TXT. "jobIdx" is checked by the calling 
   function.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf;

   /* prepOptJobLhcLim */
   sprintf(buf, JOBLI_LHC_LIM_JOB_VAL_TXT, l_round(evo_ws[jobIdx].ziel_note));
   strcat(resultBuf, buf); 
} /* prepOptJobLhcLim */
#endif /* GLEAM_USR */


/* ================================ prepActJobLimitsBuf =============================== */
static void prepActJobLimitsBuf (void)
/* ---------------------------------------------------------------------------------------
   Prepares "actJobLimitsBuf" for repetitive display of the actual job limits on job 
   execution.
--------------------------------------------------------------------------------------- */
{
   STR_TYPE  buf1, buf2;

   /* prepActJobLimitsBuf */
   if (do_evo)
   { /* opt.job based on evolution */
      time2String(buf1, evo_ws[first_evo_item].job_zeit, LONG_FORMAT);
      if (evo_ws[first_evo_item].maxFEs == INT_MAX)
	 strcpy(buf2, "      -");
      else
	 if (evo_ws[first_evo_item].maxFEs < 10000000)
	    sprintf(buf2, "%7ld", evo_ws[first_evo_item].maxFEs);
	 else
	    strcpy(buf2, JOBLI_TO_BIG_FES_TXT);
      sprintf(actJobLimitsBuf, ACTIVE_JOB_HDR2_TXT, l_round(evo_ws[first_evo_item].ziel_note), buf1,
	      evo_ws[first_evo_item].generationen,    evo_ws[first_evo_item].genno_deme_impr, 
	        evo_ws[first_evo_item].gen_no_accept, buf2);
   } /* opt.job based on evolution */
   else /* LHC job */
      sprintf(actJobLimitsBuf, ACTIVE_LHC_JOB_HDR2_TXT, l_round(evo_ws[first_evo_item].ziel_note));
} /* prepActJobLimitsBuf */



#ifdef MPI_PAR_VERS
/* =================================== logOptJobData ================================== */
void logOptJobData (INT  initStrat,
		    INT  genCtr,
	            INT  gdvCtr,
		    INT  gakCtr)
{
   gen_counter              = genCtr;                          /* Damit es die Routine  */
   no_deme_impr_gen_counter = gdvCtr;                          /* "popul_end_statistik" */
   no_accept_gen_counter    = gakCtr;                          /* anzeigt.              */
   popul_end_statistik ();
} /* logOptJobData */
#endif /* MPI_PAR_VERS */



/* =================================== init_evo_anzg ================================== */
BOOL init_evo_anzg (void)
{
   if (((actJobHdrBuf1      = (char*)malloc(MAX_STR    * sizeof(char))) == NULL) ||
       ((actJobHdrBuf2      = (char*)malloc(MAX_STR    * sizeof(char))) == NULL) ||
       ((actJobDataBuf      = (char*)malloc(MAX_STR    * sizeof(char))) == NULL) || 
       ((actJobDataSaveBuf1 = (char*)malloc(MAX_STR    * sizeof(char))) == NULL) || 
       ((actJobDataSaveBuf2 = (char*)malloc(MAX_STR    * sizeof(char))) == NULL) || 
       ((actJobLimitsBuf    = (char*)malloc(MAX_STR    * sizeof(char))) == NULL) ||  
       ((longSeperatorLine  = (char*)malloc(190        * sizeof(char))) == NULL) ||  
       ((logBuffer          = (char*)malloc(MAX_STRING * sizeof(char))) == NULL)   )
      return (FALSE);
   initFileReloaded = FALSE;
   memset(longSeperatorLine, '-', 190);
#ifdef GLEAM_USR
   detailed_log = TRUE;     /* controlled by TSK-file in command line version */
#endif /* GLEAM_USR */
   return (TRUE);
} /* init_evo_anzg */

