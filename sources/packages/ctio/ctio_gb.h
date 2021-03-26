/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Englisch)           
Package: ctio                    File: ctio_gb.h             Version:     V1.6.0
Status : o.k.                   Autor: W.Jakob                 Datum: 14.08.2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Aenderungen:
 1. Entfernung der Definitionen fuer BGI_MODE.                       WJ 20.06.12
 2. Renovierung: Streichung obsoleter Texte.                         WJ 10.05.16
 3. Erweiterung der Startmeldung wegen der LGPL.                     WJ 12.09.16
 4. Headertexts for GLEAM/AE, HyGLEAM/A and HyGLEAM/OPALV moved to 
    vers_d.h                                                         WJ 14.08.20
-------------------------------------------------------------------------------- */

/* ----------------------- Texte des Moduls: scr_gsw ------------------------ */
#define BAD_MTIEFE_TXT  "Bad menu depth: %d! Max=%d."
#define BAD_MENITEM_TXT "Bad menue item number: %d! Max=%d."
#define END_ITEM_TXT    "Close"
#define MEN_QUERY_TXT   "Select menu item: "


/* ----------------------- Texte des Moduls: user_gsw ----------------------- */
#define WANDEL_FMELD      " format error. Repeat input!"
#define ZU_KLEIN_TXT      "Value to small!  Minimum = "
#define ZU_GROSS_TXT      "Value to large!  Maximum = "


/* ----------------------- Texte des Moduls: str_gsw ------------------------ */
#define FDIR_QUERY        "Directory (CR=current): "
#define F_AKT_DIR_TXT     "current "
#define F_DIR_TXT         "\"%s\"-"
#define F_KEINE_TXT       "No %s-files in %sdirectory!"
#define FSELECT_QUERY     "File selection (1..%d): "
#define SF_F_DEF_QUERY    "\"%s\"-FileSpec (without ext) [%s]: "
#define SF_FSPEC_QUERY    "\"%s\"-FileSpec (without ext): "
#define SF_EXIST_QUERY    "File already exists! Overwrite? "NEIN_JA_TXT
#define SF_BAD_SPEC       "No access to \"%s\"!"
#define CH_ADR_QUERY      "[class/index]: "
#define INVALID_ITEM_TXT  "Invalid menue item selected!"
