include(${CMAKE_SOURCE_DIR}/src/log/logger.cmake)
include(${CMAKE_SOURCE_DIR}/src/conf/conf.cmake)

add_executable(TestLogger test_logger.cpp ${LoggerSource} ${ConfigureSource})
target_link_libraries(TestLogger
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

#add_executable(TestFrameLess test_frameless.cpp )
#target_link_libraries(TestFrameLess
#        Qt::Core
#        Qt::Gui
#        Qt::Widgets
#        AFCore
#)