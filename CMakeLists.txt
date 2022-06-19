cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Emulator VERSION 0.1)
add_executable(Emulator src/main.cpp src/oputils.cpp src/emulator.cpp)

add_library(main_compiler_flags INTERFACE)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(main_compiler_flags INTERFACE
	"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wformat=2;-Wunused;-ggdb3>"
	"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

target_link_libraries(Emulator main_compiler_flags)



target_include_directories(Emulator PUBLIC "${PROJECT_SOURCE_DIR}/include")
