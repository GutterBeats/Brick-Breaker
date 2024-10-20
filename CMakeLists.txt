cmake_minimum_required(VERSION 3.28)
project(Brick_Breaker LANGUAGES C VERSION 0.5.0)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Brick_Breaker
        src/main.c
        include/BrickManager.h
        src/Gameplay/BrickManager.c
        include/Entity.h
        src/Gameplay/Entity.c
        include/Game.h
        src/Gameplay/Game.c
        src/Scenes/Ending.c
        src/Scenes/Gameplay.c
        src/Scenes/Menu.c
        src/Scenes/Options.c
        src/Scenes/Title.c
        include/Audio.h
        src/Utilities/Audio.c
        include/ButtonContainer.h
        src/Utilities/ButtonContainer.c
        include/Collision.h
        src/Utilities/Collision.c
        include/Keyboard.h
        src/Utilities/Keyboard.c
        include/Renderer.h
        src/Utilities/Renderer.c
        include/Resources.h
        include/Text.h
        src/Utilities/Text.c
        include/Types.h
        include/Utils.h
        src/Utilities/Utils.c
        include/EventSystem.h
        src/Utilities/EventSystem.c
        src/Scenes/Pause.c
)

set(SDL2MIXER_VENDORED ON)
set(SDL2TTF_VENDORED ON)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_ttf)

if (WIN32)
    target_link_libraries(
            Brick_Breaker PRIVATE SDL2main
    )
endif ()

target_link_libraries(
        Brick_Breaker PRIVATE
        SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
)

target_include_directories(
        Brick_Breaker PRIVATE
        external/SDL/include
        external/SDL_image/include
        external/SDL_mixer/include
        external/SDL_ttf
)

add_custom_command(
        TARGET Brick_Breaker POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_custom_command(
        TARGET Brick_Breaker POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE:SDL2_image>
        $<TARGET_FILE:SDL2_ttf>
        $<TARGET_FILE:SDL2_mixer>
        $<TARGET_FILE_DIR:Brick_Breaker>
        VERBATIM
)
