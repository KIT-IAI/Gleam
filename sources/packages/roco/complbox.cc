#include "schalter.h"  /* wj */

#include <stdlib.h>
#include <stdio.h>       /* wj */
// #include <fstream.h>  /* wj */
#include <fstream>       /* wj */
#include <time.h>
// #include <iostream.h> /* wj */
#include <iostream>      /* wj */
#ifdef BS_LINUX
#include <sstream>       /* wj */
#else
#include <strstream.h>
#endif
#include <assert.h>

#include "vector.h"
#include "param.h"
#include "merkmal.h"
#include "function.h"
#include "constr.h"
#include "sim.h"
#include "complbox.h"   /* wj */

#undef DEBUG

#ifdef BS_DOS
#define dcout  cout
#define xendl  endl
#define xflush flush
#define xcerr  cerr
#else
#define dcout  std::cout
#define xendl  std::endl
#define xflush std::flush
#define xcerr  std::cerr
#endif /* BS_DOS */



void Reflection (PointWValue *Vertex, int N, vector &Xr, vector &Xc, double alpha);
void Contraction (vector &Xr, vector &Xc);
void ExpConCheck (sim &Simulation, int &j, vector &Xr, vector &Xc, double epsilon);
bool ImpConCheck (sim &Simulation, int &j, vector &Xr, vector &Xc, int &ImpConstFail, int &ImpConstFailIndex, double &old_g);
void CreateStartComplex_Random (sim &Simulation, PointWValue *&Vertex, int &N, double s);
void CreateStartComplex_Inifile (sim &Simulation, PointWValue *&Vertex, int &N);
void CheckStartComplex (sim &Simulation, PointWValue *Vertex, int N);


int ComplexIndex;  // Index des reflektierten Complex-Punktes

// Prototypen

/*  Version mit Parametern:
int complex_box (const char *OptimizationIni, const char *Initdatei, const char *QF_Berechnung)
*/

int complex_box (void)
{
#ifdef DEBUG
  dcout<<"complex_box"<<xendl<<xflush;
#endif
  srand (time(NULL));

  char output[200];
  char optIniFile [80], iniDatei [80], QF_Datei[80];
  strcpy (output, "verlauf.dat");
  strcpy (optIniFile, COMPL_STARTW_FNAME);  // Vorherige
  strcpy (iniDatei,   COMPL_INI_FNAME);     // Aufrufparameter
  strcpy (QF_Datei,   QF_FKT_NAME);         // nun als Konstanten

  std::ofstream Result;
  Result.open("ergebnis.dat");
  if(!Result)
  {
    xcerr<<"ergebnis.dat konnte nicht angelegt werde!"<<xendl;
    exit(1);
  }  
  Result.precision(20);
  //  Result.setf(ios::scientific);      /* wj */
  Result.setf(std::ios_base::scientific);
  
  // ********************** //
  // Variablen Def          //
  // ********************** //
  sim Simulation (optIniFile, iniDatei, QF_Datei, output);

  int n;                  // Anzahl der Parameter
  int m;                  // Anzahl der Constraints
  int m1;                 // Anzahl der expliziten Constraints
  int N=0;                // Anzahl der Complex Punkte
  vector Xc;              // Mittel(Spiegel)punkt
  vector Xr;              // Reflektierter Punkt
  int j;                  // Laufvariable f?r die Constraints
  int i=0;

  bool improvement;       // wurde Verbesserung erreicht?
  int Fail = 0;           // Abbruch Z?hler
  double FXr, old_FXr = 0.0;

  // Variablen f?r das Abbruchkriterium in ImpConCheck()
  double old_g = 0.0;     // Alter Wert der Constraint Funktion
  int ImpConstFail = 0;   // Contraint Fail Z?hler
  int ImpConstFailIndex = -1; // Index der Constraint Funktion, die den Fehler verursacht hat

  double alpha = 1.3;     // OverReflection-Factor
  double epsilon = 1e-6;  // Abstand zur expliziten Constraint nach Verletzung und Korrektur der Con.
  double E = 1e-6;        // F?r alternatives Abbruchkrit.: Abbruch, wenn sich der Punkt 5x um weniger als E bewegt
  double s = 10.0;        // Gr??e des Start-Complex

  int StartComplexMethod = 0; // 0 = per Zufallszahlen (Original), 1 = ...
  int nr_of_complex_points = 0;
  int max_iter = 10000;
  int pfile = 1;

  // ***************************** //
  // Initialisierung der Variablen //
  // ***************************** //
  n = Simulation.dim();                      // Anzahl der Parameter
  m = Simulation.cons.getnumber();           // Anzahl der Constraints
  m1 = Simulation.cons.getexconstraints();   // Anzahl der expliziten Constraints

  Xc = vector (n);                           // Punkte auf richtige Gr??e bringen
  Xr = vector (n);

  // ****************************************** //
  // Einlesen der Daten aus dem ini-File        //
  // ****************************************** //
  {
    double value = 0.0;
    Simulation.properties.getvalue("error_goal", E);
    Simulation.properties.getvalue("start_complex_size", s);
    if (Simulation.properties.getvalue("start_complex_method", value))
	   StartComplexMethod = int(value);
    if (StartComplexMethod == 1){
        if (Simulation.properties.getvalue("nr_of_complex_points", value))
            nr_of_complex_points = (int)value;
#ifdef DEBUG
	dcout<<"nr_of_complex_points = "<<nr_of_complex_points<<xendl<<xflush;
#endif /* DEBUG */
    }
    if (Simulation.properties.getvalue("max_iter", value))
      max_iter = (int)value;
    if (Simulation.properties.getvalue("pfile", value))
      pfile = (int)value;
  }

  // ************************ //
  // Initialisierung (Step 0) //
  // ************************ //

  // **************** //
  // Complex aufbauen //
  // **************** //
  switch (StartComplexMethod)
  {
      case 0:
	    if(n<5)
	      N=2*n;
	    else
	      N=n+1;
//	    CreateStartComplex_Random (Simulation, Vertex, N, s);
	    break;
      case 1:
	    if (nr_of_complex_points == 0){
	      if(n<5)
	        N=2*n;
	      else
	        N=n+1;
	    } else {
	      N=nr_of_complex_points;
	    }
#ifdef DEBUG
            dcout<<"Startcomplex wird kreiirt!\n";
	    dcout<<"N = "<<N<<xendl<<xflush;
#endif /* DEBUG */
//	    CreateStartComplex_Inifile (Simulation, Vertex, N);
	    break;
      case 2:
	    xcerr << "\nNoch nicht implementiert!" << xendl;
//	    exit (1);
            break;
      default:
	    xcerr << "\nEine StartComplex-Methode " << StartComplexMethod << " ist nicht implementiert!" << xendl;
//	    exit (1);
	    break;
  };

/*  PointWValue Vertex[N];    // Array der N Complex-Punkte
  for(int o=0; o<N; o++){
    dcout<<"o="<<o<<endl<<flush;
    dcout<<"N="<<N<<endl<<flush;
    Vertex[o].Point=Xc;
    dcout<<"Vertex["<<o<<"].Point= "<<Vertex[o].Point<<endl<<endl<<flush;
  }
  dcout<<"OLLI"<<std::endl<<std::flush;*/

  PointWValue *Vertex=NULL;    // Array der N Complex-Punkte
  assert(Vertex = new PointWValue [N]);
  for(int o=0; o<N; o++)
    Vertex[o]=PointWValue(n);

  
  switch (StartComplexMethod)
  {
      case 0:
	    if(n<5)
	      N=2*n;
	    else
	      N=n+1;
	    CreateStartComplex_Random (Simulation, Vertex, N, s);
	    break;
      case 1:
	char name[20];                                            /* vorher 10, wj */
	char value[1024];                                         /* vorher 80, wj */
	
#ifdef DEGUG
	dcout<<"a\n";
#endif /* DEBUG */
	for(N=0,sprintf(name,"vertex0");Simulation.properties.defined(name);sprintf(name,"vertex%d",++N));
#ifdef DEGUG
	dcout<<"b\n";
	dcout<<"c\n";
#endif /* DEBUG */
	
	for(i=0;i<N;i++)
	{
#ifdef DEGUG
		dcout<<"d\n";
#endif /* DEBUG */
		sprintf(name,"vertex%d",i);
		Simulation.properties.getvalue(name,value);
#ifdef BS_LINUX
		std::istringstream valstr(value);           /* wj */
#else
	        istrstream valstr(value);
#endif
		valstr >> Vertex[i].Point;
#ifdef DEGUG
		dcout<<"Vertex["<<i<<"].Point = "<<Vertex[i].Point<<xendl;
#endif /* DEBUG */
		Vertex[i].Point=Simulation.invtransform(Vertex[i].Point);
	}
#ifdef DEGUG
	dcout<<"e\n";
#endif /* DEBUG */

#ifdef DEGUG
            dcout<<"Startcomplex wurde kreiirt!\n"<<xflush;
#endif /* DEBUG */
	    break;
      case 2:
	    xcerr << "\nNoch nicht implementiert!" << xendl;
	    exit (1);
            break;
      default:
	    xcerr << "\nEine StartComplex-Methode " << StartComplexMethod << " ist nicht implementiert!" << xendl;
	    exit (1);
  };

  // ********************************* //
  // Constraint Check des StartComplex //
  // ********************************* //
#ifdef DEGUG
  dcout<<"CheckStartComplex wird gestartet!"<<xendl<<xflush;
#endif /* DEGUG */
  CheckStartComplex (Simulation, Vertex, N);
#ifdef DEGUG
  dcout<<"CheckStartComplex beendet!"<<xendl<<xflush;
#endif /* DEGUG */

  // ********************************************************** //
  // Berechnung der Funktionswerte des endg?ltigen StartComplex //
  // ********************************************************** //
#ifdef DEGUG
  dcout<<"Berechnung Funktionswerte!"<<xendl<<xflush;
#endif /* DEBUG */
  for (int k=0; k<N; k++)
  {
    ComplexIndex = Vertex[k].Index = k;
    Vertex[k].Value = Simulation (Vertex[k].Point);
    if(pfile) Simulation.output << Vertex[k].Index << "   " << Simulation.transform(Vertex[k].Point) << "   " << Vertex[k].Value << xendl;
  }
#ifdef DEGUG
  dcout<<"Berechnung Funktionswerte abgeschlossen!"<<xendl<<xflush;
#endif /* DEGUG */
  
  // Sortieren der Complex-Punkte nach Gr??e ihrer Funktionswerte
  // => Vertex[0] = bester Punkt; Vertex[N-1] = schlechtester Punkt
  qsort (Vertex, N, sizeof(class PointWValue), compare_PointWValue);
  
  // ******************** //
  // Start des Verfahrens //
  // ******************** //

  // ******************** //
  // Erste Reflection     //
  // ******************** //
#ifdef DEGUG
  dcout<<"Erste Reflection!"<<xendl<<xflush;
#endif /* DEGUG */
  Reflection (Vertex, N, Xr, Xc, alpha);
#ifdef DEGUG
  dcout<<"Erste Reflection abgeschlossen!"<<xendl<<xflush;
#endif /* DEGUG */

  // ******************** //
  // Hauptschleife        //
  // ******************** //
  do
  {
#ifdef DEGUG
    dcout<<"Hauptschleife!"<<xendl<<xflush;
#endif /* DEGUG */
    // ************************ //
    // Step 2: Constraint Check //
    // ************************ //
    for (j=0; j<m; j++)
    {
#ifdef DEGUG
      dcout<<"a"<<xendl<<xflush;
#endif /* DEGUG */
      if (j < m1) ExpConCheck(Simulation, j, Xr, Xc, epsilon);
#ifdef DEGUG
      dcout<<"b"<<xendl<<xflush;
#endif /* DEGUG */
      if ((j >= m1) && (j < m))
        if (! ImpConCheck(Simulation, j, Xr, Xc, ImpConstFail, ImpConstFailIndex, old_g))
          goto ende;
    }
#ifdef DEGUG
    dcout<<"c"<<xendl<<xflush;
#endif /* DEGUG */

    // ************************* //
    // Step 7: Improvement Check //
    // ************************* //
#ifdef DEGUG
    dcout<<"d"<<xendl<<xflush;
#endif /* DEGUG */
    ComplexIndex = Vertex[N-1].Index;        // Index des schlechtesten (=reflektierten) Punktes in globale Variable schreiben
    FXr = Simulation (Xr);                   // dann tats?chlichen Funktionswert anfordern
#ifdef DEGUG
    dcout<<"e"<<xendl<<xflush;
#endif /* DEGUG */
    
    improvement = false;                     // Auf Verbesserung mit dem tats?chlichen Funktionswert pr?fen
#ifdef DEGUG
    dcout<<"f"<<xendl<<xflush;
#endif /* DEGUG */
    for (int k = 0; k < (N-1); k++)          // Vergleich von Xr mit allen Complex-Punkten bis auf den schlechtesten (N-1)
    {
      if (FXr < Vertex[k].Value)
        improvement = true;
    }

#ifdef DEGUG
    dcout<<"g"<<xendl<<xflush;
#endif /* DEGUG */
    if (improvement)                           // Wenn Verbesserung
    {
#ifdef DEGUG
     dcout<<"h"<<xendl<<xflush;
#endif /* DEGUG */
     Vertex[N-1].Point = Xr;                  // Schlechtesten Punkt des Complex durch neuen ersetzen
      Vertex[N-1].Value = FXr;
      Fail = 0;

      // Sortieren der Complex-Punkte nach Gr??e ihrer Funktionswerte
      // => Vertex[0] = bester Punkt; Vertex[N-1] = schlechtester Punkt
      qsort (Vertex, N, sizeof(class PointWValue), compare_PointWValue);

      if(pfile) Simulation.output << Vertex[N-1].Index << "   " << Simulation.transform(Vertex[N-1].Point) << "   " << Vertex[N-1].Value << xendl;

#if 0      
      // ************************************** //
      // Nur f?r die Kontroll-Ausgabe           //
      if(pfile) Simulation.output << "0 " << Vertex[0].Point << " " << Vertex[0].Value << " ";
      for (int k=0; k<m; k++){
        if(pfile) Simulation.output << " " << Simulation.consvalue (k+1, Vertex[0].Point);
      }
      if(pfile) Simulation.output << endl;
      // ************************************* //
#endif

      Reflection (Vertex, N, Xr, Xc, alpha);   // Neue Reflektion durchf?hren
    }
    else
    {
#ifdef DEGUG
      dcout<<"i"<<xendl<<xflush;
#endif /* DEGUG */
      if (FXr == old_FXr){                     // Wenn sich der Funktionswert nicht ge?ndert hat
        Fail++;                                // Fail-Z?hler erh?hen
#ifdef DEGUG
        dcout<<"ia"<<xendl<<xflush;	
#endif /* DEGUG */
      }
      else
      {
#ifdef DEGUG
        dcout<<"ib"<<xendl<<xflush;	
#endif /* DEGUG */
        Fail = 1;                              // sonst Fail-Z?hler auf 1 zur?cksetzen und FXr merken
        old_FXr = FXr;
      }
#ifdef DEGUG
      dcout<<"j"<<xendl<<xflush;
#endif /* DEGUG */
      Contraction(Xr, Xc);                     // Kontaktion durchf?hren
#ifdef DEGUG
      dcout<<"k"<<xendl<<xflush;
#endif /* DEGUG */
    }
#ifdef DEGUG
    dcout<<"l"<<xendl<<xflush;
    dcout<<"Fail = "<<Fail<<xendl<<xflush;
#endif /* DEGUG */
  }
//  while (Fail < 5);	// Abbruch bei 5 Fehlschl?gen
  while ((Fail < 5)&&(Simulation.NumberOfGetValueCalls() < max_iter));   // Abbruch bei 5 Fehlschl?gen  wj
#ifdef DEGUG
  dcout<<"Hauptschleife wurde verlassen!"<<xendl<<xflush;
#endif /* DEGUG */
ende:
  // ********************************** //
  // Index des besten Punktes bestimmen //
  // ********************************** //
#if 0
#ifdef DEGUG
  dcout<<"ifA"<<xendl<<xflush;
#endif /* DEGUG */
  if(pfile) Simulation.output << "0 " << Vertex[0].Point << " " << Vertex[0].Value << " ";
  for (int k=0; k<m; k++){
    if(pfile) Simulation.output << " " << Simulation.consvalue (k+1, Vertex[0].Point);
  }
  if(pfile) Simulation.output << endl;
#endif

// ********************************** //
// Ergebnisse ausgeben                //
// ********************************** //
#ifdef DEGUG
  dcout<<"A"<<xendl<<xflush;
#endif /* DEGUG */
  if(pfile) Simulation.output << xendl << Vertex[0].Index << "   " << Simulation.transform(Vertex[0].Point) << "   " << Vertex[0].Value << xendl;
#ifdef DEGUG
  dcout<<"B"<<xendl<<xflush;
#endif /* DEGUG */

  Result << "Minimum = " << Vertex[0].Value << " bei " << Simulation.transform(Vertex[0].Point) << xendl;
  Result << "Anzahl der angeforderten QF Werte betraegt: " << Simulation.NumberOfGetValueCalls() << xendl;
  Result << "Anzahl der tats?chlich durchgefuehrten QF Berechnungen (Simulationen) betraegt: " << Simulation.NumberOfSimCalls() << xendl;
 
#ifdef DEGUG
  dcout<<"C"<<xendl<<xflush;
#endif /* DEGUG */
  
  // ********************************** //
  // Speicher freigeben                 //
  // ********************************** //
#ifdef DEGUG
  dcout<<"D"<<xendl<<xflush;
  dcout<<"Vertex="<<Vertex<<xendl<<xflush;
#endif /* DEGUG */

  
  // Destruktor verwenden!
  /*  wj
  for(int oo=0; oo<N; oo++)
    delete &Vertex[oo].Point;
  */
#ifdef DEGUG
  dcout<<"Vertex="<<Vertex<<xendl<<xflush;  
#endif /* DEGUG */
    
  /*  delete Vertex;  wj */
  delete [] Vertex;   /* wj */
#ifdef DEGUG
  dcout<<"Vertex="<<Vertex<<xendl<<xflush;
  dcout<<"E"<<xendl<<xflush;
#endif /* DEGUG */

  Result.close();
#ifdef DEGUG
  dcout<<"F"<<xendl<<xflush;
#endif /* DEGUG */

  return 1;
}





// ********************************** //
// Weitere Funktionen                 //
// ********************************** //

void Reflection (PointWValue *Vertex, int N, vector &Xr, vector &Xc, double alpha)
{
// Step 1

  // Mittel(Spiegel)punkt bestimmen
  Xc.set(0.0);
  for (int k=0; k<N; k++)
  {
    if (k != (N-1))         
    {
      Xc = Xc + Vertex[k].Point;
    }
  }
  Xc = Xc / double(N-1);

  // Reflektion
  Xr = Xc + (Xc - Vertex[N-1].Point) * alpha;
}


void Contraction (vector &Xr, vector &Xc)
{
// Step 8

  Xr = (Xc + Xr) * 0.5;
}

void ExpConCheck (sim &Simulation, int &j, vector &Xr, vector &Xc, double epsilon)
{
// Step 3: ?perpr?fung der expliziten Constraints
  double g;

  g = Simulation.consvalue (j+1, Xr);              // Ermittlung des Werts der Constraint-Funktion
  if (g < 0.0)                                     // Ist Beschr?nkung verletzt?
  {
    int i = Simulation.cons.getconparam(j);        // Auf welchen Parameter bezieht sich die aktuelle Beschr?nkung?
	 g = -g;                         // entspricht hier fabs(g), da g < 0 / f?r folgende Korrektur muss g positiv sein!
    g /= Simulation.scale[i];                      // g wird ebenfalls normiert! Wichtig f?r die folgende Korrektur!
	 Xr[i] = Xr[i] + g + epsilon;                   // Korrektur des entsprechenden Parameters von Xr in eine Richtung
    if (Simulation.consvalue (j+1, Xr) < 0.0)      // War Korrektur erfolgreich?
      Xr[i] = Xr[i] - 2*(g + epsilon);             // Wenn nicht, Korrektur in die andere Richtung
  }
  if (Simulation.consvalue(j+1, Xr) < 0)           // Fall Beschr?nkung immer noch verletzt -> Fehlermeldung
    xcerr << "ExpConCheck: " << j << "  "<<"  "<< g << Simulation.consvalue(j+1,Xr) << Simulation.transform (Xr) << xendl;
}

bool ImpConCheck (sim &Simulation, int &j, vector &Xr, vector &Xc, int &ImpConstFail, int & ImpConstFailIndex, double &old_g)
{
// Step 5: ?berpr?fung der impliziten Constraints
  double g;

  g = Simulation.consvalue (j+1, Xr);              // Ermittlung des Werts der Constraint-Funktion
  if (g < 0.0)                                     // Ist Beschr?nkung verletzt?
  {
    if (ImpConstFail >= 5)                         // Abbruch bei 5 Fehlschl?gen
    {
      return false;
    }
    else
    {
      Contraction(Xr, Xc);                         // Kontraktion durchf?hren

      if ((j == ImpConstFailIndex) && (g == old_g)) // Wurde wieder dieselbe Beschr. verletzt und hat sich g nicht ge?ndert
        ImpConstFail++;                            // Dann Fail-Z?hler erh?hen
      else
      {
        ImpConstFail = 1;                          // sonst Fail-Z?hler auf 1 setzen
        ImpConstFailIndex = j;                     // Index der verletzten Constraint
        old_g = g;                                 // und Wert der Constraint-Funktion merken
      }
      j = -1;          // -1, da j nach Inkrementieren am Ende der for-Schleife im Hauptprogramm auf 0 stehen muss!
    }
  }
  return true;
}

void CreateStartComplex_Random (sim &Simulation, PointWValue *&Vertex, int &N, double s)
{
  vector help;

  /*  wj 
  Vertex = new PointWValue [N] (Simulation.dim());               // Erzeugung der N Punkte der Gr??e n
  */
  Vertex = new PointWValue [N];                   /* wj */
  for (int i=0; i<N; i++)                         /* wj */
     Vertex[i] = PointWValue(Simulation.dim());   /* wj */

  Vertex[0].Point = Simulation.start;

  for (int k=1; k<N; k++)   // Durch alle Complex-Punkte laufen mit Ausnahme des Startpunkts
  {
	help = Simulation.transform (Vertex[0].Point);              // Umrechnung des Startpunkts in reale Werte
    for (int l=0; l<Simulation.dim(); l++)                     // Durch alle Parameter laufen
    {
      if ((Simulation.parameters[l].defined("lowerbound")) && (Simulation.parameters[l].defined("upperbound")))
      {
        double lb, ub;
        Simulation.parameters[l].getvalue("lowerbound", lb);   // Grenzen einlesen
        Simulation.parameters[l].getvalue("upperbound", ub);

        help [l] = (double(rand())/RAND_MAX) * (ub-lb) + lb;   // Zufallszahl im Intervall [lb, ub]
      }
      else
		{
		  double RandomValue;
        RandomValue = (double(rand())/RAND_MAX) - 0.5;         // Zufallszahl im Intervall [-0.5, 0.5]
		  help[l] = help[l] + RandomValue * s;                     // Neuer Punkt = Startpunkt + [-0.5, 0.5] * s
	   }
	 }
	 Vertex[k].Point = Simulation.invtransform (help);           // R?ckrechnung in normierte Darstellung
  }
}


void CreateStartComplex_Inifile (sim &Simulation, PointWValue *&Vertex, int &N)
{
	char name[20];                                 /* vorher 10, wj */
	char value[1024];                              /* vorher 80, wj */
	int i=0;
	
#ifdef DEGUG
	dcout<<"a\n";
#endif /* DEGUG */
	for(N=0,sprintf(name,"vertex0");Simulation.properties.defined(name);sprintf(name,"vertex%d",++N));
#ifdef DEGUG
	dcout<<"b\n";
#endif /* DEGUG */
	/* Vertex = new PointWValue [N] (Simulation.dim());   wj */
        Vertex = new PointWValue [N];                      /* wj */
        for (i=0; i<N; i++)                                /* wj */
        Vertex[i] = PointWValue(Simulation.dim());         /* wj */

#ifdef DEGUG
	dcout<<"c\n";
#endif /* DEGUG */
	
	for(i=0;i<N;i++)
	{
#ifdef DEGUG
		dcout<<"d\n";
#endif /* DEGUG */
		sprintf(name,"vertex%d",i);
		Simulation.properties.getvalue(name,value);
#ifdef BS_LINUX
		std::istringstream valstr(value);           /* wj */
#else
	        istrstream valstr(value);
#endif
		valstr >> Vertex[i].Point;
#ifdef DEGUG
		dcout<<"Vertex["<<i<<"].Point = "<<Vertex[i].Point<<endl;
#endif /* DEGUG */
		Vertex[i].Point=Simulation.invtransform(Vertex[i].Point);
	}
#ifdef DEGUG
	dcout<<"e\n";
#endif /* DEGUG */
}


void CheckStartComplex (sim &Simulation, PointWValue *Vertex, int N)
{
  int j, k;
  int n = Simulation.dim();
  int m = Simulation.cons.getnumber();
  int m1 = Simulation.cons.getexconstraints();
  bool ConsOK;
  do
  {
    ConsOK = true;
    for (j=0; j<m1; j++)                           // Durch alle expliziten Constraints laufen
    {
      for (k=1; k<N; k++)                          // Durch alle Complex-Punkte (bis auf den Startpunkt) laufen
      {
        if (Simulation.consvalue (j+1, Vertex[k].Point) < 0) // Wenn Constraint verletzt wird
        {
          // "Spiegelung" des entspr. Parameters des Punktes am Startpunkt
          Vertex[k].Point [Simulation.cons.getconparam(j)] = 2 * Vertex[0].Point [Simulation.cons.getconparam(j)] - Vertex[k].Point [Simulation.cons.getconparam(j)];
        }
      }
    }
    for (j=0; j<m; j++)                            // Durch ALLE Constraints laufen
    {
      for (k=1; k<N; k++)                          // Durch alle Complex-Punkte (bis auf den Startpunkt) laufen
      {
        if (Simulation.consvalue (j+1, Vertex[k].Point) < 0) // Wenn Constraint verletzt wird
        {
          vector center (n, 0.0);                      // Mittelpunkt der "guten" Complex-Punkte bestimmen
          for (int k2=0; k2<k; k2++)
            center = center + Vertex[k2].Point;
          center = center / double(k);

          Vertex[k].Point = (Vertex[k].Point + center) * 0.5;      // Punkt durch Mittelwert des problem. Punktes und des Mittelpunktes ersetzen
          ConsOK = false;                              // Merken, da? noch nicht alle Constr. OK waren
        }
      }
    }
  }
  while (! ConsOK);   // Pr?fung wiederholen bis alle Contraints eingehalten werden
}
