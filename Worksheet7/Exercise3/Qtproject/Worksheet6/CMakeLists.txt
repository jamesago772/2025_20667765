cmake_minimum_required(VERSION 3.16)

project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# ---- VTK ----
find_package(VTK REQUIRED)

# ---- Sources ----
set(APP_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    
)

# ---- Target ----
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(untitled
        ${APP_SOURCES}
    )
else()
    add_executable(untitled
        ${APP_SOURCES}
    )
endif()

# ---- Link libraries ----
target_link_libraries(untitled
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ${VTK_LIBRARIES}
)

# If you don't want console window, you can turn this ON later once everything links.
# For now keep it OFF to avoid "missing main" issues.
set_target_properties(untitled PROPERTIES
    WIN32_EXECUTABLE FALSE
)

include(GNUInstallDirs)
install(TARGETS untitled
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- Copy OpenVR bindings into build folder ----
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/vrbindings
    ${CMAKE_BINARY_DIR}/
)