#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char** argv)
{
    void *handle;
    void (*libfunc)(int argc, char** argv);


    handle = dlopen("./libexample.so", RTLD_LAZY);

    if (!handle) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    *(void**)(&libfunc) = dlsym(handle, "libfunc");
    if (!libfunc) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    libfunc(argc, argv);
    dlclose(handle);

    return EXIT_SUCCESS;
}
