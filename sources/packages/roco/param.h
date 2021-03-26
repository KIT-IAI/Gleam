#ifndef _PARAM_H_
#define _PARAM_H_

#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "merkmal.h"


// Klasse param
//
// f�r Parameter mit Namen und einer Liste von merkmalen

class param : public merkmalListe 
{
private:
  char* _name;
  
public:
  param () : merkmalListe(), _name(NULL) {}                 // Standard-Constructor
  param (char *name) : merkmalListe()                       // Constructor: Parametername vorgebbar 
    { _name = strdup(name); }
  ~param () { if (_name) free (_name); }                    // Destructor
  
  param& operator= (const param &p);                        // Zuweisungs-Operator
  void setname (char *n)                                    // Parameternamen setzen
    { if (_name) free (_name); _name = strdup(n); }
  const char* getname () const { return _name; }            // Parameternamen zur�ckgeben
};


// Klasse paramListe
//
// Liste von Parametern

class paramListe 
{
private:
  param *pliste;
  int _n;
  
public:
  paramListe () : pliste(NULL), _n(0) {}                    // Standard-Constructor
  ~paramListe () { if (pliste) delete [] pliste; }          // Destructor
  
  int addparam (char *name);                                // Parameter "name" hinzuf�gen
  int number_of_params () const { return _n; }              // Anzahl Parameter
  param& operator[] (int);                                  // auf einen Parameter in der Liste zugreifen
};


// Inifiles einlesen
//   OptimizationIniName: Name des Inifiles f�r das Verfahren
//   FunctionIniName: Name des Inifiles f�r die Zielfunktion
//   pl: paramListe, in der die Parametereigenschaften abgelegt werden sollen
//   ml: merkmalListe, in der die allgemeinen Eigenschaften abgelegt werden sollen

void ReadIni (char *OptimizationIniName, char *FunctionIniName, paramListe &pl, merkmalListe &ml);

#endif
