/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: men                     File: men.h                      Version: 1.3.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Das Package enthaelt neben der allgemeinen Package-Initialisierung die Menue-
Basisroutinen zusammen mit den Menue-Routinen der TEXT_MODE-Variante und 
Ladefunktionen der Init-Files.

Module des Packages:
    f_load   Allg.Package-Initialisierungen und Ladefunktionen der Init-Files.
    men_gsw  Elementare Routinen der Benutzerfunktionen.
    g_cmen   Menues der TEXT_MODE-Variante.
    
-------------------------------------------------------------------------------- 
Status : o.k.                   Autor: W.Jakob                 Datum: 08.12.2016
-------------------------------------------------------------------------------- */


/* ========================== Exportierte Variable: ========================= */
extern INT     exp_afile_anz;     /* Anzahl im EXP-File angegebener ApplFiles */
#ifdef GUI_MODE
  extern char *tmp_bew_file_spec, /* Temp. File-Bez. fuer Bewertungsfile      */
              *tmp_tsk_file_spec; /* Temp. File-Bez. fuer Parameterfile       */
#endif /* GUI_MODE */



/* ========================== Exportierte Routinen: ========================= */


/* ---------------------- Routinen aus Modul: f_load ------------------------ */
extern BOOL gen_init (BOOL  chio_logging,
                      BOOL  gen_err_logging,
                      char *log_fname);

extern BOOL load_exp (BOOL *mit_ch_file,
                      char *exp_fspec,
                      BOOL  init_mode);

#ifdef GUI_MODE
  extern BOOL load_mod_file (char *mod_fspec,
                             BOOL  init_mode);

  extern BOOL load_bew_file (char *bew_fspec,
                             BOOL  init_mode);

  extern BOOL load_tsk_file (char *tsk_fspec,
                             BOOL  init_mode);

  extern BOOL load_evo_file (char *evo_fspec,
                             BOOL  init_mode);
#endif /* GUI_MODE */


/* ---------------------- Routinen aus Modul: g_cmen ------------------------ */
extern void main_menue    (void);


/* ---------------------- Routinen aus Modul: men_gsw ----------------------- */
extern void     gklassen            (INT     k_art, 
                                     BOOL    alles);

#ifdef GUI_MODE
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
#endif /* GUI_MODE */

