/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File
Package: chio                    File: chioInt.h                 Version: V1.0.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Package-lokaler Export findet nur in der MPI-Version statt.

--------------------------------------------------------------------------------
Status : o.k.                   Autor: W.Jakob                 Datum: 21.01.2013
-------------------------------------------------------------------------------- */

#undef AKBUF_TST



/* ========================== Exportierte Variable: ========================= */
extern DOUBLE  *akPuffer;           /* aktueller Puffer zur AK-Versendung     */
extern INT      akPufferIdx,        /* Beginn des freie Bereichs im akPuffer  */
                akPufferLen;        /* Laenge des akPuffer                    */



/* ======================== Package-globale Routinen: ======================= */
extern BOOL initMPI_Chio (void);

#ifdef AKBUF_TST
   void belegeEintrag (INT  freeElemIdx,
		       INT  anzahl);
#endif /* AKBUF_TST */
