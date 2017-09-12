# Compiler

[![Awesome Badges](https://img.shields.io/badge/badges-awesome-green.svg)](https://github.com/Naereen/badges)

> This is a compiler developed for CS4280 Program Translation at UMSL.

This project was a semester long project to build a working compiler translating from some subset of a modern programming language into assembly or machine language with available virtual machine for execution. The end product is a compiler translating a source program into target and then executing the target through the provided virtual machine.  This compiler was successful in translating all given source code. Virtual Machine is only available as an executable, and was developed my the course instructor, Cezary Z Janikow,Ph.D.

![Picture](https://github.com/thetotaljim/Compiler/blob/master/Assets/stateTableThumb.png)

## Requirements

C/C++ compiler.

## Installation

To use this compiler, download the repository, and move all the test files into the same directory as the .cpp and .h files, as well as the script and Virtual Machine.  The included makefile can be used as such:

> `make comp`

