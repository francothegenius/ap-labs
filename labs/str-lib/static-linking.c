#include <stdio.h>
#include <string.h>
int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char **argv) {


	if(argc < 4){
		printf("Input invalid, more arguments needed\n");
		printf("Format as follows: ./xxxx -add 'String1' 'String2' OR ./xxxx -find 'String' 'Substring'\n");
	} else{
		if(strcmp(argv[1],"-add") == 0){
			printf("Initial Length	: %d\n", mystrlen(argv[2]));
			printf("New String      : %s\n", mystradd(argv[2], argv[3]));
			printf("New Length 	    : %d\n", mystrlen(mystradd(argv[2], argv[3])));
		}
		else if(strcmp(argv[1],"-find") == 0){
			if(mystrfind(argv[2],argv[3]) != -1){
				printf("['%s'] string was found at %d position\n", argv[3], mystrfind(argv[2], argv[3]));
			} else{
				printf("['%s'] string was not found \n", argv[3]);
			}
		}
	}
}
