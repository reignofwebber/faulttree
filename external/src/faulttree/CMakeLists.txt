# CMakeList.txt : CMake project for faulttree, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_PREFIX_PATH E:\\_GIT\\hub\\vcpkg\\installed\\x64-windows)

find_package(RapidJSON REQUIRED)

message(...)
# Add source to this project's executable.
add_executable (faulttreetest  "main.cpp" "faulttree_impl.cpp" "faulttree_impl.h"  "faulttree.h" "test.h")
add_library (faulttree "faulttree_impl.cpp" "faulttree_impl.h"  "faulttree.h")
include_directories(faulttree PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_compile_features(faulttreetest PUBLIC cxx_std_11)
target_compile_features(faulttree PUBLIC cxx_std_11)

configure_file(config/config.json config.json)


# TODO: Add tests and install targets if needed.
install(TARGETS faulttree DESTINATION ${CMAKE_SOURCE_DIR}/../libs/debug)
# install(DIRECTORY ./ DESTINATION ${CMAKE_SOURCE_DIR}/../include
        # FILES_MATCHING PATTERN "faulttree.h")
install(FILES "faulttree.h" DESTINATION ${CMAKE_SOURCE_DIR}/../include)
install(FILES "config/config.json" DESTINATION ${CMAKE_SOURCE_DIR}/../../config)