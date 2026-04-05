#pragma once

#include <string>

#define LLVM_EXE_TIMEOUT_ 0
#define LLVM_MEMORY_LIMIT_ 0

#define TEMP_DIR_ "temp/"
#define PROJECT_OUTPUT_ TEMP_DIR_ "output"

#define MLIR_TRANSLATE_CMD_ "mlir-translate"
#define MLIR_TRANSLATE_FLAGS_ "--mlir-to-llvmir"
#define MLIR_TRANSLATE_OUTPUT_ PROJECT_OUTPUT_ ".ll"
#define MLIR_TRANSLATE_LOG_ TEMP_DIR_ "mlir.translate.log"

#define ONNX_MLIR_OPT_CMD_ "onnx-mlir-opt"
#define ONNX_MLIR_OPT_OUTPUT_ TEMP_DIR_ "llvm_dialect.mlir"
#define ONNX_MLIR_OPT_FLAGS_ "--convert-krnl-to-llvm"
#define ONNX_MLIR_OPT_LOG_ TEMP_DIR_ "mlir.opt.log"

#define ONNX_MLIR_CMD_ "onnx-mlir"
#define ONNX_MLIR_FLAG_ "--EmitMLIR"
#define ONNX_MLIR_EXT_ ".onnx.mlir"
#define ONNX_MLIR_LOG_ TEMP_DIR_ "mlir.log"

struct keyNames {
	std::string inputPath;
	std::string outputPath;
	std::string cmdName;
	std::string cmdFlags;
	std::string cmdlogPath;
	std::string cmdProduceExt;
};

void createDirectory(const std::string& path);
std::string& createStep(keyNames&& names);
