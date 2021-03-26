/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: chio                    File: chio.h                    Version: V1.0.1
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Das Package enthaelt Routinen zur Linearisierung und Delinearisierung von AKs 
zum Zwecke der Stream-I/O. Der aktuelle Implementierungsstand realisiert bi-
naere und textuelle File-I/O sowie PVM-basierte I/O fuer die Parallelversion,
siehe Schalter "PAR_VERS".

Module des Packages:
    chain_io   File- und MPI-I/O, Kettenrestaurierung und -rettung.
    mpi_chio   Hoehere Routinen der MPI-basierten AK-Kommunikation.

--------------------------------------------------------------------------------
Status : o.k.                   Autor: W.Jakob                 Datum: 27.06.2019
-------------------------------------------------------------------------------- */



/* ========================== Exportierte Variable: ========================= */
extern INT  initialChrFileMode;   /* Fileformat der initialen Chromosomendatei*/
extern INT  defaultChrFileMode;   /* Default-Fileformat der Chromosomendatei  */



/* ========================= Exportierte Konstanten: ======================== */
#define CH_READ         TRUE      /* Fuer Param. "lesen" von "open_ch_file"   */
#define CH_WRITE        FALSE     /* Fuer Param. "lesen" von "open_ch_file"   */
#define GLOBAL_SAVE     TRUE      /* Fuer Param. "glo_sav" von "open_ch_file" */
#define SELECTIVE_SAVE  FALSE     /* Fuer Param. "glo_sav" von "open_ch_file" */
#define CH_START        TRUE      /* F. Param. "reset_state" von "read_chain" */
#define NO_CH_START     FALSE     /* F. Param. "reset_state" von "read_chain" */
#define FILE_BIN_IO     1         /* Code von "ch_io_mode" bzw. "io_mode"     */
#define FILE_TXT_IO     2         /* Code von "ch_io_mode" bzw. "io_mode"     */
#define MPI2_IO         3         /* Code von "ch_io_mode" bzw. "io_mode"     */


/* ========================== Exportierte Routinen: ========================= */

/* ---------------------- Routinen aus Module chain_io: --------------------- */
extern void   set_log_state    (BOOL        mit_logging);

extern BOOL   open_ch_file     (const char *fspec,
                                INT         start_znr,
                                BOOL        lesen,
                                BOOL        glo_sav,
                                INT         io_mode);

extern void   close_ch_file    (void);

extern GSTATE markiere_ak      (INT         guete,
                                INT         lfd_nr);

extern GSTATE markiere_aks     (INT         guete);

extern BOOL   save_marked_aks  (BOOL        als_bak);

extern void   clear_save_marks (void);

extern BOOL   save_chain       (CHAIN_ELEM_PTR_TYPE  ch_ptr, 
                                BOOL                 als_bak);

extern BOOL   save_ch_mem      (void);

extern BOOL   save_baks        (void);

extern void   clr_bak_indices  (void);

extern BOOL   restore_chs      (BOOL                 basis_aks,  
                                BOOL                 anzeige);

extern BOOL   ch_file_ok       (void);

extern BOOL   read_chain       (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                                BOOL                 reset_state);

extern BOOL   restore_ch_ref   (CHAIN_ELEM_PTR_TYPE  ch_ptr);

extern BOOL   chain_io_init    (BOOL                 mit_logging);


#ifdef MPI_PAR_VERS
/* ---------------------- Routinen aus Module mpi_chio: --------------------- */
  extern BOOL sendeAK          (CHAIN_ELEM_PTR_TYPE  akPtr,
                                INT                  msgCode,
                                INT                  indIdx,
                                INT                  recAnz,
                 	        INT                 *ReceiverList);

  extern BOOL recSendInitAK    (DOUBLE              *note,
                                INT                  laenge);

  extern BOOL recAKMsg         (CHAIN_ELEM_PTR_TYPE *akPtr,
                                INT                 *indIdx,
	                        INT                  laenge,
                                INT                  msgCode,
                 	        INT                  sender);

  extern void logMaxParMSGCount(void);

  extern BOOL freeAKBuffers    (INT                 *freeElemIdx);
#endif /* MPI_PAR_VERS */
