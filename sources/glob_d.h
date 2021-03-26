/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Deutsch)
Package: -                            File: glob_d.h                   Version:     V1.4.0
Status : o.k.                        Autor: W.Jakob                      Datum: 08.08.2020
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
--------------------------------------------------------------------------------------- */


/* ----------------------------- Texte des Hauptprogramms ----------------------------- */
#define INIT_HDR_1          "Initialisierung:"
#define INIT_HDR_2          "================"
#define EXP_FILE_TXT        "Experimentdatei: %s"
#define INIT_FIN_TXT        "Initialisierung abgeschlossen."


/* -------------------------------- Allgemeine Texte: --------------------------------- */
#define PROG_ID_STR         "%s V%s vom %s"
#define LOG_HDR             "======== %s. Logfile vom: %s %s ========"
#define JA                  'J'
#define NEIN                'N'
#define JA_TXT              "ja"
#define NEIN_TXT            "nein"
#define JA_NEIN_LEN         4
#define JA_NEIN_TXT         "[J/n]: "
#define NEIN_JA_TXT         "[j/N]: "
#define BAD_KEY             "Eingabe unzul"ae"ssig!"
#define ABBRUCH_TXT         "Eingabe abgebrochen!"
#define FKT_TERM_TXT        "Funktion abgebrochen!"
#define SAVE_OK_TXT         "Sicherung erfolgreich durchgef"ue"hrt."
#define SAVE_FEHL_TXT       "Sicherung fehlgeschlagen!"
#define NIX_CH              "Dieses Chromosom gibt es nicht!"
#define CONTINUE_TXT        "Weiter mit irgendeiner Taste: "
#define FILE_EXT_TXT        "Dateiname mit Extension u. ggf. Pfad:"
#define FILE_PUR_TXT        "Dateiname ohne Extension aber ggf. mit Pfad:"
#define FILE_QUERY          "File: "
#define AK_QUERY_TXT        "AK "
#define ON_FATAL_NO_FKT     "Funktion gesperrt, da ein oder mehrere fatale Fehler auftraten!" 
#define FATAL_WARNUNG       "Warnung: Wegen fataler(m) Fehler kann Funktion gest"oe"rt sein!" 
#define STRICH_40_TXT       "----------------------------------------"
#define SCROLL_QUERY        "===> <ENTER>=Weiter, <ESC>=Stop "
#define MSG_LINE_TXT        "*** %s\n"
#define FMELD_LINE_TXT      "**** Fehler: %s\n"
#define FATAL_LINE_TXT      "***** Fatal: %s\n"
#define HDR_TIME_STAMP_TXT  "erzeugt am"
#define SECT_DELIMITER_TXT  "#! "
#define NO_CH_FILE_TXT      "  Kein Chromosomenfile eingelesen."
#define FKT_DISABLED_TXT    "Funktion bei aktivem Optimierungsjob gesperrt!"
#define NICHT_TXT           "nicht "


/* ------------------------------ Eintraege im Hauptmenue ----------------------------- */
#define MAIN_MEN_TXT        "Hauptmenue"
#define SIMU_MEN_TXT        "Simulator"
#define BEW_MEN_TXT         "Bewertung"
#define AUFG_MEN_TXT        "ProgParam"
#define EVO_MEN_TXT         "Evo/Opt"


/* ----------------------- In mehreren Packages benoetigte Texte: --------------------- */
#define OK_STATE_TXT           "Ok"
#define ERR_STATE_TXT          "Fehler"
#define FAT_STATE_TXT          "FATAL"

#define INT_PARAMS_TXT         "Integer-Parameter eines Gens"
#define REAL_PARAMS_TXT        "Real-Parameter eines Gens"


/* --------------------------- Urspruenglich in Package lgsw: ------------------------- */
#define DESCR_ELEM_TXT         "Chromosomendeskriptoren"


/* --------------------------- Urspruenglich in Package webIO: ------------------------ */
#define RES_LIST_TXT           "Ergebnisliste"
#define CHR_ENTRY_TXT          "Chromosomeneintrag"
#define RES_DATA_ENTRY_TXT     "Ergebniswerten"


/* --------------------------- Urspruenglich in Package chio: ------------------------- */
#define REST_OK_TXT            "%s  %d Chromosomen erfolgreich restauriert! Beste Fitness: %g"


/* --------------------------- Urspruenglich in Package aufg: ------------------------- */
#define INIT_PROC_NEW_TXT      "neu"           /* valid procs for the start population: */
#define INIT_PROC_BEST_TXT     "best"
#define INIT_PROC_MIX_TXT      "mix"
#define INIT_PROC_BEST_NEW_TXT "bestNeu"
#define INIT_PROC_LHC_NEW_TXT  "lsvNeu"
#define INIT_PROC_LHC_BEST_TXT "lsvBest"
#define INIT_PROC_FILE_TXT     "file"


/* --------------------------- Urspruenglich in Package simu: ------------------------- */
#define EXT_SIMU_STARTUP_TXT   "     Externe Simulation! Die Ausf"ue"hrung kann l"ae"nger dauern ..."
#define BAD_SIMU_RUNS_TXT      "*** Von %d Simulationsl"ae"ufen sind %d fehlerhaft!"
#define STARTED_TXT            "gestartet"


/* --------------------------- Urspruenglich in Package men: ------------------------- */
#define LOAD_ALLG_TXT          "  %s \"%s\" gelesen."
#define LOAD_NO_APPL           "Die geladene Anwendung hat keine Zusatz-Anwendungsfiles!"
#define LSTAT_BAD_DATA         "Inkonsistente Daten wegen Fehler in Datei!"
#define SAV_GEN_TXT            "  %s %sgerettet in \"%s\"."
#define LOAD_OK_TXT            " erfolgreich geladen."
