#include <stdio.h>

/*
 * Print an integer (of int) in four formats:
 */
 
 
 
/*
 * The output is:
 * name
 * binary
 * hexadecimal
 * decimal with bits interpreted as signed int
 * decimal with bits interpreted as unsigned int.
 *
 * The function does not return a value.
 */
void print_int(int n, char *name) // pointer to what??
{
    // print name
    printf("%s\tbin:", name ? name : "");

    // a loop to print bits from most significant to least significant
    // also an example of using sizeof
    for (int i = sizeof(int) * 8 - 1; i >= 0; i --)
       printf("%d", (n >> i) & 1);

    // other formats
    printf(" hex:%08X signed:%10d unsigned:%10u\n", n, n, n);
}

int main(void){
     int a=0, b=0;
     double r=0;
     char c;
     while (scanf("%i %c%i", &a, &c, &b) == 3) {
         if (c=='+') {
             r = a + b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='-'){
             r = a - b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='*'){
             r = a * b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='/'){
             r = a / b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='%'){
             r = a % b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='&'){
             r = a & b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='|'){
             r = a | b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='^'){
             r = a ^ b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='l'){
             r = a << b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         else if (c=='r'){
             r = a >> b;
             print_int(a,"a");
             printf("%c\n",c);
             print_int(b,"b");
             printf("=\n");
             print_int(r,"r");
         }
         
    }
  return 0;
}