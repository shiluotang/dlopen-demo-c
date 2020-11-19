#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dlfcn.h>

#include "dynamic_library.h"

static
int DynamicLibrary_load0(DynamicLibrary *me,
        char const *path, int modes) {
    int rc = 0;

    if (!me)
        goto failure;
    me->clear_error();
    me->_M_handle = dlopen(path, modes);
    if (!me->_M_handle) {
        (void) me->dump_error();
        goto failure;
    }

    goto success;
exit:
    return rc;
success:
    rc = 1;
    goto cleanup;
failure:
    rc = 0;
    goto cleanup;
cleanup:
    goto exit;
}

static
int DynamicLibrary_load(DynamicLibrary *me, char const *path) {
    if (!me)
        return 0;
    return me->load0(me, path, RTLD_LAZY | RTLD_LOCAL);
}

static
int DynamicLibrary_unload(DynamicLibrary *me) {
    int rc = 0;

    if (!me)
        goto failure;
    if (!me->_M_handle)
        goto success;
    me->clear_error();
    if (dlclose(me->_M_handle) != 0) {
        (void) me->dump_error();
        goto failure;
    }
    me->_M_handle = NULL;

    goto success;
exit:
    return rc;
success:
    rc = 1;
    goto cleanup;
failure:
    rc = 0;
    goto cleanup;
cleanup:
    goto exit;
}

static
void* DynamicLibrary_search(DynamicLibrary *me, char const* symbol) {
    void *r = NULL;

    if (!me)
        goto failure;
    me->clear_error();
    r = dlsym(me->_M_handle, symbol);
    if (me->dump_error())
        goto failure;

    goto success;
exit:
    return r;
success:
    goto cleanup;
failure:
    goto cleanup;
cleanup:
    goto exit;
}

static
int DynamicLibrary_dump_error() {
    char const *err = NULL;
    err = dlerror();
    if (err)
        fprintf(stderr, "%s\n", err);
    return !!err;
}

static
void DynamicLibrary_clear_error() {
    (void) dlerror();
}

int DynamicLibrary_free(DynamicLibrary *lib) {
    int rc = 0;
    if (!lib)
        goto success;
    if (!lib->unload(lib))
        goto failure;

    goto success;
exit:
    return rc;
success:
    rc = 1;
    goto cleanup;
failure:
    rc = 0;
    goto cleanup;
cleanup:
    goto exit;
}

DynamicLibrary* DynamicLibrary_init(DynamicLibrary *lib) {
    DynamicLibrary *r = NULL;
    if (!lib) {
        *(void**) &lib = calloc(1, sizeof(*lib));
        if (!lib)
            goto failure;
    } else
        memset(lib, 0, sizeof(*lib));
    lib->load0 = DynamicLibrary_load0;
    lib->load = DynamicLibrary_load;
    lib->unload = DynamicLibrary_unload;
    lib->search = DynamicLibrary_search;
    lib->dump_error = DynamicLibrary_dump_error;
    lib->clear_error = DynamicLibrary_clear_error;

    goto success;
exit:
    return r;
success:
    r = lib;
    lib = NULL;
    goto cleanup;
failure:
    goto cleanup;
cleanup:
    if (lib) {
        free(lib);
        lib = NULL;
    }
    goto exit;
}
