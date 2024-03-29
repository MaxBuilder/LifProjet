cmake_minimum_required(VERSION 3.16)
project(LifProjet)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
endif(WIN32)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(LifProjet src/main.cpp
        src/Game/Application/Application.cpp src/Game/Application/Application.hpp
        src/Game/Pathfinding/Pathfinding.hpp src/Game/Pathfinding/Pathfinding.cpp
        src/Game/Pathfinding/PathfindingTile.hpp src/Game/Pathfinding/PathfindingTile.cpp
        src/Core/StateStack/State.cpp src/Core/StateStack/State.hpp
        src/Core/StateStack/StateStack.cpp src/Core/StateStack/StateStack.hpp
        src/Game/Resources/ResourceHolder.hpp src/Game/Resources/ResourceIdentifiers.hpp
        src/Game/GameStates/StateIdentifiers.hpp
        src/Core/Util/Rand.hpp src/Core/Util/Foreach.hpp
        src/Core/Util/Utility.hpp src/Core/Util/Utility.cpp
        src/Game/GameStates/MainMenuState.cpp src/Game/GameStates/MainMenuState.hpp
        src/Game/GameStates/SettingsState.cpp src/Game/GameStates/SettingsState.hpp
        src/Core/GUI/Component.cpp src/Core/GUI/Component.hpp
        src/Core/GUI/Button.cpp src/Core/GUI/Button.hpp
        src/Core/GUI/ButtonTexture.cpp src/Core/GUI/ButtonTexture.hpp
        src/Core/GUI/Container.cpp src/Core/GUI/Container.hpp
        src/Core/GUI/ButtonList.cpp  src/Core/GUI/ButtonList.hpp
        src/Core/GUI/CheckBox.cpp src/Core/GUI/CheckBox.hpp
        src/Core/GUI/ScrollingContainer.cpp src/Core/GUI/ScrollingContainer.hpp
        src/Game/GameStates/MapEditorState.cpp src/Game/GameStates/MapEditorState.hpp
        src/Game/Map/TilesMap.hpp src/Game/Map/TilesMap.cpp
        src/Game/Map/Tile.cpp src/Game/Map/Tile.hpp
        src/Core/Audio/SoundPlayer.cpp src/Core/Audio/SoundPlayer.hpp
        src/Core/Audio/MusicPlayer.cpp src/Core/Audio/MusicPlayer.hpp
        src/Core/Scene/SceneNode.cpp src/Core/Scene/SceneNode.hpp
        src/Game/GameStates/GameState.cpp src/Game/GameStates/GameState.hpp
        src/Game/GameStates/PauseState.cpp src/Game/GameStates/PauseState.hpp
        src/Game/Map/EntityInfo.cpp src/Game/Map/EntityInfo.hpp
        src/Game/Entities/Entity.cpp src/Game/Entities/Entity.hpp
        src/Game/Entities/Soldier.cpp src/Game/Entities/Soldier.hpp
        src/Game/Entities/Projectile.cpp src/Game/Entities/Projectile.hpp
        src/Game/Entities/Building.cpp src/Game/Entities/Building.hpp
        src/Game/World/World.cpp src/Game/World/World.hpp
        src/Core/Commands/Command.cpp src/Core/Commands/Command.hpp
        src/Core/Commands/CommandQueue.cpp src/Core/Commands/CommandQueue.hpp
        src/Core/Util/Utility.cpp src/Core/Util/Debug.hpp)
target_link_libraries(LifProjet sfml-graphics sfml-audio)
