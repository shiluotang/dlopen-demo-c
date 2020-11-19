#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#   include <Windows.h>
#elif defined(__MINGW32__) || defined(__CYGWIN__)
#   include <windows.h>
#endif

#include <jni.h>

#include "../src/jvm.h"

int main(int argc, char* *argv) {
    int rc = 0;
    Jvm *jvm = NULL;

    jvm = Jvm_init(NULL, "/usr/lib/jvm/java-1.8.0-openjdk-amd64/jre/lib/amd64/server/libjvm.so");
    if (!jvm)
        goto failure;
    if (!jvm->create(jvm, JNI_VERSION_1_8, 1, "-verbose:jni"))
        goto failure;

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
    if (jvm) {
        Jvm_free(jvm);
        free(jvm);
        jvm = NULL;
    }
    goto exit;
}
