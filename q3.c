#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparisonCount = 0;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int randomPartition(int arr[], int low, int high) {
    srand(time(NULL)); 
    int randomIndex = low + rand() % (high - low + 1);
    swap(&arr[randomIndex], &arr[low]);  

    int pivot = arr[low];  
    int i = low - 1, j = high + 1;
    
    while (1) {
        i++;
        comparisonCount++;
        while (arr[i] < pivot) {
            i++;
            comparisonCount++;
        }
        j--;
        comparisonCount++;
        while (arr[j] > pivot) {
            j--;
            comparisonCount++;
        }
        if (i >= j) return j;
        swap(&arr[i], &arr[j]);
    }
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int p = randomPartition(arr, low, high); 
        quickSort(arr, low, p);
        quickSort(arr, p + 1, high);
    }
}
void printArray(FILE *file, int arr[], int size) {
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", arr[i]);
    }
}

void generatePermutations(int arr[], int start, int size, FILE *infile) {
    if (start == size) {
        printArray(infile, arr, size);
        fprintf(infile, "\n");
        return;
    }
    for (int i = start; i < size; i++) {
        swap(&arr[start], &arr[i]);
        generatePermutations(arr, start + 1, size, infile);
        swap(&arr[start], &arr[i]);
    }
}

void processPermutations(const char *inputFile, const char *outputFile, int size) {
    FILE *infile = fopen(inputFile, "r");
    FILE *outfile = fopen(outputFile, "w");
    if (!infile || !outfile) {
        printf("Error opening files!\n");
        return;
    }
    int arr[size];
    long long totalComparisons = 0;
    int count = 0;
    while (1) {
        for (int i = 0; i < size; i++) {
            if (fscanf(infile, "%d", &arr[i]) == EOF) {
                fclose(infile);
                fclose(outfile);
                double avgComparisons = (double)totalComparisons / count;
                printf("Average Comparisons: %.2lf\n", avgComparisons);
                return;
            }
        }
        comparisonCount = 0;
        quickSort(arr, 0, size - 1);
        printArray(outfile, arr, size);
        fprintf(outfile, " | Comparisons: %lld\n", comparisonCount);
        totalComparisons += comparisonCount;
        count++;
    }
}

int main() {
    int n;
    printf("Enter number of elements (n): ");
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) arr[i] = i;
    
    FILE *infile = fopen("permutations_input.txt", "w");
    if (!infile) {
        printf("Error creating input file!\n");
        return 1;
    }
    generatePermutations(arr, 0, n, infile);
    fclose(infile);
    printf("Permutations generated successfully.\n");
    
    processPermutations("permutations_input.txt", "sorted_permutations_output.txt", n);
    printf("Sorting completed. Check 'sorted_permutations_output.txt' for results.\n");
    return 0;
}
