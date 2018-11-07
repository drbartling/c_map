#include "c_map.h"
#include "catch.hpp"

TEST_CASE("Map new") {
  map_handle_t map_handle = MAP_New();
  REQUIRE(NULL != map_handle);
}
