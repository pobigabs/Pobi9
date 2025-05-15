#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <time.h>
#include <sys/time.h>

#define MAX_TERM_LEN 50
#define MAX_EQ_LEN 200

// Structure to store matrix and constants
typedef struct {
    double **coefficients;
    double *constants;
    int n;
} LinearSystem;

// Function to create a linear system
LinearSystem* create_system(int n) {
    LinearSystem *sys = malloc(sizeof(LinearSystem));
    sys->n = n;
    sys->coefficients = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        sys->coefficients[i] = calloc(n, sizeof(double));
    }
    sys->constants = calloc(n, sizeof(double));
    return sys;
}

// Function to free memory
void free_system(LinearSystem *sys) {
    for (int i = 0; i < sys->n; i++) {
        free(sys->coefficients[i]);
    }
    free(sys->coefficients);
    free(sys->constants);
    free(sys);
}

// Regex-based term parser
int parse_term(const char *term, int system_type, int n, int *col, double *coeff) {
    regex_t regex;
    regmatch_t matches[6];
    char pattern[] = "^([+-]?)((?:\\d+\\.?\\d*|\\.\\d+)(?:[eE][+-]?\\d+)?)?([VI])(\\d+)$";
    
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        fprintf(stderr, "Regex compilation failed\n");
        return -1;
    }
    
    if (regexec(&regex, term, 6, matches, 0) != 0) {
        regfree(&regex);
        return -1;
    }
    
    // Extract components
    char sign = (matches[1].rm_so != -1) ? term[matches[1].rm_so] : '+';
    char *coeff_str = strndup(term + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
    char var_type = term[matches[3].rm_so];
    char *idx_str = strndup(term + matches[4].rm_so, matches[4].rm_eo - matches[4].rm_so);
    
    // Validate variable type
    if ((system_type == 'A' && var_type != 'V') || (system_type == 'B' && var_type != 'I')) {
        regfree(&regex);
        free(coeff_str);
        free(idx_str);
        return -1;
    }
    
    // Parse coefficient
    *coeff = (coeff_str[0] != '\0') ? atof(coeff_str) : 1.0;
    *coeff *= (sign == '-') ? -1.0 : 1.0;
    
    // Parse index
    *col = atoi(idx_str) - 1;
    if (*col < 0 || *col >= n) {
        regfree(&regex);
        free(coeff_str);
        free(idx_str);
        return -1;
    }
    
    regfree(&regex);
    free(coeff_str);
    free(idx_str);
    return 0;
}

// Gaussian elimination with partial pivoting
int gauss_eliminate(LinearSystem *sys) {
    int n = sys->n;
    for (int i = 0; i < n; i++) {
        // Partial pivoting
        int max_row = i;
        for (int k = i+1; k < n; k++) {
            if (abs(sys->coefficients[k][i]) > abs(sys->coefficients[max_row][i])) {
                max_row = k;
            }
        }
        
        // Swap rows
        double *temp = sys->coefficients[i];
        sys->coefficients[i] = sys->coefficients[max_row];
        sys->coefficients[max_row] = temp;
        double t = sys->constants[i];
        sys->constants[i] = sys->constants[max_row];
        sys->constants[max_row] = t;
        
        // Singular matrix check
        if (sys->coefficients[i][i] == 0) {
            return -1;
        }
        
        // Elimination
        for (int k = i+1; k < n; k++) {
            double factor = sys->coefficients[k][i] / sys->coefficients[i][i];
            for (int j = i; j < n; j++) {
                sys->coefficients[k][j] -= factor * sys->coefficients[i][j];
            }
            sys->constants[k] -= factor * sys->constants[i];
        }
    }
    
    // Back substitution
    for (int i = n-1; i >= 0; i--) {
        sys->constants[i] /= sys->coefficients[i][i];
        for (int k = i-1; k >= 0; k--) {
            sys->constants[k] -= sys->coefficients[k][i] * sys->constants[i];
        }
    }
    return 0;
}

int main() {
    char system_type;
    int n;
    
    printf("Enter 'A' for nodal analysis or 'B' for mesh analysis: ");
    scanf(" %c", &system_type);
    system_type = toupper(system_type);
    
    printf("Enter number of equations: ");
    scanf("%d", &n);
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    LinearSystem *sys = create_system(n);
    
    for (int i = 0; i < n; i++) {
        char equation[MAX_EQ_LEN];
        printf("Equation %d: ", i+1);
        scanf(" %[^\n]", equation);
        
        char *rhs = strchr(equation, '=');
        if (!rhs) {
            fprintf(stderr, "Invalid equation: missing '='\n");
            return 1;
        }
        *rhs = '\0';
        rhs++;
        
        sys->constants[i] = atof(rhs);
        
        char *term = strtok(equation, " +-");
        while (term != NULL) {
            double coeff;
            int col;
            if (parse_term(term, system_type, n, &col, &coeff) == 0) {
                sys->coefficients[i][col] += coeff;
            }
            term = strtok(NULL, " +-");
        }
    }
    
    gettimeofday(&end, NULL);
    long input_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    
    struct timeval solve_start, solve_end;
    gettimeofday(&solve_start, NULL);
    
    if (gauss_eliminate(sys) != 0) {
        fprintf(stderr, "Singular matrix detected\n");
        return 1;
    }
    
    gettimeofday(&solve_end, NULL);
    long solve_time = (solve_end.tv_sec - solve_start.tv_sec) * 1000000 + (solve_end.tv_usec - solve_start.tv_usec);
    
    printf("\nSolutions:\n");
    for (int i = 0; i < n; i++) {
        printf("%c%d = %.4f %s\n", (system_type == 'A') ? 'V' : 'I', i+1, sys->constants[i], (system_type == 'A') ? "V" : "A");
    }
    
    printf("\nInput processing time: %.6f sec\n", input_time / 1e6);
    printf("Computation time: %.6f sec\n", solve_time / 1e6);
    printf("Total time: %.6f sec\n", (input_time + solve_time) / 1e6);
    
    free_system(sys);
    return 0;
}
