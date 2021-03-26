/*******************************************************************************
GLEAM/AE                                                                  V1.2.4

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                                 Package: MENues
                                  Module: MENue-GrundSoftWare
 
Package: men                   File: men_gsw.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Der Modul enthaelt die allgemeine Initialisierung und die Default-File-Initiali-
sierung sowie Routinen zur Realisierung von Benutzerfunktionen fuer die 
GLEAM_USR-Varianten soweit die Benutzerfunktionen nicht breits durch die Packages 
"bew", "aufg", "simu" und "evo" abgedeckt sind.


Die Routinen im einzelnen:

void   gklassen (INT   k_art,
                 BOOL  alles);
          Zeigt die Kettenanzahl der durch "k_art" spezifizierten Kettensorte 
          pro Guete an. Bei gesetztem "alles" wird bei AKs auch die Anzahl der 
          BAKs und die Gesamtsumme angezeigt. Die Anzeige erfolgt ueber den 
          Textpuffer.


Im GUI_MODE exportierte Routinen, welche sonst nur package-lokal exportiert werden:

BOOL   file_data_ok (BOOL  for_evo);
          Liefert TRUE, wenn die aktuellen Zusatzanwendungsdaten und die MOD-, 
          BEW- und TSK-Daten nicht durch Ladefehler inkonsistent sind. Bei ge-
          setztem "for_evo" wird auch der Status der EVO-Daten beruecksichtigt.

BOOL   save_exp (char *exp_fspec,
                 BOOL  bew_save,
                 BOOL  tsk_save,
                 BOOL  mem_save);
          Fuehrt die Experiment-Rettung durch, hinterlegt die Ergebnisse im 
          Textpuffer und liefert TRUE, wenn alles gut ging.
          Je nach den Parametern "bew_save", "tsk_save" und "mem_save" werden
          zuerst die entsprechenden Daten gerettet. Bei erfolgreicher Rettung 
          werden "bew_aend" und "tsk_aend" zurueckgesetzt. Ferner werden die 
          Filespezifikationen gemerkt. Desgleichen wird mit zu rettenden Zu-
          satzanwendungsfiles verfahren. Wenn alle zu schreibenden Dateien er-
          folgreich geschrieben wurden, wird das EXP-File angelegt.

void   zeige_k_laengen (INT     k_art,
                        GSTATE *erg_state);
          Ausgabe von Informationen ueber die AK-Laengen aller Gueteklassen im 
          Textpuffer im nachstehenden Format:
            Güte     Noten-      AK-      Kettenlänge   
                     bereich     Anz    min   max  Schn
             xx   xxxxx-xxxxxx xxxxx  xxxxx xxxxx xxxxx 

void   prep_lade_status (void);
          Gibt die Namen geladener Files samt ihrem Status und ggf. Informatio-
          nen zum Modell des externen Simulators in den Textpuffer aus.

void   prep_vers_info (void);
          Gibt ein kurzes Standard-Versions-Info in den Textpuffer aus, wenn
          das File VERS_DOKU_FNAME nicht existiert, sonst dessen Inhalt.

GSTATE do_gen_ak (void);
          Erzeugt eine (unbewertete) AK und speichert sie in der Gueteklasse 1.
          Im Textpuffer werden (Fehler)-Meldungen zum Ergebnis der Funktion
          hinterlegt.

GSTATE do_gen_rep (char *erg_buf,
                   INT   guete,
                   INT   lfd_nr);
          Fuehrt das Genetic Repair fuer die durch "guete" und "lfd_nr" angege-
          bene AK durch und liefert das Ergebnis inkl eventueller Fehlerhinweise
          als String in "erg_buf" ab.

GSTATE do_ham_dist (char *erg_buf,
                    INT   guete1,
                    INT   lfd_nr1,
                    INT   guete2,
                    INT   lfd_nr2);
          Ermittelt den Hamming-Abstand fuer die durch "guete1,2" u. "lfd_nr1,2"
          angegebenen AKs und liefert das Ergebnis inkl eventueller Fehlerhin-
          weise als String in "erg_buf" ab.

void   display_mut_zahl (INT  mut_op,
                         INT  line_anz);
          Erzeugt im Textpuffer die Ausgabe der Verteilung des zu "mut_op"
          gehoerigen Mutationsoperators fuer alle drei Notenklassen. Angezeigt
          werden pro Notenklassen "line_anz" Intervalle.

void   display_lsk_results (INT  k_art);
          Erzeugt im Textpuffer die Ausgabe der zur "k_art" gehoerigen 
          Gesamt-Job-Statistik basierend auf "ges_ak_stat_dat" bzw. "ges_
          rk_stat_dat".

void   prep_opt_glob_erg (INT  k_art);
          Erzeugt im Textpuffer eine Auflistung der maximal MAX_EVO_ERG 
          letzten durch Optimierung erzeugten "kart" Ketten.

BOOL   do_teste_ak (INT  guete,
                    INT  lfd_nr);
          Fuehrt "teste_ak" aus "balisto" mit der angegebenen AK aus.


Package-lokaler Export:

void   ketten_pro_guete (INT     anzeige,
                         INT     guete,
                         INT     k_art,
                         GSTATE *erg_state);
          Anzeige der Ketten der Guete "guete" und der Kettensorte "k_art" 
          (z.Zt. nur AK) mit Hilfe von "print_chs" im Textpuffer. "anzeige"
          gibt den Umfang der Ausgaben an und wird an "print_chs" durchgereicht.

void   show_baks (GSTATE *erg_state);
          Auflistung aller Basis-AKs im Textpuffer. 

GSTATE ak_to_bak (char *erg_buf,
                  INT   guete,
                  INT   lfd_nr);
          Wandelt die durch "guete" und "lfd_br" angegebene AK in eine Basis-AK
          um und liefert das Ergebnis inkl eventueller Fehlerhinweise als String
          in "erg_buf" ab.

GSTATE bak_to_ak (char *erg_buf,
                  INT   guete,
                  INT   lfd_nr);
          Wandelt die durch "guete" und "lfd_br" angegebene Basis-AK in eine AK
          um und liefert das Ergebnis inkl eventueller Fehlerhinweise als String
          in "erg_buf" ab.

GSTATE delete_baks (void);
          Loescht alle BAKs und erzeugt im Textpuffer (Fehler)-Meldungen 
          zum Ergebnis der Funktion. Auftretende Fehler schlagen sich auch Funk-
          tionsergebnis nieder.

GSTATE baks_to_aks (void);
          Wandelt alle Basis-AKs in AKs um und erzeugt im Textpuffer (Feh-
          ler)-Meldungen zum Ergebnis der Funktion.

BOOL   make_ak_str (char *buf,
                    INT   guete,
                    INT   lfd_nr);
          Erzeugt einen String mit der Information Guete, LfdNr und Note der
          durch "guete" und "lfd_nr" angegebenen AK und liefert ihn in "buf" ab.
          Das Funktionsergebnis ist TRUE, wenn es diese AK gibt.

void startMeldung (char *vers);
          Ausgabe des Programmkopfes unter Verwendung der Versionskennung "vers".

--------------------------------------------------------------------------------
basiert auf den C-Units: gado_gsw.c, gado_smen.c
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Vergroesserung der Stringlaenge der Ergebnis-Anzeige in "save_exp".
 2. Neue Routine "do_ham_dist".
 3. Neue Routine "display_mut_zahl".
 4. Kleinen Fehler in "do_gen_rep" repariert.
 5. Kleinere Anpassungen in "prep_vers_info".                        WJ 07.03.03
 6. Linux-Portierung: Aus "round" wird "i_round" und "l_round".      WJ 12.05.05
 7. "do_gen_ak" sortiert simulierte AKs (z.B. von GORBA) notenge-
    recht in den AK-Speicher ein.                                    WJ 25.10.06
 8. Erweiterung fuer die MatLab-Koppung: Aus GAMA wird MatLab. An-
    passung von "prep_lade_status".                                  WJ 23.08.07
 9. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).      WJ 26.10.07
10. Entfernung von BGI_MODE und des dazugehoerigen Codes. Entfernung 
    ueberfluessiger Variable.                                        WJ 26.06.12
11. Dummy-Zeile anstelle der "maus_delay"-Zeile ins EXP-File.        WJ 29.06.12
12. "prep_lade_status" um Anzeige der Topologie-Datei erweitert.     WJ 30.08.12
13. Renovierung: Streichung des "link"-Includes. Layoutanpassungen
    bei "gklassen", "zeige_k_laengen", "prep_lade_status" und
    "save_exp".                                                      WJ 18.05.16
14. Die meisten Routinen werden im GUI_MODE Package-Export anstelle
    der package-lokalen Exports. Fehler in "prep_lade_status" 
    beseitigt.                                                       WJ 14.07.16
15. Ueberarbeitung von "prep_vers_info" wegen der LGPL.              WJ 08.09.16
16. Integration der neuen TSK-Datenverwaltung. Aus MOD_FILE_IDENT 
    wird PROG_IDENT. Ueberarbeitung von "save_exp".                  WJ 08.12.16
17. Aktualisierung der LadeStatus-Anzeige. Ersetzung von 
    "sim_mod_art_name" durch "extSimSubDir".                         WJ 10.03.17
18. Verlagerung von "clear_evo_results" nach Modul "evo_steu.c".     WJ 06.06.17
19. "print_chs" um ref-Anzeige bei CHECKS erweitert.                 WJ 31.07.17
20. Include von "webIO.h" und Ausgabe der URL der Externen 
    Simulationsservices in "prep_lade_status".                       WJ 05.10.17
21. Rettung des MEM-Files beim EXP-File-Retten gemaess dem Wert von 
    "defaultChrFileMode".                                            WJ 27.06.19
22. prep_vers_info() gibt bei gesetztem WITH_DYN_GENES einen ent-
    sprechenden Hinweis (WITH_DYN_GENES_TXT) mit aus.                WJ 10.07.19
23. Avoiding buffer overflow of static strings: print_chs(), 
    gklassen(), save_exp(), prep_lade_status(), do_gen_ak(), 
    display_lsk_results(). 
    Reorganisation of the "info" menue: Removal of displays of Basic 
    Chromosomes, see compiler switch WITH_B_CHRS.                    WJ 30.07.20
24. Some minor modifications to prep_vers_info(). Integration of 
    startMeldung() from package ctio.                                WJ 16.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 16.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "chio.h"
#include "bew.h"
#include "appl.h"
#include "hmod.h"
#include "aufg.h"
#ifdef EXT_SIMU_SERV
  #include "webIO.h"
#endif /* EXT_SIMU_SERV */
#include "simu.h"
#include "evo.h"
#include "men.h"
#include "menInt.h"

#undef WITH_B_CHRS             /* to be set only if basic chrs are used again */
                               /* Must be set before the include of MEN_TEXTS */

#include GLOBAL_TEXTS
#include VERS_TEXTS
#include MEN_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ============================ Lokale Konstante: =========================== */
#define EXP_HALB_STRICH  "================================"
#define MAX_LEN          32767                    /* fuer "display_mut_zahl"  */



/* ============================ Lokale Routinen: ============================ */
static void print_chs (CHAIN_ELEM_PTR_TYPE  vk_ptr,
                       INT                  znr, 
                       INT                  anzeige);





/* ================================ gklassen ================================ */
void gklassen (INT   k_art,
               BOOL  alles)
{
   STRING_TYPE  buffer;
   STR_TYPE     buf1, buf2;
   BUF_TYPE     k_art_buf;
   LONG         ug, og;
   INT          k_anz, k_sum, guete, znr;
   GSTATE       ret_state = GLEAM_OK;
  
   /* GKLASSEN/LSK_SMEN */
   reset_text_buf ();
   znr = 1;
   if (alles) 
   {
      if (k_art == AK) 
         strcpy (k_art_buf, AKTIONS_TXT);
      else 
         strcpy (k_art_buf, " ??");
      sprintf (buffer, G_KLASSEN_HDR1, k_art_buf);
      write_buf (znr, buffer);
      znr = 3;
   } 
   write_buf (znr, G_KLASSEN_HDR2);
   k_sum = 0;
   for (guete = 1;  guete <= MAX_GUETE;  guete++) 
   { /* Alle Gueteklassen */
      k_anz  = number_of_chains (guete, guete, k_art, &ret_state);
      k_sum += k_anz;
      get_rank_limits (&ug, &og, guete);
      sprintf (buf1, "    %2d      %5ld - %6ld   %4d", guete, ug, og, k_anz);
      write_buf (guete+znr, buf1);
   } /* Alle Gueteklassen */
   if (alles) 
   { /* Endanzeige */
#ifdef WITH_B_CHRS
      if (k_art == AK) 
      { /* AKs */
         guete = BASIS_AK_GUETE;
         sprintf (buf2, G_KLASS_BAKS, 
                  number_of_chains (guete, guete, k_art, &ret_state));
      } /* AKs */
      else
#endif /* WITH_B_CHRS */
         strcpy (buf2, "");
      sprintf (buffer, G_KLASS_FINTXT, k_sum, k_art_buf);
      write_buf (18 + znr, buffer);
      write_buf (19 + znr, buf2);
   } /* Endanzeige */
} /* gklassen */



/* ============================== file_data_ok ============================== */
BOOL file_data_ok (BOOL  for_evo)
{
   INT   ii;
   BOOL  ok;

   /* file_data_ok */
   ok = !(bad_mod_data || bad_bew_data || bad_tsk_data);
   for (ii = 0;  ii < appl_file_anz;  ii++)
      if (appl_file_descr[ii].bad_data)
         ok = FALSE;
   if (for_evo)
      return (ok && !bad_evo_data);
   else
      return (ok);
} /* file_data_ok */
 


/* ================================ save_exp ================================ */
BOOL save_exp (char *exp_fspec,
               BOOL  bew_save,
               BOOL  tsk_save,
               BOOL  mem_save)
{
   FILE        *exp_file;
   STRING_TYPE  buffer, exp_name, tmp_mem_fspec;
   STR_TYPE     buf;
   INT          znr, len, ii;
   GSTATE       erg_state;
   BOOL         erg      = TRUE,       /* final result                        */
                badFiles = FALSE;      /* TRUE, if file(s) with spoilded data */

   /* save_exp */
   reset_text_buf ();
   if ((exp_file = fopen (exp_fspec, "w")) == NULL)
   { /* EXP-File nicht offen */
      sprintf (buffer, SF_BAD_SPEC, exp_fspec);
      write_buf (1, buffer);
      erg = FALSE;
   } /* EXP-File nicht offen */
   else
   { /* EXP-File offen */
      znr = 1;
      sprintf (buffer, SAV_EXP_HDR, exp_fspec);
      write_buf (znr++, buffer);
      len = strlen (exp_fspec) - 4;
      strcpy (exp_name, exp_fspec);
      exp_name [len] = '\0';
      strcpy (tmp_mem_fspec, exp_name);
      strcat (tmp_mem_fspec, ".mem");

      /* ---------------------------- BEW-File: ----------------------------- */
      if (bew_save)
      { /* Bewertung sichern */
         if ((erg_state = save_bew (tmp_bew_file_spec)) == GLEAM_OK)
         { /* Rettung ok */
            sprintf (buffer, SAV_GEN_TXT, IO_BEW_MEN, "", tmp_bew_file_spec);
	    setStringVal(tmp_bew_file_spec, BEW_FILE_SPEC_IDX);
            bew_aend = FALSE;
         } /* Rettung ok */
         else
         { /* Rettung fehlgeschlagen */
            sprintf (buffer, SAV_GEN_TXT, IO_BEW_MEN, NICHT_TXT, tmp_bew_file_spec);
            erg = FALSE;
         } /* Rettung fehlgeschlagen */
         write_buf (znr++, buffer);
         err_state |= erg_state;                  /* Fehlerstatus uebernehmen */
      } /* Bewertung sichern */
      
      /* ---------------------------- TSK-File: ----------------------------- */
      if (tsk_save)
      { /* Aufgabe sichern */
         if ((erg_state = saveTskFile (tmp_tsk_file_spec)) == GLEAM_OK)
         { /* Rettung ok */
            sprintf (buffer, SAV_GEN_TXT, IO_TSK_MEN, "", tmp_tsk_file_spec);
            setStringVal(tmp_tsk_file_spec, TSK_FILE_SPEC_IDX);
            tsk_aend = FALSE;
         } /* Rettung ok */
         else
         { /* Rettung fehlgeschlagen */
            sprintf (buffer, SAV_GEN_TXT, IO_TSK_MEN, NICHT_TXT, tmp_tsk_file_spec);
            erg = FALSE;
         } /* Rettung fehlgeschlagen */
         write_buf (znr++, buffer);
         err_state |= erg_state;                  /* Fehlerstatus uebernehmen */
      } /* Aufgabe sichern */

      /* ---------------------------- MEM-File: ----------------------------- */
      if (mem_save)
      { /* AK-Mem sichern */
         erg = FALSE;
         if (open_ch_file (tmp_mem_fspec, znr, CH_WRITE, GLOBAL_SAVE, defaultChrFileMode))
         { /* File offen */
            if (save_ch_mem ())
            { /* Rettung ok */
               setStringVal(tmp_mem_fspec, MEM_FILE_SPEC_IDX);
               erg = TRUE;
            } /* Rettung ok */      
         } /* File offen */
         close_ch_file ();
         znr = get_line_anz() + 1;
      } /* AK-Mem sichern */
      
      /* --------------------------- Appl-Files: ---------------------------- */
      switch (appl_code)
      { /* "appl_code"-Switch */
         case LESAK_APPL:
         break; /* LESAK_APPL */

         case ADD_APPL:
	    if (appl_file_anz > 0)
	       erg &= add_appl_save_afiles (&znr, tmp_afile_spec);
         break; /* ADD_APPL */

         case GADO_APPL:
         case SIMU_SERV_APPL:
         case MATLAB_APPL:
         case MATH_FKT_APPL:
         case CEC_MATHFKT_APPL:
         break; /* ..._APPL */

         default:
	    erg = FALSE;
	    err_state |= fehler (MEN_GSW, 10, appl_code, NO_INFO, "", BAD_APPL_CODE);
      } /* "appl_code"-Switch */

      /* ---------------------------- EXP-File: ----------------------------- */
      if (erg)
      { /* Alles klar, EXP-File anlegen! */
	 fprintf (exp_file, "# %s %s %s\n", EXP_HALB_STRICH, EXP_FILE_HDR_TXT, EXP_HALB_STRICH); 
         fprintf (exp_file, "%s\n", PROG_IDENT);
         get_time_stamp (buffer, buf, buf, buf);
	 fprintf(exp_file, "# %s %s\n", HDR_TIME_STAMP_TXT, buffer);
	 fprintf(exp_file, "# %s%s%s\n", EXP_FILE_LINE, EXP_STD_FILE_TXT, EXP_FILE_LINE);
	 if (getStringVal(buffer, MOD_FILE_SPEC_IDX, GLEAM_ERROR))
	    erg &= (fprintf(exp_file, "%-*s = %s\n", getMaxNameLength(), MOD_FILE_SPEC_TXT, buffer) > 0);
	 badFiles = (strcmp(buffer, BAD_DATA_TXT) == 0);
	 if (getStringVal(buffer, BEW_FILE_SPEC_IDX, GLEAM_ERROR))
	    erg &= (fprintf(exp_file, "%-*s = %s\n", getMaxNameLength(), BEW_FILE_SPEC_TXT, buffer) > 0);
	 badFiles |= (strcmp(buffer, BAD_DATA_TXT) == 0);
	 if (getStringVal(buffer, TSK_FILE_SPEC_IDX, GLEAM_ERROR))
	    erg &= (fprintf(exp_file, "%-*s = %s\n", getMaxNameLength(), TSK_FILE_SPEC_TXT, buffer) > 0);
	 badFiles |= (strcmp(buffer, BAD_DATA_TXT) == 0);
	 if (getStringVal(buffer, EVO_FILE_SPEC_IDX, GLEAM_ERROR))
	    erg &= (fprintf(exp_file, "%-*s = %s\n", getMaxNameLength(), EVO_FILE_SPEC_TXT, buffer) > 0);
	 badFiles |= (strcmp(buffer, BAD_DATA_TXT) == 0);
	 if (getStringVal(buffer, MEM_FILE_SPEC_IDX, GLEAM_OK))
	    if (strlen(buffer) > 0) 
	       erg &= (fprintf(exp_file, "%-*s = %s\n", getMaxNameLength(), MEM_FILE_SPEC_TXT, buffer) > 0);
	 if (getStringVal(buffer, TOP_FILE_SPEC_IDX, GLEAM_OK))
	    if (strlen(buffer) > 0) {
	       erg &= (fprintf(exp_file, "%-*s = %s\n", getMaxNameLength(), TOP_FILE_SPEC_TXT, buffer) > 0);
	       badFiles |= (strcmp(buffer, BAD_DATA_TXT) == 0);
	    }
	 if (appl_file_anz > 0)
	 { /* there is at least one application specific file */
	    erg &= (fprintf (exp_file, "%s%s%s%s\n", SECT_DELIMITER_TXT, EXP_HALB_STRICH, 
			     EXP_APPL_FILE_TXT, EXP_HALB_STRICH) > 0);
	    for (ii = 0;  ii < appl_file_anz; ii++)
	       erg = erg && (fprintf (exp_file, "%s\n", appl_file_descr[ii].afile_spec) > 0);
	 } /* there is at least one application specific file */
	 if (badFiles)
	    meldung(MEN_GSW, NO_INFO, NO_INFO, BAD_DATA_TXT, BAD_FILE_SPECS_IN_EXP_FILE);
	 if (erg && !badFiles)
	    write_buf (znr, SAVE_OK_TXT);
	 else
	    write_buf (znr, SAV_EXP_ERR2);
      } /* Alles klar, EXP-File anlegen! */
      else
         write_buf (znr, SAV_EXP_ERR);
      fclose (exp_file);
   } /* EXP-File offen */
   return (erg);
} /* save_exp */



/* ============================ zeige_k_laengen ============================= */
void zeige_k_laengen (INT     k_art,
                      GSTATE *erg_state)
{
   STR_TYPE             buffer, buf1; 
   CHAIN_ELEM_PTR_TYPE  vk_ptr, vk_elem_ptr;
   INT                  guete, ch_anz, len, min_len, max_len;
   LONG                 len_sum, ug, og;
   BOOL                 ak_len;
  
   /* zeige_k_laengen */
   reset_text_buf ();
   ak_len = k_art == AK;
   if (ak_len) 
   {
      strcpy (buffer, AK_LEN_HDR1);
      strcpy (buf1, AK_LEN_HDR2);
   } 
   else
   {
      strcpy (buffer, "??");
      strcpy (buf1, "??");
   }
   write_buf (1, buffer);
   write_buf (3, buf1);
   write_buf (4, AK_LEN_HDR3);
   for (guete = 1;  guete <= MAX_GUETE;  guete++)
   { /* Ketten einer Guete */
      vk_ptr = get_vk (k_art, guete, erg_state);
      if  (*erg_state != GLEAM_OK)
         break;                                                    /* Abbruch */
      ch_anz = vk_ptr->u_el.u_xel.u_hdr.chain_length;
      get_rank_limits (&ug, &og, guete);
      sprintf (buffer, "%3d   %5ld-%6ld %5d", guete, ug, og, ch_anz);
      if (ch_anz > 0) 
      { /* es gibt Ketten */
         min_len = 32600;
         max_len = 0;
         len_sum = 0;
         vk_elem_ptr = vk_ptr->u_el.next_action;
         while (vk_elem_ptr != NULL)
         { /* Kettenlaenge verarbeiten */
            len = vk_elem_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.
               chain_length;
            if (len < min_len) 
               min_len = len;
            if (len > max_len) 
               max_len = len;
            len_sum += len;
            vk_elem_ptr = vk_elem_ptr->u_el.next_action;
         } /* Kettenlaenge verarbeiten */
         sprintf (buf1, "  %5d %5d %5d", min_len, max_len, 
                  i_round ((DOUBLE)(len_sum)/(DOUBLE)(ch_anz)));
         strcat (buffer, buf1);
      } /* es gibt Ketten */
      write_buf (guete + 4, buffer);
   } /* Ketten einer Guete */
} /* zeige_k_laengen */



/* ============================ prep_lade_status ============================ */
void prep_lade_status (void)
{
   char         buffer[2*MAX_STRING];
   char         buf1[MAX_STRING+MAX_BUF];
   STRING_TYPE  fSpecBuf;
   INT          znr, ii;

   /* prep_lade_status */
   reset_text_buf ();
   write_buf (1, LADE_STAT_HDR);

   getStringVal(fSpecBuf, MOD_FILE_SPEC_IDX, GLEAM_ERROR);
   if (strcmp (fSpecBuf, BAD_DATA_TXT))
   { /* ok */
      sprintf (buf1, "\"%s\"", fSpecBuf);
      sprintf (buffer, LSTAT_APPL_TXT, appl_name);
   } /* ok */
   else
   { /* Inkonsistente HMod-Daten */
      strcpy (buf1, LSTAT_BAD_DATA);
      sprintf (buffer, LSTAT_APPL_TXT, LSTAT_BAD_DATA);
   } /* Inkonsistente HMod-Daten */
   write_buf (3, buffer);                                  /* Akt. Anwendung  */
   if (appl_code == LESAK_APPL)
      sprintf (buffer, LSTAT_HMOD_TXT, buf1);
   else
      sprintf (buffer, LSTAT_GMOD_TXT, buf1);
   write_buf (5, buffer);                        /* Gen- oder Handlungsmodell */
   if (!strcmp (exp_file_spec, "")) 
      strcpy (buf1, LSTAT_NO_EXP);
   else 
      sprintf (buf1, "\"%s\"", exp_file_spec);
   sprintf (buffer, LSTAT_EXP_TXT, buf1);
   write_buf (4, buffer);                                  /* Experiment      */
   znr = 6;
   for (ii = 0;  ii < appl_file_anz;  ii++)
   { /* Zusatz-Anwendungsfiles */
      if (strcmp (appl_file_descr[ii].afile_spec, BAD_DATA_TXT))
         sprintf (buf1, "\"%s\"", appl_file_descr[ii].afile_spec);
      else
         strcpy (buf1, LSTAT_BAD_DATA);

      sprintf (buffer, "%-*s: %s", AFILE_BEZ_LEN, 
               appl_file_descr[ii].afile_bez, buf1);
      write_buf (znr++, buffer);                           /* ZusatzApplFile  */
   } /* Zusatz-Anwendungsfiles */
   if (mit_ext_simu)
   { /* Mit externem Simulator */
#ifdef EXT_SIMU_SERV
      sprintf (buffer, LSTAT_EXT_SIM_SERV, serverBuffer);
#else /* no EXT_SIMU_SERV */
      sprintf (buffer, LSTAT_EXT_SIM, ext_simu_name);
#endif /* no EXT_SIMU_SERV */
      write_buf (znr++, buffer);                           /* Externer Simu.  */
      sprintf (buffer, LSTAT_ESIM_MOD, extSimuModelName); 
      write_buf (znr++, buffer);                           /* ExtSim: Modell  */
      if (appl_code == GADO_APPL) {     
         sprintf (buffer, LSTAT_ESIMPFAD, sim_root_dir_name, 
                  DIR_DELIMITER, extSimSubDir);
         write_buf (znr++, buffer);                        /* ExtSim: Pfad    */
      }
   } /* Mit externem Simulator */
   else
      if (mbf_appl)
      { /* Math. Benchmark-Funktionen */
         if (appl_code == CEC_MATHFKT_APPL)
	    strcpy (buf1, " (CEC'05)");
         else
	    strcpy (buf1, "");
         sprintf (buffer, LSTAT_MBF_SIM, buf1, ext_simu_name);
         write_buf (znr++, buffer);                           /* Externer Simu.  */
      } /* Math. Benchmark-Funktionen */
      else
         write_buf (znr++, LSTAT_INT_SIM);                 /* Interner Simu.  */

   getStringVal(fSpecBuf, BEW_FILE_SPEC_IDX, GLEAM_ERROR);
   if (strcmp (fSpecBuf, BAD_DATA_TXT))
      sprintf (buf1, "\"%s\"", fSpecBuf);
   else
      strcpy (buf1, LSTAT_POS_BAD_DATA);
   if (bew_aend)
      sprintf (buffer, LSTAT_MBEW_TXT, buf1);
   else
      sprintf (buffer, LSTAT_BEW_TXT, buf1);
   write_buf (znr++, buffer);                              /* Bewertungsfile  */ 
   getStringVal(fSpecBuf, TSK_FILE_SPEC_IDX, GLEAM_ERROR);
   if (strcmp (fSpecBuf, BAD_DATA_TXT))
      sprintf (buf1, "\"%s\"", fSpecBuf);
   else
      strcpy (buf1, LSTAT_POS_BAD_DATA);
   if (tsk_aend)
      sprintf (buffer, LSTAT_MTSK_TXT, buf1);
   else
      sprintf (buffer, LSTAT_TSK_TXT, buf1);
   write_buf (znr++, buffer);                              /* Aufgabenfile    */ 
   getStringVal(fSpecBuf, EVO_FILE_SPEC_IDX, GLEAM_ERROR);
   if (strcmp (fSpecBuf, BAD_DATA_TXT))
      sprintf (buf1, "\"%s\"", fSpecBuf);
   else
      strcpy (buf1, LSTAT_BAD_DATA);
   sprintf (buffer, LSTAT_EVO_TXT, buf1);
   write_buf (znr++, buffer);                              /* EvoParam-File   */
   if (!getStringVal(fSpecBuf, MEM_FILE_SPEC_IDX, GLEAM_OK))
      strcpy (buf1, LSTAT_NO_CHF);
   else 
      if (strcmp (fSpecBuf, BAD_DATA_TXT))
	 sprintf (buf1, "\"%s\"", fSpecBuf);
      else
	 strcpy (buf1, LSTAT_BAD_DATA);                /* not really possible */
   sprintf (buffer, LSTAT_CHF_TXT, buf1);
   write_buf (znr++, buffer);                              /* Ketten-File     */
   if (getStringVal(fSpecBuf, TOP_FILE_SPEC_IDX, GLEAM_OK))
      if (strcmp (fSpecBuf, BAD_DATA_TXT))
	 if (strlen (fSpecBuf) == 0)
	    strcpy (buf1, SYM_RING_TXT);
	 else
	    sprintf (buf1, "\"%s\"", fSpecBuf);
      else
	 strcpy (buf1, LSTAT_BAD_DATA);
   else
      strcpy (buf1, SYM_RING_TXT);
   sprintf (buffer, LSTAT_TOP_TXT, buf1);
   write_buf (znr++, buffer);                              /* Topologie-File  */
   if (!last_exp_ok)
      write_buf (znr+1, LOAD_EXP_F_TXT);
} /* prep_lade_status */



/* ============================= prep_vers_info ============================= */
void prep_vers_info (void)
{
   STRING_TYPE  buffer;
   FILE        *vers_file;
   INT          ii;
   BOOL         weiter,
                from_file = FALSE;

   /* prep_vers_info */
   reset_text_buf ();
   if (file_exists (VERS_DOKU_FNAME))
      from_file = (vers_file = fopen (VERS_DOKU_FNAME, "r")) != NULL;
   ii = 1;
   if (from_file)
   { /* Vers-File offen */
      do
         if (weiter = (fgets (buffer, MAX_STRING, vers_file) != NULL))
         {
            buffer [strlen(buffer)-1] = '\0';
            weiter = write_buf (ii++, buffer);
         }
      while (weiter);
      fclose (vers_file);
      ii++;
   } /* Vers-File offen */
   else
   { /* Default Text */
      write_buf (ii++, VERS_INFO_1);
      write_buf (ii++, VERS_INFO_2);
      write_buf (ii++, VERS_INFO_3);
      write_buf (ii++, VERS_INFO_4);
      write_buf (ii++, VERS_INFO_5);
      write_buf (ii++, VERS_INFO_6);
      write_buf (ii++, VERS_INFO_7);
      write_buf (ii++, VERS_INFO_8);
      write_buf (ii++, VERS_INFO_9);
      write_buf (ii++, VERS_INFO_10);
      write_buf (ii++, VERS_INFO_11);
      write_buf (ii++, VERS_INFO_12);
      write_buf (ii++, VERS_INFO_13);
      write_buf (ii++, VERS_INFO_14);
      write_buf (ii++, VERS_INFO_15);
      write_buf (ii++, VERS_INFO_16);
      write_buf (ii++, VERS_INFO_17);
      write_buf (ii++, VERS_INFO_18);
      write_buf (ii++, VERS_INFO_19);
      write_buf (ii++, VERS_INFO_20);
      write_buf (ii++, VERS_INFO_21);
      write_buf (ii++, VERS_INFO_22);
#ifdef WITH_DYN_GENES
      write_buf (ii++, DYN_GENES_1_TXT);
      write_buf (ii++, DYN_GENES_2_TXT);
#endif /* WITH_DYN_GENES */
#ifdef MIT_LSV /* HyGLEAM */
      ii++;
      write_buf (ii++, VERS_INFO_H1);
      write_buf (ii++, VERS_INFO_H2);
      write_buf (ii++, VERS_INFO_H3);
      write_buf (ii++, VERS_INFO_H4);
      ii++;
      write_buf (ii++, VERS_INFO_CBL);
      write_buf (ii++, VERS_INFO_CWJ1);
      write_buf (ii++, VERS_INFO_CWJ2);
#else /* einfaches GLEAM */
      write_buf (ii++, VERS_INFO_CBL);
#endif  /* einfaches GLEAM */
      write_buf (ii++, VERS_INFO_L1);
      write_buf (ii++, VERS_INFO_L2);
      write_buf (ii++, VERS_INFO_L3);
      write_buf (ii++, VERS_INFO_L4);
      write_buf (ii++, VERS_INFO_L5);
   } /* Default Text */
   ii++;
   write_buf (ii, prog_id_buf);
} /* prep_vers_info */



/* =============================== do_gen_ak ================================ */
GSTATE do_gen_ak (void)
{
   STR_TYPE             buffer;
   BUF_TYPE             adr_buf;
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   GSTATE               res_state = GLEAM_OK;
   INT                  lfd_nr, guete;

   /* do_gen_ak */
   reset_text_buf ();
   write_buf (1, AK_GEN_HDR);
   if (gen_chain (&ak_ptr, &res_state))
   { /* AK ok */
      if (ak_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED) {
         if (!berechne_guete (&guete, ak_ptr->u_el.u_xel.u_hdr.fitness_note))
            guete = 1;
      }
      else
         guete = 1;
      res_state |= store_chain (&lfd_nr, ak_ptr, AK, guete);
      if (res_state == GLEAM_OK)
      { /* AK gespeichert */
        adr_to_text (adr_buf, guete, lfd_nr, NO_ADJUST);
        sprintf (buffer, GEN_AK_OK_TXT, adr_buf);
        write_buf (3, buffer);
      } /* AK gespeichert */
      else
         write_buf (3, GEN_AK_F1_TXT);
   } /* AK ok */
   else
      write_buf (3, GEN_AK_F2_TXT);
   return (res_state);
} /* do_gen_ak */



/* =============================== do_gen_rep =============================== */
GSTATE do_gen_rep (char *erg_buf,
                   INT   guete,
                   INT   lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr, ak2_ptr;
   GSTATE               res_state = GLEAM_OK;
   INT                  old_ref_ctr;

   /* do_gen_rep */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &res_state))
   { /* AK existiert */
      old_ref_ctr = ak_ptr->u_el.u_xel.u_hdr.ref_counter;
      (ak_ptr->u_el.u_xel.u_hdr.ref_counter)++;
      ak2_ptr = ak_ptr;  /* weil repair.. bei unplausiblen AKs den Ptr loescht*/
      if (repair_1_chain (&ak2_ptr, &res_state))
         strcpy (erg_buf, ERLEDIGT_TXT);                      /* AK repariert */
      else
      { /* AK irreparabel */
         if (res_state == GLEAM_OK)
            strcpy (erg_buf, GENREP_NIX_TXT);               /* AK unplausibel */
         else
            strcpy (erg_buf, FKT_UNVOLLST);                         /* Fehler */
      } /* AK irreparabel */
      ak_ptr->u_el.u_xel.u_hdr.ref_counter = old_ref_ctr;
   } /* AK existiert */
   else
      strcpy (erg_buf, NIX_CH);
   return (res_state);
} /* do_gen_rep */



/* ============================== do_ham_dist =============================== */
GSTATE do_ham_dist (char *erg_buf,
                    INT   guete1,
                    INT   lfd_nr1,
                    INT   guete2,
                    INT   lfd_nr2)
{
   CHAIN_ELEM_PTR_TYPE  ak1_ptr, ak2_ptr;
   GSTATE               res_state = GLEAM_OK;

   /* do_ham_dist */
   if (get_chain (&ak1_ptr, &lfd_nr1, AK, guete1, &guete1, &res_state) &&
       get_chain (&ak2_ptr, &lfd_nr2, AK, guete2, &guete2, &res_state))
      if (erg_genauigkeit > 6)
         sprintf (erg_buf, HAM_DIST_TXT2, erg_genauigkeit, 
                  ham_dist (ak1_ptr, ak2_ptr) * 100.0);
      else
         sprintf (erg_buf, HAM_DIST_TXT, ham_dist (ak1_ptr, ak2_ptr) * 100.0);
   else
      strcpy (erg_buf, NIX_CH);
   return (res_state);
} /* do_ham_dist */



/* ============================ display_mut_zahl ============================ */
void display_mut_zahl (INT  mut_op,
                       INT  line_anz)
{
   STR_TYPE  buf1;
   LONG      n_og1, n_og2, n_ug, n_og;
   INT       last_range, range, nklasse, znr, len, last_len, ii;
   BOOL      bdummy;

   /* display_mut_zahl */
   reset_text_buf ();
   get_noten_limits (&n_og1, &n_og2);
   n_ug = 0;
   n_og = n_og1;
   znr  = 1;
   for (nklasse = 0;  nklasse < 3;  nklasse++)
   { /* Notenklassen */
      sprintf (buf1, N_KLASSEN_TXT, nklasse+1, n_ug, n_og);
      write_buf (znr++, buf1);
      len = 1;
      calc_p_go (&bdummy, 0, n_ug);
      mutations_zahl (mut_op, len);
      last_range = get_mz_bereich ();
      do
      {
         len++;
         mutations_zahl (mut_op, len);
         range = get_mz_bereich ();
      }
      while ((last_range == range) && (len < MAX_LEN));
      sprintf (buf1, MZ_ANZG2_TXT, 1, len - 1, last_range);
      write_buf (znr++, buf1);
      last_range = range;
      last_len   = len;
      for (ii = 1;  (ii < line_anz) && (len < MAX_LEN);  ii++)
      { /* weitere Zeilen */
         do
         {
            len++;
            mutations_zahl (mut_op, len);
            range = get_mz_bereich ();
         }
         while ((last_range == range) && (len < MAX_LEN));
         sprintf (buf1, MZ_ANZG3_TXT, last_len, len - 1, last_range);
         write_buf (znr++, buf1);
         last_range = range;
         last_len   = len;
      }  /* weitere Zeilen */
      if ((nklasse < 2) && (line_anz < 8))
         write_buf (znr++, "");
      
      if (nklasse == 0)
      { /* Als naechstes 2. Notenklasse */
         n_ug = n_og1 + 1;
         n_og = n_og2;
      } /* Als naechstes 2. Notenklasse */
      else
      { /* Als naechstes 3. Notenklasse */
         n_ug = n_og2 + 1;
         n_og = INOTEN_MAX;
      } /* Als naechstes 3. Notenklasse */
   } /* Notenklassen */
} /* display_mut_zahl */



/* ========================== display_lsk_results =========================== */
void display_lsk_results (INT  k_art)
{
   STRING_TYPE    buffer;
   STR_TYPE       buf1;
   BUF_TYPE       k_buf;
   GES_STAT_TYPE  stat_dat;
   INT            znr, guete;
  
   /* display_lsk_results */
   reset_text_buf ();
   if (k_art == AK)
   { /* AK-Evo-Ergebnisse */
      strcpy (k_buf, AK_TXT);
      znr = 1;
      stat_dat = ges_ak_stat_dat;
   } /* AK-Evo-Ergebnisse */
   else
   { /* RK-Evo-Ergebnisse */
      strcpy (k_buf, "??");
      znr = 9;
      stat_dat = ges_rk_stat_dat;
   } /* RK-Evo-Ergebnisse */
   sprintf (buf1, ANZG_JOBS_TXT, stat_dat.job_anz, k_buf);
   write_buf (znr++, buf1);
   sprintf (buf1, ANZG_POPUL_TXT, stat_dat.popel_anz);
   write_buf (znr++, buf1);
   sprintf (buf1, ANZG_GEN_TXT, stat_dat.gen_anz);
   write_buf (znr++, buf1);
   sprintf (buf1, ANZG_OFFSPRTXT, stat_dat.jobli_indivs);
   write_buf (znr++, buf1);
   berechne_guete (&guete, stat_dat.best_note);
   sprintf (buf1, ANZG_NOTE_TXT, k_buf,
            stat_dat.best_note, guete);
   write_buf (znr++, buf1);
   sprintf (buf1, ANZG_ERGAK_TXT, stat_dat.res_ch_sum);
   write_buf (znr++, buf1);
   display_time (buf1, stat_dat.zeit_sum, ADJUST);
   sprintf (buffer, ANZG_ZEIT_TXT, buf1);
   write_buf (znr++, buffer);
   write_buf (znr, STRICH_40_TXT);
} /* display_lsk_results */



/* =========================== prep_evo_glob_erg ============================ */
void prep_opt_glob_erg (INT  k_art)
{
   STR_TYPE  buffer, buf1;
   INT       znr, eintr_pro_zeile, anz, ii, jj;

   /* prep_evo_glob_erg */
   reset_text_buf ();
   strcpy (buffer, "");
   znr             = 1;
   eintr_pro_zeile = 0;
   jj = (last_evo_erg[k_art].last_evo_index+1) % MAX_EVO_ERG;
   if (last_evo_erg[k_art].evo_erg_feld[jj].note == KEINE_NOTE)
   { /* nicht voll */
      anz = last_evo_erg[k_art].last_evo_index + 1;
      jj  = 0;
   } /* nicht voll */
   else
      anz = MAX_EVO_ERG;
   for (ii = 0;  ii < anz;  ii++)
   { /* Ausgabe der AK-Ergebnisse */
      sprintf (buf1, "%6ld:", 
               l_round (last_evo_erg[k_art].evo_erg_feld[jj].note));
      strcat (buffer, buf1);
      adr_to_text (buf1, last_evo_erg[k_art].evo_erg_feld[jj].guete,  
                   last_evo_erg[k_art].evo_erg_feld[jj].lfdnr, ADJUST);
      strcat (buffer, buf1);
      strcat (buffer, " ");
      eintr_pro_zeile++;
      if (eintr_pro_zeile == 5)
      { /* Ausgabe */
         write_buf (znr++, buffer);
         eintr_pro_zeile = 0;
         strcpy (buffer, "");
      } /* Ausgabe */
      jj = (jj + 1) % MAX_EVO_ERG;
   } /* Ausgabe der AK-Ergebnisse */
   if (eintr_pro_zeile > 0)
      write_buf (znr++, buffer);
} /* prep_evo_glob_erg */



/* =============================== do_teste_ak ============================== */
BOOL do_teste_ak (INT  guete,
                  INT  lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   GSTATE               res_state = GLEAM_OK;

   /* do_teste_ak */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &res_state))
      return (teste_ak (ak_ptr));
   else
   {
      write_buf (1, NIX_CH);
      return (FALSE);
   }
} /* do_teste_ak */



/* =============================== print_chs ================================ */
static void print_chs (CHAIN_ELEM_PTR_TYPE  vk_ptr,
                       INT                  znr, 
                       INT                  anzeige)
/*----------------------------------------------------------------------------*/
/*  Anzeige der Ketten einer Gueteklasse oder der BAKs ab "znr" im Format     */
/*  gemaess "anzeige" im Textpuffer:                                          */
/*                                                                            */
/*  "zeige_baks":                                                             */
/*      nnnnnn:gg/lll   mit nnnnnn = Notenwert                                */
/*                          gg    = Guete                                     */
/*                          lll   = Laufende Nummer                           */
/*      Dieses Format beansprucht 13 Zeichen, so dass in einer 66er-          */
/*      Zeile 4 Spalten bei einem Trenner von 3 Zeichen untergebracht         */
/*      werden koennen.                                                       */
/*                                                                            */
/*  "zeige_chs_kurz":                                                         */
/*      nnnnnn:lll      mit nnnnnn = Notenwert                                */
/*                          lll    = Laufende Nummer                          */
/*      Dieses Format beansprucht 10 Zeichen, so dass in einer 66er-          */
/*      Zeile 5 Spalten bei einem Trenner von 3 Zeichen untergebracht         */
/*      werden koennen. Angezeigt werden die Ketten einer Gueteklasse,        */
/*      sodass sich die Ausgabe des Guetewertes eruebrigt.                    */
/*                                                                            */
/*  "zeige_chs_detail":                                                       */
/*      Angezeigt werden in einer Zeile folgende Daten einer Kette in         */
/*      tabellarischer Form, wobei die "ref"-Spalte nur bei gesetztem CHECKS- */
/*      Schalter angezeigt wird:                                              */
/*      If WITH_B_CHRS is set:                                                */
/*          Adr    Fitness   Len   Segm   BAK   Sim   Ref                     */
/*        gg/lll  ffffff.f   lll    sss    -     +     rr                     */
/*      otherwise:                                                            */
/*          Adr    Fitness   Len   Segm   Sim   Ref                           */
/*        gg/lll  ffffff.f   lll    sss    +     rr                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
{
   char                 buffer[2*MAX_STRING];
   STRING_TYPE          buf1;
   BUF_TYPE             buf2, buf3;
   CHAIN_ELEM_PTR_TYPE  vk_elem_ptr, ch_ptr;
   INT                  max_eintr_pro_zeile, eintr_pro_zeile;
   BOOL                 weiter;
  
   /* print_chs */
   eintr_pro_zeile = 0;
   weiter          = TRUE;
   vk_elem_ptr     = vk_ptr->u_el.next_action;
   strcpy (buffer, "");
   while ((vk_elem_ptr != NULL) && weiter)
   { /* Alle VK-Elemente abarbeiten */
      ch_ptr = vk_elem_ptr->u_el.u_xel.u_sub.sub_chain;
      switch (anzeige) 
      {
      case ZEIGE_BAKS:
         adr_to_text (buf2, ch_ptr->u_el.u_xel.u_hdr.guete, 
                      ch_ptr->u_el.u_xel.u_hdr.lfd_nr, ADJUST);
         sprintf (buf1, "%6ld:%s ", 
                  l_round (ch_ptr->u_el.u_xel.u_hdr.fitness_note), buf2);
         max_eintr_pro_zeile = 5;
         break; /* Zeige BAKs */

      case ZEIGE_CHS_KURZ:
         /*         left_adj (buf2, ch_ptr->u_el.u_xel.u_hdr.lfd_nr, 3); */
         sprintf (buf1, "%6ld:%-3d   ", 
                  l_round (ch_ptr->u_el.u_xel.u_hdr.fitness_note), 
                  ch_ptr->u_el.u_xel.u_hdr.lfd_nr);
         max_eintr_pro_zeile = 5;
         break; /* Zeige Kette in Kurzform */

      case ZEIGE_CHS_DETAIL:
         adr_to_text (buf2, ch_ptr->u_el.u_xel.u_hdr.guete,  
                      ch_ptr->u_el.u_xel.u_hdr.lfd_nr, ADJUST);
#ifdef WITH_B_CHRS
         strcpy (buf3, "");
         if (ch_ptr->u_el.u_xel.u_hdr.chain_status & BASIS_AK)
            strcpy (buf3, "   +     ");
         else 
            strcpy (buf3, "   -     ");
#else /* no WITH_B_CHRS */
         strcpy (buf3, "   ");
#endif /* no WITH_B_CHRS */
         if (ch_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED)
            strcat (buf3, "+");
         else
            strcat (buf3, "-");
         sprintf (buf1, " %s  %8.1f %5d  %5d %s", buf2, 
                  ch_ptr->u_el.u_xel.u_hdr.fitness_note, 
                  ch_ptr->u_el.u_xel.u_hdr.chain_length, 
                  ch_ptr->u_el.u_xel.u_hdr.segment_anz, buf3);
#ifdef CHECKS
	 sprintf(buf2, "     %2d", ch_ptr->u_el.u_xel.u_hdr.ref_counter);
	 strcat (buf1, buf2);
#endif /* CHECKS */
         max_eintr_pro_zeile = 1;
         break; /* Zeige Kette detailiert */
      } /* CASE */

      strcat (buffer, buf1);
      eintr_pro_zeile++;
      if (eintr_pro_zeile == max_eintr_pro_zeile)
      { /* Zeile voll */
         weiter = write_buf (znr++, buffer);
         eintr_pro_zeile = 0;
         strcpy (buffer, "");
      } /* Zeile voll */
      vk_elem_ptr = vk_elem_ptr->u_el.next_action;
   } /* Alle VK-Elemente abarbeiten */
   if (eintr_pro_zeile > 0) 
      write_buf (znr, buffer);
} /* print_chs */



/* ============================ ketten_pro_guete ============================ */
void ketten_pro_guete (INT     anzeige,
                       INT     guete,
                       INT     k_art,
                       GSTATE *erg_state)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr;
  
   /* ketten_pro_guete */
   reset_text_buf ();
   vk_ptr = get_vk (k_art, guete, erg_state);
   if (*erg_state == GLEAM_OK)
      print_chs (vk_ptr, 1, anzeige);
} /* ketten_pro_guete */



/* =============================== show_baks ================================ */
void show_baks (GSTATE *erg_state)
{
   STR_TYPE             buf1;
   CHAIN_ELEM_PTR_TYPE  vk_ptr;

   /* show_baks */
   vk_ptr = get_vk (AK, BASIS_AK_GUETE, erg_state);
   if (*erg_state == GLEAM_OK)
   { /* ok */
      sprintf (buf1, BAKS_HDR1, vk_ptr->u_el.u_xel.u_hdr.chain_length);
      reset_text_buf ();
      write_buf (1, buf1);
      write_buf (2, BAKS_HDR2);
      print_chs (vk_ptr, 3, ZEIGE_BAKS);
   } /* ok */
} /* show_baks */



/* =============================== ak_to_bak ================================ */
GSTATE ak_to_bak (char *erg_buf,
                  INT   guete,
                  INT   lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr, item_ptr;
   GSTATE               res_state = GLEAM_OK;
  
   /* ak_to_bak */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &res_state))
   { /* AK existiert */
      if (!(ak_ptr->u_el.u_xel.u_hdr.chain_status & BASIS_AK))
      { /* AK -> Basis-AK */
         ak_ptr->u_el.u_xel.u_hdr.chain_status |= BASIS_AK;
         if ((res_state |= new_chain_elem (&item_ptr)) == GLEAM_OK)
         { /* Es ist noch Suppe da! */
            item_ptr->u_el.activity = ACTIVITY_ACT;
            item_ptr->u_el.u_xel.u_sub.ref_typ = VK_ITEM_REF;
            item_ptr->u_el.u_xel.u_sub.sub_chain = NULL;
            res_state  = append_sub_chain (item_ptr, ak_ptr);
            res_state |= append_item (get_vk (AK, BASIS_AK_GUETE, &res_state), 
                                      item_ptr);
         } /* Es ist noch Suppe da! */
         if (res_state == GLEAM_OK)
            strcpy (erg_buf, ERLEDIGT_TXT);
         else
            strcpy (erg_buf, FKT_UNVOLLST);
      } /* AK -> Basis-AK */
      else
         strcpy (erg_buf, AK_SCHON_BAK);
   } /* AK existiert */
   else 
      strcpy (erg_buf, NIX_CH);
   return (res_state);
} /* ak_to_bak */ 



/* =============================== bak_to_ak ================================ */
GSTATE bak_to_ak (char *erg_buf,
                  INT   guete,
                  INT   lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   GSTATE               res_state = GLEAM_OK;
  
   /* bak_to_ak */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &res_state))
   { /* AK existiert */
      res_state = do_bak_to_ak (ak_ptr);
      if (res_state == GLEAM_OK)
         strcpy (erg_buf, ERLEDIGT_TXT);
      else
         strcpy (erg_buf, FKT_UNVOLLST);
   } /* AK existiert */
   else 
      strcpy (erg_buf, NIX_CH);
   return (res_state);
} /* bak_to_ak */ 



/* =============================== delete_baks ============================== */
GSTATE delete_baks (void)
{
   STR_TYPE             buffer;
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   GSTATE               res_state = GLEAM_OK;
   INT                  baks_deleted, baks_anz, lfd_nr, guete, ii;
   BOOL                 done;

   /* delete_baks */
   reset_text_buf ();
   write_buf (2, DEL_BAKS_HDR);
   baks_anz = number_of_chains (BASIS_AK_GUETE, BASIS_AK_GUETE, AK, &res_state);
   if (baks_anz > 0) 
   { /* Es gibt BAKs */
      baks_deleted = 0;
      lfd_nr       = 1;
      guete        = BASIS_AK_GUETE;
      for (ii = 1;  (ii <= baks_anz) && (res_state == GLEAM_OK);  ii++) 
      { /* BAK-VK abklappern */
         if (get_chain (&ch_ptr, &lfd_nr, AK, BASIS_AK_GUETE, &guete, 
                        &res_state))
         { /* BAK existiert */
            res_state |= del_chain (&done, AK, ch_ptr->u_el.u_xel.u_hdr.guete, 
                                    ch_ptr->u_el.u_xel.u_hdr.lfd_nr, TRUE);
            if (done)
               baks_deleted++;
         } /* BAK existiert */
      } /* BAK-VK abklappern */
      sprintf (buffer, DEL_BAK_ERG1, baks_anz);
      write_buf (4, buffer);
      sprintf (buffer, DEL_BAK_ERG2, baks_deleted);
      write_buf (5, buffer);
      if (res_state != GLEAM_OK)
         write_buf (7, FKT_UNVOLLST);
   } /* Es gibt BAKs */
   else 
      write_buf (4, NO_BAK_TXT);       
   return (res_state);
} /* delete_baks */



/* ============================== baks_to_aks =============================== */
GSTATE baks_to_aks (void)
{
   STR_TYPE             buffer;
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   GSTATE               res_state = GLEAM_OK;
   INT                  baks_umgew, baks_anz, lfd_nr, guete, ii;

   /* baks_to_aks */
   reset_text_buf ();
   write_buf (2, BAKS2AKS_HDR);
   baks_anz = number_of_chains (BASIS_AK_GUETE, BASIS_AK_GUETE, AK, &res_state);
   if (baks_anz > 0) 
   { /* Es gibt BAKs */
      lfd_nr     = 1;
      guete      = BASIS_AK_GUETE;
      baks_umgew = 0;
      for (ii = 1;  (ii <= baks_anz) && (res_state == GLEAM_OK);  ii++) 
      { /* Alle BAKs */
         if (get_chain (&ch_ptr, &lfd_nr, AK, BASIS_AK_GUETE, &guete, 
                        &res_state))
         {
            res_state |= do_bak_to_ak (ch_ptr);
            if (res_state == GLEAM_OK)
               baks_umgew++;
         }
      } /* Alle BAKs */
      if (baks_anz == baks_umgew)
         sprintf (buffer, BAKS2AKS_OKTXT, baks_anz);
      else
         sprintf (buffer, BAKS2AKS_F_TXT, baks_anz, baks_umgew);
      write_buf (4, buffer);
   } /* Es gibt BAKs */
   else 
      write_buf (4, NO_BAK_TXT);       
   return (res_state);
} /* baks_to_aks */



/* =============================== make_ak_str ============================== */
BOOL make_ak_str (char *buf,
                  INT   guete,
                  INT   lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   GSTATE               res_state = GLEAM_OK;
   BOOL                 erg = TRUE;

   /* make_ak_str */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &res_state))
      sprintf (buf, AK_ANZG_TXT, guete, lfd_nr, 
               ak_ptr->u_el.u_xel.u_hdr.fitness_note);
   else
   {
      sprintf (buf, NIX_AK_TXT, guete, lfd_nr);
      erg = FALSE;
   }
   return (erg);
} /* make_ak_str */



/* ============================== startMeldung ============================== */
void startMeldung (char *vers)
{
   printf ("\n%s", BOX_LINE);
   printf (E_BOX_LINE);
   printf ("##  %s ##\n", PROG_TITLE1);
   printf ("##  %s ##\n", PROG_TITLE2);
   printf (E_BOX_LINE);
   printf ("##  %s ##\n", PROG_TITLE3);
   printf ("##  %s ##\n", PROG_TITLE4);
   printf (E_BOX_LINE);
   printf ("##  %s ##\n", PROG_TITLE5);
   printf ("##  %s ##\n", PROG_TITLE6);
   printf ("##  %s ##\n", PROG_TITLE7);
   printf ("##  %s ##\n", PROG_TITLE8);
   printf ("##  %s ##\n", PROG_TITLE9);
   printf ("##  %s ##\n", PROG_TITLEA);
   printf ("##  %s ##\n", PROG_TITLEB);
   printf (E_BOX_LINE);
   printf ("##  %s ##\n", PROG_TITLEC);
   printf ("##  %s ##\n", PROG_TITLED);
   printf (E_BOX_LINE);
   printf ("##  %s%50s%s\n", VERS_LINE1, vers, VERS_LINE2);
   printf ("%s\n", BOX_LINE);
} /* startMeldung */ 
