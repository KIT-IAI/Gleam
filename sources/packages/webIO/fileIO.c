/*****************************************************************************************
GLEAM/AE                                                                            V1.0.0

                                    G L E A M  /  A E
       General Learning Evolutionary Algorithm and Method / Application Environment

                        Package: WEB-based IO for external simulators
                         Module: FILE-IO
 
Package: webIO                                                              File: fileIO.c
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
------------------------------------------------------------------------------------------

This module contains an alternative implementation to "curlIO.c", which uses libcurl for 
TCP/IP-based communication to the simulation service. The implementation of this module is
based on File-IO and serves for test purposes. It implements a simple interface to the OPAL-
simulator based on an assessment of one chromosome after the other.


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
        In GLEAM_USR mode the waiting can be interrupted by the user. The "waitCtr" para-
        meter supports this: The local counter ist initialized with this value and the 
        amount of already waited cycles is returned in case of an interruption. 

void deleteOldResList (void);
        Deletes a possibly existing old result list file.

BOOL getTaskID (void);
        Nothing is done. 
        If DBG_MODE is set, a dialog is started, which allows to simulate a communication 
        error or to enter a task ID. A positive task ID is stored in "taskID" and TRUE is 
        returned, FALSE otherwise and "taskID" is set to NIX_IDX.

BOOL checkModel (BOOL       *modelState,
                 const char *modelName);
        Nothing is done. 
        If DBG_MODE is set, a dialog is started, which allows to simulate a communication 
        error or to enter the state of the model (TRUE if available). TRUE is returned after 
        a successful communication and FALSE otherwise.

INT getModelList (BOOL  withNumberedItems);
        Nothing is done. 
        If DBG_MODE is set, a dialog is started, which allows to simulate a communication 
        error or to create an empty model list or a list with some predefined model names.
        The list is processed, i.e. their names are written line by line to the internal 
        text buffer. If "withNumberedItems" is set, each line is prefixed with a serial 
        number. The length of the chosen list is returned.

BOOL startModel (const char *modelName);
        Nothing is done. 
        If DBG_MODE is set, a dialog is started, which allows to simulate a communication 
        error or to enter a workflow ID, which is stored in "modelWFID". Negative IDs are
        treated as an error. On error, "modelWFID" is set to NIX_IDX and FALSE is returned.

BOOL stopTask (void);
        Nothing is done. 
        If DBG_MODE is set, a dialog is started, which allows to simulate a communication 
        error. In case of an error, FALSE is returned and TRUE otherwise. "taskID" and 
        "modelWFID" are set to NIX_IDX. 

void abortJobOfWorkflow (void);
        The actual job of the workflow denoted by "urlComBuf" is aborted. In case of a 
        simulated communication error or a negative return value an appropriate message 
        is generated and "sim_up" is set to FALSE.


Exported routines for ESS-EvalFrame side:
=========================================

BOOL prepComToServer (BOOL  logServerName);
        If "logServerName" is set, the URL contained in "serverBuffer" is logged. 
        Nothing else is done and TRUE is returned always.

BOOL sendResListToServer (void);
        Writes the result list contained in the io buffer "postBuf" to the file denoted 
        by DATA_FROM_SIM for testing purposes. Returns TRUE on success and FALSE otherwise.
        I/O errors are reported to the logfile.


Package-global routines in detail:
==================================

BOOL sendChromosomeList (BOOL  secondTry);
        This function may call itself once recursevly and must be called otherwise with 
        unset "secondTry" parameter. If DBG_MODE is set, it simulates the receiving of 
        the return value by a user dialog. Otherwise the return value is assumed to be 1. 
        In case of 1, it writes the contents of the "chrLiBuf" to the file denoted by 
        DATA_TO_SIM. In case of an (simulated) error response an appropriate message is 
        generated and an attempt is made for loading the correct model by a 
        call to simu_hochfahren() including a call to terminate_ext_simu(). If this was
        successful, a recursive call with "secondTry" set to TRUE is performed. If the 
        second operation fails again, an error message is generated, "sim_up" is set to 
        FALSE and "taskID" to NIX_IDX, and no further sending attempt is made. In all 
        cases of failure FALSE is returned and TRUE otherwise.

BOOL setupCom (void);
        Creates all three io buffers and returns TRUE on success.

void ioInit (void);
        Modulinitialization.


------------------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Mehr Defaults bei den Test-Dialogen.
------------------------------------------------------------------------------------------
Status: Impl                          Autor: W.Jakob                     Datum: 24.10.2018
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

#define ERR_PRINTS
#undef DBG_MODE



/* =============================== Exportierte Variable: ============================== */


/* ============================= Package-globale Variable: ============================ */



/* ================================= Lokale Konstante: ================================ */
#define DATA_FROM_SIM     "dataFromSimu.dat"
#define DATA_TO_SIM_TMP   "data2Simu.tmp"
#define DATA_TO_SIM       "data2Simu.dat"



/* ============================= Lokale Typdeklarationen: ============================= */



/* ================================= Lokale Variable: ================================= */
static FILE        *dataFile;
static STRING_TYPE  urlBuf;            /* for temporary URLs different from "urlComBuf" */



/* ================================= Lokale Routinen: ================================= */




/* ==================================== finalizeCom =================================== */
void finalizeCom (void)
{
   if (sim_up)
   { /* ext. simu. services running and ready */
      if (writeChrListHeader(0))
      { /* Opal-Bew. abschalten: Leere ChrListe schicken */
	 if (sendChrList2OptServ())
	    sprintf(logBuf, "\nTermination file for external simulation services successfully written.\n");
	 else
	    sprintf(logBuf, "\nError creating termination file for external simulation services!\n");
	 write_to_log(logBuf, CLOSE_LOG);
      } /* Opal-Bew. abschalten: Leere ChrListe schicken */
      else
	 err_state = fatal(FILE_IO, 130, NO_INFO, NO_INFO, BAD_EMPTY_LIST_TXT, CHROMOSOME_LIST_ERR);
   } /* ext. simu. services running and ready */
   if (resBuf.memory != NULL)
      free(resBuf.memory);
   if (postBuf.memory != NULL)
      free(postBuf.memory);
   if (chrLiBuf.memory != NULL)
      free(chrLiBuf.memory);
   sim_ok = FALSE;
} /* finalizeCom */



/* ================================= waitForResultList ================================ */
BOOL waitForResultList (INT *waitCtr)
{
   STRING_TYPE  buf;
   INT          ctr,                                          /* counts the wait cycles */
                ij;
   BOOL         fileExists,
                res;

   /* waitForResultList */
   fileExists = FALSE;
   res        = FALSE;
   ctr        = *waitCtr;
   while((ctr < simuResultWaitCycles) && !fileExists)
      if (file_exists(DATA_FROM_SIM))
      { /* reply received */
	 fileExists = TRUE;
	 if (open_file(&dataFile, DATA_FROM_SIM, "r"))
	 { /* In-File ist offen */
	    res = TRUE;
	    while ((fgets(buf, MAX_STRING, dataFile) != NULL) && res)   /* read line    */
	       res = (writeLocalBuffer(buf, strlen(buf), &resBuf) > 0); /* and store it */
	    fclose(dataFile);
	    if (res)
            { /* delete result file */
	       if (remove(DATA_FROM_SIM) == -1) 
               { /* file administration failed */
		  sprintf(buf, "*** Failed to delete result file \"%s\": %s", 
			  DATA_FROM_SIM, strerror(errno));
#ifdef ERR_PRINTS
		  printf("%s\n", buf);
#endif /* ERR_PRINTS */
		  write_to_log(buf, CLOSE_LOG);
	       } /* file administration failed */
#ifdef DBG_MODE
	       if (lies_wert(&ij, "waitForResultList: Enter return status [0]: ", -9, 99, 0))
		  ij = -5;                                         /* aborted by escape */
	       else
	       { /* returnCode successfully read */
		  sprintf(buf, "%2d", ij);
		  resBuf.memory[0] = buf[0];
		  resBuf.memory[1] = buf[1];
	       } /* returnCode successfully read */
#endif /* DBG_MODE */
	    } /* delete result file */
	    else
	    { /* retain faulty result file and report it */
	       sprintf(buf, "*** Error reading result file. Result file not deleted!\n"); 
#ifdef ERR_PRINTS
	       printf("%s\n", buf);
#endif /* ERR_PRINTS */
	       write_to_log(buf, CLOSE_LOG);
	    } /* retain faulty result file and report it */
	 } /* In-File ist offen */
      } /* reply received */
      else
      { /* wait and see */
#ifdef GLEAM_USR
  #ifdef DBG_MODE
	 taste = (char_dialog("waitForResultList: Taste? [j/N]: ") == JA);
  #else /* no DBG_MODE */
	 taste = kb_hit();                               /* check for interrupt by user */
  #endif /* no DBG_MODE */
#endif /* GLEAM_USR */
	 if (taste) 
	 { /* abandon wait loop and return amount of already done wait cycles */
	    *waitCtr = ctr;                                    /* performed wait cycles */
	    ctr = simuResultWaitCycles;                               /* aborts waiting */
	 } /* abandon wait loop and return amount of already done wait cycles */
	 else
	 { /* wait one more time */
	    warte(WAIT_FOR_READ);
	    ctr++;
	 } /* wait one more time */
      } /* wait and see */
#ifdef DBG_MODE
   printf("waitForResultList: Ende! res=%d initial waitCtr=%d waits=%d taste=%d\n", res, *waitCtr, ctr, taste);
#endif /* DBG_MODE */
#ifdef MITSIMULOG
   if (mit_simulog)
   { /* logging of the simulator interface */
      if (taste)
	 sprintf(buf, RESLI_INTRPTED_TXT, *waitCtr, simuResultWaitCycles);
      else
	 sprintf(buf, RESLI_RECEIVED_TXT, (INT)resBuf.usedSize, ctr, simuResultWaitCycles);
      write_to_simulog(buf, CLOSE_LOG);
   } /* logging of the simulator interface */
#endif /* MITSIMULOG */
   return (res);
} /* waitForResultList */



/* ================================= deleteOldResList ================================= */
void deleteOldResList (void)
{
   if (file_exists(DATA_FROM_SIM))
      if (remove(DATA_FROM_SIM) == -1) 
      { /* file administration failed */
	 sprintf(logBuf, "*** Failed to delete result file \"%s\": %s", DATA_FROM_SIM, strerror(errno));
#ifdef ERR_PRINTS
	 printf("%s\n", logBuf);
#endif /* ERR_PRINTS */
	 write_to_log(logBuf, CLOSE_LOG);
      } /* file administration failed */
} /* deleteOldResList */



/* ===================================== getTaskID ==================================== */
BOOL getTaskID (void)
{
   BOOL  res;

   /* getTaskID */
   res = TRUE;
#ifdef DBG_MODE
   strcpy(urlBuf, serverBuffer);
   strcat(urlBuf, GET_TASK_ID_CMD);
   printf("getTaskID: get request: URL=\"%s\"\n", urlBuf);
   if (char_dialog("getTaskID: Communication error [j/N]: ") == JA) { /* i/o error */
      res = FALSE;
      fehler(FILE_IO, 10, NO_INFO, NO_INFO, "simulated I/O error", HTTP_GET_ERR);
   } /* i/o error */
   else
   { /* reply received */
      if (lies_wert(&taskID, "getTaskID: Enter taskID [1]: ", -1000, 90000, 1))
	 res = FALSE;                                              /* aborted by escape */
      else 
	 res = taskID >= 0;
      if (res)
	 strcpy(lastSimServDomainName, extSimServDomainName);
      else
	 fehler(FILE_IO, 11, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR); 
   } /* reply received */
   if (!res)
      taskID = NIX_IDX;
#else /* no DBG_MODE */
   taskID = 42;                                            /* for testing purposes only */
#endif /* no DBG_MODE */
   return (res);
} /* getTaskID */



/* ==================================== checkModel ==================================== */
BOOL checkModel (BOOL       *modelState,
                 const char *modelName)
{
   char  ch;
   BOOL  res;

   /* checkModel */
   res         = TRUE;
   *modelState = TRUE;
#ifdef DBG_MODE
   if (taskID != NIX_IDX)
   { /* taskID ok */
      sprintf(urlBuf, "%s/%d/%s%s", serverBuffer, taskID, modelName, GET_MODEL_STATE_CMD);
      printf("checkModel: get request: URL=\"%s\"\n", urlBuf);
      if (char_dialog("checkModel: Communication error [j/N]: ") == JA) { /* i/o error */
	 res = FALSE;
	 fehler(FILE_IO, 20, NO_INFO, NO_INFO, "simulated I/O error", HTTP_GET_ERR);
      } /* i/o error */
      else
      { /* reply received */
	 sprintf(logBuf, "checkModel: Does model \"%s\" exist [J/n]: ", modelName);
	 ch = char_dialog(logBuf);
	 *modelState = ((ch == JA) || (ch == CR));
      } /* reply received */
   } /* taskID ok */
   else
      res = (fehler(FILE_IO, 29, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR) == GLEAM_OK);
   if (!res)
      *modelState = FALSE;
#endif /* DBG_MODE */
   return (res);
} /* checkModel */



/* =================================== getModelList =================================== */
INT getModelList (BOOL  withNumberedItems)
{
   STR_TYPE  listEntryBuf,     /* one list entry, i.e. model name                       */
             lineBuf;          /* line buffer for storing a model name entry            */
   INT       numberOfModels,   /* number of abtained models, i.e. length of the list    */
             resVal,           /* result of reading operations from "resBuf"            */
             pos;              /* start position of the first/next string in "resBuf"   */
   char     *nextBufPos;       /* points to the position "pos" in input buffer "resBuf" */
   BOOL      res;              /* result of a get request and later of write operations */

   /* getModelList */
   numberOfModels = 0;
#ifdef DBG_MODE
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      sprintf(urlBuf, "%s/%d%s", serverBuffer, taskID, GET_MODEL_LIST_CMD);
      printf("getModelList: get request: URL=\"%s\"\n", urlBuf);
      if (char_dialog("getModelList: Communication error [j/N]: ") == JA) { /* i/o error */
	 res = FALSE;
	 err_state |= fehler(FILE_IO, 30, NO_INFO, NO_INFO, "simulated I/O error", HTTP_GET_ERR);
      } /* i/o error */
      else
      { /* reply received */
         /* ----------------------- specify simulated input list: --------------------- */
	 if (char_dialog("getModelList: Empty list? [j/N]: ") != JA)
	 { /* no empty string */
	    if (char_dialog("getModelList: Empty list with terminating LF ? [j/N]: ") == JA) 
	       strcpy(urlBuf, "\n");
	    else 
	       if (char_dialog("getModelList: List with terminating LF ? [j/N]: ") == JA) 
		  strcpy(urlBuf, "Rumpelstielzchen\nSchneewittchen\nFrau Holle\nHans im Glueck\n");
	       else
		  strcpy(urlBuf, "Rumpelstielzchen\nSchneewittchen\nFrau Holle\nHans im Glueck\nHotzenplotz");
	    writeLocalBuffer(urlBuf, strlen(urlBuf), &resBuf);
	 } /* no empty string */

         /* ----------------------- process simulated input list: --------------------- */
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
	    meldung(FILE_IO, NO_INFO, NO_INFO, "", EMPTY_MODEL_LIST_ERR);
      } /* reply received */
   } /* taskID ok */
   else
      err_state |= fehler(FILE_IO, 39, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR);
#endif /* DBG_MODE */
   return (numberOfModels);
} /* getModelList */



/* ==================================== startModel ==================================== */
BOOL startModel (const char *modelName)
{
   BOOL  res;

   /* startModel */
   res = TRUE;
#ifdef DBG_MODE
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&postBuf);
      sprintf(urlBuf, "%s/%d%s", serverBuffer, taskID, POST_MODEL_CONFIG_CMD);
      printf("startModel: post request: URL=\"%s\"\n", urlBuf);
      sprintf(urlBuf, MOD_START_MOD_TXT, modelName);
      writeLocalBuffer(urlBuf, strlen(urlBuf), &postBuf);
      sprintf(urlBuf, MOD_START_ALG_TXT, SIMU_JOB_TXT);
      writeLocalBuffer(urlBuf, strlen(urlBuf), &postBuf);
  #ifdef MITSIMULOG
      sprintf(logBuf, "\nData for post request of model start:\n\"%s\"\n%s", postBuf.memory, STRICH_40_TXT);
      write_to_simulog(logBuf, CLOSE_LOG);
  #endif /* MITSIMULOG */
      if (char_dialog("startModel: Communication error [j/N]: ") == JA) { /* i/o error */
	 res = FALSE;
	 fehler(FILE_IO, 40, NO_INFO, NO_INFO, "simulated I/O error", HTTP_GET_ERR);
      } /* i/o error */
      else
      { /* reply received */
	 if (lies_wert(&modelWFID, "startModel: Enter WFID [2]: ", -1000, 90000, 2))
	    res = FALSE;                                           /* aborted by escape */
	 else 
	    res = modelWFID >= 0;
	 if (!res)
	    fehler(FILE_IO, 42, modelWFID, NO_INFO, WORKFLOW_ID_TXT, INVALID_ID_ERR); 
      } /* reply received */
   } /* taskID ok */
   else
      res = (fehler(FILE_IO, 49, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR) == GLEAM_OK);
#else /* no DBG_MODE */
   modelWFID = 4711;                                       /* for testing purposes only */
#endif /* no DBG_MODE */
   if (res)
      sprintf(urlComBuf, "%s/%d/%d", serverBuffer, taskID, modelWFID);
   else
      modelWFID = NIX_IDX;
   return (res);
} /* startModel */



/* ===================================== stopTask ===================================== */
BOOL stopTask (void)
{
   INT   returnCode;
   BOOL  res;

   /* stopTask */
   res = TRUE;
#ifdef DBG_MODE
   if (taskID != NIX_IDX)
   { /* taskID ok */
      sprintf(urlBuf, "%s/%d%s", serverBuffer, taskID, GET_STOP_CMD);
      printf("stopTask: get request: URL=\"%s\"\n", urlBuf);
      if (char_dialog("stopTask: Communication error [j/N]: ") == JA) { /* i/o error */
	 res = FALSE;
	 fehler(FILE_IO, 50, NO_INFO, NO_INFO, "simulated I/O error", HTTP_GET_ERR);
      } /* i/o error */
      else
      { /* reply received */
	 if (lies_wert(&returnCode, "stop task: Enter return status [1]: ", -1000, 90000, 1))
	    res = FALSE;                                           /* aborted by escape */
	 else 
	 { /* return code read */
	    if (returnCode == 1)
	       sprintf(logBuf, TSK_STOP_DONE_TXT, taskID);
	    else
	       if (returnCode == -1)
		  sprintf(logBuf, TSK_STOP_ERROR_TXT, taskID);
		  else
		     if (returnCode == -2) {
			sprintf(logBuf, TSK_STOP_ERROR2_TXT, taskID, NO_PROOF_AVAIL_TXT);
			res = (fehler(FILE_IO, 51, NO_INFO, NO_INFO, logBuf, GENERAL_MESSAGE) == GLEAM_OK);
		     }
		     else /* something weird recievded */
			sprintf(logBuf, TSK_STOP_BAD_REPLY, taskID, returnCode);
  #ifdef MITSIMULOG
	    write_to_simulog("", KEEP_LOG_OPEN);
	    write_to_simulog(logBuf, CLOSE_LOG);
  #endif /* MITSIMULOG */
	    res    = TRUE;                   /* the task related to "taskID" is terminated */
	    taskID = NIX_IDX;                /* regardless of the return value.            */
	    strcpy(urlComBuf, "");  /* no URL for I/O of chromosomes and results available */
	 } /* return code read */
      } /* reply received */
   } /* taskID ok */
   else
      res = (fehler(FILE_IO, 59, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR) == GLEAM_OK);
#endif /* DBG_MODE */
   return (res);
} /* stopTask */



/* ================================ abortJobOfWorkflow ================================ */
void abortJobOfWorkflow (void)
{
   INT  returnCode;

   /* abortJobOfWorkflow */
#ifdef DBG_MODE
   sim_up = FALSE;                    /* will be set to TRUE on successful job abortion */
   if (taskID != NIX_IDX)
   { /* taskID ok */
      resetLocalBuffer(&resBuf);
      sprintf(urlBuf, "%s/%s", urlComBuf, GET_ABORT_CMD);
      printf("abortJobOfWorkflow: get request: URL=\"%s\"\n", urlBuf);
      if (char_dialog("abortJobOfWorkflow: Communication error [j/N]: ") == JA)
	 fehler(FILE_IO, 60, NO_INFO, NO_INFO, "simulated I/O error", HTTP_GET_ERR);
      else
      { /* reply received */
	 if (lies_wert(&returnCode, "abortJobOfWorkflow: Enter return status [1]: ", -1000, 90000, 1))
	    returnCode = -555;                                     /* aborted by escape */
	 if (returnCode == 1)
	    sim_up = TRUE;                          /* current job aborted successfully */
	 else                                                           /* sets sim_up  */
	    generateErrMsg(FILE_IO, 61, returnCode, JOB_ABORTION_ERR);  /* to FALSE     */
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
      } /* reply received */
   } /* taskID ok */
   else
      fehler(FILE_IO, 69, taskID, NO_INFO, TASK_ID_TXT, INVALID_ID_ERR);
#endif /* DBG_MODE */
} /* abortJobOfWorkflow */



/* ================================== prepComToServer ================================= */
BOOL prepComToServer (BOOL  logServerName)
{
   if (logServerName) {
      sprintf(logBuf, RES_SERVER_NAME_TXT, serverBuffer);
      write_to_log(logBuf, CLOSE_LOG);
   }
   return (TRUE);
} /* prepComToServer */



/* ================================ sendResListToServer =============================== */
BOOL sendResListToServer (void)
{
   BOOL  res;

   /* sendResListToServer */
   res = FALSE;
   if (open_file(&dataFile, DATA_FROM_SIM, "w"))
   { /* result list file is open */
      res = (fprintf(dataFile, "%s", postBuf.memory) > 0);
      fclose(dataFile);
      if (!res)
	 write_to_log("**** Error writing result list to file.!\n", CLOSE_LOG);
   } /* result list file is open */
   else
      write_to_log("**** result list file could not be opened!\n", CLOSE_LOG);
   return (res);
} /* sendResListToServer */



/* ================================ sendChromosomeList ================================ */
BOOL sendChromosomeList (BOOL  secondTry)
{
   INT   returnCode;
   BOOL  tryAgain,               /* TRUE, if there will be a second trial after failure */
         res;

   /* sendChromosomeList */
   res = FALSE;
   resetLocalBuffer(&resBuf);
#ifdef DBG_MODE
   printf("sendChromosomeList: post request: URL=\"%s%s\"\n", urlComBuf, POST_CHR_LIST_CMD);
   if (lies_wert(&returnCode, "sendChromosomeList: Enter return code [1]: ", -1000, 90000, 1)) {
      err_state |= fehler(FILE_IO, 97, NO_INFO, NO_INFO, NO_RESPONSE_TXT, CHR_LIST_SEND_ERR);
      return (FALSE);
   }
#else /* no DBG_MODE */
   returnCode = 1;
#endif /* no DBG_MODE */
   tryAgain = !secondTry;
   switch (returnCode)
   {
      case 1: 
	 res      = TRUE; 
	 tryAgain = FALSE;
	 if (open_file(&dataFile, DATA_TO_SIM_TMP, "w"))
         { /* Out-File ist offen */
	    res = (fprintf(dataFile, "%s", chrLiBuf.memory) > 0);
	    fclose(dataFile);
	    if (res) {
	       if (rename(DATA_TO_SIM_TMP, DATA_TO_SIM) == -1)     /* returns 0 on success */
		  res = FALSE;
	    }
	 } /* Out-File ist offen */
	 if (!res)
	    printf("sendChromosomeList: writing chromosome list file failed somehow!\n");
	 break;

      case -1:
	 meldung(FILE_IO, NO_INFO, NO_INFO, WRONG_TASK_ID_TXT, CHR_LIST_SEND_ERR);
	 break;

      case -2:
	 meldung(FILE_IO, NO_INFO, NO_INFO, WRONG_WFID_TXT, CHR_LIST_SEND_ERR);
	 break;

      case -3:
	 meldung(FILE_IO, NO_INFO, NO_INFO, IDS_OK_GEN_ERR_TXT, CHR_LIST_SEND_ERR);
	 break;

      case -4:
	 meldung(CURL_IO, NO_INFO, NO_INFO, NO_PROOF_AVAIL_TXT, CHR_LIST_SEND_ERR);
	 break;

      default:
	 sprintf(logBuf, "%s: %d", INVALID_REPLY_TXT, returnCode);
	 err_state |= fehler(FILE_IO, 93, NO_INFO, NO_INFO, logBuf, CHR_LIST_SEND_ERR);
	 tryAgain   = FALSE;
   } /* end of switch */
   if (tryAgain)
   { /* new call as a one-time recursion */
      if (ext_simu_hochfahren(TRUE))     /* TRUE: call terminate_ext_simu() first */
      { /* ext simu is ready now */
	 res = sendChromosomeList(TRUE);       /* TRUE: this is the second try */
	 if (!res) { /* 2nd trial to send data failed */
	    err_state |= fehler(FILE_IO, 94, NO_INFO, NO_INFO, SECOND_FAILURE_TXT, 
				CHR_LIST_SEND_ERR);
	    sim_up = FALSE;
	    taskID = NIX_IDX;
	 } /* 2nd trial to send data failed */
      } /* ext simu is ready now */
      else /* ext. simu not ready */
	 err_state |= fehler(FILE_IO, 95, NO_INFO, NO_INFO, NEWLY_STARTED_TXT, EXT_SIMU_NIX_DONE);
   } /* new call as a one-time recursion */
   return (res);
} /* sendChromosomeList */



/* ===================================== setupCom ===================================== */
BOOL setupCom (void)
{
   return (createLocalBuffer(&resBuf)   &&
	   createLocalBuffer(&postBuf)  &&
	   createLocalBuffer(&chrLiBuf)   );
} /* setupCom */



/* ====================================== ioInit ====================================== */
void ioInit (void)
{
   /* ioInit */
   webContext = FALSE;               /* this tells test software to be in file I/O mode */
} /* ioInit */

