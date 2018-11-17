#include "c_map.h"
#include "c_map_internal.h"
#include "catch.hpp"

#include <array>
#include <cstdint>
#include <random>
#include <string_view>

int
rand_int() {
        std::random_device r("/dev/random");
        int num = static_cast<int>(r());
        return num;
}

SCENARIO("C Map tests") {
        auto map_handle = MAP_new();

        THEN("you will have a valid pointer") {
                REQUIRE(NULL != map_handle);
        }

        THEN("The size will be 0") {
                REQUIRE(0 == MAP_size_get(map_handle));
        }

        WHEN("An integer is read from a key that doesn't exist") {
                (void)MAP_from_key_with_int(map_handle, "answer");
                THEN("Need to emit error in some way...") {
                        auto result = MAP_err_get(map_handle);

                        auto result_str = MAP_err_as_string(result);
                        INFO("Result is ");
                        INFO(result_str)
                        REQUIRE(MAP_key_not_found == result);
                }
        }

        WHEN("An integer is assigned to a key") {
                auto expected = rand_int();
                MAP_assign_key_with_int(map_handle, "key", expected);

                THEN("The size is 1") {
                        REQUIRE(1 == MAP_size_get(map_handle));
                }
                THEN("The value can be read back from the key") {
                        auto result = MAP_from_key_with_int(map_handle, "key");
                        REQUIRE(expected == result);
                }
                THEN("Reading is successful") {
                        (void)MAP_from_key_with_int(map_handle, "key");
                        auto result = MAP_err_get(map_handle);

                        auto result_str = MAP_err_as_string(result);
                        INFO("Result is ");
                        INFO(result_str)
                        REQUIRE(MAP_success == result);
                }
                WHEN("The key is re-assigned") {
                        auto old_value = expected;
                        expected = rand_int();
                        MAP_assign_key_with_int(map_handle, "key", expected);

                        THEN("The size is still 1") {
                                REQUIRE(1 == MAP_size_get(map_handle));
                        }
                        THEN("The new value can be read back from the key") {
                                auto result = MAP_from_key_with_int(
                                        map_handle, "key");
                                INFO("Old value is")
                                INFO(old_value);
                                REQUIRE(expected == result);
                        }
                }
        }

        WHEN("Multiple integers are assigned to multiple keys") {
                int const size = 21;
                char keys[size][2] = {};
                std::array<int, size> nums = {};
                for(int i = 0; i < size; i++) {
                        keys[i][0] = static_cast<char>(('a' + i));
                        nums[static_cast<size_t>(i)] = rand_int();
                }
                for(int i = 0; i < size; i++) {
                        auto const key = keys[i];
                        auto const expected_num
                                = nums.at(static_cast<size_t>(i));
                        MAP_assign_key_with_int(map_handle, key, expected_num);
                }
                THEN("The size is grows") {
                        REQUIRE(size == MAP_size_get(map_handle));
                }
                THEN("The values can be read back") {
                        for(int i = 0; i < size; i++) {
                                auto const key = keys[i];
                                auto const expected_num
                                        = nums.at(static_cast<size_t>(i));
                                auto result = MAP_from_key_with_int(
                                        map_handle, key);
                                INFO("key is");
                                INFO(key);
                                REQUIRE(expected_num == result);
                        }
                }
        }

        MAP_delete(map_handle);
}
