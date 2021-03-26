/*******************************************************************************
GLEAM/AE                                                                  V1.1.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: C-Terminal-I/O
                        Module: STRing-GrundSoftWare
 
Package: ctio                   File: str_gsw.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt zwei allgemeine Stringroutinen, die nicht auf den TEXT_MODE
beschraenkt sind.


Die allgemeinen Routinen im einzelnen:

void display_time (char *erg_buf,
                   LONG  t_in_sec,
                   BOOL  adjust)
           Wandelt die durch "t_in_sec" vorgegebene Zeit in Sekunden in das 
           Format
                           "tt hh:mm"
           um und liefert den String im "erg_buf" ab. Wenn "adjust" gesetzt ist,
           wird im Falle einer Anzeige von weniger als 24 Stunden 
                           "hh:mm:ss" 
           abgeliefert.

void adr_to_text (char *res_buf, 
                  INT   guete, 
                  INT   lfd_nr, 
                  BOOL  adjust);
           Aus "guete" und "lfd_nr" wird in "res_buf" ein String mit der Ketten-
           adresse folgenden Formats abgeliefert:
           "guete" >= "keine_guete":   nnnn      mit nnnn fuer "lfd_nr"
           "guete" <> "keine_guete":   gg/nn     mit gg fuer "guete" und
                                                     nn fuer "lfd_nr"
           Wenn "adjust" TRUE ist, werden die Formate voll ausgenutzt und wie 
           folgt mit Blanks aufgefuellt: bei "guete" >= "keine_guete" wird nnnn 
           rechtsbuendig und bei anderen "guete"-Werten wird gg rechtsbuendig 
           und nn linksbuendig geschrieben.
           Wenn jedoch "adjust" FALSE ist, wird so wenig Platz wie möglich ver-
           braucht.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Verwendung von "char *" statt STR_TYPE bei den Funktionsparametern.
 2. Vereinfachung der Routine "display_time".                        WJ 02.08.99
 3. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 4. Entfernung BGI_MODE und des dazugehörigen Codes. Streichung von
    "single_to_text" und "real_to_text". GLEAM_C --> TEXT_MODE       WJ 26.06.12
 5. "maus_delay" gestrichen.                                         WJ 29.06.12
 6. Renovierung: Ersetzung des "link"- durch einen "fbhm"-Include. 
    Anpassung folgender Aufrufe: "dialog", "char_dialog". Entfernung 
    der Zeilennummern bei "erfrage_outfile_spec" und "lies_adr". 
    Ersetzung der "write_line"- durch "printf"-Aufrufe.
    Verschiebung der Routinen "erfrage_fspec", "erfrage_outfile_spec" 
    und "lies_adr" in das Modul "user_gsw".                          WJ 10.05.16
 7. Docu of adr_to_text() corrected.                                 WJ 09.07.18
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 09.07.2016
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "chaindef.h"
#include "ctio.h"
#include "ctioInt.h"

#include CTIO_TEXTS
#include GLOBAL_TEXTS




/* ============================== display_time ============================== */
void display_time (char *erg_buf,
                   LONG  t_in_sec,
                   BOOL  adjust)
{
   char   buf [8];
   LONG   rest_sec;
   INT    tage, std, imin;
   float  rmin;
   BOOL   mit_tage;
   
   /* DISPLAY_TIME/SCR_GSW */
   rest_sec = t_in_sec % 86400L;
   tage     = (INT)(t_in_sec / 86400L);
   std      = (INT)(rest_sec / 3600);
   mit_tage = !adjust || (tage > 0);
   if (mit_tage)
      sprintf (erg_buf, "%2d ", tage);           /* Tage, Stunden und Minuten */
   else
      strcpy (erg_buf, "");                  /* Stunden, Minuten und Sekunden */
   sprintf (buf, "%02d:", std);                                    /* Stunden */
   strcat (erg_buf, buf);
   rest_sec %= 3600;
   if (mit_tage)
   { /* Mit Tage: Minuten evtl. runden */
      rmin = (float)(rest_sec) / 60;
      if (rmin < 59.5)
         imin = i_round (rmin);                              /* gerundete Min */
      else
         imin = 59;
      sprintf (buf, "%02d", imin);
   } /* Mit Tage: Minuten evtl. runden */
   else
   { /* ungerundete Minuten und Sekunden */
      sprintf (buf, "%02d:%02d", (INT)(rest_sec/60), (INT)(rest_sec%60));
   } /* ungerundete Minuten und Sekunden */
   strcat (erg_buf, buf);
} /* display_time */ 



/* =============================== adr_to_text ============================== */
VOID adr_to_text (char *res_buf,
                  INT   guete,
                  INT   lfd_nr,
                  BOOL  adjust)
{
   if (guete >= KEINE_GUETE)
   { /* nnnn */
      if (adjust)
         sprintf (res_buf, "%4d", lfd_nr);
      else
         sprintf (res_buf, "%1d", lfd_nr);
   } /* nnnn */
   else
   { /* gg/nn */
      if (adjust)
         sprintf (res_buf, "%2d/%-2d", guete, lfd_nr);
      else
         sprintf (res_buf, "%1d/%1d", guete, lfd_nr);
   } /* gg/nn */
} /* adr_to_text */ 

