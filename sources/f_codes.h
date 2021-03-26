/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: fbhm                    File: f_codes.h                  Version: 1.3.2
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Die Datei enthaelt die Fehlercodes und wird von fbhm/fbhm.h inkludiert.

--------------------------------------------------------------------------------
Aenderungen gegenueber V1.0.0:
 1. Neuer Code fuer GLEAM-R.                                     C.B. 20.11.1998
 2. Neuer Code fuer den Ketteneditor.                            W.J. 05.12.1998
 3. Neuer Code und Update fuer "aufg"-Package.                   W.J. 21.12.1998
 4. Neuer Code NO_AK_GEN                                         W.J. 27.03.2000
 5. Codes 93, 101 und 102 frei.                                  W.J. 02.08.2000
 6. Freigegebene Codes und weitere fuer LSV benutzt.             W.J. 18.04.2001
 7. Neue Codes f.LSV- u.Heuri-Angaben im erweiterten MOD-File.   W.J. 20.10.2006
 8. Neue Codes fuer "order_xo/reko".                             W.J. 23.01.2007
 9. Neuer Code f.unsimulierbare restaurierte u.unsimulierte AKs. W.J. 15.08.2007
10. Neuer Code NOT_IN_THIS_VERSION.                              W.J. 02.01.2008
11. Neuer Code BAD_PARAM_TXT_VALUE.                              W.J. 28.01.2008
12. Neuer Code PREC_UNGEEIGNET                                   W.J. 04.08.2008
13. Erweiterung fuer die MPI-Parallelversion.                    W.J. 29.08.2012
14. Renovierung: Umbenennung von LINK_BAD_ZNR.                   W.J. 27.04.2016
15. Neu: FULFILLMENT_VAL_OUT_OF_RANGE, ACT_VAL_OUT_OF_RANGE und
    weitere Meldungen der GUI, der 2. Engineversion, der neuen
    TSK-Datenhaltung und der Anbindung externer Simulatoren
    sowie des neuen "webIO"-Packages.                            W.J. 29.11.2017
16. Neue Fehlermeldungen fuer Matlab-Kopplung.                   A.B. 19.01.2018
17. Neue Fehlermeldungen fuer das verbesserte MOD-File.          W.J. 09.04.2018
18. Neue Fehlermeldungen fuer "essEvalFrame".                    W.J. 07.05.2018
19. Neue Fehlermeldungen fuer PAR_POP_VERS                       W.J. 15.06.2018
20. Neue Fehlermeldungen fuer ESS: essEvalFrame                  W.J. 18.10.2018
21. Neue Fehlermeldung MORE_CRITERIA_THAN_RESULTS.               W.J. 14.02.2019
--------------------------------------------------------------------------------
Status: o.k.                   Autor: W.Jakob                  Datum: 14.02.2019
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
/* ------------------------------ Fehlercodes: ------------------------------ */
#define ACT_NAME_TO_LONG        1
#define AK_EXPECTED             2
#define APPL_FILE_ERR         123
#define APPL_FILE_GEN_ERR     125
#define APPL_FILE_SEGM_ERR    124
#define APPL_NOT_AVAILABLE    128
#define BAD_ABSCHNITT           3
#define BAD_ACTION              4
#define BAD_ACT_AMOUNT          5
#define BAD_AFILE_AMOUNT      103
#define BAD_AFILE_ANZ         104
#define BAD_AFILE_HDR         122
#define BAD_APPL_CODE          70
#define BAD_APPLICATION        66
#define BAD_BEW_DATA           21
#define BAD_CH_LEN              8
#define BAD_CHAIN_KIND          7
#define BAD_EVAL_FKT           10
#define BAD_EVO_FILE           89
#define BAD_EXP_FILE          116
#define BAD_INIT_FILE         115
#define BAD_EXT_SIMU_CODE     133
#define BAD_EXT_SIMU_KENNUNG  131
#define BAD_G_OP_CODE_ERR      99
#define BAD_GEN_KIND           11
#define BAD_HDR_CODE           12
#define BAD_ITEM               13
#define BAD_KRIT_AMOUNT         9
#define BAD_KRIT_INDX          14
#define BAD_LSV_AMOUNT        142
#define BAD_LSV_IDX           144
#define BAD_LSV_P_SUM         147
#define BAD_LSV_RUN            93
#define BAD_MEM_DEF_ERR        84
#define BAD_MOD_FILE           15
#define BAD_MUT_AMOUNT         16
#define BAD_MUT_DATA           17
#define BAD_PAR_AMOUNT         18
#define BAD_PAR_LIMITS         52
#define BAD_PARAM_VALUE       150
#define BAD_REF_TYP             6
#define BAD_SEGM_LEN           19
#define BAD_SIMU_ROH_ERG_ANZ   55
#define BAD_SIMU_ROOT_DIR1     48
#define BAD_SIMU_ROOT_DIR2     49
#define BAD_STRATEGY           20
#define BAD_TSK_APPL           78
#define BAD_TSK_FILE           77
#define BAK_NOT_FOUND          22
#define BEW_DATA_INKONSISTENT 108
#define BEW_IO_ERROR           45
#define CHAIN_NOT_COPIED       23
#define CH_ERR_LOE_MLDG        57
#define CH_ERR_LOE_SUM_MLDG    59
#define CH_FILE_IO_ERR         56
#define CH_LEN_ZU_KLEIN        71
#define CH_REST_ERR_BAD_HDR    64
#define CH_REST_ERR_BAD_LEN    60
#define CH_REST_ERR_BAD_RECOV  65
#define CH_REST_ERR_MEM_OUT    62
#define CH_REST_ERR_SEGM_ERR   61
#define CH_REST_ERR_UNZUL_HDR  63
#define DYN_AKS_NIX_1COMPL    140
#define DYN_AKS_UNSUITED      156
#define ENV_VAR_HINWEIS       118
#define EVO_DATA_INKONSISTENT 112
#define EVO_FILE_DATA_ANZ_ERR  87
#define EVO_FILE_EOF_ERR       88
#define EVO_SCRATCH_ERR_MELD   90
#define EVO_SCRATCH_REST_ERR   91
#define EVO_WUERG_COPY_MELD    94
#define EVO_WUERG_GOP_MELD     97
#define EVO_WUERG_OPC_MELD     95
#define EVO_WUERG_OFFSPR_MELD  98
#define EVO_WUERG_SIMU_MELD    92
#define EVO_WUERG_INFO_MELD    96
#define EXT_KOPPL_ERR          54
#define EXT_SIMU_ERR_TXT       74
#define EXT_SIMU_NIX_DONE     132
#define FALSCHE_AKT_ANZ       120
#define FILE_NOT_EXISTS        53
#define FILE_NOT_OPEN         127
#define HAM_DIST_NIL_ERR      135
#define INCONSISTANT_GENCODE   69
#define FBHM_BAD_ZNR           51
#define LOG_FILE_ERR          100
#define MEINE_GUETE            25
#define MEM_OUT_NO_RELEASE     26
#define MEM_OUT_WITH_RELEASE   27
#define MOD_IO_ERROR           28
#define MOD_DATA_ERROR         29
#define MOD_DATA_INKONSISTENT 106
#define MORE_ACTIVE_THAN_USED  31
#define NIL_PTR                30
#define NO_ABSCHN              33
#define NO_ACTIVITY_ACT        34
#define NO_ACT_NAME            35
#define NO_AK_GEN             139
#define NO_CEC_INI_FILE       159
#define NO_CHAIN               36
#define NO_CH_ITEM             37
#define NO_CWD_ERR             85
#define NO_DIR_ERR             86
#define NO_EXT_SIM_MODELS     117
#define NO_EXT_SIMU           114
#define NO_GEN_AND_PHAENO_REP 153
#define NO_ID_IN_EXP_FILE      24
#define NO_INI                 38
#define NO_ITEM                39
#define NO_LSV_HEURI_IMPL     143
#define NO_LSV_ACTIVATED      145
#define NO_MEM                 40
#define NO_MOD_AVAIL_ERR       83
#define NO_PARAM_VALUE        151
#define NO_SEGM_END            41
#define NO_SIMU_INIT          113
#define NO_SIMU_NO_ERG        130
#define NO_SUB_CHAIN           42
#define NOT_ENOUGH_MEM         32
#define NOT_IMPL_LSV          146
#define NOT_IN_TSK_FILE       141
#define NOT_YET_IMPLEMENTED    67
#define OLD_BEW_STILL_VALID   107
#define OLD_EVO_STILL_VALID   111
#define OLD_MOD_STILL_VALID   105
#define OLD_TSK_STILL_VALID   109
#define ONLY_GLEAM            155
#define POPEL_OVFL             43
#define READ_ERROR_VARIABLE   158
#define REST_AK_NOT_SIMULATED 157
#define RESTORE_ERR            44
#define SEGM_NOT_FOUND         68
#define EXT_SIMU_DONE          75
#define SIMU_DATA_AMOUNT_ERR   72
#define SIMU_MODS_OVL          50
#define SIMU_RZ_MELD           73
#define GENERAL_MESSAGE        73
#define SIMU_PAS_ROT0         136
#define SUB_AK_NOT_FOUND_ERR   58
#define SW_ERROR              129
#define TOO_FEW_PARAM         149
#define TOO_MANY_ACT_LSV      148
#define TSK_BAD_DATA_ERR       82
#define TSK_DATA_ANZ_ERROR     79
#define TSK_DATA_INKONSISTENT 110
#define TSK_IN_ERROR           76
#define TSK_JOINTS_ERR         80
#define TSK_OUT_ERROR          81
#define TSK_BEZ_ZU_LANG       138
#define UNDEF_BEW_STRF_FKT    134
#define UNGLEICHE_CH_LEN       46
#define UNKNOWN_ID            121
#define UNKNOWN_LSV           102
#define UNKNOWN_OPT_VERF      154
#define UNKNOWN_PARAM         152
#define VERFUEGBARER_SPEICHER 101
#define WRITE_ERR             126
#define ZIEL_AK_OHNE_SEGM     137
#define ZU_VIEL_ABSCHN         47
#define ZU_WENIG_AKTIONEN     119

#define NOT_IN_THIS_VERSION     160
#define BAD_PARAM_TXT_VALUE     161
#define EVO_WUERG_SIMULISTE     162
#define PREC_UNGEEIGNET         163
#define TOP_FSPEC_IGNORED       164
#define REQUIRED_LHCS_NOT_AVAIL 165
#define SIMU_VAL_OUT_OF_RANGE   166
#define FULFILLMENT_VAL_OUT_OF_RANGE  167
#define FITNESS_WEIGHTS_ERROR         168 
#define NO_MATCHING_PEN_AND_FITN_FCN  169
#define PEN_FCN_FOR_EXP_FITN_FCN_MSG  170
#define NO_FITN_OR_PEN_FCN_DEF_ERR    171
#define BAD_DATA_IN_LAST_CHANGED_CRIT 172
#define SIMU_LINES_NOT_SHOWN_ERR      173
#define BAD_EVAL_FKT2                 174
#define PEN_PRIO_EXCEEDS_EVAL_PRIO    175
#define PRIO_COMPRESSED_MSG           176
#define STILL_USED_PRIOS_MSG          177
#define UNKNOWN_PARAMS                178
#define UNKNOWN_OPT_PROC_ERR          179
#define UNKNOWN_INIT_PROC_ERR         180
#define OPT_PROC_IGNORED_MLDG         181
#define HAMMING_DIST_TOO_BIG_MSG      182
#define UNKNOWN_ACCEPT_RULE_ERR       183
#define STATISTICS_PER_GEN_IMPOSSIBLE 184
#define BAD_PARAM_IDX                 185
#define PARAM_NAME_ALREADY_EXISTS     186
#define INVALID_DATA_TYPE_OF_PARAM    187
#define PARAM_NOT_FOUND               188
#define UNKNOWN_ADAPTATION_SPEED      189
#define UNKNOWN_EXT_SIMU_COMMAND      190
#define PARAM_HAS_NO_VALUE            191
#define ACT_VAL_OUT_OF_RANGE          192
#define BAD_TSK_FILE_LINE             193
#define INACTIVE_LHC_SPECIFIED        194
#define BAD_FILE_SPECS_IN_EXP_FILE    195
#define POP_SIZE_FOR_LHC_MSG          196
#define REPAIR_NOT_IMPL               197
#define NO_INT_FOUND                  198
#define BAD_VALUE_ERR                 199
#define BAD_LIST_HEADER_ERR           200
#define HTTP_POST_ERR                 201
#define HTTP_GET_ERR                  202
#define CURL_GENERAL_ERR              203
#define GENERAL_READ_ERR              204
#define GENERAL_WRITE_ERR             205
#define CHROMOSOME_LIST_ERR           206
#define TMO_READ_ERR                  207
#define EMPTY_MODEL_LIST_ERR          208
#define TOO_MANY_ITEMS_ERR            209
#define RESULTS_ALREADY_RECEIVED      210
#define BAD_PHASE_OF_OPTJOB_PROC      211
#define INVALID_ID_ERR                212
#define CHR_LIST_SEND_ERR             213
#define RES_LIST_STATUS_ERR           214
#define JOB_ABORTION_ERR              215
#define MODEL_NAME_CORRECTION_MSG     216
#define MISSING_DATA_OF_OPTION_ERR    217
#define NO_ENV_VAR                    218
#define NO_EXT_SIMU_MODEL_FILE	      219
#define NO_CASE_FILE_WRITTEN	      220
#define NO_POWER_FLOW_OPTION	      221
#define BAD_OR_MISSING_LS_DELIMITER   222
#define UNEXPECTED_LS_DELIMITER_FOUND 223
#define ERROR_READING_CHROMOSOME_LIST 224
#define MEM_OUT_CREATING_RES_LIST     225
#define NO_TOP_FILE                   226
#define TOP_DATA_INKONSISTENT         227
#define TOP_FILE_DATA_ANZ_ERR         228
#define NO_CHROMOSOME_ACCESS          229
#define START_POP_ERROR               230
#define GENE_VAL_OUT_OF_RANGE_ERR     231
#define MORE_CRITERIA_THAN_RESULTS    232


#ifdef MPI_PAR_VERS
  #include "fcodes_par.h"
#endif /* MPI_PAR_VERS */
