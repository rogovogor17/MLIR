cmake_minimum_required(VERSION 3.20)
project(MLIR2LLVM VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(DEBUG_FLAGS -DDEBUG -O0 -stdlib=libc++ -ggdb3 -fsanitize=address,alignment -Wall -Wextra -Weffc++ -Wc++17-compat
	-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body
	-Wfloat-equal -Wno-format-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual
	-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2
	-Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers
	-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -fsized-deallocation -fstack-protector
	-fstrict-overflow -fPIC -fPIE -fno-omit-frame-pointer -fPIE -Werror=vla -Wno-write-strings)

add_library(implementation)
add_subdirectory(src)
add_subdirectory(tests)
