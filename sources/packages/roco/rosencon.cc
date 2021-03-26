#include <stdlib.h>
#include <stdio.h>       /* wj! */
// #include "stdio.h"    /* wj! */
#include <string.h>
#include <math.h>
#include <fstream>       /* wj! */
// #include <fstream.h>  /* wj! */
#include <time.h>

// #include <tk.h>

#include "vector.h"
#include "param.h"
#include "merkmal.h"
#include "gramschm.h"
#include "function.h"
#include "constr.h"
#include "sim.h"

#include "rosencon.h"

#define DEBUG 0
#define debcout if(DEBUG) std::cout   /* wj! */



char UserTermination()
{
  char c;
  c=getc(stdin);
  return(c);
}


/* ************************************************************ */
/* Verfahren nach Rosenbrock mit Beschraenkungen, so wie es im  */
/* Schwefel beschrieben worden ist.                             */
/* ************************************************************ */

/*  Version mit Parametern:
int rosencon_original(const char* OptimizationIni, const char* Initdatei, const char* QF_Berechnung)
*/
int rosencon_original(void)
{
  int nr_of_orto=0;

  int i=0;            // n-Dimension des Parameterraumes,i-Richtungszaehler i=1,...,n
  double* S;            // S[i]-aktuelle Schrittweite in der Richtung i
  double step=0.1;      // Initialschrittweite
  double E = 0.0001;
  double QF1,QF2;       // QF1-Wert des aktuellen Punktes, QF2-Wert des trial steps
  double QFDELTA;       // Wert der Barrierefunktion
  double delta=1e-4,ny;     // Parameter fuer Bildung der Barrierefunktion
  double* d;            // d[i]-Gedeckte Schrittlaenge in jeder Koordinatenrichtung
  int* v;           	// v[i]-Erfolg/Misserfolg Indikatoren
  double* G,*f,*Phi;    // G-Wert der Nebenbedingungsfunktion, f-Qf-wert des
                	// letzten erfolgreichen Schrittes, Phi-fuer Bildung der Barrierefunktion
  vector* ai;           // Benotigt fuer das Gram-Schmidt Verfahren(s. Literatur)
  vector* ei;           // Einheitsvektoren des Karthesischen Koordinatensystems
  vector M, ptr1, xp;   // M-aktuelle Punkt(Vektor), xp-trial step, ptr1-speichert
                	// den ersten Punkt jeder Iteration (nach der Rotation des Koordinatensystems)

  int m=0,l,j,b=0;        // j-Zaehler der Nebenbedingungen j=[1,...,number_of_constrains]
  int hits = 1;
  
  char output[80];
  char optIniFile [80], iniDatei [80], QF_Datei[80];
  strcpy (output, "verlauf.dat");
  strcpy (optIniFile, ROSEN_INI_FNAME);      // Vorherige Aufrufparameter
  strcpy (iniDatei,   ROSEN_STARTW_FNAME);   // nun als
  strcpy (QF_Datei,   ROSEN_QF_FKT_NAME);    // Konstanten

  sim Simulation (optIniFile, iniDatei, QF_Datei, output);

  int n = Simulation.dim();
  int number_of_constraints = Simulation.cons.getnumber();
  int max_iter = 10000;
  int pfile = 1;

  Simulation.properties.getvalue("error_goal", E);
  double value;
  if(Simulation.properties.getvalue("hits", value))
    hits = (int)value;
  Simulation.properties.getvalue("delta", delta);
  Simulation.properties.getvalue("step", step);
  if(Simulation.properties.getvalue("max_iter", value))
    max_iter = (int)value;
  if(Simulation.properties.getvalue("pfile",value))
    pfile = (int)value;
    
  std::ofstream Result;   /* wj! */
  Result.open("ergebnis.dat");
  if(!Result)
  {
    std::cerr<<"ergebnis.dat konnte nicht angelegt werde!"<<std::endl;
    exit(1);
  }  
  Result.precision(20);
  //  Result.setf(ios::scientific);  /* wj! */
  Result.setf(std::ios_base::scientific);

  M=Simulation.start;

  QF1 = Simulation (M);

  ai = new vector[n];
  /*  ei = new vector[n](n);      wj */
  ei = new vector[n];          /* wj */
  for (i = 0; i < n; i++)      /* wj */
    ei[i] = vector(n, 0.0);    /* wj */

  d = new double[n];
  S = new double[n];
  v = new int[n];
  G = new double[number_of_constraints];
  f = new double[number_of_constraints];
  Phi = new double[number_of_constraints];
  ptr1 = vector(n);
  xp = vector(n);

  for (j = 0; j < number_of_constraints; j++)
    f[j]=QF1;
  for (i = 1; i <= n; i++)
    ei[i-1].axis(i-1); // Festlaegung der Anfangs Koordinatenaxen

// Step 1: (Initialisation of step sizes,distances travelled, and indicators)
  do { // Erste do-Schleife
    for(i = 1; i <= n; i++) {
      S[i-1] = step;
      d[i-1] = 0.0;
      v[i-1] = -1;
    }
    i = 1;
    l = 0;
    if ((n * l + i - 1) == 0 )
      ptr1 = M;

// Step 2: (Trial step)
    do { // Zweite do-Schleife
      xp = M + ei[i-1] * S[i - 1]; // Schritt karakteristisch fuers Rosenbrocks Verfahren
      QF2 = Simulation (xp);
      if (QF2 >= QF1)
        goto STEP6;
     else if( QF2 < QF1 )
       {
         if( number_of_constraints == 0 )
          goto STEP5;
         else
          {
          QFDELTA = QF2;
          j = 1;
          }
   do // Dritte do-Schleife
    {
    // Step 3: (Test of feasibility)
    G[j-1] = Simulation.consvalue(j,xp);
    if(G[j-1] >= Simulation.cons.getdelta(j-1))
      {
      f[j-1]=QF2;
      // Step 4: (Constrains loop)
      if(j<number_of_constraints)
      {
        j++;
      }
          else
        break;
      }
    else if(G[j-1]<0)
      goto STEP6;
    else // if((G[j-1]<DELTA) && (G[j-1]>=0))
      {
      ny=1.0-((1.0/Simulation.cons.getdelta(j-1))*G[j-1]);
      Phi[j-1]=(3.0*ny)-(4.0*pow(ny,2))+(2.0*pow(ny,3));
      QFDELTA=QFDELTA+(Phi[j-1]*(f[j-1]-QFDELTA)); // Bildung der Barrierefunkt.
      if(QFDELTA>QF1){
        goto STEP6;
      }
      else
        {
         if(j<number_of_constraints)
          j++;
             else
          break;
        }
           }
      }
    while(1); // Ende der dritte do-Schleife

        }
STEP5:   // (Store the succes and update internal memory)

       M = xp;
       QF1 = QF2;
       d[i-1] += S[i-1];
       S[i-1] = 3 * S[i-1];
       if( v[i-1] == -1 )
          v[i-1]=0;

       goto STEP7;

STEP6:    // (Internal memory update in case of failure)

       S[i-1]=-0.5*S[i-1];
       if(v[i-1]==0)
         v[i-1]=1;

STEP7:    // (Main loop)
       b=0;
       for(int z=0;z<n;z++)
          {
         if(v[z]==1)
         b++;
         }
       if(b<n)
         {
         if(i<n)
           i++;
         else
           {
           i = 1;
           l++;
            }
         }
       } while((b<n)&&(Simulation.NumberOfGetValueCalls()<max_iter)); // Ende der zweite do-Schleife

     // Step8: (Preparation for orthogonalization and check for termination)
     ai_constr(n, d, ai, ei); // Konstruktion der ai Vektoren
     M = ptr1 + ai[0];
     QF1 = Simulation (M);

      if ((n == 1) && (Simulation.NumberOfGetValueCalls()<max_iter)) {
       if (ai[0].l2norm() < E)
         m++;
       else
         m = 0;
     } else {
       double a0 = ai[0].l2norm();
       if ((a0 < E) && (ai[1].l2norm() > 0.03 * a0))
         m++;
       else
         m = 0;
     }

     debcout<<"m = "<<m<<std::endl;
     debcout<<"hits = "<<hits<<std::endl;
     debcout<<"Simulation.NumberOfGetValueCalls = "<<Simulation.NumberOfGetValueCalls()<<std::endl;
     debcout<<"max_iter = "<<max_iter<<std::endl;
     
     if ((m == hits)||(Simulation.NumberOfGetValueCalls()>max_iter)) // Rosenbrocks Vorschlag m=6 hat sich als zu streng erwiesen
     {
       // ********************************** //
       // Ergebnisse ausgeben                //
       // ********************************** //

       if(pfile){
         Simulation.output << "\n\nMinimum = " << QF1 << " bei " << Simulation.transform(M);
         Simulation.output << "\nAnzahl der angeforderten QF Werte betraegt: " << Simulation.NumberOfGetValueCalls();
         Simulation.output << "\nAnzahl der tatsächlich durchgefuehrten QF Berechnungen (Simulationen) betraegt: " << Simulation.NumberOfSimCalls();
         Simulation.output << "\nAnzahl der durchgefuehrten Rotationen: " << nr_of_orto << std::endl;
       }
       
       Result << "Minimum = " << QF1 << " bei " << Simulation.transform(M);
       Result << "\nAnzahl der angeforderten QF Werte betraegt: " << Simulation.NumberOfGetValueCalls();
       Result << "\nAnzahl der tatsächlich durchgefuehrten QF Berechnungen (Simulationen) betraegt: " << Simulation.NumberOfSimCalls();
       Result << "\nAnzahl der durchgefuehrten Rotationen: " << nr_of_orto << std::endl;

       delete [] ei; delete [] ai; delete [] v; delete [] S; delete [] d;
       delete [] G; delete [] f; delete [] Phi;

       Result.close();
	   
       return(0); // Regelmaessige Termination des Programmes
     }else{
       if(n>1){  	
         nr_of_orto++;
         gram_schmidt(n, ai, ei); // Orthogonalisationsverfahren
       }
     }
    }while(1); // Ende der erste do-Schleife
    return 1;
} // Ende der Funktion-main
