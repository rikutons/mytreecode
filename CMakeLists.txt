set(CMAKE_CXX_FLAGS "-O3")
file(GLOB sources src/*.cpp src/simulators/*.cpp)
add_executable(a.out ${sources})