/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File
Package: simu                    File: simuInt.h             Version:     V2.3.2
Status : o.k.                   Autor: W.Jakob                 Datum: 16.07.2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Aenderung gegenueber V1.0.0:
 1. Entfernung von BS_WINXP_GNU und PAR_SIMU sowie den dazugehoerigen 
    Deklarationen.                                                   WJ 22.06.12
 2. "value_2_string" wird modullokal in "simu_gsw". "zeige_simu_bew" 
    und "zeige_evo_best_bew" sind package-export bei gesetztem 
    GUI_MODE und ansonsten nur package-lokal.                        WJ 05.12.14
 3. Renovierung: "must_simu" wird modul-lokal in "simucmen".         WJ 13.05.16
 4. "bewert_data_to_int_data" , "plot_akt_wert", "restart_ext_sim",
    "kleiner_ext_restart" und "ak_mem_neu_bewert" werden im GUI_MODE 
     zum Package-Export.                                             WJ 15.09.16
 5. Einfuehrung von "doMatlabSim".                                   WJ 27.04.17
 6. bewert_data_to_???_data() wird Package-Export bei gesetztem 
    GUI_MODE oder gesetztem EXT_SIMU_SERV.                           WJ 10.07.17
 7. Anpassungen an das neue Logging der Simulatorschnittstelle.      WJ 06.10.17
 8. Neue Funktion reportResultOfModelStart().                        WJ 21.11.17
 9. EXT_MATLAB_SIMU: Neue Variable "extractCaseFile".                WJ 01.02.18
10. Anpassung des Exports an den neuen VISU_MODE.                    WJ 23.04.18
11. Die Konstante STRICH_15 wird nach simu_d/gb.h verschoben.        WJ 23.03.18
12. "extractCaseFile" wird wegen der CLV zum Package-Export.         WJ 29.10.18
13. Enlargement of simuLogBuf to avoid buffer overflow.              WJ 16.07.20
-------------------------------------------------------------------------------- */


/* --- Testrahmen des ExtSimu: TEST_SIM gesetzt = Kennziffern per Zufall: --- */
#undef TEST_SIM

#ifdef MITSIMULOG  /* nachfolgende Schalter setzen ein gesetztes MITSIMULOG voraus: */
  #undef BRIEF_LOG 
  #ifdef BRIEF_LOG
    #undef IO_LOG
    #undef DETAILED_LOG
  #else  /* Kein BRIEF_LOG: Nachfolgende Schalter koennen geaendert werden: - */
    #define IO_LOG
    #undef DETAILED_LOG
  #endif /* Kein BRIEF_LOG */
#endif /* MITSIMULOG */

#ifdef IO_LOG
  #define DETAILED_IO_LOG  FALSE
#endif


/* ======================= Package-globale Konstanten: ====================== */
#define SINGLE_LINE        FALSE
#define MULTI_LINE         TRUE

/* --------------- Fuer die Ankopplung externer Simulatoren: ---------------- */
#define IGNORE_ERRORS      TRUE
#define STOP_ON_ERROR      FALSE
#define EXPECT_OLD_PROMPT  TRUE
#define EXPECT_NEW_PROMPT  FALSE



/* ======================== Package-globale Variable: ======================= */

/* -------------------------- aus Modul: simu ------------------------------- */
extern char    simuLogBuf[2*MAX_STRING+MAX_BUF];   /* general buffer for logs */
extern DOUBLE  int_gesamt_note;  /* Ergebnis des letzten interakt. SimuLaufs  */
extern char   *akt_sim_mod_path; /* Pfad zum aktuellen Simu-Modell            */
#ifndef EXT_SIMU_SERV
  extern DOUBLE  best_ges_note;  /* Note der BestAK seit "reset_simu_erg"     */
                                 /* best_ges_note = -1: Keine guelt.Werte     */
#endif /* no EXT_SIMU_SERV */


/* -------------------------- aus Modul: simu_gsw --------------------------- */
extern BOOL    evo_plot_data_ready; /* True = plottbare EvoErgdaten vorhanden */


/* -------------------------- aus Modul: ext_sim ---------------------------- */
extern char   *inbuffer;             /* Daten vom ExtSimu         [BUF_LEN+1] */
extern DOUBLE  to_tics;              /* Umrechnungsfaktor für Simu-Wartezeit  */
#if defined(BS_SOLARIS) || defined(BS_LINUX)
  extern char *errbuffer;            /* Fehlermeldungen d.ExtSimu [BUF_LEN+1] */
  extern LONG  prompt_ctr,           /* Nummer des nachsten erwarteten Prompt */
               wait_ctr,             /* Zaehlt Wartezyklen. Global f."mit_rz" */
               simu_first_waits;     /* Wartezeit auf Simulatorantwort [tics] */
  extern BOOL  ak_bewertet,          /* True, wenn AK erfolgreich bewertet    */
               detailed_iolog;       /* Steuert Detailierungsgrad des IO_LOG  */
#endif /* Solaris oder Linux */




/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: simu -------------------------- */
#if !defined(GUI_MODE) && !defined(EXT_SIMU_SERV)
  extern void bewert_data_to_int_data  (DOUBLE  gesamt_note);

  extern void bewert_data_to_best_data (DOUBLE  gesamt_note);
#endif /* weder GUI_MODE noch EXT_SIMU_SERV */


/* ---------------------- Routinen aus Modul: simu_gsw ---------------------- */
#ifndef GUI_MODE
  extern GSTATE ak_mem_neu_bewert (BOOL   clr_simu_flag);

  extern INT  zeige_simu_bew      (INT    znr,
                                   BOOL   mitIndex);
#endif /* kein GUI_MODE */
#if !defined(GUI_MODE) && !defined(VISU_MODE)
  extern INT  zeige_evo_best_bew  (INT    znr,
                                   BOOL   mitIndex);
#endif /* weder GUI_MODE noch VISU_MODE */

#if defined(GNUPLOT) && !defined (GUI_MODE)
  extern void plot_akt_wert       (INT    krit_nr);
#endif /* GNUPLOT und kein GUI_MODE */



/* ---------------------- Routinen aus Modul: ext_sim ----------------------- */
extern void ext_simu_recovery     (void);

#ifndef GUI_MODE
  extern BOOL restart_ext_sim     (void);

  extern void kleiner_ext_restart (void);
#endif /* kein GUI_MODE */

extern BOOL test_and_read_val     (DOUBLE *wert);

#ifdef ONLINE
  extern BOOL read_line           (INT     in_pipe, 
                                   char   *buf);
#else
  extern BOOL read_line           (FILE   *in_file,
                                   char   *buf);
#endif /* ONLINE */

extern BOOL wait_for_reply        (BOOL   *time_out,
                                   LONG    wcycles);

extern BOOL check_for_prompt      (LONG    wcycles,
                                   INT     err_skips,
                                   BOOL    ident_prompt);

extern BOOL check_for_pid         (INT    *prog_pid,
                                   char   *prog_name);

extern BOOL kill_process          (char   *prog_name);

extern BOOL init_ext_koppl        (BOOL   *ext_sim_mod_ok);


/* ---------------------- Routinen aus Modul: eldokopl ---------------------- */
extern BOOL start_eldo_sim        (char   *mod_name);

extern BOOL restart_eldo_sim      (void);

extern void kleiner_eldo_restart  (void);

extern BOOL do_eldo_sim           (DOUBLE *erg_werte,
                                   INT    *i_params,
                                   DOUBLE *r_params);

extern BOOL make_eldo_startsim    (void);

extern BOOL terminate_eldo_sim    (BOOL    mit_quit);

extern BOOL check_eldo_err_meld   (void);

extern BOOL till_eldo_prompt      (LONG    wcycles,
                                   INT     max_skip,
                                   BOOL    ignore_errors);

extern void set_eldo_data_waits   (void);

extern BOOL init_eldo_koppl       (void);


/* ---------------------- Routinen aus Modul: mathkopl ---------------------- */
extern BOOL start_math_sim        (char   *mod_name);

extern BOOL restart_math_sim      (void);

extern BOOL kleiner_math_restart  (void);

extern BOOL do_math_sim           (DOUBLE *erg_werte,
                                   INT    *i_params,
                                   DOUBLE *r_params);

extern BOOL make_math_startsim   (void);

extern BOOL terminate_math_sim   (BOOL    mit_quit);

extern BOOL check_math_err_meld  (void);

extern BOOL till_math_prompt     (LONG    wcycles,
                                  INT     max_skip,
                                  BOOL    ignore_errors);

extern void set_math_data_waits  (void);

extern BOOL init_math_koppl      (void);


/* ---------------------- Routinen aus Modul: gen_kopl ---------------------- */
extern BOOL start_gen_sim        (char   *mod_name);

extern BOOL restart_gen_sim      (void);

extern BOOL kleiner_gen_restart  (void);

extern BOOL do_gen_sim           (DOUBLE *erg_werte,
                                  INT    *i_params,
                                  DOUBLE *r_params,
				  BOOL    multi_line_call);

extern BOOL make_gen_startsim    (void);

extern BOOL terminate_gen_sim    (BOOL    mit_quit);

extern BOOL check_gen_err_meld   (void);

extern BOOL till_gen_prompt      (LONG    wcycles,
                                  INT     max_skip,
                                  BOOL    ignore_errors);

extern void set_gen_data_waits   (void);

extern BOOL init_gen_koppl       (void);

  
/* -------------------- Routinen aus Modul: matlab_koppl -------------------- */

#ifdef EXT_MATLAB_SIMU
  extern BOOL doMatlabSim (DOUBLE              *erg_werte,
                           CHAIN_ELEM_PTR_TYPE  chrPtr);
#endif /* EXT_MATLAB_SIMU */


/* -------------------- Routinen aus Modul: extSimServKoppl ----------------- */
#ifdef EXT_SIMU_SERV
  extern BOOL do1ExtSimServRun (DOUBLE              *simuResults,
                                CHAIN_ELEM_PTR_TYPE  chrPtr);

  extern void reportResultOfModelStart (char *resBuf);
#endif /* EXT_SIMU_SERV */


/* ------------------ Routinen aus dem Roboter-Subpackage: ------------------ */

/* ---------------------- Routinen aus Modul: rob_sim ----------------------- */
extern BOOL rob_init             (void);



/* -------- Routinen aus dem Aehnlichkeitsvergleich-Subpackage "vgl": ------- */

/* ---------------------- Routinen aus Modul: aehnlvgl ---------------------- */
#ifdef MIT_AEHNL_CHECK   
  extern BOOL unknownIndiv       (DOUBLE *erg_werte,
                                  INT    *i_params,
                                  DOUBLE *r_params);

  extern BOOL update             (DOUBLE *erg_werte,
                                  INT    *i_params,
                                  DOUBLE *r_params);

  extern BOOL initAehnlVgl       (void);
#endif /* MIT_AEHNL_CHECK */



