set(zynmusicstudio_zms_SRCS
    zynapplication.cpp
    mainwindow.cpp
    channelwindow.cpp
    channelcontainer.cpp
    channelclip.cpp
    instrumentwindow.cpp
    instrumentcontainer.cpp
    instrumentcontrol.cpp
    keyboard.cpp
    kitwindow.cpp
    pianoroll.cpp
    pianorollnote.cpp
    synthdata.cpp
    snappinggraphicsitem.cpp
    snappingcontainer.cpp
    )
set(zynmusicstudio_zms_HEADERS
    mainwindow.h
    channelwindow.h
    channelcontainer.h
    channelclip.h
    instrumentwindow.h
    instrumentcontainer.h
    instrumentcontrol.h
    keyboard.h
    kitwindow.h
    pianoroll.h
    pianorollnote.h
    synthdata.h
    snappinggraphicsitem.h
    snappingcontainer.h
    )
set(zynmusicstudio_zms_FORMS
    mainwindow.ui
    channelwindow.ui
    channelcontainer.ui
    instrumentwindow.ui
    instrumentcontainer.ui
    instrumentcontrol.ui
    keyboard.ui
    kitwindow.ui
    pianoroll.ui
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
# QT library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
qt5_wrap_cpp(zynmusicstudio_zms_HEADERS_MOC ${zynmusicstudio_zms_HEADERS})
qt5_wrap_ui(zynmusicstudio_zms_FORMS_HEADERS ${zynmusicstudio_zms_FORMS})

message(STATUS "bin : ${CMAKE_CURRENT_BINARY_DIR}")

# Include cmake binary directory for all MOC headers
set(LOCAL_INCLUDE_DIRS
    
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    )

set(ZMS_INCLUDE_DIRS ${LOCAL_INCLUDE_DIRS} PARENT_SCOPE)

qt5_wrap_cpp(zynmusicstudio_zms_HEADERS_MOC ${zynmusicstudio_zms_HEADERS})
qt5_wrap_ui(zynmusicstudio_zms_FORMS_HEADERS ${zynmusicstudio_zms_FORMS})

add_library(zynmusicstudio_zms STATIC
    ${zynmusicstudio_zms_SRCS}
    ${zynmusicstudio_zms_HEADERS_MOC}
    ${zynmusicstudio_zms_FORMS_HEADERS}
    )

qt5_use_modules(zynmusicstudio_zms Widgets)
