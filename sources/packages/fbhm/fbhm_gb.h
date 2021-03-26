/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Englisch)
Package: fbhm                    File: fbhm_gb.h             Version:     V1.2.1
Status : o.k.                   Autor: W.Jakob                 Datum: 22.12.2017
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */



/* ------------------------- Texte des Moduls: fbhm ------------------------- */
#define EMPTY_TXT      "Not initialized message"
#define NO_FTEXT_FILE  "No error text file: \"%s\"" 
#define BAD_FTXT_FILE  "File \"%s\" is not a valid error text file"
#define FTXTFILE_ERR1  "Bad error text file: \"%s\" (header)"
#define FTXTFILE_ERR2  "Bad error text file: \"%s\" (body)"
#define FTXT_OVL       "Too manu error message texts. Max = %d"
#define MLDG_TXT       "Message ("
#define ERROR_TXT      "ERROR ("
#define FATAL_TXT      "FATAL ("
#define NIX_TXT        "????? ("
#define SKIP_TXT       "The last %d messages have been ignored!"
#define BAD_MSGNR_TXT  "Unknown error number: %d"
#define MSG_HDR_TXT    "%*s%s %d messages at state \"%s\":\n"
#define PROG_FIN_TXT   "       %s terminates ..."
#define PROG_ENDE_TXT  "    Program %s terminated.\n\n"
#define CRASH_LINE     "---------------------------------"
#define CRASH1_TXT     "             Remaining contents of the message buffer:\n"
#define CRASH2_TXT     "             =========================================\n\n"
#define BAD_MSG_TXT    "Invalid format specification in message #: %ld!"
#define MASTER_TXT     "Master:"
#define SLAVE_TXT      "Sl%04d:"
#define BAD_STATE_TXT  "%s in state %d: No transition to error state possible"
#define LOCAL_WARN_TXT "*** Warning: "
#define LOCAL_FAT_TXT  "***** Fatal: "
#define FATAL_INIT_TXT "***** Fatal initialization error!\n"
