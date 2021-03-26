#include <stdlib.h>
#include <stdio.h>
// #include <iostream.h>    wj!
#include <iostream>

#include "merkmal.h"


// ************************************************************* //
// Standard-Constructor                                          //
// ************************************************************* //

merkmal::merkmal (const merkmal &m) 
{ 
  if (_name) free (_name);
  if (_value) free (_value);        
  _name = strdup(m._name); 
  if (m._value) 
    _value = strdup(m._value); 
  else 
    _value = NULL;
}


// ************************************************************* //
// Zuweisungs-Operator                                           //
// ************************************************************* //

merkmal& merkmal::operator= (const merkmal &m)
{
  if (this != &m)                       // Nur wenn Objekte verschieden
  {
	 if (_name) free (_name);             // Speicher des alten Names freigeben
	 if (_value) free (_value);           // Speicher von value freigeben
	 _name = strdup(m._name);             // Daten kopieren
	 if (m._value) 
		_value = strdup(m._value); 
	 else 
		_value = NULL;
  }
  return *this;
}


// ************************************************************* //
// Wert auf double setzen                                        //
// ************************************************************* //

void merkmal::setvalue (double v)
{
  char buffer[80];
  sprintf(buffer, "%10E", v);
  if (_value) free (_value);
  _value = strdup(buffer);
}


// ************************************************************* //
// merkmal "name" mit double-Wert hinzufügen                     //
// ************************************************************* //

void merkmalListe::addmerkmal (char *name, double value)
{
	char buffer[512];
	sprintf(buffer, "%10E", value);
	addmerkmal (name, buffer);
}


// ************************************************************* //
// merkmal "name" mit string-Wert hinzufügen                     //
// ************************************************************* //

void merkmalListe::addmerkmal (char *name, char *value)
{
  if (this->defined(name))
  {
    this->setvalue (name, value);
  }
  else
  {
    merkmal *mliste1 = new merkmal[_n + 1];

	 for (int i = 0; i < _n; i++)
   	mliste1[i]=mliste[i];

	 mliste1[_n].setname(name);
	 mliste1[_n].setvalue(value);

	 if (mliste != NULL)
   	delete [] mliste;

	 mliste = mliste1;
	 _n++;
  }
}


// ************************************************************* //
// merkmal "name" auf double-Wert setzen                         //
// ************************************************************* //

bool merkmalListe::setvalue (char *name, double value)
{
  for (int i = 0; i < _n; i++)
    if ( strcmp(mliste[i].getname(), name) == 0 ) 
	  {
      mliste[i].setvalue(value);
      return true;
    }
  return false;
}


// ************************************************************* //
// merkmal "name" auf string-Wert setzen                         //
// ************************************************************* //

bool merkmalListe::setvalue (char *name, char *value)
{
  for (int i = 0; i < _n; i++)
    if ( strcmp(mliste[i].getname(), name) == 0 ) 
	  {
      mliste[i].setvalue(value);
      return true;
    }
  return false;
}


// ************************************************************* //
// int-Wert von merkmal "name" in v zurückgeben                  //
// ************************************************************* //

bool merkmalListe::getvalue (char *name, int &v) const
{
  for (int i = 0; i < _n; i++)
    if ( strcmp(mliste[i].getname(), name) == 0 )
      return mliste[i].getvalue(v);
  return false;
}


// ************************************************************* //
// double-Wert von merkmal "name" in v zurückgeben               //
// ************************************************************* //

bool merkmalListe::getvalue (char *name, double &v) const
{
  for (int i = 0; i < _n; i++)
    if ( strcmp(mliste[i].getname(), name) == 0 )
      return mliste[i].getvalue(v);
  return false;
}


// ************************************************************* //
// string-Wert von merkmal "name" in v zurückgeben               //
// ************************************************************* //

bool merkmalListe::getvalue (char *name, char *s) const
{
  for (int i = 0; i < _n; i++)
    if ( strcmp(mliste[i].getname(), name) == 0 )
      return mliste[i].getvalue(s);
  return false;
}


// ************************************************************* //
// prüfen, ob merkmal "name" definiert                           //
// ************************************************************* //

bool merkmalListe::defined (char *name) const
{
  for (int i = 0; i < _n; i++)
    if ( strcmp(mliste[i].getname(), name) == 0 )
      return true;
  return false;
}


// ************************************************************* //
// auf ein merkmal in der merkmalListe zugreifen                 //
// ************************************************************* //

merkmal& merkmalListe::operator[] (int i)
{
  if ((i < 0) || (i >= _n)) 
  {
#ifdef BS_DOS	
    cerr << "Class merkmalListe: wrong index." << endl;
#else
    std::cerr << "Class merkmalListe: wrong index." << std::endl;
#endif /* BS_DOS */
    exit(1);
  }

  return mliste[i];
}
