/*******************************************************************************
GLEAM/AE                                                                  V1.6.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: C-Terminal-I/O
                        Module: SCReen GrundSoftWare

Package: ctio                   File: scr_gsw.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Historie:
=========
Dieses Modul enthielt urspruenglich die low-level Terminal I/O-Routinen der 
TEXT_MODE-Variante fuer reine ASCII-Text-I/O und alternativ mit Borlands BGI-
Grafik im BGI_MODE fuer PC-Grafik basierend auf EGA/VGA-Karten.  
Im Sommer 2012 wurden die BGI-Anteile entfernt. Trotzdem wurden Beschreibungen 
des BGI-Anteils belassen, um den Kontext mancher Regelungen und Aufrufparameter 
zu erklaeren.

Anfang 2016 wurden alle BGI-Hinterlassenschaften im Rahmen der Renovierung voll-
staendig entfernt.


Aktueller Stand:
================
Die gesamte Benutzer-I/O der TEXT_MODE-Variante laeuft ueber diesen Modul in Ver-
bindung mit "user_gsw" und "str_gsw". Einzige Ausnahme ist der Modul "evo_anzg" 
des evo-Packages, von dem es aber Varianten fuer die anderen Modi gibt.
Ansonsten findet die Benutzer-IO der TEXT_MODE-Variante in der "xxx_men"-Modulen 
statt, siehe die Doku der jeweiligen Packages. 

Die Zeilen- und Dialogausgaben werden auf MAX_STRING Zeichen begrenzt, wobei bei 
den Dialogen noch etwas Platz für den Eingabetext abgezogen wird. Die Konstante
ist im Packages "sys" in "ttype.h" zu finden und betraegt 256. 

Menue-Items sind maximal 10 Zeichen (MEN_ITEM_LEN) lang. Das Menue ist 4-spaltig 
und die Anzahl der Zeilen ergibt sich aus der Itemanzahl. Letztere ist auf
MAX_MEN_ITEM_ANZ beschraenkt. MAX_MEN_ITEM_ANZ betraget derzeit 22.
Jedes Item kann eine der drei folgenden Farbwerte annehmen:
  StndMenItem   sichtbares und selektierbares Standard-Menue-Item
  DarkMenItem   ausgegrautes und im aktuellen Kontext ungueltiges Menue-Item 
                Im TEXT_MODE wird es nicht dargestellt.  
  AttMenItem    sichtbares und selektierbares Menue-Item in Warnfarbe.
                Im TEXT_MODE wird es wie ein StndMenItem dargestellt.  


Die Routinen im einzelnen:

char char_dialog (const char *out_text);
          Der Dialog findet unter Ausgabe von "out_text" in der aktuellen Zeile 
          statt. Es wird auf die Eingabe eines Zeichens gewartet, dessen Upper-
          Case-Wert als Echo direkt hinter "out_text" ausgegeben und als Funk-
          tionsergebnis abgeliefert wird. Zeichen mit einem ASCII-Code kleiner 
          als 32 werden nicht geechot.
          "out_text" wird gegebenenfalls auf eine Laenge von MAX_STRING - 2 ge-
          kuerzt.

BOOL dialog (const char *out_text,
             char       *in_text);
          Die Routine gibt "out_text" als Frage-String aus, liest die Antwort
          in "in_text" ein und liefert TRUE ab, wenn der Dialog mit ESC abge-
          brochen wurde. 
          Direkt hinter "out_text" wird ein Cursor erzeugt und in "in_text" mit
          Echo eingelesen. Der Text wird mit <enter> abgeschlossen. Die Routine 
          enthaelt einen einfachen Zeileneditor mit den Funktionen "Cursor 
          rechts/links", "Delete", "Delete backwards", "Home" und "End".

void set_and_clear_menue_window (INT         men_tiefe,
                                 const char *men_title);
          "men_tiefe" darf MAX_MEN_SCHACHTEL nicht ueberschreiten. Sonst erfolgt
          eine Fehlermeldung und es geschieht nichts. "men_title" wird entspre-
          chend "men_tiefe" in "men_titles" eingetragen. Die Menueausgabe erfolgt 
          ab der naechstenZeile. 

void write_menue_item (INT         item_nr,
                       INT         item_color,
                       const char *item_text);
          Generell gilt, dass "item_nr" kleiner als MAX_MEN_ITEM_ANZ sein muss
          und dass die Routine die Menueeintraege zur Ausgabe durch "menue_
          completion" vorbereitet. Im Falle einer zu grossen "item_nr" erfolgt
          eine Fehlermeldung und das Item wird ignoriert.
          Die "item_nr" wird in ein Itemzeichen bestehend aus Ziffern und Buch-
          staben umgerechnet und die zeilenweise Ausgabe in 4er-Spalten zusammen 
          mit dem "item_text" vorbereitet. "item_color" wirkt sich im TEXT_MODE
          nur dahingehend aus, dass ein "DarkMenItem" nicht angezeigt wird.

INT select_menue_item (void);
          Liefert die Nummer des ausgewaehlten Menue-Items ab. Der Versuch, ein
          nicht angebotenes Item zu selektieren, fuehrt zu einer Fehlermeldung.
          Es werden also nur gueltige Selektionen zugelassen. Der Returncode 
          fuer Menueende ist ESC. 
          Die Items durch den Items vorangestellte Ziffern und Buchstaben kennt- 
          lich gemacht. Aus dem eingegebenen Zeichen wird die Itemnummer berech-
          net und abgeliefert.

void menue_completion (void);
          Die Routine bewirkt die Ausgabe des Menuekopfes "men_title" und aller 
          Menue-Items, die seit dem letzten "set_and_clear_menue_window" mit 
          "write_menue_item" angelegt worden sind. Sie schliesst damit die 
          Ausgabe des Menues ab, die aus dem Menuekopf, allen Menue-Items und
          der Menueabschlusszeile besteht. 

void ctioInit (void);
          Initialisierungsroutine des package ctio.

--------------------------------------------------------------------------------
Aenderungen gegenueber Version 1.0.0:
 1. Verwendung von "char *" statt STR_TYPE bei den Funktions-
    parametern.                                                      WJ 03.11.98
 2. Im Animationsfenster bleibt der Kopf und bei GLEAM-R wird
    rechts kein Text mehr ausgegeben.                                CB 25.11.98
 3. Harmonisierung des Kopfbereichs und der Logos mit allen 
    Varianten. Registrierung des "sanserif"-Fonts.                   WJ 10.12.98
 4. Vorgabe des Startitems bei "select_menue_item".                  WJ 22.12.98
 5. "write_hdr_once" an neues GLEAM-Akronym angepasst.               WJ 01.08.00
 6. MAX_HTFLINE und LINE_LENGTH werden entsprechend den BS- und den  
    Benutzerinterface-Schaltern eingestellt. "write_line" akzeptiert 
    bei gesetztem SMALL_SCREEN die im BGI-Mode zugelassenen Zeilennr.WJ 08.08.00
 7. LINE_LENGTH bei nicht gesetztem BGI_MODE auf 88 erhoeht.         WJ 04.09.00
 8. Linux-Portierung: Differenzierung zwischen Solaris und Linux 
    durch BS-Schalter. Eigene Zeichenerkennung fuer Linux. Erweite-
    rung des Exithandlers.                                           WJ 12.05.05
 9. LINE_LENGTH bei nicht gesetztem BGI_MODE auf MAX_STR-1 gesetzt.  WJ 23.06.06
10. Ausgabe der Versionszeile in "init_normal_window" verbessert.    WJ 04.01.07
11. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
12. BS-abhaengige Anpassung der Laenge der Menue-Items und der Aus-
    gabelaenge fuer die Multibytes-Strings von Linux: Menue-Items
    mit Umlauten sind mit Blanks auf MEN_ITEM_LEN aufzufuellen.      WJ 22.04.09
13. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
14. Entfernung der Schalter und des dazugehoerigen Codes: BS_DOS, 
    BS_WINXP_GNU, BGI_MODE, SMALL_SCREEN. GLEAM_C --> TEXT_MODE.
    Streichung folgender Routinen: def_gen_graf_window, 
    set_gen_graf_window, clear_gen_graf_window, def_gen_text_window, 
    set_gen_text_window, clear_gen_text_window, def_simu_window, 
    set_simu_window, clear_simu_window, unset_simu_window, 
    flip_simu_page, start_flip, stop_flip, write_logo_twice.
    Entfernung ueberfluessiger Variable.                             WJ 26.06.12
15. Anpassung des "exit_handler" an die MPI_PAR_VERS.                WJ 26.07.12
    Package "pcom" geht in "mPar" auf.                               WJ 09.11.12
16. Die Schlussmeldung des Exithandlers wird zum Exithandler von
    Package "fbhm" verschoben.                                       WJ 21.01.13
17. Renovierung: "char_dialog" und "dialog": Korrektur des printf-
    Aufrufs und Vereinfachungen. Streichung aller BGI-Spezifika. 
    Streichung von "set_main_window", "clear_main_window", "clr_line ", 
    "clear_dialog_window", "set_menue_window" und "clear_menue_window". 
    Aus "init_normal_window" wird "startMeldung". Anpassung der 
    Menue-Routinen.                                                  WJ 10.05.16
18. Erweiterung der Startmeldung wegen der LGPL. Anpassung des Auf-
    rufs von "write_to_log".                                         WJ 19.12.16
19. Fehler bei zu langen Query-Strings in dialog() behoben.          WJ 09.04.18
20. set_and_clear_menue_window(), write_menue_item(): Avoiding 
    buffer overflow of static strings.                               
    Small change to function startMeldung() and moving it to the 
    men-package and summarizing its texts with the version texts.    WJ 14.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 14.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "ctioInt.h"

#include GLOBAL_TEXTS
#include CTIO_TEXTS



/* ========================== Exportierte Variable: ========================= */
const INT      StndMenItem = 111;         /* Standard-Menue-Item              */
const INT      DarkMenItem = 222;         /* aktuell ungueltiges Menue-Item   */
const INT      AttMenItem  = 333;         /* Menue-Item in Warn"farbe"        */



/* ============================ Lokale Konstante: =========================== */
#define HALB_STRICH_TXT    "-------------------------------"
#define MENUE_STRICH_TXT   HALB_STRICH_TXT HALB_STRICH_TXT
#define STRICH_TXT         MENUE_STRICH_TXT "----"
#define MEN_LINE_LENGTH    ((MEN_ITEM_LEN + 3) * 4 + 10)
#define MAX_MEN_ITEM_ANZ   22             /* maximale Item-Anzahl pro Menue   */
#define MAX_MEN_SCHACHTEL   4             /* maximale Menueschachtelungstiefe */
#if defined(BS_SOLARIS)
  #define MEN_ITEM_ADD      1             /* fuer NULL-Terminator             */
#endif
#ifdef BS_LINUX
  #define MEN_ITEM_ADD      3             /* f. NULL-Terminator und 2 Umlaute */
#endif

/* ----------------------- Sonderzeichen von "dialog": ---------------------- */
#ifdef BS_SOLARIS
#define BACKSP              8
#define HOME               71
#define END_CH             79
#define LEFT               68
#define RIGHT              67
#define DEL_CH            127
#endif /* BS_SOLARIS */
#ifdef BS_LINUX
#define BACKSP            127
#define HOME               72
#define END_CH             70
#define LEFT               68
#define RIGHT              67
#define DEL_CH              3
#endif /* BS_LINUX */



/* ========================= Lokale Typdeklarationen: ======================= */
typedef struct  MEN_ITEM_TYP
  { 
    char      item_txt [MEN_ITEM_LEN + MEN_ITEM_ADD];
    INT       item_col;
    BOOL      item_valid;
    char      item_char;
  } MEN_ITEM_TYP;



/* ============================= Lokale Variable: =========================== */
static char         *men_titles [MAX_MEN_SCHACHTEL];  /* akt. Menuehierarchie */
static MEN_ITEM_TYP *menue_items;          /* Daten und Texte der Menue-Items */
static INT           max_men_item_index,   /* ermittelt "menue_completion"    */
                     akt_men_tiefe = 0;    /* Aktuelle Menueschachtelungstiefe*/



/* ============================= Lokale Routinen: =========================== */
void exit_handler                 (void);

static BOOL lies_zeichen          (char *ch);




/* ============================== exit_handler ============================== */
void exit_handler (void)
/* -------------------------------------------------------------------------- */
/*   Bei Linux wird der normale Terminal-Modus wiederhergestellt.             */
/* -------------------------------------------------------------------------- */
{
#ifdef BS_LINUX
   restore_tty_params ();  /* setzt die Terminal-Eigenschaften wieder zurueck */
#endif /* BS_LINUX */
} /* exit_handler */ 



/* =============================== char_dialog ============================== */
char char_dialog (const char *out_text)
{
   STRING_TYPE  buf1;
   INT          len;
   char         ch;
   
   /* char_dialog */
   len = strlen (out_text);
   if (len > (MAX_STRING - 2)) 
   { /* Zeile kuerzen */
      len = MAX_STRING - 2;                                 
      strncpy (buf1, out_text, len);
      buf1 [len-1] = ':';    
      buf1 [len] = '\0';       /* gekuerzte Zeile mit Nullzeichen terminieren */
   } /* Zeile kuerzen */
   else
      strcpy (buf1, out_text);
   printf ("%s", buf1);
   fflush (stdout);
   ch = get_kb_char ();
   if (ch >= 32)
      printf ("%c\n", ch);                                         /* Echo    */
   else
      printf ("\n");
   ch = toupper (ch);
   return (ch);
} /* char_dialog */ 



/* ============================== lies_zeichen ============================== */
static BOOL lies_zeichen (char *ch)
/*----------------------------------------------------------------------------*/
/*  Liest ein ACII- oder Spezialzeichen ein. Spezialzeichen sind folgende     */
/*  Zeichen: HOME, END_CH, LEFT, RIGHT, DEL_CH. Zu den ASCII-Zeichen zaehlen  */
/*  auch CR, BACKSP und ESC. Wenn ein ASCII-Zeichen eingelesen wird, liefert  */
/*  die Routine TRUE, sonst FALSE. Es wird auf die Eingabe gewartet.          */
/*----------------------------------------------------------------------------*/
{
   BOOL  ascii_key = TRUE;
   
   /* lies_zeichen */
   *ch = get_kb_char ();
   
#if defined(BS_SOLARIS) || defined(BS_LINUX)
#ifdef BS_SOLARIS
   ascii_key = (int)(*ch) != DEL_CH;
#endif /* Solaris */
   if (*ch == ESC)
   { /* Spezial-Zeichen */
      if (kb_hit ())
      { /* es kommt noch mehr */
         ascii_key = FALSE;
         *ch = get_kb_char ();                               /* Ueberlese "[" */
         *ch = get_kb_char ();
         switch (*ch)
         {
            case 'H':
               *ch = HOME;    break;
            case 'D':
               *ch = LEFT;    break;
            case 'C':
               *ch = RIGHT;   break;
#ifdef BS_LINUX
            case 'F':
               *ch = END_CH;  break;
            case '3':
               *ch = get_kb_char ();                         /* Ueberlese "~" */
               *ch = DEL_CH;  break;
#else /* Solaris */
            case '2':
               *ch = END_CH;  break;
#endif /* Solaris */
         } /* CASE */
      } /* es kommt noch mehr */
   } /* Spezial-Zeichen */
#endif /* Solaris oder Linux */

   return (ascii_key);
} /* lies_zeichen */ 



/* ================================= dialog ================================= */
BOOL dialog (const char *out_text,
             char       *in_text)
{
#define  MIN_PLATZ  5
   
   STRING_TYPE  buffer,         /* Dialogpuffer                               */
                out_buf;        /* Gegebenenfalls gekuerzter Querypuffer      */
   INT       len,               /* Laenge des "out_text"s                     */
             cursor_pos    = 1; /* Cursor-Position in lfd.Nr des Zeichens     */
   INT       max_cursor_pos,    /* Maximale Anzahl einlesbarer Zeichen        */
             text_len      = 0; /* Anzahl eingelesener Zeichen                */
   char      ch;                /* aktuelles Zeichen                          */
   BOOL      norm_key,          /* TRUE, wenn "ch" ein ASCII-Zeichen ist      */
             zeile_neu = FALSE; /* TRUE, wenn Update der Eingabe erforderlich */
   
   /* dialog */
   len = strlen (out_text);
   if (len > (MAX_STRING - MIN_PLATZ))   /*  */
   { /* Zeile kuerzen */
      len = MAX_STRING - MIN_PLATZ;                                 
      strncpy (out_buf, out_text, len);
      out_buf [len-1] = ':';    
      out_buf [len] = '\0';    /* gekuerzte Zeile mit Nullzeichen terminieren */
   } /* Zeile kuerzen */
   else
      strcpy (out_buf, out_text);
   printf ("%s", out_buf);                       /* Ausgabe des Query-Strings */
   fflush (stdout);  
   max_cursor_pos = MAX_STRING - len;
   strcpy (in_text, "");
      
   do
   { /* Einlese-Schleife */
      norm_key = lies_zeichen (&ch);
      if (ch == ESC)
	 break;                                   /* abandon loop immediately */
      if (norm_key)
      { /* ASCII-Zeichen */
	 if (ch != CR)
         { /* Kein CR */
	    if (ch == BACKSP)
            { /* Backspace */
	       if (cursor_pos > 1)
               { /* Es geht noch zurueck */
		  zeile_neu = TRUE;
		  text_len--;
		  cursor_pos--;
		  strncpy (buffer, in_text, cursor_pos-1);
		  buffer[cursor_pos-1] = '\0';
		  strcat (buffer, &(in_text[cursor_pos]));
		  strcpy (in_text, buffer);
	       } /* Es geht noch zurueck */
	    } /* Backspace */
	    else
            { /*  Zeichen einlesen */
	       strncpy (buffer, in_text, cursor_pos-1);
	       buffer[cursor_pos-1] = ch;
	       buffer[cursor_pos]   = '\0';
	       strcat (buffer, &(in_text[cursor_pos-1]));
	       strcpy (in_text, buffer);
	       if ((cursor_pos - 1) == text_len)
               { /* Akt. Pos. am Zeilenende: delete cursor and echo char */
		  printf ("%c", ch);
		  fflush (stdout);
	       } /* Akt. Pos. am Zeilenende: delete cursor and echo char */
	       else
		  zeile_neu = TRUE;
	       cursor_pos++;
	       text_len++;
	    } /*  Zeichen einlesen */
	 } /* Kein CR */
      } /* ASCII-Zeichen */
      else
      { /* Sonderzeichen */
	 zeile_neu = TRUE;
	 switch (ch)
         {
            case HOME:
               cursor_pos = 1;
               break;
                  
            case END_CH:
               cursor_pos = text_len + 1;
               break;
                  
            case LEFT:
               if (cursor_pos > 1)
                  cursor_pos--;
               break;
                  
            case RIGHT:
               if (cursor_pos <= text_len)
                  cursor_pos++;
               break;
                  
            case DEL_CH:
               strncpy (buffer, in_text, cursor_pos-1);
               buffer[cursor_pos-1] = '\0';
               strcat (buffer, &(in_text[cursor_pos]));
               strcpy (in_text, buffer);
               zeile_neu = TRUE;
               break;
	 } /* CASE */
      } /* Sonderzeichen */
         
      if (zeile_neu)
      { /* Update Echo */
	 zeile_neu = FALSE;
	 strncpy (buffer, in_text, cursor_pos-1);
	 buffer[cursor_pos-1] = '\0';
	 printf ("\r%s%s \r%s%s", out_buf, in_text, out_buf, buffer);
	 fflush (stdout);
      } /* Update Echo */
   } /* Einlese-Schleife */
   while ((ch != CR) && (cursor_pos <= max_cursor_pos));
      
   printf ("\n");
   return (ch == ESC);
} /* dialog */ 



/* ======================= set_and_clear_menue_window ======================= */
void set_and_clear_menue_window (INT         men_tiefe,
                                 const char *men_title)
{
   STRING_TYPE  buf2;
   STR_TYPE     buf;
   INT          ii;
   
   /* set_and_clear_menue_window */
   if ((men_tiefe < 1) || (MAX_MEN_SCHACHTEL < men_tiefe))
   { /* ungueltige Schachtelungstiefe */
      sprintf (buf, BAD_MTIEFE_TXT, men_tiefe, MAX_MEN_SCHACHTEL);
#ifdef TEXT_MODE
      printf (FMELD_LINE_TXT, buf);
#endif /* TEXT_MODE */
      sprintf (buf2, ">>>> %s", buf);
      write_to_log (buf2, CLOSE_LOG);
   } /* ungueltige Schachtelungstiefe */
   else
   { /* "men_tiefe" ok */
      akt_men_tiefe  = men_tiefe;
      strcpy (men_titles[men_tiefe - 1], men_title);
      for (ii = 0;  ii < MAX_MEN_ITEM_ANZ;  ii++)
      { /* Init Menue-Items */
         strcpy (menue_items[ii].item_txt, "");
         menue_items[ii].item_valid = FALSE;
      } /* Init Menue-Items */
   } /* "men_tiefe" ok */
} /* set_and_clear_menue_window */ 



/* ============================ write_menue_item ============================ */
void write_menue_item (INT         item_nr,
                       INT         item_color,
                       const char *item_text)
{
   STRING_TYPE  buf2;
   STR_TYPE     buf;
   
   /* write_menue_item */
   if ((item_nr <= 0) || (MAX_MEN_ITEM_ANZ <= item_nr))
   { /* Item-Nr fehlerhaft */
      sprintf (buf, BAD_MENITEM_TXT, item_nr, MAX_MEN_ITEM_ANZ - 1);
#ifdef TEXT_MODE
      printf (FMELD_LINE_TXT, buf);
#endif /* TEXT_MODE */
      sprintf (buf2, ">>>> %s", buf);
      write_to_log (buf2, CLOSE_LOG);
   } /* Item-Nr fehlerhaft */
   else
   { /* Item-Nr ok */
      strcpy (menue_items[item_nr-1].item_txt, item_text);
      menue_items[item_nr-1].item_col   = item_color;
      menue_items[item_nr-1].item_valid = item_color != DarkMenItem;
   } /* Item-Nr ok */
} /* write_menue_item */ 



/* ============================ select_menue_item =========================== */
INT select_menue_item (void)
{
   INT   item_index;
   char  ch;
   BOOL  not_sel = TRUE;
   
   /* select_menue_item */
   do
   { /* Hauptschleife der Selektion */
      ch = char_dialog (MEN_QUERY_TXT);
      if (ch == ESC)
      { /* Menue-ende */
         not_sel    = FALSE;
         item_index = ESC - 1;
      } /* Menue-ende */
      else
      { /* Kein ESC */
         if (ch <= '9')
            item_index = ch - '1';
         else
         { /* Bereich 'A', .. ,'M' */
            if ('A' <= ch)
               item_index = ch - 'A' + 9;      /* wegen 9 Items '1', .. ,'9'  */
            else
               item_index = -1;                /* ':', .. ,'@' sind ungueltig */
         } /* Bereich 'A', .. ,'M' */
         if ((item_index < 0) || (MAX_MEN_ITEM_ANZ <= item_index))
            printf (FMELD_LINE_TXT, BAD_KEY);
         else
         { /* Das Item gibts */
            if (menue_items[item_index].item_valid)
            { /* Gueltiges Item selektiert */
               not_sel = FALSE;
               if ((item_index == max_men_item_index) && (akt_men_tiefe > 1))
                  item_index = ESC - 1;                         /* Menue-Ende */
            } /* Gueltiges Item selektiert */
            else
               printf (FMELD_LINE_TXT, INVALID_ITEM_TXT);
         } /* Das Item gibts */
      } /* Kein ESC */
   } /* Hauptschleife der Selektion */
   while (not_sel);
   printf ("\n");
   return (item_index + 1);
} /* select_menue_item */ 



/* ============================ menue_completion ============================ */
void menue_completion (void)
{
   STR_TYPE  buf;
   INT       len, rest_len, halb_len, aktLen, ii;
   
   /* menue_completion */
   max_men_item_index = 0;
   for (ii = 0;  ii < MAX_MEN_ITEM_ANZ;  ii++)
   { /* Groestes gueltiges Menue-Item bestimmen */
      if (strcmp (menue_items[ii].item_txt, "") != 0)
         max_men_item_index = ii;
   } /* Groestes gueltiges Menue-Item bestimmen */
   if ((akt_men_tiefe > 1) &&
       (strcmp (menue_items[max_men_item_index].item_txt, END_ITEM_TXT) != 0))
   { /* Menue-Ende-Item erzeugen */
      max_men_item_index++;
      strcpy (menue_items[max_men_item_index].item_txt, END_ITEM_TXT);
      menue_items[max_men_item_index].item_col   = StndMenItem;
      menue_items[max_men_item_index].item_valid = TRUE;
   } /* Menue-Ende-Item erzeugen */
   len = strlen (men_titles[akt_men_tiefe - 1]);
   rest_len = MEN_LINE_LENGTH - len - 2;
   halb_len = rest_len / 2;
   strcpy (buf, "-");
   for (ii = 1;  ii < halb_len;  ii++)
      strcat (buf, "-");
   printf ("\n%s %s %s", buf, men_titles[akt_men_tiefe - 1], buf);
   if ((rest_len % 2) == 1)
      printf ("-\n");
   else
      printf ("\n");
   strcpy (buf, "");
   for (ii = 0;  ii <= max_men_item_index;  ii++)
   { /* Menue-Item oder Leerbereich ausgeben */
      if (menue_items[ii].item_valid) {
	 len = MEN_ITEM_LEN;
#ifdef BS_LINUX
	 aktLen = strlen (menue_items[ii].item_txt);
	 if (aktLen > MEN_ITEM_LEN)
	    len = aktLen;
#endif /* Linux */
         printf ("%c: %-*s   ", menue_items[ii].item_char,  /* Gueltiges Item */
                 len, menue_items[ii].item_txt);            /* ausgeben       */
      }
      else
         printf ("%-*s   ", MEN_ITEM_LEN + 3, buf);         /* Leerplatz      */
      if (((ii + 1) % 4) == 0)
         printf ("\n");
   } /* Menue-Item oder Leerbereich ausgeben */
   if (((max_men_item_index + 1) % 4) != 0)
      printf ("\n");                        /* unvollstaendige Zeile ausgeben */
   printf ("%s\n", MENUE_STRICH_TXT);
} /* menue_completion */ 



/* ================================ ctioInit ================================ */
void ctioInit (void)
{
   INT   ii;
   char  ch_ii;
   
   /* scr_gsw_init */
   atexit (exit_handler);                              /* general exithandler */
   
   /* ------------------- Intialisierung der Menue-Items: ------------------- */
   ch_ii = '1';
   menue_items = (MEN_ITEM_TYP*)malloc (MAX_MEN_ITEM_ANZ * sizeof (MEN_ITEM_TYP));
   for (ii = 0;  ii < MAX_MEN_ITEM_ANZ;  ii++)
   { /* Init Menue-Items */
      strcpy (menue_items[ii].item_txt, "");
      menue_items[ii].item_valid = FALSE;
      menue_items[ii].item_char  = ch_ii;
      if (ch_ii == '9')
         ch_ii = 'A';
      else
         ch_ii++;
   } /* Init Menue-Items */
   for (ii = 0;  ii < MAX_MEN_SCHACHTEL;  ii++)  {
      men_titles [ii] = (char*)malloc((MEN_ITEM_LEN + MEN_ITEM_ADD) * sizeof(char));
      strcpy (men_titles [ii], "");
   }
   
} /* ctioInit */ 
