
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to print optimal parenthesization
void printOptimalParenthesis(int i, int j, int **s, char *name) {
    if (i == j) {
        printf("A%d", i);
        return;
    }
    printf("(");
    printOptimalParenthesis(i, s[i][j], s, name);
    printOptimalParenthesis(s[i][j] + 1, j, s, name);
    printf(")");
}

void matrixChainMultiplication(int p[], int n) {
    // Allocating memory for m and s
    int **m = (int **)malloc(n * sizeof(int *));
    int **s = (int **)malloc(n * sizeof(int *)); // Stores the optimal split positions

    for (int i = 0; i < n; i++) {
        m[i] = (int *)malloc(n * sizeof(int));
        s[i] = (int *)malloc(n * sizeof(int));
    }

  
    for (int i = 1; i < n; i++) {
        m[i][i] = 0;
    }

    for (int L = 2; L < n; L++) {
        for (int i = 1; i < n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = INT_MAX;

            for (int k = i; k < j; k++) {
                // Calculate cost
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];

                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k; 
                }
            }
        }
    }
    printf("Minimum number of scalar multiplications: %d\n", m[1][n - 1]);
    printf("Optimal Parenthesization: ");
    char matrixName = 'A';
    printOptimalParenthesis(1, n - 1, s, &matrixName);
    printf("\n");

    for (int i = 0; i < n; i++) {
        free(m[i]);
        free(s[i]);
    }
    free(m);
    free(s);
}

int main() {
    int p;
    printf("Enter the number of matrices: ");
    scanf("%d", &p);

    if (p <= 0) {
        printf("Invalid number of matrices. Please enter a positive integer.\n");
        return 1;
    }

    int *arr = (int *)malloc((p + 1) * sizeof(int)); 
    printf("Enter the dimensions of the matrices: ");

    for (int i = 0; i <= p; i++) { 
        scanf("%d", &arr[i]);
    }

    matrixChainMultiplication(arr, p + 1); 
    free(arr);

    return 0;
}
