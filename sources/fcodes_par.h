/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: fbhm                    File: fcodes_par.h               Version: 1.0.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Die Datei enthaelt die Fehlercodes und wird von "f_codes.h" bei gesetztem
MPI_PAR_VERS inkludiert.
Alle Fehlercodes werden relativ zu PAR_FNR_BEGIN beginnend mit PAR_FNR_BEGIN
definiert.

--------------------------------------------------------------------------------
Aenderungen gegenueber V1.0.0:
--------------------------------------------------------------------------------
Status: o.k.                   Autor: W.Jakob                  Datum: 06.12.2012
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define PAR_FNR_BEGIN          1000

/* ------------------------------ Fehlercodes: ------------------------------ */
#define UNKNOWN_MSG            PAR_FNR_BEGIN
#define CHIO_SEND_FAIL         PAR_FNR_BEGIN +  1
#define CHIO_RECV_FAIL         PAR_FNR_BEGIN +  2
#define OLD_TOP_STILL_VALID    PAR_FNR_BEGIN +  4
#define UNEXPECTED_MSG         PAR_FNR_BEGIN +  6
#define BAD_IND_IDX            PAR_FNR_BEGIN +  7
#define TOP_FILE_EOF_ERR       PAR_FNR_BEGIN +  9
#define BAD_TOP_FILE           PAR_FNR_BEGIN + 10
#define UNKNOWN_STATE_ERR      PAR_FNR_BEGIN + 11
#define OBSOLET_TSK_FILE_DATA  PAR_FNR_BEGIN + 12
#define OBSOLET_SWITCH_DATA    PAR_FNR_BEGIN + 13
#define NO_MEM_ERR             PAR_FNR_BEGIN + 14
#define UNKNOWN_SOURCE         PAR_FNR_BEGIN + 15
#define TOO_MANY_PAR_AK_MSGS   PAR_FNR_BEGIN + 16
#define TOO_MANY_REC           PAR_FNR_BEGIN + 17
#define AK_EMPFANGSFEHLER      PAR_FNR_BEGIN + 18
#define BAD_MSG_OR_STATE       PAR_FNR_BEGIN + 19
#define MSG_BUF_STILL_IN_USE   PAR_FNR_BEGIN + 20
#define MSG_REC_ERR            PAR_FNR_BEGIN + 21
#define MSG_LEN_ERR            PAR_FNR_BEGIN + 22
#define EVO_START_POP_ERR      PAR_FNR_BEGIN + 23
#define POP_SIZE_ENLARGED_MELD PAR_FNR_BEGIN + 24
#define POP_SIZE_TOO_SMALL_ERR PAR_FNR_BEGIN + 25
#define BAD_SLAVE_STATE        PAR_FNR_BEGIN + 26
#define WRONG_AC_RECEIVED      PAR_FNR_BEGIN + 27
#define AC_WITH_BAD_FITNESS    PAR_FNR_BEGIN + 28
#define UNEXPECTED_MSG_OR_SND  PAR_FNR_BEGIN + 29
#define NO_SLAVE_RESPONSE      PAR_FNR_BEGIN + 30
#define AC_SEND_ERR            PAR_FNR_BEGIN + 31
#define AC_DOES_NOT_EXIST      PAR_FNR_BEGIN + 32
#define EVO_CRASH_ERR          PAR_FNR_BEGIN + 33
#define NO_RES_NO_SIM_NO_SAVE  PAR_FNR_BEGIN + 34
#define TMO_RISK               PAR_FNR_BEGIN + 35


/* ---------------------------- Modulkennungen: ----------------------------- */
#define PCOM_SENDREC            "PCOM_SNDRC"
#define PCOM_PARDATA            "PCOM_PDATA"
#define CHIO_MPI                "CHIO_MPI"
#define MPAR_MASTER             "MPAR_MSTR"
#define MPAR_SLAVE              "MPAR_SLAV"
#define MPAR_GSW                "MPAR_GSW"
#define MPAR_ENGINE             "MPAR_ENG"

