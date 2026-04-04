#CMakeLists.txt for source files

find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})
add_definitions(${MLIR_DEFINITIONS})

target_sources(implementation
    PUBLIC utils.cpp
)
target_include_directories(implementation PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(MAIN_TARGET ${PROJECT_NAME})
add_executable(${MAIN_TARGET}
	main.cpp
)

target_include_directories(${MAIN_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(${MAIN_TARGET} PUBLIC $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_options(${MAIN_TARGET} PUBLIC $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_libraries(${MAIN_TARGET}
	implementation
	MLIRIR
    MLIRParser
    MLIRPass
    MLIRDialect
    MLIRConversionPasses
    MLIRTransformsPasses
    MLIRTargetLLVMIR
    LLVMSupport
    LLVMCore
)
