/*******************************************************************************
GLEAM/AE                                                                  V1.1.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                      Package: SYStemnahe routinen
                       Module: EXTerne anKOPPLung
 
Package: sys                   File: extkoppl.c                    gcc/g++ 4.8.2
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
 
Der Modul enthaelt Routinen zur Anbindung externer Software. Konkret werden 2 Kopp-
lungen zur Verfuegung gestellt: 

1. Kopplung ("plot_koppl"):
      Kommunikation ueber eine Pipe zum angekoppelten Prozess und ueber Files
      fuer Daten. Aktuell wird darueber der GNUPLOT angebunden.

2. Kopplung ("sim_koppl"):
      Kommunikation ueber drei Pipes mit dem angekoppelten Prozess (stdin, stdout, 
      stderr). Aktuell wird darueber SIMPILOT und der ELDO-Simulator angebunden.

Der Start der abgeforkten Sub-Prozesse wird ueber den Signal-Handler "child_handler"
ueberwacht. Im Fehlerfalle liefern die "init_..."-Routinen FALSE und es erfolgt eine 
Meldung in das Logfile bzw. nach "stderr", wenn es kein Logfile gibt. Die 1. Kopp-
lung wird nur fuer GLEAM_USR-Varianten benoetigt.
 
 
Die Routinen im einzelnen:

void start_child_handler (void);
          Startet den Signalhandler "child_handler". Dieser loescht "plot_ok", wenn 
          das Plottprogramm (derzeit "gnuplot", 1.Kopplung) terminiert und "sim_ok", 
          wenn das Simulatorprogramm (derzeit "simpilot", 2.Kopplung) terminiert.

BOOL init_sim (void);
          Startet den externen Simulator und stellt die Pipes bereit. Wenn alles 
          gut ging, ist die globale Variable "sim_ok" danach TRUE. Diese wird auch
	  als Funktionsergebnis abgeliefert. Wenn das in STARTSIM_FNAME hinterlegte
          Kommando zur Ausfuehrung des externen Simulators nicht aufgerufen werden 
          kann oder die Pipes nicht eingerichtet werden koennen, erfolgt eine fa-
          tale Fehlermeldung.

Routinen der C-Implementierung:

BOOL init_plot (INT       gnu_index,
                STR_TYPE  gnu_param);
          Startet das externe Plot-Programm mit "gnu_param" und stellt die Pipe 
	  in der globalen Variable "pipe_p2g [2] [MAX_GNU]" bereit. Wenn alles 
          gut ging, ist die globale Variable "plot_ok [MAX_GNU]" danach TRUE und
	  das Funktionsergebnis ebenfalls. Gleichzeitig koennen MAX_GNU Plot-
	  Programme aktiv sein. Sie werden durch "gnu_index" unterschieden.

--------------------------------------------------------------------------------
Aenderung gegenueber V1.0.0:
 1. Vereinheitlichung des STARTSIM_FNAME als eine package-lokale Konstante.
 2. Ausblendung von GNU-Plot-Teilen bei nicht gesetztem Schalter GNUPLOT.
 3. Linux-Portierung: Differenzierung zwischen Solaris und Linux. 
    Andere Includes fuer Terminal-I/O                                WJ 11.05.05
 4. "is_command": Deklaration des Parameters "str", der mit String-
    konstanten aufgerufen wird, als "const".                         WJ 19.04.12
 5. "is_command" wird lokale Routine. Streichung von GLEAM_C.        WJ 22.06.12
 6. Vereinfachung von "fmeld" durch Aufruf von ""write_to_log".      WJ 19.12.16
 7. Fatale Fehlermeldungen, wenn das Kommando des externen Simulators
    nicht ausfuehrbar ist oder die Pipes in "init_sim" nicht angelegt 
    werden koennen. Setzen der beiden zu lesenden Pipes auf nicht 
    blockierendes Lesen.
 8. Definition des "_XOPEN_SOURCE_EXTENDED"-Makros.                  WJ 14.03.17
 9. Funktion fmeld() an das Stringhandlicg von ISO C++ angepasst.    WJ 23.08.18
--------------------------------------------------------------------------------
Status: o.k.          Autoren: W.Jakob / K.Lindemann           Datum: 23.08.2018
================================================================================ */
#define _DEFAULT_SOURCE                   /* allows X/Open Unix functionality */
#define _XOPEN_SOURCE 500                 /* allows X/Open Unix functionality */
#define _XOPEN_SOURCE_EXTENDED            /* allows X/Open Unix functionality */

#include "schalter.h"

#ifdef BS_SOLARIS
  #include <sys/types.h>
  #include <sys/time.h>
  #include <sys/filio.h>
#endif /* BS_SOLARIS */
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "fbhm.h"
#include "sysInt.h"
#include "sys.h"

#include SYS_TEXTS



/* ========================== Exportierte Variable: ========================= */
BOOL  sim_ok = FALSE;           /* TRUE, wenn der Simulator laeuft.           */
INT   pipe_p2s [2];             /* Pipe zum Simulator                         */
INT   pipe_s2p [2];             /* Pipe vom Simulator                         */
INT   pipe_se2p[2];             /* Pipe vom Simulator fuer Fehlermeldungen    */
FILE* tsim;                     /* Filedescriptor fuer Sim-Pipe zum Simulator */
FILE* fsim;                     /* Filedescriptor fuer Sim-Pipe vom Simulator */
FILE* efsim;                    /* Filedescriptor fuer Sim-Fehler-Pipe        */
#if defined (GNUPLOT)
  BOOL  plot_ok [MAX_GNU] = {FALSE, FALSE, FALSE, FALSE};
                                /* TRUE, wenn das Plotprogramm laeuft.        */
  INT   pipe_p2g [MAX_GNU] [2]; /* Pipes zur Kommunikation mit dem Plot-Prog  */
#endif /* GNUPLOT */



/* ============================ Lokale Konstante: =========================== */
#define PLOT_PROG_NAME "gnuplot"



/* ======================== Lokale Typdeklarationen: ======================== */



/* ============================ Lokale Variable: ============================ */
static INT    sim_pid,                /* PID of simulator child process       */
              plot_pid [MAX_GNU] =    /* PID of plot child processes          */
                  { 0, 0, 0, 0};



/* ============================ Lokale Routinen: ============================ */
static void child_handler (INT         sig);

static BOOL is_command    (const char *str);

static void fmeld         (const char *fbuf);




/* ============================== child_handler ============================= */
void child_handler (INT  sig)
/*----------------------------------------------------------------------------*/
/*   Signalhandler. Loescht "plot_ok", wenn das Plottprogramm (1.Kopplung)    */
/*   terminiert und "sim_ok", wenn das Simulatorprogramm (2.Kopplung)         */
/*   terminiert.                                                              */
/*----------------------------------------------------------------------------*/
{
   pid_t  pid;
   INT    ii;
 
   /* child_handler */
   pid = wait (0);
   if (pid == sim_pid)
      sim_ok = FALSE;
#if defined (GNUPLOT)
   else
   { /* GNUPLOT pid ? */
      for (ii = 0;  ii < MAX_GNU;  ii++)
	 if (pid == plot_pid [ii])
	    plot_ok [ii] = FALSE;
   } /* GNUPLOT pid ? */
#endif /* GNUPLOT */
} /* child_handler */



/* =========================== start_child_handler ========================== */
void start_child_handler (void)
{
   sigset (SIGCHLD, child_handler);        /* Setup signal handler for childs */
} /* start_child_handler */



/* =============================== is_command =============================== */
static BOOL is_command (const char *str)
/*----------------------------------------------------------------------------*/
/*   Die Routine checkt, ob "str" ein von der Shell aus aufrufbares Kommando  */
/*   ist. Aliases werden nicht erkannt.                                       */
/*----------------------------------------------------------------------------*/
{
   register char *dir,
                 *name,
                 *cp,
                 *tp;
   char           dirbuf  [MAXPATHLEN],
                  namebuf [MAXPATHLEN];
   struct   stat  sbuf;

   /* is_command */
   if (!str || !*str)
      return (0);
   name = namebuf;
   if ((strchr(str, '/')) != NULL) 
   { /* "/" in command, don't look at $PATH */
      if (stat (str, &sbuf) < 0 || (sbuf.st_mode & S_IFMT) != S_IFREG)
	 return (0); 
      if (access (str, X_OK) == 0)  
	 return (1);
      return(0) ; 
   } /* "/" in command, don't look at $PATH */
   else        
   { /* look through $PATH variable */
      if ((tp = getenv ("PATH")) == NULL)                /* no $PATH available */
	 return (0);

      for (;;) 
      { /* Loop */
	 if (*tp == 0)                                        /* end of $PATH */
	    return (0);
	 else
	    if (*tp == ':')
	       tp++;                   /* step to beginning of next directory */
	 if ((cp = strchr(tp, ':')) == NULL)
	    cp = tp + strlen(tp);
	 for (dir = dirbuf; tp < cp;)     /* copy directory from $PATH in dir */
	    *dir++ = *tp++;
	 *dir = 0;
	 strcpy(name, dir = dirbuf);
	 strcat(name, "/");
	 strcat(name, str);
	 if (stat(name, &sbuf) < 0 || (sbuf.st_mode & S_IFMT) != S_IFREG)
	    continue;
	 if (access(name, X_OK) == 0)
	 {
	    if (strcmp(dir, ".") == 0 &&
		(dir = getcwd(dirbuf, MAXPATHLEN)) == NULL)
	       return (0);
	    break;
	 }
      } /* Loop */
   } /* look through $PATH variable */
   return (1);
} /* is_command */



/* ================================== fmeld ================================= */
static void fmeld (const char *fbuf)
/*----------------------------------------------------------------------------*/
/*   Ergaenzt die Meldung in "fbuf" um die zur "errno" gehoerige und durch    */
/*  "strerror" gewonnene Fehlermeldung und schreibt sie in das Logfile,       */
/*   sofern es zur Verfuegung steht. Ausserdem wird mit dem Text eine Fehler- */
/*   meldung (GLEAM_ERROR) erzeugt.                                           */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  fmeld_buf;

   /* fmeld */
   sprintf (fmeld_buf, "*** %s: %s", fbuf, strerror (errno));
   fehler (SYS_EXT_KOPPL, 1, NO_INFO, NO_INFO, &(fmeld_buf[4]), EXT_KOPPL_ERR);
   write_to_log (fmeld_buf, CLOSE_LOG);
} /* fmeld */



/* ================================ init_sim ================================ */
BOOL init_sim (void)
/*----------------------------------------------------------------------------*/
/*  Initialisiert die Anbindung des externen Simulators durch Pipes und durch */
/*  "abforken" eines Subprozesses. Die Datenkommunikation erfolgt ueber       */
/*  Pipes oder assoziierte Files.                                             */
/*                                                                            */
/*  The pipes will be used in the following way:                              */
/*                                                                            */
/*  parent -> pipe_p2s[1]  -----> pipe_p2s[0]  -> sim  (stdin)                */
/*  parent <- pipe_s2p[0]  <----- pipe_s2p[1]  <- sim  (stdout)               */
/*  parent <- pipe_se2p[0] <----- pipe_se2p[1] <- sim  (stderr)               */
/*----------------------------------------------------------------------------*/
{
   INT            ii;
   struct rlimit  rlp;
   STR_TYPE       fbuf;

   /* init_sim */
   sim_ok = FALSE;
   if (is_command (STARTSIM_FNAME))
   { /* Simulator-Kommando bekannt */
      if ((pipe(pipe_p2s)  != 0) ||   /* create data-pipe  from parent to sim */
	  (pipe(pipe_s2p)  != 0) ||   /* create data-pipe  from sim to parent */
	  (pipe(pipe_se2p) != 0))     /* create error-pipe from sim to parent */
      {
	 fatal(SYS_EXT_KOPPL, 2, NO_INFO, NO_INFO, PIPES_ERR_TXT, EXT_SIMU_ERR_TXT);
	 return (FALSE);
      }
      sighold (SIGCHLD);
      switch (sim_pid = fork()) 
      { /* CASE */
	 case -1 :     
	 { /* fork failed */ 
	    sprintf (fbuf, FORK_ERR_TXT, STARTSIM_FNAME);
	    fmeld (fbuf);
	    close (pipe_p2s[0]);
	    close (pipe_p2s[1]);
	    close (pipe_s2p[0]);
	    close (pipe_s2p[1]);
	    close (pipe_se2p[0]);
	    close (pipe_se2p[1]);
	    break;
	 } /* fork failed */ 

	 case  0 :     
	 { /* child, used to run simulator program */
	    dup2 (pipe_p2s[0], 0);                     /* redirect stdin  (0) */
	    dup2 (pipe_s2p[1], 1);                     /* redirect stdout (1) */
	    dup2 (pipe_se2p[1], 2);                    /* redirect stderr (2) */
	    getrlimit (RLIMIT_NOFILE, &rlp);
	    for (ii = rlp.rlim_cur;  ii > 2;  ii--) 
	       (void) close(ii);           /* close all open file descriptors */ 
	    for (ii = 0;  ii < NSIG;  ii++)    /* disable all signal handlers */
	       (void) signal(ii, SIG_DFL); 
         
	    execlp (STARTSIM_FNAME, STARTSIM_FNAME, (char *) 0); 
	    sprintf (fbuf, EXECLP_ERR_TXT, STARTSIM_FNAME);
	    fprintf (stderr, "STDERR: %s: %s\n", fbuf, strerror (errno));
	    _exit(-1);
	 } /* child, used to run simulator program */
      
	 default:       /* parent */
	 sim_ok = TRUE;
	 sigrelse (SIGCHLD);
	 close (pipe_p2s[0]); 
	 close (pipe_s2p[1]); 
	 close (pipe_se2p[1]);
	 sleep (1);
	 break;
      } /* CASE */
      tsim  = fdopen (pipe_p2s[1], "w");
      fsim  = fdopen (pipe_s2p[0], "r");
      efsim = fdopen (pipe_se2p[0], "r");
      if (fcntl(pipe_s2p[0], F_SETFL, O_NONBLOCK) != 0)     /* allow non-     */
	 fmeld(DAT_PIPE_ERR_TXT);
      if (fcntl(pipe_se2p[0], F_SETFL, O_NONBLOCK) != 0)    /* blocking reads */
	 fmeld(ERR_PIPE_ERR_TXT );
   } /* Simulator-Kommando bekannt */
   else
      fatal(SYS_EXT_KOPPL, 3, NO_INFO, NO_INFO, STARTSIM_FNAME, UNKNOWN_EXT_SIMU_COMMAND);
   return (sim_ok);
} /* init_sim */



#if defined (GNUPLOT)
/* ================================ init_plot =============================== */
BOOL init_plot (INT       gnu_index,
                STR_TYPE  gnu_param)
/*----------------------------------------------------------------------------*/
/*  Initialisiert die Anbindung des externen Plot-Programs ueber Pipes und    */
/*  durch "abforken" eines Subprozesses. Die Datenkommunikation erfolgt ueber */
/*  Files.                                                                    */
/*  Die Initialisierung erfolgt nur, wenn "gnuplot" ein bekannter Befehl ist. */
/*  The pipe will be used in the following way:                               */
/*                                                                            */
/*  parent -> pipe_p2g[1] -----> pipe_p2g[0] -> plot                          */
/*----------------------------------------------------------------------------*/
{
   INT            ii;
   struct rlimit  rlp;
   STR_TYPE       fbuf;

   /* init_plot */
   plot_ok [gnu_index] = FALSE;
   if (is_command (PLOT_PROG_NAME))
   { /* "gnuplot" bekannt */
      if (pipe(pipe_p2g [gnu_index]) != 0)       /* create the pipes for plot */
      {
	 fatal(SYS_EXT_KOPPL, 4, NO_INFO, NO_INFO, PIPES_ERR_TXT, EXT_SIMU_ERR_TXT);
	 return (FALSE);
      }
      sighold (SIGCHLD);
      switch (plot_pid [gnu_index] = fork()) 
      {
	 case -1 :     
	 { /* fork failed */ 
	    close (pipe_p2g [gnu_index][0]);
	    close (pipe_p2g [gnu_index][1]);
	    sprintf (fbuf, FORK_ERR_TXT, PLOT_PROG_NAME);
	    fmeld (fbuf);
	    break;
	 }

	 case  0 :     
	 { /* child, used to run plot */
	    dup2 (pipe_p2g [gnu_index][0], 0);          /* redirect stdin (0) */
	    getrlimit (RLIMIT_NOFILE, &rlp);
	    for (ii = rlp.rlim_cur;  ii > 2;  ii--) 
	       (void) close(ii);           /* close all open file descriptors */ 
	    for (ii = 0;  ii < NSIG;  ii++)    /* disable all signal handlers */
	       (void) signal(ii, SIG_DFL); 
	    execlp (PLOT_PROG_NAME, PLOT_PROG_NAME, "-geometry", gnu_param, (char *) 0); 
	    sprintf (fbuf, EXECLP_ERR_TXT, PLOT_PROG_NAME);
	    fprintf (stderr, "STDERR: %s: %s\n", fbuf, strerror (errno));
	    _exit(-1);
	 } /* child, used to run plot */
      
	 default: /* parent */
	 plot_ok [gnu_index] = TRUE;
	 close (pipe_p2g [gnu_index][0]); 
	 sigrelse (SIGCHLD);
	 sleep (1);
	 break;
      } /* CASE */
   } /* "gnuplot" bekannt */
   return (plot_ok [gnu_index]);
} /* init_plot */
#endif /* GNUPLOT */


