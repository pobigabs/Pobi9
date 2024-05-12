#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_EQUATIONS 10
//Function to allow the user to input the coeffients and conatant
void getUserInput(int n, float coefficients[n][n+1], char system) {
    printf("The format for:\n1. Nodal analysis is y1V1 + y2V2+ ...+ynVn = I1\n2. Mesh analysis is x1I1 + x2I2+ ...+ xnIn = V1 where\ny = admittance and x = impedance\n");

    if (system == 'A') {
        printf("Enter the coefficients and constants for each equation in system A separating them with spaces:\n");
    } else {
        printf("Enter the coefficients and constants for each equation in system B separating them with spaces:\n");
    }

    for (int i = 0; i < n; i++) {
        printf("Equation %d: ", i+1);
        for (int j = 0; j <= n; j++) {
            scanf("%f", &coefficients[i][j]);
        }
    }
}
//Function to solve the system of equation using the augmented matrix format
void solveSystem(int n, float coefficients[n][n+1], float solution[n]) {
    float augmentedMatrix[n][n+1];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            augmentedMatrix[i][j] = coefficients[i][j];
        }
    }
    // Measure the start time
    clock_t start_time = clock();


    for (int i = 0; i < n; i++) {
        float maxVal = augmentedMatrix[i][i];
        int maxRow = i;

        for (int j = i+1; j < n; j++) {
            if (fabs(augmentedMatrix[j][i]) > fabs(maxVal)) {
                maxVal = augmentedMatrix[j][i];
                maxRow = j;
            }
        }

        if (maxVal == 0) {
            printf("The system of equations has no unique solution or no solution.\n");
            return;
        }

        for (int j = 0; j <= n; j++) {
            float temp = augmentedMatrix[maxRow][j];
            augmentedMatrix[maxRow][j] = augmentedMatrix[i][j];
            augmentedMatrix[i][j] = temp;
        }

        for (int j = i+1; j < n; j++) {
            float factor = augmentedMatrix[j][i] / augmentedMatrix[i][i];
            for (int k = 0; k <= n; k++) {
                augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
            }
        }
    }

    for (int i = n-1; i >= 0; i--) {
        solution[i] = augmentedMatrix[i][n];
        for (int j = i+1; j < n; j++) {
            solution[i] -= augmentedMatrix[i][j] * solution[j];
        }
        solution[i] /= augmentedMatrix[i][i];
    }
    // Measure the end time
    clock_t end_time = clock();
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken for C implementation: %f seconds\n", cpu_time_used);
}

//main function to know whether the user wants to solve for nodal voltages or mesh currnets 
int main() {
    char system;
    int n;
    float coefficients[MAX_EQUATIONS][MAX_EQUATIONS+1];
    float solution[MAX_EQUATIONS];

    printf("Enter 'A' to solve nodal analysis(system A), or 'B' to solve mesh analysis(system B): ");
    scanf(" %c", &system);

    if (system != 'A' && system != 'B') {
        printf("Invalid input. Please enter 'A' or 'B'.\n");
        return 0;
    }

    if (system == 'A') {
        printf("Enter the number of equations for system A: ");
    } else {
        printf("Enter the number of equations for system B: ");
    }
    scanf("%d", &n);

    getUserInput(n, coefficients, system);
    solveSystem(n, coefficients, solution);
    
    //prints the solution from the operations in the row operations 
    printf("The solution for system %c is:\n", system);
    for (int i = 0; i < n; i++) {
        if (system == 'A') {
            printf("V%d = %.2f V\n", i+1, solution[i]);
        } else {
            printf("I%d = %.2f A\n", i+1, solution[i]);
        }
    }

    return 0;
}
