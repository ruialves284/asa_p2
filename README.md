# ASA Project 2

School project for Analysis and Synthesis of Algorithms.

The objective was to do image segmentation using max-flow/min-cut algorithms.
The algorithm implemented was Edmonds-Karp.
More info regarding the project and its objectives can be found in the file p2-enunciado.pdf but it's in Portuguese.
There's also a report that was part of the project, al058-proj2ASA-relatorio.pdf, also in Portuguese.

## Getting Started

There's already a compiled version of the code (file a.out).
The source code is in the file finalV.c, including some useful comments. The code does not have exhaustive comments explaining every step.

## Running the tests

There are two folders with public tests: publicTests1 and publicTests2_BigIO. For every test there's an input file (.in) and an expected output file (.out).

To run the tests and confirm the results one should do the following:
Run the program a.out with a .in file as input and redirect the output to an empty file of your choice. Example:
$ ./a.out < publicTests1/t01.in > results.txt
Compare the output of that file with the respective expected output file. Example:
$ diff results.txt publicTests1/t01.out
This command should have no output.

## Authors

* **Rui Alves** - *Initial work* - [ruialves284](https://github.com/ruialves284)

* **João Coelho**
