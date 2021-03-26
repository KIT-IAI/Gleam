/* -----------------------------------------------------------------------------
GLEAM/AE                 Anwendungsspezifisches Header-File      Version: V1.2.0

                         LeSaK-handlungsMODell fuer HYgleam

Package: simu                     File: lskmodhy.h            Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
-------------------------------------------------------------------------------- 

Konstantendeklarationen der Handlungen entsprechend der Codierung in "activity"
und Modellfiles der HyGLEAM-Variante von LESAK/Plus.
Typdeklarationen geometrischer Datentypen, soweit sie ausserhalb des Simulators 
benoetigt werden.
    
-------------------------------------------------------------------------------- 
Aenderungen:
 1. Die Motornummern sind Bestandteil des Handlungsmodells. Alle sensor-
    bezogenen Aktionen wurden gestrichen. Gilt nur fuer das Mitsu-
    bishi-Modell!                                                    WJ 20.01.01
 2. Streichung der TSK-Daten-Indices.                                WJ 22.11.16
-------------------------------------------------------------------------------- 
Status: o.k.             Autoren: C.Blume / W.Jakob            Datum: 22.11.2016
-------------------------------------------------------------------------------- */


/* ----------------------------- Handlungscodes: ---------------------------- */
#define MOTOR_1 		 1    
#define MOTOR_2 		 2    
#define MOTOR_3 		 3    
#define MOTOR_4 		 4    
#define MOTOR_5 		 5    
#define MOTOR_1_AUS 		 6
#define MOTOR_2_AUS 		 7
#define MOTOR_3_AUS 		 8
#define MOTOR_4_AUS 		 9
#define MOTOR_5_AUS 		10
#define START_BLOCK_BEGIN 	11
#define START_BLOCK_END 	12
#define UNVERAENDERT 		13

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

