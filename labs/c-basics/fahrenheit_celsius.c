#include <stdio.h>
#include <stdlib.h>


/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    int fahr;
    int lower = atoi(argv[1]);
    int upper;
    int step;

    if(argv[2] == NULL){
    	printf("Fahrenheit: %3d, Celcius: %6.1f\n", lower, (5.0/9.0)*(lower-32));
    }
    else{
    	upper = atoi(argv[2]);
    	step = atoi(argv[3]);
    	for(fahr = lower; fahr <= upper; fahr = fahr + step){
    		printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    	}
    }
}