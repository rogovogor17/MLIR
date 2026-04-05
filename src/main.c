#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OnnxMlirRuntime.h"

/*
 * Сигнатура функции, сгенерированной onnx-mlir:
 * int run_main_graph(OmTensorList* input, OmTensorList** output);
 */
extern int run_main_graph(OmTensorList* input, OmTensorList** output);

int main() {
    printf("🚀 Запуск SqueezeNet 1.0...\n");

    // 1. Создаём входной тензор: [1, 3, 224, 224] типа float32
    int64_t input_shape[] = {1, 3, 224, 224};
    OmTensor* input_om = omTensorCreateWithShape(
        input_shape, 4, ONNX_MLIR_FLOAT32);

    // Заполняем входные данные (здесь нулями, в реальности сюда подставляют нормализованное изображение)
    float* input_data = (float*)omTensorGetDataPtr(input_om);
    memset(input_data, 0, sizeof(float) * 1 * 3 * 224 * 224);
    printf("📦 Входной тензор создан: %.2f MB\n",
           (1*3*224*224*sizeof(float)) / 1024.0 / 1024.0);

    // 2. Формируем список входов
    OmTensor* inputs[] = {input_om};
    OmTensorList* input_list = omTensorListCreate(inputs, 1);

    // 3. Запускаем модель
    OmTensorList* output_list = NULL;
    int status = run_main_graph(input_list, &output_list);
    if (status != 0) {
        fprintf(stderr, "❌ Ошибка выполнения модели: код %d\n", status);
        return 1;
    }

    // 4. Читаем результат: [1, 1000]
    OmTensor* output_om = omTensorListGetOmt(output_list, 0);
    float* output_data = (float*)omTensorGetDataPtr(output_om);

    // Находим топ-3 класса
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

    printf("✅ Успешно! Топ-3 предсказания:\n");
    printf("   1. Класс %d: %.4f\n", idx1, max1);
    printf("   2. Класс %d: %.4f\n", idx2, max2);
    printf("   3. Класс %d: %.4f\n", idx3, max3);

    // 5. Очистка памяти
    omTensorListDestroy(input_list);
    omTensorListDestroy(output_list);

    return 0;
}
