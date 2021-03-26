#include "constr.h"
#include "vector.h"
#include "param.h"
#include "merkmal.h"
#include "function.h"
#include <stdio.h>
// #include "stdio.h"     wj!


// ************************************************************* //
// Initialisierung des constraints-Objekts                       //
// ************************************************************* //

void constraints::init(paramListe& params, merkmalListe& global, double delta)
{
  number = 0; 
  imconstraints = 0; 
  exconstraints = 0;
  
  // Anzahl der impliziten und expliziten Beschränkungen ermitteln
  count_constraints(params, global);

  if(func) 
    delete[] func;
  func = new function[number];

  if(zuordnung) 
    delete[] zuordnung;
  zuordnung = new int[exconstraints];

  // Einlesen der Beschränkungen aus dem Inifile
  constraint_functions(number,params,global); 

  // für die Barriere- und Tendenzfunktion
  if(cdelta) 
    delete[] cdelta;
  cdelta = new double[number];

  calculateDelta(params,delta);

  G = new double[number];
  B = 0.0;
  char t[20];

  if (global.defined("conshandl")) {
    global.getvalue("conshandl", t);

    if (strcmp(t,"inv") == 0)
      typ = Inv_;
    if (strcmp(t,"exp") == 0)
      typ = Exp_;
    if (strcmp(t,"tnd") == 0) 
      typ = Tnd_;
    if (strcmp(t,"log") == 0) 
      typ = Log_;
  } else 
   typ = None_;

  norm = 1.0;

  switch (typ) {
    case Inv_:
      my = 1e-27;
      eps = 0.1;
      break;
    case Log_:
      my = 1e-4;
      eps = 0.1;
      break;
    case Exp_:
      break;
    case Tnd_:
      Phi = new double[number];
      ny = 0.0;
      ql1 = ql2 = 0.0;
      break;
    case None_:
      my = 1;
      break;
   }
   if (global.defined("my"))
     global.getvalue("my", my);
   if (global.defined("eps"))
     global.getvalue("eps", eps);
} 


// ************************************************************* //
// Anzahl der impliziten und expliziten Beschränkungen ermitteln //
// ************************************************************* //

void constraints::count_constraints(paramListe& params, merkmalListe& global)
{  
  int k;

  // Die folgende Schleife zählt die funktionalen Nebenbedingungen der Form Gx = f(x1,..., xn)
  for (k = 0; k < global.number_of_merkmal(); k++) {
    if (strchr("gG",global[k].getname()[0]) && strchr(global[k].getname(),'(') && strchr(global[k].getname(),')')) {
      number++;
      imconstraints++;
    }    
  }

  // Die folgende Schleife zählt die Nebenbedingungen der Form: ai <= xi <= bi
  for (k = 0; k < params.number_of_params(); k++ ) {
    if (params[k].defined("lowerbound")){
      number++;
      exconstraints++;
    }
    if (params[k].defined("upperbound")){
      number++;
      exconstraints++;
    }
  }
}


// ***************************************************************** //
// Einlesen der Beschränkungen aus dem Inifile                       //
// ***************************************************************** //

void constraints::constraint_functions(int number_of_constraints,paramListe& params, merkmalListe& global)
{  
  int k, k1 = 0;
  
  // Einlesen der Nebenbedingungen der Form: ai <= xi <= bi mittels Klasse "function"
  for (k = 0; k < params.number_of_params(); k++) {
    if (params[k].defined("lowerbound"))  {
      double lb;
      params[k].getvalue("lowerbound",lb);
      func[k1] = (string)"Gl("+params[k].getname()+")="+params[k].getname()+"-("+str(lb)+")";
      zuordnung[k1] = k; 
      k1++;
    }    
    if (params[k].defined("upperbound"))  {
      double ub;
      params[k].getvalue("upperbound",ub);
      func[k1] = (string)"Gu("+params[k].getname()+")="+str(ub)+"-("+params[k].getname()+")";
      zuordnung[k1] = k; 
      k1++;
    }    
  } 
 
  // Einlesen der funktionalen Nebenbedingungen mittels Klasse "function"
  for (k = 0; k < global.number_of_merkmal(); k++) {
    if (strchr("gG",global[k].getname()[0]) && strchr(global[k].getname(),'(') && strchr(global[k].getname(),')')){
      char constraint[300];
      global[k].getvalue(constraint);
      func[k1++] = (string)global[k].getname()+"="+constraint;
    }
  }
}


// ************************************************************* //
// Wert der j. Beschränkung im Punkt p                           //
// ************************************************************* //

double constraints::calculateConst(const vector &p, int j, paramListe& params)
{
  for (int k = 0; k < params.number_of_params(); k++ ) {
    func[j-1][params[k].getname()] = p[k];
  }
  return (double)func[j-1];
}


// ************************************************************* //
// Deltaumgebung der Beschränkungen berechnen                    //
// ************************************************************* //

void constraints::calculateDelta(paramListe& params, double delta)
{
  double s=0.0, bound = 0.0;
  for (int c = 0; c < number; c++ ) {
    for (int k = 0; k < params.number_of_params(); k++ )
      func[c][params[k].getname()] = 0;
    bound = (double)func[c];
    if (bound != 0)
      cdelta[c] = fabs(bound) * delta;
    else
      for (int k = 0; k < params.number_of_params(); k++ )  {
         params[k].getvalue("start", s); 
         func[c][params[k].getname()] = s;
       }
       bound = (double)func[c];
       if(bound != 0) 
          cdelta[c] = fabs(bound) * delta;
        else                                      // es wurde aus dem Nullpunkt gestartet, bzw. Gk(x) = 0
          cdelta[c] = delta; 
  }
}


// ************************************************************* //
// Wert der Barrierefunktion ermitteln                           //
// ************************************************************* //

double constraints::add_barrier(vector PP, paramListe& params) {

 B = 0.0;
 for(int j = 1; j <= number; j++) {
    G[j-1] = calculateConst(PP, j, params);
    switch (typ) {
      case Inv_:
       B += my*(1.0/(G[j-1]));
        break;
      case Log_:
        B -= my*(log(G[j-1])); 
        break;
      case Exp_:
       B += my*(1.0/(G[j-1]));
      case Tnd_:
        B = 0.0;
        break;
      case None_:
        break;
    }
  }
  return B; 
}


// ************************************************************* //
// Prüft, ob eine Beschränkung verletzt wird, oder nicht!	 //                                                          //
// ************************************************************* //

bool constraints::BarrierisnotValid(vector PP, paramListe& params) {

 double Strafterm = 0.0;
 ny = 0.0;
 
 for(int j = 1; j <= number; j++) {
    G[j-1] = calculateConst(PP, j, params);
    switch (typ) {
      case Inv_:
        if (G[j-1] < 0)  
          return true;
        break;
      case Log_:
        if (G[j-1] < 0)  
          return true; 
        break;
      case Exp_:
        break;
      case Tnd_:
        if (G[j-1] < 0) {
          return true;
        } else if (G[j-1] >= cdelta[j-1])  {
          Phi[j-1] = 0.0;
        } else  {  
          ny = 1.0 - ((1.0/cdelta[j-1])*G[j-1]);
          Phi[j-1] = (3.0*ny) - (4.0*pow(ny,2.0)) + (2.0*pow(ny,3.0));
        }
        Strafterm +=  Phi[j-1]*(ql2 - ql1);
        if((j >= number) && ((ql1 + Strafterm) > ql2)) {
          return true;
        }
        break;
      case None_:
        break;
    } //Ende switch
  } //Ende der for-Schleife
  return false;
}


// ************************************************************* //
// letzten Zielfunktionswert setzen                              //
// ************************************************************* //

void constraints::setLastQual(double q) {
  ql2 = ql1;
  ql1 = q;
}


// ************************************************************* //
// die letzten beiden Zielfunktionswerte setzen                  //
// ************************************************************* //

void constraints::setLastQual(double q1, double q2) {
  ql1 = q1; ql2 = q2;
}


// ********************************************************************* //
// Prüft, ob die Straffunktionsverfahren terminieren können, oder nicht! //
// ********************************************************************* //

bool constraints::BarrierTerminate(double B) {
  if (((B * my) > eps) && ((typ == Log_) || (typ == Inv_) || (typ == Exp_))) {
    my = 0.1 * my;
    return false;
  }
  return true;
}
 
