cmake_minimum_required(VERSION 3.20)
project(MLIR VERSION 1.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEBUG_FLAGS -DDEBUG -O0 -ggdb3 -fsanitize=address,alignment -Wall -Wextra -Weffc++ -Wc++20-compat
	-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body
	-Wfloat-equal -Wno-format-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual
	-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2
	-Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers
	-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -fsized-deallocation -fstack-protector
	-fstrict-overflow -fPIC -fPIE -fno-omit-frame-pointer -fPIE -Werror=vla -Wno-write-strings)

find_program(IWYU_PATH NAMES include-what-you-use iwyu)
set(IWYU_COMMAND "${IWYU_PATH}"
	"-Xiwyu;--verbose=1;"
	"-Xiwyu;--comment_style=long;"
	"-isysroot;${CMAKE_OSX_SYSROOT};"
	"-Xiwyu;--mapping_file=$ENV{IWYU_MAPPING_FILE};"
)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_COMMAND})

add_library(implementation)
add_subdirectory(src)
add_subdirectory(tests)
