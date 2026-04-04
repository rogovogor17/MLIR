// Упрощённый main.cpp
#include "mlir/IR/MLIRContext.h"
#include "src/Compiler/CompilerUtils.hpp"  // из onnx-mlir

int main(int argc, char **argv) {
    if (argc != 2) return 1;

    mlir::MLIRContext context;
    context.allowUnregisteredDialects();

    std::string mlirStr;
    // Прямой вызов внутренней функции компиляции
    auto result = mlir::onnx_mlir::compileModuleToMLIRString(
        argv[1],           // путь к .onnx
        mlirStr,           // выходная строка
        {},                // опции (пустые = дефолт)
        &context
    );

    if (mlir::failed(result)) return 1;
    std::cout << mlirStr;
    return 0;
}
