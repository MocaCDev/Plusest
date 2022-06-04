#ifndef RUNNER
#define RUNNER
#include "util.h"

typedef struct PreAssigns {
    bool is_grounded;
    bool is_overrided;
} _PreAssigns;

typedef struct ConfigInformation
{
   struct {
        enum OS_TYPES types[MAX_OS];
        int amnt_of_types;
        enum OS_TYPES targetting_OS;
        _PreAssigns pa;
    } OS_INFO;

    _Parser *p;
} ConfigInfo;

static ConfigInfo *config_i;

#endif