/* -------------------------------------------------------------------------------------------------
GLEAM/AE                            Sprach-Header-File (Englisch)
Package: aufg                              File: aufg_gb.h                       Version:     V2.0.5
Status : o.k.                             Autor: W.Jakob                           Datum: 28.08.2020
----------------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
----------------------------------------------------------------------------------------------------
Abbreviations:
  opt:           optional
  required:      required for command line version
  popReq:        required for command line version, if population based optimization
  LSKPreq:       required for robot application (LSKP)
  OPALreq:       required for scheduling application (OPAL)
  reqAccOptProc: required according to optimization procedure
  def:           default value, if any
  (def):         the associated variable has a default according to the external simulator
  noOpt:         the command line version has no corresponding option for this parameter
  HyGLEAM:       Parameter of the HyGLEAM extension (memetic algorithm based on GLEAM)
------------------------------------------------------------------------------------------------- */

/* ------------------------- TSK-File Parameter of all GLEAM versions: -------------------------- */
#define WITH_RANDGEN_INIT_TXT "with init. of random generator"      /* opt, def: TRUE,  noOpt     */ 
#define WITH_REC_EVO_BEST_TXT "with recording of best of evolution" /* opt, def: TRUE,  noOpt     */ 
#define WITH_STAT_DAT_COL_TXT "with recording of statistical data"  /* opt, def: TRUE,  noOpt     */ 
#define WITH_GEN_STAT_COL_TXT "with stat.data recording per gen."   /* opt, def: FALSE, noOpt     */ 
#define WITH_TEXTCHR_FILE_TXT "with initial textual chrom. file"    /* opt, def: FALSE, noOpt     */
#define OPT_PROCEDURE_TXT     "optimization procedure"              /* HyGLEAM: required,if no -O */ 
#define ACCEPTANCE_RULE_TXT   "acceptance rule for offspring"       /* opt, def: localLeast-ES    */ 
#define XO_HAMDIST_TXT        "minimal hamming dist.f.XO/Reco [%]"  /* popReq, noOpt              */ 
#define RANKING_PAR_TXT       "ranking parameter"                   /* opt, def: 1.4              */ 
#define POP_SIZE_TXT          "population size"                     /* popReq                     */ 
#define DEME_SIZE_TXT         "deme size"                           /* popReq                     */ 
#define INIT_STRATEGY_TXT     "init. strategy for start population" /* required                   */ 
#define INIT_SPAR_TXT         "init. strategy parameter"            /* required                   */ 
#define NUMBER_OF_RESULTS_TXT "number of results"                   /* opt, def: 1                */ 
#define TARGET_FITNESS_TXT    "target fitness"                      /* required                   */ 
#define GEN_LIMIT_TXT         "limit for generations"               /* popReq                     */ 
#define GAC_LIMIT_TXT         "max.gen. w/o child accept. in deme"  /* popReq, noOpt              */ 
#define GDI_LIMIT_TXT         "max.gen. w/o deme improvement (GDI)" /* popReq, noOpt              */ 
#define EVAL_LIMIT_TXT        "limit for evaluations"               /* opt, def: INT_MAX          */ 
#define RUNTIME_LIMIT_TXT     "run time limit in seconds"           /* popReq                     */ 
#define BACKUP_RATE_TXT       "backup rate [generations]"           /* opt, def: INT_MAX, noOpt   */ 
#define SIMU_RES_PREC_TXT     "number of digits of (simu)parameter" /* opt, def: 7                */ 

#ifdef MPI_PAR_VERS
  #define STOP_AT_LIMIT_TXT     "EvoStop at reaching gen/eval-limit"  /* opt, def: TRUE             */
  #define PAR_UPDRATE_AK_TXT    "Update rate for best local chromos." /* opt, def: 2*Gen, noOpt     */
  #define PAR_UPDRATE_FIT_TXT   "Update rate for best fitness"        /* opt, def: 2*Gen, noOpt     */
  #define PAR_NO_UPDLIMIT_TXT   "Initial generations w/o updates"     /* opt, def: 2*Gen, noOpt     */
#endif /* MPI_PAR_VERS */


/* ------------------------- Application-specific TSK-File Parameter: --------------------------- */
#define MBF_ROT_ANGLE_TXT     "rotation angle in degrees"           /* opt, noOpt                 */
#define WITH_PHAENO_REP_TXT   "with phenotypic repair"              /* opt, def: FALSE, noOpt     */ 
#define GEN_REP_RATE_TXT      "genetic repair rate [%]"             /* opt, def: 0.0, noOpt       */ 
#define SIMU_MOD_NAME_TXT     "simulator model"                     /* required for extSimu, noOpt*/ 
#define SIMU_MAX_TIME_TXT     "timeout for ext. simulator [sec]"    /* opt, noOpt, (def)          */ 
#define SIMU_RUNS_PER_INIT_TXT "runs per (re)init. of ext.simu."    /* opt, def: 10000000, noOpt  */ 
#define WITH_SIMU_LOG_TXT     "with logging of ext.simu. interface" /* opt, def: FALSE, noOpt     */ 
#define WITH_SIMU_TIME_TXT    "with display of ext.simu. runtime"   /* opt, def: FALSE, noOpt     */
#define LSKP_R500_TXT         "with Mitsubishi R500"                /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_COLLI_TEST_TXT   "with collision test"                 /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_STOP_TEST_TXT    "with end stop test"                  /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_STOP_DEPREC_TXT  "with depreciation on stop/crash"     /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_AXES_NUMBER_TXT  "number of axes"                      /* LSKPreq, def: 5, noOpt     */
#define LSKP_CYCLE_LENGTH_TXT "simulator cycle time [sec]"          /* LSKPreq, def: 0.1 noOpt    */
#define CEC_FES_LIMIT_TXT     "CEC: limit of fct.evaluations (FEs)" /* required for CEC, noOpt    */
#define SIMU_DOMAIN_NAME_TXT  "Domain name of extern. SimuServices" /* req.for extSimuServ., noOpt*/
#define OPAL_MAX_SHIFTS_TXT   "Max. employee peak per shift"        /* OPALreq, def: 9.0,  noOpt  */


/* ----------------------- TSK-File Parameter of the HyGLEAM Extension: ------------------------- */
#define MA_WITH_ALL_IMPR_TXT  "MA with all-improvement"             /* HyGLEAM: opt, def: FALSE   */ 
#define LHC_INDEX_TXT         "LHC (Local Hill Climber) index"      /* HyGLEAM: reqAccOptProc     */ 
#define LAMARCK_RATE_TXT      "Lamarck rate [%]"                    /* HyGLEAM: opt, def: 1.0     */
#define LHC_ITER_LIMIT_TXT    "LHC iteration limit"                 /* HyGLEAM: reqAccOptProc     */
#define LHC_TERM_LIMIT_TXT    "LHC termination threshold"           /* HyGLEAM: reqAccOptProc     */
#define LHC_ADAPT_SPEED_TXT   "speed of LHC adaption"               /* HyGLEAM:reqAccOptProc,noOpt*/
#define LEVEL_ADAPT_SPEED_TXT "speed of level adaption"             /* HyGLEAM:reqAccOptProc,noOpt*/
#define NUMB_OF_FCLASSES_TXT  "number of fitness classes"           /* HyGLEAM:reqAccOptProc,noOpt*/
#define OLD_DISTR_FRAC_TXT    "fraction of old LHC-distribution[%]" /* HyGLEAM: reqAccOptProc     */


/* ---------------------- TSK-File Parameter of the command line version: ----------------------- */
#define WITH_DETAILED_LOG_TXT "with detailed logging"               /* opt, def: TRUE             */ 
#define WITH_RES_CHR_SIMU_TXT "with result simulation"              /* opt, def: FALSE, noOpt     */ 
#define WITH_RES_CHR_SAVE_TXT "with saving of result chromosomes"   /* opt, def: FALSE, noOpt     */ 
#define WITH_TERM_TEST_TXT    "with job termination tests"          /* opt, def: FALSE, noOPt     */ 
#define TERM_TEST_FREQ_TXT    "termination test rate [pairings]"    /* opt, def: 500, noOPt       */ 


/* ------------------ TSK-File Parameter: Values of the Enumeration Types: ---------------------- */
#define OPT_PROC_GLEAM_TXT     "GLEAM"                          /* valid optimization procedures: */
#define OPT_PROC_SMA_TXT       "SMA"
#define OPT_PROC_ASMA_TXT      "ASMA"
#define OPT_PROC_AMMA_TXT      "AMMA"
#define OPT_PROC_LSV_TXT       "LHC"
#define ACCEPT_RULE_ALW_TXT    "always"                  /* valid rules for offspring acceptance: */
#define ACCEPT_RULE_LL_TXT     "localLeast"
#define ACCEPT_RULE_BP_TXT     "betterParent"
#define ACCEPT_RULE_ALW_ES_TXT "always-ES"
#define ACCEPT_RULE_LL_ES_TXT  "LocalLeast-ES"
#define SPEED_SLOW_TXT         "slow"                                 /* valid adaptation speeds: */
#define SPEED_MEDIUM_TXT       "medium"
#define SPEED_FAST_TXT         "fast"
/* End of TSK-File-Parameter */


/* --------------------------------- Texte des Moduls: tsk_data --------------------------------- */
#define HDR_LINE_TXT           "--------------------------"
#define HDR_TITLE_TEXT         "Program Parameter File (TSK file) -"
#define LSK_SECTION_HDR_TXT    "---------------- start- and target-data for LESAK-application: ----------------"
#define BAD_VAL_MSG_TXT        "Error in value specification for parameter \"%s\": \"%s\""
#define CUT_OFF_LEADING_BLANKS "leading blanks were cut off"
#define CUT_OFF_TRAIL_BLANKS   "trailing blanks were cut off"
#define SUBSTITUTE_BLANKS_TXT  "%d inner blanks were substituted by \"_\""
#define ONLY_BLANKS_TXT        "the name consisted only of blanks and is empty now"
#define UNSUITED_PARAM_TXT     "Not meaningful specification of a %s for GLEAM and variants ignored!"


/* --------------------------------- Texte des Moduls: aufg_gsw --------------------------------- */
#define STND_WERTE         "standard "
#define APPL_WERTE         "application specific "
#define AUFG_PAR_HDR       "     Program-Parameter                 Value"
#define AUFG_PAR_XHDR      "No. Program-Parameter                     Value"
#define MOD_NIX_DA_TXT     "SimModel \"%s\" not available in \"%s%s%s\""


/* --------------------------------- Texte des Moduls: aufgcmen --------------------------------- */
#define AUFG_MEN1          "ShowParam"
#define AUFG_MEN2          "ChngParam"
#define AUFG_MEN3          "All Param"
#define AUFG_MEN5_SIMOT    "ShowSimMod"
#define AUFG_MEN6_SIMOT    "ChngSimMod"
#define AUFG_MEN5_LESAK    "ShowStartD"
#define AUFG_MEN6_LESAK    "Teach-in"
#define AUFG_MEN8_LESAK    "Obstacles"
#define PARAM_ANZ_TXT      "There are a total of %d defined program parameters."
#define AUFG_ALL_PAR_TITLE "    List of All Program Parameters:"
#define AUFG_PAR_TITLE     "List of all editable Program Parameters:"
#define AUFG_PAR_XTITLE    " Modification of current Program Parameters:"
#define START_ZIEL_HDR     "                      Start and Destination:"
#define AUFG_PIDX_QUERY    "parameter number (%d..%d): "
#define AKT_MOD_HDR_TXT    "           Present Simulation Model:"
#define SIM_MOD_OK_TXT     "available"
#define NO_SIM_MOD_TXT     "not available"
#define NEU_MOD_HDR_TXT    "             New Simulation Model:"
#define AKT_MODELL_TXT     "Present model: %s (%s)"
#define NEU_MOD_QUERY      "Modell name (<CR>=no change): "
#define NIX_NEU_MOD_TXT    "Simulation model remains unchanged"
#define KEIN_MODELL_TXT    "No simulation model available!"
#define MOD_INDEX_QUERY    "Index of simulation model (1..%d): "
#define NEU_MOD_ANZEIGE    "New model    : %s (available)"
#define CHANGE_WARNING_TXT "Changed program parameters stored and processed."
