//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_WORLD_HPP
#define LIFPROJET_WORLD_HPP

#include "../Map/TilesMap.hpp"
#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Audio/SoundPlayer.hpp"
#include "../../Core/Audio/MusicPlayer.hpp"
#include "../../Core/Commands/CommandQueue.hpp"
#include "../../Core/Util/Debug.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../Entities/Soldier.hpp"
#include "../Entities/Building.hpp"

#include <array>
#include <list>

class World {

public:
    struct SimulationData { // Possible d'ajouter plus de détails
        int nbRedSoldierBegin, nbBlueSoldierBegin;
        int nbRedSoldierEnd, nbBlueSoldierEnd;
        int nbRedBuildingBegin, nbBlueBuildingBegin;
        int nbRedBuildingEnd, nbBlueBuildingEnd;
        bool mBlueVictory, mRedVictory;

        SimulationData();
        bool isEnded() const;
    };

public:
    World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds, MusicPlayer& music);
    void init(const std::string& mapPath);

    void update(sf::Time dt);
    void draw();

    void switchDisplayDebug();
    SimulationData& getSimData();

    // Tracking functions :
    void trackNext();
    void trackPrev();
    void untrack();
    sf::Vector2f trackedPos();

    // Entity functions :
    void createEntity();
    void onCommand();
    void updateMovement(); // Updates entity relative to the environment
    void updateTargets(); // Updates entity target from perspective
    void updateBonus(); // Checks if entities are in range of buildings
    void updateDeath(); // check if an entity is dead

    // Util :
    static bool inMap(sf::Vector2f dpl);
    void recBarrier(sf::Vector2i);

private:
    enum Layer {
        Back,
        Front,
        Air,
        LayerCount
    };

private:
    sf::RenderTarget& mTarget;
    TextureHolder& mTextures;
    FontHolder& mFonts;
    SoundPlayer& mSounds;
    MusicPlayer& mMusic;
    CommandQueue mCommandQueue;

    // Current scene data
    SimulationData mSimData;

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

    // Pathfinding
    Pathfinding mAstar;

    // Tracking
    int mTracked;

};


#endif //LIFPROJET_WORLD_HPP
