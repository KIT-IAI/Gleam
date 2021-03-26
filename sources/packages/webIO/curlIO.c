/*****************************************************************************************
GLEAM/AE                                                                            V1.1.0

                                    G L E A M  /  A E
       General Learning Evolutionary Algorithm and Method / Application Environment

                        Package: WEB-based IO for external simulators
                         Module: libCURL based IO
 
Package: webIO                                                              File: curlIO.c
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
------------------------------------------------------------------------------------------

This module uses libcurl and contains a TCP/IP-based communication to the simulation 
service, which itself provides access to external simulators. The communication is based
on HTTP get and post requests.
As chromosome lists may be long the http ppst operation may last for some seconds. E.g.,
for sending 2 MB data between 9 and 10 seconds are required.

An alternative implementation to this module is "fileIO.c", which is based on File-IO and 
which serves for test purposes only. 


Exported routines in detail:
============================

void finalizeCom (void);
        Termination of the IO connection, release of buffers, and "sim_ok" is set to FALSE.


Exported routines for GLEAM side:
=================================

BOOL waitForResultList (INT *waitCtr);
        Checks cyclically up to an amount given by the TSK-File in seconds, if a response 
        of the optimization service has been received. If so, the received data is stored 
        in "resBuf" and TRUE is returned. Otherwise FALSE is returned.
        In GLEAM_USR mode the process of receiving the results can be interrupted by the 
        user. The test on a keyboard input is performed before the get request. Thus, the 
        process of receiving can be interrupted indepentently from wait cycles. The 
        "waitCtr" parameter supports the time out handling for the wait: The local counter 
        is initialized with this value and the amount of already waited cycles is returned 
        in case of an interruption. 
        If MITSIMULOG is set, the at maximum MAX_STRING first characters of the "resBuf" 
        are written to the "simu_logfile".

void deleteOldResList (void);
        Does nothing in the context of tcp/ip-based result list communication.

BOOL getTaskID (void);
        An attempt is made to receive a new task ID from the simulation services. If 
        successful, the ID is stored in "taskID" and TRUE is returned. Otherwise or if 
        the ID is negative, "taskID" is set to NIX_IDX and FALSE is returned. Errors and
        negative IDs are reported on GLEAM_ERROR level.

BOOL checkModel (BOOL       *modelState,
                 const char *modelName);
        The existence of the simulation model denominated by "modelName" is checked and 
        the result is returned in "modelState". TRUE is returned after a successful 
        communication and FALSE otherwise. Thus, the function can also be used to check 
        the availability of the simulation services.  Errors are reported on GLEAM_ERROR 
        level.

INT getModelList (BOOL  withNumberedItems);
        The list of models known to the external simulation services is requested. In 
        case of a communication error or an empty list (service error), 0 is returned 
        and an appropriate error message is generated. Otherwise, the number of models 
        is returned and their names are written line by line to the internal text buffer. 
        If "withNumberedItems" is set, each line is prefixed with a serial number.

BOOL startModel (const char *modelName);
        The simulation model denominated by "modelName" is started and a workflow ID is
        obtained from the simulation services and stored in "modelWFID". On sucess, TRUE 
        is returned and the url variables are set: "urlComBuf", "jobUrlBuf", "resultUrlBuf", 
        and "clResultUrlBuf". As the model name MUST have been ckecked in advance, only 
        communication errors can occur or the service is not able to start the workflow 
        associated with the model name properly. The latter is indicated by a negative ID. 
        On error, "modelWFID" is set to NIX_IDX and FALSE is returned. Errors are reported 
        on GLEAM_ERROR level.

BOOL stopTask (void);
        The current work flow and task is stopped. "taskID" and "modelWFID" are set to 
        NIX_IDX. If a communication error is detected, it is reported and FALSE is returned.

void abortJobOfWorkflow (void);
        The actual job of the workflow denoted by "urlComBuf" is aborted. In case of a 
        communication error or a negative return value an appropriate message is generated 
        and "sim_up" is set to FALSE.


Exported routines for ESS-EvalFrame side:
=========================================

BOOL prepComToServer (BOOL  logServerName);
        The URL contained in "serverBuffer" is associated with the "posHandle". If
        "logServerName" is set, the URL is logged. The function returns TRUE on success 
        and FALSE otherwise. In case of an error an appropriate fatal error message is 
        generated including the curl message.

BOOL sendResListToServer (void);
        Writes the result list contained in the io buffer "postBuf" to the file denoted 
        by DATA_FROM_SIM for testing purposes. Returns TRUE on success and FALSE otherwise.
        I/O errors are reported to the logfile.


Package-global routines in detail:
==================================

BOOL sendChromosomeList (BOOL  secondTry);
        This function may call itself once recursively and must be called otherwise with 
        unset "secondTry" parameter. It sends the contents of the chromosome list buffer 
        "chrLiBuf" to the optimization service. In case of an error response an appropriate 
        message is generated and an attempt is made for loading the correct model by a 
        call to simu_hochfahren() including a call to terminate_ext_simu(). If this was
        successful, a further sending is performed by a recursive call with "secondTry" 
        set to TRUE. If the send operation fails again, an error message is generated, 
        "sim_up" is set to FALSE and "taskID" to NIX_IDX, and no further sending attempt 
        is made. In all cases of failure FALSE is returned and TRUE otherwise.

BOOL setupCom (void);
        Creates the buffers and prepares a libcurl based IO connection and returns TRUE on 
        success.

void ioInit (void);
        Modulinitialization.


------------------------------------------------------------------------------------------
Changes with respect to version 1.0.0:
 1. The call back function writeMemoryCallback(), the interface of which must 
    not be changed, calls the new function writeLocalBuffer(), which does all 
    the buffer management stuff.                                               WJ 26.04.18
------------------------------------------------------------------------------------------
Status: Impl                          Autor: W.Jakob                     Datum: 22.10.2018
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <curl/curl.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "simu.h"
#include "webIO.h"
#include "webIOInt.h"

#include WEBIO_TEXTS
#include GLOBAL_TEXTS


#undef  DBG_WAIT_MODE
#undef  CURL_DBG_MODE




/* ================================= Lokale Konstante: ================================ */
#define MAX_BAD_CHARS    20      /* maximum displayed amount of chars of bad int number */



/* ============================= Lokale Typdeklarationen: ============================= */



/* ================================= Lokale Variable: ================================= */
static struct curl_slist *hdrList;  /* for content type specifications                  */
static STRING_TYPE        urlBuf,   /* for temporary URLs different from "urlComBuf"    */
                jobUrlBuf,      /* URL for extSimServices jobs like SimuJob or LSVJob   */
                resultUrlBuf,   /* URL for obtaining the result list(s) of the act. job */
                clResultUrlBuf; /* URL for obtaining the res.chromos. of the act.LSVJob */

static CURL  *postHandle,           /* curl handle for post requests                    */
             *getHandle;            /* curl handle for get requests                     */
static BOOL   curlSetupDone;        /* TRUE, if curl_global_init performed successfully */



/* ================================= Lokale Routinen: ================================= */
size_t writeMemoryCallback (void   *contents, 
                            size_t  size, 
                            size_t  nmemb, 
                            void   *userp);

static BOOL noIntErrMsg (const char *inBuf,
                         INT         origin);

#ifdef MITSIMULOG
static void logResponse (const char *titleBuf,
                         const char *inBuf1,
                         const char *inBuf2);
#endif /* MITSIMULOG */




/* ================================ writeMemoryCallback =============================== */
size_t writeMemoryCallback (void   *contents, 
                            size_t  size, 
                            size_t  nmemb, 
                            void   *userp)
/*----------------------------------------------------------------------------------------
   The calling interface of this function must not be changed! It calls the slightly 
   simpler function writeLocalBuffer(), which is also used by the higher i/o-functions of 
   the package.  
----------------------------------------------------------------------------------------*/
{
   return (writeLocalBuffer(contents, size * nmemb, userp));
} /* WriteMemoryCallback */



/* ==================================== finalizeCom =================================== */
void finalizeCom (void)
{
   BOOL  res;

   /* finalizeCom */
#ifdef CURL_DBG_MODE
   printf("finalizeCom: Start curlSetupDone=%d\n", curlSetupDone); fflush(stdout); 
#endif /* CURL_DBG_MODE */
   if (resBuf.memory != NULL)
      free(resBuf.memory);
   if (postBuf.memory != NULL)
      free(postBuf.memory);
   if (chrLiBuf.memory != NULL)
      free(chrLiBuf.memory);
   if (curlSetupDone) 
   { /* at minimum some curl actions done */
#ifdef CURL_DBG_MODE
      printf("finalizeCom: CURL shutdown! curlSetupDone=%d\n", curlSetupDone); fflush(stdout);
#endif /* CURL_DBG_MODE */
      if (getHandle != NULL)
	 curl_easy_cleanup(getHandle);
      if (postHandle != NULL)
	 curl_easy_cleanup(postHandle);
      if (hdrList != NULL)
	 curl_slist_free_all(hdrList); 
      curl_global_cleanup();
      curlSetupDone = FALSE;
   } /* at minimum some curl actions done */
   sim_ok = FALSE;
#ifdef CURL_DBG_MODE
   printf("finalizeCom: Ende\n"); fflush(stdout); 
#endif /* CURL_DBG_MODE */
#ifdef MITSIMULOG
   write_to_simulog(CURL_FIN_RES_TXT, CLOSE_LOG);
#endif /* MITSIMULOG */
} /* finalizeCOM */



/* ================================= waitForResultList ================================ */
BOOL waitForResultList (INT *waitCtr)
{
   INT       ctr;                                             /* counts the wait cycles */
   BOOL      replyReceived;
   CURLcode  curlRes;

   /* waitForResultList */
   replyReceived = FALSE;
   if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, resultUrlBuf)) != CURLE_OK)
      fatal(CURL_IO, 1, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR);
   else
   { /* handle successfully prepared */
#ifdef MITSIMULOG
      sprintf(logBuf, GET_REQUEST_TXT, TO_OBTAIN_A_TXT, RES_LIST_TXT, resultUrlBuf);
      write_to_simulog(logBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
      warte(WAIT_FOR_READ);                                             /* initial wait */
      ctr  = 0;
      while((ctr < simuResultWaitCycles) && !replyReceived)
      { /* main cycle */
	 resetLocalBuffer(&resBuf);
#ifdef GLEAM_USR
	 taste = kb_hit();                               /* check for interrupt by user */
#endif /* GLEAM_USR */
	 if (taste) 
         { /* abandon wait loop and return amount of already done wait cycles */
	    *waitCtr = ctr;                                    /* performed wait cycles */
	    break;                                                    /* aborts waiting */
	 } /* abandon wait loop and return amount of already done wait cycles */
	 else
         { /* check for reply */
	    curlRes = curl_easy_perform(getHandle);
	    if (curlRes == CURLE_OK) 
            { /* something received */
	       if (resBuf.usedSize < 3)            /* blank line: data is not ready yet */
               { /* blank line */
#ifdef DBG_WAIT_MODE
		  printf("waitForResultList: Noch keine Daten verfuegbar: Warten! Zyklus %d/%d\n", 
			 ctr, simuResultWaitCycles); fflush(stdout);
#endif /* DBG_WAIT_MODE */
		  warte(WAIT_FOR_READ);
		  ctr++;
	       } /* blank line */
	       else
		  replyReceived = TRUE;
	    } /* something received */
	    else /* nothing received: this is not a valid option */
	       meldung(CURL_IO, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_GET_ERR);
	 } /* check for reply */
      } /* main cycle */
#ifdef MITSIMULOG
      if (taste)
	 sprintf(logBuf, RESLI_INTRPTED_TXT, *waitCtr, simuResultWaitCycles);
      else
	 if (replyReceived) 
	    sprintf(logBuf, RESLI_RECEIVED_TXT, (INT)resBuf.usedSize, ctr, simuResultWaitCycles);
	 else
	    sprintf(logBuf, RESLI_TMO_TXT, ctr, (INT)((simuResultWaitCycles * WAIT_FOR_READ) / 1000));
      write_to_simulog(logBuf, CLOSE_LOG);
  #ifdef DBG_WAIT_MODE
      printf("%s\n", logBuf);  fflush(stdout);
  #endif /* DBG_WAIT_MODE */
      if (replyReceived) 
      { /* log start of resBuf */
	 sprintf(logBuf, "%s %s %s", STRICH_40_TXT, START_OF_BUF_TXT, STRICH_40_TXT);
         write_to_simulog(logBuf, KEEP_LOG_OPEN);
	 strncpy(logBuf, resBuf.errStartPos, MAX_STR);
	 logBuf[MAX_STR] = '\0';                 /* logging of MAX_STR chars at maximum */
	 write_to_simulog(logBuf, KEEP_LOG_OPEN);
	 sprintf(logBuf, "%s %s %s\n", STRICH_40_TXT, END_OF_BUF_TXT, STRICH_40_TXT);
	 write_to_simulog(logBuf, CLOSE_LOG);
      } /* log start of resBuf */
#endif /* MITSIMULOG */
   } /* handle successfully prepared */
   return (replyReceived);
} /* waitForResultList */



/* ================================= deleteOldResList ================================= */
void deleteOldResList (void)
{
} /* deleteOldResList */



/* =================================== noIntErrMsg ==================================== */
static BOOL noIntErrMsg (const char *inBuf,
                         INT         origin)
/*----------------------------------------------------------------------------------------
   The first MAX_BAD_CHARS characters of "inBuf" are used to create an error message of
   level GLEAM_ERROR reporting a string, which did not contain a valid integer. The global
   variable "err_state" is set accordingly. FALSE is returned always.
----------------------------------------------------------------------------------------*/
{
   if (strlen(inBuf) > MAX_BAD_CHARS) {
      strncpy(logBuf, inBuf, MAX_BAD_CHARS);
      logBuf[MAX_BAD_CHARS] = '\0';
   }
   else
      strcpy(logBuf, inBuf);
   err_state |= fehler(CURL_IO, origin, NO_INFO, NO_INFO, logBuf, NO_INT_FOUND);
   return(FALSE);
} /* noIntErrMsg */



#ifdef MITSIMULOG
/* =================================== logResponse ==================================== */
static void logResponse (const char *titleBuf,
                         const char *inBuf1,
                         const char *inBuf2)
/*----------------------------------------------------------------------------------------
   A line build from "titleBuf" containing the two other buffers is written to the 
   "simu_logfile", followed by the contents of "resBuf". The output is terminated by a 
   limiting line.
----------------------------------------------------------------------------------------*/
{
   sprintf(logBuf, titleBuf, inBuf1, inBuf2);
   write_to_simulog(logBuf, KEEP_LOG_OPEN);
   write_to_simulog(resBuf.memory, KEEP_LOG_OPEN);
   write_to_simulog(STRICH_40_TXT, CLOSE_LOG);
} /* logResponse */
#endif /* MITSIMULOG */



/* ===================================== getTaskID ==================================== */
BOOL getTaskID (void)
{
   CURLcode  curlRes;
   BOOL      res;

   /* getTaskID */
   resetLocalBuffer(&resBuf);
   strcpy(urlBuf, serverBuffer);
   strcat(urlBuf, GET_TASK_ID_CMD);
   if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, urlBuf)) != CURLE_OK)
      res = (fatal(CURL_IO, 10, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
   else
   { /* handle successfully prepared */
      curlRes = curl_easy_perform(getHandle);
      res     = (curlRes == CURLE_OK);
#ifdef MITSIMULOG
      sprintf(logBuf, GET_REQ_RESULT_TXT, TO_OBTAIN_A_TXT, TASK_ID_TXT, res, urlBuf);
      write_to_simulog(logBuf, CLOSE_LOG);
      if (res) 
	 logResponse(DATA_RECEIVED_TXT, TO_OBTAIN_A_TXT, TASK_ID_TXT);
#endif /* MITSIMULOG */
      if (res) 
      { /* something received */
	 if (readIntVal(&taskID, resBuf.memory, &(resBuf.nextPos)))
	 { /* id successfully read */
	    res = taskID >= 0;
	    if (res)
	       strcpy(lastSimServDomainName, extSimServDomainName);
	    else
	       fehler(CURL_IO, 11, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR); 
	 } /* id successfully read */
	 else 
	    res = noIntErrMsg(resBuf.memory, 18);
      } /* something received */
      else /* io error */
	 res = (fehler(CURL_IO, 19, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_GET_ERR) == GLEAM_OK);
   } /* handle successfully prepared */
   if (!res)
      taskID = NIX_IDX;
   return (res);
} /* getTaskID */ 



/* ==================================== checkModel ==================================== */
BOOL checkModel (BOOL       *modelState,
                 const char *modelName)
{
   CURLcode  curlRes;
   BOOL      res;

   /* checkModel */
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      sprintf(urlBuf, "%s/%d/%s%s", serverBuffer, taskID, modelName, GET_MODEL_STATE_CMD);
      if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, urlBuf)) != CURLE_OK)
	 res = (fatal(CURL_IO, 20, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
      else
      { /* handle successfully prepared */
	 curlRes = curl_easy_perform(getHandle);
	 res     = (curlRes == CURLE_OK);
#ifdef MITSIMULOG
	 sprintf(logBuf, GET_REQ_RESULT_TXT, FOR_A_TXT, MODEL_CHECK_TXT, res, urlBuf);
	 write_to_simulog(logBuf, CLOSE_LOG);
	 if (res) 
	    logResponse(DATA_RECEIVED_TXT, FOR_A_TXT, MODEL_CHECK_TXT);
#endif /* MITSIMULOG */
	 if (res) 
         { /* something received */
	    res         = TRUE;
	    *modelState = FALSE;
	    if (strcmp(resBuf.memory, "yes") == 0)
	       *modelState = TRUE;
	    else
	       if (strcmp(resBuf.memory, "no") != 0)
		  res = FALSE;                              /* something weird recieved */
	 } /* something received */
	 else /* io error */
	    res = (fehler(CURL_IO, 28, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_GET_ERR) == GLEAM_OK);
      } /* handle successfully prepared */
   } /* taskID ok */
   else
      res = (fehler(CURL_IO, 29, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR) == GLEAM_OK);
   if (!res)
      *modelState = FALSE;
   return (res);
} /* checkModel */



/* =================================== getModelList =================================== */
INT getModelList (BOOL  withNumberedItems)
{
   STR_TYPE  listEntryBuf,     /* one list entry, i.e. model name                       */
             lineBuf;          /* line buffer for storing a model name entry            */
   CURLcode  curlRes;
   INT       numberOfModels,   /* number of abtained models, i.e. length of the list    */
             resVal,           /* result of reading operations from "resBuf"            */
             pos;              /* start position of the first/next string in "resBuf"   */
   char     *nextBufPos;       /* points to the position "pos" in input buffer "resBuf" */
   BOOL      res;              /* result of a get request and later of write operations */

   /* getModelList */
   numberOfModels = 0;
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      sprintf(urlBuf, "%s/%d%s", serverBuffer, taskID, GET_MODEL_LIST_CMD);
      if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, urlBuf)) != CURLE_OK)
	 err_state = fatal(CURL_IO, 30, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR);
      else
      { /* handle successfully prepared */
	 curlRes = curl_easy_perform(getHandle);
	 res     = (curlRes == CURLE_OK);
#ifdef MITSIMULOG
	 sprintf(logBuf, GET_REQ_RESULT_TXT, TO_OBTAIN_A_TXT, MODEL_LIST_TXT, res, urlBuf);
	 write_to_simulog(logBuf, CLOSE_LOG);
	 if (res) 
	    logResponse(DATA_RECEIVED_TXT, TO_OBTAIN_A_TXT, MODEL_LIST_TXT);
#endif /* MITSIMULOG */
	 if (res) 
         { /* something received */
	    if (resBuf.usedSize > 1)
            { /* buffer contains something */
	       pos        = 0;
	       nextBufPos = &(resBuf.memory[0]);
	       reset_text_buf();
	       do 
               { /* read one list entry */
		  resVal = sscanf(nextBufPos, "%[^\n]s", listEntryBuf);
		  if (resVal == 1)
		  { /* count item and calculate the next start position in "resBuf" */
		     numberOfModels++;
		     pos += strlen(listEntryBuf);
		     if (resBuf.memory[pos] == '\n')
			pos++;
		     nextBufPos = &(resBuf.memory[pos]);
		     if (withNumberedItems)
			sprintf(lineBuf, "  %2d: %s", numberOfModels, listEntryBuf);
		     else
			sprintf(lineBuf, "  %s", listEntryBuf);
		     res = write_buf(numberOfModels, lineBuf);  /*FALSE: buffer overflow*/
		  } /* count item and calculate the next start position in "resBuf" */
	       } /* read one list entry */
	       while ((resVal == 1) && res);
	    } /* buffer contains something */
	    else
	       meldung(CURL_IO, NO_INFO, NO_INFO, "", EMPTY_MODEL_LIST_ERR);
	 } /* something received */
	 else /* io error */
	    err_state |= fehler(CURL_IO, 38, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_GET_ERR);
      } /* handle successfully prepared */
   } /* taskID ok */
   else
      err_state |= fehler(CURL_IO, 39, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR);
   return (numberOfModels);
} /* getModelList */



/* ==================================== startModel ==================================== */
BOOL startModel (const char *modelName)
{
   STR_TYPE  buf;
   CURLcode  curlRes;
   BOOL      res;

   /* startModel */
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      resetLocalBuffer(&postBuf);
      sprintf(urlBuf, "%s/%d%s", serverBuffer, taskID, POST_MODEL_CONFIG_CMD);
      sprintf(buf, MOD_START_MOD_TXT, modelName);
      res = writeLocalBuffer(buf, strlen(buf), &postBuf) == strlen(buf);
      sprintf(buf, MOD_START_ALG_TXT, SIMU_JOB_TXT);
      if (res)
	 res = writeLocalBuffer(buf, strlen(buf), &postBuf) == strlen(buf);
      if (res)
	 curlRes = curl_easy_setopt(postHandle, CURLOPT_POSTFIELDS, postBuf.memory);
      if (curlRes == CURLE_OK) {
	 curlRes = curl_easy_setopt(postHandle, CURLOPT_URL, urlBuf);
	 if (curlRes != CURLE_OK) 
	    res = (fatal(CURL_IO, 40, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
      }
      else
	 res = (fatal(CURL_IO, 41, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
      if (res)
      { /* execute prepared post request */
	 curlRes = curl_easy_perform(postHandle);
	 res     = (curlRes == CURLE_OK);
#ifdef MITSIMULOG
	 sprintf(logBuf, POST_REQ_RESULT_TXT, FOR_A_TXT, MODEL_START_TXT, res, urlBuf);
	 write_to_simulog(logBuf, KEEP_LOG_OPEN);
	 sprintf(logBuf, DATA_MOD_START_TXT, MODEL_START_TXT);
	 write_to_simulog(logBuf, KEEP_LOG_OPEN);
	 write_to_simulog(postBuf.memory, KEEP_LOG_OPEN);
	 write_to_simulog(STRICH_40_TXT, CLOSE_LOG);
	 if (res) 
	    logResponse(POST_DATA_REC_TXT, FOR_A_TXT, MODEL_START_TXT);
#endif /* MITSIMULOG */
	 if (res) 
         { /* post request successfully done */
	    if (readIntVal(&modelWFID, resBuf.memory, &(resBuf.nextPos))) 
	    { /* id successfully read */
	       res = modelWFID >= 0;
	       if (!res)
		  fehler(CURL_IO, 42, modelWFID, NO_INFO, WORKFLOW_ID_TXT, INVALID_ID_ERR); 
	    } /* id successfully read */
	    else
	       res = noIntErrMsg(resBuf.memory, 47);
	 } /* post request successfully done */
	 else /* io error */
	    res = (fehler(CURL_IO, 48, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_POST_ERR) == GLEAM_OK);
      } /* execute prepared post request */
   } /* taskID ok */
   else
      res = (fehler(CURL_IO, 49, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR) == GLEAM_OK);
   if (res) 
   { /* create URLs for subsequent IO */
      sprintf(urlComBuf, "%s/%d/%d", serverBuffer, taskID, modelWFID);
      strcpy(jobUrlBuf, urlComBuf);           /* URL for jobs for the optimisation ser- */
      strcat(jobUrlBuf, POST_CHR_LIST_CMD);   /* vices. At present: SimuJob or LSVJob   */
      strcpy(resultUrlBuf, urlComBuf);               /* URL for obtaining the result    */
      strcat(resultUrlBuf, GET_RESULT_LIST_CMD);     /* list(s) of the actual job       */
      strcpy(clResultUrlBuf, urlComBuf);             /* URL for obtaining the resulting */
      strcat(clResultUrlBuf, GET_RESULT_CHR_CMD);    /* chromosome of the actual LSVJob */
   } /* create URLs for subsequent IO */
   else
      modelWFID = NIX_IDX;
   return (res);
} /* startModel */



/* ===================================== stopTask ===================================== */
BOOL stopTask (void)
{
   STR_TYPE  msgBuf;
   CURLcode  curlRes;
   INT       returnCode;
   BOOL      res;

   /* stopTask */
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      sprintf(urlBuf, "%s/%d%s", serverBuffer, taskID, GET_STOP_CMD);
      if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, urlBuf)) != CURLE_OK)
	 res = (fatal(CURL_IO, 50, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
      else
      { /* handle successfully prepared */
	 curlRes = curl_easy_perform(getHandle);
	 res     = (curlRes == CURLE_OK);
	 if (res) 
         { /* something received */
	    if (readIntVal(&returnCode, resBuf.memory, &(resBuf.nextPos)))
	    { /* returnCode successfully read */
	       if (returnCode == 1)
		  sprintf(msgBuf, TSK_STOP_DONE_TXT, taskID);
	       else
		  if (returnCode == -1)
		     sprintf(msgBuf, TSK_STOP_ERROR_TXT, taskID);
		  else
		     if (returnCode == -2) {
			sprintf(msgBuf, TSK_STOP_ERROR2_TXT, taskID, NO_PROOF_AVAIL_TXT);
			res = (fehler(CURL_IO, 51, NO_INFO, NO_INFO, msgBuf, GENERAL_MESSAGE) == GLEAM_OK);
		     }
		     else /* something weird recievded */
			sprintf(msgBuf, TSK_STOP_BAD_REPLY, taskID, returnCode);
#ifdef MITSIMULOG
	       write_to_simulog("", KEEP_LOG_OPEN);
	       write_to_simulog(msgBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
	       res    = TRUE;                /* the task related to "taskID" is terminated */
	       taskID = NIX_IDX;             /* regardless of the return value.            */
	       strcpy(urlComBuf, ""); /* no URL for I/O of chromosomes & results available */
	    } /* returnCode successfully read */
	    else 
            { /* some rubbish received */
	       res = noIntErrMsg(resBuf.memory, 57);
#ifdef MITSIMULOG
	       sprintf(logBuf, GET_REQ_RESULT_TXT, TASK_STOP_TXT, FAILED_TXT, res, urlBuf);
	       write_to_simulog(logBuf, KEEP_LOG_OPEN);
	       logResponse(DATA_RECEIVED_TXT, TASK_STOP_TXT, "");
#endif /* MITSIMULOG */
	    } /* some rubbish received */
	 } /* something received */
	 else /* io error */
	    res = (fehler(CURL_IO, 58, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_GET_ERR) == GLEAM_OK);
      } /* handle successfully prepared */
   } /* taskID ok */
   else
      res = (fehler(CURL_IO, 59, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR) == GLEAM_OK);
   return (res);
} /* stopTask */



/* ================================ abortJobOfWorkflow ================================ */
void abortJobOfWorkflow (void)
{
   CURLcode  curlRes;
   INT       returnCode;

   /* abortJobOfWorkflow */
   sim_up = FALSE;                    /* will be set to TRUE on successful job abortion */
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      sprintf(urlBuf, "%s%s", urlComBuf, GET_ABORT_CMD);
      if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, urlBuf)) != CURLE_OK)
	 fatal(CURL_IO, 60, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR);
      else
      { /* handle successfully prepared */
	 curlRes = curl_easy_perform(getHandle);
	 if (curlRes == CURLE_OK) 
         { /* something received */
	    if (readIntVal(&returnCode, resBuf.memory, &(resBuf.nextPos)))
	    { /* returnCode successfully read */
	       if (returnCode == 1)
		  sim_up = TRUE;                    /* current job aborted successfully */
	       else
		  generateErrMsg(CURL_IO, 61, returnCode, JOB_ABORTION_ERR);
	       if (sim_up)
		  sprintf(logBuf, ABORT_JOB_DONE_TXT, taskID, modelWFID);
	       else
		  sprintf(logBuf, ABORT_JOB_ERR_TXT, taskID, modelWFID, returnCode);
#ifdef GLEAM_USR
	       write_buf(get_line_anz()+1, logBuf);
#endif /* GLEAM_USR */
#ifdef MITSIMULOG
	       write_to_simulog("", KEEP_LOG_OPEN);
	       write_to_simulog(logBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
	    } /* returnCode successfully read */
	    else 
            { /* some rubbish received */
	       noIntErrMsg(resBuf.memory, 67);
#ifdef MITSIMULOG
	       sprintf(logBuf, GET_REQUEST_TXT, JOB_STOP_TXT, FAILED_TXT, urlBuf);
	       write_to_simulog(logBuf, KEEP_LOG_OPEN);
	       logResponse(DATA_RECEIVED_TXT, TASK_STOP_TXT, "");
#endif /* MITSIMULOG */
	    } /* some rubbish received */
	 } /* something received */
	 else /* io error */
	    fehler(CURL_IO, 68, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_GET_ERR);
      } /* handle successfully prepared */
   } /* taskID ok */
   else
      fehler(CURL_IO, 69, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR);
} /* abortJobOfWorkflow */



/* ================================== prepComToServer ================================= */
BOOL prepComToServer (BOOL  logServerName)
{
   BOOL      res;
   CURLcode  curlRes;

   /* prepComToServer */
   res = FALSE;
   if (logServerName) {
      sprintf(logBuf, RES_SERVER_NAME_TXT, serverBuffer);
      write_to_log(logBuf, CLOSE_LOG);
   }
   curlRes = curl_easy_setopt(postHandle, CURLOPT_URL, serverBuffer);
   if (curlRes == CURLE_OK) 
      res = TRUE;
   else
      fatal(CURL_IO, 70, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR);
   return (res);
} /* prepComToServer */



/* ================================ sendResListToServer =============================== */
BOOL sendResListToServer (void)
{
   STR_TYPE  buf;
   BOOL      res;
   CURLcode  curlRes;

   /* sendResListToServer */
   res = FALSE;
   resetLocalBuffer(&resBuf);
   curlRes = curl_easy_setopt(postHandle, CURLOPT_POSTFIELDS, postBuf.memory); 
   if (curlRes == CURLE_OK)
   { /* execute prepared post request */
      curlRes = curl_easy_perform(postHandle);
      if(curlRes == CURLE_OK)
      { /* post request successfully done */
	 res = TRUE;
	 if ((int)resBuf.usedSize > 0)
	 { /* unexpected reply received */
	    sprintf(buf, "------- %d bytes retrieved on post request. Data: --------", 
		    (int)resBuf.usedSize);
	    write_to_log(buf, KEEP_LOG_OPEN);
	    write_to_log(resBuf.memory, KEEP_LOG_OPEN);
	    write_to_log("-----------------------------------------------------\n",CLOSE_LOG);
	 } /* unexpected reply received */
      } /* post request successfully done */
      else
	 fehler(CURL_IO, 88, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_POST_ERR);
   } /* execute prepared post request */
   else
      fatal(CURL_IO, 89, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR);
   return (res);
} /* sendResListToServer */



/* ================================ sendChromosomeList ================================ */
BOOL sendChromosomeList (BOOL  secondTry)
{
   STR_TYPE  localBuf;
   CURLcode  curlRes;
   INT       returnCode;
   BOOL      tryAgain,           /* TRUE, if there will be a second trial after failure */
             res;

   /* sendChromosomeList */
   resetLocalBuffer(&resBuf);
   res = TRUE; 
   if (!secondTry)                                                       /* first call? */
   { /* prepare post request */
      curlRes = curl_easy_setopt(postHandle, CURLOPT_POSTFIELDS, chrLiBuf.memory);
      if (curlRes == CURLE_OK) {
	 curlRes = curl_easy_setopt(postHandle, CURLOPT_URL, jobUrlBuf);
	 if (curlRes != CURLE_OK) 
	    res = (fatal(CURL_IO, 90, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
      }
      else
	 res = (fatal(CURL_IO, 91, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR) == GLEAM_OK);
#ifdef DBG_WAIT_MODE
      printf("sendChromosomeList: Vor dem post request mit %d Bytes.\n",(INT)chrLiBuf.usedSize); fflush(stdout);
#endif /* DBG_WAIT_MODE */
   } /* prepare post request */
   if (res)                                      /* is post request prepared correctly? */
   { /* execute prepared post request */
      curlRes = curl_easy_perform(postHandle);
      res     = (curlRes == CURLE_OK);
#ifdef DBG_WAIT_MODE
      printf("sendChromosomeList: Nach dem post request: res=%d\n", res); fflush(stdout);
#endif /* DBG_WAIT_MODE */
#ifdef MITSIMULOG
      sprintf(logBuf, POST_REQ_RESULT_TXT, FOR_A_TXT, JOB_START_TXT, res, jobUrlBuf);
      write_to_simulog(logBuf, CLOSE_LOG);
      if (res && (resBuf.usedSize > 0))                          /* something received? */
	 logResponse(POST_DATA_REC_TXT, FOR_A_TXT, JOB_START_TXT); 
#endif /* MITSIMULOG */
      if (res)
      { /* post request successfully done */
	 res = FALSE;
	 if (resBuf.usedSize > 0)
	 { /* analyze received response */
	    if (readIntVal(&returnCode, resBuf.memory, &(resBuf.nextPos)))
	    { /* return code successfully read */
	       tryAgain = !secondTry;
	       switch (returnCode) {
  	          case 1: 
		     res      = TRUE; 
		     tryAgain = FALSE;
		     break;

	          case -1:
		     meldung(CURL_IO, NO_INFO, NO_INFO, WRONG_TASK_ID_TXT, CHR_LIST_SEND_ERR);
		     break;

	          case -2:
		     meldung(CURL_IO, NO_INFO, NO_INFO, WRONG_WFID_TXT, CHR_LIST_SEND_ERR);
		     break;

	          case -3:
		     meldung(CURL_IO, NO_INFO, NO_INFO, IDS_OK_GEN_ERR_TXT, CHR_LIST_SEND_ERR);
		     break;

	          case -4:
		     meldung(CURL_IO, NO_INFO, NO_INFO, NO_PROOF_AVAIL_TXT, CHR_LIST_SEND_ERR);
		     break;

	          default:
		     sprintf(logBuf, "%s: %d", INVALID_REPLY_TXT, returnCode);
		     err_state |= fehler(CURL_IO, 93, NO_INFO, NO_INFO, logBuf, CHR_LIST_SEND_ERR);
		     tryAgain   = FALSE;
	       } /* end of switch */
	       if (tryAgain)
	       { /* new call as a one-time recursion */
		  if (ext_simu_hochfahren(TRUE)) /*TRUE=calls terminate_ext_simu() first*/
                  { /* ext simu is ready now */
		     res = sendChromosomeList(TRUE);    /* TRUE: this is the second try */
		     if (!res) { /* 2nd trial to send data failed */
			err_state |= fehler(CURL_IO, 94, NO_INFO, NO_INFO, SECOND_FAILURE_TXT, 
					    CHR_LIST_SEND_ERR);
			sim_up = FALSE;
			taskID = NIX_IDX;
		     } /* 2nd trial to send data failed */
		  } /* ext simu is ready now */
		  else /* ext. simu not ready */
		     err_state |= fehler(CURL_IO, 95, NO_INFO, NO_INFO, NEWLY_STARTED_TXT, EXT_SIMU_NIX_DONE);
	       } /* new call as a one-time recursion */
	    } /* return code successfully read */
	    else
	    { /* bad response */
	       if (strlen(resBuf.memory) > MAX_BAD_CHARS) {
		  strncpy(localBuf, resBuf.memory, MAX_BAD_CHARS);
		  localBuf[MAX_BAD_CHARS] = '\0';
	       }
	       else
		  strcpy(localBuf, resBuf.memory);
	       sprintf(logBuf, "%s: \"%s\"", INVALID_REPLY_TXT, localBuf);
	       err_state &= fehler(CURL_IO, 96, NO_INFO, NO_INFO, logBuf, CHR_LIST_SEND_ERR);
	    } /* bad response */
	 } /* analyze received response */
	 else /* report no response */
	    err_state &= fehler(CURL_IO, 97, NO_INFO, NO_INFO, NO_RESPONSE_TXT, CHR_LIST_SEND_ERR);
      } /* post request successfully done */
      else
	 err_state = fatal(CURL_IO, 98, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_POST_ERR);
   } /* execute prepared post request */
   else  /* failed to prepare post request */
      err_state = fatal(CURL_IO, 99, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), HTTP_POST_ERR);
   return (res);
} /* sendChromosomeList */



/* ===================================== setupCom ===================================== */
BOOL setupCom (void)
{
   CURLcode  curlRes;
   BOOL      res;

   /* setupCom */
#ifdef CURL_DBG_MODE
   printf("setupCom: Start\n"); fflush(stdout);
#endif /* CURL_DBG_MODE */
   curlSetupDone = ((curlRes = curl_global_init(CURL_GLOBAL_ALL)) == CURLE_OK);
   if (curlSetupDone) 
   { /* setup communication handles */
      if ((postHandle = curl_easy_init()) == NULL)
	 err_state = fatal(CURL_IO, 100, NO_INFO, NO_INFO, POST_HANDLE_TXT, CURL_GENERAL_ERR);
      if ((getHandle  = curl_easy_init()) == NULL)
         err_state = fatal(CURL_IO, 101, NO_INFO, NO_INFO, GET_HANDLE_TXT, CURL_GENERAL_ERR);
      if (!createLocalBuffer(&resBuf))
	 err_state = fatal(CURL_IO, 102, NO_INFO, NO_INFO, "resBuf", NOT_ENOUGH_MEM);
      if (!createLocalBuffer(&postBuf))
	 err_state = fatal(CURL_IO, 103, NO_INFO, NO_INFO, "postBuf", NOT_ENOUGH_MEM);
      if (!createLocalBuffer(&chrLiBuf))
	 err_state = fatal(CURL_IO, 104, NO_INFO, NO_INFO, "chrLiBuf", NOT_ENOUGH_MEM);
	 
      if (getHandle && postHandle && resBuf.memory && postBuf.memory && chrLiBuf.memory) 
      { /* handles and buffers ok */
	 /* ---------------------- prepare header attributes for content type --------- */
	 hdrList = curl_slist_append(hdrList, "Accept: text/html");
	 hdrList = curl_slist_append(hdrList, "Content-Type: text/plain");
	 if (hdrList == NULL)
	    err_state = fatal(CURL_IO, 105, NO_INFO, NO_INFO, HDR_LIST_ATTR_TXT, CURL_GENERAL_ERR);

	 /* ---------------------- create and configure post handle ------------------- */
	 res = TRUE;
	 if ((curlRes = curl_easy_setopt(postHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback)) != CURLE_OK) /* call */
	    res = (fatal(CURL_IO, 111, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(postHandle, CURLOPT_WRITEDATA, (void *)&resBuf)) != CURLE_OK)     /* back fcn */
	    res = (fatal(CURL_IO, 112, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(postHandle, CURLOPT_CONNECTTIMEOUT_MS, 4000L)) != CURLE_OK) /* connection TMO */
	    res = (fatal(CURL_IO, 113, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(postHandle, CURLOPT_HTTPHEADER, hdrList)) != CURLE_OK)    /* set content type */
	    res = (fatal(CURL_IO, 114, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(postHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0")) != CURLE_OK)
	    res = (fatal(CURL_IO, 115, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);

	 /* ---------------------- create and configure get handle -------------------- */
	 if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback)) != CURLE_OK) /* call */
	    res = (fatal(CURL_IO, 121, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_WRITEDATA, (void *)&resBuf)) != CURLE_OK)     /* back fcn */
	    res = (fatal(CURL_IO, 122, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_CONNECTTIMEOUT_MS, 4000L)) != CURLE_OK) /* connection TMO */
	    res = (fatal(CURL_IO, 123, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_HTTPHEADER, hdrList)) != CURLE_OK)    /* set content type */
	    res = (fatal(CURL_IO, 124, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if ((curlRes = curl_easy_setopt(getHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0")) != CURLE_OK)
	    res = (fatal(CURL_IO, 125, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), 
			 CURL_GENERAL_ERR) == GLEAM_OK);
	 if (!res)
	    err_state = GLEAM_FATAL;
      } /* handles and buffers ok */
      else
	 res = FALSE;
   } /* setup communication handles */
   else {
      postHandle = NULL;
      getHandle  = NULL;
      err_state  = fatal(CURL_IO, 106, NO_INFO, NO_INFO, curl_easy_strerror(curlRes), CURL_GENERAL_ERR);
      res        = FALSE;
   }
#ifdef CURL_DBG_MODE
   printf("setupCom: Ende res=%d\n", res); fflush(stdout); 
#endif /* CURL_DBG_MODE */
#ifdef MITSIMULOG
   sprintf(logBuf, CURL_SETUP_RES_TXT, res);
   write_to_simulog(logBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
   return (res);
} /* setupCom */



/* ====================================== ioTest ====================================== */
void ioTest (void)
/*----------------------------------------------------------------------------------------
   This test routine tries to read some data from the URL stored in "serverBuffer" and 
   displays the result on screen. Appropriate error messages are generated if applicable.
----------------------------------------------------------------------------------------*/
{
   CURLcode  curlRes;

   /* ioTest */
   resetLocalBuffer(&resBuf);
   strcpy(urlBuf, serverBuffer);
   printf("ioTest: urlBuf=\"%s\"\n", urlBuf);  fflush(stdout);
   curlRes = curl_easy_setopt(getHandle, CURLOPT_URL, urlBuf);
   printf("ioTest: A curlRes=%d (CURLE_OK=%d)\n", curlRes, CURLE_OK);  fflush(stdout);
   if (curlRes == CURLE_OK) 
   {
      curlRes = curl_easy_perform(getHandle);
      printf("ioTest: B curlRes=%d\n", curlRes); fflush(stdout);
      if (curlRes == CURLE_OK) 
	 printf("\nioTest: len=%d buffer=\n%s\n\n", (INT)strlen(resBuf.memory), resBuf.memory);
      else
	 printf("ioTest: Curl-error: %s\n", curl_easy_strerror(curlRes));
   }
   else
      printf("ioTest: Curl-error: %s\n", curl_easy_strerror(curlRes));
} /* ioTest */



/* ====================================== ioInit ====================================== */
void ioInit (void)
{
   webContext      = TRUE;            /* this tells test software to be in curl IO mode */
   curlSetupDone   = FALSE;
   hdrList         = NULL;
} /* ioInit */

