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

set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)

set(LLVM_IR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/temp/output.ll")
set(LLVM_OBJECT_FILE "${CMAKE_CURRENT_BINARY_DIR}/temp/output.o")

add_custom_command(
    OUTPUT ${LLVM_OBJECT_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/temp"
    COMMAND /opt/homebrew/opt/llvm/bin/llc -filetype=obj "${LLVM_IR_FILE}" -o "${LLVM_OBJECT_FILE}"
    DEPENDS ${LLVM_IR_FILE}
    COMMENT "Compiling LLVM IR to object file"
    VERBATIM
)

add_custom_target(LLVM_Object DEPENDS ${LLVM_OBJECT_FILE})

add_executable(LLVM2EXE
    src/main.c
    ${LLVM_OBJECT_FILE}
)

add_dependencies(LLVM2EXE LLVM_Object)

target_compile_options(LLVM2EXE PRIVATE
    -O3
    -Wno-override-module
)

target_include_directories(LLVM2EXE PRIVATE
    $ENV{HOME}/onnx-mlir/include
    include
)

target_link_directories(LLVM2EXE PRIVATE
    $ENV{HOME}/onnx-mlir/build/Release/lib
)

target_link_libraries(LLVM2EXE PRIVATE
    cruntime
)

set_target_properties(LLVM2EXE PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/temp"
    OUTPUT_NAME "app"
)
