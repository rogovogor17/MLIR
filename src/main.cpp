#include <iostream>
#include <stdlib.h>
#include "utils.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <model.onnx>\n";
        return 1;
    }

    try {
        createMLIR(argv[1]);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
