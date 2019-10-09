#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

void filecopy(int fd)
{
    char c[1];
    
    while(read(fd,c,1)){ //Reading char by char
        if(write(1,c,1)==-1){
            printf("ERROR WRITING\n");
            return ;
        }
        
    }
    c[0]='\n';
    write(1,c,1);

}

int main(int argc, char *argv[])
{

    if(argc==1){
        filecopy(0);
    }
    else{
        int fd;
        while(--argc >0){
            if( (fd=open(*++argv,O_RDONLY)) <0){
                printf(stderr, "%s: can't open %s\n",argv[0],*argv);
                return -1;
            }
            else{
                filecopy(fd);
                close(fd);
            }
        }
    }
    if(ferror(stdout)){
        fprintf(stderr,"%s: eeror writing stdout\n",argv[0]);
        return -1;
    }
    return 0;

}
