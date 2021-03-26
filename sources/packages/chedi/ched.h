/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: ched                    File: ched.h                    Version: V1.1.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Das Package enthält Routinen zur Aktions- und Kettenausgabe und den Ketteneditor.

Module des Packages:
    ch_ausg   Ausgabe von Ketten und Kettenelementen mit Hilfe des Textpuffers 
              des "fbhm"-Packages. Fuer die GLEAM_USR-Varianten.
    c_ch_edi  Der Ketteneditor der TEXT_MODE-Variante.

--------------------------------------------------------------------------------
Aenderungen:
 1. GLEAM_C wird gestrichen, da das Package nur fuer die textuelle 
    Oberflaeche ueberstzt wird. Neuorganisation entsprechend Punkt 3 
    von "ch_ausg".                                                   WJ 28.06.12
 2. "zeige_aktion" und "c_chain_edi" werden nur bei gesetztem
    TEXT_MODE exportiert.                                            WJ 10.11.14
-------------------------------------------------------------------------------- 
Status : O.k.                   Autor: W.Jakob                 Datum: 10.11.2014
-------------------------------------------------------------------------------- */



/* ---------------------- Routinen aus Modul: ch_ausg ----------------------- */
#ifdef GUI_MODE
  extern void set_elem_nr       (int                  elem_nr);

  extern BOOL prep_zeige_aktion (CHAIN_ELEM_PTR_TYPE  ptr);
#endif /* GUI_MODE */


#ifdef TEXT_MODE
  extern void zeige_kette       (CHAIN_ELEM_PTR_TYPE  ptr);


/* --------------------- Routinen aus Modul: c_ch_edi ----------------------- */
  extern void c_chain_edi       (void);
#endif /* TEXT_MODE */



