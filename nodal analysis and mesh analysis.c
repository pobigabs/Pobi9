#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_EQUATIONS 10

// Function to get user input for coefficients and constants
void getUserInput(int n, float coefficients[n][n + 1], char system) {
    printf("\nEquation format:\n");
    printf("1. Nodal Analysis: y1V1 + y2V2 + ... + ynVn = I1\n");
    printf("2. Mesh Analysis: x1I1 + x2I2 + ... + xnIn = V1\n");
    printf("where y = admittance, x = impedance.\n\n");

    printf("Enter the coefficients and constants for each equation (space-separated):\n");

    for (int i = 0; i < n; i++) {
        printf("Equation %d: ", i + 1);
        for (int j = 0; j <= n; j++) {
            scanf("%f", &coefficients[i][j]);
        }
    }
}

// Function to solve the system of equations using Gaussian elimination
void solveSystem(int n, float coefficients[n][n + 1], float solution[n]) {
    float augmentedMatrix[n][n + 1];

    // Copy coefficients into augmented matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            augmentedMatrix[i][j] = coefficients[i][j];
        }
    }

    // Start measuring time for solving the system
    clock_t start_time = clock();

    // Forward Elimination (Gaussian Elimination)
    for (int i = 0; i < n; i++) {
        float maxVal = augmentedMatrix[i][i];
        int maxRow = i;

        // Find the row with the maximum absolute value in column i
        for (int j = i + 1; j < n; j++) {
            if (fabs(augmentedMatrix[j][i]) > fabs(maxVal)) {
                maxVal = augmentedMatrix[j][i];
                maxRow = j;
            }
        }

        // If the pivot is zero, system has no unique solution
        if (maxVal == 0) {
            printf("\nError: The system of equations has no unique solution.\n");
            return;
        }

        // Swap rows to bring the largest absolute value to the pivot position
        for (int j = 0; j <= n; j++) {
            float temp = augmentedMatrix[maxRow][j];
            augmentedMatrix[maxRow][j] = augmentedMatrix[i][j];
            augmentedMatrix[i][j] = temp;
        }

        // Make all rows below the pivot zero in the current column
        for (int j = i + 1; j < n; j++) {
            float factor = augmentedMatrix[j][i] / augmentedMatrix[i][i];
            for (int k = i; k <= n; k++) {
                augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
            }
        }
    }

    // Back Substitution
    for (int i = n - 1; i >= 0; i--) {
        solution[i] = augmentedMatrix[i][n];
        for (int j = i + 1; j < n; j++) {
            solution[i] -= augmentedMatrix[i][j] * solution[j];
        }
        solution[i] /= augmentedMatrix[i][i];
    }

    // End measuring time
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\nExecution Time: %.6f seconds\n", execution_time);
}

// Main function
int main() {
    char system;
    int n;
    float coefficients[MAX_EQUATIONS][MAX_EQUATIONS + 1];
    float solution[MAX_EQUATIONS];

    // Get system type from user
    printf("Enter 'A' for nodal analysis or 'B' for mesh analysis: ");
    scanf(" %c", &system);

    if (system != 'A' && system != 'B') {
        printf("Invalid input. Please enter 'A' or 'B'.\n");
        return 1;
    }

    printf("\nEnter the number of equations: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_EQUATIONS) {
        printf("Invalid number of equations. Please enter a positive integer (≤ %d).\n", MAX_EQUATIONS);
        return 1;
    }

    // Get coefficients and constants
    getUserInput(n, coefficients, system);

    // Solve the system
    solveSystem(n, coefficients, solution);

    // Print solution
    printf("\nSolution for system %c:\n", system);
    for (int i = 0; i < n; i++) {
        if (system == 'A') {
            printf("V%d = %.4f V\n", i + 1, solution[i]);
        } else {
            printf("I%d = %.4f A\n", i + 1, solution[i]);
        }
    }

    return 0;
}
