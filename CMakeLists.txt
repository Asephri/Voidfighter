# CMakeLists.txt
cmake_minimum_required(VERSION 3.5)

set(CMAKE_GENERATOR "Ninja")
message("generator is set to ${CMAKE_GENERATOR}")

project(Voidfighter)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

add_compile_options(-Wall -Wempty-body -Werror -Wstrict-prototypes -Werror=maybe-uninitialized -Warray-bounds -g)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

file(GLOB SOURCES src/defs.h src/structs.h src/*.c src/*.h src/*/*.c src/*/*.h gfx/* sound/* music/*)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} appicon.rc)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    m
)

if (WIN32)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if (UNIX AND NOT APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/icon.ico ${CMAKE_CURRENT_BINARY_DIR}/icon.ico COPYONLY)

    #set(RESOURCES_CPP ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
    #add_custom_command(
    #    OUTPUT ${RESOURCES_CPP}
    #    COMMAND rcc -binary ${CMAKE_CURRENT_BINARY_DIR}/icon.ico -o ${RESOURCES_CPP}
    #    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/icon.ico
    #    COMMENT "Generating resources.cpp"
    #)
    
    add_custom_target(${CMAKE_PROJECT_NAME}_resources DEPENDS ${RESOURCES_CPP})
    list(APPEND SOURCES ${RESOURCES_CPP})
    
    add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_resources)
endif()
