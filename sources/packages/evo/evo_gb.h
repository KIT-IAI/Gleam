/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Englisch)
Package: evo                          File: evo_gb.h                   Version:     V2.4.0
Status : o.k.                        Autor: W.Jakob                      Datum: 21.09.2020
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------ */


/* ---------------------------- Texte des Moduls: evo_anzg ---------------------------- */
#define JOBLI_BASIC_HDR1_TXT      "---Job-- --Sizes- -------Init------- Res"
#define JOBLI_BASIC_HDR2_TXT      " No OptP Pop/Deme  RNG   Popul  Spar Chr"
#define JOBLI_BASIC_JOB_VAL_TXT   "%3d %s %5d/%s %s %s %s %3d"
#define JOBLI_LIMITS_HDR1_TXT     " -----------Target Values------------"
#define JOBLI_LIMITS_HDR2_TXT     " Fitness hh:mm   Gen  GDI GAc MaxEval"
#define JOBLI_LIMITS_JOB_VAL_TXT  "  %6ld %s %5d %4d %3d %s"
#define JOBLI_LHC_LIM_HDR1_TXT    "  Target"
#define JOBLI_LHC_LIM_HDR2_TXT    " Fitness"
#define JOBLI_LHC_LIM_JOB_VAL_TXT "  %6ld"
#define JOBLI_ACT_LHC_HDR1_TXT    "  active"
#define JOBLI_ACT_LHC_HDR2_TXT    "    LHCs"
#define JOBLI_ACT_LHC_JOB_VAL_TXT " %7s"
#define JOBLOG_EVO_BASIC_HDR1_TXT " HamDist Rank AcceptanceRule"
#define JOBLOG_EVO_BASIC_HDR2_TXT "     [%]  Par  for Offspring"
#define JOBLOG_EVO_BASIC_VAL_TXT  " %7.4g %4.3g %s"
#define JOBLOG_MA_BASIC_HDR1_TXT  " UpdRate  Offspring"
#define JOBLOG_MA_BASIC_HDR2_TXT  "     [%]  Selection"
#define JOBLOG_MA_BASIC_VAL_TXT   "     %3d %s"
#define JOBLOG_INIT_LHC_HDR1_TXT  " | Init:"
#define JOBLOG_INIT_LHC_HDR2_TXT  " |  LHC:"
#define JOBLOG_INIT_LHC_VAL_TXT   " | %s "
#define JOBLOG_AMA_HDR1_TXT       " Fitness AdaptationSpeed Learn"
#define JOBLOG_AMA_HDR2_TXT       " Classes  Param/LHC-Sel.  Rate"
#define JOBLOG_AMA_VAL_TXT        "       %d %7s/%-7s %5.2f"
#define ACTIVE_JOB_HDR1_TXT       "       Fitness  Runtime   Gen  GDI GAc MaxEval |"    
#define ACTIVE_JOB_HDR2_TXT       " Limit: %6ld %s %5d %4d %3d %s |"
#define ACTIVE_LHC_JOB_HDR2_TXT   " Limit: %6ld        -     -    -   -       - |"
#define ACTIVE_JOB_HDR_LINE       "-----------------------------------------------+"
#define ACTIVE_JOB_VAL_TXT        "%6ld %s %5d %4d %3d %7ld |"
#define ACTIVE_LHC_JOB_VAL_TXT    "%6ld %s     -    -   - %7ld |"
#define ACTIVE_JOB_ACT_VAL_TXT    "ActVal"
#define ACTIVE_JOB_INIT_TXT       "  Init"
#define ACT_JOB_END_HDR1_TXT      "         Job List Results"    
#define ACT_JOB_END_HDR2_TXT      " Number BestChrom.: Fitness Length"
#define ACT_JOB_END_HDR_LINE      "----------------------------------"
#define ACT_JOB_END_VAL_TXT       "%s: %s   %4d              %6ld  %5d"
#define JOBLI_TO_BIG_FES_TXT      "too big"
#define BAD_JOB_INDEX_TXT         "Bad job index: %d (free_evo_index=%d)"
#define INIT_FILES_RELOADED_TXT   "                 Some initialization files have been reloaded:\n"

#define INIT_FILE_TXT             "   File"
#define INIT_NEU_TXT              "    new"
#define INIT_BEST_TXT             "   best"
#define INIT_MIX_TXT              "    mix"
#define INIT_NEU_BEST             "bestNew"
#define INIT_LSV_N_TXT            " lhcNew"
#define INIT_LSV_B_TXT            "lhcBest"
#define INIT_ONE_CHR_TXT          " Chromo"
#define LL_STRAT_TXT              "    localLeast"
#define LL_ES_STRAT_TXT           " localLeast-ES"
#define AA_STRAT_TXT              " accept always"  
#define AA_ES_STRAT_TXT           "acceptAlwaysES"
#define BP_STRAT_TXT              " better parent"
#define NO_STRAT_TXT              "      invalid!"
#define MA_ALL_OFFSPRING_TXT      "all offspr"
#define MA_ADAPT_ALL_OFFSPR_TXT   "adapt. all"
#define MA_BEST_OFFSPRING_TXT     " best only"
#define ADI_LANGSAM_TXT           "slow"
#define ADI_MITTEL_TXT            "medium"
#define ADI_SCHNELL_TXT           "fast"
#define JA_FORMATIERT_TXT         "   yes "
#define NO_FORMATIERT_TXT         "   no  "
#define EVO_TXT                   " Evo"
#define EVO_SMA_TXT               " SMA"
#define EVO_ASMA_TXT              "ASMA"
#define EVO_AMMA_TXT              "AMMA"


#define JOBLI_STRICH_LANG  "------------------------------------------------------------------------------------------"
#define ADAPAR_INTRAKT_HDR "Interactive adjustable control parameters of adaptation:"
#define ADAPAR_INIT_P_TXT  "Initial probabilities of the LHCs:"
#define ADAPAR_SPEED_TXT   "Values of the LHC and level adaptation speeds \"%s\" and \"%s\":"
#define ADAPAR_ANW_LSV_TXT "  Minimum applications of one/all LHCs           : %d / %d"
#define ADAPAR_ANW_LEV_TXT "  Minimum applications of one/all levels         : %d / %d"
#define ADAPAR_NKL_TXT     "Number of fitness classes with (limits, in thsd) : %d  ("
#define ADAPAR_OLDFRAC_TXT "Fraction of the old LHC distrib. for the new one : %g %%"
#define ADAPAR_NIX_LSV_TXT "Small Plhc repetition limit for LHC deactiviation: %d"
#define ADAPAR_LSV_MIN_TXT "Initial Pmin for LHC deactiviation               : %.1f %%"
#define ADAPAR_HMOD_HDR    "Gene model included control parameters of the adaptation:"
#define ADAPAR_ALLPLEV_TXT "Values of the %d levels for all-P: "
#define ADAPAR_LSVNAME_TXT "LHC \"%s\":"
#define ADAPAR_ALLPINI_TXT "    all-P: Startlevel and initial probabilities: %1d  (%g, %g, %g)"
#define ADAPAR_PAR_NAM_TXT "    LHC parameter \"%s\":"
#define ADAPAR_PAR_LEV_TXT "      Startlevel and initial probab.: %1d  (%g, %g, %g)"
#define ADAPAR_PAR_VAL_TXT "      Values of the %d levels: "
#define JOB_DEAKTIV_TXT    "    job deactivated!"

#define EVOPAR_HDR1        "        Present Parameters of Evolution (%s):"
#define EVOPAR_HDR2        "        ================================"
#define EVOPAR_DEMESIZE    "Deme size                             : %2d"
#define EVOPAR_LL_ES       "local least (elitist)"
#define EVOPAR_LL          "local least"
#define EVOPAR_AA          "always"
#define EVOPAR_AA_ES       "always (elitist)"
#define EVOPAR_BP          "better parent"
#define EVOPAR_SURVRULE    "Acceptance rule for offspring         : "
#define EVOPAR_RANK        "Ranking parameter (1.0 .. 2.0)        : %5.2f"
#define EVOPAR_XO_HAM      "Minimum Hamming distance for Reco/XO  :  %g %%"
#define EVOPAR_LAMARCK     "Lamarck or chromosome update rate     : %5.1f %%"
#define EVOPAR_REPAIR      "Application-specific repair procedures: "
#define EVOPAR_GENO_KL_TXT "genotypic"
#define EVOPAR_PHAENOKLTXT "phenotypic"
#define AKT_INIT_LSV_TXT   "Default LHC for start population init.: %s"
#define AKT_LSV_TXT        "Present default Local Hill Climber LHC: %s"
#define EVOPAR_PHAENO      EVOPAR_PHAENO_Q"                : "
#define EVOPAR_GENO_PHENO  "Warning: Simultaneous genotypic and phenotypic repair may not make sense!"
#define OPTPAR_HDR1        "          Present Default Optimization Strategy:"
#define OPTPAR_HDR2        "          ======================================"

#define EVOPAR_ABORTION    " !!!  Job termination because of lack of memory  !!!"
#define ENDSTAT_HDR        "Result summary of all jobs of the last job list:"
#define ENDSTAT_NOTE       "Best fitness              :   %10.2f"
#define ENDSTAT_LEN        "Length of this chromosome : %9d"
#define ENDSTAT_ERG_ANZ    "# of result chromosomes   : %9d"
#define ENDSTAT_HDR2       "Result summary of all jobs of all job lists since program start:"
#define ENDSTAT_ALL_ERG    "Number of all result chrs : %9d"
#define ENDSTAT_JOB_ANZ    "Number of processed jobs  : %9d"
#define ENDSTAT_POP_ANZ    "Number of populations     : %9d"
#define ENDSTAT_GEN_ANZ    "Number of generations     : %9ld"
#define ENDSTAT_OFFSPR     "Number of eval.offsprings : %9ld"
#define ENDSTAT_TIME       "Total time of all jobs    :  %s"

#define LSV_LIST_TXT       "Local Hill Climbers (LHC) according to gene model:\nTheir control parameters can be overwritten by the TSK-file."
#define STEU_PARAM_TXT     "  %s, control parameter:"
#define NO_STEU_PARAM_TXT  "  %s w/o control parameter"

/*
#define 
#define 
#define 
#define 
*/


/* ---------------------------- Texte des Moduls: evo_steu ---------------------------- */
#define JOB_TERMINATED_TXT "Optimization job terminated due to errors! See messages at the end of job list."


/* ---------------------------- Texte des Moduls: evo_gsw ----------------------------- */
#define CHROMOSOM_TXT      "Chromosome"
#define CHR_SAVE_TXT       "  Out of %d chromosomes %d were saved."


/* ---------------------------- Texte des Moduls: evo --------------------------------- */
#define G_INTERVAL_TXT     "of fitness class %d..%d"
#define AK_SCHON_GESP      "Chromosome is already in memory or an unchanged original."
#define AK_GESPEICHERT     "Chromosome saved. Address: %s"
#define AK_ERR_TXT         "Chromosome due to errors not saved!"
#define GET_BEST_SAVTXT    "Chromosome %3d with fitness = %7.1f saved at %2d/%d."
#define EVO_CRASH_TIME     ", %s %s I=%d"
#define GEN_OP_ERR_TXT     "Op %d, Index=%d"
#define JOBLI_HDR_TXT      "                           Job List:"
#define JOBLI_LEER_TXT     "The job list is empty!"
#define STARTPOP_LSV_OK    "StartPop: Chromosome improved by LHC    : %8.1f  -->  %8.1f"
#define STARTPOP_LSVNIX    "StartPop: Chromosome left unchanged     : %8.1f"
#define EXT_EVO_FIN_TXT    ">>> External evo termination at generation %d and individual %d. Checked every %d matings."


/* ------------------------- Texte des Moduls: parSimEvo ------------------------------ */
#define BAD_RES_AMOUNT_TXT "number of entries in result list"
#define BAD_RES_ITEMS_TXT  "entries of faulty simulations in result list"
#define CHR_PTR_TXT        "non-null chromosome pointers"
#define NO_LIST_RESET_TXT  "no reset of the list!"


/* ---------------------------- Texte des Moduls: evo_cmen: --------------------------- */
#define JOB_DEF_MEN        "DefOptJob"
#define JOB_AEND_MEN       "ModOptJob"
#define OPT_START_MEN      "StartJobLi"
#define ZEIG_JOBLI_MEN     "ShowJobLi"
#define COPY_JOB_MEN       "Copy Job"
#define LOE_JOB_MEN        "Delete Job"
#define RET_ERG_AK_MEN     "SaveResChr"
#define RET_BEST_MEN       "SavBestRes"
#define EVO_PARAM_MEN      "Opt Params"
#define DEL_EP_MEN         "Del Popul."
#define JOB_NR_QUERY       "Job number [%d..%d]: "
#define JOB_SEQ_LEN_Q      "Length of the job sequence to be copied (1..%d) [1]: "
#define NIX_TUN_ERR_TXT    "Function disabled due to current error status!"
#define FKT_STOER_ERR_TXT  "Function may be disturbed due to current error situation!"
#define GJOB_DEF_HDR_TXT   "                     Define GLEAM-Job %1d:"
#define JOB_DEF_HDR_TXT    "                  Define Optimization Job %1d:"
#define JOB_DEF_ABR_TXT    " Definition of optimization job aborted!"
#define JOBAEND_HDR_TXT    "                    Change Optimization Job:"
#define JOBAEND_ABR_TXT    " Alteration of optimization job aborted!"
#define JLSTART_HDR_TXT    "                  Start Optimization Job List:"
#define JOBCOPY_HDR_TXT    "                     Copy Optimization Job:"
#define JOBCOPY_ANZ_TXT    "Number of copies (0..%1d) [1]: "
#define JOBCOPY_OK_TXT     "%1d optimization jobs copied!"
#define JOBCOPY_NIX_TXT    "Optimization job list is full!"
#define JOBLOE_HDR_TXT     "           Deletionn of One or All Optimization Jobs"
#define JOBLOE_AKT1_TXT    "Cancel active and only opt. job? "JA_NEIN_TXT
#define JOBLOE_NR_QUERY    "Number of job to be deleted (%1d..%1d, 0=all): "
#define JOBLOE_WAIT_QUERY  "Delete waiting jobs only "NEIN_JA_TXT
#define JOBLOE_LOE1_TXT    "Entire optimization job list deleted."
#define JOBLOE_LOE2_TXT    "All waiting optimization jobs deactivated."
#define JOBLOE_AKT3_TXT    "Cancel active optimization job? "NEIN_JA_TXT
#define JOBLOE_LOE3_TXT    "Optimization job %d deactivated.\n"
#define JOBLOE_LOE4_TXT    "Optimization job %d already processed.\n"
#define JOBLOE_AKT4_TXT    "Finish generation of current job and save results? "NEIN_JA_TXT
#define JOBLOE_FIN_TXT     "Optimization job %d ended prematurely by user."
#define JOBLOE_ABORT_TXT   "Optimization job %d aborted by user!"
#define AKSAV_HDR_TXT      "                Save Single Result Chromosomes:"
#define SAV_FKT_NIX_TXT    "No chromosome saving from active job possible!"
#define AKSAV_NIXDA_TXT    "No evolution done so far or population erased!"
#define AKSAV_INFO1_TXT    "You can save a chromosome by entering its sequential number or "
#define AKSAV_INFO2_TXT    "you can scan the population and select individuals."
#define AKSAV_SCANQUERY    "Scan the population? "JA_NEIN_TXT
#define AKSAV_STRTQUERY    "Enter start number for scan? (1..%d): "
#define AKSAV_INFO3_TXT    "Scanning may be aborted by ESC."
#define AKSAV_DONE_HDR     "Not yet saved individuals of the population:"
#define AKSAV_AK_QUERY     "Save chromosome (fitness = %7.1f  class = %2d  number =%3d)? %s"
#define AKSAV_WUERG_TXT    "Saving aborted."
#define AKSAV_ENDE_TXT     "Population scanned. Saving finished."
#define AKSAV_IDX_QUERY    "Number of chromosomes to be saved (1..%d): "
#define BESTSAV_HDR_TXT    "               Save the Best Result Chromosomes:"
#define BESTSAV_ANZ_TXT    "Maximum number of chromosomes to be saved (1..%d) [%d]: "
#define BESTSAV_ERG_TXT    "%d chromosomes were saved:"
#define EPOPLOE_HDR_TXT    "                       Erase Population:"
#define EPOPLOE_NIX_TXT    "Population cannot be erased while job is active!"
#define EPOPLOE_OK_TXT     "Population erased."
#define EVOPAR_HDR_TXT     "      Default Settings for Optimization Parameters:"
#define ADI_LANGSAM_CH     's'
#define ADI_MITTEL_CH      'm'
#define ADI_SCHNELL_CH     'f'

#define OPT_VERF_TXT       "OptimProc:  GLEAM  SMA  ASMA  M=AMMA  L=LHC       [%c]"
#define PSIZE_QUERY_TXT    "Population size                    (%2d..%5d) [%4d]"
#ifdef MIT_LSV
  #define INIT_STRAT_TXT   "Init: New  Best  Mix  +=B+N  LHC  #=LHC+B  File   [%c]"
#else /* Kein MIT_LSV */
  #define INIT_STRAT_TXT   "Init: New  Best  Mix  +=B+N  File                 [%c]"
#endif /* Kein MIT_LSV */
#define INIT_LSV_TXT       "Init: New  Best  Chromosome                       [%c]"
#define AK_NGO_TXT         "Strategy parameter: Chromosomes with fitn. > 0  [%3ld]: "
#define NO_ESS_PARAM_TXT   "(fixed value when using the external simulation services)"
#define AK_MIND_N_TXT      "Strategy parameter: minimum fitness          [%6ld]: "
#define AK_BEST_ANZ_TXT    "Strategy parameter: number of best chromosomes  [%3ld]: "
#define ANZ_LSV_N_TXT      "Strategy parameter: # of LHC-optimised Chrs     [%3ld]: "
#define ZGEN_INIT_Q        "Initialise random number generator (RNG) ?      "
#define SURVIVAL_TXT       "Acceptance rule: 1=LL 2=Alw 3=LLES 4=AlwES 5=BP   [%c]"
#define RANK_PAR_TXT       "Ranking parameter                   (1.0..2.0) [%4.2f]"
#define XO_HD_TXT          "Minimal HammingDist for recomb./x-over in "PROZ" [%7g]"
#define EVO_SAVE_TXT       "Population backup rate (generations)     [%10d]: "
#define ERG_AK_QUERY_TXT   "(Max) number of result chromosomes (0..%4d)   [%4d]: "
#define OPT_ENDKRIT_TXT    "                  Termination Conditions:"
#define ZNOTE_TXT          "Minimum fitness                              [%6lu]: "
#define MAX_GEN_TXT        "Maximum number of generations                 [%5d]: "
#define MAX_NO_ACC_TXT     "Max. gen. without offspring acceptance (GAc)  [%5d]: "
#define MAX_NO_DEME_TXT    "Max. gen. without deme improvement (GDI)      [%5d]: "
#define JOB_ZEIT_TXT       "Maximum job time in minutes   [%s = %6g min]: "
#define EVAL_MAX_TXT       "Maximum number of evaluations            [%10ld]: "
#define LAMARCK_PAR_TXT    "Chromosome update rate (Lamarck rate) in "PROZ"     [%4g]"
#define LSV_ALL_OFFSPRQ    "(Adaptive) LHC-optimization of all offspring?   "
#define POP_INIT_LSV_TXT   "Initialization of the start population with: %s\n"
#define GLEAM_CHAR         'G'
#define GLEAM_SMA_CHAR     'S'
#define GLEAM_ASMA_CHAR    'A'
#define GLEAM_AMMA_CHAR    'M'
#define LSV_CHAR           'L'
#define NEU_INIT_CHAR      'N'
#define BEST_INIT_CHAR     'B'
#define MIX_INIT_CHAR      'M'
#define BN_INIT_CHAR       '+'
#define LSV_N_INIT_CHAR    'L'
#define LSV_B_INIT_CHAR    '#'
#define FILE_INIT_CHAR     'F'
#define CHR_INIT_CHAR      'C'
#define AK_UNVERAENDERT    "Old chromosome remains unchanged."
#define AK_OPT_STORED      "New chromosome improved and stored if specified in job."
#define ORG_AK_STORED      "New chromosome stored unchanged if specified in job."
#define LSV_NIX_AK_SAV     "LHC optimization: Function not available."

#define EVOPARAM_OPT       "OptStrat"
#define EVOPARAM_DEME      "Deme Size"
#define EVOPARAM_SURV      "AcceptRule"
#define EVOPARAM_RANK      "Rank-Param"
#define EVOPARAM_HAM       "HammDistXO"
#define EVOPARAM_LAMARCK   "LamarckPar"
#define EVOPARAM_GENREPPAR "GenRepRate"
#define EVOPARAM_PHEREPPAR "PhenoRep"
#define EVOPARAM_LSV_AUSW  "Select LS"
#define EVOPARAM_LSV_PARAM "LHC Params"
#define EVOPARAM_LSV_VERT  "LSIniDistr"
#define EVOPARAM_ADA_SPEED "AdaptSpeed"
#define EVOPARAM_N_KLASSE  "FitnClass"
#define EVOPARAM_ADAPT_ALT "AdaptOld"
#define EVOPARAM_NIX_LSV   "NoLHCLimit"
#define EVOPARAM_POPMOD    "PopulModel"

#define OPTSTRAT_EVO       "GLEAM"
#define OPTSTRAT_SMA       "GLEAM-SMA"
#define OPTSTRAT_ASMA      "GLEAM-ASMA"
#define OPTSTRAT_AMMA      "GLEAM-AMMA"
#define OPTSTRAT_LSV       "LHC"
#define OPTPAR_LSV_TXT     "                        LHC %s:"
#define OPTSTRAT_HDR       "           Selection of a New Optimization Procedure"
#define OPTSTRAT_ANZG      "Present default optimization procedure : "
#define EVOPAR_DSIZFRAG    "Deme size           (%d..%d) [%d]: "
#define EVOPAR_DSIZ_F1     "Only even values allowed!"
#define EVOPAR_SURV_HDR    "           Selection of a New Default Acceptance Rule"
#define EVOPAR_SURVANZG    "Present default acceptance rule    : "
#define EVOPAR_SURV_BP     "BetParent"
#define EVOPAR_SURVLLES    "LocLeastES"
#define EVOPAR_SURVAAES    "AlwaysES"
#define EVOPAR_SURV_LL     "LocalLeast"
#define EVOPAR_SURV_AA     "Always"
#define EVOPAR_RANKFRAG    "Default ranking param. of mate selection (1..2) [%4.2f]: "
#define EVOPAR_XO_HAM_Q    "Minimal Hamming distance of 2 chromosomes for Reko/XO in "PROZ" [%g]: "
#define EVOPAR_PHAENO_Q    "With phenotypic repair"
#define EVOPAR_LAMA_Q      "Lamarck rate (chromosome update rate in case of LHC success) in "PROZ" [%g]: "
#define EVOPAR_AMMA_LSV_P  "Specification of initial probabilities of active LHCs:"
#define EVOPAR_AMMA_NEU_P  "New probabilities are active now."
#define EVOPAR_AMMA_P_ERR1 "Abort: Old probabilities are still valid."  
#define EVOPAR_AMMA_P_ERR2 "Sum of new probabilities = %g < 1.0! Old values remain valid."
#define EVOPAR_AMMA_SPEED  "Parameterization of LHCs \"%s\":"
#define EVOPAR_AMMA_ERR1   "The LHC could not be parameterized!"
#define EVOPAR_AMMA_ERR2   "The LHC \"%s\" has no control parameters."
#define EVOPAR_AMMA_SPEED1 "Speed of LHC adaption     (%c, %c, %c) [%c]: "
#define EVOPAR_AMMA_SPEED2 "Speed of level adaption (%c, %c, %c)   [%c]: "
#define EVOPAR_AMMA_NKL    "Parameterization of fitness classes: standard (1,3,4,5) or manual [%d]: "
#define EVOPAR_AMMA_NKLANZ "Number of fitness classes (1..%d) [%d]: "
#define EVOPAR_AMMA_NKLWRT "Fitness value of class %d (%5g..%g) [%5g]: " 
#define EVOPAR_AMMA_ALT    "Proportion of the old distribution in the new one in %% [%g]: "
#define EVOPAR_A_FAST_CHAR 'F'
#define EVOPAR_A_MED_CHAR  'M'
#define EVOPAR_A_SLOW_CHAR 'S'
#define EVOPAR_AMMA_NIXLIM "                         Limit for shutdown of an LHC\n\n"
#define EVOPAR_LSVOFF_TXT1 "If the newly calculated execution probability of an LHC \"Plhc\" drops below the\n"
#define EVOPAR_LSVOFF_TXT2 "initial limit of %g"PROZ", this is counted. Plhc is raised to %g"PROZ", if it falls below \n"
#define EVOPAR_LSVOFF_TXT3 "this value. Only if Plhc falls below the limit value %d times in succession, the \n"
#define EVOPAR_LSVOFF_TXT4 "LHC is switchend off.\n\n"
#define EVOPAR_LSVOFF_QUERY "Maximum for consecutive Plhc-undercuttings (1..%d) [%d]: "


/* ---------------------------- Texte des Moduls: ls_koppl: --------------------------- */
#define IPAR_ZU_GROSS      "Int-Parameter too big: Max=%d act=%d\n"
#define IPAR_ZU_KLEIN      "Int-Parameter too small: Min=%d act=%d\n"
#define RPAR_ZU_GROSS      "Real-Parameter too big: Max=%g act=%g\n"
#define RPAR_ZU_KLEIN      "Real-Parameter too small: Min=%g act=%g\n"


/* ---------------------------- Texte des Moduls: lsv_steu: --------------------------- */
#define ROS_LONG_NAME_TXT  "Rosenbrock procedure"
#define ROS_MENUE_NAME_TXT "Rosenbrock"
#define ROS_JOBLI_NAME_TXT " Ros"
#define ROS_PAR1_NAME_TXT  "R-iteration limit"
#define ROS_PAR1_JOBLI_1   "    Max-"
#define ROS_PAR1_JOBLI_2   "    Iter"
#define ROS_PAR2_NAME_TXT  "termination limit"
#define ROS_PAR2_JOBLI_1   " Rosenbr"
#define ROS_PAR2_JOBLI_2   "   limit"
#define ROS_PAR3_NAME_TXT  "initial step size"
#define ROS_PAR3_JOBLI_1   " Rosenbr"
#define ROS_PAR3_JOBLI_2   "StepSize"
#define COMPL_LONG_NAME    "Complex algorithm"
#define COMPL_MENUE_NAME   "ComplexAlg"
#define COMPL_JOBLI_NAME   "Comp"
#define COMPL_PAR1_NAME    "C-iteration limit"
#define COMPL_PAR1_JOBLI_1 "    Max-"
#define COMPL_PAR1_JOBLI_2 "    Iter"
