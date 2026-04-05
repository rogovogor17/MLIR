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

void createDirectory(const std::string& path) {
    std::error_code ec;

    if (std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec))
        return;

    if (!std::filesystem::create_directories(path, ec))
        return;

    std::string error_msg = "[Error] Cannot create directory '" + path
                            + "': " + ec.message() + "\n";
    throw std::runtime_error(error_msg);
}

std::string& createStep(keyNames&& names) {

    std::string error_msg;
    std::string& outputPath = names.outputPath;

    llvm::ErrorOr<std::string> exePathOrErr = llvm::sys::findProgramByName(names.cmdName);
    if (!exePathOrErr) {
        error_msg = "[Error]" + names.cmdName + " not found: " + exePathOrErr.getError().message();
        throw std::runtime_error(error_msg);
    }
    std::string exePath = *exePathOrErr;
    llvm::SmallVector<llvm::StringRef, 8> args = {
        exePath, names.inputPath, "-o", outputPath, names.cmdFlags,
    };

    std::optional<llvm::StringRef> redirects[] = {
        std::nullopt,
        llvm::StringRef(names.cmdlogPath),
        llvm::StringRef(names.cmdlogPath)
    };

    int rc = llvm::sys::ExecuteAndWait(exePath, args, std::nullopt,
        llvm::ArrayRef<std::optional<llvm::StringRef>>(redirects),
        LLVM_EXE_TIMEOUT_, LLVM_MEMORY_LIMIT_, &error_msg
    );

    if (rc != 0) {
        error_msg = "[Error]" + names.cmdName + " exited with code " + std::to_string(rc)
                    + "\n" + "Details: " + error_msg.c_str();
        throw std::runtime_error(error_msg);
    }

    outputPath += names.cmdProduceExt;
    std::ifstream mlirFile(outputPath);
    if (!mlirFile) {
        error_msg = "[Error] Could not open output file: " + outputPath;
        throw std::runtime_error(error_msg);
    }

    return outputPath;
}
