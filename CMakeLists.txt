cmake_minimum_required(VERSION 3.20)

project(VoVision LANGUAGES CXX)

find_package(OpenCV REQUIRED)

add_executable(vovision
    src/main.cpp
)

target_compile_features(vovision PRIVATE cxx_std_20)

target_include_directories(vovision PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(vovision PRIVATE
    ${OpenCV_LIBS}
)