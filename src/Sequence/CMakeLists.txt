set(zynaddsubfx_sequence_SRCS
    sequence.cpp
    midiclip.cpp
    )

add_library(zynaddsubfx_sequence STATIC
    ${zynaddsubfx_sequence_SRCS}
    )
