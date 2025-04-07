file(GLOB SourceFiles CONFIGURE_DEPENDS "*.hpp" "*.cpp")

# Enable no console on apple Release
if(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(snek MACOSX_BUNDLE ${SourceFiles})
else()
  add_executable(snek ${SourceFiles})
endif()

# Enable no console on windows Release
if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(snek PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()


add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/../copy_assets.cmake
)
add_dependencies(snek copy_assets)

if(NOT EMSCRIPTEN)
  if(WIN32)
    if(AUTO_DOWNLOAD_DEPS)
      target_link_libraries(snek SDL2::SDL2 SDL2::SDL2main SDL2_ttf)
      add_custom_command(
        TARGET snek POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:snek>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2_ttf>" "$<TARGET_FILE_DIR:snek>"
        VERBATIM
      )
    else()
      target_link_libraries(snek SDL2::SDL2 SDL2::SDL2main  ${SDL2_TTF_LIBRARY})
      add_custom_command(
        TARGET snek POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:snek>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_TTF_DLL}" "$<TARGET_FILE_DIR:snek>"
        VERBATIM
      )
    endif()
    add_custom_command(
      TARGET snek POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
              "${CMAKE_SOURCE_DIR}/assets" 
              "$<TARGET_FILE_DIR:snek>/assets"
      VERBATIM
    )
  else()
    if(AUTO_DOWNLOAD_DEPS)
      target_link_libraries(snek SDL2::SDL2 SDL2::SDL2main SDL2_ttf)
    else()
      target_link_libraries(snek ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARY})
    endif()
  endif()
  
  install(TARGETS snek DESTINATION bin)
endif()