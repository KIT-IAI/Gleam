/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Deutsch)
Package: HP                           File: vers_d.h                   Version:     V1.8.0
Status : o.k.                        Autor: W.Jakob                      Datum: 19.08.2016
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------------- */

/* ---------------------------------- Versions Info ----------------------------------- */
#define APPL_TXT_1       "In dieser Version implementierte Anwendungen:"
#define APPL_TXT_2       "  LESAK   Kollisionsfreie Bahnplanung f"ue"r Industrieroboter (Blume)"
#define APPL_TXT_3       "  MBF     Mathematische Benchmarkfunktionen (Jakob)"
#define APPL_TXT_4       "  OPALV   Produktionsplanung in der Verfahrenstechnik (Blume, Jakob)"
#define AUTHOR_CB        "Prof. Dr. Christian Blume (HS K"oe"ln, Campus Gummersbach)"
#define AUTHOR_WJ        "Dr. Wilfried Jakob (KIT, Campus Nord, IAI)             "

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
  #define VERS_INFO_6    "Adaptives hybrides Optimierungsverfahren bestehend aus dem globalen "
  #define VERS_INFO_7    "Optimierungsalgorithmus GLEAM und mehreren lokalen Suchverfahren, "
  #define VERS_INFO_8    "darunter der Rosenbrock-Algorithmus und das COMPLEX-Verfahren von Box. "
  #define VERS_INFO_9    "GLEAM ist ein eigenst"ae"ndiger Evolution"ae"rer Algorithmus, der Elemente "
  #define VERS_INFO_10   "der Informatik, der Evolutionsstrategie von Rechenberg und Schwefel "
  #define VERS_INFO_11   "und der Genetischen Algorithmen von Holland in sich vereint. "
  #define VERS_INFO_12   "Neben den Basisalgorithmen GLEAM, Rosenbrock und Complex sind folgende"
  #define VERS_INFO_13   "Memetischen Algorithmen (MA) implementiert: "
  #define VERS_INFO_14   "  SMA:  Simple Memetic Algorithm: GLEAM + ein LSV mit fixen Parametern "
  #define VERS_INFO_15   "  ASMA: Adaptive Single Memetic Alg.: GLEAM + ein LSV mit Parameteradaption "
  #define VERS_INFO_16   "  AMMA: Adaptive Multi-Memetic Alg.: GLEAM + LSVs, Parameter- u. Auswahladapt."
  #define VERS_INFO_21   "                    "AUTHOR_WJ
  #define VERS_INFO_22   "                    "AUTHOR_CB
#else /* kein HyGLEAM */
  #define VERS_INFO_1    "                           G L E A M / A E"
  #define VERS_INFO_2    "                           ==============="
  #define VERS_INFO_3    "         General Learning Evolutionary Algorithm and Method"
  #define VERS_INFO_4    ""
  #define VERS_INFO_5    "GLEAM ist ein eigenst"ae"ndiger Evolution"ae"rer Algorithmus, der Elemente"
  #define VERS_INFO_6    "der Informatik (Konzept der abstrakten Datentypen), der Evolutions-"
  #define VERS_INFO_7    "strategie von Rechenberg und Schwefel und der Genetischen Algorithmen "
  #define VERS_INFO_8    "von Holland in sich vereint. "
  #define VERS_INFO_9    "Einige Charakteristika von GLEAM:"
  #define VERS_INFO_10   "  - Anwendungsorientierte Konfiguration der Chromosomen basierend auf"
  #define VERS_INFO_11   "    Gentypen und dazu passende genetische Operatoren"
  #define VERS_INFO_12   "  - Zusammenfassung von Genen zu Segmenten und deren Evolution"
  #define VERS_INFO_13   "  - Strukturierte Population nach dem Diffusionsmodell basierend auf "
  #define VERS_INFO_14   "    einer Ringnachbarschaft"
  #define VERS_INFO_15   "  - Bewertung durch die kaskadierte gewichtete Summe und Straffunktionen"
  #define VERS_INFO_16   ""
  #define VERS_INFO_17   APPL_TXT_1
  #define VERS_INFO_18   APPL_TXT_2
  #define VERS_INFO_19   APPL_TXT_3
  #define VERS_INFO_20   "                   "AUTHOR_CB
  #define VERS_INFO_21   "                   "AUTHOR_WJ
  #define VERS_INFO_22   ""
#endif  /* kein HyGLEAM */
#define VERS_INFO_CBL  "Das Copyright f"ue"r GLEAM liegt bei Prof. Dr. Christian Blume."
#define VERS_INFO_CWJ1 "Das Copyright f"ue"r HyGLEAM/A, einem auf GLEAM beruhenden Werk, liegt bei "
#define VERS_INFO_CWJ2 "Dr. Wilfried Jakob, Karlsruher Institut f"ue"r Technologie (KIT)."

#define VERS_INFO_H1   "Die in HyGLEAM/A enthaltenen lokalen Suchverfahren von Rosenbrock und "
#define VERS_INFO_H2   "Box wurden an der Universit"ae"t Bremen, ITEM, Prof. R.Laur, implementiert "
#define VERS_INFO_H3   "und in HyGLEAM/A als Ergebnis eines gemeinsamen Forschungsprojekts "
#define VERS_INFO_H4   "integriert."
#define VERS_INFO_L1   "GLEAM und HyGLEAM/A sind Freie Software und unterliegen der GNU LGPL v3, "
#define VERS_INFO_L2   "siehe www.gnu.org. Die Lizenz ist in den Quellen der Hauptprogramme und "
#define VERS_INFO_L3   "den \"schalter.h\"-Dateien enthalten. Die Ver"oe"ffentlichung dieser Program-"
#define VERS_INFO_L4   "me erfolgt OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie "
#define VERS_INFO_L5   "der MARKTREIFE oder der VERWENDBARKEIT F"UE"R EINEN BESTIMMTEN ZWECK."


/* -------------------------------- Start Header Texts -------------------------------- */
#define P_TITLE_GL1   "GLEAM ist ein eigenst"ae"ndiger EA, der Elemente d.Informatik,"
#define P_TITLE_GL2   "der ES von Rechenberg und der GAs von Holland vereint.     "
#define P_TITLE_GPL1  "Dieses Programm ist unter der GNU LGPL ver"oe"ffentlichte     "
#define P_TITLE_GPL2  "Freie Software, siehe Men"ue"punkt Info/Version u. HP-Quellen."
#define BOX_LINE     "##################################################################\n"
#define E_BOX_LINE   "##                                                              ##\n"
#define VERS_LINE1  "        "
#define VERS_LINE2  "  ##"

#ifdef MIT_LSV         /* ====================== HyGLEAM ======================= */
  #ifdef OPAL_V  
    #define PROG_TITLE1 "                 H y G L E A M / O P A L V                 "
    #define PROG_TITLE2 "                 =========================                 "
    #define PROG_TITLE4 "               Algorithm and Method / OPALV                "
    #define PROG_TITLE5 "HyGLEAM ist ein adaptives hybrides Optimierungsverfahren   "
  #else /* no OPAL_V */
    #define PROG_TITLE1 "                     H y G L E A M / A                     "
    #define PROG_TITLE2 "                     =================                     "
    #define PROG_TITLE4 "              Algorithm and Method / Adaptive              "
    #define PROG_TITLE5 "HyGLEAM/A ist ein adaptives hybrides Optimierungsverfahren "
  #endif /* no OPAL_V */
  #define PROG_TITLE3 "            Hybrid GeneraL purpose Evolutionary            "
  #define PROG_TITLE6 "bestehend aus dem globalen Optimierungsalgorithmus GLEAM   "
  #define PROG_TITLE7 "von Blume und mehreren lokalen Suchverfahren.              "
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
  #define PROG_TITLE7 "In dieser Version implementierte Anwendungen: Kollisions-  "
  #define PROG_TITLE8 "freie Roboterbahnplanung und verschiedene mathematische    "
  #define PROG_TITLE9 "Testfunktionen.                                            "
  #define PROG_TITLEA P_TITLE_GPL1
  #define PROG_TITLEB P_TITLE_GPL2
  #define PROG_TITLEC "    "AUTHOR_CB
  #define PROG_TITLED "    "AUTHOR_WJ
#endif  /* MIT_LSV */
