file(GLOB SourceFiles CONFIGURE_DEPENDS "*.hpp" "*.cpp")
add_executable(snek ${SourceFiles})

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/../copy_assets.cmake
)
add_dependencies(snek copy_assets)

if(NOT EMSCRIPTEN)
  if(WIN32)
    target_link_libraries(snek SDL2::SDL2 SDL2::SDL2main  ${SDL2_TTF_LIBRARY})
    add_custom_command(
      TARGET snek POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:snek>"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_TTF_DLL}" "$<TARGET_FILE_DIR:snek>"
      VERBATIM
    )
  else()
    target_link_libraries(snek ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARY})
  endif()
  
  install(TARGETS snek DESTINATION bin)
endif()