#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PrintArray(char *a, int length){  //Function to print array  
    for(int i=0;i<length;i++)    //Here we go backwards in the array
        printf("%c",a[i]);      //Printing each char
    printf("\n"); //New line
}
void strReverser(char *a,int length){
    char *rev = malloc(length* sizeof(char)); //Allocating memory for a new array to store the reversed string
    for(int i=length-1;i>=0;i--)    //Here we go backwards in the array
        rev[length-(i+1)]=a[i];      //storing each char in the reversed array
    PrintArray(rev,length);
}

int main(){
    char word[64]; //String length = 64
    int d;  
    int counting=0; //Declare this counter to assign each char to the array
    while( (d=getchar()) !=EOF){ //Here i scan each char and check if is not End Of File (Ctrl + D)
        if(d != '\n'){      //check that the char introduced is different than end of line (\n)
            word[counting]=d;   //Storing each char in the array
            counting+=1;    //Incrementing the counter;
        }
        else{                           //When user hits enter
            strReverser(word,counting); //Calling function to reverse and print the string
            //PrintArray(word,counting);  //Calling the function to print my array
            strcpy(word,"");    //Clearing the array | Making the string empty again
            counting=0;    //Making counter 0 again
        }
    }
    
}