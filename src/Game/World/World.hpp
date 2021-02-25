//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_WORLD_HPP
#define LIFPROJET_WORLD_HPP

#include "../Map/TilesMap.hpp"
#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Audio/SoundPlayer.hpp"

class World {

public:
    World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds);

    void update(sf::Time dt);
    void draw();

private:
    enum Layer {
        Front,
        Back,
        LayerCount
    };

private:
    sf::RenderTarget& mTarget;
    FontHolder& mFonts;
    SoundPlayer& mSounds;

    // Scene
    TilesMap mMap;
    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;


};


#endif //LIFPROJET_WORLD_HPP
