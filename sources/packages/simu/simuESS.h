/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: simu                   File: simuESS.h                  Version: V1.0.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Export of the "matlab_koppl" module which is the simulator interface to Matlab 
within the external simulation services.

--------------------------------------------------------------------------------
Status: impl.                   Author: W.Jakob                Datum: 26.04.2018
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
/* --------------- Fuer die Ankopplung externer Simulatoren: ---------------- */
#define WAIT_FOR_READ      50                         
#define sec                * (1000 / WAIT_FOR_READ) 



/* =========================== Exported Variables: ========================== */
extern INT   simuResultWaitCycles, /* wait cycles for simulator reply TMO     */
             matlabNoConvCtr;      /* counts matlab runs without convergence  */
extern char *extSimuModelName,     /* name of the simulation model of extSimu.*/
            *lastSimuModelName;    /* last loaded simulator model             */



/* ========================== Exportierte Routinen: ========================= */
extern BOOL do_ext_sim          (DOUBLE *erg_werte,
                                 INT    *i_params,
                                 DOUBLE *r_params);

extern BOOL doMatlabSim         (DOUBLE              *erg_werte,
                                 CHAIN_ELEM_PTR_TYPE  chrPtr);

extern BOOL ext_simu_hochfahren (BOOL    mit_fin);

extern void update_ext_simu_tmo (INT     tmo_zeit_in_sec);

extern void terminate_ext_simu  (void);

extern BOOL init_ext_koppl      (BOOL   *ext_sim_mod_ok);

