# mwrap - Library and Harness for overwriting malloc() and friends for profiling and instrumentation

To Use:

    - Add your instrumentation code to the functions in the file `hooks.c`. Communication with the harness is possible through the MWRAPCOMMFD file descriptor.
    - Run `make` to build the library and harness
    - Assuming the program under test is `foo`, run `./mwrap foo` or `path/to/mwrap -l path/to/libmwrap.so foo`

