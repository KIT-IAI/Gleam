/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File 
Package: ctio                    File: ctio.h                    Version: V1.3.4
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Package enthaelt die Terminal I/O-Routinen der TEXT_MODE-Variante und einige 
allgemeine String-Routinen.

Module des Packages:
  Fuer TEXT_MODE:
    scr_gsw    Low-Level-I/O-Routinen. Menue-Routinen, Zeileneditor.
    user_gsw   Scrolling und Werte-Eingabe-Routinen.
    str_gsw    String- und FileSpec-Query-Routinen des C-Kontexts sowie
               allgemeine String-Routinen.

--------------------------------------------------------------------------------
Aenderungen:
 1. Entfernung der Schalter und dazugehoerigen Deklarationen:
    BS_DOS, BS_WINXP_GNU u. BGI_MODE. Zuordnung des Routinen-Exports 
    zu TEXT_MODE und internem Export.                                WJ 21.06.12
 2. Doku aktualisiert. "maus_delay" gestrichen.                      WJ 29.06.12
 3. Integration des Moduls "ctio_job" in "ctio_eng".                 WJ 01.08.12
 4. Renovierung: Streichung von "set_main_window", write_line, 
    "clear_main_window", clr_line, "clear_dialog_window", 
    "set_menue_window", "clear_menue_window", "select_scroll" und 
    "write_scroll". Streichung aller BGI-Spezifika. 
    Aus "init_normal_window" wird "startMeldung". Streichung der 
    Zeilennummer-Parameter bei den "lies_..."-Routinen,
    "erfrage_outfile_spec" und "lies_adr".                           WJ 10.05.16
 5. Shift of startMeldung() to package "men".                        WJ 14.08.20
-------------------------------------------------------------------------------- */

/* ======================== Exportierte Konstanten: ========================= */
#define ADJUST                TRUE
#define NO_ADJUST             FALSE
#define MEN_ITEM_LEN          10     /* Menue-Itemgroesse. Wird auch fuer     */
                                     /* Anwendungsfiles in "hmod" benoetigt.  */
#if defined(BS_SOLARIS) || defined(BS_LINUX)
  #define CR                  (char)(10)    /* auch von "ext_simu" benoetigt. */
#endif /* Solaris oder Linux */
#ifdef TEXT_MODE
  #define ESC                 (char)(27)
  #define MAX_LINE            60     /* Seitenlaenge bei seitenweiser Ausgabe */
#endif /* TEXT_MODE */



/* ========================== Exportierte Variable: ========================= */
#ifdef TEXT_MODE
  extern const INT  StndMenItem,     /* Standard-Menue-Item                   */
                    DarkMenItem,     /* aktuell ungueltiges Menue-Item        */
                    AttMenItem;      /* Menue-Item in Warnfarbe               */
#endif /* TEXT_MODE */



/* ========================== Exportierte Routinen: ========================= */

/* ---------------------- Routinen aus Module str_gsw: ---------------------- */
extern void display_time         (char *erg_buf,
                                  LONG  t_in_sec,
                                  BOOL  adjust);

extern void adr_to_text          (char *res_buf,
                                  INT   guete,
                                  INT   lfd_nr,
                                  BOOL  adjust);


/* ---------------------- Routinen aus Module scr_gsw: ---------------------- */
#ifdef TEXT_MODE
extern char char_dialog          (const char *out_text);

extern BOOL dialog               (const char *out_text,
                                  char       *in_text);

extern void set_and_clear_menue_window (INT         men_tiefe,
                                        const char *men_title);

extern void write_menue_item     (INT         item_nr,
                                  INT         item_color,
                                  const char *item_text);

extern INT  select_menue_item    (void);

extern void menue_completion     (void);

extern void ctioInit             (void);


/* ---------------------- Routinen aus Module user_gsw: --------------------- */
extern BOOL get_word             (USHORT *w,
                                  char   *str);

extern BOOL get_int              (INT    *i,
                                  char   *str);

extern BOOL get_single           (SINGLE *s,
                                  char   *str);

extern BOOL get_real             (DOUBLE *r,
                                  char   *str);

extern BOOL lies_wert            (INT          *wert,
                                  const char   *qbuf,
                                  INT           ug,
                                  INT           og,
                                  INT           def);

extern BOOL lies_lwert           (LONG         *wert,
                                  const char   *qbuf,
                                  LONG          ug,
                                  LONG          og,
                                  LONG          def);

extern BOOL lies_swert           (SINGLE       *wert,
                                  const char   *qbuf,
                                  SINGLE        ug,
                                  SINGLE        og,
                                  SINGLE        def);

extern BOOL lies_rwert           (DOUBLE       *wert,
                                  const char   *qbuf,
                                  DOUBLE        ug,
                                  DOUBLE        og,
                                  DOUBLE        def);

extern BOOL lies_adr             (INT         *guete,
                                  INT         *lfd_nr,
                                  const char  *v_bezeichner);

extern BOOL erfrage_fspec        (char        *fspec_buf,
                                  const char  *ext_buf);

extern BOOL erfrage_outfile_spec (BOOL        *neu_file,
                                  char        *fspec,
                                  const char  *def_fspec,
                                  const char  *abbr_buf);
#endif /* TEXT_MODE */
