#/* --------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Englisch)
Package: simu                         File: simu_gb.h                  Version:     V2.1.7
Status : o.k.                        Autor: W.Jakob                      Datum: 25.08.2020
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
--------------------------------------------------------------------------------------- */

#define STRICH_15              "---------------"


/* ----------------------------- Texte des Moduls: ext_sim ---------------------------- */
#define MATLAB_SIMU_TXT        "Matlab/Matpower simulator"
#define TERMINATED_TXT         "terminated"


/* ---------------------------- Texte des Moduls: xxxxkopl ---------------------------- */
#define SIMUSTART_TXT1         "External simulator startet at %s%s ..."
#define SIMUSTART_TXT2         " using model \"%s\""
#define SIMU_RZ_TXT            "Elapsed simulation time = %6.2f sec"
#define NO_LICENCE_TXT         "No unused licence available!"
#define GEN_EXT_SIMU_TXT       "general external simulator"


/* ---------------------------- Texte des Moduls: matlab_koppl ------------------------ */
#define MODEL_CHANGED_TXT      "Simulator model \"%s\" substituted by \"%s\"."


/* ---------------------------- Texte des Moduls: extSimuServKoppl -------------------- */
#define TOO_MANY_RESLI_TXT     "Too many entries in result list: %d"
#define BAD_RESLI_AMOUNT_TXT   "Bad result list. Status: %d"
#define WORKFLOW_START_RES_TXT "Start of external simulation services with model \"%s\" "
#define WORKFLOW_TERM_RES_TXT  "Termination of external simulation services with task ID %d "
#define WF_ACTION_DONE_TXT     "done."
#define WF_ACTION_FAILED_TXT   "failed!"
#define NO_MODEL_NO_START_TXT  "No start of the external simulation service with empty model name!"
#define CHR_LIST_HDR           "chromosome list header"


/* ---------------------------- Texte des Moduls: simu -------------------------------- */
#define EXP_FILE_ANZG_TXT      "Experiment        : "
#define MOD_FILE_ANZG_TXT      "Gene model        : "
#define BEW_FILE_ANZG_TXT      "Evaluation        : "
#define TSK_FILE_ANZG_TXT      "Program parameters: "
#define EVO_FILE_ANZG_TXT      "Evo-parameters    : "
#define CH_FILE_ANZG_TXT       "Chromosome file   : "
#define TOP_FILE_ANZG_TXT      "Topology file     : "
#define ADD_FILE_ANZG_TXT      "Additional application-specific files:"


/* ---------------------------- Texte des Moduls: simucmen ---------------------------- */
#define SIM_MEN_START_TXT      "SimuStart"
#define SIM_MEN_FIN_TXT        "ExtSimQuit"
#define SIM_MEN_RESTART_TXT    "ExtSimStrt"
#define SIM_MEN_RES_TXT        "SaveSimRes"
#define SIM_MEN_BEW_TXT        "ShowEval"
#define SIM_MEN_LOG_TXT        "Log.on/off"
#define SIM_MEN_EVO_BEST_TXT   "EvoBest"
#define SIM_MEN_STATUS_TXT     "State"
#define SIM_MEN_PLOT_TXT       "PlotEval"
#define SIM_MEN_MODELLIST_TXT  "ModelSelec"
#define SIM_MEN_CASE_FILE_TXT  "CaseFile"
#define SIM_MEN_DEL_SIMVAL_TXT "DelSimVal"
#define SIM_MEN_NEU_BEW_TXT    "MemEvalUpd"
#define SIMU_HDR_TXT           "    Simulation of a Chromosome:"
#define SIMU_HDR2_TXT          "              Simulation Results for Chromosome %s:"
#define EXT_SIMU_FIN_HDR_TXT   "           Terminate External Simulator:"
#define EXT_SIMU_FIN_QUERY_TXT "Really terminate simulator ? "NEIN_JA_TXT
#define EXT_SIMU_RESTART_TXT   "            (Re-)Start of External Simulator:"
#define EXT_SIMU_STATUS_TXT    "Simulator state is \"activ\"!"
#define SIMU_RESTART_QUERY_TXT "(Re-)Start ext. simulator nevertheless? "NEIN_JA_TXT
#define EXT_SIMU_RESTART_QUERY "I=Init & start   restart: P=Partial  C=Complete: "
#define EXT_SIMU_KL_RESTART_CH 'P'
#define EXT_SIMU_GR_RESTART_CH 'C'
#define EXT_SIMU_RECOVERY_CH   'I'
#define EXT_SIMU_NIX_TUN_TXT   "OK! Nothing done."
#define EXT_SIMU_DO_RESTRT_TXT "Complete (re-)start ..."
#define EXT_SIMU_RESTART_DONE  "Simulator (re-)start was successful!"
#define NIX_EXT_SIMU_TXT       "The (external) simulator is not available!"
#define SIMU_LOGFILE_QUERY_TXT "Simulator test logging "
#define NIX_LOG_TXT            "No simulator log file in this version!"
#define SIM_PLOT_HEADER_TXT    "    Plot of the Evaluation of the Recent Simulation"
#define EVO_PLOT_HEADER_TXT    "      Plot of the Evaluation of the Recent EvoBest"
#define NIX_PLOT_DATA_TXT      "No simulation data for plotting available!"
#define SAVE_ERG_HDR           "          Save of Recent Simulation Results"
#define KRIT_IND_QUERY_TXT     "Index of criterion: "
#define KRIT_IND_QUERY2_TXT    "Index of criterion ["
#define HARDCOPY_QUERY_TXT     "Hardcopy ? "NEIN_JA_TXT
#define AK_SIM_LOE_HDR_TXT     "      Deletion of the Simulation Results of a Chromosome"
#define SIM_VAL_DEL_TXT        "Simulation results of chromosome deleted."
#define NEUBEWERT_HDR1         "   Update of the Evaluation of the Entire Chromosome Memory"
#define NEUBEWERT_HDR2         "        Based on Present Task and Evaluation Criteria"
#define NEUBEW_SIM_LOE_QUERY   "Deletion of the stored simulator results? "JA_NEIN_TXT
#define NEUBEW_TO_DO_TXT       "Evaluation of %1d chromosomes will be updated ..."
#define NEUBEW_NIX_DONE_TXT    "No evaluation update done! (No external simulator)"
#define EXTSIM_MODLIST_HDR_TXT "List of models of external simulation services:"
#define EXTSIM_MOD_AUSW_TXT    "Model selection (1..%d): "
#define EXTSIM_NO_COM_TXT      "No communication with the external simulation services possible! Check status"
#define MATLAB_CASEF_HDR_TXT   "     Creation of a MatPower case-file"
#define MATLAB_CASEF_QUERY_TXT "With creation of a MatPower case-fil? "NEIN_JA_TXT
#define MATLAB_CASEF_ON_TXT    "Until the Simu menu is exited, all subsequent simulations create a MatPower case file!\n"

/* --------------------------- Texte des Moduls: simu_gsw: ---------------------------- */
#define AK_ADR_BLEIBT_TXT      "Chromosome address remains %s"
#define AK_ADR_NEU_TXT         "New chromosome address: %s"
#define NOTE_UNVERANDERT_TXT   "Chromosome remains uncanged."
#define NOTE_SCHLECHT_TXT      "New fitness %ld is invalid."
#define NO_SIMU_DONE_TXT       "No simulation done!"
#define NOTE_BLEIBT_TXT        "Fitness remains unchanged: %ld (%.9f)"
#define NOTE_NEU_TXT           "New fitness: %ld (%.9f)"
#define ZEIG_BEW_HDR_TXT       "                    Evaluation of the Recent Simulation:"
#define ZEIG_EVO_BEW_HDR_TXT   "       Evaluation of the Best Solution of the Actual Optimization Run:"
#define ERG_HDR                "===== %s. Results from: %s =====\n\n"
#define EXP_FILES_HDR          "Loaded Files:\n=============\n"
#define NO_EVO_DONE_TXT        "No evolution done!"
#define PARAM_HDR_TXT          "             %s:"
#define STRICH_TXT             STRICH_40_TXT"--------------"
#define BEWERT_TITLE1_TXT      "Objective           Prio Wght Unwght. --- Weighted ---  Simulator Results"
#define BEWERT_TITLE2_TXT      "                          [%] Fitness MinFitn. Fitness"
#define BEWERT_ITITLE1_TXT     "No. "
#define BEWERT_ITITLE2_TXT     "    "
#define ROH_SUMME_TXT          "Total fitness (without penalty functions):      "
#define STRAF_TITLE_TXT        "Penalty functions (in brackets: lack of prio):"
#define STRAF_FAKTOR_TXT       "factor"
#define KEINE_STRAFE_TXT       "No penalty functions applied."
#define ERG_NOTE_TXT           "Total fitness:                                  "
#define ERG_UNTERSTRICH_TXT    "                                                ======"
#define STATUS_ANZG_HDR_TXT    "                Simulator State:"
#define STATUS_ANZG_SIM1_TXT   "External simulator started  (sim_ok) = %s"
#define STATUS_ANZG_SIM2_TXT   "External simulator ready    (sim_up) = %s"
#define STATUS_ANZG_SIM3_TXT   "Internal simulator ready    (sim_up) = %s"
#define STATUS_ANZG_MP1_TXT    "External Matlab simulator ready         (sim_ok)     = %s"
#define STATUS_ANZG_MP2_TXT    "Ext.Sim. with model \"%s\" started %*s(sim_up) = "
#define STATUS_ANZG_MP3_TXT    "Simulator logging (nearly not impl. with Matlab)     = "
#define STATUS_ANZG_MP4_TXT    "Each simulator run creates a MatPower case-file      = %s"
#define STATUS_ANZG_MP5_TXT    "Identifier of a Matlab/MatPower application          = \"%s\""
#define STATUS_ANZG_AKTIV_TXT  "active"
#define STATUS_ANZG_TOT_TXT    "dead"
#define SIM_OK_TXT             "sim_ok"
#define SIM_UP_TXT             "sim_up"
#define STATUS_ANZG_LOG_TXT    "Simulator logging                    = "
#define STATUS_ANZG_AUS_TXT    "off"
#define STATUS_ANZG_ESS1_TXT   "External Simulation Services:"
#define STATUS_ANZG_ESS2_TXT   "URL beginning: \"%s\""
#define STATUS_ANZG_ESS3_TXT   "Communication package ready%*s (sim_ok) = "
#define STATUS_ANZG_ESS4_TXT   "Model \"%s\" loaded   %*s (sim_up) = "
#define STATUS_ANZG_ESS5_TXT   "Logging of the simulator interface %*s  = "
#define STATUS_ANZG_ESS6_TXT   "  Work URL: \"%s\""
#define PLOT_AKT_WERT_TXT      "present value"
#define NEUBEWERT_AK_DEL_TXT   "%1d. not evaluatable chromosome %s deleted."
#define NEUBEWERT_ERG_TXT      "Evaluation of %1d chromosomes updated."
#define NO_PARAMS_WITHOUT_SIMU "The display of the %s is possible only after previous simulation."


/* ---------------------------- Texte des Moduls: par_simu ---------------------------- */
#define NOT_SIMULATED_TXT      "not simulated"


/* ---------------------------- Texte des Moduls: mbf_sim ----------------------------- */
#define X_VAL_TXT              "Buffer foe X-values"


/* ---------------------------- Texte des Moduls: rob_sim ----------------------------- */
#define NO_KIN_LOADED_TXT      "No kinematics loaded!"
#define JUMP_DONE_TXT          "%d actions skipped."
#define TAKT_ERLEDIGT_TXT      "***  Stroke %3d performed, time = %4.1f sec  ***"
#define TAKT_ERLEDIGT2_TXT     "***"TAKT_ERLEDIGT_TXT
#define AK_FIN_TXT             "********** End of AC interpretation due to: "
#define AK_ENDE_TXT            "End of AC encountered."
#define AK_WUERG_TXT           "Collision or axis impact."
#define AK_FEHLER_TXT          "Interpretation error."
#define AUSLAUFEN_TXT          "*** \"running out / breaking \" of motors:"
#define SIMU_ENDE_TXT          "********* End of Simulation *********"
#define SIMU_WUERG_QUERY_TXT   "Abort simulation ? "NEIN_JA_TXT
#define SIMU_WUERG_TXT         "*** SIMULATION ABORTED !"
#define SIMU_NOT_WUERG_TXT     "Simulation continued ..."
#define SIMU_WUERG_LOG_TXT     "******  Stroke %5d: Simulation stopped !!!"
#define ZIEL_ABW_ERG_LOG_TXT   "Evaluation: target devi.: rest_weg=%g bahndistanz=%g res=%g"
#define BAHN_ABW_ERG_LOG_TXT   "Evaluation: path deviat.: bges=%g bahndistanz=%g rwert=%g"
#define ENERGIE_ERG_LOG_TXT    "Evaluation: energy cons.: summedv=%g summeda=%g rwert=%g"
#define GES_ERG_ALLG_TXT       "Result: distance=%4.2f  target dev.=%4.2f"
#define GES_ERG_BAHN_TXT       "  BGes=%4.2f"
#define GES_ERG_ENERG_TXT      "  SDV=%4.2f SDA=%4.2f"
#define ACHS_KOLL_TXT          "Collision of axis %d"
#define GREIFER_KOLL_TXT       "Collision of gripper"
#define SENSOR_ERR_TXT         "BEAM DISTANCE: Bad sensor code: %d"
#define ZUVIEL_ACHS_TXT        "Superfluous axis specification ignorded  (max=%d)."
#define ACHS_OK_TXT            "      %d axes successfully read."
#define ACHS_FEHLER_TXT        "Error at axis %d!"
#define ACHSANZ_UNGL_TXT       "Different number of axes TSK:%d KIN:%d"
#define KIN_ERR_TXT            "      Error in KIN-File. "NO_KIN_LOADED_TXT
#define MITSU_LOAD_TXT         "      Model of the Mitsubishi R500 with 5 axes loaded."
#define NO_HIND_LINE_TXT       "\"Hindernis ...\"-line missing!"
#define HIND_OK_TXT            "      %d obstacles successfully read."
#define HIND_ERR_TXT           "      Error in OBS-File. No obstacles loaded!"
#define HIND_FEHLER_TXT        "Error at %dth obstacle definition!"
#define NIX_MOTOR_TXT          "Motor %d does not exist!"
#define AK_ERROR_TXT           "Error in AC: "
#define BAD_POS_VGL_OP_TXT     AK_ERROR_TXT"Bad PosCompOperator!"
#define BAD_GESCHW_VGL_OP_TXT  AK_ERROR_TXT"Bad SpeedCompOp!"
#define BAD_BESCHL_VGL_OP_TXT  AK_ERROR_TXT"Bad AccelerationCompOp!"
#define BAD_RAMP_VAL_TXT       AK_ERROR_TXT"Ramp < 0! Set to 0.1 !"
#define BAD_AK_ELEM_TXT        AK_ERROR_TXT"No Header allowed!"
#define NO_SUB_AK_TXT          AK_ERROR_TXT"Sub-AC reference not implemented!"
#define TCP_VERGL_LOG_TXT      "TCP-Comp: Distance=%4.1f%s%5.2f"
#define MOTOR_AUS_LOG_TXT      "Motor %d off, Ramp=%4.2f"
#define MOTOR_AN_LOG_TXT       "Motor %d:  U=%4.2f Ramp=%4.2f"
#define DREHE_ABST_SENS_LOGTXT "Outer DistSensor turned to  %4.2f degrees."
#define KONTAKT_QUERY_TXT      "Contact at sensor %d ? [j/n]: "
#define KONTAKT_JA_LOG_TXT     "On contact: Contact at sensor %s: %s actions skipped."
#define KONTAKT_NEIN_LOG_TXT   "On contact: No contact at sensor %s: No actions skipped."
#define VGL_WAHR_LOG_TXT       "CompTrue: %d actions skipped."
#define START_BLOCK_ANF_LOGTXT "Begin of StartBlock"
#define START_BLOCK_END_LOGTXT "End of StartBlock"
#define UNVERAENDERT_LOG_TXT   "Unchanged for  %d Strokes."
#define STORE_SENS_ERR_TXT     "Store_sens: Bad numbers: SensorNo=%d SensorVarNo=%d Max=%d"
#define STORE_SENS_LOG_TXT     "StoreSens: Sens%d Var%d Value = %3.1f"
#define SET_TIME_ERR_TXT       "Set_Time: Bad RegisterNo.:%d. Max=%d"
#define SET_TIME_LOG_TXT       "Set_Time: TimeRegister%d = %4.1f sec."
#define STORE_TIME_ERR_TXT     "Store_Time: Bad RegisterNo.:%d. Max=%d"
#define STORE_TIME_LOG_TXT     "Store_Time: TimeRegister%d = %4.1f sec."
#define BAD_AKTION_TXT         "Unknown action! Code=%d"
#define ACHS_AM_ANSCHLAG_TXT   "Impact of axis %d !"
#define TAKT_ZEIT_TXT          "%3dth Stroke                                       Time: %4.1f sec"
#define ZWISCHEN_ERG_TXT1      "-------------- Values of all axes: --------------\n"
#define ZWISCHEN_ERG_TXT2      "  Axis %2d:  U =%7.2f  Ramp =%7.2f  TargetU =%7.2f\n"
#define ZWISCHEN_ERG_TXT3      "  Interpolated robotjoints:\n   "
#define ZWISCHEN_ERG_TXT4      "  Position : %5.1f,%5.1f,%5.1f\n"
#define ACHS_POS_VGL_LOG_TXT   "Compare axis %d: Position %s %5.2f"
#define ACHS_GESCHW_VGL_LOGTXT "Compare axis %d: Speed %s %d"
#define ACHS_BESCHL_VGL_LOGTXT "Compare axis %d: Accel. %s %d"
#define BAD_ZEIT_VGL_OP_TXT    AK_ERROR_TXT"Bad TimeCompOp!"
#define BAD_ZEIT_REG_NR_TXT    "Comp_Time: Bad RegisterNo: %d Max=%d"
#define ZEIT_VGL_LOG_TXT       "TimeComp: Var%d Value=%3.1f %s %3.1f"
#define BAD_SENS_VGL_OP_TXT    AK_ERROR_TXT"Bad SensCompOp!"
#define SENS_VGL_LOG_TXT       "Comp_Sens: Sens%d Var%d: %3.1f %s %3.1f"
#define BAD_SENS_REG_NR_TXT    "Comp_Sens: Bad Sensor/RegisterNo: %d/%d Max=%d"
#define ABST_VGL_LOG_TXT       "CompGripper"
#define ABST_AUSSEN_TXT        "External"
#define ABST_ACHSE_TXT         " Axis=%d"
#define ABST_KEIN_KONTAKT_TXT  " no contact"


/* ---------------------------- Texte des Moduls: rob_gsw ----------------------------- */
#define START_POS_TXT          "Start values:"
#define ZIEL_POS_TXT           "Destination values:"
#define POS_TXT                "  Pos.:  "
#define JOINTS_TXT             "  Joints:"
#define OFFSET_TXT             "         "


/* ---------------------------- Texte des Moduls: rob_dial ---------------------------- */
#define TEACH_IN_HDR_TXT       "                 New axis values (Teach-in):"
#define ALTE_WERTE_TXT         "Old values:"
#define AKT_WERTE_TXT          "Present values:"
#define NEUE_WERTE_TXT         "New values:"
#define START_QUERY_TXT        "Change values of start position? "JA_NEIN_TXT
#define ZIEL_QUERY_TXT         "Change values of target position? "JA_NEIN_TXT
#define ACHS_QUERY_TXT         "New value for axis %2d [%6.1f]: "


