#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, char **pmonth, int *pday);

int main(int argc, char ** argv) {
    if (argc==3){
        int year = atoi(argv[1]);
        int yearday=atoi(argv[2]);
        int restriction=366;
        char* month[] ={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
        if(year%4==0){
                days[1]=29;
                restriction=367;
        }
        if(year >0 && (yearday>0 && yearday<restriction)){    
            
            month_day(year,yearday,month,days);
            return 0;
        }
    }
    printf("Parameters error.\nCorrect way to run the program: ./month_day <Year> <Yearday>\n");
    exit(-1);
}

void month_day(int year, int yearday, char **pmonth, int *pday){

    for(int i=0;i<12;i++){
        if(yearday<=pday[i]){
            printf("%s %02d, %d\n",pmonth[i],yearday,year);
            return ;
        }
        yearday-=pday[i];
    }
}
