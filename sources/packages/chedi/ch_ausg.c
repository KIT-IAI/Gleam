/*******************************************************************************
GLEAM/AE                                                                  V1.2.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                            Package: CHain EDitor
                             Module: CHain-AUSGabe
 
Package: ched                   File: ch_ausg.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul stellt die Routinen zur Anzeige einzelner Kettenelemente und ganzer 
Ketten zur Verfuegung. Dazu werden pro Kettenelement die notwendigen Strings 
im Textpuffer des "fbhm"-Packages aufgebaut.

Die TEXT_MODE-Version enthaelt zusaetzlich Routinen zur Ausgabe ganzer Ketten 
in eine Textdatei oder per Scroll-Ausgabe auf den Schirm zur Verfuegung.

Das Ausgabeformat betraegt einheitlich maximal 66 Zeichen. Die Ausgabe erfolgt 
in folgenden Formaten:

Kettenkopf:
    123456789012345678901234567890123456789012345678901234567890123456
    VerwaltungsKette der Laenge xxxx:
    AktionsKette gg/nnn der Laenge xxxx:
 
    Kopfdaten:
       Fitness=xxxxxx  Simu=jjnn  Refs=xxx  Stat=xx  Segm=xxx 
    
Kettenlemente:
    1234567890123456789012345678901234567890123456..9012345678901234567890123456789012345678901234
    nnn.: <20 Char Aktionsname>:
    <----- Par-Name ---> = xxxxxxxxxxxx uuuuuuuuuu  <----- Par-Name ---> = xxxxxxxxxxxx uuuuuuuuuu
                                 . . .
    <----- Par-Name ---> = xxxxxxxxxxxx uuuuuuuuuu  <----- Par-Name ---> = xxxxxxxxxxxx uuuuuuuuuu
 
Dabei wird der Wert maximal 12-stellig ("xxxxxxxxxxxx") im G-Format ausgegeben 
und "uuuuuu" steht fuer Unit.


Die Routinen im einzelnen:

void set_elem_nr (INT  elem_nr);
         Setzt die Elementnummer, die bei der Elementausgabe durch "prep_zeige_
         aktion" ausgegeben wird. Die Routine ist vor Beginn der Ketten- oder
         Kettenelementausgabe mit der laufenden Nummer des ersten auszugebenden 
         Kettenelements aufzurufen.

BOOL prep_zeige_aktion (CHAIN_ELEM_PTR_TYPE  ptr);
         Die Routine gibt die Daten des durch "ptr" bezeichneten Kettenelements
         in den Textpuffer des "fbhm"-Packages aus. Dabei wird die interne 
         Elementnummer "loc_elem_nr", die die laufende Nummer des Elements in der 
         Kette angibt, mit ausgegeben und inkrementiert. Die Routine liefert 
         TRUE, wenn es noch ein Folgeelement in der Kette gibt und "ptr" selbst 
         ungleich Nil ist. 

BOOL zeige_aktion (FILE                *dest,
                   CHAIN_ELEM_PTR_TYPE  ptr,
                   BOOL                 on_screen);
         Die Routine gibt Objekte vom Typ "chain_elem_type" aus, also Ketten-
         elemente und Header samt deren Parametern. "ptr" zeigt auf das auszu-
         gebende Objekt. Wenn "on_screen" TRUE ist, erfolgt die Ausgabe auf den 
         Bildschirm und der Returnwert ist immer TRUE. Andernfalls gibt die 
         Funktion in das durch "dest" bezeichnete File aus und liefert bei 
         fehlerfreier Ausgabe TRUE zurueck, sonst FALSE.
         Die Routine benutzt "prep_zeige_aktion" zum Aufbau der Texte eines 
         Kettenelements im Textpuffer des "fbhm"-Packages.

void zeige_kette (CHAIN_ELEM_PTR_TYPE  ptr);
         Gibt die in "ptr" enthaltene Kette je nach Benutzeranforderung auf den 
         Bildschirm oder in eine Datei unter Benutzung von "zeige_aktion" aus. 
         Die Bildschirmausgabe kann alle 20 AK-Elemente abgebrochen werden.
         Wenn "lfd_nr" im Kettenkopf Null ist, wird keine Kettenadresse ausge-
         geben (VK).
         
--------------------------------------------------------------------------------
basiert auf C-Unit: ch_edi.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Linux-Portierung: Aus "round" wird "l_round".                    WJ 11.05.05
 2. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 07.01.08
 3. Streichung von GLEAM_C. Stattdessen regelt GUI_MODE die Details 
    des Exports unter der Randbedingung, dass das Package in der 
    Engine-Version nicht mituebersetzt wird.                         WJ 28.06.12
 4. "zeige_aktion" und "zeige_kette" werden nur bei gesetztem
    TEXT_MODE uebersetzt und exportiert.                             WJ 10.11.14
 5. "prep_zeige_aktion" an die Laengen der Bezeichner angepasst.     WJ 24.03.16
 6. Renovierung: Streichung des "link"-Includes. Anpassung folgender 
    Aufrufe: "zeige_aktion", "zeige_kette", "dialog", "char_dialog"
    und "lies_...". Streichung der Aufrufe von "clr_line". Ersetzung 
    der "write_line"- durch "printf"-Aufrufe.                        WJ 04.05.16
 7. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
 8. prep_zeige_aktion(), zeige_kette(): Avoiding buffer overflow of
    static strings.                                                  WJ 19.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 19.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "ctio.h"
#include "lgsw.h"
#include "hmod.h"
#include "chedInt.h"
#include "ched.h"

#include GLOBAL_TEXTS
#include CHED_TEXTS



/* ============================ Lokale Konstante: =========================== */
#define  ACT_MAX_LEN  (PAR_NAME_MAX + EINHEIT_MAX + 12 + 4)



/* ============================ Lokale Variable: ============================ */
INT      loc_elem_nr;      /* Ordnungsnummer des auszugebenden Kettenelements */



/* ============================ Lokale Routinen: ============================ */
#ifndef GUI_MODE
static BOOL prep_zeige_aktion (CHAIN_ELEM_PTR_TYPE  ptr);
#endif




/* =============================== set_elem_nr ============================== */
void set_elem_nr (INT  elem_nr)
{
   loc_elem_nr = elem_nr;
} /* set_elem_nr */ 



#ifndef GUI_MODE
static
#endif
/* ============================ prep_zeige_aktion =========================== */
BOOL prep_zeige_aktion (CHAIN_ELEM_PTR_TYPE  ptr)
{
   ACT_DESCR_PTR_TYPE  act_descr_ptr;
   STRING_TYPE  buffer;
   STR_TYPE     l_buf, buf, buf1;
   BUF_TYPE     chain_adr_buf, buf2, buf3, buf5;
   INT          ii, ij, znr;
   SHORT        act_code;
   BOOL         r_zeige_aktion;
   BOOL         hdr;
   
   /* prep_zeige_aktion */
   znr = 2;
   if (ptr == NULL)
   { /* Nix da ! */
      sprintf (buffer, KEIN_ELEM_TXT, loc_elem_nr);
      write_buf (1, buffer);
      r_zeige_aktion = FALSE;
   } /* Nix da ! */
   else
   { /* ptr OK */
      r_zeige_aktion = ptr->u_el.next_action != NULL;
      act_code       = ptr->u_el.activity;
      hdr            = FALSE;
      sprintf (buf1, "%3u", loc_elem_nr);
      if ((ACTIVITY_ACT <= act_code) && (act_code <= last_act))
      { /* Aktion */
         get_act_descr (&act_descr_ptr, act_code, WITHOUT_GENECODE_CHECK);
         sprintf (buffer, "%s.: %s:", buf1, act_descr_ptr->act_name);
         write_buf (1, buffer);
         if (act_code == ACTIVITY_ACT)
         { /* Aufruf einer Unterhandlung */
            if (ptr->u_el.u_xel.u_sub.sub_chain == NULL)
               strcpy (buffer, NO_SUB_AK);
            else
            { /* es gibt einer Unterkette */
               if (ptr->u_el.u_xel.u_sub.ref_typ == SUB_AK_REF)
               { /* AK-Referenz */
                  adr_to_text (buf1, ptr->u_el.u_xel.u_sub.sub_chain->
                               u_el.u_xel.u_hdr.guete, ptr->u_el.u_xel.
                               u_sub.sub_chain->u_el.u_xel.u_hdr.lfd_nr, 
                               NO_ADJUST);
                  sprintf (buffer, UNTER_AK_TXT, buf1);
               } /* AK-Referenz */
               else
                  if (ptr->u_el.u_xel.u_sub.ref_typ == LOC_SUB_AK)
                  { /* Lokale Unter-AK */
                     sprintf (buffer, LOK_UAK_TXT, 
                              ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.
                              chain_length);
                  } /* Lokale Unter-AK */
                  else
                     sprintf (buffer, BAD_UAK_TXT, 
                              ptr->u_el.u_xel.u_sub.ref_typ);
            } /* es gibt einer Unterkette */
            write_buf (znr++, buffer);
         } /* Aufruf einer Unterhandlung */
         else
         { /* Alle anderen Handlungen */
            znr = 1;
            for (ii = 0;  ii < act_descr_ptr->i_par_anz;  ii++)
            { /* I-Parameter */
               sprintf (buf, "%s = %1d ", 
                        act_descr_ptr->i_par_descr[ii].par_name,
                        ptr->u_el.u_xel.u_act.i_params[ii]);
               if (strlen (act_descr_ptr->i_par_descr[ii].einheit) < (size_t)EINHEIT_MAX)
                  ij = strlen (act_descr_ptr->i_par_descr[ii].einheit);
               else
                  ij = EINHEIT_MAX;
               strncpy (l_buf, act_descr_ptr->i_par_descr[ii].einheit, ij);
               l_buf[ij] = '\0';
               strcat (buf, l_buf);                    /* add unit name       */
               for (ij = strlen (buf);  ij <= ACT_MAX_LEN;  ij++)
                  strcat (buf, " ");                   /* fill up with blanks */
               if ((ii % 2) == 0)
                  strcpy (buffer, buf);                /* Beginne Puffer      */
               else
               { /* Fuelle Puffer auf */
                  strcat (buffer, "  ");
                  strcat (buffer, buf);
                  znr++;
                  write_buf (znr, buffer);
               } /* Fuelle Puffer auf */
            } /* I-Parameter */
            if ((act_descr_ptr->i_par_anz % 2) == 1)
            { /* Schreibe angefangenen Puffer */
               znr++;
               write_buf (znr, buffer);
            } /* Schreibe angefangenen Puffer */
            for (ii = 0;  ii < act_descr_ptr->r_par_anz;  ii++)
            { /* R-Parameter */
               sprintf (buf, "%s = %e ", 
                        act_descr_ptr->r_par_descr[ii].par_name,
                        ptr->u_el.u_xel.u_act.r_params[ii]);
               if (strlen (act_descr_ptr->r_par_descr[ii].einheit) < (size_t)EINHEIT_MAX)
                  ij = strlen (act_descr_ptr->r_par_descr[ii].einheit);
               else
                  ij = EINHEIT_MAX;
               strncpy (l_buf, act_descr_ptr->r_par_descr[ii].einheit, ij);
               l_buf[ij] = '\0';
               strcat (buf, l_buf);                    /* add unit name       */
               for (ij = strlen (buf);  ij <= ACT_MAX_LEN;  ij++)
                  strcat (buf, " ");                   /* fill up with blanks */
               if ((ii % 2) == 0)
                  strcpy (buffer, buf);                /* Beginne Puffer      */
               else
               { /* Fuelle Puffer auf */
                  strcat (buffer, " ");
                  strcat (buffer, buf);
                  znr++;
                  write_buf (znr, buffer);
               } /* Fuelle Puffer auf */
            } /* R-Parameter */
            if ((act_descr_ptr->r_par_anz % 2) == 1)
            { /* Schreibe angefangenen Puffer */
               znr++;
               write_buf (znr, buffer);
            } /* Schreibe angefangenen Puffer */
            znr++;
         } /* Alle anderen Handlungen */
      } /* Aktion */
      else
         if ((VK_HEADER <= act_code) && (act_code <= AK_HEADER))
         { /* Header */
            hdr = TRUE;
            adr_to_text (chain_adr_buf, ptr->u_el.u_xel.u_hdr.guete,
                         ptr->u_el.u_xel.u_hdr.lfd_nr, NO_ADJUST);
            if (ptr->u_el.u_xel.u_hdr.guete == CH_LOKAL)
            {
               strcpy (buf5, PUFFER_TXT);
               strcat (buf5, chain_adr_buf);
               strcpy (chain_adr_buf, buf5);
            }
            if (act_code == VK_HEADER)
            { /* vk_header */
               strcpy (chain_adr_buf, "");
               strcpy (buf, VERW_TXT);
            } /* vk_header */
            else
               if (act_code == AK_HEADER)
               { /* ak_header */
                  strcpy (buf, AKTIONS_TXT);
                  write_buf (2, "");
                  write_buf (3, HDR_DATA_TXT);
                  if (ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED)
                     strcpy (buf2, JA_TXT);
                  else
                     strcpy (buf2, NEIN_TXT);
                  if (ptr->u_el.u_xel.u_hdr.fitness_note == KEINE_NOTE)
                     strcpy (buf3, NIX_NOTE_TXT);
                  else
                     sprintf (buf3, "%.4f", ptr->u_el.u_xel.u_hdr.fitness_note);
                  sprintf (buffer, HDR_ZEILE_TXT,
                           buf3, buf2, ptr->u_el.u_xel.u_hdr.ref_counter,
                           ptr->u_el.u_xel.u_hdr.chain_status,
                           ptr->u_el.u_xel.u_hdr.segment_anz);
                  write_buf (4, buffer);
                  znr = 5;
               } /* ak_header */
               else
               { /* Header-Mist */
                  sprintf (buffer, BAD_HDR_TXT, buf1, chain_adr_buf, act_code);
                  write_buf (1, buffer);
                  hdr = FALSE;
               } /* Header-Mist */
               if (hdr)
               { /* gueltiger Header */
                  sprintf (buffer, X_KETTE_TXT, buf, chain_adr_buf,
                           ptr->u_el.u_xel.u_hdr.chain_length);
                  write_buf (1, buffer);
               } /* gueltiger Header */
         } /* Header */
         else
         { /* Mist */
            sprintf (buffer, BAD_CODE_TXT, buf1, act_code);
            write_buf (1, buffer);
         } /* Mist */
         write_buf (znr, "");
   } /* ptr OK */
   loc_elem_nr++;
   return (r_zeige_aktion);
} /* prep_zeige_aktion */ 



#ifdef TEXT_MODE
/* ============================== zeige_aktion ============================== */
BOOL zeige_aktion (FILE                *dest,
                   CHAIN_ELEM_PTR_TYPE  ptr,
                   BOOL                 on_screen)
{
   STR_TYPE  buffer;
   INT       ii;
   BOOL      weiter;
   
   /* zeige_aktion */
   prep_zeige_aktion (ptr);                        /* Daten in den Textpuffer */
   weiter = TRUE;
   if (on_screen)
      write_text_buf (CLEAR);                            /* Bildschirmausgabe */
   else
   { /* Ausgabe in Datei */
      ii  = 1;
      while (get_buf (ii, buffer) && weiter)
      { /* Puffer ausgeben */
         strcat (buffer, "\n");
         weiter = print_and_check (dest, buffer);           /* Puffer-Ausgabe */
         ii++;
      } /* Puffer ausgeben */
      reset_text_buf ();
   } /* Ausgabe in Datei */
   return (weiter);
} /* zeige_aktion */ 



/* =============================== zeige_kette ============================== */
void zeige_kette (CHAIN_ELEM_PTR_TYPE  ptr)
{
   STRING_TYPE  buffer;
   STR_TYPE     fname, buf;
   INT          index, end_index, anz, ii;
   char         ch;
   BOOL         weiter, on_screen, fertig;
   FILE        *ak_print_file;
   
   /* zeige_kette */
   if (ptr == NULL)
      printf (FMELD_LINE_TXT, NIX_CH_TXT);
   else
   { /* ptr OK */
      /* Wo soll's den hingehen?: */
      printf ("\n%s\n\n",AUSG_HDR_TXT);
      do
      { /* Wohin solls ausgegeben werden? */
         fertig = TRUE;
         ch     = char_dialog (AUSG_QUERY);
         if (ch == CR)
            ch = SCREEN_CHAR;                              /* "B" ist Default */
         end_index = ptr->u_el.u_xel.u_hdr.chain_length;
         switch (ch)
         {
            case SCREEN_CHAR: /* Bildschirm */
               on_screen = TRUE;
               weiter    = TRUE;
            break; /* Bildschirm */
               
            case FILE_CHAR: /* File */
               on_screen = FALSE;
               printf ("%s\n", FILE_EXT_TXT);
               if (dialog (FILE_QUERY, fname))
                  weiter = FALSE;                                  /* Abbruch */
               else  /* File-Name erhalten */
                  weiter = (ak_print_file = fopen (fname, "w+t")) != NULL;
            break; /* File */
               
            case ESC:
               weiter = FALSE;                                     /* Abbruch */
            break;
               
            default:
               fertig = FALSE;
               printf (FMELD_LINE_TXT, BAD_KEY);
            break;
         } /* CASE */
      } /* Wohin solls ausgegeben werden? */
      while (!(fertig));
      
      if (weiter)
      { /* Ausgabemedium ist bereit */
         reset_text_buf ();
         sprintf (buf, START_QUERY, end_index);
         if (lies_wert (&index, buf, 0, end_index, 0))
         { /* Abbruch */
            if (!on_screen)
               fclose (ak_print_file);        /* Fileausgabe: File schliessen */
         } /* Abbruch */
         else
         { /* Startindex erhalten */
            if (on_screen)
               printf ("\n");                /* Bildschirm-Ausgabe: Leerzeile */
            else
            { /* Dateiausgabe */
               sprintf (buf, END_QUERY, end_index, end_index);
               weiter = !lies_wert (&end_index, buf, 0, end_index, end_index);
            } /* Dateiausgabe */
            
            /* -------------- Nun kann die Ausgabe losgehen: --------------*/
            if (weiter)
            { /* Jetzt wird wirklich ausgegeben */
               if (index > 0)
                  weiter = count_out_item (&ptr, ptr, index, &ii);
               if (ptr == NULL)
                  printf ("%s\n", NIX_DA_TXT);
               set_elem_nr (index);
               ii  = 1;
               anz = end_index - index + 1;
               while (weiter && (ptr != NULL) && (index <= end_index))
               { /* Ausgabeschleife */
                  weiter = zeige_aktion (ak_print_file, ptr, on_screen);
                  ptr = ptr->u_el.next_action;
                  index++;
		  if (on_screen && ((ii % 20) == 0) && (ii != anz)) 
		  { /* Unterbrechung ? */  
		     if (char_dialog (SCROLL_QUERY) == ESC)
			break;                                /* abandon loop */
		  } /* Unterbrechung ? */
		  ii++;
               } /* Ausgabeschleife */
            } /* Jetzt wird wirklich ausgegeben */
            if (!on_screen)
            { /* Keine Bildschirmausgabe */
               fclose (ak_print_file);
               sprintf (buffer, ABSCHL_TXT, fname);
               printf ("%s\n", buffer);
            } /* Keine Bildschirmausgabe */
         } /* Startindex erhalten */
      } /* Ausgabemedium ist bereit */
   } /* ptr OK */
} /* zeige_kette */ 
#endif /* TEXT_MODE */
