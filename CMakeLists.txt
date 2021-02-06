cmake_minimum_required(VERSION 3.17)
project(LifProjet)

set(CMAKE_CXX_STANDARD 17)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(LifProjet src/main.cpp src/Game/Application/Application.cpp src/Game/Application/Application.hpp
        src/Core/StateStack/State.cpp src/Core/StateStack/State.hpp src/Core/StateStack/StateStack.cpp src/Core/StateStack/StateStack.hpp
        src/Game/Resources/ResourceHolder.hpp src/Game/GameStates/StateIdentifiers.hpp src/Game/Resources/ResourceIdentifiers.hpp
        src/Core/Util/Rand.hpp src/Core/Util/Foreach.hpp src/Core/Util/Utility.hpp src/Game/GameStates/MainMenuState.cpp
        src/Game/GameStates/MainMenuState.hpp src/Core/GUI/Component.cpp src/Core/GUI/Component.hpp
        src/Core/GUI/Button.cpp src/Core/GUI/Button.hpp src/Core/Util/Utility.cpp src/Core/GUI/Container.cpp src/Core/GUI/Container.hpp
        src/Game/GameStates/MapEditorState.cpp src/Game/GameStates/MapEditorState.hpp src/Game/Map/TilesMap.hpp src/Game/Map/TilesMap.cpp
        src/Game/Map/Tile.cpp src/Game/Map/Tile.hpp src/Core/Audio/SoundPlayer.cpp src/Core/Audio/SoundPlayer.hpp src/Core/Audio/MusicPlayer.cpp src/Core/Audio/MusicPlayer.hpp)
target_link_libraries(LifProjet sfml-graphics sfml-audio)
