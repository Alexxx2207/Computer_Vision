cmake_minimum_required(VERSION 3.22.1)
project(Convolution)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    main.cpp
    src/Contracts/Mask.cpp
    src/ConvolutionKernels/BoxBlur.cpp
    src/ConvolutionKernels/GaussianBlur.cpp
    src/ConvolutionKernels/Sharpening.cpp
    src/ConvolutionKernels/EdgeDetection.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)

