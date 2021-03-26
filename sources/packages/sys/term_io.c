/*******************************************************************************
GLEAM/AE                                                                  V1.1.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SYStemnahe routinen
                        Module: TERMinal-I/O
 
Package: sys                    File: term_io.c                    gcc/g++ 4.8.2
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

Der Modul stellt fuer die TEXT_MODE-Variante Routinen zur Terminal-I/O zur Ver-
fuegung.


Die exportierten Routinen im einzelnen:

void restore_tty_params (void);
        Restauriert die urspruenglichen Eigenschaften der Terminalschnittstelle.

BOOL kb_hit (VOID);
         Die Routine prueft, ob eine Taste des Keyboards gedrueckt wurde. Falls
         dies geschehen ist, ist der Funktionswert "TRUE", sonst FALSE. Unter 
         Solaris wird das Zeichen intern fuer "get_kb_char" zwischengepuffert.
   
char get_kb_char (VOID);
         Die Routine wartet, bis eine Taste des Keyboards gedrueckt wurde und 
         liefert das Zeichen ab. Solaris: Wenn ueber "kb_hit" bereits ein Zeichen 
         eingelesen wurde, wird dies sofort abgeliefert.


Package-lokale Routinen:

void set_tty_params (VOID);
         Unter Solaris und Linux setzt die Routine die Terminalschnittstelle so 
         um, dass ohne Echo direkt gelesen werden kann. Die Verwendung von 
         "kb_hit" und "get_kb_char" setzt den vorherigen Aufruf von 
         "set_tty_params" voraus! Die Routine terminiert bei auftretenden 
         Fehlern nach Ausgabe einer Fehlermeldung das Programm.
    
--------------------------------------------------------------------------------
basiert auf C-Unit: sys_gsw
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Linux-Portierung: Differenzierung zwischen Solaris und Linux. 
    Andere Includes fuer Terminal-I/O                                WJ 11.05.05
 2. Restaurierung der Datei: Entfernung ferhlerhafter Zeichen.       WJ 21.08.07
 3. Entfernung der Schalter und des dazugehoerigen Codes:
    BS_DOS, BS_WINXP_GNU.                                            WJ 22.06.12
 4. Renovierung: Layout-und Doku-Anpassungen.                        WJ 24.05.16
 5. Bessere Fehlermeldungen in set_tty_params() und 
    restore_tty_params().                                            WJ 24.08.18
 6. Error messages in English.                                       WJ 03.10.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 03.10.2020
================================================================================ */
#include "schalter.h"

#include <stdio.h>

#ifdef old  /* ====================================================== */
  #if defined(BS_SOLARIS) || defined(BS_LINUX)
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <fcntl.h>
  #endif /* Solaris oder Linux */

  #ifdef BS_SOLARIS
    #include <sys/filio.h>
    #include <sys/termio.h>
    #include <sys/termios.h>
  #endif /* BS_SOLARIS */

  #ifdef BS_LINUX
    #include <termio.h>
  #endif /* BS_LINUX */
#endif /* old ====================================================== */

#include <stdlib.h>  
#include <string.h>
#include <termios.h>
/* #include <fcntl.h>       */
/* #include <sys/ioctl.h>   */
#include <unistd.h>

#include "sys.h"
#include "sysInt.h"



#if defined(BS_SOLARIS) || defined(BS_LINUX)
/* ---------------------------- lokale Variable: ---------------------------- */
  static char  last_ch;      /* Nur bei Solaris: Zuletzt gelesens Zeichen     */
  static BOOL  ch_da;        /* Nur bei Solaris: TRUE, wenn Zeichen vorhanden */

  #ifdef BS_LINUX
    static struct termios  tios_saved; /* Zur Terminalrestaurierung bei Linux */
  #endif /* BS_LINUX */



/* ---------------------------- Lokale Routinen: ---------------------------- */
  static BOOL read_ch (char *ch);




  #ifdef BS_LINUX
/* =========================== restore_tty_params: ========================== */
    void restore_tty_params (void)
    {
/*     ioctl (0, TCSETSW, &tios_saved);    Terminal-Restaurierung unter Linux */
       if (tcsetattr (0, TCSANOW, &tios_saved) < 0) {
	  perror ("*** restore_tty_params: Error with tcsetattr");
	  exit (1);
       }
    } /* restore_tty_params */
  #endif /* BS_LINUX */



/* ================================ read_ch: ================================ */
  static BOOL read_ch (char *ch)
/*----------------------------------------------------------------------------*/
/*  Liest ein Zeichen in "ch" ein und liefert TRUE, wenn das Zeichen gelesen  */
/*  werden konnte. Bei einem Lesefehler der Routine "read" erfolgt eine       */
/*  Fehlermeldung.                                                            */
/*----------------------------------------------------------------------------*/
  {
     INT  ii;

     /* read_ch */
     ii = read (0, ch, 1);
     if (ii < 0) { 
	perror ("Error with read ");
	return (FALSE);
     }
     else
	return (ii == 1);
  } /* read_ch */
#endif /* Solaris oder Linux */



/* ================================= kb_hit ================================= */
BOOL kb_hit (VOID)
{
  
#ifdef GLEAM_USR
  #ifdef TEXT_MODE
    #if defined(BS_SOLARIS) || defined(BS_LINUX)
      if (!ch_da)
         ch_da = read_ch (&last_ch);
      return (ch_da);
    #else /* kein Solaris oder Linux */
      return (FALSE);                        /* hier fuer andere BS anpassen! */
    #endif /* kein Solaris oder Linux */
  #else /* kein TEXT_MODE, also GUI_MODE */
      return (TRUE);   /* GUI: Unterbrechbar nach Init. u. nach jeder Paarung */
  #endif /* GUI_MODE */
#else /* Kein GLEAM_USR, also GLEAM-(MPI)-Engine */
      return (FALSE);  /* Kommandozeilenversion laesst sich nie unterbrechen! */
#endif /* GLEAM-(MPI)-Engine */
} /* kb_hit */ 



/* ============================== get_kb_char =============================== */
char get_kb_char (VOID)
{
#if defined(BS_SOLARIS) || defined(BS_LINUX)
   char  ch;
   BOOL  kein_ch;

   /* gwt_kb_char */
   if (ch_da)
   { /* Bereits gelesenes Zeichen abliefern */
      ch_da = FALSE;
      return (last_ch);
   } /* Bereits gelesenes Zeichen abliefern */
   else
   { /* Solange lesen bis Zeichen da */
      do
      {
         kein_ch = !(read_ch (&ch));
         warte (10);                                        /* 10 msec warten */
      }
      while (kein_ch);
      return (ch);
   } /* Solange lesen bis Zeichen da */
#endif /* Solaris oder Linux */
} /* get_kb_char */ 



/* ============================= set_tty_params: ============================ */
void set_tty_params (void)
{
   INT             ii;
   struct termios  tios;
  
   /* hole die aktuellen Parameter der seriellen Schnittstelle */
   if((ii = tcgetattr (0, &tios)) < 0)
   {
      perror ("*** set_tty_params: Error with tcgetattr") ;
      fprintf (stderr, "code = %d\n", ii) ;
      exit (1) ;
   } 
   tios_saved = tios;                                    /* save old settings */
  
   tios.c_lflag = (tios.c_lflag & ~ECHO);                 /* ECHO ausschalten */
   tios.c_lflag = (tios.c_lflag & ~ICANON);        /* disable edit-Funktionen */
  
   /* setze minimale Zeichenzahl und Timer fuer Lesen */ 
   tios.c_cc[VMIN]  = 0;
   tios.c_cc[VTIME] = 0;
  
   /* setze jetzt die Parameter der seriellen Schnittstelle */
   if (tcsetattr (0, TCSANOW, &tios) < 0)
   {
      perror ("*** set_tty_params: Error with tcsetattr") ;
      exit (1) ;
   }
  
   ch_da   = FALSE;
   last_ch = '\0';
} /* set_tty_params */



#ifdef old
/* ============================= set_tty_params: ============================ */
   void set_tty_params (void)
   {
  #if defined(BS_SOLARIS) || defined(BS_LINUX)
      INT             ii;
      struct termios  tios;
      char            arg;
  
      /* hole die aktuellen Parameter der seriellen Schnittstelle */
      if((ii = ioctl (0, TCGETS, &tios)) < 0)
      {
	 perror ("Error with ioctl:TCGETS ") ;
	 fprintf (stderr, "code = %d\n", ii) ;
	 exit (1) ;
      } 
      printf ("set_tty_params: Nach ioctl\n");
    #ifdef BS_LINUX
      tios_saved = tios;                                    /* nur fuer Linux */
    #endif /* BS_LINUX */
  
      tios.c_lflag = (tios.c_lflag & ~ECHO);                 /* ECHO ausschalten */
      tios.c_lflag = (tios.c_lflag & ~ICANON);        /* disable edit-Funktionen */
  
      /* setze minimale Zeichenzahl und Timer fuer Lesen */ 
      tios.c_cc[VMIN]  = 0;
      tios.c_cc[VTIME] = 0;
  
      /* setze jetzt die Parameter der seriellen Schnittstelle */
      if (ioctl (0, TCSETSW,&tios) < 0) {
	 perror ("Error with ioctl:TCSETSW") ;
	 exit (1) ;
      }
  
      ch_da   = FALSE;
      last_ch = '\0';
  #endif /* Solaris oder Linux */
   } /* set_tty_params */
#endif /* old */
