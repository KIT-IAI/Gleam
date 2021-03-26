/* ---------------------------------------------------------------------------------------
GLEAM/AE                           Internal Header-File                Version:     V1.1.0
Package: webIO                        File: webIOInt.h
Status : o.k.                        Autor: W.Jakob                      Datum: 18.10.2018
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or the files "schalter.h".
------------------------------------------------------------------------------------------ */



/* ============================ Package-globale Konstanten: =========================== */
#define ACCESS_METHOD          "http://"  
#define PORT_TXT               ":8090"    
#define ROOT_DIR               "/opt"
#define GET_TASK_ID_CMD        "/taskID"
#define GET_MODEL_STATE_CMD    "/active"
#define GET_MODEL_LIST_CMD     "/models"
#define POST_MODEL_CONFIG_CMD  "/start"
#define POST_CHR_LIST_CMD      "/input"
#define GET_RESULT_LIST_CMD    "/result"
#define GET_RESULT_CHR_CMD     "/clresult"
#define GET_STOP_CMD           "/stop"
#define GET_ABORT_CMD          "/abort"

#define MOD_START_MOD_TXT      "model: %s\n"
#define MOD_START_ALG_TXT      "algorithmName: %s\n"
#define SIMU_JOB_TXT           "SimuJob"
#define LSV_JOB_TXT            "LSVJob"




/* ========================= Package-globale Typdeklarationen: ======================== */
typedef struct IO_BUFFER_TYPE {
   char   *memory;             /* memory buffer                                         */
   size_t  totalSize,          /* total size of the memory buffer                       */
           usedSize;           /* currently used size of the memory buffer              */
   char   *nextPos,            /* pointer to the character after the last conversion    */
          *errStartPos;        /* points to the begin of the string used for the actual */
                               /* reading operation. For error messages due to bad data */
} IO_BUFFER_TYPE;



/* ============================= Package-globale Variable: ============================ */
extern IO_BUFFER_TYPE  resBuf,   /* common result buffer for web-based communication    */
                       postBuf,  /* common post buffer (at present used by "/start" only*/
                       chrLiBuf; /* post buffer for a chromosome list                   */
extern STRING_TYPE     logBuf;   /* buffer for creating log messages                    */
extern INT             modelWFID;/* workflow ID of the current model                    */

 

/* ============================= Package-globale Routinen: ============================ */

/* -------------------------- routines from module: listIO ---------------------------- */
extern BOOL   generateErrMsg      (const char *modulName,
                                   INT         origin,
                                   INT         retStatus,
                                   INT         errNo);

extern size_t writeLocalBuffer  (void   *contents, 
                                 size_t  size, 
                                 void   *locBuf);

extern BOOL   createLocalBuffer   (IO_BUFFER_TYPE *memBuf);

extern void   resetLocalBuffer    (IO_BUFFER_TYPE *memBuf);

extern void   dumpResBuf          (const char  *title);



/* -------------------------- routines from module: fileIO or curlIO ------------------ */
extern BOOL   sendChromosomeList  (BOOL  secondTry);

extern BOOL   setupCom            (void);

extern void   ioInit              (void);

