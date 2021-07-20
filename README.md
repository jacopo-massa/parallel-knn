# Parallel-knn
## SPM Project

Experimental results of the comparison between _C++ Standard Library_ (STL), and _FastFlow_ C++ programming framework, performing a parallel version of **K-NN algorithm**.

For more information about implementation and achieved results, please have a look to the [report](https://github.com/jacopo-massa/parallel-knn/blob/main/report.pdf)
and the [results' spreadsheet](https://github.com/jacopo-massa/parallel-knn/blob/main/res.xlsx).

## Setup 💻

To build and run the project you can use the given `Makefile`, whose main targets are `stl`, `ff` and `seq`, to build all the three program versions.
So, first of all, you need to run the following command:

```console
$ make
```

After that, you need to generate some points to work on. Use the given **Python3** `generate.py` script, by running the following command:

```console
$ python3 generate.py <seed> <len> <max>
```

This command will generate a file named `input_{len}.csv` in the `input` subfolder. Once you have generated some points you can make a single run of the program with the following command:

```console
$ ./{version} <k> <workers> <inputfile>
```
where `version` can be _stl_ or _ff_.
For the sequential version, command doesn’t take the number of workers as input:

```console
$ ./seq <k> <inputfile>
```

Another script is given to make an average of timings, by executing several times the same version (_stl_ or _ff_):

```console
$ python3 avg.py <version> <iterations> <k> <workers> <inputfile>
```

