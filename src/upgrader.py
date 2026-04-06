import onnx
from onnx import version_converter

input_file = "ONNX-Frontend/onnx/squeezenet1.0-3/squeezenet1.0-3.onnx"
output_file = "squeezenet1.0-12.onnx"

print(f"Uploading model {input_file}...")
model = onnx.load(input_file)

print(f"Opset transformation {model.opset_import[0].version} -> 12...")
try:
    converted_model = version_converter.convert_version(model, 12)

    onnx.save(converted_model, output_file)
    print(f"Saved ad: {output_file}")

except Exception as e:
    print(f"[ERROR] Failed to transform model: {e}")
