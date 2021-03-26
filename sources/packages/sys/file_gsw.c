/*******************************************************************************
GLEAM/AE                                                                  V1.2.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SYStemnahe routinen
                        Module: FILE-GrundSoftWare
 
Package: sys                    File: file_gsw.c                   gcc/g++ 4.8.2
--------------------------------------------------------------------------------
Betriebssysteme und Entwicklungswerkzeuge: 
  Linux Ubuntu 14.04.4 LTS / gcc 4.8.2 und g++
  Solaris 5.8              / Sun C 4.2 und g++
  Linux Suse 9.2           / gcc 3.3.4 und g++
  Windows/NT               / Borland C++ 5.0
  DOS 6.22                 / Borland C++ 4.51
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt verschiedene systemnahe Routinen zum File- und Directory-
Handling.


Die Routinen im einzelnen:

BOOL file_exists (const STR_TYPE  fspec);       
         Liefert TRUE, wenn das durch "fspec" bezeichnete File existiert.

BOOL open_file (FILE       **file_var,
                const char  *fspec,
                const char  *mode);
         Oeffnet das durch "fspec" angegebene File gemaess "mode" und liefert es
         in "file_var" ab. Wenn das gut ging liefert die Routine TRUE. Andern-
         falls FALSE bei Erzeugung einer entsprechenden Fehlermeldung.
         "mode" muss als erstes Zeichen ein "r" oder "w" enthalten, damit im
         Fehlerfalle eine korrekte Meldung erzeugt werden kann.

BOOL print_and_check (FILE       *dest,
                      const char *buffer);
         Versucht maximal MAX_PRINT mal den "buffer" in das File "dest" ohne
         Zeilenwechsel auszugeben. Es wird davon ausgegangen, dass "buffer" durch 
         einen Zeilenwechsel abgeschlossen wird.
         Liefert TRUE bei Erfolg, sonst FALSE.

void write_to_log (const char *buffer,
                   BOOL        keepOpen);
         Sofern das Logfile geoeffnet werden kann, wird "buffer" als eine Zeile 
         hineingeschrieben. Anderfalls wird eine fehlermeldung erzeugt. Je nach 
         "keepOpen" wird das Logfile danach wieder geschlossen oder offen gelassen.

void write_to_simulog (const char *buf,
                       BOOL        keepOpen);
         Bei gesetztem "mit_simulog" wird der Inhalt von "buf" in das  Simu-Logfile
         geschrieben. Ein ungeoeffnetes File wird zuvor geoeffnet. Wenn das nicht 
         geht, wird eine Meldung erzeugt. Je nach "keepOpen" wird das Logfile da-
         nach wieder geschlossen oder offen gelassen.
          
BOOL make_startsim (const char *shell,
                    const char *pack,
                    const char *dir_name,
                    const char *prep_cmd1,
                    const char *prep_cmd2,
                    const char *sim_cmd);
         Erzeugt unter Unix (BS_SOLARIS oder BS_LINUX) das "startsim"-File fuer
         den externen Simulator im aktuellen Directory. Der Inhalt des Files ist
         folgender:
           <shell>                                  falls "shell" nicht leer
           #Initialisierung des externen Simulators
           load-net-package <pack>                  falls "pack" nicht leer
           cd <dir_name>                            falls "dir_name" nicht leer
           <prep_cmd1>                              falls "prep_cmd1" nicht leer
           <prep_cmd2>                              falls "prep_cmd2" nicht leer
           <sim_cmd>
         Die Datei erhaelt Schreib- Lese- und Ausfuehrungsrechte fuer User, 
         Gruppe und Others.
         Die Routine liefert positivenfalls TRUE ab, sonst FALSE.
         Bei anderen Betriebssystemen tut die Routine nichts und liefert 
         FALSE ab.

void get_env_var_name (char *buf);
         Liefert den Inhalt der Konstanten SIM_MOD_ROOT_ENV (Namen der Environ-
         mentvariable fuer den Modellpfad des externen Simulators) in "buf" ab.

BOOL check_dir (const STR_TYPE  dir_name);
         Prueft, ob "dir_name" ein Directory ist.

int  show_files (STR_TYPE        dir_name,
                 const STR_TYPE  extention,
                 BOOL            mit_nr);
         Gibt nach einer Kopfzeile alle Files mit der Extention "extention" im 
         Directory "dir_name" in den Textpuffer von Package "fbhm" ab der ersten
         Zeile aus. Bei gesetztem "mit_nr" wird jedem File-Eintrag eine (zwei-
         stellige) laufende Nummer vorangestellt (Vorbereitung zur Auswahl durch
         Nummer). Das Ergebnis ist die Anzahl der ausgegebenen File-Namen. Eine
         Null bedeutet, dass das Directory keine Files mit der angegbenen 
         "extention" enthaelt. Eine -1 bedeutet, dass das Directory "dir_name"
         nicht existiert (Ausgabe einer Fehlermeldung). 
         Bei einem leeren "dir_name" erfolgt die Suche im aktuellen Directory.
         Besteht "extention" aus dem String "*", werden alle Files im angege-
         Directory angezeigt (Die Extention wird ohne "." angegegeben).
         Wenn der Textpuffer zu klein ist, erfolgt eine Meldung und es werden 
         nur die MAX_BUF_ANZ-1 ersten Directory-Namen eingetragen. 

INT show_sim_mods (STR_TYPE  sim_root_dir,
                   INT       znr,
                   BOOL      mit_nr);
         Gibt nach einer Kopfzeile alle Directory-Namen in "sim_root_dir" in den
         Textpuffer von Package "fbhm" ab Zeile "znr" aus. Bei gesetztem "mit_
         nr" wird jedem Directory-Eintrag eine (zweistellige) laufende Nummer
         vorangestellt (Vorbereitung zur Auswahl durch Nummer). Das Ergebnis ist
         die Anzahl der ausgegebenen Directory-Namen. Wenn der Textpuffer zu 
         klein ist, erfolgt eine Meldung und es werden nur die MAX_BUF_ANZ-1 
         ersten Directory-Namen eingetragen. Wenn "sim_root_dir" kein Directory
         ist, erfolgt eine Fehlermeldung und das Funktionsergebnis ist 0. 

void init_sim_mods (STR_TYPE  sim_root_dir);
         Initialisiert den Parameter "sim_root_dir" mit dem Wert der Environ-
         mentvariable SIM_MOD_ROOT_ENV ("SIM_MOD_ROOT"), sofern diese existiert.
         Ansonsten wird das aktuelle Arbeitsverzeichnis ermittelt und, sofern 
         bekannt, daran SIM_MOD_ROOT_DEF gehaengt. Wenn beide Environmentvariable 
         nicht existieren, wird der Inhalt von SIM_MOD_ROOT_DEF als Default-Wert 
         genommen. 

char *getEnvVar (char *envVarName);
         Liefert den Wert der Environmentvariable "envVarName", sofern die
         Stringvariable von NULL verschieden ist. Sonst NULL.

BOOL init_file_gsw (VOID);
         Legt je nach den Schaltern LOGFILE und MITSIMULOG das entsprechende 
         Logfile an und liefert TRUE, wenn alle angeforderten Logfiles geoeffnet
         werden konnten. Die Filenamen ergeben sich aus den globalen Variablen
         "logfilename" u. "simu_logfilename". Falls sie leere Strings enthalten,
         werden die Defaults aus dem Schalter-File benutzt.

--------------------------------------------------------------------------------
basiert auf den C-Units: sys_gsw, sgsw_ext und mmsw
--------------------------------------------------------------------------------
Aenderung:
 1. "make_startsim": Das File erhaelt die Schreib-, Lese- und Ausfueh-
    rungsrechte auch fuer others. "load-net-package"-Aufruf ersetzt. WJ 30.10.02
 2. Linux-Portierung: Differenzierung zwischen Solaris und Linux.    WJ 10.05.05
 3. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
 4. Korrektur einiger Fehlermeldungsaufrufe wg. Umstellung in fbhm.  WJ 13.08.08
 5. Verlagerung von SIM_MOD_ROOT_ENV ins schalter.h-File.            WJ 28.08.09
 6. Neue Routine "getEnvVar".                                        WJ 22.07.10
 7. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 8. Verschiebung von "mit_simulog" und "write_to_simulog" nach 
    Package "sys", Modul "file_gsw".                                 WJ 20.04.12
 9. Entfernung der Schalter und des dazugehoerigen Codes:
    BS_DOS, BS_WINXP_GNU.                                            WJ 21.06.12
10. "print_and_check" durch ein fflush ergaenzt.                     WJ 24.09.12
11. Renovierung: "print_and_check": Korrektur des printf-Aufrufs. 
    Streichung des "link"-Includes.                                  WJ 18.04.16
12. Vereinfachte Fehlerbehandlung in "open_file".                    WJ 10.10.16
13. Streichung von "mit_logfile", da es immer angelegt wird.         WJ 19.12.16
14. Aenderung von SIM_MOD_ROOT_DEF.                                  WJ 03.03.17
15. "write_to_simulog" um "keep_open" erweitert.                     WJ 13.09.17
16. Kleine Korrektur bei der Ermittlung des Arbeitsverzeichnisses 
    in "show_files" und "init_sim_mods". Die Rechte der Logfiles
    werden allgemein auf RW fuer alle Benutzerkategorien gesetzt.    WJ 09.02.18
17. Beseitigung moeglicher Buffer-Overflows angezeigt vom neuen gcc. WJ 20.08.18
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 20.08.2018
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "schalter.h"

#if defined(BS_SOLARIS) || defined(BS_LINUX)
#include <unistd.h>
#include <sys/stat.h>
#endif /* Solaris oder Linux */

#include "sys.h"
#include "fbhm.h"
#include "sysInt.h"

#include SYS_TEXTS


/* ========================== Exportierte Variable: ========================= */
FILE    *logfile;                       /* Log-File                           */
BOOL     logfile_open;                  /* TRUE, solange das Logfile offen ist*/
char    *logfilename;                   /* Name des Log-Files                 */
#ifdef MITSIMULOG
FILE    *simu_logfile;                  /* Log-File des Simulators            */
BOOL     simu_logfile_open,             /* TRUE, solange Simu-Logfile offen   */
         mit_simu_logfile,              /* TRUE, wenn SimuLogfile angelegt    */
         mit_simulog;                   /* Steuert generell LOGFILE-Ausgaben  */
char    *simu_logfilename;              /* Name des Log-Files fuer Simulator  */
#endif



/* ============================ Lokale Konstante: =========================== */
#define MAX_PRINT           3          /* Druckversuche von "print_and_check" */
#define SIM_MOD_ROOT_DEF    "extSimu"  /* alt: "simu"                         */
#define LOGF_RIGHTS         0666



/* =============================== file_exists ============================== */
BOOL file_exists (const STR_TYPE  fspec)
{
   return (access (fspec, F_OK) == 0);
} /* file_exists */ 



/* =============================== open_file ================================ */
BOOL open_file (FILE       **file_var,
                const char  *fspec,
                const char  *mode)
{
   /* open_file */
   if ((*file_var = fopen (fspec, mode)) == NULL)
   { /* File existiert nicht / laesst sich nicht oeffnen */
      if (mode[0] == 'r')
         fehler (SYS_FILE_GSW, 1, NO_INFO, NO_INFO, fspec, FILE_NOT_EXISTS);
      else
         fehler (SYS_FILE_GSW, 2, NO_INFO, NO_INFO, fspec, FILE_NOT_OPEN);
      return (FALSE);
   } /* File existiert nicht / laesst sich nicht oeffnen */
   return (TRUE);
} /* open_file */



/* ============================= print_and_check ============================ */
BOOL print_and_check (FILE       *dest,
                      const char *buffer)
{
   INT  ires;
   INT  ii = 0;
   
   /* print_and_check */
   do
   {
      ires = fprintf (dest, "%s", buffer);
      ii++;
   }
   while ((ires < 0) && (ii < MAX_PRINT));
   if (ires > 0)
      fflush (dest);
   return (ii < MAX_PRINT);
} /* print_and_check */ 



/* ============================== write_to_log ============================== */
void write_to_log (const char *buffer,
                   BOOL        keepOpen)
{
   if (!logfile_open)
      logfile = fopen(logfilename, "at");
   logfile_open = FALSE;       /* will be set to TRUE, if opened and keepOpen */
   if (logfile != NULL) 
   { /* logile opened */
      fprintf(logfile, "%s\n", buffer);
      if (keepOpen) {
	 fflush(logfile);
	 logfile_open = TRUE;
      }
      else 
	 fclose(logfile);
   } /* logile opened */
   else 
      fehler(SYS_FILE_GSW, 3, NO_INFO, NO_INFO, logfilename, FILE_NOT_OPEN);
} /* write_to_log */ 



#ifdef MITSIMULOG
/* ============================ write_to_simulog ============================ */
void write_to_simulog (const char *buf,
                       BOOL        keepOpen)
{
   if (mit_simulog)
   { /* Logging */
      if (!simu_logfile_open)
	 simu_logfile = fopen(simu_logfilename, "at");
      simu_logfile_open = FALSE;      /* will be TRUE, if opened and keepOpen */
      if (simu_logfile != NULL) 
      { /* file opened */
	 fprintf(simu_logfile, "%s\n", buf);
	 if (keepOpen) {
	    fflush(simu_logfile);
	    simu_logfile_open = TRUE;
	 }
	 else
	    fclose(simu_logfile);
      } /* file opened */
      else /* file could not be opened */
	 meldung(SYS_FILE_GSW, NO_INFO, NO_INFO, simu_logfilename, FILE_NOT_OPEN);
   } /* Logging */
} /* write_to_simulog */
#endif /* MITSIMULOG */



/* ============================== make_startsim ============================= */
BOOL make_startsim (const char *shell,
                    const char *pack,
                    const char *dir_name,
                    const char *prep_cmd1,
                    const char *prep_cmd2,
                    const char *sim_cmd)
{
#if defined(BS_SOLARIS) || defined(BS_LINUX)
   FILE  *txt_file;
   
   /* make_startsim */
   if ((txt_file = fopen (STARTSIM_FNAME, "wt")) != NULL)
   { /* File offen */
      if (strcmp (shell, ""))
         fprintf (txt_file, "%s\n", shell);
      fprintf (txt_file, "#Initialisierung des externen Simulators\n");
      if (strcmp (pack, ""))
         /* fprintf (txt_file, "load-net-package %s\n", pack);  */
         fprintf (txt_file, "source /home/lib/packages/%s/shellenv\n", pack);
      if (strcmp (dir_name, ""))
         fprintf (txt_file, "cd %s\n", dir_name);
      if (strcmp (prep_cmd1, ""))
         fprintf (txt_file, "%s\n", prep_cmd1);
      if (strcmp (prep_cmd2, ""))
         fprintf (txt_file, "%s\n", prep_cmd2);
      fprintf (txt_file, "%s\n", sim_cmd);
      fclose (txt_file);
      chmod (STARTSIM_FNAME, S_IRWXU | S_IRWXG | S_IRWXO);
      return (TRUE);
   } /* File offen */
   else
      return (FALSE);
#else /* weder Solaris noch Linux */
   return (FALSE);
#endif /* weder Solaris noch Linux */
} /* make_startsim */ 



/* ============================ get_env_var_name ============================ */
void get_env_var_name (char *buf)
{
   if (strlen (SIM_MOD_ROOT_ENV) == 0)
      strcpy (buf, " ");
   else
      strcpy (buf, SIM_MOD_ROOT_ENV);
} /* get_env_var_names */



/* ================================ check_dir =============================== */
BOOL check_dir (const STR_TYPE  dir_name)
{
   DIR *dir_ptr;
   
   /* check_dir */
   if ((dir_ptr = opendir (dir_name)) == NULL)
      return (FALSE);
   else
   {
      closedir (dir_ptr);
      return (TRUE);
   }
} /* check_dir */ 



/* =============================== show_files =============================== */
INT  show_files (STRING_TYPE     dir_name,
                 const STR_TYPE  extention,
                 BOOL            mit_nr)
{
   DIR           *dir_ptr;
   struct dirent *dir_entry_ptr;
   STRING_TYPE    buffer;
   STR_TYPE       ext_buf;
   INT            punkt_pos, ii;
   BOOL           mit_ext, file_ok;
   
   /* show_files */
   mit_ext = strcmp (extention, "*") != 0;
   strcpy (ext_buf, extention);
   if (strlen (dir_name) == 0)
   { /* Aktuelles Arbeits-Directory nehmen */
#if defined(BS_SOLARIS) || defined(BS_LINUX)
      if (getcwd(dir_name, MAX_STRING) == NULL)
      { /* Mist */
         fehler (SYS_FILE_GSW, 4, NO_INFO, NO_INFO, "", NO_CWD_ERR);
         return (-1);
      } /* Mist */
#endif /* Solaris oder Linux */
   } /* Aktuelles Arbeits-Directory nehmen */
   dir_ptr = opendir (dir_name);
   if (dir_ptr == NULL)
   { /* "sim_root_dir" ist kein Directory */
      if (strlen (dir_name) == 0)
         strcpy (buffer, " ");                       /* never an empty string */
      else
         strcpy (buffer, dir_name);
      fehler (SYS_FILE_GSW, 5, NO_INFO, NO_INFO, buffer, NO_DIR_ERR);
      return (-1);
   } /* "sim_root_dir" ist kein Directory */
   else
   { /* Dir OK */
      sprintf (buffer, FILE_NAMES_HDR, ext_buf, dir_name);
      write_buf (1, buffer);
      ii = 1;
      while (((dir_entry_ptr = readdir (dir_ptr)) != NULL) &&
             (ii < MAX_BUF_ANZ))
      { /* Dir lesen */
         sprintf (buffer, "%s%s%s", dir_name, DIR_DELIMITER, 
                  dir_entry_ptr->d_name);
         if (opendir (buffer) == NULL)
         { /* File-Name */
            if (mit_ext)
            { /* Extention pruefen */
               punkt_pos = strcspn (dir_entry_ptr->d_name, ".");
               strncpy (buffer, &(dir_entry_ptr->d_name[punkt_pos + 1]),
                        strlen (dir_entry_ptr->d_name) - punkt_pos);
               file_ok = strcmp (buffer, ext_buf) == 0;
            } /* Extention pruefen */
            else
               file_ok = TRUE;
            if (file_ok)
            { /* File mit gesuchter Extention */
               if (mit_nr)
                  sprintf (buffer, "  %2d: %s", ii, dir_entry_ptr->d_name);
               else
                  sprintf (buffer, "  %s", dir_entry_ptr->d_name);
               write_buf (ii+1, buffer);
               ii++;
            } /* File mit gesuchter Extention */
         } /* File-Name */
      } /* Dir lesen */
      closedir (dir_ptr);
      if ((ii == MAX_BUF_ANZ) && (dir_entry_ptr != NULL))
         meldung (SYS_FILE_GSW, MAX_BUF_ANZ-1, NO_INFO, FILES_TXT, SIMU_MODS_OVL);
      return (ii-1);
   } /* Dir OK */
} /* show_files */ 



/* ============================== show_sim_mods ============================= */
INT show_sim_mods (STR_TYPE  sim_root_dir,
                   INT       znr,
                   BOOL      mit_nr)
{
   DIR           *dir_ptr;
   struct dirent *dir_entry_ptr;
   STRING_TYPE    buffer;
   INT            ii;
   
   /* show_sim_mods */
   dir_ptr = opendir (sim_root_dir);
   if (dir_ptr == NULL)
   { /* "sim_root_dir" ist kein Directory */
      sprintf (buffer, "%s ", sim_root_dir);        /* never an empty string! */
      fehler (SYS_FILE_GSW, 6, NO_INFO, NO_INFO, "",  BAD_SIMU_ROOT_DIR1);
      fehler (SYS_FILE_GSW, 7, NO_INFO, NO_INFO, buffer, BAD_SIMU_ROOT_DIR2);
      return (0);
   } /* "sim_root_dir" ist kein Directory */
   else
   { /* Dir OK */
      write_buf (znr, SIM_DIR_HDR);
      ii = 1;
      while (((dir_entry_ptr = readdir (dir_ptr)) != NULL) &&
             (ii < MAX_BUF_ANZ))
      { /* Dir lesen */
         if (dir_entry_ptr->d_name[0] != '.')
         { /* Check for Dir */
            sprintf (buffer, "%s%s%s", sim_root_dir, DIR_DELIMITER,
                     dir_entry_ptr->d_name);
            if (opendir (buffer) != NULL)
            { /* Modell-Directory ausgeben */
               if (mit_nr)
                  sprintf (buffer, "  %2d: %s", ii, dir_entry_ptr->d_name);
               else
                  sprintf (buffer, "  %s", dir_entry_ptr->d_name);
               write_buf (znr+ii, buffer);
               ii++;
            } /* Modell-Directory ausgeben */
         } /* Check for Dir */
      } /* Dir lesen */
      closedir (dir_ptr);
      if ((ii == MAX_BUF_ANZ) && (dir_entry_ptr != NULL))
         meldung (SYS_FILE_GSW, MAX_BUF_ANZ-1, NO_INFO, SIM_MODS_TXT, SIMU_MODS_OVL);
      return (ii-1);
   } /* Dir OK */
} /* show_sim_mods */ 



/* ============================== init_sim_mods ============================= */
void init_sim_mods (STRING_TYPE  sim_root_dir)
{
   STRING_TYPE  buffer;
   char        *var_wert;
   
   /* init_sim_mods */
   if ((var_wert = getenv (SIM_MOD_ROOT_ENV)) == NULL)
   { /* Environment-Variable nicht gesetzt: Default aufbauen */
      if (getcwd(buffer, MAX_STRING) == NULL)
         strcpy (sim_root_dir, SIM_MOD_ROOT_DEF);
      else                               /* PWD-Environment-Variable bekannt: */
         sprintf (sim_root_dir, "%s%s%s", buffer, DIR_DELIMITER, SIM_MOD_ROOT_DEF);
   } /* Environment-Variable nicht gesetzt: Default aufbauen */
   else
      strcpy (sim_root_dir, var_wert);
} /* init_sim_mods */ 



/* ================================ getEnvVar =============================== */
char *getEnvVar (char *envVarName)
{
   if (envVarName != NULL)
      return (getenv (envVarName));
   else
      return (NULL);
} /* getEnvVar */ 



/* ============================== init_file_gsw ============================= */
BOOL init_file_gsw (VOID)
{
   BOOL  result,
         local_mit_simu_logfile = FALSE;
   
   if (!strcmp (logfilename, ""))
      strcpy (logfilename, LOGFILE_NAME);         /* use default logfile name */
   logfile      = fopen (logfilename, LOGFILE_MODE);
   logfile_open = logfile != NULL;
   result       = logfile_open;
#ifdef MITSIMULOG
   if (!strcmp (simu_logfilename, ""))
      strcpy (simu_logfilename, SIMU_LOGFILE_NAME);
   simu_logfile           = fopen (simu_logfilename, SIMU_LOGF_MODE);
   simu_logfile_open      = simu_logfile != NULL;
   mit_simu_logfile       = simu_logfile_open;
   local_mit_simu_logfile = simu_logfile_open;
   result                 = result && simu_logfile_open;
   mit_simulog            = FALSE;
#endif /* kein SimuLogfile */
   if (!result)
      fprintf (stderr, LOG_ERR_TXT, logfile_open, local_mit_simu_logfile);
#if defined(BS_SOLARIS) || defined(BS_LINUX)
   if (logfile_open)
      chmod (logfilename, LOGF_RIGHTS);
  #ifdef MITSIMULOG
   if (simu_logfile_open)
      chmod (simu_logfilename, LOGF_RIGHTS);
  #endif /* MITSIMULOG */
#endif /* Solaris oder Linux */
   return (result);
} /* init_file_gsw */ 
