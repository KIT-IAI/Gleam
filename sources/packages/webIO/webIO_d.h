/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Sprach-Header-File (Deutsch)  
Package: webIO                        File: webIO_d.h                  Version:     V1.0.0
Status : o.k.                        Autor: W.Jakob                      Datum: 22.10.2018
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------ */


/* ---------------------------- Texts of Several Modules:  ---------------------------- */
#define TASK_ID_TXT          "Task-ID"
#define WORKFLOW_ID_TXT      "Workflow-ID"
#define MODEL_CHECK_TXT      "Modellpr"ue"fung"
#define MODEL_LIST_TXT       "Modellliste"
#define MODEL_START_TXT      "Modellstart"
#define TASK_STOP_TXT        "zur Task-Beendigung"
#define JOB_START_TXT        "SimuJob-Start"
#define JOB_STOP_TXT         "zum Abbruch des aktellen Jobs"
#define FAILED_TXT           "fehlgeschlagen"
#define GET_REQUEST_TXT      "\nGet-Request %s %s: URL=\"%s\""
#define GET_REQ_RESULT_TXT   "\nGet-Request %s %s: gesendet=%d URL=\"%s\""
#define POST_REQ_RESULT_TXT  "\nPost-Request %s %s: gesendet=%d URL=\"%s\""
#define RESLI_INTRPTED_TXT   "Unterbrechung des Wartens auf eine Ergebnisliste nach %d Wartezyklen (Max: %d)."
#define RESLI_RECEIVED_TXT   "Ergebnisliste mit %d Bytes nach %d Wartezyklen empfangen (Max: %d)."
#define RESLI_TMO_TXT        "Keine g"ue"ltige Antwort beim Warten auf eine Ergebnislsite erhalten. Timeout nach %d Zyklen (%d Sekunden)."
#define START_OF_BUF_TXT     "Pufferanfang:"
#define END_OF_BUF_TXT       "Ende des Pufferanfangs"
#define DATA_RECEIVED_TXT    "Empfangene Daten eines get-Requests %s %s:"
#define POST_DATA_REC_TXT    "Empfangene Daten eines post-Requests %s %s:"
#define TO_OBTAIN_A_TXT      "zum Erhalt einer"
#define FOR_A_TXT            "f"ue"r eine(n)"
#define DATA_MOD_START_TXT   "Daten eines post-Requests f"ue"r %s:"
#define TSK_STOP_DONE_TXT    "Task %d erfolgreich gestoppt."
#define TSK_STOP_ERROR_TXT   "Task %d trotz Fehler gestoppt."
#define TSK_STOP_ERROR2_TXT  "Task %d mit Fehler gestoppt: %s"
#define TSK_STOP_BAD_REPLY   "Unzul"ae"ssige Antwort beim Stoppen der Task %d erhalten. Code: %d"
#define ABORT_JOB_DONE_TXT   "Aktueller Job von Task %d und WFID %d erfolgreich abgebrochen."
#define ABORT_JOB_ERR_TXT    "Abbruch des aktuellen Jobs von Task %d und WFID %d fehlgeschlagen. Fehlercode: %d"
#define WRONG_TASK_ID_TXT    "falsche Task-ID (impliziert falsche Workflow-ID)"
#define WRONG_WFID_TXT       "falsche Workflow-ID"
#define IDS_OK_GEN_ERR_TXT   "Fehler bei korrekten IDs"
#define NO_PROOF_AVAIL_TXT   "PROOF-Dienste nicht verf"ue"gbar!"
#define CHRLI_RENAME_ERR_TXT "  *** Fehler bei der Umbenennung der Chromosomenlistendatei nach \"%s\": %s" 
#define CHRLI_ERASE_ERR_TXT  "  *** Fehler beim L"oe"schen der Chromosomenlistendatei \"%s\": %s"


/* ---------------------------- Texts of Module: listIO ------------------------------- */
#define CHR_NUMBER_TXT       "Chromosomenanzahl im Puffer"
#define CHR_LIST_TXT         "Chromosomenliste"
#define CHROMOSOMES_TXT      "Chromosomen"
#define GEN_LIST_TXT         "Genliste"
#define RES_NUMBER_TXT       "Anzahl der Ergebniswerte"
#define RES_DATA_TXT         "Ergebnisdatensatz"
#define BAD_BUF_HDR_TXT      "------- Puffer mit Fehlern in %s: -------"
#define RES_LIST_HDR_TXT     "Ergebnislistenkopf"
#define BAD_CHR_HDR          "eines fehlerhaften Chromosomenkopfs"
#define BAD_GENE_TXT         "fehlerhafter Gendaten"
#define BAD_EMPTY_LIST_TXT   "Leere Liste (Terminierungsliste)"
#define EXT_SIMU_ERR_MSG     "*** Externer Simulationsdienst liefert Fehler! Message:"
#define CHR_LIST_SENT_RES    "\nChromosomenliste mit %d Chromosomen gesendet. Ergebnis (Bool): %d"
#define RESLI_REC_RESULT     "Ergebnisliste mit %d Eintr"ae"gen erhalten. Ergebnis (Bool): %d"
#define URL_BEGIN_TXT        "URL-Anfang f"ue"r http-Requests: \"%s\""
#define GENE_CODE_TXT        "Gencode"
#define GENE_INT_PARAM_TXT   "ganzzahhliger Genparameter"
#define GENE_REAL_PARAM_TXT  "reellwertiger Genparameter"
#define FAILED_SIMU_RUNS_TXT "Simulatorfehll"ae"ufe"


/* ---------------------------- Texts of Module: curlIO ------------------------------- */
#define POST_HANDLE_TXT      "Handle f"ue"r POST requests"
#define GET_HANDLE_TXT       "Handle f"ue"r GET requests"
#define HDR_LIST_ATTR_TXT    "Liste der Header Attribute"
#define NEWLY_STARTED_TXT    "mit dem aktuellen Modell gestartet"
#define INVALID_REPLY_TXT    "ung"ue"ltige Antwort"
#define SECOND_FAILURE_TXT   "zweiter Sendeversuch gescheitert"
#define NO_RESPONSE_TXT      "keine Antwort"
#define CURL_SETUP_RES_TXT   "Kommunikationspaket \"libcurl\" initialisiert. Ergebnis (bool): %d"
#define CURL_FIN_RES_TXT     "Kommunikationspaket \"libcurl\" beendet.\n"
#define RES_SERVER_NAME_TXT  "  Server-URL f"ue"r Ergebnislisten: \"%s\""
