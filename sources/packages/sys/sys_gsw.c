/*******************************************************************************
GLEAM/AE                                                                  V1.2.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SYStemnahe routinen
                        Module: SYStemnahe GrundSoftWare
 
Package: sys                    File: sys_gsw.c                    gcc/g++ 4.8.2
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

Der Modul enthaelt verschiedene systemnahe Routinen. Dazu gehoeren insbesondere 
der Zufallszahlengenerator, Zeit- und Datumsfunktionen sowie verschiedene mathe-
matische Routinen. Ausserdem befindet sich hier alle systemnahe Software, die 
nicht in die Module "file_gsw", "ext_koppl" und "term_io" gehoert.


Die Routinen im einzelnen:

void warte (unsigned INT  m_sec);
         Wartet m_sec Millisekunden ohne CPU-Belastung. Bei einem Zeitwert == 0 
         wird nicht gewartet und die Routine terminiert umgehend.

void seed_rand (long int  offset);
         Initialisierung der Zufallszahlengeneratoren mit einem Wert, der aus 
         der Zeit in Sekunden plus einem "offset" gebildet wird. Der Offset 
         dient dazu, gleiche Zufallszahlen zu verhindern, die sonst beim Start 
         mehrerer Laeufe in einer Sekunde oder beim Start paralleler Slaves 
         entstehen koennen. 

INT irand (INT  range);
         Die Routine liefert eine Zufallszahl im Bereich [0..range-1] zurueck.
   
DOUBLE frand (VOID);
         Die Routine berechnet eine Zufallszahl im Bereich [0..1).
   
void get_time_stamp (STR_TYPE  t_stamp,
                     STR_TYPE  datum,
                     STR_TYPE  w_tag,
                     STR_TYPE  uhr_zeit);
         Liefert folgendes ab: in "datum" das Datum im Format "tt.mm.yyyy", in
         "w_tag" den Wochentag in Englisch auf drei Zeichen abgekuerzt und in
         "uhr_zeit" die Uhrzeit im Format "hh:mm:ss". In "t_stamp" wird alles
         im Format "tag, tt.mm.yyyy hh:mm:ss" zusammengefasst (24 Zeichen).
         Als Parameter duerfen auch identische (Dummy-)Puffer dabei sein.

void start_timer (LONG  saved_time);
         Startet einen Timer mit sekunden-genauer Aufloesung. Die gestartete 
         Zeit wird mit "saved_time" Sekunden vorbesetzt. 

LONG calc_elapsed_time (void);
         Liefert die seit dem letzten Aufruf von "start_timer" abgelaufene Zeit
         in Sekunden plus der eventuell beim "start_timer"-Aufruf vorgegebenen 
         "saved_time" Sekunden.

void startTimerMS (void);
         Starts a timer function for time recording in micro seconds.

DOUBLE calcElapsedTimeMS (void);
         Calculates the time elapsed since the last call of startTimerMS() in 
         micro seconds.

DOUBLE d_round (DOUBLE  a);
         Die Routine rundet den Wert in "a" nach der folgenden Formel und lie-
         fert das Ergebnis als DOUBLE zurueck.
         a < 0.0 : result = (DOUBLE)((LONG)(a - 0.5))
         sonst   : result = (DOUBLE)((LONG)(a + 0.5))
         Bei gesetztem BS_Linux wird stattdessen "round" aufgerufen.

LONG l_round (DOUBLE  a);
         Die Routine rundet den Wert in "a" nach der folgenden Formel und lie-
         fert das Ergebnis als LONG zurueck.
         a < 0.0 : result = (LONG) (a - 0.5)
         sonst   : result = (LONG) (a + 0.5)
         Bei gesetztem BS_Linux wird stattdessen "lround" aufgerufen.

INT  i_round (DOUBLE  a);
         Die Routine rundet den Wert in "a" nach der folgenden Formel und lie-
         fert das Ergebnis als INT zurueck.
         a < 0.0 : result = (INT) (a - 0.5)
         sonst   : result = (INT) (a + 0.5)
         Bei gesetztem BS_Linux wird stattdessen "lround" aufgerufen und noch 
         eine Typanpassung nach INT durchgefuehrt.

DOUBLE fsqr (DOUBLE  x);
         Die Routine berechnet das Quadrat einer Realzahl im Format "DOUBLE".
   
LONG isqr (LONG  x);
         Die Routine berechnet das Quadrat einer Integerzahl im Format LONG.

void drei_klang (INT  dauer);
         DOS:
         Erzeugt je nach "art" einen kurzen oder langen Dreiklang, das Fehler-
         geraeusch (Brumm-Ton) oder das Fatalgeraeusch (Absturz mit Aufschlag).
         Bei ungueltiger "art" wird kurz gepiepst.
         Solaris:
         Erzeugt je nach "dauer" mehr oder weniger viele Pieps-Toene.


BOOL sysInit (void);
         Obligatorische Initialsierungsroutine. Liefert FALSE, wenn die durch 
         die globalen Schalter angeforderten Logfilenamen nicht angelegt werden 
         konnten.
   
--------------------------------------------------------------------------------
basiert auf den C-Units: sys_gsw, evo_steu
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Linux-Portierung: Aus "round" wird "d_round", "l_round" und "i_round".
    BS-Schalter: Differenzierung zwischen Solaris und Linux.         WJ 11.05.05
 2. "seed_rand" hinterlaesst die verwendete Zeit in "rand_seed".     WJ 10.11.06
 3. Zufallszahlenerzeugung unter Verwendung des neuen Schalters 
    BS_WINXP_GNU an die Gegebenheiten des GNU-Compilers von Windows 
    angepasst.                                                    WJ/SB 25.07.07
 4. Korrektur des Schalternamens. "seed_rand" aktiviert. "warte" 
    deaktiviert (d.h. kein Warten) fuer WinXP/GNU.                WJ/SB 21.08.07
 5. "frand" liefert DOUBLE statt SINGLE.                             WJ 03.01.08
 6. Entfernung der Schalter und des dazugehoerigen Codes:BS_DOS, 
    BS_WINXP_GNU, SLAVE. GLEAM_C --> TEXT_MODE: In "sysInit".        WJ 25.06.12
 7. Erweiterung von "seed_rand" um einen "offset"-Parameter.         WJ 25.10.12
 8. ExtSimuServ: "taste" wird von Modul "evo_gsw" uebernommen.       WJ 29.08.17
 9. New variable "atProgStart".                                      WJ 06.10.17
10. New timer functions startTimerMS() and calcElapsedTimeMS().      WJ 16.07.18
11. get_time_stamp(): Avoiding buffer overflow of static strings.    WJ 17.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 17.07.2020
================================================================================ */
#include "schalter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <sys/time.h>


#if defined(BS_SOLARIS) || defined(BS_LINUX)
#include <poll.h>
#endif /* BS_SOLARIS oder BS_LINUX */

#include "sys.h"
#include "sysInt.h"




/* ========================== Exportierte Variable: ========================= */
char          *prog_id_buf;        /* Programm-Name, Version und Stand        */
time_t         rand_seed;          /* die Zeit vom Aufruf von "seed_rand"     */
BOOL           taste,              /* TRUE = Unterbrechungswunsch             */
               atProgStart;        /* TRUE until init files first read        */



/* ============================ Lokale Variable: ============================ */
static time_t   start_secs;         /* Startzeit von "start_timer"            */
struct timeval  startTime, endTime; /* for time recording in micro seconds    */




/* ================================= warte: ================================= */
void warte (unsigned INT  m_sec)
{
#if defined(BS_SOLARIS) || defined(BS_LINUX)
   struct pollfd  pfd;
#endif /* Solaris oder Linux */
 
   if (m_sec == 0)
      return;

#if defined(BS_SOLARIS) || defined(BS_LINUX)
   pfd.fd = -1;                             /* wir warten auf keinerlei I/O ! */
   poll (&pfd, (unsigned int)1, m_sec);
#endif /* Solaris oder Linux */
} /* warte */



/* =============================== seed_rand ================================ */
VOID seed_rand (long int  offset)
{
#if defined(BS_SOLARIS) || defined(BS_LINUX)
   srand48 (time (&rand_seed) + offset);
#endif /* Solaris oder Linux */
} /* seed_rand */



/* ================================= irand ================================== */
INT irand (INT  range)
{
   if (range == 0)
      return (0);

#if defined(BS_SOLARIS) || defined(BS_LINUX)
   return ((INT)(drand48() * range));
#endif /* Solaris oder Linux */
} /* irand */ 



/* ================================= frand ================================== */
DOUBLE frand (VOID)
{

#if defined(BS_SOLARIS) || defined(BS_LINUX)
   return (drand48());
#endif /* Solaris oder Linux */
} /* frand */ 



/* ============================= get_time_stamp ============================= */
void get_time_stamp (STR_TYPE  t_stamp,
                     STR_TYPE  datum,
                     STR_TYPE  w_tag,
                     STR_TYPE  uhr_zeit)
{
   char       *buf;
   char        xtag[10], xdatum[30];
   STR_TYPE    loc_buf;
   struct tm  *akt_dattim;
   time_t      akt_secs;

   /* get_time_stamp */
   buf = &(loc_buf[0]);
   time (&akt_secs);
   buf = ctime (&akt_secs);               /* buf: Fri Sep 13 00:00:00 1986\n\ */
   akt_dattim = localtime (&akt_secs);    /*      012345678901234567890123456 */
   sprintf (xdatum, "%02d.%02d.%4d", akt_dattim->tm_mday, akt_dattim->tm_mon + 1,
        akt_dattim->tm_year + 1900);
   strncpy (xtag, buf, 3);
   xtag [3] = '\0';
   strncpy (uhr_zeit, &(buf[11]), 8);
   uhr_zeit [8] = '\0';
   sprintf (loc_buf, "%s, %s %s", xtag, xdatum, uhr_zeit);   /* einige Proc-  */
   strcpy (t_stamp, loc_buf);               /* Params koennten identisch sein */
   strcpy (datum, xdatum);
   strcpy (w_tag, xtag);
} /* get_time_stamp */
  


/* ============================== start_timer =============================== */
void start_timer (LONG  saved_time)
{
   /* start_timer */
   time (&start_secs);
   start_secs -= (time_t)saved_time;
} /* start_timer */ 



/* =========================== calc_elapsed_time ============================ */
LONG calc_elapsed_time (void)
{
   time_t     akt_secs;
  
   /* calc_elapsed_time */
   time (&akt_secs);
   return ((LONG)(akt_secs - start_secs));
} /* calc_elapsed_time */ 



/* ============================== startTimerMS ============================== */
void startTimerMS (void)
{
   gettimeofday (&startTime, NULL);                   /* time in microseconds */
} /* startTimerMS */



/* =========================== calcElapsedTimeMS ============================ */
DOUBLE calcElapsedTimeMS (void)
{
   DOUBLE  usedTime;

   /* calcElapsedTimeMS */
   if (startTime.tv_sec > 0)
   { /* timer was started */
      gettimeofday (&endTime, NULL);                     /* time in microseconds */
      usedTime  = endTime.tv_sec - startTime.tv_sec;
      usedTime += endTime.tv_usec/1000000.0 - startTime.tv_usec/1000000.0;
   } /* timer was started */
   else
      usedTime = 0.0;
   return (usedTime);
} /* calcElapsedTimeMS */



/* ================================ d_round ================================= */
DOUBLE d_round (DOUBLE  a)
{
#ifdef BS_LINUX
   return (round (a));
#else /* kein Linux */
   return ((DOUBLE)((a < 0.0) ? ((LONG)(a-0.5)) : ((LONG) (a+0.5))));
#endif /* kein Linux */
} /* d_round */ 



/* ================================ l_round ================================= */
LONG l_round (DOUBLE  a)
{
#ifdef BS_LINUX
   return (lround (a));
#else /* kein Linux */
   return ((a < 0.0) ? ((LONG)(a-0.5)) : ((LONG) (a+0.5)));
#endif /* kein Linux */
} /* l_round */ 



/* ================================ i_round ================================= */
INT i_round (DOUBLE  a)
{
#ifdef BS_LINUX
   return ((INT)(lround (a)));
#else /* kein Linux */
   return ((a < 0.0) ? ((INT)(a-0.5)) : ((INT) (a+0.5)));
#endif /* kein Linux */
} /* i_round */ 



/* ================================== fsqr ================================== */
DOUBLE fsqr (DOUBLE x)
{
   return (x * x);
} /* fsqr */ 



/* ================================== isqr ================================== */
LONG isqr (LONG  x)
{
   return (x * x);
} /* isqr */ 



/* =============================== drei_klang =============================== */
VOID drei_klang (INT  dauer)
{
   INT  ii, jj, base;

   /* drei_klang */
#if defined(BS_SOLARIS) || defined(BS_LINUX)
   base = 0;
   jj   = 0;
   if ((KURZ_TON < dauer) && (dauer <= FATAL_TON))
   { /* "dauer" ok */
      base = 350;
      jj   =  50;
      if (dauer == FATAL_TON) {
         base = 450;
         jj   =  70;
      }
   } /* "dauer" ok */
   else
      if (FATAL_TON < dauer)
      { /* "dauer" nicht ok */
         dauer =   2;
         base  = 250;
      } /* "dauer" nicht ok */
   for (ii = 0;  ii < dauer;  ii++)
   {
      printf ("%c", (char)7);
      fflush (stdout);
      warte (base - ii*jj);
   }
#endif /* BS_SOLARIS oder BS_LINUX */
} /* drei_klang */ 



/* -------------------------------- sysInit --------------------------------- */
BOOL sysInit (void)
{
#ifdef TEXT_MODE
   set_tty_params ();                            /* Init von Modul "term_io"  */
#endif /* TEXT_MODE */
   taste       = FALSE;  /* i.e. no key was pressed. TRUE, if any key was hit */
   atProgStart = TRUE;   /* TRUE until all initialization files read          */
   if (((logfilename         = (char*)malloc(MAX_STR*sizeof(char))) != NULL) &&
       ((prog_id_buf         = (char*)malloc(MAX_STR*sizeof(char))) != NULL) 
#ifdef MITSIMULOG
       && ((simu_logfilename = (char*)malloc(MAX_STR*sizeof(char))) != NULL)
#endif /* MITSIMULOG */
      )
   { /* Strings ok */
      strcpy (logfilename, "");
#ifdef MITSIMULOG
      strcpy (simu_logfilename, "");
#endif /* MITSIMULOG */
      rand_seed        = 0;
      startTime.tv_sec = 0;                          /* timer not started yet */
      return (TRUE);
   } /* Strings ok */
   return (FALSE);
} /* sysInit */


