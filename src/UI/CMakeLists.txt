set(FLTK_SKIP_OPENGL true)
find_package(FLTK)

set(GuiModule fltk CACHE STRING "GUI module, either fltk, ntk or off")

if(GuiModule STREQUAL ntk AND NTK_FOUND)
    set(NtkGui TRUE)
elseif(GuiModule STREQUAL fltk AND FLTK_FOUND)
    set(FltkGui TRUE)
elseif(GuiModule STREQUAL off)
    set(LOCAL_DEFINITIONS "-DDISABLE_GUI")
else()
    set(GuiModule off CACHE STRING "GUI module, either fltk, qt or off")
    set(LOCAL_DEFINITIONS "-DDISABLE_GUI")
    message(STATUS "GUI module defaulting to off")
endif()

if(FLTK_FOUND)
    mark_as_advanced(FORCE FLTK_BASE_LIBRARY)
    mark_as_advanced(FORCE FLTK_CONFIG_SCRIPT)
    mark_as_advanced(FORCE FLTK_DIR)
    mark_as_advanced(FORCE FLTK_FLUID_EXECUTABLE)
    mark_as_advanced(FORCE FLTK_FORMS_LIBRARY)
    mark_as_advanced(FORCE FLTK_GL_LIBRARY)
    mark_as_advanced(FORCE FLTK_IMAGES_LIBRARY)
    mark_as_advanced(FORCE FLTK_INCLUDE_DIR)
    mark_as_advanced(FORCE FLTK_MATH_LIBRARY)
endif(FLTK_FOUND)

if(NTK_FOUND)
    mark_as_advanced(FORCE NTK_BASE_LIBRARY)
    mark_as_advanced(FORCE NTK_CONFIG_SCRIPT)
    mark_as_advanced(FORCE NTK_DIR)
    mark_as_advanced(FORCE FLTK_FLUID_EXECUTABLE)
    mark_as_advanced(FORCE NTK_FORMS_LIBRARY)
    mark_as_advanced(FORCE NTK_GL_LIBRARY)
    mark_as_advanced(FORCE NTK_IMAGES_LIBRARY)
    mark_as_advanced(FORCE NTK_INCLUDE_DIR)
    mark_as_advanced(FORCE NTK_MATH_LIBRARY)
endif(NTK_FOUND)

if(FltkGui)
    message(STATUS "Will build FLTK gui")
    find_program(FLTK_CONFIG fltk-config)
    if(FLTK_CONFIG)
        execute_process(COMMAND ${FLTK_CONFIG} --use-images --ldflags OUTPUT_VARIABLE FLTK_LDFLAGS)
        string(STRIP ${FLTK_LDFLAGS} FLTK_LIBRARIES)
    endif()

    set(LOCAL_LIBRARIES ${FLTK_LIBRARIES} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES} zynaddsubfx_gui)
    set(LOCAL_DEFINITIONS "-DFLTK_GUI")
    set(LOCAL_INCLUDE_DIRS
        ${FLTK_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        )
endif(FltkGui)

if(NtkGui)
    message(STATUS "Will build NTK gui")
    find_program(FLTK_FLUID_EXECUTABLE ntk-fluid)

    set(LOCAL_LIBRARIES ${NTK_LIBRARIES} ${NTK_IMAGES_LIBRARIES} ${OPENGL_LIBRARIES} zynaddsubfx_gui)
    set(LOCAL_DEFINITIONS "-DNTK_GUI")
    set(LOCAL_INCLUDE_DIRS
        ${NTK_INCLUDE_DIRS}
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        )
endif(NtkGui)

set(UI_INCLUDE_DIRS ${LOCAL_INCLUDE_DIRS} PARENT_SCOPE)
include_directories(${LOCAL_INCLUDE_DIRS})

set(UI_DEFINITIONS ${LOCAL_DEFINITIONS} PARENT_SCOPE)
add_definitions(${LOCAL_DEFINITIONS})

set(UI_fl_files
    ADnoteUI.fl
    BankUI.fl
    ConfigUI.fl
    EffUI.fl
    EnvelopeUI.fl
    FilterUI.fl
    LFOUI.fl
    MasterUI.fl
    MicrotonalUI.fl
    OscilGenUI.fl
    PADnoteUI.fl
    PartUI.fl
    PresetsUI.fl
    ResonanceUI.fl
    SUBnoteUI.fl
    VirKeyboard.fl
)

set_source_files_properties(UI/MasterUI.h PROPERTIES GENERATED 1)
fltk_wrap_ui(zynaddsubfx_gui ${UI_fl_files})

if(LibloEnable)
    set(zynaddsubfx_gui_FLTK_UI_SRCS ${zynaddsubfx_gui_FLTK_UI_SRCS} NSM.C NSM/Client.C)
endif()

add_library(zynaddsubfx_gui STATIC
    ${UI_objs}
    ${zynaddsubfx_gui_FLTK_UI_SRCS}
    NioUI.cpp
    WidgetPDial.cpp
    )

set(UI_LIBRARIES ${LOCAL_LIBRARIES} PARENT_SCOPE)
target_link_libraries(zynaddsubfx_gui ${LOCAL_LIBRARIES})
