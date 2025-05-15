import numpy as np
import re
import time

def get_user_input(system):
    print("\nEquation format:")
    if system == 'A':
        print("Example: 3V1 - 2V2 + 5V3 = 0.5 (Nodal Analysis)")
    else:
        print("Example: 4I1 + 2I2 - 3I3 = 10 (Mesh Analysis)")
    print("Ensure variables are correctly formatted and indices are sequential starting from 1.\n")

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

    print(f"\nEnter the {n} equations for system {system}:")
    for i in range(n):
        while True:
            equation = input(f"Equation {i+1}: ").strip()
            if '=' not in equation:
                print("Invalid equation: missing '='. Please re-enter.")
                continue
            lhs_rhs = equation.split('=', 1)
            lhs = lhs_rhs[0].strip()
            rhs = lhs_rhs[1].strip()

            # Process RHS
            try:
                constants[i] = float(rhs)
            except ValueError:
                print(f"Invalid RHS '{rhs}'. Must be a numeric value.")
                continue

            # Process LHS
            lhs_clean = lhs.replace(" ", "")  # Remove all whitespace
            terms = re.findall(r'([+-]?[^+-]+)', lhs_clean)
            if not terms:
                print("Invalid LHS: no terms found.")
                continue

            current_row = np.zeros(n)
            valid_equation = True

            for term in terms:
                # Use regex to parse each term
                match = re.fullmatch(
                    r'^([+-]?)((?:[0-9]*\.?[0-9]+|[0-9]+\.?[0-9]*)(?:[eE][+-]?[0-9]+)?)?([VI])([0-9]+)$',
                    term
                )
                if not match:
                    print(f"Invalid term '{term}'. Please check format.")
                    valid_equation = False
                    break

                sign_part = match.group(1)
                coeff_part = match.group(2)
                var_type = match.group(3)
                var_index_str = match.group(4)

                # Check variable type
                expected_var = 'V' if system == 'A' else 'I'
                if var_type != expected_var:
                    print(f"Invalid variable type '{var_type}' in system {system}. Expected {expected_var}.")
                    valid_equation = False
                    break

                # Check variable index
                try:
                    var_index = int(var_index_str)
                except ValueError:
                    print(f"Invalid variable index '{var_index_str}' in term '{term}'.")
                    valid_equation = False
                    break
                if var_index < 1 or var_index > n:
                    print(f"Variable index {var_index} is out of range (1 to {n}).")
                    valid_equation = False
                    break

                # Calculate column (0-based)
                col = var_index - 1

                # Parse coefficient
                if not coeff_part:
                    # No coefficient, check sign
                    if sign_part == '-':
                        coeff = -1.0
                    else:
                        coeff = 1.0
                else:
                    coeff_str = sign_part + coeff_part
                    try:
                        coeff = float(coeff_str)
                    except ValueError:
                        print(f"Invalid coefficient '{coeff_str}' in term '{term}'.")
                        valid_equation = False
                        break

                current_row[col] += coeff

            if not valid_equation:
                continue

            # Check if row is all zeros (invalid equation)
            if np.all(current_row == 0):
                print("Equation has no coefficients. Please re-enter.")
                continue

            coefficients[i] = current_row
            break  # Proceed to next equation

    return coefficients, constants

def solve_system(A, b):
    try:
        solution = np.linalg.solve(A, b)
        return solution
    except np.linalg.LinAlgError:
        print("Error: The system of equations has no unique solution or is singular.")
        return None

def main():
    while True:
        system = input("\nEnter 'A' for nodal analysis or 'B' for mesh analysis: ").strip().upper()
        if system in ['A', 'B']:
            break
        print("Invalid input. Please enter 'A' or 'B'.")

    start_time = time.time()

    A, b = get_user_input(system)

    process_start = time.time()

    solution = solve_system(A, b)

    end_time = time.time()

    if solution is not None:
        print(f"\nSolution for system {system}:")
        for i, x in enumerate(solution):
            unit = "V" if system == 'A' else "A"
            print(f"{'V' if system == 'A' else 'I'}{i+1} = {x:.4f} {unit}")

        print(f"\nTime taken for input processing: {process_start - start_time:.6f} seconds")
        print(f"Time taken for computation: {end_time - process_start:.6f} seconds")
        print(f"Total execution time: {end_time - start_time:.6f} seconds")

if __name__ == "__main__":
    main()
