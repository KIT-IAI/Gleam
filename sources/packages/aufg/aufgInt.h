/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File 
Package: aufg                    File: aufgInt.h             Version:     V1.2.0
Status : o.k.                   Autor: W.Jakob                 Datum: 09.01.2016
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
-------------------------------------------------------------------------------- */


/* ======================= Package-globale Konstanten: ====================== */
#define PARAM_FROM_FILE           TRUE
#define PARAM_FROM_MENU          FALSE



/* ==================== Package-globale Typdeklarationen: =================== */



/* ======================== Package-globale Variable: ======================= */



/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: tsk_data ---------------------- */
#ifndef GUI_MODE
  extern void prepareTskDataDisplay (BOOL  withIndices,
                                     BOOL  allParam);
#endif /* kein GUI_MODE */

extern void mod_dir_mecker (STR_TYPE  mod_dir_buf);

extern GSTATE processReadTskData (BOOL  dataFromFile);
