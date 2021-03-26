#ifndef vector_h
#define vector_h

#include <stdlib.h>
#include <math.h>
#include <iostream>
// #include <iostream.h>     wj!

#include "schalter.h"
#include "bcc.h"

// #include "workaround.h"  // Nur nötig zum Compilieren mit Visula C++ 6

// Klasse vector
//
// Mathematischer Vektor
// - double-Elemente
// - grundlegende Vektoroperationen

class matrix;   /* neu, 14.7.06 wj */

class vector
{	friend class matrix; 
	
	double* element;            // double-Pointer zum internen speichern der Elemente
	unsigned int n;             // Dimension des vectors

public:
	vector() :element(0), n(0) {}                             // Standard-Constructor
	vector(const vector& v) :element(0) {*this=v;}            // Copy-Constructor
	vector(const unsigned int l, const double initval=0.0);   // Constructor: Dimension und Anfangswerte vorgebbar
	vector(const unsigned int, const double []);              // Constructor: double-array dem vector zuweisen
	vector(const matrix& mat);                                // Constructor: matrix dem vector zuweisen
	~vector() {if(element) delete[] element;}                 // Destructor

	double& operator[](const unsigned int);                   // Elementzugriffs-Operator
	const double operator[](const unsigned int) const;        // Elementzugriffs-Operator für konstanten Objekt
	double* return_satz() {return element;}                   // gibt vector als double-array zurück

	unsigned int dim() const {return n;}                      // Dimension des vectors
	void dim(const unsigned int l,const bool save=true);      // Neudimensionierung des vectors
	                                                          //   save: bisherigen Inhalt retten
	void set(const double);                                   // alle Elemente auf einen Wert setzen
	void axis(const unsigned int);                            // Achseneinheitsvektor erzeugen
	void exchange(const unsigned int,const unsigned int);     // zwei Elemente vertauschen
	void append(const double);                                // double an den vector anhängen
	void append(const vector&);                               // vector an den vector anhängen
	void insert(const unsigned int,const double);             // double in den vector einfügen
	
	double l2norm() const {return sqrt((*this)*(*this));}     // Euklidische Norm des Vektors

 	vector& operator=(const vector&);                         // Zuweisungs-Operator
	vector& operator+=(const vector&);                        // vector zum vector hinzuaddieren
	vector& operator-=(const vector&);                        // vector vom vector subtrahieren
	vector& operator*=(const vector&);                        // vector elementweise mit vector multiplizieren
	vector& operator/=(const vector&);                        // vector elementweise durch vector dividieren
	vector& operator*=(const double);                         // vector elementweise mit double multiplizieren
	vector& operator/=(const double);                         // vector elementweise durch double dividieren
	
	friend bool operator==(const vector&,const vector&);      // Vergleich auf Gleichheit
	friend bool operator!=(const vector& v1,const vector& v2) // Vergleich auf Ungleichheit
    {return !(v1==v2);}
	friend bool operator<(const vector&,const vector&);       // lexikographischer Vergleich
	friend bool operator>(const vector&,const vector&);       // lexikographischer Vergleich
	friend vector operator+(const vector&,const vector&);     // Vektoraddition
	friend vector operator-(const vector&,const vector&);     // Vektorsubtraktion
	friend vector operator-(const vector& v)                  // Vorzeichenumkehr
	{return (-1.0*v);}	
	friend vector operator*(const double,const vector&);      // double mit vector multiplizieren
	friend vector operator*(const vector&,const double);      // vector mit double multiplizieren
	friend vector operator/(const vector&,const double);      // vector durch double dividieren
	friend double operator*(const vector&,const vector&);     // Skalarprodukt
	friend vector product(const vector&,const vector&);       // vectoren elementweise multiplizieren
	friend vector quotient(const vector&,const vector&);      // vectoren elementweise dividieren	

#ifdef BS_DOS	
	friend ostream& operator<<(ostream&, const vector&);       // Ausgabe-Operator  wj!
	friend istream& operator>>(istream&, vector&);             // Eingabe-Operator  wj!
#else
	friend std::ostream& operator<<(std::ostream&, const vector&);      // Ausgabe-Operator
	friend std::istream& operator>>(std::istream&, vector&);            // Eingabe-Operator
#endif /* BS_DOS */
};	

vector product(const vector&,const vector&);
vector quotient(const vector&,const vector&);



// Klasse PointWValue
//
// vector und double zusammen speichern
// - Punkt mit dazugehörigem Funktionswert speichern
// - ermöglicht einfache Sortierung

class PointWValue 
{
public:
	vector Point;               // Punkt
	double Value;               // Funktionswert
	int Index;                  // Index zur Identifikation eines PWV

	PointWValue ()                                            // Standard-Constructor
		: Point(), Value(0.0), Index(0) {}
		
	PointWValue (const PointWValue &PWV)                      // Copy-Constructor
		: Point(PWV.Point), Value(PWV.Value), Index(PWV.Index) {}
	
	PointWValue (unsigned int l, double initval=0.0)          // Constructor: Dimension des Punkts vorgebbar
		: Point(l, initval), Value(0.0), Index(0) {}
	PointWValue (unsigned int l, const double *array, PointWValue* Vertex)         // Constructor: double-array als Punkt verwenden
		: Point(l, array), Value(0.0), Index(0) {}
	~PointWValue(){}
	PointWValue operator= (const PointWValue &PWV)            // Zuweisungs-Operator
		{ this->Point = PWV.Point; this->Value = PWV.Value; return *this; }
};

int compare_PointWValue (const void *a, const void *b);     // Funktionswerte vergleichen (benötigt von qsort)

/*class PointWValueList
{

private:
  PointWValue *vwlist;
  int _n;
  
public:
  PointWValueList () : vwlist(NULL), _n(0) {}                    // Standard-Constructor
  ~paramListe () { if (vwlist) delete [] vwlist; }          // Destructor
  
  int add (char *name);                                // Parameter "name" hinzufügen
  int number_of_params () const { return _n; }              // Anzahl Parameter
  param& operator[] (int);                                  // auf einen Parameter in der Liste zugreifen
};*/


#endif
