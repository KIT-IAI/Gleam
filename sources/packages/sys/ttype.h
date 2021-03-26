/* -----------------------------------------------------------------------------
GLEAM/AE                      Global Header-File                                
Package: sys                     File: ttype.h               Version:      1.3.2
Status : o.k.                   Autor: W.Jakob                 Datum: 08.06.2018
--------------------------------------------------------------------------------
Betriebssysteme und Entwicklungswerkzeuge: 
  Solaris 5.8 / Sun C 4.2 und g++
  Windows/NT  / Borland C++ 5.0
  DOS 6.22    / Borland C++ 4.51
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Aenderungen:
 1. MAX_STR auf 90 erhoeht.                                          WJ 04.09.00
 2. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
 3. Definition von NIX_IDX.                                          WJ 19.08.08
 4. Entfernung der Schalter und dazugehoerigen Deklarationen:
    BS_DOS, BS_WINXP_GNU und PAR_VERS.                               WJ 21.06.12
 5. Einfuehrung von MAX_LONG_INT und dem Inclide von <values.h>.     WJ 25.01.13
 6. Streichung der Definition von "msec".                            WJ 05.12.14
 7. MAX_STR auf 120 erhoeht.                                         WJ 09.04.18
 8. MAX_STRING auf 280 erhoeht, um moegliche Buffer-Overflows, die
    vom Ubuntu 7.3.0c-Compiler gemeldet werden, zu verhindern.       WJ 20.08.18
 9. Small BUF_TYPE added.                                            WJ 16.07.20
-------------------------------------------------------------------------------- */

#include <values.h>


/* ------------------------ Allgemeine Definitionen: ------------------------ */
#ifndef TRUE
  #define TRUE      1
#endif
#ifndef FALSE
  #define FALSE     0
#endif

#ifndef NULL
  #define NULL      0L
#endif

#ifndef VOID
  #define VOID   void
#endif

#define NIX_IDX  -1                         /* Kennung fuer unbenutzten Index */



/* ------------------------- Allgemeine Konstanten: ------------------------- */
#define BEZ_LEN         25          /* Laenge von Bezeichner-Strings          */
#define MAX_STRING     280          /* Stringlaenge f.grosse Puffer           */
#define MAX_STR        120          /* Standard-Stringlaenge                  */
#define MAX_BUF         60          /* Standard-Stringbufferlaenge            */
#define ON               1
#define OFF              0


/* --------------------------- Typ-Deklarationen: --------------------------- */
#define UBYTE    unsigned       char       /*           0..  255       1 Byte */
#define USHORT   unsigned short int        /*           0..65535       2 Byte */
#define SHORT             short int        /*      -32768..32767       2 Byte */
#define BOOL                    INT
#define SINGLE                  float      /* 1.5E-45  .. 3.4E+38      4 Byte */
#define DOUBLE                  double     /* 5.0E-324 .. 1.7E+308     8 Byte */
/* --------------------- Solaris/Linux/WinXP-Spezifika: --------------------- */
#if defined(BS_SOLARIS) || defined(BS_LINUX) 
  #define INT                   int        /* -2147483648..2147483647  4 Byte */
#endif /* Solaris oder Linux */

#if defined(BS_SOLARIS)
  #define LONG                  int        /* -2147483648..2147483647  4 Byte */
  #define MAX_LONG_INT      INT_MAX
#endif /* BS_SOLARIS */
#if defined(BS_LINUX)
  #define LONG             long int        /* 4 oder 8 Byte je nach HW (gcc)  */
  #define MAX_LONG_INT     LONG_MAX
#endif /* BS_LINUX */

typedef char  BUF_TYPE    [MAX_BUF+1];     /* Standard-Textpuffer-String      */
typedef char  STR_TYPE    [MAX_STR+1];     /* Standard-Textpuffer-String      */
typedef char  STRING_TYPE [MAX_STRING+1];  /* Grosser Textpuffer (skip-buffer)*/


/* ---------------------------- Parallel-Version: --------------------------- */

