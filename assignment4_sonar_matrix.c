#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MIN_DIMENSION 2
#define MAX_DIMENSION 10
#define MAXIMUM_PIXEL 256

bool isValidMatrix(int dimension);
int getMatrixData();
void generateRandomMatrix(int *matrix, int sizeofMatrix);
void displayMatrix(int *matrix, int sizeofMatrix);
void transposeMatrix(int *matrix, int sizeofMatrix);
void reverseMatrixRow(int *matrix, int sizeofMatrix);
void rotateMatrix(int *matrix, int sizeofMatrix);
void encodeSmoothedData(int* matrix, int sizeofMatrix);
void decodeSmoothedData(int* matrix, int sizeofMatrix);
void smoothingFilter(int* matrix, int sizeofMatrix);
int calculateNeighborAvg(int* matrix, int sizeofMatrix, int row, int column);

int main()
{
    int matrixSize = getMatrixData();
    
    int *sonarMatrix = (int*)malloc(matrixSize * matrixSize * sizeof(int));
    
    if(sonarMatrix == NULL){
        printf("Memory is not allocated.\n");
        return 1;
    }

    generateRandomMatrix(sonarMatrix, matrixSize);
    printf("\nOriginal Randomly Generated Matrix:\n");
    displayMatrix(sonarMatrix, matrixSize);
    
    rotateMatrix(sonarMatrix, matrixSize);
    printf("Matrix after 90 degree Clockwise Rotation:\n");
    displayMatrix(sonarMatrix, matrixSize);
    
    smoothingFilter(sonarMatrix, matrixSize);
    printf("Matrix after Applying 3*3 Smoothing Filter:\n");
    displayMatrix(sonarMatrix, matrixSize);
    
    free(sonarMatrix);

    return 0;
}

bool isValidMatrix(int dimension) {
    return (dimension >= MIN_DIMENSION && dimension <= MAX_DIMENSION);
}

int getMatrixData() {
    int size;
    
    while(1){
        printf("Enter matrix size (%d-%d): ", MIN_DIMENSION, MAX_DIMENSION);
        if (scanf("%d", &size) != 1) {
            while (getchar() != '\n');
            size = -1; 
        }

        if (isValidMatrix(size)) {
            return size;
        }
        
        printf("Invalid matrix size. Please try between %d and %d.\n", MIN_DIMENSION, MAX_DIMENSION);
    }
}

void generateRandomMatrix(int *matrix, int sizeofMatrix) {
    srand(time(NULL)); 
    
    for (int index = 0; index < sizeofMatrix; index++) {
        for (int index_1 = 0; index_1 < sizeofMatrix; index_1++) {
            *(matrix + index * sizeofMatrix + index_1) = rand() % MAXIMUM_PIXEL; 
        }
    }
}

void displayMatrix(int *matrix, int sizeofMatrix) {
    for (int index = 0; index < sizeofMatrix; index++) {
        for (int index_1 = 0; index_1 < sizeofMatrix; index_1++) {
            printf("%4d", *(matrix + index * sizeofMatrix + index_1));
        }
        printf("\n");
    }
    printf("\n");
}

void transposeMatrix(int *matrix, int sizeofMatrix) {
    for (int index = 0; index < sizeofMatrix; index++) {
        for (int index_1 = index + 1; index_1 < sizeofMatrix; index_1++) {
            int *a = matrix + index * sizeofMatrix + index_1;
            int *b = matrix + index_1 * sizeofMatrix + index;
            int temporary = *a;
            *a = *b;
            *b = temporary;
        }
    }
}

void reverseMatrixRow(int *matrix, int sizeofMatrix) {
    for (int index = 0; index < sizeofMatrix; index++) {
        int *rowStart = matrix + index * sizeofMatrix;
        int *rowEnd = rowStart + sizeofMatrix - 1;
        while (rowStart < rowEnd) {
            int temporary = *rowStart;
            *rowStart = *rowEnd;
            *rowEnd = temporary;
            rowStart++;
            rowEnd--;
        }
    }
}

void rotateMatrix(int *matrix, int sizeofMatrix) {
    transposeMatrix(matrix, sizeofMatrix);
    reverseMatrixRow(matrix, sizeofMatrix);
}

int calculateNeighborAvg(int* matrix, int sizeofMatrix, int row, int column) {
    int sum = 0, count = 0;

    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaColumn = -1; deltaColumn <= 1; deltaColumn++) {

            int newRow = row + deltaRow;
            int newColumn = column + deltaColumn;

            if (newRow >= 0 && newRow < sizeofMatrix && newColumn >= 0 && newColumn < sizeofMatrix) {
                
                sum += *(matrix + newRow * sizeofMatrix + newColumn) % MAXIMUM_PIXEL;
                count++;
            }
        }
    }
    return sum / count;
}

void encodeSmoothedData(int* matrix, int sizeofMatrix) {
    for (int row = 0; row < sizeofMatrix; row++) {
        for (int column = 0; column < sizeofMatrix; column++) {
            int average = calculateNeighborAvg(matrix, sizeofMatrix, row, column);
            
            *(matrix + row * sizeofMatrix + column) += average * MAXIMUM_PIXEL;
        }
    }
}

void decodeSmoothedData(int* matrix, int sizeofMatrix) {
    for (int row = 0; row < sizeofMatrix; row++) {
        for (int column = 0; column < sizeofMatrix; column++) {
            
            *(matrix + row * sizeofMatrix + column) /= MAXIMUM_PIXEL;
        }
    }
}

void smoothingFilter(int* matrix, int sizeofMatrix) {
    encodeSmoothedData(matrix, sizeofMatrix);
    decodeSmoothedData(matrix, sizeofMatrix);
}