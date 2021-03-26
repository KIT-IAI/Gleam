/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Deutsch) 
Package: simu                         File: simu_d.h                   Version:     V2.1.7
Status : o.k.                        Autor: W.Jakob                      Datum: 05.08.2020
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------ */

#define STRICH_15              "---------------"


/* ---------------------------- Texte des Moduls: ext_sim ----------------------------- */
#define MATLAB_SIMU_TXT        "Matlab/Matpower-Simulator"
#define TERMINATED_TXT         "terminiert"
#define STARTED_TXT            "gestartet"


/* ---------------------------- Texte des Moduls: xxxxkopl ---------------------------- */
#define SIMUSTART_TXT1         "Ext.Simu. um %s%s hochgefahren ..."
#define SIMUSTART_TXT2         " mit Modell \"%s\""
#define SIMU_RZ_TXT            "Simulator-Laufzeit=%6.2f sec"
#define NO_LICENCE_TXT         "Keine freie Lizenz mehr verf"ue"gbar!"
#define GEN_EXT_SIMU_TXT       "Allgemeiner externer Simulator"


/* ---------------------------- Texte des Moduls: matlab_koppl ------------------------ */
#define MODEL_CHANGED_TXT      "Simulatormodell \"%s\" durch \"%s\" ersetzt."


/* ---------------------------- Texte des Moduls: extSimuServKoppl -------------------- */
#define TOO_MANY_RESLI_TXT     "Zu viele Ergebnislisteneintr"ae"ge: %d"
#define BAD_RESLI_AMOUNT_TXT   "Fehlerhafte Ergebnisliste. Status: %d"
#define WORKFLOW_START_RES_TXT "Start der externen Simulationsdienste mit Modell \"%s\" "
#define WORKFLOW_TERM_RES_TXT  "Terminierung der externen Simulationsdienste mit Task-ID %d "
#define WF_ACTION_DONE_TXT     "ausgef"ue"hrt."
#define WF_ACTION_FAILED_TXT   "fehlgeschlagen!"
#define NO_MODEL_NO_START_TXT  "Kein Start des externen Simulationsdienstes mit leerem Modellnamen!"
#define CHR_LIST_HDR           "Chromosomenlistenkopf"


/* ---------------------------- Texte des Moduls: simu -------------------------------- */
#define EXP_FILE_ANZG_TXT      "Experiment        : "
#define MOD_FILE_ANZG_TXT      "Genmodell         : "
#define BEW_FILE_ANZG_TXT      "Bewertung         : "
#define TSK_FILE_ANZG_TXT      "Programmparameter : "
#define EVO_FILE_ANZG_TXT      "Evo-Parameter     : "
#define CH_FILE_ANZG_TXT       "Chromosomenfile   : "
#define TOP_FILE_ANZG_TXT      "Topologiefile     : "
#define ADD_FILE_ANZG_TXT      "zus"ae"tzliche anwendungsspezifische Files:"


/* ---------------------------- Texte des Moduls: simucmen ---------------------------- */
#define SIM_MEN_START_TXT      "SimuStart"
#define SIM_MEN_FIN_TXT        "ExtSimQuit"
#define SIM_MEN_RESTART_TXT    "ExtSimStrt"
#define SIM_MEN_BEW_TXT        "ZeigBewert"
#define SIM_MEN_RES_TXT        "RettSimErg"
#define SIM_MEN_LOG_TXT        "Log.an/aus"
#define SIM_MEN_EVO_BEST_TXT   "EvoBest"
#define SIM_MEN_STATUS_TXT     "Status"
#define SIM_MEN_PLOT_TXT       "PlotBewert"
#define SIM_MEN_MODELLIST_TXT  "ModellAusw"
#define SIM_MEN_CASE_FILE_TXT  "CaseFile"
#define SIM_MEN_DEL_SIMVAL_TXT "L"oe"SimWerte"
#define SIM_MEN_NEU_BEW_TXT    "NeuBewert"
#define SIMU_HDR_TXT           "   Simulation einer Aktionskette"
#define SIMU_HDR2_TXT          "                 Simulationsergebnisse f"ue"r AK %s:"
#define EXT_SIMU_FIN_HDR_TXT   "           Externen Simulator beenden:"
#define EXT_SIMU_FIN_QUERY_TXT "Simulator wirklich beenden ? "NEIN_JA_TXT
#define EXT_SIMU_RESTART_TXT   "       (Re-)Start des externen Simulators:"
#define EXT_SIMU_STATUS_TXT    "Simulator-Status ist \"aktiv\"!"
#define SIMU_RESTART_QUERY_TXT "(Re-)Start trotzdem ausf"ue"hren? "NEIN_JA_TXT
#define EXT_SIMU_RESTART_QUERY "H=vollst"ae"ndiges Hochfahren  Restart: K=klein  G=gro"szt": "
#define EXT_SIMU_KL_RESTART_CH 'K'
#define EXT_SIMU_GR_RESTART_CH 'G'
#define EXT_SIMU_RECOVERY_CH   'H'
#define EXT_SIMU_NIX_TUN_TXT   "Na gut! Dann eben nicht."
#define EXT_SIMU_DO_RESTRT_TXT "Kompletter (Re-)Start ..."
#define EXT_SIMU_RESTART_DONE  "Simulator-Start/Restart war erfolgreich!"
#define NIX_EXT_SIMU_TXT       "Der (externe) Simulator steht nicht zur Verf"ue"gung!"
#define SIMU_LOGFILE_QUERY_TXT "SimulatorTest Logging "
#define NIX_LOG_TXT            "Kein Simulator-Logile in dieser Version."
#define SIM_PLOT_HEADER_TXT    "Plot der Bewertung der letzten Simulation"
#define EVO_PLOT_HEADER_TXT    "Plot der Bewertung des besten Evo-Laufs des letzten Jobs"
#define NIX_PLOT_DATA_TXT      "Keine plotbare Simulation vorhanden!"
#define SAVE_ERG_HDR           "      Sicherung der Ergebnisse des letzten Simulatorlaufs"
#define KRIT_IND_QUERY_TXT     "Index des Kriteriums: "
#define KRIT_IND_QUERY2_TXT    "Index des Kriteriums ["
#define HARDCOPY_QUERY_TXT     "Hardcopy ? "NEIN_JA_TXT
#define AK_SIM_LOE_HDR_TXT     "           L"oe"schung der AK-Simulationswerte einer AK"
#define SIM_VAL_DEL_TXT        "Simulatorwerte der AK gel"oe"scht."
#define NEUBEWERT_HDR1         "        Neubewertung aller AKs im AK-Speicher anhand der"
#define NEUBEWERT_HDR2         "          aktuellen Aufgabenstellung und Bewertung"
#define NEUBEW_SIM_LOE_QUERY   "L"oe"schung der AK-Simulationswerte? "JA_NEIN_TXT
#define NEUBEW_TO_DO_TXT       "%1d Aktionsketten neuzubewerten ..."
#define NEUBEW_NIX_DONE_TXT    "Neubewertung nicht durchgeführt! (Kein externer Simulator)"
#define EXTSIM_MODLIST_HDR_TXT "Liste der Modelle der externen Simulationsdienste:"
#define EXTSIM_MOD_AUSW_TXT    "Modellauswahl (1..%d): "
#define EXTSIM_NO_COM_TXT      "Keine Kommunikation mit den externen Simulationsdiensten m"oe"glich! Status "ue"berpr"ue"fen"
#define MATLAB_CASEF_HDR_TXT   "   "
#define MATLAB_CASEF_QUERY_TXT "Erzeugung einer MatPower case-Datei? "NEIN_JA_TXT
#define MATLAB_CASEF_ON_TXT    "Bis zum Verlassen des Simu-Men"ue"s legen alle nachfolgenden Simulationen eine MatPower case-Datei an!\n"


/* ---------------------------- Texte des Moduls: simu_gsw: --------------------------- */
#define AK_ADR_BLEIBT_TXT      "Die Kettenadresse bleibt %s"
#define AK_ADR_NEU_TXT         "Neue Kettenadresse: %s"
#define NOTE_UNVERANDERT_TXT   "Die Kette bleibt unver"ae"ndert."
#define NOTE_SCHLECHT_TXT      "Neue Note %ld ist ung"ue"ltig."
#define NO_SIMU_DONE_TXT       "Keine Simulation durchgef"ue"hrt!"
#define NOTE_BLEIBT_TXT        "Note bleibt: %ld (%.9f)"
#define NOTE_NEU_TXT           "Neue Note: %ld (%.9f)"
#define ZEIG_BEW_HDR_TXT       "                     Bewertung der letzten Simulation"
#define ZEIG_EVO_BEW_HDR_TXT   "               Bewertung der besten L"oe"sung des aktuellen Jobs"
#define ERG_HDR                "===== %s. Ergebnisse vom: %s =====\n\n"
#define EXP_FILES_HDR          "Geladene Files:\n===============\n"
#define NO_EVO_DONE_TXT        "Keine Evolution durchgef"ue"hrt!"
#define PARAM_HDR_TXT          "             %s:"
#define STRICH_TXT             STRICH_40_TXT"--------------"
#define BEWERT_TITLE1_TXT      "Kriterium           Prio  Gew   ungew     gewichtete    Simulationsergebnisse"
#define BEWERT_TITLE2_TXT      "                          [%]    Note  MinNote    Note"
#define BEWERT_ITITLE1_TXT     "Nr. "
#define BEWERT_ITITLE2_TXT     "    "
#define ROH_SUMME_TXT          "Gesamtnote (ohne Straffunktionen):              "
#define STRAF_TITLE_TXT        "Straffunktionen (in Klammern: mangelnde Prio):"
#define STRAF_FAKTOR_TXT       "Faktor"
#define KEINE_STRAFE_TXT       "Keine Straffunktion angewandt."
#define ERG_NOTE_TXT           "Gesamtnote:                                     "
#define ERG_UNTERSTRICH_TXT    "                                                ======"
#define STATUS_ANZG_HDR_TXT    "                  Simulator-Status:"
#define STATUS_ANZG_SIM1_TXT   "Externer Simulator gestartet  (sim_ok) = %s"
#define STATUS_ANZG_SIM2_TXT   "Externer Simulator bereit     (sim_up) = %s"
#define STATUS_ANZG_SIM3_TXT   "Interner Simulator bereit     (sim_up) = %s"
#define STATUS_ANZG_MP1_TXT    "Externer Matlab-Simulator bereit            (sim_ok) = %s"
#define STATUS_ANZG_MP2_TXT    "Ext.Sim. mit Model \"%s\" gestartet%*s(sim_up) = "
#define STATUS_ANZG_MP3_TXT    "Simulator Logging (bei Matlab kaum umgesetzt)        = "
#define STATUS_ANZG_MP4_TXT    "Simulationslauf erzeugt eine MatPower case-Datei     = %s"
#define STATUS_ANZG_MP5_TXT    "Kennung der Matlab/MatPower-Anwendung                = \"%s\""
#define STATUS_ANZG_AKTIV_TXT  "aktiv"
#define STATUS_ANZG_TOT_TXT    "gestorben"
#define STATUS_ANZG_LOG_TXT    "Simulator Logging                      = "
#define STATUS_ANZG_AUS_TXT    "aus"
#define STATUS_ANZG_ESS1_TXT   "Externe Simulationsdienste:"
#define STATUS_ANZG_ESS2_TXT   "URL-Anfang: \"%s\""
#define STATUS_ANZG_ESS3_TXT   "Kommunikationspaket bereit%*s (sim_ok) = "
#define STATUS_ANZG_ESS4_TXT   "Modell \"%s\" geladen%*s (sim_up) = "
#define STATUS_ANZG_ESS5_TXT   "Logging der Simulatorschnittstelle %*s = "
#define STATUS_ANZG_ESS6_TXT   "  Arbeits-URL: \"%s\""
#define PLOT_AKT_WERT_TXT      "Aktueller Wert"
#define NEUBEWERT_AK_DEL_TXT   "%1d. unbewertbare AK %s gel"oe"scht."
#define NEUBEWERT_ERG_TXT      "%1d Aktionsketten neubewertet."
#define NO_PARAMS_WITHOUT_SIMU "Die  Anzeige der %s ist erst nach vorheriger Simulation m"oe"glich."


/* ---------------------------- Texte des Moduls: par_simu ---------------------------- */
#define NOT_SIMULATED_TXT      "nicht simuliert"


/* ---------------------------- Texte des Moduls: mbf_sim ----------------------------- */
#define X_VAL_TXT              "Puffer f. X-Werte"


/* ---------------------------- Texte des Moduls: rob_sim ----------------------------- */
#define NO_KIN_LOADED_TXT      "Keine Kinematik geladen!"
#define JUMP_DONE_TXT          "%d Aktionen "ue"bersprungen."
#define TAKT_ERLEDIGT_TXT      "***  Takt %3d ausgef"ue"hrt, Zeit = %4.1f sec  ***"
#define TAKT_ERLEDIGT2_TXT     "***"TAKT_ERLEDIGT_TXT
#define AK_FIN_TXT             "********** AK-Interpretationsende weil: "
#define AK_ENDE_TXT            "Aktionskettenende erreicht."
#define AK_WUERG_TXT           "Kollision oder Achsanschlag."
#define AK_FEHLER_TXT          "Fehler bei Interpretation."
#define AUSLAUFEN_TXT          "*** \"Auslaufen / Bremsen\" der Motoren:"
#define SIMU_ENDE_TXT          "********* Simulationsende *********"
#define SIMU_WUERG_QUERY_TXT   "Simulationsabbruch ? "NEIN_JA_TXT
#define SIMU_WUERG_TXT         "*** SIMULATIONSABBRUCH !"
#define SIMU_NOT_WUERG_TXT     "Fortsetzung der SIMULATION ..."
#define SIMU_WUERG_LOG_TXT     "******  Takt %5d: SIMU-ABBRUCH !!!"
#define ZIEL_ABW_ERG_LOG_TXT   "Bewertung: Zielabweich.: rest_weg=%g bahndistanz=%g erg=%g"
#define BAHN_ABW_ERG_LOG_TXT   "Bewertung: Bahnabweich.: bges=%g bahndistanz=%g rwert=%g"
#define ENERGIE_ERG_LOG_TXT    "Bewertung: Energieverbr: summedv=%g summeda=%g rwert=%g"
#define GES_ERG_ALLG_TXT       "Erg: Bahndist=%4.2f  Zielabw=%4.2f"
#define GES_ERG_BAHN_TXT       "  BGes=%4.2f"
#define GES_ERG_ENERG_TXT      "  SDV=%4.2f SDA=%4.2f"
#define ACHS_KOLL_TXT          "Kollision bei Achse %d"
#define GREIFER_KOLL_TXT       "Kollision am Greifer"
#define SENSOR_ERR_TXT         "STRAHLABSTAND: Fehlerhafter Sensorcode: %d"
#define ZUVIEL_ACHS_TXT        UE"berfl"ue"ssige Achsangaben ignoriert (max=%d)."
#define ACHS_OK_TXT            "      %d Achsen erfolgreich eingelesen."
#define ACHS_FEHLER_TXT        "Fehler bei Achse %d!"
#define ACHSANZ_UNGL_TXT       "Ungleiche Achsanzahl TSK:%d KIN:%d"
#define KIN_ERR_TXT            "      Fehler in KIN-Datei. "NO_KIN_LOADED_TXT
#define MITSU_LOAD_TXT         "      Modell des Mitsubishi R500 mit 5 Achsen geladen."
#define NO_HIND_LINE_TXT       "\"Hindernis ...\"-Zeile fehlt!"
#define HIND_OK_TXT            "      %d Hindernisse erfolgreich eingelesen."
#define HIND_ERR_TXT           "      Fehler in OBS-Datei. Keine Hindernisse geladen!"
#define HIND_FEHLER_TXT        "Fehler beim %d. Hindernis!"
#define NIX_MOTOR_TXT          "Motor Nr %d nicht da!"
#define AK_ERROR_TXT           "Fehler in AK: "
#define BAD_POS_VGL_OP_TXT     AK_ERROR_TXT"Falscher PosVglOperator!"
#define BAD_GESCHW_VGL_OP_TXT  AK_ERROR_TXT"Falscher GeschwVglOp!"
#define BAD_BESCHL_VGL_OP_TXT  AK_ERROR_TXT"Falscher BeschlVglOp!"
#define BAD_RAMP_VAL_TXT       AK_ERROR_TXT"Rampe < 0! Auf 0.1 gesetzt!"
#define BAD_AK_ELEM_TXT        AK_ERROR_TXT"Kein Kopf erlaubt!"
#define NO_SUB_AK_TXT          AK_ERROR_TXT"Unter-AK-Verweis nicht implementiert!"
#define TCP_VERGL_LOG_TXT      "TCP-Vergl: Distanz=%4.1f%s%5.2f"
#define MOTOR_AUS_LOG_TXT      "Motor %d aus, Rampe=%4.2f"
#define MOTOR_AN_LOG_TXT       "Motor %d:  U=%4.2f Rampe=%4.2f"
#define DREHE_ABST_SENS_LOGTXT "Abst.Sensor aussen auf %4.2f Grad gedreht."
#define KONTAKT_QUERY_TXT      "Kontakt an Sensor %d ? [j/n]: "
#define KONTAKT_JA_LOG_TXT     "Falls_Kontakt: Kontakt an Sensor %s: %s Aktionen "ue"bersprungen."
#define KONTAKT_NEIN_LOG_TXT   "Falls_Kontakt: Kein Kontakt an Sensor %s: Kein Sprung."
#define VGL_WAHR_LOG_TXT       "VerglWahr: %d Aktionen "ue"bersprungen."
#define START_BLOCK_ANF_LOGTXT "Start Block ANFANG"
#define START_BLOCK_END_LOGTXT "Start Block ENDE"
#define UNVERAENDERT_LOG_TXT   "Unver"ae"ndert f"ue"r %d Takte."
#define STORE_SENS_ERR_TXT     "Store_sens: Falsche Nummern: SensorNr=%d SensorVarNr=%d Max=%d"
#define STORE_SENS_LOG_TXT     "StoreSens: Sens%d Var%d Wert = %3.1f"
#define SET_TIME_ERR_TXT       "Set_Time: Falsche RegisterNr.:%d. Max=%d"
#define SET_TIME_LOG_TXT       "Set_Time: ZeitRegister%d = %4.1f sec."
#define STORE_TIME_ERR_TXT     "Store_Time: Falsche RegisterNr.:%d. Max=%d"
#define STORE_TIME_LOG_TXT     "Store_Time: ZeitRegister%d = %4.1f sec."
#define BAD_AKTION_TXT         "Unbekannte Aktion! Code=%d"
#define ACHS_AM_ANSCHLAG_TXT   "Achse %d am Anschlag !"
#define TAKT_ZEIT_TXT          "%3d.Takt                                          Zeit: %4.1f sec"
#define ZWISCHEN_ERG_TXT1      "-------------- Werte aller Achsen: --------------\n"
#define ZWISCHEN_ERG_TXT2      "  Achse %2d:  U =%7.2f  Rampe =%7.2f  ZielU =%7.2f\n"
#define ZWISCHEN_ERG_TXT3      "  Interpolierte robotjoints:\n   "
#define ZWISCHEN_ERG_TXT4      "  Position : %5.1f,%5.1f,%5.1f\n"
#define ACHS_POS_VGL_LOG_TXT   "Vergl.Achse %d: Position %s %5.2f"
#define ACHS_GESCHW_VGL_LOGTXT "Vergl.Achse %d: Geschwdgk. %s %d"
#define ACHS_BESCHL_VGL_LOGTXT "Vergl.Achse %d: Beschleung.%s %d"
#define BAD_ZEIT_VGL_OP_TXT    AK_ERROR_TXT"Falscher ZeitVglOp!"
#define BAD_ZEIT_REG_NR_TXT    "Comp_Time: Falsche RegisterNr: %d Max=%d"
#define ZEIT_VGL_LOG_TXT       "ZeitVergl: Var%d Wert=%3.1f %s %3.1f"
#define BAD_SENS_VGL_OP_TXT    AK_ERROR_TXT"Falscher SensVglOp!"
#define SENS_VGL_LOG_TXT       "Comp_Sens: Sens%d Var%d: %3.1f %s %3.1f"
#define BAD_SENS_REG_NR_TXT    "Comp_Sens: Falsche Sensor/RegisterNr: %d/%d Max=%d"
#define ABST_VGL_LOG_TXT       "VerglGreifer"
#define ABST_AUSSEN_TXT        "Aussen"
#define ABST_ACHSE_TXT         " Achse=%d"
#define ABST_KEIN_KONTAKT_TXT  " kein Kontakt"


/* ---------------------------- Texte des Moduls: rob_gsw ----------------------------- */
#define START_POS_TXT          "Startwerte:"
#define ZIEL_POS_TXT           "Zielwerte:"
#define POS_TXT                "  Pos.:  "
#define JOINTS_TXT             "  Joints:"
#define OFFSET_TXT             "         "


/* ---------------------------- Texte des Moduls: rob_dial ---------------------------- */
#define TEACH_IN_HDR_TXT       "              Vorgabe neuer Achswerte (Teach-in):"
#define ALTE_WERTE_TXT         "Alte Werte:"
#define AKT_WERTE_TXT          "Aktuelle Werte:"
#define NEUE_WERTE_TXT         "Neue Werte:"
#define START_QUERY_TXT        "Startwerte ver"ae"ndern? "JA_NEIN_TXT
#define ZIEL_QUERY_TXT         "Zielwerte ver"ae"ndern? "JA_NEIN_TXT
#define ACHS_QUERY_TXT         "Neuer Wert von Achse %2d [%6.1f]: "

