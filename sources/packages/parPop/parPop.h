/* ---------------------------------------------------------------------------------------
GLEAM/AE                            Package-Header-File                Version:     V1.0.0
Package: parPop                        File: parPop.h
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
------------------------------------------------------------------------------------------

This package contains functions for the parallel execution of an optimization job based on 
the island model.

Modules of the package:
    parPop   Functions for the initializer and the islands. ...

------------------------------------------------------------------------------------------
Status: impl                 Author: W.Jakob / P.Ostheimer                Date: 06/27/2018
------------------------------------------------------------------------------------------ */



/* ================================ Exported Constants: =============================== */
#define ISLAND_TXT       "_island-%03d"



/* ============================ Exported Type Declarations: =========================== */



/* ================================ Exported Variables: =============================== */

/* ------------------------------- from module: parPop -------------------------------- */




/* ================================= Exported Routines: =============================== */

/* -------------------------- routines from module: parPop ---------------------------- */
extern BOOL initializer   (void);

extern BOOL initParPop    (void);



