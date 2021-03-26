#ifndef matrix_h
#define matrix_h

#include <iostream>
// #include <iostream.h>    wj!
#include "vector.h"


// Klasse matrix
//
// Mathematische Matrix
// - double-Elemente
// - grundlegende Matrixoperationen
// - basiert auf der Klasse vector

class matrix
{ 
	vector* rowvec;             // vector-Pointer zum internen speichern der Zeilenvektoren
	unsigned int m,n;           // Zeilen und Spalten der matrix

public:
	matrix() :rowvec(0),m(0),n(0) {}                          // Standard-Constructor
	matrix(const matrix& mat) :rowvec(0) {*this=mat;}         // Copy-Constructor
	matrix(const unsigned int r,const unsigned int c,const double initval=0.0)  // Constructor: Dimension und Anfangswerte vorgebbar
		// :rowvec(new vector[r](c,initval)), m(r), n(c) {}   // Diese Art der Initialisierung ist eine GCC-Erweiterung!  
		:rowvec(new vector[r]), m(r), n(c) {         
		for (unsigned int i=0; i<r; i++) 
			rowvec[i] = vector (c, initval);
		}
			
	matrix(const vector& vec);                                // Constructor: vector der matrix zuweisen
	~matrix() {if(rowvec) delete[] rowvec;}                   // Destructor

	operator double();                                        // 1x1-matrix als double casten
	
	vector& operator[](const unsigned int);                   // auf Zeilenvektor zugreifen
	const vector& operator[](const unsigned int) const;       // auf Zeilenvektor zugreifen bei konstantem Objekt
	vector row(const unsigned int r) const {return rowvec[r];}// r. Zeile zurückgeben
	vector col(const unsigned int c) const;                   // c. Spalte zurückgeben
		
	unsigned int rows() const {return m;}                     // Anzahl Zeilen
	unsigned int cols() const {return n;}                     // Anzahl Spalten
	void dim(const unsigned int r,const unsigned int c,bool save=true); // Neudimensionierung der matrix
	                                                                    //   save: bisherigen Inhalt retten
	void set(const double);                                   // alle Elemente auf einen Wert setzen
	void identity(const unsigned int);                        // Einheitsmatrix erzeugen
	
	matrix T() const;                                         // Transponierte der matrix
	void rowexchange(const unsigned int,const unsigned int);  // zwei Zeilen vertauschen
	void colexchange(const unsigned int,const unsigned int);  // zwei Spalten vertauschen
	void rowappend(const vector&);                            // eine Zeile anhängen
	void colappend(const vector&);                            // eine Spalte anhängen
	void rowinsert(const unsigned int, const vector&);        // eine Zeile einfügen
	
	matrix& operator=(const matrix&);                         // Zuweisungs-Operator
	matrix& operator+=(const matrix&);                        // matrix zur matrix hinzuaddieren
	matrix& operator-=(const matrix&);                        // matrix von matrix subtrahieren
	matrix& operator*=(const double);                         // matrix elementweise mit double multiplizieren
	matrix& operator/=(const double);                         // matrix elementweise durch double dividieren

	friend bool operator==(const matrix&,const matrix&);      // Vergleich auf Gleichheit
	friend bool operator!=(const matrix& mat1,const matrix& mat2) // Vergleich auf Ungleichheit
		{return !(mat1==mat2);}
	friend matrix operator+(const matrix&,const matrix&);     // Matrizenaddition
	friend matrix operator-(const matrix&,const matrix&);     // Matrizensubtraktion
	friend matrix operator-(const matrix& mat)                // Vorzeichenumkehr
		{return (-1.0*mat);}
	friend matrix operator*(const double,const matrix&);      // double mit matrix multiplizieren
	friend matrix operator*(const matrix&,const double);      // matrix mit double multiplizieren
	friend matrix operator/(const matrix&,const double);      // matrix durch double dividieren
	friend matrix operator*(const matrix&,const matrix&);     // Matrizenmultiplikation

#ifdef BS_DOS	
	friend ostream& operator<<(ostream&, const matrix&);       // Ausgabe-Operator
	friend istream& operator>>(istream&, matrix&);             // Eingabe-Operator
#else
	friend std::ostream& operator<<(std::ostream&, const matrix&);  // Ausgabe-Operator 
	friend std::istream& operator>>(std::istream&, matrix&);        // Eingabe-Operator
#endif /* BS_DOS */
};

#endif
