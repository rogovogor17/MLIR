#pragma once

#include <string>

#define LLVM_EXE_TIMEOUT_ 0
#define LLVM_MEMORY_LIMIT_ 0

#define TEMP_DIR_ "temp/"
#define MLIR_OUTPUT_ TEMP_DIR_ "output"
#define ONNX_MLIR_CMD_ "onnx-mlir"
#define ONNX_MLIR_FLAG_ "--EmitMLIR"
#define ONNX_MLIR_EXT_ ".onnx.mlir"
#define CMD_LOG_PATH_ TEMP_DIR_ "log.log"

bool createDirectory(const std::string& path);
void createMLIR(const std::string& path);
