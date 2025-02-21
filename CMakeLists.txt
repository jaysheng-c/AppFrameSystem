cmake_minimum_required(VERSION 3.27)
project(AppFrameSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(test)


#add_executable(AppFrameSystem main.cpp)

add_custom_target(BUILD_ALL
        DEPENDS
#        AppFrameSystem
)