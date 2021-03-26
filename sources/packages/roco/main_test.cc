#include <stdio.h>
#include <iostream.h>
#include "rosencon.h"

/* ===== test === */
double objfct_complex(double*);

double objfct_complex(double*)
{
  return (0.0);
}
/* ===== test === */


int main(int argc, char *argv[])
{
	char qfini[80];
	switch(argc)
	{
		case 2:
			sprintf(qfini,"%s.ini",argv[1]);
			rosencon_original("rosenbrock.ini",qfini,argv[1]);
			break;
		case 3:
			sprintf(qfini,"%s.ini",argv[1]);
			rosencon_original(argv[2],qfini,argv[1]);
			break;
		default:
			std::cerr<<"usage: "<<argv[0]<<" qualityfunction [rosenbrockini]"<<endl
			    <<"   inputfile:  qualityfunction.ini"<<endl
			    <<"   outputfile: date__qualityfunction__orosen.dat"<<endl
			    <<"   rosenbrockini (optional) contains general settings"<<endl
			    <<"                    default: rosenbrock.ini"<<endl;
	}
}
