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
#include <list>

class World {

public:
    World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds);
    void init(const std::string& mapPath);

    void update(sf::Time dt);
    void switchDisplayDebug();
    bool isEnded() const;
    bool returnVictoryState() const;

    void draw();

    // Tracking functions :
    void trackNext();
    void trackPrev();
    void untrack();
    sf::Vector2f trackedPos();
    std::pair<int, int> getRemaining();

    // Entity functions :
    void createEntity();
    void onCommand();
    void updateMovement(); // Updates entity relative to the environment
    void updateTargets(); // Updates entity target from perspective
    void updateBonus(); // Checks if entities are in range of buildings
    void updateDeath(); // check if an entity is dead

    //aide
    static bool inMap(sf::Vector2f dpl);
    void recBarrier(sf::Vector2i);

private:
    enum Layer {
        Back,
        Front,
        flying,
        LayerCount
    };

private:
    bool mBlueVictory, mRedVictory;
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
    std::list<Projectile*> mArrows;
    int mNbRed, mNbBlue;

    // Pathfinding
    Pathfinding mAstar;

    // Tracking
    int mTracked;

};


#endif //LIFPROJET_WORLD_HPP
