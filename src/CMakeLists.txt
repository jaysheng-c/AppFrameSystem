include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(QRC_SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resource/af.qrc)
qt_add_resources(${QRC_SOURCE_FILE})


include(log/logger.cmake)
include(conf/conf.cmake)
add_subdirectory(core)