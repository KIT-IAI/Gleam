/* -------------------------------------------------------------------------------------------------
GLEAM/AE                            Sprach-Header-File (Deutsch)  
Package: aufg                              File: aufg_d.h                        Version:     V2.0.5
Status : o.k.                             Autor: W.Jakob                           Datum: 28.08.2020
----------------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, see the 
comment section at the end of the main program files or the files "schalter.h".
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

/* ------------------------- TSK-File-Parameter aller GLEAM-Versionen: -------------------------- */
#define WITH_RANDGEN_INIT_TXT "Mit Init. d.Zufallszahlengenerators" /* opt, def: TRUE,  noOpt     */
#define WITH_REC_EVO_BEST_TXT "Merke Daten des Evolutionsbesten"    /* opt, def: TRUE,  noOpt     */
#define WITH_STAT_DAT_COL_TXT "Mit Statistikdaten-Sammlung"         /* opt, def: TRUE,  noOpt     */
#define WITH_GEN_STAT_COL_TXT "Mit StatDatenSammlung je Generation" /* opt, def: FALSE, noOpt     */
#define WITH_TEXTCHR_FILE_TXT "Mit initialem textuellem Chr.File"   /* opt, def: FALSE, noOpt     */
#define OPT_PROCEDURE_TXT     "Optimierungsverfahren"               /* HyGLEAM: required,if no -O */
#define ACCEPTANCE_RULE_TXT   "Akzeptanzregel fuer Nachkommen"      /* opt, def: localLeast-ES    */
#define XO_HAMDIST_TXT        "Mindesthammingabstand f.XO/Reko [%]" /* popReq, noOpt              */
#define RANKING_PAR_TXT       "Ranking-Parameter"                   /* opt, def: 1.4              */
#define POP_SIZE_TXT          "Populationsgroesse"                  /* popReq                     */
#define DEME_SIZE_TXT         "Nachbarschaftsgroesse"               /* popReq                     */
#define INIT_STRATEGY_TXT     "InitStrategie f.die Startpopulation" /* required                   */
#define INIT_SPAR_TXT         "InitStrategie-Parameter"             /* required                   */
#define NUMBER_OF_RESULTS_TXT "Anzahl der Ergebnis-Chromosome"      /* opt, def: 1                */
#define TARGET_FITNESS_TXT    "Mindest-Zielfitness"                 /* required                   */
#define GEN_LIMIT_TXT         "Maximale Generationsanzahl"          /* popReq                     */
#define GAC_LIMIT_TXT         "Max. Gen. ohne Akzeptanz im Deme"    /* popReq, noOpt              */
#define GDI_LIMIT_TXT         "Max. Gen. ohne Deme-Verbesserung"    /* popReq, noOpt              */
#define EVAL_LIMIT_TXT        "Maximale Anzahl der Evaluationen"    /* opt, def: INT_MAX, noOpt   */
#define RUNTIME_LIMIT_TXT     "Maximale Laufzeit [sec]"             /* popReq                     */
#define BACKUP_RATE_TXT       "Backup rate [Generationen]"          /* opt, def: INT_MAX, noOpt   */
#define SIMU_RES_PREC_TXT     "Stellenanzahl der (Simu)-Parameter"  /* opt, def: 7, noOpt         */

#ifdef MPI_PAR_VERS
  #define STOP_AT_LIMIT_TXT     "EvoStop bei Gen/Eval-Limit-Erreich." /* opt, def: TRUE             */
  #define PAR_UPDRATE_AK_TXT    "Update-Rate f. bestes lokales Chr."  /* opt, def: 2*Gen, noOpt     */
  #define PAR_UPDRATE_FIT_TXT   "Update-Rate fuer beste Fitness"      /* opt, def: 2*Gen, noOpt     */
  #define PAR_NO_UPDLIMIT_TXT   "AnfangsGen-Anzahl ohne Updates"      /* opt, def: 2*Gen, noOpt     */
#endif /* MPI_PAR_VERS */


/* --------------------------- Anwendungsbezogene TSK-File-Parameter: --------------------------- */
#define MBF_ROT_ANGLE_TXT     "Drehwinkel in Grad"                  /* opt, noOpt                 */
#define WITH_PHAENO_REP_TXT   "Mit phaenotypischer Reparatur"       /* opt, def: FALSE, noOpt     */
#define GEN_REP_RATE_TXT      "Genetic-Repair-Rate [%]"             /* opt, def: 0.0, noOpt       */
#define SIMU_MOD_NAME_TXT     "Simulatormodell"                     /* required for extSimu, noOpt*/
#define SIMU_MAX_TIME_TXT     "Maximale Zeit pro ExtSimu-Lauf[sec]" /* opt, noOpt, (def)          */
#define SIMU_RUNS_PER_INIT_TXT "Laeufe pro (Re)Init. des ExtSimu"   /* opt, def: 10000000, noOpt  */
#define WITH_SIMU_LOG_TXT     "Mit Logging d.ExtSimu-Schnittstelle" /* opt, def: FALSE, noOpt     */
#define WITH_SIMU_TIME_TXT    "Mit ExtSimu-Rechenzeit-Anzeige"      /* opt, def: FALSE, noOpt     */
#define LSKP_R500_TXT         "Mit Mitsubishi R500"                 /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_COLLI_TEST_TXT   "Mit Kollisionstest"                  /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_STOP_TEST_TXT    "Mit Anschlagtest"                    /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_STOP_DEPREC_TXT  "Mit Abwertung bei Anschlag"          /* LSKPreq, def: TRUE, noOpt  */
#define LSKP_AXES_NUMBER_TXT  "Achsanzahl"                          /* LSKPreq, def: 5, noOpt     */
#define LSKP_CYCLE_LENGTH_TXT "Simulatortakt in Sekunden"           /* LSKPreq, def: 0.1,  noOpt  */
#define CEC_FES_LIMIT_TXT     "CEC: Limit d. FktEvaluationen (FEs)" /* required for CEC, noOpt    */
#define SIMU_DOMAIN_NAME_TXT  "Domainname der externen SimuDienste" /* req.for extSimuServ., noOpt*/
#define OPAL_MAX_SHIFTS_TXT   "Soll-SchichtspitzenMax"              /* OPALreq, def: 9.0,  noOpt  */


/* ------------------------ TSK-File-Parameter der HyGLEAM-Erweiterung: ------------------------- */
#define MA_WITH_ALL_IMPR_TXT  "MA mit all-Verbesserung"             /* HyGLEAM: opt, def: FALSE   */
#define LHC_INDEX_TXT         "LSV-Index"                           /* HyGLEAM: reqAccOptProc     */
#define LAMARCK_RATE_TXT      "Lamarck-Rate [%]"                    /* HyGLEAM: opt, def: 1.0     */
#define LHC_ITER_LIMIT_TXT    "LSV-Iterationslimit"                 /* HyGLEAM: reqAccOptProc     */
#define LHC_TERM_LIMIT_TXT    "LSV-Abbruchschranke"                 /* HyGLEAM: reqAccOptProc     */
#define LHC_ADAPT_SPEED_TXT   "LSV-Adaptionsgeschwindigkeit"        /* HyGLEAM:reqAccOptProc,noOpt*/
#define LEVEL_ADAPT_SPEED_TXT "Level-Adaptionsgeschwindigkeit"      /* HyGLEAM:reqAccOptProc,noOpt*/
#define NUMB_OF_FCLASSES_TXT  "Notenklassenanzahl"                  /* HyGLEAM:reqAccOptProc,noOpt*/
#define OLD_DISTR_FRAC_TXT    "Anteil der alten LSV-Verteilung [%]" /* HyGLEAM: reqAccOptProc     */


/* ---------------------- TSK-File-Parameter der Kommandozeilenversionen: ----------------------- */
#define WITH_DETAILED_LOG_TXT "Mit detaliertem Logging"             /* opt, def: TRUE             */
#define WITH_RES_CHR_SIMU_TXT "Mit Ergebnis-Simulation"             /* opt, def: FALSE, noOpt     */
#define WITH_RES_CHR_SAVE_TXT "Mit Rettung der ErgebnisChromosomen" /* opt, def: FALSE, noOpt     */
#define WITH_TERM_TEST_TXT    "Mit Tests auf Jobabbruch"            /* opt, def: FALSE, noOPt     */ 
#define TERM_TEST_FREQ_TXT    "Abbruch-Testrate [Paarungen]"        /* opt, def: 500, noOPt       */


/* ---------------------- TSK-File-Parameter: Werte der Enumerationstypen: ---------------------- */
#define OPT_PROC_GLEAM_TXT     "GLEAM"                /* valid optimization procedures: */
#define OPT_PROC_SMA_TXT       "SMA"
#define OPT_PROC_ASMA_TXT      "ASMA"
#define OPT_PROC_AMMA_TXT      "AMMA"
#define OPT_PROC_LSV_TXT       "LSV"
#define ACCEPT_RULE_ALW_TXT    "always"        /* valid rules for offspring acceptance: */
#define ACCEPT_RULE_LL_TXT     "localLeast"
#define ACCEPT_RULE_BP_TXT     "betterParent"
#define ACCEPT_RULE_ALW_ES_TXT "always-ES"
#define ACCEPT_RULE_LL_ES_TXT  "localLeast-ES"
#define SPEED_SLOW_TXT         "langsam"                    /* valid adaptation speeds: */
#define SPEED_MEDIUM_TXT       "mittel"
#define SPEED_FAST_TXT         "schnell"
/* End of TSK-File-Parameter */


/* --------------------------------- Texte des Moduls: tsk_data --------------------------------- */
#define HDR_LINE_TXT           "--------------------------"
#define HDR_TITLE_TEXT         "Programmparameter Datei (TSK file) "
#define LSK_SECTION_HDR_TXT    "---------------- Start- und Zielwerte fuer die LESAK-Anwendung: ----------------"
#define BAD_VAL_MSG_TXT        "Fehler in Wertangabe bei Parameter \"%s\": \"%s\""
#define CUT_OFF_LEADING_BLANKS "F"ue"hrende Leerzeichen wurden entfernt"
#define CUT_OFF_TRAIL_BLANKS   "Nachfolgende Leerzeichen wurden entfernt"
#define SUBSTITUTE_BLANKS_TXT  "%d innere Leerzeichen wurden durch \"_\" ersetzt"
#define ONLY_BLANKS_TXT        "Der Name bestand nur aus Leerzeichen und ist jetzt leer"
#define UNSUITED_PARAM_TXT     "Nicht sinnvolle Angabe eines %s bei GLEAM und Varianten ignoriert!"


/* --------------------------------- Texte des Moduls: aufg_gsw --------------------------------- */
#define STND_WERTE         "Standard-"
#define APPL_WERTE         "anwendungsspezifischen "
#define AUFG_PAR_HDR       "     Programmparameter                 Wert"
#define AUFG_PAR_XHDR      "Nr. Programmparameter                     Wert"
#define MOD_NIX_DA_TXT     "Sim-Modell \"%s\" nicht verf"ue"gbar in \"%s%s%s\""

/* --------------------------------- Texte des Moduls: aufgcmen --------------------------------- */
#define AUFG_MEN1          "ZeigParam"
#define AUFG_MEN2          AE"ndParam"
#define AUFG_MEN3          "Alle Param"
#define AUFG_MEN5_SIMOT    "ZeigSimMod"
#define AUFG_MEN6_SIMOT    AE"ndSimMod"
#define AUFG_MEN5_LESAK    "ZeigStartZ"
#define AUFG_MEN6_LESAK    "Teach-in"
#define AUFG_MEN8_LESAK    "Hindernis"
#define PARAM_ANZ_TXT      "Es sind insgesamt %d Programmparameter definiert."
#define AUFG_ALL_PAR_TITLE "  Liste aller Programmparameter:"
#define AUFG_PAR_TITLE     "Liste aller editierbaren Programmparameter:"
#define AUFG_PAR_XTITLE    AE"nderung aktueller Programmparameter:"
#define START_ZIEL_HDR     "                    Werte von Start und Ziel:"
#define AUFG_PIDX_QUERY    "Parameternummer (%d..%d): "
#define AKT_MOD_HDR_TXT    "         Aktuelles Simulationsmodell:"
#define SIM_MOD_OK_TXT     "verf"ue"gbar"
#define NO_SIM_MOD_TXT     "nicht verf"ue"gbar"
#define NEU_MOD_HDR_TXT    "           Neues Simulationsmodell:"
#define AKT_MODELL_TXT     "Aktuelles Modell: %s (%s)"
#define NEU_MOD_QUERY      "Modellname (<CR>=keine "AE"nderung):"
#define NIX_NEU_MOD_TXT    "Simulationsmodell bleibt unver"ae"ndert."
#define KEIN_MODELL_TXT    "Kein Simuationsmodell verf"ue"gbar!"
#define MOD_INDEX_QUERY    "Index des Simulationsmodells (1..%d):"
#define NEU_MOD_ANZEIGE    "Neues Modell    : %s (verf"ue"gbar)"
#define CHANGE_WARNING_TXT "Ge"ae"nderte Programmparameter "ue"bernommen.\n"
