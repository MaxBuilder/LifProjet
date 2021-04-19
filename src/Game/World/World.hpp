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

/**
 * @class World
 * @brief Stores all the information on the simulation (entities, map, ...)
 */
class World {
public:
    /**
     * @struct SimulationData
     * @brief Stores all the data on the teams (number of entities, kills, buildings)
     */
    struct SimulationData { // Possible d'ajouter plus de détails
        int nbRedSoldierBegin, nbBlueSoldierBegin;
        int nbRedSoldierEnd, nbBlueSoldierEnd;
        int nbRedBuildingBegin, nbBlueBuildingBegin;
        int nbRedBuildingEnd, nbBlueBuildingEnd;
        bool mBlueVictory, mRedVictory;

        /**
         * @brief Initializes all values to 0
         */
        SimulationData();
        /**
         * @brief Returns if any of the teams has won
         * @return true if an army won
         */
        bool isEnded() const;
    };

public:
    /**
     * @brief Parameter constructor
     * @param outputTarget Window
     * @param textures Texture holder
     * @param fonts Font holder
     * @param sounds SoundPlayer
     * @param music MusicPlayer
     */
    World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds, MusicPlayer& music);
    /**
     * @brief Initializes the simulation from a file
     * @param mapPath Path to the save file containing all the data
     */
    void init(const std::string& mapPath);
    /**
     * @brief Ends the simulation by putting all entities in standby
     */
    void end();

    /**
     * @brief Updates the world
     * @param dt Time interval since last update
     */
    void update(sf::Time dt);
    /**
     * @brief Draws the world
     */
    void draw();

    /**
     * @brief Switches the display types for all the entities
     */
    void switchDisplayDebug();
    /**
     * @brief return the simulation data
     * @return SimulationData
     */
    SimulationData& getSimData();

    // Tracking functions :

    /**
     * @brief Tracks the next soldier
     */
    void trackNext();
    /**
     * Tracks the previous soldier
     */
    void trackPrev();
    /**
     * @brief Stops tracking
     */
    void untrack();
    /**
     * @brief Returns the position of the tracked entity
     * @return Position of the tracked entity
     */
    sf::Vector2f trackedPos();

    // Entity functions :

    /**
     * @brief Creates the entities from the save file
     */
    void createEntity();

    /**
     * @brief Processes the commands from the command queue
     */
    void onCommand();
    /**
     * @brief Moves the entities relative to the environment (speed, collision, ...)
     */
    void updateMovement();
    /**
     * @brief Updates the targets of entities from their perception
     */
    void updateTargets();
    /**
     * @brief Updates the bonuses givent by the buildings
     */
    void updateBonus();
    /**
     * @brief Checks for dead entities and updates their textures and stats
     */
    void updateDeath(); // check if an entity is dead

    // Util :

    /**
     * @brief Return if an entity is within the map bounds
     * @param dpl Future postiton of the entity
     * @return true if entity is in bounds
     */
    static bool inMap(sf::Vector2f dpl);
    /**
     * @brief Helper for the barries building type
     */
    void recBarrier(sf::Vector2i);

private:
    /**
     * @enum Layer
     * @brief Specifies the different layers of the scene
     */
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
    bool ended;

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
