/* ---------------------------------------------------------------------------------------
GLEAM/AE                           Package-Header-File   
Package: aufg                         File: aufg.h                         Version: V1.6.4
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------

Das Package enthaelt Routinen fuer die applikationsspezifische Aufgabenstellung. Dazu ge-
hoert das Retten und Restaurieren der Aufgabendaten (TSK-File) und das Aufgabensubmenue.
Ausserdem stellt das Package Funktionen fuer die Parameterbehandlung anderer Files (bisher
nur das MOD-File) zu Verfuegung.

Module des Packages:
    tsk_data  Retten und Restaurieren der Aufgabendaten (TSK-File), Registrierung von 
              Parametern und applikationsneutrale Menue-Grundfunktionen.
    aufgcmen  Aufgabe-Submenue der TEXT_MODE-Variante.

------------------------------------------------------------------------------------------
Status : o.k.                        Autor: W.Jakob                      Datum: 28.08.2020
------------------------------------------------------------------------------------------ */


/* ============================== Exportierte Konstanten: ============================= */
#define MOD_FILE_SPEC_IDX            0
#define BEW_FILE_SPEC_IDX            1
#define TSK_FILE_SPEC_IDX            2
#define EVO_FILE_SPEC_IDX            3
#define MEM_FILE_SPEC_IDX            4
#define TOP_FILE_SPEC_IDX            5
#define FIRST_TSK_DATA_IDX           6           /* TOP_FILE_SPEC_IDX + 1 */

#define WITH_PHAENO_REP_IDX          FIRST_TSK_DATA_IDX             /* 06 */
#define WITH_DETAILED_LOG_IDX       (FIRST_TSK_DATA_IDX +  1)       /* 07 */
#define MA_WITH_ALL_IMPR_IDX        (FIRST_TSK_DATA_IDX +  2)       /* 08 */
#define WITH_RES_CHR_SIMU_IDX       (FIRST_TSK_DATA_IDX +  3)       /* 09 */
#define WITH_RANDGEN_INIT_IDX       (FIRST_TSK_DATA_IDX +  4)       /* 10 */
#define WITH_RES_CHR_SAVE_IDX       (FIRST_TSK_DATA_IDX +  5)       /* 11 */
#define WITH_SIMU_LOG_IDX           (FIRST_TSK_DATA_IDX +  6)       /* 12 */
#define OPT_PROCEDURE_IDX           (FIRST_TSK_DATA_IDX +  7)       /* 13 */
#define LHC_INDEX_IDX               (FIRST_TSK_DATA_IDX +  8)       /* 14 */
#define POP_SIZE_IDX                (FIRST_TSK_DATA_IDX +  9)       /* 15 */
#define INIT_STRATEGY_IDX           (FIRST_TSK_DATA_IDX + 10)       /* 16 */
#define INIT_SPAR_IDX               (FIRST_TSK_DATA_IDX + 11)       /* 17 */
#define DEME_SIZE_IDX               (FIRST_TSK_DATA_IDX + 12)       /* 18 */
#define NUMB_OF_FCLASSES_IDX        (FIRST_TSK_DATA_IDX + 13)       /* 19 */
#define LHC_ADAPT_SPEED_IDX         (FIRST_TSK_DATA_IDX + 14)       /* 20 */
#define LEVEL_ADAPT_SPEED_IDX       (FIRST_TSK_DATA_IDX + 15)       /* 21 */
#define NUMBER_OF_RESULTS_IDX       (FIRST_TSK_DATA_IDX + 16)       /* 22 */
#define RUNTIME_LIMIT_IDX           (FIRST_TSK_DATA_IDX + 17)       /* 23 */
#define GEN_LIMIT_IDX               (FIRST_TSK_DATA_IDX + 18)       /* 24 */
#define GAC_LIMIT_IDX               (FIRST_TSK_DATA_IDX + 19)       /* 25 */
#define GDI_LIMIT_IDX               (FIRST_TSK_DATA_IDX + 20)       /* 26 */
#define EVAL_LIMIT_IDX              (FIRST_TSK_DATA_IDX + 21)       /* 27 */
#define ACCEPTANCE_RULE_IDX         (FIRST_TSK_DATA_IDX + 22)       /* 28 */
#define BACKUP_RATE_IDX             (FIRST_TSK_DATA_IDX + 23)       /* 29 */
#define TERM_TEST_FREQ_IDX          (FIRST_TSK_DATA_IDX + 24)       /* 30 */
#define TARGET_FITNESS_IDX          (FIRST_TSK_DATA_IDX + 25)       /* 31 */
#define XO_HAMDIST_IDX              (FIRST_TSK_DATA_IDX + 26)       /* 32 */
#define LAMARCK_RATE_IDX            (FIRST_TSK_DATA_IDX + 27)       /* 33 */
#define LHC_ITER_LIMIT_IDX          (FIRST_TSK_DATA_IDX + 28)       /* 34 */
#define LHC_TERM_LIMIT_IDX          (FIRST_TSK_DATA_IDX + 29)       /* 35 */
#define OLD_DISTR_FRAC_IDX          (FIRST_TSK_DATA_IDX + 30)       /* 36 */
#define GEN_REP_RATE_IDX            (FIRST_TSK_DATA_IDX + 31)       /* 37 */
#define RANKING_PAR_IDX             (FIRST_TSK_DATA_IDX + 32)       /* 38 */
#define CEC_FES_LIMIT_IDX           (FIRST_TSK_DATA_IDX + 33)       /* 39 */
#define SIMU_RES_PREC_IDX           (FIRST_TSK_DATA_IDX + 34)       /* 40 */
#define SIMU_MAX_TIME_IDX           (FIRST_TSK_DATA_IDX + 35)       /* 41 */
#define SIMU_MOD_NAME_IDX           (FIRST_TSK_DATA_IDX + 36)       /* 42 */
#define SIMU_RUNS_PER_INIT_IDX      (FIRST_TSK_DATA_IDX + 37)       /* 43 */
#define SIMU_DIR_NAME_IDX           (FIRST_TSK_DATA_IDX + 38)       /* 44 */
#define MBF_ROT_ANGLE_IDX           (FIRST_TSK_DATA_IDX + 39)       /* 45 */
#define LSKP_R500_IDX               (FIRST_TSK_DATA_IDX + 40)       /* 46 */
#define LSKP_COLLI_TEST_IDX         (FIRST_TSK_DATA_IDX + 41)       /* 47 */
#define LSKP_STOP_TEST_IDX          (FIRST_TSK_DATA_IDX + 42)       /* 48 */
#define LSKP_STOP_DEPREC_IDX        (FIRST_TSK_DATA_IDX + 43)       /* 49 */
#define LSKP_AXES_NUMBER_IDX        (FIRST_TSK_DATA_IDX + 44)       /* 50 */
#define LSKP_CYCLE_LENGTH_IDX       (FIRST_TSK_DATA_IDX + 45)       /* 51 */
#define WITH_REC_EVO_BEST_IDX       (FIRST_TSK_DATA_IDX + 46)       /* 52 */
#define WITH_STAT_DAT_COL_IDX       (FIRST_TSK_DATA_IDX + 47)       /* 53 */
#define WITH_GEN_STAT_COL_IDX       (FIRST_TSK_DATA_IDX + 48)       /* 54 */
#define WITH_SIMU_TIME_IDX          (FIRST_TSK_DATA_IDX + 49)       /* 55 */
#define WITH_TERM_TEST_IDX          (FIRST_TSK_DATA_IDX + 50)       /* 56 */
#define OPAL_MAX_SHIFTS_IDX         (FIRST_TSK_DATA_IDX + 51)       /* 57 */
#define SIMU_DOMAIN_NAME_IDX        (FIRST_TSK_DATA_IDX + 52)       /* 58 */
#define WITH_TEXTCHR_FILE_IDX       (FIRST_TSK_DATA_IDX + 53)       /* 59 */

#define PARAM_LIST_LENGTH           (FIRST_TSK_DATA_IDX + 54)		/* 60 */

#define BOOL_DATA                    0     /*   } internally boolean and integer data   */
#define INT_DATA                     1     /*   } are treated as int data.              */
#define ENUM_DATA                    2     /* enumeration data mapped to integer        */
#define DOUBLE_DATA                  3     /* parameter of data type double             */
#define STRING_DATA                  4     /* string parameter                          */

#define WITH_DEFAULT              TRUE
#define WITHOUT_DEFAULT          FALSE
#define WITH_INDICES              TRUE
#define WITHOUT_INDICES          FALSE
#define SHOW_ALL_PARAMS           TRUE
#define SHOW_EDITABLE_PARAMS     FALSE
#define EDITABLE                  TRUE
#define NOT_EDITABLE             FALSE
#define CHECK_APPL_ID_ALSO        TRUE
#define CHECK_PROG_ID_ONLY       FALSE
#define PARAM_FROM_FILE           TRUE
#define PARAM_FROM_MENU          FALSE



/* =========================== Exportierte Typdeklarationen: ========================== */


/* =============================== Exportierte Variable: ============================== */
extern INT   paramListActEntries[],      /* list of indices for access in GUI_MODE      */
             paramListActEntryAmount;    /* number of entries in paramListActEntries    */
extern BOOL  mit_statistik,           /* TRUE = acquisition of statistical data         */
             statistik_pro_gen,       /* acquisition of statistical data per generation */
             mit_rz;                  /* True if display of elapsed runtime of ext.simu */
extern char *sim_root_dir_name;       /* name of the root-directory of simulator models */



/* =============================== Exportierte Routinen: ============================== */

/* --------------------------- Routinen aus Modul: tsk_data --------------------------- */
extern BOOL   readHdr          (FILE       *inFile,
                                BOOL        checkApplID,
                                const char *fileIdBuf,
                                char       *remainder);

extern GSTATE readTskFile      (char *fileName, 
                                BOOL *badData);

extern BOOL   processInputLine (BOOL *errorFlag,
                                char *lineBuffer);

extern BOOL   getFirstString   (char *resultBuf,
                                char *sourceBuf);

extern GSTATE saveTskFile      (char *fileName);

extern BOOL registerParam (const char  *paramName,
                           INT          paramIdx,
                           INT          dataType,
                           DOUBLE       lowerLim,
                           DOUBLE       upperLim,
                           BOOL         editable,
                           BOOL         setDefault,
                           INT          intDef,
                           DOUBLE       doubleDef,
                           const char **enumWordList);

extern BOOL setBoolVal   (BOOL        newVal,
                          INT         paramIdx);

extern BOOL setIntVal    (INT         newVal,
                          INT         paramIdx);

extern BOOL setEnumVal   (const char *newVal,
                          INT         paramIdx);

extern BOOL setDoubleVal (DOUBLE      newVal,
                          INT         paramIdx);

extern BOOL setStringVal (const char *newVal,
                          INT         paramIdx);

extern void setParamEditable (BOOL  newVal,
                              INT   paramIdx);

extern BOOL getParamData    (char         *paramName,
                             INT          *paramType,
                             const char ***enumList,
                             INT           paramIdx);

extern BOOL getIntLimits    (INT    *lowerLim,
                             INT    *upperLim,
                             INT     paramIdx);

extern BOOL getDoubleLimits (DOUBLE *lowerLim,
                             DOUBLE *upperLim,
                             INT     paramIdx);

extern BOOL getBoolVal   (BOOL   *retVal,
                          INT     paramIdx,
                          GSTATE  invalidDataErrLevel);

extern BOOL getIntVal    (INT    *retVal,
                          INT     paramIdx,
                          GSTATE  invalidDataErrLevel);

extern BOOL getEnumText  (char   *retVal,
                          INT     paramIdx,
                          GSTATE  invalidDataErrLevel);

extern BOOL getDoubleVal (DOUBLE *retVal,
                          INT     paramIdx,
                          GSTATE  invalidDataErrLevel);

extern BOOL getStringVal (char   *retVal,
                          INT     paramIdx,
                          GSTATE  invalidDataErrLevel);

extern BOOL getEnumIndexFromParIdx (INT        *retVal,
                                    const char *enumText,
		                    INT         parIdx);

extern INT getMaxNameLength (void);

#ifdef GUI_MODE
  extern void prepareTskDataDisplay (BOOL  withIndices,
                                     BOOL  allParam);
#endif /* GUI_MODE */

extern BOOL registerTskFileParameter (void);

extern BOOL tskDataInit (void);


/* --------------------------- Routinen aus Modul: aufgcmen --------------------------- */
extern BOOL aufg_sub_men (void);

