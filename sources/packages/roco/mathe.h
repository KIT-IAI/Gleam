
#ifndef _mathe_
#define _mathe_

#include <math.h>

#define TRUE  1
#define FALSE 0


#define PI 3.14159265358979

#ifdef sun5

#define MACH_EPS 1.925929944387235853055977942585e-34
#define EPSROOT  1.387778780781445675529539585114e-17
#define EPSQUAD  3.709206150687421385731735261548e-68
#define MAXROOT  9.48075190810918e+153

#else

#define MACH_EPS 2.220446049250313080847263336182e-16
#define EPSROOT  1.490116119384765625000000000000e-08
#define EPSQUAD  4.930380657631323783823303533017e-32
#define MAXROOT  9.48075190810918e+153

#endif


struct rectangle {
  int x_min;
  int x_max;
  int y_min;
  int y_max;
};

#define max(a,b) (((a)>(b)) ? (a) : (b))
#define min(a,b) (((a)<(b)) ? (a) : (b))
#define absolute(a) (((a)<0) ? -(a) : (a))
#define trunc(x)   ((double)((long)(x)))
// #define log10(x)   (log(x)/log(10))
#define dB(x)      (double)(20*log10(x))
#define degree(rad) (double)((rad)/PI*180)

inline double power(double x, double y) {
  if (y == 0)
    return(1.0);
  else
    return(pow(x,y));
};

double n_k(int n, int k);

#endif
