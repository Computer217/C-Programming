// This is the starter code for lab0. Only one line comment.
#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");
    
int sum = 0, i;

i = 1;
while (i < 200)
{
    if (i%2 == 0)
    {
        sum += i;
        i += 1;
    }
    
    else
    {
    i += 1;
    }
}

printf("%d \n",sum);
return 0;
}
