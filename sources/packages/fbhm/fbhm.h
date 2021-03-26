/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: fbhm                    File: fbhm.h                     Version: 2.1.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Package dient der Fehlerbehandlung. Es koennen drei Arten von Meldungen be-
arbeitet werden; Meldungen, Fehler und fatale Fehler. Die Daten einer Meldung
werden in einem Puffer zwischengespeichert und koennen je nach Variante unter-
schiedlich ausgelesen werden. Je nach eingehenden Meldungen wird eine interne
Statusvariable gepflegt. Das Package enthaelt auch Routinen zum Aufbau der 
Meldungstexte.
Alle Routinen, die einen "GLEAM_..."-Returncode zurueckliefern, benutzen 
"GSTATE", um dies deutlich zu machen.

Ausserdem verwaltet das Package einen Textpuffer zur einfacheren Benutzung vor-
handener Ausgabe-Routinen im Kontext einer zentralisierten Benutzerschnittstelle.


Module des Packages:
    fbhm   Meldungsroutinen, Pufferverwaltung, Initialsierung und Meldungstext-
           aufbau. 
    
--------------------------------------------------------------------------------
Aenderungen:
 1. Entfernung von SLAVE und den dazugehoerigen Deklarationen.   
    GLEAM_C --> TEXT_MODE: "show_and_clear_fbuf".                    WJ 25.06.12
 2. Neue Konstante STATE_OFFSET.                                     WJ 06.08.12
 3. Renovierung: Integration der Routinen zur Verwaltung des Text-
    puffers (ehemaliges Package "link"). Streichung ueberfluessiger 
    Deklarationen. Einfuehrung von "MIT_TERM_AUSG" und "OHNE_TERM_AUSG"
    fuer den Aufruf von "logTextBuffer".                             WJ 19.05.16
 4. Neue Codes fuer die GUI. Aus "AUFG_GSW" wird "AUFG_DATA".        WJ 23.11.16
 5. Neue Codes: SIMU_MATLAB_KOPPL, EVO_PAR_SIM_EVO.                  WJ 05.07.17
 6. Neuer Parameter fuer "show_and_clear_fbuf".                      WJ 10.11.17
 7. New variable "numberOfStdErrMsgs".                               WJ 04.06.18
 8. Better module IDs for the messages.                              WJ 04.10.20
--------------------------------------------------------------------------------
Status : o.k.                   Autor: W.Jakob                 Datum: 04.10.2020
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define GSTATE                INT
#define GLEAM_OK                0       /* Return-Code fuer Fehlerfreiheit    */
#define GLEAM_ERROR             1       /* Return-Code fuer behebbare Fehle   */
#define GLEAM_FATAL             3       /* Return-Code fuer fatale Fehler     */
#define STATE_OFFSET           32       /* Offset zur Uebertragung mit MPI    */
#define NO_INFO        -123456789L      /* Kennung "keine Info"               */

#define MAX_BUF_ANZ           210       /* Max. Groesse des Textpuffer-Feldes */
#define KEEP                FALSE       /* write_text_buf: Puffer belassen    */
#define CLEAR                TRUE       /* write_text_buf: Puffer loeschen    */
#define MIT_TERM_AUSG        TRUE       /* logTextBuffer: Mit Terminalausgabe */
#define OHNE_TERM_AUSG      FALSE       /* logTextBuffer: Ohne Terminalausgabe*/
#define LOG_ALL_MSGS         TRUE       /* logTextBuffer: Mit Terminalausgabe */
#define LIMITED_LOGGING     FALSE       /* logTextBuffer: Ohne Terminalausgabe*/


/* ----------------------------- Modulkennungen: ---------------------------- */
#define SYS_FILE_GSW            "SYS_file_gsw"
#define SYS_EXT_KOPPL           "SYS_extkoppl"
#define CTIO_SCR_GSW            "CTIO_scr_gsw"
#define CTIO_USER_GSW           "CTIO_user_gsw"
#define CTIO_STR_GSW            "CTIO_str_gsw"
#define FBHM_FBHM               "FBHM_fbhm"
#define LGSW_MMSW               "LGSW_mmsw"
#define LGSW_BALISTO            "LGSW_balisto"
#define LGSW_BACHA              "LGSW_bacha"
#define LGSW_CH_INIT            "LGSW_ch_init"
#define CHIO_CHAIN_IO           "CHIO_chain_io"
#define BEW_BEWERTUNG           "BEW_bewert"
#define BEW_GSW                 "BEW_bew_gsw"
#define BEW_CMEN                "BEW_bew_cmen"
#define AUFG_DATA               "AUFG_tsk_data"
#define AUFG_CMEN               "AUFG_aufgcmen"
#define APPL_IF                 "APPL_appl_if"
#define APPL_SIM                "APPL_appl_sim"
#define APPL_GO                 "APPL_appl_go"
#define APPL_GEN                "APPL_appl_gen"
#define APPL_REP                "APPL_appl_rep"
#define APPL_FIO                "APPL_appl_fio"
#define HMOD_DATA               "HMOD_mod_data"
#define HMOD_SEGM_GEN           "HMOD_segm_gen"
#define HMOD_HAMMING            "HMOD_hamming"
#define CHED_CH_AUSG            "CHEDI_ch_ausg"
#define CHED_CH_EDI             "CHEDI_c_ch_edi"
#define SIMU_CMEN               "SIMU_simucmen"
#define SIMU_GSW                "SIMU_simu_gsw"
#define SIMU_SIMU               "SIMU_simu"
#define SIMU_MBF                "SIMU_mbf_sim"
#define SIMU_CEC_MBF            "SIMU_cec_sim"
#define SIMU_MATLAB_KOPPL       "SIMU_matlab_koppl"
#define SIMU_ESD_KOPPL          "SIMU_extSimuServKoppl"
#define SIMU_GENEXTSIM          "SIMU_gen_kopl"
#define SIMU_EXT_SIMU           "SIMU_ext_sim"
#define SIMU_ELDO_KOPPL         "SIMU_eldokopl"
#define SIMU_MATH_KOPPL         "SIMU_mathkopl"
#define SIMU_ROB_PASRO          "SIMU_LSKP_pasromin"
#define SIMU_ROB_GSW            "SIMU_LSKP_rob_gsw"
#define SIMU_ROB_SIM            "SIMU_LSKP_robsimhy"
#define EVO_GO_GSW              "EVO_go_gsw"
#define EVO_GO_RXO              "EVO_go_rxo"
#define EVO_GO_PMUT             "EVO_go_pmut"
#define EVO_GO_AMUT             "EVO_go_amut"
#define EVO_GO_SMUT             "EVO_go_smut"
#define EVO_RC_KOPPL            "EVO_rc_koppl"
#define EVO_ADI                 "EVO_adapt_di"
#define EVO_LSVSTEU             "EVO_lsv_steu"
#define EVO_GSW                 "EVO_evo_gsw"
#define EVO_PAR_SIM_EVO         "EVO_parSimEvo"
#define EVO_EVO                 "EVO_evo"
#define EVO_STEU                "EVO_evo_steu"
#define EVO_CMEN                "EVO_evo_cmen"
#define MEN_FLOAD               "MEN_f_load"
#define MEN_GSW                 "MEN_men_gsw"
#define MEN_CMEN                "MEN_g_cmen"
#define GLEAM_MAIN              "GLEAM-HP"
#define GUI_EVAL                "GUI_EVAL"
#define GUI_SIMU                "GUI_SIMU"
#define GUI_TREE                "GUI_TREE"
#define LIST_IO                 "WEBIO_listIO"
#define CURL_IO                 "WEBIO_curlIO"
#define FILE_IO                 "WEBIO_fileIO"


/* ------------------------------ Fehlercodes: ------------------------------ */
#include "f_codes.h"



/* ====================== Exportierte Typdeklarationen: ===================== */
typedef char MOD_STR_TYPE [22];



/* ========================== Exportierte Variable: ========================= */
extern GSTATE  err_state;             /* Statusvariable fuer Fehler-Level     */
extern INT     numberOfStdErrMsgs;    /* number of standard error messages    */
extern BOOL    textBufOK;             /* TRUE if the text buffer is available */



/* ========================= Exportierte Routinen: ========================== */
extern BOOL   fbhmInit      (BOOL  msg_log);

extern void   meldung       (const MOD_STR_TYPE  modul,
                             LONG                info1,
                             LONG                info2,
                             const STR_TYPE      info_buf,
                             INT                 mnr);

extern GSTATE fehler        (const MOD_STR_TYPE  modul,
                             INT                 herkunft,
                             LONG                info1,
                             LONG                info2,
                             const STR_TYPE      info_buf,
                             INT                 fnr);

extern GSTATE fatal         (const MOD_STR_TYPE  modul,
                             INT                 herkunft,
                             LONG                info1,
                             LONG                info2,
                             const STR_TYPE      info_buf,
                             INT                 fnr);

extern INT  get_fbuf_data   (GSTATE             *state);

extern BOOL get_1msg        (char               *mbuf);

extern void clear_fbuf      (void);

extern void clear_err_state (void);

extern BOOL log_msgs        (void);


extern BOOL write_buf       (INT         line_nr,
                             const char *buf);

extern BOOL get_buf         (INT   line_nr,
                             char *buf);

extern void clear_buf       (INT   line_nr);

extern INT  get_line_anz    (void);

extern void reset_text_buf  (void);

extern void logTextBuffer   (BOOL  mitTerm);



#ifdef TEXT_MODE
extern void show_and_clear_fbuf (BOOL  logAllMsgs);

extern void write_text_buf      (BOOL  del_buf);
#endif /* TEXT_MODE */
