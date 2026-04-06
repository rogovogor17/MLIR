#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OnnxMlirRuntime.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern OMTensorList* run_main_graph(OMTensorList* input);

int main() {
    printf("Run ONNX exe!\n");

    int64_t input_shape[] = {1, 3, 224, 224};
    OMTensor* input_om = omTensorCreateEmpty(input_shape, 4, ONNX_TYPE_FLOAT);
    if (!input_om) {
        fprintf(stderr, "[ERROR] Failed to create input tensor!\n");
        exit(EXIT_FAILURE);
    }
    float* input_data = (float*)omTensorGetDataPtr(input_om);

    int width, height, channels;
    unsigned char* img_data = stbi_load("data/cat.png", &width, &height, &channels, 3);

    if (!img_data) {
        fprintf(stderr, "[ERROR] Failed to upload image!\n");
        exit(EXIT_FAILURE);
    }

    float means[3] = {0.485f, 0.456f, 0.406f};
    float stds[3]  = {0.229f, 0.224f, 0.225f};
    int target_w = 224, target_h = 224;

    for (int c = 0; c < 3; ++c) {
        for (int y = 0; y < target_h; ++y) {
            for (int x = 0; x < target_w; ++x) {

                float src_x = (float)x * (width - 1) / (target_w - 1);
                float src_y = (float)y * (height - 1) / (target_h - 1);

                int ix = (int)src_x;
                int iy = (int)src_y;

                int idx = (iy * width + ix) * 3 + c;
                float val = img_data[idx] / 255.0f;

                val = (val - means[c]) / stds[c];

                int out_idx = c * (target_h * target_w) + y * target_w + x;
                input_data[out_idx] = val;
            }
        }
    }

    stbi_image_free(img_data);

    printf("Created input tensor: %.2f MB\n",
           (1 * 3 * 224 * 224 * sizeof(float)) / 1024.0 / 1024.0);

    OMTensor* inputs[] = {input_om};
    OMTensorList* input_list = omTensorListCreate(inputs, 1);

    OMTensorList* output_list = run_main_graph(input_list);
    if (!output_list) {
        fprintf(stderr, "ONNX executing failed!\n");
        omTensorListDestroy(input_list);
        exit(EXIT_FAILURE);
    }

    OMTensor** out_oms = omTensorListGetOmtArray(output_list);
    if (!out_oms || !out_oms[0]) {
        fprintf(stderr, "No output data!\n");
        omTensorListDestroy(input_list);
        omTensorListDestroy(output_list);
        exit(EXIT_FAILURE);
    }
    OMTensor* output_om = out_oms[0];
    float* output_data = (float*)omTensorGetDataPtr(output_om);

    float max1 = -1.0f, max2 = -1.0f, max3 = -1.0f;
    int idx1 = -1, idx2 = -1, idx3 = -1;
    for (int i = 0; i < 1000; i++) {
        if (output_data[i] > max1) {
            max3 = max2; idx3 = idx2;
            max2 = max1; idx2 = idx1;
            max1 = output_data[i]; idx1 = i;
        } else if (output_data[i] > max2) {
            max3 = max2; idx3 = idx2;
            max2 = output_data[i]; idx2 = i;
        } else if (output_data[i] > max3) {
            max3 = output_data[i]; idx3 = i;
        }
    }

    printf("Success! Top-3:\n");
    printf("   1. Class %d: %.4f\n", idx1, max1);
    printf("   2. Class %d: %.4f\n", idx2, max2);
    printf("   3. Class %d: %.4f\n", idx3, max3);

    omTensorListDestroy(input_list);
    omTensorListDestroy(output_list);
    exit(EXIT_SUCCESS);
}
