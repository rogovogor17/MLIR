# MLIR

export PATH="/path/to/onnx-mlir/build/bin:$PATH"
export PATH="/path/to/llvm/bin:$PATH"

onnx-mlir-opt --convert-krnl-to-llvm temp/output.onnx.mlir -o temp/llvm_dialect.mlir
mlir-translate --mlir-to-llvmir temp/llvm_dialect.mlir -o temp/output.ll
