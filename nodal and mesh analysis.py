import numpy as np
import time

# Function to prompt for the number of equations and take the equations from the user
def get_user_input(system):
    print("The format for:\n1. Nodal analysis is y1V1 + y2V2 + ... + ynVn = I1\n2. Mesh analysis is x1I1 + x2I2 + ... + xnIn = V1 where\ny = admittance and x = impedance")
    
    while True:
        try:
            n = int(input(f"Enter the number of equations for system {system}: "))
            if n <= 0:
                raise ValueError("The number of equations must be a positive integer.")
            break
        except ValueError as e:
            print(f"Invalid input: {e}. Please enter a positive integer.")

    print(f"Enter the coefficients and constants for each equation in system {system}:")
    coefficients = []
    constants = []
    for i in range(n):
        while True:
            try:
                row = [float(x) for x in input(f"Equation {i+1}: ").split()]
                if len(row) != n + 1:
                    raise ValueError(f"Each equation must have {n} coefficients followed by a constant.")
                coefficients.append(row[:-1])
                constants.append(row[-1])
                break
            except ValueError as e:
                print(f"Invalid input: {e}. Please re-enter the equation.")
    
    return np.array(coefficients), np.array(constants)

# Function to solve a system of equations using matrices
def solve_system(A, b):
    try:
        x = np.linalg.solve(A, b)
        return x
    except np.linalg.LinAlgError:
        print("The system of equations has no unique solution or no solution.")
        return None

def main():
    while True:
        system = input("Enter 'A' to solve nodal analysis(system A), or 'B' to solve mesh analysis(system B): ").upper()
        if system in ['A', 'B']:
            break
        print("Invalid input. Please enter 'A' or 'B'.")
    
    # Measure the start time before user input
    start_time = time.time()
    
    A, b = get_user_input(system)
    
    solution = solve_system(A, b)
    
    # Measure the end time after solving the system
    end_time = time.time()

    for i in range(1000000):
    pass 
    
    if solution is not None:
        print(f"The solution for system {system} is:")
        for i, x in enumerate(solution):
            if system == "A":
                print(f"V{i+1} = {x:.2f} V")
            else:
                print(f"I{i+1} = {x:.2f} A")
        print("Time taken for Python implementation:", end_time - start_time, "seconds")#Prints out the time it takes the program to finish 


if __name__ == "__main__":
    main()

    
