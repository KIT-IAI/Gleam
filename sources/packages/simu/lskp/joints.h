/* -----------------------------------------------------------------------------
GLEAM/AE                       Global Header-File
Package: simu/lskp               File: joints.h              Version:     V1.0.0
Status : o.k.                   Autor: C.Blume/W.Jakob         Datum: 15.12.1998
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define MAXAXISANZ     16              /* Obergrenze der Roboterachsen-Anzahl */
#define RADGRAD        57.29578        /* Rad -> Grad                         */
#define GRADRAD       	0.01745329     /* Grad -> Rad                         */


/* ====================== Exportierte Typdeklarationen: ===================== */
typedef double  *JOINTS;

typedef struct IPTYP *IPZEIGER;

typedef struct IPTYP
  { 
    IPZEIGER         next;
    JOINTS           winkel;
    double           min_dist;
    double           ung_note;
  } IPTYP;



/* ========================== Exportierte Variable: ========================= */

/* ------------------------------ aus rob_gsw: ------------------------------ */
extern IPZEIGER   IPanf,
                  IPi,
                  IPloeschen;

/* ------------------------------ aus rob_sim/robsimhy: --------------------- */
extern JOINTS  start_joints,            /* Startpunkt in Joint-Koordinaten    */
               ziel_joints;             /* Zielpunkt in Joint-Koordinaten     */
