#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
// #include <iostream.h>  /* wj !*/
// #include <fstream.h>   /* wj !*/

#include "vector.h"
#include "matrix.h"
#include "function.h"
#include "merkmal.h"
#include "param.h"
#include "constr.h"
#include "getfctp.h"
#include "strings.h"

#include "sim.h"

// ************************************************************* //
// Initialisierung des sim-Objekts                               //
//   OptimizationIniName: Name des Inifiles für das Verfahren    //
//   FunctionIniName: Name des Inifiles für die Zielfunktion     //
//   test: Name der Zielfunktion                                 //
//   outname: Name der Ausgabedatei                              //
//   verlaufname: Name der Verlaufsdatei                         //
//   noreeval: wiederholte Berechnung in einem Punkt vermeiden   //
//   norm: Art der Normierung (siehe dort)                       //
// ************************************************************* //

void sim::Init (char* OptimizationIniName, char *FunctionIniName, char *test, char *outname, bool noreeval, int norm)
{
  int ii;   /* wj! */
  double value=0.0;
  char str[256];

  if (testname) free (testname);
  testname = strdup(test);

  // Inifiles einlesen
  ReadIni(OptimizationIniName, FunctionIniName,parameters,properties);

  // Beschränkungen initialisieren
  cons.init(parameters,properties);

  if(properties.getvalue("pfile",value))
    pfile = (int)value;

  // Ausgabedateien öffnen
  if(pfile){
    output.open(outname);
    if(!output)
    {
#ifdef BS_DOS
      cerr<<outname<<" konnte nicht angelegt werde!"<<endl;
#else
      std::cerr<<outname<<" konnte nicht angelegt werde!"<<std::endl;
#endif /* BS_DOS */
      exit(1);
    }
    output.precision(20);
#ifdef BS_DOS
    output.setf(ios::scientific | ios::showpos);
#else
    output.setf(std::ios::scientific | std::ios::showpos);
#endif /* BS_DOS */
  }

  // Werte aus den Inifiles den Variablen zuweisen
  noreevaluations=noreeval;

  if(properties.getvalue("function",str))        // Zielfunktion steht analytisch im Ini-File
  {
    FunctionType = 0;
    Function = string (str);
  }

  if(properties.getvalue("function_name",str))   // Testfunktion ist über C-Funktion realisiert
  {
    if (testname) free (testname);
    testname = strdup(str);
    FunctionP = GetFunctionPointer (testname);
    FunctionType = 1;
  }  

  if(properties.getvalue("model_name",str))      // Zielfunktion ist ein Modell
  {
    if (testname) free (testname);
    testname = strdup(str);
    FunctionP = GetFunctionPointer (testname);
	// evtl. Initialsierung des Modells bzw. des verwendeten Simulators einfügen
    FunctionType = 2;
  }
  
  if ((!properties.defined("function")) && (!properties.defined("function_name")) && (!properties.defined("model_name")))
  {
#ifdef BS_DOS
    cerr << "Inifile muss einen Eintrag 'function', 'function_name' oder 'model_name' enthalten!\n";
#else
    std::cerr << "Inifile muss einen Eintrag 'function', 'function_name' oder 'model_name' enthalten!\n";
#endif /* BS_DOS */
  }
  
#ifdef BS_DOS
  if(pfile) output<<"Einstellung der Verfahrensparameter"<<endl<<endl;
#else
  if(pfile) output<<"Einstellung der Verfahrensparameter"<<std::endl<<std::endl;
#endif /* BS_DOS */
  if(properties.getvalue("norm",value)){
    norm=(int)value;
#ifdef BS_DOS
    if(pfile) output<<"norm = "<<norm<<endl;
#else
    if(pfile) output<<"norm = "<<norm<<std::endl;
#endif /* BS_DOS */
  }
  if (properties.getvalue ("avoid_reevaluations", value))
  {
    noreevaluations = bool(value);
#ifdef BS_DOS
    if(pfile) output << "avoid_reevaluations = " << noreevaluations << endl;
#else
    if(pfile) output << "avoid_reevaluations = " << noreevaluations << std::endl;
#endif /* BS_DOS */
  }
  if(properties.getvalue("max_iter",value))
    max_iter = (int)value;
#ifdef BS_DOS
  if(properties.getvalue("step",value))
   	if(pfile) output<<"step = "<<(double)value<<endl;
  if(properties.getvalue("error_goal", value))
   	if(pfile) output<<"error_goal = "<<(double)value<<endl;
  if(properties.getvalue("hits", value))
   	if(pfile) output<<"hits = "<<(int)value<<endl;
  if(properties.getvalue("delta", value))
   	if(pfile) output<<"delta = "<<(double)value<<endl;
  if(properties.getvalue("start_complex_size", value))
   	if(pfile) output<<"start_complex_size = "<<(double)value<<endl;
  if(properties.getvalue("restart", value))
   	if(pfile) output<<"restart = "<<(int)value<<endl;
  if(pfile) output<<endl<<endl<<endl;
  if(pfile) output<<"Verlauf der Optimierung"<<endl<<endl;
#else
  if(properties.getvalue("step",value))
   	if(pfile) output<<"step = "<<(double)value<<std::endl;
  if(properties.getvalue("error_goal", value))
   	if(pfile) output<<"error_goal = "<<(double)value<<std::endl;
  if(properties.getvalue("hits", value))
   	if(pfile) output<<"hits = "<<(int)value<<std::endl;
  if(properties.getvalue("delta", value))
   	if(pfile) output<<"delta = "<<(double)value<<std::endl;
  if(properties.getvalue("start_complex_size", value))
   	if(pfile) output<<"start_complex_size = "<<(double)value<<std::endl;
  if(properties.getvalue("restart", value))
   	if(pfile) output<<"restart = "<<(int)value<<std::endl;
  if(pfile) output<<std::endl<<std::endl<<std::endl;
  if(pfile) output<<"Verlauf der Optimierung"<<std::endl<<std::endl;
#endif /* BS_DOS */

  dimension=parameters.number_of_params();

  // Normierung festlegen
  offset=vector(dimension,0.0);
  scale=vector(dimension,1.0);
  start=vector(dimension,1.0);
  transform(norm);

  // Startwert einlesen und normieren
  for(int i=0;i<dimension;i++)
    parameters[i].getvalue("start",start[i]);
  start=invtransform(start);
/*  wj!
  if(int i=infeasible(start))
*/
  ii = infeasible(start);   /* wj! */
  if(ii != 0)               /* wj! */
  {
#ifdef BS_DOS
    cerr<<"Ungültiger Startpunkt! constraint "<<ii<<": "<<consvalue(ii,start)<<endl;              /* wj! */
#else
    std::cerr<<"Ungültiger Startpunkt! constraint "<<ii<<": "<<consvalue(ii,start)<<std::endl;    /* wj! */
#endif /* BS_DOS */
    exit(1);
  }
}

// ************************************************************* //
// Berechnet den originalen Zielfunktionswert in p (normiert)    //
// ************************************************************* //

double sim::operator() (const vector &p)
{
  getvaluecalls++;
  double value;
  vector op=transform(p);

  if(noreevaluations)
  {
    for(unsigned int i=0;i<samplepoints.rows();i++)
	  if(p==samplepoints[i])
      {
        value=samplevalues[i];
#ifdef BS_DOS
        if(pfile) output << op <<  " "<< value << endl;
#else
        if(pfile) output << op <<  " "<< value << std::endl;
#endif /* BS_DOS */
        return value;
      }
    value=GetFunctionValue(op);
	samplepoints.rowappend(p);
	samplevalues.append(value); 
  } else {
    value=GetFunctionValue (op);
  }

#ifdef BS_DOS
  if(pfile) output << op << " " << value << endl;
#else
  if(pfile) output << op << " " << value << std::endl;
#endif /* BS_DOS */
  return value;
}


// ************************************************************* //
// Punkt p (normiert) auf Gültigkeit überprüfen                  //
//   gibt letzte Beschränkung zurück, die verletzt ist, sonst 0  //
// ************************************************************* //

int sim::infeasible(const vector &p)
{
  int i;
  for(i=cons.getnumber();(i>0) && (cons.calculateConst(transform(p),i,parameters)>=0);i--);
  return i;
}


// ********************************************************************* //
// Ermitteln der Transformationsbeziehung                                //
//   0: keine Normierung                                                 //
//   1: explizite Angabe von Verschiebung und Skalierung im Inifile      //
//   2: Normierung auf untere und obere Grenze (--> p (normiert) = 0..1) //
// ********************************************************************* //

void sim::transform(int norm)
{
  unsigned int ii;

  // Alle bereits gesampleten Punkte neu transformieren
  for(ii=0;ii<samplepoints.rows();ii++)
    samplepoints[ii]=transform(samplepoints[ii]);

  // Transformationsparameter bestimmen
  for(int i=0;i<dimension;i++)
  {
    bool lbos=false;
    switch (norm)
    {
      case 1:
        if(parameters[i].getvalue("offset",offset[i]))
          break;
      case 2:
        if(parameters[i].getvalue("lowerbound",offset[i]))
        {
          lbos=true;
          break;
		}
		parameters[i].getvalue("upperbound",offset[i]);
		break;
      default:
        offset[i]=0;
	}
    switch (norm)
    {
      case 1:
        if(parameters[i].getvalue("scale",scale[i]))
          break;
      case 2:
        if(lbos && parameters[i].getvalue("upperbound",scale[i]))
        {
          scale[i]-=offset[i];
          break;
        }
      case 3:
        parameters[i].getvalue("start",scale[i]);
        if((scale[i]-=offset[i])==0)
          scale[i]=1;
        break;
      default:
        scale[i]=1;
	}
  }

  // Alle bereits gesampleten Punkte neu zurücktransformieren
  for(ii=0;ii<samplepoints.rows();ii++)
    samplepoints[ii]=invtransform(samplepoints[ii]);
}


double sim::GetFunctionValue (/*const wj */ vector &p)
{
	double result=0;

	switch (FunctionType)
	{
		case 0:
			result = Function(p);
			break;
		case 1:
			// result = simulation (parameters, p, testname, tcl_interpreter, properties);
			result = FunctionP (*this, p);
			break;
		case 2:
			// result = simulation (parameters, p, testname, tcl_interpreter, properties);
			result = FunctionP (*this, p);
			break;
	};
	simcalls++;
	return result;
}  
