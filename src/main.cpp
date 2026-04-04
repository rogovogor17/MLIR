#include <iostream>
#include <memory>
#include <string>

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/OwningOpRef.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Conversion/Passes.h"
#include "mlir/Target/LLVMIR/Export.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace mlir;

int main(int argc, char** argv) {
    if (argc < 2) {
        llvm::errs() << "Usage: " << argv[0] << " <input.mlir> [output.ll]\n";
        return 1;
    }

//     std::string inputFile = argv[1];
//     std::string outputFile = (argc >= 3) ? argv[2] : "output.ll";
//
//     // Инициализация MLIR
//     MLIRContext context;
//
//     // Регистрация всех диалектов (новый API)
//     DialectRegistry registry;
//     registerAllDialects(registry);
//     context.appendDialectRegistry(registry);
//
//     // Регистрация всех пассов
//     registerAllPasses();
//
//     // Загрузка MLIR файла
//     auto module = parseSourceFile<ModuleOp>(inputFile, &context);
//     if (!module) {
//         llvm::errs() << "Failed to parse MLIR file: " << inputFile << "\n";
//         return 1;
//     }
//
//     // Конвертация в LLVM диалект через PassManager
//     PassManager pm(&context);
//
//     // В LLVM 19+ используется единый пасс для конвертации в LLVM
//     // Добавляем стандартную последовательность конвертации
//     if (failed(pm.addPass(createConvertToLLVMPass()))) {
//         llvm::errs() << "Failed to add conversion pass\n";
//         return 1;
//     }
//
//     // Выполняем пассы
//     if (failed(pm.run(*module))) {
//         llvm::errs() << "Failed to convert to LLVM dialect\n";
//         return 1;
//     }
//
//     // Трансляция в LLVM IR
//     llvm::LLVMContext llvmContext;
//     auto llvmModule = translateModuleToLLVMIR(*module, llvmContext);
//     if (!llvmModule) {
//         llvm::errs() << "Failed to translate to LLVM IR\n";
//         return 1;
//     }
//
//     // Сохранение в файл
//     std::error_code ec;
//     llvm::raw_fd_ostream out(outputFile, ec);
//     if (ec) {
//         llvm::errs() << "Failed to open output file: " << ec.message() << "\n";
//         return 1;
//     }
//
//     llvmModule->print(out, nullptr);
//     out.close();
//
//     llvm::outs() << "Successfully converted to " << outputFile << "\n";

    return 0;
}
