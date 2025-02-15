cmake_minimum_required(VERSION 3.28)
project(QubitTango LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(QubitTango src/main.cpp)
target_compile_features(QubitTango PRIVATE cxx_std_17)
target_link_libraries(QubitTango PRIVATE SFML::Graphics)
