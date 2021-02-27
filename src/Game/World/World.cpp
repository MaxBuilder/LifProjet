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
, mSoldiers()
, mTracked(0)
, mMap(textures.get(Textures::MapGround), 20.f)
{
    mMap.load("data/Maps/demo1.map");

    // Construction de la scène
    for(std::size_t i = 0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers.push_back(layer.get());

        mSceneGraph.attachChild(std::move(layer));
    }

    // Construction de la scène (à bouger dans une fonction)
    std::unique_ptr<Soldier> soldier1 = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts);
    soldier1->setPosition(100, 100);
    mSoldiers.push_back(soldier1.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1));

    std::unique_ptr<Soldier> soldier2 = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts);
    soldier2->setPosition(100, 200);
    mSoldiers.push_back(soldier2.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2));

    mSoldiers[0]->setDirection(1, 1);
    mSoldiers[1]->setDirection(2, 3);
}

void World::draw() {
    mTarget.draw(mMap);
    mTarget.draw(mSceneGraph);
}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);

}

void World::trackNext() {
    mTracked++;
    if(mTracked > mSoldiers.size() - 1)
        mTracked = 0;
}

void World::trackPrev() {
    mTracked--;
    if(mTracked < 0)
        mTracked = mSoldiers.size() - 1;
}

void World::untrack() {
    mTracked = -1;
}

sf::Vector2f World::trackedPos() {
    return mSoldiers[mTracked]->getPosition();
}

void World::trackedMove(sf::Vector2f direction) {
    mSoldiers[mTracked]->setDirection(direction);
}