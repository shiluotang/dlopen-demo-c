#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "dynamic_library.h"

#include "jvm.h"

static
int Jvm_vcreate(Jvm *me, int version, int noptions, va_list args) {
    int rc = 0;
    JavaVMInitArgs vmargs;
    JavaVMOption *options = NULL;
    int i, n;

    memset(&vmargs, 0, sizeof(vmargs));
    if (noptions > 0) {
        *(void**) & options = calloc(noptions, sizeof(*options));
        if (!options)
            goto failure;
        for (i = 0, n = noptions; i < n; ++i) {
            options[i].optionString = (char*) va_arg(args, char const*);
            options[i].extraInfo = NULL;
        }
    }
    vmargs.nOptions = noptions;
    vmargs.options = options;
    vmargs.ignoreUnrecognized = JNI_TRUE;
    vmargs.version = version;

    /* JNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args) */
    if (JNI_OK != me->_M_ptr(&me->_M_vm, (void**) &me->_M_env, &vmargs))
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
    if (options) {
        free(options);
        options = NULL;
    }
    goto exit;
}

static
int Jvm_create(Jvm *me, int version, int noptions, ...) {
    int rc = 0;
    va_list args;
    va_start(args, noptions);
    rc = Jvm_vcreate(me, version, noptions, args);
    va_end(args);
    return rc;
}

static
int Jvm_destroy(Jvm *me) {
    int rc = 0;

    if (!me)
        goto failure;
    if (!me->_M_vm)
        goto failure;

    if (JNI_OK != (*me->_M_vm)->DestroyJavaVM(me->_M_vm))
        goto failure;
    me->_M_vm = NULL;
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

int Jvm_free(Jvm *jvm) {
    int rc = 0;
    if (jvm) {
        if (jvm->_M_vm) {
            if (!jvm->destroy(jvm))
                goto failure;
            jvm->_M_vm = NULL;
        }
        if (jvm->_M_lib) {
            if (!DynamicLibrary_free(jvm->_M_lib))
                goto failure;
            free(jvm->_M_lib);
            jvm->_M_lib = NULL;
        }
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

Jvm* Jvm_init(Jvm *jvm, char const *path) {
    Jvm *r = NULL;
    if (!jvm) {
        *(void**) &jvm = calloc(1, sizeof(*jvm));
        if (!jvm)
            goto failure;
    } else
        memset(jvm, 0, sizeof(*jvm));
    jvm->_M_lib = DynamicLibrary_init(NULL);
    if (!jvm->_M_lib)
        goto failure;
    if (!jvm->_M_lib->load(jvm->_M_lib, path))
        goto failure;
    jvm->_M_ptr = jvm->_M_lib->search(jvm->_M_lib, "JNI_CreateJavaVM");
    if (!jvm->_M_ptr)
        goto failure;
    jvm->create = Jvm_create;
    jvm->vcreate = Jvm_vcreate;
    jvm->destroy = Jvm_destroy;

    goto success;
exit:
    return r;
success:
    r = jvm;
    jvm = NULL;
    goto cleanup;
failure:
    goto cleanup;
cleanup:
    if (jvm) {
        Jvm_free(jvm);
        free(jvm);
        jvm = NULL;
    }
    goto exit;
}
