file(GLOB SourceFiles CONFIGURE_DEPENDS "*.hpp" "*.cpp")
add_executable(snek ${SourceFiles})
target_link_libraries(snek ${SDL2_LIBRARIES})
install(TARGETS snek DESTINATION bin)