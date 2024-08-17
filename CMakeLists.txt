cmake_minimum_required(VERSION 3.28)
project(Brick_Breaker LANGUAGES C VERSION 0.5.0)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Brick_Breaker
        src/main.c
        src/Keyboard.c
        src/Gameplay.c
        src/Renderer.c
        src/Title.c
        src/Ending.c
        src/Entity.c
        src/Utils.c
        src/Game.c
        src/BrickManager.c
        src/Text.c
        src/Audio.c
        src/Collision.c
        src/GameOver.c
        src/Menu.c
        src/Options.c
        src/ButtonContainer.c
)

add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(Brick_Breaker CopyResources)

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
