/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File                
Package: bew                     File: bewInt.h              Version:     V3.2.6
Status : o.k.                   Autor: W.Jakob                 Datum: 08.07.2019
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
----------------------------------------------------------------------------- */


/* ======================= Package-globale Konstanten: ====================== */
/* #define EPSILON           0.000001                              alter Wert */
#define EPSILON           DBL_EPSILON


/* ======================== Package-globale Variable: ======================= */

/* ----------------------- Variable aus Modul: bewert ----------------------- */
extern char     *bez_len_str,      /* "xxx..xxx"-Schablone f.Krit-Bezeichner  */
                *unit_len_str;     /* "xxx..xxx"-Schablone f.Krit-Einheitsbez.*/

#if !defined(GUI_MODE)
  extern INT     akt_anz_used_krit,/* aktuelle Anzahl der genutzten Kriterien */
                 used_krit_og;     /* OG der benutzten Krits in "kriterien"   */
#endif /* kein GUI_MODE */


/* ----------------------- Variable aus Modul: bew_gsw ---------------------- */
#ifdef GNUPLOT
  extern BOOL    gnuplot_done;     /* TRUE, wenn GnuPlot mind. 1x plottet     */

  #if !defined(GUI_MODE)  
    extern STR_TYPE  gnu_win_pos[MAX_GNU];   /* Positionen des Plot-Fensters  */
  #endif /* GNUPLOT und kein GUI_MODE */

#endif /* GNUPLOT */



/* ======================== Package-globale Routinen: ======================= */

/* ----------------------- Routinen aus Modul: bewert ----------------------- */
#ifdef GLEAM_USR
  extern void gewichten        (void);
#endif /* GLEAM_USR */


/* ----------------------- Routinen aus Modul: bew_gsw ---------------------- */
#if defined(GNUPLOT) && !defined(GUI_MODE)
  extern void prep_gnu_plot    (INT     k_index,
                                BOOL    simRes);

  extern void gnu_hard_copy    (void);
#endif /* GNUPLOT und kein GUI_MODE */

#if !defined(GUI_MODE)
  extern INT  zeige_kriterien  (BOOL       *gew_ok);
#endif /* kein GUI_MODE */
#ifdef TEXT_MODE
  extern LONG addiere_noten    (void);
#endif /* TEXT_MODE */

extern BOOL zeige_krit_werte   (INT             znr);

extern void zeige_x_wert       (STR_TYPE        buf,
                                DOUBLE          x_wert);

extern void zeige_bew_par      (INT             znr,
                                const STR_TYPE  param_txt,
                                DOUBLE          x_wert,
                                INT             fkt_index);

extern void fkt_to_text        (char            *buf,
                                const char      *abst_buf,
                                INT              index);

extern void zeig_fkt           (INT             index,
                                INT             krit_index);

extern void compress_crit_name (STR_TYPE        erg_buf,
                                STR_TYPE        name_buf);
      
