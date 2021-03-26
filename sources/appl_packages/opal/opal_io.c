/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: OPAL/V
                        Module: OPAL-Input/Output
 
Package: opal_v                 File: opal_io.c                    
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

The package provides all data structures of the static simulation model which 
are needed for the evaluation of OPAL/V. This is the list of processes for the 
production of the products in batches with the time profiles and the value 
"schichtspitzen_ug", from where on shift peaks are recorded for the evaluation.

Unless otherwise noted, all file I/O routines close the files again.


List of routines:

BOOL read_verf_li_file (char *fspec,
                        BOOL *bad_data);
        Reads the file specified by "fspec" and returns TRUE if everything went 
        well. "bad_data" indicates in case of an error whether the old data have 
        already become inconsistent and then has the value TRUE.

--------------------------------------------------------------------------------
List of changes:
 1. Korrektur einiger Fehlermeldungsaufrufe wg. Umstellung in fbhm.  WJ 13.08.08
 2. Anpassungen an die allg. Ueberarbeitung 2016/17.                 WJ 04.04.17
 3. Removal of the error message for non-existence of the input file.WJ 20.08.20
 4. Deletion of everything that is not needed for the sample 
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
#include "opalInt.h"



/* ======================= Package-global Variables: ======================= */
VERFAHRENS_TYP  verfahren [MAX_VERFAHREN_ANZ];   /* product list             */
DOUBLE          schichtspitzen_ug;               /* recording of shift peaks */
                                                 /* above this value         */
INT             verf_anz;                   /* number of products in VL-file */



/* =========================== Local Variables: ============================ */
FILE    *stammdat_file,        /* master data with process list from VL file */
        *erg_file;             /* result file                                */



/* =========================== Local Constants: ============================ */
#define APPL_EXT       "VL"   /* extension of master data file               */



/* =========================== Local Routines: ============================= */




/* ============================= read_verf_li_file ======================== */
BOOL read_verf_li_file (char *fspec,
                        BOOL *bad_data)
{
   STRING_TYPE   buffer;
   STR_TYPE      buf;
   INT           v_anz, anz, ii, jj;
   BOOL          erg = TRUE;
   char         *ch_tmp;

   /* read_verf_li_file */
   *bad_data = FALSE;
   if (!open_file (&stammdat_file, fspec, "rt"))  /* error msg by open_file() */
      return (FALSE);

   /* ---------------------- Reading the file header: ----------------------- */
   if (fscanf (stammdat_file, "%s", buffer) != 1)          /* read program ID */
   { /* ID not readable */
      fehler (OPAL_IO, 2, NO_INFO, NO_INFO, APPL_EXT, BAD_AFILE_HDR);
      erg = FALSE;
   } /* ID not readable */
   else
      if (strcmp (buffer, PROG_IDENT) != 0)
      { /* wrong MOD-File-ID */
	 fehler (OPAL_IO, 3, NO_INFO, NO_INFO, APPL_EXT, BAD_AFILE_HDR);
	 erg = FALSE;
      } /* wrong MOD-File-ID */
   if (erg)
      if (fscanf (stammdat_file, "%s", buf) != 1)             /* read Appl-ID */
      { /* ID not readable */
	 fehler (OPAL_IO, 4, NO_INFO, NO_INFO, APPL_EXT, BAD_AFILE_HDR);
	 erg = FALSE;
      } /* ID not readable */
      else
	 if (!check_appl_id (buf))
	 { /* wrong MOD-File-ID */
	    fehler (OPAL_IO, 5, NO_INFO, NO_INFO, APPL_EXT, BAD_AFILE_HDR);
	    erg = FALSE;
	 } /* wrong MOD-File-ID */
   ch_tmp = fgets (buffer, MAX_STRING+1, stammdat_file); /* skip rest of line */

   /* ---------------------- Reading the process data: ---------------------- */
   if (erg)
   { /* header of the VL file is ok */
      ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);        /* skip line */
      erg = (fscanf (stammdat_file, "%d %d", &v_anz, &ii) == 2);
      if (erg)
      { /* data read */
	 if (v_anz > MAX_VERFAHREN_ANZ)
	 { /* too many processes */
	    fehler (OPAL_IO, 6, v_anz, NO_INFO, APPL_EXT, APPL_FILE_ERR);
	    erg = FALSE;
	 } /* too many processes */
      } /* data read */
      else
	 fehler (OPAL_IO, 7, NO_INFO, NO_INFO, APPL_EXT, BAD_AFILE_HDR);
      ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);         /* skip rest of line */
      if (erg)
      { /* number of processes ok. From now on data update */
	 verf_anz          = v_anz;
	 schichtspitzen_ug = ii;
	 for (ii = 0;  (ii < v_anz) && erg;  ii++)
	 { /* read the data of one process */
            ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);     /* skip blank line */
	    erg = (fscanf (stammdat_file, "%d %lf %d %d %lf %lf", 
			   &(verfahren[ii].anl_idx), 
			   &(verfahren[ii].soll_menge),
			   &(verfahren[ii].soll_zeit),
			   &(verfahren[ii].quell_verf_idx),
			   &(verfahren[ii].min_kap), 
			   &(verfahren[ii].max_kap)) == 6);
	    ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);   /* skip rest of line */
	    erg &= (fscanf (stammdat_file, "%d", &anz) == 1);
	    ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);   /* skip rest of line */
	    if (anz > MAX_INTERVALL_ANZ)
	    { /* too many time intervals */
	       fehler (OPAL_IO, 8, ii+1, anz, "", TOO_MUCH_INTERVALLS);
	       erg = FALSE;
	    } /* too many time intervals */
	    verfahren[ii].ges_dauer = 0;
	    for (jj = 0;  (jj < anz) && erg;  jj++)
	    { /* read interval lengths */
	       erg = (fscanf (stammdat_file, "%d", 
			      &(verfahren[ii].interv_dauer[jj])) == 1);
	       verfahren[ii].ges_dauer += verfahren[ii].interv_dauer[jj];
	    } /* read interval lengths */
	    ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);   /* skip rest of line */
	    for (jj = 0;  (jj < anz) && erg;  jj++)
	    { /* read employee demand of intervals */
	       erg = (fscanf (stammdat_file, "%lf", 
			      &(verfahren[ii].interv_ma_bedarf[jj])) == 1);
	    } /* read employee demand of intervals */
	    ch_tmp = fgets (buffer, MAX_STRING, stammdat_file);   /* skip rest of line */
	    verfahren[ii].mit_vorprod   = verfahren[ii].quell_verf_idx != -1;
	    verfahren[ii].intervall_anz = anz;
	    if (!erg)
	       fehler (OPAL_IO, 9, ii+1, NO_INFO, "", ERROR_IN_PROCEDURE);
	 } /* /* read the data of one process */
	 *bad_data = !erg;
      } /* number of processes ok. From now on data update */
   } /* header of the VL file is ok */
   fclose (stammdat_file);

   return (erg);
} /* read_verf_li_file */




