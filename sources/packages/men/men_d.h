/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Deutsch)
Package: men                     File: men_d.h               Version:     V1.7.0
Status : o.k.                   Autor: W.Jakob                 Datum: 29.09.2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
-------------------------------------------------------------------------------- */


/* ----------------------- Texte mehrerer Module: -------------------------- */
#define DYN_GENES_1_TXT    "Genmodell basierend auf Gentypen mit unbegrenzer Anzahl an Integer-"
#define DYN_GENES_2_TXT    "oder Realparametern."

/* ----------------------- Texte des Moduls: f_load ------------------------ */
#define GLEAM_ROOT_OK_TXT  "  Die Initialisierungsdateien werden auch im %s-Verzeichnis gesucht: \"%s\""
#define GLEAM_ROOT_ERR_TXT "  Die Umgebungsvariable %s enth"ae"lt kein g"ue"ltiges Verzeichnis: \"%s\""
#define NO_GLEAM_ROOT_TXT  "  Die Environmentvariable %s existiert nicht! Daher werden dort auch keine Initialisierungsdateien gesucht."
#define MOD_FILE_SPEC_TXT  "Genmodell"
#define BEW_FILE_SPEC_TXT  "Bewertung"
#define TSK_FILE_SPEC_TXT  "Programmparameter"
#define EVO_FILE_SPEC_TXT  "Evolutionsparameter"
#define MEM_FILE_SPEC_TXT  "Chromosomenspeicher"
#define TOP_FILE_SPEC_TXT  "Populationstopologie"
#define SYM_RING_USED      "Symmetr. Ring genommen."
#define TERMINAT_TXT       "Abbruch!"


/* ----------------------- Texte des Moduls: men_gsw ------------------------ */
#define EXP_FILE_HDR_TXT   "Experimentbeschreibung"
#define EXP_FILE_LINE      "-----------------------"
#define EXP_STD_FILE_TXT   "-------- Liste der Standardfiles: --------"
#define EXP_APPL_FILE_TXT  " Liste der anwendungsspezifischen Files: "
#define ERLEDIGT_TXT       "Erledigt!"
#define FKT_UNVOLLST       "Funktion wegen Fehler eventuell unvollst"ae"ndig ausgef"ue"hrt!"

#define AKTIONS_TXT        "Aktions"
#define G_KLASSEN_HDR1     "%sketten-Anzahl pro G"ue"teklasse"
#define G_KLASSEN_HDR2     "G"ue"teklasse    Notenbereich   Anzahl"
#define G_KLASS_BAKS       "Davon sind %1d Basisaktionsketten."
#define G_KLASS_FINTXT     "%1d %sketten."
#define BAKS_HDR1          "                     %2d Basisaktionsketten:"
#define BAKS_HDR2          "                     (Notenwert:G"ue"te/LfdNr)"
#define AK_LEN_HDR1        "                  AK-L"ae"ngen:"
#define AK_LEN_HDR2        "G"ue"te     Noten-      AK-      Kettenl"ae"nge"
#define AK_LEN_HDR3        "         bereich     Anz    min   max  Schn"
#define LADE_STAT_HDR      "  Geladene Initialisierungsdateien und Statusinformationen:"
#define LSTAT_POS_BAD_DATA "Daten m"oe"gleicherweise inkonsistent wegen Fehler in Datei!"
#define LSTAT_NO_EXP       "Kein Experiment geladen."
#define LSTAT_EXP_TXT      "Experiment      : %s"
#define LSTAT_HMOD_TXT     "Handlungsmodell : %s"
#define LSTAT_GMOD_TXT     "Genmodell       : %s"
#define LSTAT_APPL_TXT     "Akt. Anwendung  : %s"
#define LSTAT_ESIM_MOD     "ExtSimu-Modell  : \"%s\""
#define LSTAT_ESIMPFAD     "SimuModell-Pfad : \"%s%s%s\""
#define LSTAT_EXT_SIM      "Simulator       : Externer Simulator: \"%s\""
#define LSTAT_EXT_SIM_SERV "Simulator       : URL der externen Dienste: \"%s\""
#define LSTAT_INT_SIM      "Simulator       : Interner Simulator"
#define LSTAT_MBF_SIM      "Simulator       : Interne Benchmark-Funktion%s: \"%s\""
#define LSTAT_BEW_TXT      "Bewertung       : %s"
#define LSTAT_MBEW_TXT     "Bewertung ("ae"nd) : %s"
#define LSTAT_TSK_TXT      "Prog.Parameter  : %s"
#define LSTAT_MTSK_TXT     "Prog.Param ("ae"nd): %s"
#define LSTAT_EVO_TXT      "Evo-Parameter   : %s"
#define LSTAT_CHF_TXT      "Ketten-Datei    : %s"
#define LSTAT_NO_CHF       "Keine Ketten-Datei geladen."
#define LSTAT_TOP_TXT      "Topologie (Par) : %s"

#define AK_SCHON_BAK       "AK ist bereits Basis-AK."
#define DEL_BAK_ERG1       "Alle %1d BAKs in AKs umgewandelt."
#define DEL_BAK_ERG2       "Davon konnten %1d gel"oe"scht werden."
#define BAKS2AKS_OKTXT     "Alle %d BAKs in AKs umgewandelt."
#define BAKS2AKS_F_TXT     "Von %d BAKs konnten nur %d umgewandelt werden!"
#define GEN_AK_OK_TXT      "AK unter %s gespeichert."
#define GEN_AK_F1_TXT      "Fehler beim Speichern der AK!"
#define GEN_AK_F2_TXT      "AK-Generierung fehlgeschlagen!"
#define AK_TXT             "AK"
#define ANZG_JOBS_TXT      "    %2u abgearbeitete %s-OptJobs mit:"
#define ANZG_POPUL_TXT     "Populationen        : %6d"
#define ANZG_GEN_TXT       "Generationen        : %6ld"
#define ANZG_OFFSPRTXT     "Bewertete Offsprings: %6ld"
#define ANZG_NOTE_TXT      "Note der besten %s  : %8.1f  G"ue"te: %2u"
#define ANZG_ERGAK_TXT     "Anz. Ergebnisketten : %6d"
#define ANZG_ZEIT_TXT      "Verbrauchte GesZeit : %s"
#define AK_ANZG_TXT        "AK %2d/%-3d: Note =%8.1lf"
#define NIX_AK_TXT         "Eine AK %d/%d gibt es nicht!"
#define N_KLASSEN_TXT      "Verteilung in der %d.Notenklasse (%ld .. %ld):"
#define MZ_ANZG1_TXT       "%4d ..%5d: %2d"
#define MZ_ANZG2_TXT       "Bereich f"ue"r AK-L"ae"nge "MZ_ANZG1_TXT
#define MZ_ANZG3_TXT       "                     "MZ_ANZG1_TXT
#define SYM_RING_TXT       "symmetrischer Ring"


/* ----------------------- Texte des Moduls:g_cmen.c  ----------------------- */
#define DO_IT_YESQUERY "  Durchf"ue"hren? "JA_NEIN_TXT
#define DO_IT_NO_QUERY "  Durchf"ue"hren? "NEIN_JA_TXT
#define FIN_FRAG_TXT   "  Wirklich beenden? "NEIN_JA_TXT

#define LOAD_MEN_ITEM   7
#define SAVE_MEN_ITEM   8
#define SYS_MEN_ITEM    9
#define FIN_MEN_ITEM   12

#define INFO_MEN_TXT   "Info"
#define LOAD_MEN_TXT   "Laden"
#define SAVE_MEN_TXT   "Sichern"
#define SYS_MEN_TXT    "System"
#define FIN_MEN_TXT    "Abbruch"
#define AK_MEM_MEN_TXT "AK-Mem"
#define AK_GLO_MEN_TXT "AK/G Glob"
#define AK_INFOMEN_TXT "AK/G Info"
#define AK_LEN_MEN_TXT "AK-L"ae"ngen "
#define OPTERG_MEN_TXT "Opt-Ergebn"
#define LADSTATMEN_TXT "LadeStatus"
#define BAKS_MEN_TXT   "Basis-AKs"
#define AK_AUSGMEN_TXT "AK-Ausgabe"
#define VERS_MEN_TXT   "Vers-Info"
#define BAD_DATA_FTXT  "Funktionen wegen inkonsistenter Init-Dateien gesperrt!"
#define LAST_EXP_FAIL  "Fehler beim Laden des letzten Experiments! Daten jetzt konsistent?"

#define AK_GLOB_HDR1   "               Ketten einer G"ue"teklasse ("UE"bersicht):"
#define AK_GLOB_HDR2   "        "UE"bersicht "ue"ber die Ketten der G"ue"teklasse %1d:"
#define AK_GLOB_HDR3   "                     (Notenwert:LfdNr)"
#define GUETE_QUERY    "G"ue"teklasse (1..%1d): "
#define AK_INFO_HDR1   "  Ketten einer Guteklasse (Detail)"
#define AK_INFO_HDR2   "   Daten der Ketten der G"ue"teklasse %1d:"
#ifdef WITH_BACS
  #define AK_INFO_HDR3   "  Adr    Fitness   Len   Segm   BAK   Sim"
#else /* no WITH BACS */
  #define AK_INFO_HDR3   "  Adr    Fitness   Len   Segm   Sim"
#endif /* no WITH BACS */
#define AK_AUSG_HDR    "                          Kettenausgabe:"
#define OPT_ERG_HDR1   "Ergebnisse der bisher ausgef"ue"hrten Jobs:"
#define OPT_ERG_HDR2   "            "
#define AK_ERG_QUERY   "AK-Ergebnisse darstellen? "JA_NEIN_TXT
#define AKERGCLR_QUERY "AK-OptErgebnisse zur"ue"cksetzen? "NEIN_JA_TXT
#define CHIOMODE_QUERY "Standard-Ketten-File (binary) "JA_NEIN_TXT

#define IO_EXP_MEN     "Experiment"
#define LOAD_HMOD_MEN  "Handl.Mod "
#define LOAD_GMOD_MEN  "Genmodell "
#define IO_BEW_MEN     "Bewertung "
#define IO_TSK_MEN     "ProgParams"
#define LOAD_EVO_MEN   "EvoParams "
#define IO_MEM_MEN     "Chr.Memory"
#define IO_AKS_MEN     "Chromosome"
#define LOAD_TOP_MEN   "Topologie "
#define LOAD_EXP_HDR1  "                        Experiment laden:"
#define LOAD_EXP_HDR2  "Experiment \"%s\":"
#define LOAD_EXP_QUERY "Ungesicherte Evo-Ergebnisse. Trotzdem neues Experiment laden? "NEIN_JA_TXT
#define LOAD_EXPOK_TXT "Experiment"LOAD_OK_TXT
#define LOAD_EXP_F_TXT "Fehler beim Laden des letzten Experiments!"
#define LOAD_MIT_ANZGQ "Geladene Ketten anzeigen? "NEIN_JA_TXT
#define LOAD_FILE_OK   "\"%s\""LOAD_OK_TXT
#define LOAD_EXP_MDEL  "AK-Mem wegen Wechsel der Anwendung gel"oe"scht."
#define LOAD_MOD_HDR   "                      Handlungsmodell laden:"
#define LOAD_BEW_HDR   "                         Bewertung laden:"
#define LOAD_TSK_HDR   "                     Programmparameter laden:"
#define LOAD_EVO_HDR   "                       Evo-Parameter laden:"
#define LOAD_MEM_HDR   "                      Kettenspeicher laden:"
#define LOAD_AKS_HDR   "                    Aktionsketten-File laden:"
#define LOAD_APPLF_HDR "                     %s laden:"
#define LOAD_MEM_CLR   "Zuvor mu"szt" der Kettenspeicher gel"oe"scht werden!"
#define LOAD_AFILE_OK  "Datei"LOAD_OK_TXT

#define SAVE_BAKS_MEN  "Basis-AKs"
#define SAVE_EXP_HDR1  "                       Experiment sichern:"
#define SAVE_BEW_HDR   "                        Bewertung sichern:"
#define SAVE_TSK_HDR   "                   Programmparameter sichern:"
#define SAVE_MEM_HDR   "                     Kettenspeicher sichern:"
#define SAVE_AKS_HDR   "                      Aktionsketten sichern:"
#define SAVE_BAKS_HDR  "                   Basis-Aktionsketten sichern:"
#define SAVE_APPLF_HDR "                    %s sichern:"
#define SAV_XBEW_TXT   "Bewertung m"oe"glicherweise ver"ae"ndert."
#define SAV_XBEW_QUERY "Entspricht die Bewertung der in File \"%s\"? "NEIN_JA_TXT
#define SAV_XBEWQUERY2 "Entspricht die Bewertung der in File \"%s\"? "JA_NEIN_TXT
#define SAV_XTSK_TXT   "Programmparameter m"oe"glicherweise ver"ae"ndert."
#define SAV_XTSK_QUERY "Entsprechen die Programmparameter denen in File \"%s\"? "NEIN_JA_TXT
#define SAV_XTSKQUERY2 "Entsprechen die Programmparameter denen in File \"%s\"? "JA_NEIN_TXT
#define SAV_APPL_QUERY "%s neu schreiben? "JA_NEIN_TXT
#define SAV_NIX_RCODE  "Robotercodegenerierung im Simulator!" 
#define SAV_MEM_QUERY  "Kettenspeicher retten? "JA_NEIN_TXT
#define SAVE_INFO1_TXT "Nachfolgend k"oe"nnen zu rettende AKs markiert werden, wobei gilt:"
#define SAVE_INFO2_TXT "G"ue"te   = 0: Dialogende und Rettung aller zuvor markierten AKs."
#define SAVE_INFO3_TXT "Lfd-Nr = 0: Markierung aller AKs der angegebenen G"ue"te."
#define SAVE_INFO4_TXT "ESC       : Abbruch, Markierungen l"oe"schen und nichts retten."
#define SAVE_AK_QUERY  "G"ue"te und Lfd-Nr der zu rettenden AK: "
#define AK_MARKED_TXT  "AK %s zur Rettung markiert."
#define AKS_MARKED_TXT "Alle AKs der G"ue"te %d zur Rettung markiert."
#define SF_BAD_SPEC    "Kein Zugriff auf \"%s\"!"
#define SAV_EXP_HDR    "Rettung des Experiments in \"%s\":"
#define SAV_EXP_ERR    "Es trat ein Fehler auf! EXP-File nicht angelegt."
#define SAV_EXP_ERR2   "Es trat ein Fehler beim Schreiben des EXP-Files auf!"
#define SAV_EXP_TXT1   "Experiment"
#define SAV_EXP_TXT2   "vom"
#define SAV_NOT_SAVED  "Datei wird nicht gerettet!"

#define SYS_MINFO_MEN  "MemInfo"
#define SYS_CHEDI_MEN  "K-Editor"
#define SYS_DELMEM_MEN "L"oe".AK-Mem "
#define SYS_DEL_G_MEN  "L"oe"AKs/G"ue"te"
#define SYS_DELBAKSMEN "L"oe"AlleBAKs"
#define SYS_DEL_AK_MEN "L"oe"sche AK "
#define SYS_AK2BAK_MEN "AK  -> BAK"
#define SYS_AKG2BAKMEN "AK/G-> BAK"
#define SYS_BAK2AK_MEN "BAK -> AK"
#define SYS_BAKS2AKMEN "BAKs-> AKs"
#define SYS_AK_GEN_MEN "Erzeuge AK"
#define SYS_AKTEST_MEN "Teste AK"
#define SYS_GENREP_MEN "Gen.Repair"
#define SYS_HAM_MEN    "HamDist"
#define SYS_ZEIG_MUTZ  "MutOp-Vert"
#define IGN_FATAL_MEN  "IgnorFatal"
#define SYS_GOPTST_MEN "GenOp-Test"
#define DELETING_TXT   "Es wird gel"oe"scht  ..."
#define DEL_AKMEM_HDR  "              L"oe"schung des gesamten Kettenspeichers:"
#define DEL_GCLASS_HDR "              L"oe"schung aller Ketten einer G"ue"teklasse:"
#define DEL_BAKS_HDR   "            L"oe"schung aller Basis-Aktionsketten (BAKs):"
#define DEL_AK_HDR     "                   L"oe"schung einer Aktionskette:"
#define AK2BAK_HDR     "                          AK -> Basis-AK:"
#define AKG2BAK_HDR    "                Alle AKs einer G"ue"te -> Basis-AKs:"
#define BAK2AK_HDR     "                          Basis-AK -> AK:"
#define BAKS2AKS_HDR   "                         Basis-AKs -> AKs:"
#define AK_GEN_HDR     "                           AK-Erzeugung:"
#define AK_TEST_HDR    "                    AK-Datenkonsistenzpr"ue"fung:"
#define GEN_REPAIR_HDR "                          Genetic Repair:"
#define HAM_DIST_HDR   "                 Berechnung des Hamming-Abstands:"
#define IGN_FATAL_HDR  "                   R"ue"cksetzen des Fehlerstatus:"
#define NO_BAK_TXT     "Es gibt keine Basis-AKs!"
#define XFER_ERG_TXT   "Alle %d Ketten der G"ue"te %d in Basis-AKs umgewandelt."
#define DEL_BAK_QUERY  "Basis-AK l"oe"schen? "NEIN_JA_TXT
#define DEL_AK_DONETXT "Kette gel"oe"scht!"
#define DEL_AK_NIX_TXT "Kette nicht gel"oe"scht!"
#define AK_OK_TEXT     "AK-Daten (L"ae"nge und Segmentstruktur) fehlerfrei."
#define GENREP_NIX_TXT "AK irreparabel!"
#define HAM_DIST_TXT   "Der Hammingabstand der beiden AKs betr"ae"gt: %6.4f %%"
#define HAM_DIST_TXT2  "Der Hammingabstand der beiden AKs betr"ae"gt: %.*g %%"
#define IGN_FATAL_TXT1 "Fehlerstatus zur"ue"ckgesetzt. Vorsicht! Fehlersituation unver"ae"ndert!"
#define IGN_FATAL_TXT2 "Ein Aufruf von \"mem_info\" setzt den Fehlerstatus wieder!"
#define MZ_ANZG_HDR1   "          Verteilungsfunktion eines Mutationsoperators"
#define MZ_ANZG_HDR2   "               Verteilung f"ue"r Mutationsoperator %d:"
#define MZ_ANZG_QUERY1 "Index des Mutations-Operators (0..%d): "
#define MZ_ANZG_QUERY2 "Anzahl der angezeigten AK-L"ae"ngenbereiche (4..30) [%d]:"
#define CHR_LOE_ERR    "Fehler bei der L"oe"schung der Chromosomen im Speicher und/oder der Population!"
#define OP_TEST_HDR    "                 Test f"ue"r genetische Operatoren:"
#define OP_IDX_QUEWRY  "Index des gen. Operators [-%d..%d]: "
#define FITNESS_TXT    "Noten: Elter-AK: %g  Nachkomme: %g"
#define CHILD_STORED_1 "Nachkomme unter"
#define CHILD_STORED_2 " gespeichert."
#define CHILD_NOT_SIM  "Nachkomme nicht simulierbar!"
#define NO_MUT_DONE    "Nachkomme wurde nicht mutiert! Gl"oe"scht."
#define NO_CHILD_TXT   "Kein Nachkomme erzeugt!"
#define PARENT_FITN    "Elter-Noten : Elter1: %g  Elter2: %g"
#define OFFSPRING_FITN "Kinder-Noten: Kind1 : %g  Kind2 : %g"
#define NO_CHILDREN    "Keine  Nachkommen erzeugt!"
#define FIRST_TXT      "1."
#define SECOND_TXT     "2."
