/* ---------------------------------------------------------------------------------------
GLEAM/AE                       Language Header File (English)  
Package: webIO                        File: webIO_d.h                  Version:     V1.0.0
Status : o.k.                        Autor: W.Jakob                       Date: 10/22/2018
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------ */


/* ---------------------------- Texts of Several Modules:  ---------------------------- */
#define TASK_ID_TXT          "task ID"
#define WORKFLOW_ID_TXT      "workflow ID"
#define MODEL_CHECK_TXT      "model check"
#define MODEL_LIST_TXT       "model list"
#define MODEL_START_TXT      "model start"
#define TASK_STOP_TXT        "for task stopping"
#define JOB_START_TXT        "SimuJob start"
#define JOB_STOP_TXT         "for aborting the current job"
#define FAILED_TXT           "failed"
#define GET_REQUEST_TXT      "\nGet request %s %s: URL=\"%s\""
#define GET_REQ_RESULT_TXT   "\nGet request %s %s: sent=%d URL=\"%s\""
#define POST_REQ_RESULT_TXT  "\nPost request %s %s: sent=%d URL=\"%s\""
#define RESLI_INTRPTED_TXT   "Waiting for result list interrupted after %d wait cycles (max: %d)."
#define RESLI_RECEIVED_TXT   "Result list of %d bytes received after %d wait cycles (max: %d)."
#define RESLI_TMO_TXT        "No valid response when waiting for the results. Time out after %d cycles (%d seconds)."
#define START_OF_BUF_TXT     "Start of buffer beginning:"
#define END_OF_BUF_TXT       "End of buffer beginning"
#define DATA_RECEIVED_TXT    "Data received from a get request %s %s:"
#define POST_DATA_REC_TXT    "Data received from a post request %s %s:"
#define TO_OBTAIN_A_TXT      "to obtain a"
#define FOR_A_TXT            "for a"
#define DATA_MOD_START_TXT   "Data for a post request of %s:"
#define TSK_STOP_DONE_TXT    "Task %d successfully stopped."
#define TSK_STOP_ERROR_TXT   "Task %d stopped with errors."
#define TSK_STOP_ERROR2_TXT  "Task %d stopped with error: %s"
#define TSK_STOP_BAD_REPLY   "Bad reply received when trying to stop task %d: %d"
#define ABORT_JOB_DONE_TXT   "Current job of task %d and WFID %d succesfully aborted."
#define ABORT_JOB_ERR_TXT    "Error aborting current job of task %d and WFID %d. Error code: %d"
#define WRONG_TASK_ID_TXT    "wrong task ID (implies wrong work flow ID)"
#define WRONG_WFID_TXT       "wrong work flow ID"
#define IDS_OK_GEN_ERR_TXT   "error with correct IDs"
#define NO_PROOF_AVAIL_TXT   "PROOF services not available!"
#define CHRLI_RENAME_ERR_TXT "  *** Error renaming chromosome list file to \"%s\": %s"
#define CHRLI_ERASE_ERR_TXT  "  *** Error deleting chromosome list file \"%s\": %s"


/* ---------------------------- Texts of Module: listIO ------------------------------- */
#define CHR_NUMBER_TXT       "number of chromosomes in buffer"
#define CHR_LIST_TXT         "chromosome list"
#define CHROMOSOMES_TXT      "chromosomes"
#define GEN_LIST_TXT         "gene list"
#define RES_NUMBER_TXT       "number of result values"
#define RES_DATA_TXT         "result list entry"
#define BAD_BUF_HDR_TXT      "---------- Buffer of bad %s: ----------"
#define RES_LIST_HDR_TXT     "result list header"
#define BAD_CHR_HDR          "chromosome header"
#define BAD_GENE_TXT         "gene data"
#define BAD_EMPTY_LIST_TXT   "Empty list (termination list)"
#define EXT_SIMU_ERR_MSG     "*** External simulation service reports error! Message:"
#define CHR_LIST_SENT_RES    "\nChromosome list containing %d chromosomes sent. Result (bool): %d"
#define RESLI_REC_RESULT     "Result list containig %d entries received. Result (bool): %d"
#define URL_BEGIN_TXT        "URL start for http requests: \"%s\""
#define ESS_GENE_CODE_TXT    "Gene code"
#define GENE_INT_PARAM_TXT   "integer gene parameter"
#define GENE_REAL_PARAM_TXT  "real-valued gene parameter"
#define FAILED_SIMU_RUNS_TXT "failed simulator runs"


/* ---------------------------- Texts of Module: curlIO ------------------------------- */
#define POST_HANDLE_TXT      "handle for POST requests"
#define GET_HANDLE_TXT       "handle for GET requests"
#define HDR_LIST_ATTR_TXT    "list of header attributes"
#define NEWLY_STARTED_TXT    "started with present model"
#define INVALID_REPLY_TXT    "invalid response"
#define SECOND_FAILURE_TXT   "second transmission attempt failed"
#define NO_RESPONSE_TXT      "no response"
#define CURL_SETUP_RES_TXT   "IO connection based on libcurl initialized. Result (bool): %d"
#define CURL_FIN_RES_TXT     "IO connection based on libcurl terminated.\n"
#define RES_SERVER_NAME_TXT  "  Server URL for result lists: \"%s\""
