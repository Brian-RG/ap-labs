int mystrlen(char *str){
    int i=0;            //Initializing i as my counter
    while(str[i]!='\0'){//Loop while i is not '\0' that is the end of every string.
        i+=1;           //Just adding 1 to the counter
    }
    return i;//Return the actual size of the string
}

char *mystradd(char *origin, char *addition){
    int a=mystrlen(origin);                 //Getting the length of the original string
    int b=mystrlen(addition);               
    char *new=malloc((a+b)*sizeof(char));   //Allocating enough memory to store a new string containing both
    int i=0;                                //Initialize i as my counter that will fill the new string
    for(i;i<a;i++){                         
        new[i]=origin[i];                   //Assigning the value of each character in the original string to the new one
    }
    for(int j=0;j<b;j++){
        new[i++]=addition[j];               //Adding the additional string
    }
    return new;
}

int mystrfind(char *origin, char *substr){
    int a=mystrlen(origin);                 //
    int b=mystrlen(substr);
    int i=0;
    int val=0;
    for(i;i<a;i++){
        if(origin[i]==substr[0]) {
             val=1;
	     for(int j=1;j<b;j++){
                if(origin[i+j]!=substr[j]){
                    val=0;
                    break;
                }
                else if(!val){
                    val=1;
                }
            }
            if(val){
                break;
            }
        }
    } 
    return val;
}
