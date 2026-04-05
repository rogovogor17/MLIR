#include <iostream>
#include <stdlib.h>
#include "utils.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <model.onnx>\n";
        return 1;
    }

    try {
        std::string proc_file_path;
        createDirectory(TEMP_DIR_);
        proc_file_path = createStep({
            .inputPath = argv[1], .outputPath = PROJECT_OUTPUT_,
            .cmdName = ONNX_MLIR_CMD_, .cmdFlags = ONNX_MLIR_FLAG_,
            .cmdlogPath = ONNX_MLIR_LOG_, .cmdProduceExt = ONNX_MLIR_EXT_
        });
        proc_file_path = createStep({
            .inputPath = proc_file_path, .outputPath = ONNX_MLIR_OPT_OUTPUT_,
            .cmdName = ONNX_MLIR_OPT_CMD_, .cmdFlags = ONNX_MLIR_OPT_FLAGS_,
            .cmdlogPath = ONNX_MLIR_OPT_LOG_, .cmdProduceExt = ""
        });
        proc_file_path = createStep({
            .inputPath = proc_file_path, .outputPath = MLIR_TRANSLATE_OUTPUT_,
            .cmdName = MLIR_TRANSLATE_CMD_, .cmdFlags = MLIR_TRANSLATE_FLAGS_,
            .cmdlogPath = MLIR_TRANSLATE_LOG_, .cmdProduceExt = ""
        });
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
