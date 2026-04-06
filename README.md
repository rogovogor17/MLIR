# MLIR

> Проект представляет собой исследование-изучение интерфейсов, предоставляемых MLIR и LLVM, для компиляции нейронной сети в формате ONNX. В качестве предмета-примера исследования была взята нейронная сеть SqueezeNet1.0-3, граф нейронной сети которой был построен в предыдущем проекте ONNX-Frontend.

---

## Содержание

0. [Дисклеймер](#дисклеймер)
1. [Цель и обзор исследования](#цель-и-обзор-исследования)
2. [Архитектура и поток данных](#архитектура-и-поток-данных)
3. [Окружение и зависимости](#окружение-и-зависимости)
4. [Шаг 1: Подготовка ONNX модели](#шаг-1-подготовка-onnx-модели)
5. [Шаг 2: Конвертация ONNX → MLIR → LLVM IR](#шаг-2-конвертация-onnx--mlir--llvm-ir)
6. [Шаг 3: Исполняемый файл](#шаг-3-исполняемый-файл)
7. [Шаг 4: Предобработка изображений](#шаг-4-предобработка-изображений)
8. [Шаг 5: Валидация на Python](#шаг-5-валидация-на-python)
9. [Troubleshooting и уроки](#troubleshooting-и-уроки)
10. [Ссылки и ресурсы](#ссылки-и-ресурсы)

---

## Дисклеймер

> [!CAUTION]
> Хоть проект и предоставляет возможность самостоятельно пройти все стадии исследования с использованием программ из исходных файлов, ОДНАКО **исходные файлы максимально чувствительны к путям установленных и используемых в данном проекте инструментов на конкретном устройстве**. Как автор проекта, считаю результатом исследования данное README описание проекта, в связи с чем не рекомендую производить запуск исходных файлов без веской на то причины и без предварительной настройки своего окружения. Используемое окружение и зависимости этого проекта обговариваются далее.

---

## Цель и обзор исследования

Исследование ставило цель: **произвести запуск нейронной сети в формате ONNX, используя интерфейсы MLIR и LLVM**:

1. Взять нейронную сеть в формате `.onnx` и произвести ее последовательное преобразование в исполняемый файл со следующими промежуточными стадиями: `ONNX-MLIR`, `MLIR с LLVM диалектами` и `LLVM IR`, написав драйвер на C++.
2. Скомпилировать в бинарный файл для macOS (ARM64).
3. Написать C++ клиент, тестирующий драйвер на примере конкретной нейронной сети `SqueezeNet1.0-3`, которая классифицирует изображение по своему контентному содержанию среди 1000 доступных классов.
4. Подтвердить работоспособность клиента непосредственным запуском `.onnx` модели через инструмент `onnxruntime`.

В ходе работы были преодолены проблемы совместимости API LLVM 22+, выявлены неточности при классификации изображения объектом исследования.

---

## Архитектура и поток данных

```mermaid
A[ONNX Model .onnx] -->|onnx-mlir --EmitMLIR| B[MLIR .onnx.mlir]
B -->|onnx-mlir-opt --convert-krnl-to-llvm| C[MLIR with LLVM Dialects .mlir]
C -->|mlir-translate --mlir-to-llvmir| D[LLVM IR .ll]
E[OMTensor API main.c] --> F[LLVM IR usage]
F -->|clang .ll main.c| D[Native Executable]
D --> G[Top-3 Classes]
```

---

## Окружение и зависимости

Что использовалось при создании проекта и чему соответствую исходные файлы проекта:

| Компонент         | Версия / Путь                                    | Назначение                            |
| ----------------- | ------------------------------------------------ | ------------------------------------- |
| OS                | macOS 14+ (Apple Silicon)                        | Хост-система                          |
| LLVM / MLIR       | 22.1.1 (PATH += `/path/to/llvm/bin`)             | Компилятор и диалекты                 |
| onnx-mlir         | `v0.5+` (PATH += `/path/to/onnx-mlir/build/bin`) | Фронтенд ONNX → MLIR                  |
| CMake             | ≥3.20                                            | Система сборки                        |
| C++ Стандарт      | C++17 / C99                                      | Язык реализации                       |
| Python            | 3.10+                                            | Валидация и конвертация               |
| Библиотеки Python | `onnx`, `onnxruntime`, `Pillow`, `numpy`         | Эталонный инференс                    |
| C Header          | `stb_image.h`                                    | Загрузка изображений без зависимостей |

---

## Шаг 1: Подготовка ONNX модели

Если нет уверенности в том, что модель соответствует современным стандартам наборов операций ONNX, то можно привести модель к этому стандарту. В качестве примера как это можно сделать, в директории исходных файлов расположен файл `upgrader.py`.

> ⚠️ Модель `SqueezeNet1.0-3` обучена в Caffe2 и экспортирована с `opset_version: 3`. Современные рантаймы могут требовать апгрейда до `opset 12+`.

---

## Шаг 2: Конвертация ONNX → MLIR → LLVM IR

Используя написанный драйвер `src/main.cpp`:

```bash
make debug
make run-debug ARGS="squeezenet1.0-12.onnx"
```

получим в сформированной директории `temp/` следующий набор промежуточных стадий компиляции модели:

0. `output.tmp` - временный файл работы onnx-mlir.
1. `output.onnx.mlir` - MLIR модели, который следует "понизить" до уровня диалектов LLVM IR.
2. `llvm_dialect.mlir` - MLIR модели, который можно перевести в LLVM IR используя соответствующий интерфейс.
3. `output.ll` - LLVM IR модели, который можно перевести в исполняемый файл.
4. `*.log` - информация о работе соответствующих интерфейсов LLVM.

---

## Шаг 3: Исполняемый файл

Используя написанный драйвер `src/main.cpp`:

```bash
make debug
make run-debug ARGS="squeezenet1.0-12.onnx"
```

---

## Шаг 4: Предобработка изображений

SqueezeNet из ONNX Model Zoo ожидает вход в формате **BGR**, нормализованный по ImageNet статистике.

### Алгоритм предобработки

1. Загрузка изображения → `RGB` (через `stb_image` или `PIL`).
2. Ресайз до `224×224`.
3. Конвертация каналов: `RGB → BGR`.
4. Нормализация: `(pixel/255.0 - mean) / std`.
   - `mean = [0.406, 0.456, 0.485]` (B, G, R)
   - `std  = [0.225, 0.224, 0.229]`
5. Перестановка осей: `HWC → CHW` → добавление батча `NCHW`.
6. Запись в `float32` тензор.

### Пример на C++

```c
// Каналы модели: 0=B, 1=G, 2=R
float means[3] = {0.406f, 0.456f, 0.485f};
float stds[3]  = {0.225f, 0.224f, 0.229f};
int rgb_to_bgr[3] = {2, 1, 0};

for (int c = 0; c < 3; ++c) {
    for (int y = 0; y < 224; ++y) {
        for (int x = 0; x < 224; ++x) {
            int src_idx = (y * width + x) * 3 + rgb_to_bgr[c];
            float val = img_data[src_idx] / 255.0f;
            val = (val - means[c]) / stds[c];
            input_data[c * 224 * 224 + y * 224 + x] = val;
        }
    }
}
```

---

## Шаг 5: Валидация на Python

Для верификации корректности C++ пайплайна используется эталонный скрипт на `onnxruntime`:

```python
import onnxruntime as ort
import numpy as np
from PIL import Image

img = Image.open("cat.jpg").resize((224, 224)).convert("RGB")
arr = np.array(img, dtype=np.float32) / 255.0
# RGB -> BGR + нормализация
arr = arr[:, :, ::-1]
mean = np.array([0.406, 0.456, 0.485])
std  = np.array([0.225, 0.224, 0.229])
arr = (arr - mean) / std
arr = np.expand_dims(arr.transpose(2, 0, 1), 0)

session = ort.InferenceSession("squeezenet1.0.onnx")
out = session.run(None, {session.get_inputs()[0].name: arr})[0]
print("Top-3:", np.argsort(out.flatten())[::-1][:3])
```

> ✅ Результаты C++ и Python должны совпадать с точностью до `1e-5`.

---

## Troubleshooting и уроки

| Ошибка                                                | Причина                         | Решение                                                            |
| ----------------------------------------------------- | ------------------------------- | ------------------------------------------------------------------ |
| `no member named 'cout' in namespace 'std'`           | Пропущен `#include <iostream>`  | Добавить заголовок                                                 |
| `executeAndWait: no viable conversion from nullopt_t` | API изменён в LLVM 22+          | Использовать `std::optional<ArrayRef<StringRef>>{}`                |
| `omTensorCreate(NULL) → segfault`                     | Функция не выделяет память      | Заменить на `omTensorCreateEmpty()`                                |
| `Class 669 (bridge) вместо кота`                      | Перепутаны RGB/BGR или mean/std | Поменять каналы местами и использовать BGR-нормализацию            |
| `CMake: No rule to make target output.ll`             | Файл ещё не сгенерирован        | Добавить `if(EXISTS ...)` в `CMakeLists.txt`                       |
| `Relu(1) not implemented`                             | Устаревший opset 3              | Апгрейднуть модель: `version_converter.convert_version(model, 12)` |

---

## Ссылки и ресурсы

- 🔗 [onnx-mlir Repository](https://github.com/onnx/onnx-mlir)
- 🔗 [ImageNet 1000 Classes](https://raw.githubusercontent.com/pytorch/hub/master/imagenet_classes.txt)
- 🔗 [stb_image.h (Single-file image loader)](https://github.com/nothings/stb/blob/master/stb_image.h)
- 🔗 [MLIR Documentation](https://mlir.llvm.org/)
- 🔗 [ONNX Runtime C API](https://onnxruntime.ai/docs/api/c/)

---

## Заключение

Исследование успешно продемонстрировало полный цикл компиляции и запуска нейронной сети:

1. Восстановление ONNX из текстового дампа.
2. Генерация MLIR и LLVM IR через `onnx-mlir`.
3. Интеграция с `libcruntime` и исправление API-несовместимостей LLVM 22+.
4. Корректная предобработка изображений (BGR, нормализация, HWC→CHW).
5. Валидация через Python и совпадение предсказаний.

Полученный пайплайн готов к расширению: добавлению динамических шейпов, квантованию (`INT8`), деплою на edge-устройства или интеграции в production-сервисы.

---

export PATH="/path/to/onnx-mlir/build/Release/bin:$PATH"
export PATH="/path/to/llvm/bin:$PATH"

onnx-mlir-opt --convert-krnl-to-llvm temp/output.onnx.mlir -o temp/llvm_dialect.mlir
mlir-translate --mlir-to-llvmir temp/llvm_dialect.mlir -o temp/output.ll
/opt/homebrew/opt/llvm/bin/clang -O3 -Wno-override-module \
 -I${HOME}/onnx-mlir/include \
  temp/output.ll src/main.c \
  -L${HOME}/onnx-mlir/build/Release/lib -lcruntime \
 -o temp/app
