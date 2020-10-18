#include    <stdio.h>
#include    <stdlib.h>

// return the number of perfect shuffles that place a deck of n cards back to original order
// n must be positive
int CompareArray(int array1[], int array2[], int n){
	int boolean = 1;
	for (int i=0; i<n; i++){
		if (array1[i] == array2[i])
		{
			continue;
		}
		else
		{
			boolean = 0;
			break;
		}
	}
	return boolean;
}


int find_cycle(int n)
{
    // TODO
    int count = 0; 
    // initialize the array of cards
    int cards[n];
    int original[n];
    int midpoint=0;
    int coun=0;
    int x=0;
    // make array numbering all the cards
    for (int i = 0; i < n; i++){
    	cards[i] = i;
    	original[i] = i;
    }
    //split array in two and sort
    while (x != 1)
    {
	    if (n % 2 == 0)
	    {
	    	midpoint = n/2; 
	    	int L1[midpoint];
	    	int R1[midpoint];
	    	for (int i = 0; i < midpoint; i++){
	    		L1[i] = cards[i];
	    	}
	    		
	    	for (int i = midpoint; i < n; i++){
	    		R1[coun] = cards[i];
	    		coun += 1;
	    	}
	    	coun = 0;
	    	//sort
	    	for (int i=0; i<n; i+=2){
	    		cards[i]=L1[coun];
	    		coun += 1;
	    	}
	    	coun = 0;
	    	for (int i=1; i<n; i+=2){
	    		cards[i]=R1[coun];
	    		coun += 1;
			}
			coun = 0;
	    }
	    else{
	    	midpoint = n/2 + 1;
	    	int L1[midpoint];
	    	int R1[midpoint];
			for (int i=0; i < midpoint; i++){
				L1[i] = cards[i];
			}
			
			for (int i = midpoint; i < n; i++){
	    		R1[coun] = cards[i];
	    		coun += 1;
	    	}
	    	coun = 0;
	    	//sort
	    	for (int i=0; i<n; i+=2){
	    		cards[i]=L1[coun];
	    		coun += 1;
	    	}
	    	coun = 0;
	    	for (int i=1; i<n; i+=2){
	    		cards[i]=R1[coun];
	    		coun += 1;
			}
			coun = 0;
	    }
	    x = CompareArray(cards, original, n);
	    //printf("%d\n",x);
	    count += 1;
    }
   
   return count;
    
    
    //make copy of array by pointing at address
    //original = (int*)malloc(sizeof(int)*n);
    //original = &cards;	//points to address of original cards
    
	//cards[0] = 1;
    //printf("%d\n", cards[0]);
    //printf("%d\n", original[0]);

    
} 

int main(int argc, char **argv)
{
    int     n;

    /* the upper bound of n is not checked. Try what happens if you enter a large number */
    if (argc == 1) {
        int     rv;
        while ((rv = scanf("%d", &n)) >= 0) {
            if (rv != 1 || n < 1) {
                printf("Number of cards must be a positive integer.\n");
                return 1;
            }
            printf("%d %d\n", n, find_cycle(n)); 
        }
    }
    else {
        for (int i = 1; i < argc; i ++) {
            n = atoi(argv[i]);
            if (n < 1) {
                printf("Number of cards must be a positive integer.\n");
                return 1;
            }
            printf("%d %d\n", n, find_cycle(n)); 
        }
    }
    return 0;
}
