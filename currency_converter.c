#include <stdio.h>

int main() {
    float amount, result;
    int from, to;
    
    // Exchange rates relative to 1 USD (Base)
    // 1 USD = 1.0 USD
    // 1 USD = 0.95 EUR 
    // 1 USD = 90.61 INR
    float rates[] = {1.0, 0.95, 90.61};

    printf("1: USD\n2: EUR\n3: INR\n");

    printf("Enter source currency (1-3): ");
    scanf("%d", &from);

    printf("Enter target currency (1-3): ");
    scanf("%d", &to);

    printf("Enter amount: ");
    scanf("%f", &amount);

    // Convert: (Amount / Source_Rate) * Target_Rate
    result = (amount / rates[from - 1]) * rates[to - 1];

    printf("Result: %.2f\n", result);

    return 0;
}