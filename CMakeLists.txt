cmake_minimum_required(VERSION 3.31)
project(1945Demo LANGUAGES C)

# Found from many sources this MACRO ${CMAKE_SOURCE_DIR} to select 
# directly the Cmake file directory as principal referement(in any position)

# Had to set it to not change my project architecture and put all exe in bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin)#(deafult dir was /bin/Debug)


# =====================================================
# 1 Game CORE sources  (* .c except main.c and test.c)
file(GLOB GAME_ALL_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
)
# remove operation for main.c e test_main.c
list(REMOVE_ITEM GAME_ALL_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/test_main.c
)

# Static library called with all headers inside
add_library(game_core STATIC ${GAME_ALL_SOURCES})
target_include_directories(game_core PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Linked raylib here on game_core library
set(RAYLIB_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(game_core PRIVATE ${RAYLIB_LIB_DIR})

#all raylib dependency source: gemini
target_link_libraries(game_core PRIVATE
    raylibdll#fix
    #raylib
    #winmm
    #gdi32
    #opengl32
)
# =====================================================
# 2 Main executable: 1945_damiano

add_executable(1945_damiano
    ${CMAKE_SOURCE_DIR}/src/main.c
)
target_include_directories(1945_damiano PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_directories(1945_damiano PRIVATE ${RAYLIB_LIB_DIR})
target_link_libraries(1945_damiano PRIVATE
    game_core   # my library with also raylib inside
)

# --- copy raylib.dll in same directory of 1945_damiano ---
add_custom_command(TARGET 1945_damiano POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/lib/raylib.dll"
            "$<TARGET_FILE_DIR:1945_damiano>/raylib.dll"
)

# --- copy resources/ directory in same directory of 1945_damiano ---
add_custom_command(TARGET 1945_damiano POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:1945_damiano>/resources"
)

# =====================================================
# 3 Test executable: test_app

add_executable(test_app
    ${CMAKE_SOURCE_DIR}/src/test_main.c
)
target_include_directories(test_app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_directories(test_app PRIVATE ${RAYLIB_LIB_DIR})
target_link_libraries(test_app PRIVATE
    game_core   # my library with also raylib inside
)

# --- copy of raylib.dll and resources not needed for now cause test_app is in the same directory of 1945_damiano ---