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
// Behandlung von Beschr�nkungen

class constraints{
  int number;            // Anzahl Beschr�nkungen
  int imconstraints;     // Anzahl implizite Beschr�nkungen
  int exconstraints;     // Anzahl explizite Beschr�nkungen
  double *cdelta;        // Deltaumgebungen der Beschr�nkungen
  function *func;        // Beschr�nkungsfunktionen
  int *zuordnung;        // Zuordnung epl. Beschr�nkung --> Parameternummer

  BarriereType typ;      // Art der Barrierefunktion
  double my;             // Steuerparameter fuer die Straffunktionsverfahren
  double ny;             // Parameter fuer die Berechnung des Barriereterms
  double B;              // Barriereterm 
  double norm;		 // Normierung
  double ql1, ql2;       // die letzten beiden Werte der Zielfunktion
  double eps;            // Grenze fuer die Termination der Straffunktionsverfahren
  double* G;             // G-Wert der Nebenbedingungsfunktion
  double* Phi;           // f�r Bildung der Tendenzfkt
	
  // Einlesen der Beschr�nkungen aus dem Inifile
  void constraint_functions(int number,paramListe& params, merkmalListe& global);
  // Anzahl der impliziten und expliziten Beschr�nkungen ermitteln
  void count_constraints(paramListe& params, merkmalListe& global);
  // Deltaumgebung der Beschr�nkungen berechnen
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

  // Wert der j. Beschr�nkung im Punkt p (j>=1 !)
  double calculateConst(const vector &p, int j, paramListe& params);
  // Anzahl Beschr�nkungen zur�ckgeben
  int getnumber(void) { return(number); } ;
  // Anzahl impliziter Beschr�nkungen zur�ckgeben
  int getimconstraints(void) { return(imconstraints); } ;
  // Anzahl expliziter Beschr�nkungen zur�ckgeben
  int getexconstraints(void) { return(exconstraints); } ;
  // Gr��e der Deltaumgebung der i. Beschr�nkung zur�ckgeben
  double getdelta(int i) {return(cdelta[i]); } ;
  // zur i. Beschr�nkung geh�rigen Parameter zur�ckgeben
  int getconparam(int i) {return(zuordnung[i]); } ;
  // Array mit Beschr�nkungsfunktionen zur�ckgeben
  function *getfunction(void) {return(func); } ;

  // Barriereterm ermitteln
  double add_barrier(vector PP, paramListe& params);

  // Barriereterm zur�ckgeben
  double get_barrier() {return B;};
  // Pr�ft, ob eine Beschr�nkung verletzt wird, oder nicht!
  bool BarrierisnotValid(vector PP, paramListe& params);
  // letzten Zielfunktionswert setzen
  void setLastQual(double q);
  // die letzten beiden Zielfunktionswerte setzen
  void setLastQual(double q1, double q2);
  // Pr�ft, ob die Straffunktionsverfahren terminieren k�nnen, oder nicht.
  bool BarrierTerminate(double B);
  // Art der Barrierefunktion zur�ckgeben
  BarriereType getType() {return typ;}; 
};

#endif
