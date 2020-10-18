// Only this line of comment is provided 
#include <stdio.h>


float main(void) {
    double x;
    double total=0;
    double average;
    int i=1;
    while (scanf("%lf", &x) == 1){
        total += x;
        average = total/i;
        i += 1;
        printf("Total=%f Average=%f\n", total, average);
    }
  return 0;
}