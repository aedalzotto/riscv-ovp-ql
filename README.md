# riscv-ovp-dbg

Imperas QuantumLeap execution example.

This is made using 28 RISC-V CPUs and the dhrystone benchmark.

## Running the test:

No parallelism:
```
make run
```

Set the number of threads you wish to run:
```
make run THREADS=N
```

Set the number of threads to the maximum supported:
```
make run MAXTHREADS=1
```

## Check results

The `make run` command should output the elapsed time of the application.