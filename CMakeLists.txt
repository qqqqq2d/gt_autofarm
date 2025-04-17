cmake_minimum_required(VERSION 3.10)
project(X11_AutoCycle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(X11 REQUIRED)

add_executable(X11_AutoCycle main.cpp)

target_include_directories(X11_AutoCycle PRIVATE ${X11_INCLUDE_DIR})

target_link_libraries(X11_AutoCycle
        ${X11_LIBRARIES}
        X11 Xext Xrender Xft Xinerama pthread
)
