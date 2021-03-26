/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Deutsch)
Package: HP                    File: gleam_eng2_d.h          Version:     V1.0.5
Status : o.k.                 Autor: W.Jakob                   Datum: 08.02.2019
--------------------------------------------------------------------------------
Dies ist freie Software, die der Lesser GNU Public License (LGPL) Version 3, 
unterliegt. Weitere Einzelheiten sind im Kommentar-Abschnitt am Ende der Haupt-
programmdateien oder in der "schalter.h" -Datei zu finden.
--------------------------------------------------------------------------------

Abkuerzungen:
   popReq       : obligatorischn fuer populationsbasierte Optimierungsverfahren 
   opt          : optional
   def          : Defaultwert
   noOpt        : dazu gibt es keine korrespondierende Programmaufrufoption 
   reqAccOptProc: ja nach Optimierungsverfahren oblogatorische Angabe
   HyGLEAM      : nur relevant im HyGLEAM context, d.h. dass das Programm mit
                  gesetztem MIT_LSV-Scgalter uebersetzt wurde.
Regeln:
   1. Programm Optionen ueberschreiben korrespondierende TSK-File-Parameter.
   2. Optionen oder Parameters, die fuer das ausgewaehlte Optimierungsverfahren
      nicht erforderlich sind, werden ignoriert.
------------------------------------------------------------------------------- */


/* ------------------------ Texte des Hauptprogramms: ----------------------- */
#define CALL_MAIN_TXT    "Aufruf:"
#define CALL_EXP_TXT     "     EXP_FILE    : Dateiname der Experimentbeschreibung (mit Pfad). Listet alle beteiligten Dateien auf.\n"
#define CALL_LOG_TXT     "     LOG_FILE    : Logfilename (mit Pfad). Bei fehlender Angabe wird der Name der Experimentdatei genommen.\n"
#define CALL_OPT_TXT     "\n  Alle Aufrufparameter (Optionen) "ue"berschreiben eventuelle Angaben im TSK-File.\n"
#define CALL_PAR_A_TXT   "    -A<text>     : Akzeptanzregel der Nachkommen: always, localLeast, betterParent, always-ES, localLeast-ES\n"
#define CALL_PAR_B_TXT   "    -B(+|-)      : +: best-Verbesserung,  -: all-Verbesserung\n"
#define CALL_PAR_C_TXT   "    -C(+|-)      : Mit/Ohne Erzeugung eines case-Files bei einer Ergebnissimulation. Nur f"ue"r MATPOWER-Anwendungen!\n"
#define CALL_PAR_D_TXT   "    -D<demeSize> : Nachbarschaftssgr"oe"sse, muss gerade und kleiner als popSize sein. (%d - %d)\n"
#define CALL_PAR_E_TXT   "    -E<anz>      : Obergrenze f"ue"r Funktionsevaluationen (Default: Abbruchvorgabe abgeschaltet)\n"
#define CALL_PAR_F_TXT   "    -F<fitness>  : Fitness- oder Notenlimit (Abbruchvorgabe) (0.0 - %3.1f)\n"
#define CALL_PAR_G_TXT   "    -G<gen>      : Generationslimit (Abbruchvorgabe)\n"
#define CALL_PAR_H_TXT   "    -h -H --help : Online-Hilfe (dieser Text)\n"
#define CALL_PAR_Ig_TXT  "    -I<text>     : Initialisierungsstrategie: neu, best, mix, bestNeu, file\n"
#define CALL_PAR_Ih_TXT  "    -I<text>     : Initialisierungsstrategie: neu, best, mix, bestNeu, lsvNeu, lsvBest, file\n"
#define CALL_PAR_ig_TXT  "    -i<wert>     : Initialisierungsparameter: mix:Mindestfitness,  bestNeu:BestAnzahl\n"
#define CALL_PAR_ih_TXT  "    -i<wert>     : Initialisierungsparameter: mix:Mindestfitness,  bestNeu,lsvBest:BestAnzahl,  lsvNeu:Anzahl LSV-Optimierter\n"
#define CALL_PAR_L_TXT   "    -L(+|-)      : Lamarcksche Evolution: +:an,  -:aus (Baldwin Evolution)\n"
#define CALL_PAR_l_TXT   "    -l<frac>     : Lernrate: Anteil der alten LSV-Verteilung an der neuen in "PROZ" (0.0 - 100.0)\n"
#define CALL_PAR_M_TXT   "    -M<wert>     : Maximale Iterationen je LSV-Lauf (LSV-Abbruchschranke)\n"
#define CALL_PAR_N_TXT   "    -N<text>     : Domain-Name der \"external Simulation Services\" (ESS)\n"
#define CALL_PAR_n_TXT   "    -n<islandNo> : 0: Initializer, >0: Island-Nnummer, f"ue"r die PAR_POP Programmvarianten\n"
#define CALL_PAR_Og_TXT  "    -O<text>     : Optimierungsverfahren: gleam\n"
#define CALL_PAR_Oh_TXT  "    -O<text>     : Optimierungsverfahren: gleam, sma, asma, amma, lsv\n"
#define CALL_PAR_P_TXT   "    -P<popSize>  : Populationsgr"oe"sse (4 - %d)\n"
#define CALL_PAR_p_TXT   "    -p(+|-)      : Protokollierung: +:detailliert,  -:minimal\n"
#define CALL_PAR_R_TXT   "    -R<wert>     : Ranking Parameter, von minimalen zu maximalem Selektionsdruck (1.0 - 2.0)\n"
#define CALL_PAR_r_TXT   "    -r<anz>      : Resultanzahl. Default: 1,  Max: popSize/2,  0: keine Ergebnissimulation oder -rettung m"oe"glich.\n"
#define CALL_PAR_S_TXT   "    -S<index>    : Searcher: LSV-Index f"ue"r LSV jobs oder GLEAM, SMA, ASMA oder AMMA jobs mit LSV-Initialisierung (lsvNeu oder lsvBest)\n"
#define CALL_PAR_T_TXT   "    -T<wert>     : Zeitlimit: maximale Joblaufzeit in Sekunden (Abbruchvorgabe)\n"
#define CALL_PAR_t_TXT   "    -t<wert>     : Threshold des LSVs (optionale 2.LSV-Abbruchschranke)\n" 
#define CALL_PAR_W_TXT   "    -W<nr>:<val> : Neues Gewicht des Kriteriums <nr> ausgedr"ue"ckt als maximale Fitness <val>.\n"
#define CALL_PAR_X_TXT   "    -X           : Reserviert f"ue"r undokumentierte experimentelle Einstellungen.\n"
#define AUFRUF_DOKU          "Aufrufparameter: "
#define IMPROPER_TXT         "Unzul"ae"ssiges"
#define PSIZE_TXT            "PopGr"oe""szt"e"
#define DEME_SIZE2_TXT       "DemeGr"oe""szt"e"
#define NO_EXT_TERM_MLDG     "Externer Abbruch einer LSV-Optimierung nicht m"oe"glich!"
#define BAD_MA_SW_TXT        "Optionen f"ue"r memetische Evolution ignoriert."
#define BAD_LSV_SW_TXT       "LSV-Optionen unzul"ae"ssig."
#define REDUZ_TXT            " *** Hinweis: Zuviel Ergebnis-AKs angefordert. Von %d auf %d reduziert."
#define MIN_HAM_XO_TXT       "MinHamDistXO"
#define ZEIT_TXT             "Zeit"
#define GEN_TXT              "Generations"
#define RES_ANZ_TXT          "Result-Anzahl"
#define NO_SAVE_NO_SIM       " *** Hinweis: Ohne Ergebnischromosome ist keine Ergebnisrettung oder -simulation m"oe"glich!"
#define NO_SIM_NO_CASE_FILE  " *** Hinweis: Ohne Ergebnissimulation ist keine case-Filegenerierung m"oe"glich!"
#define LSV_ADA_SPEED_TXT    "LSV-Adaptionsgeschwindigkeit"
#define LEV_ADA_SPEED_TXT    "Level-Adaptionsgeschwindigkeit"
#define OLD_FRAC_TXT         "Anteil der alten LSV-Verteilung"
#define FITNESS_TXT          "Fitness"
#define UNKNOWN_TXT          "Unbekanntes"
#define NO_SIMU_LOG_TXT      " *** Hinweis: Version ohne SimuLogFileIO. Daher kann keines angelegt werden."
#define CEC_BEW_DATA_ERR     "\n *** Fehler: Unzul"ae"ssige Bewertungsdaten f"ue"r CEC-MBFs!"
#define RESOURCES_TXT        "Verbrauchte Gesamtzeit: %ld:%02ld. Gesamtevaluationen: %ld."
#define NO_GENERATIONS       " Keine Generationen!"
#define NO_TIME_ELAPSED      " Kein Zeitverbrauch!"
#define NO_JOB_TIME_GEN      " *** Hinweis: Keine Angaben zu Laufzeit und Generationsanzahl m"oe"glich. (\"#t\"-Zeile)"
#define ERG_AK_TXT           "Ergebnis: %d ErgAKs. Beste ist %d-te: %d/%d"
#define NOTE_TXT             "%s, Note: %.9f\n"
#define NOTEN_DIFF_TXT       "\n *** Fehler: Ergebnissimulation ergab andere Note!!!\n"
#define SIMU_ERG_TXT         "Simulation result: fitness= %.9f"
#define NO_ACCESS_TXT        "%s: Kein Zugriff, kein Ergebnis!\n"
#define NICHT_ALLE_TXT       " *** Hinweis: Es konnten nicht alle Ergebnis-AKs f"ue"r die Sicherung markiert werden!"
#define RES_FILE_TXT         "_erg.aks"
#define NO_SAVE_TXT          " *** Hinweis: Es konnte keine Ergebnis-AK fuer die Sicherung markiert "\
                             "werden! Kein Sicherungsfile angelegt."
#define OK_FIN_MLDG_TXT      "Optimierungslauf erfolgreich beendet.\n"
#define BAD_FIN_MLDG_TXT     "Optimierungslauf mit Fehler abgebrochen (siehe Logfile \"%s\").\n\n"
#define INIT_SPAR_CORR_MSG   "Parameter der Initialisierungsstrategie \"neu\" von %d auf %d angepasst!"
#define WEIGHTS_ADJUSTED_TXT "\nFitnessgewichte entsprechend der Aufrufparameter erfolgreich angepasst."

#ifdef PAR_POP_VERS
  #define MBF_DELAY_TXT      "Verz"oe"gerung pro Ausf"ue"hrung einer MBF: %d msec"
#endif /* PAR_POP_VERS */


#ifdef ESS_EVAL_SLAVE 
  #define PROG_NAME         "ESS_EVAL_HP"
#else /* no ESS_EVAL_SLAVE */
  #define PROG_NAME         "CLV_HP"
#endif /* ESS_EVAL_SLAVE */

/* --------------------- Texte des ESS-Evaluation-Hauptprogramms: --------------------- */
#define ESS_CALL_TXT        "[OPTIONS] <domain-or-ip> <simu-model>\n"
#define ESS_CALL_DOMAIN_TXT "      <domain-or-ip>: Domain-Name oder IP-Adresse des Dienstes, mit dem kommuniziert werden soll.\n"
#define ESS_CALL_SIMMOD_TXT "      <simu-model>  : Simulationsmodell. Wird normalerweise vom Simulator oder seiner Schnittstelle ben"oe"tigt.\n"
#define ESS_OPTIONS_TXT     "\n    Optionen:\n"
#define ESS_CALL_M1_TXT     "      -M<mod-file>  : Genmodell-Datei zur Erstellung von Name-Wert-Paaren. Ist die Option nicht angegeben,\n"
#define ESS_CALL_M2_TXT     "                      wird die Standarddatei \"%s\" von Anwendungen verwendet, die ein \n"
#define ESS_CALL_M3_TXT     "                      Genmodell ben"oe"tigen. Je nach Anwendung bricht das Programm ab, wenn die Modelldatei \n"
#define ESS_CALL_M4_TXT     "                      nicht ge"oe"ffnet werden kann.\n"
#define ESS_CALL_T1_TXT     "      -T<simu-tmo>  : Zeitlimit in Sekunden f"ue"r einen Simulatorlauf. Es h"ae"ngt von der Anwendung ab, \n"
#define ESS_CALL_T2_TXT     "                      ob die Simulationsaufrufe mit Timeout-Steuerung erfolgen. Default: 1 Sek.\n"
#define ESS_CALL_L1_TXT     "      -L(+|-)       : Steuert den Detailierungsgrad des Loggings. Eine Logdatei wird immer im Arbeitsver-\n"
#define ESS_CALL_L2_TXT     "                      zeichnis angelegt, wenn sie nicht existiert, und kann leer bleiben. Ihr Name leitet \n"
#define ESS_CALL_L3_TXT     "                      sich aus dem Namen des Simulationsmodells durch Hinzuf"ue"gen von \".log\" ab. Wird kein \n"
#define ESS_CALL_L4_TXT     "                      Modellname angegeben, wird stattdessen der Programmname verwendet. Logfiles werden im\n"
#define ESS_CALL_L5_TXT     "                      Append-Modus geschrieben. Kein Schalter angegeben: Standard-Logging. Andernfalls:\n"
#define ESS_CALL_L6_TXT     "                        -: Solange kein Fehler auftritt, wird keine Protokollierung durchgef"ue"hrt.\n"
#define ESS_CALL_L7_TXT     "                        +: Detailiertes Logging\n"
#define ESS_CALL_H_TXT      "      -H --help     : Online Hilfe (dieser Text)\n"
#define ESS_CALL_FIN_TXT    "    Die Schalterbuchstaben k"oe"nnen auch klein geschrieben werden.\n\n"
#define ESS_TERMINATED_TXT  "*** %s beendet.\n"
#define ESS_FIN_SEE_LOG_TXT "*** %s wegen %s abgebrochen! Siehe \"%s\".\n"
#define ESS_MISSING_PAR_TXT "*** %s wegen fehlenden Programmparameter(n) abgebrochen!"
#define ESS_BAD_CALL_TXT    "fehlerhaften Aufrufparametern"
#define ESS_ERRORS_TXT      "Fehler"
#define ESS_NO_PARAM_TXT    "Fehlende(r) Programmparameter!\n"
#define ESS_BAD_CHR_HDR_TXT "Fehlerhafte Kopfdaten"
#define ESS_BAD_GENE_TXT    "Fehlerhafte Gendaten"
#define ESS_LIST_HEADER_TXT "Listenkopf"
#define ESS_LIST_ENTRY_TXT  "Listenelement"
#define ESS_LIST_DONE_TXT   "Liste mit %2d Chromosomen %sbearbeitet. Zeit: %.6f sec"
#define ESS_NO_ERR_TXT      "fehlerfrei "
#define ESS_WITH_ERR_TXT    "mit Fehlern "
#define ESS_CHR_NUMBER_TXT  "Beim %d.Chromosom: "
#define ESS_CHRLIST_ERR_TXT "Fehler in der Chromosomenliste! "
#define ESS_SIMU_ERR_TXT    "%d fehlerhafte Simulationen! "
#define ESS_LIST_FIN_TXT    "Bearbeitung der Liste abgebrochen!"
