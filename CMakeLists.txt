cmake_minimum_required(VERSION 4.1)
project(sdl_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_COMPILER g++-13)
#set(CMAKE_C_COMPILER gcc-13)
include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.x
)
FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.4.x
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_MakeAvailable(SDL3 SDL3_image)
add_executable(sdl_test
        src/main.cpp
        src/utils.cpp
        src/GameObject.cpp
        src/RigidBody.cpp
        src/RigidBody.h
        src/sprites.h
        src/generated_sprites.h
        src/Tileset.h
        src/Collider.cpp
        src/Collider.h
        src/PrefabProccessor.cpp
        src/PrefabProccessor.h
)

add_custom_command(TARGET sdl_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/assets $<TARGET_FILE_DIR:sdl_test>/assets
)



target_link_libraries(sdl_test
        PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        nlohmann_json::nlohmann_json
)

add_executable(prefab_generator
        src/PrefabGenerator.cpp
        src/utils.cpp
        src/PrefabProccessor.cpp
        src/PrefabProccessor.h
        src/Collider.cpp
        src/Collider.h
        src/sprites.h
        src/generated_sprites.h
        src/PrefabGenerator.cpp
)
add_custom_command(TARGET prefab_generator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/assets $<TARGET_FILE_DIR:sdl_test>/assets
)
target_link_libraries(prefab_generator
        PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        nlohmann_json::nlohmann_json
)