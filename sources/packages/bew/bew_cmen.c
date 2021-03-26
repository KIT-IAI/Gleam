/*******************************************************************************
GLEAM/AE                                                                  V3.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: BEWertung
                        Module: BEWertung-C-subMENue
 
Package: bew                    File: bew_cmen.c                  
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt Routinen, die nur fuer die TEXT_MODE-Variante benoetigt werden. 
Sie realisieren das Bewertungs-Submenue und unterstuetzen die Einbindung des Gnu-
Plot-Programms.


Die Routinen im einzelnen:

BOOL bew_men (void);
          Realisiert das Submenue zur Bewertung und Bewertungsveraenderung. 
          Liefert TRUE, wenn eine Funktion im Submenue benutzt wurde, sonst 
          FALSE.

void kill_all_plots (void);
          Terminiert alle aktiven GnuPlot-Fenster.

void c_gnu_hard_copy (void)
          Erzeugt ein Postscript-Ausgabe des letzten Plots. Es wird vorausge-
          setzt, dass zuvor etwas geplottet wurde! 

BOOL c_prep_gnu_plot (INT   fkt_nr,
                      INT   krit_nr,
                      BOOL  simRes);
          Bereitet die Ausgabe einer Bewertungs- oder Straffunktion fuer das Be-
          wertungs- (mit und ohne GnuPlot) und das Simulator-Submenue (nur mit 
          GnuPlot) vor. "fkt_nr" bezeichnet die Nummer der Funktion, waehrend 
          "krit_nr" die Nummer des Kriteriums angibt. Im Falle von Bewertungs-
          funktionen sind beide gleich. 
          Dazu werden zunaechst die Dialoge fuer die Anzahl der X-Werte, die 
          Schrittweite und den Startwert sowie ggf fuer die GnuFenster-Nummer 
          durchgefuehrt. Danach wird ggf. versucht, das Gnu-Fenster zu oeffnen. 
          Positivenfalls wird die Ausgabe mit Hilfe von "prep_gnu_plot" vorbe-
          reitet. Andernfalls erfolgt eine Fehlermeldung. Die Routine liefert 
          TRUE, wenn alles gut ging und kein Dialogabbruch erfolgte.

--------------------------------------------------------------------------------
basiert auf C-Unit: aufgabe
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
1. Anpassung an Erweiterung der "kriterien"-Daten.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 2.0.0:
 1. Probleme bei Erfuellungswert und rein exponentieller Bewertung beseitigt.
 2. "max_note" wird bei Deaktivierung eines Kriteriums auf Null gesetzt.
 3. Ueberarbeitung der Menuesteuerung.                               WJ 22.12.98
 4. Einige Ausgaben werden wegen kleinem DOS-Fenster gescrollt.      WJ 08.08.00
 5. Korrektur am Aenderungsdialog fuer Kriterien. GnuPlots werden    WJ 19.03.01
    bei Funktionsende nicht mehr automatisch gelöscht.
 6. In "c_prep_gnu_plot" Obergrenze f. x-Werte heraufgesetzt.        WJ 08.11.01
 7. Obergrenze f. x-Werte in "c_prep_gnu_plot" haengt vom BS ab.     WJ 27.06.02
 8. Linux-Portierung: Aus "round" wird "l_round" und "i_round".
    BS-Schalter: Differenzierung zwischen Solaris und Linux.         WJ 11.05.05
 9. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
10. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
11. Entfernung der Schalter und des dazugehörigen Codes:
    BS_DOS, BGI_MODE. Kleine Korrektur in "change_fkt". Entfernung 
    ueberfluessiger Variable.                                        WJ 26.06.12
12. Fehler bei der Anzeige linearer Straffunktionen in "change_fkt"  WJ 11.06.15
    beseitigt.
13. Renovierung: Aus EIN_DRITTEL wird NOTEN_DRITTEL. Korrektur der 
    sprintf- u. der write-Aufrufe. Stringverarbeitung von "read_new_val"
     an Umlaute angepasst. Streichung des kleinen Plotfensters und 
    Vereinheitlichung beim GnuPlot-Aufruf. Streichung von "kurz_name", 
    des "link"-Includes und des Aurufs von "clr_line". Anpassungen 
    bei "c_prep_gnu_plot". Anpassung folgender Aufrufe: "lies_...", 
    "write_text_buf", "zeige_krit_werte", "dialog", "char_dialog", 
    "set_and_clear_menue_window" und "select_menue_item".            WJ 28.04.16
14. Zur Einbindung des gnuplots in die GUI muss "gnu_win_pos" expor-
    tierbar werden. Daher wird es nach "bew_gsw" verschoben.         WJ 21.07.16
15. Aufrufanpassung von "zeige_kriterien" und "zeige_krit_werte",
    Verbesserungen in "bew_men" und "krit_update".                   WJ 18.08.16
16. "change_fkt": Anpassung an ueberarbeitete Bewertung und Ersetzung 
    von 0.0 durch DBL_EPSILON bei MIXED_*-Funktionen fuer xs2 bzw. 
    xs1.                                                             WJ 24.08.16
17. Neuer Parameter fuer "show_and_clear_fbuf".                      WJ 10.11.17
18. Avoiding buffer overflow of static strings: update_krit_name(),
    krit_update(), read_new_val(), c_prep_gnu_plot().                WJ 18.07.20
19. update_krit_name(): Correction of the dialog for a new scale 
    unit.                                                            WJ 16.09.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 16.09.2020
================================================================================ */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "bew.h"
#include "bewInt.h"

#if defined(BS_SOLARIS) || defined(BS_LINUX)
  #include <unistd.h>
#endif /* Solaris oder Linux */

#include GLOBAL_TEXTS
#include BEW_TEXTS



/* ============================ lokale Konstante: =========================== */
#define STRENG          TRUE
#define SCHWACH         FALSE
#define STEIGEND        TRUE
#define FALLEND         FALSE

#define MAX_X_WERTE     5000



/* ============================ Lokale Variable: ============================ */
static BOOL    neu_krit;               /* TRUE bei Krit-Wechsel od.-Aenderung */
static INT     last_gnu_krit = NIX_IDX;/* Index des zuletzt bearb. Kriteriums */
static DOUBLE  last_delta_og;          /* Zuletzt benutzte x-Delta-Obergrenze */
static DOUBLE  last_xs_delta;          /* Zuletzt benutztes Delta f. x-Start  */
static DOUBLE  ug, og;                 /* Fuer Lesen mit "read_new_val"       */
#ifdef GNUPLOT
  static INT   last_x_anz    = 201;    /* Letzte Anzahl angezeigter x-Werte   */
#else
  static INT   last_x_anz    =  21;    /* Letzte Anzahl angezeigter x-Werte   */
#endif /* GNUPLOT */



/* ============================ lokale Routinen: ============================ */
static BOOL krit_update        (INT             index);

static BOOL update_krit_name   (INT             kk);

static BOOL read_new_val       (DOUBLE         *neu_wert,
                                const STR_TYPE  query_txt,
                                BOOL            mode_streng,
                                BOOL            mode_steigend);

static void change_fkt         (INT             kk,
                                INT             krit_index);




/* =============================== krit_update ============================== */
static BOOL krit_update (INT  index)
/*----------------------------------------------------------------------------*/
/*  Die Routine fuehrt den Benutzerdialog zur Festlegung neuer Noten- und     */
/*  Schwellwerte des Kriteriums mit dem Index "index" durch.                  */
/*  Liefert bei Dialogabbruch FALSE, sonst TRUE.                              */
/*  Alte Werte koennen jeweils durch <CR> uebernommen werden. Eine neuer      */
/*  "erfuellungs_wert" muss innerhalb der Grenzen fuer x-Werte liegen. Die    */
/*  Eingabe einer Null bei "klasse" ergibt auch fuer die "klasse" der zuge-   */
/*  hoerigen Straffunktion den Wert 0 (Deaktivierung).                        */
/*  Am Ende wird geprueft, ob die Summe der "max_note"-Werte von "INOTEN_     */
/*  MAX" abweicht. Wenn ja, wird gewarnt. Schliesslich werden "akt_anz_krit"  */
/*  und "krit_og" neu bestimmt.                                               */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE  buf1;
   DOUBLE       neu_erfuellungs_wert;
   LONG          neu_max_note, noten_summe;
   INT           str_index, ij,
                 neu_klasse, neu_str_klasse;
   BOOL          neu_krit;                        /* neu angelegtes Kriterium */
   char          ch;
   
   /* krit_update */
   neu_krit    = index == akt_anz_used_krit;
   str_index   = index + max_anz_bew_krit;
   noten_summe = addiere_noten ();
   sprintf (buf1, "%s%2u]: ", NKRIT_TXT_03, kriterien[index].klasse);
   if (lies_wert (&neu_klasse, buf1, 0, max_anz_bew_krit, kriterien[index].klasse))
      return (FALSE);                                  /* Abbruch der Eingabe */
   if  (neu_klasse == 0)
   { /* Kriterium abschalten */
      kriterien[index].klasse     = KRITERIUM_AUS;
      kriterien[str_index].klasse = KRITERIUM_AUS;
      kriterien[index].max_note   = 0.0;
   } /* Kriterium abschalten */
   else
   { /* Dieses Kriterium soll gelten */
      sprintf (buf1, NKRIT_TXT_02, (INOTEN_MAX - noten_summe));
      printf ("%s\n", buf1);
      sprintf (buf1, NKRIT_TXT_04, INOTEN_MAX, 
           l_round (kriterien[index].max_note));
      if (lies_lwert (&neu_max_note, buf1, 0, INOTEN_MAX, 
		      l_round (kriterien[index].max_note)))
         return (FALSE);                               /* Abbruch der Eingabe */
      if (neu_krit || (kriterien[index].bew_alg == NIX_IDX))
      { /* Bei Neudefinition und fehlender BewFkt noch nicht angebbar */
         neu_erfuellungs_wert = 0;
	 printf ("%s\n", NKRIT_TXT_05A);
      } /* Bei Neudefinition und fehlender BewFkt noch nicht angebbar */
      else
      { /* Update */
         sprintf (buf1, "%s%8G]: ", NKRIT_TXT_05, kriterien[index].erfuellungs_wert);
         if (lies_rwert (&neu_erfuellungs_wert, buf1, 
             kriterien[index].xstart, kriterien[index].xend,
             kriterien[index].erfuellungs_wert))
            return (FALSE);                            /* Abbruch der Eingabe */
      } /* Update */
      sprintf (buf1, "%s%2u]: ", NKRIT_TXT_06, kriterien[str_index].klasse);
      if (lies_wert (&neu_str_klasse, buf1, 0, 
                     max_anz_bew_krit, kriterien[str_index].klasse))
         return (FALSE);                               /* Abbruch der Eingabe */
      strcpy (buf1, NKRIT_TXT_08);
      if (kriterien[index].nmax_cont)
         strcat (buf1, JA_NEIN_TXT);
      else
         strcat (buf1, NEIN_JA_TXT);
      ch = char_dialog (buf1);
      if (ch != ESC)
      { /* Kein Abbruch */
         if (ch == JA)
            kriterien[index].nmax_cont = TRUE;
         else if (ch == NEIN)
            kriterien[index].nmax_cont = FALSE;
         else if (ch != CR)
            printf (FMELD_LINE_TXT, BAD_KEY);
      } /* Kein Abbruch */
      else
         return (FALSE);                               /* Abbruch der Eingabe */

      /* --------------- Kein Abbruch, also Daten uebernehmen: -------------- */
      kriterien[index].klasse           = neu_klasse;
      kriterien[index].max_note         = neu_max_note;
      kriterien[index].erfuellungs_wert = neu_erfuellungs_wert;
      if (kriterien[index].bew_alg != NIX_IDX)     /* calculate weighted ful- */
	 kriterien[index].erfuellungs_note =       /* filment fitness only if */
	    calc_ungew_note (index, neu_erfuellungs_wert) *  /* fitness func- */
	    (kriterien[index].max_note / RNOTEN_MAX);      /* tion is defined */
      if (neu_str_klasse == 0)
      { /* Straffunktion abschalten */
         kriterien[str_index].klasse           = KRITERIUM_AUS;
         kriterien[str_index].erfuellungs_note = 0.0;
      } /* Straffunktion abschalten */
      else
         kriterien[str_index].klasse = neu_str_klasse;
   } /* Dieses Kriterium soll gelten */

   /* ---------------------- Obergrenzen neu berechnen: --------------------- */
   akt_anz_krit = 0;
   for (ij = 0;  ij < max_anz_bew_krit;  ij++)
      if (kriterien[ij].klasse > 0)
         akt_anz_krit = ij;
   akt_anz_krit++;                                        /* Index --> Anzahl */
   krit_og     = max_anz_bew_krit + akt_anz_krit;
   noten_summe = addiere_noten ();
   sprintf (buf1, NKRIT_TXT_02, (INOTEN_MAX - noten_summe));
   printf ("%s\n", buf1);
   if (noten_summe != INOTEN_MAX)
   { /* Warnung, da Summe aller Noten <> noten_max. */
      sprintf (buf1, NKRIT_TXT_07, noten_summe, INOTEN_MAX);
      printf (FMELD_LINE_TXT, buf1);
   } /* Warnung, da Summe aller Noten <> noten_max. */
   gewichten ();
   return (TRUE);
} /* krit_update */ 



/* ============================ update_krit_name ============================ */
static BOOL update_krit_name (INT  kk)
/*----------------------------------------------------------------------------*/
/*  Durch Dialoge werden die Bezeichner fuer das durch "kk" angegebene Kri-   */
/*  terium und dessen Straffunktion vorgegeben. Die Funktion liefert TRUE,    */
/*  wenn die Funktion nicht abgebrochen wurde.                                */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE  neu_bez;
   STR_TYPE     buf1, buf2, neu_einheit, neu_alt_einheit;
   BOOL         kein_alt_bez;
   char         ch;

   /* update_krit_name */
   strcpy (buf1, NKRIT_TXT_10);
   if (strcmp (kriterien[kk].bezeichnung, ""))
   { /* Update */
      make_krit_text (buf2, " [%s]: ", kk);
      strcat (buf1, buf2);
   } /* Update */
   else
      strcat (buf1, ": ");                                   /* Neudefinition */
   printf ("%*s%s\n", (int)strlen(buf1), NKRIT_TXT_12, bez_len_str);
   if (dialog (buf1, buf2))
      return (FALSE);                                              /* Abbruch */
   if (strlen (buf2) == 0)
      strcpy (neu_bez, kriterien[kk].bezeichnung);
   else
   { /* Neuer Bezeichner */
      if (strlen (buf2) > KRIT_BEZ_LEN)
         buf2[KRIT_BEZ_LEN] = '\0';
      sprintf (neu_bez, "%-*s:", KRIT_BEZ_LEN, buf2);
   } /* Neuer Bezeichner */

   strcpy (buf1, NKRIT_TXT_13);
   if ((strlen(kriterien[kk].einheit) > 0) && strcmp(kriterien[kk].einheit, " "))
   { /* update if string is not empty and different from a blank */
      if ((kriterien[kk].einheit[0] == '%') && 
          (strlen (kriterien[kk].einheit) == 1))
#ifdef BS_SOLARIS
         strcpy (buf2, " [%%]: ");         /* wegen dem bloeden Sun-Compiler! */
#else
         strcpy (buf2, " [%]: ");
#endif /* BS_SOLARIS */
      else
         sprintf (buf2, " [%s]: ", kriterien[kk].einheit);
      strcat (buf1, buf2);
   } /* update if string is not empty and different from a blank */
   else
      strcat (buf1, "        : ");                           /* Neudefinition */
#ifdef DEU                                     /* because of ß for Maßeinheit */
   printf ("%*s%s\n", ((int)strlen(buf1))-1, NKRIT_TXT_12, unit_len_str);
#else /* any other language */
   printf ("%*s%s\n", (int)(strlen(buf1)), NKRIT_TXT_12, unit_len_str);
#endif /* any other language */
   if (dialog (buf1, neu_einheit))
      return (FALSE);                                              /* Abbruch */
   if (strlen (neu_einheit) == 0)
      strcpy (neu_einheit, kriterien[kk].einheit);
   else
   { /* Neue Einheit */
      if (strlen (neu_einheit) > ENAME_LEN)
         neu_einheit[ENAME_LEN] = '\0';
   } /* Neue Einheit */
   
   if (strcmp (neu_einheit, "%"))
   { /* Einheit bei Bereichsueber/unterschreitung */
      if (kein_alt_bez = (kriterien[kk+max_anz_bew_krit].einheit[0] == '!'))
         strcpy (buf1, NKRIT_TXT_15);
      else
         strcpy (buf1, NKRIT_TXT_14);
      ch = char_dialog (buf1);
      if ((ch == JA) || ((ch == CR) && !kein_alt_bez))
      { /* Mit alternativer Anzeige */
         strcpy (buf1, NKRIT_TXT_16);
         if (strcmp (kriterien[kk+max_anz_bew_krit].einheit, ""))
         { /* Update */
            sprintf (buf2, " [%s]: ", kriterien[kk+max_anz_bew_krit].einheit);
            strcat (buf1, buf2);
         } /* Update */
         else
            strcat (buf1, ": ");                             /* Neudefinition */
#ifdef DEU                                     /* because of ß for Maßeinheit */
         printf ("%*s%s\n", ((int)strlen(buf1))-1, NKRIT_TXT_12, unit_len_str);
#else /* any other language */
	 printf ("%*s%s\n", (int)(strlen(buf1)), NKRIT_TXT_12, unit_len_str);
#endif /* any other language */
         if (dialog (buf1, neu_alt_einheit))
            return (FALSE);                                        /* Abbruch */
         if (strlen (neu_alt_einheit) == 0)
            strcpy (neu_alt_einheit, kriterien[kk+max_anz_bew_krit].einheit);
         else
         { /* Neue 2.Einheit */
           if (strlen (neu_alt_einheit) > ENAME_LEN)
           neu_alt_einheit[ENAME_LEN] = '\0';
         } /* Neue 2.Einheit */
      } /* Mit alternativer Anzeige */
      else
         if (ch == ESC)
            return (FALSE);                                        /* Abbruch */
         else
            strcpy (neu_alt_einheit, "!");
   } /* Einheit bei Bereichsueber/unterschreitung */
   else
      strcpy (neu_alt_einheit, "!");
   
   strcpy (kriterien[kk].bezeichnung, neu_bez);
   strcpy (kriterien[kk].einheit,     neu_einheit);
   strcpy (kriterien[kk+max_anz_bew_krit].bezeichnung, neu_bez);
   strcpy (kriterien[kk+max_anz_bew_krit].einheit, neu_alt_einheit);
   return (TRUE);
} /* update_krit_name */



/* ============================== read_new_val ============================== */
static BOOL read_new_val (DOUBLE         *neu_wert,
                          const STR_TYPE  query_txt,
                          BOOL            mode_streng,
                          BOOL            mode_steigend)
/*----------------------------------------------------------------------------*/
/*  Liest mit Hilfe von "query_txt" einen neuen DOUBLE-Wert ein. Die Routine  */
/*  liefert bei Dialogabbruch TRUE und einen undefinierten Wert, sonst FALSE. */
/*  Wenn der alte in "neu_wert" enthaltenen Wert je nach "mode_steigend" und  */
/*  zu den globalen Variablen "ug" bzw. "og" passt, wird mit Default einge-   */
/*  lesen, sonst ohne. Je nach den Werten der "mode_*"-Variablen werden ab-   */
/*  schliessend neue "ug"- und "og"-Werte berechnet.                          */
/*  Mit Hilfe dieser Routine koennen (streng) steigende oder fallende Werte-  */
/*  sequenzen eingelesen werden, wobei die alten Werte nur dann als Default   */
/*  angeboten werden, wenn sie innerhalb der Grenzen liegen.                  */
/*----------------------------------------------------------------------------*/
{
   STRING_TYPE  buffer;
   STR_TYPE     buf3;
   BUF_TYPE     valBuf;
   DOUBLE       def_wert;
   BOOL         mit_def;
   
   /* read_new_val */
   if (mode_steigend)
   { /* Werte in steigender Folge */
      if (mode_streng)
         mit_def = ug <  *neu_wert;
      else
         mit_def = ug <= *neu_wert;
   } /* Werte in steigender Folge */
   else
   { /* Werte in fallender Folge */
      if (mode_streng)
         mit_def = *neu_wert <  og;
      else
         mit_def = *neu_wert <= og;
   } /* Werte in fallender Folge */
   if (mit_def)
   { /* mit Default */
      def_wert = *neu_wert;
      zeige_x_wert (valBuf, def_wert);
      sprintf (buf3, "[%s]: ", valBuf);
   } /* mit Default */
   else
   { /* ohne Default */
      def_wert = DBL_MAX;
      strcpy (buf3, "             : ");
   } /* ohne Default */
   sprintf (buffer, "%s %s", query_txt, buf3);
   if (lies_rwert (neu_wert, buffer, ug, og, def_wert))
      return (TRUE);
   
   if (mode_steigend)
   { /* Werte in steigender Folge */
      if (mode_streng)
         ug = *neu_wert + EPSILON;
      else
         ug = *neu_wert;
   } /* Werte in steigender Folge */
   else
   { /* Werte in fallender Folge */
      if (mode_streng)
         og = *neu_wert - EPSILON;
      else
         og = *neu_wert;
   } /* Werte in fallender Folge */
   return (FALSE);
} /* read_new_val */ 



/* =============================== change_fkt =============================== */
static void change_fkt (INT  kk,
                        INT  krit_index)
/*----------------------------------------------------------------------------*/
/*  Aenderung des Typs, der Grenzen und Stuetzpunkte der Bewertungs- oder     */
/*  Straffunktion mit dem Index "kk". "krit_index" gibt den Index des zur     */
/*  Funktion gehoerigen Kriteriums an.                                        */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  buf1, buf2;
   double    rwert1, rwert2;
   INT       neu_bew_alg;
   BOOL      straf_fkt;
   
   /* change_fkt */
   straf_fkt   = kk > krit_index;
   neu_bew_alg = kriterien[kk].bew_alg;
   if (kk >=  max_anz_bew_krit)
      make_krit_text (buf1, CHG_FKT_TXT1, krit_index);
   else
      make_krit_text (buf1, CHG_FKT_TXT2, krit_index);
   printf ("\n%s\n\n", buf1);
   printf ("%s\n", CHG_FKT_TXT3);
   printf ("%s\n", CHG_FKT_TXT4);
   if (kriterien[kk].bew_alg == NIX_IDX)
      sprintf (buf1, "%s: ", CHG_FKT_TXT5);
   else
      sprintf (buf1, "%s [%1d]: ", CHG_FKT_TXT5, neu_bew_alg+1);
   if (lies_wert (&neu_bew_alg, buf1, 1, 6, neu_bew_alg+1))
      return;                                 /* abandon function immediately */
   neu_bew_alg--;
   kriterien[kk].bew_alg = neu_bew_alg;
   fkt_to_text (buf1, ZEIG_FKT_ABST, kk);
   printf ("%s\n", buf1);
   ug = -DBL_MAX;
   og =  DBL_MAX / 2.0;
   if ((neu_bew_alg == EXP_MINCR) || (neu_bew_alg == EXP_MDECR))
   { /* nur exponentiel */
      if (neu_bew_alg == EXP_MINCR)
      { /* EXP_MINCR */
         rwert1 = kriterien[kk].xend;
         if (read_new_val (&rwert1, XMAX_TXT, STRENG, FALLEND))
            goto fkt_wuerg;                  /* abandon function via end code */
         kriterien[kk].xend = rwert1;
         if (kriterien[kk].xend == 0.0)
            kriterien[kk].xend = DBL_EPSILON;
         rwert1 = kriterien[kk].xdrittel;
         if (read_new_val (&rwert1, XD_TXT, STRENG, FALLEND))
            goto fkt_wuerg;                  /* abandon function via end code */
         kriterien[kk].xdrittel = rwert1;
         kriterien[kk].xstart   = -DBL_MAX;
      } /* EXP_MINCR */
      else
      { /* EXP_MDECR */
         rwert1 = kriterien[kk].xstart;
         if (read_new_val (&rwert1, XMIN_TXT, STRENG, STEIGEND))
            goto fkt_wuerg;                  /* abandon function via end code */
         kriterien[kk].xstart = rwert1;
         if (kriterien[kk].xstart == 0.0)
            kriterien[kk].xstart = DBL_EPSILON;
         rwert1 = kriterien[kk].xdrittel;
         if (read_new_val (&rwert1, XD_TXT, STRENG, STEIGEND))
            goto fkt_wuerg;                  /* abandon function via end code */
         kriterien[kk].xdrittel = rwert1;
         kriterien[kk].xend     = DBL_MAX;
      } /* EXP_MDECR */
   } /* nur exponentiel */
   else
   { /* linear und gemischt linear-exponentiel */
      rwert1 = kriterien[kk].xstart;
      if (read_new_val (&rwert1, XMIN_TXT, STRENG, STEIGEND))
         goto fkt_wuerg;                     /* abandon function via end code */
      kriterien[kk].xstart = rwert1;
      rwert1 = kriterien[kk].xs1;
      if (read_new_val (&rwert1, XS1_TXT, STRENG, STEIGEND))
         goto fkt_wuerg;                     /* abandon function via end code */
      kriterien[kk].xs1 = rwert1;
      if ((neu_bew_alg == MIXED_MINCR) || (neu_bew_alg == MIXED_MDECR))
      {
         rwert1 = kriterien[kk].xdrittel;
         if (read_new_val (&rwert1, XD_TXT, STRENG, STEIGEND))
            goto fkt_wuerg;                  /* abandon function via end code */
         kriterien[kk].xdrittel = rwert1;
      }
      rwert1 = kriterien[kk].xs2;
      if (read_new_val (&rwert1, XS2_TXT, STRENG, STEIGEND))
         goto fkt_wuerg;                     /* abandon function via end code */
      kriterien[kk].xs2 = rwert1;
      rwert1 = kriterien[kk].xend;
      if (read_new_val (&rwert1, XMAX_TXT, STRENG, STEIGEND))
         goto fkt_wuerg;                     /* abandon function via end code */
      kriterien[kk].xend = rwert1;
      ug = 0.0;
      printf ("\n");
      if (straf_fkt)
      { /* Straffunktion */
         printf ("%s\n", CHG_FKT_TXT7);
         strcpy (buf1, STR_S1_TXT);
         strcpy (buf2, STR_S2_TXT);
	 rwert1 = kriterien[kk].ns1 / RNOTEN_MAX;
	 rwert2 = kriterien[kk].ns2 / RNOTEN_MAX;
         og     = 1.0;
      } /* Straffunktion */
      else
      { /* Bewertungsfunktion */
         sprintf (buf1, CHG_FKT_TXT6, INOTEN_MAX);
         printf ("%s\n", buf1);
         strcpy (buf1, NS1_TXT);
         strcpy (buf2, NS2_TXT);
	 rwert1 = kriterien[kk].ns1;
	 rwert2 = kriterien[kk].ns2;
         og     = RNOTEN_MAX;
      } /* Bewertungsfunktion */
      switch (neu_bew_alg)
      {
         case LIN_MINCR:
            if (read_new_val (&rwert1, buf1, SCHWACH, STEIGEND))
               goto fkt_wuerg;               /* abandon function via end code */
            if (read_new_val (&rwert2, buf2, SCHWACH, STEIGEND))
               goto fkt_wuerg;               /* abandon function via end code */
            if (straf_fkt)
            { /* Straffunktion */
               kriterien[kk].ns1 = rwert1 * RNOTEN_MAX;
               kriterien[kk].ns2 = rwert2 * RNOTEN_MAX;
            } /* Straffunktion */
            else
            { /* Bewertungsfunktion */
               kriterien[kk].ns1 = rwert1;
               kriterien[kk].ns2 = rwert2;
            } /* Bewertungsfunktion */
	    kriterien[kk].nstart = 0.0;
	    kriterien[kk].nend   = RNOTEN_MAX;
            break; /* LIN_MINCR */
            
         case LIN_MDECR:
            if (read_new_val (&rwert1, buf1, SCHWACH, FALLEND))
               goto fkt_wuerg;               /* abandon function via end code */
            if (read_new_val (&rwert2, buf2, SCHWACH, FALLEND))
               goto fkt_wuerg;               /* abandon function via end code */
            if (straf_fkt)
            { /* Straffunktion */
               kriterien[kk].ns1 = rwert1 * RNOTEN_MAX;
               kriterien[kk].ns2 = rwert2 * RNOTEN_MAX;
            } /* Straffunktion */
            else
            { /* Bewertungsfunktion */
               kriterien[kk].ns1 = rwert1;
               kriterien[kk].ns2 = rwert2;
            } /* Bewertungsfunktion */
	    kriterien[kk].nstart = RNOTEN_MAX;
	    kriterien[kk].nend   = 0.0;
            break; /* LIN_MDECR */
            
         case MIXED_MINCR:
	    if (kriterien[kk].xs2 == 0.0)
	       kriterien[kk].xs2 = DBL_EPSILON;
            if (straf_fkt)
               ug = (1.0 / 3.0) + EPSILON;              /* Straffunktion      */
            else
               ug = NOTEN_DRITTEL + EPSILON;            /* Bewertungsfunktion */
            if (read_new_val (&rwert2, buf2, STRENG,  STEIGEND))
               goto fkt_wuerg;               /* abandon function via end code */
            if (straf_fkt)
               kriterien[kk].ns2 = rwert2 * RNOTEN_MAX;      /* Straffunktion */
            else
               kriterien[kk].ns2 = rwert2;              /* Bewertungsfunktion */
            kriterien[kk].ns1 = NOTEN_DRITTEL/2.0; /* damit "prepare_eval" u. */
            prepare_eval ();                      /* "calc_ungew_note" laufen */
	    kriterien[kk].ns1 = calc_ungew_note (kk, kriterien[kk].xs1);
            break; /* MIXED_MINCR */

         case MIXED_MDECR:
	    if (kriterien[kk].xs1 == 0.0)
	       kriterien[kk].xs1 = DBL_EPSILON;
            if (straf_fkt)
               ug = (1.0 / 3.0) + EPSILON;              /* Straffunktion      */
            else
               ug = NOTEN_DRITTEL + EPSILON;            /* Bewertungsfunktion */
            if (read_new_val (&rwert1, buf1, STRENG,  STEIGEND))
               goto fkt_wuerg;               /* abandon function via end code */
            if (straf_fkt)
               kriterien[kk].ns1 = rwert1 * RNOTEN_MAX;      /* Straffunktion */
            else
               kriterien[kk].ns1 = rwert1;              /* Bewertungsfunktion */
            kriterien[kk].ns2 = NOTEN_DRITTEL/2.0; /* damit "prepare_eval" u. */
            prepare_eval ();                      /* "calc_ungew_note" laufen */
	    kriterien[kk].ns2 = calc_ungew_note (kk, kriterien[kk].xs2);
            break; /* MIXED_MDECR */
      } /* CASE */
   } /* linear und gemischt linear-exponentiel */
   
   fkt_wuerg:
   if (prepare_eval () == GLEAM_OK)                             /* Daten ok ? */
      /* Bewertertungsfunktion UND Erfuellungsnote wurde schon mal berechnet: */
      if ((kk == krit_index) && (kriterien[kk].erfuellungs_note != KEINE_NOTE))
         kriterien[kk].erfuellungs_note =         /* berechne Erfuellungsnote */
             calc_ungew_note (kk, kriterien[kk].erfuellungs_wert) *
             kriterien[kk].noten_gewicht;
} /* change_fkt */ 



/* ================================= bew_men ================================ */
BOOL bew_men (void)
{
   STR_TYPE  buf1, buf2;
   DOUBLE    x_wert;
   INT       kk, ii, 
             item_kind, men_item;
   BOOL      bew_aend, menue_ende, wuerg, 
             bdummy;
   ssize_t   dummy;
   
   /* bew_men */
#ifdef GNUPLOT
   item_kind = StndMenItem;
   strcpy (buf1, BEW_MENPKT7P);
   strcpy (buf2, BEW_MENPKTBP);
#else
   item_kind = DarkMenItem;
   strcpy (buf1, BEW_MENPKT7T);
   strcpy (buf2, BEW_MENPKTBT);
#endif /* GNUPLOT */
   set_and_clear_menue_window (2, BEW_MEN_TXT);
   write_menue_item ( 1, StndMenItem, BEW_MENPKT1);       /* ZeigeKrits       */
   write_menue_item ( 2, StndMenItem, BEW_MENPKT2);       /* AendKrit         */
   write_menue_item ( 3, StndMenItem, BEW_MENPKT3);       /* Neues Krit       */
   write_menue_item ( 5, StndMenItem, BEW_MENPKT5);       /* ZeigBewFkt       */
   write_menue_item ( 6, StndMenItem, BEW_MENPKT6);       /* AendBewFkt       */
   write_menue_item ( 7, StndMenItem, buf1);              /* Plot/Test BewFkt */
   write_menue_item ( 9, StndMenItem, BEW_MENPKT9);       /* ZeigStrFkt       */
   write_menue_item (10, StndMenItem, BEW_MENPKTA);       /* AendStrFkt       */
   write_menue_item (11, StndMenItem, buf2);              /* Plot/Test StrFkt */
   write_menue_item (13, item_kind,   BEW_MENPKTD);       /* Plot->File       */
   write_menue_item (14, item_kind,   BEW_MENPKTE);       /* BeendePlot       */
   menue_ende = FALSE;
   bew_aend   = FALSE;
   do
   {
      menue_completion ();
      men_item = select_menue_item ();
      switch (men_item)
      {
         case 1: /* ==================== Zeige Kriterien ==================== */
            zeige_kriterien (&bdummy);
            write_text_buf (CLEAR);
            break;  /* Zeige Kriterien */
            
         case 2: /* ==================== Aendere Krit. ====================== */
            printf ("%s\n", MENFKT2_TXT1);
            printf ("%s\n", NOTEN_HDR2);
            printf ("%s\n", NOTEN_HDR3);
            zeige_krit_werte (1);
            write_text_buf (CLEAR); 
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
            last_krit = kk;
            kk--;
	    printf ("\n"); /* Leerzeile zwischen KritListe und Aenderungsdoku */
            if (update_krit_name (kk))
               wuerg = !(bew_aend = krit_update (kk));
            else
               wuerg = TRUE;
            if (wuerg)
               printf ("\n%s\n", FKT_TERM_TXT);
            break;  /* Aendere Krit. */
            
         case 3: /* ====================== Neues Krit ======================= */
            if (akt_anz_used_krit < max_anz_bew_krit)
               kk = akt_anz_used_krit;   /* Vergroesserung d.Kriterien-Anzahl */
            else
            { /* Keine neuen Kriterien definierbar: Abbruch! */
               sprintf (buf1, MENFKT3_TXT1, MIN_ANZ_BEW_KRIT);
               printf (FMELD_LINE_TXT, buf1);
               break;
            } /* Keine neuen Kriterien definierbar: Abbruch! */
            printf ("%s\n", MENFKT3_TXT2);
            wuerg = TRUE;
            if (update_krit_name (kk))
            { /* Neues Kriterium angelegt */
               kriterien[kk].klasse                    = KRITERIUM_AUS;
               kriterien[kk].bew_alg                   = NIX_IDX; /*Bew/StrFkt*/
               kriterien[kk+max_anz_bew_krit].bew_alg  = NIX_IDX; /*noch undef*/
               kriterien[kk].erfuellungs_note          = KEINE_NOTE;  /*undef!*/
               kriterien[kk].nmax_cont                 = TRUE;
               kriterien[kk+max_anz_bew_krit].klasse   = KRITERIUM_AUS;
               kriterien[kk+max_anz_bew_krit].nmax_cont= TRUE;
               if (krit_update (kk))
               { /* Neues Kriterium angelegt */
                  akt_anz_used_krit++;
                  used_krit_og = max_anz_bew_krit + akt_anz_used_krit;
                  printf ("\n%s\n", NOTEN_HDR2);
                  printf ("%s\n", NOTEN_HDR3);
                  zeige_krit_werte (1);
                  write_text_buf (CLEAR);
                  last_krit = akt_anz_used_krit;
                  bew_aend  = TRUE;
                  wuerg     = FALSE;
               } /* Neues Kriterium angelegt */
            } /* Neues Kriterium angelegt */
            if (wuerg)
               printf ("\n%s\n", FKT_TERM_TXT);
            break;  /* Neues Krit. */

         case 5: /* ==================== Zeige BewertFkt ==================== */
            printf ("%s\n", MENFKT5_TXT0);
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
            neu_krit  = neu_krit || (last_krit != kk);
            last_krit = kk;
            zeig_fkt (kk - 1, kk - 1);
            write_text_buf (CLEAR);
            break;  /* Zeige BewertFkt */
            
         case 6: /* =================== Aend. BewertFkt ===================== */
            printf ("%s\n", MENFKT6_TXT1);
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
            neu_krit  = TRUE;
            last_krit = kk;
            kk--;
            change_fkt (kk, kk);
            bew_aend = TRUE;
            break;  /* Aend.  BewertFkt */
            
         case 7: /* ================= Teste/Plot BewertFkt ================== */
            printf ("%s\n", MENFKT7_TXT1);
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
#ifdef GNUPLOT
            if (c_prep_gnu_plot (kk, kk, SHOW_BEW_STR_FKT))
            {
               dummy = write (pipe_p2g[last_gnu_index][1], gnu_buffer, 
                              strlen (gnu_buffer));  /* returns -1, if failed */
               gnuplot_done = TRUE;
            }
#else /* kein GNUPLOT */
            if (c_prep_gnu_plot (kk, kk, SHOW_BEW_STR_FKT))
            { /* Funktion nicht abgebrochen */
               kk--;
               make_krit_text (buf1, CHG_FKT_TXT2, kk);
               printf ("%s\n\n", buf1);
               printf ("%s\n", MENFKT7_TXT2);
               for (ii = 0;  ii < x_anz;  ii++)
               {
                  x_wert = last_x_start + (last_delta * ii);
                  zeige_bew_par (ii+1, "", x_wert, kk);
               }
               write_text_buf (CLEAR);
            } /* Funktion nicht abgebrochen */
#endif /* kein GNUPLOT */
            break;  /* Teste/Plot BewertFkt */
            
         case 9: /* ==================== Zeige StrafFkt ===================== */
            printf ("%s\n", MENFKT9_TXT0);
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
            neu_krit  = neu_krit || (last_krit != kk);
            last_krit = kk;
            kk--;                                  /* Index des Kriteriums    */
            ii = kk + max_anz_bew_krit;            /* Index der Straffunktion */
            if (kriterien[ii].klasse == KRITERIUM_AUS)
            { /* Diese Straffunktion gibt es nicht */
               make_krit_text (buf1, MENFKT9_TXT1, kk);
               printf ("\n%s\n", buf1);
            } /* Diese Straffunktion gibt es nicht */
            else
            { /* Darstellung der Straffunktion */
               zeig_fkt (ii, kk);
               write_text_buf (CLEAR);
            } /* Darstellung der Straffunktion */
            break;  /* Zeige StrafFkt */
            
         case 10: /* ==================== Aend. StrafFkt ==================== */
            printf ("%s\n", MENFKT10_TXT1);
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
            neu_krit  = TRUE;
            last_krit = kk;
            kk--;                                  /* Index des Kriteriums    */
            ii = kk + max_anz_bew_krit;            /* Index der Straffunktion */
            if (kriterien[ii].klasse == KRITERIUM_AUS)
            { /* Diese Straffunktion gibt es nicht */
               make_krit_text (buf1, MENFKT9_TXT1, kk);
               printf ("\n%s\n", buf1);
            } /* Diese Straffunktion gibt es nicht */
            else
            { /* Diese Straffunktion gibt es */
               change_fkt (ii, kk);
               bew_aend = TRUE;
            } /* Diese Straffunktion gibt es */
            break;  /* Aend. StrafFkt */
            
         case 11: /* ================== Teste/Plot StrafFkt ================= */
            printf ("%s\n\n", MENFKT11_TXT1);
            if (last_krit == NIX_IDX)
               sprintf (buf1, MENFKT2_TXT2, akt_anz_used_krit);
            else
               sprintf (buf1, MENFKT2_TXT3, akt_anz_used_krit, last_krit);
            if (lies_wert (&kk, buf1, 1, akt_anz_used_krit, last_krit))
               break;                                     /* Funktionsabbruch */
            if (kriterien[kk-1+max_anz_bew_krit].klasse == KRITERIUM_AUS)
            { /* Diese Straffunktion gibt es nicht */
               make_krit_text (buf1, MENFKT9_TXT1, kk-1);
               printf ("%s\n", buf1);
            } /* Diese Straffunktion gibt es nicht */
            else
            { /* Diese Straffunktion gibt es */
#ifdef GNUPLOT
               if (c_prep_gnu_plot (kk + max_anz_bew_krit, kk, SHOW_BEW_STR_FKT))
               {
                  dummy = write (pipe_p2g[last_gnu_index][1], gnu_buffer,
                                 strlen (gnu_buffer)); /*returns -1, if failed*/
                  gnuplot_done = TRUE;
               }
#else /* GNUPLOT */
               if (c_prep_gnu_plot (kk + max_anz_bew_krit, kk, SHOW_BEW_STR_FKT))
               { /* Funktion nicht abgebrochen */
                  kk--;
                  make_krit_text (buf1, CHG_FKT_TXT1, kk);
                  printf ("%s\n\n", buf1);
                  printf ("%s\n", MENFKT11_TXT2);
                  kk += max_anz_bew_krit;          /* Index der Straffunktion */
                  for (ii = 0;  ii < x_anz;  ii++)
                  { /* StrafFaktor-Werte */
                     x_wert = last_x_start + (last_delta * ii);
                     zeige_bew_par (ii+1, "", x_wert, kk);
                  } /* StrafFaktor-Werte */
                  write_text_buf (CLEAR);
               } /* Funktion nicht abgebrochen */
#endif /* GNUPLOT */
            } /* Diese Straffunktion gibt es */
            break;  /* Teste/Plot StrafFkt */
            
#ifdef GNUPLOT
         case 13: /* ==================== GnuPlot Hardcpy =================== */
            printf ("%s\n", MENFKT13_TXT1);
            printf ("%s\n", MENFKT13_TXT2);
            if (gnuplot_done)
               c_gnu_hard_copy ();     /* Es wurde vorher ein GNUPLOT gemacht */
            else
               printf ("%s\n", MENFKT13_TXT3);
            break; /* GnuPlot Hardcpy */
            
         case 14: /* ==================== Beende GnuPlot ==================== */
            printf ("%s\n", MENFKT14_TXT1);
            if (gnuplot_done)
               kill_all_plots ();      /* Es wurde vorher ein GNUPLOT gemacht */
            else
               printf ("%s\n", MENFKT14_TXT2);
            break; /* Beende GnuPlot */
#endif /* GNUPLOT */
            
         case ESC:
            menue_ende = TRUE;
            break;
      } /* case */
      show_and_clear_fbuf(LIMITED_LOGGING);
   }
   while (!(menue_ende));
   bew_aend_sim = bew_aend_sim || bew_aend;
   return (bew_aend);
} /* bew_men */ 



#ifdef GNUPLOT
/* ============================= c_gnu_hard_copy ============================ */
void c_gnu_hard_copy (void)
{
   ssize_t  dummy;

   gnu_hard_copy ();
   dummy = write (pipe_p2g[last_gnu_index][1], gnu_buffer, strlen (gnu_buffer));
   sprintf (gnu_buffer, HCPY_MLDG, gnu_fname);
   printf ("%s\n", gnu_buffer);
} /* c_gnu_hard_copy */ 



/* ============================= kill_all_plots ============================= */
void kill_all_plots (void)
{
   INT      ii;
   ssize_t  dummy;
   
   for (ii = 0;  ii < MAX_GNU;  ii++)
      if (plot_ok[ii])
         dummy = write (pipe_p2g[ii][1], "quit\n", 5); /*returns -1, if failed*/
} /* kill_all_plots */ 
#endif /* GNUPLOT */



/* ============================= c_prep_gnu_plot ============================ */
BOOL c_prep_gnu_plot (INT   fkt_nr,
                      INT   krit_nr,
                      BOOL  simRes)
{
   STR_TYPE  geom_buf,            /* Geometrie des GnuPlot-Fensters           */
             buf1, buf2;          /* Hilfs-Puffer                             */
   BUF_TYPE  buf;                 /* Hilfs-Puffer                             */
   INT       gnu_index,           /* Index des GnuPlot-Fensters               */
             idx;                 /* Index der Funktion                       */
   BOOL      ret_val = TRUE;      /* Funktionsergebnis                        */
   
   /* c_prep_gnu_plot */
   neu_krit  = neu_krit || (last_krit != krit_nr);
   idx       = fkt_nr - 1;
   if (kriterien[idx].bew_alg == NIX_IDX)
   { /* noch undefinierter Bewertungsalgorithnmus */
      meldung (BEW_BEWERTUNG, NO_INFO, NO_INFO, "", UNDEF_BEW_STRF_FKT);
      return (FALSE);
   } /* noch undefinierter Bewertungsalgorithnmus */
   sprintf (buf1, PREP_GNU_TXT1, last_x_anz);
   if (lies_wert (&x_anz, buf1, 1, MAX_X_WERTE, last_x_anz))
      return (FALSE);                                     /* Funktionsabbruch */
   if (neu_krit || (last_gnu_krit != idx))
   { /* X-Startwerte neuberechnen */
      if (kriterien[idx].bew_alg == EXP_MINCR)
      {
         last_xs_delta = kriterien[idx].xend - kriterien[idx].xdrittel;
         last_x_start  = kriterien[idx].xdrittel - 2 * last_xs_delta;
      }
      else
      {
         if (kriterien[idx].bew_alg == EXP_MDECR)
            last_xs_delta = kriterien[idx].xdrittel - kriterien[idx].xstart;
         else
            last_xs_delta = kriterien[idx].xend - kriterien[idx].xstart;
         last_x_start = kriterien[idx].xstart;
      }
   } /* X-Startwerte neuberechnen */
   zeige_x_wert (buf, last_x_start);
   sprintf (buf2, PREP_GNU_TXT2, buf);
   if (lies_rwert (&last_x_start, buf2, 
                   last_x_start - 2*last_xs_delta,
                   last_x_start + 3*last_xs_delta,
                   last_x_start))
   { /* Funktionsabbruch */
      last_krit = NIX_IDX;
      return (FALSE);
   } /* Funktionsabbruch */
   if (neu_krit || (x_anz != last_x_anz) || (last_gnu_krit != idx))
   { /* Schrittwert neuberechnen */
      if (kriterien[idx].bew_alg == EXP_MDECR)
      {
         last_delta    = 3 * (kriterien[idx].xdrittel - kriterien[idx].xstart) /
                         (DOUBLE)(x_anz - 1);
         last_delta_og = 10 * last_delta;
      }
      else
      {
         last_delta    = (kriterien[idx].xend - last_x_start) / (DOUBLE)(x_anz-1);
         last_delta_og = 10 * last_delta;
      }
      neu_krit = FALSE;
   } /* Schrittwert neuberechnen */
   last_x_anz    = x_anz;
   zeige_x_wert (buf, last_delta);
   sprintf (buf2, PREP_GNU_TXT3, buf);
   if (lies_rwert (&last_delta, buf2, 0.0, last_delta_og, last_delta))
   { /* Funktionsabbruch */
      last_krit = NIX_IDX;
      return (FALSE);
   } /* Funktionsabbruch */
#ifdef GNUPLOT
   /* ------------- GnuPlot-Fenster erfragen und ggf oeffnen: --------------- */
   if (last_gnu_index == NIX_IDX)
      gnu_index = 0;                                            /* erstes mal */
   else
   { /* Fenster-Index erfragen */
      sprintf (buf2, PREP_GNU_TXT4, MAX_GNU, last_gnu_index+1);
      if (lies_wert (&gnu_index, buf2, 1, MAX_GNU, last_gnu_index+1))
         return (FALSE);                                  /* Funktionsabbruch */
      gnu_index--;
   } /* Fenster-Index erfragen */
   last_gnu_index = gnu_index;
   if (!plot_ok [gnu_index])
   { /* GnuPlot starten */                  /* die Geometrieangaben nuetzen   */
      sprintf (geom_buf, "%s%s", GNU_WIN_SIZE, gnu_win_pos [gnu_index]);
      init_plot (gnu_index, geom_buf);      /* beim aktuellen GnuPlot nichts! */
   } /* GnuPlot starten */
   
   if (plot_ok[gnu_index])
      prep_gnu_plot (idx, simRes);               /* GNUPLOT ist initialisiert */
   else {
      ret_val = FALSE;
      printf (FMELD_LINE_TXT, NIX_PLOT_TXT);
   }
#endif /* GNUPLOT */
   
   last_krit     = krit_nr;
   last_gnu_krit = idx;
   return (ret_val);
} /* c_prep_gnu_plot */ 
