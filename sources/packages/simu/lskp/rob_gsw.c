/*******************************************************************************
GLEAM/AE                                                                  V1.0.4

			       G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

		       Package: SIMUlator
			Module: ROBotersimulator-GrundSoftWare
 
Package: simu                   File: rob_gsw.c               Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
--------------------------------------------------------------------------------

Basis- und Anzeigeroutinen des Robotersimulators (ohne Animation).


Die Routinen im einzelnen:

void   zeige_start_ziel (INT  znr,
                         BOOL interaktiv);
	  Start und Ziel werden zusammen mit je einer Kopfzeile in Joint-Koordi-
	  naten und als Positionsvektor ab Zeile "znr" in den "Textpuffer aus-
	  gegeben. "interaktiv" ist hier nur aus Kompatibilitaetsgruenden und 
          wuerde das Zeichnen des Roboters steuern.

void   setze_start_ziel_frame (void);
	  Update der Positionen der Frames "anfangsframe" und "zielframe" 
	  ausgehend von "start_joints" bzw. "ziel_joints".

void   zeig_pos_joints (JOINTS  ausg_joints,
			INT    *znr);
	  Anzeige der zu den "ausg_joints" gehoerigen kartesischen Position so-
	  wie der Winkelwerte in 2-3 Zeilen ab "znr" im Textpuffer ("fbhm"). 
	  "znr" wird gepflegt.



void   r500kollicheck (JOINTS  th);

void   ktrwr500 (FRAME  *fout,
		 JOINTS  th);

void   posktrwr500 (VECTOR *pos,
		    JOINTS  th);

void   init_mitsubishi (void);

void   convertvs (char   *vtext,
		  VECTOR *vp);

void   dhmatrixmult (M44MATRIX  mout,
		     M44MATRIX  m1,
		     M44MATRIX  m2);

void   initr500 (ROBOTR500TYP *rrobot);

void   initrobmod (ROBMODTYP *rrobot);

DOUBLE strahlabstand (M44MATRIX  rygreifer,
		      VECTOR    *anfpos,
		      VECTOR    *strahlpos,
		      INT        switch_1);

void   zeigr500 (ROBOTR500TYP *rin,
		 JOINTS        th,
		 M44MATRIX     projm,
		 POINT        *tcpp,
		 INT           switch_1);

void   robmodinit (INT  achszahl);

void   ktrwrobmod (JOINTS    th,
		   INT       achszahl,
		   INT       switch_1);

void   zeigrobmod (M44MATRIX  projm,
		   POINT     *tcpp,
		   INT        achszahl);

BOOL   rob_gsw_init (void);
	  Modulinitialsierung und Anlegen der internen Datenstrukturen. Liefert
	  TRUE, wenn dazu genug Speicherplatz vorhanden ist. Vor Aufruf dieser
	  Routine MUSS "pasromin_init" aufgerufen worden sein.

--------------------------------------------------------------------------------
Basiert auf den LESAK-Units: "simuhelp" und "grafsimu".
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Anpassung von "zeige_start_ziel".                                WJ 10.12.98
 2. IP-Zeiger integriert und auf NULL gesetzt.                       WJ 15.12.98
 3. Linux-Portierung: Aus "round" wird "d_round".                    WJ 12.05.05
 4. Entfernung ueberfluessiger Variable.                             WJ 27.06.12
 5. Renovierung: Streichung des "link"-Includes.                     WJ 18.04.16
 6. Anpassungen an die neue TSK-Datenverwaltung. Uebernahme von 
    "mit_simu_anim" als package-globale Variable in "rob_int.h".     WJ 02.12.16
 7. Anpassung des Aufrufs von "write_to_simu_log".                   WJ 14.09.17
 8. Include von "joints.h" hinzugefuegt. Sreichung ueberfluessiger 
    Includes.                                                        WJ 08.01.18
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 08.01.2018
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "joints.h"
#include "rob_int.h"

#include SIMU_TEXTS



/* ========================== Exportierte Variable: ========================= */


/* ======================== Package-globale Variable: ======================= */
char            *meld_buf;      /* Allgemeiner lokaler Meldungspuffer         */
DOUBLE           bildtakt;      /* Zeitinkrement des Simulators in Sekunden   */
INT              axisanz;       /* ANzahl der Roboterachsen                   */
BOOL             mitr500,       /* Zeigt an, ob mit Mitsubishi-IR oder Modell */
		 kollitest,     /* TRUE, wenn Kollisionstext durchzufuehren   */
		 anschlagtest,  /* TRUE, wenn Anschlagtest durchzufuehren ist */
		 anschlag_ko,   /* TRUE, wenn bei Anschlag Abwertung (N=0)    */
                 mit_simu_anim; /* Mit Animation der Simulat. Nur teilw.impl. */
FRAME           *anfangsframe,  /* Startposition und -orientierung            */
		*zielframe;     /* Zielposition und -orientierung             */

CUBUS           *axiscubusfeld;
ROBOTR500TYP    *robotr500;
ROBMODTYP       *robmod;
AXISDATTYP      *axisdatfeld;
DOUBLE          *sensorposvar;       /* Speichert Positionen                  */
DOUBLE          *anschlag_oben;
DOUBLE          *anschlag_unten;
HINDERNISZEIGER  hindernisanf,
		 hindernis,
		 hindernisloeschen;
IPZEIGER         IPanf      = NULL,
                 IPi        = NULL,
                 IPloeschen = NULL;
M44MATRIX        pm;
MFELDTYP         armfeld;            /* fuers Zeichnen des Robotermodells     */
JOINTS           oldrobotjoints,
		 beschlrobotjoints;  /* Roboterwinkel, die vom Mitsu werden   */
				     /* aktuell in "robotjoints" umgespeichert*/
FRAME           *oldrobotframe;      /* f. Bewertung der Geradeninterpolation */
DOUBLE           sensordrehung;      /* Drehung des Abstangssensors           */
INT              h;
BOOL             kollision,          /* TRUE, wenn Kollision stattgefunden hat*/
		 nichtevo;           /* TRUE = Aufruf ausserhalb Evo-Kontext  */

POINT           *p;                  /* Feld der angezeigten Bahnpunkte       */


/* ============================ Lokale Konstante: =========================== */
#define STRAHLLAENGE   30



/* ======================== Lokale Typdeklarationen: ======================== */



/* ============================ Lokale Variable: ============================ */
static INT     hindachse;          /* Achse, die an ein Hindernis anstiess    */
static DOUBLE  l_ox;               /* fuer "strahlabstand"                    */
static DOUBLE  l_oy;               /* fuer "strahlabstand"                    */
static DOUBLE  l_oz;               /* fuer "strahlabstand"                    */
static DOUBLE  l_px;               /* fuer "strahlabstand"                    */
static DOUBLE  l_py;               /* fuer "strahlabstand"                    */
static DOUBLE  l_pz;               /* fuer "strahlabstand"                    */
static DOUBLE  l_xh;               /* fuer "strahlabstand"                    */
static DOUBLE  l_yh;               /* fuer "strahlabstand"                    */
static DOUBLE  l_zh;               /* fuer "strahlabstand"                    */
static DOUBLE  l_d;                /* fuer "strahlabstand"                    */
static DOUBLE *l_hindernisfeld;    /* fuer "strahlabstand"                    */
static DOUBLE *l_bodenfeld;        /* fuer "strahlabstand"                    */


/* ============================ Lokale Routinen: ============================ */
static void kollicheck        (DOUBLE     xp,
			       DOUBLE     yp,
			       DOUBLE     zp,
			       INT        achse);

static void define_mitsubishi (void);

static void rsinit            (void);

static void makezrotmatrix    (M44MATRIX  rotzmat,
			       DOUBLE     degree,
			       INT        x,
			       INT        z);

static void makeyrotmatrix    (M44MATRIX  rotymat,
			       DOUBLE     degree,
			       INT        x,
			       INT        z);

static void makexrotmatrix    (M44MATRIX  rotxmat,
			       DOUBLE     degree,
			       INT        x,
			       INT        z);

static void dhmatrixtranslz   (M44MATRIX  mout,
			       DOUBLE     zverschiebung);

static void bodentrefftest    (void);

static void dtest             (void);





/* ============================ zeige_start_ziel ============================ */
void zeige_start_ziel (INT  znr,
                       BOOL interaktiv)
{ /* zeige_start_ziel */
   write_buf (znr++, START_POS_TXT);
   zeig_pos_joints (start_joints, &znr);
   write_buf (znr++, ZIEL_POS_TXT);
   zeig_pos_joints (ziel_joints, &znr);
} /* zeige_start_ziel */



/* ========================= setze_start_ziel_frame ========================= */
void setze_start_ziel_frame (void)
{
   if (mitr500)
   { /* Mitsu */
      posktrwr500 (&(anfangsframe->transl), start_joints);
      posktrwr500 (&(zielframe->transl),    ziel_joints);
   } /* Mitsu */
   else
   { /* freidefinierter Roboter */
      ktrwrobmod (start_joints, axisanz, OHNEORI);
      *anfangsframe = *robotframe;
      ktrwrobmod (ziel_joints,  axisanz, OHNEORI);
      *zielframe    = *robotframe;
   } /* freidefinierter Roboter */
} /* setze_start_ziel_frame */



/* ============================ zeig_pos_joints ============================= */
void zeig_pos_joints (JOINTS  ausg_joints,
		      INT    *znr)
{
   STR_TYPE  buf, buf2;
   char      offset [3];
   VECTOR   *pos;
   INT       ii, lim1, 
	     width = 3;
   BOOL      z2    = FALSE;

   /* zeig_pos_joints */
   pos = new_vector (0.0, 0.0, 0.0);
   if (mitr500)
      posktrwr500 (pos, ausg_joints);
   else
   {
      ktrwrobmod (ausg_joints, axisanz, OHNEORI);
      *pos = robotframe->transl;
   }
   sprintf (buf, "%s  %3.1f  %3.1f  %3.1f", POS_TXT, pos->x, pos->y, pos->z);
   write_buf ((*znr)++, buf);                         /* Ausgabe der Position */
   if (axisanz > (MAXAXISANZ / 2))
   { /* 2-zeilige Ausgabe */
      lim1  = MAXAXISANZ / 2;
      width = 6;
      z2    = TRUE;
      strcpy (offset, " ");
   } /* 2-zeilige Ausgabe */
   else
   {
      lim1 = axisanz;
      strcpy (offset, "  ");
   }
   strcpy (buf, JOINTS_TXT);
   for (ii = 0;  ii < lim1;  ii++)
   { /* 1.Zeile */
      sprintf (buf2, "%s%*.1f", offset, width,  ausg_joints[ii] * RADGRAD);
      strcat (buf, buf2);
      if (((ii + 1) % 6) == 0)
	 strcat (buf, " ");
   } /* 1.Zeile */
   write_buf ((*znr)++, buf);       /* Ausgabe der Robot-Jointwerte (1.Zeile) */
   strcpy (buf, OFFSET_TXT);
   if (z2)
   { /* 2.Zeile schreiben */
      for (ii = lim1;  ii < axisanz;  ii++)
      { /* 2.Zeile */
	 sprintf (buf2, "%s%*.1f", offset, width,  ausg_joints[ii] * RADGRAD);
	 strcat (buf, buf2);
	 if (((ii + 1) % (6 + lim1) == 0))
	    strcat (buf, " ");
      } /* 2.Zeile */
      write_buf ((*znr)++, buf);    /* Ausgabe der Robot-Jointwerte (2.Zeile) */
   } /* 2.Zeile schreiben */
} /* zeig_pos_joints */



/* =============================== kollicheck =============================== */
static void kollicheck (DOUBLE  xp,
			DOUBLE  yp,
			DOUBLE  zp,
			INT     achse)
{
   if (((hindernis->daten.hul.x <= xp) && (hindernis->daten.vul.x >= xp) &&
	(hindernis->daten.vul.y <= yp) && (hindernis->daten.hur.y >= yp) &&
	(hindernis->daten.vul.z <= zp) && (hindernis->daten.vol.z >= zp)))
   {
      if (nichtevo)
      {
	 hindachse = achse;
	 if (achse <= MAXAXISANZ)
	    sprintf (meld_buf, ACHS_KOLL_TXT, achse);
	 else
	    strcpy (meld_buf, GREIFER_KOLL_TXT);
	 meldung (SIMU_ROB_GSW, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
	 write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
      }                                       /* SIMU_RZ_MELD bringt nur Text */
      kollision = TRUE;
   }
} /* kollicheck */ 



/* ============================= r500kollicheck ============================= */
void r500kollicheck (JOINTS  th)
{
   INT     achse;
   DOUBLE  s1, c1, s2, c2, s23, c23,
	   a2x, a2y, a2z,
	   a3x, a3y, a3z,
	   tcpx, tcpy, tcpz,
	   r;
  
   /* R500KOLLICHECK/GRAFSIMU */
   if (kollitest)
   {
      hindernis = hindernisanf;
      hindachse = 0;              /* keine Achse an ein Hindernis angestossen */
      s1   = sin (th[0]);
      c1   = cos (th[0]);
      s2   = sin (th[1]);
      c2   = cos (th[1]);
      s23  = sin (th[1] + th[2]);
      c23  = cos (th[1] + th[2]);
      a2x  = c1*s2*22.0 + arobotbase;
      a2y  = s1*s2*22.0 + brobotbase;
      a2z  = c2*22.0 + 25.0 + crobotbase;
      a3x  = c1*(s2*22.0 + s23*16.0) + arobotbase;
      a3y  = s1*(s2*22.0 + s23*16.0) + brobotbase;
      a3z  = c2*22.0 + c23*16.0 + 25.0 + crobotbase;
      r    = s2*22.0 + s23*16.0 + sin (th[1] + th[2] + th[3])*19.0;
      tcpx = c1*r + arobotbase;
      tcpy = s1*r + brobotbase;
      tcpz = c2*22.0 + c23*16.0 + cos (th[1] + th[2] + th[3])*19.0 + 25.0
	     + crobotbase;
      if (hindernisanf != NULL)
      {
	 do
	 {
	    kollicheck (a2x, a2y, a2z, 2);
	    kollicheck (a3x, a3y, a3z, 3);
	    kollicheck (tcpx, tcpy, tcpz, 99);
	    hindernis = hindernis->next;
	 }
	 while (hindernis != NULL);
      }
      /* ----------------- Test auf Kollision mit dem Boden: ---------------- */
      achse = 0;
      if (a2z <= 0.0)
	 achse = 2;
      if (a3z <= 0.0)
	 achse = 3;
      if (tcpz <= 0.0)
	 achse = 99;
      if (achse > 0)
      {
	 if (nichtevo)
	 {
	    hindachse = achse;
	    if (achse <= MAXAXISANZ)
	       sprintf (meld_buf, ACHS_KOLL_TXT, achse);
	    else
	       strcpy (meld_buf, GREIFER_KOLL_TXT);
	    meldung (SIMU_ROB_GSW, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
	    write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
	 }                                    /* SIMU_RZ_MELD bringt nur Text */
	 kollision = TRUE;
      }
   }
} /* r500kollicheck */ 



/* ================================ ktrwr500 ================================ */
void ktrwr500 (FRAME  *fout,
	       JOINTS  th)
{
   typedef DOUBLE  T_F [9];
  
   DOUBLE  s1, s2, s23, s234, s5,
	   c1, c2, c23, c234, c5, r;
   INT     i;
   T_F     f;
  
   /* KTRWR500/GRAFSIMU */
   s1   = sin (th[0]);
   c1   = cos (th[0]);
   s2   = sin (th[1]);
   c2   = cos (th[1]);
   s23  = sin (th[1] + th[2]);
   c23  = cos (th[1] + th[2]);
   s234 = sin (th[1] + th[2] + th[3]);
   c234 = cos (th[1] + th[2] + th[3]);
   s5   = sin (th[4]);
   c5   = cos (th[4]);
   r    = s2*22.0 + s23*16.0 + s234*19.0;
   fout->transl.x = c1*r + arobotbase;
   fout->transl.y = s1*r + brobotbase;
   fout->transl.z = c2*22.0 + c23*16.0 + c234*19.0 + 25.0 + crobotbase;
   f[0] = c1*c234*c5-s1*s5;
   f[1] = s1*c234*c5 + c1*s5;
   f[2] = -s234*c5;
   f[3] = -c1*c234*s5-s1*c5;
   f[4] = -s1*c234*s5 + c1*c5;
   f[5] = s234*s5;
   f[6] = c1*s234;
   f[7] = s1*s234;
   f[8] = c234;
   for (i = 0;  i < 9;  i++)
   {
      if (fabs (f[i]) < 0.0000001)
	 f[i] = 0.0;
      if (f[i] > 0.999999)
	 f[i] = 1.0;
      if (f[i] < -0.999999)
	 f[i] = -1.0;
   }
   fout->rot.matrix.t.x = f[0];
   fout->rot.matrix.t.y = f[1];
   fout->rot.matrix.t.z = f[2];
   fout->rot.matrix.o.x = f[3];
   fout->rot.matrix.o.y = f[4];
   fout->rot.matrix.o.z = f[5];
   fout->rot.matrix.a.x = f[6];
   fout->rot.matrix.a.y = f[7];
   fout->rot.matrix.a.z = f[8];
  
   /*  OHNE Orientierung !!!  */
   fout->rot.angle  = 0.0;
   fout->rot.axis.x = 0.0;
   fout->rot.axis.y = 0.0;
   fout->rot.axis.z = 1.0;
} /* ktrwr500 */ 



/* ============================== posktrwr500 =============================== */
void posktrwr500 (VECTOR *pos,
		  JOINTS  th)
{
   DOUBLE  r;
  
   /* POSKTRWR500/GRAFSIMU */
   r = sin (th[1])*22.0 + sin (th[1] + th[2])*16.0 +
       sin (th[1] + th[2] + th[3])*19.0;
   pos->x = cos (th[0])*r + arobotbase;
   pos->y = sin (th[0])*r + brobotbase;
   pos->
   z = cos (th[1])*22.0 + cos (th[1] + th[2])*16.0 +
       cos (th[1] + th[2] + th[3])*19.0 + 25.0 + crobotbase;
} /* posktrwr500 */ 



/* ============================ init_mitsubishi ============================= */
void init_mitsubishi (void)
{
   INT   ii;
  
   /* INIT_MITSUBISHI/SIMUHELP */
   define_mitsubishi ();
   for (ii = 0;  ii < 5;  ii++)
      robotjoints[ii] = start_joints[ii];
   posktrwr500 (&(robotframe->transl), robotjoints);
} /* init_mitsubishi */ 



/* =========================== define_mitsubishi ============================ */
static void define_mitsubishi (void)
{ /* DEFINE_MITSUBISHI/GRAFSIMU */
   mitr500 = TRUE;
   axisanz = 5;    /* Anschlaege des Mitsubishi setzen : */
   anschlag_oben [0] =  150.0*GRADRAD;
   anschlag_unten[0] = -150.0*GRADRAD;
   anschlag_oben [1] =  120.0*GRADRAD;
   anschlag_unten[1] =  -10.0*GRADRAD;
   anschlag_oben [2] =  110.0*GRADRAD;
   anschlag_unten[2] =    0.0;
   anschlag_oben [3] =   90.0*GRADRAD;
   anschlag_unten[3] =  -90.0*GRADRAD;
   anschlag_oben [4] =  180.0*GRADRAD;
   anschlag_unten[4] = -180.0*GRADRAD;
} /* define_mitsubishi */ 



/* ================================= rsinit ================================= */
static void rsinit (void)
{
   INT  ii;
  
   /* RSINIT/GRAFSIMU */
   for (ii = 0;  ii < MAXAXISANZ;  ii++)
   {
      robotjoints[ii]       = 0.0;
      oldrobotjoints[ii]    = 0.0;
      beschlrobotjoints[ii] = 0.0;
      sensorposvar[ii]      = 0.0;
   }
   *oldrobotframe = *robotframe;
   hindachse      = 0;             /* keine Achse an ein Hindernis angestossen */
   sensordrehung  = 0.0;
   kollision = FALSE;
   for (ii = 0;  ii < MAXAXISANZ;  ii++)
      sensorposvar[ii] = 0.0;
   hindernisanf  = NULL;                 /* Initialisieren der Hinderniskette */
#ifdef nix                               /* TSK-File wird vorher gelesen.  WJ */
   define_mitsubishi ();
   if (MAXAXISANZ > 5)
      for (ii = 5;  ii < MAXAXISANZ; ii++)
      {
	 anschlag_oben[ii]  =  160.0*GRADRAD;     /* Anschlaege Robotermodell */
	 anschlag_unten[ii] = -160.0*GRADRAD;
      }
#endif
} /* rsinit */ 



/* =============================== convertvs ================================ */
void convertvs (char   *vtext,
		VECTOR *vp)
{
   sprintf (vtext, "%4.1lf  %4.1lf  %4.1lf", vp->x, vp->y, vp->z);
} /* convertvs */ 



/* ============================== dhmatrixmult ============================== */
void dhmatrixmult (M44MATRIX  mout,
		   M44MATRIX  m1,
		   M44MATRIX  m2)
{
   INT  i, j;
  
   /* DHMATRIXMULT/GRAFSIMU */
   for (i = 0;  i < 3;  i++)
   {
      for (j = 0;  j < 4;  j++)
	 mout[i][j] = m1[i][0]*m2[0][j] + m1[i][1]*m2[1][j] + m1[i][2]*m2[2][j];
      mout[i][3] = mout[i][3] + m1[i][3];
   }
   mout[3][0] = 0.0;
   mout[3][1] = 0.0;
   mout[3][2] = 0.0;
   mout[3][3] = 1.0;
} /* dhmatrixmult */ 



/* ============================= makezrotmatrix ============================= */
static void makezrotmatrix (M44MATRIX rotzmat,
			    DOUBLE    degree,
			    INT       x,
			    INT       z)
{ /* MAKEZROTMATRIX/GRAFSIMU */
   rotzmat[0][0] = cos (degree);
   rotzmat[0][1] = -(sin (degree));
   rotzmat[0][2] = 0.0;
   rotzmat[0][3] = (DOUBLE)x;
   rotzmat[1][0] = -rotzmat[0][1];
   rotzmat[1][1] = rotzmat[0][0];
   rotzmat[1][2] = 0.0;
   rotzmat[1][3] = 0.0;
   rotzmat[2][0] = 0.0;
   rotzmat[2][1] = 0.0;
   rotzmat[2][2] = 1.0;
   rotzmat[2][3] = (DOUBLE)z;
   rotzmat[3][0] = 0.0;
   rotzmat[3][1] = 0.0;
   rotzmat[3][2] = 0.0;
   rotzmat[3][3] = 1.0;
} /* makezrotmatrix */ 



/* ============================ makeyrotmatrix ============================= */
static void makeyrotmatrix (M44MATRIX  rotymat,
			    DOUBLE     degree,
			    INT        x,
			    INT        z)
{ /* MAKEYROTMATRIX/GRAFSIMU */
   rotymat[0][0] = cos (degree);
   rotymat[0][1] = 0.0;
   rotymat[0][2] = sin (degree);
   rotymat[0][3] = (DOUBLE)x;
   rotymat[1][0] = 0.0;
   rotymat[1][1] = 1.0;
   rotymat[1][2] = 0.0;
   rotymat[1][3] = 0.0;
   rotymat[2][0] = -rotymat[0][2];
   rotymat[2][1] = 0.0;
   rotymat[2][2] = rotymat[0][0];
   rotymat[2][3] = (DOUBLE)z;
   rotymat[3][0] = 0.0;
   rotymat[3][1] = 0.0;
   rotymat[3][2] = 0.0;
   rotymat[3][3] = 1.0;
} /* makeyrotmatrix */ 



/* ============================= makexrotmatrix ============================= */
static void makexrotmatrix (M44MATRIX  rotxmat,
			    DOUBLE     degree,
			    INT        x,
			    INT        z)
{ /* MAKEXROTMATRIX/GRAFSIMU */
   rotxmat[0][0] = 1.0;
   rotxmat[0][1] = 0.0;
   rotxmat[0][2] = 0.0;
   rotxmat[0][3] = (DOUBLE)x;
   rotxmat[1][0] = 0.0;
   rotxmat[1][1] = cos (degree);
   rotxmat[1][2] = sin (degree);
   rotxmat[1][3] = 0.0;
   rotxmat[2][0] = 0.0;
   rotxmat[2][1] = -rotxmat[1][2];
   rotxmat[2][2] = rotxmat[1][1];
   rotxmat[2][3] = (DOUBLE)z;
   rotxmat[3][0] = 0.0;
   rotxmat[3][1] = 0.0;
   rotxmat[3][2] = 0.0;
   rotxmat[3][3] = 1.0;
} /* makexrotmatrix */ 



/* ============================ dhmatrixtranslz ============================= */
static void dhmatrixtranslz (M44MATRIX  mout,
			     DOUBLE     zverschiebung)
{ /* DHMATRIXTRANSLZ/GRAFSIMU */
   mout[0][3] = mout[0][2]*zverschiebung + mout[0][3];
   mout[1][3] = mout[1][2]*zverschiebung + mout[1][3];
   mout[2][3] = mout[2][2]*zverschiebung + mout[2][3];
} /* dhmatrixtranslz */ 



/* ================================ initr500 ================================ */
void initr500 (ROBOTR500TYP *rrobot)
{ /* INITR500/GRAFSIMU */
   rrobot->sockel.vul.x =  10.0;
   rrobot->sockel.vul.y = -10.0;
   rrobot->sockel.vul.z =   0.0;
  
   rrobot->sockel.vur.x =  10.0;
   rrobot->sockel.vur.y =  10.0;
   rrobot->sockel.vur.z =   0.0;
  
   rrobot->sockel.hur.x = -10.0;
   rrobot->sockel.hur.y =  10.0;
   rrobot->sockel.hur.z =   0.0;
  
   rrobot->sockel.hul.x = -10.0;
   rrobot->sockel.hul.y = -10.0;
   rrobot->sockel.hul.z =   0.0;
  
   rrobot->sockel.vol.x =  10.0;
   rrobot->sockel.vol.y = -10.0;
   rrobot->sockel.vol.z =  15.0;
  
   rrobot->sockel.vor.x =  10.0;
   rrobot->sockel.vor.y =  10.0;
   rrobot->sockel.vor.z =  15.0;
  
   rrobot->sockel.hor.x = -10.0;
   rrobot->sockel.hor.y =  10.0;
   rrobot->sockel.hor.z =  15.0;
  
   rrobot->sockel.hol.x = -10.0;
   rrobot->sockel.hol.y = -10.0;
   rrobot->sockel.hol.z =  15.0;
  
   rrobot->zwstck1.vul.x =  5.0;
   rrobot->zwstck1.vul.y = -5.0;
   rrobot->zwstck1.vul.z = 15.0;
  
   rrobot->zwstck1.vur.x =  5.0;
   rrobot->zwstck1.vur.y =  5.0;
   rrobot->zwstck1.vur.z = 15.0;
  
   rrobot->zwstck1.hur.x = -5.0;
   rrobot->zwstck1.hur.y =  5.0;
   rrobot->zwstck1.hur.z = 15.0;
  
   rrobot->zwstck1.hul.x = -5.0;
   rrobot->zwstck1.hul.y = -5.0;
   rrobot->zwstck1.hul.z = 15.0;
  
   rrobot->zwstck1.vol.x =  5.0;
   rrobot->zwstck1.vol.y = -5.0;
   rrobot->zwstck1.vol.z = 20.0;
  
   rrobot->zwstck1.vor.x =  5.0;
   rrobot->zwstck1.vor.y =  5.0;
   rrobot->zwstck1.vor.z = 20.0;
  
   rrobot->zwstck1.hor.x = -5.0;
   rrobot->zwstck1.hor.y =  5.0;
   rrobot->zwstck1.hor.z = 20.0;
  
   rrobot->zwstck1.hol.x = -5.0;
   rrobot->zwstck1.hol.y = -5.0;
   rrobot->zwstck1.hol.z = 20.0;
  
   rrobot->oberstck.vul.x =   8.0;
   rrobot->oberstck.vul.y =  -8.0;
   rrobot->oberstck.vul.z =   0.0;
  
   rrobot->oberstck.vur.x =   8.0;
   rrobot->oberstck.vur.y =   8.0;
   rrobot->oberstck.vur.z =   0.0;
  
   rrobot->oberstck.hur.x = -15.0;
   rrobot->oberstck.hur.y =   8.0;
   rrobot->oberstck.hur.z =   0.0;
  
   rrobot->oberstck.hul.x = -15.0;
   rrobot->oberstck.hul.y =  -8.0;
   rrobot->oberstck.hul.z =   0.0;
  
   rrobot->oberstck.vol.x =   8.0;
   rrobot->oberstck.vol.y =  -8.0;
   rrobot->oberstck.vol.z =  12.0;
  
   rrobot->oberstck.vor.x =   8.0;
   rrobot->oberstck.vor.y =   8.0;
   rrobot->oberstck.vor.z =  12.0;
  
   rrobot->oberstck.hor.x = -15.0;
   rrobot->oberstck.hor.y =   8.0;
   rrobot->oberstck.hor.z =   6.0;
  
   rrobot->oberstck.hol.x = -15.0;
   rrobot->oberstck.hol.y =  -8.0;
   rrobot->oberstck.hol.z =   6.0;
  
   rrobot->arm1.vul.x =  3.0;
   rrobot->arm1.vul.y = -4.0;
   rrobot->arm1.vul.z =  0.0;
  
   rrobot->arm1.vur.x =  3.0;
   rrobot->arm1.vur.y =  4.0;
   rrobot->arm1.vur.z =  0.0;
  
   rrobot->arm1.hur.x = -3.0;
   rrobot->arm1.hur.y =  4.0;
   rrobot->arm1.hur.z =  0.0;
  
   rrobot->arm1.hul.x = -3.0;
   rrobot->arm1.hul.y = -4.0;
   rrobot->arm1.hul.z =  0.0;
  
   rrobot->arm1.vol.x =  3.0;
   rrobot->arm1.vol.y = -4.0;
   rrobot->arm1.vol.z = 22.0;
  
   rrobot->arm1.vor.x =  3.0;
   rrobot->arm1.vor.y =  4.0;
   rrobot->arm1.vor.z = 22.0;
  
   rrobot->arm1.hor.x = -3.0;
   rrobot->arm1.hor.y =  4.0;
   rrobot->arm1.hor.z = 22.0;
  
   rrobot->arm1.hol.x = -3.0;
   rrobot->arm1.hol.y = -4.0;
   rrobot->arm1.hol.z = 22.0;
  
   rrobot->arm2.vul.x =  3.0;
   rrobot->arm2.vul.y = -4.0;
   rrobot->arm2.vul.z =  0.0;
  
   rrobot->arm2.vur.x =  3.0;
   rrobot->arm2.vur.y =  4.0;
   rrobot->arm2.vur.z =  0.0;
  
   rrobot->arm2.hur.x = -3.0;
   rrobot->arm2.hur.y =  4.0;
   rrobot->arm2.hur.z =  0.0;
  
   rrobot->arm2.hul.x = -3.0;
   rrobot->arm2.hul.y = -4.0;
   rrobot->arm2.hul.z =  0.0;
  
   rrobot->arm2.vol.x =  3.0;
   rrobot->arm2.vol.y = -4.0;
   rrobot->arm2.vol.z = 16.0;
  
   rrobot->arm2.vor.x =  3.0;
   rrobot->arm2.vor.y =  4.0;
   rrobot->arm2.vor.z = 16.0;
  
   rrobot->arm2.hor.x = -3.0;
   rrobot->arm2.hor.y =  4.0;
   rrobot->arm2.hor.z = 16.0;
  
   rrobot->arm2.hol.x = -3.0;
   rrobot->arm2.hol.y = -4.0;
   rrobot->arm2.hol.z = 16.0;
  
   rrobot->zwstck2.vul.x =  2.0;
   rrobot->zwstck2.vul.y = -2.0;
   rrobot->zwstck2.vul.z =  0.0;
  
   rrobot->zwstck2.vur.x =  2.0;
   rrobot->zwstck2.vur.y =  2.0;
   rrobot->zwstck2.vur.z =  0.0;
  
   rrobot->zwstck2.hur.x = -2.0;
   rrobot->zwstck2.hur.y =  2.0;
   rrobot->zwstck2.hur.z =  0.0;
  
   rrobot->zwstck2.hul.x = -2.0;
   rrobot->zwstck2.hul.y = -2.0;
   rrobot->zwstck2.hul.z =  0.0;
  
   rrobot->zwstck2.vol.x =  2.0;
   rrobot->zwstck2.vol.y = -2.0;
   rrobot->zwstck2.vol.z =  9.0;
  
   rrobot->zwstck2.vor.x =  2.0;
   rrobot->zwstck2.vor.y =  2.0;
   rrobot->zwstck2.vor.z =  9.0;
  
   rrobot->zwstck2.hor.x = -2.0;
   rrobot->zwstck2.hor.y =  2.0;
   rrobot->zwstck2.hor.z =  9.0;
  
   rrobot->zwstck2.hol.x = -2.0;
   rrobot->zwstck2.hol.y = -2.0;
   rrobot->zwstck2.hol.z =  9.0;
  
   rrobot->greifer.vul.x =  2.5;
   rrobot->greifer.vul.y = -4.0;
   rrobot->greifer.vul.z =  9.0;
  
   rrobot->greifer.vur.x =  2.5;
   rrobot->greifer.vur.y =  4.0;
   rrobot->greifer.vur.z =  9.0;
  
   rrobot->greifer.hur.x = -2.5;
   rrobot->greifer.hur.y =  4.0;
   rrobot->greifer.hur.z =  9.0;
  
   rrobot->greifer.hul.x = -2.5;
   rrobot->greifer.hul.y = -4.0;
   rrobot->greifer.hul.z =  9.0;
  
   rrobot->greifer.vol.x =  2.5;
   rrobot->greifer.vol.y = -4.0;
   rrobot->greifer.vol.z = 13.0;
  
   rrobot->greifer.vor.x =  2.5;
   rrobot->greifer.vor.y =  4.0;
   rrobot->greifer.vor.z = 13.0;
  
   rrobot->greifer.hor.x = -2.5;
   rrobot->greifer.hor.y =  4.0;
   rrobot->greifer.hor.z = 13.0;
  
   rrobot->greifer.hol.x = -2.5;
   rrobot->greifer.hol.y = -4.0;
   rrobot->greifer.hol.z = 13.0;
  
   rrobot->grleft.vul.x =  2.0;
   rrobot->grleft.vul.y =  4.0;
   rrobot->grleft.vul.z = 13.0;
  
   rrobot->grleft.vur.x =  2.0;
   rrobot->grleft.vur.y =  2.0;
   rrobot->grleft.vur.z = 13.0;
  
   rrobot->grleft.hur.x = -2.0;
   rrobot->grleft.hur.y =  2.0;
   rrobot->grleft.hur.z = 13.0;
  
   rrobot->grleft.hul.x = -2.0;
   rrobot->grleft.hul.y =  4.0;
   rrobot->grleft.hul.z = 13.0;
  
   rrobot->grleft.vol.x =  2.0;
   rrobot->grleft.vol.y =  3.0;
   rrobot->grleft.vol.z = 19.0;
  
   rrobot->grleft.vor.x =  2.0;
   rrobot->grleft.vor.y =  2.0;
   rrobot->grleft.vor.z = 19.0;
  
   rrobot->grleft.hor.x = -2.0;
   rrobot->grleft.hor.y =  2.0;
   rrobot->grleft.hor.z = 19.0;
  
   rrobot->grleft.hol.x = -2.0;
   rrobot->grleft.hol.y =  3.0;
   rrobot->grleft.hol.z = 19.0;
  
   rrobot->grright.vul.x =  2.0;
   rrobot->grright.vul.y = -2.0;
   rrobot->grright.vul.z = 13.0;
  
   rrobot->grright.vur.x =  2.0;
   rrobot->grright.vur.y = -4.0;
   rrobot->grright.vur.z = 13.0;
  
   rrobot->grright.hur.x = -2.0;
   rrobot->grright.hur.y = -4.0;
   rrobot->grright.hur.z = 13.0;
  
   rrobot->grright.hul.x = -2.0;
   rrobot->grright.hul.y = -2.0;
   rrobot->grright.hul.z = 13.0;
  
   rrobot->grright.vol.x =  2.0;
   rrobot->grright.vol.y = -2.0;
   rrobot->grright.vol.z = 19.0;
  
   rrobot->grright.vor.x =  2.0;
   rrobot->grright.vor.y = -3.0;
   rrobot->grright.vor.z = 19.0;
  
   rrobot->grright.hor.x = -2.0;
   rrobot->grright.hor.y = -3.0;
   rrobot->grright.hor.z = 19.0;
  
   rrobot->grright.hol.x = -2.0;
   rrobot->grright.hol.y = -2.0;
   rrobot->grright.hol.z = 19.0;
} /* initr500 */ 



/* =============================== initrobmod =============================== */
void initrobmod (ROBMODTYP *rrobot)
{
   /* INITROBMOD/GRAFSIMU */
   rrobot->staender.vul.x =  5.0;
   rrobot->staender.vul.y = -5.0;
   rrobot->staender.vul.z =  0.0;
  
   rrobot->staender.vur.x =  5.0;
   rrobot->staender.vur.y =  5.0;
   rrobot->staender.vur.z =  0.0;
  
   rrobot->staender.hur.x = -5.0;
   rrobot->staender.hur.y =  5.0;
   rrobot->staender.hur.z =  0.0;
  
   rrobot->staender.hul.x = -5.0;
   rrobot->staender.hul.y = -5.0;
   rrobot->staender.hul.z =  0.0;
  
   rrobot->staender.vol.x =  5.0;
   rrobot->staender.vol.y = -5.0;
   rrobot->staender.vol.z =  7.0;
  
   rrobot->staender.vor.x =  5.0;
   rrobot->staender.vor.y =  5.0;
   rrobot->staender.vor.z =  7.0;
  
   rrobot->staender.hor.x = -5.0;
   rrobot->staender.hor.y =  5.0;
   rrobot->staender.hor.z =  7.0;
  
   rrobot->staender.hol.x = -5.0;
   rrobot->staender.hol.y = -5.0;
   rrobot->staender.hol.z =  7.0;
  
   rrobot->modgreifer.vul.x =  1.5;
   rrobot->modgreifer.vul.y = -3.0;
   rrobot->modgreifer.vul.z =  0.0;
  
   rrobot->modgreifer.vur.x =  1.5;
   rrobot->modgreifer.vur.y =  3.0;
   rrobot->modgreifer.vur.z =  0.0;
  
   rrobot->modgreifer.hur.x = -1.5;
   rrobot->modgreifer.hur.y =  3.0;
   rrobot->modgreifer.hur.z =  0.0;
  
   rrobot->modgreifer.hul.x = -1.5;
   rrobot->modgreifer.hul.y = -3.0;
   rrobot->modgreifer.hul.z =  0.0;
  
   rrobot->modgreifer.vol.x =  1.5;
   rrobot->modgreifer.vol.y = -3.0;
   rrobot->modgreifer.vol.z =  2.0;
  
   rrobot->modgreifer.vor.x =  1.5;
   rrobot->modgreifer.vor.y =  3.0;
   rrobot->modgreifer.vor.z =  2.0;
  
   rrobot->modgreifer.hor.x = -1.5;
   rrobot->modgreifer.hor.y =  3.0;
   rrobot->modgreifer.hor.z =  2.0;
  
   rrobot->modgreifer.hol.x = -1.5;
   rrobot->modgreifer.hol.y = -3.0;
   rrobot->modgreifer.hol.z =  2.0;
  
   rrobot->modgrleft.vul.x  =  1.0;
   rrobot->modgrleft.vul.y  =  3.0;
   rrobot->modgrleft.vul.z  =  2.0;
  
   rrobot->modgrleft.vur.x  =  1.0;
   rrobot->modgrleft.vur.y  =  2.0;
   rrobot->modgrleft.vur.z  =  2.0;
  
   rrobot->modgrleft.hur.x  = -1.0;
   rrobot->modgrleft.hur.y  =  2.0;
   rrobot->modgrleft.hur.z  =  2.0;
  
   rrobot->modgrleft.hul.x  = -1.0;
   rrobot->modgrleft.hul.y  =  3.0;
   rrobot->modgrleft.hul.z  =  2.0;
  
   rrobot->modgrleft.vol.x  =  1.0;
   rrobot->modgrleft.vol.y  =  2.5;
   rrobot->modgrleft.vol.z  =  7.0;
  
   rrobot->modgrleft.vor.x  =  1.0;
   rrobot->modgrleft.vor.y  =  2.0;
   rrobot->modgrleft.vor.z  =  7.0;
  
   rrobot->modgrleft.hor.x  = -1.0;
   rrobot->modgrleft.hor.y  =  2.0;
   rrobot->modgrleft.hor.z  =  7.0;
  
   rrobot->modgrleft.hol.x  = -1.0;
   rrobot->modgrleft.hol.y  =  2.5;
   rrobot->modgrleft.hol.z  =  7.0;
  
   rrobot->modgrright.vul.x =  1.0;
   rrobot->modgrright.vul.y = -2.0;
   rrobot->modgrright.vul.z =  2.0;
  
   rrobot->modgrright.vur.x =  1.0;
   rrobot->modgrright.vur.y = -3.0;
   rrobot->modgrright.vur.z =  2.0;
  
   rrobot->modgrright.hur.x = -1.0;
   rrobot->modgrright.hur.y = -3.0;
   rrobot->modgrright.hur.z =  2.0;
  
   rrobot->modgrright.hul.x = -1.0;
   rrobot->modgrright.hul.y = -2.0;
   rrobot->modgrright.hul.z =  2.0;
  
   rrobot->modgrright.vol.x =  1.0;
   rrobot->modgrright.vol.y = -2.0;
   rrobot->modgrright.vol.z =  7.0;
  
   rrobot->modgrright.vor.x =  1.0;
   rrobot->modgrright.vor.y = -2.5;
   rrobot->modgrright.vor.z =  7.0;
  
   rrobot->modgrright.hor.x = -1.0;
   rrobot->modgrright.hor.y = -2.5;
   rrobot->modgrright.hor.z =  7.0;
  
   rrobot->modgrright.hol.x = -1.0;
   rrobot->modgrright.hol.y = -2.0;
   rrobot->modgrright.hol.z =  7.0;
} /* initrobmod */ 



/* ============================= strahlabstand ============================= */
DOUBLE strahlabstand (M44MATRIX  rygreifer,
		      VECTOR    *anfpos,
		      VECTOR    *strahlpos,
		      INT        switch_1)
{
   DOUBLE     r_strahlabstand;
   M44MATRIX  smatrix, m44rotx;
   DOUBLE     faktor;
  
   /* STRAHLABSTAND/GRAFSIMU */
   l_hindernisfeld[0] = (DOUBLE)STRAHLLAENGE + 1;
   l_bodenfeld[0]     = (DOUBLE)STRAHLLAENGE + 1;
   switch (switch_1)
   {
      case GREIFER:
      memmove (smatrix, rygreifer, sizeof (M44MATRIX));
      if (mitr500)
	 dhmatrixtranslz (smatrix, 13.0);
      else
	 dhmatrixtranslz (smatrix, 2.0);
      break; /* GREIFER */

      case GREIFEROUT:
      faktor = sensordrehung * GRADRAD;
      if (mitr500)
      {
	 makexrotmatrix (m44rotx, faktor, 0, 19);
	 m44rotx[1][3] = 3.0;                          /* Greiferbreite/2 = 3 */
      }
      else
      {
	 makexrotmatrix (m44rotx, faktor, 0, 7);
	 m44rotx[1][3] = 2.0;                          /* Greiferbreite/2 = 2 */
      }
      dhmatrixmult (smatrix, rygreifer, m44rotx);      /* Greiferout          */
      break; /* GREIFEROUT */

      default:
      sprintf (meld_buf, SENSOR_ERR_TXT, switch_1);
      fehler (SIMU_ROB_GSW, 1, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
      break;                                  /* SIMU_RZ_MELD bringt nur Text */
   } /* CASE */
   if (switch_1 == GREIFEROUT)
   {
      l_ox = smatrix[0][1];
      l_oy = smatrix[1][1];
      l_oz = smatrix[2][1];
   }
   else
   {
      l_ox = smatrix[0][2];
      l_oy = smatrix[1][2];
      l_oz = smatrix[2][2];
   }
   l_px = smatrix[0][3];
   l_py = smatrix[1][3];
   l_pz = smatrix[2][3];
   bodentrefftest ();
   if (hindernisanf != NULL)
   { /* es gibt Hindernisse */
      hindernis = hindernisanf;
      do
      { /* Schleife ueber alle Hindernisse : */
	 if (fabs (l_ox) > 0.0000001)                       /* sonst overflow */
	 { /* Strahl geht nicht parallel zur Hindernisflaeche: */
	    l_xh = hindernis->daten.vul.x;
	    if (!((((l_xh - l_px) > 0.0) && (l_ox < 0.0)) ||
		  (((l_xh - l_px) < 0.0) && (l_ox > 0.0))))
	    { /* Strahl geht nicht "durch den Greifer" zum Hindernis: */
	       faktor = (DOUBLE)fabs ((l_xh - l_px) / l_ox);
	       l_yh   = l_py + l_oy*faktor;
	       l_zh   = l_pz + l_oz*faktor;
	       if ((hindernis->daten.vul.y <= l_yh)                   &&
		   (l_yh                   <= hindernis->daten.vur.y) &&
		   (hindernis->daten.vul.z <= l_zh)                   &&
		   (l_zh                   <= hindernis->daten.vol.z)   )
		  dtest ();                 /* Strahl trifft auf Hindernis auf */
	    } /* Strahl geht nicht "durch den Greifer" zum Hindernis */
	    l_xh = hindernis->daten.hul.x;
	    if (!((((l_xh - l_px) > 0.0) && (l_ox < 0.0)) ||
		  (((l_xh - l_px) < 0.0) && (l_ox > 0.0))))
	    { /* Strahl geht nicht "durch den Greifer" zum Hindernis: */
	       faktor = (DOUBLE)fabs ((l_xh - l_px) / l_ox);
	       l_yh = l_py + l_oy * faktor;
	       l_zh = l_pz + l_oz * faktor;
	       if ((hindernis->daten.vul.y <= l_yh)                   &&
		   (l_yh                   <= hindernis->daten.vur.y) &&
		   (hindernis->daten.vul.z <= l_zh)                   &&
		   (l_zh                   <= hindernis->daten.vol.z)   )
		  dtest ();                /* Strahl trifft auf Hindernis auf */
	    } /* Strahl geht nicht "durch den Greifer" zum Hindernis */
	 } /* Strahl geht nicht parallel zur Hindernisflaeche */
	 if (fabs (l_oy) > 0.0000001)                       /* sonst overflow */
	 { /* Strahl geht nicht parallel zur Hindernisflaeche: */
	    l_yh = hindernis->daten.vul.y;
	    if (!((((l_yh - l_py) > 0.0) && (l_oy < 0.0)) ||
		  (((l_yh - l_py) < 0.0) && (l_oy > 0.0))   ))
	    { /* Strahl geht nicht "durch den Greifer" zum Hindernis: */
	       faktor = (DOUBLE)fabs ((l_yh - l_py) / l_oy);
	       l_xh   = l_px + l_ox*faktor;
	       l_zh   = l_pz + l_oz*faktor;
	       if ((hindernis->daten.hul.x <= l_xh)                   &&
		   (l_xh                   <= hindernis->daten.vul.x) &&
		   (hindernis->daten.vul.z <= l_zh)                   &&
		   (l_zh                   <= hindernis->daten.vol.z)    )
		  dtest ();                /* Strahl trifft auf Hindernis auf */
	    } /* Strahl geht nicht "durch den Greifer" zum Hindernis */
	    l_yh = hindernis->daten.vur.y;
	    if (!((((l_yh - l_py) > 0.0) && (l_oy < 0.0)) ||
		  (((l_yh - l_py) < 0.0) && (l_oy > 0.0))   ))
	    { /* Strahl geht nicht "durch den Greifer" zum Hindernis: */
	       faktor = (DOUBLE)fabs ((l_yh - l_py) / l_oy);
	       l_xh   = l_px + l_ox*faktor;
	       l_zh   = l_pz + l_oz*faktor;
	       if ((hindernis->daten.hul.x <= l_xh)                   &&
		   (l_xh                   <= hindernis->daten.vul.x) &&
		   (hindernis->daten.vul.z <= l_zh)                   &&
		   (l_zh                   <= hindernis->daten.vol.z)   )
		  dtest ();                /* Strahl trifft auf Hindernis auf */
	    } /* Strahl geht nicht "durch den Greifer" zum Hindernis */
	 } /* Strahl geht nicht parallel zur Hindernisflaeche */
	 if (fabs (l_oz) > 0.0000001)                       /* sonst overflow */
	 { /* Strahl geht nicht parallel zur Hindernisflaeche: */
	    l_zh = hindernis->daten.vul.z;
	    if (!((((l_zh - l_pz) > 0.0) && (l_oz < 0.0)) ||
		  (((l_zh - l_pz) < 0.0) && (l_oz > 0.0)))  )
	    { /* Strahl geht nicht "durch den Greifer" zum Hindernis: */
	       faktor = (DOUBLE)fabs ((l_zh - l_pz) / l_oz);
	       l_xh   = l_px + l_ox*faktor;
	       l_yh   = l_py + l_oy*faktor;
	       if ((hindernis->daten.hul.x <= l_xh)                   &&
		   (l_xh                   <= hindernis->daten.vul.x) &&
		   (hindernis->daten.vul.y <= l_yh)                   &&
		   (l_yh                   <= hindernis->daten.vur.y)   )
		  dtest ();                /* Strahl trifft auf Hindernis auf */
	    } /* Strahl geht nicht "durch den Greifer" zum Hindernis */
	    l_zh = hindernis->daten.vol.z;
	    if (!((((l_zh - l_pz) > 0.0) && (l_oz < 0.0)) ||
		  (((l_zh - l_pz) < 0.0) && (l_oz > 0.0))   ))
	    { /* Strahl geht nicht "durch den Greifer" zum Hindernis: */
	       faktor = (DOUBLE)fabs ((l_zh - l_pz) / l_oz);
	       l_xh   = l_px + l_ox*faktor;
	       l_yh   = l_py + l_oy*faktor;
	       if ((hindernis->daten.hul.x <= l_xh)                   &&
		   (l_xh                   <= hindernis->daten.vul.x) &&
		   (hindernis->daten.vul.y <= l_yh)                   &&
		   (l_yh                   <= hindernis->daten.vur.y)   )
		  dtest ();                /* Strahl trifft auf Hindernis auf */
	    } /* Strahl geht nicht "durch den Greifer" zum Hindernis */
	 } /* Strahl geht nicht parallel zur Hindernisflaeche */
	 hindernis = hindernis->next;
      }
      while (hindernis != NULL);  /* Ende der Schleife ueber alle Hindernisse */
   } /* falls es kein Hindernis gibt */
   if (l_hindernisfeld[0] < l_bodenfeld[0])
   { /* Hindernis getroffen und Abstand kleiner als zum Boden: */
      l_d          = l_hindernisfeld[0];
      strahlpos->x = l_hindernisfeld[1];
      strahlpos->y = l_hindernisfeld[2];
      strahlpos->z = l_hindernisfeld[3];
   }
   else
   { /* kein Hindernis getroffen oder Abstand zum Boden kleiner: */
      if (l_bodenfeld[0] <= (DOUBLE)STRAHLLAENGE)
      { /* Bodenabstand kleiner/gleich Strahllaenge */
	 l_d          = l_bodenfeld[0];
	 strahlpos->x = l_bodenfeld[1];
	 strahlpos->y = l_bodenfeld[2];
	 strahlpos->z = l_bodenfeld[3];
      }
      else
      { /* Strahl trifft weder Hindernis noch Boden: */
	 l_d = -1.0;     /* Strahl zeigt max Reichweite in Y- bzw. Z-Richtung */
	 if (switch_1 == GREIFEROUT)
	    faktor = 3.0;                        /* Versatz an Greifer aussen */
	 else
	    faktor = 0.0;
	 strahlpos->x = l_px + l_ox*(STRAHLLAENGE + faktor);
	 strahlpos->y = l_py + l_oy*(STRAHLLAENGE + faktor);
	 strahlpos->z = l_pz + l_oz*(STRAHLLAENGE + faktor);
      }
   }
   r_strahlabstand = l_d;
   anfpos->x       = l_px;                      /* Anfangsposition am Greifer */
   anfpos->y       = l_py;
   anfpos->z       = l_pz;
   return (r_strahlabstand);
} /* strahlabstand */ 



/* ============================= bodentrefftest ============================= */
static void bodentrefftest (void)
{
   DOUBLE  xboden, yboden, faktor;
  
   /* BODENTREFFTEST/STRAHLABSTAND/GRAFSIMU */
   if (l_oz < 0.0)
   {
      faktor = fabs (l_pz / l_oz);
      xboden = l_px + l_ox*faktor;
      yboden = l_py + l_oy*faktor;
      l_d = sqrt ((l_px - xboden) * (l_px - xboden) +
		  (l_py - yboden) * (l_py - yboden) + l_pz * l_pz);
      if (l_d <= (DOUBLE)STRAHLLAENGE)
      {
	 l_bodenfeld[0] = l_d;
	 l_bodenfeld[1] = xboden;
	 l_bodenfeld[2] = yboden;
	 l_bodenfeld[3] = 0.0;
      }
   }
} /* bodentrefftest */ 



/* ================================= dtest ================================== */
static void dtest (void)
{ /* DTEST/STRAHLABSTAND/GRAFSIMU */
   l_d = sqrt ((l_px - l_xh) * (l_px - l_xh) +
	       (l_py - l_yh) * (l_py - l_yh) +
	       (l_pz - l_zh) * (l_pz - l_zh)  );
   if (l_d < l_hindernisfeld[0])
   { /* Strahldistanz auf Hindernis <= bisheriger Eintrag */
      l_hindernisfeld[0] = l_d;
      l_hindernisfeld[1] = l_xh;
      l_hindernisfeld[2] = l_yh;
      l_hindernisfeld[3] = l_zh;
   } /* Strahldistanz auf Hindernis <= bisheriger Eintrag */
} /* dtest */ 



/* ================================ zeigr500 ================================ */
void zeigr500 (ROBOTR500TYP *rin,
	       JOINTS        th,
	       M44MATRIX     projm,
	       POINT        *tcpp,
	       INT           switch_1)
{
   M44MATRIX  rmth0, ryarm1, ryarm2,
	      rygreifer,
	      proberstck,
	      pryarm1, pryarm2,
	      prygreifer;
#ifdef MIT_ANIM
   VECTOR     tcpoint;
   POINT      aa;
#endif /* MIT_ANIM */
   DOUBLE     h,
	      s1, s2, s3, s4, s5,
	      c1, c2, c3, c4, c5,
	      c1c2, c1s2,
	      s1c2, s1s2;
  
   /* ZEIGR500/GRAFSIMU */
   s1 = sin (th[0]);
   c1 = cos (th[0]);
   s2 = sin (th[1]);
   c2 = cos (th[1]);
   s3 = sin (th[2]);
   c3 = cos (th[2]);
   s4 = sin (th[3]);
   c4 = cos (th[3]);
   s5 = sin (th[4]);
   c5 = cos (th[4]);
   c1c2 = c1*c2;
   c1s2 = c1*s2;
   s1c2 = s1*c2;
   s1s2 = s1*s2;
   if (switch_1 != MITSTRAHL)
   {
      rmth0[0][0] = c1;
      rmth0[0][1] = -s1;
      rmth0[0][2] = 0.0;
      rmth0[0][3] = 0.0;
      rmth0[1][0] = s1;
      rmth0[1][1] = c1;
      rmth0[1][2] = 0.0;
      rmth0[1][3] = 0.0;
      rmth0[2][0] = 0.0;
      rmth0[2][1] = 0.0;
      rmth0[2][2] = 1.0;
      rmth0[2][3] = 20.0;
      rmth0[3][0] = 0.0;
      rmth0[3][1] = 0.0;
      rmth0[3][2] = 0.0;
      rmth0[3][3] = 1.0;
      ryarm1[0][0] = c1c2;
      ryarm1[0][1] = -s1;
      ryarm1[0][2] = c1s2;
      ryarm1[0][3] = 0.0;
      ryarm1[1][0] = s1c2;
      ryarm1[1][1] = c1;
      ryarm1[1][2] = s1s2;
      ryarm1[1][3] = 0.0;
      ryarm1[2][0] = -s2;
      ryarm1[2][1] = 0.0;
      ryarm1[2][2] = c2;
      ryarm1[2][3] = 25.0;
      ryarm1[3][0] = 0.0;
      ryarm1[3][1] = 0.0;
      ryarm1[3][2] = 0.0;
      ryarm1[3][3] = 1.0;
   }
   ryarm2[0][0] = c1c2*c3-c1s2*s3;
   ryarm2[0][1] = -s1;
   ryarm2[0][2] = c1c2*s3 + c1s2*c3;
   ryarm2[0][3] = c1s2*22.0;
   ryarm2[1][0] = s1c2*c3-s1s2*s3;
   ryarm2[1][1] = c1;
   ryarm2[1][2] = s1c2*s3 + s1s2*c3;
   ryarm2[1][3] = s1s2*22.0;
   ryarm2[2][0] = -s2*c3-c2*s3;
   ryarm2[2][1] = 0.0;
   ryarm2[2][2] = -s2*s3 + c2*c3;
   ryarm2[2][3] = c2*22.0 + 25.0;
   ryarm2[3][0] = 0.0;
   ryarm2[3][1] = 0.0;
   ryarm2[3][2] = 0.0;
   ryarm2[3][3] = 1.0;
   h = ryarm2[0][0]*c4 - ryarm2[0][2]*s4;
   rygreifer[0][0] = h*c5-s1*s5;
   rygreifer[0][1] = -h*s5-s1*c5;
   rygreifer[0][2] = ryarm2[0][0]*s4 + ryarm2[0][2]*c4;
   rygreifer[0][3] = ryarm2[0][2]*16.0 + ryarm2[0][3];
   h = ryarm2[1][0]*c4 - ryarm2[1][2]*s4;
   rygreifer[1][0] = h*c5 + c1*s5;
   rygreifer[1][1] = -h*s5 + c1*c5;
   rygreifer[1][2] = ryarm2[1][0]*s4 + ryarm2[1][2]*c4;
   rygreifer[1][3] = ryarm2[1][2]*16.0 + ryarm2[1][3];
   h = ryarm2[2][0]*c4 - ryarm2[2][2]*s4;
   rygreifer[2][0] = h*c5;
   rygreifer[2][1] = -h*s5;
   rygreifer[2][2] = ryarm2[2][0]*s4 + ryarm2[2][2]*c4;
   rygreifer[2][3] = ryarm2[2][2]*16.0 + ryarm2[2][3];
   rygreifer[3][0] = 0.0;
   rygreifer[3][1] = 0.0;
   rygreifer[3][2] = 0.0;
   rygreifer[3][3] = 1.0;
   if (switch_1 == MITSTRAHL)
      memmove (armfeld[axisanz], rygreifer, sizeof (M44MATRIX));
   else
   {
      dhmatrixmult (proberstck, projm, rmth0);       /*  Proj.-Rot.-Matrizen  */
      dhmatrixmult (pryarm1, projm, ryarm1);
      dhmatrixmult (pryarm2, projm, ryarm2);
      dhmatrixmult (prygreifer, projm, rygreifer);
    
#ifdef MIT_ANIM
      if (nichtevo)
      {
	 drawcubus (&(rin->sockel), projm);
	 drawcubus (&(rin->zwstck1), projm);
	 drawcubus (&(rin->oberstck), proberstck);
	 if (hindachse == 2)
	    _setcolor (LIGHTRED);
	 drawcubus (&(rin->arm1), pryarm1);
	 if (hindachse == 2)
	    _setcolor (WHITE);
	 if (hindachse == 3)
	    _setcolor (LIGHTRED);
	 drawcubus (&(rin->arm2), pryarm2);
	 if (hindachse == 2)
	    _setcolor (WHITE);
	 drawcubus (&(rin->zwstck2), prygreifer);
	 if (hindachse > MAXAXISANZ)
	    _setcolor (LIGHTRED);
	 drawcubus (&(rin->greifer), prygreifer);
	 drawcubus (&(rin->grright), prygreifer);
	 drawcubus (&(rin->grleft), prygreifer);
	 if (hindachse == 2)
	    _setcolor (WHITE);
      }
      tcpoint.x =  0.0;
      tcpoint.y =  0.0;
      tcpoint.z = 18.0;
      projection (&aa, prygreifer, &tcpoint);
      tcpp->x = aa.x;
      tcpp->y = aa.y;
#endif /* MIT_ANIM */
   }
} /* zeigr500 */



/* ============================== robmodinit ============================== */
void robmodinit (INT  achszahl)
{ /* ROBMODINIT/GRAFSIMU */
   achszahl--;                                 /* wegen Felddefinition in C */
   if (axisdatfeld[achszahl].axrichtg == ZR)
   {
      axiscubusfeld[achszahl].vul.x = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vul.y = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].vul.z = 0.0;
    
      axiscubusfeld[achszahl].vur.x = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vur.y = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].vur.z = 0.0;
    
      axiscubusfeld[achszahl].hur.x = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hur.y = d_round (axisdatfeld[achszahl].
						      axhoehe  / 2.0);
      axiscubusfeld[achszahl].hur.z = 0.0;
    
      axiscubusfeld[achszahl].hul.x = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hul.y = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].hul.z = 0.0;
    
      axiscubusfeld[achszahl].vol.x = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vol.y = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].vol.z = (DOUBLE)axisdatfeld[achszahl].axlaenge;
    
      axiscubusfeld[achszahl].vor.x = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vor.y = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].vor.z = (DOUBLE)axisdatfeld[achszahl].axlaenge;
    
      axiscubusfeld[achszahl].hor.x = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hor.y = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].hor.z = (DOUBLE)axisdatfeld[achszahl].axlaenge;
    
      axiscubusfeld[achszahl].hol.x = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hol.y = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].hol.z = (DOUBLE)axisdatfeld[achszahl].axlaenge;
   }
   else
   {
      axiscubusfeld[achszahl].vul.y = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vul.z = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].vul.x = 0.0;
    
      axiscubusfeld[achszahl].vur.y = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vur.z = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].vur.x = 0.0;
    
      axiscubusfeld[achszahl].hur.y = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hur.z = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].hur.x = 0.0;
    
      axiscubusfeld[achszahl].hul.y = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hul.z = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].hul.x = 0.0;
    
      axiscubusfeld[achszahl].vol.y = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vol.z = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].vol.x = (DOUBLE)axisdatfeld[achszahl].axlaenge;
    
      axiscubusfeld[achszahl].vor.y = d_round (axisdatfeld[achszahl].
						     axbreite / 2.0);
      axiscubusfeld[achszahl].vor.z = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].vor.x = (DOUBLE)axisdatfeld[achszahl].axlaenge;
    
      axiscubusfeld[achszahl].hor.y = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hor.z = d_round (axisdatfeld[achszahl].
						     axhoehe  / 2.0);
      axiscubusfeld[achszahl].hor.x = (DOUBLE)axisdatfeld[achszahl].axlaenge;
    
      axiscubusfeld[achszahl].hol.y = -d_round (axisdatfeld[achszahl].
						       axbreite / 2.0);
      axiscubusfeld[achszahl].hol.z = -d_round (axisdatfeld[achszahl].
						       axhoehe  / 2.0);
      axiscubusfeld[achszahl].hol.x = (DOUBLE)axisdatfeld[achszahl].axlaenge;
   } /* ELSE-Teil, d.h. x-Achsenrichtung */
} /* robmodinit */ 



/* =============================== ktrwrobmod =============================== */
void ktrwrobmod (JOINTS    th,
		 INT       achszahl,
		 INT       switch_1)
{
   typedef M44MATRIX  T_THFELD [MAXAXISANZ + 2];
  
   M44MATRIX    tcp, greiferdreh;
   T_THFELD     thfeld;
   INT          achse, i, xd, zd, axplus1;
  
   /* KTRWROBMOD/GRAFSIMU */
   axplus1 = achszahl + 1;
   if (achszahl > 0)                   /* Abfrage, ob Achsen definiert wurden */
   {
      for (i = 1;  i <= achszahl;  i++)
      {
	 xd = 0;
	 zd = 0;
	 if (i == 1)
	    zd = 7;
	 else
	 { /* Achslaenge einlesen */
	    if (axisdatfeld[i-2].axrichtg == XR)
	       xd = axisdatfeld[i-2].axlaenge;
	    else
	       zd = axisdatfeld[i-2].axlaenge;
	 } /* Achslaenge einlesen */
	 switch (axisdatfeld[i-1].rotax)
	 {
	    case XR:
	    makexrotmatrix (thfeld[i], th[i-1], xd, zd);
	    break;
	    case YR:
	    makeyrotmatrix (thfeld[i], th[i-1], xd, zd);
	    break;
	    case ZR:
	    makezrotmatrix (thfeld[i], th[i-1], xd, zd);
	    break;
	 } /* CASE */
      } /* FOR-Schleife ueber alle Achsen */
      memmove (armfeld[0], thfeld[1], sizeof (M44MATRIX));
      for (i = 2;  i <= achszahl;  i++)
	 dhmatrixmult (armfeld[i-1], armfeld[i-2], thfeld[i]);
      if (kollitest)
      {
	 if (hindernisanf != NULL)
	 {
	    hindernis = hindernisanf;
	    hindachse = 0;        /* keine Achse an ein Hindernis angestossen */
	    do
	    {
	       for (i = 0;  i < achszahl;  i++)
		  kollicheck (armfeld[i][0][3], armfeld[i][1][3],
			      armfeld[i][2][3], i+1);
	       hindernis = hindernis->next;
	    }
	    while (hindernis != NULL);
	 }
      }
      if (axisdatfeld[achszahl-1].axrichtg == XR)
      {
	 makeyrotmatrix (greiferdreh, PIH,                         /* 90 Grad */
			 axisdatfeld[achszahl-1].axlaenge, 0);
	 dhmatrixmult (armfeld[axplus1-1], armfeld[achszahl-1], greiferdreh);
      }
      else
      {
	 memmove (armfeld[axplus1-1], armfeld[achszahl-1], sizeof (M44MATRIX));
	 dhmatrixtranslz (armfeld[axplus1-1],
			  (DOUBLE)axisdatfeld[achszahl-1].axlaenge);
      }
      memmove (tcp, armfeld[axplus1-1], sizeof (M44MATRIX));
      dhmatrixtranslz (tcp, 6.0);
      if (kollitest)
      {
	 if (hindernisanf != NULL)
	 {
	    hindernis = hindernisanf;
	    do
	    {
	       kollicheck (tcp[0][3], tcp[1][3], tcp[2][3], 99);
	       hindernis = hindernis->next;
	    }
	    while (hindernis != NULL);
	 }
	 /* --------------- Test auf Kollision mit dem Boden: --------------- */
	 achse = 0;
	 for (i = 1;  i <= achszahl;  i++)
	 {
	    if (armfeld[i-1][2][3] <= 0.0)
	       achse = i;
	 }
	 if (tcp[2][3] <= 0.0)
	    achse = 99;
	 if (achse > 0)
	 {
	    if (nichtevo)
	    {
	       hindachse = achse;
	       if (achse <= MAXAXISANZ)
		  sprintf (meld_buf, ACHS_KOLL_TXT, achse);
	       else
		  strcpy (meld_buf, GREIFER_KOLL_TXT);
	       meldung (SIMU_ROB_GSW, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
	       write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
	    }                                 /* SIMU_RZ_MELD bringt nur Text */
	    kollision = TRUE;
	 }
      }
      robotframe->transl.x = tcp[0][3];
      robotframe->transl.y = tcp[1][3];
      robotframe->transl.z = tcp[2][3];
      if (switch_1 == MITORI)
      {
	 robotframe->rot.matrix.t.x = tcp[0][0];
	 robotframe->rot.matrix.t.y = tcp[1][0];
	 robotframe->rot.matrix.t.z = tcp[2][0];
	 robotframe->rot.matrix.o.x = tcp[0][1];
	 robotframe->rot.matrix.o.y = tcp[1][1];
	 robotframe->rot.matrix.o.z = tcp[2][1];
	 robotframe->rot.matrix.a.x = tcp[0][2];
	 robotframe->rot.matrix.a.y = tcp[1][2];
	 robotframe->rot.matrix.a.z = tcp[2][2];
      } /* mitORI */
   } /* achszahl > 0, d.h. es wurden Achsen definiert */
} /* ktrwrobmod */ 



#ifdef MIT_ANIM
/* =============================== zeigrobmod =============================== */
void zeigrobmod (M44MATRIX  projm,
		 POINT     *tcpp,
		 INT        achszahl)
{
   MFELDTYP  axmatfeld;
   VECTOR    tcpoint;
   POINT     aa;
   INT       i, axplus1;
  
   /* ZEIGROBMOD/GRAFSIMU */
   axplus1 = achszahl + 1;
   if (achszahl > 0)
   {
      for (i = 0;  i < axplus1;  i++)
	 dhmatrixmult (axmatfeld[i], projm, armfeld[i]);
   } /* achszahl > 0 */
   if (nichtevo)
      drawcubus (&(robmod.staender), projm);
   if (achszahl > 0)
   { /* Achsanzahl groesser als 0 */
      if (nichtevo)
      { /* Aufruf nicht aus Evo */
	 for (i = 1;  i <= achszahl;  i++)
	 {
	    if ((hindachse == i))
	       _setcolor (LIGHTRED);
	    drawcubus (&(axiscubusfeld[i-1]), axmatfeld[i-1]);
	    if ((hindachse == i))
	       _setcolor (WHITE);
	 }
	 if (hindachse > MAXAXISANZ)
	    _setcolor (LIGHTRED);
	 drawcubus (&(robmod.modgreifer), axmatfeld[axplus1-1]);
	 drawcubus (&(robmod.modgrright), axmatfeld[axplus1-1]);
	 drawcubus (&(robmod.modgrleft), axmatfeld[axplus1-1]);
	 if (hindachse > MAXAXISANZ)
	    _setcolor (WHITE);
	 hindachse = 0;
      } /* Aufruf nicht aus Evo */
      tcpoint.x = 0.0;
      tcpoint.y = 0.0;
      tcpoint.z = 6.0;
      projection (&aa, axmatfeld[axplus1-1], &tcpoint);
      tcpp->x = aa.x;
      tcpp->y = aa.y;
   } /* Achsanzahl groesser als 0 */
} /* zeigrobmod */ 
#endif /* MIT_ANIM */



/* ============================== rob_gsw_init ============================== */
BOOL rob_gsw_init (void)
{
   size_t  dsize = sizeof (DOUBLE);
   size_t  csize = sizeof (CUBUS);
   size_t  asize = sizeof (AXISDATTYP);
   BOOL    ok;

   /* rob_gsw_init */
   ok = ((anfangsframe      = new_frame (nilrot, nilvector))       != NULL) &&
	((zielframe         = new_frame (nilrot, nilvector))       != NULL) &&
	((oldrobotframe     = new_frame (nilrot, nilvector))       != NULL) &&
	((oldrobotjoints    = new_joints ()                  )     != NULL) &&
	((beschlrobotjoints = new_joints ()                  )     != NULL) &&
	((axiscubusfeld     = (CUBUS*)malloc(MAXAXISANZ*csize))    != NULL) &&
	((robotr500 = (ROBOTR500TYP*)malloc(sizeof(ROBOTR500TYP))) != NULL) &&
	((robmod    = (ROBMODTYP*)   malloc(sizeof(ROBMODTYP)))    != NULL) &&
	((axisdatfeld    = (AXISDATTYP*)malloc(MAXAXISANZ*asize))  != NULL) &&
	((sensorposvar   = (DOUBLE*)malloc(MAXAXISANZ*dsize))      != NULL) &&
	((anschlag_oben  = (DOUBLE*)malloc(MAXAXISANZ*dsize))      != NULL) &&
	((anschlag_unten = (DOUBLE*)malloc(MAXAXISANZ*dsize))      != NULL) &&
	((l_hindernisfeld= (DOUBLE*)malloc(4*dsize))               != NULL) &&
	((l_bodenfeld    = (DOUBLE*)malloc(4*dsize))               != NULL) &&
	((meld_buf       = (char*)malloc(MAX_STR*sizeof(char)))    != NULL) &&
	((p              = (POINT*)malloc(MAXBAHNP*sizeof(POINT))) != NULL) &&
	((armfeld=(M44MATRIX*)malloc((MAXAXISANZ+1)*sizeof(M44MATRIX))) !=NULL);
   mit_simu_anim = FALSE;                    /* nur rudimentaer implementiert */
   if (ok)
      rsinit ();
   return (ok);
} /* rob_gsw_init */



