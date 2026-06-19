# Digital Logic Circuit Simplification and Analysis

PHM311s – Discrete Math

---

## Project Description

This project is a C++ application for building, analyzing, and comparing digital logic circuits.

The program allows the user to construct an original logic circuit and its simplified form using logic gates. It then generates truth tables, checks whether the two circuits are equivalent, finds satisfiable input combinations, and determines whether a circuit is satisfiable, unsatisfiable, or a tautology.

The application supports multi-level circuits and validates all user inputs to ensure correct circuit construction.

---

## Supported Logic Gates

* AND
* OR
* NOT
* NAND
* NOR
* XOR
* XNOR

---

## Features

### Circuit Construction

* Build digital circuits interactively
* Support up to 8 gates
* Support up to 5 logic levels
* Allow gates to use outputs of previous gates as inputs

### Truth Table Generation

* Generate complete truth tables
* Support circuits with 1 to 3 input variables
* Display outputs for every possible input combination

### Expression Generation

* Automatically convert the circuit into a Boolean expression
* Generate expressions for both original and simplified circuits

### Equivalence Checking

* Compare truth tables of two circuits
* Determine whether the original and simplified circuits are logically equivalent

### Satisfiability Analysis

* Find all input combinations that make the circuit output TRUE
* Detect:

  * Satisfiable circuits
  * Unsatisfiable circuits (contradictions)
  * Tautologies

### Circuit Validation

* Reject invalid gate types
* Reject invalid input references
* Enforce gate input limits
* Enforce circuit level constraints

---

## Program Workflow

1. Enter the number of inputs.
2. Enter the maximum circuit depth.
3. Build the original circuit.
4. Build the simplified circuit.
5. Generate truth tables.
6. Generate Boolean expressions.
7. Check circuit equivalence.
8. Find satisfiable input combinations.
9. Analyze whether each circuit is:

   * Satisfiable
   * Unsatisfiable
   * A Tautology
10. If the original circuit is invalid (unsatisfiable or tautological), the user may modify and reanalyze it.

---

## Example Gates

Example circuit:

```text
Gate 1: AND(A, B)
Gate 2: OR(G1, C)
```

Generated expression:

```text
((A&B)|C)
```

---

## Input Flow

The program guides the user through building two logic circuits: the original circuit and its simplified form.

### Step 1: Specify Circuit Parameters

The user enters:

```text
Number of inputs (1-3):
Number of levels in circuit (1-5):
```

### Step 2: Build the Original Circuit

For each gate, the user enters:

```text
Gate type (AND, OR, XOR, XNOR, NOT, NAND, NOR):
Number of inputs:
Input 1:
Input 2:
Input 3:
```

Inputs can be:

* Input variables: `A`, `B`, `C`
* Outputs of previous gates: `G1`, `G2`, `G3`, ...

Example:

```text
Gate 1
Type: AND
Input 1: A
Input 2: B

Gate 2
Type: OR
Input 1: G1
Input 2: C
```

### Step 3: Build the Simplified Circuit

The same process is repeated for the simplified version of the circuit.

### Step 4: Automatic Analysis

The program then automatically:

1. Builds Boolean expressions.
2. Generates truth tables.
3. Compares both circuits.
4. Checks logical equivalence.
5. Finds satisfiable input combinations.
6. Determines whether each circuit is:

   * Satisfiable
   * Unsatisfiable
   * A Tautology

---
## Error Handling

The program validates user input and prevents invalid circuit construction.

Checks include:

* Invalid gate names
* Invalid circuit references
* Invalid number of gate inputs
* Invalid number of circuit inputs
* Exceeding maximum gate count
* Exceeding maximum circuit levels
* Non-numeric input where numbers are required

The program repeatedly prompts the user until valid input is entered.

---

## Authors

Abdelrahman Khaled Aboellel

---

## Technologies Used

* C++
* Boolean Algebra
* Digital Logic Design
* Data Structures and Algorithms
