set(CMAKE_CXX_FLAGS "-O3")
file(GLOB sources src/*.cpp)
add_executable(a.out ${sources})