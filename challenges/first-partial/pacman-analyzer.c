#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define REPORT_FILE "packages_report.txt"

int installed=0;
int removed=0;
int upgraded=0;
int actualinstalled=0;

struct pack{
    char* name;
    char* installdate;
    char* update;
    char* removedate;
    char Isup;
    int updates;
    struct pack* next;
};
struct hash{
   struct pack* packages[64]; 
};
struct pack* newPack(char* n, char* d){
    struct pack* p=(struct pack*)malloc(sizeof(struct pack));
    p->name=n;
    p->installdate=d;
    p->update="-";
    p->removedate="-";
    p->Isup='F';
    p->updates=0;
}
struct hash* createhash(){
    struct hash *h=(struct hash*)malloc(sizeof(struct hash));
    return h;
}
int hashcode(char *name){
    return (strlen(name)%64);
}
void insert(struct hash* hm, char* nombre,char* fecha){
    struct pack* p=newPack(nombre,fecha);
    int hcode=hashcode(p->name);
    if(hm->packages[hcode]==NULL){
        hm->packages[hcode]=p;
        return;
    }
    else{
        struct pack* tmp=hm->packages[hcode];
        while(tmp){
            if(tmp->next==NULL){
                tmp->next=p;
                return;
            }
            tmp=tmp->next;
        }
    }

}
struct pack* search(struct hash* hm, char* nombre){
    int pos=hashcode(nombre);
    struct pack* p=hm->packages[pos];
    while(p){
        if(strcmp(p->name,nombre)==0){
            return p;
        }
        p=p->next;
    }
    return NULL;
}
char* getPosDate(char* line){
    char *date=malloc(sizeof(char)*18);
    for(int i=1;i<17;i++){
        date[i-1]=line[i];
    }
    return date;
}
char* getMethod(char* line){
    char *met=malloc(sizeof(char)*4);
    int c=19;
    int counter=0;
    while(line[c]!=' '){
        c+=1;
    }
    c+=1;
    while(counter<3){
        met[counter]=line[c];
        c++;
        counter++;
    }
    return met;
}
char* getPackName(char* line,int offset){
    char *name=malloc(sizeof(char)*32);
    int c=19;
    while(line[c]!=' '){
        c+=1;
    }
    c+=offset;
    int counter=0;
    while(line[c]!=' '){
        name[counter]=line[c];
        c+=1;
        counter+=1;
    }
    return name;
}
void analizeLog(char *logFile, char *report);
void analize(char *buffer, struct hash* hm){
    char *date;
    char *met=getMethod(buffer);
    char *name;
    if(strcmp(met,"ins")==0){
        name=getPackName(buffer,11);
        date=getPosDate(buffer);
        struct pack *p=search(hm,name);
        if(p){
            p->installdate=date;
        }
        else{
            insert(hm,name,date);
            installed+=1;
            actualinstalled+=1;
        }
    }
    else if(strcmp(met,"rem")==0){
        name=getPackName(buffer,9);
        date=getPosDate(buffer);
        struct pack *p=search(hm,name);
        if(p){
            p->removedate=date;
            removed+=1;
            actualinstalled-=1;
        }
        else{
            exit(-1);
        }
    }
    else if(strcmp(met,"upg")==0){
        name=getPackName(buffer,10);
        date=getPosDate(buffer);
        struct pack* p=search(hm,name);
        if(p){
            p->update=date;
            p->updates+=1;
            if(p->Isup=='F'){
                upgraded+=1;
                p->Isup='T';
            }
        }
        else{
            printf("ERROR, MISSING PACKAGE\n");
            exit(-1);
        }
    }

}
int readline(int fd,char *buffer,int *state){ //Function to read line
    memset(buffer,0,128*sizeof(char)); //Clearing the buffer
    char c[1];
    int counter=0;
    
    if(*state==0){ //This is for the first time i enter in the function
        *state=1;
        while(read(fd,c,1)){ //reading char by char
            if(c[0]!='\n'){
                buffer[counter]=c[0];
            }
            else{
                return 1;
            }
            counter+=1;
        }
    }
    else if(*state==1){     
        while(read(fd,c,1)){ //reading char by char
            if(c[0]!='\n'){
                buffer[counter]=c[0];
            }
            else{
                return 1;
            }
            counter+=1;
        }
    }
    return -1;
}

int write_register(int outfd){
    char first[256];
    sprintf(first,"Pacman Packages Report\n----------------------\n- Installed packages : %d\n- Removed packages   : %d\n- Upgraded packages  : %d\n- Current installed  : %d\n\n",installed,removed,upgraded,actualinstalled);
    if(write(outfd,first,strlen(first))==-1){
        return -1;
    }
    return 1;
}
int write_packages(int outfd,struct hash* hm){
    char package_info[512]="List of packages\n----------------\n";
    if(write(outfd,package_info,strlen(package_info)) ==-1){
        return -1;
        }
    for(int i=0;i<64;i++){    
        struct pack* p=hm->packages[i];
        while(p){
            memset(package_info,0,256*sizeof(char));
            sprintf(package_info,"- Package Name        : %s\n  - Install date      : %s\n  - Last update date  : %s\n  - How many updates  : %d\n  - Removal date      : %s\n",p->name,p->installdate,p->update,p->updates,p->removedate);
            if(write(outfd,package_info,strlen(package_info))==-1){
                return -1;
            }
            p=p->next;
        }
    }
    return 1;
}
int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }
    analizeLog(argv[1], REPORT_FILE);
    return 0;
}
void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    struct hash* hm=createhash();
    int fd,outfd;
    int buf=512;
    char buffer[buf];
    int x=0 ; //starting to read
    int *z=&x;
    fd=open(logFile,O_RDONLY);
    if(fd==-1){
        printf("Error\n");
        exit(-1);
    }
    int counter=1;
    while(readline(fd,buffer,z) >0){
        analize(buffer,hm);
    }
    close(fd);
    outfd=open(report,O_CREAT | O_WRONLY | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(outfd<0){
        printf("Error opening the report file\n");
        exit(-1);
    }
    if(write_register(outfd)==-1){
        printf("Error writing the report file\n");
    }
    if(write_packages(outfd,hm)==-1){
        printf("Error writing the report file\n");
    }
    close(outfd);
    printf("Report is generated at: [%s]\n", report);
}
