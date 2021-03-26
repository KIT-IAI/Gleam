#ifndef _GETFUNCTIONPOINTER_H_
#define _GETFUNCTIONPOINTER_H_

#include "vector.h"

class sim; 

/* typedef double (*FunctionP_Type) (sim &, const vector &); */
typedef double (*FunctionP_Type) (sim &, vector &);


FunctionP_Type GetFunctionPointer (const char *);

#endif
