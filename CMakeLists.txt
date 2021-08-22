cmake_minimum_required(VERSION 3.18) 

project(twng)

set(CMAKE_CXX_STANDARD 17)

set(APP_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(EXTERNALS_DIR "${PROJECT_SOURCE_DIR}/externals")
set(LUA_DIR "${EXTERNALS_DIR}/lua")
set(SDL_DIR "${EXTERNALS_DIR}/sdl")
set(SOL_DIR "${EXTERNALS_DIR}/sol")

if(EMSCRIPTEN)

 message(STATUS "Building for web")

 else()

  message(STATUS "Building for desktop")

  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  add_subdirectory(${SDL_DIR})

endif()

add_library (
  lua STATIC
  ${LUA_DIR}/onelua.c 
)
target_compile_definitions(lua PRIVATE MAKE_LIB)


add_executable(
  twng
  ${APP_SOURCE_DIR}/main.cpp
  ${APP_SOURCE_DIR}/game/game.cpp
)

target_include_directories(
  twng
  PRIVATE ${EXTERNALS_DIR}
  PRIVATE ${LUA_DIR}
)


if(EMSCRIPTEN) 

 add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_LINK COMMAND ${CMAKE_COMMAND}
    -E create_symlink
    ${PROJECT_SOURCE_DIR}/lua
    ${PROJECT_BINARY_DIR}/lua
  )

  target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    lua
    "-s USE_SDL=2"
    "--preload-file lua/"
  )

   set(CMAKE_EXECUTABLE_SUFFIX ".html")

else()

  target_link_libraries(
    ${PROJECT_NAME}
	PRIVATE lua
	SDL2main
	SDL2-static
  )

  add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_LINK COMMAND ${CMAKE_COMMAND}
    -E create_symlink
    ${PROJECT_SOURCE_DIR}/lua
    ${PROJECT_BINARY_DIR}/lua
  )

endif()

