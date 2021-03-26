
#include "gramschm.h"
#include "vector.h"


void ai_constr(int n, double* d, vector* ai, vector* ei)
{
  for (int t = 0; t < n; t++)
  {
    ai[t]=vector(ei[0].dim());            // Nicht nullvector(n), da n < ei[?].size sein kann (bei DSC)
    for (int j = t; j < n; j++){
      ai[t] = ai[t] +  (ei[j] * d[j]);
    }
  }
}


void gram_schmidt(int n, vector* ai, vector* ei)
{
  double e,a1;
  a1=ai[0].l2norm();
  ei[0] = ai[0] * (1.0/a1);
  vector XO;
  for(int t=2;t<=n;t++)
  {
    ei[t-1] = ai[t-1];
    for(int j=1;j<=t-1;j++)
    {
      XO = ei[j-1] * (ai[t-1] * ei[j-1]);
      ei[t-1] = ei[t-1] - XO;
    }
    e = ei[t-1].l2norm();
    ei[t-1] = ei[t-1] * (1.0/e);
  }
}
