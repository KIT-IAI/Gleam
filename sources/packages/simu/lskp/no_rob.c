/*******************************************************************************
GLEAM/AE                                                                  V1.1.5

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: ROBot-SIMulator
 
Package: simu                   File: rob_sim.c               Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Der Modul enthaelt Dummy-Routinen zur Befriedigung der Schnittstelle fuer den
Robotersimulator und die Teach-In-Komponente von LESAK. 

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. NOT_YET_IMPLEMENTED ersetzt durch NOT_IN_THIS_VERSION.           WJ 02.01.08
 2. Entfernung der Routine "write_to_simulog", die nun vom Package 
    "sys" zur Verfuegung gestellt wird.                              WJ 20.04.12
 3. Renovierung: Streichung des "link"-Includes. Aenderung der 
    Aufrufschnittstelle von "ladekinematik" und "hole_obs".          WJ 13.05.16
 4. Lokalisierung von JOINTS: Die Variablen "start_joints" und 
    "ziel_joints" werden von dem Modul "rob_sim.c" bzw. "robsimhy.c" 
    exportiert. Diese werden nur in den Packages "aufg" und "simu/lskp" 
    benoetigt. "joints.h" wird nicht mehr in "simu.h" inkludiert und 
    damit automatisch exportiert, sondern muss stattdessen separat 
    von den Module inkludiert werden, die es wirklich benoetigen. 
    Damit wird der Package-Import insgesamt schlanker.               WJ 08.01.18
--------------------------------------------------------------------------------
Status: o.k.                  Autoren: W.Jakob                 Datum: 08.01.2018
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "simu.h"


/* ========================== Exportierte Variable: ========================= */
IPZEIGER   IPanf        = NULL,
           IPi          = NULL,
           IPloeschen   = NULL;
JOINTS     start_joints,            /* Startpunkt in Joint-Koordinaten        */
           ziel_joints;             /* Zielpunkt in Joint-Koordinaten         */



/* ======================== Package-globale Variable: ======================= */




/* =============================== do_rob_sim =============================== */
BOOL do_rob_sim (DOUBLE              *simu_erg_werte,
                 CHAIN_ELEM_PTR_TYPE  ch_ptr,
                 BOOL                 interaktiv)
{
   INT     ii;
   GSTATE  fstat = GLEAM_OK;
   BOOL    simu_done;

   /* do_rob_sim */
   if (ch_ptr->u_el.activity != AK_HEADER)
   { /* Kette ist keine AK */
      fehler (SIMU_ROB_SIM, 1, ch_ptr->u_el.activity, NO_INFO, "", 
	      BAD_CHAIN_KIND);
      return (FALSE);                                              /* Abbruch */
   } /* Kette ist keine AK */
   for (ii = 0;  ii < 7;  ii++)
      simu_erg_werte[0] = 0.0;                            /* keine Ergebnisse */
   fehler (SIMU_ROB_SIM, 2, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);

   return (FALSE);
} /* do_rob_sim */



/* ============================ update_rob_task ============================= */
BOOL update_rob_task (void)
{ /* update_rob_task */
   fehler (SIMU_ROB_SIM, 3, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
   return (FALSE);
} /* update_rob_task */



/* ============================= ladekinematik ============================== */
BOOL ladekinematik (const char *file_name,
		    INT         znr)
{
   fehler (SIMU_ROB_SIM, 4, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
   return (FALSE);
} /* ladekinematik */



/* ================================ hole_obs ================================ */
BOOL hole_obs (const char *file_name,
	       INT         znr)
{
   fehler (SIMU_ROB_SIM, 5, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
   return (FALSE);
} /* hole_obs */



/* =========================== hindernismanagement ========================== */
void hindernismanagement (void)
{
   meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
} /* hindernismanagement */



/* ============================= write_code_file ============================ */
BOOL write_code_file (const char *fspec, 
	              INT         guete, 
	              INT         lfd_nr)
{
   fehler (SIMU_ROB_SIM, 6, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
   return (FALSE);
} /* write_code_file */



/* ================================ rob_init ================================ */
BOOL rob_init (void)
{
   BOOL  ok;

   /* rob_init */
  return (TRUE);
} /* rob_init */



/* ============================ zeige_start_ziel ============================ */
void zeige_start_ziel (INT  znr,
                       BOOL interaktiv)
{
   reset_text_buf ();
   meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
} /* zeige_start_ziel */



/* ================================ teach_in ================================ */
BOOL teach_in (void)
{
   meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
   return (FALSE);
} /* teach_in */



/* ============================== lesak_p_test ============================== */
BOOL lesak_p_test (CHAIN_ELEM_PTR_TYPE *ch_ptr,
		   GSTATE              *ret_state)
{
   if (*ret_state == GLEAM_OK)
     fehler (SIMU_ROB_SIM, 7, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
   return (FALSE);
} /* lesak_p_test */
