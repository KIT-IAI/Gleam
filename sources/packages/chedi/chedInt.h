/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File  
Package: ched                    File: chedInt.h             Version:     V1.2.0
Status : o.k.                   Autor: W.Jakob                 Datum: 13.05.2016
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Aenderungen:
 1. Aus GLEAM_C wird TEXT_MODE. Verlegung von "prep_gnu_plot" und 
    "gnu_hard_copy" nach bewInt.h.                                   WJ 28.06.12
 2. "zeige_aktion" wird nur bei nicht gesetztem GUI_MODE exportiert. WJ 27.10.14
 3. Renovierung: Streichung von ueberfluessigen Variablen.           WJ 13.05.16
-------------------------------------------------------------------------------- */


/* ======================= Package-globale Konstanten: ====================== */
#define CH_LOKAL   KEINE_GUETE+1  /* Interner Code f. "guete" f.lokale Ketten */
#define CH_BUF_MAX 9              /* Anzahl der lokalen Kettenpuffer d.Editors*/



/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: ch_ausg ----------------------- */
#ifndef GUI_MODE
  extern void set_elem_nr  (int                  elem_nr);

  extern BOOL zeige_aktion (FILE                *dest,
                            CHAIN_ELEM_PTR_TYPE  ptr,
                            BOOL                 on_screen);
#endif

