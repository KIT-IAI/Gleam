/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Englisch)
Package: HP                    File: gleam_eng_d.h           Version:     V1.0.7
Status : o.k.                 Author: W.Jakob                   Date: 09/29/2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3 For more details the see the comment section at the end of the main
program files or the "schalter.h"-file.
--------------------------------------------------------------------------------

Abbreviations:
   popReq       : required for population based optimization 
   opt          : optional
   def          : default value
   noOpt        : no program call option 
   reqAccOptProc: required according to optimization procedure
   HyGLEAM      : only relevant in HyGLEAM context. I.e., program is generated with 
                  MIT_LSV-switch set.
Rules:
   1. Program options override corresponding TSK-file parameters
   2. Options or parameters, which are not required for the selected 
      optimization procedure, are ignored.
------------------------------------------------------------------------------- */


/* ------------------------ Texte des Hauptprogramms: ----------------------- */
#define CALL_MAIN_TXT     "usage:"
#define CALL_EXP_TXT     "     EXP_FILE    : filename of the experiment file (with path). Lists all files involved.\n"
#define CALL_LOG_TXT     "     LOG_FILE    : filename of the log file (with path). If not specified, the name of the experiment file is used.\n"
#define CALL_OPT_TXT     "\n  All options override TSK-file specifications, if provided.\n"
#define CALL_PAR_A_TXT   "    -A<text>     : acceptance rule for offspring: always, localLeast, betterParent, always-ES, localLeast-ES\n"
#define CALL_PAR_B_TXT   "    -B(+|-)      : +: best-improvement,  -: all-improvement\n" 
#define CALL_PAR_C_TXT   "    -C(+|-)      : With/without creation of a case-file when simulating the final result. Only for MATPOWER applications!\n"
#define CALL_PAR_D_TXT   "    -D<demeSize> : neighbourhood size, must be even and smaller than popSize. (%d - %d)\n"
#define CALL_PAR_E_TXT   "    -E<number>   : limit for function evaluations (default: termination condition switched off)\n"
#define CALL_PAR_F_TXT   "    -F<fitness>  : fitness limit (termination condition) (0.0 - %3.1f)\n"
#define CALL_PAR_G_TXT   "    -G<gen>      : generation limit (termination condition)\n"
#define CALL_PAR_H_TXT   "    -h -H --help : online help (this text)\n"  
#define CALL_PAR_Ig_TXT  "    -I<text>     : initialization strategy: new, best, mix, bestNew, file\n"
#define CALL_PAR_Ih_TXT  "    -I<text>     : initialization strategy: new, best, mix, bestNew, lhcNew, lhcBest, file\n" 
#define CALL_PAR_ig_TXT  "    -i<value>    : initialization parameter: mix:minimal fitness,  bestNew:amount of best\n"
#define CALL_PAR_ih_TXT  "    -i<value>    : initialization parameter: mix:minimal fitness,  bestNew,lhcBest:amount of best,  lhcNew:amount of LHC-optimized\n"
#define CALL_PAR_L_TXT   "    -L(+|-)      : Lamarckian evolution: +:on,  -:off (Baldwinian evolution)\n" 
#define CALL_PAR_l_TXT   "    -l<frac>     : learning rate: proportion of old LHC distribution used for the new one in "PROZ" (0.0 - 100.0)\n" 
#define CALL_PAR_M_TXT   "    -M<value>    : maximum iterations per LHC run (termination threshold)\n" 
#define CALL_PAR_N_TXT   "    -N<text>     : domain name of the external simulation services (ESS)\n"
#define CALL_PAR_n_TXT   "    -n<islandNo> : 0: initializer, >0: islang number, used by PAR_POP program variants\n"
#define CALL_PAR_Og_TXT  "    -O<text>     : optimization procedure: gleam\n" 
#define CALL_PAR_Oh_TXT  "    -O<text>     : optimization procedure: gleam, sma, asma, amma, lhc\n" 
#define CALL_PAR_P_TXT   "    -P<popSize>  : population size (4 - %d)\n"  
#define CALL_PAR_p_TXT   "    -p(+|-)      : logging: +:detailed,  -:minimal\n" 
#define CALL_PAR_R_TXT   "    -R<value>    : ranking parameter, from minimal to maximal selective pressure (1.0 - 2.0)\n"
#define CALL_PAR_r_TXT   "    -r<number>   : number of results. Default: 1,  max: popSize/2,  0: no simulation or save of results possible.\n"
#define CALL_PAR_S_TXT   "    -S<index>    : Searcher: LHC index for LHC jobs or GLEAM, SMA, ASMA, or AMMA jobs with LHC-initialization (lhcNew or lhcBest)\n"
#define CALL_PAR_T_TXT   "    -T<value>    : time limit: maximal job run time in seconds (termination condition)\n" 
#define CALL_PAR_t_TXT   "    -t<value>    : threshold for LHCs (optional 2nd LHC termination threshold)\n"
#define CALL_PAR_W_TXT   "    -W<nr>:<val> : New weight of criterion <nr> expressed as its maximum fitness <val>.\n"
#define CALL_PAR_X_TXT   "    -X           : reserved for undocumented experimental settings.\n"
#define AUFRUF_DOKU          "Program parameter: "
#define IMPROPER_TXT         "Improper"
#define PSIZE_TXT            "population size"
#define DEME_SIZE2_TXT       "deme size"
#define NO_EXT_TERM_MLDG     "LHC optimization cannot be terminated externally!"
#define BAD_MA_SW_TXT        "Options for memetic evolution ignored."
#define BAD_LSV_SW_TXT       "Options for local hill climbers not supported."
#define REDUZ_TXT            " *** Note: Too many result chromosomes specified. Reduced from %d to %d."
#define MIN_HAM_XO_TXT       "MinHamDistXO"
#define ZEIT_TXT             "time"
#define GEN_TXT              "generations"
#define RES_ANZ_TXT          "number of results"
#define NO_SAVE_NO_SIM       " *** Note: Result save or simulation is impossible without result chromosome(s)!"
#define LSV_ADA_SPEED_TXT    "meme adaptation speed"
#define LEV_ADA_SPEED_TXT    "parameter adaptation speed"
#define OLD_FRAC_TXT         "old meme distribution"
#define FITNESS_TXT          "fitness"
#define UNKNOWN_TXT          "Unknown"
#define NO_SIMU_LOG_TXT      " *** Note: Version without SimuLogFileIO. Thus no log file generated."
#define CEC_BEW_DATA_ERR     "\n *** Error: Bad evaluation data for CEC-MBFs!"
#define RESOURCES_TXT        "Total elapsed time: %ld:%02ld. Number of evaluations: %ld."
#define NO_GENERATIONS       " No generations!"
#define NO_TIME_ELAPSED      " No time elapsed!"
#define NO_JOB_TIME_GEN      " *** Note: No data available for duration and number of generations (\"#t\"-line)"
#define ERG_AK_TXT           "Result: %d chromosomes. Best is #%d: %d/%d"
#define NOTE_TXT             "%s, fitness: %.9f\n"
#define NOTEN_DIFF_TXT       "\n *** Error: Result simulation yields different fitness values!!!\n"
#define SIMU_ERG_TXT         "Simulation result: fitness= %.9f"
#define NO_ACCESS_TXT        "%s: no access, no result!\n"
#define NICHT_ALLE_TXT       " *** Note: Not all result chromosomes could be marked for save!"
#define RES_FILE_TXT         "_res.aks"
#define NO_SAVE_TXT          " *** Note: No result chromosome could be marked for save! No file generated."
#define OK_FIN_MLDG_TXT      "Optimization run successfully finished.\n"
#define BAD_FIN_MLDG_TXT     "Optimization aborted with error (see log file \"%s\").\n\n"
#define INIT_SPAR_CORR_MSG   "Correction of the initialization parameter of strategy \"new\" from %d to %d."
#define WEIGHTS_ADJUSTED_TXT "\nFitness weights successfully adjusted according to the program parameters."

#ifdef PAR_POP_VERS
  #define MBF_DELAY_TXT      "Delay per execution of an MBF: %d msec"
#endif /* PAR_POP_VERS */


#ifdef ESS_EVAL_SLAVE 
  #define PROG_NAME         "ESS_EVAL_MP"
#else /* no ESS_EVAL_SLAVE */
  #define PROG_NAME         "CLV_MP"
#endif /* ESS_EVAL_SLAVE */

/* --------------------- Texte des ESS-Evaluation-Hauptprogramms: --------------------- */
#define ESS_CALL_TXT        "[OPTIONS] <domain-or-ip> <simu-model>\n"
#define ESS_CALL_DOMAIN_TXT "      <domain-or-ip>: Domain name or IP address of the service to communicate with.\n"
#define ESS_CALL_SIMMOD_TXT "      <simu-model>  : Simulation model. Usually required by the simulator or its interface.\n"
#define ESS_OPTIONS_TXT     "\n    Options:\n"
#define ESS_CALL_M1_TXT     "      -M<mod-file>  : Gene-model file used for the creation of name-value-pairs. If the \n"
#define ESS_CALL_M2_TXT     "                      option is not given, the default file \"%s\" is used \n"
#define ESS_CALL_M3_TXT     "                      by applications, which need a gene model. Depending on the actual \n"
#define ESS_CALL_M4_TXT     "                      application the program terminates, if the model file cannot be opened.\n"
#define ESS_CALL_T1_TXT     "      -T<simu-tmo>  : Time-out in seconds for one simulator run. It depends on the application\n"
#define ESS_CALL_T2_TXT     "                      whether simulation calls are made with time-out control. Default: 1 sec.\n"
#define ESS_CALL_L1_TXT     "      -L(+|-)       : Controls the degree of detail of logging. A log file is always created if\n"
#define ESS_CALL_L2_TXT     "                      it does not exist and can remain empty. Its name is derived from the name\n"
#define ESS_CALL_L3_TXT     "                      of the simulation model by adding \".log\". If no model name is specified\n"
#define ESS_CALL_L4_TXT     "                      the program name is used instead. Log files are written in append mode.\n"
#define ESS_CALL_L5_TXT     "                      No switch specified: Standard logging. Otherwise:\n"
#define ESS_CALL_L6_TXT     "                        -: No logging is done as long as no error occurs.\n"
#define ESS_CALL_L7_TXT     "                        +: detailed logging\n"
#define ESS_CALL_H_TXT      "      -H --help     : Online help (this text)\n"
#define ESS_CALL_FIN_TXT    "    The switch letters may also be in lower case.\n\n"
#define ESS_TERMINATED_TXT  "*** %s terminated.\n"
#define ESS_FIN_SEE_LOG_TXT "*** %s aborted due to %s! See \"%s\".\n"
#define ESS_MISSING_PAR_TXT "*** %s aborted due to missing  program parameter(s)!"
#define ESS_BAD_CALL_TXT    "incorrect program parameters"
#define ESS_ERRORS_TXT      "error(s)"
#define ESS_NO_PARAM_TXT    "Missing  program parameter(s)!\n"
#define ESS_BAD_CHR_HDR_TXT "Bad header data"
#define ESS_BAD_GENE_TXT    "Bad gene data"
#define ESS_LIST_HEADER_TXT "List header"
#define ESS_LIST_ENTRY_TXT  "List element"
#define ESS_LIST_DONE_TXT   "List of %2d chromosomes %processed. Time: %.6f sec"
#define ESS_NO_ERR_TXT      "without errors "
#define ESS_WITH_ERR_TXT    "with errors "
#define ESS_CHR_NUMBER_TXT  "In %d.chromosome: "
#define ESS_CHRLIST_ERR_TXT "Incorrect chromosome list! "
#define ESS_SIMU_ERR_TXT    "%d faulty simulations! "
#define ESS_LIST_FIN_TXT    "Processing of the list aborted!"
