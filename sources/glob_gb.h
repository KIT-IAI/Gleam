/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Englisch)
Package: -                            File: global_gb.h                Version:     V1.4.1
Status : o.k.                        Autor: W.Jakob                      Datum: 29.09.2020
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
--------------------------------------------------------------------------------------- */


/* ----------------------------- texts of the main program ---------------------------- */
#define INIT_HDR_1          "Initialization:"
#define INIT_HDR_2          "==============="
#define EXP_FILE_TXT        "Experiment file: %s"
#define INIT_FIN_TXT        "Initialization completed."



/* ---------------------------------- general texts: ---------------------------------- */
#define PROG_ID_STR         "%s V%s from %s"
#define LOG_HDR             "======== %s. Log file from: %s %s ========"
#define JA                  'Y'
#define NEIN                'N'
#define JA_TXT              "yes"
#define NEIN_TXT            "no"
#define JA_NEIN_LEN          4
#define JA_NEIN_TXT         "[Y/n]: "
#define NEIN_JA_TXT         "[y/N]: "
#define BAD_KEY             "Input not allowed!"
#define ABBRUCH_TXT         "Input stoped!"
#define FKT_TERM_TXT        "Function aborted!"
#define SAVE_OK_TXT         "File successfully saved"
#define SAVE_FEHL_TXT       "No save done!"
#define NIX_CH              "This chromosome does not exist!"
#define CONTINUE_TXT        "Type anything to proceed: "
#define FILE_EXT_TXT        "Filename with extension and opt. path:"
#define FILE_PUR_TXT        "Filename without extension but with opt. path:"
#define FILE_QUERY          "File: "
#define AK_QUERY_TXT        "Chr "
#define ON_FATAL_NO_FKT     "Function disabled due to one or more fatal errors!" 
#define FATAL_WARNUNG       "Warning: Function may not work properly due to fatal error(s)!" 
#define STRICH_40_TXT       "----------------------------------------"
#define SCROLL_QUERY        "==> <ENTER>=Continue, <ESC>=Stop "
#define MSG_LINE_TXT        "*** %s\n"
#define FMELD_LINE_TXT      "**** Error: %s\n"
#define FATAL_LINE_TXT      "***** Fatal: %s\n"
#define HDR_TIME_STAMP_TXT  "written on"
#define SECT_DELIMITER_TXT  "#! "
#define NO_CH_FILE_TXT      "  No chromosome file read."
#define FKT_DISABLED_TXT    "Function deactivated with active optimization job!"
#define NICHT_TXT           "not "


/* -------------------------------- main menu entries: -------------------------------- */
#define MAIN_MEN_TXT        "Main Menue"
#define SIMU_MEN_TXT        "Simulator"
#define BEW_MEN_TXT         "Evaluation"
#define AUFG_MEN_TXT        "ProgParam"
#define EVO_MEN_TXT         "Evo/Opt"


/* ------------------------ texts used in more than one package: ---------------------- */
#define OK_STATE_TXT          "OK"
#define ERR_STATE_TXT         "ERROR"
#define FAT_STATE_TXT         "FATAL"

#define INT_PARAMS_TXT        "integer parameters of a gene"
#define REAL_PARAMS_TXT       "real parameters of a gene"


/* ----------------------------- originally in package lgsw: -------------------------- */
#define DESCR_ELEM_TXT        "Chromosome descriptors"


/* ----------------------------- originally in package webIO: ------------------------- */
#define RES_LIST_TXT          "result list"
#define CHR_ENTRY_TXT         "chromosome entry"
#define RES_DATA_ENTRY_TXT    "result values"


/* --------------------------- Urspruenglich in Package chio: ------------------------- */
#define REST_OK_TXT            "%s  %d chromosomes successfully restored! Best fitness: %g"


/* ----------------------------- originally in package aufg: -------------------------- */
#define INIT_PROC_NEW_TXT      "new"           /* valid procs for the start population: */
#define INIT_PROC_BEST_TXT     "best"
#define INIT_PROC_MIX_TXT      "mix"
#define INIT_PROC_BEST_NEW_TXT "bestNew"
#define INIT_PROC_LHC_NEW_TXT  "lhcNew"
#define INIT_PROC_LHC_BEST_TXT "lhcBest"
#define INIT_PROC_FILE_TXT     "file"


/* ----------------------------- originally in package simu: -------------------------- */
#define EXT_SIMU_STARTUP_TXT  "     External simulation! Execution may require some time ..."
#define BAD_SIMU_RUNS_TXT     "*** Out of %d simulation runs %d are faulty!"
#define STARTED_TXT           "started"


/* ----------------------------- originally in package men: --------------------------- */
#define LOAD_ALLG_TXT         "  %s \"%s\" loaded."
#define LOAD_NO_APPL          "The loaded application does not require any additional files!"
#define LSTAT_BAD_DATA        "Inconsistent data due to errors in file!"
#define SAV_GEN_TXT           "  %s %ssaved in file \"%s\"."
#define LOAD_OK_TXT           " successfully loaded."
