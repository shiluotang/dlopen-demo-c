#ifndef JVM_H_INCLUDED
#define JVM_H_INCLUDED

#if defined(_MSC_VER)
#   include <Windows.h>
#elif defined(__MINGW32__) || defined(__CYGWIN__)
#   include <windows.h>
#endif
#include <jni.h>

#include "dynamic_library.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct tagJvm;

    typedef struct tagJvm Jvm;

    typedef jint (*vmcreate_fptr) (JavaVM **, void**, void*);

    struct tagJvm {
        int (*vcreate) (Jvm*, int version, int noptions, va_list options);
        int (*create) (Jvm*, int version, int noptions, ...);
        int (*destroy) (Jvm*);

        DynamicLibrary *_M_lib;
        vmcreate_fptr _M_ptr;
        JavaVM *_M_vm;
        JNIEnv *_M_env;
    };

    extern Jvm* Jvm_init(Jvm*, char const*);
    extern int Jvm_free(Jvm*);

#ifdef __cplusplus
}
#endif

#endif /* JVM_H_INCLUDED */
