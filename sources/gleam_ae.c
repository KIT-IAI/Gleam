/*******************************************************************************
GLEAM/AE                                                                  V1.1.4

                               G L E A M  /  A E
        Genetic LEarning Algorithm and Methods / Application Environment

                       Main program of the TEXT_MODE variant
 
Package: c_vers                 File: gleam_ae.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Angabe eines Logfilenamens als 2.Parameter und Benutzung des existierenden 
    EXP-Filenamens fuers Logfile, falls kein 2.Parameter angegeben.
 2. Bei DOS u.Textmode: Fortsetzungsfrage nach dem Programmkopf.     WJ 08.08.00
 3. Integration von BS_WINXP_GNU (moeglicherweise ueberfluessig)     WJ 21.08.07
 4. Aktualisierung der Lizenz wegen GLEAM/GORBA.                     WJ 24.04.08
 5. Entfernung von bedingtem Code bei SMALL_SCREEN und
    bedingter Includes bei BS_DOS odder BS_WINXP_GNU.                WJ 20.06.12
 6. exp- und log-File können mit und ohne Dateierweiterung angegeben 
    werden. Verbesserung der Meldung bei parameterlosem Aufruf.      WJ 15.04.16
 7. Renovierung: Anpassung des "write_text_buf"-Aufrufs. Ersetzung 
    der "write_line"-Ausgaben durch "printf"-Aufrufe. Streichung des 
    "link"-Includes. Init-Meldungen auch ins Logfile.                WJ 13.05.16
 8. Neuer Lizenztext basierend auf der LGPL-Lizenz.                  WJ 09.09.16
 9. Anpassung an die neue Datenhaltung der Filenamen. Anpassung des 
    "write_to_log"-Aufrufs.                                          WJ 19.12.16
10. Neue Include-Datei USR_MAIN_TEXTS.                               WJ 05.10.17
11. Neuer Parameter fuer "show_and_clear_fbuf".                      WJ 10.11.17
12. New variable "initialChrFileMode", which controls the file 
    format of an initial chromosome file.                            WJ 18.04.18
13. New big string buffer "bigBuf" to avoid overflow warnings. 
    A chromosome file (MEM file) that may need to be loaded is 
    searched for first in the present working directory and if it 
    does not exist there also in the directory given by the environ-
    ment variable, the name of which is contained in GLEAM_ROOT_ENV 
    string constant.                                                
    Some improvements regarding logging and messages.                WJ 11.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Author: W.Jakob                Date: 11.08.2020
================================================================================ */
#include "schalter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "chio.h"
#include "hmod.h"
#include "simu.h"
#include "aufg.h"
#include "men.h"

#include MAIN_TEXTS
#include USR_MAIN_TEXTS
#include GLOBAL_TEXTS



/* ======================================== main ====================================== */
INT main (int argc, char *argv[])
{
   char         bigBuf[2*MAX_STRING],
               *envVar; 
   STRING_TYPE  fSpecBuf, logFSpecBuf, buf1;
   STR_TYPE     buf2;  
   INT          len;           /* length of file name parameter                         */
   INT          ret_code = 0;  /* programm return code                                  */
   BOOL         mit_ch_file,   /* TRUE, if there is a chr. file spec. in the EXP-File   */
                exp_load_ok,   /* TRUE, if successfilly loaded EXP-file and its files   */
                ohneExt;       /* filen name without extension                          */

   strcpy (logFSpecBuf, "");       /* as long as empty the default logfile name is used */
   if (argc > 1)
   { /* at least one parameter available */
      len = strlen (argv[1]);
      if (len > 4)
	 ohneExt = strcasecmp (&(argv[1][len-4]), ".exp") != 0;
      else
	 ohneExt = TRUE;
      strcpy (fSpecBuf, argv[1]);
      if (ohneExt)
	 strcat (fSpecBuf, ".exp");
      if (argc == 3)
      { /* take logfile specification of the user */
	 len = strlen (argv[2]);
	 if (len > 4)
	    ohneExt = strcasecmp (&(argv[2][len-4]), ".log") != 0;
	 else
	    ohneExt = TRUE;
	 strcpy (logFSpecBuf, argv[2]);
	 if (ohneExt)
	    strcat (logFSpecBuf, ".log");
      } /* take logfile specification of the user */
      else                                     /* use the EXP file name for the logfile */
	 if (file_exists (fSpecBuf)) 
	 { /* EXP file does not exist */
	    if (ohneExt) 
	       strcpy (logFSpecBuf, argv[1]);
	    else {
	       strncpy (logFSpecBuf, argv[1], len-4);
               logFSpecBuf[len-4] = '\0';
	    }
	    strcat (logFSpecBuf, ".log");
	 } /* EXP file does not exist */
   } /* at least one parameter available */

   if (gen_init(TRUE, FALSE, logFSpecBuf))            /* ChainIO-Logging, Error-Logging */
   { /* packages loded succesfully */
      if (argc == 1) 
      { /* no parameter given */
	 strcpy (fSpecBuf, EXP_DEFAULT_FNAME);
	 sprintf (buf2, NO_EXP_PARAM, EXP_DEFAULT_FNAME);
	 printf (MSG_LINE_TXT, buf2);
      } /* no parameter given */

      exp_load_ok = load_exp (&mit_ch_file, fSpecBuf, TRUE);
      printf ("%s\n", INIT_HDR_1);
      printf ("%s\n", INIT_HDR_2);
      sprintf(bigBuf, EXP_FILE_TXT, fSpecBuf);
      write_to_log(bigBuf, KEEP_LOG_OPEN);
      logTextBuffer (MIT_TERM_AUSG);          /* output to the logfile and the terminal */
      if (exp_load_ok)
      { /* initialisation files loaded */
         if (mit_ch_file)
	 { /* with chromosome file */
	    getStringVal(fSpecBuf, MEM_FILE_SPEC_IDX, GLEAM_OK);
	    envVar= getenv(GLEAM_ROOT_ENV);                 /* get GLEAM_ROOT contents  */
	    strcpy(bigBuf, fSpecBuf);
	    if (!file_exists(fSpecBuf) && (envVar != NULL))         /* check GLEAM_ROOT */
	       sprintf(bigBuf, "%s/%s", envVar, fSpecBuf);          /* and fSpecBuf     */
	    if (open_ch_file (bigBuf, 1, CH_READ, GLOBAL_SAVE, initialChrFileMode))
	    { /* chromosome file open */
	       if (restore_chs (TRUE, FALSE))            /* MitBAKs=TRUE, anzeige=FALSE */
	       { /* ok */
		  get_buf (1, buf1);
		  printf ("  %s\n", buf1);
		  get_buf (2, buf1);
		  printf ("    %s\n", buf1);
		  reset_text_buf ();
	       } /* ok */
	       close_ch_file ();
	    } /* chromosome file open */
	    else {
	       printf ("%s\n", NO_CH_FILE_TXT);
	       write_to_log (NO_CH_FILE_TXT, CLOSE_LOG);
	    }
	 } /* with chromosome file */
	 else {
	    printf ("%s\n", NO_CH_FILE_TXT);
	    write_to_log (NO_CH_FILE_TXT, CLOSE_LOG);
	 }
	 show_and_clear_fbuf(LOG_ALL_MSGS);
	 printf ("%s\n", INIT_FIN_TXT);
	 write_to_log(INIT_FIN_TXT, CLOSE_LOG);

         main_menue ();                                         /* start the main menue */
      } /* initialisation files loaded */
      else
      { /* error during EXP file loading */
	 fatal (GLEAM_MAIN, 1, NO_INFO, NO_INFO, "", NO_INI);
	 if (argc < 3)
	 { /* no or only one parameter specified */
	    sprintf (buf1, HELP_TXT2, EXE_NAME);
	    sprintf (buf2, HELP_TXT4, LOGFILE_NAME);
	    printf ("\n%s\n", HELP_TXT1);
	    printf ("%s\n", buf1);
	    printf ("%s\n", HELP_TXT3);
	    printf ("%s\n", buf2);
	    sprintf (buf1, HELP_TXT5, EXP_DEFAULT_FNAME, LOGFILE_NAME);
  	    printf ("%s\n", buf1);
	    printf ("%s\n", HELP_TXT6);
	    printf ("%s\n\n", HELP_TXT7);
	 } /* no or only one parameter specified */
      } /* rror during EXP file loading */
   } /* packages loded succesfully */
   else
   { /* error during loading the packages */
      if (textBufOK)
      { /* possible messages on screen and into logfile */
	 logTextBuffer(MIT_TERM_AUSG);             
	 write_to_log("", CLOSE_LOG);
      } /* possible messages on screen and into logfile */
      fprintf (stderr, NO_INIT_CRASH);
      ret_code = 1;
   } /* error during loading the packages */

   return (ret_code);
} /* main */




/* -----------------------------------------------------------------------------
Copyright (c) 2003 .. 2020  Christian Blume, Wilfried Jakob

Das Urheberrecht (Copyright) fuer GLEAM (General Learning Evolutionary Algorithm 
and Method, frueher: Genetic Learning Algorithm and Method) liegt bei 
Prof. Dr. Christian Blume.

Das Urheberrecht (Copyright) fuer HyGLEAM (Hybrid General purpose Evolutionary 
Algorithm and Method), einem auf GLEAM beruhenden Werk, liegt bei
Dr. Wilfried Jakob, Karlsruher Institut fuer Technologie (KIT).

Diese Datei ist Teil von GLEAM und HyGLEAM. GLEAM und HyGLEAM sind Freie Software: 
Sie koennen sie unter den Bedingungen der GNU Lesser General Public License, wie 
von der Free Software Foundation, Version 3 der Lizenz oder (nach Ihrer Wahl) 
jeder spaeteren veroeffentlichten Version, weiterverbreiten und/oder modifizieren.

GLEAM und HyGLEAM werden in der Hoffnung, dass sie nützlich sein werden, aber
OHNE JEDE GEWAEHRLEISTUNG, bereitgestellt; sogar ohne die implizite Gewaehr-
leistung der MARKTFAEHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die 
GNU Lesser General Public License fuer weitere Details.

In allen Publikationen, welche unter Verwendung von GLEAM, HyGLEAM oder darauf 
beruhender Werke entstanden sind, ist auf die Urheber von GLEAM und gegebenen-
falls auch von HyGLEAM hinzuweisen. Dies kann durch nachstehende Literaturver-
weise geschehen:

  C. Blume: GLEAM - A System for Intuitive Learning. In: Schwefel, H.P., 
  Maenner, R. (eds.): Proc. of PPSN I, LNCS 496, Springer, Berlin, S.48-54, 1990.

  C. Blume, W. Jakob:  GLEAM - ein Evolutionärer Algorithmus und seine 
  Anwendungen. KIT Scientific Publishing, Schriftenreihe des Instituts fuer 
  Angewandte Informatik / Automatisierungstechnik (AIA), Band 32, 
  ISBN 978-3-86644-436-2, 2009. 

  W. Jakob: A general cost-benefit-based adaptation framework for multimeme 
  algorithms. Memetic Computing, 2(2010), S.201-218. 

--------------------------------------------------------------------------------
Copyright (c) 2003 .. 2020  Christian Blume, Wilfried Jakob

The copyright of GLEAM (General Learning Evolutionary Algorithm and Method, 
former: Genetic Learning Algorithm and Method) is owned by Prof. Dr. Christian 
Blume.

The copyright of HyGLEAM (Hybrid General purpose Evolutionary Algorithm and 
Method),  a software based on GLEAM, is owned by Dr. Wilfried Jakob, Karlsruhe 
Institute of Technology (KIT).

This file is part of GLEAM and HyGLEAM. GLEAM and HyGLEAM are free software: 
you can redistribute them and/or modify them under the terms of the GNU Lesser 
General Public License as published by the Free Software Foundation, either 
version 3 of the License, or (at your option) any later version.

GLEAM and HyGLEAM are distributed in the hope that they will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License 
for more details.

In all publications which refer to GLEAM, HyGLEAM or works based on it or to 
results obtained by the usage of GLEAM, HyGLEAM or works based on it, a
reference to the Author(s) of GLEAM and HyGLEAM, if applicable, shall be 
includeded. This can be done by the following literature references:

  C. Blume: GLEAM - A System for Intuitive Learning. In: Schwefel, H.P., 
  Maenner, R. (eds.): Proc. of PPSN I, LNCS 496, Springer, Berlin, S.48-54, 1990.

  W. Jakob:  A general cost-benefit-based adaptation framework for multimeme 
  algorithms. Memetic Computing, 2(2010), S.201-218. 

-------------------------------------------------------------------------------

GNU LESSER GENERAL PUBLIC LICENSE

Version 3, 29 June 2007

Copyright © 2007 Free Software Foundation, Inc. <http://fsf.org/>

Everyone is permitted to copy and distribute verbatim copies of this license document, 
but changing it is not allowed.

This version of the GNU Lesser General Public License incorporates the terms and conditions 
of version 3 of the GNU General Public License, supplemented by the additional permissions 
listed below.


0. Additional Definitions.

As used herein, "this License" refers to version 3 of the GNU Lesser General Public License, 
and the "GNU GPL" refers to version 3 of the GNU General Public License.

"The Library" refers to a covered work governed by this License, other than an Application or 
a Combined Work as defined below.

An "Application" is any work that makes use of an interface provided by the Library, but 
which is not otherwise based on the Library. Defining a subclass of a class defined by the 
Library is deemed a mode of using an interface provided by the Library.

A "Combined Work" is a work produced by combining or linking an Application with the Library. 
The particular version of the Library with which the Combined Work was made is also called 
the "Linked Version".

The "Minimal Corresponding Source" for a Combined Work means the Corresponding Source for 
the Combined Work, excluding any source code for portions of the Combined Work that, 
considered in isolation, are based on the Application, and not on the Linked Version.

The "Corresponding Application Code" for a Combined Work means the object code and/or source 
code for the Application, including any data and utility programs needed for reproducing the 
Combined Work from the Application, but excluding the System Libraries of the Combined Work.


1. Exception to Section 3 of the GNU GPL.

You may convey a covered work under sections 3 and 4 of this License without being bound by 
section 3 of the GNU GPL.


2. Conveying Modified Versions.

If you modify a copy of the Library, and, in your modifications, a facility refers to a 
function or data to be supplied by an Application that uses the facility (other than as an 
argument passed when the facility is invoked), then you may convey a copy of the modified 
version:

    a) under this License, provided that you make a good faith effort to ensure that, in 
       the event an Application does not supply the function or data, the facility still 
       operates, and performs whatever part of its purpose remains meaningful, or
    b) under the GNU GPL, with none of the additional permissions of this License applicable 
       to that copy.


3. Object Code Incorporating Material from Library Header Files.

The object code form of an Application may incorporate material from a header file that is 
part of the Library. You may convey such object code under terms of your choice, provided 
that, if the incorporated material is not limited to numerical parameters, data structure 
layouts and accessors, or small macros, inline functions and templates (ten or fewer lines 
in length), you do both of the following:

    a) Give prominent notice with each copy of the object code that the Library is used in 
       it and that the Library and its use are covered by this License.
    b) Accompany the object code with a copy of the GNU GPL and this license document.


4. Combined Works.

You may convey a Combined Work under terms of your choice that, taken together, effectively do 
not restrict modification of the portions of the Library contained in the Combined Work and 
reverse engineering for debugging such modifications, if you also do each of the following:

    a) Give prominent notice with each copy of the Combined Work that the Library is used in 
       it and that the Library and its use are covered by this License.
    b) Accompany the Combined Work with a copy of the GNU GPL and this license document.
    c) For a Combined Work that displays copyright notices during execution, include the 
       copyright notice for the Library among these notices, as well as a reference directing 
       the user to the copies of the GNU GPL and this license document.
    d) Do one of the following:
        0) Convey the Minimal Corresponding Source under the terms of this License, and the 
           Corresponding Application Code in a form suitable for, and under terms that permit, 
           the user to recombine or relink the Application with a modified version of the Linked 
           Version to produce a modified Combined Work, in the manner specified by section 6 of 
           the GNU GPL for conveying Corresponding Source.
        1) Use a suitable shared library mechanism for linking with the Library. A suitable 
           mechanism is one that (a) uses at run time a copy of the Library already present 
           on the user's computer system, and (b) will operate properly with a modified 
           version of the Library that is interface-compatible with the Linked Version.
    e) Provide Installation Information, but only if you would otherwise be required to provide 
       such information under section 6 of the GNU GPL, and only to the extent that such 
       information is necessary to install and execute a modified version of the Combined Work 
       produced by recombining or relinking the Application with a modified version of the 
       Linked Version. (If you use option 4d0, the Installation Information must accompany 
       the Minimal Corresponding Source and Corresponding Application Code. If you use option 
       4d1, you must provide the Installation Information in the manner specified by section 6 
       of the GNU GPL for conveying Corresponding Source.)


5. Combined Libraries.

You may place library facilities that are a work based on the Library side by side in a single 
library together with other library facilities that are not Applications and are not covered 
by this License, and convey such a combined library under terms of your choice, if you do both 
of the following:

    a) Accompany the combined library with a copy of the same work based on the Library, 
       uncombined with any other library facilities, conveyed under the terms of this License.
    b) Give prominent notice with the combined library that part of it is a work based on 
       the Library, and explaining where to find the accompanying uncombined form of the same 
       work.


6. Revised Versions of the GNU Lesser General Public License.

The Free Software Foundation may publish revised and/or new versions of the GNU Lesser 
General Public License from time to time. Such new versions will be similar in spirit to 
the present version, but may differ in detail to address new problems or concerns.

Each version is given a distinguishing version number. If the Library as you received it 
specifies that a certain numbered version of the GNU Lesser General Public License "or any 
later version" applies to it, you have the option of following the terms and conditions 
either of that published version or of any later version published by the Free Software 
Foundation. If the Library as you received it does not specify a version number of the 
GNU Lesser General Public License, you may choose any version of the GNU Lesser General 
Public License ever published by the Free Software Foundation.

If the Library as you received it specifies that a proxy can decide whether future 
versions of the GNU Lesser General Public License shall apply, that proxy's public 
statement of acceptance of any version is permanent authorization for you to choose 
that version for the Library.

----------------------------------------------------------------------------- */
