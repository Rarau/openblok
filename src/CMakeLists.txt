# OpenBlok executable settings

configure_file(version.h.in generated/version.h @ONLY)
include_directories(openblok.elf PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
include_directories(openblok.elf PRIVATE "./common")

add_executable(openblok.elf main.cpp version.h)

add_subdirectory(system)
add_subdirectory(game)
#target_link_libraries(openblok )
target_link_libraries(openblok.elf
    module_game
    debugnet
    SceNetCtl_stub
	SceNet_stub
    SDL2pp
    SDL2
    vita2d
    SceDisplay_stub
    SceCtrl_stub
    SceAudio_stub
    SceSysmodule_stub
    SceGxm_stub
    SceCommonDialog_stub
    SceTouch_stub
    SceHid_stub
    SDL2_mixer
    m
)



# use custom std::make_unique if necessary
if (NOT CXX_HAS_MAKEUNIQUE)
    target_compile_definitions(openblok.elf PRIVATE NEED_MAKEUNIQUE)
endif()


# Install
#install(TARGETS openblok RUNTIME DESTINATION ${EXEDIR})
