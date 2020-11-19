#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../src/dynamic_library.h"

int main(int argc, char* *argv) {
    int rc = 0;
    DynamicLibrary *lib = NULL;

    lib = DynamicLibrary_init(NULL);
    if (!lib)
        goto failure;
    if (!lib->load(lib, "/usr/lib/libacl.so"))
        goto failure;

    fprintf(stdout, "Hello World!\n");

    goto success;
exit:
    return rc;
success:
    rc = EXIT_SUCCESS;
    goto cleanup;
failure:
    rc = EXIT_FAILURE;
    goto cleanup;
cleanup:
    if (lib) {
        DynamicLibrary_free(lib);
        free(lib);
        lib = NULL;
    }
    goto exit;
}
