/* ---------------------------------------------------------------------------------------
GLEAM/AE                            Package-Header-File                Version:     V1.1.0
Package: webIO                         File: webIO.h
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------

This package contains functions for reading and writing chromosome and result lists based 
on either web- or file-IO. The later is for test purposes only.

Modules of the package:
    listIO   Functions for reading and writing chromosome and result lists based on 
             buffers of type IO_BUFFER_TYPE.
    curlIO   HTTP get and post functions using libcurl.
    fileIO   A file-I/O-based substitute for curlIO for test purposes.

The modules "curlIO" and "fileIO" are alternative implementations of the same interface.
    
------------------------------------------------------------------------------------------
List of changes (first version from 22.10.2018):
 1. "nodeNumber" is declared here, if switches PAR_POP_VERS and EXT_SIMU_SERV 
    are set, as it is used for communication with the External Simulation 
    Services (ESS) for island parallelization.                                 WJ 17.03.20
------------------------------------------------------------------------------------------
Status : o.k.                         Autor: W.Jakob                     Datum: 17.03.2020
------------------------------------------------------------------------------------------ */



/* ================================ Exported Constants: =============================== */
#define NUMBERED_MODEL_LIST   TRUE
#define UNNUMBERED_LIST       FALSE


/* ============================ Exported Type Declarations: =========================== */


/* ================================ Exported Variables: =============================== */

/* -------------------------- variables from module: listIO --------------------------- */
extern STRING_TYPE  urlComBuf;     /* URL incl. taskID/modelWFID for iterative I/O      */
extern STR_TYPE     serverBuffer;  /* URL start for i/o (<access>://<domain>:<port>/opt)*/
extern INT          taskID;        /* task ID of the current task and model             */
extern BOOL         webContext;    /* TRUE, if http com. with ext.SimuServ. For test SW */

#if defined(PAR_POP_VERS) && defined(EXT_SIMU_SERV)
  extern INT        nodeNumber;    /* 0 = initializer, >0: number of island node        */
#endif /* PAR_POP_VERS und EXT_SIMU_SERV */


/* -------------------------- variables from module: visuIO --------------------------- */
extern STR_TYPE     visuServBuf;   /* URL start for i/o (<access>://<domain>:<port>/)   */



/* ================================= Exported Routines: =============================== */

/* -------------------------- routines from module: listIO ---------------------------- */
extern BOOL readIntVal          (INT         *val,
                                 const char  *buf,
                                 char       **endPtr);

extern BOOL readRealVal         (DOUBLE      *val,
                                 const char  *buf,
                                 char       **endPtr);

extern void dumpPostBuf         (void);

extern BOOL initListIO          (void);

/* ------------------- routines from module listIO for gleam side: -------------------- */
extern BOOL writeChrListHeader  (INT     chrNumber);

extern BOOL updateChrListHeader (INT     chrNumber);

extern BOOL writeChrToList      (CHAIN_ELEM_PTR_TYPE  chrPtr,
		                 INT                  chrAddr1,
		                 INT                  chrAddr2);

extern BOOL sendChrList2OptServ (void);

extern BOOL getResultListHeader (INT    *retStatus,
                                 INT    *entryNumber);

extern BOOL getNextResults      (DOUBLE *resDataList,
                                 INT    *parentID,
                                 INT    *childID);

extern void setServerPraeFix    (void);

/* --------------- routines from module listIO for ESS-EvalFrame side: ---------------- */
extern INT  waitForChrList      (INT *maxIntAmount,
                                 INT *maxRealAmount);

extern BOOL getChrHdrData       (INT *parentID,
                                 INT *childID,
                                 INT *chrLen);

extern BOOL get1GeneData        (INT    *geneID,
                                 INT    *intParams,
                                 DOUBLE *realParams);

extern BOOL get1GeneItem        (CHAIN_ELEM_PTR_TYPE  genePtr);

extern BOOL writeResListHeader  (INT     resNumber);

extern void updateResListHeader (INT     badResNumber);

extern BOOL writeResToResList   (INT     chrID1,
                                 INT     chrID2,
                                 DOUBLE *resValues);



/* -------------------------- routines from module: fileIO or curlIO ------------------ */
extern void finalizeCom         (void);

extern BOOL waitForResultList   (INT *waitCtr);

extern void deleteOldResList    (void);

extern BOOL getTaskID           (void);

extern BOOL checkModel          (BOOL       *modelState,
                                 const char *modelName);

extern INT  getModelList        (BOOL        withNumberedItems);

extern BOOL startModel          (const char *modelName);

extern BOOL stopTask            (void);

extern void abortJobOfWorkflow  (void);

extern BOOL prepComToServer     (BOOL  logServerName);

extern BOOL sendResListToServer (void);


/* -------------------------- routines from module: curlIO ---------------------------- */
extern void ioTest              (void);               /* test routine for test programs */

