cmake_minimum_required(VERSION 3.10)
project(proj1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE main.cpp vector3.cpp objects.cpp scene.cpp builder.cpp RayTracer.cpp)
add_executable(${PROJECT_NAME} $(SOURCE))
target_link_libraries(${PROJECT_NAME} opencv_core opencv_highgui opencv_imgcodecs)

