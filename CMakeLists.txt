cmake_minimum_required(VERSION 3.9)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


#cmake_policy(SET CMP0069 NEW)
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)


project(openblok)



include("${VITASDK}/share/vita.cmake" REQUIRED)


set(VITA_APP_NAME "openblok")
set(VITA_TITLEID  "VSDK00012")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -lto -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(EXEDIR "." CACHE STRING "Install location of the runtime executable")
set(DATADIR "./data" CACHE STRING "Install location of the data files")

# Add any additional library paths here
# ${CMAKE_CURRENT_BINARY_DIR} lets you use any library currently being built
link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Dependencies: SDL2
if(CMAKE_CXX_STANDARD EQUAL 14)
    set(SDL2PP_CXXSTD "c++14" CACHE STRING "libSDL2pp C++ standard")
else()
    set(SDL2PP_CXXSTD "c++11" CACHE STRING "libSDL2pp C++ standard")
endif()
#set(SDL2PP_CXXSTD "c++11" CACHE STRING "libSDL2pp C++ standard")
set(SDL2PP_WITH_IMAGE ON)
set(SDL2PP_WITH_MIXER ON)
set(SDL2PP_WITH_TTF ON)
add_subdirectory(external/libSDL2pp)

# Dependencies: tinydir
include_directories(external/tinydir)
add_subdirectory(thirdparty)

# The main game source
include_directories(src)
add_subdirectory(src)
if(CMAKE_BUILD_TYPE STREQUAL "debug" AND BUILD_TESTS)
    add_subdirectory(external/unittest-cpp)
    include_directories(external/unittest-cpp)
    add_subdirectory(tests)
endif()

# Package
#include(CPack)


vita_create_self(${PROJECT_NAME}.self src/openblok.elf UNCOMPRESSED)
# vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
# vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

  FILE data/fonts/helsinki.ttf data/fonts/helsinki.ttf 										
  FILE data/fonts/PTC75F.ttf data/fonts/PTC75F.ttf
  FILE data/fonts/PTN57F.ttf data/fonts/PTN57F.ttf 
  FILE data/fonts/PTN77F.ttf data/fonts/PTN77F.ttf
  FILE data/fonts/PTS55F.ttf data/fonts/PTS55F.ttf
  FILE data/fonts/PTS75F.ttf data/fonts/PTS75F.ttf
  FILE data/fonts/PTS76F.ttf data/fonts/PTS76F.ttf
  
  FILE data/gamecontrollerdb data/gamecontrollerdb
  FILE data/gamepad-connect.png data/gamepad-connect.png
  FILE data/icon.png data/icon.png
  FILE data/icon0.png sce_sys/icon0.png
  FILE data/startup.png sce_sys/livearea/contents/startup.png
  FILE data/bg.png sce_sys/livearea/contents/bg.png
  FILE data/template.xml sce_sys/livearea/contents/template.xml

  FILE data/themes/default/graphics/game_fill.png data/themes/default/graphics/game_fill.png
  FILE data/themes/default/graphics/garbage.png data/themes/default/graphics/garbage.png
  FILE data/themes/default/graphics/ghost.png data/themes/default/graphics/ghost.png
  FILE data/themes/default/graphics/logo.png data/themes/default/graphics/logo.png
  FILE data/themes/default/graphics/matrix.png data/themes/default/graphics/matrix.png
  FILE data/themes/default/graphics/menu_fill.png data/themes/default/graphics/menu_fill.png
  FILE data/themes/default/graphics/mino.png data/themes/default/graphics/mino.png
  FILE data/themes/default/graphics/well/narrow.png data/themes/default/graphics/well/narrow.png
  FILE data/themes/default/graphics/well/narrow_battle.png data/themes/default/graphics/well/narrow_battle.png
  FILE data/themes/default/graphics/well/wide.png data/themes/default/graphics/well/wide.png
  FILE data/themes/default/graphics/well/wide_battle.png data/themes/default/graphics/well/wide_battle.png
  
  FILE data/themes/default/music/credits.txt data/themes/default/music/credits.txt
  FILE data/themes/default/music/gameplay/gameplay.ogg data/themes/default/music/gameplay/gameplay.ogg
  FILE data/themes/default/music/menu/menu.ogg data/themes/default/music/menu/menu.ogg
  
  FILE data/themes/default/sfx/countdown1.ogg data/themes/default/sfx/countdown1.ogg
  FILE data/themes/default/sfx/countdown2.ogg data/themes/default/sfx/countdown2.ogg
  FILE data/themes/default/sfx/countdown3.ogg data/themes/default/sfx/countdown3.ogg
  FILE data/themes/default/sfx/finish.ogg data/themes/default/sfx/finish.ogg
  FILE data/themes/default/sfx/gameover.ogg data/themes/default/sfx/gameover.ogg 
  FILE data/themes/default/sfx/garbage.ogg data/themes/default/sfx/garbage.ogg
  FILE data/themes/default/sfx/hold.ogg data/themes/default/sfx/hold.ogg
  FILE data/themes/default/sfx/levelup.ogg data/themes/default/sfx/levelup.ogg
  FILE data/themes/default/sfx/lineclear1.ogg data/themes/default/sfx/lineclear1.ogg
  FILE data/themes/default/sfx/lineclear2.ogg data/themes/default/sfx/lineclear2.ogg 
  FILE data/themes/default/sfx/lineclear3.ogg data/themes/default/sfx/lineclear3.ogg
  FILE data/themes/default/sfx/lineclear4.ogg data/themes/default/sfx/lineclear4.ogg
  FILE data/themes/default/sfx/lock.ogg data/themes/default/sfx/lock.ogg
  FILE data/themes/default/sfx/rotate.ogg data/themes/default/sfx/rotate.ogg
  
  FILE data/themes/default/theme.cfg data/themes/default/theme.cfg
)

add_custom_target(copy
  COMMAND cp openblok.self E:/app/${VITA_TITLEID}/eboot.bin
  DEPENDS openblok.self
)

add_custom_target(copyToEmu
  COMMAND cp openblok.self C:/Users/Nevak/AppData/Roaming/Vita3K/Vita3K/ux0/app/${VITA_TITLEID}/eboot.bin
  DEPENDS openblok.self
)

add_custom_target(send
  COMMAND curl -T openblok.self ftp://192.168.1.64:1337/ux0:/app/${VITA_TITLEID}/eboot.bin
  DEPENDS openblok.self
)