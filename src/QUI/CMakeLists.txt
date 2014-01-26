set(zynmusicstudio_qui_SRCS
    mainwindow.cpp
    partwindow.cpp
    partcontainer.cpp
    )
set(zynmusicstudio_qui_HEADERS
    mainwindow.h
    partwindow.h
    partcontainer.h
    )
set(zynmusicstudio_qui_FORMS
    mainwindow.ui
    partwindow.ui
    partcontainer.ui
    )

# QT library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
qt5_wrap_cpp(zynmusicstudio_gui_HEADERS_MOC ${zynmusicstudio_gui_HEADERS})
qt5_wrap_ui(zynmusicstudio_gui_FORMS_HEADERS ${zynmusicstudio_gui_FORMS})

# Include cmake binary directory for all MOC headers
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("${CMAKE_CURRENT_BINARY_DIR}/Gui")
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_cpp(zynmusicstudio_qui_HEADERS_MOC ${zynmusicstudio_qui_HEADERS})
qt5_wrap_ui(zynmusicstudio_qui_FORMS_HEADERS ${zynmusicstudio_qui_FORMS})

add_library(zynmusicstudio_qui STATIC
    ${zynmusicstudio_qui_SRCS}
    ${zynmusicstudio_qui_HEADERS_MOC}
    ${zynmusicstudio_qui_FORMS_HEADERS}
    )
