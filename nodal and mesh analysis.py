import numpy as np
import time

#This function asks of the number of equations and also takes the equations from the user 
def get_user_input(system):
    print("The format for;\n1. Nodal analysis is y1V1 + y2V2+ ...+ynVn = I1\n2. Mesh analysis is x1I1 + x2I2+ ...+ xnIn = V1 where\ny = amittance and x = impedance")
    if system == 'A':
        n = int(input("Enter the number of equations for system A: "))
        print("Enter the coefficients and constants for each equation in system A separated by spaces:")
    else:
        n = int(input("Enter the number of equations for system B: "))#Takes the number of equations 
        print("Enter the coefficients and constants for each equation in system B separated by spaces:")#Allows the user to input the coefficients and constants separated by spaces from the equation the user has
    coefficients = []
    constants = []
    
    #To form the coefficient matrix and constant matrix from the equations provided
    for i in range(n):
        row = [float(x) for x in input(f"Equation {i+1}: ").split()]
        coefficients.append(row[:-1])
        constants.append(row[-1])
    return np.array(coefficients), np.array(constants)
    
#This function solves the system of equations using matrices
def solve_system(A, b):
    try:
        x = np.linalg.solve(A, b)#The function to solve the system of equations 
        return x
    except np.linalg.LinAlgError:
        print("The system of equations has no unique solution or no solution.")
        return None
#The main function to solve for nodal analysis  or mesh analysis and output the solution
def main():
    system = input("Enter 'A' to solve nodal analysis(system A), or 'B' to solve mesh analysis(system B): ").upper()
    if system not in ['A', 'B']:
        print("Invalid input. Please enter 'A' or 'B'.")
        return
    A, b = get_user_input(system)
    solution = solve_system(A, b)
    if solution is not None:
        print(f"The solution for system {system} is:")
        for i, x in enumerate(solution):
            if system== "A":
                print(f"The nodal voltages are: V{i+1} = {x:.2f}V")#Prints out the values of the nodal voltages 
            else:
                print(f"The mesh currents are: I{i+1} = {x:.2f}A")
# Measure the start time
start_time = time.time()

# Measure the end time
end_time = time.time()

if __name__ == "__main__":
    main()
    print("Time taken for Python implementation:", end_time - start_time, "seconds")#Prints out the time it takes the program to finish 
    
    
