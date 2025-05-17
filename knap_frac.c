#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int weight;
} Item;

int compare(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    double ratio1 = (double)item1->value / item1->weight;
    double ratio2 = (double)item2->value / item2->weight;
    return (ratio2 > ratio1) ? 1 : -1;
}

double fractional_knapsack(int W, Item items[], int n) {
    qsort(items, n, sizeof(Item), compare);
    
    double total = 0.0;
    int current_weight = 0;
    
    for (int i = 0; i < n; i++) {
        if (current_weight + items[i].weight <= W) {
            current_weight += items[i].weight;
            total += items[i].value;
        } else {
            int remaining = W - current_weight;
            total += items[i].value * ((double)remaining / items[i].weight);
            break;
        }
    }
    return total;
}

int main() {
    Item items[] = {{60, 10}, {100, 20}, {120, 30}};
    int W = 50;
    int n = sizeof(items)/sizeof(items[0]);
    
    printf("Fractional Knapsack Maximum Value: %.2f\n", fractional_knapsack(W, items, n));
    return 0;
}