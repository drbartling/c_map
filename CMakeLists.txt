cmake_minimum_required(VERSION 3.12)

#
# Static analysis
#
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
        set(CMAKE_C_CPPCHECK ${CPPCHECK} --enable=warning,performance,style)
else()
        message(SEND_ERROR "cppcheck requested but executable not found")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        find_program(CLANGTIDY clang-tidy)
        if(CLANGTIDY)
                set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
                set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
        else()
                message(
                        SEND_ERROR
                                "clang-tidy requested but executable not found"
                )
        endif()
endif()

#
# Warning Configurations
#
add_library(c_warnings INTERFACE)
add_library(cxx_warnings INTERFACE)
target_link_libraries(cxx_warnings INTERFACE c_warnings)

if(MSVC)
        # Visual studio warnings
        target_compile_options(
                c_warnings
                INTERFACE
                        "/permissive-"
                        /W4 # All reasonable warnings
                        /w14242 # 'identfier': conversion from 'type1' to
                                # 'type1', possible loss of data
                        /w14254 # 'operator': conversion from 'type1:field_bits'
                                # to 'type2:field_bits', possible loss of data
                        /w14263 # 'function': member function does not override
                                # any base class virtual member function
                        /w14265 # 'classname': class has virtual functions, but
                                # destructor is not virtual instances of this
                                # class may not be destructed correctly
                        /w14287 # 'operator': unsigned/negative constant
                                # mismatch
                        /w14296 # 'operator': expression is always
                                # 'boolean_value'
                        /w14311 # 'variable': pointer truncation from 'type1' to
                                # 'type2'
                        /w14545 # expression before comma evaluates to a
                                # function which is missing an argument list
                        /w14546 # function call before comma missing argument
                                # list
                        /w14547 # 'operator': operator before comma has no
                                # effect; expected operator with side-effect
                        /w14549 # 'operator': operator before comma has no
                                # effect; did you intend 'operator'?
                        /w14555 # expression has no effect; expected expression
                                # with side-effect
                        /w14619 # pragma warning: there is no warning number
                                # 'number'
                        /w14640 # Enable warning on thread un-safe static member
                                # initialization
                        /w14826 # Conversion from 'type1' to 'type_2' is sign-
                                # extended. This may cause unexpected runtime
                                # behavior.
                        /w14905 # wide string literal cast to 'LPSTR'
                        /w14906 # string literal cast to 'LPWSTR'
                        /w14928 # illegal copy-initialization; more than one
                                # user-defined conversion has been implicitly
                                # applied
                        /we4289 # nonstandard extension used: 'variable': loop
                                # control variable declared in the for-loop is
                                # used outside the for-loop scope
        )
else()
        # Warnings common to clang and GCC
        target_compile_options(
                c_warnings
                INTERFACE
                        -Wall
                        -Wextra # reasonable and standard
                        -Wcast-align # warn for potential performance problem
                                     # casts
                        -Wconversion # warn on type conversions that may lose
                                     # data
                        -Wdouble-promotion # warn if float is implicit promoted
                                           # to double
                        -Wformat=2 # warn on security issues around functions
                                   # that format output (ie printf)
                        -Wnull-dereference # warn if a null dereference is
                                           # detected
                        -Wpedantic # warn if non-standard C++ is used
                        -Wshadow # warn the user if a variable declaration
                                 # shadows one from a parent context
                        -Wsign-conversion # warn on sign conversions
                        -Wunused # warn on anything being unused
        )
        target_compile_options(
                cxx_warnings
                INTERFACE
                        -Wnon-virtual-dtor # warn the user if a class with
                                           # virtual functions has a non-virtual
                                           # destructor. This helps catch hard
                                           # to track down memory errors
                        -Wold-style-cast # warn for c-style casts
                        -Woverloaded-virtual # warn if you overload (not
                                             # override) a virtual function
        )
        # GCC specific warnings
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
                target_compile_options(
                        c_warnings
                        INTERFACE
                                -Wduplicated-branches # warn if if / else
                                                      # branches have duplicated
                                                      # code
                                -Wduplicated-cond # warn if if / else chain has
                                -Wlogical-op # warn about logical operations
                                             # being used where bitwise were
                                             # probably wanted
                                -Wmisleading-indentation # warn if indentation
                                                         # implies blocks where
                                                         # blocks do not exist
                )
                target_compile_options(
                        cxx_warnings
                        INTERFACE
                                -Wuseless-cast # warn if you perform a cast to
                                               # the same type
                )
        endif()
endif()

#
# Build Option Configurations
#
add_library(c_options INTERFACE)
add_library(cxx_options INTERFACE)
target_link_libraries(cxx_options INTERFACE c_options)
if(MSVC)

else()
        option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
        if(ENABLE_COVERAGE)
                target_compile_options(
                        c_options
                        INTERFACE
                                -fprofile-instr-generate
                                -fcoverage-mapping
                                -O0
                                -g
                )
                target_link_libraries(
                        c_options
                        INTERFACE -fprofile-instr-generate -fcoverage-mapping
                )

                add_custom_target(
                        test-run
                        COMMAND ./test_c_map --use-colour=yes
                        DEPENDS test_c_map
                )

                add_custom_target(
                        cov-data
                        COMMAND
                                llvm-profdata merge default.profraw -o
                                default.profdata
                        DEPENDS test-run
                )

                add_custom_target(
                        test-cov
                        COMMAND
                                llvm-cov report -use-color
                                -instr-profile=default.profdata test_c_map
                        DEPENDS cov-data
                )

                add_custom_target(
                        test-cov-full
                        COMMAND
                                llvm-cov show -use-color
                                -instr-profile=default.profdata
                                -line-coverage-lt=100 -region-coverage-lt=100
                                test_c_map
                        DEPENDS cov-data
                )
        endif()

        option(ENABLE_ASAN "Enable address sanitizer" FALSE)
        if(ENABLE_ASAN)
                target_compile_options(c_options INTERFACE -fsanitize=address)
                target_link_libraries(c_options INTERFACE -fsanitize=address)
        endif()
endif()

#
# Library Build
#

add_library(c_map src/c_map.c include/c_map.h)
target_compile_features(c_map PRIVATE c_std_99)
target_include_directories(c_map PUBLIC include)
target_include_directories(c_map PRIVATE src)
target_link_libraries(c_map PRIVATE c_warnings c_options)

#
# Test Build
#

enable_testing()

include(ExternalProject)

externalproject_add(
        catch2
        GIT_REPOSITORY
        https://github.com/catchorg/Catch2
        GIT_TAG
        v2.4.2
        GIT_SHALLOW
        true
        CONFIGURE_COMMAND
        ""
        BUILD_COMMAND
        ""
        INSTALL_COMMAND
        ""
)

add_executable(test_c_map test/test_c_map.cpp test/test_main.cpp)
target_link_libraries(test_c_map PRIVATE c_map cxx_warnings)
target_include_directories(
        test_c_map
        PRIVATE
                ${CMAKE_BINARY_DIR}/catch2-prefix/src/catch2/single_include/catch2
                src
)
add_dependencies(test_c_map catch2)
add_test(Tester test_c_map)
