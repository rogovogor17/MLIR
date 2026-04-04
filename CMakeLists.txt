cmake_minimum_required(VERSION 3.20)
project(ONNX2MLIR2LLVM LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)
find_path(ONNX_MLIR_INCLUDE_DIR "src/Compiler/CompilerUtils.hpp"
          PATHS ${ONNX_MLIR_ROOT} $ENV{ONNX_MLIR_ROOT})

if(NOT ONNX_MLIR_INCLUDE_DIR)
    message(FATAL_ERROR "Не найден onnx-mlir. Укажите ONNX_MLIR_ROOT или установите библиотеку.")
endif()

include_directories(${LLVM_INCLUDE_DIRS} ${MLIR_INCLUDE_DIRS} ${ONNX_MLIR_INCLUDE_DIR} $ENV{ONNX_MLIR_ROOT}/include $ENV{ONNX_MLIR_ROOT}/build)
link_directories(${LLVM_LIBRARY_DIRS} ${MLIR_LIBRARY_DIRS} ${ONNX_MLIR_ROOT}/build/Release/lib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    MLIRIR
    MLIRSupport
    LLVMSupport
    onnx-mlir
)

llvm_map_components_to_libnames(llvm_libs Support Core)
target_link_libraries(${PROJECT_NAME} PRIVATE ${llvm_libs})
