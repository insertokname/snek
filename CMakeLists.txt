cmake_minimum_required(VERSION 3.14)
project(snek VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  
  set(EMSCRIPTEN_FLAGS 
    "-s USE_SDL=2"
    "--bind"
    "--use-preload-plugins"
    "--shell-file ${CMAKE_SOURCE_DIR}/web/snek_shell.html"
  )
  
  string(REPLACE ";" " " EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_FLAGS}")
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_LINK_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_LINK_FLAGS}")
else()
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIRS})
endif()

add_subdirectory(src)
