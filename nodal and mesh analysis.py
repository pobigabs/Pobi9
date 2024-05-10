import numpy as np

def get_user_input(system):
    print("The format for;\n1. Nodal analysis is y1V1 + y2V2+ ...+ynVn = I1\n2. Mesh analysis is x1I1 + x2I2+ ...+ xnIn = V1 where\ny = amittance and x = impedance")
    if system == 'A':
        n = int(input("Enter the number of equations for system A: "))
        print("Enter the coefficients and constants for each equation in system A:")
    else:
        n = int(input("Enter the number of equations for system B: "))
        print("Enter the coefficients and constants for each equation in system B:")
    coefficients = []
    constants = []
    for i in range(n):
        row = [float(x) for x in input(f"Equation {i+1}: ").split()]
        coefficients.append(row[:-1])
        constants.append(row[-1])
    return np.array(coefficients), np.array(constants)

def solve_system(A, b):
    try:
        x = np.linalg.solve(A, b)
        return x
    except np.linalg.LinAlgError:
        print("The system of equations has no unique solution or no solution.")
        return None

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
                print(f"V{i+1} = {x:.2f}V")
            else:
                print(f"I{i+1} = {x:.2f}A")

if __name__ == "__main__":
    main()