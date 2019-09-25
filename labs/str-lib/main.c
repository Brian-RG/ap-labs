#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv) {
    if(argc!=4){
        printf("Bad usage of program.\nCorrect use:%s <Original String> <String to add> <Substring to search>\n",argv[0]);
        exit(-1);
    }
    int strlength=mystrlen(argv[1]);
    char* newstr=mystradd(argv[1],argv[2]);
    //originalVeryLongString Addition Add
    printf("Original string length: %d\n",strlength);
    printf("New string: %s\n",newstr);
    //int found=mystrfind(newstr,argv[3]);
    //char *f=found?"yes":"no";
    int *found;
    printf("SubString was found : %s\n",found=mystrfind(newstr,argv[3])?"yes":"no");

    return 0;
}
