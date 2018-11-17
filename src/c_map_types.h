#ifndef C_MAP_TYPES_H
#define C_MAP_TYPES_H

#include "c_map.h"

typedef struct map_s {
        char **   keys;
        int *     values;
        int       size;
        int       capacity;
        MAP_err_t err;
} map_t;

#endif /* end of include guard: C_MAP_TYPES_H */
