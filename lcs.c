#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200


void print_lcs(char **b, char *X, int i, int j) {
    if (i == 0 || j == 0) {
        return;
    }
    if (b[i][j] == 'D') {
        print_lcs(b, X, i - 1, j - 1);
        printf("%c", X[i - 1]);
    } else if (b[i][j] == 'U') {
        print_lcs(b, X, i - 1, j);
    } else {
        print_lcs(b, X, i, j - 1);
    }
}

int main() {
    char X[MAX], Y[MAX];
    printf("Enter first sequence: ");
    fgets(X, MAX + 1, stdin);
    X[strcspn(X, "\n")] = '\0';

    printf("Enter second sequence: ");
    fgets(Y, MAX + 1, stdin);
    Y[strcspn(Y, "\n")] = '\0';

    int m = strlen(X);
    int n = strlen(Y);

    int **c = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        c[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    char **b = (char **)malloc((m + 1) * sizeof(char *));
    for (int i = 0; i <= m; i++) {
        b[i] = (char *)malloc((n + 1) * sizeof(char));
    }

    for (int i = 1; i <= m; i++) {
        c[i][0] = 0;
    }
    for (int j = 0; j <= n; j++) {
        c[0][j] = 0;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
                b[i][j] = 'D'; 
            } else if (c[i - 1][j] >= c[i][j - 1]) {
                c[i][j] = c[i - 1][j];
                b[i][j] = 'U';
            } else {
                c[i][j] = c[i][j - 1];
                b[i][j] = 'L'; 
            }
        }
    }

    printf("Longest Common Subsequence: ");
    print_lcs(b, X, m, n);
    printf("\n");

    for (int i = 0; i <= m; i++) {
        free(c[i]);
    }
    free(c);

    for (int i = 0; i <= m; i++) {
        free(b[i]);
    }
    free(b);

    return 0;
}