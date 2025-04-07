cmake_minimum_required(VERSION 3.14)
project(snek VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(SNEK_ALGORITHM)
  add_compile_definitions(SNEK_ALGORITHM)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  
  set(EMSCRIPTEN_FLAGS 
    "-s USE_SDL=2"
    "-s USE_SDL_TTF=2"
    "--bind"
    "--use-preload-plugins"
    "--preload-file ${CMAKE_SOURCE_DIR}/assets@/assets"
    "--shell-file ${CMAKE_SOURCE_DIR}/web/snek_shell.html"
  )
  
  string(REPLACE ";" " " EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_FLAGS}")
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_LINK_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_LINK_FLAGS}")
else()
  if(AUTO_DOWNLOAD_DEPS)
    include(FetchContent)
    FetchContent_Declare(
      SDL2
      GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
      GIT_TAG release-2.28.3
    )

    FetchContent_MakeAvailable(SDL2)

    FetchContent_Declare(
      SDL2_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-2.24.0
    )

    set(SDL2TTF_VENDORED ON CACHE BOOL "Use bundled FreeType library")

    FetchContent_MakeAvailable(SDL2_ttf)
    set(SDL2_TTF_INCLUDE_DIR "${sdl2_ttf_SOURCE_DIR}/include")
  else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
  endif()


  include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR})
endif()

add_subdirectory(src)
