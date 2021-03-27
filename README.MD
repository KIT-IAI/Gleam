## Evolutionary Algorithms GLEAM and HyGLEAM

GLEAM is an Evolutionary Algorithm (EA) of its own and HyGLEAM is its memetic extension. 

### GLEAM at a Glance
GLEAM uses chromosomes whose genes can be easily configured in an application-specific way, so that a 
gene can determine all (relevant) properties of a given phenotypic element. For example, in a scheduling 
problem, this could be the resources to be used in a scheduling operation, e.g. a workstation or a 
classroom and an appropriate tutor. GLEAM works with three types of chromosomes: Fixed-length chromosomes 
with or without meaning-bearing order, and dynamic-length chromosomes with meaning-bearing order. 
In GLEAM, fitness calculation is based on the weighted sum. To control the selective pressure, partner 
selection is performed with linear ranking. Furthermore, instead of the otherwise common panmictic 
population, a structured one based on the neighborhood model is used. The self-adaptive balance between 
breadth-first and depth-first search thus achieved efficiently counteracts premature convergence. The 
neighborhood model can be viewed as a linear variant of cellular EAs. The memetic extension of GLEAM 
called HyGLEAM integrates local search procedures or heuristics to speed up evolutionary search.

Further details and references can be found in the User Manual and in the rest of the documentation found 
in the `Docu` sub directory.

There are two program versions of GLEAM and HyGLEAM per application, one command line version and one with a simple 
textual user interface. Both are started in a shell. There is no implementation with a graphical user 
interface.

### Implementation Issues
The software is written in C under Linux. The distribution is based on Ubuntu 20.04.1 LTS and the GNU C and 
C++ compiler version 9.3.0. 

### Intended Use
The software is suitable 
* to gain experience with EAs and Memetic Algorithms (MA), 
* to implement your own optimization project based on a well-proven EA or MA, and
* to extend the MA by own heuristics or other local search methods

### Prerequisites
If you want to extend GLEAM or HyGLEAM and/or adapt it to an optimization problem, you should have
* at minimum basic knowledge about Evolutionary Algorithms,
* programming experience and experience with Linux, and
* some experience with the C programming language is certainly helpful.

### Support
The project is the result of about 30 years of research and development work at KIT, where the software 
was mainly developed in the context of different applications. Since the authors have retired in the 
meantime, there can only be little support. A further development of the project is currently not planned. 
Unless someone wants to take it over.

### Distribution Contents
The distribution consists of 
* the sources, 
* make files for the generation of executables using the make utility, 
* software documentation, a user guide, and a guide for included and new applications, 
* some initialization files for prepared test cases, and 
* some pre-compiled executables suited for linux and Ubuntu in particular.

### Acronyms:
**GLEAM** is the abbreviation of **G**eneral **L**earning **E**volutionary **A**lgorithm and **M**ethod and 
HyGLEAM stands for 
**Hy**brid **G**enera**L** purpose **E**volutionary **A**lgorithm and **M**ethod.

### Legal Issues
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, see the 
`LICENSE.md` file.

Copyright (c) 2003 .. 2021  Christian Blume, Wilfried Jakob
