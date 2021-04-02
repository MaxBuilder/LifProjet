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
    // Map initialization
    mMap.load("data/Maps/demo1.map");
    // mMap.load("data/Maps/demo2.map");
    std::cout<<"map loaded\n";

    // Setting up pathfinding
    mAstar.setMap(mMap);

    // Scene building based on 2 plans (back and front)
    for(std::size_t i = 0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers.push_back(layer.get());

        mSceneGraph.attachChild(std::move(layer));
    }

    // Adding entities to the scene
    auto e = mMap.getEntitiesIt();
    int idr = 0;
    int idb = 0;
    int id;

    for (; e.first != e.second ; e.first++) {
        if(e.first->getTeam() == Editor::Tool::BlueTeam) {
            id = idb;
            idb++;
        }
        else {
            id = idr;
            idr++;
        }
        createEntity(e.first->getPosition(), e.first->getTeam(), e.first->getType(), id);
    }

    // Initialisation of defender (to expand)
    for(auto &soldier : mSoldiers)
        soldier->init();

    // Adding buildings to the scene
    auto builds = mMap.getBuildingsIt();
    for (;builds.first != builds.second;builds.first++){
        std::cout<<builds.first->getID()<<" pos : "<<builds.first->getPosition().left<<"/"<<builds.first->getPosition().top<<std::endl;
        std::unique_ptr<Building> build = std::make_unique<Building>(builds.first->getID(),builds.first->getPosition(),mCommandQueue);
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
                std::cout << command.mSender << " " << command.mReceiver << " In position";
                mRedTeam[command.mReceiver]->nbInPlace++;
                std::cout << " nb in position : " << mRedTeam[command.mReceiver]->nbInPlace << std::endl;
                break;

            case CommandType::Assault:
                std::cout << "Assault ordred " << command.mReceiver << std::endl;
                mRedTeam[command.mReceiver]->setAction(Soldier::Assaulting);
                break;

            case CommandType::Dead :
                std::cout<<"Entity died\n";
                updateDeath();
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
                    red->setTarget(static_cast<Entity*>(blue));
                    distMin = dist;
                    gotAssigned = true;
                }
            }
        }

        if(!gotAssigned) {
            distMin = 99999999.0;
            for (auto &build : mBuildings){
                float dist = distance(red->getPosition(), build->getPosition());
                if(dist < 150 and !build->isDestroyed() and build->getTeam() == Entity::BlueTeam) { // In sight
                    if(dist < distMin and dist < 100) {
                        red->setTarget(static_cast<Entity*>(build));
                        distMin = dist;
                        gotAssigned = true;
                    }
                }
            }
            if (!gotAssigned)
                red->setTarget(nullptr);
        }

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
                blue->setTarget(static_cast<Entity*>(red));
                distMin = dist;
                gotAssigned = true;
            }
        }
        if(!gotAssigned)
            blue->setTarget(nullptr);
    }
}

void World::updateBonus(){
    bool entityHaveBonus;
    for (auto &entity : mSoldiers){

        if (entity->isDestroyed()){
            entity->changeBonus(Entity::None);
            continue;
        }

        entityHaveBonus = false;
        for (const auto &build : mBuildings ){
            if(entity->getTeam() != build->getTeam() or build->isDestroyed()) continue;
            if (distance(build->getPosition(),entity->getPosition()) <= build->getRange()) {
                entity->changeBonus(build->getBonusFlag());
                entityHaveBonus = true;
            }
        }
        if (!entityHaveBonus)
            entity->changeBonus(Entity::None);
    }
}

void World::updateMovement() {
    for (auto entity : mSoldiers){
        if(entity->isDestroyed()) continue;
        Editor::Tool team;
        if(entity->getTeam() == Entity::BlueTeam)
            team = Editor::Tool::BlueTeam;
        else
            team = Editor::Tool::RedTeam;
        sf::Vector2f tmpVeloce = entity->getVelocity();
        entity->setVelocity(entity->getVelocity()*mMap.getTile(entity->getPosition()/20.f).getMoveSpeed());
        sf::Vector2f point = (entity->getPosition()+entity->getVelocity());

        for (auto other : mSoldiers){
            if (other == entity or other->isDestroyed() ) continue;
            if(distance(other->getPosition(), entity->getPosition())<10 ) {
                sf::Vector2f diff = (entity->getPosition()-other->getPosition())/norm(entity->getPosition() - other->getPosition());
                entity->setVelocity(entity->getVelocity() + diff * 0.1f);
                // break;
            }
        }

        if(entity->getVelocity().x > 0)
            point.x += 5.f;
        else if(entity->getVelocity().x < 0)
            point.x -= 5.f;

        if(entity->getVelocity().y > 0)
            point.y += 10.f;

        sf::Vector2i pos = sf::Vector2i(point.x/20, point.y/20);
        if (mMap.getTile(pos).isCrossable(team) and inMap(point))
            entity->travel();
        else{
            sf::Vector2i pos1 = sf::Vector2i(point.x/20, entity->getPosition().y/20);
            sf::Vector2i pos2 = sf::Vector2i((entity->getPosition().x)/20, point.y/20);
            if (mMap.getTile(pos1).isCrossable(team) and inMap(point))
                entity->move(entity->getVelocity().x,0);
            else if(mMap.getTile(pos2).isCrossable(team) and inMap(point))
                entity->move(0,entity->getVelocity().y);
        }
        entity->setVelocity(tmpVeloce);
    }
}

void World::updateDeath(){

    for (auto *e : mSoldiers){
        if (e->isDestroyed() and not e->down) {
            mSceneLayers[Back]->attachChild(mSceneLayers[Front]->detachChild(static_cast<SceneNode *>(e)));
            e->down = true;
            std::cout<<"enter change scene\n";
        }
    }

    for( auto &build : mBuildings)
        if(build->isDestroyed() and not build->down and build->getBonusFlag() == Entity::None)
            recBarrier(build->getOnMapPosition());

    for (auto &build : mBuildings){
        if(build->isDestroyed() and not build->down){
            auto size = build->getOnMapSize();
            auto pos = build->getOnMapPosition();
            auto id  = build->getMapId();
            for (int y(0); y < size.y;y++){
                for (int x(0); x <size.x;x++){
                    mMap.getTile(x+pos.x,y+pos.y).paint(sf::Vector2i(id.x+x,id.y+y),0);
                }
            }
            build->down = true;
        }
    }
}

void World::recBarrier(sf::Vector2i position){
    sf::Vector2i dir[4]={position+sf::Vector2i(0,1),
                         position+sf::Vector2i(1,0),
                         position+sf::Vector2i(0,-1),
                         position+sf::Vector2i(-1,0)};
    for (auto & i : dir){
        if (i.x < 0 or i.y < 0 or i.x >= 64 or i.y >= 36 ) continue;
        if(mMap.getTile(i).getTop().x == 2
        and mMap.getTile(i).getTop().y >= 36
        and mMap.getTile(i).getTop().y <= 39)
            for(auto &b :mBuildings){
                if (b->getOnMapPosition() == i
                and not b->isDestroyed()
                and b->getBonusFlag() == Entity::None){
                    b->destroy();
                    recBarrier(i);
                    break;
                }
            }
    }
}

void World::createEntity(sf::Vector2f position, Editor::Tool team, Editor::Entity type, int id){
    Entity::Team mteam;
    if(team == Editor::Tool::BlueTeam)
        mteam = Entity::BlueTeam;
    else
        mteam = Entity::RedTeam;

    if(type != Editor::Entity::Soldier)
        return;

    std::unique_ptr<Soldier> soldier = std::make_unique<Soldier>(id, mteam, mTextures, mFonts, mAstar, mCommandQueue);
    soldier->setPosition(position*mMap.getBlockSize());

    if(mteam ==  Entity::RedTeam)
        mRedTeam.push_back(soldier.get());
    else
        mBlueTeam.push_back(soldier.get());

    mSoldiers.push_back(soldier.get());
    mSceneLayers[Front]->attachChild(std::move(soldier));

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
        mTracked = static_cast<int>(mSoldiers.size()) - 1;
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