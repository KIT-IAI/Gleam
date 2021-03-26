#include <stdlib.h>

#include "vector.h"
#include "objfct.h"


extern double objfct_complex (double* param);


/* double objfct (sim &Simulation, const vector &Point) */
double objfct (sim &Simulation, vector &Point)
{
	return objfct_complex (Point.return_satz());
}
