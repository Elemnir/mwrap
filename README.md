# mwrap - Library and Harness for overwriting malloc() and friends for profiling and instrumentation

To Use:

    - Add your instrumentation code to the functions in the file `apphooks.c`. 
    - Communication with the harness is possible through the MWRAPCOMMFD file descriptor. The functions in the file `harnesshooks.cpp` allow for performing online processing of messages sent by the instrumentation code.
    - Run `make` to build the library and harness
    - Assuming the program under test is `foo`, run `./mwrap foo` or `path/to/mwrap -l path/to/libmwrap.so foo`

