#include <stdio.h>
#include <stdlib.h>


/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    if(argc==2){
        int fahr=atoi(argv[1]);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
        return 0;
    }
    if(argc==4){
        int LOWER=atoi(argv[1]);
        int UPPER=atoi(argv[2]);
        int STEP=atoi(argv[3]);
        int fahr;
        if(STEP<=0){
            printf("Error, please insert a value higher than 0 for the increment.\nTo make a range conversion use: ./fahrenheit_celsius <start> <end> <increment>\n");
            exit(-1);
        }
        for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
	    printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
        return 0;
    }
    printf("Error, invalid number of arguments.\nTo convert one value use: ./fahrenheit_celsius <num_fahrenheit_degrees>\nTo make a range conversion use: ./fahrenheit_celsius <start> <end> <increment>\n");
    exit(-1);
}
