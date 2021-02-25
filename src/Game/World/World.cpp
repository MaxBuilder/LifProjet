//
// Created by 33771 on 23/02/2021.
//

#include "World.hpp"

World::World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds)
: mTarget(outputTarget)
, mSounds(sounds)
, mFonts(fonts)
, mMap(textures.get(Textures::MapGround), 20.f)
{
    mMap.load("data/Maps/demo1.map");

    // Construction de la scène
    for(std::size_t i=0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

}

void World::draw() {
    mTarget.draw(mMap);
}