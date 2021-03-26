/* -----------------------------------------------------------------------------
GLEAM/AE                 Sprach-Header-File (Englisch)
Package: lgsw                    File: lgsw_gb.h             Version:     V1.1.3
Status : o.k.                   Autor: W.Jakob                 Datum: 29.09.2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */


/* ----------------------- Texte des Moduls: mmsw ------------------------------ */
#define STRICH_STR      "----------------------------------------"
#define MEM_HDR_TXT     "           Memory Statistics:"
#define TOTAL_MEM_TXT   "Total memory:"
#define CH_ELEMS_TXT    "             List Elements:"
#define DESCR_REC_TXT   "          Descriptor Elements:"
#define MAX_BELEG_TXT   "Maximum used:"
#define AKT_BELEG_TXT   "Present used:"
#define FREI_AK_TXT     "Released ACs:"
#define MEM_ANZG_TXT    "Memory allocation:"
#define MEM_LEG_TXT     "    Legend for memory allocation:"
#define MAXBELMEM_TXT   "xxx = maximum of used memory: "
#define AKTBELMEM_TXT   "### = presently used memory : "
#define FREI_MEM_TXT    "... = unused memory         : "
#define FEHL_MEM_TXT    "--- = unavailable memory    : "
#define MEMLEGEND_TXT   "  . = %G%% of %ld KB = %3.1f KB"
#define FL_OK_TXT       "The unused element lists are ok."
#define FL_ERR_TXT      "Error in unused element list for: "
#define BAD_ELEM_FL     "elements "
#define BAD_DESCR_FL    "descriptors"
#define NO_ERR_TXT      "No"
#define F_MSG_TXT       " messages at state: "
#define CHR_ELEM_TXT    "Chromosome elements"
#define DESCR_ELEM_TXT  "Chromosome descriptors"


/* ----------------------- Texte des Moduls: balisto --------------------------- */
#define AK_NULL_TXT     "Chromosome pointer is NULL"
#define NO_AK_TXT       "No chromosome! Header code = %d"
#define BAD_LASTACT_TXT "\"last_action\"-field of chromosome header does not point to last gene."
#define BAD_AKLEN_TXT   "Incorrect chromosome length! Hdr=%d  act.val=%d"
#define NO_DESCR_TXT    "Segmented chromosome has no segment descriptor record!"
#define NO_MORE_SEGM    "Segment descriptor end found. Segment number: Hdr=%d act.val=%d "
#define NO_SEGM_ENDE    "Segment %d: SegmEnd not found. Length: Descr=%d RestAC=%d"
#define SEGM_LEN_ERR    "Segment %d: Bad Segmentlength: Hdr=%d act.val=%d"
#define SEGM_NIX_DA     "Segment %d does not exist: Length=%d, pred_ptr: "
#define PRED_PTR_OK     "OK, end_ptr: "
#define PRED_PTR_NULL   "NULL, end_ptr: "
#define END_PTR_OK      "OK"
#define END_PTR_NULL    "NULL"
#define NIX_GUT_ENDE    "End of chromosome not equal to end of last segment."
#define LEN_NE_SUM      "Chromosome length (%d) unequal to sum of segment elements (%d)!"
#define AK_IST_UNSEGM   "Chromosome is not segmented."
#define UNSEGM_REC      "Unsegmented chromosome has at least one segment descriptor record!"
