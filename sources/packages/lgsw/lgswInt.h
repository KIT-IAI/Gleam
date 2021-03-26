/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File              cc / Solaris 2.4
Package: lgsw                    File: lgswInt.h             Version:     V1.0.0
Status : o.k.                   Autor: W.Jakob                 Datum: 14.11.1995
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */


/* ======================== Package-globale Routinen: ======================= */
/* ------------------------ Routinen aus Modul mmsw:  ----------------------- */
extern void   incr_rel_aks  (LONG  amount);

extern void   reset_rel_aks (void);

extern GSTATE mmsw_init     (void);


/* ------------------------ Routinen aus Modul bacha: ----------------------- */
extern void   bacha_init    (void);
