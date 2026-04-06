# MLIR

export PATH="/path/to/onnx-mlir/build/Release/bin:$PATH"
export PATH="/path/to/llvm/bin:$PATH"

onnx-mlir-opt --convert-krnl-to-llvm temp/output.onnx.mlir -o temp/llvm_dialect.mlir
mlir-translate --mlir-to-llvmir temp/llvm_dialect.mlir -o temp/output.ll
/opt/homebrew/opt/llvm/bin/clang -O3 -Wno-override-module \
 -I${HOME}/onnx-mlir/include \
  temp/output.ll src/main.c \
  -L${HOME}/onnx-mlir/build/Release/lib -lcruntime \
 -o temp/app
