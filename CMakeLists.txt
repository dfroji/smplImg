
cmake_minimum_required(VERSION 3.5)

project(smplfilter
  LANGUAGES CXX
  )

include(FetchContent)

FetchContent_Declare(fpng
  GIT_REPOSITORY https://github.com/richgel999/fpng
  GIT_TAG origin/main
  )

FetchContent_GetProperties(fpng)
if(NOT fpng_POPULATED)
  FetchContent_Populate(fpng)
  add_subdirectory(${fpng_SOURCE_DIR} ${fpng_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(fpng)

add_executable(smplfilter 
    src/main.cpp 
    src/image.cpp 
    ${fpng_SOURCE_DIR}/src/lodepng.cpp
    )
target_include_directories(smplfilter PRIVATE src)
include_directories(${fpng_SOURCE_DIR}/src)
