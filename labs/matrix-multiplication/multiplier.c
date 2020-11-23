#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include <pthread.h>

#define NUM_THREADS 2000
#define MATRIX_SIZE 2000

long * readMatrix(char *filename);
long * getColumn(int col, long *matrix);
long * getRow(int row, long *matrix);
int getLock();
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long * multiply(long *matA, long *matB);
int saveResultMatrix(long *result);


int NUM_BUFFERS;
char* RESULT_MATRIX_FILE;
long **buffers;
pthread_mutex_t *mutexes;
pthread_t threads[NUM_THREADS];

struct matIndex {
    int row;
    int col;
    long* matA;
    long* matB;
};

int main(int argc, char *argv[]){
     if (argc < 5)
    {
        errorf("ERROR: Invalid parameters");
        exit(1);
    }
    else
    {
        // Validation
        if (strcmp(argv[1], "-n") == 0 && strcmp(argv[3], "-out") == 0)
        {
            NUM_BUFFERS = atoi(argv[2]);
            RESULT_MATRIX_FILE = argv[4];
        }
        else if (strcmp(argv[1], "-out") == 0 && strcmp(argv[3], "-n") == 0)
        {
            NUM_BUFFERS = atoi(argv[4]);
            RESULT_MATRIX_FILE = argv[2];
        }
        else
        {
            errorf("ERROR: Input not valid");
            exit(1);
        }
    }

    buffers = malloc(NUM_BUFFERS * sizeof(long *));
    mutexes = malloc(NUM_BUFFERS * sizeof(pthread_mutex_t));

    for(int i = 0; i < NUM_BUFFERS; i++){
        pthread_mutex_init(&mutexes[i], NULL);
    }

    long *matrixA = readMatrix("matA.dat");
    long *matrixB = readMatrix("matB.dat");

    long *result = multiply(matrixA,matrixB);
    infof("INFO: Operations done\n");

    saveResultMatrix(result);
    free(matrixA);
    free(matrixB);
    free(mutexes);
    free(buffers);
    free(result);

    return 0;
}

long *readMatrix(char *filename){
    FILE *fp = fopen(filename, "r");
    long lines = 0;

    if(fp == NULL){
        errorf("ERROR: Error trying to open the file");
        return NULL;
    }
    char c;
    while((c = fgetc(fp))!=EOF){
        if(c == '\n')
            lines++;
    }
    rewind(fp);
    long *matrix = malloc(lines * sizeof(long));
    int i = 0;
    while(fscanf(fp,"%ld", &matrix[i]) != EOF){
        i++;
    }
    fclose(fp);
    return matrix;  
}

long *getRow(int r, long *matrix) {
    long *row = malloc(MATRIX_SIZE * sizeof(long));
    for(int i = 0; i < MATRIX_SIZE; i++){
        row[i] = matrix[r * MATRIX_SIZE + i];
    }
    return row;
}

long *getColumn(int col, long *matrix) {
    long *column = malloc(MATRIX_SIZE * sizeof(long));
    for (int i = 0; i < MATRIX_SIZE; i++){
        column[i] = matrix[i * MATRIX_SIZE + col];
    }
    return column;
}

int getLock() {
    for(int i = 0; i < NUM_BUFFERS; i++){
        if(pthread_mutex_trylock(&mutexes[i])==0){
            return i;
        }   
    }
    return -1;
}

int releaseLock(int lock) {
    if(pthread_mutex_unlock(&mutexes[lock]) == 0){
        return 0;
    }
    return -1;
}

long dotProduct(long *vec1, long *vec2) {
    long res = 0;
    for(int i = 0; i < MATRIX_SIZE; i++) {
        res += (vec1[i] * vec2[i]);
    }
    return res;
}

long multiplyThread(struct matIndex *data){
    int buffA = -1, buffB = -1;
    while(buffA == -1 || buffB == -1){
        if(buffA == -1){
            buffA = getLock();
        }
        if(buffB == -1){
            buffB = getLock();
        }
    }
    buffers[buffA] = getRow(data->row, data->matA);
    buffers[buffB] = getColumn(data->col, data->matB);
    long res = dotProduct(buffers[buffA], buffers[buffB]);
    free(buffers[buffA]);
    free(buffers[buffB]);
    free(data);
    releaseLock(buffA);
    releaseLock(buffB);
    return res;
}

long * multiply(long *matA, long *matB) {
    infof("INFO: Starting Multiplication\n");
    long *data = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(long));

    for(int i = 0; i < NUM_THREADS; i++){
        for(int j = 0; j < NUM_THREADS; j++){
            struct matIndex *curr_matIndex = malloc(sizeof(struct matIndex));
            curr_matIndex->row = i;
            curr_matIndex->col = j;
            curr_matIndex->matA = matA;
            curr_matIndex->matB = matB;
            pthread_create(&threads[j], NULL, (void * (*)(void *))multiplyThread, (void *)curr_matIndex);
        }

        for(int j = 0; j < NUM_THREADS; j++) {
            void *status;
            pthread_join(threads[j], &status);
            data[NUM_THREADS * j + i] = (long) status;
        }
    }
    return data;
}

int saveResultMatrix(long *result) {    
    FILE *fp = fopen(RESULT_MATRIX_FILE, "w+");
    if(fp == NULL){
         errorf("ERROR: Error while opening the file");
        return -1;
    }
    infof("INFO: Results saved at %s\n", RESULT_MATRIX_FILE);
    int size = MATRIX_SIZE * MATRIX_SIZE;
    for(int i = 0; i < size; i++) {
        fprintf(fp, "%ld\n", result[i]);
    }
    fclose(fp);
    return 0;
}