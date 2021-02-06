cmake_minimum_required(VERSION 3.17)
project(LifProjet)

set(CMAKE_CXX_STANDARD 17)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(LifProjet src/main.cpp src/Application/Application.cpp src/Application/Application.hpp
        src/StateStack/State.cpp src/StateStack/State.hpp src/StateStack/StateStack.cpp src/StateStack/StateStack.hpp
        src/Resources/ResourceHolder.hpp src/GameStates/StateIdentifiers.hpp src/Resources/ResourceIdentifiers.hpp
        src/Util/Rand.hpp src/Util/Foreach.hpp src/Util/Utility.hpp src/GameStates/MainMenuState.cpp
        src/GameStates/MainMenuState.hpp src/GUI/Component.cpp src/GUI/Component.hpp
        src/GUI/Button.cpp src/GUI/Button.hpp src/Util/Utility.cpp src/GUI/Container.cpp src/GUI/Container.hpp
        src/GameStates/MapEditorState.cpp src/GameStates/MapEditorState.hpp src/Map/TilesMap.hpp src/Map/TilesMap.cpp
        src/Map/Tile.cpp src/Map/Tile.hpp src/Audio/SoundPlayer.cpp src/Audio/SoundPlayer.hpp src/Audio/MusicPlayer.cpp src/Audio/MusicPlayer.hpp)
target_link_libraries(LifProjet sfml-graphics sfml-audio)
