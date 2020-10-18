/* Compute e ** x, using Taylor series.  */

#include <stdio.h>

int main(void)
{
    double x=0, sum=1, mult=1;
    int n=0, count=1;
    while (scanf("%lf%d", &x, &n) == 2)
    {
        sum = 1;
        mult = 1;
        for (count=1; count < n; count++)
        {
            sum += (mult *= (x/(float)count));
        }
        printf("exp(%.10lf)=%.10lf\n", x, sum);
    }
}

  /*  scanf("%lf%d", &x, &n);
    while (n  != 0) { // %lf == double and %d == int
            sum += (mult *= x/count);
            n -= 1;
            count += 1;
        }
  printf("exp(%.10lf) = %.10lf\n", x, sum);
  return 0;
}*/