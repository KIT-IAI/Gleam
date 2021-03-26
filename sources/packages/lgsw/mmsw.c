/*******************************************************************************
GLEAM/AE                                                                  V1.1.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: Listen-GrundSoftWare
                        Module: MemoryManagement-grundSoftWare
 
Package: lgsw                   File: mmsw.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul "mmsw" organisiert eine Freilistenverwaltung der primitiven Datenob-
jekte, die zur Kettenbildung benutzt werden. Diese Objekte sind im File "chain-
def.h" deklariert. Ferner stellt der Modul eine Auskunfts- und eine Anzeige-
funktionen zur Verfuegung. Es kann zum einen abgefragt werden, ob der Speicher-
platz zur Bildung einer bestimmten Kette ausreicht. Ferner kann die aktuelle 
Speicherplatzbelegung angezeigt werden.

Alle Routinen, die ggf. "fehler" oder "fatal" aufrufen, liefern GSTATE als 
Return-Status ab (vgl. Package "fbhm").

Die vorliegende Implementierung unterstellt, dass unter Solaris, Linux und
WinXP/GNU-C genuegend freier Speicher verfuegbar ist und es wird zunaechst von 
einer Maximalbelegung von 2 MB (MAX_MEM) ausgegangen. Ueberschreitet der tat-
saechlich belegte Speicher diesen Wert, wird er jeweils verdoppelt. Dies ist 
aber nur für die Anzeigefuktion interessant ("prep_mem_info" und "mem_info").

Speicherverwaltung unter Solaris, Linux und WinXP/GNU-C:
--------------------------------------------------------
Die Ketten- und RefDescr-Elemente werden direkt mit malloc vom BS geholt und in
getrennten Freilisten verwaltet. 


Die Routinen im einzelnen:

GSTATE new_chain_elem (CHAIN_ELEM_PTR_TYPE *elem_ptr);
         Es wird der Zeiger des naechsten freien Kettenelements in "elem_ptr" ab-
         geliefert. Wenn kein freies Element mehr zur Verfuegung steht, liegt ein
         fataler Fehler vor. Das Element wird mit memset() mit 0 initialisiert.

GSTATE new_ref_descr (REF_DESCR_PTR_TYPE *ref_descr_ptr);
         Es wird der Zeiger des naechsten freien Referenzdescriptorelements in 
         "ref_descr_ptr" abgeliefert. Der Zeiger auf den naechsten Descriptor 
         "next_ref_descr_ptr" ist mit NULL initialisiert. Wenn kein freies 
         Element mehr zur Verfuegung steht, liegt ein fataler Fehler vor. Das 
         Element wird mit Hilfe von memset() mit 0 initialisiert.

GSTATE free_chain_elem (CHAIN_ELEM_PTR_TYPE  elem_ptr);
         Das Datenobjekt, auf das "elem_ptr" zeigt, wird in die Freiliste einge-
         tragen. Ein NIL-Pointer fuehrt zu einem fatalen Fehler. 
         Bei gesetztem WITH_DYN_GENES wird bei Genen bzw. Aktionen geprueft, ob
         Parameterfelder angelegt wurden und diese werden positivenfalls freigegeben.

GSTATE freeHdrParElem (CHAIN_ELEM_PTR_TYPE  elemPtr);
         Das Datenobjekt, auf das "elem_ptr" zeigt, wird in die Freiliste einge-
         tragen. Ein NIL-Pointer fuehrt zu einem fatalen Fehler.

GSTATE free_ref_descr (REF_DESCR_PTR_TYPE  ref_descr_ptr);
         Das Datenobjekt, auf das "ref_descr_ptr" zeigt, wird in die Freiliste 
         eingetragen. Ein NIL-Pointer fuehrt zu einem fatalen Fehler. 

GSTATE free_seq (CHAIN_ELEM_PTR_TYPE  start_ptr,
                 CHAIN_ELEM_PTR_TYPE  end_ptr,
                 INT                  seq_len);
         Die durch "start_ptr" u. "end_ptr" angegebene Sequenz der Laenge 
         "seq_len" wird als Ganzes in die Freiliste eingefuegt. NIL-Pointer 
         fuehren zu einem fatalen Fehler. 

void incr_rel_aks (LONG  amount);
         Dient der Erfassung freigegebener AKs. Der interne Zaehler "released_
         ak" wird um "amount erhoeht.

void reset_rel_aks (void);
         Setzt den internen Zaehler "released_ak" auf Null.

INT check_fl (void);
         Prueft die Freilisten daraufhin ab, ob sie genauso lang sind, wie in 
         den "unused_..."-Zaehlern vermerkt ist. Dabei wird auch die Freilisten-
         kettenkonsistens bis zur erwarteten Laenge geprueft. Die Routine lie-
         fert eine Null ab, wenn alles ok ist. Sonst werden die Fehler binaer 
         codiert, wobei bedeuten:
                1 = "chain_elem_free_list"
                2 = "ref_descr_free_list"

BOOL enough_free_mem (SHORT   chain_type,
                      INT     len,
                      INT     segm_anz);
         Die Funktion TRUE, wenn noch soviel Speicherplatz vorhanden ist oder
         allokiert werden kann, dass eine Kette vom Typ "chain_type" und der 
         Laenge "len" sowie ggf. mit "segm_anz" Segmenten angelegt werden kann. 

GSTATE mmsw_init (void);
         Initialisiert den Modul und prueft, ob mindestens 64 kB verfuegbar 
         sind. Wenn nicht, liegt ein fataler Fehler vor, der als GLEAM-Status 
         zurueckgeliefert wird.

VOID prep_mem_info (BOOL  alles,
                    char *mem_txt_buf);
         Bereitet die Ausgabe von Informationen zur Heap-Speicherbelegung und 
         zur Speicherbereinigung in den Kettenspeichern vor und hinterlegt bei 
         gesetztem "alles" die Texte im Textpuffer von Package "fbhm". Ist 
         "alles" dagegen FALSE, wird nur die Speicherbelegungszeile aufgebaut 
         und in "mem_buf" abgeliefert.


Routinen fuer den TEXT_MODE:

VOID mem_info (VOID);
         Ruft "prep_mem_info" mit gesetztem "alles" auf, gibt den Textpuffer ab 
         Zeile 1 aus und loescht ihn.

--------------------------------------------------------------------------------
basiert auf C-Unit: mmsw
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. DOS-Version: Meldung des verfuegbaren Speichers beim Start.      WJ 07.08.00
 2. Linux-Portierung: Aus "round" wird "l_round" und "i_round".
    BS-Schalter: Differenzierung zwischen Solaris und Linux.         WJ 11.05.05
 3. Im Rahmen der Linux-Portierung wird die Speicherverwaltung fuer
    Solaris u. Linux vereinfacht, indem die Ketten- und RefDescrEle-
    mente direkt mit malloc geholt werden. Die Freilisten bleiben.   WJ 28.09.05
 4. Die Speicherbelegungsanzeige von "prep_mem_info" wurde gegen 
    Ueberlauf abgesichert.                                           WJ 14.12.05
 5. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    
    WinXP/GNU: gleiche Speicherverwaltung wie bei Solaris und Linux. WJ 21.08.07
 6. Entfernung der alten DOS-Speicherverwaltung.                     WJ 21.04.08
 7. Entfernung von BS_WINXP_GNU. "mem_info" wird Teil von TEXT_MODE.
    Entfernung ueberfluessiger Variable. Initialisierung von 
    uninitialisierten Variablen.                                     WJ 26.06.12
 8. Renovierung: Anpassung des "write_text_buf"-Aufrufs. Streichung 
    des "link"-Includes.                                             WJ 18.04.16
 9. Kleinere Verbesserungen am Layout von "prep_mem_info".           WJ 11.07.16
10. Fehlermeldung NOT_ENOUGH_MEM korrigiert.                         WJ 31.05.17
11. Streichung des Includes von Sprachfiles aus anderen Packages.    WJ 22.12.17
12. new_chain_elem() und new_ref_descr() setzten mit Hilfe von 
    memset() alle Werte auf 0. Dadurch entfaellt bei new_ref_descr()
    die Schleife zum Nullen der Pointer.                             WJ 15.06.18
13. free_chain_elem() und free_seq() wurden fuer WITH_DYN_GENE 
    erweitert. Neue Routine freeHdrParElem().                        WJ 08.07.19
14. prep_mem_info(): Avoiding buffer overflow of static strings.     WJ 17.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 17.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "lgsw.h"
#include "lgsw.h"

#include LGSW_TEXTS
#include GLOBAL_TEXTS


#if defined(BS_SOLARIS) || defined(BS_LINUX)
  #define STD_MEM_MAN
#endif /* Solaris oder Linux */



/* ========================== Exportierte Variable: ========================= */
INT   *int_pro_aktion,   /* Integerparameter je Aktion           } von Pack-  */
      *real_pro_aktion;  /* Realparameter je Aktion              } age hmod   */
SHORT  last_act;         /* Genanzahl u. max. Wert v. "activity" } verwaltet  */



/* ============================ Lokale Konstante: =========================== */
#define MB              * 1048576L
#define MAX_MEM              2 MB



/* ============================ Lokale Variable: ============================ */
static LONG                 released_ak;          /* Anz. freigegebener AKs   */
static CHAIN_ELEM_PTR_TYPE  chain_elem_free_list; /* Kettenelement-Freiliste  */
static REF_DESCR_PTR_TYPE   ref_descr_free_list;  /* Ref-Descriptor-Freiliste */
static LONG                 gen_chain_elems,      /* Anz. gen. Kettenelemente */
                            gen_ref_descr,        /* Anz. gen.Ref-Descriptoren*/
                            unused_chain_elems,   /* Länge der KElemFreiliste */
                            unused_ref_descr,     /* Länge d.RefDescr.Freilst.*/
                            avail_bytes;          /* Anzahl verfuegbarer Bytes*/
static INT                  chain_elem_size,      /* Groesse eines Kettenelems*/
                            ref_descr_size;       /* Groesse eines RefDescr.  */




/* ============================= new_chain_elem ============================= */
GSTATE new_chain_elem (CHAIN_ELEM_PTR_TYPE *elem_ptr)
{ /* NEW_CHAIN_ELEM/MMSW */
   if (chain_elem_free_list == NULL)
   { /* Freiliste ist leer */
#if defined(STD_MEM_MAN)
      if ((*elem_ptr = (CHAIN_ELEM_PTR_TYPE)malloc(chain_elem_size)) == NULL)
#endif /* Standard Memory Management */
         return (GLEAM_FATAL);
      gen_chain_elems++;
   } /* Freiliste ist leer */
   else
   { /* Hol's aus der Freiliste */
      *elem_ptr            = chain_elem_free_list;
      chain_elem_free_list = (*elem_ptr)->u_el.next_action;
      unused_chain_elems--;
   } /* Hol's aus der Freiliste */
   memset(*elem_ptr, 0, chain_elem_size);
   return (GLEAM_OK);
} /* new_chain_elem */ 



/* ============================== new_ref_descr ============================= */
GSTATE new_ref_descr (REF_DESCR_PTR_TYPE *ref_descr_ptr)
{
   INT  ii;
   
   /* NEW_REF_DESCR/MMSW */
   if (ref_descr_free_list == NULL)
   { /* Freiliste ist leer */
#if defined(STD_MEM_MAN)
      if ((*ref_descr_ptr = (REF_DESCR_PTR_TYPE)malloc(ref_descr_size)) == NULL)
#endif /* Standard Memory Management */
         return (GLEAM_FATAL);
      gen_ref_descr++;
   } /* Freiliste ist leer */
   else
   { /* Hol's aus der Freiliste */
      *ref_descr_ptr      = ref_descr_free_list;
      ref_descr_free_list = (*ref_descr_ptr)->next_ref_descr_ptr;
      unused_ref_descr--;
   } /* Hol's aus der Freiliste */
   (*ref_descr_ptr)->next_ref_descr_ptr = NULL;
   memset(*ref_descr_ptr, 0, ref_descr_size);
   return (GLEAM_OK);
} /* new_ref_descr */ 



/* ============================= free_chain_elem ============================ */
GSTATE free_chain_elem (CHAIN_ELEM_PTR_TYPE  elem_ptr)
{ /* FREE_CHAIN_ELEM/MMSW */
   if (elem_ptr == NULL)
      return (fatal (LGSW_MMSW, 1, NO_INFO, NO_INFO, "", NIL_PTR));
   elem_ptr->u_el.next_action = chain_elem_free_list;
   chain_elem_free_list       = elem_ptr;
#ifdef WITH_DYN_GENES
   if ((0 < elem_ptr->u_el.activity) && (elem_ptr->u_el.activity < MAX_GENE_CODE))
   { /* element is a gene */
      if (elem_ptr->u_el.u_xel.u_act.i_params != NULL)
	 free(elem_ptr->u_el.u_xel.u_act.i_params);
      if (elem_ptr->u_el.u_xel.u_act.r_params != NULL)
	 free(elem_ptr->u_el.u_xel.u_act.r_params);
   } /* element is a gene */
#endif /* WITH_DYN_GENES */
   unused_chain_elems++;
   return (GLEAM_OK);
} /* free_chain_elem */ 



/* ============================= freeHdrParElem ============================= */
GSTATE freeHdrParElem (CHAIN_ELEM_PTR_TYPE  elemPtr)
{ /* freeHdrParElem */
   if (elemPtr == NULL)
      return (fatal (LGSW_MMSW, 10, NO_INFO, NO_INFO, "", NIL_PTR));
   elemPtr->u_el.next_action = chain_elem_free_list;
   chain_elem_free_list      = elemPtr;
   unused_chain_elems++;
   return (GLEAM_OK);
} /* freeHdrParElem */ 



/* ============================= free_ref_descr ============================= */
GSTATE free_ref_descr (REF_DESCR_PTR_TYPE  ref_descr_ptr)
{ /* FREE_REF_DESCR/MMSW */
   if (ref_descr_ptr == NULL)
      return (fatal (LGSW_MMSW, 2, NO_INFO, NO_INFO, "", NIL_PTR));
   ref_descr_ptr->next_ref_descr_ptr = ref_descr_free_list;
   ref_descr_free_list               = ref_descr_ptr;
   unused_ref_descr++;
   return (GLEAM_OK);
} /* free_ref_descr */ 



/* ================================ free_seq ================================ */
GSTATE free_seq (CHAIN_ELEM_PTR_TYPE  start_ptr,
                 CHAIN_ELEM_PTR_TYPE  end_ptr,
                 INT                  seq_len)
{ 
   CHAIN_ELEM_PTR_TYPE  elemPtr;
   INT                  ii;

   /* FREE_SEQ/MMSW */
   if (start_ptr == NULL)
      return (fatal (LGSW_MMSW, 3, NO_INFO, NO_INFO, "", NIL_PTR));
   if (end_ptr == NULL)
      return (fatal (LGSW_MMSW, 4, NO_INFO, NO_INFO, "", NIL_PTR));
#ifdef WITH_DYN_GENES
   elemPtr = start_ptr;
   for (ii = 0;  (ii < seq_len) && (elemPtr != NULL);  ii++)
   { /* all along the sequence */
      if ((0 < elemPtr->u_el.activity) && (elemPtr->u_el.activity < MAX_GENE_CODE))
      { /* element is a gene */
         if (elemPtr->u_el.u_xel.u_act.i_params != NULL)
	    free(elemPtr->u_el.u_xel.u_act.i_params);
         if (elemPtr->u_el.u_xel.u_act.r_params != NULL)
            free(elemPtr->u_el.u_xel.u_act.r_params);
      } /* element is a gene */
      elemPtr = elemPtr->u_el.next_action;
   } /* all along the sequence */
#endif /* WITH_DYN_GENES */
   end_ptr->u_el.next_action = chain_elem_free_list;
   chain_elem_free_list      = start_ptr;
   unused_chain_elems       += seq_len;
   return (GLEAM_OK);
} /* free_seq */ 



/* ============================== incr_rel_aks ============================== */
void incr_rel_aks (LONG  amount)
{
   released_ak += amount;
} /* incr_rel_aks */ 



/* ============================== reset_rel_aks ============================= */
void reset_rel_aks (void)
{
   released_ak = 0;
} /* reset_rel_aks */ 



/* ================================ check_fl ================================ */
INT check_fl (VOID)
{
   CHAIN_ELEM_PTR_TYPE  ch_elem_ptr;
   REF_DESCR_PTR_TYPE   refd_ptr;
   LONG                 ii;
   INT                  erg;
   
   /* CHECK_FL/MMSW */
   erg = 0;
   ii  = 0;
   if (unused_chain_elems == 0)
   { /* chain_elem-Freiliste soll leer sein */
      if (chain_elem_free_list != NULL)
         erg = 1;
   } /* chain_elem-Freiliste soll leer sein */
   else
   { /* chain_elem-Freiliste soll Elemente enthalten */
      ch_elem_ptr = chain_elem_free_list;
      while ((ch_elem_ptr != NULL) && (ii < unused_chain_elems))
      { /* all along the free list */
         ch_elem_ptr = ch_elem_ptr->u_el.next_action;
         ii++;
      } /* all along the free list */
      if ((ch_elem_ptr != NULL) || (ii != unused_chain_elems))
         erg = 1;
   } /* chain_elem-Freiliste soll Elemente enthalten */
   
   ii = 0;
   if (unused_ref_descr == 0)
   { /* ref_descr-Freiliste soll leer sein */
      if (ref_descr_free_list != NULL)
         erg += 2;
   } /* ref_descr-Freiliste soll leer sein */
   else
   { /* ref_descr-Freiliste soll Elemente enthalten */
      refd_ptr = ref_descr_free_list;
      while ((refd_ptr != NULL) && (ii < unused_ref_descr))
      { /* all along the free list */
         refd_ptr = refd_ptr->next_ref_descr_ptr;
         ii++;
      } /* all along the free list */
      if ((refd_ptr != NULL) || (ii != unused_ref_descr))
         erg += 2;
   } /* ref_descr-Freiliste soll Elemente enthalten */
   return (erg);
} /* check_fl */ 



/* ============================= enough_free_mem ============================ */
BOOL enough_free_mem (SHORT   chain_type,
                      INT     len,
                      INT     segm_anz)
{
   CHAIN_ELEM_PTR_TYPE  elem_ptr;
   REF_DESCR_PTR_TYPE   descr_ptr;
   LONG                 needed_ch_elems,
                        needed_descr;
   INT                  ii;
   BOOL                 mem_ok;
   
   /* enough_free_mem */
   /* --- Speicherbedarf (abzuegl.Freilisten) in MemBlockElems ermitteln: --- */
   needed_ch_elems = len + 2;     /* f. Kettenkopf u. Header-Parameter-Record */
#if defined(STD_MEM_MAN)
   needed_ch_elems -= unused_chain_elems;
#endif /* Standard Memory Management */
   if (chain_type == AK_HEADER)
   { /* Die Kette ist segmentiert */
      needed_descr = ((segm_anz + 1) / SEGM_PTR_ANZ) + 1;
#if defined(STD_MEM_MAN)
      needed_descr -= unused_ref_descr;
#endif /* Standard Memory Management */
   } /* Die Kette ist segmentiert */
   else
      needed_descr = 0;

#if defined(STD_MEM_MAN)
   mem_ok = TRUE;
   if (needed_ch_elems > 0)
   {  /* Speicher in AK-Elem-Freiliste reicht nicht aus */
      for (ii = 0;  (ii < needed_ch_elems) && mem_ok;  ii++)
      { /* Elemente organisieren */
         mem_ok = ((elem_ptr = (CHAIN_ELEM_PTR_TYPE)malloc(chain_elem_size)) != NULL);
         if (mem_ok)
         { /* ab in die  Freiliste */
            elem_ptr->u_el.next_action = chain_elem_free_list;
            chain_elem_free_list       = elem_ptr;
         } /* ab in die  Freiliste */
      } /* Elemente organisieren */
      if (mem_ok)
      { /* es war genuegend Speicher vorhanden */
         unused_chain_elems += needed_ch_elems;
         gen_chain_elems    += needed_ch_elems;
      } /* es war genuegend Speicher vorhanden */
      else
         fatal (LGSW_MMSW, 5, NO_INFO, NO_INFO, CHR_ELEM_TXT, NOT_ENOUGH_MEM);
   }  /* Speicher in AK-Elem-Freiliste reicht nicht aus */
   if (needed_descr > 0)
   {  /* Speicher in Desrcr-Elem-Freiliste reicht nicht aus */
      for (ii = 0;  (ii < needed_descr) && mem_ok;  ii++)
      { /* Elemente organisieren */
         mem_ok = ((descr_ptr = (REF_DESCR_PTR_TYPE)malloc(ref_descr_size)) != NULL);
         if (mem_ok)
         { /* ab in die  Freiliste */
            descr_ptr->next_ref_descr_ptr = ref_descr_free_list;
            ref_descr_free_list           = descr_ptr;
         } /* ab in die  Freiliste */
      } /* Elemente organisieren */
      if (mem_ok)
      { /* es war genuegend Speicher vorhanden */
         unused_ref_descr += needed_descr;
         gen_ref_descr    += needed_descr;
      } /* es war genuegend Speicher vorhanden */
      else
         fatal (LGSW_MMSW, 6, NO_INFO, NO_INFO, DESCR_ELEM_TXT, NOT_ENOUGH_MEM);
   }  /* Speicher in Descr-Elem-Freiliste reicht nicht aus */
   return (mem_ok);
#endif /* Standard Memory Management */
} /* enough_free_mem */ 



/* ================================ mmsw_init =============================== */
GSTATE mmsw_init (void)
{
   /* mmsw_init */
#if defined(STD_MEM_MAN)
   avail_bytes = MAX_MEM;
#endif /* Standard Memory Management */
   chain_elem_size  = sizeof (CHAIN_ELEM_TYPE);
   ref_descr_size   = sizeof (REF_DESCR_TYPE);
   
   /* ------ Mindestspeicher vorhanden und Speicher korrekt definiert: ------ */
   chain_elem_free_list = NULL;
   ref_descr_free_list  = NULL;
   gen_chain_elems      = 0;
   gen_ref_descr        = 0;
   unused_chain_elems   = 0;
   unused_ref_descr     = 0;
   released_ak          = 0;

   /* ------------------ von Package hmod verwaltete Daten: ----------------- */
   last_act             = 0;                 /* no mod file loaded.           */
   int_pro_aktion       = NULL;              /* Datenstruktur noch            */
   real_pro_aktion      = NULL;              /* nicht angelegt.               */

   return (GLEAM_OK);
} /* mmsw_init */ 



/* ============================== prep_mem_info ============================= */
void prep_mem_info (BOOL  alles,
                    char *mem_txt_buf)
/*----------------------------------------------------------------------------*/
/*  Die Anzeige beruht auf der Annahme, dass MAX_MEM Speicher zur Verfuegung  */
/*  steht und verwaltet wird. Wenn jedoch die aktuelle Belegung groesser ist, */
/*  wird die Speichergroesse jeweils verdoppelt. Bei gesetztem "alles" werden */
/*  die Daten in den Textpuffer des Packages "fbhm" abgelegt. Ansonsten wird  */
/*  nur die Speicherbelegungszeile mit "evo_skal"-Skalierung gebildet und in  */
/*  "mem_txt_buf" abgeliefert.                                                */
/*  Anzeigen-Layout:                                                          */
/*                                                                            */
/*             Speicher-Statistik:                                            */
/*                                                                            */
/*  Gesamtspeicher   :yyyyy.x KBytes                                          */
/*  Maximale Belegung:yyyyy.x KBytes yyy.x %                                  */
/*                                                                            */
/*               Kettenelemente:                                              */
/*  Maximale Belegung:xxxxxxx                                                 */
/*  Aktuelle Belegung:xxxxxxx        yyy.x %                                  */
/*                                                                            */
/*             Descriptor-Records:                                            */
/*  Maximale Belegung:xxxxxxx                                                 */
/*  Aktuelle Belegung:xxxxxxx        yyy.x %                                  */
/*                                                                            */
/*  Freigegebene AKs :xxxxxxx                                                 */
/*                                                                            */
/*  Speicherbelegung:                                                         */
/*  ######xxxxxxxxxxxxxxx...................                                  */
/*                                                                            */
/*  Legende zur Speicherbelegung:                                             */
/*  ### = aktuell belegter Speicher: xxx.x%                                   */
/*  xxx = maximal belegter Speicher: xxx.x%                                   */
/*  ... = freier Speicher          : xxx.x%                                   */
/*  --- = anderw.genutzter Speicher: xxx.x%                                   */
/*    . = 2.5% von 4096 KB = 102.4 KB                                         */
/*                                                                            */
/*  <Freilisten-Statuszeile>                                                  */
/*  <Fehler-Statuszeile>                                                      */
/*----------------------------------------------------------------------------*/
{
   const double std_skal = 2.5;          /* Speicheranzeige: Ein Zeichen ent- */
   const double evo_skal = 100.0/66.0;   /* spricht "...skal" Prozent.        */
   
   STRING_TYPE  buffer1;
   STR_TYPE     mem_buf, buffer2;
   LONG         akt_bel_descr,      akt_bel_elems;
   DOUBLE       max_belegt_kbytes,  akt_elems_kb,
                max_restl_mem_kb,   akt_restl_mem_kb,
                max_bel_bytes_proz, akt_bel_bytes_proz,
                akt_bel_elems_proz, akt_bel_descr_proz,
                free_mem_proz,      free_mem_kb,
                sonst_mem_kb,       sonst_mem_proz,
                avail_kb,           skal;
   INT          free_mem_chars,     sonst_mem_chars,
                max_bel_proz_chars, akt_bel_proz_chars,
                ges_anzg_chars,     char_sum,
                offset, znr, ii;
   GSTATE       fstat;
   BOOL         show_all = TRUE;
   
   /* prep_mem_info */
   if (alles)
   { /* Gesamt-Anzeige */
      reset_text_buf ();
      skal = std_skal;
   } /* Gesamt-Anzeige */
   else
      skal = evo_skal;
   ges_anzg_chars = i_round (100.0 / skal);

   max_belegt_kbytes  = 0.0;
   max_belegt_kbytes += ((DOUBLE)(gen_chain_elems) * chain_elem_size +
                         (DOUBLE)(gen_ref_descr)   * ref_descr_size   ) / 1024.0;
   avail_kb           = (DOUBLE)avail_bytes / 1024.0;
   if (max_belegt_kbytes > avail_kb)
   { /* Mehr als MAX_MEM belegt */
      avail_bytes = 2 * avail_bytes;
      avail_kb    = (DOUBLE)avail_bytes / 1024.0;
      show_all    = max_belegt_kbytes <= avail_kb;
   } /* Mehr als MAX_MEM belegt */
   
   akt_bel_elems      = gen_chain_elems - unused_chain_elems;
   akt_elems_kb       = ((DOUBLE)akt_bel_elems * chain_elem_size) / 1024.0;
   if (gen_chain_elems == 0)
      akt_bel_elems_proz = 0.0;
   else
      akt_bel_elems_proz = ((DOUBLE)akt_bel_elems / gen_chain_elems) * 100.0;
   akt_bel_descr    = gen_ref_descr - unused_ref_descr;
   if (gen_ref_descr == 0)
      akt_bel_descr_proz = 0.0;
   else
      akt_bel_descr_proz = ((DOUBLE)akt_bel_descr / gen_ref_descr) * 100.0;
   max_restl_mem_kb   = ((DOUBLE)gen_ref_descr * ref_descr_size) / 1024.0;
   akt_restl_mem_kb   = max_restl_mem_kb -
                        (((DOUBLE)unused_ref_descr * ref_descr_size) / 1024.0);
   akt_bel_bytes_proz = ((akt_elems_kb + akt_restl_mem_kb) / avail_kb) * 100.0;
   max_bel_bytes_proz = (max_belegt_kbytes / avail_kb) * 100.0;
#if defined(STD_MEM_MAN)
   free_mem_kb        = avail_kb - max_belegt_kbytes;
   sonst_mem_kb       = 0.0;
#endif /* Standard Memory Management */
   free_mem_proz      = (free_mem_kb / avail_kb ) * 100.0;
   sonst_mem_proz     = (sonst_mem_kb / avail_kb ) * 100.0;
   
   akt_bel_proz_chars = i_round (akt_bel_bytes_proz / skal);
   max_bel_proz_chars = i_round ((max_bel_bytes_proz - akt_bel_bytes_proz) /
                                     skal);
   free_mem_chars     = i_round (free_mem_proz / skal);
   sonst_mem_chars    = i_round (sonst_mem_proz / skal);
   char_sum           = akt_bel_proz_chars + max_bel_proz_chars +
                        free_mem_chars + sonst_mem_chars;
   if (char_sum != i_round (100.0 / skal))
   { /* Rundungsfehler */
      if (char_sum > i_round (100.0 / skal))
         offset = -1;
      else
         offset = 1;
      if ((akt_bel_proz_chars + max_bel_proz_chars) < free_mem_chars)
         free_mem_chars += offset;
      else
         if (akt_bel_proz_chars < max_bel_proz_chars)
            max_bel_proz_chars += offset;
         else
            akt_bel_proz_chars += offset;
   } /* Rundungsfehler */
   if (alles)
   { /* alles */
      znr = 1;
#ifdef TEXT_MODE
      write_buf (znr++, STRICH_STR);
#endif /* TEXT_MODE */
      write_buf (znr++, MEM_HDR_TXT);
      znr++;                                        /* bewirkt eine Leerzeile */
      if (show_all)
         sprintf (mem_buf, "%7.0f", avail_kb);
      else
         strcpy (mem_buf, "       ");
      sprintf (buffer1, "%s%s KBytes", TOTAL_MEM_TXT, mem_buf);
      write_buf (znr++, buffer1);
      if (show_all)
         sprintf (buffer1, "%s%7.0f KBytes %5.1f %%", MAX_BELEG_TXT,
                  max_belegt_kbytes, max_bel_bytes_proz);
      else
         sprintf (buffer1, "%s%7.0f KBytes", MAX_BELEG_TXT, max_belegt_kbytes);
      write_buf (znr, buffer1);
      znr += 2;
      write_buf (znr++, CH_ELEMS_TXT);
      sprintf (buffer1, "%s%7ld", MAX_BELEG_TXT, gen_chain_elems);
      write_buf (znr++, buffer1);
      sprintf (buffer1, "%s%7ld        %5.1f %%", AKT_BELEG_TXT,
               akt_bel_elems, akt_bel_elems_proz);
      write_buf (znr, buffer1);
      znr += 2;
      write_buf (znr++, DESCR_REC_TXT);
      sprintf (buffer1, "%s%7ld", MAX_BELEG_TXT, gen_ref_descr);
      write_buf (znr++, buffer1);
      sprintf (buffer1, "%s%7ld        %5.1f %%", AKT_BELEG_TXT,
               akt_bel_descr, akt_bel_descr_proz);
      write_buf (znr, buffer1);
      znr += 2;
      sprintf (buffer1, "%s%7ld", FREI_AK_TXT, released_ak);
      write_buf (znr, buffer1);
      znr += 2;
      write_buf (znr++, MEM_ANZG_TXT);
   } /* alles */
   
   strcpy (buffer2, "");
   if ((akt_bel_proz_chars + abs(max_bel_proz_chars) + abs(free_mem_chars) +
        abs(sonst_mem_chars)) <= ges_anzg_chars) 
   { /* Anzeige passt in den Puffer */
      for (ii = 1;  ii <= akt_bel_proz_chars;  ii++)
         strcat (buffer2, "#");
      for (ii = 1;  ii <= max_bel_proz_chars;  ii++)
         strcat (buffer2, "x");
      for (ii = 1;  ii <= free_mem_chars;  ii++)
         strcat (buffer2, ".");
      for (ii = 1;  ii <= sonst_mem_chars;  ii++)
         strcat (buffer2, "-");
   } /* Anzeige passt in den Puffer */
   else
   { /* falsche Anzeige wegen grosser Speichernachforderung */
      for (ii = 1;  ii < ges_anzg_chars;  ii++)
         strcat (buffer2, "#");
      strcat (buffer2, ">>");
   } /* falsche Anzeige wegen grosser Speichernachforderung */

   if (alles)
   { /* alles */
      write_buf (znr, buffer2);
      znr += 2;
      write_buf (znr++, MEM_LEG_TXT);
      sprintf (buffer1, "%s%5.1f%%", MAXBELMEM_TXT, max_bel_bytes_proz);
      write_buf (znr++, buffer1);
      sprintf (buffer1, "%s%5.1f%%", AKTBELMEM_TXT, akt_bel_bytes_proz);
      write_buf (znr++, buffer1);
      sprintf (buffer1, "%s%5.1f%%", FREI_MEM_TXT, free_mem_proz);
      write_buf (znr++, buffer1);
      sprintf (buffer1, "%s%5.1f%%", FEHL_MEM_TXT, sonst_mem_proz);
      write_buf (znr++, buffer1);
      sprintf (buffer1, MEMLEGEND_TXT, skal, l_round (avail_kb),
               (avail_bytes * skal / 102400.0));
      write_buf (znr, buffer1);

      znr += 2;
      ii = check_fl ();
      if (ii == 0)
         strcpy (buffer1, FL_OK_TXT);
      else
      { /* Freilisten-Fehler */
         strcpy (buffer1, FL_ERR_TXT);
         if (ii & 0x1)
            strcat (buffer1, BAD_ELEM_FL);
         if (ii & 0x2)
            strcat (buffer1, BAD_DESCR_FL);
      } /* Freilisten-Fehler */
      write_buf (znr++, buffer1);
      ii = get_fbuf_data (&fstat);
      if (ii == 0)
         strcpy (buffer1, NO_ERR_TXT);
      else
         sprintf (buffer1, "%d", ii);
      switch (fstat)
      {
         case GLEAM_OK:    strcpy (buffer2, OK_STATE_TXT);  break;
         case GLEAM_ERROR: strcpy (buffer2, ERR_STATE_TXT); break;
         case GLEAM_FATAL: strcpy (buffer2, FAT_STATE_TXT); break;
         default         : strcpy (buffer2, "???");         break;
      } /* switch */
      strcat (buffer1, F_MSG_TXT);
      strcat (buffer1, buffer2);
      write_buf (znr++, buffer1);
#ifdef TEXT_MODE
      write_buf (znr++, STRICH_STR);
#endif /* TEXT_MODE */
   } /* alles */
   else
      strcpy (mem_txt_buf, buffer2);
} /* prep_mem_info */ 



#ifdef TEXT_MODE
/* ================================ mem_info ================================ */
void mem_info (void)
{
   prep_mem_info (TRUE, NULL);
   write_text_buf (CLEAR);
} /* mem_info */ 
#endif /* TEXT_MODE */

