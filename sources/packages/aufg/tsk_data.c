/*****************************************************************************************
HyGLEAM/A                                                                           V1.0.3

                                       H y G L E A M
            Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                             Management of Configuration Files
 
                                     File: tsk_data.c                Ubuntu, gcc/g++ 5.4.0
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------

This module manages all variables and parameters, which can be controlled by the TSK- and 
the EXP-file. All parameters are identified by their names, and may have a default value
and lower and upper limits, which are checked when reading a parameter from a file. Sup-
ported data types are boolean, integer, enumeration, double, and string. Enumeration data 
consists of some key words, which are translated to integer values for internal usage.

Parameters from the EXP-file are usually mandatory and only some may be left out. A few 
from the TSK-file parameters are also mandatory. Many variables contained in the TSK file 
are relevant for the GLEAM engine (command line version) only and serve as default values 
for usually interactivly determined values, if read by the interactive variants.
Thus, all parameters which may be used by the different variants are registered, so that 
they can be read even if not required by the present GELAM variant. This is to facilitate 
the exchange and use of TSK-files.

A set of parameters apply to the HyGLEAM variant (i.e. GLEAM plus local hill climbers 
forming some MAs) only. To ease the use of TSK-files, not required parameters for pure 
GLEAM are ignored and not offered for editing in the corresponding menus.

The TSK-file has a header, which must contain the program identification (PROG_IDENT) and
the application identification, which must match with the one read from the MOD-file.
Lines starting with a "#"-sign are treated as comments. Everything in a line right from a
"#"-sign is also treated as comment. A minimal header just contains the two IDs.
The header of a saved TSK file has the following form:
  # --------------------------- Program Parameter File (TSK file) --------------------------
  <program id>  <application id>
  # written on <date and time>
  # ----------------------------------------------------------------------------------------

The header is followed by the parameter section. After that section the optional section 
containing application specific data starts with a special comment line beginning with "#!".
At present the only application making use of this, is the robot application "LESAK/Plus".

All TSK-file parameters which are either read from the file or which were altered manually 
are written to a new TSK-file.
When a new TSK file is read, all parameters contained in the file are replaced by the 
values of that file. Variables that are not contained in the file are not affected. 

The parameters are contained in the files as name-value-pairs of the form:
  <name-string> = <value> [#<comment>]
Examples:
  optimization procedure                = SMA  # simple memetic algorithm
  parameter name explainig its function = 7.5
  simulator model                       = someModel.xyz
  some string parameter                 = some string value  # explaining comment

Parameters are registered with their names, data types, and limits in the "paramList" 
array and are internally accessed by their indices of that array. Thus, there is a 
strong linkage between the name and the index of a parameter. Only registered parameters
can be read from a file successfully. 

Most of the access functions of the "paramList" are split according to the data types 
involved. New values can be set by the "processInputLin" for reading from files or by the 
"setXxxVal" functions, which are used for interactive changes. Values can be read using 
the "getXxxVal" or the "getXxxLimits" functions. The latter are used together with 
"getEnumText" for dialog or display functions.


List of exported functions:
---------------------------

BOOL readHdr (FILE       *inFile,
              BOOL        checkApplID,
              const char *fileIdBuf,
              char       *remainder);
        The header line is searched for in "inFile" and if found, analysed. It must con-
        tain the programID stored in PROG_IDENT. If "checkApplID" is set, it must also
        contain an application ID, which must match with the one found in the MOD-file. 
        The remaining string following the (last) ID found is copied to "remainder" for 
        further processing.
        If no header line is found or if one of the checks fails, FALSE is returned and 
        appropriate error messages are issued, for which "fileIdBuf" may be used. Other-
        wise, TRUE is returned. 

GSTATE readTskFile (char *fileName, 
                    BOOL *badData);
        The TSK-file denoted by "fileName" is read. The header is checked by "readHdr" 
        including a test for the application id comming from the MOD-file. If the header 
        data do not match the input is aborted and GLEAM_ERROR is returned.
        An error message is issued, if parameter names read from the file cannot 
        be found in the "paramList", if there is an I/O error, if the parameter value 
        read does not match the data type found in "paramList", or if the limits are 
        violated. In such a case, GLEAM_ERROR is returned and "badData" becomes TRUE, if 
        list entries were already altered so that it is likely to have an inconsistent 
        list now.

BOOL processInputLine (BOOL *errorFlag,
                       char *lineBuffer);
        This function is called with an input line read from a file. At first, a blank and 
        tab suppress starting left is done, followed by a search for a "#"-sign. If found 
        the string is terminated at this position as this sign starts a comment. If the 
        string is not empty now, the "="-sign is searched for. The name string on the left 
        of the "="-sign is read and searched for in the "paramList". If found the value on 
        the right of the "="-sign is read according to the stored data type. If the reading 
        was successful and the value is within the given limits (if applicable), the value 
        is stored and "paramValid" and "paramToBeSaved" are set TRUE. In case of an enume- 
        ration type its key word is stored in "stringVal" and the correspondig integer value 
        of the key word is estimated and stored in "intVal". In case of a Boolean value any
        number different from 0 is treated as TRUE. The strings 'yes' and 'no' or their  
        equivalents in other languages are also accepted, ignoring the case of the 
        characters.
        If the checks are passed, the reading was successful, and the read key word is
        known in case of enumeration types, the affected parameter is set accordingly.
        Otherwise it remains unchanged and the "errorFlag" is set to TRUE. The function
        returns TRUE, if no error occurs and no EOF and no terminating special comment "#!" 
        (cf. SECT_DELIMITER_TXT) was not encountered. Otherwise, FALSE is returned to 
        inform the calling function that the reading of the parameter section is now either 
        complete or terminated due to an error.

BOOL getFirstString (char *resultBuf,
                     char *sourceBuf);
        Searches for the first string in "sourceBuf" ignoring leading blanks and copies it
        to "resultBuf". The result string is zero-terminated. An empty source string or a 
        comment string leads to an empty result string. In these cases, the routine returns 
        FALSE, otherwise TRUE.
        The length of the source string and the position of the last handled character are 
        stored in the modul-local Variables "bufLen" and "bufPos" for further processing.

GSTATE saveTskFile (char *fileName);
        Data, which may be changed by the "Evo/Opt"- or "Simulator"-menues, is copied 
        first to "paramList" for later saving. In case of an error a cumulative warning 
        message is issued.
        The data contained in "paramList" are saved to a TSK-file denoted by "fileName". 
        If the file could not be opened or on any other I/O error, an error message is 
        issued and GLEAM_ERROR is returned.

BOOL registerParam (const char  *paramName,
                    INT          paramIdx,
                    INT          dataType,
                    DOUBLE       lowerLim,
                    DOUBLE       upperLim,
                    BOOL         editable,
                    BOOL         setDefault,
                    INT          intDef,
                    DOUBLE       doubleDef,
                    const char **enumWordList);
        Initialization of an entry in the "paramList". The new parameter is entered in the
        "paramList" at the position given by "paramIdx", which is checked for validity. It 
        is also checked that there is no entry with the same name. If ok, the limits and 
        the property of being editable are stored. If "setDefault" is set, "intDef" or 
        "doubleDef" is ckecked against the limits according to "dataType". If ok, the 
        default value is set and "paramValid" becomes TRUE. Default values for strings are 
        not accepted by this function.
        For enumeration data the corresponding key word is searched for and stored in 
        "stringVal", if found. Otherwise, a fatal error message is issued.
        If one of the mentioned checks fails an error message (GLEAM_FATAL) is issued and 
        FALSE is returned. Otherwise, the function returns TRUE.


BOOL setBoolVal   (BOOL        newVal,
                   INT         paramIdx);

BOOL setIntVal    (INT         newVal,
                   INT         paramIdx);

BOOL setEnumVal   (const char *newVal,
                   INT         paramIdx);

BOOL setDoubleVal (DOUBLE      newVal,
                   INT         paramIdx);

BOOL setStringVal (const char *newVal,
                   INT         paramIdx);
        The "paramIdx" is tested for validity as well as the consistency of the stored 
        data type "paramDataType". Furthermore, the limits are ckecked except for 
        "setBoolVal" and "setStringVal". For "setBoolValue" every value except 0 is 
        treated as TRUE. A string must not exceed a length of MAX_STRING and is truncated 
        without notification otherwise.
        Enumeration values can be stored using both, "setIntVal" and "setEnumVal" and in 
        both cases the pair of corresponding index and key word is determined and stored, 
        if found. Otherwise an error message is issued.
        Only if everything is ok, the data list entry is updated, "paramToBeSaved" is set, 
        and TRUE is returned. FALSE otherwise.

void setParamEditable (BOOL  newVal,
                       INT   paramIdx);
        The "paramIdx" is tested for validity and if ok, the parameter property "editable"
        is set to "newVal", provided that "paramValid" is set. Errors are reported.

BOOL getParamData (char         *paramName,
                   INT          *paramType,
                   const char ***enumList,
                   INT           paramIdx);
        The "paramIdx" is tested for validity as well as for the existance of the corre-
        sponding parameter. If ok, parameter name, type, and the pointer to the 
        "enumNameList" are returned and the function  returns TRUE. Otherwise FALSE is 
        returned.


BOOL getIntLimits    (INT    *lowerLim,
                      INT    *upperLim,
                      INT     paramIdx);

BOOL getDoubleLimits (DOUBLE *lowerLim,
                      DOUBLE *upperLim,
                      INT     paramIdx);
        The "paramIdx" is tested for validity as well as the consistency of the stored 
        data type "paramDataType". If ok, the stored limits are returned and the function 
        returns TRUE.  Otherwise, FALSE is returned.
        "getIntLimits" can be used for enumeration data also.


BOOL getBoolVal   (BOOL   *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel);

BOOL getIntVal    (INT    *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel);

BOOL getEnumText  (char   *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel);

BOOL getDoubleVal (DOUBLE *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel);

BOOL getStringVal (char   *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel);
        The "paramIdx" is tested and if ok, the matching of the data type ("paramDataType") 
        is checked. If not ok, a fatal error is issued and FALSE is returned. 
        Otherwise, the availability of the parameter ("paramValid") is checked and if 
        available,  the stored value is returned in "retVal" and the function returns 
        TRUE. Otherwise, "retVal" remains unchanged, FALSE is returned and according to 
        "invalidDataErrLevel" no message is issued for GLEAM_OK, an error message for 
        GLEAM_ERROR and a fatal error message for GLEAM_FATAL.
        For enumeration values two access functions are available: getIntVal returns the 
        integer representation of the value, while getEnumText returns the string of that
        enumeration value.


BOOL getEnumIndexFromParIdx (INT        *retVal,
                             const char *enumText,
		             INT         parIdx);
        The "paramIdx" is tested and if ok, it is checked whether the data type is enume-
        ration. If ok, "enumText" is searched for in "enumNameList" of the entry. Its index 
        is returned in "retVal", if found. In this case the function returns TRUE. Other-
        rwise an error message is issued, "retVal" is set to NIX_IDX, and FALSE is returned.

INT getMaxNameLength (void);
        Liefert den Wert von "maxNameLength" ab.

void prepareTskDataDisplay (BOOL  withIndices,
                            BOOL  allParam);     
        Preparation of the display of the stored entries in "paramList" using the text 
        buffer from package "fbhm", which is cleard first. The entries are sorted according 
        to their data types. If "withIndices" is set, the index in "paramList" is prepended 
        to each entry to allow access in TEXT_MODE. These Indices are written to 
        "paramListActEntries" and their amount is stored in "paramListActEntryAmount" for
        the access in GUI_MODE. If "allParam" is not set, only editable parameters are shown.

void mod_dir_mecker (STR_TYPE  mod_dir_buf);
        Erzeugt eine Fehlermeldung wegen fehlerhaftem Simulationsmodellnamen 
        "mod_dir_buf". 
        This function is no package export but part of the package local routines.

BOOL registerTskFileParameter (void);
        The function registers all parameters contained in the TSK-file using "registerParam". 
        Per parameter the following items are initialized: Name, data type, lower, and upper 
        limit. Optionally, default values can be given.
        The function returns TRUE, if all calls to "registerParam" return successfull and
        FALSE otherwise.

GSTATE processReadTskData (BOOL  dataFromFile);
        Read-in or interactively changed program parameters are assigned to the corresponding 
        variables. If "dataFromFile" is set all available parameters are assigned. If not
        only those parameters are changed, which are regarded as editable by the program 
        parameter menu. Most variables are checked for their limits by the set-functions. 
        The function returns GLEAM_OK on success of all assignements and GLEAM_ERROR other-
        wise. This function is not a package export but part of the package local routines.

BOOL tskDataInit (void);
        Allocates space for the package-internal data structures and returns TRUE on 
        success. Otherwise, FALSE is returned and a fatal error message is issued.

------------------------------------------------------------------------------------------
List of changes:
 1. New program parameter WITH_TEXTCHR_FILE for reading initial textual 
    chromosome files.                                                        wj 04/18/2018
 2. Transfer of "mit_rz" and "statistik_pro_gen" from "simu"-package.        wj 07/17/2018
 3. Removal of possible buffer overflows indicated by the new gcc. A Speci-
    fication of an LHC-index within the context of GLEAM (MIT_LSV not set)
    is only reported and does not cause an error eny more.                   wj 08/22/2018
 4. processInputLine() accepts for Boolean values the strings yes and no.    wj 09/20/2018
 5. The program parameter WITH_TEXTCHR_FILE_TXT is only evaluated if 
    WITH_DYN_GENES is not set.                                               wj 06/27/2019
 6. processInputLine(): Avoiding buffer overflow of static strings.          wj 07/18/2020
 7. Transfer of "mit_statistik" from module "evo_steu.c" of package "evo".   wj 08/11/2020
 8. Removal of program parameter OPAL_MAX_PRODTIME.                          wj 08/28/2020
------------------------------------------------------------------------------------------
Status: o.k.                          Author: W.Jakob                     Date: 08/11/2020
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <values.h>
#include <float.h>
#include <errno.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "chio.h"
#include "hmod.h"
#include "bew.h"
#ifdef EXT_SIMU_SERV
  #include "webIO.h"
#endif /* EXT_SIMU_SERV */
#include "appl.h"
#include "joints.h"
#include "simu.h"
#include "evo.h"
#include "aufg.h"
#include "aufgInt.h"

#include GLOBAL_TEXTS
#include AUFG_TEXTS


#undef PARAM_LIST_DBG




/* =============================== Exportierte Variable: ============================== */
INT   paramListActEntries[PARAM_LIST_LENGTH], /* list of indices for access in GUI_MODE */
      paramListActEntryAmount;                /* number of entries in above list        */
BOOL  mit_statistik,                  /* TRUE = acquisition of statistical data         */
      statistik_pro_gen,              /* acquisition of statistical data per generation */
      mit_rz;                         /* True if display of elapsed runtime of ext.simu */
char *sim_root_dir_name;              /* name of the root-directory of simulator models */



/* ============================= Package-globale Variable: ============================ */



/* ================================= Lokale Konstante: ================================ */
#define MIN_LEN                    6  /* for display of boolean and integer values      */
#define DOUBLE_LEN                10  /* # of significant digits for doubles in tsk-file*/
#define MAX_NUMBER_OF_ENUM_WORDS   7  /* max. number of values of an enumeration type   */


/* ============================= Lokale Typdeklarationen: ============================= */
typedef struct PROG_PARAM_TYPE
{
   STR_TYPE     paramName;        /* name, which may contain blanks                     */
   INT          paramDataType;    /* indicates the data type of the parameter           */
   BOOL         paramValid,       /* either read, interactively set or has a default    */
	        editable,         /* editable in interactive variants                   */
                paramToBeSaved;   /* it was either read or interactively set            */
   INT          intVal;           /* value of boolean or integer parameter              */
   DOUBLE       doubleVal,        /* value of double parameter                          */
                lowerLimit,       /* lower limit of a numeric value                     */
                upperLimit;       /* upper limit of a numeric value                     */
   char        *stringVal;        /* value of string parameter OR enumeration key word  */
   const char **enumNameList;     /* array of names of enumeration items for enum types */
} PROG_PARAM_TYPE;



/* ================================= Lokale Variable: ================================= */
static const char *optProcNames[]    = {OPT_PROC_GLEAM_TXT,     OPT_PROC_SMA_TXT,      OPT_PROC_ASMA_TXT, 
		                        OPT_PROC_AMMA_TXT,      OPT_PROC_LSV_TXT,      "", ""};
static const char *initProcNames[]   = {INIT_PROC_NEW_TXT,      INIT_PROC_BEST_TXT,    INIT_PROC_MIX_TXT,
                                        INIT_PROC_BEST_NEW_TXT, INIT_PROC_LHC_NEW_TXT, INIT_PROC_LHC_BEST_TXT,
                                        INIT_PROC_FILE_TXT    };
static const char *acceptRuleNames[] = {ACCEPT_RULE_ALW_TXT,    ACCEPT_RULE_LL_TXT,    ACCEPT_RULE_BP_TXT, 
                                        ACCEPT_RULE_ALW_ES_TXT, ACCEPT_RULE_LL_ES_TXT, "", ""};
static const char *adaptSpeedNames[] = {SPEED_FAST_TXT,         SPEED_MEDIUM_TXT,      SPEED_SLOW_TXT, 
                                        "",                     "",                    "", ""};

static PROG_PARAM_TYPE *paramList;     /* all parameters readable from MOD- or TSK-file */
static INT              maxNameLength, /* maximum length of parameter names registered  */
                        bufLen,        /* length of the input buffer                    */
                        bufPos;        /* last position within the input buffer         */
static FILE            *tskFile;       /* TSK-File                                      */
static char            *applIdStr;     /* application ID of the previously read TSK-File*/



/* ================================= Lokale Routinen: ================================= */
static BOOL getNameIdx      (INT         *idx,
                             const char  *name);

static BOOL checkIdxAndType (INT         idx,
                             INT         actType,
                             INT         errNumber);

static BOOL readJoints      (JOINTS      wert);

static void delIPs          (void);

static void writeJoints     (char       *txt,
                             JOINTS      joint_var);

static BOOL getEnumIndex    (INT         *retVal,
                             const char  *enumText,
		             const char **enumValues);

static void checkAndCorrectSimuModelName (void);




/* ==================================== getNameIdx ==================================== */
static BOOL getNameIdx (INT        *index,
                        const char *name)
/*----------------------------------------------------------------------------------------
  Searches in "paramList" for the given name and returns the corresponding index in 
  "index", if it is found. In this case the function returns TRUE and FALSE otherwise.
----------------------------------------------------------------------------------------*/
{
   INT   idx;
   BOOL  notFound;

   /* getNameIdx */
   notFound = TRUE;
   for (idx = 0;  (idx < PARAM_LIST_LENGTH) && notFound;  idx++)
      notFound = (strcmp(paramList[idx].paramName, name) != 0);
   if (!notFound)
      *index = idx - 1;
   return (!notFound);
} /* getNameIdx */



/* ================================= checkIdxAndType ================================== */
static BOOL checkIdxAndType (INT   idx,
                             INT   actType,
                             INT   errNumber)
/*----------------------------------------------------------------------------------------
  The "idx" is tested for validity and if ok, the correspondence of the stored data type 
  "paramDataType" and the actual one "actType" is subsequently checked. If ok, TRUE is
  returned and FALSE otherwise. Errors are reported as fatal errors and use the numbers
  "errNumber" to "errNumber + 2".
----------------------------------------------------------------------------------------*/
{
  BOOL  result;

   /* checkIdxAndType */
   result = FALSE;
   if ((idx < 0) || (PARAM_LIST_LENGTH < idx))
      fatal(AUFG_DATA, errNumber, idx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
   else
   { /* index ok */
      if ((actType == INT_DATA) || (actType == ENUM_DATA))
	 result = ((paramList[idx].paramDataType == INT_DATA) ||
		   (paramList[idx].paramDataType == ENUM_DATA));
      else
	 result = paramList[idx].paramDataType == actType;
      if (!result)
	 if ( paramList[idx].paramDataType == NIX_IDX)
	    fatal(AUFG_DATA, errNumber+1, idx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
	 else
	    fatal(AUFG_DATA, errNumber+2, NO_INFO, NO_INFO, paramList[idx].paramName,
		  INVALID_DATA_TYPE_OF_PARAM);
   } /* index ok */
   return (result);
} /* checkIdxAndType */
            
            

/* =============================== readJoints =============================== */
static BOOL readJoints (JOINTS  joint_var)
/*----------------------------------------------------------------------------*/
/*   Liest MAXAXISANZ Werte in "joint_var" ein und liefert FALSE bei einem    */
/*   I/O-Fehler.                                                              */
/*----------------------------------------------------------------------------*/
{
   INT          ii, kk,
                io_res;
   STRING_TYPE  buffer;
   char         ch,
               *dummy;
   BOOL         neue_struktur = FALSE;
   
   /* readJoints */
   io_res = 1;
   ch = fgetc(tskFile);
   if ((ch == 'S') || (ch == 'Z') || (ch == 'i') || (ch == 'P'))
   { /* neue Struk. Start/Ziel/IP */
      neue_struktur = TRUE;
      do 
         ch = fgetc(tskFile);                              /* Text ueberlesen */
      while (ch != ' ');
   } /* neue Struk. Start/Ziel/IP */
   for (ii = 0;  (ii < MAXAXISANZ) && (io_res == 1);  ii++)
      io_res = fscanf (tskFile, "%lf", &(joint_var[ii]));
   dummy = fgets (buffer, MAX_STRING+1, tskFile);        /* Skip rest of line */
   if (neue_struktur)
   {
      dummy = fgets (buffer, MAX_STRING+1, tskFile);   /* Skip Angabe in Grad */
      kk = 4;
      while (kk < ii)
      {
         dummy = fgets (buffer, MAX_STRING+1, tskFile);  /*Skip Angabe in Grad*/
         kk += 4;
      }
   }
   return (io_res == 1);
} /* readJoints */ 



/* ================================= delIPs ================================= */
static void delIPs (void)
/*----------------------------------------------------------------------------*/
/*  Loescht die Zwischenpunktliste, auf die "IPanf" zeigt.                    */
/*----------------------------------------------------------------------------*/
{
   IPZEIGER  neuerIP;

   /* delIPs */
   if (IPanf != NULL)
   { /* IPs loeschen: */
      IPi = IPanf;
      do
      { /* IP-Kette von vorn loeschen */
         neuerIP = IPi->next;
         free (IPi);
         IPi = neuerIP;
      } /* IP-Kette von vorn loeschen */
      while (IPi != NULL);
      IPanf = NULL;
   } /* IPs loeschen */
} /* delIPs */



/* =============================== writeJoints ============================== */
static void writeJoints (char   *txt,
                          JOINTS  joint_var)
/*----------------------------------------------------------------------------*/
/*   Schreibt MAXAXISANZ Werte aus "joint_var" in das Taskfile mit 4 Werten   */
/*   pro Zeile.                                                               */
/*----------------------------------------------------------------------------*/
{
   INT  ii;
   
   /* write_joints */
   fprintf (tskFile, "%s", txt);
   for (ii = 0;  ii < MAXAXISANZ;  ii++)
   {
      fprintf (tskFile, "  %5G", joint_var[ii]);
      if ((ii % 4) == 3)
         fprintf (tskFile, "\n         ");
   }
   if ((MAXAXISANZ % 4) != 0)
      fprintf (tskFile, "\n");
   fprintf (tskFile, "in Grad: ");
   for (ii = 0;  ii < MAXAXISANZ;  ii++)
   {
      fprintf (tskFile, "  %7.2lf", joint_var[ii]*RADGRAD);
      if ((ii % 4) == 3)
         fprintf (tskFile, "\n         ");
   }
   if ((MAXAXISANZ % 4) != 0)
      fprintf (tskFile, "\n");
   fprintf (tskFile, "\n");
} /* writeJoints */ 



/* ====================================== readHdr ===================================== */
BOOL readHdr (FILE       *inFile,
              BOOL        checkApplID,
              const char *fileIdBuf,
              char       *remainder)
{
   STRING_TYPE  buffer, buf;
   INT          ioRes,
                ii;
   char        *dummy;
   BOOL         idsFound,         /* TRUE, if (both) IDs were found                     */
                hdrLineFound,     /* TRUE, if header line with content found            */
                result,           /* TRUE, if ID(s) found and ok and no EOF encountered */
                eofFound;         /* TRUE, if EOF is encountered                        */

   /* readHdr */
   idsFound     = FALSE;
   eofFound     = FALSE;
   hdrLineFound = FALSE;
   result       = TRUE;
   do { /* IDs not found */
      ioRes = fscanf(inFile, "%[^\n]s", buffer);
      if (ioRes > 0)
      { /* line read */
	 if (getFirstString(buf, buffer))        /* comment line or line with contents? */
	 { /* line with content: first string found */
	    hdrLineFound = TRUE;
	    if (strcmp(buf, PROG_IDENT) != 0)                     /* ID does not match? */
	       fehler (AUFG_DATA, 200, NO_INFO, NO_INFO, buf, BAD_TSK_FILE);
	    else
	    { /* MOD-File-ID ok */
	       if (checkApplID)
	       { /* search for applID and test it */
		  strcpy(buf, "");
		  while ((bufPos < bufLen) && (buffer[bufPos] == ' '))/*blank supr. left*/
		     bufPos++;
		  if (buffer[bufPos] != '#')
		  { /* not a comment */
		     ii = 0;
		     while ((bufPos < bufLen) && (buffer[bufPos] != ' '))  /*copy string*/
			buf[ii++] = buffer[bufPos++];
		     buf[ii]  = '\0';                            /* set null terminator */
		     idsFound = check_appl_id(buf);                      /* test applID */
		  } /* not a comment */
		  if (idsFound)
		     strcpy (applIdStr, buf);         /* save valid application ID read */
		  else
		     fehler (AUFG_DATA, 201, NO_INFO, NO_INFO, buf, BAD_TSK_APPL);
	       } /* search for applID and test it */
	       else
		  idsFound = TRUE;         /* only first ID must be equal to PROG_IDENT */
	    } /* MOD-File-ID ok */   
	    strncpy(remainder, &(buffer[bufPos]), bufLen - bufPos + 1);
	 } /* line with content: first string found */
      } /* line read */ 
      else
	 eofFound = (ioRes < 0);
      dummy = fgets (buffer, MAX_STRING, inFile);                  /* skip rest of line */
   } /* IDs not found */
   while (!hdrLineFound && !eofFound);
   if (eofFound || !idsFound) {
      fehler(AUFG_DATA, 202, NO_INFO, NO_INFO, fileIdBuf, BAD_AFILE_HDR);
      result = FALSE;
   }
   return (result);
} /* readHdr */



/* ==================================== readTskFile =================================== */
GSTATE readTskFile (char *fileName, 
                    BOOL *badData)
{
   STRING_TYPE  buffer, buf;
   INT          ioRes,
                intVal;
   GSTATE       retState;
   IPZEIGER     IPalt;
   char         ch,
               *dummy;
   BOOL         errorFound,           /* TRUE on error                                  */
                terminate,            /* TRUE, if end of parameter section or error     */
                eofFound,             /* TRUE, if EOF is encountered                    */
                applOK;               /* for application specific (read) results        */

   /* readTskFile */
   *badData = FALSE;
   if (!open_file(&tskFile, fileName, "rt"))
      return (GLEAM_ERROR);

   /* ------------- read header and check program id and application id: -------------- */
   retState = GLEAM_ERROR;
   applOK   = TRUE;
   eofFound = FALSE;

   if (readHdr(tskFile, CHECK_APPL_ID_ALSO, "TSK", buf))
   { /* processing of further sections */
      *badData   = TRUE;                           /* tsk data are affected from now on */
      errorFound = FALSE;
      terminate  = FALSE;
      retState   = GLEAM_OK;
      /* ----------------------- processing of parameter section: --------------------- */
      do { /* process one line */
	 ioRes = fscanf(tskFile, "%[^\n]s", buffer);
	 if (ioRes > 0)
	    terminate = !processInputLine(&errorFound, buffer);    /* process read line */
	 else
	    eofFound = (ioRes < 0);
	 dummy = fgets (buffer, MAX_STRING, tskFile);              /* skip rest of line */
      } /* process one line */
      while (!terminate && !errorFound && !eofFound);
      if (errorFound) 
	 retState =  fehler(AUFG_DATA, 203, NO_INFO, NO_INFO, "TSK", BAD_INIT_FILE);
      else
      { /* parameter section successfully read */
	 if (!eofFound)
	 { /* process appl. data */
	    /* ----------------- processing of application specific data: ------------- */
	    switch (appl_code) {
	    case GADO_APPL:     /* -------------------- GADO_APPL --------------------- */
	       if (getIntVal(&intVal, SIMU_MAX_TIME_IDX, GLEAM_OK))
		  update_ext_simu_tmo(intVal);
	       break; /* GADO_APPL */
         
	    case LESAK_APPL:    /* -------------------- LESAK_APPL -------------------- */
	       applOK = readJoints(start_joints);
	       if (applOK)
	       { /* start joints read */
		  dummy = fgets(buffer, MAX_STRING, tskFile);      /* skip (blank) line */
		  delIPs();
		  do
                  { /* Schleife ueber alle IPs */
		     ch = fgetc(tskFile);
		     if (ch == 'I')                     /* neue Struktur, liegt IP vor? */
                     { /* neue Struktur, IP liegt vor */
			IPi = (IPZEIGER)malloc(sizeof(IPTYP));
			IPi->winkel = (double*)malloc((MAXAXISANZ+1)*sizeof(DOUBLE));
			IPi->next = NULL;
			IPi->min_dist = -1.0;                  /* als unbelegt markiert */
			applOK = readJoints(IPi->winkel);
			if (applOK) {
			   if (IPanf == NULL)
			      IPanf = IPi;
			   else
			      IPalt->next = IPi;
			   IPalt = IPi;
			   dummy = fgets(buffer, MAX_STRING, tskFile); /*skip blank line*/
			}
			else
			   retState = fehler(AUFG_DATA, 204, NO_INFO, NO_INFO, "", TSK_JOINTS_ERR);
		     } /* neue Struktur, IP liegt vor */
		  } /* Schleife ueber alle IPs */
		  while ((ch == 'I') && applOK);    
		  if (applOK)
		  { /* reading ok */
		     applOK = readJoints(ziel_joints);
		     if (applOK) {
			applOK = update_rob_task();            /* transfer of read data */
			if (!applOK)
			   retState = fehler(AUFG_DATA, 205, NO_INFO, NO_INFO, "", TSK_BAD_DATA_ERR);
		     } else
			retState = fehler(AUFG_DATA, 206, NO_INFO, NO_INFO, "", TSK_JOINTS_ERR);
		  } /* reading ok */
	       } /* start joints read */
	       else
		  retState = fehler(AUFG_DATA, 207, NO_INFO, NO_INFO, "", TSK_JOINTS_ERR);
	       break; /* LESAK_APPL */

            case SIMU_SERV_APPL:
	    case MATLAB_APPL:
	    case MATH_FKT_APPL:
	    case CEC_MATHFKT_APPL:
	       ;
	       break; /* SIMU_SERV_APPL, MATLAB_APPL, CEC_MATHFKT_APPL, MATH_FKT_APPL */
	    } /* switch */
	 } /* process appl. data */

         /* ----------------------- processing of general data: ----------------------- */
	 if (retState == GLEAM_OK)
	    retState = processReadTskData(PARAM_FROM_FILE);
	 if ((retState == GLEAM_OK) & applOK)
	    *badData = FALSE;      /* parameters and appl. spec. data successfully read */ 
	 else
	    retState |= fehler (AUFG_DATA, 208, NO_INFO, NO_INFO, "TSK", BAD_INIT_FILE);
      } /* parameter section successfully read */
   } /* processing of further sections */

   fclose (tskFile);
   return (retState);
} /* readTskFile */



/* ================================= processInputLine ================================= */
BOOL processInputLine (BOOL *errorFlag,
                       char *lineBuffer)
{
   char         msgBuf[2*MAX_STRING+MAX_STR];
   STRING_TYPE  buffer, nameBuffer;
   char        *endPtr;            /* for the call of strtol and strtod                 */
   DOUBLE       doubleVal;         /* double value read                                 */
   INT          equalPos,          /* Position of the  "="-sign                         */
                startPos,          /* position of the beginning of string or name       */
                endPos,            /* position of the right end of string or name       */
                intVal,            /* integer or boolean value read                     */
                paramIdx;          /* index pf the parameter found                      */
   BOOL         res,               /* final result of the function                      */
                ok;                /* TRUE, if no i/o and no value setting error        */

   /* processInputLine */
   *errorFlag = FALSE;
   res        = TRUE;
   bufLen     = strlen(lineBuffer);
   if (bufLen > 0) 
   { /* buffer is not empty */
      startPos = 0;
      while ((startPos < bufLen) && ((lineBuffer[startPos] == ' ') ||  /* blank and tab */
				     (lineBuffer[startPos] == '\t')  )) /* supress left */
	 startPos++;
      if (lineBuffer[startPos] == '#')               /* check for comment and if found  */
	 res = (lineBuffer[startPos+1] != '!');      /* check for parameter section end */
      else
      { /* not a comment */
	 if (startPos < bufLen)
	 { /* line with characters different from blanks */
	    *errorFlag = TRUE;
	    res        = FALSE;
	    equalPos   = startPos;
	    while ((equalPos < bufLen) && (lineBuffer[equalPos] != '='))
	       equalPos++;                                            /* search for "=" */
	    if (equalPos < bufLen)
	    { /* "="-sign found */
	       endPos = equalPos - 1;
	       while ((startPos < endPos) && ((lineBuffer[endPos] == ' ') ||   /* blank */
					      (lineBuffer[endPos] == '\t')  )) /* and   */
		  endPos--;                                        /* tab supress right */
	       if (startPos <= endPos)
	       { /* parameter name string found */
		  strncpy(nameBuffer, &(lineBuffer[startPos]), endPos - startPos + 1);
		  nameBuffer[endPos - startPos + 1] = '\0';
		  if (getNameIdx(&paramIdx, nameBuffer))
	          { /* known parameter */
                     startPos = equalPos + 1;
		     while ((startPos < bufLen) && ((lineBuffer[startPos] == ' ') || 
						    (lineBuffer[startPos] == '\t')  ))
			startPos++;                       /* blank and tab supress left */
		     if (lineBuffer[startPos] == '#')              /* check for comment */
			fehler(AUFG_DATA, 220, NO_INFO, NO_INFO, lineBuffer, BAD_TSK_FILE_LINE);
		     else
		     { /* start of content right from "="-sign found */
			ok = FALSE;  /* set TRUE, if value successfully read and stored */
			strcpy(msgBuf, "");     /* empty buffer means nothing to report */
			if (paramList[paramIdx].paramDataType == STRING_DATA)
			{ /* isolate and assign string value */
			   endPos = startPos + 1;
			   while ((endPos < bufLen) && (lineBuffer[endPos] != '#'))
			      endPos++;           /* search for "#"-sign or end of line */
			   endPos--;
			   while ((startPos < endPos) && ((lineBuffer[endPos] == ' ') || 
							  (lineBuffer[endPos] == '\t')  ))
			      endPos--;                  /* blank and tab supress right */
			   strncpy(buffer, &(lineBuffer[startPos]), endPos - startPos + 1);
			   buffer[endPos - startPos + 1] = '\0'; /*extract value string */
			   ok = setStringVal(buffer, paramIdx);    /* no error possible */
			} /* isolate and assign string value */
			else
			{ /* search for one word value (bool, int, double, enum) */
			   endPos = startPos + 1;
			   while ((endPos < bufLen) && (lineBuffer[endPos] != ' ') && 
				  (lineBuffer[endPos] != '\t') && (lineBuffer[endPos] != '#'))
			      endPos++;  /* search for first blank, or tab, or "#"-sign */
			   strncpy(buffer, &(lineBuffer[startPos]), endPos - startPos);
		           buffer[endPos - startPos] = '\0';    /* extract value string */
			   switch (paramList[paramIdx].paramDataType) {
			   case BOOL_DATA:
			   case INT_DATA:
			      errno  = 0;
			      ok     = TRUE;
			      intVal = (INT)strtol(buffer, &endPtr, 10);
			      if ((errno != 0) || (buffer == endPtr) || (*endPtr != '\0'))
			      { /* no int value */
				 if (paramList[paramIdx].paramDataType == BOOL_DATA)
			         { /* Boolean value expected */
				    if ((strcasecmp(buffer, JA_TXT)   == 0) || 
					(strcasecmp(buffer, NEIN_TXT) == 0)   )
				    { /* 'yes' or 'no' found */
				       if (strcasecmp(buffer, JA_TXT) == 0)
					  intVal = 1;
				       else
					  intVal = 0;
				    } /* 'yes' or 'no' found */
				    else
				       ok = FALSE;
				 } /* Boolean value expected */
				 else
				    ok = FALSE;
			      } /* no int value */
			      if (!ok)
				 sprintf(msgBuf, BAD_VAL_MSG_TXT, nameBuffer, buffer);
			      else 
                              { /* value successfully read */
				 if (paramList[paramIdx].paramDataType == BOOL_DATA)
				    ok = setBoolVal(intVal, paramIdx);  /* no error possible */
				 else
				 { /* int data parameter */
#ifndef MIT_LSV /* no LHCs */
				    if (paramIdx == LHC_INDEX_IDX)
				    { /* special case: pure gleam does not support LHCs */
				       sprintf(msgBuf, UNSUITED_PARAM_TXT, LHC_INDEX_TXT);
				       meldung(AUFG_DATA, NO_INFO, NO_INFO, msgBuf, 
					       GENERAL_MESSAGE);
				       strcpy(msgBuf, "");
				       intVal = 0;
				    } /* special case: pure gleam does not support LHCs */
#endif /* no LHCs */
				    ok = setIntVal(intVal, paramIdx);
				 } /* int data parameter */
			      } /* value successfully read */
			      break; /* bool or int data */

			   case ENUM_DATA:
			      ok = setEnumVal(buffer, paramIdx);
			      if (!ok)
				 sprintf(msgBuf, BAD_VAL_MSG_TXT, nameBuffer, buffer);
			      break; /* ENUM_DATA */

			   case DOUBLE_DATA:
			      errno = 0;
			      doubleVal = strtod(buffer, &endPtr);
			      if ((errno != 0) || (buffer == endPtr) || (*endPtr != '\0'))
				 sprintf(msgBuf, BAD_VAL_MSG_TXT, nameBuffer, buffer);
			      else  /* value successfully read */
				 ok = setDoubleVal(doubleVal, paramIdx);
			      break; /* DOUBLE_DATA */

		              default:
		   	         fehler(AUFG_DATA, 223, NO_INFO, NO_INFO, nameBuffer, 
					INVALID_DATA_TYPE_OF_PARAM);
				 ok = FALSE;
			   } /* end switch */
			} /* search for one word value (bool, int, double, enum) */
			if (ok) { /* value successfully read and stored */
			   *errorFlag = FALSE;
			   res        = TRUE;
			} /* value successfully read and stored */
			else
			   if (strlen(msgBuf) > 0)
			      fehler(AUFG_DATA, 224, NO_INFO, NO_INFO, msgBuf, GENERAL_MESSAGE);
		     } /* start of content right from "="-sign found */
		  } /* known parameter */
		  else
		     fehler(AUFG_DATA, 225, NO_INFO, NO_INFO, nameBuffer, PARAM_NOT_FOUND);
	       } /* parameter name string found */
	       else
		  fehler(AUFG_DATA, 226, NO_INFO, NO_INFO, lineBuffer, BAD_TSK_FILE_LINE);
	    } /* "="-sign found */
	    else
	       fehler(AUFG_DATA, 227, NO_INFO, NO_INFO, lineBuffer, BAD_TSK_FILE_LINE);
	 } /* line with characters different from blanks */
      } /* not a comment */
   } /* buffer is not empty */
   return (res);
} /* processInputLine */



/* ================================== getFirstString ================================== */
BOOL getFirstString (char *resultBuf,
                     char *sourceBuf)
{
   INT   ii;
   BOOL  res;
   
   /* getFirstString */
   strcpy(resultBuf, "");
   bufLen = strlen(sourceBuf);
   res    = FALSE;
   if (bufLen > 0) 
   { /* buffer not empty */
      bufPos = 0;
      while ((bufPos < bufLen) && (sourceBuf[bufPos] == ' '))    /* blank supress left  */
	 bufPos++;
      if (sourceBuf[bufPos] != '#')
      { /* not a comment */
	 ii = 0;
	 while ((bufPos < bufLen) && (sourceBuf[bufPos] != ' ')) /* copy string         */
	    resultBuf[ii++] = sourceBuf[bufPos++];
	 resultBuf[ii] = '\0';                                   /* set null terminator */
	 res = ii > 0;
      } /* not a comment */
   } /* buffer not empty */
   return (res);
} /* getFirstString */



/* =================================== saveTskFile ==================================== */
GSTATE saveTskFile (char *fileName)
{
   STR_TYPE  timeStampBuf, buf;
   DOUBLE    dVal;
   INT       lhcSpeed, levelSpeed,
             idx, iVal, ii;
   BOOL      ok, bVal;

   /* saveTskFile */
   if (!open_file(&tskFile, fileName, "wt"))
      return (GLEAM_ERROR);

   ok = TRUE;
   getIntVal(&iVal,                           DEME_SIZE_IDX,         GLEAM_OK);
   if (iVal != anz_nachbarn)
      ok &= setIntVal(anz_nachbarn,           DEME_SIZE_IDX);
   getIntVal(&iVal,                           ACCEPTANCE_RULE_IDX,   GLEAM_OK);
   if (iVal != default_survival_rule)
      ok &= setIntVal(default_survival_rule,  ACCEPTANCE_RULE_IDX);
   getDoubleVal(&dVal,                        RANKING_PAR_IDX,       GLEAM_OK);
   if (dVal != def_max_fit)
      ok &= setDoubleVal(def_max_fit,         RANKING_PAR_IDX);
   getDoubleVal(&dVal,                        XO_HAMDIST_IDX,        GLEAM_OK);
   if (fabs(dVal - def_xo_min_ham_dist * 100.0) > 0.001)
      ok &= setDoubleVal(def_xo_min_ham_dist * 100.0, XO_HAMDIST_IDX);
   if (mit_gen_rep_par) {
      getDoubleVal(&dVal,                     GEN_REP_RATE_IDX,      GLEAM_OK);
      if  (fabs(dVal -  gen_rep_par* 100.0) > 0.001)
	 ok &= setDoubleVal(gen_rep_par * 100.0, GEN_REP_RATE_IDX);
   }
   if (mitOptionPhaenoRep) {
      getBoolVal(&bVal,                       WITH_PHAENO_REP_IDX,   GLEAM_OK);
      if (bVal != mitPhaenoRepair)
	 ok &= setBoolVal(mitPhaenoRepair,    WITH_PHAENO_REP_IDX);
   }
   getBoolVal(&bVal,                          WITH_SIMU_LOG_IDX,     GLEAM_OK);
#ifdef MITSIMULOG
   if (bVal != mit_simulog)
      ok &= setBoolVal(mit_simulog,           WITH_SIMU_LOG_IDX);
#endif /* MITSIMULOG */

#ifdef MIT_LSV
   getIntVal(&iVal,                           OPT_PROCEDURE_IDX,     GLEAM_OK);
   if (iVal != def_opt_strat)
       ok &= setIntVal(def_opt_strat,         OPT_PROCEDURE_IDX);
   if (lsv_anz > 0)
   { /* present application (see MOD-File) uses LHCs */
      getIntVal(&iVal,                        LHC_INDEX_IDX,         GLEAM_OK);
      if ((iVal != def_lsv_strat) && (def_lsv_strat != KEIN_LSV))
       ok &= setIntVal(def_lsv_strat,         LHC_INDEX_IDX);
      if (lsv[def_lsv_strat].lsvParAnz > 0) 
      { /* Default-LHC has at least one parameter */
	 getIntVal(&iVal,                     LHC_ITER_LIMIT_IDX,    GLEAM_OK);
	 if (iVal != lsv[def_lsv_strat].lsvPar[0])
	    ok &= setIntVal(lsv[def_lsv_strat].lsvPar[0], LHC_ITER_LIMIT_IDX);
	 if (lsv[def_lsv_strat].lsvParAnz > 1)
         { /* Default-LHC has at least two params: the second one is handled */
	    getDoubleVal(&dVal,               LHC_TERM_LIMIT_IDX,    GLEAM_OK);
	    if (fabs(dVal - lsv[def_lsv_strat].lsvPar[1]) > 1.0e-9)  /* smallest value for Rosenbrock */
	       ok &= setDoubleVal(lsv[def_lsv_strat].lsvPar[1], LHC_TERM_LIMIT_IDX);
	 } /* Default-LHC has at least two params: the second one is handled */
      } /* Default-LHC has at least one parameter */
      getDoubleVal(&dVal,                     LAMARCK_RATE_IDX,      GLEAM_OK);
      if (fabs(dVal - def_lamarck_rate * 100.0) > 0.001)
	 ok &= setDoubleVal(def_lamarck_rate * 100.0, LAMARCK_RATE_IDX);
      getDoubleVal(&dVal,                     OLD_DISTR_FRAC_IDX,    GLEAM_OK);
      if (fabs(dVal - oldDistribFrac * 100.0) > 0.001)
	 ok &= setDoubleVal(oldDistribFrac * 100.0, OLD_DISTR_FRAC_IDX);
      getAdaptSpeeds(&lhcSpeed, &levelSpeed);
      getIntVal(&iVal,                        LHC_ADAPT_SPEED_IDX,   GLEAM_OK);
      if (iVal != lhcSpeed)
	 ok &= setIntVal(lhcSpeed,            LHC_ADAPT_SPEED_IDX);
      getIntVal(&iVal,                        LEVEL_ADAPT_SPEED_IDX, GLEAM_OK);
      if (iVal != levelSpeed)
	 ok &= setIntVal(levelSpeed,          LEVEL_ADAPT_SPEED_IDX);
      getIntVal(&iVal,                        NUMB_OF_FCLASSES_IDX,  GLEAM_OK);
      if (iVal != getActNKlAnz())
	 ok &= setIntVal(getActNKlAnz(),      NUMB_OF_FCLASSES_IDX);
   } /* present application (see MOD-File) uses LHCs */

   /* !!!
   getDoubleVal(&dVal,                        _IDX,         GLEAM_OK);
   if (fabs(dVal -  * 100.0) > 0.001)
      ok &= setDoubleVal( * 100.0, _IDX);

   getIntVal(&iVal,                           _IDX,      GLEAM_OK);
   if (iVal != )
       ok &= setIntVal(,         _IDX);
   */
#endif /* with LHCs */

   get_time_stamp(timeStampBuf, buf, buf, buf);
   fprintf(tskFile, "# %s %s%s\n", HDR_LINE_TXT, HDR_TITLE_TEXT, HDR_LINE_TXT);
   fprintf(tskFile, "%s  %s\n", PROG_IDENT, applIdStr);
   fprintf(tskFile, "# %s %s\n", HDR_TIME_STAMP_TXT, timeStampBuf);
   fprintf(tskFile, "# ------------------------------------------------------------------"
	   "----------------------\n");
   ok = TRUE;
   for (idx = FIRST_TSK_DATA_IDX;  (idx < PARAM_LIST_LENGTH) && ok;  idx++)
   { /* all entries */
      if (paramList[idx].paramToBeSaved)
      { /* to be saved entry */
	 ok &= (fprintf (tskFile, "%-*s = ", maxNameLength, paramList[idx].paramName) > 0);
	 switch (paramList[idx].paramDataType) {
	 case BOOL_DATA:
	 case INT_DATA:
	    ok &= (fprintf(tskFile, "%*d\n", MIN_LEN, paramList[idx].intVal) > 0);
	    break; /* bool or int data */

	 case ENUM_DATA:
	    ok &= (fprintf(tskFile, "%*s   # (%d)\n", MIN_LEN, paramList[idx].stringVal, 
			    paramList[idx].intVal) > 0);
	    break; /* ENUM_DATA */

	 case DOUBLE_DATA:
	    ok &= (fprintf(tskFile, "%.*g\n", DOUBLE_LEN, paramList[idx].doubleVal) > 0);
	    break; /* DOUBLE_DATA */

	 case STRING_DATA:
	    ok &= (fprintf(tskFile, "%s\n", paramList[idx].stringVal) > 0);
	    break; /* STRING_DATA */

	 default:
	    fprintf(tskFile, "unkown data type!\n");
	    ok = FALSE;
	 } /* end switch */
	 if (!ok)
	    fehler(AUFG_DATA, 250, NO_INFO, NO_INFO, paramList[idx].paramName, TSK_OUT_ERROR);
      } /* to be saved entry */
   } /* all entries */
   if (ok) 
   { /* optional application specific data */
      switch (appl_code) {
      case GADO_APPL: /* -- GADO_APPL, SIMU_SERV_APPL, MATLAB_APPL, MATH_FKT_APPL -- */
      case SIMU_SERV_APPL:
      case MATLAB_APPL:
      case CEC_MATHFKT_APPL:
      case MATH_FKT_APPL:
	 break; /* GADO_APPL, SIMU_SERV_APPL, MATLAB_APPL, MATH_FKT_APPL */
         
      case LESAK_APPL: /* ----------------------- LESAK_APPL ----------------------- */
	 fprintf(tskFile, "%s%s\n", SECT_DELIMITER_TXT, LSK_SECTION_HDR_TXT);
	 sprintf(buf, "Start:  ");
	 writeJoints(buf, start_joints);
	 if (IPanf != NULL)  
         { /* IPs rausschreiben */
	    IPi = IPanf;
	    ii  = 1;
	    do {
	       sprintf(buf, "IP%d:     ",ii++);
	       writeJoints(buf, IPi->winkel);
	       IPi = IPi->next;                
	    }
	    while (IPi != NULL);
	 } /* IPs rausschreiben */
	 sprintf(buf, "Ziel:   ");
	 writeJoints(buf, ziel_joints);
	 if (fprintf(tskFile, "\n") < 1)
	 { /* Schreibfehler */
	    fehler (AUFG_DATA, 251, NO_INFO, NO_INFO, APPL_WERTE, TSK_OUT_ERROR);
	    ok = FALSE;
	 } /* Schreibfehler */
	 break; /* LESAK_APPL */
      } /* switch */
   } /* robot application: save start and target data */
   fclose(tskFile);
   if (ok) 
      return (GLEAM_OK);
   else
      return (GLEAM_ERROR);
} /* saveTskFile */



/* ================================== registerParam =================================== */
BOOL registerParam (const char  *paramName,
                    INT          paramIdx,
                    INT          dataType,
                    DOUBLE       lowerLim,
                    DOUBLE       upperLim,
                    BOOL         editable,
                    BOOL         setDefault,
                    INT          intDef,
                    DOUBLE       doubleDef,
                    const char **enumWordList)
{
   STR_TYPE  buf;
   INT       ii;
   GSTATE    localErrLevel;

   /* registerParam */
   localErrLevel = GLEAM_OK;
   if ((paramIdx < 0) || (PARAM_LIST_LENGTH < paramIdx))
      localErrLevel = fatal(AUFG_DATA, 1, paramIdx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
   else
   { /* index ok */
      if (getNameIdx(&ii, paramName))       /* check for a parameter with the same name */
	 localErrLevel = fatal(AUFG_DATA, 2, NO_INFO, NO_INFO, paramName, PARAM_NAME_ALREADY_EXISTS);
      else
      { /* name not contained in list */
	 strcpy(paramList[paramIdx].paramName, paramName);
	 paramList[paramIdx].paramDataType = dataType;
	 paramList[paramIdx].lowerLimit    = lowerLim;
	 paramList[paramIdx].upperLimit    = upperLim;
         paramList[paramIdx].editable      = editable;
	 if (maxNameLength < strlen(paramName))
	    maxNameLength = strlen(paramName);
	 if (setDefault)
	 { /* check and set default value */
	    paramList[paramIdx].enumNameList = NULL;
	    switch (dataType) {
	    case BOOL_DATA:
	       paramList[paramIdx].intVal = (intDef != 0);
	       break;  /* boolean data */

	    case INT_DATA:
	    case ENUM_DATA:
	       if ((lowerLim <= intDef) && (intDef <= upperLim))
		  paramList[paramIdx].intVal = intDef;
	       else
		  localErrLevel = fatal(AUFG_DATA, 3, intDef, NO_INFO, paramName, BAD_PARAM_VALUE);
               if ((dataType == ENUM_DATA) && (localErrLevel == GLEAM_OK))
	       { /* save key word */
		  free (paramList[paramIdx].stringVal);
		  if (enumWordList == NULL)
		     localErrLevel = fatal(AUFG_DATA, 4, NO_INFO, NO_INFO, "", NIL_PTR);
		  else 
		  { /* "enumWordList" received */
		     paramList[paramIdx].enumNameList = enumWordList;
		     strcpy (buf, enumWordList[intDef]);
		     paramList[paramIdx].stringVal = (char*)malloc(strlen(buf) * sizeof(char));
		     if (paramList[paramIdx].stringVal == NULL)
			localErrLevel = fatal(AUFG_DATA, 5, NO_INFO, NO_INFO, "", TSK_DATA_ANZ_ERROR);
		     else
			strcpy (paramList[paramIdx].stringVal, buf);
		  } /* "enumWordList" received */
	       } /* save key word */
	       break;  /* integer or enumeration data */

	    case DOUBLE_DATA:
	       if ((lowerLim <= doubleDef) && (doubleDef <= upperLim))
		  paramList[paramIdx].doubleVal = doubleDef;
	       else
		  localErrLevel = fatal(AUFG_DATA, 6, (LONG)doubleDef, NO_INFO, paramName, BAD_PARAM_VALUE);
	       break; /* double data */

	    case STRING_DATA:
	       localErrLevel = fatal(AUFG_DATA, 7, NO_INFO, NO_INFO, "", NO_INI);
	       break; /* string data */

	    default:
	       localErrLevel = fatal(AUFG_DATA, 8, NO_INFO, NO_INFO, paramName, INVALID_DATA_TYPE_OF_PARAM);
	    } /* end switch */
	    paramList[paramIdx].paramValid = (localErrLevel == GLEAM_OK);
         } /* check and set default value */
      } /* name not contained in list */
   } /* index ok */
   return (localErrLevel == GLEAM_OK);
} /* registerParam */



/* ==================================== setBoolVal ==================================== */
BOOL setBoolVal (BOOL  newVal,
                 INT   paramIdx)
{
   BOOL result;

   /* setBoolVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, BOOL_DATA, 20))
   { /* index and type ok */
      paramList[paramIdx].intVal         = newVal != 0;
      paramList[paramIdx].paramValid     = TRUE;
      paramList[paramIdx].paramToBeSaved = TRUE;
      result = TRUE;
   } /* index ok */
   return (result);
} /* setBoolVal */



/* ===================================== setIntVal ==================================== */
BOOL setIntVal (INT  newVal,
                INT  paramIdx)
{
   BOOL result;

   /* setIntVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, INT_DATA, 30))  /* returns TRUE for INT_ and ENUM_DATA */
   { /* index and type ok */
      if ((newVal < paramList[paramIdx].lowerLimit) || (paramList[paramIdx].upperLimit < newVal))
	 fehler(AUFG_DATA, 32, newVal, NO_INFO, paramList[paramIdx].paramName, ACT_VAL_OUT_OF_RANGE);
      else { /* ok */
	 paramList[paramIdx].intVal         = newVal;
	 paramList[paramIdx].paramValid     = TRUE;
	 paramList[paramIdx].paramToBeSaved = TRUE;
	 if (paramList[paramIdx].paramDataType == ENUM_DATA)
	 { /* enumeration data: store new enumeration word */
	    free (paramList[paramIdx].stringVal);
	    paramList[paramIdx].stringVal = 
	       (char*)malloc(strlen(paramList[paramIdx].enumNameList[newVal]) * sizeof(char));
	    if (paramList[paramIdx].stringVal != NULL) 
            { /* ok */
	       strcpy (paramList[paramIdx].stringVal, paramList[paramIdx].enumNameList[newVal]);
	       result = TRUE;
	    } /* ok */
	    else
	       fatal (AUFG_DATA, 33, NO_INFO, NO_INFO, "", TSK_DATA_ANZ_ERROR);
	 } /* enumeration data: store new enumeration word */
	 else
	    result = TRUE;
      } /* ok */
   } /* index and type ok */
   return (result);
} /* setIntVal */



/* ==================================== setEnumVal ==================================== */
BOOL setEnumVal (const char *newVal,
                 INT         paramIdx)
{
   INT  enumIdx;
   BOOL result;

   /* setEnumVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, ENUM_DATA, 40)) /* returns TRUE for INT_ and ENUM_DATA */
   { /* index ok */
      if (paramList[paramIdx].paramDataType == ENUM_DATA)
      { /* data type ok */
	 if (getEnumIndex(&enumIdx, newVal, paramList[paramIdx].enumNameList)) 
         { /* ok */
	    free (paramList[paramIdx].stringVal);
	    paramList[paramIdx].stringVal = (char*)malloc(strlen(newVal) * sizeof(char));
	    if (paramList[paramIdx].stringVal != NULL) 
            { /* ok */
	       strcpy (paramList[paramIdx].stringVal, newVal);
	       paramList[paramIdx].intVal         = enumIdx;
	       paramList[paramIdx].paramValid     = TRUE;
	       paramList[paramIdx].paramToBeSaved = TRUE;
	       result = TRUE;
	    } /* ok */
	    else
	       fatal (AUFG_DATA, 42, NO_INFO, NO_INFO, "", TSK_DATA_ANZ_ERROR);
	 } /* ok */
      } /* data type ok */
      else
	 fatal (AUFG_DATA, 43, NO_INFO, NO_INFO, paramList[paramIdx].paramName, 
		INVALID_DATA_TYPE_OF_PARAM);
   } /* index ok */
   return (result);
} /* setEnumVal */



/* =================================== setDoubleVal =================================== */
BOOL setDoubleVal (DOUBLE  newVal,
                   INT     paramIdx)
{
   BOOL result;

   /* setDoubleVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, DOUBLE_DATA, 50))
   { /* index and type ok */
      if ((newVal < paramList[paramIdx].lowerLimit) || (paramList[paramIdx].upperLimit < newVal))
	 fehler(AUFG_DATA, 52, NO_INFO, NO_INFO, paramList[paramIdx].paramName, BAD_PARAM_TXT_VALUE);
      else { /* ok */
	 result = TRUE;
	 paramList[paramIdx].doubleVal      = newVal;
	 paramList[paramIdx].paramValid     = TRUE;
	 paramList[paramIdx].paramToBeSaved = TRUE;
      } /* ok */
   } /* index and type ok */
   return (result);
} /* setDoubleVal */



/* =================================== setStringVal =================================== */
BOOL setStringVal (const char *newVal,
                   INT         paramIdx)
{
   INT  len;
   BOOL result;

   /* setStringVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, STRING_DATA, 60))
   { /* index and type ok */
      free (paramList[paramIdx].stringVal);
      len = strlen(newVal);
      if (len > MAX_STRING)
	 len = MAX_STRING;                /* truncate to a maximum length of MAX_STRING */
      paramList[paramIdx].stringVal = (char*)malloc((len + 1) * sizeof(char));
      if (paramList[paramIdx].stringVal != NULL) 
      { /* ok */
	 strncpy (paramList[paramIdx].stringVal, newVal, len);
	 paramList[paramIdx].stringVal[len] = '\0';
	 paramList[paramIdx].paramValid     = TRUE;
	 paramList[paramIdx].paramToBeSaved = TRUE;
	 result = TRUE;
      } /* ok */
      else
	 fatal (AUFG_DATA, 62, NO_INFO, NO_INFO, "", TSK_DATA_ANZ_ERROR);
   } /* index and type ok */
   return (result);
} /* setStringVal */



/* ================================= setParamEditable ================================= */
void setParamEditable (BOOL  newVal,
                       INT   paramIdx)
{
   /* setParamEditable */
   if ((paramIdx < 0) || (PARAM_LIST_LENGTH < paramIdx))
      fatal(AUFG_DATA, 63, paramIdx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
   else
   { /* index ok */
      if (paramList[paramIdx].paramDataType != NIX_IDX)
	 paramList[paramIdx].editable = newVal;
      else
	 if (newVal)
	    fehler(AUFG_DATA, 64, paramIdx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
   } /* index ok */
} /* setParamEditable */



/* =================================== getParamData =================================== */
BOOL getParamData (char         *paramName,
                   INT          *paramType,
                   const char ***enumList,
                   INT           paramIdx)
{
   BOOL  result;

   /* getParamData */
   result = FALSE;
   if ((paramIdx < 0) || (PARAM_LIST_LENGTH < paramIdx))
      fatal(AUFG_DATA, 65, paramIdx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
   else
   { /* index ok */
      if (paramList[paramIdx].paramDataType == NIX_IDX)
	 fatal(AUFG_DATA, 66, paramIdx, PARAM_LIST_LENGTH - 1, "", BAD_PARAM_IDX);
      else
      { /* the index denotes a valid parameter */
	 result     = TRUE;
	 *paramType = paramList[paramIdx].paramDataType;
         *enumList  = paramList[paramIdx].enumNameList;
	 strcpy(paramName, paramList[paramIdx].paramName);
      } /* the index denotes a valid parameter */
   } /* index ok */
   return (result);
} /* getParamData */



/* =================================== getIntLimits =================================== */
BOOL getIntLimits (INT *lowerLim,
                   INT *upperLim,
                   INT  paramIdx)
{
   BOOL result;

   /* getIntLimits */
   result = FALSE;
   if (checkIdxAndType(paramIdx, INT_DATA, 70))
   { /* index and type ok */
      result = TRUE;
      *lowerLim = i_round(paramList[paramIdx].lowerLimit);
      *upperLim = i_round(paramList[paramIdx].upperLimit);
   } /* index and type ok */
   return (result);
} /* getIntLimits */



/* ================================= getDoubleLimits ================================== */
BOOL getDoubleLimits (DOUBLE *lowerLim,
                      DOUBLE *upperLim,
                      INT     paramIdx)
{
   BOOL result;

   /* getDoubleLimits */
   result = FALSE;
   if (checkIdxAndType(paramIdx, DOUBLE_DATA, 75))
   { /* index and type ok */
      result = TRUE;
      *lowerLim = paramList[paramIdx].lowerLimit;
      *upperLim = paramList[paramIdx].upperLimit;
   } /* index and type ok */
   return (result);
} /* getDoubleLimits */



/* ==================================== getBoolVal ==================================== */
BOOL getBoolVal (BOOL   *retVal,
                 INT     paramIdx,
                 GSTATE  invalidDataErrLevel)
{
   BOOL  result;

   /* getBoolVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, BOOL_DATA, 80))
   { /* index and type ok */
      if (paramList[paramIdx].paramValid)
      { /* data available */
	 result  = TRUE;
	 *retVal = (BOOL)(paramList[paramIdx].intVal);
      } /* data available */
      else
	 if (invalidDataErrLevel != GLEAM_OK) 
         { /* report unavailability of data */
	    if (invalidDataErrLevel == GLEAM_ERROR)
	       fehler(AUFG_DATA, 83, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	    else
	       fatal(AUFG_DATA, 83, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	 } /* report unavailability of data */
   } /* index and type ok */
   return (result);
} /* getBoolVal */



/* ==================================== getIntVal ===================================== */
BOOL getIntVal (INT    *retVal,
                INT     paramIdx,
                GSTATE  invalidDataErrLevel)
{
   BOOL  result;

   /* getIntVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, INT_DATA, 85))
   { /* index and type ok */
      if (paramList[paramIdx].paramValid)
      { /* data available */
	 result  = TRUE;
	 *retVal = paramList[paramIdx].intVal;
      } /* data available */
      else
	 if (invalidDataErrLevel != GLEAM_OK) 
         { /* report unavailability of data */
	    if (invalidDataErrLevel == GLEAM_ERROR)
	       fehler(AUFG_DATA, 88, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	    else
	       fatal(AUFG_DATA, 88, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	 } /* report unavailability of data */
   } /* index and type ok */
   return (result);
} /* getIntVal */



/* ==================================== getEnumText =================================== */
BOOL getEnumText (char   *retVal,
                  INT     paramIdx,
                  GSTATE  invalidDataErrLevel)
{
   BOOL  result;

   /* getEnumText */
   result = FALSE;
   if (checkIdxAndType(paramIdx, ENUM_DATA, 90))
   { /* index ok */
      if (paramList[paramIdx].paramDataType == ENUM_DATA)
      { /* matching data type */
	 if (paramList[paramIdx].paramValid) { /* value available */
	    strcpy (retVal, paramList[paramIdx].stringVal);
	    result = TRUE;
	 } /* value available */
	 else
	    if (invalidDataErrLevel != GLEAM_OK) 
	    { /* report unavailability of data */
	       if (invalidDataErrLevel == GLEAM_ERROR)
		  fehler(AUFG_DATA, 93, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	       else
		  fatal(AUFG_DATA, 93, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	    } /* report unavailability of data */
      } /* matching data type */
      else 
	 fatal(AUFG_DATA, 94, NO_INFO, NO_INFO, paramList[paramIdx].paramName, INVALID_DATA_TYPE_OF_PARAM);
   } /* index ok */
   return (result);
} /* getEnumText */



/* =================================== getDoubleVal =================================== */
BOOL getDoubleVal (DOUBLE *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel)
{
   BOOL  result;

   /* getDoubleVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, DOUBLE_DATA, 95))
   { /* index and type ok */
      if (paramList[paramIdx].paramValid)
      { /* data available */
	 result  = TRUE;
	 *retVal = paramList[paramIdx].doubleVal;
      } /* data available */
      else
	 if (invalidDataErrLevel != GLEAM_OK) 
         { /* report unavailability of data */
	    if (invalidDataErrLevel == GLEAM_ERROR)
	       fehler(AUFG_DATA, 98, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	    else
	       fatal(AUFG_DATA, 98, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	 } /* report unavailability of data */
   } /* index and type ok */
   return (result);
} /* getDoubleVal */



/* =================================== getStringVal =================================== */
BOOL getStringVal (char   *retVal,
                   INT     paramIdx,
                   GSTATE  invalidDataErrLevel)
{
   BOOL  result;

   /* getStringVal */
   result = FALSE;
   if (checkIdxAndType(paramIdx, STRING_DATA, 100))
   { /* index and type ok */
      if (paramList[paramIdx].paramValid)
      { /* data available */
	 result = TRUE;
	 strcpy (retVal, paramList[paramIdx].stringVal);
      } /* data available */
      else
	 if (invalidDataErrLevel != GLEAM_OK) 
         { /* report unavailability of data */
	    if (invalidDataErrLevel == GLEAM_ERROR)
	       fehler(AUFG_DATA, 103, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	    else
	       fatal(AUFG_DATA, 103, NO_INFO, NO_INFO, paramList[paramIdx].paramName, PARAM_HAS_NO_VALUE);
	 } /* report unavailability of data */
   } /* index and type ok */
   return (result);
} /* getStringVal */



/* =================================== getEnumIndex =================================== */
static BOOL getEnumIndex (INT         *retVal,
                          const char  *enumText,
	              	  const char **enumValues)
/*----------------------------------------------------------------------------------------
  "enumText" is searched for in "enumValues" and its index is returned in "retVal", if
  found. In this case the function returns TRUE. Otherwise an error message is issued and 
  the return value is FALSE.
----------------------------------------------------------------------------------------*/
{
   INT   ii;
   BOOL  found;

   /* getEnumIndex */
   found = FALSE;
   for (ii = 0;  (ii < MAX_NUMBER_OF_ENUM_WORDS) && !found;  ii++)
      found = (strcasecmp(enumValues[ii], enumText) == 0);
   if (found)
      *retVal = ii - 1;
   else
      fehler(AUFG_DATA, 110, NO_INFO, NO_INFO, enumText, UNKNOWN_ID);
   return (found);
} /* getEnumIndex */



/* ============================== getEnumIndexFromParIdx ============================== */
BOOL getEnumIndexFromParIdx (INT        *retVal,
                             const char *enumText,
		             INT         parIdx)
{
   BOOL  result;

   /* getEnumIndexFromParIdx */
   result = FALSE;
   if (checkIdxAndType(parIdx, ENUM_DATA, 115))
   { /* index ok */
      if (getEnumIndex(retVal, enumText, paramList[parIdx].enumNameList))
	 result = TRUE;
      else
	 *retVal = NIX_IDX;                                                /* not found */
   } /* index ok */
   return (result);
} /* getEnumIndexFromParIdx */



/* ================================= getMaxNameLength ================================= */
INT getMaxNameLength (void)
{
   return(maxNameLength);
} /* getMaxNameLength */



/* =============================== prepareTskDataDisplay ============================== */
void prepareTskDataDisplay (BOOL  withIndices,
                            BOOL  allParam)
{
   char         buffer[MAX_STRING+MAX_STR];
   STRING_TYPE  buf;
   INT          dataType,
                lineNo,
                idx;

   /* prepareTskDataDisplay */
   reset_text_buf();
   lineNo = 1;
#ifdef PARAM_LIST_DBG
   printf("prepareTskDataDisplay: Complete raw listing of parameter list \"paramList\":\n"); 
   for (idx = 0;  idx < PARAM_LIST_LENGTH;  idx++)
   { /* all entries */
      sprintf (buffer, "%2d: %-*s valid=%d paramType=%2d editable=%d ", idx, maxNameLength, 
	       paramList[idx].paramName, paramList[idx].paramValid, paramList[idx].paramDataType, paramList[idx].editable);
      if (paramList[idx].paramDataType != NIX_IDX)
      { /* valid entry */
	 if (paramList[idx].paramDataType == STRING_DATA) 
	    if (paramList[idx].stringVal != NULL)
	       strcat (buffer, paramList[idx].stringVal);
	    else
	       strcat (buffer, "-");
	 else {
	    sprintf (buf, "iVal=%d dVal=%g [%g, %g] ", paramList[idx].intVal, paramList[idx].doubleVal,
		     paramList[idx].lowerLimit, paramList[idx].upperLimit);
	    strcat(buffer, buf);
	    if (paramList[idx].paramDataType == ENUM_DATA)
	       if (paramList[idx].stringVal != NULL)
		  strcat (buffer, paramList[idx].stringVal);
	       else
		  strcat (buffer, "-");
	 }
      } /* valid entry */
      printf("%s\n", buffer); 
   } /* all entries */
   printf("\n"); 
#endif /* PARAM_LIST_DBG */
   for (dataType = 0;  dataType <= STRING_DATA;  dataType++) 
   { /* all data types */
      for (idx = 0;  idx < PARAM_LIST_LENGTH;  idx++) 
      { /* all entries */
	 if ((paramList[idx].paramDataType == dataType) && (allParam || paramList[idx].editable))
	 { /* display this entry */
	    if (withIndices)
	       sprintf (buffer, "%2d: ", lineNo);
	    else
	       strcpy (buffer, "");
	    paramListActEntries[lineNo - 1] = idx; 
	    sprintf (buf, "%-*s = ", maxNameLength, paramList[idx].paramName);
	    strcat(buffer, buf);
	    if (paramList[idx].paramValid)
	    { /* valid parameter value */
	       switch (dataType) {
	       case BOOL_DATA:
		  if (paramList[idx].intVal)
		     sprintf (buf, "%*s",  MIN_LEN, JA_TXT);
		  else
		     sprintf (buf, "%*s",  MIN_LEN, NEIN_TXT);
		  strcat (buffer, buf);
		  break; /* BOOL_DATA */

	       case INT_DATA:
		  sprintf (buf, "%*d", MIN_LEN, paramList[idx].intVal);
		  strcat (buffer, buf);
		  break; /* INT_DATA */

	       case ENUM_DATA:
		  sprintf (buf, "%*s", MIN_LEN, paramList[idx].stringVal);
		  strcat (buffer, buf);
		  break; /* ENUM_DATA */

	       case DOUBLE_DATA:
		  sprintf (buf, "%.*g", DOUBLE_LEN, paramList[idx].doubleVal);
		  strcat (buffer, buf);
		  break; /* DOUBLE_DATA */

	       case STRING_DATA:
		  strcat (buffer, "\"");
		  strcat (buffer, paramList[idx].stringVal);
		  strcat (buffer, "\"");
		  break; /* STRING_DATA */
	       } /* end switch */
	    } /* valid parameter value */
	    else
	       strcat (buffer, "  void");
	    write_buf(lineNo++, buffer);
	 } /* display this entry */
      } /* all entries */
   } /* all data types */
   paramListActEntryAmount = lineNo - 1;
#ifdef PARAM_LIST_DBG
   printf ("prepareTskDataDisplay: paramListActEntryAmount=%d\n\n", paramListActEntryAmount);fflush(stdout);
#endif /* PARAM_LIST_DBG */
} /* prepareTskDataDisplay */



/* ================================== mod_dir_mecker ================================== */
void mod_dir_mecker (STR_TYPE  mod_dir_buf)
{
   STR_TYPE  buf;

   /* mod_dir_mecker */
   sprintf (buf, MOD_NIX_DA_TXT, mod_dir_buf, sim_root_dir_name, DIR_DELIMITER, extSimSubDir);
   fehler (AUFG_DATA, 120, NO_INFO, NO_INFO, buf, NO_MOD_AVAIL_ERR);
} /* mod_dir_mecker */



/* ============================= registerTskFileParameter ============================= */
BOOL registerTskFileParameter (void)
{
   BOOL  result;

   /* registerTskFileParameter */
   /* remark 1: the variables indicated are editable if LHCs are later activated 
      remark 2: the variables indicated are not editable because they can be changed by 
                - the Evo/Opt sub menu "Opt Params" (modif_evo_defaults() in "evo_cmen.c" of evo-package)
                  according to the selected default optimization procedure, the default LHC, or the current application 
                - the Simulator menu (affects only the logging of the simulator interface)
      remark 3: the variables indicated are not editable because they apply to the command line version only
      remark 4: editability is set after reading the gene model file (read_mod_file() from package "hmod").
   */
   result  = registerParam(WITH_PHAENO_REP_TXT,   WITH_PHAENO_REP_IDX,   BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 2 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(WITH_DETAILED_LOG_TXT, WITH_DETAILED_LOG_IDX, BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 3 */
			   WITH_DEFAULT, 1, 0.0, NULL);
   result &= registerParam(MA_WITH_ALL_IMPR_TXT,  MA_WITH_ALL_IMPR_IDX,  BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 1 */
			   WITH_DEFAULT, withAllImprovement, 0.0, NULL);
   result &= registerParam(WITH_RES_CHR_SIMU_TXT, WITH_RES_CHR_SIMU_IDX, BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 3 */
			   WITH_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(WITH_RANDGEN_INIT_TXT, WITH_RANDGEN_INIT_IDX, BOOL_DATA,   0, 1,          EDITABLE,
			   WITH_DEFAULT, initZufGen, 0.0, NULL);
   result &= registerParam(WITH_RES_CHR_SAVE_TXT, WITH_RES_CHR_SAVE_IDX, BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 3 */
			   WITH_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(WITH_TEXTCHR_FILE_TXT, WITH_TEXTCHR_FILE_IDX, BOOL_DATA,   0, 1,          NOT_EDITABLE, 
			   WITH_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(OPT_PROCEDURE_TXT,     OPT_PROCEDURE_IDX,     ENUM_DATA,   0, 4,          NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, def_opt_strat, 0.0, optProcNames);
   result &= registerParam(POP_SIZE_TXT,          POP_SIZE_IDX,          INT_DATA,    4, MAX_POPEL_SIZE, EDITABLE, 
			   WITH_DEFAULT, def_psize, 0.0, NULL);
   result &= registerParam(INIT_STRATEGY_TXT,     INIT_STRATEGY_IDX,     ENUM_DATA,   0, 6,          EDITABLE, 
			   WITH_DEFAULT, defInitStrat, 0.0, initProcNames);
#ifdef GLEAM_USR
   result &= registerParam(INIT_SPAR_TXT,         INIT_SPAR_IDX,         INT_DATA,    0, RNOTEN_MAX, EDITABLE, 
			   WITH_DEFAULT, defInitStratPar, 0.0, NULL);
#else /* command line version */
   result &= registerParam(INIT_SPAR_TXT,         INIT_SPAR_IDX,         INT_DATA,    0, RNOTEN_MAX, EDITABLE, 
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
#endif /* command line version */
   result &= registerParam(DEME_SIZE_TXT,         DEME_SIZE_IDX,         INT_DATA,    MIN_ANZ_NACHBARN,            /* remark 2 */
                                                                                      MAX_ANZ_NACHBARN, NOT_EDITABLE, 
			   WITH_DEFAULT, anz_nachbarn, 0.0, NULL);
   result &= registerParam(LHC_ADAPT_SPEED_TXT,   LHC_ADAPT_SPEED_IDX,   ENUM_DATA,   0, 2,          NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, ADAPT_SPEED_SCHNELL, 0.0, adaptSpeedNames);
   result &= registerParam(LEVEL_ADAPT_SPEED_TXT, LEVEL_ADAPT_SPEED_IDX, ENUM_DATA,   0, 2,          NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, ADAPT_SPEED_SCHNELL, 0.0, adaptSpeedNames);
   result &= registerParam(NUMBER_OF_RESULTS_TXT, NUMBER_OF_RESULTS_IDX, INT_DATA,    0, MAX_POPEL_SIZE / 2, EDITABLE, 
			   WITH_DEFAULT, defResultChrs, 0.0, NULL);
   result &= registerParam(RUNTIME_LIMIT_TXT,     RUNTIME_LIMIT_IDX,     INT_DATA,    1, INT_MAX,    EDITABLE, 
			   WITH_DEFAULT, defTimeLimit, 0.0, NULL);
   result &= registerParam(GEN_LIMIT_TXT,         GEN_LIMIT_IDX,         INT_DATA,    0, INT_MAX,    EDITABLE, 
			   WITH_DEFAULT, defGenLimit, 0.0, NULL);
   result &= registerParam(GAC_LIMIT_TXT,         GAC_LIMIT_IDX,         INT_DATA,    0, INT_MAX,    EDITABLE, 
			   WITH_DEFAULT, defGAcLimit, 0.0, NULL);
   result &= registerParam(GDI_LIMIT_TXT,         GDI_LIMIT_IDX,         INT_DATA,    0, INT_MAX,    EDITABLE,
			   WITH_DEFAULT, defGDI_Limit, 0.0, NULL);
   result &= registerParam(EVAL_LIMIT_TXT,        EVAL_LIMIT_IDX,        INT_DATA,    0, INT_MAX,    EDITABLE,
			   WITH_DEFAULT, defEvalLimit, 0.0, NULL);
   result &= registerParam(ACCEPTANCE_RULE_TXT,   ACCEPTANCE_RULE_IDX,   ENUM_DATA,   0, 4,          NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, default_survival_rule, 0.0, acceptRuleNames);
   result &= registerParam(BACKUP_RATE_TXT,       BACKUP_RATE_IDX,       INT_DATA,    0, INT_MAX,    EDITABLE, 
			   WITH_DEFAULT, save_rate_def, 0.0, NULL);
   result &= registerParam(TERM_TEST_FREQ_TXT,    TERM_TEST_FREQ_IDX,    INT_DATA,    1, INT_MAX,    NOT_EDITABLE, /* remark 3 */
			   WITH_DEFAULT, evo_tst_frequ, 0.0, NULL);
   result &= registerParam(WITH_TERM_TEST_TXT,    WITH_TERM_TEST_IDX,    BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 3 */
			   WITH_DEFAULT, with_evo_term_tst, 0.0, NULL);
   result &= registerParam(TARGET_FITNESS_TXT,    TARGET_FITNESS_IDX,    DOUBLE_DATA, 0, RNOTEN_MAX, EDITABLE,
			   WITH_DEFAULT, 0, defTargetFitness, NULL);
   result &= registerParam(XO_HAMDIST_TXT,        XO_HAMDIST_IDX,        DOUBLE_DATA, 0.0, 100.0,    NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, 0, def_xo_min_ham_dist * 100.0, NULL);
   result &= registerParam(GEN_REP_RATE_TXT,      GEN_REP_RATE_IDX,      DOUBLE_DATA, 0.0, 100.0,    NOT_EDITABLE, /* remark 2 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(LAMARCK_RATE_TXT,      LAMARCK_RATE_IDX,      DOUBLE_DATA, 0.0, 100.0,    NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, 0, def_lamarck_rate * 100.0, NULL);
   result &= registerParam(RANKING_PAR_TXT,       RANKING_PAR_IDX,       DOUBLE_DATA, 1.0, 2.0,      NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, 0, def_max_fit, NULL);
   result &= registerParam(LHC_ITER_LIMIT_TXT,    LHC_ITER_LIMIT_IDX,    INT_DATA,    1, INT_MAX,    NOT_EDITABLE, /* remark 2 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(LHC_TERM_LIMIT_TXT,    LHC_TERM_LIMIT_IDX,    DOUBLE_DATA, 0.0, DBL_MAX,  NOT_EDITABLE, /* remark 2 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(OLD_DISTR_FRAC_TXT,    OLD_DISTR_FRAC_IDX,    DOUBLE_DATA, 0.0, 100.0,    NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, 0, oldDistribFrac * 100.0, NULL);
   result &= registerParam(SIMU_RES_PREC_TXT,     SIMU_RES_PREC_IDX,     INT_DATA,    4, 16,         EDITABLE,
			   WITH_DEFAULT, erg_genauigkeit, 0.0, NULL);
   result &= registerParam(SIMU_MAX_TIME_TXT,     SIMU_MAX_TIME_IDX,     INT_DATA,    0, INT_MAX,    NOT_EDITABLE, /* remark 4 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(SIMU_MOD_NAME_TXT,     SIMU_MOD_NAME_IDX,     STRING_DATA, 0, 0,          NOT_EDITABLE, /* remark 4 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(SIMU_DOMAIN_NAME_TXT,  SIMU_DOMAIN_NAME_IDX,  STRING_DATA, 0, 0,          NOT_EDITABLE, /* remark 4 */
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(SIMU_RUNS_PER_INIT_TXT,SIMU_RUNS_PER_INIT_IDX,INT_DATA,    0, INT_MAX,    NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, extSimuRunLimit, 0.0, NULL);
   result &= registerParam(MBF_ROT_ANGLE_TXT,     MBF_ROT_ANGLE_IDX,     DOUBLE_DATA, 0.0, 360.0,    NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 0, 0.0, NULL);
   result  = registerParam(LSKP_R500_TXT,         LSKP_R500_IDX,         BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 1, 0.0, NULL);
   result  = registerParam(LSKP_COLLI_TEST_TXT,   LSKP_COLLI_TEST_IDX,   BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 1, 0.0, NULL);
   result  = registerParam(LSKP_STOP_TEST_TXT,    LSKP_STOP_TEST_IDX,    BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 1, 0.0, NULL);
   result  = registerParam(LSKP_STOP_DEPREC_TXT,  LSKP_STOP_DEPREC_IDX,  BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 1, 0.0, NULL);
   result  = registerParam(LSKP_AXES_NUMBER_TXT,  LSKP_AXES_NUMBER_IDX,  INT_DATA,    1, 16,         NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 5, 0.0, NULL);
   result  = registerParam(LSKP_CYCLE_LENGTH_TXT, LSKP_CYCLE_LENGTH_IDX, DOUBLE_DATA, 0.001, 1.0,    NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, 0, 0.1, NULL);
   result &= registerParam(WITH_REC_EVO_BEST_TXT, WITH_REC_EVO_BEST_IDX, BOOL_DATA,   0, 1,          EDITABLE,
			   WITH_DEFAULT, record_evo_best, 0.0, NULL);
   result &= registerParam(WITH_STAT_DAT_COL_TXT, WITH_STAT_DAT_COL_IDX, BOOL_DATA,   0, 1,          EDITABLE,
			   WITH_DEFAULT, mit_statistik, 0.0, NULL);
   result &= registerParam(WITH_GEN_STAT_COL_TXT, WITH_GEN_STAT_COL_IDX, BOOL_DATA,   0, 1,          EDITABLE,
			   WITH_DEFAULT, statistik_pro_gen, 0.0, NULL);
   result &= registerParam(WITH_SIMU_TIME_TXT,    WITH_SIMU_TIME_IDX,    BOOL_DATA,   0, 1,          NOT_EDITABLE, /* remark 4 */
			   WITH_DEFAULT, mit_rz, 0.0, NULL);
   result &= registerParam(OPAL_MAX_SHIFTS_TXT,   OPAL_MAX_SHIFTS_IDX,   DOUBLE_DATA, 4.0, 14.0,     NOT_EDITABLE, /* remark 4 */
                       	   WITH_DEFAULT, 0, 9.0, NULL);

#ifdef MIT_LSV
   result &= registerParam(LHC_INDEX_TXT,         LHC_INDEX_IDX,         INT_DATA,    0, LSV_ANZ-1,  NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(NUMB_OF_FCLASSES_TXT,  NUMB_OF_FCLASSES_IDX,  INT_DATA,    1, 5,          NOT_EDITABLE, /* remark 2 */
			   WITH_DEFAULT, getActNKlAnz(), 0.0, NULL);
#else /* no LHCs: pure GLEAM */
   result &= registerParam(LHC_INDEX_TXT,         LHC_INDEX_IDX,         INT_DATA,    0, 0,          NOT_EDITABLE,
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
   result &= registerParam(NUMB_OF_FCLASSES_TXT,  NUMB_OF_FCLASSES_IDX,  INT_DATA,    1, NKL_MAX,    NOT_EDITABLE,
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
#endif /* no LHCs: pure GLEAM */
#ifdef CEC_MBF
   result &= registerParam(CEC_FES_LIMIT_TXT,     CEC_FES_LIMIT_IDX,     INT_DATA,    1, INT_MAX,    EDITABLE, 
			   WITHOUT_DEFAULT, 0, 0.0, NULL);
#endif /* CEC_MBF */
#ifdef MITSIMULOG
   result &= registerParam(WITH_SIMU_LOG_TXT,     WITH_SIMU_LOG_IDX,     BOOL_DATA,   0, 1,          NOT_EDITABLE, 
			   WITH_DEFAULT, mit_simulog, 0.0, NULL);
#else /* without simulator (interface) logging */
   result &= registerParam(WITH_SIMU_LOG_TXT,     WITH_SIMU_LOG_IDX,     BOOL_DATA,   0, 1,          NOT_EDITABLE, 
			   WITH_DEFAULT, 0, 0.0, NULL);
#endif /* without simulator (interface) logging */

   if (!result)
      err_state = fatal(AUFG_DATA, 125, NO_INFO, NO_INFO, "", NO_INI);
   return (result);
} /* registerTskFileParameter */



/* =========================== checkAndCorrectSimuModelName =========================== */
static void checkAndCorrectSimuModelName (void)
/*----------------------------------------------------------------------------------------
  As "extSimuModelName" must not contain blanks, spaces are cut off right and left. This 
  is needed for edited names only, as names from the TSK-file already start and end with 
  non blanks. Furthermore, blanks within the name string are substituted by "_" charac-
  ters. A changed name is written back to the program parameter list. Empty names remain
  unchanged. All changes are reported by messages i.e. a call of meldung().
----------------------------------------------------------------------------------------*/
{
   STR_TYPE  newName,               /* buffer containg the changed model name           */
             buf;                   /* auxiliary buffer                                 */
   INT       nameLen,               /* length of the model name                         */
             innerBlanks,           /* number of blanks within the model name           */
             startIdx,              /* index of the start char in "extSimuModelName"    */
             newLen,                /* length of "newName" after blank suppress         */
             ii;
   BOOL      modelNameChanged;      /* TRUE, if the name of the model was changed       */

   /* checkAndCorrectSimuModelName */
   nameLen = (INT)strlen(extSimuModelName);
   if (nameLen > 0)
   { /* the name string is not empty */
      startIdx         = 0;
      innerBlanks      = 0;
      modelNameChanged = FALSE;
      while ((extSimuModelName[startIdx] == ' ') && (startIdx < nameLen))
	 startIdx++;                                                /* blank suppress left */
      ii = nameLen - 1;
      while ((extSimuModelName[ii] == ' ') && (startIdx < ii))
	 ii--;                                                     /* blank suppress right */
      newLen = ii- startIdx + 1;
      if (newLen > 0)
      { /* the name contains some characters different from blanks */
	 strncpy(newName, &(extSimuModelName[startIdx]), newLen);
	 newName[newLen] = '\0';                                       /* terminate string */
	 if (startIdx > 0) {
	    meldung(AUFG_DATA, NO_INFO, NO_INFO, CUT_OFF_LEADING_BLANKS, MODEL_NAME_CORRECTION_MSG);
	    modelNameChanged = TRUE;
	 }
	 if (ii < (nameLen - 1)) {
	    meldung(AUFG_DATA, NO_INFO, NO_INFO, CUT_OFF_TRAIL_BLANKS, MODEL_NAME_CORRECTION_MSG);
	    modelNameChanged = TRUE;
	 }
	 for (ii = 0;  ii < newLen;  ii++)
	    if (newName[ii] == ' ') { /* blank found */
	       newName[ii] = '_';                             /* substitute found blank */
	       innerBlanks++;
	    } /* blank found */
	 if (innerBlanks > 0) {
	    sprintf(buf, SUBSTITUTE_BLANKS_TXT, innerBlanks);
	    meldung(AUFG_DATA, NO_INFO, NO_INFO, buf, MODEL_NAME_CORRECTION_MSG);
	    modelNameChanged = TRUE;
	 }
      } /* the name contains some characters different from blanks */
      else { /* name of blanks */
	 meldung(AUFG_DATA, NO_INFO, NO_INFO, ONLY_BLANKS_TXT, MODEL_NAME_CORRECTION_MSG);
	 strcpy(newName, "");
	 modelNameChanged = TRUE;
      } /* name of blanks */
      if (modelNameChanged) 
      { /* write back the corrected name */
	 strcpy(extSimuModelName, newName);
	 setStringVal(extSimuModelName, SIMU_MOD_NAME_IDX);
      } /* write back the corrected name */
   } /* the name string is not empty */
} /* checkAndCorrectSimuModelName */



/* ================================ processReadTskData ================================ */
GSTATE processReadTskData (BOOL  dataFromFile)
{
   DOUBLE  realVal;
   INT     intVal;
   BOOL    boolVal;
   GSTATE  localErrLevel;

   /* processReadTskData */
   localErrLevel = GLEAM_OK;
   if (mitOptionPhaenoRep)
      getBoolVal(&mitPhaenoRepair,      WITH_PHAENO_REP_IDX,   GLEAM_OK);
   if (dataFromFile)
   { /* update these parameters from file only */
      getIntVal(&anz_nachbarn,          DEME_SIZE_IDX,         GLEAM_OK);
      getIntVal(&default_survival_rule, ACCEPTANCE_RULE_IDX,   GLEAM_OK);
      getDoubleVal(&def_max_fit,        RANKING_PAR_IDX,       GLEAM_OK);
      getDoubleVal(&realVal,            XO_HAMDIST_IDX,        GLEAM_OK);
      def_xo_min_ham_dist = realVal / 100.0;
   } /* update these parameters from file only */
  
   getIntVal(&def_psize,              POP_SIZE_IDX,          GLEAM_OK);
   getIntVal(&defInitStrat,           INIT_STRATEGY_IDX,     GLEAM_OK);
   getIntVal(&defInitStratPar,        INIT_SPAR_IDX,         GLEAM_OK);
   getIntVal(&defResultChrs,          NUMBER_OF_RESULTS_IDX, GLEAM_OK);
   getIntVal(&defTimeLimit,           RUNTIME_LIMIT_IDX,     GLEAM_OK);
   getIntVal(&defGenLimit,            GEN_LIMIT_IDX,         GLEAM_OK);
   getIntVal(&defGAcLimit,            GAC_LIMIT_IDX,         GLEAM_OK);
   getIntVal(&defGDI_Limit,           GDI_LIMIT_IDX,         GLEAM_OK);
   getIntVal(&defEvalLimit,           EVAL_LIMIT_IDX,        GLEAM_OK);
   getIntVal(&erg_genauigkeit,        SIMU_RES_PREC_IDX,     GLEAM_OK);
   getIntVal(&save_rate_def,          BACKUP_RATE_IDX,       GLEAM_OK);
   getDoubleVal(&defTargetFitness,    TARGET_FITNESS_IDX,    GLEAM_OK);
   getBoolVal(&initZufGen,            WITH_RANDGEN_INIT_IDX, GLEAM_OK);
   getBoolVal(&record_evo_best,       WITH_REC_EVO_BEST_IDX, GLEAM_OK);
   getBoolVal(&mit_statistik,         WITH_STAT_DAT_COL_IDX, GLEAM_OK);
   getBoolVal(&statistik_pro_gen,     WITH_GEN_STAT_COL_IDX, GLEAM_OK);
#ifndef WITH_DYN_GENES
   getBoolVal(&boolVal,               WITH_TEXTCHR_FILE_IDX, GLEAM_OK);
   if (boolVal)                                  /* this influences the expected file   */
      initialChrFileMode = FILE_TXT_IO;          /* format of of chromosome files which */
   else                                          /* optionally may be read during       */
      initialChrFileMode = FILE_BIN_IO;          /* program initialization              */
#endif /* no WITH_DYN_GENES */
   if (statistik_pro_gen && !mit_statistik) 
   { /* no required general collection */
      statistik_pro_gen = FALSE;
      setBoolVal(statistik_pro_gen, WITH_GEN_STAT_COL_IDX);
      meldung (AUFG_DATA, NO_INFO, NO_INFO, "", STATISTICS_PER_GEN_IMPOSSIBLE);
   } /* no required general collection */
   if (mit_gen_rep_par) {
      if (getDoubleVal(&realVal, GEN_REP_RATE_IDX, GLEAM_OK))
	 gen_rep_par = realVal / 100.0;
   }

#ifdef MIT_LSV
   if (lsv_anz > 0)
   { /* MOD-file specifies some usable LHCs */
      getBoolVal(&withAllImprovement, MA_WITH_ALL_IMPR_IDX, GLEAM_OK);
      if (dataFromFile)
      { /* update these parameters from file only */
	 if (getIntVal(&intVal, LHC_INDEX_IDX, GLEAM_OK)) 
         { /* LHC specified by TSK file or by default */
	    if (lsv[intVal].aktiv)
	    { /* existing and active LHC */
	       def_lsv_strat = intVal;
	       if (lsv[def_lsv_strat].lsvParAnz > 0) 
	       { /* LHC has at least one parameter */
		  if (getIntVal(&intVal, LHC_ITER_LIMIT_IDX, GLEAM_OK))
		     lsv[def_lsv_strat].lsvPar[0] = intVal;
		  if (lsv[def_lsv_strat].lsvParAnz > 1)  /* LHC has at least two params */
		     if (getDoubleVal(&realVal, LHC_TERM_LIMIT_IDX, GLEAM_OK))
			lsv[def_lsv_strat].lsvPar[1] = realVal;
		  if (!parameterise_lsv(def_lsv_strat))
		     localErrLevel = GLEAM_ERROR;    /* msg issued by "parameterise_lsv"*/
	       } /* LHC has at least one parameter */
	    } /* existing and active LHC */
	    else
	       localErrLevel = fehler(AUFG_DATA, 130, intVal, NO_INFO, "", INACTIVE_LHC_SPECIFIED);
	 } /* LHC specified by TSK file or by default */
	 else
	    localErrLevel = fehler(AUFG_DATA, 131, NO_INFO, NO_INFO, LHC_INDEX_TXT, PARAM_HAS_NO_VALUE);
	 getIntVal(&def_opt_strat, OPT_PROCEDURE_IDX,     GLEAM_OK); 
	 getDoubleVal(&realVal,    LAMARCK_RATE_IDX,      GLEAM_OK);
	 def_lamarck_rate = realVal / 100.0;
	 getDoubleVal(&realVal,    OLD_DISTR_FRAC_IDX,    GLEAM_OK);
	 oldDistribFrac = realVal / 100.0;
	 if (getIntVal(&intVal,    LHC_ADAPT_SPEED_IDX,   GLEAM_OK))
	    setLSVAdaptSpeed(intVal, 0);
	 if (getIntVal(&intVal,    LEVEL_ADAPT_SPEED_IDX, GLEAM_OK))
	    setLevelAdaptSpeed(intVal);
	 if (getIntVal(&intVal,    NUMB_OF_FCLASSES_IDX,  GLEAM_OK))
	    if (intVal == 2)
	       localErrLevel = fehler(AUFG_DATA, 132, intVal, NO_INFO, NUMB_OF_FCLASSES_TXT, BAD_PARAM_VALUE);
	    else
	       set_std_nkl(intVal);
      } /* update these parameters from file only */
   } /* MOD-file specifies some usable LHCs */
   else 
#endif /* with LHCs */
   { /* if no LHCs only GLEAM */
      def_opt_strat = GLEAM;
      setIntVal(def_opt_strat, OPT_PROCEDURE_IDX);
   } /* if no LHCs only GLEAM */

   switch (appl_code)
   {
      case LESAK_APPL: /* ------------------------ LESAK_APPL ------------------------- */
	 if (!update_rob_task ())                 /* treats all LSKP_..._IDX parameters */
	    localErrLevel = fehler (AUFG_CMEN, 2, NO_INFO, NO_INFO, "", TSK_BAD_DATA_ERR);
         break; /* LESAK_APPL */

      case MATH_FKT_APPL:  /* ------------------- MATH_FKT_APPL ----------------------- */
	 init_mbf_sim ();                       /* uses the MBF_ROT_ANGLE_IDX parameter */
         break; /* MATH_FKT_APPL */

      case CEC_MATHFKT_APPL:  /* ---------------- CEC_MATHFKT_APPL -------------------- */
         break; /* CEC_MATHFKT_APPL */
   } /* switch */

   if (mit_ext_simu) 
   { /* process settings for the use of an external simulator */
      getBoolVal(&mit_rz,         WITH_SIMU_TIME_IDX,     GLEAM_OK);
      getIntVal(&extSimuRunLimit, SIMU_RUNS_PER_INIT_IDX, GLEAM_OK); 
      if (getIntVal(&intVal, SIMU_MAX_TIME_IDX, GLEAM_ERROR))
	 update_ext_simu_tmo(intVal);
      else 
	 localErrLevel = GLEAM_ERROR;                /* default value set by  ext_sim.c */
#ifdef EXT_SIMU_SERV
      if (getStringVal(extSimServDomainName, SIMU_DOMAIN_NAME_IDX, GLEAM_FATAL))
      { /* domain name ok */
	 if (atProgStart)
	    setServerPraeFix();   /* creates the beginning of the URL in "serverBuffer" */
	 if (sim_up && strcmp(lastSimServDomainName, extSimServDomainName))
	 { /* terminate only, if active domain name was changed   */
	    terminate_ext_simu();
	    setServerPraeFix();   /* creates the beginning of the URL in "serverBuffer" */
	 } /* terminate only, if active domain name was changed   */
      } /* domain name ok */
      else
	 localErrLevel = GLEAM_FATAL;
#endif /* EXT_SIMU_SERV */
      if (getStringVal(extSimuModelName, SIMU_MOD_NAME_IDX, GLEAM_FATAL))
      { /* model name obtained */
	 checkAndCorrectSimuModelName();
	 if (!atProgStart)	                     /* not in the program start phase? */
	    ext_simu_hochfahren(!sim_ok);
      } /* model name obtained */
      else
	 localErrLevel = GLEAM_FATAL;
   } /* process settings for the use of an external simulator */
	    
#ifdef CEC_MBF
   getIntVal(&defEvalLimit, CEC_FES_LIMIT_IDX, GLEAM_OK);
#endif /* CEC_MBF */
#ifdef MITSIMULOG
   if (dataFromFile)
      getBoolVal(&mit_simulog, WITH_SIMU_LOG_IDX, GLEAM_OK);
#endif /* MITSIMULOG */
   return (localErrLevel);
} /* processReadTskData */



/* =================================== tskDataInit ==================================== */
BOOL tskDataInit (void)
{
   INT   ii;
   BOOL  result;

   /* tskDataInit */
   result = FALSE;
   paramList         = (PROG_PARAM_TYPE*)malloc(PARAM_LIST_LENGTH * sizeof(PROG_PARAM_TYPE));
   sim_root_dir_name = (char*)malloc((MAX_STR+1) * sizeof(char));
   applIdStr         = (char*)malloc((BEZ_LEN+1) * sizeof(char));
   start_joints      = (DOUBLE*)malloc(MAXAXISANZ*sizeof(DOUBLE));
   ziel_joints       = (DOUBLE*)malloc(MAXAXISANZ*sizeof(DOUBLE));
   if ((paramList == NULL)    || (sim_root_dir_name == NULL) || (applIdStr == NULL) || 
       (start_joints == NULL) || (ziel_joints== NULL))
      err_state = fatal(AUFG_DATA, 130, NO_INFO, NO_INFO, "", TSK_DATA_ANZ_ERROR);
   else
   { /* memory ok */
      result = TRUE;
      for (ii = 0;  ii < PARAM_LIST_LENGTH;  ii++) {
	 strcpy(paramList[ii].paramName, "");
	 paramList[ii].paramDataType  = NIX_IDX;
	 paramList[ii].paramValid     = FALSE;
	 paramList[ii].editable       = FALSE;
	 paramList[ii].paramToBeSaved = FALSE;
         paramList[ii].stringVal      = NULL;
         paramList[ii].enumNameList   = NULL;
      }
      maxNameLength = 0;
      for (ii = 0;  ii < MAXAXISANZ;  ii++) {
	 start_joints[ii] = 0.0;
	 ziel_joints[ii]  = 0.0;
      }
      strcpy (applIdStr, "");
   } /* memory ok */
   mit_rz            = FALSE;        /* useful for debugging link to external simulator */
   mit_statistik     = TRUE;
   statistik_pro_gen = FALSE;
   return (result);
} /* tskDataInit */

