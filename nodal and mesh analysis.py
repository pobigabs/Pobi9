import numpy as np
import time

# Function to prompt for the number of equations and take user input
def get_user_input(system):
    print("\nEquation format:")
    print("1. Nodal analysis: y1V1 + y2V2 + ... + ynVn = I1")
    print("2. Mesh analysis: x1I1 + x2I2 + ... + xnIn = V1")
    print("where y = admittance and x = impedance.\n")

    while True:
        try:
            n = int(input(f"Enter the number of equations for system {system}: "))
            if n <= 0:
                raise ValueError("The number of equations must be a positive integer.")
            break
        except ValueError as e:
            print(f"Invalid input: {e}. Please enter a positive integer.")

    coefficients = np.zeros((n, n))
    constants = np.zeros(n)

    print(f"\nEnter the {n} coefficients followed by the constant term for each equation in system {system}:")
    for i in range(n):
        while True:
            try:
                row = list(map(float, input(f"Equation {i+1}: ").split()))
                if len(row) != n + 1:
                    raise ValueError(f"Each equation must have {n} coefficients followed by a constant.")
                coefficients[i] = row[:-1]
                constants[i] = row[-1]
                break
            except ValueError as e:
                print(f"Invalid input: {e}. Please re-enter the equation.")

    return coefficients, constants

# Function to solve the system of equations
def solve_system(A, b):
    try:
        solution = np.linalg.solve(A, b)
        return solution
    except np.linalg.LinAlgError:
        print("Error: The system of equations has no unique solution or is singular.")
        return None

# Main function
def main():
    while True:
        system = input("\nEnter 'A' for nodal analysis or 'B' for mesh analysis: ").strip().upper()
        if system in ['A', 'B']:
            break
        print("Invalid input. Please enter 'A' or 'B'.")

    # Measure start time (before user input)
    start_time = time.time()

    A, b = get_user_input(system)

    # Measure processing time (after input, before solving)
    process_start = time.time()

    solution = solve_system(A, b)

    # Measure end time
    end_time = time.time()

    if solution is not None:
        print(f"\nSolution for system {system}:")
        for i, x in enumerate(solution):
            if system == "A":
                print(f"V{i+1} = {x:.4f} V")  # Display with 4 decimal places
            else:
                print(f"I{i+1} = {x:.4f} A")

        print(f"\nTime taken for input processing: {process_start - start_time:.6f} seconds")
        print(f"Time taken for computation: {end_time - process_start:.6f} seconds")
        print(f"Total execution time: {end_time - start_time:.6f} seconds")

if __name__ == "__main__":
    main()
