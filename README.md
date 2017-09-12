# Compiler

[![Awesome Badges](https://img.shields.io/badge/badges-awesome-green.svg)](https://github.com/Naereen/badges)

> This is a compiler developed for CS4280 Program Translation at UMSL.

This project was a semester long project to build a working compiler translating from some subset of a modern programming language into assembly or machine language with available virtual machine for execution. The end product is a compiler translating a source program into target and then executing the target through the provided virtual machine.  This compiler was successful in translating all given source code. Virtual Machine is only available as an executable, and was developed my the course instructor, Cezary Z Janikow,Ph.D.

![Picture](https://github.com/thetotaljim/Compiler/blob/master/Assets/stateTableThumb.png)

## Requirements

C/C++ compiler.

## Installation

To use this compiler, download the repository, and move all the test files into the same directory as the .cpp and .h files, as well as the script and Virtual Machine.  

## Usage Example

The included makefile can be used as such:

```sh
~$ make comp
```
Also included is a bash script for testing different source code files. It can be used as such:

```sh
~$ ./runScript.sh
```
This will run the 12 included source code files through the compiler, and then use the resulting .asm files with the included VirtualMachine.  The resulting text will let you know if your .asm code was successful or not. Here is what it looks like:

![Picture](https://github.com/thetotaljim/Compiler/blob/master/Assets/runScriptExample.png)

## Grammar

Here is the grammar used to write the source code, found in the files with extension .4280E01:

![Picture](https://github.com/thetotaljim/Compiler/blob/master/Assets/compiler_grammer.png)

## Contents 

Here is a list of the included files and their usage in this project:

* ```sh main.cpp ```


