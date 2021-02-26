//
// Created by 33771 on 23/02/2021.
//

#include "World.hpp"

World::World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds)
: mTarget(outputTarget)
, mTextures(textures)
, mSounds(sounds)
, mFonts(fonts)
, mSceneLayers()
, mMap(textures.get(Textures::MapGround), 20.f)
//, soldier(Soldier::BlueTeam, mTextures, mFonts)
{
    mMap.load("data/Maps/demo1.map");

    // Construction de la scène
    /*
    for(std::size_t i=0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer(new SceneNode());
        //mSceneLayers[i] = layer.get();
        mSceneLayers.push_back(layer.get());

        mSceneGraph.attachChild(std::move(layer));
    }*/

    // Construction de la scène (à bouger dans une fonction)

    std::unique_ptr<Soldier> soldier = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts);
    soldier->setPosition(100, 100);
    //mSceneLayers[Front]->attachChild(std::move(soldier));
    mSceneGraph.attachChild(std::move(soldier));

}

void World::draw() {
    mTarget.draw(mMap);
    mTarget.draw(mSceneGraph);
    //mTarget.draw(soldier);
}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);
}