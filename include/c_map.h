#ifndef C_MAP_H
#define C_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct map_s *map_handle_t;

map_handle_t
MAP_new();

void
MAP_delete(map_handle_t map);

typedef enum {
        MAP_success = 0,
        MAP_key_not_found,
} MAP_err_t;

MAP_err_t
MAP_err_get(map_handle_t map);

char const *
MAP_err_as_string(MAP_err_t err);

int
MAP_size_get(map_handle_t map);

void
MAP_assign_key_with_int(map_handle_t map, char const *key, int num);

int
MAP_from_key_with_int(map_handle_t map, char const *key);

#ifdef __cplusplus
}
#endif

#endif // C_MAP_H
