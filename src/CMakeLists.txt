message(STATUS "Checking Include Path" $ENV{CMAKE_INCLUDE_PATH} ${CMAKE_INCLUDE_PATH})
message(STATUS "Checking Library Path" $ENV{CMAKE_LIBRARY_PATH} ${CMAKE_LIBRARY_PATH})

find_package(FFTW REQUIRED)
find_package(MXML REQUIRED)
find_package(PTHREAD REQUIRED)
find_package(Alsa)
find_package(PORTAUDIO)
set(FLTK_SKIP_OPENGL true)
find_package(FLTK)

set(GuiModule fltk CACHE STRING "GUI module, either fltk, ntk or off")
set(AlsaEnable ${ALSA_FOUND} CACHE BOOL
    "Enable support for Advanced Linux Sound Architecture")
set(JackEnable ${JACK_FOUND} CACHE BOOL
    "Enable support for JACK Audio Connection toolKit")
set(PaEnable ${PORTAUDIO_FOUND} CACHE BOOL
    "Enable support for Port Audio System")
set(LashEnable ${LASH_FOUND} CACHE BOOL
    "Enable LASH Audio Session Handler")
set(DssiEnable ${DSSI_FOUND} CACHE BOOL
    "Enable DSSI Plugin compilation")
set(LibloEnable ${LIBLO_FOUND} CACHE BOOL
    "Enable Liblo")

add_definitions(-DVERSION="${VERSION}")

message(STATUS "Building on a '${CMAKE_SYSTEM_NAME}' System")

if(JackEnable)
    set(DefaultOutput jack CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
    if(AlsaEnable)
        set(DefaultInput alsa CACHE STRING
            "Default Input module: [null, alsa, jack, pa]")
    else()
        set(DefaultInput jack CACHE STRING
            "Default Input module: [null, alsa, jack, pa]")
    endif()
elseif(AlsaEnable)
    set(DefaultOutput alsa CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
    set(DefaultInput alsa CACHE STRING
        "Default Input module: [null, alsa, jack, pa]")
elseif(PaEnable)
    set(DefaultOutput pa CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
else()
    set(DefaultOutput null CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
    set(DefaultInput null CACHE STRING
        "Default Input module: [null, alsa, jack, pa]")
endif()



if(GuiModule STREQUAL ntk AND NTK_FOUND)
    set(NtkGui TRUE)
elseif(GuiModule STREQUAL fltk AND FLTK_FOUND)
    set(FltkGui TRUE)
elseif(GuiModule STREQUAL off)
    add_definitions(-DDISABLE_GUI)
else()
    set(GuiModule off CACHE STRING "GUI module, either fltk, qt or off")
    add_definitions(-DDISABLE_GUI)
    message(STATUS "GUI module defaulting to off")
endif()


if(MSVC)
    if(CMAKE_CL_64)
        add_definitions(-D_AMD64_)
    endif(CMAKE_CL_64)
    add_definitions(-D__WINDOWS_MM__)
    set(OS_LIBRARIES "winmm.lib")
else(MSVC)
    option(BuildForAMD_X86_64 "Build for AMD x86_64 system" OFF)
    option(BuildForCore2_X86_64 "Build for Intel Core2 x86_64 system" OFF)
    option(BuildForDebug "Include gdb debugging support" OFF)

    set(CMAKE_BUILD_TYPE "Release")

    set(BuildOptions_x86_64AMD
        "-O3 -march=athlon64 -m64 -Wall -ffast-math -fno-finite-math-only -fomit-frame-pointer"
        CACHE STRING "X86_64 compiler options"
    )

    set(BuildOptions_X86_64Core2
        "-O3 -march=core2 -m64 -Wall -ffast-math -fno-finite-math-only -fomit-frame-pointer"
        CACHE STRING "X86_64 compiler options"
    )

    set(BuildOptionsBasic
        "-O3 -msse -msse2 -mfpmath=sse -ffast-math -fomit-frame-pointer"
        CACHE STRING "basic X86 complier options"
    )

    set(BuildOptionsDebug
        "-O0 -g3 -ggdb -Wall -Wpointer-arith" CACHE STRING "Debug build flags")
endif(MSVC)

if(AlsaEnable)
    list(APPEND AUDIO_LIBRARIES ${ASOUND_LIBRARY})
    list(APPEND AUDIO_LIBRARY_DIRS ${ASOUND_LIBRARY_DIRS})
    add_definitions(-DALSA=1)
    message(STATUS "Compiling with alsa")
endif(AlsaEnable)

if(JackEnable)
    list(APPEND AUDIO_LIBRARIES ${JACK_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${JACK_LIBRARY_DIRS})
    add_definitions(-DJACK=1)
    message(STATUS "Compiling with jack")
endif(JackEnable)

if(PaEnable)
    include_directories(${PORTAUDIO_INCLUDE_DIR})
    list(APPEND AUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${PORTAUDIO_LIBRARY_DIRS})
    add_definitions(-DPORTAUDIO=1)
    message(STATUS "Compiling with portaudio")
endif(PaEnable)

if(LashEnable)
    include_directories(${LASH_INCLUDE_DIRS})
    list(APPEND AUDIO_LIBRARIES ${LASH_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${LASH_LIBRARY_DIRS})
    add_definitions(-DLASH=1)
    message(STATUS "Compiling with lash")
endif(LashEnable)

if(LibloEnable)
    include_directories(${LIBLO_INCLUDE_DIRS})
    list(APPEND AUDIO_LIBRARIES ${LIBLO_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${LIBLO_LIBRARY_DIRS})
    add_definitions(-DUSE_NSM=1)
    message(STATUS "Compiling with liblo")
endif(LibloEnable)

include_directories(
    ${MXML_INCLUDE_DIR}
    )

if(MSVC)
else(MSVC)
    add_definitions(
         -g #TODO #todo put in a better location
         -Wall
         -Wextra
         )
    if(NOT AVOID_ASM)
        message(STATUS "Compiling with x86 opcode support")
        add_definitions(-DASM_F2I_YES)
    endif()

    if(BuildForDebug)
        set(CMAKE_BUILD_TYPE "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG ${BuildOptionsDebug})
        message(STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_CXX_FLAGS_DEBUG}")
    else(BuildForDebug)
        set(CMAKE_BUILD_TYPE "Release")
        if(BuildForAMD_X86_64)
            set(CMAKE_CXX_FLAGS_RELEASE ${BuildOptions_x86_64AMD})
        else(BuildForAMD_X86_64)
            if(BuildForCore2_X86_64)
                set(CMAKE_CXX_FLAGS_RELEASE ${BuildOptions_X86_64Core2})
            else(BuildForCore2_X86_64)
                set(CMAKE_CXX_FLAGS_RELEASE ${BuildOptionsBasic})
            endif(BuildForCore2_X86_64)
        endif(BuildForAMD_X86_64)
        message(STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_CXX_FLAGS_RELEASE}")
    endif(BuildForDebug)

    add_definitions(-fPIC)

endif(MSVC)

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
    find_program(FLTK_CONFIG fltk-config)
    if(FLTK_CONFIG)
        execute_process(COMMAND ${FLTK_CONFIG} --use-images --ldflags OUTPUT_VARIABLE FLTK_LDFLAGS)
        string(STRIP ${FLTK_LDFLAGS} FLTK_LIBRARIES)
    endif()

    message(STATUS ${FLTK_LDFLAGS})

    set(GUI_LIBRARIES ${FLTK_LIBRARIES} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES} zynaddsubfx_gui)

    add_definitions(-DFLTK_GUI)
    message(STATUS "Will build FLTK gui")

    include_directories(
        ${FLTK_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/UI"
        "${CMAKE_CURRENT_BINARY_DIR}/UI"
        )

    add_subdirectory(UI)
endif()

if(NtkGui)
    find_program(FLTK_FLUID_EXECUTABLE ntk-fluid)

    message(STATUS ${NTK_LDFLAGS} ${NTK_IMAGES_LDFLAGS})

    set(GUI_LIBRARIES ${NTK_LIBRARIES} ${NTK_IMAGES_LIBRARIES} ${OPENGL_LIBRARIES} zynaddsubfx_gui)

    add_definitions(-DNTK_GUI)
    message(STATUS "Will build NTK gui")

    include_directories(
        ${NTK_INCLUDE_DIRS}
        "${CMAKE_CURRENT_SOURCE_DIR}/UI"
        "${CMAKE_CURRENT_BINARY_DIR}/UI"
        )

    add_subdirectory(UI)
endif()

link_directories(
    ${AUDIO_LIBRARY_DIRS}
    ${FFTW_LIBRARY_DIRS}
    ${MXML_LIBRARY_DIRS}
    ${FLTK_LIBRARY_DIRS}
    ${NTK_LIBRARY_DIRS}
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

set(NONGUI_LIBRARIES
    zynaddsubfx_misc
    zynaddsubfx_synth
    zynaddsubfx_effect
    zynaddsubfx_params
    zynaddsubfx_dsp
    zynaddsubfx_nio
    )


add_subdirectory(Misc)
add_subdirectory(Synth)
add_subdirectory(Effects)
add_subdirectory(Params)
add_subdirectory(DSP)
add_subdirectory(Nio)
add_subdirectory(QUI)

add_library(zynaddsubfx_core STATIC
    ${zynaddsubfx_dsp_SRCS}
    ${zynaddsubfx_effect_SRCS}
    ${zynaddsubfx_misc_SRCS}
    ${zynaddsubfx_params_SRCS}
    ${zynaddsubfx_synth_SRCS}
    )

target_link_libraries(zynaddsubfx_core
    ${FFTW_LIBRARIES}
    ${MXML_LIBRARIES}
    ${OS_LIBRARIES}
    ${PTHREAD_LIBRARIES}
    )

add_executable(zynaddsubfx zynaddsubfx.cpp)

target_link_libraries(zynaddsubfx
    zynaddsubfx_core
    zynaddsubfx_nio
    ${GUI_LIBRARIES}
    ${NIO_LIBRARIES}
    ${AUDIO_LIBRARIES}
    )

if(DssiEnable)
    add_library(zynaddsubfx_dssi SHARED
        Output/DSSIaudiooutput.cpp
        )

    target_link_libraries(zynaddsubfx_dssi
        zynaddsubfx_core
        ${OS_LIBRARIES}
        )
    if(${CMAKE_SIZEOF_VOID_P} EQUAL "8")
        install(TARGETS zynaddsubfx_dssi LIBRARY DESTINATION lib64/dssi/)
    else()
        install(TARGETS zynaddsubfx_dssi LIBRARY DESTINATION lib/dssi/)
    endif()
endif()

add_executable(zynmusicstudio zynmusicstudio.cpp)

target_link_libraries(zynmusicstudio
    zynaddsubfx_core
    zynaddsubfx_nio
    zynmusicstudio_qui
    ${NIO_LIBRARIES}
    ${AUDIO_LIBRARIES}
    )

qt5_use_modules(zynmusicstudio Widgets)
