# AVL Tree
This project provides an implementation of an AVL (Adelson-Velsky and Landis) self-balancing binary search tree in C++.  
It also includes automated tests comparing the results of the AVL tree with the C++ Standard Library `std::set`.

## Features:
1. An implementation of an AVL tree with insertion and range queries
2. Comparison of results with `std::set` for correctness
3. Python scripts for automated testing and output verification

## Installation:
Clone this repository, then reach the project directory:
```sh
git clone https://github.com/bgclutch/AVL_Tree_Task_cpp.git
cd AVL_Tree_Task_cpp


## Building:
1. Build the project:
 ```sh
cmake -B build
cmake --build build
```

## Usage:
1. Navigate to the ```build``` folder:
```sh
cd build
```
2. Choose tree to run:
```sh
./avltree/avltree
```
or
```sh
./stdset/stdset
```

## Running tests:
For End To End tests:
1.1 Navigate to the ```tests``` directory:
```sh
cd tests/EndToEnd
```
1.2 Run default tests with the Python script:
```sh
python3 testrun.py
```
1.3 (Optional) Or ```regenerate``` test cases:
```sh
python3 testgen.py
```
And run it as in step 2.

For unit tests:
2.1 Navigate to the ```build``` folder:
```sh
cd build
```
2. Run unit tests:
```sh
./tests/tests
```
