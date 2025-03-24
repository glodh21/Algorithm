#include <stdio.h>

 int moveCountLinear = 0;
 int moveCountLog = 0;

long long int power_linear(long long int X, int n) {
    if (n == 0)
        return 1;
    else if (n > 0) {
        moveCountLinear++;
        return X * power_linear(X, n - 1);
    } else {
        moveCountLinear++;
        return 1 / (X * power_linear(X, -n - 1));
    }
}

long long int power_log(long long int X, int n) {
    if (n < 0)
        return 1 / power_log(X, -n);
    if (n == 0)
        return 1;
    long long int temp = power_log(X, n / 2);
    if (n % 2 == 0) {
        moveCountLog++;
        return temp * temp;
    } else {
        moveCountLog++;
        return X * temp * temp;
    }
}

int main() {
    long long int X;
    int n;
    printf("Enter base X: ");
    scanf("%lld", &X);
    printf("Enter exponent n: ");
    scanf("%d", &n);
    long long int resultLinear = power_linear(X, n);
    long long int resultLog = power_log(X, n);
    printf("\nUsing Linear Recursion:\n");
    printf("%lld raised to the power %d is %lld\n", X, n, resultLinear);
    printf("Number of multiplications (linear): %lld\n", moveCountLinear);
    printf("\nUsing Logarithmic Recursion:\n");
    printf("%lld raised to the power %d is %lld\n", X, n, resultLog);
    printf("Number of multiplications (logarithmic): %lld\n", moveCountLog);
    return 0;
}
 