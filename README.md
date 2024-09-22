# Polish Notation Converter

## Overview

This project provides a tool to handle mathematical expressions in **infix notation** by performing the following key operations:
1. **Convert Infix to Postfix Notation** (Reverse Polish Notation)
2. **Evaluate the Postfix Expression**
3. **Plot the Graph** of the given mathematical expression over a specified range

The code is implemented in C and uses a stack-based approach.

## Features

1. **Infix to Postfix Conversion:**
   - Handles mathematical operators (`+`, `-` (unary and binary), `*`, `/`, `^`)
   - Includes function support (`sin`, `cos`, `tan`, `cot`, `sqrt`, `ln`)
   - Expression is converted using a stack

2. **Postfix Expression Evaluation:**
   - After converting to postfix, the expression is evaluated using a stack

3. **Graph Plotting:**
   - The tool can evaluate the expression for a range of values and plot a simple graph
   - Graphs are plotted using text-based characters in the terminal

## Directory Structure

```plaintext
polish_notation/
├── build/              # Build files and compiled output
├── src/                # Source code directory
│   ├── dejkstra_alg/   # Contains code for infix to postfix conversion and expression logic check
│   ├── stack/          # Stack implementation for handling operators and operands
│   ├── graph/          # Code for graph plotting functionality
│   ├── whotype/        # Helper functions for type-checking (e.g., isDigit, isOperator)
│   ├── polish_calc/    # Polish calculator for postfix evaluation
│   └── main.c          # Main entry point of the program
└── README.md           # This file
```

## Usage Example

![Example Image](images/funcsTest01.png)
