#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARRAYSIZE 100

void mergeSort(char *array[], int init, int final, int isNumber);
void merge(char *array[], int initial, int middle, int final, int (*comp)(void *, void *));
int numCmp(char *, char *);
void printArray(char *array[], int arraySize);


void mergeSort(char *array[], int init, int final, int isNumber){
    if (init < final){
        int middle = init + (final - init) / 2;
        mergeSort(array, init, middle, isNumber);
        mergeSort(array, middle + 1, final, isNumber);
        merge(array, init, middle, final, (int (*)(void *, void *))(isNumber ? numCmp : strcmp));
    }
}

void merge(char *array[], int initial, int middle, int final, int (*comp)(void *, void *)){
    int i, j, k;
    int leftSize = middle - initial + 1;
    int rightSize = final - middle;
    char *L[leftSize];
    char *R[rightSize];
    for (i = 0; i < leftSize; i++){
        L[i] = array[initial + i];
    }
    for (j = 0; j < rightSize; j++){
        R[j] = array[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = initial;

    while (i < leftSize && j < rightSize){
        if ((*comp)(L[i], R[j]) <= 0){
            array[k] = L[i];
            i++;
        }
        else{
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < leftSize){
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < rightSize){
        array[k] = R[j];
        j++;
        k++;
    }
}

void printArray(char *array[], int arraySize){
    for (int i = 0; i < arraySize; i++)
    {
        //not adding comma
        if (i == arraySize - 1){
            printf("%s \n", array[i]);
        }
        else{
            printf("%s, ", array[i]);
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

    char **array = malloc(sizeof(char *) * ARRAYSIZE);
    
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
        array[numberOfLine] = malloc(sizeof(char) * line_buf_size);
        sprintf(array[numberOfLine], "%s", line_buf);
        array[numberOfLine][strlen(line_buf) - 1] = 0;
        numberOfLine++;
        //new line
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    free(line_buf);
    fclose(fp);

    printf("Before MergeSort: \n");
    printArray(array, ARRAYSIZE);
    mergeSort(array, 0, ARRAYSIZE - 1, sortNumbers);
    printf("--------------------------------------------------------------------------\n");
    printf("After MergeSort: \n");
    printArray(array, ARRAYSIZE);

    return 0;
}


