//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_WORLD_HPP
#define LIFPROJET_WORLD_HPP

#include "../Map/TilesMap.hpp"
#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Audio/SoundPlayer.hpp"
#include "../Entities/Soldier.hpp"

#include <array>

class World {

public:
    World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds);

    void update(sf::Time dt);
    void draw();

    // Tracking functions
    void trackNext();
    void trackPrev();
    void untrack();
    sf::Vector2f trackedPos();

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

    // Scene
    TilesMap mMap;
    SceneNode mSceneGraph;
    std::vector<SceneNode*> mSceneLayers;

    // Entites
    std::vector<Soldier*> mSoldiers;

    // Tracking
    int mTracked;
};


#endif //LIFPROJET_WORLD_HPP
