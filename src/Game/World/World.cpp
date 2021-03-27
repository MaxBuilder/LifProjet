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
, mCommandQueue()
{
    mMap.load("data/Maps/demo1.map");
    //mMap.load("data/Maps/demo2.map");
    //mMap.load("data/Maps/demo3.map");

    mAstar.setMap(mMap);

    // Scene building based on 2 plans (back and front)
    for(std::size_t i = 0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers.push_back(layer.get());

        mSceneGraph.attachChild(std::move(layer));
    }

    // Adding entities (to move in separate function) and to serialize
    /*
    std::unique_ptr<Soldier> soldier1b = std::make_unique<Soldier>(Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue, true);
    soldier1b->setPosition(300, 40);
    mBlueTeam.push_back(soldier1b.get());
    mSoldiers.push_back(soldier1b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1b));

*/
    std::unique_ptr<Soldier> soldier1r = std::make_unique<Soldier>(0, Soldier::RedTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier1r->setPosition(60, 40);
    mRedTeam.push_back(soldier1r.get());
    mSoldiers.push_back(soldier1r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1r));

    std::unique_ptr<Soldier> soldier2r = std::make_unique<Soldier>(1, Soldier::RedTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier2r->setPosition(60, 120);
    mRedTeam.push_back(soldier2r.get());
    mSoldiers.push_back(soldier2r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2r));

    std::unique_ptr<Soldier> soldier3r = std::make_unique<Soldier>(2, Soldier::RedTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier3r->setPosition(60, 240);
    mRedTeam.push_back(soldier3r.get());
    mSoldiers.push_back(soldier3r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier3r));
/*
    std::unique_ptr<Soldier> soldier4r = std::make_unique<Soldier>(3, Soldier::RedTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier4r->setPosition(300, 60);
    mRedTeam.push_back(soldier4r.get());
    mSoldiers.push_back(soldier4r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier4r));
*/


    std::unique_ptr<Soldier> soldier1b = std::make_unique<Soldier>(0, Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier1b->setPosition(1111, 20);
    mBlueTeam.push_back(soldier1b.get());
    mSoldiers.push_back(soldier1b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1b));

    std::unique_ptr<Soldier> soldier2b = std::make_unique<Soldier>(1, Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier2b->setPosition(1100, 70);
    mBlueTeam.push_back(soldier2b.get());
    mSoldiers.push_back(soldier2b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2b));

    std::unique_ptr<Soldier> soldier3b = std::make_unique<Soldier>(2, Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier3b->setPosition(1130, 120);
    mBlueTeam.push_back(soldier3b.get());
    mSoldiers.push_back(soldier3b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier3b));

    std::unique_ptr<Soldier> soldier4b = std::make_unique<Soldier>(3, Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier4b->setPosition(1190, 150);
    mBlueTeam.push_back(soldier4b.get());
    mSoldiers.push_back(soldier4b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier4b));

    std::unique_ptr<Soldier> soldier5b = std::make_unique<Soldier>(4, Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier5b->setPosition(1230, 200);
    mBlueTeam.push_back(soldier5b.get());
    mSoldiers.push_back(soldier5b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier5b));

    std::unique_ptr<Soldier> soldier6b = std::make_unique<Soldier>(5, Soldier::BlueTeam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier6b->setPosition(1120, 150);
    mBlueTeam.push_back(soldier6b.get());
    mSoldiers.push_back(soldier6b.get());
    mSceneLayers[Front]->attachChild(std::move(soldier6b));
/*
    std::unique_ptr<Soldier> soldier1r = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts,mAstar);
    soldier1r->setPosition(88, 551);
    mRedTeam.push_back(soldier1r.get());
    mSoldiers.push_back(soldier1r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier1r));


    std::unique_ptr<Soldier> soldier2r = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts,mAstar);
    soldier2r->setPosition(169, 553);
    mRedTeam.push_back(soldier2r.get());
    mSoldiers.push_back(soldier2r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier2r));

    std::unique_ptr<Soldier> soldier3r = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts,mAstar);
    soldier3r->setPosition(235, 564);
    mRedTeam.push_back(soldier3r.get());
    mSoldiers.push_back(soldier3r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier3r));

    std::unique_ptr<Soldier> soldier4r = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts,mAstar);
    soldier4r->setPosition(340, 564);
    mRedTeam.push_back(soldier4r.get());
    mSoldiers.push_back(soldier4r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier4r));

    std::unique_ptr<Soldier> soldier5r = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts,mAstar);
    soldier5r->setPosition(391, 600);
    mRedTeam.push_back(soldier5r.get());
    mSoldiers.push_back(soldier5r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier5r));

    std::unique_ptr<Soldier> soldier6r = std::make_unique<Soldier>(Soldier::RedTeam, mTextures, mFonts,mAstar);
    soldier6r->setPosition(407, 590);
    mRedTeam.push_back(soldier6r.get());
    mSoldiers.push_back(soldier6r.get());
    mSceneLayers[Front]->attachChild(std::move(soldier6r));
*/

    for(auto &soldier : mSoldiers) // Init of defense positions
        soldier->init();

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

}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);
    updateMovement();
    updateTargets();
    updateBonus();
    onCommand();

    //std::cout << mRedTeam[0]->getAction() << " " << mRedTeam[1]->getAction() << " " << mRedTeam[2]->getAction() << std::endl;
}

void World::onCommand() {
    while(!mCommandQueue.isEmpty()) {
        Command command = mCommandQueue.pop();

        switch(command.mType) {
            case CommandType::MakeTeam:
                for(auto &red : mRedTeam) {
                    if(red != mRedTeam[command.mSender] and distance(red->getPosition(), mRedTeam[command.mSender]->getPosition()) < 250) {
                        red->createTeam(mRedTeam[command.mSender]->getId());
                        mRedTeam[command.mSender]->nbRequested++;
                    }
                }
                std::cout << mRedTeam[command.mSender]->getId() << " Demande de mise en groupe : " << mRedTeam[command.mSender]->nbRequested << std::endl;
                break;

            case CommandType::TeamAccept:
                std::cout << "Accepted " << command.mSender << std::endl;
                // Nommer receiver chef + associer les unités et modifier le comportement
                mRedTeam[command.mReceiver]->nbResponse++;
                mRedTeam[command.mReceiver]->mSquadSize++;
                mRedTeam[command.mReceiver]->mSquadIds.push_back(command.mSender);
                mRedTeam[command.mSender]->setAction(Soldier::WithSquad);
                mRedTeam[command.mSender]->setLeader(mRedTeam[command.mReceiver]);
                break;

            case CommandType::TeamDeny:
                std::cout << "Declined " << command.mSender << std::endl;
                mRedTeam[command.mReceiver]->nbResponse++;
                break;

            case CommandType::InPosition:
                std::cout << command.mSender << " " << command.mReceiver << " In position" << std::endl;
                mRedTeam[command.mReceiver]->nbResponse++;
                std::cout << mRedTeam[command.mReceiver]->nbResponse << std::endl;
                break;

            case CommandType::Assault:
                std::cout << "Assault ordred " << command.mReceiver << std::endl;
                mRedTeam[command.mReceiver]->setAction(Soldier::Assaulting);
                break;
        }
    }
}

void World::updateTargets() {
    // Cibles des attaquants
    for(auto &red : mRedTeam) {
        red->mTargetInSight = 0;
        red->mAllyInSight = 0;
        bool gotAssigned = false;
        float distMin = 99999999.0;

        for(auto &blue : mBlueTeam) {
            float dist = distance(red->getPosition(), blue->getPosition());
            if(dist < 150 and !blue->isDestroyed()) { // In sight
                red->mTargetInSight++;
                if(dist < distMin and dist < 100) {
                    red->setTarget(blue);
                    distMin = dist;
                    gotAssigned = true;
                }
            }

        }
        if(!gotAssigned)
            red->setTarget(nullptr);

        for(auto &ally : mRedTeam) { // Pertinent (on sait jamais)
            if (ally != red and distance(ally->getPosition(), red->getPosition()) < 150)
                red->mAllyInSight++;
        }
    }

    // Cibles des défenseurs
    for(auto &blue : mBlueTeam) {
        bool gotAssigned = false;
        float distMin = 99999999.0;

        for(auto &red : mRedTeam) {
            float dist = distance(red->getPosition(), blue->getPosition());
            if(dist < distMin and dist < 80 and !red->isDestroyed()) {
                blue->setTarget(red);
                distMin = dist;
                gotAssigned = true;
            }
        }
        if(!gotAssigned)
            blue->setTarget(nullptr);
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

void World::updateMovement(){
    for (auto entity : mSoldiers){
        sf::Vector2f tmpVeloce = entity->getVelocity();
        entity->setVelocity(entity->getVelocity()*mMap.getTile(entity->getPosition()/20.f).getMoveSpeed());
        sf::Vector2f point = (entity->getPosition()+entity->getVelocity());

        for (auto other : mSoldiers){
            if (other == entity) continue;
            if(distance(other->getPosition(), entity->getPosition())<10) {
                sf::Vector2f diff = (entity->getPosition()-other->getPosition())/norm(entity->getPosition() - other->getPosition());
                entity->setVelocity(entity->getVelocity() + diff * 0.5f);
                // break;
            }
        }

        if(entity->getDirection().x > 0)
            point.x += 5.f;
        else if(entity->getDirection().x < 0)
            point.x -= 5.f;

        if(entity->getDirection().y > 0)
            point.y += 10.f;

        sf::Vector2i pos = sf::Vector2i(point.x/20, point.y/20);
        if (mMap.getTile(pos.x,pos.y).isCrossable() and inMap(point))
            entity->travel();
        else{
            sf::Vector2i pos1 = sf::Vector2i(point.x/20, (point.y-entity->getVelocity().y)/20);
            sf::Vector2i pos2 = sf::Vector2i((point.x-entity->getVelocity().x)/20, point.y/20);
            if (mMap.getTile(pos1.x,pos1.y).isCrossable() and inMap(point))
                entity->move(entity->getVelocity().x,0);
            else if(mMap.getTile(pos2.x,pos2.y).isCrossable() and inMap(point))
                entity->move(0,entity->getVelocity().y);
        }
        entity->setVelocity(tmpVeloce);
    }
}

bool World::inMap(sf::Vector2f dpl){
    return (dpl.x > 0 and dpl.x < 1280 and dpl.y > 0 and dpl.y < 720);
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