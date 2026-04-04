#include <iostream>
#include <string>
#include <fstream>
#include <optional>

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Program.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/ErrorOr.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <model.onnx>\n";
        return 1;
    }

    std::string inputPath  = argv[1];
    std::string outputPath = inputPath + ".mlir";

    // 🔹 1. Поиск исполняемого файла onnx-mlir
    llvm::ErrorOr<std::string> exePathOrErr = llvm::sys::findProgramByName("onnx-mlir");
    if (!exePathOrErr) {
        std::cerr << "[Error] onnx-mlir not found: "
                  << exePathOrErr.getError().message() << "\n";
        return 1;
    }
    std::string exePath = *exePathOrErr;

    // 🔹 2. Формирование аргументов
    llvm::SmallVector<llvm::StringRef, 8> args = {
        exePath, inputPath, "-o", outputPath, "--EmitMLIR"
    };

    // 🔹 3. Запуск процесса (LLVM 22+ API)
    // Сигнатура:
    //   ExecuteAndWait(Program, Args,
    //                  Env: std::optional<ArrayRef<StringRef>> = {},
    //                  Redirects: ArrayRef<std::optional<StringRef>> = {},
    //                  SecondsToWait, MemoryLimit, ErrMsg)
    std::string errMsg;
    int rc = llvm::sys::ExecuteAndWait(
        exePath,
        args,
        /*Env=*/std::nullopt,  // 🔹 std::optional<ArrayRef<StringRef>>
        /*Redirects=*/llvm::ArrayRef<std::optional<llvm::StringRef>>{},  // 🔹 ArrayRef<std::optional<StringRef>>
        /*SecondsToWait=*/0,
        /*MemoryLimit=*/0,
        &errMsg
    );

    if (rc != 0) {
        std::cerr << "[Error] onnx-mlir exited with code " << rc << "\n";
        if (!errMsg.empty()) {
            std::cerr << "Details: " << errMsg << "\n";
        }
        return rc;
    }

    // 🔹 4. Чтение и вывод результата
    std::ifstream mlirFile(outputPath);
    if (!mlirFile) {
        std::cerr << "[Error] Could not open output file: " << outputPath << "\n";
        return 1;
    }

    std::cout << mlirFile.rdbuf();
    return 0;
}
