#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int comparisons = 0;

int hoarePartition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;
    while (1) {
        do {
            i++;
            comparisons++;
        } while (arr[i] < pivot);
        do {
            j--;
            comparisons++;
        } while (arr[j] > pivot);

        if (i >= j)
            return j;

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int p = hoarePartition(arr, low, high);
        quickSort(arr, low, p);
        quickSort(arr, p + 1, high);
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
            arr[i] = rand() % 100;
            fprintf(infile, "%d ", arr[i]);
        }
        fprintf(infile, "\n");

        comparisons = 0;  
 
        quickSort(arr, 0, n - 1);
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
