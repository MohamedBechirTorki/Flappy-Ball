cmake_minimum_required(VERSION 3.10)
project(FlappySound)

set(CMAKE_CXX_STANDARD 17)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# PortAudio
find_path(PORTAUDIO_INCLUDE_DIR NAMES portaudio.h)
find_library(PORTAUDIO_LIBRARY NAMES portaudio)
if(NOT PORTAUDIO_INCLUDE_DIR OR NOT PORTAUDIO_LIBRARY)
    message(FATAL_ERROR "PortAudio not found. Install it or set PORTAUDIO_INCLUDE_DIR/PORTAUDIO_LIBRARY.")
endif()

# Include dirs
include_directories(
    ${SFML_INCLUDE_DIRS}
    ${PORTAUDIO_INCLUDE_DIR}
)

add_executable(FlappySound
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Pipe.cpp
    src/SoundInput.cpp
)

target_link_libraries(FlappySound
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    ${PORTAUDIO_LIBRARY}
)
