/*******************************************************************************
GLEAM/AE                                                                  V1.3.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                                 Package: MENues
                                  Module: File_LOADing
 
Package: men                   File: f_load.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

The module contains the general package initialization and the load functions 
for the initialization files and the experiment file (EXP-files).

The EXP-file consists of a header, the section for the specifications of standard 
files and an optional section for application dependent additional initialisation 
files. Lines starting with a "#"-sign are treated as comments. Everything in a 
line right from a "#"-sign is also treated as comment. 

The header must contain the program identification (PROG_IDENT). A minimal header 
just contains that ID. The header of a saved EXP-file has the following form:
  # =========================== Experiment File List ===========================
  GLEAM/AE
  # written on <date and time>
  # -------------------------- List of Standard Files: -------------------------

The header is followed by the section for standard files consisting of lines with 
name-value-pairs seperated by a "="-sign. For this section four file specifications 
must be and two additional may be given. Required are:
  "Gene model"           MOD-file containing the gene or action model of chromosomes
  "Evaluation"           BEW-file describing the criteria and their parameters
  "Program parameters"   TSK-file containing program parameters
  "Evolution parameters" EVO-file describing genetic operator groups and probabilites
Optional are 
  "Chromosome memory"    MEM-file containing chromosomes
  "Population topology"  TOP-file containing a description of topology and neighbourhood

Here is an example a minimal EXP file:
  # =========================== Experiment File List ===========================
  GLEAM/AE
  # written on Wed, 07.12.2016 12:11:40
  # -------------------------- List of Standard Files: -------------------------
  Gene model           = /projects/gleam/InitFiles/mbf_fox.mod
  Evaluation           = /projects/gleam/InitFiles/mbf_fox.bew
  Evolution parameters = /projects/gleam/InitFiles/mbf_wpar.evo
  Program parameters   = mbf_new.tsk
Note that the order of the files is not prescribed. 

The files are first searched based on the working directory and the given path and file 
name. If a file does not exist the contents of the environment variable GLEAM_ROOT is used 
as a prefix for the path and file name found in the EXP file. The name of the environment 
variable is taken from the GLEAM_ROOT_ENV string constant, which is contained in the 
schalter.h file.

After that section the optional section containing specifications of application specific 
files starts with a special comment line beginning with "#!". The specifications of this 
section are simplier: For every file contained in the MOD-file one line consisting of the 
file specification only must be given in the same order. Empty or comment lines, blanks 
or comments following a file specification are allowed. The number of additional files is
limited to AFILE_MAX_ANZ.

A further optional file specification was added for the MPI-based parallel version of 
GLEAM. A TOP-file describing the topology of neighbourhood relations between the indivi-
duals of a population may be given by the parameter "Population topology". 
For The MPI-based versions the following applies:
  TOP-file given:    It is used for the initialization by calling "loadTopFile".
  no TOP-file given: Command line version: fatal error
                     Interactive version:  message and default (symRing.top) is used
The sequential versions ignore a possible topolgy file and issue an appropriate message.


Die Routinen im einzelnen:

BOOL   gen_init (BOOL  chio_logging,
                 BOOL  gen_err_logging,
                 char *log_fname);
          Die Routine fuehrt die allgemeine Modul-und Package-Initialisierungen 
          durch und liefert TRUE ab, wenn alles gut ging.
          "chio_logging" spezifiziert, ob Ketten-I/O im Logfile dokumentiert 
          werden soll oder nicht. "gen_err_logging" gibt an, ob grundsaetzlich 
          alle (Fehler-)Meldungen ins Logfile gschrieben werden oder nur ab 
          Fehlerstatus FATAL. Durch "log_fname" (!= "") kann ein anderes Log-
          file als das durch LOGFILE_NAME angegebene geoeffnet werden.

BOOL   load_exp (BOOL *mit_ch_file,
                 char *exp_fspec,
                 BOOL  init_mode);
          Die Routine laedt die durch "exp_fspec" angegebene EXP-Datei und mit 
          Hilfe von "load_ini_files" die darin spezifizierten Dateien. Sie liefert 
          TRUE ab, wenn alles gut ging. In "mit_ch_file" wird zurueckgegeben, ob 
          ein Kettenfile eingelesen werden soll.
          "init_mode" gibt an, ob die Routine beim Hochfahren aufgerufen wird.
          In diesem Fall muss das EXP-File existieren und es wird die Simulator-
          Initialisierung "simuInit" aufgerufen.
          Nach dem Laden der Standardfiles wird geprueft, ob mehr Bewertungs-
          kriterien als Simulationsergebnisse angegeben wurden. Letzteres ist in 
          der CLV-Version ein fataler und sonst nur ein normaler Fehler.
          Mit jedem erfolgreich gelesenen Standard-File wird der entsprechende
          Filename gesetzt.
          Erst wenn alles gut ging, werden eventuelle Zusatzfiles gelesen (sofern 
          deren Anzahl im MOD und EXP-File uebereinstimmt. Fuer jedes erfolgreich 
          gelesene Zusatzfile wird dessen Filespezifikation uebernommen. Schliess-
          lich wird "atProgStart" auf FALSE gesetzt.

BOOL   load_mod_file (char *mod_fspec,
                      BOOL  init_mode);
          Liest das durch "mod_fspec" bezeichnete MOD-File ein und liefert TRUE,
          wenn alles gut ging. 
          Bei fehlerfreiem Einlesen wird der Filebezeichner kopiert und es wird
          geprueft, ob die Anzahl der im EXP-File angegebenen und im MOD-File 
          verlangten zusaetzl. ApplFiles uebereinstimmen. Wenn nicht, erfolgt
          eine Fehlermeldung und die Loeschung der "afile_spec"-Eintraege in 
          allen "appl_file_descr". 

BOOL   load_bew_file (char *bew_fspec,
                      BOOL  init_mode);
          Liest das durch "bew_fspec" bezeichnete BEW-File ein und liefert TRUE,
          wenn alles gut ging. 
          Bei fehlerfreiem Einlesen wird der Filebezeichner kopiert. 
  
BOOL   load_tsk_file (char *tsk_fspec,
                      BOOL  init_mode);
          Liest das durch "tsk_fspec" bezeichnete TSK-File ein und initiali-
          siert bei gesetztem "init_mode" und erfolgreichem Einlesen das Simu-
          lator-Package. Die Routine liefert TRUE, wenn alles gut ging. 
          Bei fehlerfreiem Einlesen wird der Filebezeichner kopiert. 

BOOL   load_evo_file (char *evo_fspec,
                      BOOL  init_mode);
          Liest das durch "evo_fspec" bezeichnete EVO-File ein und liefert TRUE,
          wenn alles gut ging. 
          Bei fehlerfreiem Einlesen wird der Filebezeichner kopiert. 

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. "hole_obs" statt "load_obs".                                     CB 30.11.98
 2. Vergroesserung der Stringlaenge der Ladestatus-Anzeige in 
    "load_ini_files".                                                WJ 05.12.98
 3. hole_obs mit Parameter interaktiv (nicht vom Compiler erkannt)   CB 19.04.99
 4. Includierung von "rob_int.h" entfernt.                           WJ 21.06.99
 5. Linux-Portierung: Differenzierung zwischen Solaris und Linux.    WJ 11.05.05
 6. Erweiterung fuer die MatLab-Koppung: Aus GAMA wird MatLab.       WJ 22.08.07
 7. Erweiterung in "gen_init" fuer die parallele Simulation.         WJ 24.06.08
 8. Entfernung der Erweiterung von 7.                                WJ 22.06.12
 9. Entfernung des Codes zu "maus_delay". Die Zeile wird ignoriert.  
    Ersetzung von GLEAM_C durch !GUI_MODE.                           WJ 29.06.12
10. Erweiterungen zum Einlesen der TOP-Fileangabe im EXP-File. Be-
    troffen sind vor allem "load_exp" und "load_ini_files".          WJ 14.09.12
11. "gen_init" wertet Returnwert von "chain_io_init" aus.            WJ 05.10.12
    Package "pcom" geht in "mPar" auf.                               WJ 09.11.12
12. In "load_bew_file" wird "bew_aend_sim" beim initialen Klesen auf 
    FALSE gesetzt.                                                   WJ 03.12.14
13. Renovierung: Korrektur der fgets-Aufrufe. "load_exp" erwartet
    jetzt bei Verwendung beim Programmaufruf die Existenz der EXP-
    Datei. Streichung des "link"-Includes und des "linkInit"-Auf-
    rufs. Anpassunge der Aufrufe von "ladekinematik" und "hole_obs". WJ 12.05.16
14. "load_ini_files" schreibt ab der ersten freien Zeile in den 
    Textpuffer. Nicht vorhandenes EXP-File wird als normaler 
    anstelle eines fatalen Fehlers gewertet. "tmp_bew_file_spec" und 
    "tmp_tsk_file_spec" werden im GUI_MODE Package-Export.           WJ 15.07.16
15. Ueberarbeitung von "load_exp" entsprechend obiger Doku. Im 
    Wesentlichen wird nun die Fileangabe als erster String aus der 
    jeweiligen Zeile isoliert. Meldung beim Versuch generations-
    weiser Datensammlung, wenn diese generell deaktiviert ist. In
    "gen_init" werden alle "bad_xxx_data"-Variable auf TRUE gesetzt. WJ 25.10.16
16. Integration der neuen TSK-Datenverwaltung. Vereinfachung des 
    EXP-Files. Der Anwendungsdateienteil kann Leerzeilen, Kommentare 
    und Blanks enthalten. Anpassung des "write_to_log"-Aufrufs.      WJ 19.12.16
17. Korrektur der Herkunftsnummern der Fehlermeldungen. Umbenennung 
    von "go_gsw_init" nach "loadEvoParamFile"                        WJ 30.06.17
18. Abschalten von "atProgStart" in load_exp().                      WJ 06.10.17
19. Include von GLOBAL_TEXTS.                                        WJ 10.01.18
20. "gen_init" calls initVisuIO() if VISU_MODE and initParPop() if 
    PAR_POP_VERS is set.                                             WJ 15.06.18
21. Pruefung in load_exp(), ob mehr Bewertungskriterien als 
    Simulationsergebnisse angegeben wurden.                          WJ 14.02.19
22. gen_init() schreibt bei gesetztem WITH_DYN_GENES einen ent-
    sprechenden Hinweis (WITH_DYN_GENES_TXT) in den Logfilekopf.     WJ 10.07.19
23. Avoiding buffer overflow of static strings: gen_init(), 
    load_exp().                                                      WJ 20.07.20
24. Files specified in the EXP file are searched for first in the 
    present working directory and if they do not exist there also 
    in the directory given by the environment variable, the name of
    which is contained in GLEAM_ROOT_ENV string constant. This applies 
    to the standard files as well as to any application-specific 
    additional files.                                                WJ 20.08.20
--------------------------------------------------------------------------------
Status: o.k.                    Autor: W.Jakob                 Datum: 20.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "chio.h"
#include "bew.h"
#include "appl.h"
#include "hmod.h"
#include "aufg.h"
#include "simu.h"
#include "evo.h"
#ifdef MPI_PAR_VERS
  #include "mPar.h"
#endif /* MPI_PAR_VERS */
#ifdef PAR_POP_VERS
  #include "parPop.h"
#endif /* PAR_POP_VERS */
#if defined(VISU_MODE)
  #include "visuIO.h"
#endif /* VISU_MODE */
#include "men.h"
#include "menInt.h"

#include GLOBAL_TEXTS
#include MAIN_TEXTS
#include MEN_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ========================== Exportierte Variable: ========================= */
char  *tmp_bew_file_spec,  /* Temp. File-Bez. f. Bewertungsfile } nur im GUI- */
      *tmp_tsk_file_spec;  /* Temp. File-Bez. f. Parameterfile  } MODE Export */
INT    exp_afile_anz;      /* Anzahl im EXP-File angegebener Appl.-Files      */



/* ======================== Package-globale Variable: ======================= */
char  *tmp_afile_spec [AFILE_MAX_ANZ], /* Temp. File-Bez. fuer ApplFiles      */
      *memFileSpecSave;    /* stores last successfully read MEM-FileSpec      */
BOOL   last_exp_ok;        /* TRUE, wenn zuletzt geladenes Exp. fehlerfrei    */
BOOL   bew_aend = FALSE;   /* Zeigt manuelle Aenderungen nach BEW-File-I/O an */
BOOL   tsk_aend = FALSE;   /* Zeigt manuelle Aenderungen nach TSK-File-I/O an */
BOOL   bad_mod_data,       /* TRUE, wenn die Modell-Daten inkonsist. wurden   */
       bad_bew_data,       /* TRUE, wenn die Bewertungs-Daten inkonsist.wurden*/
       bad_tsk_data,       /* TRUE, wenn die AufgabenDaten inkonsist. wurden  */
       bad_evo_data,       /* TRUE, wenn die EvoParamDaten inkonsist. wurden  */
       bad_top_data;       /* TRUE, wenn die TopologieDaten inkonsist. wurden */


/* ============================ Lokale Variable: ============================ */



/* ============================ Lokale Routinen: ============================ */
static BOOL load_ini_files (BOOL  init_mode);
   



/* ================================ gen_init ================================ */
BOOL gen_init (BOOL  chio_logging,
               BOOL  gen_err_logging,
               char *log_fname)
{
   STRING_TYPE  buffer;
   BUF_TYPE     buf1, buf2;
   GSTATE       ret_state;
   INT          ch_size, ii;
   BOOL         erg = TRUE,
                mit_hdr;

   /* gen_init */
   ch_size = sizeof (char);
   for (ii = 0;  (ii < AFILE_MAX_ANZ) && erg;  ii++)
      erg = (tmp_afile_spec[ii] = (char*)malloc(MAX_STR * ch_size)) != NULL;
   if (erg)
      erg = ((tmp_bew_file_spec = (char*)malloc(MAX_STRING * ch_size)) != NULL) &&
	    ((tmp_tsk_file_spec = (char*)malloc(MAX_STRING * ch_size)) != NULL) &&
	    ((memFileSpecSave   = (char*)malloc(MAX_STRING * ch_size)) != NULL) &&
            sysInit ();
   if (erg)
   { /* initialize strings */
      strcpy (logfilename, log_fname);
      strcpy (tmp_bew_file_spec, "");
      strcpy (tmp_bew_file_spec, "");
      strcpy (memFileSpecSave,   "");
      for (ii = 0;  ii < AFILE_MAX_ANZ;  ii++)
	 strcpy (tmp_afile_spec[ii], "");
   } /* initialize strings */
   erg &= init_file_gsw ();
   if (erg)
   { /* "sys", logfile, and memory ok */
#if defined(BS_SOLARIS) || defined(BS_LINUX)
      start_child_handler ();
#endif /* Solaris oder Linux */
      sprintf (prog_id_buf, PROG_ID_STR, PROGRAM_NAME, VERSION, STAND);
      ii = strlen (log_fname);
      if (ii > 0) 
         mit_hdr = log_fname[ii-1] != '0';
      else
         mit_hdr = TRUE;
      if (mit_hdr)
      { /* Logfile-Kopfzeile(n) schreiben */
         get_time_stamp (buffer, buf1, buffer, buf2);  /* "buffer" ist hier Dummy-Param */
         sprintf (buffer, LOG_HDR, prog_id_buf, buf1, buf2);
         write_to_log (buffer, CLOSE_LOG);
#ifdef WITH_DYN_GENES
	 sprintf(buffer, "%s %s", DYN_GENES_1_TXT, DYN_GENES_2_TXT);
	 write_to_log(buffer, KEEP_LOG_OPEN);
         write_to_log("", CLOSE_LOG);
#endif /* WITH_DYN_GENES */
      } /* Logfile-Kopfzeile(n) schreiben */
#ifdef TEXT_MODE
      ctioInit ();
      startMeldung (prog_id_buf);
#endif /* TEXT_MODE */
      if (fbhmInit(gen_err_logging))
      { /* message system ready */
	 erg  = (lgswInit() == GLEAM_OK);
	 erg &= chain_io_init (chio_logging);
	 erg &= bew_init ();
	 erg &= hmodInit ();
	 erg &= tskDataInit ();
	 erg &= simu_gsw_init ();
	 erg &= evoInit ();
	 erg &= appl_init (FALSE);
	 bad_mod_data = TRUE;
	 bad_bew_data = TRUE;
	 bad_tsk_data = TRUE;
	 bad_evo_data = TRUE;
	 bad_top_data = TRUE;
#ifdef MPI_PAR_VERS
	 erg &= initParData();
#endif
#ifdef PAR_POP_VERS
	 erg &= initParPop();
#endif
#ifdef VISU_MODE
	 erg &= initVisuIO();
#endif /* VISU_MODE */
	 if (erg)
         { /* ok */
	    erg = registerTskFileParameter();
	    erg &= registerParam (MOD_FILE_SPEC_TXT, MOD_FILE_SPEC_IDX, STRING_DATA, 0, 0, 
				  NOT_EDITABLE, WITHOUT_DEFAULT, 0, 0.0, NULL);
	    erg &= registerParam (BEW_FILE_SPEC_TXT, BEW_FILE_SPEC_IDX, STRING_DATA, 0, 0, 
				  NOT_EDITABLE, WITHOUT_DEFAULT, 0, 0.0, NULL);
	    erg &= registerParam (TSK_FILE_SPEC_TXT, TSK_FILE_SPEC_IDX, STRING_DATA, 0, 0, 
				  NOT_EDITABLE, WITHOUT_DEFAULT, 0, 0.0, NULL);
	    erg &= registerParam (EVO_FILE_SPEC_TXT, EVO_FILE_SPEC_IDX, STRING_DATA, 0, 0, 
				  NOT_EDITABLE, WITHOUT_DEFAULT, 0, 0.0, NULL);
	    erg &= registerParam (MEM_FILE_SPEC_TXT, MEM_FILE_SPEC_IDX, STRING_DATA, 0, 0, 
				  NOT_EDITABLE, WITHOUT_DEFAULT, 0, 0.0, NULL);
	    erg &= registerParam (TOP_FILE_SPEC_TXT, TOP_FILE_SPEC_IDX, STRING_DATA, 0, 0, 
				  NOT_EDITABLE, WITHOUT_DEFAULT, 0, 0.0, NULL);
	 } /* ok */
      } /* message system ready */
      else
	 erg = FALSE;
   } /* "sys", logfile, and memory ok */
   return (erg);
} /* gen_init */



/* ===================================== load_exp ===================================== */
BOOL load_exp (BOOL *mit_ch_file,
               char *exp_fspec,
               BOOL  init_mode)
{
   STRING_TYPE  buffer, buf2;
   FILE        *exp_file;
   INT          ioRes,
                ii;
   BOOL         ok,                   /* Funktionsergebnis: TRUE, wenn alle Daten ok    */
                terminate,            /* TRUE, if end of parameter section or error     */
                eofFound,             /* TRUE, if EOF is encountered                    */
                errorFound;           /* TRUE on error                                  */
   char        *ch_tmp;

   /* load_exp */
   exp_afile_anz    = 0;
   ok               = FALSE;
   if (file_exists (exp_fspec))
   { /* EXP-file exists */
      if ((exp_file = fopen (exp_fspec, "r")) == NULL)
	 if (init_mode)
	    fehler (MEN_FLOAD, 1, NO_INFO, NO_INFO, buffer, FILE_NOT_EXISTS);
	 else
	    meldung (MEN_FLOAD, NO_INFO, NO_INFO, buffer, FILE_NOT_EXISTS);
      else
      { /* EXP-file open */
         /* --------------- read header, check it and read file names : --------------- */
	 eofFound = FALSE;
	 if (readHdr(exp_file, CHECK_PROG_ID_ONLY, "EXP", buffer))
         { /* processing of file contents */
	    terminate = FALSE;
	    setStringVal("", MOD_FILE_SPEC_IDX);                     /* reset all file  */
	    setStringVal("", BEW_FILE_SPEC_IDX);;                    /* specifications, */
	    setStringVal("", TSK_FILE_SPEC_IDX);;                    /* which           */
	    setStringVal("", EVO_FILE_SPEC_IDX);;                    /* will be or      */
	    setStringVal("", MEM_FILE_SPEC_IDX);;                    /* may be          */
	    setStringVal("", TOP_FILE_SPEC_IDX);;                    /* read next       */
	    do { /* process one line */
	       ioRes = fscanf(exp_file, "%[^\n]s", buffer);
	       if (ioRes > 0)
		  terminate = !processInputLine(&errorFound, buffer);   /* process line */
	       else
		  eofFound = (ioRes < 0);
	       ch_tmp = fgets (buffer, MAX_STRING, exp_file);      /* skip rest of line */
	    } /* process one line */
	    while (!terminate && !errorFound && !eofFound);
	    ok = !errorFound;
	    if (ok && !eofFound)
            { /* no error and still something to read */
	       ii = 0;
	       terminate = FALSE;
	       /* ---------- read additional application file sepcifications: --------- */
	       do
               { /* read remaining lines */
		  if (fscanf(exp_file, "%[^\n]s", buffer) == 1)
		  { /* line with some contents read */
		     if (getFirstString(tmp_afile_spec[ii], buffer))
			ii++;   /* increment only, if string different from blanks read */
		  } /* line with some contents read */
		  else
		     terminate = feof(exp_file);
		  if (!terminate)
		     ch_tmp = fgets(buffer, MAX_STRING, exp_file);  /* skip rest of line*/
	       } /* read remaining lines */
	       while (!terminate && (ii < AFILE_MAX_ANZ));
	       exp_afile_anz = ii;
	    } /* no error and still something to read */
	    if (!ok)
	       fehler (MEN_FLOAD, 2, NO_INFO, NO_INFO, "EXP", BAD_INIT_FILE);
	 } /* processing of file contents */
	 fclose (exp_file);
      } /* EXP-file open */
   } /* EXP-file exists */
   else
   { /* EXP-File existiert nicht */
      if (init_mode)
	 fehler (MEN_FLOAD, 3, NO_INFO, NO_INFO, exp_fspec, FILE_NOT_EXISTS);
      else
	 meldung (MEN_FLOAD, NO_INFO, NO_INFO, exp_fspec, FILE_NOT_EXISTS);
   } /* EXP-File existiert nicht */

   /* ----------------- processing of successfully read file contents: ---------------- */
   *mit_ch_file = FALSE;
   if (ok)
   { /* file ok */
      if (getStringVal(buffer, MEM_FILE_SPEC_IDX, GLEAM_OK))
	 *mit_ch_file = (strlen(buffer) > 0);
      ok = load_ini_files (init_mode);
      if (akt_anz_krit > akt_anz_roh_erg_werte)
      { /* mehr Bewertungskriterien als Simulationsergebnisse */
#ifdef GLEAM_USR
	 err_state |= fehler(MEN_FLOAD, 4, akt_anz_krit, akt_anz_roh_erg_werte, "", 
			     MORE_CRITERIA_THAN_RESULTS);
#else /* CLV */
	 err_state = fatal(MEN_FLOAD, 4, akt_anz_krit, akt_anz_roh_erg_werte, "", 
			   MORE_CRITERIA_THAN_RESULTS);
	 ok = FALSE;
#endif /* CLV */
      } /* mehr Bewertungskriterien als Simulationsergebnisse */
   } /* file ok */
   if (ok)
      strcpy (exp_file_spec, exp_fspec);
   atProgStart = FALSE;   /* program start phase is now finished. Usefull for ext.Simu. */
   return (ok);
} /* load_exp */



/* ============================= load_ini_files ============================= */
static BOOL load_ini_files (BOOL  init_mode)
/*----------------------------------------------------------------------------*/
/*   The routine reads the initialization files and calls the simulator       */
/*   initialization "simuInit" if "init_mode" is set.                         */
/*   TRUE is returned if everything went well.                                */
/*   With each successfully read standard file a message is written into the  */
/*   text buffer. When all standard files have been successfully read, the    */
/*   application files are read, provided that the number of additional       */
/*   ApplFiles specified in the EXP file and those required in the MOD file   */
/*   are the identical.                                                       */
/*   The files are searched for in the current working directory and in the   */
/*   directory specified by the environment variable GLEAM_ROOT. On startup,  */
/*   the status and content of GLEAM_ROOT are shown and documented in the     */
/*   logfile.                                                                 */
/*----------------------------------------------------------------------------*/
{
   char         bigBuf[2*MAX_STRING],
                logBuf[2*MAX_STRING+MAX_BUF],
               *envVar; 
   STRING_TYPE  fileSpec;
   INT          znr, ii;
   BOOL         withTopFSpec, loadOk,
                withEnvVar,                 /* TRUE: The environment variable existists */
                badTopData;                 /* TRUE: Topology data became inconsistent  */

   /* load_ini_files */
   last_exp_ok = TRUE;
   znr         = get_line_anz() + 1;                  /* from the next free line onward */
   envVar      = getenv(GLEAM_ROOT_ENV);              /* get GLEAM_ROOT contents        */
   withEnvVar  = envVar != NULL;
   if (init_mode) 
   { /* at startup */
      if (withEnvVar)
      { /* the environment variable contained in GLEAM_ROOT_ENV exists and at startup */
	 if (check_dir(envVar))
	    sprintf(logBuf, GLEAM_ROOT_OK_TXT, GLEAM_ROOT_ENV, envVar);
	 else
	    sprintf(logBuf, GLEAM_ROOT_ERR_TXT, GLEAM_ROOT_ENV, envVar);
      } /* the environment variable contained in GLEAM_ROOT_ENV exists */
      else
	 sprintf(logBuf, NO_GLEAM_ROOT_TXT, GLEAM_ROOT_ENV);
      write_buf(znr++, logBuf);
   } /* at startup */

   if (getStringVal(fileSpec, MOD_FILE_SPEC_IDX, GLEAM_ERROR))
   { /* MOD file specification obtained */
      strcpy(bigBuf, fileSpec);
      if (!file_exists(fileSpec) && withEnvVar)
	 sprintf(bigBuf, "%s/%s", envVar, fileSpec);     /* check GLEAM_ROOT + fileSpec */
      if (load_mod_file (bigBuf, init_mode))
      { /* MOD-File ok */
	 if (appl_code == LESAK_APPL)
	    sprintf(logBuf, LOAD_ALLG_TXT, LOAD_HMOD_MEN, bigBuf);
	 else
	    sprintf(logBuf, LOAD_ALLG_TXT, LOAD_GMOD_MEN, bigBuf);
	 write_buf(znr++, logBuf);
	 if ((strlen(bigBuf) > strlen(fileSpec)) && (strlen(bigBuf) < MAX_STRING))
	    setStringVal(bigBuf, MOD_FILE_SPEC_IDX);
      } /* MOD-File ok */
      else
	 last_exp_ok = FALSE;
   } /* MOD file specification obtained */
   else
      last_exp_ok = FALSE; 
   if (!last_exp_ok && (init_mode || bad_mod_data))
      return (FALSE);                      /* "bad_mod_data" set by "load_mod_file"     */

   if (getStringVal(fileSpec, BEW_FILE_SPEC_IDX, GLEAM_ERROR))
   { /* BEW file specification obtained */
      strcpy(bigBuf, fileSpec);
      if (!file_exists(fileSpec) && withEnvVar)
	 sprintf(bigBuf, "%s/%s", envVar, fileSpec);     /* check GLEAM_ROOT + fileSpec */
      if (load_bew_file (bigBuf, init_mode))
      { /* BEW-File ok */
	 sprintf(logBuf, LOAD_ALLG_TXT, IO_BEW_MEN, bigBuf);
	 write_buf(znr++, logBuf); 
	 if ((strlen(bigBuf) > strlen(fileSpec)) && (strlen(bigBuf) < MAX_STRING))
	    setStringVal(bigBuf, BEW_FILE_SPEC_IDX);
      } /* BEW-File ok */
      else
	 last_exp_ok = FALSE;
   } /* BEW file specification obtained */
   else
      last_exp_ok = FALSE;
   if (!last_exp_ok && (init_mode || bad_bew_data))
      return (FALSE);                      /* "bad_bew_data" set by "load_bew_file"     */

   if (getStringVal(fileSpec, TSK_FILE_SPEC_IDX, GLEAM_ERROR))
   { /* TSK file specification obtained */
      strcpy(bigBuf, fileSpec);
      if (!file_exists(fileSpec) && withEnvVar)
	 sprintf(bigBuf, "%s/%s", envVar, fileSpec);     /* check GLEAM_ROOT + fileSpec */
      if (load_tsk_file (bigBuf, init_mode))
      { /* TSK-File ok */
	 sprintf(logBuf, LOAD_ALLG_TXT, IO_TSK_MEN, bigBuf);
	 write_buf(znr++, logBuf); 
	 if ((strlen(bigBuf) > strlen(fileSpec)) && (strlen(bigBuf) < MAX_STRING))
	    setStringVal(bigBuf, TSK_FILE_SPEC_IDX);
      } /* TSK-File ok */
      else
	 last_exp_ok = FALSE;
   } /* TSK file specification obtained */
   else
      last_exp_ok = FALSE;
   if (!last_exp_ok && (init_mode || bad_tsk_data))
      return (FALSE);                      /* "bad_tsk_data" set by "load_tsk_file"     */

   if (getStringVal(fileSpec, EVO_FILE_SPEC_IDX, GLEAM_ERROR))
   { /* EVO file specification obtained */
      strcpy(bigBuf, fileSpec);
      if (!file_exists(fileSpec) && withEnvVar)
	 sprintf(bigBuf, "%s/%s", envVar, fileSpec);     /* check GLEAM_ROOT + fileSpec */
      if (load_evo_file(bigBuf, init_mode))
      { /* EVO-File ok */
	 sprintf(logBuf, LOAD_ALLG_TXT, LOAD_EVO_MEN, bigBuf);
	 write_buf(znr++, logBuf); 
	 if ((strlen(bigBuf) > strlen(fileSpec)) && (strlen(bigBuf) < MAX_STRING))
	    setStringVal(bigBuf, EVO_FILE_SPEC_IDX);
      } /* EVO-File ok */
      else
	 last_exp_ok = FALSE;
   } /* EVO file specification obtained */
   else
      last_exp_ok = FALSE;
   if (!last_exp_ok && (init_mode || bad_evo_data))
      return (FALSE);                      /* "bad_evo_data" set by "load_evo_file"     */

   if (getStringVal(fileSpec, TOP_FILE_SPEC_IDX, GLEAM_OK))
      withTopFSpec = (strlen(fileSpec) > 0);
   else
      withTopFSpec = FALSE;
#if defined(MPI_PAR_VERS)
   #ifdef GLEAM_USR
   if (!withTopFSpec) { /* keine TOP-File-Angabe im EXP-File */
      setStringVal(TOP_RING_DEF_FNAME, TOP_FILE_SPEC_IDX);
      strcpy(fileSpec, TOP_RING_DEF_FNAME);
      withTopFSpec = TRUE;
      meldung(MEN_FLOAD, NO_INFO, NO_INFO, SYM_RING_USED, NO_TOP_FILE);
   } /* keine TOP-File-Angabe im EXP-File */
   #endif /* GLEAM_USR */
   if (withTopFSpec)
   { /* Es gibt eine TOP-Fileangabe */
      if (strcasecmp(fileSpec, TOP_RING_DEF_FNAME) == 0)
      { /* Defaultfile angegeben: symmetr. Ring-Nachbarschaft */
         symRing = TRUE;
	 loadOk  = TRUE;
	 setStringVal("", TOP_FILE_SPEC_IDX);     /* d.h., es wurde keine Datei gelesen */
	 sprintf(bigBuf, "  %s: %s", LOAD_TOP_MEN, SYM_RING_TXT);
	 write_buf(znr++, bigBuf); 
      } /* Defaultfile angegeben: symmetr. Ring-Nachbarschaft */
      else
      { /* TOP-Datei lesen */
	 strcpy(bigBuf, fileSpec);
	 if (!file_exists(fileSpec) && withEnvVar)
	    sprintf(bigBuf, "%s/%s", envVar, fileSpec);     /* check GLEAM_ROOT + fileSpec */
	 loadOk = loadTopFile(bigBuf, &badTopData);
	 if (loadOk)
	 { /* TOP-File ok */
	    sprintf(logBuf, LOAD_ALLG_TXT, LOAD_TOP_MEN, bigBuf);
	    write_buf(znr++, logBuf);
	    if ((strlen(bigBuf) > strlen(fileSpec)) && (strlen(bigBuf) < MAX_STRING))
	       setStringVal(bigBuf, TOP_FILE_SPEC_IDX);
	    if (!init_mode)
	       initFileReloaded = TRUE;
	 } /* TOP-File ok */
	 else
	 { /* TOP-File nicht ok */
  #ifdef MPI_PAR_VERS
            symRing = badTopData;                    /* wenn schlechte Daten, dann Ring */
	    if (!init_mode && !badTopData) 
	       meldung(MEN_FLOAD, NO_INFO, NO_INFO, fileSpec, OLD_TOP_STILL_VALID);
  #endif /* MPI_PAR_VERS */
	    last_exp_ok = FALSE;
	    if (init_mode || badTopData) {
	       fehler(MEN_FLOAD, 20, NO_INFO, NO_INFO, "", TOP_DATA_INKONSISTENT);
	       return (FALSE);
	    }
	 } /* TOP-File nicht ok */
      } /* TOP-Datei lesen */
      bad_top_data = badTopData || (!loadOk && bad_top_data);
   } /* Es gibt eine TOP-Fileangabe */
   else
   { /* Keine TOP-Fileangabe */
      last_exp_ok = FALSE;
      fatal(MEN_FLOAD, 21, NO_INFO, NO_INFO, TERMINAT_TXT, NO_TOP_FILE);
      return (FALSE);
   } /* Keine TOP-Fileangabe */
#else /* keine Parallelversion */
   if (withTopFSpec)
      meldung(MEN_FLOAD, NO_INFO, NO_INFO, fileSpec, TOP_FSPEC_IGNORED);
#endif /* keine Parallelversion */

   if ((appl_file_anz == exp_afile_anz) && (appl_file_anz > 0))
   { /* there are additional application files and their number is correct */
      for (ii = 0;  ii < exp_afile_anz;  ii++)
      { /* check for loadable addititional files */
	 if (appl_file_descr[ii].ladbar)
	 { /* additional file is loadable */
	    if (!file_exists(tmp_afile_spec[ii]) && withEnvVar)
	    { /* tmp_afile_spec[ii] does not exist, so check in GLEAM_ROOT directory */
	       sprintf(fileSpec, "%s/%s", envVar, tmp_afile_spec[ii]);
	       strcpy(tmp_afile_spec[ii], fileSpec);
	    } /* tmp_afile_spec[ii] does not exist, so check in GLEAM_ROOT directory */
	 } /* additional file is loadable */
      } /* check for loadable addititional files */
      switch (appl_code)
      {
         case GADO_APPL:
         break; /* GADO_APPL */

         case MATLAB_APPL:
         break; /* MATLAB_APPL */

         case SIMU_SERV_APPL:
         break; /* SIMU_SERV_APPL */

         case LESAK_APPL:
	    if (ladekinematik(tmp_afile_spec[0], znr+1))
            { /* Kinematik-Datei gelesen */
	       strcpy(appl_file_descr[0].afile_spec, tmp_afile_spec[0]);
	       sprintf(logBuf, LOAD_ALLG_TXT, appl_file_descr[0].afile_bez,
		       appl_file_descr[0].afile_spec);
	       znr = get_line_anz();
	       write_buf(znr, logBuf); 
	       appl_file_descr[0].bad_data = FALSE;
	       znr += 2;
	       if (!init_mode)
		  initFileReloaded = TRUE;
	    } /* Kinematik-Datei gelesen */
	    else {
	       appl_file_descr[0].bad_data = TRUE;
	       return (FALSE);
	    }

	    if (hole_obs (tmp_afile_spec[1], znr+1))
	    { /* Hindernis-Datei gelesen */
	       strcpy (appl_file_descr[1].afile_spec, tmp_afile_spec[1]);
	       sprintf (logBuf, LOAD_ALLG_TXT, appl_file_descr[1].afile_bez,
			appl_file_descr[1].afile_spec);
	       write_buf (znr, logBuf); 
	       appl_file_descr[1].bad_data = FALSE;
	       znr += 2;
	       if (!init_mode)
		  initFileReloaded = TRUE;
	    } /* Hindernis-Datei gelesen */
	    else {
	       appl_file_descr[1].bad_data = TRUE;
	       return (FALSE);
	    }
	    strcpy (appl_file_descr[2].afile_spec, tmp_afile_spec[2]);
	    appl_file_descr[2].bad_data = FALSE;
         break; /* LESAK_APPL */

         case ADD_APPL:
	    loadOk = add_appl_load_afiles(tmp_afile_spec);
	    if (!init_mode && loadOk)
	       initFileReloaded = TRUE;
	    return (loadOk);

         default:
            fehler (MEN_FLOAD, 22, appl_code, NO_INFO, "", BAD_APPL_CODE);
	    return (FALSE);
      } /* CASE */
   } /* there are additional application files and their number is correct */
   return (TRUE);
} /* load_ini_files */



/* ============================= load_mod_file ============================== */
BOOL load_mod_file (char *mod_fspec,
                    BOOL  init_mode)
{
   BOOL  status_ok,  /* GSTATE war GLEAM_OK                                   */
         bad_mod;    /* alte Daten wurden bei fehlerhaftem Lesen inkonsistent */
   INT   ii;

   /* load_mod_file */
   status_ok = read_mod_file (mod_fspec, &bad_mod) == GLEAM_OK;

#ifndef SLAVE
   /* Slave muss nicht ueberpruefen, da der Master das bereits erledigt hat  */
   if (status_ok)
   { /* MOD-File erfolgreich geladen */
      if (appl_file_anz != exp_afile_anz)
      { /* Experiment- und MOD-File-ApplFileAnz stimmen nicht ueberein */
	 fehler (MEN_FLOAD, 5, appl_file_anz, exp_afile_anz, "", BAD_AFILE_ANZ);
         for (ii = 0;  ii < AFILE_MAX_ANZ;  ii++)
            strcpy (appl_file_descr[ii].afile_spec, "");
      } /* Experiment- und MOD-File-ApplFileAnz stimmen nicht ueberein */
   } /* MOD-File erfolgreich geladen */
#endif /* kein SLAVE */
   if (!init_mode)
   { /* interactive */
      if (status_ok) {
	 setStringVal(mod_fspec, MOD_FILE_SPEC_IDX);  /* update MOD-file spec */
	 initFileReloaded = TRUE;
      }
      else /* faulty file */
	 if (bad_mod)
	    setStringVal(BAD_DATA_TXT, MOD_FILE_SPEC_IDX);    /* data spoiled */
	 else {
	    if (getStringVal(mod_fspec, MOD_FILE_SPEC_IDX, GLEAM_ERROR))
	       if ((strlen(mod_fspec) > 0) && strcmp(mod_fspec, BAD_DATA_TXT))
		  meldung (MEN_FLOAD, NO_INFO, NO_INFO, mod_fspec, OLD_MOD_STILL_VALID);
	 }
   } /* interactive */
   if ((init_mode && !status_ok) || (bad_mod && !status_ok))
      fehler (MEN_FLOAD, 6, NO_INFO, NO_INFO, "", MOD_DATA_INKONSISTENT);
   bad_mod_data = bad_mod || (!status_ok && bad_mod_data);

   return (status_ok);
} /* load_mod_file */



/* ============================= load_bew_file ============================== */
BOOL load_bew_file (char *bew_fspec,
                    BOOL  init_mode)
{
   BOOL  status_ok,  /* GSTATE war GLEAM_OK                                   */
         bad_data;   /* alte Daten wurden bei fehlerhaftem Lesen inkonsistent */

   /* load_bew_file */
   status_ok = restore_bew (bew_fspec, &bad_data) == GLEAM_OK;
   if (status_ok)
   { /* BEW-File erfolgreich geladen */
      bew_aend     = FALSE;
      bew_aend_sim = !init_mode; 
   } /* BEW-File erfolgreich geladen */
   if (!init_mode)
   { /* interactive */
      if (status_ok) {
	 setStringVal(bew_fspec, BEW_FILE_SPEC_IDX);  /* update BEW-file spec */
	 initFileReloaded = TRUE;
      }
      else /* faulty file */
	 if (bad_data)
	    setStringVal(BAD_DATA_TXT, BEW_FILE_SPEC_IDX);    /* data spoiled */
	 else {
	    if (getStringVal(bew_fspec, BEW_FILE_SPEC_IDX, GLEAM_ERROR))
	       if ((strlen(bew_fspec) > 0) && strcmp(bew_fspec, BAD_DATA_TXT))
		  meldung (MEN_FLOAD, NO_INFO, NO_INFO, bew_fspec, OLD_BEW_STILL_VALID);
	 }
   } /* interactive */
   if ((init_mode && !status_ok) || (bad_data && !status_ok))
      fehler (MEN_FLOAD, 7, NO_INFO, NO_INFO, "", BEW_DATA_INKONSISTENT);
   bad_bew_data = bad_data || (!status_ok && bad_bew_data);

   return (status_ok);
} /* load_bew_file */



/* ============================= load_tsk_file ============================== */
BOOL load_tsk_file (char *tsk_fspec,
                    BOOL  init_mode)
{
   BOOL  ok,         /* File erfolgr. gelesen ODER Fehler u. alte Daten OK    */
         status_ok,  /* GSTATE war GLEAM_OK                                   */
         bad_data,   /* alte Daten wurden bei fehlerhaftem Lesen inkonsistent */
         ext_sim_ok = TRUE;     /* TRUE, wenn ExtSimu ok oder nicht benoetigt */

   /* load_tsk_file */
   status_ok = readTskFile(tsk_fspec, &bad_data) == GLEAM_OK;
   ok        = status_ok || (!init_mode && !bad_data);
   if (status_ok)
   { /* TSK-File erfolgreich geladen */
      tsk_aend = FALSE;
      if (init_mode)
      { /* System-Start */
         ok = simuInit (&ext_sim_ok);        /* erst nach dem TSK-File laden! */
         if (!ok)
            fatal (MEN_FLOAD, 8, NO_INFO, NO_INFO, "", NO_SIMU_INIT);
         if (!ext_sim_ok)
            fehler (MEN_FLOAD, 9, NO_INFO, NO_INFO, "", NO_EXT_SIMU);
      } /* System-Start */
   } /* TSK-File erfolgreich geladen */
   if (!init_mode)
   { /* interactive */
      if (status_ok) {
	 setStringVal(tsk_fspec, TSK_FILE_SPEC_IDX);  /* update TSK-file spec */
	 initFileReloaded = TRUE;
      }
      else /* faulty file */
	 if (bad_data)
	    setStringVal(BAD_DATA_TXT, TSK_FILE_SPEC_IDX);    /* data spoiled */
	 else {
	    if (getStringVal(tsk_fspec, TSK_FILE_SPEC_IDX, GLEAM_ERROR))
	       if ((strlen(tsk_fspec) > 0) && strcmp(tsk_fspec, BAD_DATA_TXT))
		  meldung (MEN_FLOAD, NO_INFO, NO_INFO, tsk_fspec, OLD_TSK_STILL_VALID);
	 }
   } /* interactive */
   if ((init_mode && !status_ok) || (bad_data && !status_ok))
      fehler (MEN_FLOAD, 10, NO_INFO, NO_INFO, "", TSK_DATA_INKONSISTENT);
   bad_tsk_data = bad_data || (!status_ok && bad_tsk_data);

   return (status_ok && ok && ext_sim_ok);
} /* load_tsk_file */



/* ============================= load_evo_file ============================== */
BOOL load_evo_file (char *evo_fspec,
                    BOOL  init_mode)
{
   BOOL  status_ok,  /* GSTATE war GLEAM_OK                                   */
         bad_data;   /* alte Daten wurden bei fehlerhaftem Lesen inkonsistent */

   /* load_evo_file */
   status_ok = loadEvoParamFile(evo_fspec, &bad_data) == GLEAM_OK;
   if (!init_mode)
   { /* interactive */
      if (status_ok) {
	 setStringVal(evo_fspec, EVO_FILE_SPEC_IDX);  /* update EVO-file spec */
	 initFileReloaded = TRUE;
      }
      else /* faulty file */
	 if (bad_data)
	    setStringVal(BAD_DATA_TXT, EVO_FILE_SPEC_IDX);    /* data spoiled */
	 else {
	    if (getStringVal(evo_fspec, EVO_FILE_SPEC_IDX, GLEAM_ERROR))
	       if ((strlen(evo_fspec) > 0) && strcmp(evo_fspec, BAD_DATA_TXT))
		  meldung (MEN_FLOAD, NO_INFO, NO_INFO, evo_fspec, OLD_EVO_STILL_VALID);
	 }
   } /* interactive */
   if ((init_mode && !status_ok) || (bad_data && !status_ok))
      fehler (MEN_FLOAD, 11, NO_INFO, NO_INFO, "", EVO_DATA_INKONSISTENT);
   bad_evo_data = bad_data || (!status_ok && bad_evo_data);

   return (status_ok);
} /* load_evo_file */



