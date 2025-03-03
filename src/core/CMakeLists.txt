add_library(Core SHARED)

set(CoreHeader
        ${CMAKE_CURRENT_SOURCE_DIR}/global/af_enum.h
)

set(CoreSource
        ${ConfigureSource}
        ${LoggerSource}

        ${CMAKE_CURRENT_SOURCE_DIR}/private/af_frameless_p.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/af_widget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/af_title_widget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/af_status_widget.cpp
)

target_sources(Core PRIVATE ${CoreHeader} ${CoreSource} ${QRC_SOURCE_FILE})
target_link_libraries(Core PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)