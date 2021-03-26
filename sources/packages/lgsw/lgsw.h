/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: lgsw                    File: lgsw.h                    Version: V1.2.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Das Package enthaelt enthaelt alle Routinen zur Erzeugung, Manipulation und Ver-
waltung von einfach-verzeigerten Listen (oder Ketten), wie sie fuer die Gen-Re-
praesentation benoetigt werden.

Module des Packages:
    mmsw     Low-Level-Speicherverwaltung. Freilsistenverwaltung. Speicherbele-
             gungsauskunft mit Hilfe des Textpuffers aus "fbhm".
    balisto  Grundlegende Listenmanipulation. Loeschen und Kopieren von Ketten 
             und Sequenzen (verzeigerte Kettenelemente ohne Kopf), Segmentver-
             waltung, Unterketten. 
    bacha    Kettenspeicher und dessen Verwaltung
    ch_init  Erzeugung und Initialisierung von Ketten. Package-Initialisierung.

-------------------------------------------------------------------------------- 
Status : o.k.                   Autor: W.Jakob                 Datum: 08.07.2019
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define WITH_MEM_CHECK  TRUE                /* fuer "copy_chain"              */
#define NO_MEM_CHECK    FALSE	            /* fuer "copy_chain"              */
#define URKETTE         TRUE	            /* fuer "copy_chain"              */
#define KEINE_URKETTE   FALSE	            /* fuer "copy_chain"              */
#define DAVOR           FALSE               /* fuer "insert_segm_entry"       */
#define DAHINTER        TRUE                /* fuer "insert_segm_entry"       */
#define MIN_GUTE_KETTEN 1                   /* Minimum "guter" Ketten f.SubAK */
#define AK              0                   /* Codes fuer "ketten_art"        */
#define RK              1                   /* Codes fuer "ketten_art"        */
#define SONST           2                   /* Codes fuer "ketten_art"        */



/* ====================== Exportierte Typdeklarationen: ===================== */
#include "chaindef.h"



/* ========================== Exportierte Variable: ========================= */
extern INT     *int_pro_aktion,   /* Integerparameter je Aktion               */
               *real_pro_aktion;  /* Realparameter je Aktion                  */
extern SHORT    last_act;         /* Aktionsanz. u.groesster Wert v."activity"*/



/* ========================== Exportierte Routinen: ========================= */

/* ---------------------- Routinen aus Modul: mmsw -------------------------- */
extern GSTATE new_chain_elem  (CHAIN_ELEM_PTR_TYPE *elem_ptr);

extern GSTATE new_ref_descr   (REF_DESCR_PTR_TYPE  *ref_descr_ptr);

extern GSTATE free_chain_elem (CHAIN_ELEM_PTR_TYPE  elem_ptr);

extern GSTATE freeHdrParElem  (CHAIN_ELEM_PTR_TYPE  elemPtr);

extern GSTATE free_ref_descr  (REF_DESCR_PTR_TYPE   ref_descr_ptr);

extern GSTATE free_seq        (CHAIN_ELEM_PTR_TYPE  start_ptr,
                               CHAIN_ELEM_PTR_TYPE  end_ptr,
                               INT                  seq_len);

extern void   incr_rel_aks    (LONG                 amount);

extern void   reset_rel_aks   (void);

extern INT    check_fl        (void);

extern BOOL   enough_free_mem (SHORT  chain_type,
                               INT    len,
                               INT    segm_anz);

extern void   prep_mem_info   (BOOL  alles,
                               char *mem_txt_buf);

#ifdef TEXT_MODE
extern void   mem_info        (void);
#endif

/* ---------------------- Routinen aus Modul: balisto ----------------------- */
extern GSTATE find_predecessor    (CHAIN_ELEM_PTR_TYPE *pred_item_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr);

extern BOOL   count_out_item      (CHAIN_ELEM_PTR_TYPE *item_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  start_ptr, 
                                   INT                  number,
                                   GSTATE              *ret_status);

extern GSTATE append_item         (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr);

extern GSTATE insert_item         (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr);

extern GSTATE remove_next_item    (CHAIN_ELEM_PTR_TYPE *item_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  behind_ptr);
    
extern GSTATE copy_act            (CHAIN_ELEM_PTR_TYPE *copied_item_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr);

extern GSTATE copy_item           (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr);

extern GSTATE delete_item         (CHAIN_ELEM_PTR_TYPE  item_ptr);

extern GSTATE insert_seq          (CHAIN_ELEM_PTR_TYPE  target_chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  behind_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  anf_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  end_ptr, 
                                   INT                  seq_len);

extern GSTATE remove_seq          (CHAIN_ELEM_PTR_TYPE *anf_ptr, 
                                   CHAIN_ELEM_PTR_TYPE *end_ptr, 
                                   INT                 *number, 
                                   CHAIN_ELEM_PTR_TYPE  source_chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  behind_ptr);

extern GSTATE copy_seq            (CHAIN_ELEM_PTR_TYPE *start_ptr, 
                                   CHAIN_ELEM_PTR_TYPE *end_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  first_ptr, 
                                   INT                  number);

extern GSTATE delete_seq          (CHAIN_ELEM_PTR_TYPE  start_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  end_ptr, 
                                   INT                  seq_len);

extern BOOL   copy_chain          (CHAIN_ELEM_PTR_TYPE *new_chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  old_chain_ptr, 
                                   BOOL                 urkette, 
                                   BOOL                 mem_check,
                                   GSTATE              *ret_status);

extern GSTATE delete_chain        (CHAIN_ELEM_PTR_TYPE *chain_ptr);

extern GSTATE append_sub_chain    (CHAIN_ELEM_PTR_TYPE  item_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE remove_sub_chain    (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr);

extern GSTATE inc_segm_nr         (REF_DESCR_PTR_TYPE  *segm_descr_ptr, 
                                   INT                 *index);
  
extern BOOL   check_segm_ptr      (REF_DESCR_PTR_TYPE  *segm_descr_ptr, 
                                   INT                 *index, 
                                   INT                  segm_anz, 
                                   CHAIN_ELEM_PTR_TYPE  item_ptr,
                                   GSTATE              *ret_status);

extern BOOL   ermittle_abschnitt  (CHAIN_ELEM_PTR_TYPE *pred_ptr, 
                                   CHAIN_ELEM_PTR_TYPE *last_ptr, 
                                   INT                 *len, 
                                   INT                 *index, 
                                   REF_DESCR_PTR_TYPE  *sd_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   INT                  number,
                                   GSTATE              *ret_status);

extern GSTATE insert_segm_entry   (CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   REF_DESCR_PTR_TYPE   segm_d_ptr, 
                                   INT                  index, 
                                   CHAIN_ELEM_PTR_TYPE  last_ptr, 
                                   INT                  seq_len, 
                                   BOOL                 wohin);

extern GSTATE remove_segm_entry   (REF_DESCR_PTR_TYPE   segm_d_ptr, 
                                   INT                  index, 
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE copy_segm_structure (CHAIN_ELEM_PTR_TYPE  new_ch_ptr, 
                                   CHAIN_ELEM_PTR_TYPE  old_ch_ptr);

extern BOOL   teste_ak            (CHAIN_ELEM_PTR_TYPE  ch_ptr);

                            
/* ---------------------- Routinen aus Modul: bacha ------------------------- */
extern BOOL   berechne_guete      (INT                 *guete,
                                   DOUBLE               fit_note);

extern void   get_rank_limits     (LONG                *ug,
                                   LONG                *og,
                                   INT                  guete);

extern GSTATE store_chain         (INT                 *number, 
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                                   INT                  ketten_art, 
                                   INT                  guete);

extern BOOL   get_chain           (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                                   INT                 *number, 
                                   INT                  ketten_art, 
                                   INT                  guete_ug, 
                                   INT                 *guete_og,
                                   GSTATE              *ret_status);

extern INT    number_of_chains    (INT                  guete_ug, 
                                   INT                  guete_og, 
                                   INT                  ketten_art,
                                   GSTATE              *ret_status);

extern CHAIN_ELEM_PTR_TYPE get_vk (INT                  k_art, 
                                   INT                  guete,
                                   GSTATE              *ret_status);

extern void   update_ch_adr       (CHAIN_ELEM_PTR_TYPE  start_ptr);

extern GSTATE umhaengen           (INT                 *neue_lfd_nr, 
                                   INT                  ketten_art, 
                                   INT                  neue_guete,
                                   INT                  alte_guete, 
                                   INT                  alte_lfd_nr);

extern BOOL   select_action_chain (CHAIN_ELEM_PTR_TYPE  item_ptr,
                                   GSTATE              *ret_status);

extern INT    clear_chains        (INT                  ketten_art, 
                                   INT                  zu_loeschen,
                                   GSTATE              *ret_status);

extern GSTATE reclaim_memory      (INT                  ketten_art);

extern GSTATE clr_and_reorg       (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                                   INT                  ketten_art);

extern GSTATE clear_vk            (INT                  ketten_art, 
                                   INT                  guete);

extern GSTATE delete_ak_mem       (void);

extern GSTATE delete_mem          (void);

extern GSTATE check_bak           (BOOL                *ak_exists,
                                   BOOL                *ak_is_bak,
                                   INT                  guete,
                                   INT                  lfd_nr);

extern GSTATE do_bak_to_ak        (CHAIN_ELEM_PTR_TYPE  ak_ptr);

extern GSTATE del_chain           (BOOL                *chain_deleted,
                                   INT                  ketten_art, 
                                   INT                  guete, 
                                   INT                  lfd_nr, 
                                   BOOL                 del_bak);


/* ---------------------- Routinen aus Modul: ch_init ----------------------- */
extern GSTATE init_chain          (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                                   SHORT                ch_typ);

extern GSTATE lgswInit            (void);










