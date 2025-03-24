file(GLOB SourceFiles CONFIGURE_DEPENDS "*.hpp" "*.cpp")
add_executable(snek ${SourceFiles})

if(NOT EMSCRIPTEN)
  if(WIN32)
    target_link_libraries(snek SDL2::SDL2 SDL2::SDL2main)
    add_custom_command(
      TARGET snek POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:snek>"
      VERBATIM
    )
  else()
    target_link_libraries(snek ${SDL2_LIBRARIES})
  endif()
  
  install(TARGETS snek DESTINATION bin)
endif()