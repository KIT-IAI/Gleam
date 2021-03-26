/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File 
Package: simu/lskp               File: rob_int.h             Version:     V1.0.2
Status : o.k.                   Autor: C.Blume/W.Jakob         Datum: 02.12.2016
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
-------------------------------------------------------------------------------- */
 
/* ======================= Package-globale Konstanten: ====================== */
#define PIH 	  	  1.570796326795
#define PI 		  3.14159265359 
#define ZWEIPI 		  6.28318530718 
#define MINREAL 	  10e-15
#define MAXREAL 	  10e+15

#ifdef MIT_ANIM
  #define MAXBAHNP        70  /* MAXBAHNP Bahnpunkte werden gezeigt. (f.SIMU) */
#else
  #define MAXBAHNP        2   /* aus Konsistenzgruenden. Angezeigt wird nichts*/
#endif

#define MITORI            0
#define OHNEORI           1
#define MITMITTELP        2
#define OHNEMITTELP       3
#define GRFENSTER         4
#define KLFENSTER         5
#define MITZEIGROB        6
#define OHNEZEIGROB       7
#define MITSTRAHL         8
#define OHNESTRAHL        9
#define GREIFEROUT       10
#define GREIFER          11



/* ==================== Package-globale Typdeklarationen: =================== */
typedef struct 
  { DOUBLE  x,
            y,
            z;
  } VECTOR;

typedef struct 
  { VECTOR  t,
            o,
            a;
  } ROTMATRIX;

typedef struct 
  { VECTOR     axis;
    DOUBLE     angle;
    ROTMATRIX  matrix;
  } ROTATION;

typedef struct 
  { ROTATION  rot;
    VECTOR    transl;
  } FRAME;

typedef DOUBLE     M44MATRIX [4][4];
typedef M44MATRIX *MFELDTYP;

typedef struct 
  { 
    INT  x, y;
  } POINT;

typedef struct 
  { 
    VECTOR  vul, vur, hur, hul, vol, vor, hor, hol;
  } CUBUS;

typedef struct 
  { 
    CUBUS  sockel, zwstck1, oberstck, arm1, arm2, zwstck2, greifer, grleft,
           grright;
  } ROBOTR500TYP;

typedef struct 
  { 
    CUBUS  staender, modgreifer, modgrleft, modgrright;
  } ROBMODTYP;

typedef struct 
  { 
    VECTOR  xaxanf, yaxanf, xpfr, xpfl, ypfr, ypfl, xaxend, yaxend;
  } KOSYST;

typedef enum { XR, YR, ZR} RTYP;

typedef enum { FEST, ROT, TRANS} BEWARTTYP;

typedef struct 
  { 
    RTYP       axrichtg;
    INT        axlaenge,
               axbreite,
               axhoehe;
    BEWARTTYP  bewart;
    RTYP       rotax;
  } AXISDATTYP;

typedef struct HINDERNISTYP *HINDERNISZEIGER;

typedef struct HINDERNISTYP 
  { 
    HINDERNISZEIGER  next;
    STR_TYPE         name;
    CUBUS            daten;
  } HINDERNISTYP;



/* ======================== Package-globale Variable: ======================= */

/* ---------------------- Variable aus Modul: rob_gsw ----------------------- */
extern char      *meld_buf;     /* Allgemeiner lokaler Meldungspuffer         */
extern DOUBLE     bildtakt;     /* Zeitinkrement des Simulators in Sekunden   */
extern INT        axisanz;      /* Anzahl der Roboterachsen                   */
extern BOOL       mitr500,      /* Zeigt an, ob mit Mitsubishi-IR oder Modell */
                  kollitest,    /* TRUE, wenn Kollisionstext durchzufuehren   */
                  anschlagtest, /* TRUE, wenn Anschlagtest durchzufuehren ist */
                  anschlag_ko,  /* TRUE, wenn bei Anschlag Abwertung (N=0)    */
                  mit_simu_anim;/* Mit Animation der Simulat. Nur teilw.impl. */
extern FRAME     *anfangsframe, /* Startposition und -orientierung            */
                 *zielframe;    /* Zielposition und -orientierung             */

extern CUBUS           *axiscubusfeld;
extern ROBOTR500TYP    *robotr500;
extern ROBMODTYP       *robmod;
extern AXISDATTYP      *axisdatfeld;
extern DOUBLE          *sensorposvar;
extern DOUBLE          *anschlag_oben;
extern DOUBLE          *anschlag_unten;
extern HINDERNISZEIGER  hindernisanf,
                        hindernis,
                        hindernisloeschen;
extern M44MATRIX        pm;
extern MFELDTYP         armfeld;
extern JOINTS           oldrobotjoints,
                        beschlrobotjoints;
extern FRAME           *oldrobotframe;
extern DOUBLE           sensordrehung;
extern INT              h;
extern BOOL             kollision, /* TRUE, wenn Kollision stattgefunden hat */
                        nichtevo;

extern POINT           *p;                /* Feld der angezeigten Bahnpunkte */


/* ---------------------- Variable aus Modul: pasromin ---------------------- */
extern double     arobotbase,       /*  */ 
                  brobotbase,       /*  */ 
                  crobotbase,       /*  */ 
                  speedfactor;      /*  */ 
extern VECTOR    *nilvector,        /* Null-Vektor                            */ 
                 *xaxis,            /* Einheitsvektor in Richtung der X-Achse */ 
                 *yaxis,            /* Einheitsvektor in Richtung der Y-Achse */ 
                 *zaxis;            /* Einheitsvektor in Richtung der Z-Achse */ 
extern ROTATION  *nilrot;           /* Null-Rotation                          */ 
extern FRAME     *nilframe,         /* Null-Frame                             */ 
                 *robotframe;       /*  */ 
extern JOINTS     robotjoints;      /*  */ 
extern INT        joints_mem_size;  /* Platzbedarf eines JOINTS-Obj. in Bytes */



/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: rob_gsw ----------------------- */
extern void     setze_start_ziel_frame (void);

extern void     zeig_pos_joints   (JOINTS     ausg_joints,
                                   INT       *znr);

extern void     r500kollicheck    (JOINTS     th);

extern void     ktrwr500          (FRAME     *fout,
                                   JOINTS     th);

extern void     posktrwr500       (VECTOR    *pos,
                                   JOINTS     th);

extern void     init_mitsubishi   (void);

extern void     convertvs         (char      *vtext,
                                   VECTOR    *vp);

extern void     dhmatrixmult      (M44MATRIX  mout,
                                   M44MATRIX  m1,
                                   M44MATRIX  m2);

extern void     initr500          (ROBOTR500TYP *rrobot);

extern void     initrobmod        (ROBMODTYP *rrobot);

extern DOUBLE   strahlabstand     (M44MATRIX  rygreifer,
                                   VECTOR    *anfpos,
                                   VECTOR    *strahlpos,
                                   INT        switch_1);

extern void     zeigr500          (ROBOTR500TYP *rin,
                                   JOINTS        th,
                                   M44MATRIX     projm,
                                   POINT        *tcpp,
                                   INT           switch_1);

extern void     robmodinit        (INT        achszahl);

extern void     ktrwrobmod        (JOINTS     th,
                                   INT        achszahl,
                                   INT        switch_1);

extern void     zeigrobmod        (M44MATRIX  projm,
                                   POINT     *tcpp,
                                   INT        achszahl);

extern BOOL     rob_gsw_init      (void);

 
/* ---------------------- Routinen aus Modul: pasromin ---------------------- */
extern void      vsub           (VECTOR   *vout,
                                 VECTOR   *vin1,
                                 VECTOR   *vin2);

extern DOUBLE    vdist          (VECTOR   *vin1,
                                 VECTOR   *vin2);
 
extern VECTOR   *new_vector     (double    x_init,
                                 double    y_init,
                                 double    z_init);

extern ROTATION *new_rot        (VECTOR   *achse,
                                 double    winkel);

extern FRAME    *new_frame      (ROTATION *rot,
                                 VECTOR   *pos);

extern JOINTS    new_joints     (void);

extern BOOL      pasromin_init  (void);

