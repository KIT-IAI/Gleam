/*******************************************************************************
GLEAM/AE                                                                  V1.2.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: C-Terminal-I/O
                        Module: USERinterface GrundSoftWare
 
Package: ctio                   File: user_gsw.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt Routinen der TEXT_MODE-Variante: Verschiedene Routinen zur
Werte-Eingabe, eine Kettenadress-Leseroutine sowie Routinen zur kontrollierten
Eingabe von Filespezifikationen.

Dieser Modul wurde urspruenglich fuer EGA/VGA-Karten und Borlands BGI geschrie-
ben. Fuer die vorliegende Version gelten die im Modul "scr_gsw" gemachten Anga-
ben gleichermassen. Die gesamte Benutzer-I/O der TEXT_MODE-Variante laeuft ueber 
diesen Modul und "scr_gsw". 


Die Routinen im einzelnen:

BOOL get_byte (UBYTE *b,
               char  *str);
          Der String "str" wird in eine vorzeichenloses Byte umgewandelt und in 
          "b" abgeliefert. Die Funktion ist TRUE, wenn die Umwandlung fehlerfrei
          moeglich war.

BOOL get_word (USHORT *w,
               char   *str);
          Der String "str" wird in ein USHORT umgewandelt und in "w" abgelie-
          fert. Die Funktion ist TRUE, wenn die Umwandlung fehlerfrei moeglich 
          war.

BOOL get_int (INT  *i,
              char *str);
          Der String "str" wird in eine Integer umgewandelt und in "i" abgelie-
          fert. Die Funktion ist TRUE, wenn die Umwandlung fehlerfrei erfolgte.

BOOL get_long (LONG  *li,
               char *str);
          Der String "str" wird in eine LongInteger umgewandelt und in "li" ab-
          geliefert. Die Funktion ist bei fehlerfreier Umwandlung TRUE.

BOOL get_single (SINGLE *s,
                 char   *str);
          Der String "str" wird in eine Sinle umgewandelt und in "s" abgelie-
          fert. Die Funktion ist TRUE, wenn die Umwandlung fehlerfrei erfolgte.

BOOL get_real (DOUBLE *r,
               char   *str);
          Der String "str" wird in eine Double umgewandelt und in "r" abgelie-
          fert. Die Funktion ist TRUE, wenn die Umwandlung fehlerfrei erfolgte.

BOOL lies_wert (INT        *wert,
                const char *qbuf,
                INT         ug,
                INT         og,
                INT         def);
          Mit der Routine "dialog" wird mit dem Querybuffer "qbuf" im Dialog-
          fenster solange nach einem Wert gefragt, bis dieser zwischen "ug" und 
          "og" liegt oder der Dialog mit Escape abgebrochen wird. Bei Abbruch 
          liefert die Routine TRUE sonst FALSE. Wenn "ug" <= "def" <= "og" ist, 
          wird eine leere Eingabe als Default "def" interpretiert. Das Ergebnis 
          wird in "wert" abgeliefert. "lies_wert" operiert mit Integerwerten. 
          Fuer die nachfolgend aufgelisteten Routinen "lies_lwert", "lies_swert"
          und "lies_rwert" gilt diese Beschreibung gleichermassen fuer das Ein-
          lesen von LongInteger-, Float- oder Double-Werten.

BOOL lies_lwert (LONG         *wert,
                 const char   *qbuf,
                 LONG          ug,
                 LONG          og,
                 LONG          def);

BOOL lies_swert (SINGLE       *wert,
                 const char   *qbuf,
                 SINGLE        ug,
                 SINGLE        og,
                 SINGLE        def);

BOOL lies_rwert (DOUBLE       *wert,
                 const char   *qbuf,
                 DOUBLE        ug,
                 DOUBLE        og,
                 DOUBLE        def);


BOOL lies_adr (INT        *guete,
               INT        *lfd_nr,
               const char *v_bezeichner);
           Liest die Kettenadresse bestehend aus "guete" und "lfd_nr" unter Aus-
           gabe von "bezeichner" ein. Liefert TRUE ab, wenn der "guete"-Wert 
           zulaessig ist, d.h. zwischen 1 und "max_guete" liegt und der Dialog 
           nicht abgebrochen wurde. Ansonsten FALSE.

BOOL erfrage_fspec (char       *fspec_buf,
                    const char *ext_buf);
           Erfragt die Filespezifikation eines zu ladenden Files mit der Exten-
           tion "ext_buf" und liefert sie in "fspec_buf" ab. Wenn der Dialog 
           nicht abgebrochen und ein existierendes File selektiert wird, liefert
           die Routine TRUE, sonst FALSE.
           Die Dialoge erfolgen ab Zeile 4 und koennen je nach Anzahl der Files
           den Rest des Bildschirms beanspruchen.
           In der BGI-Version erfolgt die Fileauswahl per Maus (Ausbaustufe).

BOOL erfrage_outfile_spec (BOOL       *neu_file,
                           char       *fspec,
                           const char *def_fspec,
                           const char *abbr_buf);
           Erfragt die Filespezifikation eines zu schreibenden Files, wobei die
           Extention in "fspec" und ein Defaultwert in "def_fspec" vorgegeben 
           werden koennen. Wenn das File bereits existiert und "neu_file" TRUE
           ist, wird nachgefragt, ob es ueberschrieben werden soll. Wenn der 
           Dialog nicht abgebrochen und eine gueltige Filespezifikation vorge-
           geben wird, wird die Filespezifikation in "fspec" abgeliefert und die 
           Funktion ist TRUE, sonst FALSE. "neu_file" ist TRUE, wenn das File 
           noch nicht existiert hat. "abbr_buf" enthaelt den bei Funktionsabbruch 
           auszugebenden Text. 

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Verwendung von "char *" statt STR_TYPE bei den Funktions-
    parametern.                                                      WJ 20.11.98
 2. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 3. Entfernung von BGI_MODE und des dazugehoerigen Codes. Entfernung 
    ueberfluessiger Variable.                                        WJ 26.06.12
 4. Die "lies_?wert"-Routinen pruefen nun auch die untere Grenze.    WJ 09.11.15
 5. Renovierung: Streichung von "select_scroll" und "write_scroll".
    Streichung des "znr"-Parameters bei den "lies_..."-Routinen. 
    Anpassung des "dialog"-Aufrufs. Uebername der Routinen "lies_adr",
    "erfrage_fspec" und "erfrage_outfile_spec" vom Modul "str_gsw".  WJ 10.05.16
 6. Avoiding buffer overflow of static strings in: lies_adr(),
    erfrage_fspec(), erfrage_outfile_fspec()                         WJ 17.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 17.07.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "chaindef.h"
#include "fbhm.h"
#include "ctio.h"
#include "ctioInt.h"

#include GLOBAL_TEXTS
#include CTIO_TEXTS



/* ================================ get_byte ================================ */
BOOL get_byte (UBYTE *b,
               char  *str)
{
   STR_TYPE  lbuff;
   BOOL      r_get_byte;
   SHORT     res;
   INT       erg;
   
   /* GET_BYTE/USER_GSW */
   r_get_byte = TRUE;
   res        = sscanf (str, "%d", &erg);
   if ((res == 1) && (0 <= erg) && (erg <= 255))
      *b = (UBYTE)(erg);
   else
   { /* Fehler */
      r_get_byte = FALSE;
      sprintf (lbuff, "Byte%s", WANDEL_FMELD);
      printf (FMELD_LINE_TXT, lbuff);
   } /* Fehler */
   return (r_get_byte);
} /* get_byte */ 



/* ================================ get_word ================================ */
BOOL get_word (USHORT *w,
               char   *str)
{
   STR_TYPE  lbuff;
   BOOL      r_get_word;
   INT       res;
   LONG      erg;
   
   /* GET_WORD/USER_GSW */
   r_get_word = TRUE;
   res        = sscanf (str, "%ld", &erg);
   if ((res == 1) && (0 <= erg) && (erg <= 65535L))
      *w = (USHORT)(erg);
   else
   { /* Fehler */
      r_get_word = FALSE;
      sprintf (lbuff, "Word%s", WANDEL_FMELD);
      printf (FMELD_LINE_TXT, lbuff);
   } /* Fehler */
   return (r_get_word);
} /* get_word */ 



/* ================================= get_int ================================ */
BOOL get_int (INT  *i,
              char *str)
{
   STR_TYPE  lbuff;
   BOOL      r_get_int;
   INT       res;
   
   /* GET_INT/USER_GSW */
   r_get_int = TRUE;
   res       = sscanf (str, "%d", i);
   if (res != 1)
   { /* Fehler */
      r_get_int = FALSE;
      sprintf (lbuff, "Integer%s", WANDEL_FMELD);
      printf (FMELD_LINE_TXT, lbuff);
   } /* Fehler */
   return (r_get_int);
} /* get_int */ 



/* ================================ get_long ================================ */
BOOL get_long (LONG *li,
               char *str)
{
   STR_TYPE  lbuff;
   BOOL      r_get_int;
   INT       res;
   
   /* get_long */
   r_get_int = TRUE;
   res       = sscanf (str, "%ld", li);
   if (res != 1)
   { /* Fehler */
      r_get_int = FALSE;
      sprintf (lbuff, "LongInteger%s", WANDEL_FMELD);
      printf (FMELD_LINE_TXT, lbuff);
   } /* Fehler */
   return (r_get_int);
} /* get_long */ 



/* =============================== get_single =============================== */
BOOL get_single (SINGLE *s,
                 char   *str)
{
   STR_TYPE  lbuf;
   BOOL      r_get_single;
   INT       res;
   
   r_get_single = TRUE;
   res = sscanf (str, "%f", s);
   if ((res != 1))
   { /* Fehler */
      r_get_single = FALSE;
      sprintf (lbuf, "Float%s", WANDEL_FMELD);
      printf (FMELD_LINE_TXT, lbuf);
   } /* Fehler */
   return (r_get_single);
} /* get_single */ 



/* ================================ get_real ================================ */
BOOL get_real (DOUBLE *r,
               char   *str)
{
   STR_TYPE  lbuff;
   BOOL      r_get_real;
   INT       res;
   
   /* GET_REAL/USER_GSW */
   r_get_real = TRUE;
   res        = sscanf (str, "%lf", r);
   if ((res != 1))
   { /* Fehler */
      r_get_real = FALSE;
      sprintf (lbuff, "Double%s", WANDEL_FMELD);
      printf (FMELD_LINE_TXT, lbuff);
   } /* Fehler */
   return (r_get_real);
} /* get_real */ 



/* ================================ lies_wert =============================== */
BOOL lies_wert (INT         *wert,
                const char  *qbuf,
                INT          ug,
                INT          og,
                INT          def)
{
   STR_TYPE  buf;
   BOOL      wuerg = FALSE;
   BOOL      ok    = FALSE;
   
   /* lies_wert */
   do
   { /* main loop */
      if (dialog (qbuf, buf))
         break;                              /* abandon main loop immediately */
      if (!strcmp (buf, "") && (ug <= def) && (def <= og))
      { /* default */
         *wert = def;
         ok    = TRUE;
      } /* default */
      else
      { /* Eingabe */
         while (!get_int (wert, buf))
         { /* inner loop */
            wuerg = dialog (qbuf, buf);
            if (wuerg)
               break;                       /* abandon inner loop immediately */
         } /* inner loop */
         if (wuerg)
            break;                          /* abandon main loop immediately  */
         if (*wert < ug)
         {
            sprintf (buf, "%s%1d", ZU_KLEIN_TXT, ug);
            printf (FMELD_LINE_TXT, buf);
         }
         else
            if (og < *wert)
            {
               sprintf (buf, "%s%1d", ZU_GROSS_TXT, og);
               printf (FMELD_LINE_TXT, buf);
            }
            else
               ok = TRUE;
      } /* Eingabe */
   } /* main loop */
   while (!ok);
   return (!ok);
} /* lies_wert */ 



/* =============================== lies_lwert =============================== */
BOOL lies_lwert (LONG       *wert,
                 const char *qbuf,
                 LONG        ug,
                 LONG        og,
                 LONG        def)
{
   STR_TYPE  buf;
   BOOL      wuerg = FALSE;
   BOOL      ok    = FALSE;
   
   /* lies_lwert */
   do
   { /* main loop */
      if (dialog (qbuf, buf))
         break;                              /* abandon main loop immediately */
      if (!strcmp (buf, "") && (ug <= def) && (def <= og))
      { /* default */
         *wert = def;
         ok    = TRUE;
      } /* default */
      else
      { /* Eingabe */
         while (!get_long (wert, buf))
         { /* inner loop */
            wuerg = dialog (qbuf, buf);
            if (wuerg)
               break;                       /* abandon inner loop immediately */
         } /* inner loop */
         if (wuerg)
            break;                          /* abandon main loop immediately  */
         if (*wert < ug)
         {
            sprintf (buf, "%s%1ld", ZU_KLEIN_TXT, ug);
            printf (FMELD_LINE_TXT, buf);
         }
         else
            if (og < *wert)
            {
               sprintf (buf, "%s%1ld", ZU_GROSS_TXT, og);
               printf (FMELD_LINE_TXT, buf);
            }
            else
               ok = TRUE;
      } /* Eingabe */
   } /* main loop */
   while (!ok);
   return (!ok);
} /* lies_lwert */ 



/* =============================== lies_swert =============================== */
BOOL lies_swert (SINGLE     *wert,
                 const char *qbuf,
                 SINGLE      ug,
                 SINGLE      og,
                 SINGLE      def)
{
   STR_TYPE  buf;
   BOOL      wuerg = FALSE;
   BOOL      ok    = FALSE;
   
   /* lies_swert */
   do
   { /* main loop */
      if (dialog (qbuf, buf))
         break;                              /* abandon main loop immediately */
      if (!strcmp (buf, "") && (ug <= def) && (def <= og))
      { /* default */
         *wert = def;
         ok    = TRUE;
      } /* default */
      else
      { /* Eingabe */
         while (!get_single (wert, buf))
         { /* inner loop */
            wuerg = dialog (qbuf, buf);
            if (wuerg)
               break;                       /* abandon inner loop immediately */
         } /* inner loop */
         if (wuerg)
            break;                          /* abandon main loop immediately  */
         if (*wert < ug)
         {
            sprintf (buf, "%s%g", ZU_KLEIN_TXT, ug);
            printf (FMELD_LINE_TXT, buf);
         }
         else
            if (og < *wert)
            {
               sprintf (buf, "%s%g", ZU_GROSS_TXT, og);
               printf (FMELD_LINE_TXT, buf);
            }
            else
               ok = TRUE;
      } /* Eingabe */
   } /* main loop */
   while (!ok);
   return (!ok);
} /* lies_swert */ 



/* =============================== lies_rwert =============================== */
BOOL lies_rwert (DOUBLE     *wert,
                 const char *qbuf,
                 DOUBLE      ug,
                 DOUBLE      og,
                 DOUBLE      def)
{
   STR_TYPE  buf;
   BOOL      wuerg = FALSE;
   BOOL      ok    = FALSE;
   
   /* lies_rwert */
   do
   { /* main loop */
      if (dialog (qbuf, buf))
         break;                              /* abandon main loop immediately */
      if (!strcmp (buf, "") && (ug <= def) && (def <= og))
      { /* default */
         *wert = def;
         ok    = TRUE;
      } /* default */
      else
      { /* Eingabe */
         while (!get_real (wert, buf))
         { /* inner loop */
            wuerg = dialog (qbuf, buf);
            if (wuerg)
               break;                       /* abandon inner loop immediately */
         } /* inner loop */
         if (wuerg)
            break;                          /* abandon main loop immediately  */
         if (*wert < ug) {
            sprintf (buf, "%s%lg", ZU_KLEIN_TXT, ug);
            printf (FMELD_LINE_TXT, buf);
         }
         else
            if (og < *wert) {
               sprintf (buf, "%s%lg", ZU_GROSS_TXT, og);
               printf (FMELD_LINE_TXT, buf);
            }
            else
               ok = TRUE;
      } /* Eingabe */
   } /* main loop */
   while (!ok);
   return (!ok);
} /* lies_rwert */ 



/* ================================ lies_adr ================================ */
BOOL lies_adr (INT        *guete,
               INT        *lfd_nr,
               const char *v_bezeichner)
{
   BOOL         r_lies_adr;
   STRING_TYPE  buf1;
   STR_TYPE     bezeichner;
   
   /* LIES_ADR/CH_EDI */
   strcpy (bezeichner, v_bezeichner);
   strcat (bezeichner, CH_ADR_QUERY);
   if (lies_wert (guete, bezeichner, 1, MAX_GUETE, MAX_GUETE + 1))
      r_lies_adr = FALSE;                              /* Abbruch bei "guete" */
   else
   { /* "lfd_nr" einlesen */
      sprintf (buf1, "%s%1u/ ", bezeichner, *guete);
      r_lies_adr = !lies_wert (lfd_nr, buf1, 0, INT_MAX - 1, INT_MAX);
   } /* "lfd_nr" einlesen */
   return (r_lies_adr);
} /* lies_adr */ 



/* ============================= erfrage_fspec ============================== */
BOOL erfrage_fspec (char       *fspec_buf,
                    const char *ext_buf)
{
   char         buf2[2*MAX_STRING];
   STRING_TYPE  buf1;
   STR_TYPE     dir_buf;
   INT          f_anz, selektion;
   BOOL         akt_dir,
                erg = FALSE;

   /* erfrage_fspec */
   if (dialog (FDIR_QUERY, dir_buf))
      printf ("\n%s\n", FKT_TERM_TXT);
   else
   { /* Directory ok */
      reset_text_buf ();
      if (akt_dir = !strcmp (dir_buf, ""))
         strcpy (buf1, F_AKT_DIR_TXT);
      else
         sprintf (buf1, F_DIR_TXT, dir_buf);
      f_anz = show_files (dir_buf, ext_buf, TRUE);   /* TRUE: Mit Nr.-Anzeige */
      if (f_anz >= 0)
      { /* Directory existiert */
         if (f_anz == 0)
         { /* Keine Files vorhanden */
            sprintf (buf2, F_KEINE_TXT, ext_buf, buf1);
            printf ("\n%s\n", buf2);
         } /* Keine Files vorhanden */
         else
         { /* Files vorhanden */
            write_text_buf (KEEP);
            sprintf (buf2, FSELECT_QUERY, f_anz);
            if (lies_wert (&selektion, buf2,  1, f_anz, f_anz+1))
               printf (MSG_LINE_TXT, FKT_TERM_TXT);
            else
            { /* File selektiert */
               get_buf (selektion+1, buf1);
               if (akt_dir)
                  strcpy (fspec_buf, &(buf1[6]));   /* im aktuellen Directory */
               else
               { /* Mit Dir-Vorgabe */
                  strcpy (fspec_buf, dir_buf);
                  strcat (fspec_buf, DIR_DELIMITER);
                  strcat (fspec_buf, &(buf1[6]));
               } /* Mit Dir-Vorgabe */
               erg = TRUE;
            } /* File selektiert */
         } /* Files vorhanden */
      } /* Directory existiert */
      reset_text_buf ();
   } /* Directory ok */
   return (erg);
} /* erfrage_fspec */



/* ========================== erfrage_outfile_spec ========================== */
BOOL erfrage_outfile_spec (BOOL       *neu_file,
                           char       *fspec, 
                           const char *def_fspec,
                           const char *abbr_buf)
{
   STRING_TYPE  buffer;
   STR_TYPE     ext_buf;
   FILE        *test_file;
   BOOL         mit_def, 
                erg = FALSE;

   /* erfrage_outfile_spec */
   strcpy (ext_buf, fspec);
   strcpy (fspec, "");
   mit_def = strlen (def_fspec)> 0;
   if (mit_def)
      sprintf (buffer, SF_F_DEF_QUERY, ext_buf, def_fspec);
   else
      sprintf (buffer, SF_FSPEC_QUERY, ext_buf);
   if (dialog (buffer, fspec))
      printf ("%s\n", abbr_buf);
   else
   { /* Eingabe analysieren */
      if (!strcmp (fspec, "") && mit_def)
         strcpy (fspec, def_fspec);                    /* Default soll gelten */
      if (strcmp (fspec, ""))
      { /* String erhalten */
         strcat (fspec, ".");
         strcat (fspec, ext_buf);
         if (file_exists (fspec))
         { /* File existiert */
            if (*neu_file)
               erg = char_dialog (SF_EXIST_QUERY) == JA;
            else
               erg = TRUE;
            if (!erg)
               printf ("%s\n", abbr_buf);
            *neu_file = FALSE;                      /* File existiert bereits */
         } /* File existiert */
         else
         { /* File gibts nicht */
            *neu_file = TRUE;                       /* File ist neu           */
            if ((test_file = fopen (fspec, "w")) == NULL)
            { /* no access */
               sprintf (buffer, SF_BAD_SPEC, fspec);
               printf ("%s\n", buffer);
            } /* no access */
            else
            { /* File-Spec ok */
               erg = TRUE;
               fclose (test_file);
            } /* File-Spec ok */
         } /* File gibts nicht */
      } /* String erhalten */
      else
         printf ("%s\n", abbr_buf);
   } /* Eingabe analysieren */
   return (erg);
} /* erfrage_outfile_spec */
