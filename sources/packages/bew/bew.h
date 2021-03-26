/*------------------------------------------------------------------------------
GLEAM/AE                        Package-Header-File               
Package: bew                       File: bew.h                   Version: V3.2.2
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Package enthaelt alle zur Bewertung gehoerigen Datenstrukturen und Routinen.
Dazu gehoeren insbesondere das Lesen und Retten von BEW-Files, die Vorbereitung 
der Bewertung, die Kalkulation ungewichteter Noten und die Vorbereitung der Gnu-
Plot-Ausgabe.

In der TEXT_MODE-Variante enthaelt das Package auch das Bewertungs-Submenue.

Module des Packages:
    bewert     Realisiert die I/O- und die algorithmischen Funktionen.
    bew_gsw    Anzeige-Routinen fuer die GLEAM_USR-Varianten.
    bew_cmen   Bewertungs-Submenue der TEXT_MODE-Variante.

--------------------------------------------------------------------------------
Aenderungen:
 1. Aus GLEAM_C wird TEXT_MODE. Verlegung von "prep_gnu_plot" und 
    "gnu_hard_copy" nach bewInt.h.                                   WJ 28.06.12
 2. "calc_a", "calc_b" und "x_test" sowie einige lokale Konstanten 
    aus "bewert.c" werden Package-Export bei gesetztem GUI_MODE und 
    sind ansonsten nur package-lokal.                                WJ 27.07.15
 3. Renovierung: Punkt 2 wird rueckgaengig gemacht. Aenderung 
    folgender Konstanten:
    EXP_PROP --> EXP_MINCR,  EXP_UPROP --> EXP_MDECR,
    LIN_EXP_PROP_LIN  --> MIXED_MINCR, LIN_EXP_UPROP_LIN --> MIXED_MDECR.
    Streichung der xtics und des "kurz_name" aus KRITERIUMS_TYP.
    Anpassung von "c_gnu_hard_copy". "c_prep_gnu_plot" wird immer 
    exportiert.                                                      WJ 27.04.16
 4. Anpassung von ENAME_LEN. Neu: "critLine2Index".  Einige bisher 
    package-lokale Variable oder Funktionen werden Package-Export
    bei gesetztem GUI_MODE. Neue Variable: "simValOutOfEvalRange",
    "int_simValOutOfEvalRange" und "best_simValOutOfEvalRange". 
    Ueberarbeitung von "zeige_kriterien" und "zeige_krit_werte.      WJ 02.08.16
 5. Anpassung an ueberarbeitete Bewertung. Neue Routinen 
    "reset_krit_gueltig" und "resetSimValOutOfEvalRange".            WJ 25.08.16
 6. "addiere_noten" wird bei nicht gesetztem TEXT_MODE exportiert.   WJ 19.01.18
--------------------------------------------------------------------------------
Status: o.k.                      Autor: W.Jakob               Datum: 19.01.2018
-------------------------------------------------------------------------------- */



/* ========================= Exportierte Konstanten: ======================== */
#define NOTEN_DRITTEL     (RNOTEN_MAX/3.0)
#define MIN_ANZ_BEW_KRIT   16       /* StartAnz d.echten Bewertungskriterien  */
#define KRIT_BEZ_LEN       19       /* MaxLen der "bezeichnung" eines Krits   */
#define ENAME_LEN          12       /* MaxLen der "einheit" eines Kriteriums  */
#define KRITERIUM_AUS       0       /* Kennung fuer inaktive Kriterien        */
#define LIN_MINCR           0       /* Lin. dreisegmentige steigende Bewertung*/
#define LIN_MDECR           1       /* Lin. dreisegmentige fallende Bewertung */
#define EXP_MINCR           2       /* steigende exponentielle Bewertung      */
#define EXP_MDECR           3       /* fallende exponentielle Bewertung       */
#define MIXED_MINCR         4       /* monoton steigende Kombi.aus LIN u. EXP */
#define MIXED_MDECR         5       /* monoton sfallende Kombi.aus LIN u. EXP */

#define SHOW_SIM_RES       TRUE     /* Fuer den "simRes"-Parameter von        */
#define SHOW_BEW_STR_FKT   FALSE    /* "c_prep_gnu_plot" (GnuPlot-Aufruf).    */

#define GNU_WIN_SIZE    "565x410"   /* GnuPlot-Fenster                        */



/* ====================== Exportierte Typdeklarationen: ===================== */
typedef struct 
  {
    INT     klasse,                        /* Klasse (Prio) des Kriteriums    */
            bew_alg;                       /* Bewertungsalgorithmus           */
    DOUBLE  noten_gewicht,                 /* Gewichtung des Kriteriums in %  */
            max_note,                      /* Maximale Note des Kriteriums    */
            erfuellungs_note,              /* Mindestnote zur KritErfuellung  */
            a1, a2, a3,                    /* a und b von "y = ax + b" der    */
            b1, b2, b3,                    /* drei Geradensegmente            */
            xstart, xs1, xs2, xend,        /* Begrenzungen der 3 Geradensegm. */
            nstart, ns1, ns2, nend,        /* Noten obiger x-Werte            */
            xdrittel,                      /* f.ExpBew: f(xdrittel) = 1/3     */
            erfuellungs_wert;              /* x-Wert zur "erfuellungs_note"   */
    BOOL    nmax_cont;                     /* TRUE: N=RNOTEN_MAX f. x > xmax  */
    char    bezeichnung [KRIT_BEZ_LEN+2];  /* Langname, 19 Zeichen plus ":"   */
    char    einheit     [ENAME_LEN+1];     /* Mass-Einheit, max. 12 Zeichen   */
  } KRITERIUMS_TYP;


/* ============================ globale Variable: =========================== */

/* ------------------------------ aus bewert: ------------------------------- */
extern KRITERIUMS_TYP *kriterien;      /* Bewertungskriterien                 */
extern DOUBLE *simu_erg,        /* Aufbereitete Daten eines SimuLaufs         */
              *int_simu_erg,    /* Aufb.Daten d.letzten interaktiven SimuLaufs*/
              *best_simu_erg,   /* Aufb.Daten der BestAK seit "reset_simu_erg"*/
              *ungew_note,      /* Ungew. Noten aller Krits [max_anz_bew_krit]*/
              *int_ungew_note,  /* Erg. der letzten interaktiven Simulation   */
              *best_ug_note,    /* UGNote der BestAK seit "reset_simu_erg"    */
              *gew_note,        /* Gewichtete Noten aller Kriterien           */
              *int_gew_note,    /* Ergebnis des letzten interaktiven SimuLaufs*/
              *best_g_note,     /* Gewicht.Note d.BestAK seit "reset_simu_erg"*/
              *straf_fkt,       /* angewandte Straffaktoren                   */
              *int_straf_fkt,   /* Ergebnis des letzten interaktiven SimuLaufs*/
              *best_str_fkt,    /* StrafFkts. d.BestAK seit "reset_simu_erg"  */
               last_x_start,    /* Start des X-Bereichs der GnuPlot-Anzeige   */
               last_delta,      /* Zwischenschritte der GnuPlot-Anzeige       */
               gnu_x_max;       /* Ende des X-Bereichs der GnuPlot-Anzeige    */
extern INT     max_klasse,      /* Maximaler Klassenwert                      */
               akt_anz_krit,    /* maximale Anzahl der aktiven Kriterien      */
               max_anz_bew_krit,/* maximale Kriterienanzahl im Feld           */
               krit_og,         /* OG d.aktiven Kriterien im "kriterien"-Feld */
               max_anz_kriterien,/* MaxAnz der Kriterien und Straffunktionen  */
               last_krit,       /* Zuletzt bearbeitetes Kriterium             */
               x_anz;           /* Anzahl der Zwischenschritte d.GnuPlot-Anzg */
extern BOOL   *krit_gueltig,    /* Krits.entspr.Gueltigk. [max_anz_kriterien] */
              *int_krit_gueltig,/* Ergebnisse des letzten interakt.SimuLaufs  */
              *best_krit_g,     /* Gueltige Krit d.BestAK seit"reset_simu_erg"*/
              *simValOutOfEvalRange,   /* SimuWert im Bereich der Bestrafung  */
              *int_simValOutOfEvalRange, /* sim... der letzten interakt. Simu */
              *best_simValOutOfEvalRange,/* der bestAK  seit "reset_simu_erg" */
               record_evo_best,        /* TRUE: SimuDaten von EvoBest sammeln */
               std_simu_interface,     /* TRUE, wenn Simu-Standard-Schnittst. */
               bew_aend_sim;           /* TRUE: Neubew. wg. BewAenderung notw.*/
#ifdef GUI_MODE
  extern INT  *critLine2Index,   /* Krit-Indices in d. Darstellungsreihenfolge*/
               akt_anz_used_krit,/* aktuelle Anzahl der genutzten Kriterien   */
               used_krit_og;     /* OG der benutzten Krits in "kriterien"     */
#endif /* GUI_MODE */


/* ------------------------------ aus bew_gsw: ------------------------------ */
#ifdef GNUPLOT
  extern char         *gnu_buffer;     /* Puffer fuer "gnuplot"-Kommando      */
  extern FILE         *gnu_com_file;   /* Kommandofile fuer "gnuplot"         */
  extern char         *gnu_fname;      /* GnuPlot-File-Name ohne Extention    */
  extern INT           last_gnu_index; /* letzter GnuPlot-Fenster-Index       */
#endif /* GNUPLOT */

#if defined(GNUPLOT) && defined(GUI_MODE)  
  extern STR_TYPE  gnu_win_pos[MAX_GNU];     /* Positionen des Plot-Fensters  */
#endif /* GNUPLOT und GUI_MODE */



/* ========================== Exportierte Routinen: ========================= */
extern BOOL   bew_init        (void);


/* ---------------------- Routinen aus Modul: bewert ------------------------ */
extern DOUBLE ak_bewerten     (BOOL    interaktiv);

extern DOUBLE calc_phaen_dist (DOUBLE *simu_erg1,
                               DOUBLE *simu_erg2);

extern DOUBLE calc_ungew_note (INT     indx,
                               double  xwert);

extern GSTATE save_bew        (char   *file_name);

extern GSTATE restore_bew     (char   *file_name, 
                               BOOL   *bad_data);

extern GSTATE prepare_eval    (void);

extern void   reset_krit_gueltig        (void);

extern void   resetSimValOutOfEvalRange (void);

#ifndef GLEAM_USR
  extern void gewichten       (void);
#endif /* no GLEAM_USR */



/* ---------------------- Routinen aus Modul: bew_cmen ---------------------- */
#ifdef TEXT_MODE
  extern BOOL bew_men         (void);
#endif /* TEXT_MODE */

#if defined(GNUPLOT) 
  extern void kill_all_plots  (void);

  extern void c_gnu_hard_copy (void);
#endif /* GNUPLOT */

extern BOOL c_prep_gnu_plot (INT   fkt_nr,
                             INT   krit_nr,
                             BOOL  simRes);


/* ---------------------- Routinen aus Modul: bew_gsw ----------------------- */
extern void make_krit_text     (char       *erg_buf,
                                const char *txt_buf,
                                INT         index);

#if defined(GNUPLOT) && defined(GUI_MODE)
  extern void prep_gnu_plot    (INT     k_index,
                                BOOL    simRes);

  extern void gnu_hard_copy    (void);
#endif /* GNUPLOT und GUI_MODE */

#if defined(GUI_MODE)
  extern INT zeige_kriterien   (BOOL       *gew_ok);
#endif /* GUI_MODE */
#ifndef TEXT_MODE
  extern LONG addiere_noten    (void);
#endif /* no TEXT_MODE */
