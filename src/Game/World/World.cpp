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
, mTracked(-1)
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
    mBlueTeam.push_back(soldier1.get());
    mSoldiers.push_back(soldier1.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1));

    std::unique_ptr<Soldier> soldier2 = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts);
    soldier2->setPosition(100, 200);
    mRedTeam.push_back(soldier2.get());
    mSoldiers.push_back(soldier2.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2));

    mBlueTeam[0]->setDirection(1, 1);
    mRedTeam[0]->setDirection(2, 3);
}

void World::draw() {
    mTarget.draw(mMap);
    mTarget.draw(mSceneGraph);
}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);
    updateTargets();
}

void World::updateTargets() {
    for(int i = 0 ; i < mBlueTeam.size() ; i++) {
        for(int j = 0 ; j < mRedTeam.size() ; j++) {
            if(distance(mBlueTeam[i]->getPosition(), mRedTeam[j]->getPosition()) < 100 and !mRedTeam[j]->isDestroyed()) {
                mBlueTeam[i]->setTarget(mRedTeam[j]);
                //std::cout << "Target acquired !" << std::endl;
            }
            else mBlueTeam[i]->setTarget(nullptr);
        }

    }

    /*
    display(mSoldiers[0]->getPosition()); std::cout << " "; display(mSoldiers[1]->getPosition()); std::cout << std::endl;
    if(distance(mSoldiers[0]->getPosition(), mSoldiers[1]->getPosition()) < 100) {
        mSoldiers[0]->setTarget(mSoldiers[1]);
        std::cout << "Target acquired !" << std::endl;
    }

    if(mSoldiers[0]->getTarget() != nullptr) {
        std::cout << " Target "; display(mSoldiers[0]->getTarget()->getPosition());  std::cout << std::endl;
    }
     */
}

void World::trackNext() {
    trackedReset();
    mTracked++;
    if(mTracked > mSoldiers.size() - 1)
        mTracked = 0;
    mSoldiers[mTracked]->setAction(Soldier::Action::Override);
}

void World::trackPrev() {
    trackedReset();
    mTracked--;
    if(mTracked < 0)
        mTracked = mSoldiers.size() - 1;
    mSoldiers[mTracked]->setAction(Soldier::Action::Override);
}

void World::untrack() {
    trackedReset();
    mTracked = -1;
}

sf::Vector2f World::trackedPos() {
    return mSoldiers[mTracked]->getPosition();
}

void World::trackedMove(sf::Vector2f direction) {
    mSoldiers[mTracked]->setDirection(direction);
}

void World::trackedReset() {
    if(mTracked != -1)
        mSoldiers[mTracked]->setAction(Soldier::Action::Move);
}