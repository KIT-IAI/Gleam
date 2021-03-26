/* Filedefinitionen der Version ohne Parameter: */
#define ROSEN_INI_FNAME       "rosenbrock.ini"
#define ROSEN_STARTW_FNAME    "objfct.ini"
#define ROSEN_QF_FKT_NAME     "objfct"


/*  Version mit Parametern:
extern int rosencon_original(const char* OptimizationIni, const char* Initdatei, const char* QF_Berechnung);
*/

extern int rosencon_original(void);
