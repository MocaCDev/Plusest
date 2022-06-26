#ifndef RustState
#define RustState

typedef struct __SetupInfo {
    unsigned int OS_SUPPORT;
} SetupInfo;

extern SetupInfo init_SetupInfo(unsigned int support_type);


#endif