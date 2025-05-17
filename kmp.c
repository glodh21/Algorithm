//bacbababacababaca
//ababaca

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_prefix_function(const char *P, int m, int *pi) {
    pi[0] = 0;
    int k = 0;
    for(int q = 1; q < m; q++) {
        while (k > 0 && P[k] != P[q]) {
            k = pi[k - 1];
        }
        if (P[k] == P[q]) {
            k++;
        }
        pi[q] = k;
    }
}

void kmp_matcher(const char *T, const char *P) {
    int n = strlen(T);
    int m = strlen(P);
    int *pi = (int *)malloc(sizeof(int) * m);
    if (!pi) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
 
    compute_prefix_function(P, m, pi);
    int q = 0;  
    for (int i = 0; i < n; i++) {
        while (q > 0 && P[q] != T[i]) {
            q = pi[q - 1];   
        }
        if (P[q] == T[i]) {
            q++;
        }
        if (q == m) {
            printf("Pattern occurs at index %d\n", i - m + 1);
            q = pi[q - 1];    
        }
    }
    
    free(pi);
}

int main(void) {
    char T[10000], P[10000];
    
    printf("Enter text T: ");
    if (!fgets(T, sizeof(T), stdin)) return 0;
    T[strcspn(T, "\n")] = '\0';
    
    printf("Enter pattern P: ");
    if (!fgets(P, sizeof(P), stdin)) return 0;
    P[strcspn(P, "\n")] = '\0';
    
    printf("\nSearching for \"%s\" in \"%s\"\n", P, T);
    kmp_matcher(T, P);
    
    return 0;
}