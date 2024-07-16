cmake_minimum_required(VERSION 3.28)
project(Brick_Breaker C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Brick_Breaker main.c)

# Variables storing SDL Framework locations
set(SDL2
        /Library/Frameworks/SDL2.framework)

set(SDL2_image
        /Library/Frameworks/SDL2_image.framework)

set(SDL2_ttf
        /Library/Frameworks/SDL2_ttf.framework)

set(SDL2_mixer
        /Library/Frameworks/SDL2_mixer.framework)

target_link_libraries(
        Brick_Breaker PRIVATE
        ${SDL2}/Versions/A/SDL2
        ${SDL2_image}/Versions/A/SDL2_image
        ${SDL2_ttf}/Versions/A/SDL2_ttf
        ${SDL2_mixer}/Versions/A/SDL2_mixer
)

target_include_directories(
        Brick_Breaker PRIVATE
        ${SDL2}/Versions/A/Headers
        ${SDL2_image}/Versions/A/Headers
        ${SDL2_ttf}/Versions/A/Headers
        ${SDL2_mixer}/Versions/A/Headers
)
