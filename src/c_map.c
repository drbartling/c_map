#include "c_map.h"

#include "c_map_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
map_handle_t
MAP_new()
{
        int          default_capacity = 20;
        map_handle_t new_map          = malloc(sizeof(map_t));
        new_map->size                 = 0;
        new_map->err                  = MAP_success;

        new_map->capacity = default_capacity;
        new_map->keys     = malloc(sizeof(char *) * (size_t)default_capacity);
        new_map->values   = malloc(sizeof(int) * (size_t)default_capacity);
        return new_map;
}

void
MAP_delete(map_handle_t map)
{
        free(map->values);
        for(int i = 0; i < map->size; i++) {
                free(map->keys[i]);
        }
        free(map->keys);
        free(map);
}

MAP_err_t
MAP_err_get(map_handle_t map)
{
        return map->err;
}

char const *
MAP_err_as_string(MAP_err_t err)
{
        switch(err) {
        case MAP_success:
                return "MAP_success";
        case MAP_key_not_found:
                return "MAP_key_not_found";
        }
}

int
MAP_size_get(map_handle_t map)
{
        return map->size;
}

void
MAP_assign_key_with_int(map_handle_t map, char const *key, int num)
{
        if(map->size == map->capacity) {
                int new_capacity = (int)(2 * map->capacity);
                map->keys        = realloc(
                        map->keys, sizeof(int) * (size_t)new_capacity);
                map->values = realloc(
                        map->values, sizeof(char *) * (size_t)new_capacity);
                map->capacity = new_capacity;
        }
        for(int i = 0; i < map->size; i++) {
                if(0 == strcmp(key, map->keys[i])) {
                        map->values[i] = num;
                        return;
                }
        }
        int   length  = (int)strlen(key);
        char *new_key = malloc((size_t)(length + 1));
        strcpy(new_key, key);
        map->keys[map->size]   = new_key;
        map->values[map->size] = num;
        map->size++;
}

int
MAP_from_key_with_int(map_handle_t map, char const *key)
{
        int i = 0;
        for(; i < map->size; i++) {
                if(!strcmp(key, map->keys[i])) {
                        return map->values[i];
                }
        }
        map->err = MAP_key_not_found;
        return 0;
}
