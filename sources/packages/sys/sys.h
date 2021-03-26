/* -----------------------------------------------------------------------------
GLEAM/AE                     Package-Header-File                  
Package: sys                     File: sys.h                     Version: V1.2.2
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Betriebssysteme und Entwicklungswerkzeuge:
  Solaris 5.8    / Sun C 4.2 und g++
  Linux Suse 9.2 / gcc
  Windows/NT     / Borland C++ 5.0
  DOS 6.22       / Borland C++ 4.51
--------------------------------------------------------------------------------

Das Package enthaelt Routinen zur Anbindung systemnaher Funktionen. Diese wurden
hier zusammengefasst, um die Portabilitaet zu erhoehen.

Module des Packages:
    sys_gsw   Systemnahe Funktionen, die nicht in einem der nachstehenden 
              Module enthalten sind. Enthaelt unter anderem den Zufallszahlen-
	      generator, Zeit- und Datumsfunktionen und mathematische Routinen.
    file_gsw  File-I/O-Routinen, Existenzpruefung von Files, Directory-Routinen.
    extkoppl  Low-Level-Ankopplung externer Programme, konkret: "gnu_plot" und
              "simpilot"/"eldo". (Nur fuer Sun/Solaris-Version)
    term_io   Keyboard-Routinen der TEXT_MODE-Variante.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Linux-Portierung: Aus "round" wird "roundd", "roundl" und 
    "roundi". Toene verkuerzt.                                       WJ 12.05.05
 2. Export von "rand_seed".                                          WJ 10.11.06
 3. Neue Routine "getEnvVar".                                        WJ 22.07.10
 4. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 5. Verschiebung von "mit_simulog" und "write_to_simulog" nach 
    Package "sys", Modul "file_gsw".                                 WJ 20.04.12
 6. Anpassung von Schaltern: GLEAM_C gestrichen oder zu !GUI_MODE.   WJ 28.06.12
 7. Erweiterung von "seed_rand" um einen "offset"-Parameter.         WJ 25.10.12
 8. "kb_hit" und "get_kb_char" werden uneingeschraenkt exportiert.
    Vorher nur bei nicht gesetztem GUI_MODE.                         WJ 27.10.14
 9. Vereinheitlichung der Module (vor allem "term_io") derart, dass 
    das gesamte Package von allen Versionen (Kommandozeile, User-
    interface) eingebunden werden kann. Aenderung: "kb_hit" und 
    "get_kb_char" werden nur bei gesetztem GLEAM_USR exportiert.     WJ 23.05.16
10. Aenderung von "write_to_log" und Streichung von "mit_logfile".   WJ 19.12.16
11. ExtSimuServ: "taste" wird von Modul "evo_gsw" uebernommen.       WJ 29.08.17
12. New variable "atProgStart".                                      WJ 06.10.17
13. New time functions startTimerMS() and calcElapsedTimeMS().       WJ 16.07.18
--------------------------------------------------------------------------------
Status : o.k.                   Autor: W.Jakob                 Datum: 16.07.2018
----------------------------------------------------------------------------- */


/* --------------------------- exportierte Macros: -------------------------- */
#define trunc(a)   ((LONG)(a))



/* ========================= Exportierte Konstanten: ======================== */
#define MAX_GNU          6      /* Max. Anzahl gleichzeitig offener GNU-Plots */
#define KURZ_TON         1
#define LANG_TON         2
#define ERROR_TON        3
#define FATAL_TON        5
#define KEEP_LOG_OPEN    TRUE
#define CLOSE_LOG        FALSE



/* ========================== Exportierte Variable: ========================= */

/* ----------------------- Variable aus Module sys_gsw: --------------------- */
extern char    *prog_id_buf;            /* Programm-Name, Version und Stand   */
extern time_t   rand_seed;              /* die Zeit vom Aufruf von "seed_rand"*/
extern BOOL     taste,                  /* TRUE = Unterbrechungswunsch        */
                atProgStart;            /* TRUE until init files first read   */

/* ----------------------- Variable aus Module file_gsw: -------------------- */
extern FILE    *logfile;                /* Log-File                           */
extern BOOL     logfile_open;           /* TRUE, solange das Logfile offen ist*/
extern char    *logfilename;            /* Name des Log-Files                 */
#ifdef MITSIMULOG
  extern FILE  *simu_logfile;           /* Log-File des Simulators            */
  extern BOOL   simu_logfile_open,      /* TRUE, solange SimuLogfile offen    */
                mit_simu_logfile,       /* TRUE, wenn SimuLogfile angelegt    */
                mit_simulog;            /* Steuert generell LOGFILE-Ausgaben  */
  extern char  *simu_logfilename;       /* Name des Log-Files fuer Simulator  */
#endif /* MITSIMULOG */

/* ----------------------- Variable aus Module extkoppl: -------------------- */
extern BOOL   sim_ok;                   /* TRUE, wenn der Simulator laeuft.   */
extern INT    pipe_p2s [2];             /* Pipe zum Simulator                 */
extern INT    pipe_s2p [2];             /* Pipe vom Simulator                 */
extern INT    pipe_se2p [2];            /* Pipe vom Simulator f. Fehlermldgen */
extern FILE*  tsim;                     /* Filedescriptor f.Sim-Pipe zum Sim. */
extern FILE*  fsim;                     /* Filedescriptor f.Sim-Pipe vom Sim. */
extern FILE*  efsim;                    /* Filedescriptor f.Sim-Fehler-Pipe   */
#if defined (GNUPLOT)
  extern BOOL plot_ok [MAX_GNU];        /* TRUE, wenn das Plotprogramm laeuft.*/
  extern INT  pipe_p2g [MAX_GNU] [2];   /* Pipes zur Komm. mit dem PlotProg   */
#endif /* GNUPLOT */



/* ========================== Exportierte Routinen: ========================= */
extern BOOL   sysInit  (void);

/* ----------------------- Routinen aus Module sys_gsw: --------------------- */
extern void   warte             (unsigned INT  m_sec);

extern void   seed_rand         (long int  offset);

extern INT    irand             (INT       range);

extern DOUBLE frand             (VOID);

extern void   get_time_stamp    (STR_TYPE  t_stamp,
                                 STR_TYPE  datum,
                                 STR_TYPE  w_tag,
                                 STR_TYPE  uhr_zeit);

extern void   start_timer       (LONG      saved_time);

extern LONG   calc_elapsed_time (void);

extern void   startTimerMS      (void);

extern DOUBLE calcElapsedTimeMS (void);

extern DOUBLE d_round           (DOUBLE    a);

extern LONG   l_round           (DOUBLE    a);

extern INT    i_round           (DOUBLE    a);

extern DOUBLE fsqr              (DOUBLE    x);

extern LONG   isqr              (LONG      x);

extern void   drei_klang        (INT       dauer);



/* ---------------------- Routinen aus Module extkoppl: --------------------- */
extern void start_child_handler (void);

extern BOOL init_sim            (void);

#if defined (GNUPLOT)
  extern BOOL init_plot         (INT         gnu_index,
                                 STR_TYPE    gnu_param);
#endif /* GNUPLOT */



/* ---------------------- Routinen aus Module file_gsw: --------------------- */
extern BOOL init_file_gsw    (void);

extern BOOL file_exists      (const STR_TYPE  fspec); 

extern BOOL open_file        (FILE          **file_var,
                              const char     *fspec,
                              const char     *mode);
 
extern BOOL print_and_check  (FILE           *dest,
                              const char     *buffer);

extern void write_to_log     (const char     *buffer,
                              BOOL            keepOpen);

#ifdef MITSIMULOG
  extern void write_to_simulog (const char   *buf,
                                BOOL          keepOpen);
#endif /* MITSIMULOG */

extern BOOL make_startsim    (const char     *shell,
                              const char     *pack,
                              const char     *dir_name,
                              const char     *prep_cmd1,
                              const char     *prep_cmd2,
                              const char     *sim_cmd);

extern void get_env_var_name (char           *buf);

extern BOOL check_dir        (const STR_TYPE  dir_name);

extern INT  show_files       (STR_TYPE        dir_name,
                              const STR_TYPE  extention,
                              BOOL            mit_nr);

extern INT  show_sim_mods    (STR_TYPE  sim_root_dir,
                              INT       znr,
                              BOOL      mit_nr);

extern char *getEnvVar       (char     *envVarName);

extern void init_sim_mods    (STR_TYPE  sim_root_dir);


/* ----------------------- Routinen aus Module term_io: --------------------- */
#ifdef BS_LINUX
  extern void restore_tty_params (void);
#endif /* BS_LINUX */

#ifdef GLEAM_USR
  extern BOOL kb_hit             (VOID);

  extern char get_kb_char        (VOID);
#endif /* GLEAM_USR */
