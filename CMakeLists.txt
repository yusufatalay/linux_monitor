cmake_minimum_required(VERSION 3.10)
project(linux_monitor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
	src/main.cpp
	src/CpuCollector.cpp
	src/MemoryCollector.cpp
	src/DiskCollector.cpp
	src/RuleEngine.cpp
	src/AlertManager.cpp
	src/DataManager.cpp
	src/UIManager.cpp
	src/ConfigManager.cpp
)

add_executable(linux_monitor ${SOURCES})

target_link_libraries(linux_monitor PRIVATE pthread)

target_include_directories(linux_monitor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(linux_monitor PRIVATE -Wall -Wextra -pedantic)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


