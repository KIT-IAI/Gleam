/*******************************************************************************
GLEAM/AE                                                                  V1.0.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: APPLication
                        Module: APPLication dependent File-I/O
 
Package: appl                   File: appl_fio.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

This module contains application-specific functions for the I/O of additional 
files, provided that a corresponding additional application is linked to GLEAM.

The present implementation of the module acts as a kind of "blind plug" and 
must always be included if no additional application is to be included OR 
as a distributor for the applications to be included (see also module "appl_if").


The routines in detail:

BOOL add_appl_load_afiles (char *tmp_afspec[]);
          Reads in the files specified in "tmp_afspec" depending on the applica-
          tion and returns TRUE if all files could be read in successfully. For 
          each successfully read file, first the file specification is entered 
          in "appl_file_descr" and second a message is stored in the text buffer 
          of package "fbhm" starting with line 5.

BOOL add_appl_load_1_afile (INT   index,
                            char *fspec);
          Loads the application-specific additional file designated by "index". 
          Any errors occurring during this process are reported and the routine 
          returns FALSE, otherwise TRUE.
          If the file is loaded successfully, the "fsepc" is saved in the 
          "appl_file_descr". Result outputs can be stored in the text buffer of 
          package "fbhm". Valid "index" values can be assumed.

BOOL add_appl_save_afiles (INT  *znr,
                           char *tmp_afspec[]);
          Depending on the application, saves the files specified in "tmp_afspec" 
          with the "to_save" flag set in "appl_file_descr" and returns TRUE if 
          everything was successfully saved. For each successfully rescued file, 
          first the file specification is entered in "appl_file_descr" and second 
          a message is stored in the text buffer of package "fbhm" beginning with 
          line "znr".

BOOL add_appl_save_1_afile (INT   index,
                            char *fspec);
          Saves the application-specific additional file designated by "index". 
          Any errors occurring during this process are reported and the routine 
          returns FALSE, otherwise TRUE. 
          If the file has been successfully saved, the "fsepc" is stored in the 
          "appl_file_descr". Result outputs can be stored in the text buffer. 
          Valid "index" values can be assumed.

BOOL add_appl_save_erg (FILE *fdescr);
          Saves the results of the last interactive simulation into the file 
          specified by "fdescr", depending on the application. The file is 
          already open and will not be closed. Returns TRUE if everything went 
          well, otherwise FALSE.

--------------------------------------------------------------------------------
List of changes:
 1. Renovation: deletion of the "link" includes.                     WJ 18.04.16
 2. Correction of the message output in "add_appl_load_afiles".      WJ 05.04.17
 3. Include of GLOBAL_TEXTS instead of MEN_TXT.                      WJ 09.01.18
 4. Removal of possible buffer overflows indicated by the new gcc.   WJ 20.08.18
 5. Avoiding buffer overflow of static strings.                      WJ 20.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Author: W.Jakob                Date: 20.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "hmod.h"
#include "appl.h"
#include "applInt.h"

#include GLOBAL_TEXTS



/* ======================== Package-global Variables: ======================= */



/* ============================= Local Constants: =========================== */



/* ======================== Local Type Declarations: ======================== */



/* ============================= Local Variables: =========================== */



/* ============================= Local Routines: ============================ */




/* ========================== add_appl_load_afiles ========================== */
BOOL add_appl_load_afiles (char *tmp_afspec[])
{
   char  buffer[MAX_STRING+MAX_BUF];
   BOOL  erg = TRUE;

   /* add_appl_load_afiles */
   switch (add_appl_code)
   {
      case TSP_APPL:
      if (TRUE /*lade (tmp_afspec[0])*/)
      { /* -Datei gelesen */
         strcpy (appl_file_descr[0].afile_spec, tmp_afspec[0]);
         sprintf (buffer, LOAD_ALLG_TXT, appl_file_descr[0].afile_bez,
                  appl_file_descr[0].afile_spec);
         write_buf (get_line_anz() + 1, buffer); 
         appl_file_descr[0].bad_data  = FALSE;
         appl_file_descr[0].zu_retten = FALSE;
      } /* -Datei gelesen */
      else
      {
         appl_file_descr[0].bad_data = TRUE;
         erg = FALSE;
      }
      meldung (APPL_FIO, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      break; /* TSP_APPL */

      default:
      fehler (APPL_FIO, 1, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
      erg = FALSE;
   } /* switch */
   return (erg);
} /* add_appl_load_afiles */



/* ========================= add_appl_load_1_afile ========================== */
BOOL add_appl_load_1_afile (INT   index,
                            char *fspec)
{
   char  buffer[MAX_STRING+MAX_BUF];
   BOOL  erg = TRUE;

   /* add_appl_load_1_afile */
   switch (add_appl_code)
   {
      case TSP_APPL:
      if (index == 0)
      { /* Index ok */
         erg = TRUE; /*lade (fspec)*/
         if (erg)
         { /* File geladen */
            strcpy (appl_file_descr[index].afile_spec, fspec);
            appl_file_descr[index].bad_data = FALSE;
            sprintf (buffer, LOAD_ALLG_TXT, appl_file_descr[index].afile_bez,
                     appl_file_descr[index].afile_spec);
            write_buf (1, buffer); 
         } /* File geladen */
         else
            appl_file_descr[index].bad_data = TRUE;
      } /* Index ok */
      else
         write_buf (1, LOAD_NO_APPL);
      meldung (APPL_FIO, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      break; /* TSP_APPL */

      default:
      fehler (APPL_FIO, 2, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
      erg = FALSE;
   } /* switch */
   return (erg);
} /* add_appl_load_1_afile */



/* ========================== add_appl_save_afiles ========================== */
BOOL add_appl_save_afiles (INT  *znr,
                           char *tmp_afspec[])
{
   BOOL  erg = TRUE;

   /* add_appl_save_afiles */
   switch (add_appl_code)
   {
      case TSP_APPL:
      fehler (APPL_FIO, 103, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      erg = FALSE;
      break; /* TSP_APPL */

      default:
      fehler (APPL_FIO, 2, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
      erg = FALSE;
   } /* switch */
   return (erg);
} /* add_appl_save_afiles */



/* ========================= add_appl_save_1_afile ========================== */
BOOL add_appl_save_1_afile (INT   index,
                            char *fspec)
{
   BOOL  erg = TRUE;

   /* add_appl_save_1_afile */
   switch (add_appl_code)
   {
      case TSP_APPL:
      fehler (APPL_FIO, 104, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      erg = FALSE;
      break; /* TSP_APPL */

      default:
      fehler (APPL_FIO, 2, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
      erg = FALSE;
   } /* switch */
   return (erg);
} /* add_appl_save_1_afile */



/* =========================== add_appl_save_erg ============================ */
BOOL add_appl_save_erg (FILE *fdescr)
{
   BOOL erg = TRUE;

   /* add_appl_save_erg */
   switch (add_appl_code)
   {
      case TSP_APPL:
      meldung (APPL_FIO, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      erg = fprintf (fdescr, "Test TSP-Results Test\n") > 0;
      break;

      default:
      fehler (APPL_FIO, 3, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
      erg = FALSE;
   } /* switch */
   return (erg);
} /* add_appl_save_erg */
