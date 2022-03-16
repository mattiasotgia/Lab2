# Lab2 Group (2021-22) :rocket:

This repo is made to hold all the code and data generated by lab experiments.

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/mattiasotgia/Lab2)](https://github.com/mattiasotgia/Lab2/releases/latest)

There are two branches: `main` and `active`.

***Latex files*** Current status of latex files.

[![Build and Relase papers](https://github.com/mattiasotgia/Lab2/actions/workflows/build_latex.yml/badge.svg)](https://github.com/mattiasotgia/Lab2/actions/workflows/build_latex.yml)


## Usage

This repo should be maintained up-to-date with the latest experiments and data.

There is a `./esperienza_#__<title>` folder for each one lab. experience. Those are then divided as described in README for each sub-directory. 

The `setup.py` file is meant to be used to create the base project for a lab. experience. To run [setup.py](setup.py) use `python3 setup.py <OPTIONAL: expNumber_title>`

## Requirements

Software needed to use this repo is listed below:

- [`root`](https://github.com/root-project/root), An open-source data analysis framework used by high energy physics and others, developed by CERN (currently using [`v6.24/02`](https://github.com/root-project/root/tree/v6-24-02) or [`v6.22/06`](https://github.com/root-project/root/tree/v6-22-06) to run code);
- `stat` custom library (Currently `v1.0`). Run as `g++ -o <exe_name> <program.cpp> stat.cpp`.
- `LabTools.h` is a C++ library only used for general purpouse C++ classes and namespaces for easier graph manipulation and statistical computation (see [LabTools installation and usage](#labtools-installation)).
- [ErrorAnalysis/ErrorAnalysis.h](https://github.com/mattiasotgia/ErrorAnalysis) class function used to compute error propagationa and analysis.
- various python libraries are used: `numpy`, `matplotlib`, `scipy`, `pandas`, mainly for data analisys on-the-go, alongside with `jupiter-notebook`.
- [LTSpice](https://www.analog.com/en/design-center/design-tools-and-calculators/ltspice-simulator.html) is a software used for schematics simulation, needed to read `.asc` files.
- [`PyLTSpice v1.5`](https://pypi.org/project/PyLTSpice/)(also on [Github](https://github.com/nunobrum/PyLTSpice)) and [`ltspice 1.0.4`](https://pypi.org/project/ltspice/) are used for LTSpice simulation automation in Python and for data visualization.
<!-- - `numpy` Python library is only needed in order for the [setup.py](setup.py) program to work.  -->

Future code might need other libraries, that will be eventually be written here.

## Stat Library

`stat` is a `C++` library made exclusively to compute the statistical average and standard error. It is located in [`stat/stat.h`](stat/stat.h).

***Library Usage***

This library is just a piece of code written to facilitate some computing, eventually it needs to be implemented in the code.

* `stat::read_values()` takes a `stdin::vector` as input, whereas the vector contain all data needed in calculation.

* `stat::avg()` return average of elements in the `vector`.

* `stat::std_err()` return standard error computed from RMS (Root Minimum Square).

* `stat::clear()` is needed to clean up the data vector, in order to compute other data. 

## LabTools

Inside [LabTools](LabTools/) there is the full definition and implementation for the LabTools c++ library. It needs to be compiled and if used in ROOT needs to be included in the `rootlogon.C` root file. 

### How to install

inside `LabTools` folder go to `build/` (`cd build`).
Then do the following
```bash
$ cmake ..
$ make
$ sudo make install
```
Then go to $HOME and create (if non existing!) the `.rootrc` file by doing
```bash
$ touch .rootrc
```
and add the following line to `.rootrc`:
```
Rint.Logon: ~/rootlogon.C
```

Finally, in your $HOME directory create a file named `rootlogon.C` by doing
```bash
$ touch rootlogon.C
```
and modify this file adding the following lines
```cpp
{
    gInterpreter->AddIncludePath("/usr/local/include");
    gSystem->Load("/usr/local/lib/libLabTools");
}
```

Now to load the Library in any program you'll need to
```cpp
#include<LabTools.h>
```

If using ROOT macros, no other compiler instruction is needed.
To use this library in compiled c/c++ programs you'll need to do the following: 
```bash
$ g++ file_name.cpp -o exe_name -I/usr/local/include -L/usr/local/lib/ -lLabTools `root-config --glibs --cflags`
```
where `` `root-config --glibs --cflags` `` is only needed if the code calls for other ROOT specific functions.


Changelog
---------