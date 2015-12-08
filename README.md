# ahalloc - Tool for overwriting malloc() and friends for profiling and instrumentation

To Use:

    - Add your instrumentation code to the functions `prof_pre_alloc`, `prof_post_alloc`, `prof_pre_free` and `prof_post_free`
    - Run the `make` to build the library
    - Assuming the program under test is `foo`, run `LD_PRELOAD=$PWD/ahalloc.so foo`

