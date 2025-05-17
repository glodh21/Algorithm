#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) { return (a > b) ? a : b; }

int knapsack_01(int W, int wt[], int val[], int n) {
    int dp[W+1];
    for (int w = 0; w <= W; w++)
        dp[w] = 0;

    for (int i = 0; i < n; i++) {
        for (int w = W; w >= wt[i]; w--) {
            dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);
        }
    }
    return dp[W];
}

int main() {
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    int W = 50;
    int n = sizeof(val)/sizeof(val[0]);
    
    printf("0/1 Knapsack Maximum Value: %d\n", knapsack_01(W, wt, val, n));
    return 0;
}