#include <stdio.h>

void add(int x, int y); // Function prototype

void main() {
    long bs, da, hra, gs;

    // Take basic salary as input
    printf("Enter Basic Salary: ");
    scanf("%ld", &bs);

    // Calculate allowances
    da = bs * 0.40; // Assuming DA is 40% of basic salary
    hra = bs * 0.20; // Assuming HRA is 20% of basic salary
    gs = bs + da + hra; // Gross Salary = Basic + DA + HRA

    // Display salary slip
    printf("Basic Salary: %ld\n", bs);
    printf("DA: %ld\n", da);
    printf("HRA: %ld\n", hra);
    printf("Gross Salary: %ld\n", gs);
}

void add(int x, int y) {
    printf("Addition of %d and %d is %d\n", x, y, x + y);
}
