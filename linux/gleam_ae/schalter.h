/* -----------------------------------------------------------------------------
GLEAM und HyGLEAM               Global Header-File
Package: -                       File: schalter.h            Version:     1.11.1
Status : o.k.                   Autor: W.Jakob                 Datum: 19.08.2020
--------------------------------------------------------------------------------

                            For GLEAM/AE and HyGLEAM/A
                                   (gleam_ae)
Operating systems:
  current:   Ubuntu 20.04.1 LTS / gcc 9.3.0 
  previous:  Linux Suse  11.0   / gcc 4.3.1
             Solaris 5.8        / Sun cc und CC 5.1 sowie g++ 2.95

Benutzerschnittstelle:
    GLEAM_USR   Entscheidet , ob interaktive Version (gesetzt) oder reines 
                Kommandozeilenprogramm (CLV). Falls gesetzt:
    TEXT_MODE   Dialoge und Menues scrollend als ASCII-Text, BS-unabhaengig (TUI).
    GUI_MODE    GUI mit Qt.

--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------
Changes:
 1. Linux-Portierung: Differenzierung zw. BS_SOLARIS und BS_LINUX.   WJ 12.05.05
 2. Entfernung des Schalters MIT_ADI, der in MIT_LSV aufgegangen ist. 
    Neuer Schalter MIT_ROCO, der neu bei MIT_LSV gueltig sein kann.  WJ 23.08.06
 3. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07 
 4. Definition der Environmentvariablen hier. Pfad der Fehlertext-
    files aus dem Environment und daher hier nur noch die Namen.     WJ 28.08.09
 5. Zur Vorbereitung der MPI-basierten Parallelversion wurde fol-
    gendes entfernt: MITGRAFIK, MIT_ANIM, SMALL_SCREEN
    - alte Eintraege: BGI_MODE und DOS
    - Neuerungen der Matlab-Kopplung: PAR_SIMU und BS_WINXP_GNU
    - Ueberbleibsel der pvm-Version: PAR_VERS
    - Neu geregelt wurden: GNUPLOT
    Neu hinzugekommen sind: MPI_PAR_VERS, GUI_MODE                   WJ 20.06.12
 6. Erweiterungen fuer die MPI-Parallelversion. Einrueckung der 
    bedingten Preprocessor-Anweisungen. NEU: MIN_ANZ_NACHBARN.       WJ 31.08.12
 7. Streichung von MOD_, BEW_, TSK_ und EVO_DEFAULT_FNAME.           WJ 10.03.16
 8. Umbenennung von MOD_FILE_IDENT nach PROG_IDENT. Streichung des 
    Schalters LOGFILE, da immer eins angelegt wird.                  WJ 19.12.16
 9. Neue alternative Schalter EXT_MATLAB_SIMU fuer die matlab-Kopplung 
    und EXT_SIMU_SERV fuer die Kopplung zu den "external simulation 
    services". Einer von beiden gesetzt: Entsprechendes Makefile be-
    nutzen! Zusammenfuehrung der Definition des EXE_NAME aus 
    "gleam_d/gb.h" und des EXE_FNAME aus "hyGleamEngine2.c" in
    dieser Datei.                                                    WJ 19.05.17
10. Neue Datei USR_MAIN_TEXTS.                                       WJ 05.10.17
11. Vereinheitlichtes schalter-File fuer GLEAM und HyGLEAM als 
    Kommandozeilenversion oder mit textuellem Userinterface.         WJ 12.12.17
12. Streichung von ENG_MAIN_TEXTS. CLV_MAIN_TEXTS ersetzt   
    ENG2_MAIN_TEXTS.                                                 WJ 22.12.17
-------------------------------------------------------------------------------- */

/* ----------- Global Switches for Versions and Operating Systems: ---------- */
#undef BS_SOLARIS          /* set: Sun/Solaris     } only one BS_... switch  */
#define BS_LINUX            /* set: Linux           } may be set!             */
#define GLEAM_USR           /* set: With user interface, otherwise CLV        */
#undef EXT_MATLAB_SIMU     /* set: Matlab/Matpower as ext. Simu. } only one  */
#undef EXT_SIMU_SERV       /* set: extern. Simu.Services (ESS)   } may be set*/
#undef MPI_PAR_VERS        /* MPI based parallel implmentation for Linux     */
#undef MIT_AEHNL_CHECK     /* with similarity check before simulation        */
#undef MIT_LSV             /* set: GLEAM with local hill climbers            */

/* ----------------------------- Language Selection: ------------------------ */
#undef DEU                 /* set: Deutsch.                     } only one   */
#define ENG                 /* set: English.                     } may be set */


/* -------------------- Name of Program and Executable: --------------------- */
#ifdef MIT_LSV
  #define PROGRAM_NAME   "HyGLEAM/A"
  #ifdef GLEAM_USR 
    #define EXE_NAME       "hyGleam"
  #else /* no (textual) user interface */
    #define EXE_NAME       "hyGleamCLV"
  #endif /* no (textual) user interface */
#else /* no MIT_LSV */
  #define PROGRAM_NAME   "GLEAM/AE"
  #ifdef GLEAM_USR 
    #define EXE_NAME       "gleam_ae"
  #else /* no (textual) user interface */
    #define EXE_NAME       "gleamCLV"
  #endif /* no (textual) user interface */
#endif /* no MIT_LSV */


/* ----------------------------- User-Interface: ---------------------------- */
#if defined(GLEAM_USR)
  #define TEXT_MODE          /* textual user interface  }     alter-          */
  #undef GUI_MODE           /* grafical user interface }     native          */
  #define GNUPLOT            /* integration of GNUPLOT for eval.fct.display   */
#else /* --------------------------- no GLEAM_USR --------------------------- */
  #undef TEXT_MODE          /* textual user interface  }     alter-          */
  #undef GUI_MODE           /* grafical user interface }     native          */
  #undef GNUPLOT            /* integration of GNUPLOT for eval.fct.display   */
#endif /* no GLEAM_USR */


/* ---------------------- Global Switch for Test Code: ---------------------- */
#define GLO_CHECKS           /* details are determined in the packages        */


/* ----------------- Switches for Statistics and Log File: ------------------ */
#define MIT_OP_STATISTIK         /* opt. statistics of genetic operator usage */
#define LOGFILE_MODE      "at"   /* writing file in append mode               */


/* --------------------------- Simulator Switches: -------------------------- */
#define MITSIMULOG           /* with log file for the simulator (interface)   */
#define SIMU_LOGF_MODE  "wt" /* writing the file as a new one                 */
#undef SIMU_DBG             /* debug output to scroll window and log file    */
#undef ONLINE               /* data exch.with simulator OR test I/O from file*/


/* ------------------ Names of the Environment Variables: ------------------- */
#define SIM_MOD_ROOT_ENV     "SIM_MOD_ROOT"
#define GLEAM_ROOT_ENV       "GLEAM_ROOT"


/* ========================================================================== */
/* --------------------------- Derived Switches: ---------------------------- */
#ifdef MIT_LSV
  #define MIT_ROCO           /* with Rosenbrock and Complex LHCs from ITEM    */
#else
  #undef MIT_ROCO            /* no local hill climbers (LHCs) at all          */
#endif /* MIT_LSV */


/* ----------------------------- Solaris/Linux: ----------------------------- */
#if defined(BS_SOLARIS) || defined(BS_LINUX)
  #define DIR_DELIMITER      "/"
  #define MAX_POPEL_SIZE   20000     /* maximum population size               */
  #define MIN_ANZ_NACHBARN     4     /* minimum neighborhood size (even)      */
  #define MAX_ANZ_NACHBARN    32     /* maximum neighborhood size (even)      */
  #define MAX_EVO_WS         101     /* size of the optimization job list     */
#endif /* Solaris, Linux */


/* ------------------------ Language Dependent Files: ----------------------- */
#ifdef DEU        /* ----------------------- Deutsch: ----------------------- */
  #define FTEXTE_FNAME       "ftext_d.txt"
  #define ADD_FTEXTE_FNAME   ""
  #ifdef MPI_PAR_VERS            
    #define FTEXTEPAR_FNAME  "ftext_mpi_d.txt"
  #endif /* MPI_PAR_VERS */
  #define GLOBAL_TEXTS       "glob_d.h"
  #define SYS_TEXTS          "sys_d.h"
  #define CTIO_TEXTS         "ctio_d.h"
  #define FBHM_TEXTS         "fbhm_d.h"
  #define LGSW_TEXTS         "lgsw_d.h"
  #define CHIO_TEXTS         "chio_d.h"
  #define BEW_TEXTS          "bew_d.h"
  #define AUFG_TEXTS         "aufg_d.h"
  #define HMOD_TEXTS         "hmod_d.h"
  #define CHED_TEXTS         "ched_d.h"
  #define SIMU_TEXTS         "simu_d.h"
  #define EVO_TEXTS          "evo_d.h"
  #define APPL_TEXTS         "appl_d.h"
  #define MEN_TEXTS          "men_d.h"
  #define VERS_TEXTS         "vers_d.h"
  #define MAIN_TEXTS         "gleam_d.h"
  #define USR_MAIN_TEXTS     "gleam_usr_d.h"
  #define CLV_MAIN_TEXTS     "gleamCLV_d.h"
  #ifdef MPI_PAR_VERS
    #define PCOM_TEXTS       "pcom_d.h"
    #define MPAR_TEXTS       "mPar_d.h"
    #define PAR_MEN_TEXTS    "par_men_d.h"
  #endif /*MPI_PAR_VERS*/
#endif /* DEU */
#ifdef ENG        /* ----------------------- English: ----------------------- */
  #define FTEXTE_FNAME       "ftext_gb.txt"
  #define ADD_FTEXTE_FNAME   ""
  #ifdef MPI_MPI_PAR_VERS            
    #define FTEXTEPAR_FNAME  "ftext_mpi_gb.txt"
  #endif /* MPI_PAR_VERS */
  #define GLOBAL_TEXTS       "glob_gb.h"
  #define SYS_TEXTS          "sys_gb.h"
  #define CTIO_TEXTS         "ctio_gb.h"
  #define FBHM_TEXTS         "fbhm_gb.h"
  #define LGSW_TEXTS         "lgsw_gb.h"
  #define CHIO_TEXTS         "chio_gb.h"
  #define BEW_TEXTS          "bew_gb.h"
  #define AUFG_TEXTS         "aufg_gb.h"
  #define HMOD_TEXTS         "hmod_gb.h"
  #define CHED_TEXTS         "ched_gb.h"
  #define SIMU_TEXTS         "simu_gb.h"
  #define EVO_TEXTS          "evo_gb.h"
  #define APPL_TEXTS         "appl_gb.h"
  #define MEN_TEXTS          "men_gb.h"
  #define VERS_TEXTS         "vers_gb.h"
  #define MAIN_TEXTS         "gleam_gb.h"
  #define USR_MAIN_TEXTS     "gleam_usr_gb.h"
  #define CLV_MAIN_TEXTS     "gleamCLV_gb.h"
  #ifdef MPI_PAR_VERS
    #define PCOM_TEXTS       "pcom_gb.h"
    #define MPAR_TEXTS       "mPar_gb.h"
    #define PAR_MEN_TEXTS    "par_men_gb.h"
  #endif /* PAR_VERS */
#endif /* ENG */


/* ----------------------- File Names and MOD-File-ID: ---------------------- */
#define PROG_IDENT         "GLEAM/AE"
#define EXP_DEFAULT_FNAME  "default.exp"
#define TOP_RING_DEF_FNAME "symRing.top"                  /* case insensitive */
#define VERS_DOKU_FNAME    "versdoku.txt"
#define LOGFILE_NAME       "gleam_ae.log"
#define SIMU_LOGFILE_NAME  "simu_tst.log"
#define EVO_ABBRUCH_FSPEC  "evo_stop.tmp"


/* --- Includes for all Sources: ttype.h, szsunsol.h: ------- */
#include "ttype.h"
#if defined(BS_SOLARIS) || defined(BS_LINUX)
  #include "szsunsol.h"
#endif /* Solaris or Linux */


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

------------------------------------------------------------------------------------------
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

-----------------------------------------------------------------------------------------

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
