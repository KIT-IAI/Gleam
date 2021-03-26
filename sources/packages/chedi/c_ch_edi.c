/*******************************************************************************
GLEAM/AE                                                                  V1.2.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                           Package: CHain EDitor
                            Module: C_CHain-EDItor
 
Package: ched                   File: c_ch_edi.c  
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt den Chain-Editor der TEXT_MODE-Variante. Der Ketteneditor ist 
im Entwurfsdokument "GLEAM: AK-EDI - Aktionsketteneditor" ausfuehrlich beschrie-
ben.
 
 
Die Routinen im einzelnen:
 
void c_chain_edi (void);
          Ketteneditor fuer AKs in der TEXT_MODE-Variante.
 
--------------------------------------------------------------------------------
basiert auf C-Unit: ch_edi.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Neue Funktion "ConcatAKs"                                        WJ 05.12.98
 2. Kontrolle auf AK-Segmentierung beim Abspeichern.                 WJ 21.12.98
 3. Ueberarbeitung der Menuesteuerung.                               WJ 22.12.98
 4. Linux-Portierung: Aus "round" wird "l_round". Differenzierung 
    zwichen limits.h und values.h                                    WJ 02.06.05
 5. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
 6. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 07.01.08
 7. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 8. Entfernung der Schalter und des dazugehörigen Codes: BS_DOS, 
    BS_WINXP_GNU. Entfernung ueberfluessiger Variable. Initialisie-
    rung von uninitialisierten Variablen.                            WJ 26.06.12
 9. Renovierung: "parameterize_header": "kurz_name" durch "bezeichnung" 
    ersetzt. Streichung aller Scrollingausgaben und des Zeilennummern-
    managements. Anpassung folgender Aufrufe: "char_dialog", "lies_...", 
    "set_and_clear_menue_window", "lies_adr", "select_menue_item". 
    Streichung der Aufrufe von "clr_line". Ersetzung der "write_line"- 
    durch "printf"-Aufrufe.                                          WJ 03.05.16
10. Fehlermeldung NOT_ENOUGH_MEM korrigiert.                         WJ 31.05.17
11. Neuer Parameter fuer "show_and_clear_fbuf".                      WJ 10.11.17
12. Streichung des Includes von LGSW_TEXTS.                          WJ 22.12.17
13. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
14. WITH_DYN_GENES: Additional optional code for the case of genes 
    with unlimited number of parameters.                             WJ 04.07.19
15. Avoiding buffer overflow of static strings: c_chain_edi(),
    puffer_bel_anzeige(), parameterize_header().
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 19.07.2020
================================================================================ */
#include "schalter.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#if defined(BS_SOLARIS)
#include <limits.h>
#endif /* BS_SOLARIS */

#include "sys.h"
#include "fbhm.h"
#include "ctio.h"
#include "lgsw.h"
#include "hmod.h"
#include "bew.h"
#include "chedInt.h"
#include "ched.h"

#include GLOBAL_TEXTS
#include CHED_TEXTS



/* ======================== Package-globale Variable: ======================= */



/* ============================ Lokale Konstante: =========================== */
#define ABORT_FCNT    -1


/* ======================== Lokale Typdeklarationen: ======================== */
typedef CHAIN_ELEM_PTR_TYPE  CHAIN_BUFFER_TYP  [CH_BUF_MAX];


/* ============================ Lokale Variable: ============================ */
static INT               loc_ketten_art; /* Akt. Kettenart des Ketteneditors  */
static INT               lfd_chain_nr,   /* Nummer des aktuellen Kettenpuffers*/
                         lfd_ch_idx;     /* Index des aktuellen Kettenpuffers */
static CHAIN_BUFFER_TYP  chain_buffer;   /* lok.Kettenpuffer von "c_chain_edi"*/


/* ============================ Lokale Routinen: ============================ */
static INT  lies_ch_buf_nr      (INT                  def,
                                 const char          *buf);

static BOOL loe_und_weiter      (INT                  ak_nr);

static BOOL parameterize_action (CHAIN_ELEM_PTR_TYPE  ptr);

static void parameterize_header (CHAIN_ELEM_PTR_TYPE  ptr);

static void ch_art_to_text      (STR_TYPE             buf);

static void hdr_to_text         (STR_TYPE             buf,
                                 SHORT                hdr_code);

static void del_anzeige         (STR_TYPE             buf,
                                 SHORT                act_code);

static BOOL segm_nachfrage      (CHAIN_ELEM_PTR_TYPE  ch_ptr);

static void puffer_bel_anzeige  (void);

static BOOL concat_aks          (INT  ziel_buf_idx,
                                 INT  quell_buf_idx);
 


/* =============================== c_chain_edi ============================== */
void c_chain_edi (void)
{
   char                   bigBuf[2*MAX_STRING];
   STRING_TYPE            buffer;
   STR_TYPE               buf1, buf2, buf3, buf4, buf5;
   BUF_TYPE               sbuf;
   ACT_DESCR_PTR_TYPE     act_descr_ptr;
   REF_DESCR_PTR_TYPE     new_sd_ptr, segm_d_ptr;
   CHAIN_ELEM_PTR_TYPE    anf_ptr, end_ptr, elem_ptr, ak_ptr, neu_ak_ptr,
                          this_chain_ptr, akt_ptr;
   INT     ii, ij, jj, anz, header_code, act_code, anf, end_nr, segm_index,
           str_len, ch_len, ak_nr, akt_nr, lfd_nr, guete, men_item;
   GSTATE  erg_state = GLEAM_OK;
   char    ch;
   BOOL    weiter, fertig, menue_ende, def_is_yes;
   
   /* CHAIN_EDI/CH_EDI */
   lfd_chain_nr   = 1;                                  /* Defaulteinstellung */
   lfd_ch_idx     = 0;                                  /* Defaulteinstellung */
   loc_ketten_art = AK;                                 /* Defaulteinstellung */
   header_code    = AK_HEADER;                          /* Defaulteinstellung */
   menue_ende     = FALSE;
   for (ii = 1;  ii <= CH_BUF_MAX;  ii++)
      chain_buffer[ii-1] = NULL;
   printf ("\n%s\n", CHED_TXT1);
   printf ("\n%s\n", CHED_TXT2);
   printf ("%s\n", CHED_TXT3);
   printf ("\n%s\n\n", CHED_TXT4);
   puffer_bel_anzeige ();
   do
   { /* Menue-Loop */
      set_and_clear_menue_window (3, CHED_MEN_TXT);
      write_menue_item (1,  StndMenItem, CHED_MEN1);            /* GenElement */
      write_menue_item (2,  StndMenItem, CHED_MEN2);            /* ModElement */
      write_menue_item (3,  StndMenItem, CHED_MEN3);            /* LoeElement */
      write_menue_item (4,  StndMenItem, CHED_MEN4);            /* ZeigElem   */
      write_menue_item (5,  StndMenItem, CHED_MEN5);            /* LadeKette  */
      write_menue_item (6,  StndMenItem, CHED_MEN6);            /* RetteKette */
      write_menue_item (7,  StndMenItem, CHED_MEN7);            /* GenKette   */
      write_menue_item (8,  StndMenItem, CHED_MEN8);            /* ModKopf    */
      write_menue_item (9,  StndMenItem, CHED_MEN9);            /* ZeigeKette */
      write_menue_item (10, StndMenItem, CHED_MEN10);           /* ZeigeSegm  */
      write_menue_item (11, StndMenItem, CHED_MEN11);           /* ZeigPuffer */
      write_menue_item (13, StndMenItem, CHED_MEN13);           /* SetzeSegm  */
      write_menue_item (15, StndMenItem, CHED_MEN15);           /* KopiereSeq */
      write_menue_item (16, StndMenItem, CHED_MEN16);           /* SchiebeSeq */
      write_menue_item (17, StndMenItem, CHED_MEN17);           /* KopierePuf */
      write_menue_item (18, StndMenItem, CHED_MEN18);           /* LoeschePuf */
      write_menue_item (19, StndMenItem, CHED_MEN19);           /* ConcatAKs  */
      write_menue_item (21, StndMenItem, CHED_MEN21);           /* NeuDefault */
      menue_completion ();
      men_item = select_menue_item ();
      puffer_bel_anzeige ();
      if ((chain_buffer[lfd_ch_idx] == NULL) &&
          ((men_item != 21) && (men_item != ESC) && (men_item !=  7) &&
           (men_item !=  5) && (men_item != 11)  && (men_item != 18) &&
           (men_item != 19)))
         printf (FMELD_LINE_TXT, NO_WORK_CHAIN);
      else
      { /* Arbeitskette ok */
         this_chain_ptr = chain_buffer[lfd_ch_idx];
         if (this_chain_ptr != NULL)
         {
            header_code = this_chain_ptr->u_el.activity;
            ch_len      = this_chain_ptr->u_el.u_xel.u_hdr.chain_length;
         }
	 else
            ch_len = 0;
         switch (men_item)
         {
            case 1: /* ---------------------- GenItem ----------------------- */
               printf ("\n%s\n\n", CHEDFKT5_TXT1);
               printf ("%s\n", CHEDFKT5_TXT2);
               anf    = ACTIVITY_ACT;
               weiter = TRUE;
               
               for (ii = anf;  ii <= last_act;  ii++) 
               { /* Ausgabe aller Aktionsnamen */
                  get_act_descr (&act_descr_ptr, ii, WITHOUT_GENECODE_CHECK);
                  printf ("%3u.: %s\n", ii, act_descr_ptr->act_name);
		  if ((((ii+1) % MAX_LINE) == 0) && (ii != last_act)) 
		  { /* Unterbrechung ? */  
		     if (char_dialog (SCROLL_QUERY) == ESC)
			break;                                /* abandon loop */
		  } /* Unterbrechung ? */
               } /* Ausgabe aller Aktionsnamen */
               sprintf (buf1, CHEDFKT5_TXT3, anf, last_act);
               if (lies_wert (&act_code, buf1, anf, last_act, last_act+1))
                  break;                            /* Abbruch durch Benutzer */
               get_act_descr (&act_descr_ptr, act_code, WITHOUT_GENECODE_CHECK);
               
               /* ------- Neues Element anlegen und initialisieren: --------- */
               new_chain_elem (&akt_ptr);
#ifdef WITH_DYN_GENES  /* with genes of unlimited size */
	       initGene(akt_ptr, act_code);
#else /* with genes of limited and fixed size */
	       akt_ptr->u_el.activity = act_code;
#endif /* with genes of limited and fixed size */

               /* ---- Werte des neuen Elements vorgeben: ---- */
               if (header_code == AK_HEADER)
               { /* Aktion */
                  if ((act_descr_ptr->i_par_anz > 0) || 
                      (act_descr_ptr->r_par_anz > 0) ||
                      (act_code == ACTIVITY_ACT))
                  { /* Parametrierung einer Aktion */
                     sprintf (buf4, CHEDFKT5_TXT5, act_descr_ptr->act_name);
                     printf ("\n%s\n", buf4);
                     if (act_code != ACTIVITY_ACT)
                     { /* Handlungs-Aktion */
                        if (parameterize_action (akt_ptr))
                        { /* Abbruch durch Benutzer */
                           delete_item (akt_ptr);
                           break;
                        } /* Abbruch durch Benutzer */
                     } /* Handlungs-Aktion */
                     else
                     { /* Unterkette dranhaengen */
                        akt_ptr->u_el.u_xel.u_sub.sub_chain = NULL;
                        akt_ptr->u_el.u_xel.u_sub.ref_typ   = SUB_AK_REF;
                        printf ("%s\n", CHOOSE_SUB_AK);
                        if (lies_adr (&guete, &lfd_nr, "AK "))
                        { /* guete ok */
                           if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete,
                                          &erg_state))
                              append_sub_chain (akt_ptr, ak_ptr);
                           else
                              printf (FMELD_LINE_TXT, NIX_CH);
                        } /* guete ok */
                     } /* Unterkette dranhaengen */
                  } /* Parametrierung einer Aktion */
               } /* Aktion */
               
               /* --------- Neues Element in die Kette einhaengen: ---------- */
               printf ("\n%s\n", CHEDFKT5_TXT6);
               if (ch_len > 0)
               { /* Kette nicht leer */
                  printf ("%s\n", CHEDFKT5_TXT7);
                  sprintf (buf2, CHEDFKT5_TXT8, ch_len);
                  printf ("%s\n", buf2);
                  sprintf (buf2, CHEDFKT5_TXT9, ch_len);
                  if (lies_wert (&akt_nr, buf2, 0, ch_len, ch_len))
                  { /* Abbruch durch Benutzer */
                     delete_item (akt_ptr);
                     break;
                  } /* Abbruch durch Benutzer */
                  else
                  { /* Element einfuegen */
                     count_out_item (&elem_ptr, this_chain_ptr, akt_nr, 
                                     &erg_state);
                     insert_item (this_chain_ptr, elem_ptr, akt_ptr);
                  } /* Element einfuegen */
               } /* Kette nicht leer */
               else
                  append_item (this_chain_ptr, akt_ptr);         /*leere Kette*/
               printf ("\n%s\n", CHEDFKT5_TXT10);
            break; /* GenItem */

            case 2: /* ---------------------- ModItem ----------------------- */
               printf ("\n%s\n\n", CHEDFKT8_TXT1);
               if (ch_len > 0)
               { /* AK nicht leer */
                  sprintf (buf1, CHEDFKT8_TXT2, ch_len);
                  if (lies_wert (&akt_nr, buf1, 1, ch_len, ch_len+1))
                     break;                        /* Abbruch durch Benutzer */
                  count_out_item (&akt_ptr, this_chain_ptr, akt_nr, &erg_state);
                  act_code = akt_ptr->u_el.activity;
		  fertig   = FALSE;             /* steuert die Schlussmeldung */
                  if (act_code == ACTIVITY_ACT)
                  { /* Unterkette */
                     set_elem_nr (akt_nr);
                     zeige_aktion (stdout, akt_ptr, TRUE);
                     printf ("%s\n", CHEDFKT8_TXT3);
                     if (header_code == AK_HEADER)
                     { /* sub_ak */
                        if (lies_adr (&guete, &lfd_nr, "AK "))
                        { /* guete ok */
                           if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete,
                                          &erg_state))
                           {
                              fertig = TRUE;
                              append_sub_chain (akt_ptr, ak_ptr);
                           }
                           else
                              printf (FMELD_LINE_TXT, NIX_CH);
                        } /* guete ok */
                     } /* sub_ak */
                  } /* Unterkette */
                  else
                  { /* elementare Handlung */
                     if (parameterize_action (akt_ptr))
                        break;                      /* Abbruch durch Benutzer */
                     fertig = TRUE;
                  } /* elementare Handlung */
                  if (fertig)
                     printf ("\n%s\n", CHEDFKT8_TXT5);
               } /* AK nicht leer */
               else
                  printf ("\n%s\n", EMPTY_CHAIN);
            break; /* ModItem */

            case 3: /* ---------------------- LoeItem ----------------------- */
               printf ("\n%s\n\n", CHEDFKT9_TXT1);
               if (ch_len > 0)
               { /* AK nicht leer */
                  sprintf (buf4, CHEDFKT9_TXT2, ch_len);
                  if (lies_wert (&anf, buf4, 1, ch_len, ch_len+1))
                     break;                         /* Abbruch durch Benutzer */
                  count_out_item (&akt_ptr, this_chain_ptr, anf-1, &erg_state);
                  remove_next_item (&elem_ptr, this_chain_ptr, akt_ptr);
                  del_anzeige (buf4, elem_ptr->u_el.activity);
                  sprintf (buffer, CHEDFKT9_TXT3, anf, buf4);
                  printf ("\n%s\n", buffer);
                  delete_item (elem_ptr);
               } /* AK nicht leer */
               else
                  printf ("\n%s\n", EMPTY_CHAIN);
            break; /* LoeItem */

            case 4: /* ---------------------- ZeigItem ---------------------- */
               printf ("\n%s\n\n", CHEDFKT6_TXT1);
               sprintf (buf1, CHEDFKT6_TXT2, ch_len);
               if (lies_wert (&akt_nr, buf1, 0, ch_len, ch_len+1))
                  break;                            /* Abbruch durch Benutzer */
               count_out_item (&akt_ptr, chain_buffer[lfd_ch_idx], akt_nr, &erg_state);
               set_elem_nr (akt_nr);
               printf ("\n");
               zeige_aktion (stdout, akt_ptr, TRUE);
            break; /* ZeigItem */
               
            case 5: /* ----------------------- LadeCh ----------------------- */
               printf ("\n%s\n\n", CHEDFKT4_TXT1);
               ch_art_to_text (sbuf);
               ii     = loc_ketten_art;
               weiter = FALSE;
               if (loc_ketten_art == AK)
               {
                  sprintf (buf4, "%s ", sbuf);
                  weiter = lies_adr (&guete, &lfd_nr, buf4);
               }
               else
                  printf ("\n%s\n", CHEDFKT4_TXT2);
               if (weiter)
               { /* weiter */
                  if (loe_und_weiter (lfd_chain_nr))
                  {
                     if (get_chain (&(chain_buffer[lfd_ch_idx]), &lfd_nr, ii, 
                                    guete, &guete, &erg_state))
                     {
                        printf ("\n%s\n", CHAIN_LOADED);
                        (chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.
                         ref_counter)++;
                        puffer_bel_anzeige ();
                     }
                     else
                        printf ("\n%s\n", NIX_CH);
                  }
                  else
                     printf ("\n%s\n", FKT_TERM_TXT);
               } /* weiter */
            break; /* LadeCh */
               
            case 6: /* ---------------------- RetteCh ----------------------- */
               hdr_to_text (sbuf, header_code);
               if (header_code < AK_HEADER)
               {
                  sprintf (buf4, CHEDFKT7_TXT1, sbuf);
                  printf ("\n%s\n", buf4);
               }
               else
               { /* speicherbare Kette */
                  switch (header_code)
                  {
                     case AK_HEADER:
                        ii = AK;
                        break;
                     default:
                        ii = 999;
                        break;
                  } /* CASE */
                  sprintf (buf2, CHEDFKT7_TXT2, sbuf, lfd_chain_nr);
                  printf ("\n%s\n\n", buf2);
                  strcpy (sbuf, JA_NEIN_TXT);
                  def_is_yes = TRUE;
                  if (this_chain_ptr->u_el.u_xel.u_hdr.guete < KEINE_GUETE)
                  { /* Kette bereits gespeichert */
                     printf ("%s\n", CHEDFKT7_TXT3);
                     printf ("%s\n\n", CHEDFKT7_TXT4);
                     strcpy (sbuf, NEIN_JA_TXT);
                     def_is_yes = FALSE;
                  } /* Kette bereits gespeichert */
                  if ((this_chain_ptr->u_el.u_xel.u_hdr.segment_anz <= 0) && (ii == AK))
                  { /* AK nicht segmentiert */
                     printf ("%s\n", CHEDFKT7_TXT9);
                     printf ("%s\n\n", CHEDFKT7_TXT0);
                     strcpy (sbuf, NEIN_JA_TXT);
                     def_is_yes = FALSE;
                  } /* AK nicht segmentiert */
                  sprintf (buf5, "%s%s", CHEDFKT7_TXT6, sbuf);
                  ch = char_dialog (buf5);
                  if ((ch == JA) || (def_is_yes && (ch == CR)))
                  { /* Do it */
                     if (ii == AK)
                        weiter = berechne_guete (&guete, this_chain_ptr->
                            u_el.u_xel.u_hdr.fitness_note);
		     else
			weiter = FALSE;
                     if (weiter)
                     { /* Note ok */
                        store_chain (&lfd_nr, this_chain_ptr, ii, guete);
                        delete_chain (&(chain_buffer[lfd_ch_idx]));
                        adr_to_text (sbuf, guete, lfd_nr, NO_ADJUST);
                        sprintf (buf4, CHEDFKT7_TXT7, sbuf);
                        printf ("%s\n\n", buf4);
                        puffer_bel_anzeige ();
                     } /* Note ok */
                     else
                        printf ("\n%s\n", CHEDFKT7_TXT8);
                  } /* Do it */
                  else
                     printf ("\n%s\n", FKT_TERM_TXT);
               } /* speicherbare Kette */
            break; /* RetteCh */
               
            case 7: /* ----------------------- GenCh ------------------------ */
               ch_art_to_text (sbuf);
               sprintf (buf3, CHEDFKT2_TXT1, sbuf);
               printf ("\n%s\n\n", buf3);
               if (loe_und_weiter (lfd_chain_nr))
               {
                  init_chain (&(chain_buffer[lfd_ch_idx]), header_code);
                  chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.lfd_nr = lfd_chain_nr;
                  chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.guete  = CH_LOKAL;
                  chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.ref_counter = 1;
                  parameterize_header (chain_buffer[lfd_ch_idx]);
                  printf ("\n");
                  puffer_bel_anzeige ();
               }
               else
                  printf ("\n%s\n", FKT_TERM_TXT);
            break; /* GenCh */
               
            case 8: /* ---------------------- ModKopf ----------------------- */
               printf ("\n%s\n\n", CHEDFKT11_TXT1);
               parameterize_header (chain_buffer[lfd_ch_idx]);
            break; /* ModKopf */
               
            case 9: /* ---------------------- ZeigeCh ----------------------- */
               zeige_kette (chain_buffer[lfd_ch_idx]);
            break; /* ZeigeCh */
               
            case 10: /* ------------------- ZeigeSegmente ------------------- */
               printf ("\n%s\n", CHEDFKT12_TXT1);
               if (chain_buffer[lfd_ch_idx]->u_el.activity != AK_HEADER)
                  printf ("\n%s\n", NO_AK_NO_SEGM);
               else
               { /* WITH chain_buffer[lfd_chain_nr]^.header_par_ptr^ */
                  sprintf (buf1, CHEDFKT12_TXT2,
                           chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.segment_anz);
                  printf ("\n%s\n", buf1);
                  if (chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.segment_anz > 0)
                  { /* Die Kette ist segmentiert */
                     printf ("%s\n", CHEDFKT12_TXT3);
                     segm_index = 0;
                     segm_d_ptr = chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.segm_descr_ptr;
                     anf    = 1;
                     end_nr = 0;
		     anz    = chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.segment_anz;
		     for (ii = 1;  ii <= anz;  ii++)
                     { /* Anzeige aller Segmente */
                        sprintf (bigBuf,  "%3u",  ii);
                        akt_ptr = segm_d_ptr->segm_ptrs[segm_index];
                        for (jj = 1;  jj <= 2;  jj++)
                        { /* Segmentanfang und -ende */
                           if (jj == 1)
                              act_code = akt_ptr->u_el.next_action->u_el.activity;
                           else
                              act_code = akt_ptr->u_el.activity;
                           get_act_descr (&act_descr_ptr, act_code, WITHOUT_GENECODE_CHECK);
                           str_len = strlen (act_descr_ptr->act_name);
                           if (str_len > 16) {
                              strncpy (buf2, act_descr_ptr->act_name, 16);
                              buf2[16] = '\0';
                           } else {
                              strcpy (buf2, act_descr_ptr->act_name);
                              for (ij = str_len+1;  ij <= 16;  ij++)
                                 strcat (buf2, " ");
                           }
                           if (jj == 1)
                           { /* Segmentanfang */
                              sprintf (buf1, "%3u", anf);
                              inc_segm_nr (&segm_d_ptr, &segm_index);
                              anf    += segm_d_ptr->segm_length[segm_index];
                              akt_ptr = segm_d_ptr->segm_ptrs  [segm_index];
                           } /* Segmentanfang */
                           else
                           { /* Segmentende */
                              end_nr += segm_d_ptr->segm_length[segm_index];
                              sprintf (buf1,  "%3u",  end_nr);
                           } /* Segmentende */
                           strcat (bigBuf, "  ");
                           strcat (bigBuf, buf1);
                           strcat (bigBuf, ": ");
                           strcat (bigBuf, buf2);
                        } /* Segmentanfang und -ende */
			printf ("%s\n", bigBuf);
			if (((ii % MAX_LINE) == 0) && (ii != anz)) 
		        { /* Unterbrechung ? */  
			   if (char_dialog (SCROLL_QUERY) == ESC)
			      break;                          /* abandon loop */
			} /* Unterbrechung ? */

                     } /* Anzeige aller Segmente */
                  } /* Die Kette ist segmentiert */
               } /* WITH chain_buffer[lfd_chain_nr]^.header_par_ptr^ */
            break; /* ZeigeSegmente */
               
            case 11: /* ---------------------- ZeigPuf ---------------------- */
               printf ("\n%s\n\n", CHEDFKT10_TXT1);
               printf ("%s\n", CHEDFKT10_TXT2);
               for (ii = 0;  ii < CH_BUF_MAX;  ii++)
               { /* alle chain_buffer-Plaetze */
                  sprintf (bigBuf, "%4u", ii+1);
                  if (chain_buffer[ii] == NULL)
                     strcat (bigBuf, CHEDFKT10_TXT3);
                  else
                  { /* Platz belegt */
                     hdr_to_text (buf1, chain_buffer[ii]->u_el.activity);
                     if (strlen (buf1) == 2)
                        strcat (buf1, "  ");
                     strcat (bigBuf, "       ");
                     strcat (bigBuf, buf1);
                     adr_to_text (buf1,
                                  chain_buffer[ii]->u_el.u_xel.u_hdr.guete,
                                  chain_buffer[ii]->u_el.u_xel.u_hdr.lfd_nr,
                                  ADJUST);
                     strcpy (buf5, "  ");
                     strcat (buf5, buf1);
                     strcpy (buf1, buf5);
                     if (chain_buffer[ii]->u_el.u_xel.u_hdr.guete ==
                         KEINE_GUETE)
                        strcat (buf1, " ");
                     else if (chain_buffer[ii]->u_el.u_xel.u_hdr.guete ==
                              CH_LOKAL)
                        sprintf (buf1, CHEDFKT10_TXT4, buf1[5]);
                     sprintf (buf2, "%4u", chain_buffer[ii]->u_el.u_xel.u_hdr.
                              chain_length);
                     strcat (bigBuf, "    ");
                     strcat (bigBuf, buf1);
                     strcat (bigBuf, "    ");
                     strcat (bigBuf, buf2);
                  } /* Platz belegt */
                  printf ("%s\n", bigBuf);
               } /* alle chain_buffer-Plaetze */
            break; /* ZeigPuf */
               
            case 13: /* ------------------- SetzeSegmente ------------------- */
               printf ("\n%s\n", CHEDFKT14_TXT1);
               ak_ptr = chain_buffer[lfd_ch_idx];
               if (ak_ptr->u_el.activity != AK_HEADER)
                  printf ("\n%s\n", NO_AK_NO_SEGM);
               else
               { /* AK */
                  ch_len = ak_ptr->u_el.u_xel.u_hdr.chain_length;
                  if (ch_len > 0)
                  { /* AK nicht leer */
                     segm_d_ptr = ak_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
                     weiter = TRUE;
                     if (segm_d_ptr != NULL)
                     { /* AK bereits segmentiert */
                        printf ("\n%s\n", CHEDFKT14_TXT2);
                        ch = char_dialog (CHEDFKT14_TXT3);
                        if (ch == ESC)
                           break;                         /* Benutzer-Abbruch */
                        if ((ch == CR) || (ch == JA))
                        { /* Do it */
                           while (segm_d_ptr != NULL)
                           { /* Loesche alte Segment-Descriptoren */
                              new_sd_ptr = segm_d_ptr->next_ref_descr_ptr;
                              free_ref_descr (segm_d_ptr);
                              segm_d_ptr = new_sd_ptr;
                           } /* Loesche alte Segment-Descriptoren */
                        } /* Do it */
                        else
                           weiter = FALSE;
                     } /* AK bereits segmentiert */
                     if (weiter)
                     { /* Segmentiere ! */
                        sprintf (buf1, CHEDFKT14_TXT4, ch_len);
                        printf ("\n%s\n", buf1);
                        printf ("%s\n", CHEDFKT14_TXT5);
                        printf ("%s\n", CHEDFKT14_TXT6);
                        printf ("%s\n", CHEDFKT14_TXT9);
                        segm_index = 1;
                        anf        = 1;
                        ii         = 0;
                        weiter     = TRUE;
                        new_ref_descr (&segm_d_ptr);
                        segm_d_ptr->segm_ptrs[0] = ak_ptr;
                        ak_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = segm_d_ptr;
                        while (weiter)
                        { /* Segmentende eingeben */
                           sprintf (buf4, CHEDFKT14_TXT7, ii+1, anf, ch_len);
                           if (lies_wert (&akt_nr, buf4, anf, ch_len, ch_len+1))
                           { /* Abbruch durch Benutzer */
                              weiter = FALSE;
                              akt_nr = ch_len; /* letztes Segm.bis Kettenende */
                           } /* Abbruch durch Benutzer */
                           weiter = akt_nr != ch_len;
                           ii++;
                           count_out_item (&akt_ptr, ak_ptr, akt_nr, 
                                           &erg_state);
                           if (segm_index == SEGM_PTR_ANZ)
                           { /* Ein neues Segmentdescriptor-Item */
                              segm_index = 0;
                              new_ref_descr (&new_sd_ptr);
                              segm_d_ptr->next_ref_descr_ptr = new_sd_ptr;
                              segm_d_ptr = new_sd_ptr;
                           } /* Ein neues Segmentdescriptor-Item */
                           segm_d_ptr->segm_ptrs[segm_index]   = akt_ptr;
                           segm_d_ptr->segm_length[segm_index] = akt_nr+1-anf;
                           anf = akt_nr + 1;
                           segm_index++;
                        } /* Segmentende eingeben */
                        printf ("%s\n", CHEDFKT14_TXT8);
                        ak_ptr->u_el.u_xel.u_hdr.segment_anz = ii;
                     } /* Segmentiere ! */
                  } /* AK nicht leer */
                  else
                     printf ("\n%s\n", EMPTY_CHAIN);
               } /* AK */
            break; /* SetzeSegmente */
               
            case 15: /* ------------------ KopiereSequenz ------------------- */
               printf ("\n%s\n", CHEDFKT13_TXT1);
               this_chain_ptr = chain_buffer[lfd_ch_idx];
               ch_len         = this_chain_ptr->u_el.u_xel.u_hdr.chain_length;
               if (ch_len > 0)
               { /* AK nicht leer */
                  if (segm_nachfrage (this_chain_ptr))
                  { /* ausfuehren */
		     printf ("\n");
                     sprintf (buf4, CHEDFKT13_TXT2, ch_len);
                     if (lies_wert (&anf, buf4, 1, ch_len, ch_len+1))
                        break;                            /* Benutzer-Abbruch */
                     count_out_item (&akt_ptr, this_chain_ptr, anf, &erg_state);
                     sprintf (buf4, CHEDFKT13_TXT3, anf, ch_len);
                     if (lies_wert (&end_nr, buf4, anf, ch_len, ch_len+1))
                        break;                            /* Benutzer-Abbruch */
                     jj = end_nr - anf + 1;
                     copy_seq (&anf_ptr, &end_ptr, akt_ptr, jj);
                     sprintf (buf4, CHEDFKT13_TXT4, ch_len);
                     if (lies_wert (&akt_nr, buf4, 0, ch_len, ch_len+1))
                     { /* Benutzer-Abbruch */
                        delete_seq (anf_ptr, end_ptr, jj);
                        printf ("%s\n", FKT_TERM_TXT);
                     } /* Benutzer-Abbruch */
                     else
                     { /* Ausfuehren */
                        count_out_item (&akt_ptr, this_chain_ptr, akt_nr, &erg_state);
                        insert_seq (this_chain_ptr, akt_ptr, anf_ptr, end_ptr, jj);
                        printf ("%s\n", CHEDFKT13_TXT6);
                     } /* Ausfuehren */
                  } /* ausfuehren */
               } /* AK nicht leer */
               else
                  printf ("\n%s\n", EMPTY_CHAIN);
            break; /* KopiereSequenz */
               
            case 16: /* ------------------ SchiebeSequenz ------------------- */
               printf ("\n%s\n", CHEDFKT15_TXT1);
               this_chain_ptr = chain_buffer[lfd_ch_idx];
               ch_len = this_chain_ptr->u_el.u_xel.u_hdr.chain_length;
               if (ch_len > 0)
               { /* AK nicht leer */
                  if (segm_nachfrage (this_chain_ptr))
                  { /* ausfuehren */
		     printf ("\n");
                     sprintf (buf4, CHEDFKT13_TXT2, ch_len);
                     if (lies_wert (&anf, buf4, 1, ch_len, ch_len+1))
                        break;                            /* Benutzer-Abbruch */
                     count_out_item (&akt_ptr, this_chain_ptr, anf-1, &erg_state);
                     sprintf (buf4, CHEDFKT13_TXT3, anf, ch_len);
                     lies_wert (&end_nr, buf4, anf, ch_len, ch_len+1);
                     jj = end_nr - anf + 1;
                     sprintf (buf4, CHEDFKT15_TXT2, ch_len);
                     if (lies_wert (&akt_nr, buf4, 0, ch_len, ch_len+1))
                        break;                            /* Benutzer-Abbruch */
                     if ((anf <= akt_nr) && (akt_nr <= end_nr))
                        printf ("%s\n", CHEDFKT15_TXT4);
                     else
                     { /* Ziel ok */
                        count_out_item (&elem_ptr, this_chain_ptr, akt_nr, &erg_state);
                        remove_seq (&anf_ptr, &end_ptr, &jj, this_chain_ptr, akt_ptr);
                        insert_seq (this_chain_ptr, elem_ptr, anf_ptr, end_ptr, jj);
                        printf ("%s\n", CHEDFKT15_TXT5);
                     } /* Ziel ok */
                  } /* ausfuehren */
               } /* AK nicht leer */
               else
                  printf ("\n%s\n", EMPTY_CHAIN);
            break; /* SchiebeSequenz */
               
            case 17: /* ------------------- KopierePuffer ------------------- */
               printf ("\n%s\n\n", CHEDFKT16_TXT1);
               if ((ak_nr = lies_ch_buf_nr (lfd_chain_nr, CHEDFKT16_TXT2)) == ABORT_FCNT)
		  break;                                  /* Benutzer-Abbruch */
               if (chain_buffer[ak_nr-1] == NULL)
                  printf ("\n%s\n", NIX_CH);
               else
               { /* Quelle OK */         /* 0 = kein Default */
                  if ((jj = lies_ch_buf_nr (0, CHEDFKT16_TXT3)) == ABORT_FCNT)
                     break;                               /* Benutzer-Abbruch */
                  if (loe_und_weiter (jj))
                  { /* Ziel frei */
                     ak_ptr   = chain_buffer[ak_nr-1];
                     act_code = ak_ptr->u_el.activity;
                     if (act_code != VK_HEADER)
                     { /* Keine VK */
                        if (copy_chain (&neu_ak_ptr, ak_ptr, KEINE_URKETTE,
                                        WITH_MEM_CHECK, &erg_state))
                        { /* OK */
                           neu_ak_ptr->u_el.u_xel.u_hdr.lfd_nr      = jj;
                           neu_ak_ptr->u_el.u_xel.u_hdr.guete       = CH_LOKAL;
                           neu_ak_ptr->u_el.u_xel.u_hdr.ref_counter = 1;
                           printf ("%s\n", CHEDFKT16_TXT4);
                           chain_buffer[jj-1] = neu_ak_ptr;
                        } /* OK */
                        else
                           printf ("%s\n", CHEDFKT16_TXT5);
                     } /* Keine VK */
                     else
                     { /* VK */
                        hdr_to_text (buf1, act_code);
                        strcat (buf1, CHEDFKT16_TXT6);
                        printf ("%s\n", buf1);
                     } /* VK */
                  } /* Ziel frei */
               } /* Quelle OK */
            break; /* KopierePuffer */

            case 18: /* ------------------- LoeschePuffer ------------------- */
               printf ("\n%s\n\n", CHEDFKT17_TXT1);
               if ((ak_nr = lies_ch_buf_nr (lfd_chain_nr, "")) == ABORT_FCNT)
                  break;                            /* Abbruch durch Benutzer */
               if (chain_buffer[ak_nr-1] != NULL)
               { /* OK */
                  delete_chain (&(chain_buffer[ak_nr-1]));
                  printf ("%s\n\n", CHEDFKT17_TXT2);
                  puffer_bel_anzeige ();
               } /* OK */
            break; /* LoeschePuffer */

            case 19: /* ----------------- Konkateniere AKs ------------------ */
               printf ("\n%s\n\n", CHEDFKT18_TXT1);
               if ((ak_nr = lies_ch_buf_nr (lfd_chain_nr, CHEDFKT16_TXT3)) == ABORT_FCNT)
                  break;                                  /* Benutzer-Abbruch */
               if (chain_buffer[ak_nr-1] == NULL)
                  printf ("%s\n", NIX_CH);
               else
               { /* Ziel OK */           /* 0 = kein Default */
                  if ((jj = lies_ch_buf_nr (0, CHEDFKT16_TXT2)) == ABORT_FCNT)
                     break;                               /* Benutzer-Abbruch */
                  if (chain_buffer[jj-1] == NULL)
                     printf ("%s\n", NIX_CH);
                  else
                  { /* Beide AKs existieren */
                     if (concat_aks (ak_nr-1, jj-1))
                        printf ("%s\n", CHEDFKT18_TXT2);
                     else
                        printf ("%s\n", CHEDFKT18_TXT3);
                  } /* Beide AKs existieren */
               } /* Ziel OK */
            break; /*  Konkateniere AKs */
  
            case 21: /* ---------------------- NeuDefs ---------------------- */
               printf ("\n%s\n\n", CHEDFKT1_TXT1);
#ifdef nix /* saved for further extensions */
               printf ("\n%s\n", CHEDFKT1_TXT2);
               ch_art_to_text (buf1);
               do
               {
                  fertig = TRUE;
                  sprintf (buf3, CHEDFKT1_TXT3, buf1);
                  switch (char_dialog (buf3))
                  {
                     case 'A':
                        loc_ketten_art = AK;
                        header_code = AK_HEADER;
                        break;
                     case 'E':
                        loc_ketten_art = EK;
                        header_code = EK_HEADER;
                        break;
                     case 'R':
                        loc_ketten_art = RK;
                        header_code = RK_HEADER;
                        break;
                     case CR:
                        break;
                     default:
                        fertig = FALSE;
                        printf (FMELD_LINE_TXT, BAD_KEY);
                        break;
                  } /* CASE */
               }
               while (!fertig);
#endif /* saved for further extensions */
               puffer_bel_anzeige ();
	       printf ("\n");
               if ((ii = lies_ch_buf_nr (lfd_chain_nr, CHEDFKT1_TXT4)) != ABORT_FCNT)
               { /* Neuer Puffer */
                  lfd_chain_nr = ii;
                  lfd_ch_idx   = ii - 1;
               } /* Neuer Puffer */
	       printf ("\n");
               puffer_bel_anzeige ();
            break; /* NeuDefs */
               
            case ESC: /* --------------------- End/Quit --------------------- */
               ch = char_dialog (CHEDFKT100_TXT1);
               if ((ch == CR) || (ch == JA))
               {
                  menue_ende = TRUE;
                  for (ii = 0;  ii < CH_BUF_MAX;  ii++)
                     if (chain_buffer[ii] != NULL)
                        delete_chain (&(chain_buffer[ii]));
                  printf ("%s\n", CHEDFKT100_TXT2);
               }
              break; /* End/Quit */
         } /* case */
      } /* Arbeitskette ok */
      show_and_clear_fbuf(LIMITED_LOGGING);
   } /* Menue-Loop */
   while (!(menue_ende));
} /* c_chain_edi */ 



/* ============================= lies_ch_buf_nr ============================= */
static INT lies_ch_buf_nr (INT         def,
                           const char *buf)
/*----------------------------------------------------------------------------*/
/*  Liest mit Hilfe von "char_dialog" solange eine Kettenpuffer-Nummer ein,   */
/*  bis ein gueltiger Wert eingegeben wurde (Bereich: 1..CH_BUF_MAX). Dabei   */
/*  kann in "def' ein Defaultwert ( != 0) vorgegeben werden. In "buf" kann    */
/*  der Kettenpuffer naeher bezeichnet werden. Bei Abbruch mit ESC liefert    */
/*  die Routine ABORT_FCNT ab.                                                */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  qbuf, buffer;
   INT       ii;
   char      ch;
   
   /* LIES_CH_BUF_NR/CHAIN_EDI/CH_EDI */
   ii = 0;
   if (def == 0)
      sprintf (qbuf, CHEDLCBN_TXT1, buf, CH_BUF_MAX);
   else
      sprintf (qbuf, CHEDLCBN_TXT2, buf, CH_BUF_MAX, def);
   while ((ii < 1) || (CH_BUF_MAX < ii))
   { /* einen zulaessigen Index verlangen */
      ch = char_dialog (qbuf);
      if (ch == ESC)
      { /* Benutzerabbruch */
         ii = ABORT_FCNT;
	 printf ("%s\n", FKT_TERM_TXT);
         break;
      } /* Benutzerabbruch */
      if ((def > 0) && (ch == CR))
         ii = def;
      else
      { /* Kein Default */
         sprintf (buffer, "%c", ch);
         if (!get_int (&ii, buffer))
            ii = 0;
      } /* Kein Default */
      sprintf (buffer, CHEDLCBN_TXT3, CH_BUF_MAX);
      if ((ii < 1) || (CH_BUF_MAX < ii))
         printf (FMELD_LINE_TXT, buffer);
   } /* einen zulaessigen Index verlangen */
   return (ii);
} /* lies_ch_buf_nr */ 



/* ============================= loe_und_weiter ============================= */
static BOOL loe_und_weiter (INT  ak_nr)
/*----------------------------------------------------------------------------*/
/*  Loescht nach positiver Nachfrage die durch "ak_nr" bezeichnete Arbeits-   */
/*  -kette und liefert TRUE, wenn der Pufferplatz frei (geworden) ist.        */
/*----------------------------------------------------------------------------*/
{
   BOOL      r_loe_und_weiter = TRUE;
   STR_TYPE  buf;
   char      ch;
   
   /* LOE_UND_WEITER/CHAIN_EDI/CH_EDI */
   if (chain_buffer[ak_nr-1] != NULL)
   { /* Arbeitskette geladen */
      sprintf (buf, "%s%1u%s", AK_LOE_QUERY1, ak_nr, AK_LOE_QUERY2);
      ch = toupper (char_dialog (buf));
      if ((ch == CR) || (ch == JA))
         delete_chain (&(chain_buffer[ak_nr-1]));
      else
         r_loe_und_weiter = FALSE;
   } /* Arbeitskette geladen */
   return (r_loe_und_weiter);
} /* loe_und_weiter */ 



/* =========================== parameterize_action ========================== */
static BOOL parameterize_action (CHAIN_ELEM_PTR_TYPE  ptr)
/*----------------------------------------------------------------------------*/
/*  Liest die Parameterwerte der durch "ptr" bezeichneten Aktion ein, wobei   */
/*  die aktuellen Parameterwerte als Default genommen werden. Die in der      */
/*  MOD-Datei enthaltenen Parametergrenzen werden lediglich angezeigt, aber   */
/*  nicht abgeprueft. Liefert TRUE bei Dialogabbruch.                         */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE               buffer;
   ACT_DESCR_PTR_TYPE     act_descr_ptr;
   INT                    ii, iwert;
   BOOL                   abort = FALSE;
   
   /* PARAMETERIZE_ACTION/CHAIN_EDI/CH_EDI */
   get_act_descr (&act_descr_ptr, ptr->u_el.activity, WITHOUT_GENECODE_CHECK);
   for (ii = 0;  (ii < act_descr_ptr->i_par_anz) && !abort;  ii++)
   { /* alle I-Parameter */
      sprintf (buffer, "%s (%1d..%1d) [%1d]: ", 
               act_descr_ptr->i_par_descr[ii].par_name,
               act_descr_ptr->i_par_descr[ii].par_u_grenze,
               act_descr_ptr->i_par_descr[ii].par_o_grenze, 
               ptr->u_el.u_xel.u_act.i_params[ii]);
      abort = lies_wert (&iwert, buffer, INT_MIN, INT_MAX, 
                         ptr->u_el.u_xel.u_act.i_params[ii]);
      ptr->u_el.u_xel.u_act.i_params[ii] = iwert;
   } /* alle I-Parameter */
   for (ii = 0;  (ii < act_descr_ptr->r_par_anz) && !abort;  ii++)
   { /* alle R-Parameter */
      sprintf (buffer, "%s (%g..%.10g) [%e]: ", 
               act_descr_ptr->r_par_descr[ii].par_name,
               act_descr_ptr->r_par_descr[ii].par_u_grenze,
               act_descr_ptr->r_par_descr[ii].par_o_grenze,
               ptr->u_el.u_xel.u_act.r_params[ii]);
      abort = lies_rwert (&(ptr->u_el.u_xel.u_act.r_params[ii]), buffer, 
                          -DBL_MAX, DBL_MAX,  ptr->u_el.u_xel.u_act.r_params[ii]);
   } /* alle R-Parameter */
   return (abort);
} /* parameterize_action */ 



/* =========================== parameterize_header ========================== */
static void parameterize_header (CHAIN_ELEM_PTR_TYPE  ptr)
/*----------------------------------------------------------------------------*/
/*  Submenue zur Header-Parametrierung (derzeit nur fuer AKs).                */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE          buf1;
   STR_TYPE             buf2;
   CHAIN_ELEM_PTR_TYPE  hdr_par_ptr;
   DOUBLE               wert;
   SHORT                hdr_code;
   INT                  men_item, ii;
   LONG                 nwert;
   BOOL                 sub_men_fin;
   
   /* PARAMETERIZE_HEADER/CHAIN_EDI/CH_EDI */
   hdr_code = ptr->u_el.activity;
   if (hdr_code == AK_HEADER)
   { /* ak_header */
      sub_men_fin = FALSE;
      set_and_clear_menue_window (4, AKHDR_MEN_TXT);
      write_menue_item (1, StndMenItem, CHEDPH_TXT1);
      write_menue_item (2, StndMenItem, CHEDPH_TXT2);
      write_menue_item (3, StndMenItem, CHEDPH_TXT3);
      write_menue_item (4, StndMenItem, CHEDPH_TXT4);
      do
      { /* Menue-Loop */
         set_elem_nr (0);
         zeige_aktion (stdout, ptr, TRUE);
         printf ("%s\n", CHEDPH_TXT5);
         menue_completion ();
         men_item = select_menue_item ();
         switch (men_item)
         {
            case 1: /* -------------------- fitness_note -------------------- */
               nwert = l_round (ptr->u_el.u_xel.u_hdr.fitness_note);
               sprintf (buf1, CHEDPH_TXT6, INOTEN_MAX, trunc (KEINE_NOTE), nwert);
               if (lies_lwert (&nwert, buf1, 0, trunc (KEINE_NOTE), nwert))
                  break;                                  /* Funktionsabbruch */
               if (nwert == trunc (KEINE_NOTE))
                  ptr->u_el.u_xel.u_hdr.fitness_note = KEINE_NOTE;
               else
                  ptr->u_el.u_xel.u_hdr.fitness_note = nwert;
            break; /* fitness_note */
               
            case 2: /* -------------------- chain_status -------------------- */
               nwert = ptr->u_el.u_xel.u_hdr.chain_status;
               sprintf (buf1, CHEDPH_TXT8, nwert);
               if (!lies_lwert (&nwert, buf1, 0, 65535, nwert))
                  ptr->u_el.u_xel.u_hdr.chain_status = (USHORT)nwert;
            break; /* chain_status */
               
            case 3: /* ------------------- SimulatorWerte ----------------- */
               hdr_par_ptr = ptr->u_el.u_xel.u_hdr.header_par_ptr;
               ptr->u_el.u_xel.u_hdr.chain_status |= SIMULATED;
               for (ii = 0;  ii < akt_anz_krit;  ii++)
               { /* Ergebniswerte des Simulators eingeben */
                  wert = hdr_par_ptr->sim_erg_werte[ii];
                  strcpy (buf2, kriterien[ii].bezeichnung);
                  buf2[KRIT_BEZ_LEN-1] = '\0';
                  sprintf (buf1, CHEDPH_TXT9, ii+1, buf2, wert);
                  if (lies_rwert (&wert, buf1, -DBL_MAX, DBL_MAX, wert))
                  { /* Funktionsabbruch */
                     ptr->u_el.u_xel.u_hdr.chain_status &= ~SIMULATED;
                     break;
                  } /* Funktionsabbruch */
                  hdr_par_ptr->sim_erg_werte[ii] = wert;
               } /* Ergebniswerte des Simulators eingeben */
            break; /* SimulatorWerte */
               
            case 4: /* -------------------- ref_counter --------------------- */
               sprintf (buf1, CHEDPH_TXT10, ptr->u_el.u_xel.u_hdr.ref_counter);
               if (ptr->u_el.u_xel.u_hdr.ref_counter < 0)
                  ii = 0;
               else
                  ii = ptr->u_el.u_xel.u_hdr.ref_counter;
               if (!lies_wert (&ii, buf1, 0, 32767, ii))
                  ptr->u_el.u_xel.u_hdr.ref_counter = ii;
            break; /* ref_counter */
               
            case ESC:
               sub_men_fin = TRUE;
            break;
               
         } /* case */
         show_and_clear_fbuf(LIMITED_LOGGING);
      } /* Menue-Loop */
      while (!(sub_men_fin));
      printf ("%s\n", CHEDPH_TXT11);
   } /* ak_header */
   else
      printf ("%s\n", CHEDPH_TXT12);
} /* parameterize_header */ 



/* ============================= ch_art_to_text ============================= */
static void ch_art_to_text (STR_TYPE  buf)
/*----------------------------------------------------------------------------*/
/* Liefert den zu "loc_ketten_art" gehoerigen Kettentyp als String in "buf"   */
/* ab.                                                                        */
/*----------------------------------------------------------------------------*/
{
   /* CH_ART_TO_TEXT/CHAIN_EDI/CH_EDI */
   switch (loc_ketten_art)
   {
      case AK:
         strcpy (buf, "AK");
         break;
   } /* CASE */
} /* ch_art_to_text */ 



/* =============================== hdr_to_text ============================== */
static void hdr_to_text (STR_TYPE  buf,
                         SHORT     hdr_code)
/*----------------------------------------------------------------------------*/
/*  Liefert den zu "hdr_code" gehörigen Kettentyp als String in "buf" ab.     */
/*----------------------------------------------------------------------------*/
{
   /* HDR_TO_TEXT/CHAIN_EDI/CH_EDI */
   switch (hdr_code)
   {
      case VK_HEADER:
         strcpy (buf, "VK");
         break;
      case AK_HEADER:
         strcpy (buf, "AK");
         break;
      default:
         strcpy (buf, "??");
         break;
   } /* CASE */
} /* hdr_to_text */ 



/* =============================== del_anzeige ============================== */
static void del_anzeige (STR_TYPE  buf,
                         SHORT     act_code)
/*----------------------------------------------------------------------------*/
/*  Liefert den zu "act_code" gehoerigen Aktions-Bezeichner in "buf" ab.      */
/*----------------------------------------------------------------------------*/
{
   ACT_DESCR_PTR_TYPE  act_descr_ptr;
   
   /* DEL_ANZEIGE/CHAIN_EDI/CH_EDI */
   if (act_code <= last_act)
   { /* Aktion */
      get_act_descr (&act_descr_ptr, act_code, WITHOUT_GENECODE_CHECK);
      sprintf (buf, CHEDDA_TXT1, act_descr_ptr->act_name);
   } /* Aktion */
} /* del_anzeige */ 



/* ============================= segm_nachfrage ============================= */
static BOOL segm_nachfrage (CHAIN_ELEM_PTR_TYPE  ch_ptr)
/*----------------------------------------------------------------------------*/
/*  Bestaetigungsdialog fuer AKs (Erhalt der Segmentierung).                  */
/*----------------------------------------------------------------------------*/
{
   BOOL  r_segm_nachfrage = TRUE;
   
   /* SEGM_NACHFRAGE/CHAIN_EDI/CH_EDI */
   if (ch_ptr->u_el.activity == AK_HEADER)
   { /* potentiell segmentierte Kette */
      if (ch_ptr->u_el.u_xel.u_hdr.segment_anz > 0)
      { /* nachfragen */
         printf ("\n%s\n", CHEDSN_TXT1);
         printf ("%s\n", CHEDSN_TXT2);
         r_segm_nachfrage = toupper (char_dialog (CHEDSN_TXT3)) == JA;
      } /* nachfragen */
   } /* potentiell segmentierte Kette */
   return (r_segm_nachfrage);
} /* segm_nachfrage */ 



/* =========================== puffer_bel_anzeige =========================== */
static void puffer_bel_anzeige (void)
/*----------------------------------------------------------------------------*/
/*  Es wird eine Pufferbelegungsanzeige entsprechend der aktuellen Arbeits-   */
/*  kette in "buffer" aufgebaut und ausgegeben.                               */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE  buffer;
   STR_TYPE     buf2;
   BUF_TYPE     buf1;
   char         buf3[20];
   
   /* PUFFER_BEL_ANZEIGE/CHAIN_EDI/CH_EDI */
   if (chain_buffer[lfd_ch_idx] == NULL)
      strcpy (buf2, CHEDPBA_TXT1);
   else
   { /* Puffer nicht leer */
      hdr_to_text (buf1, chain_buffer[lfd_ch_idx]->u_el.activity);
      if (chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.guete == CH_LOKAL)
         strcpy (buf3, CHEDPBA_TXT2);
      else
         adr_to_text (buf3,
                      chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.guete,
                      chain_buffer[lfd_ch_idx]->u_el.u_xel.u_hdr.lfd_nr,
                      NO_ADJUST);
      sprintf (buf2, "%s: %s", buf1, buf3);
   } /* Puffer nicht leer */
   ch_art_to_text (buf1);
   sprintf (buffer, CHEDPBA_TXT3, lfd_chain_nr, buf2, buf1);
   printf ("%s\n", buffer);
} /* puffer_bel_anzeige */ 




/* =============================== concat_aks =============================== */
static BOOL concat_aks (INT  ziel_buf_idx,
                        INT  quell_buf_idx)
/*----------------------------------------------------------------------------*/
/*  Wenn die durch "ziel_buf_idx" und "quell_buf_idx" bezeichneten AKs        */
/*  existieren wird eine Kopie von "quell_buf_idx" an "ziel_buf_idx" ange-    */
/*  haengt und nachsegmentiert. Die Routine liefert TRUE, wenn alles gut      */
/*  ging, sonst FALSE.                                                        */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  ziel_ak_ptr, start_ptr, end_ptr, ak_end_ptr, dummy_ptr;
   REF_DESCR_PTR_TYPE   sd_ptr;
   INT                  len, sd_idx, segm_anz, dummy;
   GSTATE               erg_status;
   BOOL                 erg = TRUE;

   /* concat_aks */
   if (chain_buffer[ziel_buf_idx] == NULL)
      erg = fehler (CHED_CH_EDI, 1, NO_INFO, NO_INFO, "", NIL_PTR) == GLEAM_OK;
   if (chain_buffer[quell_buf_idx] == NULL)
      erg = fehler (CHED_CH_EDI, 2, NO_INFO, NO_INFO, "", NIL_PTR) == GLEAM_OK;
   if (erg)
   { /* AKs existieren */
      erg        = FALSE;
      ziel_ak_ptr= chain_buffer[ziel_buf_idx];
      len        = chain_buffer[quell_buf_idx]->u_el.u_xel.u_hdr.chain_length;
      segm_anz   = ziel_ak_ptr->u_el.u_xel.u_hdr.segment_anz;
      if (segm_anz > 0)
      { /* AK ist segmentiert */
         erg_status=copy_seq(&start_ptr, &end_ptr, 
                             chain_buffer[quell_buf_idx]->u_el.next_action,len);
         if (erg_status == GLEAM_OK)
         { /* Kopie ok */
            if (ermittle_abschnitt (&dummy_ptr, &ak_end_ptr, &dummy, &sd_idx, 
                                    &sd_ptr, ziel_ak_ptr, segm_anz,&erg_status))
            { /* letztes Segment in Ziel-AK ok */
               ak_end_ptr->u_el.next_action                = start_ptr;
               ziel_ak_ptr->u_el.u_xel.u_hdr.last_action   = end_ptr;
               end_ptr->u_el.next_action                   = NULL;
               ziel_ak_ptr->u_el.u_xel.u_hdr.chain_length += len;
               ziel_ak_ptr->u_el.u_xel.u_hdr.segment_anz++;
               if (inc_segm_nr (&sd_ptr, &sd_idx) == GLEAM_OK)
               {
                  sd_ptr->segm_ptrs  [sd_idx] = end_ptr;
                  sd_ptr->segm_length[sd_idx] = len;
                  erg = segmentiere_ch (ziel_ak_ptr, FALSE) == GLEAM_OK;
               }
               else
                  fehler (CHED_CH_EDI, 3, NO_INFO, NO_INFO, DESCR_ELEM_TXT, NOT_ENOUGH_MEM);
            } /* letztes Segment in Ziel-AK ok */
         } /* Kopie ok */
         else
            fehler (CHED_CH_EDI, 4, chain_buffer[quell_buf_idx]->u_el.activity, 
                    NO_INFO, "", CHAIN_NOT_COPIED);
      } /* AK ist segmentiert */
      else
         fehler (CHED_CH_EDI, 5, NO_INFO,NO_INFO, "", ZIEL_AK_OHNE_SEGM);
   } /* AKs existieren */
   return (erg);
} /* concat_aks */


