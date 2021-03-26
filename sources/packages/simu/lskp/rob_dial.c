/*******************************************************************************
GLEAM/AE                                                                  V1.0.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: ROBotersimulator-DIALogroutinen
 
Package: simu                   File: rob_dial.c              Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
--------------------------------------------------------------------------------

Menue- und Dialog-Routinen des Robotersimulators fuer die TEXT_MODE-Version.

Die Teach-In-Komponente erlaubt die Festlegung von Start- und Zielpunkt. Wenn
die Animation nicht zur Verfuegung steht (nicht gesetzter Compilerschalter MIT_
ANIM), erlaubt die Teach-In-Komponente die Festlegung der Punkte in Achswerten. 


Die Routinen im einzelnen:

BOOL teach_in (void);
          Teach-In-Komponente zur Definition von Start- und Zielposition. Die 
          Daten werden in "start_joints" und "ziel_joints" hinterlegt. Die Rou-
          tine liefert TRUE, wenn Menuepunkte zur Aenderung von Start oder Ziel 
          aktiviert wurden. 

--------------------------------------------------------------------------------
Basiert auf den LESAK-Units: "simu" und "simuhelp".  ??
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
1. Inkludiert "lsk_mod.h" nicht mehr.                                WJ 25.03.04
2. Renovierung: Streichung des "link"-Includes. Anpassung folgender 
   Aufrufe: "char_dialog" und "lies_...". Streichung der Aufrufe von
   "clr_line". Anpassung der Ausgaben und Dialoge von "teach".       WJ 10.05.16
3. Include von "joints.h" hinzugefuegt.                              WJ 08.01.18
--------------------------------------------------------------------------------
Status: o.k.             Autoren: C.Blume / W.Jakob            Datum: 08.01.2018
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "joints.h"
#include "rob_int.h"
#include "simu.h"

#include GLOBAL_TEXTS
#include SIMU_TEXTS



/* ============================ Lokale Routinen: ============================ */
static BOOL modif_joints (JOINTS  akt_joints);



/* ============================== modif_joints ============================== */
static BOOL modif_joints (JOINTS  akt_joints)
{
   STR_TYPE  qbuf;
   JOINTS    tmp_j;
   INT       ii;
   BOOL      aend = TRUE;

   /* modif_joints */
   tmp_j = new_joints ();
   for (ii = 0;  (ii < axisanz) && aend;  ii++)
   { /* Einen Achswert updaten */
      sprintf (qbuf, ACHS_QUERY_TXT, ii+1, akt_joints[ii]*RADGRAD);
      aend = !lies_rwert (&(tmp_j[ii]), qbuf, anschlag_unten[ii]*RADGRAD,
			 anschlag_oben[ii]*RADGRAD, akt_joints[ii]*RADGRAD);
      tmp_j[ii] = tmp_j[ii] * GRADRAD;
   } /* Einen Achswert updaten */
   if (aend)
      for (ii = 0;  ii < axisanz;  ii++)
	 akt_joints[ii] = tmp_j[ii];
   return (aend);
} /* modif_joints */



/* ================================ teach_in ================================ */
BOOL teach_in (void)
{
   INT   ii;
   char  ch;
   BOOL  pos_aend = FALSE;

   /* teach_in */
   write_buf (2, TEACH_IN_HDR_TXT);
   write_buf (3, ALTE_WERTE_TXT);
   zeige_start_ziel (4, FALSE);
   write_text_buf (CLEAR);
   printf ("\n");
   ch = char_dialog (START_QUERY_TXT);
   if ((ch == JA) || (ch == CR))
   { /* Startwerte veraendern */
      if (modif_joints (start_joints))
      { /* Startwerte geaendert */
	 pos_aend = TRUE;
	 write_buf (2, NEUE_WERTE_TXT);
	 zeige_start_ziel (3, FALSE);
	 write_text_buf (CLEAR);
      } /* Startwerte geaendert */
   } /* Startwerte veraendern */

   printf ("\n");
   ch = char_dialog (ZIEL_QUERY_TXT);
   if ((ch == JA) || (ch == CR))
   { /* Zielwerte veraendern */
      if (pos_aend)
      { /* Aktuelle Werte */
	 write_buf (2, AKT_WERTE_TXT);
	 zeige_start_ziel (3, FALSE);
	 write_text_buf (CLEAR);
      } /* Aktuelle Werte */
      printf ("\n");
      if (modif_joints (ziel_joints))
      { /* Zielwerte geaendert */
	 pos_aend = TRUE;
	 write_buf (2, NEUE_WERTE_TXT);
	 zeige_start_ziel (3, FALSE);
	 write_text_buf (CLEAR);
      } /* Zielwerte geaendert */
   } /* Zielwerte veraendern */
   if (pos_aend)
      setze_start_ziel_frame ();
   return (pos_aend);
} /* teach_in */



