#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>


void clear();
int getFiles(char *id);

void getValues(char *id){
  char path[64], buff[128];
  FILE *f;
  sprintf(path,"/proc/%s/stat",id);
  char *name,*stat,*memory;
  unsigned int par,thread,op;
  f=fopen(path,"r");
  if(f==NULL){
    printf("%s\n",path);
    perror("Error reading file");
    exit(-1);
  }
  int counter=0;
  fgets(buff,128,f);
  char *inicial=strtok(buff," ");
  while(inicial!=NULL){
    switch(counter){
      case 1:
        name=inicial;
        break;
      case 2:
        switch(inicial[0]){
            case 'S':
                stat="Sleeping";
                break;
            case 'T':
                stat="Stopped";
                break;
            case 'I':
                stat="Idle";
                break;
            case 'R':
                stat="Running";
                break;
            default:
                stat=inicial;
                break;
        }
        break;      
      case 3:
        par=atoi(inicial);
        break;
      case 19:
        thread=atoi(inicial);
        break;
      case 22:
        memory=inicial;
        break;
    }
    inicial=strtok(NULL," ");
    counter+=1;
  }
  fclose(f);
  int filenames=getFiles(id);
  printf("|%7s |%40s |%15s |%7d |%15s |%7d |%10d |\n",id,name,stat,par,memory,thread,filenames);
    
}

int getFiles(char *id){
    struct dirent *d;
    char path[32];
    sprintf(path,"/proc/%s/fd",id);
    DIR *dir=opendir(path);
    if(dir==NULL){
      perror("error");
      exit(-1);
    }
    int files=0;
    while((d=readdir(dir)) != NULL){
        files+=1;
    }
    closedir(dir);
    return files;
}


void printtop(){
  struct dirent *d;
  DIR *dr;
    dr = opendir("/proc");
    if(dr==NULL){
      perror("error");
      exit(-1);
    }
    printf("|%7s |%40s |%15s |%7s |%15s |%3s |%10s |\n","PID","NAME","STATUS","PPID","MEMORY","THREADS","OPEN FILES");
    while ((d = readdir(dr)) != NULL) {
      if(isdigit(*d->d_name)){
        getValues(d->d_name);
      }
    }
    closedir(dr);
}
int main(){
    while(1){
        printtop();
        sleep(1);
        clear();
    }
}
void clear() {
  printf("\e[1;1H\e[2J \n"); 
}
