#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARRAYSIZE 100

void mergeSort(char *arrayList[], int init, int final, int isNumber);
void merge(char *arrayList[], int initial, int middle, int final, int (*comp)(void *, void *));
int numCmp(char *, char *);
void printArray(char *arrayList[], int arraySize);


void mergeSort(char *arrayList[], int init, int final, int isNumber){
    if (init < final){
        int middle = init + (final - init) / 2;
        mergeSort(arrayList, init, middle, isNumber);
        mergeSort(arrayList, middle + 1, final, isNumber);
        merge(arrayList, init, middle, final, (int (*)(void *, void *))(isNumber ? numCmp : strcmp));
    }
}

void merge(char *arrayList[], int initial, int middle, int final, int (*comp)(void *, void *)){
    int i, j, k;
    int leftSize = middle - initial + 1;
    int rightSize = final - middle;
    char *L[leftSize];
    char *R[rightSize];

    for (i = 0; i < leftSize; i++)
        L[i] = arrayList[initial + i];
    for (j = 0; j < rightSize; j++)
        R[j] = arrayList[middle + 1 + j];

    i = 0;
    j = 0;
    k = initial;
    while (i < leftSize && j < rightSize){
        if ((*comp)(L[i], R[j]) <= 0){
            arrayList[k] = L[i];
            i++;
        }
        else{
            arrayList[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < leftSize){
        arrayList[k] = L[i];
        i++;
        k++;
    }
    while (j < rightSize){
        arrayList[k] = R[j];
        j++;
        k++;
    }
}

void printArray(char *arrayList[], int arraySize){
    for (int i = 0; i < arraySize; i++)
    {
        //not adding comma
        if (i == arraySize - 1){
            printf("%s \n", arrayList[i]);
        }
        else{
            printf("%s, ", arrayList[i]);
        }
    }
    return;
}

int numCmp(char *d1, char *d2){
    double x1, x2;
    x1 = atof(d1);
    x2 = atof(d2);
    if (x1 < x2){
        return -1;
    }
    else if (x1 > x2){
        return 1;
    }
    else{
        return 0;
    }
}
int main(int argc, char *argv[]){
    char *file;
    //var used for sorting numbers
    int sortNumbers = 0;
    //strings
    if (argc == 2){
        file = argv[1];
    }
    //numbers
    else if (argc == 3){
        file = argv[2];
        sortNumbers = 1;
    }
    else{
        printf("ERROR on arguments... Try:\n");
        printf("./${APP_NAME}.o -n numbers.txt\n");
        printf("./${APP_NAME}.o strings.txt\n");
        return 0;
    }

    char **arrayList = malloc(sizeof(char *) * ARRAYSIZE);
    
    FILE *fp = fopen(file, "r");
    if (!fp){
        printf("ERROR! Failed to open %s file\n", file);
        return 0;
    }
    
    //reading a file
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;

    line_size = getline(&line_buf, &line_buf_size, fp);

    int numberOfLine = 0;
    while (line_size >= 0){

        //saving in array the info from file to be sorted
        arrayList[numberOfLine] = malloc(sizeof(char) * line_buf_size);
        sprintf(arrayList[numberOfLine], "%s", line_buf);
        arrayList[numberOfLine][strlen(line_buf) - 1] = 0;
        numberOfLine++;
        //new line
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    free(line_buf);
    fclose(fp);

    printf("Before MergeSort: \n");
    printArray(arrayList, ARRAYSIZE);

    mergeSort(arrayList, 0, ARRAYSIZE - 1, sortNumbers);
    printf("--------------------------------------------------------------------------\n");
    printf("After MergeSort: \n");
    printArray(arrayList, ARRAYSIZE);

    return 0;
}


