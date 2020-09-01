#include <stdio.h>

int main(){
    char c;
    int numChar = 0;
    char arrayChar[500];

    while((c = getchar()) != EOF ){

        if(c == '\n'){
            numChar--;
            //printf("Reversed word: ");
            while(numChar >= 0){                
                printf("%c",arrayChar[numChar]);
                numChar--;
            }
            numChar = 0;
            printf("\n");

        }
        else{
            arrayChar[numChar] = c;
            numChar++;
        }

    }
    return 0;
}
