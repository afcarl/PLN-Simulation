# PLN-Simulation

(scroll to the bottom for a screenshot and a video)

PLN is a simulation of a ball bouncing on a function (a screenshot is attached in the bottom). The function is interpolated through five points using a Lagrange polynomial. These points can be manipulated. 

The ultimate goal of the simulation was to test the [Common Lisp implementation of Neuroevolution of Augmenting Topologies](https://github.com/meatich/NEAT) by evolving a network for the following task: push the ball to the right as far as possible in three seconds. 

Note: _src/NEAT_ mostly consists of modified [C++ implementation of NEAT by Stanley](http://nn.cs.utexas.edu/soft-view.php?SoftID=4). Obviously, all appropriate licenses and copyrights apply. This code was used to check how NEAT would perform on the problem.

The network was evolved successfuly both with the C++ and Common Lisp implementations.

Here is the best-performing network evolved for over 500 generations using the Common Lisp version (floats shown truncated): 
```
ID	Type	Connections (neuron id and corresponding weight)
1	0 	15 -1.21  14  0.10  13  5.47  12  1.27  11  0.61  10  0.0 
2 	0 	16  7.23  14 -0.38  13 -0.94  12  0.25  11  0.43  10  0.0 
3 	0 	16 -1.58  14  7.02  13 -1.63  12  1.81  11  3.49  10  1.52 
4 	0 	14 -2.62  13  0.92  12  2.84  11  1.01  10  0.05 
5 	0 	15  3.87  14 -10.79 13 -5.95  12 -1.93  11  1.92  10  0.12 
6 	0 	14  1.50  13  2.35  12  3.72  11 -6.03  10 -0.69 
7 	0 	16  2.20  14  8.70  13 -2.82  12  0.88  11 -6.55  10  0.0 
8 	0	14  10.55 13 -3.04  12 -7.29  11 -0.02  10  0.0 
9 	0 	14  4.40  13 -0.43  12  0.0   11 -2.14  10 -1.84 
16 	1 	11 -0.36 
15 	1 	10  1.45 
10 	2 
11	2	 
12 	2 
13 	2 
14 	2 
```
It can be seen that two neurons were added (ID: 15, 16).

## Compilation
Things that worked for me:
* [Premake 4.4](https://premake.github.io/download.html#v4)
* SFML 2.4
* MinGW32 GCC 5.1.0

Compiler and SFML should be compatible. The easy way is to make it ensure this is to download the latest SFML version from [here](http://www.sfml-dev.org/) and download the compiler which it requires (they give links for downloads too!).

Any OS should be fine, though I tested only on Windows.

Run `premake4 --file="premake4.lua" gmake` or `premake4 --file="premakeEvaluator.lua" gmake` to construct makefiles which will be placed in build_windows directory. `premake4.lua` is for a demonstration with GUI, `premakeEvaluator.lua` is for making a shared library loaded by Common Lisp NEAT.

## Demo
![alt text](https://github.com/meatich/PLN-Simulation/blob/master/sshot-252.png "Bouncy!")
