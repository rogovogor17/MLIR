cmake_minimum_required(VERSION 3.20)
project(ONNX2MLIR LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm" CACHE PATH "LLVM CMake dir")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS} -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/utils.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE
    LLVMSupport
    LLVMCore
)

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path;/opt/homebrew/opt/llvm/lib"
        BUILD_WITH_INSTALL_RPATH FALSE)
endif()
