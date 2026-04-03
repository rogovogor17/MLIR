#CMakeLists.txt for source files

target_sources(implementation
	PUBLIC utils.cpp
)

target_include_directories(implementation PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(MAIN_TARGET ${PROJECT_NAME})
add_executable(${MAIN_TARGET}
	main.cpp
)

target_link_libraries(${MAIN_TARGET} PUBLIC implementation)
target_include_directories(${MAIN_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(${MAIN_TARGET} PUBLIC $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_options(${MAIN_TARGET} PUBLIC $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
