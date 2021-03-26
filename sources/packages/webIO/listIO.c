/*****************************************************************************************
GLEAM/AE                                                                            V1.1.2

                                    G L E A M  /  A E
       General Learning Evolutionary Algorithm and Method / Application Environment

                        Package: WEB-based IO for external simulators
                         Module: chromosome- and resultLIST-IO
 
Package: webIO                                                              File: listIO.c
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
------------------------------------------------------------------------------------------

This module provides I/O functions for chromosome lists and result lists, which are used 
for web-based I/O to the simulation service, which provides external simulators. There are
Three buffers for io-operations: the common "resBuf" for responses on get and post requests,
the "chrLiBuf" dedicated to store a chromosome list to be sent, and the general "postBuf"
for all other post requests (at present it is used by the "/start"-request only). 
The space of each of these buffers is reused and it grows with the peak amount of data 
stored.

The meaning of the return status of a result list received from the simulation service
is as follows:
  =0: OK, no unsuccessful simulation runs
  >0: Number of unsuccessful simulation runs for which a result vector is returned, which 
      will lead to a poor fitness value (presumably zero or close to it)
  <0: ErrorID. The rest of the buffer is either empty or may contain an error message.


Exported Routines in detail:
============================

BOOL readIntVal  (INT         *val,
                  const char  *buf,
                  char       **endPtr);

BOOL readRealVal (DOUBLE      *val,
                  const char  *buf,
                  char       **endPtr);
        The contents of the buffer "buf" is converted to integer or real respectively. The 
        conversion is checked for under/overflow, conversion errors, and no digits. In case 
        of one of these errors, FALSE is returned and "val" and "endPtr" remain unchanged. 
        Otherwise, the function returns TRUE, the converted number is returned in "val", and 
        "endPtr" points to the character after the last character used in the conversion.
        No error messages will be output. 

void dumpPostBuf (void);
        Prints the content of the postBuf to stdout. Just a test function.

BOOL initListIO (void);
        Package initialization and communication setup (setupCom()). Returns TRUE on 
        success and FALSE otherwise. "sim_ok" is set accordingly.


Exported routines for GLEAM side:
=================================

BOOL writeChrListHeader (INT  chrNumber);
        Creates the header of a new chromosome list of length "chrNumber" using the 
        "maxIntNumber" and "maxDoubleNumber" provided by package "hmod". The header is 
        written to the "chrLiBuf" that has been reset before. "chrNumber" is stored in 
        "actualChrNumber" for later use of "sendChrList2OptServ".
        If "chrNumber" equals to zero an empy list (a list with all header items beeing 
        zero) is written. Such a list tells the simulation service to terminate.
        Returns TRUE on success and FALSE otherwise.

BOOL updateChrListHeader (INT  chrNumber);
        Updates the header of the actual chromosome list and "actualChrNumber", if the new 
        new number fits into 6 digits and there are more than 6 characters stored in the 
        actual "chrLiBuf". In that case, TRUE is returned. If not FALSE and an error 
        message is generated.

BOOL writeChrToList (CHAIN_ELEM_PTR_TYPE  chrPtr,
		     INT                  chrAddr1,
		     INT                  chrAddr2);
        Appends the data of chromosome "chrPtr" to the "chrLiBuf".
        Returns TRUE on success and FALSE otherwise accompanied by an error messge.
        Status: Dynamic chromosomes with varying gene data (lesak) have been tested.
                Large genes, which require a writing of the buffer after processing 
                the integer parameters were not tested.

BOOL sendChrList2OptServ (void);
        Checks if the "chrNumber" given by the last call of "writeChrListHeader" and the
        number of chromosomes stored in "chrLiBuf" match. If so, the data are send to the
        optimization service. Afterwards the "chrLiBuf" is reset.
        Returns TRUE on a successful send operation and FALSE otherwise. Corresponding 
        error messages are generated, if FALSE is returned.

BOOL getResultListHeader (INT *retStatus,
                          INT *entryNumber);
        Reads the header data of a result list stored in "resBuf". The return status 
        of the list is returned in "retStatus" and the number of result entries in 
        "entryNumber". Checks if the number of result values per entry contained in the 
        list header is equal to "akt_anz_roh_erg_werte" from package "hmod". If so and 
        the list header was read successfully, and "retStatus" is positive, TRUE is 
        returned and FALSE otherwise.
        All header errors or a bad "retStatus" are reported by an error message and 
        "err_state" is set accordingly . Furthermore, in case of a negative return status,
        the message text received is written to the simu_logfile, if MITSIMULOG is set.

BOOL getNextResults (DOUBLE *resdDataList,
                     INT    *parentID,
                     INT    *childID);
        Reads the result values of the next list entry and stores them in "resDataList". 
        The address of the corresponding chromosome is returned in "parentID" and 
        "childID". If there is no unread result entry in the list or there are not enough 
        values or a conversion error occurs, an error message is generated and FALSE is 
        returned. Otherwise, the function returns TRUE.

void setServerPraeFix (void);
        A new server praefix string is created and stored in "serverBuffer". Its form 
        depends on the setting of the switch PAR_POP_VERS: 
        PAR_POP_VERS is set:
            <ACCESS_METHOD><extSimServDomainName><PORT_TXT><ROOT_DIR>/<nodeNumber>
        PAR_POP_VERS is unset:
            <ACCESS_METHOD><extSimServDomainName><PORT_TXT><ROOT_DIR>
        "nodeNumber" is the number of island node.
        This is documented by a message in the logfile and in the simu_logfile provided 
        that MITSIMULOG is set. 


Exported routines for ESS-EvalFrame side:
=========================================

INT  waitForChrList (INT *maxIntAmount,
                     INT *maxRealAmount);
        Cyclically checks the existence of file DATA_TO_SIM and as soon as it is available
        the following is done:
          1. If the file "<DATA_TO_SIM>.last" exists, it is deleted.
          2. The file DATA_TO_SIM is read into the buffer "chrLiBuf" and renamed to 
             "<DATA_TO_SIM>.last" afterwards.
          3. The header of the read chromosome list is analysed and the chromosome number 
             contained in the list is returned as function result. Furthermore, the maximum 
             amount of integer and real parameters of the genes is returned in the two 
             parameters and stored locally in "maxIntsOfGenes" and "maxRealsOfGenes". If
             WITH_DYN_GENES is not set, both numbers must be within the absolute limits of 
             "chaindef.h" I_PAR_ANZ_MAX and R_PAR_ANZ_MAX. If not, it is a fatal error. 
        If an error occurs, an appropriate message is written to the logfile and the 
        function returns 0.

BOOL getChrHdrData (INT *parentID,
                    INT *childID,
                    INT *chrLen);
        Reads the three header data items of a chromosom from the buffer "chrLiBuf" and 
        returns them in the three parameters. The function returns TRUE on success and 
        FALSE otherwise. In the latter case appropriate error message(s) are written.

BOOL get1GeneData (INT    *geneID,
                   INT    *intParams,
                   DOUBLE *realParams);
        Reads the gene ID and the integer and real parameter lists of the respective maximum 
        length and returns the data in the three parameters. The function returns TRUE on 
        success and FALSE otherwise. In the latter case appropriate error message(s) are 
        written.

BOOL get1GeneItem (CHAIN_ELEM_PTR_TYPE  genePtr);
        Reads the gene ID and the integer and real parameter lists and stores the data in 
        a gene, i.e. a chromosome list element, provided by "genePtr". The function returns 
        TRUE on success and FALSE otherwise. In the latter case appropriate error message(s) 
        are written.

BOOL writeResListHeader (INT  resNumber);
        Creates the header of a new result list of length "resNumber" entries using the 
        "akt_anz_roh_erg_werte" provided by package "hmod". The header is written to the
        "postBuf" that has been reset before. The return status is set to 0 (ok).
        Returns TRUE if there is enough memory available and FALSE otherwise.

void updateResListHeader (INT  badResNumber);
        The status value of the result list header is updated by "badResNumber". If this 
        number exceeds 9999 it is limited to 9999, which is reported by an error.

BOOL writeResToResList (INT     chrID1,
                        INT     chrID2,
                        DOUBLE *resValues);
        The result values contained in "resVAlues" are written together with the IDs of the 
        originating chromosome to the result list in "postBuf".
        Returns TRUE if there is enough memory available and FALSE otherwise.


Package-global routines in detail:
==================================

BOOL generateErrMsg (const char *modulName,
                     INT         origin,
                     INT         retStatus,
                     INT         errNo);
        Creates an error text from "retStatus" and based on it, an error message using
        "modulName, "origin", and "errNo". In case of a "-1" as "retStatus", "taskID" 
        is set to NIX_IDX. GLEAM_ERROR is stored in "err_state", "sim_up" is set to FALSE, 
        and FALSE is returned.

size_t writeLocalBuffer (void   *contents, 
                         size_t  size, 
                         void   *locBuf);
        Adds the "contents" of "size" bytes to a buffer pointed to by "locBuf". The 
        function is implemented for buffers of type IO_BUFFER_TYPE and treats the received 
        bytes as characters and the memory as a null-terminated string. The memory of the 
        buffer is expanded if required.
        The amount of received data in bytes is returnd for normal operation and 0 if not 
        enough memory is available. In the later case an error message is issued.
        This function is used for writing data to the IO buffer by the chromosome or 
        result list functions and is called by the call back function used by the "libcurl" 
        library functions that pass received data to the calling program. 

BOOL createLocalBuffer (IO_BUFFER_TYPE *memBuf);
        Create "memBuf" as empty memory and return TRUE on success.

void resetLocalBuffer (IO_BUFFER_TYPE *memBuf);
        Resets "memBuf" to empty content but preserves its memory capacity for further use.

void dumpResBuf (const char *title);
        Prints the memory of the "resBuf" to the screen with a line including "title" at 
        the begin and a line at the end.


Remark: A WAIT_TIME of 2 msec requires about 1% CPU time on a common PC. Details:
         1 msec: 1.3 - 2.0%
         2 msec: 0.7 - 1.0%
         5 msec: 0.3%
        50 msec: 0%
------------------------------------------------------------------------------------------
Changes with respect to version 1.0.0:
 1. New function writeLocalBuffer(), which does all the writing stuff and is 
    called by the higher i/o-functions and by the call back function 
    writeMemoryCallback() used by the libcurl library.                         WJ 04/26/18
 2. Adjustment of the calls of get_act_descr().                                WJ 06/28/19
 3. WITH_DYN_GENES: Additional optional code for the case of genes with 
    unlimited number of parameters.                                            WJ 07/08/19
 4. The functioin "setServerPraeFix" integrates the number of island node 
    "nodeNumber" into the server praefix string stored in "serverBuffer",
    if the global switch PAR_POP_VERS is set. For this, the variable 
    "nodeNumber" is declared here and exported.                                WJ 03/20/20
------------------------------------------------------------------------------------------
Status: o.k.                          Author: W.Jakob                     Date: 03/20/2020
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


#ifdef GLO_CHECKS
  #define PTR_CHECK                                           /* do some pointer checks */
#endif

#undef MEM_DBG                 /* activates some debuggung output of "writeLocalBuffer" */
#undef IO_DBG                  /* activates debugging output after send and receive     */



/* ================================ Exported Variables: =============================== */
STRING_TYPE  urlComBuf;      /* URL incl. task ID and modelWFID for iterative I/O       */
STR_TYPE     serverBuffer;   /* URL start for requests (<access>://<domain>:<port>/opt) */
INT          taskID;         /* task ID of the current task and model                   */
BOOL         webContext;     /* TRUE, if http communication with SimuServ. For test SW  */

#if defined(PAR_POP_VERS) && defined(EXT_SIMU_SERV)
  INT        nodeNumber;     /* 0 = initializer, >0: number of island node              */
#endif /* PAR_POP_VERS und EXT_SIMU_SERV */


/* ============================= Package-global Variables: ============================ */
IO_BUFFER_TYPE  resBuf,         /* common result buffer for web-based communication     */
                postBuf,        /* common post buffer (at present used by "/start" only */
                chrLiBuf;       /* post buffer for a chromosome list                    */
STRING_TYPE     logBuf;         /* buffer for creating log messages                     */
INT             modelWFID;      /* workflow ID of the current model                     */



/* =================================== Local Constants: =============================== */
#define RESULT_WAIT_TIME  45    /* max. wait time for the result list in sec. (default) */
#define WAIT_TIME          2    /* wait time for checking for new chrList file in msec. */
#define RES_VAL_PREC      12    /* result value precision; field width: +7 + 1 for space*/
#define DATA_TO_SIM       "data2Simu.dat"      /* name of new chromosome list file      */
#define DATA_TO_SIM_LAST  "data2Simu.dat.last" /* name of previous chromosome list file */




/* ================================ Local Typedeclarations: =========================== */



/* ================================= Local Variables: ================================= */
static INT    chrNumberInHdr,     /* provided by the last call of "writeChrListHeader"  */
              actualChrNumber,    /* actual number of chromosomes in list               */
              maxIntsOfGenes,     /* max.number of integer parameters of all gene types */
              maxRealsOfGenes;    /* max.number of real parameters of all gene types    */



/* ================================== Local Routines: ================================= */
static void logStartOfResBuf (const char  *badItem);




/* ==================================== readIntVal ==================================== */
BOOL readIntVal (INT         *val,
                 const char  *buf,
                 char       **endPtr)
{
   LONG  longVal;
   BOOL  result;
   char *locPtr;

   /* readIntVal */
   errno   = 0;                            /* to distinguish success/failure after call */
   longVal = strtol(buf, &locPtr, 10);
   /*    under/overflow  OR      bad conversion          OR    no digits  */
   if ((errno == ERANGE) || (errno != 0 && longVal == 0) || (locPtr == buf))
      result = FALSE;                                 /* something of the above occured */
   else { /* successful conversion */
      if ((longVal < INT_MIN) || (INT_MAX < longVal))
	 result = FALSE;
      else { /* valid integer received */
	 result  = TRUE;
	 *val    = (int)longVal;
	 *endPtr = locPtr;
      } /* valid integer received */
   } /* successful conversion */
   return (result);
} /* readIntVal */



/* ==================================== readRealVal =================================== */
BOOL readRealVal (DOUBLE      *val,
                  const char  *buf,
                  char       **endPtr)
{
   DOUBLE  rVal;
   BOOL    result;
   char   *locPtr;

   /* readRealVal */
   errno = 0;                              /* to distinguish success/failure after call */
   rVal = strtod(buf, &locPtr);
   /*    under/overflow  OR      bad conversion         OR    no digits  */
   if ((errno == ERANGE) || (errno != 0 && rVal == 0.0) || (locPtr == buf))
      result = FALSE;                                 /* something of the above occured */
   else { /* successful conversion */
      result  = TRUE;
      *val    = rVal;
      *endPtr = locPtr;
   } /* successful conversion */
   return (result);
} /* readRealVal */



/* ==================================== dumpPostBuf =================================== */
void dumpPostBuf(void)
{
   printf("\"%s\"\n%s%s\n\n", postBuf.memory, STRICH_40_TXT, STRICH_40_TXT);
} /* dumpPostBuf */



/* ================================ writeChrListHeader ================================ */
BOOL writeChrListHeader (INT  chrNumber)
{
   STR_TYPE  buf;

   /* writeChrListHeader */
   chrNumberInHdr  = chrNumber;
   actualChrNumber = 0;
   resetLocalBuffer(&chrLiBuf);
   if (chrNumber == 0)
      strcpy(buf, "0 0 0\n");
   else
      sprintf(buf, "%6d %d %d\n", chrNumber, maxIntNumber, maxDoubleNumber); 
   return (writeLocalBuffer(buf, strlen(buf), &chrLiBuf) == strlen(buf));
} /* writeChrListHeader */



/* ================================ updateChrListHeader =============================== */
BOOL updateChrListHeader (INT  chrNumber)
{
   STR_TYPE  buf;
   INT       ii;
   BOOL      res;

   /* updateChrListHeader */
   res = FALSE;
   if ((chrNumber < 999999) && (chrLiBuf.usedSize > 6))       /* chromosome amount is   */
   { /* new chromosome amount fits into the header */         /* stored by 6 characters */
      chrNumberInHdr  = chrNumber;
      sprintf(buf, "%6d", chrNumber);
      for (ii = 0;  ii < 6;  ii++)
	 chrLiBuf.memory[ii] = buf[ii];                    /* copy new amount to header */
      res = TRUE;
   } /* new chromosome amount fits into the header */
   else
      err_state = fehler(LIST_IO, 1, chrNumber, 999999, CHROMOSOMES_TXT, TOO_MANY_ITEMS_ERR);
   return (res);
} /* updateChrListHeader */



/* ================================== writeChrToList ================================== */
BOOL writeChrToList (CHAIN_ELEM_PTR_TYPE  chrPtr,
		     INT                  chrAddr1,
		     INT                  chrAddr2)
{
   CHAIN_ELEM_PTR_TYPE  itemPtr;
   ACT_DESCR_PTR_TYPE   geneDescrPtr;
   STRING_TYPE          buffer;
   STR_TYPE             buf;
   INT                  chrLen,
                        ii;
   BOOL                 res;

   /* writeChrToList */
#ifdef PTR_CHECK
   if (chrPtr == NULL) {
      err_state = fatal(LIST_IO, 10, NO_INFO, NO_INFO, "", NIL_PTR);
      return (FALSE);
   }
#endif /* PTR_CHECK */
   chrLen = chrPtr->u_el.u_xel.u_hdr.chain_length;
   sprintf(buffer, "%d %d %d\n", chrAddr1, chrAddr2, chrLen);  /*write chromosome header*/
   itemPtr = chrPtr->u_el.next_action;
   res     = TRUE;
   while ((itemPtr != NULL) && res)
   { /* write one gene per line after the other */
      if (get_act_descr(&geneDescrPtr, (SHORT)itemPtr->u_el.activity, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
      { /* gene descriptor received */
	 sprintf(buf, "%d ", itemPtr->u_el.activity);
	 strcat(buffer, buf);
	 for (ii = 0;  ii < geneDescrPtr->i_par_anz;  ii++) 
         { /* all integer parameters of this gene */
	    sprintf(buf, " %d", itemPtr->u_el.u_xel.u_act.i_params[ii]);
	    strcat(buffer, buf);
	 } /* all integer parameters of this gene */
	 for (ii = geneDescrPtr->i_par_anz;  ii < maxIntNumber;  ii++) 
	    strcat(buffer, " 0");         /* fill up maxIntNumber parameters with zeros */
	 if (MAX_STRING <= (strlen(buffer) +  geneDescrPtr->r_par_anz * (erg_genauigkeit + 7) +
			    1 + (maxDoubleNumber - geneDescrPtr->r_par_anz) * 2)) /* +1: '\n' */
	 { /* data will not fit completely into the buffer: write it to chrLiBuf! */
	    res = writeLocalBuffer(buffer, strlen(buffer), &chrLiBuf) == strlen(buffer);
	    strcpy (buffer, "");                                        /* clear buffer */
	 } /* data will not fit completely into the buffer: write it to chrLiBuf! */
	 if (res)
	 { /* write real parameters and write buffer to chrLiBuf */
	    for (ii = 0;  ii < geneDescrPtr->r_par_anz;  ii++) 
	    { /* all real parameters of this gene */
	       sprintf(buf, " %.*g", erg_genauigkeit, itemPtr->u_el.u_xel.u_act.r_params[ii]);
	       strcat(buffer, buf);
	    } /* all real parameters of this gene */
	    for (ii = geneDescrPtr->r_par_anz;  ii < maxDoubleNumber;  ii++) 
	       strcat(buffer, " 0");   /* fill up maxDoubleNumber parameters with zeros */
	    strcat(buffer, "\n");                                  /* one line per gene */
	    res = writeLocalBuffer(buffer, strlen(buffer), &chrLiBuf) == strlen(buffer);
	 } /* write real parameters and write buffer to chrLiBuf */
	 strcpy (buffer, "");                                           /* clear buffer */
      } /* gene descriptor received */
      else
	 res = FALSE;
      itemPtr = itemPtr->u_el.next_action;                                 /* next gene */
   } /* write one gene per line after the other */
   if (res)
      actualChrNumber++;
   else
      err_state = fatal(LIST_IO, 20, NO_INFO, NO_INFO, CHR_ENTRY_TXT, CHROMOSOME_LIST_ERR);
   return (res);
} /* writeChrToList */



/* ================================ sendChrList2OptServ =============================== */
BOOL sendChrList2OptServ (void)
{
   BOOL  res;

   /* sendChrList2OptServ */
   res = FALSE;
   if (chrNumberInHdr == actualChrNumber) 
   { /* number of list entries match */
#ifdef IO_DBG
      printf("sendChrList2OptServ: Vor dem Senden der ChrList\n");  fflush(stdout);
#endif /* IO_DBG */
      res = sendChromosomeList(FALSE);           /* FALSE: first trial to send the list */
      if (!res) { /* cannot send chromosome list */
	 sim_up = FALSE;                        /* simlator not ready if sending failed */
	 err_state = fehler(LIST_IO, 30, NO_INFO, NO_INFO, CHR_LIST_TXT, GENERAL_WRITE_ERR);
      } /* cannot send chromosome list */
#ifdef MITSIMULOG
      sprintf(logBuf, CHR_LIST_SENT_RES, actualChrNumber, res);
      write_to_simulog(logBuf, CLOSE_LOG);
  #ifdef IO_DBG
      printf("%s\n", logBuf);  fflush(stdout);
  #endif /* IO_DBG */
#endif /* MITSIMULOG */
   } /* number of list entries match */
   else 
      err_state = fehler(LIST_IO, 31, chrNumberInHdr, actualChrNumber, CHR_NUMBER_TXT, BAD_VALUE_ERR);      
   resetLocalBuffer(&chrLiBuf);
   return (res);
} /* sendChrList2OptServ */



/* ================================= logStartOfResBuf ================================= */
static void logStartOfResBuf (const char *badItem)
/*----------------------------------------------------------------------------------------
   Writes a header line containing "badItem" to the logfile. This is followed by at
   maximum MAX_STRING characters of the "resBuf" starting at the "errStartPos" of that
   buffer. The buffer of "resBuf" is a null-terminated string.
----------------------------------------------------------------------------------------*/
{
   sprintf(logBuf, BAD_BUF_HDR_TXT, badItem);
   write_to_log(logBuf, KEEP_LOG_OPEN);                       /* "resBuf" contains a    */
   strncpy(logBuf, resBuf.errStartPos, MAX_STRING);           /* null-terminated string */
   logBuf[MAX_STRING - 1] = '\0';  /* logging of MAX_STRING = 256 characters at maximum */
   write_to_log(logBuf, KEEP_LOG_OPEN);
   write_to_log("--------------------- End of buffer ----------------------------", CLOSE_LOG);
} /* logStartOfResBuf */



/* ================================ getResultListHeader =============================== */
BOOL getResultListHeader (INT *retStatus,
                          INT *entryNumber)
{
   INT   resValNumber;                        /* number of result values per list entry */
   BOOL  result;

   /* getResultListHeader */
   resBuf.errStartPos = resBuf.nextPos;         /* save start of reading for error msgs */
   result = readIntVal(retStatus, resBuf.memory, &(resBuf.nextPos));
   if (result) 
   { /* return status successfully read */
      if (*retStatus < 0) 
      { /* simu. service reports error */    /* "result" and "sim_up" are set to FALSE: */
	 result = generateErrMsg(LIST_IO, 40, *retStatus, RES_LIST_STATUS_ERR);
#ifdef MITSIMULOG
	 write_to_simulog(EXT_SIMU_ERR_MSG, KEEP_LOG_OPEN);
	 write_to_simulog(resBuf.memory, KEEP_LOG_OPEN);
	 write_to_simulog(STRICH_40_TXT, CLOSE_LOG);
#endif /* MITSIMULOG */
      } /* simu. service reports error */
      else 
      { /* simulation service returned well */
	 result = readIntVal(entryNumber, resBuf.nextPos, &(resBuf.nextPos));
	 if (result) 
	    result = readIntVal(&resValNumber, resBuf.nextPos, &(resBuf.nextPos));
	 if (!result)
         { /* bad result list header */
	    err_state = fehler(LIST_IO, 41, NO_INFO, NO_INFO, RES_LIST_TXT, BAD_LIST_HEADER_ERR);
	    logStartOfResBuf(RES_LIST_HDR_TXT);
	 } /* bad result list header */
      } /* simulation service returned well */
   } /* return status successfully read */
   else
   { /* bad result list header: no return status */
      err_state = fehler(LIST_IO, 42, NO_INFO, NO_INFO, RES_LIST_TXT, BAD_LIST_HEADER_ERR);
      sim_up    = FALSE;                 /* this causes a call to ext_simu_hochfahren() */
      logStartOfResBuf(RES_LIST_HDR_TXT);
   } /* bad result list header: no return status */
   if (result) 
   { /* all three values read */
      if (resValNumber != akt_anz_roh_erg_werte) 
      { /* wrong number of result values per entry */
	 err_state = fehler(LIST_IO, 43, akt_anz_roh_erg_werte, resValNumber, 
			   RES_NUMBER_TXT, BAD_VALUE_ERR);
	 result = FALSE;
      } /* wrong number of result values per entry */
#ifdef MITSIMULOG
      sprintf(logBuf, RESLI_REC_RESULT, *entryNumber, result);
      write_to_simulog(logBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
   } /* all three values read */
   return (result);
} /* getResultListHeader */



/* ================================== getNextResults ================================== */
BOOL getNextResults (DOUBLE *resDataList,
                     INT    *parentID,
                     INT    *childID)
{
   INT   ii;
   BOOL  result;

   /* getNextResults */
   resBuf.errStartPos = resBuf.nextPos;         /* save start of reading for error msgs */
   result = readIntVal (parentID, resBuf.nextPos, &(resBuf.nextPos));
   if (result)
   { /* parent ID read */
      result = readIntVal (childID, resBuf.nextPos, &(resBuf.nextPos));
      for (ii = 0;  (ii < akt_anz_roh_erg_werte) && result;  ii++)
	 result = readRealVal (&(resDataList[ii]), resBuf.nextPos, &(resBuf.nextPos));
   } /* parent ID read */
   if (!result) { /* bad results */
      err_state = fehler(LIST_IO, 50, NO_INFO, NO_INFO, RES_DATA_TXT, GENERAL_READ_ERR);
      logStartOfResBuf(RES_DATA_ENTRY_TXT);
   } /* bad results */
   return (result);
} /* getNextResults */



/* ================================= setServerPraeFix ================================= */
void setServerPraeFix (void)
{
#ifdef PAR_POP_VERS
   sprintf(serverBuffer, "%s%s%s%s/%d", ACCESS_METHOD, extSimServDomainName, PORT_TXT, ROOT_DIR, nodeNumber);
#else /* no PAR_POP_VERS */
   sprintf(serverBuffer, "%s%s%s%s", ACCESS_METHOD, extSimServDomainName, PORT_TXT, ROOT_DIR);
#endif /* no PAR_POP_VERS */
   if (strlen(lastSimServDomainName)) /* only at program start or if domain name changed*/
      write_to_log("\n----------------------------------------------------------", KEEP_LOG_OPEN);
   sprintf(logBuf, URL_BEGIN_TXT, serverBuffer);
   write_to_log(logBuf, CLOSE_LOG);
#ifdef MITSIMULOG
   write_to_simulog(logBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
} /* setServerPraeFix */



/* ================================== waitForChrList ================================== */
INT waitForChrList (INT *maxIntAmount,
                    INT *maxRealAmount)
{
   STRING_TYPE  buffer;             /* buffer for file reading and axiliary buffer      */
   FILE        *dataInFile;         /* chromosome list file                             */
   INT          chrAmount,          /* amount of chromosomes in the list                */
                bytesRead;          /* amount of read bytes                             */
   BOOL         chrLiFileOK,        /* TRUE: file successfully read and header ok       */
                somethingRead;      /* TRUE, if something was read                      */

   /* waitForChrList */
   chrAmount   = 0;
   chrLiFileOK = FALSE;
   while (!file_exists(DATA_TO_SIM))
      warte(WAIT_TIME);
   resetLocalBuffer(&chrLiBuf);
   strcpy(buffer, "");
   if (file_exists(DATA_TO_SIM_LAST))
   { /* erase file */
      if (remove(DATA_TO_SIM_LAST) == -1) 
      { /* file administration failed */
	 sprintf(logBuf, CHRLI_ERASE_ERR_TXT, DATA_TO_SIM_LAST, strerror(errno));
	 printf("%s\n", logBuf);
	 write_to_log(logBuf, CLOSE_LOG);
      } /* file administration failed */
   } /* erase file */

   if (open_file(&dataInFile, DATA_TO_SIM, "r"))   /* creates an error message on error */
   { /* chromosome list file opened */
      do
      { /* read one line */
	 somethingRead = (fgets(buffer, MAX_STRING, dataInFile) != NULL);
	 if (somethingRead)
	    bytesRead = (INT)writeLocalBuffer(buffer, strlen(buffer), &chrLiBuf);
	 else
	    bytesRead = 0;
      } /* read one line */
      while (!feof(dataInFile) && bytesRead);
#ifdef IO_DBG
      printf("\nwaitForChrList: eof=%d bytesRead=%d somethingRead=%d\n", 
	     feof(dataInFile), bytesRead, somethingRead);
      printf("\n------------------------ ChrListe ----------------------\n\"%s\"\n--------"
	     "-----------------------------------------------------\n", chrLiBuf.memory);
      fflush(stdout);
#endif /* IO_DBG */
      fclose(dataInFile);
      if (rename(DATA_TO_SIM, DATA_TO_SIM_LAST) == -1)
      { /* file administration failed */
	 sprintf(logBuf, CHRLI_RENAME_ERR_TXT, DATA_TO_SIM_LAST, strerror(errno));
	 printf("%s\n", logBuf);
	 write_to_log(logBuf, CLOSE_LOG);
      } /* file administration failed */

      if (readIntVal(&chrAmount, chrLiBuf.memory, &(chrLiBuf.nextPos)))
	 if (readIntVal(maxIntAmount, chrLiBuf.nextPos, &(chrLiBuf.nextPos)))
	    if (readIntVal(maxRealAmount, chrLiBuf.nextPos, &(chrLiBuf.nextPos)))
	    { /* chromosome list header successfully read */
#ifndef WITH_DYN_GENES  /* with genes of limited and fixed size */
	       if ((I_PAR_ANZ_MAX < *maxIntAmount) || (R_PAR_ANZ_MAX < *maxRealAmount))
	       { /* too many gene parameters! */
		  if (I_PAR_ANZ_MAX < *maxIntAmount)
		     err_state = fatal(LIST_IO, 70, *maxIntAmount, I_PAR_ANZ_MAX, "", BAD_PAR_AMOUNT);
		  if (R_PAR_ANZ_MAX < *maxRealAmount)
		     err_state = fatal(LIST_IO, 71, *maxRealAmount, R_PAR_ANZ_MAX, "", BAD_PAR_AMOUNT);
	       } /* too many gene parameters! */
	       else
#endif /* with genes of limited and fixed size */
	       { /* header is ok */
		  chrLiFileOK     = TRUE;  /* file successfully read and header data ok */
		  maxIntsOfGenes  = *maxIntAmount;
		  maxRealsOfGenes = *maxRealAmount;
	       } /* header is ok */
	    } /* chromosome list header successfully read */
      if (!chrLiFileOK)
      { /* bad header data */
	 err_state |= fehler(LIST_IO, 72, NO_INFO, NO_INFO, CHR_LIST_TXT, BAD_LIST_HEADER_ERR);
	 chrAmount = 0;
      } /* bad header data */
   } /* chromosome list file opened */
   return (chrAmount);
} /* waitForChrList */



/* =================================== getChrHdrData ================================== */
BOOL getChrHdrData (INT *parentID,
                    INT *childID,
                    INT *chrLen)
{
   BOOL  retVal;	                   /* TRUE: chromosome header successfully read */

   /* getChrHdrData */
   retVal = FALSE;
   if (readIntVal(parentID, chrLiBuf.nextPos, &(chrLiBuf.nextPos)))
      if (readIntVal(childID, chrLiBuf.nextPos, &(chrLiBuf.nextPos)))
	 retVal = readIntVal(chrLen, chrLiBuf.nextPos, &(chrLiBuf.nextPos));
   return (retVal);
} /* getChrHdrData */



/* =================================== get1GeneData =================================== */
BOOL get1GeneData (INT    *geneID,
                   INT    *intParams,
                   DOUBLE *realParams)
{
   INT   ii;                                       /* auxiliary variable                */
   BOOL  retVal;	                           /* TRUE: gene data successfully read */

   /* get1GeneData */
   if (readIntVal(geneID, chrLiBuf.nextPos, &(chrLiBuf.nextPos)))
   { /* gene id successfully read */
      retVal = TRUE;
      for (ii = 0;  (ii < maxIntsOfGenes) && retVal;  ii++)
	 retVal = readIntVal(&(intParams[ii]), chrLiBuf.nextPos, &(chrLiBuf.nextPos));
      for (ii = 0;  (ii < maxRealsOfGenes) && retVal;  ii++)
	 retVal = readRealVal(&(realParams[ii]), chrLiBuf.nextPos, &(chrLiBuf.nextPos));
   } /* gene id successfully read */
   else
      retVal = FALSE;
   return (retVal);
} /* get1GeneData */



/* =================================== get1GeneItem =================================== */
BOOL get1GeneItem (CHAIN_ELEM_PTR_TYPE  genePtr)
{
   ACT_DESCR_PTR_TYPE  thisGeneDescrPtr;           /* pointer to gene descriptor        */
   INT     geneCode,                               /* code of actual gene               */
           intVal,                                 /* read integer value                */
           ii;                                     /* auxiliary variable                */
   DOUBLE  realVal;                                /* read real value                   */
   BOOL    retVal;	                           /* TRUE: gene data successfully read */

   /* get1GeneItem */
   retVal = FALSE;
   if (readIntVal(&geneCode, chrLiBuf.nextPos, &(chrLiBuf.nextPos)))
   { /* gene id successfully read */
      if (geneCode > 0)
      { /* possibly valid gene code */                     /* creates an error message: */
	 if (get_act_descr(&thisGeneDescrPtr, geneCode, WITH_GENECODE_CHECK) == GLEAM_OK)
         { /* valid gene code */
#ifdef WITH_DYN_GENES  /* with genes of unlimited size */
	    retVal = (initGene(genePtr, geneCode) == GLEAM_OK);    
#else /* with genes of limited and fixed size */
	    genePtr->u_el.activity = geneCode;
	    retVal                 = TRUE;
#endif /* with genes of limited and fixed size */
	    for (ii = 0;  (ii < maxIntsOfGenes) && retVal;  ii++) 
	    { /* read and check one integer decision variable */
	       retVal = readIntVal(&intVal, chrLiBuf.nextPos, &(chrLiBuf.nextPos));
	       if (retVal)
		  if ((thisGeneDescrPtr->i_par_descr[ii].par_u_grenze <= intVal) &&
		      (intVal <= thisGeneDescrPtr->i_par_descr[ii].par_o_grenze))
		     genePtr->u_el.u_xel.u_act.i_params[ii] = intVal;
		  else
	          { /* value out of range */
		     err_state = fatal(LIST_IO, 80, geneCode, intVal, 
				       thisGeneDescrPtr->i_par_descr[ii].par_name, 
				       GENE_VAL_OUT_OF_RANGE_ERR);
		     retVal = FALSE;
		  } /* value out of range */
	       else
		  err_state = fatal(LIST_IO, 81, NO_INFO, NO_INFO, GENE_INT_PARAM_TXT, 
				    GENERAL_READ_ERR);
	    } /* read and check one integer decision variable */
	    for (ii = 0;  (ii < maxRealsOfGenes) && retVal;  ii++)
	    { /* read and check one real decision variable */
	       retVal = readRealVal(&realVal, chrLiBuf.nextPos, &(chrLiBuf.nextPos));
	       if (retVal)
		  if ((thisGeneDescrPtr->r_par_descr[ii].par_u_grenze <= realVal) &&
		      (realVal <= thisGeneDescrPtr->r_par_descr[ii].par_o_grenze))
		     genePtr->u_el.u_xel.u_act.r_params[ii] = realVal;
		  else
	          { /* value out of range */
		     err_state = fatal(LIST_IO, 82, geneCode, l_round(realVal), 
				       thisGeneDescrPtr->r_par_descr[ii].par_name, 
				       GENE_VAL_OUT_OF_RANGE_ERR);
		     retVal = FALSE;
		  } /* value out of range */
	       else
		  err_state = fatal(LIST_IO, 81, NO_INFO, NO_INFO, GENE_REAL_PARAM_TXT, 
				    GENERAL_READ_ERR);
	    } /* read and check one real decision variable */
	 } /* valid gene code */
      } /* possibly valid gene code */
      else
	 err_state = fatal(LIST_IO, 88, geneCode, NO_INFO, "", BAD_ACTION);
   } /* gene id successfully read */
   else
      err_state = fatal(LIST_IO, 89, NO_INFO, NO_INFO, GENE_CODE_TXT, GENERAL_READ_ERR);
   return (retVal);
} /* get1GeneItem */



/* ================================ writeResListHeader ================================ */
BOOL writeResListHeader (INT  resNumber)
{
   STR_TYPE  buf;

   /* writeResListHeader */
   resetLocalBuffer(&postBuf);
   sprintf(buf, "   0 %d %d\n", resNumber, akt_anz_roh_erg_werte);
   return (writeLocalBuffer(buf, strlen(buf), &postBuf) == strlen(buf));
} /* writeResListHeader */



/* ================================ updateChrListHeader =============================== */
void updateResListHeader (INT  badResNumber)
{
   STR_TYPE  buf;
   INT       ii;

   /* updateChrListHeader */
   if (badResNumber > 9999)
   { /* too many failed runs! */
      fehler(LIST_IO, 90, badResNumber, 9999, FAILED_SIMU_RUNS_TXT, TOO_MANY_ITEMS_ERR);
      badResNumber = 9999;
   } /* too many failed runs! */
   if (chrLiBuf.usedSize > 4)                                 /* return status is       */
   { /* new amount of failed evals fits into the header */    /* stored by 4 characters */
      sprintf(buf, "%4d", badResNumber);
      for (ii = 0;  ii < 4;  ii++)
	 postBuf.memory[ii] = buf[ii];                    /* copy new amount to header */
   } /* new amount of failed evals fits into the header */
   else
      err_state = fehler(LIST_IO, 91, badResNumber, 9999, FAILED_SIMU_RUNS_TXT, TOO_MANY_ITEMS_ERR);
} /* updateChrListHeader */



/* ================================ writeResToResList ================================ */
BOOL writeResToResList (INT     chrID1,
                        INT     chrID2,
                        DOUBLE *resValues)
{
   STRING_TYPE  buffer;
   STR_TYPE     buf;
   INT          ii;
   BOOL         ok;

   /* writeResToResList */
   sprintf(buffer, "%d %d ", chrID1, chrID2);
   if (akt_anz_roh_erg_werte < 10) 
   { /* data will fit into one buffer: write it to the buffer in one tranche */
      for (ii = 0;  ii < akt_anz_roh_erg_werte;  ii++) {
	 sprintf(buf, " %.*g", RES_VAL_PREC, resValues[ii]);
	 strcat(buffer, buf);
      }
      ok = TRUE;
   } /* data will fit into one buffer: write it to the buffer in one tranche */
   else
   { /* write data to buffer in zwo tranches */
      for (ii = 0;  ii < 9;  ii++) {
	 sprintf(buf, " %.*g", RES_VAL_PREC, resValues[ii]);
	 strcat(buffer, buf);
      }
      ok = writeLocalBuffer(buffer, strlen(buffer), &postBuf) == strlen(buffer);
      strcpy(buffer, "");
      for (ii = 9;  ii < akt_anz_roh_erg_werte;  ii++) {
	 sprintf(buf, " %.*g", RES_VAL_PREC, resValues[ii]);
	 strcat(buffer, buf);
      }
   } /* write data to buffer in zwo tranches */
   strcat(buffer, "\n");
   return (writeLocalBuffer(buffer, strlen(buffer), &postBuf) == strlen(buffer) && ok);
} /* writeResToResList */



/* ================================== generateErrMsg ================================== */
BOOL generateErrMsg (const char *modulName,
                     INT         origin,
                     INT         retStatus,
                     INT         errNo)
{
   switch (retStatus) {
      case -1: strcpy(logBuf, WRONG_TASK_ID_TXT); 
	 taskID = NIX_IDX; 
	 break;
      case -2: strcpy(logBuf, WRONG_WFID_TXT);     break;
      case -3: strcpy(logBuf, IDS_OK_GEN_ERR_TXT); break;
      case -4: strcpy(logBuf, NO_PROOF_AVAIL_TXT); break;
      default: sprintf(logBuf, "%s: %d", INVALID_REPLY_TXT, retStatus);
   }  /* end of switch */
   err_state |= fehler(modulName, origin, NO_INFO, NO_INFO, logBuf, errNo);
   sim_up     = FALSE;                   /* this causes a call to ext_simu_hochfahren() */
   return (FALSE);
} /* generateErrMsg */



/* ================================== writeLocalBuffer ================================ */
size_t writeLocalBuffer (void   *contents, 
                         size_t  size, 
                         void   *locBuf)
{
   size_t          requiredNewTotalSize;
   IO_BUFFER_TYPE *mem = (IO_BUFFER_TYPE *)locBuf;
 
   /* WriteLocalBuffer */
   requiredNewTotalSize = mem->usedSize + size + 1;
#ifdef MEM_DBG
   printf("WriteLocalBuffer: size=%d mem.usedSize=%d  requiredNewTotalSize=%d mem.totalSize=%d", 
	  (int)size, (int)mem->usedSize, (int)requiredNewTotalSize, (int)mem->totalSize);
#endif /* MEM_DBG */
   if (requiredNewTotalSize > mem->totalSize)
   { /* expand memory */
#ifdef MEM_DBG
      printf("  realloc called");
#endif /* MEM_DBG */
      mem->memory = (char*)realloc(mem->memory, requiredNewTotalSize);
      if(mem->memory == NULL) 
      { /* out of memory! */ 
	 err_state = fatal(LIST_IO, 55, NO_INFO, NO_INFO, "IO buffer", NOT_ENOUGH_MEM);
	 return 0;
      } /* out of memory! */
      mem->totalSize = requiredNewTotalSize;
   } /* expand memory */
#ifdef MEM_DBG
   printf("\n"); fflush(stdout);
#endif /* MEM_DBG */
   memcpy(&(mem->memory[mem->usedSize]), contents, size);
   mem->usedSize += size;
   mem->memory[mem->usedSize] = '\0';                         /* null-terminated string */ 
   mem->nextPos     = mem->memory;                 /* points to the begin of the buffer */
   mem->errStartPos = mem->memory;                 /* points to the begin of the buffer */
   return (size);
} /* WriteLocalBuffer */



/* ================================= createLocalBuffer ================================ */
BOOL createLocalBuffer (IO_BUFFER_TYPE *memBuf)
{
   memBuf->memory = (char*)malloc(1);  /* will be grown as needed by the call back fcn. */
   if (memBuf->memory == NULL)
      return (FALSE);
   else
   { /* memory ok */
      memBuf->nextPos     = memBuf->memory;    /* starts at the beginning of the buffer */
      memBuf->totalSize   = 0;                                        /* no data stored */ 
      memBuf->usedSize    = 0;                                        /* no data stored */
      memBuf->errStartPos = memBuf->memory;                 /* reset to begin of buffer */
      memBuf->memory[0]   = '\0';                                     /* empty string   */
   } /* memory ok */
   return (TRUE);
} /* createLocalBuffer */



/* ================================= resetLocalBuffer ================================= */
void resetLocalBuffer (IO_BUFFER_TYPE *memBuf)
{
   memBuf->nextPos     = memBuf->memory;       /* starts at the beginning of the buffer */
   memBuf->usedSize    = 0;                                           /* no data stored */
   memBuf->errStartPos = memBuf->memory;                    /* reset to begin of buffer */
   memBuf->memory[0]   = '\0';                                        /* empty string   */
} /* resetLocalBuffer */



/* ==================================== dumpResBuf ==================================== */
void dumpResBuf (const char *title)
{
   printf("\n------------------------ %s ----------------------\nresBuf: \"%s\"\n%s%s\n",
	  title, STRICH_40_TXT, STRICH_40_TXT, resBuf.memory);
} /* dumpResBuf */



/* ==================================== initListIO ==================================== */
BOOL initListIO (void)
{
   chrNumberInHdr       = 0;
   actualChrNumber      = 0;
   taskID               = NIX_IDX;
   modelWFID            = NIX_IDX;
   simuResultWaitCycles = RESULT_WAIT_TIME sec;    /* default: RESULT_WAIT_TIME seconds */
   resBuf.memory        = NULL;                            /* buffer is not yet created */
   postBuf.memory       = NULL;                            /* buffer is not yet created */
   chrLiBuf.memory      = NULL;                            /* buffer is not yet created */
   strcpy(urlComBuf, "");                                          /* clear "urlComBuf" */
   ioInit();
   sim_ok = setupCom();  /* curl library successfully initialized, communication package ready */
   return(sim_ok);
} /* initListIO */


