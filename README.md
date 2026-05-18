# CVM++

CVM++ is a small scripting language implemented in C++. It follows the compiler pipeline used in Crafting Interpreters:

1. Source code is scanned into tokens.
2. Tokens are parsed into an AST.
3. The AST is compiled into custom bytecode.
4. A stack-based virtual machine executes the bytecode.

## Features

- Lexer for numbers, strings, identifiers, keywords, operators, comments, and punctuation.
- Recursive descent parser with expression and statement AST nodes.
- Bytecode compiler with custom opcodes.
- Stack-based VM using `std::vector<std::any>`.
- CLI REPL and file runner.
- Debug AST and bytecode output.
- Number, boolean, and string values.
- Arithmetic: `+`, `-`, `*`, `/`.
- Comparisons: `==`, `!=`, `<`, `<=`, `>`, `>=`.
- Variables with `let`, variable reads, and assignment.
- `print` and `input`.
- Blocks with `{ ... }`.
- `if/else` control flow.
- `while` loops.

## Build

Using g++:

```bash
g++ -std=c++17 -I include src/main.cpp src/token.cpp src/scanner.cpp src/parser.cpp src/compiler.cpp src/vm.cpp -o cvm
```

Using CMake:

```bash
cmake -S . -B build
cmake --build build
```

## Run

Start the REPL:

```bash
./cvm
```

Run a script:

```bash
./cvm tests/while_loop.cvm
```

On Windows PowerShell:

```powershell
.\cvm.exe tests\while_loop.cvm
```

## Example

```c
let i = 0;

while (i < 3) {
    print i;
    i = i + 1;
}
```

Output:

```text
0
1
2
```
