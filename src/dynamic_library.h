#ifndef DYNAMIC_LIBRARY_H_INCLUDED
#define DYNAMIC_LIBRARY_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

    struct tagDynamicLibrary;
    typedef struct tagDynamicLibrary DynamicLibrary;

    struct tagDynamicLibrary {
        int (*load0)   (DynamicLibrary*, char const* path, int modes);
        int (*load)    (DynamicLibrary*, char const* path);
        int (*unload)  (DynamicLibrary*);
        void* (*search)  (DynamicLibrary*, char const* symbol);
        int (*dump_error) ();
        void (*clear_error) ();

        void *_M_handle;
    };

    extern DynamicLibrary* DynamicLibrary_init(DynamicLibrary*);
    extern int DynamicLibrary_free(DynamicLibrary*);

#ifdef __cplusplus
}
#endif

#endif /* DYNAMIC_LIBRARY_H_INCLUDED */
