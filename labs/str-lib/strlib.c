#include <stdio.h>
#include <stdlib.h>

//count length of string
int mystrlen(char *str){
    int count = 0;
    while(str[count] != NULL){
    	count++;
    }
    return count;
}

//concat
char *mystradd(char *origin, char *addition){
	int originSize = mystrlen(origin);
	int additionSize = mystrlen(addition);
    char *concatStr = malloc(originSize + additionSize);

    //first string
    for (int i = 0; i < originSize; i++){
    	concatStr[i] = origin[i];
    }
    //second string
    int i = 0;
    for(int j = originSize; j < (originSize + additionSize); j++){
    	concatStr[j] = addition[i];
    	i++;
    }
    return concatStr;
}

//find substring
int mystrfind(char *origin, char *substr){

	int originSize = mystrlen(origin);
	int subSize = mystrlen(substr);

	for(int i = 0; i < originSize; i++){

		//if the first letter of the substring is found while iterating the string
		if(origin[i] == substr[0]){
			//j is used for iterating the string
			//k is used for iterating the substring
			int j,k;
			for(j = i, k = 0; j < originSize && k < subSize; j++, k++){
				if(origin[j] != substr[k]){
					break;
				}
			}
			//if the number of iterations is the same as the size of substring
			//found
			if(k == subSize){
				return i;
			}
		}
	}

    return -1;
}
