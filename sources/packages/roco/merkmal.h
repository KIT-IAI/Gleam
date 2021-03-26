#ifndef _MERKMAL_H_
#define _MERKMAL_H_

#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "bcc.h"

// Klasse merkmal
//
// Speichern eines Merkmalnamens mit dazugehörigem Wert
// - int, double und string als Rückgabewerte

class merkmal 
{
private:
  char* _name;
  char* _value;
  
public:
  merkmal () : _name(NULL), _value(NULL) {}                   // Standard-Constructor
  merkmal (const merkmal &m);                                 // Copy-Constructor
  ~merkmal ()                                                 // Destructor
    { if (_name) free (_name); if (_value) free (_value); }

  merkmal& operator= (const merkmal &m);                      // Zuweisungs-Operator

  void setname (char *name)                                   // Namen setzen
    { if (_name) free(_name); _name = strdup(name); }
  char* getname () const                                      // Namen zurückgeben
    { return _name; }
  void setvalue (double v);                                   // Wert auf double setzen
  void setvalue (char *v)                                     // Wert auf string setzen
    { if (_value) free(_value); _value = strdup(v); }
  bool getvalue (int &v) const                                // int-Wert in v zurückgeben
    { char *endptr; v=strtol(_value, &endptr, 10); return (_value != endptr);}
  bool getvalue (double &v) const                             // double-Wert in v zurückgeben
    { char *endptr; v=strtod(_value, &endptr); return (_value != endptr);}
  
  bool getvalue (char *v) const                               // string-Wert in v zurückgeben (v muss auf ein ausreichend großes char-Array zeigen)
    { if (_value) strcpy(v, _value); else strcpy(v, ""); return true;}
};


// Klasse merkmalListe
//
// Liste von merkmalen
// - Zugriff auf Wert über Bezeichnung des merkmals
// - int, double und string als Rückgabewerte

class merkmalListe 
{
protected:
  merkmal *mliste;
  int _n;
  
public:
  merkmalListe ()  : mliste(NULL), _n(0) {}                 // Standard-Constructor
  ~merkmalListe () { if (mliste) delete [] mliste; }        // Destructor
  
  void addmerkmal (char *name, double value);               // merkmal "name" mit double-Wert hinzufügen
  void addmerkmal (char *name, char *value);                // merkmal "name" mit string-Wert hinzufügen
  bool setvalue (char *name, double value);                 // merkmal "name" auf double-Wert setzen
  bool setvalue (char *name, char *value);                  // merkmal "name" auf string-Wert setzen
  bool getvalue (char *name, int &v) const;                 // int-Wert von merkmal "name" in v zurückgeben
  bool getvalue (char *name, double &v) const;              // double-Wert von merkmal "name" in v zurückgeben
  bool getvalue (char *name, char *v) const;                // string-Wert von merkmal "name" in v zurückgeben
  bool defined (char *name) const;                          // prüfen, ob merkmal "name" definiert
  merkmal& operator [] (int);                               // auf ein merkmal in der merkmalListe zugreifen
  int number_of_merkmal() const { return _n; }              // Anzahl der merkmale
};

#endif
