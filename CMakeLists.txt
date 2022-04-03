cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(grpc-cpp-demo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -g -O -lprotobuf -lpthread")

execute_process(
        COMMAND which g++-11
        OUTPUT_VARIABLE G++11
)
if(G++11)
    message(STATUS "Found C++11 compiler:" ${G++11})
endif()

find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)
pkg_search_module(GRPC REQUIRED grpc++>=1.22.0)
find_library(gRPC_REFLECTION_LIBRARIES NAMES grpc++_reflection REQUIRED)

# fetch fmt :)
include(FetchContent)
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

file(GLOB PROTOS
        ${PROJECT_SOURCE_DIR}/proto/*.proto)

execute_process(COMMAND protoc --proto_path=${PROJECT_SOURCE_DIR}/proto --grpc_out=${PROJECT_SOURCE_DIR}/proto_generation
        --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin --cpp_out=${PROJECT_SOURCE_DIR}/proto_generation ${PROTOS})

add_executable(server
        ${PROJECT_SOURCE_DIR}/server/server.cpp
        ${PROJECT_SOURCE_DIR}/proto_generation/product.grpc.pb.cc
        ${PROJECT_SOURCE_DIR}/proto_generation/product.pb.cc
        )

target_include_directories(server
        PRIVATE
            ${Protobuf_INCLUDE_DIRS}
            ${gRPC_INCLUDE_DIRS}
            ${fmt_INCLUDE_DIRS}
        )

target_link_libraries(server
        PRIVATE
            fmt::fmt
            Threads::Threads
            ${GRPC_LINK_LIBRARIES}
            ${Protobuf_LIBRARIES}
            ${gRPC_REFLECTION_LIBRARIES}
)
