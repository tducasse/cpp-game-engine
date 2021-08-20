cmake_minimum_required(VERSION 3.18) 

project(twng)
set(CMAKE_CXX_STANDARD 17)

set(APP_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(EXTERNALS_DIR "${PROJECT_SOURCE_DIR}/externals")
set(LUA_DIR "${EXTERNALS_DIR}/lua")
set(SDL_DIR "${EXTERNALS_DIR}/sdl")
set(SOL_DIR "${EXTERNALS_DIR}/sol")

add_library (
  lua STATIC
  ${LUA_DIR}/onelua.c 
)
target_compile_definitions(lua PRIVATE MAKE_LIB)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(${SDL_DIR})


file(GLOB sources ${APP_SOURCE_DIR}/*.cpp)
add_executable(twng ${sources})

target_link_libraries(
  twng
  PRIVATE lua
  SDL2main
  SDL2-static
)

target_include_directories(
  twng
  PRIVATE ${EXTERNALS_DIR}
  PRIVATE ${LUA_DIR}
)