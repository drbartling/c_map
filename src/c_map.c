#include "c_map.h"
#include <stdlib.h>

map_handle_t MAP_New() {
  map_handle_t foo = malloc(sizeof(map_t)) ;
  return foo;
}
