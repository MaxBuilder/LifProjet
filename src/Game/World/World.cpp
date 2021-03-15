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
    soldier1->setPosition(650, 200);
    mBlueTeam.push_back(soldier1.get());
    mSoldiers.push_back(soldier1.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1));

    std::unique_ptr<Soldier> soldier4 = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts);
    soldier4->setPosition(600, 300);
    mBlueTeam.push_back(soldier4.get());
    mSoldiers.push_back(soldier4.get());
    mSceneLayers[Front]->attachChild(std::move(soldier4));


    std::unique_ptr<Soldier> soldier5 = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts);
    soldier5->setPosition(100, 300);
    mRedTeam.push_back(soldier5.get());
    mSoldiers.push_back(soldier5.get());
    mSceneLayers[Front]->attachChild(std::move(soldier5));


    std::unique_ptr<Soldier> soldier2 = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts);
    soldier2->setPosition(100, 100);
    mRedTeam.push_back(soldier2.get());
    mSoldiers.push_back(soldier2.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2));

    std::unique_ptr<Soldier> soldier3 = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts);
    soldier3->setPosition(200, 300);
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

    auto builds = mMap.getBuildingsIt();
    for (;builds.first != builds.second;builds.first++){
        std::unique_ptr<Building> build = std::make_unique<Building>(builds.first->getID(),(builds.first->getPosition()));
        mBuildings.push_back(build.get());
        mSceneLayers[Back]->attachChild(std::move(build));
    }
}

void World::draw() {
    mTarget.draw(mMap);
    mTarget.draw(mSceneGraph);
    mTarget.draw(zone);

}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);
    updateTargets();
    updateCalls();
    updateBonus();
}

void World::updateCalls() {
    // Calls for help : to modify so if unit is already helped, it doesn't enter the loop (and limit to one entity)
    for(auto & soldier : mRedTeam) {
        if(soldier->getAction() == Soldier::Calling) {
            for(auto & soldier2 : mRedTeam) {
                if(soldier != soldier2 and soldier2->isAvailable) {
                    soldier->helpRequested(soldier2);
                    soldier2->helpAlly(soldier);
                }
            }
        }
    }
}

void World::updateTargets() { // A MODIFIER, GIGA BUGGED
    bool *redHaveTarget = new bool[mRedTeam.size()];
    for(int i(0);i<mRedTeam.size();i++) redHaveTarget[i] = false;
    int countRedTeam = 0;

    for(auto & i : mBlueTeam) {
        bool blueHaveTarget = false;
        float distMin = 100;
        float dist;
        countRedTeam = 0;
        for(auto & j : mRedTeam) {
            dist = distance(i->getPosition(), j->getPosition());
            if( dist < 100 and !j->isDestroyed() ) {
                if( dist < distMin ){
                    i->setTarget(j);
                    distMin = dist;
                }
                blueHaveTarget = true;
                //Soldier * test = mBlueTeam[i];
                //Soldier * test2 = mRedTeam[j];
            }

            if(distance(j->getPosition(), i->getPosition()) < 200 and !i->isDestroyed() and j->isAvailable) {
                j->setTarget(i);
                redHaveTarget[countRedTeam] = true;
            }
            countRedTeam++;
        }
        if (!blueHaveTarget) i->dropTarget();
    }

    for(int i(0);i<mRedTeam.size();i++){
        if (!redHaveTarget[i] and mRedTeam[i]->getTarget() != nullptr){
            if(mRedTeam[i]->getTarget()->getTeam() == Soldier::BlueTeam and mRedTeam[i]->isAvailable) mRedTeam[i]->dropTarget();
        }
    }

}

void World::updateBonus(){
    bool entityHaveBonus = false;
    for (auto &entity : mSoldiers){
        entityHaveBonus = false;
        for (const auto &build : mBuildings ){
            if(entity->getTeam() != build->getTeam()) continue;
            if (distance(build->getPosition(),entity->getPosition()) <= build->getRange()) {
                entity->changeBonus(build->getBonusFlag());
                entityHaveBonus = true;
            }
        }
        if (!entityHaveBonus)
            entity->changeBonus(Entity::None);
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