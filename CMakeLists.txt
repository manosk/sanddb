cmake_minimum_required(VERSION 3.8)
project(sanddb)

include(CMake/AddClangFormat.cmake)

find_program(
        CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*")
endif()

# Including custom cmake rules
# include(CMake/clang-cxx-dev-tools.cmake)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(WARNINGS "-Wall -pedantic-errors -ansi -Wextra -Winit-self -Wold-style-cast -Werror -Woverloaded-virtual \
    -Wuninitialized -Wmissing-declarations -Winit-self")
set(STANDARDS "-ansi -std=c++17")
set(CMAKE_CXX_FLAGS "-O0 ${WARNINGS} ${STANDARDS}")

include_directories( ./src/)
set(SOURCE_FILES src/main.cpp src/operators/micro/select.h src/common/common.h src/datum/data_vector.h src/datum/array_vector.h src/datum/buffer_pool.h src/datum/data_types.h src/util/ThreadRAII.cpp src/util/ThreadRAII.h src/structures/lock_free_stack.h src/structures/lock_free_stack_hp.h src/structures/hazard_pointer.h src/structures/lock_free_stack_rc.h src/structures/stack_sp.h src/datum/table.h src/datum/column.h src/schema/types.h src/csv_helper.cpp src/csv_helper.h src/datum/int_column.cpp src/datum/int_column.h src/datum/boolean_column.cpp src/datum/boolean_column.h src/datum/float_column.cpp src/datum/float_column.h src/datum/double_column.cpp src/datum/double_column.h src/datum/string_column.cpp src/datum/string_column.h src/plan/plan_helper.cpp src/plan/plan_helper.h src/common/common.cpp src/operators/scan.cpp src/operators/scan.h src/util/catalog.cpp src/util/catalog.h src/operators/sql_operator.h src/expressions/expressions.h src/operators/select.cpp src/operators/select.h src/operators/project.cpp src/operators/project.h src/expressions/expressions.cpp)

add_executable(sanddb ${SOURCE_FILES})

set_target_properties(
        sanddb PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_FLAGS "${WARNING_FLAGS}"
)
target_link_libraries(sanddb -lfolly -lglog -larrow -lpthread -lboost_system -lboost_iostreams -lboost_filesystem)

if(CLANG_TIDY_EXE)
    set_target_properties(
            sanddb PROPERTIES
            CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
endif()