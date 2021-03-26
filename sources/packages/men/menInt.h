/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File
Package: men                     File: menInt.h              Version:     V1.5.2
Status : o.k.                   Autor: W.Jakob                 Datum: 14.08.2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */


/* ======================= Package-globale Konstanten: ====================== */
#define ZEIGE_ANGEW_AKS    0              /* Codes fuer "ketten_pro_guete"    */
#define ZEIGE_BAKS         1              /* Codes fuer "ketten_pro_guete"    */
#define ZEIGE_CHS_KURZ     2              /* Codes fuer "ketten_pro_guete"    */
#define ZEIGE_CHS_DETAIL   3              /* Codes fuer "ketten_pro_guete"    */
#define BAD_DATA_TXT       "void!"



/* ======================== Package-globale Variable: ======================= */
#ifndef GUI_MODE
  extern char *tmp_bew_file_spec,      /* Temp. File-Bez. fuer Bewertungsfile */
              *tmp_tsk_file_spec;      /* Temp. File-Bez. fuer Parameterfile  */
#endif /* no GUI_MODE */
extern char   *tmp_afile_spec [AFILE_MAX_ANZ],   /* Temp.File-Bez.f.ApplFiles */
              *memFileSpecSave; /* stores last successfully read MEM-FileSpec */
extern BOOL    last_exp_ok, /* TRUE, wenn zuletzt geladenes Exp. fehlerfrei   */
               bew_aend,    /* Zeigt manuelle Aenderungen nach BEW-File-I/O an*/
               tsk_aend;    /* Zeigt manuelle Aenderungen nach TSK-File-I/O an*/
extern BOOL    bad_mod_data,/* TRUE, wenn d. Modell-Daten inkonsist. wurden   */
               bad_bew_data,/* TRUE, wenn d. Bewertungs-Daten inkonsist.wurden*/
               bad_tsk_data,/* TRUE, wenn d. AufgabenDaten inkonsist. wurden  */
               bad_evo_data,/* TRUE, wenn d. EvoParamDaten inkonsist. wurden  */
               bad_top_data;/* TRUE, wenn d. Topologiedaten inkonsist. wurden */



/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: f_load ------------------------ */
#ifndef GUI_MODE
  extern BOOL load_mod_file (char *mod_fspec,
                             BOOL  init_mode);

  extern BOOL load_bew_file (char *bew_fspec,
                             BOOL  init_mode);

  extern BOOL load_tsk_file (char *tsk_fspec,
                             BOOL  init_mode);

  extern BOOL load_evo_file (char *evo_fspec,
                             BOOL  init_mode);
#endif /* kein GUI_MODE */


/* ---------------------- Routinen aus Modul: men_gsw ----------------------- */
#ifndef GUI_MODE
  extern BOOL   file_data_ok        (BOOL    for_evo); 

  extern BOOL   save_exp            (char   *exp_fspec,
                                     BOOL    bew_save,
                                     BOOL    tsk_save,
                                     BOOL    mem_save);

  extern void   zeige_k_laengen     (INT     k_art,
                                     GSTATE *erg_state);

  extern void   prep_lade_status    (void);

  extern void   prep_vers_info      (void);

  extern GSTATE do_gen_ak           (void);

  extern GSTATE do_gen_rep          (char   *erg_buf,
                                     INT     guete,
                                     INT     lfd_nr);

  extern GSTATE do_ham_dist         (char   *erg_buf,
                                     INT     guete1,
                                     INT     lfd_nr1,
                                     INT     guete2,
                                     INT     lfd_nr2);

  extern void   display_mut_zahl    (INT     mut_op,
                                     INT     line_anz);

  extern void   display_lsk_results (INT     k_art);

  extern void   prep_opt_glob_erg   (INT     k_art);

  extern BOOL   do_teste_ak         (INT     guete,
                                     INT     lfd_nr);
#endif /* kein GUI_MODE */

extern void   ketten_pro_guete  (INT     anzeige,
		                 INT     guete,
                                 INT     k_art,
                                 GSTATE *erg_state) ;   

extern void   show_baks         (GSTATE *erg_state);

extern GSTATE ak_to_bak         (char   *erg_buf,
                                 INT     guete,
                                 INT     lfd_nr);

extern GSTATE bak_to_ak         (char   *erg_buf,
                                 INT     guete,
                                 INT     lfd_nr);

extern GSTATE delete_baks       (void);

extern GSTATE baks_to_aks       (void);

extern BOOL   make_ak_str       (char   *buf,
                                 INT     guete,
                                 INT     lfd_nr);

extern void   startMeldung      (char   *vers);

