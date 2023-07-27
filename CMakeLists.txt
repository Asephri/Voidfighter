# CMakeLists.txt
# Minimum cmake version
cmake_minimum_required(VERSION 3.5)

# Set Generator
set(CMAKE_GENERATOR "Ninja")
message("generator is set to ${CMAKE_GENERATOR}")

# Project name (must match directory name)
project(Voidfighter)

# Locate Pkgconfig for Linux
include(FindPkgConfig)

# Using pkg to find SDL2 and other required libraries
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

# Additional compiler options
add_compile_options(-Wall -Wempty-body -Werror -Wstrict-prototypes -Werror=maybe-uninitialized -Warray-bounds -g)

# Add the include directories of the (found) libraries
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Include everything from src in the file
file(GLOB SOURCES src/defs.h src/structs.h src/*.c src/*.h src/*/*.c src/*/*.h gfx/* sound/* music/*)

# Created application output location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} appicon.rc)

# The c++ standard used
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Link these libraries into the project
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    m
)

# Set WIN32_EXECUTABLE property to hide the console on Windows
#if (WIN32)
#    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
#endif()

# Check if we are on Linux and use the icon as a resource
if (UNIX AND NOT APPLE)
    # Copy the icon file to the build directory
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/icon.ico ${CMAKE_CURRENT_BINARY_DIR}/icon.ico COPYONLY)

    # Add the generated resources.cpp to sources
    #set(RESOURCES_CPP ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
    #add_custom_command(
    #    OUTPUT ${RESOURCES_CPP}
    #    COMMAND rcc -binary ${CMAKE_CURRENT_BINARY_DIR}/icon.ico -o ${RESOURCES_CPP}
    #    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/icon.ico
    #    COMMENT "Generating resources.cpp"
    #)
    
    add_custom_target(${CMAKE_PROJECT_NAME}_resources DEPENDS ${RESOURCES_CPP})
    list(APPEND SOURCES ${RESOURCES_CPP})
    
    # Add the resources target as a dependency for the executable
    add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_resources)
endif()