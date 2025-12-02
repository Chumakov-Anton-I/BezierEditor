cmake_minimum_required(VERSION 3.16)
project(BezierEditor VERSION 0.1.0 LANGUAGES CXX)

set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
add_subdirectory(freeglut)

include_directories(BEFORE ./freeglut/include)

if(MSVC)
    message(FATAL_ERROR "MSVC compiler is not supported! Choose GCC or Clang")
endif()

add_executable(bezier
    src/BezierEditor.cc
    src/Color.h src/Color.cc
    src/Constraint.h src/Constraint.cc
    src/Curve.h src/Curve.cc
    src/Function.h src/Function.cc
    src/Selection.h src/Selection.cc
    src/TextFile.h src/TextFile.cc
    src/Vector.h src/Vector.cc
    src/World.h src/World.cc
)

target_link_libraries(bezier PRIVATE glu32 opengl32 freeglut_static)
target_compile_options(bezier PRIVATE -fpermissive)
