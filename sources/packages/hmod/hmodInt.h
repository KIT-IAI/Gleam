/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File
Package: hmod                    File: hmodInt.h             Version:     V2.2.3
Status : o.k.                   Autor: W.Jakob                 Datum: 28.06.2019
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */

/* ==================== Package-globale Typdeklarationen: =================== */
typedef struct
  { INT                  akt_anz;
    CHAIN_ELEM_PTR_TYPE  akt_ptr;
  } TAB_ELEM_TYPE;



/* ======================== Package-globale Variable: ======================= */
extern ACT_DESCR_PTR_TYPE *act_descr; /* Pointer-Feld f.d.Aktions-Deskriptoren*/
extern TAB_ELEM_TYPE      *ak1_tab,   /* Tabellen fuer "repair_2_aks"         */
                          *ak2_tab;   /* Tabellen fuer "repair_2_aks"         */
extern INT      *ak_segm_laengen;     /* Segmlen-Wahrsch.f."segmentiere_ch"   */
extern INT       min_ketten_len,      /* Mindest-Kettenlaenge                 */
                 act_prio_sum,        /* Aktuelle Prioritaets-Summe           */
                 act_anz_haelfte,     /* Abgerundete Hälfte der Aktionsanz    */
                 ak_slen_sum,         /* WahrschSumme f. "segmentiere_ak"     */
                 segm_werte_anz,      /* Anzahl gueltiger Segmentlaengen      */
                 gen_akt_mode,        /* Akt.Gen-Modus:Aktionen u.Aktionssequ.*/
                 gen_len_mode;        /* Aktueller Gen-Modus: Kettenlaenge    */
extern DOUBLE    min_delta;           /* fuer "change_r_param"                */
extern BOOL      std_ak_appl,         /* Anwendung mit Standard-AK-Generierung*/
                 std_segm_gen_appl;   /* Stnd-ElemModif- u. AK-SegmGenAnwndg  */
extern DOUBLE    ham_max_abst;        /* MaxAktAbst.AKs fester Laenge(HAMMING)*/
extern char     *appl_ident;          /* Ident-Text der geladenen Applikation */
extern BOOL     *modifyGeneParams;    /* Markierung zu aendernder Genparameter*/



/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: segm_gen ---------------------- */
extern DOUBLE calc_max_abst  (INT     len);

extern void   reset_seq_gen  (void);

extern INT    kettenlaenge   (GSTATE *ret_state);


/* ---------------------- Routinen aus Modul: mod_data ---------------------- */
extern BOOL   aktion_gueltig (SHORT   activity);

