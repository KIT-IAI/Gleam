/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Deutsch)
Package: lgsw                    File: lgsw_d.h              Version:     V1.1.2
Status : o.k.                   Autor: W.Jakob                 Datum: 29.05.2017
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */


/* ----------------------- Texte des Moduls: mmsw ------------------------------ */
#define STRICH_STR      "----------------------------------------"
#define MEM_HDR_TXT     "           Speicher-Statistik:"
#define TOTAL_MEM_TXT   "Gesamtspeicher   :"
#define CH_ELEMS_TXT    "             Kettenelemente:"
#define DESCR_REC_TXT   "           Descriptor-Records:"
#define MAX_BELEG_TXT   "Maximale Belegung:"
#define AKT_BELEG_TXT   "Aktuelle Belegung:"
#define FREI_AK_TXT     "Freigegebene AKs :"
#define MEM_ANZG_TXT    "Speicherbelegung:"
#define MEM_LEG_TXT     "Legende zur Speicherbelegung:"
#define MAXBELMEM_TXT   "xxx = maximal belegter Speicher: "
#define AKTBELMEM_TXT   "### = aktuell belegter Speicher: "
#define FREI_MEM_TXT    "... = freier Speicher          : "
#define FEHL_MEM_TXT    "--- = anderweitig bel. Speicher: "
#define MEMLEGEND_TXT   "  . = %G%% von %ld KB = %3.1f KB"
#define FL_OK_TXT       "Die Freilisten sind ok."
#define FL_ERR_TXT      "Freilisten-Fehler! Inkonsistent: "
#define BAD_ELEM_FL     "AK-Elemente-FL "
#define BAD_DESCR_FL    "Descriptor-FL"
#define NO_ERR_TXT      "Keine"
#define F_MSG_TXT       " Meldungen bei Status: "
#define CHR_ELEM_TXT    "Chromsosomenelemente"


/* ----------------------- Texte des Moduls: balisto --------------------------- */
#define AK_NULL_TXT     "AK-Zeiger ist NULL"
#define NO_AK_TXT       "Keine AK! Header-Code=%d"
#define BAD_LASTACT_TXT "\"last_action\"-Feld im AK-Kopf zeigt nicht auf die letzte Aktion."
#define BAD_AKLEN_TXT   "Ak-Laenge stimmt nicht! Kopf=%d  Ist=%d"
#define NO_DESCR_TXT    "Segmentierte AK hat keinen Segmentdescriptor-Record!"
#define NO_MORE_SEGM    "Ende der Segmentdescriptoren erreicht. Segmentanzahl: Soll=%d Ist=%d "
#define NO_SEGM_ENDE    "Segment %d: SegmEnde nicht gefunden. L"ae"nge: Soll=%d Rest-AK=%d"
#define SEGM_LEN_ERR    "Segment %d: Segmentl"ae"nge stimmt nicht: Soll=%d Ist=%d"
#define SEGM_NIX_DA     "Segment %d existiert nicht: L"ae"nge=%d, pred_ptr: "
#define PRED_PTR_OK     "OK, end_ptr: "
#define PRED_PTR_NULL   "NULL, end_ptr: "
#define END_PTR_OK      "OK"
#define END_PTR_NULL    "NULL"
#define NIX_GUT_ENDE    "AK-Ende nicht identisch mit dem Ende des letzten Segments."
#define LEN_NE_SUM      "AK-L"ae"nge (%d) ungleich Segmentelementsumme (%d)!"
#define AK_IST_UNSEGM   "AK ist unsegmentiert."
#define UNSEGM_REC      "Unsegmentierte AK hat Segmentdescriptor-Record!"
