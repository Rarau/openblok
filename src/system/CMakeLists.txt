set(MOD_SYSTEM_SRC
    Color.cpp
    ConfigFile.cpp
    Event.cpp
    InputConfigFile.cpp
    InputMap.cpp
    Log.cpp
    Paths.cpp
    Window.cpp

    # SDL2
    sdl/SDLAudioContext.cpp
    sdl/SDLFont.cpp
    sdl/SDLGraphicsContext.cpp
    sdl/SDLMusic.cpp
    sdl/SDLSoundEffect.cpp
    sdl/SDLTexture.cpp
    sdl/SDLWindow.cpp

    # Vita
    vita/VitaFont.cpp
    vita/VitaGraphicsContext.cpp
    vita/VitaTexture.cpp
    vita/VitaTextureText.cpp
    vita/VitaWindow.cpp
)

set(MOD_SYSTEM_H
    AudioContext.h
    Color.h
    ConfigFile.h
    Event.h
    Font.h
    GraphicsContext.h
    InputMap.h
    InputConfigFile.h
    Localize.h
    Log.h
    Music.h
    Paths.h
    Rectangle.h
    SoundEffect.h
    Texture.h
    Window.h

    # SDL2
    sdl/SDLAudioContext.h
    sdl/SDLFont.h
    sdl/SDLGraphicsContext.h
    sdl/SDLMusic.h
    sdl/SDLSoundEffect.h
    sdl/SDLTexture.h
    sdl/SDLWindow.h

    # Vita
    vita/VitaFont.h
    vita/VitaGraphicsContext.h
    vita/VitaTexture.h
    vita/VitaTextureText.h
    vita/VitaWindow.h

    # Util
    util/MakeUnique.h
    util/Regex.h
)

add_library(module_system ${MOD_SYSTEM_SRC} ${MOD_SYSTEM_H})

# use SDL2
target_include_directories(module_system PRIVATE ${SDL2PP_INCLUDE_DIRS})
target_compile_definitions(module_system PRIVATE OPENBLOK_DATADIR="${DATADIR}")
target_link_libraries(module_system ${SDL2PP_LIBRARIES})
target_link_libraries(module_system
    SceAudio_stub
    SceCtrl_stub
    vita2d
    SceHid_stub
    SceTouch_stub
    jpeg
    png
    SceGxm_stub
    FLAC
    ogg
    freetype
    SDL2
    mpg123
    vorbisfile
    vorbis
    z
    ogg
    SceDisplay_stub
    SceSysmodule_stub
    SceCommonDialog_stub
    crypto
    mikmod
)
# use Boost.Regex if requested
if (USE_BOOST_REGEX)
    target_include_directories(module_system PRIVATE ${Boost_INCLUDE_DIRS})
    target_compile_definitions(module_system PRIVATE USE_BOOST_REGEX)
    target_link_libraries(module_system ${Boost_LIBRARIES})
endif()

option(ENABLE_JPG "Enable JPG image support" ON)
option(ENABLE_MP3 "Enable MP3 music support" ON)
option(ENABLE_MOD "Enable MOD music support" OFF)
option(ENABLE_FLAC "Enable FLAC music support" OFF)

if (ENABLE_JPG)
    target_compile_definitions(module_system PRIVATE SDL2_ENABLE_JPG)
endif()
if (ENABLE_MP3)
    target_compile_definitions(module_system PRIVATE SDL2_ENABLE_MP3)
endif()
if (ENABLE_MOD)
    target_compile_definitions(module_system PRIVATE SDL2_ENABLE_MOD)
endif()
if (ENABLE_FLAC)
    target_compile_definitions(module_system PRIVATE SDL2_ENABLE_FLAC)
endif()

# use custom std::make_unique if necessary
if (NOT CXX_HAS_MAKEUNIQUE)
    target_compile_definitions(module_system PRIVATE NEED_MAKEUNIQUE)
endif()
