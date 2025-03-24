
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int comparisons = 0;

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1 + 1], R[n2 + 1];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    L[n1] = INT_MAX;
    R[n2] = INT_MAX;

    int i = 0, j = 0;
    for (int k = l; k <= r; k++) {
        comparisons++;
        if (L[i] <= R[j]) arr[k] = L[i++];
        else arr[k] = R[j++];
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() {
    int N, n;
    printf("Enter number of arrays (N): ");
    scanf("%d", &N);
    printf("Enter number of elements in each array (n): ");
    scanf("%d", &n);

    srand(time(NULL));

    FILE *infile = fopen("arrays_input.txt", "w");
    FILE *outfile = fopen("arrays_sorted_output.txt", "w");

    int totalComparisons = 0;

    for (int arrayIndex = 0; arrayIndex < N; arrayIndex++) {
        int arr[n];

        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 1000;  
            fprintf(infile, "%d ", arr[i]);
        }
        fprintf(infile, "\n");

        comparisons = 0;

       
        mergeSort(arr, 0, n - 1);

        for (int i = 0; i < n; i++) {
            fprintf(outfile, "%d ", arr[i]);
        }
        fprintf(outfile, "\n");

        printf("Array %d: Comparisons = %d\n", arrayIndex + 1, comparisons);
        totalComparisons += comparisons;
    }

    double avgComparisons = (double)totalComparisons / N;
    printf("Average Comparisons across %d arrays = %.2lf\n", N, avgComparisons);

    fclose(infile);
    fclose(outfile);
    return 0;
}
