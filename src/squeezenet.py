import onnxruntime as ort
import numpy as np
from PIL import Image
import os
import sys

MODEL_PATH = "squeezenet1.0-12.onnx"
IMAGE_PATH = "data/cat.png"

def preprocess_image(image_path):
    img = Image.open(image_path).convert("RGB")
    arr = np.array(img, dtype=np.float32) / 255.0
    arr_chw = arr.transpose(2, 0, 1)
    return np.expand_dims(arr_chw, axis=0)

if __name__ == "__main__":
    if not os.path.exists(MODEL_PATH):
        print(f"Model file '{MODEL_PATH}' was not found!")
        sys.exit(1)

    if not os.path.exists(IMAGE_PATH):
        print(f"Image file '{IMAGE_PATH}' was not found!")
        sys.exit(1)

    session = ort.InferenceSession(MODEL_PATH)
    input_name = session.get_inputs()[0].name
    output_name = session.get_outputs()[0].name

    USE_BGR = False
    input_tensor = preprocess_image(IMAGE_PATH)

    print(f"Input tensor shape: {input_tensor.shape}")
    outputs = session.run([output_name], {input_name: input_tensor})

    result = outputs[0]
    flat_result = result.flatten()
    top_indices = np.argsort(flat_result)[::-1][:5]

    print("\nTop-5:")
    print("-" * 40)
    for i, idx in enumerate(top_indices):
        confidence = flat_result[idx]
        print(f"#{i+1} Class {idx}: ({confidence:.4f})")
    print("-" * 40)
