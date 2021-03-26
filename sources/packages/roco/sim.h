#ifndef _SIM_H_
#define _SIM_H_

/* Version ohne TCL, Interpolation und GNUPlot!!! */

#include <stdlib.h>
// #include <fstream.h>/* wj! */
#include <fstream>


#include "vector.h"
#include "matrix.h"
#include "function.h"
#include "merkmal.h"
#include "param.h"
#include "constr.h"
#include "getfctp.h"


// Klasse sim
//
// enthält alle Angaben zur Zielfunktion, die in den einzelnen Verfahren benötigt werden
// - erstellt parameter- und merkmalListe aus den Inifiles
// - speichert alle Funktionsaufrufe und greift ggf. auf bereits berechnete Werte zurück
// - enthält die Interpolationsfunktion
// - gestattet verschiedene Normierungsarten
// - Berechnung von Beschränkungen

class sim
{
private:
	int dimension;              // Anzahl Parameter der Zielfunktion
	int getvaluecalls;          // Anzahl tatsächlicher Zielfunktionsauswertungen
	int simcalls;               // Anzahl angefordertet Zielfunktionsauswertungen
	int FunctionType;           // 0 = analyt. Zielfunk. im Inifile, 1 = Zielfunktion im C++-Code, 2 = Modell
	int max_iter;
	int pfile;
	function Function;          // function Objekt für analytisch angegebene Funktionen
	FunctionP_Type FunctionP;   // Pointer auf zu verwendende Zielfunktion
    
public:
	char *testname;             // Name der Testfunktion

	merkmalListe properties;    // allgemeine Eigenschaften
	paramListe parameters;      // Eigenschaften der Parameter
	matrix samplepoints;        // bereits untersuchte Punkte
	vector samplevalues;        // dazugehörige Funktionswerte
	constraints cons;           // alle Beschränkungen
	bool noreevaluations;       // wiederholte Berechnung in einem Punkt vermeiden

#ifdef BS_DOS
	ofstream output;            // ausführliche Ausgabedatei
	ofstream verlauf;           // Verlaufsdatei (enthält nur Punkte und Werte)
#else
	std::ofstream output;       // ausführliche Ausgabedatei
	std::ofstream verlauf;      // Verlaufsdatei (enthält nur Punkte und Werte)
#endif /* BS_DOS */

	vector start;               // Startpunkt
	vector offset;              // für Normierung: Verschiebung des KoS
	vector scale;               // für Normierung: Skalierung des KoS


  // Standard-Constructor
	sim ()                                                 
		: 	dimension(0),
			getvaluecalls(0),
			simcalls(0),
			FunctionType(1),
			max_iter(10000),
			pfile(1),
			FunctionP(NULL),
			testname(NULL),
			noreevaluations(true)
    {}
  
  // Constructor mit Initialisierung
  //   OptimizationIniName: Name des Inifiles für das Verfahren
  //   FunctionIniName: Name des Inifiles für die Zielfunktion
  //   test: Name der Zielfunktion
  //   outname: Name der Ausgabedatei
  //   verlaufname: Name der Verlaufsdatei
  //   noreeval: wiederholte Berechnung in einem Punkt vermeiden
  //   norm: Art der Normierung (siehe dort)
	sim (char *OptimizationIniName, char *FunctionIniName, char *test, char *outname, bool noreeval = true, int norm = 1)
		:	dimension(0),
			getvaluecalls(0),
			simcalls(0),
			FunctionType(1),
			max_iter(10000),
			pfile(1),
			FunctionP(NULL),
			testname(NULL),
			noreevaluations(noreeval)
		{ Init(OptimizationIniName, FunctionIniName, test, outname, noreeval, norm); }

  // Destructor
	~sim()                                                  // Destruktor
		{ if (testname) free (testname); }
    
  // Initialisierung des Objekts
  //   OptimizationIniName: Name des Inifiles für das Verfahren
  //   FunctionIniName: Name des Inifiles für die Zielfunktion
  //   test: Name der Zielfunktion
  //   outname: Name der Ausgabedatei
  //   verlaufname: Name der Verlaufsdatei
  //   noreeval: wiederholte Berechnung in einem Punkt vermeiden
  //   norm: Art der Normierung (siehe dort)
	void Init (char *OptimizationIniName, char *FunctionIniName, char *test, char *outname, bool noreeval = true, int norm = 1);


  // Zielfunktionswert im Punkt p (normiert) berechnen
	double operator() (const vector &p);

  // Punkt p (normiert) auf Gültigkeit überprüfen
  //   gibt Nr. der letzten Beschränkung zurück, die verletzt ist, sonst 0
	int infeasible(const vector &p);
  // Wert der i. Beschränkung (i>=1) im Punkt p (normiert)
	double consvalue(int i, const vector &p)
		{ return cons.calculateConst(transform(p),i,parameters); }

  // Ermitteln der Transformationsbeziehung
  //   0: ohne Normierung
  //   1: 
	void transform(int);
  // transformiert p (normiert) in den Originalbereich
	vector transform(const vector& p)
		{ return (offset+product(p,scale)); }
  // transformiert p (unnormiert) in den Bildbereich
	vector invtransform(const vector& p)
		{ return (quotient(p-offset,scale)); }
  // Anzahl Parameter der Zielfunktion zurückgeben
	int dim()
		{ return dimension; }
  // Anzahl tatsächlicher Zielfunktionsauswertungen zurückgeben
	int NumberOfGetValueCalls () const
		{ return getvaluecalls; }
  // Anzahl angeforderter Zielfunktionsauswertungen zurückgeben
	int NumberOfSimCalls () const
		{ return simcalls; }

private:
	double GetFunctionValue (/* const wj */ vector &p);
};

#endif
