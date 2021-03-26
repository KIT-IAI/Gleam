/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Englisch)
Package: HP                           File: vers_gb.h                  Version:     V1.8.0
Status : o.k.                        Autor: W.Jakob                      Datum: 19.08.2020
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------------- */

/* ---------------------------------- Versions Info ----------------------------------- */
#define APPL_TXT_1       "Applications implemented in this version:"
#define APPL_TXT_2       "  LESAK   Collision-free path planning for industrial robots (Blume)"
#define APPL_TXT_3       "  MBF     Mathematical benchmark functions (Jakob)"
#define APPL_TXT_4       "  OPALV   Production planning in process engineering (Blume, Jakob)"
#define AUTHOR_CB        "Prof. Dr. Christian Blume (HS K"oe"ln, Campus Gummersbach)"
#define AUTHOR_WJ        "Dr. Wilfried Jakob (KIT, Campus North, IAI)            "

#ifdef MIT_LSV
  #ifdef OPAL_V
    #define VERS_INFO_1    "                      H y G L E A M / O P A L V"
    #define VERS_INFO_2    "                      ========================="
    #define VERS_INFO_4    "                    Algorithm and Method / OPAL-V"
    #define VERS_INFO_17   APPL_TXT_1
    #define VERS_INFO_18   APPL_TXT_2
    #define VERS_INFO_19   APPL_TXT_3
    #define VERS_INFO_20   APPL_TXT_4
  #else /* no OPAL_V */
    #define VERS_INFO_1    "                           H y G L E A M / A"
    #define VERS_INFO_2    "                           ================="
    #define VERS_INFO_4    "                   Algorithm and Method / Adaptive"
    #define VERS_INFO_17   ""
    #define VERS_INFO_18   APPL_TXT_1
    #define VERS_INFO_19   APPL_TXT_2
    #define VERS_INFO_20   APPL_TXT_3
  #endif /* no OPAL_V */
  #define VERS_INFO_3    "             Hybrid GeneraL purpose Evolutionary learning "
  #define VERS_INFO_5    ""
  /*                      123456789012345678901234567890123456789012345678901234567890123456.8901234*/
  #define VERS_INFO_6    "Adaptive hybrid optimization algorithm consisting of the global optimi-"
  #define VERS_INFO_7    "zation procedure GLEAM and several local hill climber algorithms (LHC), " 
  #define VERS_INFO_8    "among them the Rosenbrock and the COMPLEX algorithm from Box. "
  #define VERS_INFO_9    "GLEAM is an Evolutionary Algorithm of its own, which combines elements "
  #define VERS_INFO_10   "of computer science, of the Evolution Strategy from Rechenberg and"
  #define VERS_INFO_11   "Schwefel, and of the Genetic Algorithms from Holland. "
  #define VERS_INFO_12   "Besides the basic algorithms GLEAM, Rosenbrock, and Complex the following "
  #define VERS_INFO_13   "Memetic Algorithms (MA) are implemented:"
  #define VERS_INFO_14   "  SMA:  Simple Memetic Algorithm: GLEAM + one LHC with fixed parameters "
  #define VERS_INFO_15   "  ASMA: Adaptive Single Memetic Alg.: GLEAM + one LHC with parameter adaptation"
  #define VERS_INFO_16   "  AMMA: Adaptive Multi-Memetic Alg.: GLEAM+LHCs, parameter and selection adapt."
  #define VERS_INFO_21   "                    "AUTHOR_WJ
  #define VERS_INFO_22   "                    "AUTHOR_CB
#else /* kein HyGLEAM */
  #define VERS_INFO_1    "                         G L E A M / A E"
  #define VERS_INFO_2    "                         ==============="
  #define VERS_INFO_3    "       General Learning Evolutionary Algorithm and Method"
  #define VERS_INFO_4    ""
  #define VERS_INFO_5    "GLEAM is an Evolutionary Algorithm of its own, which combines elements "
  #define VERS_INFO_6    "of computer science (concept of abstract data types), of the Evolution "
  #define VERS_INFO_7    "Strategy from Rechenberg and Schwefel, and of the Genetic Algorithms "
  #define VERS_INFO_8    "from Holland." 
  #define VERS_INFO_9    "Some characteristics of GLEAM:"
  #define VERS_INFO_10   "  - Application-oriented configuration of chromosomes based on gene "
  #define VERS_INFO_11   "    types and corresponding genetic operators"
  #define VERS_INFO_12   "  - Aggregation of genes into segments and their evolution"
  #define VERS_INFO_13   "  - Structured population according to the diffusion model based on"
  #define VERS_INFO_14   "    a ring neighbourhood"
  #define VERS_INFO_15   "  - Assessment by the cascaded weighted sum and penalty functions"
  #define VERS_INFO_16   ""
  #define VERS_INFO_17   APPL_TXT_1
  #define VERS_INFO_18   APPL_TXT_2
  #define VERS_INFO_19   APPL_TXT_3
  #define VERS_INFO_20   "                "AUTHOR_CB
  #define VERS_INFO_21   "                "AUTHOR_WJ
  #define VERS_INFO_22   ""
#endif  /* kein HyGLEAM */
#define VERS_INFO_CBL  "The copyright of GLEAM is owned by Prof. Dr. Christian Blume."
#define VERS_INFO_CWJ1 "The copyright of HyGLEAM/A, a work based on GLEAM, is owned by "
#define VERS_INFO_CWJ2 "Dr. Wilfried Jakob, Karlsruhe Institue of Technology (KIT). "

#define VERS_INFO_H1   "HyGLEAM/A includes the Rosenbrock Algorithm and the COMPLEX procedure "
#define VERS_INFO_H2   "as standard local hill climbers. They were implemented at the Univer-"
#define VERS_INFO_H3   "sity of Bremen, ITEM, Prof. R.Laur, and integrated into HyGLEAM/A as "
#define VERS_INFO_H4   "a result of a joint research project."
#define VERS_INFO_L1   "GLEAM and HyGLEAM/A are free software governed by the GNU LGPL v3, "
#define VERS_INFO_L2   "see www.gnu.org. The licence is included in the main programs and "
#define VERS_INFO_L3   "the \"schalter.h\"-files. The software is distributed WITHOUT ANY "
#define VERS_INFO_L4   "WARRANTY; without even the implied warranty of MERCHANTABILITY or "
#define VERS_INFO_L5   "FITNESS FOR A PARTICULAR PURPOSE."


/* -------------------------------- Start Header Texts -------------------------------- */
#define P_TITLE_GL1   "GLEAM is an EA of its own, which combines elements of comp."
#define P_TITLE_GL2   "sci., of the ES of Rechenberg and of the GAs of Holland.   "
#define P_TITLE_GPL1  "This program is Free Software published under the GNU LGPL,"
#define P_TITLE_GPL2  "see menu item Info/Version or sources of the main programs."
#define BOX_LINE     "##################################################################\n"
#define E_BOX_LINE   "##                                                              ##\n"
#define VERS_LINE1  "        "
#define VERS_LINE2  "  ##"

#ifdef MIT_LSV         /* ====================== HyGLEAM ======================= */
  #ifdef OPAL_V  
    #define PROG_TITLE1 "                 H y G L E A M / O P A L V                 "
    #define PROG_TITLE2 "                 =========================                 "
    #define PROG_TITLE4 "               Algorithm and Method / OPALV                "
    #define PROG_TITLE5 "HyGLEAM/A is an adaptive hybride optimization procedure    "
  #else /* no OPAL_V */
    #define PROG_TITLE1 "                     H y G L E A M / A                     "
    #define PROG_TITLE2 "                     =================                     "
    #define PROG_TITLE4 "              Algorithm and Method / Adaptive              "
    #define PROG_TITLE5 "HyGLEAM is an adaptive hybride optimization procedure      "
  #endif /* no OPAL_V */
/*                       12345678901234567890123456789.9876543210987654321 */
  #define PROG_TITLE3 "            Hybrid GeneraL purpose Evolutionary            "
  #define PROG_TITLE6 "consisting of the global optimization algorithm GLEAM of   "
  #define PROG_TITLE7 "Blume and several local search procedures (hill climbers). "
  #define PROG_TITLE8 P_TITLE_GL1
  #define PROG_TITLE9 P_TITLE_GL2
  #define PROG_TITLEA P_TITLE_GPL1 
  #define PROG_TITLEB P_TITLE_GPL2
  #define PROG_TITLEC "    "AUTHOR_WJ
  #define PROG_TITLED "    "AUTHOR_CB
#else                  /* ======================= GLEAM ======================== */
  #define PROG_TITLE1 "                     G L E A M  /  A E                     "
  #define PROG_TITLE2 "                     =================                     "
  #define PROG_TITLE3 "    General Learning Evolutionary Algorithm and Method     "
  #define PROG_TITLE4 "                  Application Environment                  "
  #define PROG_TITLE5 P_TITLE_GL1
  #define PROG_TITLE6 P_TITLE_GL2
  #define PROG_TITLE7 "Applications implemented in this version: Collision-free   "
  #define PROG_TITLE8 "path planning for industrial robots and several mathema-   "
  #define PROG_TITLE9 "tical benchmark functions.                                 "
  #define PROG_TITLEA P_TITLE_GPL1
  #define PROG_TITLEB P_TITLE_GPL2
  #define PROG_TITLEC "    "AUTHOR_CB
  #define PROG_TITLED "    "AUTHOR_WJ
#endif  /* MIT_LSV */


