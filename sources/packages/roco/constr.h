#ifndef constraints_h
#define constraints_h

#include "schalter.h"   /* wj! */
#include "param.h"
#include "vector.h"
#include "merkmal.h"
#include "function.h"

enum BarriereType { None_, Inv_, Log_, Exp_, Tnd_ };


// Klasse constraints
//
// Behandlung von Beschränkungen

class constraints{
  int number;            // Anzahl Beschränkungen
  int imconstraints;     // Anzahl implizite Beschränkungen
  int exconstraints;     // Anzahl explizite Beschränkungen
  double *cdelta;        // Deltaumgebungen der Beschränkungen
  function *func;        // Beschränkungsfunktionen
  int *zuordnung;        // Zuordnung epl. Beschränkung --> Parameternummer

  BarriereType typ;      // Art der Barrierefunktion
  double my;             // Steuerparameter fuer die Straffunktionsverfahren
  double ny;             // Parameter fuer die Berechnung des Barriereterms
  double B;              // Barriereterm 
  double norm;		 // Normierung
  double ql1, ql2;       // die letzten beiden Werte der Zielfunktion
  double eps;            // Grenze fuer die Termination der Straffunktionsverfahren
  double* G;             // G-Wert der Nebenbedingungsfunktion
  double* Phi;           // für Bildung der Tendenzfkt
	
  // Einlesen der Beschränkungen aus dem Inifile
  void constraint_functions(int number,paramListe& params, merkmalListe& global);
  // Anzahl der impliziten und expliziten Beschränkungen ermitteln
  void count_constraints(paramListe& params, merkmalListe& global);
  // Deltaumgebung der Beschränkungen berechnen
  void calculateDelta(paramListe& params, double delta);


public:
  // Standard-Constructor
  constraints()
    :number(0),imconstraints(0),exconstraints(0),cdelta(NULL),func(NULL),zuordnung(NULL)
    {typ = None_; B = 0.0; my = 0.0; ny = 0.0; norm = 1.0; ql1 = 0.0; ql2 = 0.0; eps = 0.0; G = NULL; Phi = NULL;}
  // Constructor mit init-Aufruf
  constraints(paramListe& params, merkmalListe& global, double delta)
    :number(0),imconstraints(0),exconstraints(0),cdelta(NULL),func(NULL),zuordnung(NULL)
    {init(params, global, delta);}
  // Initialisierung des constraints-Objekts
  void init(paramListe& params, merkmalListe& global, double delta=1e-4);
  // Destructor
  ~constraints(){ delete[] func; delete [] cdelta; delete [] zuordnung;delete [] G; if(Phi != NULL) delete [] Phi;} ;

  // Wert der j. Beschränkung im Punkt p (j>=1 !)
  double calculateConst(const vector &p, int j, paramListe& params);
  // Anzahl Beschränkungen zurückgeben
  int getnumber(void) { return(number); } ;
  // Anzahl impliziter Beschränkungen zurückgeben
  int getimconstraints(void) { return(imconstraints); } ;
  // Anzahl expliziter Beschränkungen zurückgeben
  int getexconstraints(void) { return(exconstraints); } ;
  // Größe der Deltaumgebung der i. Beschränkung zurückgeben
  double getdelta(int i) {return(cdelta[i]); } ;
  // zur i. Beschränkung gehörigen Parameter zurückgeben
  int getconparam(int i) {return(zuordnung[i]); } ;
  // Array mit Beschränkungsfunktionen zurückgeben
  function *getfunction(void) {return(func); } ;

  // Barriereterm ermitteln
  double add_barrier(vector PP, paramListe& params);

  // Barriereterm zurückgeben
  double get_barrier() {return B;};
  // Prüft, ob eine Beschränkung verletzt wird, oder nicht!
  bool BarrierisnotValid(vector PP, paramListe& params);
  // letzten Zielfunktionswert setzen
  void setLastQual(double q);
  // die letzten beiden Zielfunktionswerte setzen
  void setLastQual(double q1, double q2);
  // Prüft, ob die Straffunktionsverfahren terminieren können, oder nicht.
  bool BarrierTerminate(double B);
  // Art der Barrierefunktion zurückgeben
  BarriereType getType() {return typ;}; 
};

#endif
