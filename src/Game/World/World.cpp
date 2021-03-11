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

    // Scene building based on 2 plans (back and front)
    for(std::size_t i = 0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers.push_back(layer.get());

        mSceneGraph.attachChild(std::move(layer));
    }

    // Adding entities (to move in separate function)
    std::unique_ptr<Soldier> soldier1 = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts, true);
    soldier1->setPosition(600, 100);
    mBlueTeam.push_back(soldier1.get());
    mSoldiers.push_back(soldier1.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1));
    /*
    std::unique_ptr<Soldier> soldier4 = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts);
    soldier4->setPosition(600, 400);
    mBlueTeam.push_back(soldier4.get());
    mSoldiers.push_back(soldier4.get());
    mSceneLayers[Front]->attachChild(std::move(soldier4));
     */

    std::unique_ptr<Soldier> soldier2 = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts);
    soldier2->setPosition(100, 100);
    mRedTeam.push_back(soldier2.get());
    mSoldiers.push_back(soldier2.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2));

    std::unique_ptr<Soldier> soldier3 = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts);
    soldier3->setPosition(100, 300);
    mRedTeam.push_back(soldier3.get());
    mSoldiers.push_back(soldier3.get());
    mSceneLayers[Front]->attachChild(std::move(soldier3));

    for(auto &soldier : mSoldiers) // Init of defense positions
        soldier->init();

    // Debug
    zone.setRadius(50);
    zone.setPosition( mBlueTeam[0]->getOrigin() - sf::Vector2f(50, 50));
    zone.setOutlineColor(sf::Color::Cyan);
    zone.setOutlineThickness(1);
    zone.setFillColor(sf::Color::Transparent);
    //mBlueTeam[0]->setAction(Soldier::None);
}

void World::draw() {
    mTarget.draw(mMap);
    mTarget.draw(mSceneGraph);
    mTarget.draw(zone);
}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);
    updateTargets();
}

void World::updateTargets() { // A MODIFIER, GIGA BUGGED
    for(int i = 0 ; i < mBlueTeam.size() ; i++) {
        for(int j = 0 ; j < mRedTeam.size() ; j++) {
            if(distance(mBlueTeam[i]->getPosition(), mRedTeam[j]->getPosition()) < 100 and !mRedTeam[j]->isDestroyed()) {
                mBlueTeam[i]->setTarget(mRedTeam[j]);
                //Soldier * test = mBlueTeam[i];
                //Soldier * test2 = mRedTeam[j];
            }
            else mBlueTeam[i]->setTarget(nullptr);

            if(distance(mRedTeam[j]->getPosition(), mBlueTeam[i]->getPosition()) < 200 and !mBlueTeam[i]->isDestroyed() and mRedTeam[j]->isAvailable) {
                mRedTeam[j]->setTarget(mBlueTeam[i]);
            }
            else if(mRedTeam[j]->isAvailable) mRedTeam[j]->setTarget(nullptr);
        }
    }

    // Calls for help (to put in separate function)
    for(auto & soldier : mRedTeam) {
        if(soldier->getAction() == Soldier::Calling) {
            for(auto & soldier2 : mRedTeam) {
                if(soldier != soldier2 and soldier->isAvailable and soldier2->isAvailable) {
                    soldier->helpRequested(soldier2);
                    soldier2->helpAlly(soldier);
                }
            }
        }
    }

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
    if(mTracked != -1) {
        mSoldiers[mTracked]->setAction(Soldier::Action::Moving);
        if(mSoldiers[mTracked]->getTeam() == Soldier::BlueTeam) mSoldiers[mTracked]->resetTravelledDistance();
    }
}