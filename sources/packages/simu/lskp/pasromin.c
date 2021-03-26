/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: PASRO-MINi-Umgebung
  
Package: simu                   File: pasromin.c              Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
--------------------------------------------------------------------------------

Der Modul stellt ein Subset von PASRO zugeschnitten auf die Erfordernisse des 
Robotersimulators von LESAK zur Verfuegung. Dabei handelt es sich um die Dekla-
ration der geometrischen Datentypen und Routinen zu ihrer Erzeugung und Initia-
lisierung. 


Die Routinen im einzelnen:

void vsub (VECTOR *vout,
           VECTOR *vin1,
           VECTOR *vin2);
          Die Funktion liefert in "vout" die Vektor-Differenz zwischen dem
          Minuend "vin1" und dem Subtrahend "vin2" ab.

DOUBLE vdist (VECTOR *vin1,
              VECTOR *vin2);
          Berechnet die Distanz zwischen den durch die beiden Vektoren "vin1" 
          und "vin2" bezeichneten Punkten im Raum.
 
VECTOR *new_vector (DOUBLE  x_init,
                    DOUBLE  y_init,
                    DOUBLE  z_init);
          Erzeugt einen neuen Vektor, initialisiert ihn mit den Parametern und
          liefert einen Zeiger darauf ab. Falls wider Erwarten dafuer nicht 
          genuegend Speichertplatz zur Verfuegung stehen sollte, erfolgt eine
          fatale Fehlermeldung und es wird NULL abgeliefert.

ROTATION *new_rot (VECTOR *achse,
                   DOUBLE  winkel);
          Erzeugt eine neue Rotation, initialisiert sie mit den Parametern und
          liefert einen Zeiger darauf ab. Falls wider Erwarten dafuer nicht 
          genuegend Speichertplatz zur Verfuegung stehen sollte, erfolgt eine
          fatale Fehlermeldung und es wird NULL abgeliefert.

FRAME *new_frame (ROTATION *rot,
                  VECTOR   *pos);
          Erzeugt einen neuen Frame, initialisiert ihn mit den Parametern und
          liefert einen Zeiger darauf ab. Falls wider Erwarten dafuer nicht 
          genuegend Speichertplatz zur Verfuegung stehen sollte, erfolgt eine
          fatale Fehlermeldung und es wird NULL abgeliefert.

JOINTS *new_joints(void);
          Erzeugt eine neue Joint-Variable, initialisiert sie als Nulljoints und
          liefert einen Zeiger darauf ab. Falls wider Erwarten dafuer nicht 
          genuegend Speichertplatz zur Verfuegung stehen sollte, erfolgt eine
          fatale Fehlermeldung und es wird NULL abgeliefert.

BOOL pasromin_init (void);
          Legt folgende Daten an und initialsiert sie:
             "speedfactor", "arobotbase"," brobotbase", "crobotbase", 
             "xaxis", "yaxis", "zaxis", "nilvector", "nilrot", "nilframe", 
             "robotframe" und "robotjoints".
          Liefert FALSE, falls dazu nicht genuegend Speicher zur Verfuegung 
          steht, sonst TRUE.
 
--------------------------------------------------------------------------------
basiert auf C-Unit: PASROMIN
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0 vom 05.12.1998:
 1. Fehlermeldung NOT_ENOUGH_MEM korrigiert.                         WJ 31.05.17
 2. Include von "joints.h" hinzugefuegt.                             WJ 08.01.18
--------------------------------------------------------------------------------
Status: o.k.             Autoren: C.Blume / W.Jakob            Datum: 08.01.2018
================================================================================ */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "chaindef.h"
#include "joints.h"
#include "rob_int.h"



/* ======================== Package-globale Variable: ======================= */
DOUBLE     arobotbase,       /*  */ 
           brobotbase,       /*  */ 
           crobotbase,       /*  */ 
           speedfactor;      /*  */ 
VECTOR    *nilvector,        /* Null-Vektor                                   */ 
          *xaxis,            /* Einheitsvektor in Richtung der X-Achse        */ 
          *yaxis,            /* Einheitsvektor in Richtung der Y-Achse        */ 
          *zaxis;            /* Einheitsvektor in Richtung der Z-Achse        */ 
ROTATION  *nilrot;           /* Null-Rotation                                 */ 
FRAME     *nilframe,         /* Null-Frame                                    */ 
          *robotframe;       /*  */ 
JOINTS     robotjoints;      /*  */ 
INT        joints_mem_size;  /* Speicherbedarf eines JOINTS-Objekts in Bytes  */


/* ============================ Lokale Routinen: ============================ */
static VECTOR   *create_vector (void);

static ROTATION *create_rot    (void);

static FRAME    *create_frame  (void);

static JOINTS    create_joints (void);




/* ============================== create_vector ============================= */
static VECTOR *create_vector (void)
{
   return ((VECTOR*)malloc(MAXAXISANZ*sizeof(VECTOR)));
} /* create_vector */



/* =============================== create_rot =============================== */
static ROTATION *create_rot    (void)
{
   return ((ROTATION*)malloc(MAXAXISANZ*sizeof(ROTATION)));
} /* create_rot */



/* ============================== create_frame ============================== */
static FRAME *create_frame  (void)
{
   return ((FRAME*)malloc(MAXAXISANZ*sizeof(FRAME)));
} /* create_frame */



/* ============================= create_joints ============================== */
static JOINTS create_joints (void)
{
   return ((double*)malloc(joints_mem_size));
} /* create_joints */



/* ================================== vsub ================================== */
void vsub (VECTOR *vout,
           VECTOR *vin1,
           VECTOR *vin2)
{
  vout->x = vin1->x - vin2->x;
  vout->y = vin1->y - vin2->y;
  vout->z = vin1->z - vin2->z;
} /* vsub */



/* ================================== vdist ================================= */
DOUBLE vdist (VECTOR *vin1,
              VECTOR *vin2)
{
   return (sqrt (fsqr (vin1->x - vin2->x) +
		 fsqr (vin1->y - vin2->y) +
		 fsqr (vin1->z - vin2->z)  ));
} /* vdist */



/* =============================== new_vector =============================== */
VECTOR *new_vector (double  x_init,
                    double  y_init,
                    double  z_init)
{
   VECTOR *v_erg;

   /* new_vector */
   if ((v_erg = create_vector ()) != NULL)
   { /* Objekt erhalten */
      v_erg->x = x_init;
      v_erg->y = y_init;
      v_erg->z = z_init;
   } /* Objekt erhalten */
   else
      fatal (SIMU_ROB_PASRO, 1, NO_INFO, NO_INFO, "PASRO package", NOT_ENOUGH_MEM);
   return (v_erg);
} /* new_vector */



/* ================================ new_rot ================================= */
ROTATION *new_rot (VECTOR *achse,
                   double  winkel)
{
   ROTATION *r_erg;

   /* new_rot */
   if ((r_erg = create_rot ()) != NULL)
   { /* Objekt erhalten */
      r_erg->axis  = *achse;
      r_erg->angle =  winkel;
      /* Matrix ??? */
   } /* Objekt erhalten */
   else
      fatal (SIMU_ROB_PASRO, 2, NO_INFO, NO_INFO, "PASRO package", NOT_ENOUGH_MEM);
   return (r_erg);
} /* new_rot */



/* ================================ new_frame =============================== */
FRAME *new_frame (ROTATION *rot,
                  VECTOR   *pos)
{
   FRAME *f_erg;

   /* new_frame */
   if ((f_erg = create_frame ()) != NULL)
   { /* Objekt erhalten */
      f_erg->rot    = *rot;
      f_erg->transl = *pos;
   } /* Objekt erhalten */
   else
      fatal (SIMU_ROB_PASRO, 3, NO_INFO, NO_INFO, "PASRO package", NOT_ENOUGH_MEM);
   return (f_erg);
} /* new_frame */



/* =============================== new_joints =============================== */
JOINTS  new_joints (void)
{
   JOINTS  j_erg;
   INT     ii;

   /* new_joints */
   if ((j_erg = create_joints ()) != NULL)
   { /* Objekt erhalten */
      for (ii = 0;  ii < MAXAXISANZ;  ii++)
	 j_erg[ii] = 0.0;
   } /* Objekt erhalten */
   else
      fatal (SIMU_ROB_PASRO, 4, NO_INFO, NO_INFO, "PASRO package", NOT_ENOUGH_MEM);
   return (j_erg);
} /* new_joints */



/* ============================== pasromin_init ============================= */
BOOL pasromin_init (void)
{
   /* pasromin */
   joints_mem_size = MAXAXISANZ * sizeof(DOUBLE);
   arobotbase  = 0.0;
   brobotbase  = 0.0;
   crobotbase  = 0.0;
   speedfactor = 4.0;
   nilvector   = new_vector (0.0, 0.0, 0.0);
   xaxis       = new_vector (1.0, 0.0, 0.0);
   yaxis       = new_vector (0.0, 1.0, 0.0);
   zaxis       = new_vector (0.0, 0.0, 1.0);
   nilrot      = new_rot (zaxis, 0.0);
   if (nilrot == NULL)
      return (FALSE);
   nilrot->matrix.t = *xaxis;                     /* bis dies "new_rot" macht */
   nilrot->matrix.o = *yaxis;                     /* bis dies "new_rot" macht */
   nilrot->matrix.a = *zaxis;                     /* bis dies "new_rot" macht */
   nilframe    = new_frame (nilrot, nilvector);
   robotframe  = new_frame (nilrot, nilvector);
   if (robotframe == NULL)
      return (FALSE);
   robotframe->transl.x = arobotbase;
   robotframe->transl.y = brobotbase;
   robotframe->transl.z = 82.0 + crobotbase;        /* Mitsubishi ist default */
   robotjoints = new_joints ();
   return (robotjoints != NULL);
} /* pasromin */
