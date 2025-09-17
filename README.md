# SFort95 Interpreter

A lightweight interpreter for a Pascal-like programming language, implemented in C++17.
The project includes a custom lexer, parser, and runtime engine to evaluate expressions, handle variables, and execute simple control structures.

---

## ✨ Features
- **Lexical Analyzer (`lex.cpp`)**  
  Tokenizes SFort95 source code into identifiers, keywords, literals, and operators.  
- **Recursive-Descent Parser (`parserInterp.cpp`)**  
  Implements grammar rules for declarations, assignment statements, print statements, and IF/ELSE blocks.  
- **Interpreter**  
  Evaluates expressions (integer, real, character, string) with type checking and error handling.  
- **Symbol Table Management**  
  Tracks variables, types, initialization state, and runtime values.  
- **Error Handling**  
  Detects undefined variables, type mismatches, malformed tokens, and provides line-specific error messages.  
- **Driver Program (`prog3.cpp`)**  
  Manages input files, runs the interpreter, and outputs results (successful execution vs. error reports).  

---

## 📂 Project Structure
SFort95-Interpreter/
.
├── src/                 # Source files
│   ├── prog3.cpp        # Main entry point
│   ├── lex.cpp          # Lexer implementation
│   ├── parserInterp.cpp # Parser and interpreter
│   ├── val.cpp          # Value class (operator overloads, data handling)
│
├── include/             # Header files
│   ├── lex.h
│   ├── parserInterp.h
│   ├── val.h
│
├── tests/               # Sample input programs
│   ├── test1.sfort
│   ├── test2.sfort
│   └── ...
│
├── tests_answers/       # Expected outputs for tests
│   ├── test1.correct
│   ├── test2.correct
│   └── ...
│
├── build.bat            # Windows build script
└── README.md            # Project documentation



---

## 🛠️ Build Instructions

### 🔹 Windows
1. Open **PowerShell** or **Command Prompt** in the project root.  
2. Run the build script:
   ```powershell
   .\build.bat
3. If successful, an executable sfort95.exe will be created.

### Run Instructions

To run the interpreter, provide a SFort95 program file as input:

.\sfort95.exe tests\test1(for example test1)   # On Windows


### Example Input (tests\test1)
program test
  integer :: x = 5
  print *, x
end program test

### Example Output
H W 5.00
Variables in SymTable:
Variable: a, Value: 4
Variable: b, Value: 4
Variable: p, Value: 4
Variable: r, Value: 4
Variable: str1, Value: 5
Variable: str2, Value: 5
(DONE)

Successful Execution
