#include <iostream>
#include <string>
#include <cmath>

using namespace std;

string toUpper(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i] - 'a' + 'A';
    }
    return str;
}

// invalid input handling  
int getIntInRange(const string& msg, int minVal, int maxVal) {
    int x;
    while (true) {
        cout << msg;
        cin >> x;

        if (cin.fail() || x < minVal || x > maxVal) {
            cout << "Invalid input. Enter a number between "
                 << minVal << " and " << maxVal << ".\n";
            cin.clear();
            while (cin.get() != '\n');
        } else {
            while (cin.get() != '\n');
            return x;
        }
    }
}

string getGateType() {
    string gate;
    while (true) {
        cout << "Gate type (AND, OR, XOR, XNOR, NOT, NAND, NOR): ";
        cin >> gate;

        gate = toUpper(gate);

        if (gate == "AND" || gate == "OR" || gate == "XOR" ||
            gate == "XNOR" || gate == "NOT" ||
            gate == "NAND" || gate == "NOR") {
            return gate;
        }
        cout << "Invalid gate type. Try again.\n";
    }
}

string getValidInput(int maxGateIndex) {
    string in;
    while (true) {
        cin >> in;
        in = toUpper(in);

        if (in == "A" || in == "B" || in == "C") {
            return in;
        }

        if (in.length() == 2 && in[0] == 'G') {
            char digit = in[1];
            if (digit >= '1' && digit <= '8') {
                int gateNum = digit - '0';
                if (gateNum <= maxGateIndex + 1) {
                    return in;
                }
            }
        }

        cout << "Invalid input. Enter A, B, C or G1-G" 
             << (maxGateIndex + 1) << ".\n";
        cout << "Note: If you need negation, use a NOT gate with a normal input.\n";
    }
}

string getSingleInput() {
    string in;
    while (true) {
        cout << "Enter only one input (A, B, or C): ";
        cin >> in;
        in = toUpper(in);
        
        if (in == "A" || in == "B" || in == "C") {
            return in;
        }
        cout << "Invalid input. Please enter A, B, or C.\n";
    }
}

// Global inputs
int A, B, C;

// Structure to store gate information
struct GateInfo {
    string type;
    int inputsCount;
    string rawInputs[3];
};

GateInfo originalGates[8];
int originalGatesCount = 0;

// function for evaluation
int evalExpr(string expr) {
    if (expr.length() == 0) return 0;

    // Remove outer parentheses if safe (UNCHANGED)
    while (expr.length() > 0 && expr[0] == '(' && expr[expr.length()-1] == ')') {
        int balance = 0;
        bool removable = true;
        for (int i = 0; i < expr.length(); i++) {
            if (expr[i] == '(') balance++;
            else if (expr[i] == ')') balance--;
            if (balance == 0 && i != expr.length() - 1) {
                removable = false;
                break;
            }
        }
        if (removable) {
            expr = expr.substr(1, expr.length() - 2);
        } else {
            break;
        }
    }

    // Handle single variables (UNCHANGED)
    if (expr == "A") return A;
    if (expr == "B") return B;
    if (expr == "C") return C;

    // === MODIFIED PART: Check for binary operators FIRST ===
    // Find operators at depth 0
    int balance = 0;
    int orPos = -1, xorPos = -1, andPos = -1;
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == '(') balance++;
        else if (expr[i] == ')') balance--;
        else if (balance == 0 && expr[i] != '~') {  // Note: skip '~' here
            if (expr[i] == '|' && orPos == -1) orPos = i;
            else if (expr[i] == '^' && xorPos == -1 && orPos == -1) xorPos = i;
            else if (expr[i] == '&' && andPos == -1 && orPos == -1 && xorPos == -1) andPos = i;
        }
    }

    // Evaluate operators based on priority
    if (orPos != -1)
        return evalExpr(expr.substr(0, orPos)) | evalExpr(expr.substr(orPos + 1));
    if (xorPos != -1)
        return evalExpr(expr.substr(0, xorPos)) ^ evalExpr(expr.substr(xorPos + 1));
    if (andPos != -1)
        return evalExpr(expr.substr(0, andPos)) & evalExpr(expr.substr(andPos + 1));

    // === MODIFIED PART: Handle NOT operations LAST (only reached if no binary operators) ===
    if (expr.length() > 0 && expr[0] == '~') {
        // Count consecutive NOTs
        int notCount = 0;
        while (notCount < expr.length() && expr[notCount] == '~') {
            notCount++;
        }
        
        // Get the expression after all the NOTs
        string subExpr = expr.substr(notCount);
        
        // If the subexpression is just a variable
        if (subExpr == "A" || subExpr == "B" || subExpr == "C") {
            int value = (subExpr == "A") ? A : (subExpr == "B") ? B : C;
            // Apply NOT the number of times
            for (int i = 0; i < notCount; i++) {
                value = !value;
            }
            return value;
        }
        
        // If the subexpression starts with parenthesis
        if (subExpr.length() > 0 && subExpr[0] == '(') {
            int balance = 0;
            bool complete = false;
            for (int i = 0; i < subExpr.length(); i++) {
                if (subExpr[i] == '(') balance++;
                else if (subExpr[i] == ')') balance--;
                
                if (balance == 0 && i == subExpr.length() - 1) {
                    complete = true;
                    break;
                }
            }
            
            if (complete) {
                int value = evalExpr(subExpr);
                for (int i = 0; i < notCount; i++) {
                    value = !value;
                }
                return value;
            }
        }
        
        int value = evalExpr(subExpr);
        for (int i = 0; i < notCount; i++) {
            value = !value;
        }
        return value;
    }

    return 0;
}

// function for bulding the circuit
string buildCircuitFromGates(GateInfo gates[], int gateCount) {
    string gateExpr[8];

    for (int i = 0; i < gateCount; i++) {
        string in[3];

        for (int j = 0; j < gates[i].inputsCount; j++) {
            string rawIn = gates[i].rawInputs[j];

            if (rawIn[0] == 'G') {
                int refGate = rawIn[1] - '1';
                in[j] = gateExpr[refGate];
            } else {
                in[j] = rawIn;
            }
        }

        if (gates[i].type == "NOT") {
            if (in[0].length() > 0 && in[0][0] == '~') {
                gateExpr[i] = in[0].substr(1);
            } else {
                gateExpr[i] = "~" + in[0];
            }
        } else if (gates[i].inputsCount == 1) {
            gateExpr[i] = "~" + in[0];
        } else {
            string expr = "(" + in[0];
            for (int j = 1; j < gates[i].inputsCount; j++) {
                if (gates[i].type == "AND" || gates[i].type == "NAND")
                    expr += "&" + in[j];
                else if (gates[i].type == "OR" || gates[i].type == "NOR")
                    expr += "|" + in[j];
                else if (gates[i].type == "XOR" || gates[i].type == "XNOR")
                    expr += "^" + in[j];
            }
            expr += ")";

            if (gates[i].type == "NAND" || gates[i].type == "NOR" || gates[i].type == "XNOR")
                gateExpr[i] = "~" + expr;
            else
                gateExpr[i] = expr;
        }
    }

    return gateExpr[gateCount - 1];
}

string buildCircuit(string title, int maxGates, int minGates, bool saveGates, int levels) {
    cout << "\n " << title << " \n";

    int gates = getIntInRange(
        "Number of gates (" + to_string(minGates) + "-" + to_string(maxGates) + "): ", minGates, maxGates);

    if (gates == 0) {
        string singleInput = getSingleInput();
        return singleInput;
    }

    string gateExpr[8];
    int gateLevel[8] = {0};

    if (saveGates) originalGatesCount = gates;

    for (int i = 0; i < gates; i++) {
        string gate;
        int inputsCount;

        cout << "\nGate " << i + 1 << endl;
        gate = getGateType();

        if (gate == "NOT") {
            inputsCount = 1;
            cout << "NOT gate has only one input\n";
        } else {
            inputsCount = getIntInRange(
                "Number of inputs for this gate (2 or 3): ", 2, 3);
        }

        string in[3];

        for (int j = 0; j < inputsCount; j++) {
            cout << "Input " << j + 1
                 << " (A, B, C or G1-G" << i << "): ";
            in[j] = getValidInput(i);

            if (saveGates) {
                originalGates[i].rawInputs[j] = in[j];
            }

            if (in[j][0] == 'G') {
                int refGate = in[j][1] - '1';
                in[j] = gateExpr[refGate];
                if (gateLevel[refGate] + 1 > gateLevel[i])
                    gateLevel[i] = gateLevel[refGate] + 1;
            }
        }

        if (gateLevel[i] + 1 > levels) {
            cout << "Circuit exceeds the maximum number of levels.\n";
            gateLevel[i] = 0;
            i--;
            continue;
        }

        if (saveGates) {
            originalGates[i].type = gate;
            originalGates[i].inputsCount = inputsCount;
        }

        if (gate == "NOT") {
            if (in[0].length() > 0 && in[0][0] == '~') {
                gateExpr[i] = in[0].substr(1);
            } else {
                gateExpr[i] = "~" + in[0];
            }
        } else {
            string expr = "(" + in[0];
            for (int j = 1; j < inputsCount; j++) {
                if (gate == "AND" || gate == "NAND")
                    expr += "&" + in[j];
                else if (gate == "OR" || gate == "NOR")
                    expr += "|" + in[j];
                else if (gate == "XOR" || gate == "XNOR")
                    expr += "^" + in[j];
            }
            expr += ")";

            if (gate == "NAND" || gate == "NOR" || gate == "XNOR")
                gateExpr[i] = "~" + expr;
            else
                gateExpr[i] = expr;
        }
    }

    return gateExpr[gates - 1];
}

// Function to display truth table
void truthTable(string expr, int inputs, int table[], string title) {
    int rows = (int)pow(2, inputs);
    cout << "\n" << title <<"truth table" << endl;

    if (inputs == 1)
        cout << "A | Output\n------\n";
    else if (inputs == 2)
        cout << "A B | Output\n---------\n";
    else
        cout << "A B C | Output\n-----------\n";

    for (int i = 0; i < rows; i++) {
        A = (inputs >= 1) ? ((i >> (inputs - 1)) & 1) : 0;
        B = (inputs >= 2) ? ((i >> (inputs - 2)) & 1) : 0;
        C = (inputs == 3) ? (i & 1) : 0;

        table[i] = evalExpr(expr);

        if (inputs == 1)
            cout << A << " | " << table[i] << endl;
        else if (inputs == 2)
            cout << A << " " << B << " | " << table[i] << endl;
        else
            cout << A << " " << B << " " << C << " | " << table[i] << endl;
    }
}

// Function to check if two expressions are equivalent
bool equivalent(int t1[], int t2[], int rows) {
    for (int i = 0; i < rows; i++)
        if (t1[i] != t2[i]) return false;
    return true;
}

// Function to find satisfiable inputs for an expression
void findSatisfiable(int table[], int inputs, int rows, string title) {
    cout << "\nThe Satisfiable Inputs for " << title << " \n";
    bool foundSat = false;

    for (int i = 0; i < rows; i++) {
        if (table[i] == 1) {
            foundSat = true;
            int a = (inputs >= 1) ? ((i >> (inputs - 1)) & 1) : 0;
            int b = (inputs >= 2) ? ((i >> (inputs - 2)) & 1) : 0;
            int c = (inputs == 3) ? (i & 1) : 0;

            cout << "(";
            if (inputs >= 1) cout << "A=" << a;
            if (inputs >= 2) cout << ", B=" << b;
            if (inputs == 3) cout << ", C=" << c;
            cout << ") ";
        }
    }

    if (!foundSat)
        cout << "UNSATISFIABLE";
    cout << "\n";
}

// Function to check if expression is tautology and satifiable
bool isTautology(int table[], int rows) {
    for (int i = 0; i < rows; i++) {
        if (table[i] == 0) return false;
    }
    return true;
}

bool isSatisfiable(int table[], int rows) {
    for (int i = 0; i < rows; i++) {
        if (table[i] == 1) return true;
    }
    return false;
}

void analyzeCircuit(int table[], int rows, string title) {
    cout << "\n ANALYSIS OF " << title << " \n";

    bool satisfiable = isSatisfiable(table, rows);
    bool tautology = isTautology(table, rows);
    bool contradiction = !satisfiable;

    if (tautology) {
        cout << "\nThe " << title << " is a TAUTOLOGY\n";
    } else if (contradiction) {
        cout << "\nThe " << title << " is UNSATISFIABLE\n";
    } else {
        cout << "\nThe " << title << " is SATISFIABLE\n";
    }
}

int main() {
    int inputs, levels;
    inputs = getIntInRange("Number of inputs (1-3): ", 1, 3);
    levels = getIntInRange("Number of levels in circuit (1-5): ", 1, 5);

    string originalExpr = buildCircuit("ORIGINAL CIRCUIT", 8, 1, true, levels);
    string simplifiedExpr = buildCircuit("SIMPLIFIED CIRCUIT", 8, 0, false, levels);

    int rows = (int)pow(2, inputs);
    int table1[8], table2[8];

    cout << "\nTRUTH TABLES\n";
    truthTable(originalExpr, inputs, table1, "ORIGINAL CIRCUIT");
    truthTable(simplifiedExpr, inputs, table2, "SIMPLIFIED CIRCUIT");

    cout << "\nEXPRESSIONS\n";
    cout << "Original Expression: " << originalExpr << endl;
    cout << "Simplified Expression: " << simplifiedExpr << endl;

    cout << "\nEQUIVALENCE CHECK\n";
    if (equivalent(table1, table2, rows))
        cout << "Result: THE CIRCUITS ARE EQUIVALENT\n";
    else
        cout << "Result: THE CIRCUITS ARE NOT EQUIVALENT\n";

    findSatisfiable(table1, inputs, rows, "ORIGINAL CIRCUIT");
    findSatisfiable(table2, inputs, rows, "SIMPLIFIED CIRCUIT");

    analyzeCircuit(table1, rows, "ORIGINAL CIRCUIT");
    analyzeCircuit(table2, rows, "SIMPLIFIED CIRCUIT");

    bool needsModification = false;
    bool isTaut = false;
    
    if (!isSatisfiable(table1, rows)) {
        needsModification = true;
        cout << "\nOriginal circuit is unsatisfiable. Please change your circuit.\n";
    } else if (isTautology(table1, rows)) {
        needsModification = true;
        isTaut = true;
        cout << "\nOriginal circuit is a tautology. Please change your circuit.\n";
    }

    int attempts = 0;
    while (needsModification && attempts < 2) {
        attempts++;
        
        if (attempts == 1) {
            cout << "\nPlease enter a new circuit (Attempt " << attempts << ") ---\n";
        } else {
            cout << "\nLast attempt to enter a new circuit (Attempt " << attempts << ") ---\n";
        }
        
        originalExpr = buildCircuit("NEW ORIGINAL CIRCUIT", 8, 1, true, levels);
        truthTable(originalExpr, inputs, table1, "NEW ORIGINAL CIRCUIT");
        analyzeCircuit(table1, rows, "NEW ORIGINAL CIRCUIT");
        
        if (!isSatisfiable(table1, rows)) {
            if (attempts < 2) {
                cout << "\nCircuit is still unsatisfiable.\n";
            }
        } else if (isTautology(table1, rows)) {
            if (attempts < 2) {
                cout << "\nCircuit is still a tautology.\n";
            }
        } else {
            needsModification = false;
            cout << "\n✓ Circuit is now valid. Continuing with analysis...\n";
            
            cout << "\n EQUIVALENCE CHECK FOR NEW CIRCUIT \n";
            if (equivalent(table1, table2, rows))
                cout << "Result: NEW CIRCUIT IS EQUIVALENT to simplified circuit\n";
            else
                cout << "Result: NEW CIRCUIT IS NOT EQUIVALENT to simplified circuit\n";
                
            findSatisfiable(table1, inputs, rows, "NEW ORIGINAL CIRCUIT");
        }
    }

    if (needsModification) {
        return 0;
    }
    
    return 0;
}