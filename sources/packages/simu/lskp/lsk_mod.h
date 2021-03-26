/* -----------------------------------------------------------------------------
GLEAM/AE                   Anwendungsspezifisches Header-File    Version: V1.2.0

                              LeSaK-handlungsMODell

Package: simu                    File: lsk_mod.h              Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
-------------------------------------------------------------------------------- 

Konstantendeklarationen der Handlungen entsprechend der Codierung in "activity"
und Modellfiles von LESAK/Plus.
Typdeklarationen geometrischer Datentypen, soweit sie ausserhalb des Simulators 
benoetigt werden.
    
-------------------------------------------------------------------------------- 
Aenderungen:
 1. Streichung der TSK-Daten-Indices.                                WJ 22.11.16
-------------------------------------------------------------------------------- 
Status: o.k.             Autoren: C.Blume / W.Jakob            Datum: 22.11.2016
-------------------------------------------------------------------------------- */


/* ----------------------------- Handlungscodes: ---------------------------- */
#define SETZE_U_M 		 1    
#define MOTOR_AUS 		 2
#define POS_VERGL 		 3
#define GESCHW_VERGL 		 4
#define BESCHL_VERGL 		 5
#define ABSTAND_TCP 		 6
#define ABSTAND_GREIFER 	 7
#define ABSTAND_GREIFER_OUT 	 8
#define DREHE_ABST_SENSOR 	 9
#define VERGL_WAHR 		10
#define START_BLOCK_BEGIN 	11
#define START_BLOCK_END 	12
#define UNVERAENDERT 		13
#define STORE_SENS 		14
#define SET_TIME 		15
#define STORE_TIME 		16
#define COMP_SENS 		17
#define COMP_TIME 		18

/* -------- Modellspezifische Hilfskonstanten: -------- */
#define EQ 		1    /* Vergleichoperatoren     */
#define NE 		2    /* Vergleichoperatoren     */
#define GT 		3    /* Vergleichoperatoren     */
#define LT 		4    /* Vergleichoperatoren     */
#define MOTOR_NR 	0    /* Parameternummer         */
#define RAMPE 		0    /* Parameternummer         */
#define U_WERT 		1    /* Parameternummer         */
#define AKT_ANZ 	0    /* Parameternummer         */
#define VERGL_WERT 	0    /* Parameternummer         */
#define VERGL_OP 	1    /* Parameternummer         */
#define GELENK_NR 	2    /* Parameternummer         */
#define GRAD 		0    /* Parameternummer         */
#define SENS_NR 	1    /* Parameternummer         */
#define TAKT_ANZ 	2    /* Parameternummer         */
#define SENS_VAR_NR 	0    /* Parameternummer         */
#define ZEIT_VAR_NR 	0    /* Parameternummer         */
#define AKT_ANZAHL 	3    /* Parameternummer         */
#define TAKT_ANZAHL 	0    /* Parameternummer         */
#define PROZENT 	0    /* Parameternummer         */
#define VGL_OP          2    /* Parameternummer         */
#define AKT_ANZL 	2    /* Parameternummer         */


