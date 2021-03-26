

#include "mathe.h"

double n_k(int n, int k)
{ double e;
  int i;

  e = 1;
  for (i = 1; i <= n; i++)
    e *= i;
  for (i = 1; i <= k; i++)
    e /= i;
  for (i = 1; i <= n-k; i++)
    e /= i;

  return(e);
}

