#include <stdio.h>

// Global variable to count comparisons
int comparisons = 0;

void findMinAndMax(int arr[], int n, int *min, int *max) {
    int i;
    comparisons = 0;  // Reset global counter

    // If the array has an odd number of elements, initialize min and max to first element.
    // Start processing from index 1.
    if (n % 2 != 0) {
        *min = *max = arr[0];
        i = 1;
    } else {
        // For an even number of elements, compare the first two elements.
        if (arr[0] < arr[1]) {
            *min = arr[0];
            *max = arr[1];
        } else {
            *min = arr[1];
            *max = arr[0];
        }
        comparisons++;  // One comparison done above
        // i = 2;
    }

    // Process elements in pairs
    for (i=2; i < n; i += 2) {
        int local_min, local_max;
        
        // First comparison within the pair
        if (arr[i] < arr[i + 1]) {
            local_min = arr[i];
            local_max = arr[i + 1];
        } else {
            local_min = arr[i + 1];
            local_max = arr[i];
        }
        comparisons++;  // For comparing the pair

        // Compare the local minimum with the global minimum
        if (local_min < *min) {
            *min = local_min;
        }
        comparisons++;  // Comparison for min update
        
        // Compare the local maximum with the global maximum
        if (local_max > *max) {
            *max = local_max;
        }
        comparisons++;  // Comparison for max update
    }
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }
    
    int min, max;
    findMinAndMax(arr, n, &min, &max);
    
    printf("Minimum element is %d\n", min);
    printf("Maximum element is %d\n", max);
    printf("Number of comparisons: %d\n", comparisons);
    
    return 0;
}
