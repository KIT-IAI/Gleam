/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Umlaute)   Solaris 5.8 / Sun C 5.1
Package: -                       File: szsunsol.h            Version:     V1.2.0
Status : o.k.                   Autor: W.Jakob                 Datum: 14.04.2009
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Umlaute und andere Sonderzeichen aller unterstuetzten Sprachen fuer eine der
unterstuetzten Zielplattformen.                Sonderzeichen fuer: Sun / Solaris
--------------------------------------------------------------------------------
Aenderungen:
1. Aus "sz" wird "szt" wg dem Parameter "sz" in einer Systemlibrary. WJ 02.04.04
2. Unterscheidung zwischen SUN/Solaris und Linux.                    WJ 14.04.09
-------------------------------------------------------------------------------- */


/* ================================ Deutsch: ================================ */
#ifdef BS_LINUX
#define ue   "\xc3\xbc"  /* UTF-8 Codes */
#define oe   "\xc3\xb6"
#define ae   "\xc3\xa4"
#define szt  "\xc3\x9f"
#define UE   "\xc3\x9c"
#define OE   "\xc3\x96"
#define AE   "\xc3\x84"
#endif

#ifdef BS_SOLARIS
#define ue   "ü"
#define oe   "ö"
#define ae   "ä"
#define szt  "ß"
#define UE   "Ü"
#define OE   "Ö"
#define AE   "Ä"
#endif

#define PROZ "%%"

