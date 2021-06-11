# MinimalBasicInterpreter
A Minimal Basic Interpreter implemented in C++ and Qt.

## Features
- Code display
- Code loading
- Code editing
- Abstract Syntax Tree(AST) display
- Program input & output
- Console commands
- Step execute debugging
- Variable state display
- Syntax error highlight

## Supported Statements

### REM
`REM This is a comment.`

This statement is used for comments. Any text on the line after the keyword `REM` is ignored.

### LET
`LET var = 1`

This statement is BASIC’s assignment statement.
The `LET` keyword is followed by a variable name, an equal sign, and an expression.
As in C++, the effect of this statement is to assign the value of the expression to the variable, replacing any previous value.
In BASIC, assignment is not an operator and may not be nested inside other expressions.

The value can only be a string or an integer, string value should not contain `"` or `'`.

### PRINT
In minimal BASIC, the PRINT statement has the form: `PRINT exp`
where exp is an expression. The effect of this statement is to print the value of the
expression on the console and then print a newline character so that the output
from the next `PRINT` statement begins on a new line.

### PRINTF
`PRINT "Hello, {}", "WORLD"`

`PRINTF` is used for formatted output. It accepts 1 to N arguments, the first argument should be a format string
which contains 0 or N-1 "{}"s as placeholder(s), corresponding to the following N-1 arguments.
The format string must not contain a single "{" or "}".

### INPUT
In the minimal version of the BASIC interpreter, the INPUT statement has the form:
`INPUT var`
where `var` is a variable read in from the user. The effect of this statement is to
print a prompt consisting of the string "?" and then to read in a value to be stored in the variable.
(The string "?" will be displayed in the command input editbox in GUI.)

Only integer input is supported.

### INPUTS
`INPUTS var`

Similar to `INPUT`, but accepts a string as input.

### GOTO
This statement has the syntax `GOTO n`
and forces an unconditional change in the control flow of the program. When the
program hits this statement, the program continues from line n instead of
continuing with the next statement. An error will be thrown if line n
does not exist.

### IF
This statement provides conditional control. The syntax for this statement is:
`IF exp1 op exp2 THEN n`
where `exp1` and `exp2` are expressions and `op` is one of the conditional operators
`=`, `<`, or `>`. If the condition holds, the program should continue from line `n` just as
in the `GOTO` statement. If not, the program continues on to the next line.

Note that the conditional operators (`=`, `<`, `>`) are not parts of expressions.

### END
Marks the end of the program. Execution halts when this line is reached. This
statement is usually optional in BASIC programs because execution also stops if
the program continues past the last numbered line.

## Supported Commands

### RUN
This command starts program execution beginning at the lowest-numbered line.
Unless the flow is changed by `GOTO` and `IF` commands, statements are executed
in line-number order. Execution ends when the program hits the `END` statement or
continues past the last statement in the program.

### LOAD
This command loads a file containing statements and commands. Statements and
commands will be stored (also displayed in GUI) and executed respectively, as
if they were entered into input box in order. A prompt window will be
displayed when this command is entered to ask the user to choose a
file to load.

### DEBUG
Enter debug mode.

### STEP
Step execute, only available in debug mode.

### CLEAR
This command deletes the program so the user can start entering a new one.

### HELP
This command displays a simple help message.

### QUIT
Typing `QUIT` exits from the BASIC interpreter.

### Code editing
1. Add a new line by typing a command starting with a line number following a statement, e.g. `35 INPUT n3`

2. This above statement is inserted into the program between line 30 and line 40. To update line 40, type `40 LET total = n1 + n2 + n3`

3. In classical implementations of BASIC, the standard mechanism for deleting lines was to
type in a line number with nothing after it on the line. e.g. `40` deletes line 40

> The `LET`, `PRINT`, and `INPUT` statements can be executed directly by typing them without a
line number, in which case they are evaluated immediately

## Supported arithmetics
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Exponentiation: `**`
