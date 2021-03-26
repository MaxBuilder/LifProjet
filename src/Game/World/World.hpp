//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_WORLD_HPP
#define LIFPROJET_WORLD_HPP

#include "../Map/TilesMap.hpp"
#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Audio/SoundPlayer.hpp"
#include "../../Core/Commands/CommandQueue.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../Entities/Soldier.hpp"
#include "../Entities/Building.hpp"


#include <array>

class World {

public:
    World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds);

    void update(sf::Time dt);
    void draw();

    // Tracking functions :
    void trackNext();
    void trackPrev();
    void untrack();
    sf::Vector2f trackedPos();
    void trackedMove(sf::Vector2f direction);
    void trackedReset();

    // Entity functions :
    void onCommand();
    void updateMovement(); // Updates entity relative to the environment
    void updateTargets(); // Updates entity target from perspective
    void updateCalls(); // Updates entities calling for help
    void updateBonus(); // Checks if entities are in range of buildings

    //aide
    bool inMap(sf::Vector2f dpl);

private:
    enum Layer {
        Front,
        Back,
        LayerCount
    };

private:
    sf::RenderTarget& mTarget;
    TextureHolder& mTextures;
    FontHolder& mFonts;
    SoundPlayer& mSounds;
    CommandQueue mCommandQueue;

    // Scene
    TilesMap mMap;
    SceneNode mSceneGraph;
    std::vector<SceneNode*> mSceneLayers;

    // Entites (multiple references to ease access)
    std::vector<Soldier*> mBlueTeam;
    std::vector<Soldier*> mRedTeam;
    std::vector<Soldier*> mSoldiers;
    std::vector<Building*> mBuildings;

    // Pathfinding
    AstarAlgo mAstar;

    // Tracking
    int mTracked;

    // Debug
    sf::CircleShape zone;
};


#endif //LIFPROJET_WORLD_HPP
