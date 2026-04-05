#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <optional>
#include "utils.hpp"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Program.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/ErrorOr.h"

bool createDirectory(const std::string& path) {
    std::error_code ec;

    if (std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec)) {
        return true;
    }

    if (!std::filesystem::create_directories(path, ec)) {
        return true;
    }

    std::cerr << "[Error] Cannot create directory '" << path
              << "': " << ec.message() << "\n";
    return false;
}

void createMLIR(const std::string& inputPath) {
    std::string error_msg;

    std::string outputPath = MLIR_OUTPUT_;
    if (!createDirectory(TEMP_DIR_))
        throw std::runtime_error("Failed to create directory: " TEMP_DIR_);

    llvm::ErrorOr<std::string> exePathOrErr = llvm::sys::findProgramByName(ONNX_MLIR_CMD_);
    if (!exePathOrErr) {
        error_msg = "[Error] onnx-mlir not found: " + exePathOrErr.getError().message();
        throw std::runtime_error(error_msg);
    }
    std::string exePath = *exePathOrErr;

    llvm::SmallVector<llvm::StringRef, 8> args = {
        exePath, inputPath, "-o", outputPath, ONNX_MLIR_FLAG_,
    };

    std::optional<llvm::StringRef> redirects[] = {
        std::nullopt,
        llvm::StringRef(CMD_LOG_PATH_),
        llvm::StringRef(CMD_LOG_PATH_)
    };

    int rc = llvm::sys::ExecuteAndWait(exePath, args, std::nullopt,
        llvm::ArrayRef<std::optional<llvm::StringRef>>(redirects),
        LLVM_EXE_TIMEOUT_, LLVM_MEMORY_LIMIT_, &error_msg
    );

    if (rc != 0) {
        error_msg = "[Error] onnx-mlir exited with code " + std::to_string(rc)
                    + "\n" + "Details: " + error_msg.c_str();
        throw std::runtime_error(error_msg);
    }

    outputPath += ONNX_MLIR_EXT_;
    std::ifstream mlirFile(outputPath);
    if (!mlirFile) {
        error_msg = "[Error] Could not open output file: " + outputPath;
        throw std::runtime_error(error_msg);
    }
}
