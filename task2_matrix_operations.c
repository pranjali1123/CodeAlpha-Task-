#include <stdio.h>

#define MAX 10

// Function prototypes
void inputMatrix(int matrix[MAX][MAX], int rows, int cols, char name);
void displayMatrix(int matrix[MAX][MAX], int rows, int cols, char name);
void addMatrices(int mat1[MAX][MAX], int mat2[MAX][MAX], int result[MAX][MAX], int rows, int cols);
void multiplyMatrices(int mat1[MAX][MAX], int mat2[MAX][MAX], int result[MAX][MAX], int r1, int c1, int r2, int c2);
void transposeMatrix(int matrix[MAX][MAX], int result[MAX][MAX], int rows, int cols);

int main() {
    int mat1[MAX][MAX], mat2[MAX][MAX], result[MAX][MAX];
    int rows1, cols1, rows2, cols2;
    int choice;
    
    printf("=== Matrix Operations Program ===\n\n");
    
    // Input first matrix
    printf("Enter dimensions of Matrix A (rows cols): ");
    scanf("%d %d", &rows1, &cols1);
    inputMatrix(mat1, rows1, cols1, 'A');
    
    // Menu for operations
    do {
        printf("\n=== Matrix Operations Menu ===\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Matrix Transpose\n");
        printf("4. Display Matrix A\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                // Matrix Addition
                printf("\nEnter dimensions of Matrix B (rows cols): ");
                scanf("%d %d", &rows2, &cols2);
                
                if(rows1 != rows2 || cols1 != cols2) {
                    printf("Error: Matrices must have same dimensions for addition!\n");
                } else {
                    inputMatrix(mat2, rows2, cols2, 'B');
                    addMatrices(mat1, mat2, result, rows1, cols1);
                    printf("\nResult of Matrix Addition:\n");
                    displayMatrix(result, rows1, cols1, 'C');
                }
                break;
                
            case 2:
                // Matrix Multiplication
                printf("\nEnter dimensions of Matrix B (rows cols): ");
                scanf("%d %d", &rows2, &cols2);
                
                if(cols1 != rows2) {
                    printf("Error: Columns of Matrix A must equal rows of Matrix B!\n");
                } else {
                    inputMatrix(mat2, rows2, cols2, 'B');
                    multiplyMatrices(mat1, mat2, result, rows1, cols1, rows2, cols2);
                    printf("\nResult of Matrix Multiplication:\n");
                    displayMatrix(result, rows1, cols2, 'C');
                }
                break;
                
            case 3:
                // Matrix Transpose
                transposeMatrix(mat1, result, rows1, cols1);
                printf("\nTranspose of Matrix A:\n");
                displayMatrix(result, cols1, rows1, 'T');
                break;
                
            case 4:
                // Display Matrix
                displayMatrix(mat1, rows1, cols1, 'A');
                break;
                
            case 5:
                printf("\nExiting program...\n");
                break;
                
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 5);
    
    return 0;
}

// Function to input matrix elements
void inputMatrix(int matrix[MAX][MAX], int rows, int cols, char name) {
    printf("\nEnter elements of Matrix %c (%dx%d):\n", name, rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i+1, j+1);
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Function to display matrix
void displayMatrix(int matrix[MAX][MAX], int rows, int cols, char name) {
    printf("\nMatrix %c (%dx%d):\n", name, rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%6d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to add two matrices
void addMatrices(int mat1[MAX][MAX], int mat2[MAX][MAX], int result[MAX][MAX], int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

// Function to multiply two matrices
void multiplyMatrices(int mat1[MAX][MAX], int mat2[MAX][MAX], int result[MAX][MAX], int r1, int c1, int r2, int c2) {
    // Initialize result matrix to 0
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            result[i][j] = 0;
        }
    }
    
    // Multiply matrices
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            for(int k = 0; k < c1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Function to transpose a matrix
void transposeMatrix(int matrix[MAX][MAX], int result[MAX][MAX], int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}
