# Expression Evaluator

## About

This project is a simple mathematical expression evaluator written in C. It reads expressions from standard input and outputs the result to standard output. It supports basic arithmetic operations such as addition, subtraction, multiplication, and division, as well as parentheses for grouping. It also supports positive exponent powers and unary operations.
This evaluator supports both regular infix notation and [Reverse Polish Notation (RPN)](https://en.wikipedia.org/wiki/Reverse_Polish_notation) for input expressions. It makes use of the [Shunting Yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to convert infix expressions to RPN before evaluation.

### Built with

This project was built on Ubuntu 22.04 using:

* Language
* GCC compiler
* Make

## Getting started

### Prerequisites

In order to build this project, you will need:

* GCC compiler
* Make

If you want to run the project's testsuite, you will also need:

* bc `sudo apt install bc`
* dc `sudo apt install dc`

### Installation

1. Clone the repository
2. Run: ```make```

### Usage

To run the expression evaluator, you can use the following command:

```bash
$ ./eval
Enter expression(s) to evaluate (Ctrl+D to end):
```

You can then enter mathematical expressions, and the evaluator will output the result. For example:

```bash
$ ./eval
Enter expression(s) to evaluate (Ctrl+D to end):
1 + 2
3
```

In order to run the evaluator with RPN input, use the `-rpn` flag:

```bash
$ ./eval -rpn
Enter RPN expression(s) to evaluate (Ctrl+D to end):
3 4 +
7
```

As the evaluator reads from standard input, you can also pipe expressions into it:

```bash
$ echo "5 * (2 + 3)" | ./eval
25
```

Here is an example of using RPN input with a pipe:

```bash
$ echo "10 2 8 * + 3 -" | ./eval -rpn
23
```

## License

Distributed under MIT license. Please refer to `LICENSE` for more information.
