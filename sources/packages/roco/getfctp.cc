#include <stdlib.h>
#include <string.h>
// #include <iostream.h>   /* wj! */
#include <iostream>


#include "schalter.h"

#include "objfct.h"
#include "getfctp.h"


FunctionP_Type GetFunctionPointer (const char *name)
{	
	if ( strcmp(name, "objfct") == 0 )
		return objfct;

	// Weitere Zielfunktionen
	/*
	else if ( strcmp(name, "Fkt1") == 0 )
		return Fkt1;
	else if ( strcmp(name, "Fkt2") == 0 )
		return Fkt2;
	*/
	
	else
	{
#ifdef BS_DOS
		cerr << "\nDas Testproblem " << name << " ist nicht implementiert!\n";
#else
		std::cerr << "\nDas Testproblem " << name << " ist nicht implementiert!\n";
#endif /* BS_DOS */
		exit (EXIT_FAILURE);
	}
}
