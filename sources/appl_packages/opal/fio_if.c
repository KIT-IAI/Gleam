/*******************************************************************************
GLEAM/AE                                                                  V1.1.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: OPAL/V
                        Module: File-I/O InterFace
 
Package: opal_v                 File: fio_if.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

The module contains the routines for the OPAL/V file I/O according to the interface 
of the APPL package of module "appl_fio". The actual I/O is handled by the "opal_io" 
module.


BOOL appl_init (BOOL  steu_par);
          The initialization routine of this application package is in the module 
          "opal_io".


The functions in detail:
          
BOOL add_appl_load_afiles (char *tmp_afspec[]);
          Reads the files specified in "tmp_afspec" depending on the application 
          and returns TRUE if all of them could be read successfully.
          For each successfully read file, firstly the file specification is 
          entered in "appl_file_descr" and secondly a message is writen to the 
          text buffer (see "fbhm").
          The files are expected in the working directory or in the directory 
          specified by the environment variable contained in GLEAM_ROOT_ENV.

BOOL add_appl_load_1_afile (INT   index,
                            char *fspec);
          Loads the application-specific additional file designated by "index". 
          Any errors that occur are reported and the routine returns FALSE, 
          otherwise TRUE.
          If the file is successfully loaded, the "fsepc" is saved in the 
          "appl_file_descr". Result outputs are stored in the text buffer (see
          "fbhm"). Valid "index" values can be assumed.

BOOL add_appl_save_afiles (INT  *znr,
                           char *tmp_afspec[]);
          Depending on the application, saves the files specified in "tmp_afspec" 
          with the "zu_retten" flag set in "appl_file_descr" and returns TRUE if 
          everything was successfully saved. For each successfully saved file, 
          first the file specification is entered in "appl_file_descr" and second 
          a message is stored in the text buffer ("fbhm") beginning with line "znr".

BOOL add_appl_save_1_afile (INT   index,
                            char *fspec);
          Saves the application-specific additional file designated by "index". 
          Any errors that occur are reported and the routine returns FALSE, 
          otherwise TRUE. 
          If the file is successfully saved, the "fsepc" is stored in the 
          "appl_file_descr". Result outputs are stored in the text buffer (see 
          "fbhm"). Valid "index" values can be assumed.

BOOL add_appl_save_erg (char *fspec);
          Depending on the application, rescues the results of the last 
          interactive simulation into the file specified by "fspec", whereby 
          the file is written as a supplement. Returns TRUE if all went well, 
          FALSE otherwise.
          Currently not implemented!

--------------------------------------------------------------------------------
List of changes:
 1. Renovierung: Streichung des "link"-Includes. Anpassung des Aufrufs 
    von "lies_adr".                                                  WJ 10.05.16
 2. Korrektur der Meldungsausgabe in "add_appl_load_afiles".         WJ 05.04.17
 3. Streichung des Includes von MEN_TEXTS.                           WJ 10.01.18
 4. add_appl_load_afiles(): Avoiding buffer overflow of static 
    strings.                                                         WJ 19.08.20
 5. Deletion of everything that is not needed for the sample 
    implementation of a new application.                             WJ 23.10.20
--------------------------------------------------------------------------------
Status: o.k.                     Author: W.Jakob                Date: 23.10.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "appl.h"
#include "opalInt.h"

#include GLOBAL_TEXTS




/* ========================== add_appl_load_afiles ========================== */
BOOL add_appl_load_afiles (char *tmp_afspec[])
{
   char  bigBuf[MAX_STRING+MAX_BUF]; 
   BOOL  erg = FALSE;

   /* add_appl_load_afiles */
   if (add_appl_code == OPAL_APPL)
   { /* OPAL */
      if (appl_file_descr[0].ladbar)
      { /* application specific file is loadable */
	 if (read_verf_li_file (tmp_afspec[0], &(appl_file_descr[0].bad_data)))
	 { /* process list read */
	    strcpy(appl_file_descr[0].afile_spec, tmp_afspec[0]);
	    sprintf(bigBuf, LOAD_ALLG_TXT, appl_file_descr[0].afile_bez,
		     appl_file_descr[0].afile_spec);
	    write_buf(get_line_anz() + 1, bigBuf); 
	    appl_file_descr[0].zu_retten = FALSE;
	    erg = TRUE;
	 } /* process list read */
      } /* application specific file is loadable */
   } /* OPAL */
   else
      fatal(OPAL_FIO_IF, 1, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   return (erg);
} /* add_appl_load_afiles */



/* ========================= add_appl_load_1_afile ========================== */
BOOL add_appl_load_1_afile (INT   index,
                            char *fspec)
{
   char  bigBuf[2*MAX_STRING]; 
   BOOL  erg = FALSE;

   /* add_appl_load_1_afile */
   if (add_appl_code == OPAL_APPL)
   { /* OPAL */
      if (index == 0)
      {
	 erg = read_verf_li_file(fspec, &(appl_file_descr[index].bad_data));
	 if (erg)
         { /* process list read */
	    strcpy (appl_file_descr[index].afile_spec, fspec);
	    sprintf(bigBuf, LOAD_ALLG_TXT, appl_file_descr[index].afile_bez,
		     appl_file_descr[index].afile_spec);
	    write_buf(1, bigBuf); 
	 } /* process list read */
	 else
	    write_buf(1, LSTAT_BAD_DATA);
      }
      else
	 write_buf(1, LSTAT_BAD_DATA);
   } /* OPAL */
   else
      fehler(OPAL_FIO_IF, 2, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   return (erg);
} /* add_appl_load_1_afile */



/* ========================== add_appl_save_afiles ========================== */
BOOL add_appl_save_afiles (INT  *znr,
                           char *tmp_afspec[])
{
   STRING_TYPE  buf1;
   INT          guete, lfd_nr;
   BOOL         erg = FALSE;

   /* add_appl_save_afiles */
   if (add_appl_code == OPAL_APPL)
   { /* OPAL */
#ifdef nix_impl
      if (appl_file_descr[2].zu_retten)
      { /* File ist zu retten */
	 if (lies_adr (&guete, &lfd_nr, ERG_AK_QUERY))
	 { /* Adr gelesen */
	    if (write_erg_file (tmp_afspec[2], guete, lfd_nr))
	    { /* STR-Datei gerettet */
	       strcpy (appl_file_descr[2].afile_spec, tmp_afspec[2]);
	       sprintf (buf1, SAV_GEN_TXT, appl_file_descr[2].afile_bez, 
			"", tmp_afspec[2]);
	       appl_file_descr[2].zu_retten = FALSE;
	       erg = TRUE;
	    } /* STR-Datei gerettet */
	    else
	    { /* Rettung fehlgeschlagen */
	       sprintf (buf1, SAV_GEN_TXT, appl_file_descr[2].afile_bez, 
			NICHT_TXT, tmp_afspec[2]);
	    } /* Rettung fehlgeschlagen */
	 } /* Adr gelesen */
	 else
	    strcpy (buf1, FKT_TERM_TXT);
	 write_buf ((*znr)++, buf1); 
      } /* File ist zu retten */
      else
	 erg = TRUE;
#else
      erg = TRUE;
#endif /* nix_impl */
   } /* OPAL */
   else
      fehler (OPAL_FIO_IF, 3, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   return (erg);
} /* add_appl_save_afiles */



/* ========================= add_appl_save_1_afile ========================== */
BOOL add_appl_save_1_afile (INT   index,
                            char *fspec)
{
   STRING_TYPE  buf1;
   INT          guete, lfd_nr;
   BOOL         erg = FALSE;

   /* add_appl_save_1_afile */
   if (add_appl_code == OPAL_APPL)
   { /* OPAL */
 #ifdef nix_impl
     if (lies_adr (&guete, &lfd_nr, ERG_AK_QUERY))
      { /* Adr gelesen */
	 if (write_erg_file (fspec, guete, lfd_nr))
	 { /* STR-Datei gerettet */
	    strcpy (appl_file_descr[index].afile_spec, fspec);
	    sprintf (buf1, SAV_GEN_TXT, appl_file_descr[index].afile_bez, 
		     "", fspec);
	    appl_file_descr[index].zu_retten = FALSE;
	    erg = TRUE;
	 } /* STR-Datei gerettet */
	 else
	    sprintf (buf1, SAV_GEN_TXT, appl_file_descr[index].afile_bez, 
		     NICHT_TXT, fspec);
      } /* Adr gelesen */
      else
	 strcpy (buf1, FKT_TERM_TXT);
      write_buf (1, buf1); 
#endif /* nix_impl */
      write_buf (1, "Function not implemented!"); 
   } /* OPAL */
   else
      fehler(OPAL_FIO_IF, 4, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   return (erg);
} /* add_appl_save_1_afile */



/* =========================== add_appl_save_erg ============================ */
BOOL add_appl_save_erg (FILE *fdescr)
{
   BOOL erg = FALSE;

   /* add_appl_save_erg */
   if (add_appl_code == OPAL_APPL)
      meldung(OPAL_FIO_IF, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
   else
      fehler (OPAL_FIO_IF, 5, add_appl_code, NO_INFO, "", BAD_APPL_CODE);
   return (erg);
} /* add_appl_save_erg */
