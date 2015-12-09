# ahalloc - Tool for overwriting malloc() and friends for profiling and instrumentation

To Use:

    - Add your instrumentation code to the functions in the file `hooks.c`
    - Run the `make` to build the library
    - Assuming the program under test is `foo`, run `LD_PRELOAD=$PWD/ahalloc.so foo`

