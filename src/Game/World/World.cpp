//
// Created by 33771 on 23/02/2021.
//

#include "World.hpp"

World::World(sf::RenderTarget &outputTarget, TextureHolder &textures, FontHolder &fonts, SoundPlayer &sounds, MusicPlayer& music)
: mTarget(outputTarget)
, mTextures(textures)
, mSounds(sounds)
, mMusic(music)
, mFonts(fonts)
, mSceneLayers()
, mSoldiers()
, mTracked(-1)
, mMap(textures.get(Textures::MapGround), 20.f)
, mCommandQueue()
, mSimData()
{
    // Map initialization
    mMap.load("data/MapData/blank.map");
}

void World::init(const std::string &mapPath) {
    mMap.load(mapPath);
    Debug::Log("Map loaded");

    // Setting up Pathfinding
    mAstar.setMap(mMap);

    // Scene building based on 3 plans (back, front and air)
    for(std::size_t i = 0 ; i < LayerCount ; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers.push_back(layer.get());

        mSceneGraph.attachChild(std::move(layer));
    }

    // Create entities from map data
    createEntity();

    // Initialisation of position for defenders
    for(auto &soldier : mBlueTeam)
        soldier->init();

    // Initialisation of village defenders
    for(auto &building : mBuildings) {
        if(building->getType() != EntityInfo::Village)
            continue;

        std::vector<Soldier*> defenders;
        for(auto &defender : mBlueTeam) {
            if(distance(building->getPosition(), defender->getPosition()) <= 150) {
                defenders.push_back(defender);
            }
        }
        building->mDefenders = defenders;
        building->initialNbDefenders = (int)defenders.size();
    }

}

void World::end() {
    for(auto &soldier : mSoldiers) {
        if(!soldier->isDestroyed())
            soldier->setAction(Soldier::Standby);
    }

    ended = true;
}

void World::draw() {
    mTarget.draw(mMap);
    mTarget.draw(mSceneGraph);
}

void World::update(sf::Time dt) {
    mSceneGraph.update(dt);
    if(ended)
        return;
    onCommand();
    updateMovement();
    updateTargets();
    updateBonus();
}

void World::onCommand() {
    while(!mCommandQueue.isEmpty()) {
        Command command = mCommandQueue.pop();
        
        // Selection de l'équipe à affecter :
        std::vector<Soldier*> selectedTeam;
        selectedTeam = command.mTeam == EntityInfo::Blue ? mBlueTeam : mRedTeam;
        float dist;

        switch(command.mType) {
            case CommandType::MakeTeam:
                dist = command.mTeam == EntityInfo::Blue ? 200 : 150;
                for(auto &team : selectedTeam) {
                    if(team != selectedTeam[command.mSender] and distance(team->getPosition(), selectedTeam[command.mSender]->getPosition()) < dist) {
                        team->createTeam(selectedTeam[command.mSender]->getId());
                        selectedTeam[command.mSender]->nbRequested++;
                    }
                }
                Debug::Log(std::to_string(selectedTeam[command.mSender]->getId()) + " ask for a team up to " + std::to_string(selectedTeam[command.mSender]->nbRequested) + " units");
                break;

            case CommandType::TeamAccept:
                Debug::Log("Accepted by " + std::to_string(command.mSender));
                selectedTeam[command.mReceiver]->nbResponse++;
                selectedTeam[command.mReceiver]->mSquadSize++;
                selectedTeam[command.mReceiver]->mSquadIds.push_back(command.mSender);
                selectedTeam[command.mSender]->setAction(Soldier::WithSquad);
                selectedTeam[command.mSender]->setLeader(selectedTeam[command.mReceiver]);
                break;

            case CommandType::TeamDeny:
                Debug::Log("Declined by " + std::to_string(command.mSender));
                selectedTeam[command.mReceiver]->nbResponse++;
                break;

            case CommandType::InPosition:
                selectedTeam[command.mReceiver]->nbInPlace++;
                Debug::Log(std::to_string(command.mSender) + " in position, leader : " + std::to_string(command.mReceiver));
                Debug::Log("Nb in place : " + std::to_string(selectedTeam[command.mReceiver]->nbInPlace));
                break;

            case CommandType::Assault:
                Debug::Log("Assault started by " + std::to_string(command.mReceiver));
                selectedTeam[command.mReceiver]->setAction(Soldier::Assaulting);
                break;

            case CommandType::Dead:
                Debug::Log("Entity died");
                updateDeath();
                break;

            case CommandType::MakeArrow:
            {
                Soldier * s = selectedTeam[command.mSender];
                std::unique_ptr<Projectile> arrow = std::make_unique<Projectile>(s->getPosition(),s->getTarget(), mTextures.get(Textures::EntityArrow), s->getDamage());
                mArrows.push_back(arrow.get());
                if(mArrows.size() > size_t(30)){
                    mSceneLayers[Layer::Air]->detachChild(static_cast<SceneNode*>(mArrows.front()));
                    mArrows.pop_front();
                }
                mSceneLayers[Layer::Air]->attachChild(std::move(arrow));
            }
            break;

            case CommandType::CastleAssaulted:
                Debug::Log("All defenders called to defend castle");
                for(auto defender : mBlueTeam)
                    if(!defender->isDestroyed())
                        defender->setAction(Soldier::DefendingCastle);
                break;

            case CommandType::FallBack:
                Debug::Log("Village lost, falling back");
                selectedTeam[command.mReceiver]->setAction(Soldier::FallingBack);
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
        float distMin = 99999999.0f;
        float distMinTank = 99999999.0;
        float distMinKnight = 99999999.0;
        float distMinArcher = 99999999.0;
        Entity* tank = nullptr;

        for(auto &blue : mBlueTeam) {
            if(blue->getAction() == Soldier::FallingBack)
                continue;

            float dist = distance(red->getPosition(), blue->getPosition());
            if(dist < 150 and !blue->isDestroyed()) { // In sight
                red->mTargetInSight++;
                red->closetInSightDirection = blue->getPosition();
                if(dist < distMinTank and blue->getType() == EntityInfo::Tank) {
                    if(dist < 100) {
                        tank = static_cast<Entity *>(blue);
                        distMinTank = dist;
                    }
                }
                else if (dist < distMinKnight and blue->getType() == EntityInfo::Knight){
                    if(dist < 100) {
                        distMinKnight = dist;
                    }
                }else if(dist < distMinArcher and blue->getType() == EntityInfo::Archer){
                    if(dist < 100) {
                        distMinArcher = dist;
                    }
                }
                if(dist < distMin) {
                    if(dist < 100) {
                        red->setTarget(static_cast<Entity *>(blue));
                        distMin = dist;
                        gotAssigned = true;
                    }
                }
            }
        }
        if(red->getType() == EntityInfo::Archer and tank != nullptr){
            red->setTarget(tank);
        }

        if(!gotAssigned or red->getType() == EntityInfo::Tank) {
            distMin = 99999999.0;
            for (auto &build : mBuildings){
                float dist = distance(red->getPosition(), build->getPosition());
                if(dist < 150 and !build->isDestroyed() and build->getTeam() == EntityInfo::Blue) { // In sight
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
        blue->mTargetInSight = 0;
        bool gotAssigned = false;
        float distMin = 99999999.0f;
        float distMinTank = 99999999.0;
        float distMinKnight = 99999999.0;
        float distMinArcher = 99999999.0;
        Entity* tank = nullptr;

        for(auto &red : mRedTeam) {
            float dist = distance(red->getPosition(), blue->getPosition());
            if(dist < 100 and !red->isDestroyed()) { // In sight
                blue->mTargetInSight++;
                blue->closetInSightDirection = red->getPosition();
                if(dist < distMinTank and red->getType() == EntityInfo::Tank) {
                    if(dist < 80) {
                        tank = static_cast<Entity *>(red);
                        distMinTank = dist;
                    }
                }
                else if (dist < distMinKnight and red->getType() == EntityInfo::Knight){
                    if(dist < 80) {
                        distMinKnight = dist;
                    }
                }else if(dist < distMinArcher and red->getType() == EntityInfo::Archer){
                    if(dist < 80) {
                        distMinArcher = dist;
                    }
                }
                if(dist < distMin) {
                    if(dist < 80) {
                        blue->setTarget(static_cast<Entity *>(red));
                        distMin = dist;
                        gotAssigned = true;
                    }
                }
            }
        }
        if(blue->getType() == EntityInfo::Archer and tank != nullptr){
            blue->setTarget(tank);
        }
        if(!gotAssigned)
            blue->setTarget(nullptr);

        for(auto &ally : mBlueTeam) { // Pertinent (on sait jamais)
            if (ally != blue and distance(ally->getPosition(), blue->getPosition()) < 150)
                blue->mAllyInSight++;
        }
    }
}

void World::updateBonus() {
    bool entityHaveBonus;
    for (auto &entity : mSoldiers){

        if (entity->isDestroyed()){
            entity->changeBonus(EntityInfo::None);
            continue;
        }

        entityHaveBonus = false;
        for (const auto &build : mBuildings ){
            if(entity->getTeam() != build->getTeam() or build->isDestroyed()) continue;
            if (distance(build->getPosition(),entity->getPosition()) <= build->getRange()) {
                entity->changeBonus(build->getType());
                entityHaveBonus = true;
            }
        }
        if (!entityHaveBonus)
            entity->changeBonus(EntityInfo::None);
    }
}

void World::updateMovement() {
    for (auto entity : mSoldiers) {
        if(entity->isDestroyed()) continue;

        sf::Vector2f tmpVeloce = entity->getVelocity();
        entity->setVelocity(entity->getVelocity()*mMap.getTile(entity->getPosition()/20.f).getMoveSpeed());
        sf::Vector2f point = (entity->getPosition()+entity->getVelocity());

        for (auto other : mSoldiers) {
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

        sf::Vector2i pos = sf::Vector2i((int)point.x / 20, (int)point.y / 20);
        if (mMap.getTile(pos).isCrossable(entity->getTeam()) and inMap(point))
            entity->travel();
        else {
            entity->usePathFinding = true;
        }
        entity->setVelocity(tmpVeloce);
    }
}

void World::updateDeath() {

    for (auto *e : mSoldiers) {
        if (e->isDestroyed() and not e->down) {
            mSceneLayers[Back]->attachChild(mSceneLayers[Front]->detachChild(static_cast<SceneNode *>(e)));
            e->down = true;
            e->getTeam() == EntityInfo::Red ? mSimData.nbRedSoldierEnd-- : mSimData.nbBlueSoldierEnd--;
            if (mSimData.nbRedSoldierEnd == 0) {
                mSimData.mBlueVictory = true;
                Debug::Log("Blue victory");
            }
        }
    }

    for(auto &build : mBuildings)
        if(build->isDestroyed() and not build->down and build->getType() == EntityInfo::Barrier)
            recBarrier(build->getOnMapPosition());

    for (auto &build : mBuildings) {
        if(build->isDestroyed() and not build->down) {
            auto size = build->getOnMapSize();
            auto pos = build->getOnMapPosition();
            auto id  = build->getMapId();
            for (int y(0); y < size.y;y++){
                for (int x(0); x <size.x;x++){
                    mMap.getTile(x+pos.x,y+pos.y).paint(sf::Vector2i(id.x+x,id.y+y),0);
                }
            }
            build->down = true;
            build->getTeam() == EntityInfo::Red ? mSimData.nbRedBuildingEnd-- : mSimData.nbBlueBuildingEnd--;
            if(build->getType() == EntityInfo::Castle) {
                mSimData.mRedVictory = true;
                Debug::Log("Red victory");
            }
        }
    }
}

void World::createEntity() {
    sf::Vector2i blueCastle;

    // Adding buildings to the scene
    auto builds = mMap.getBuildingsIt();
    for (; builds.first != builds.second ; builds.first++) {
        std::unique_ptr<Building> build = std::make_unique<Building>(builds.first->getID(), builds.first->getTeam(), builds.first->getPosition(), mCommandQueue);

        if(build->getType() == EntityInfo::Castle and (build->getTeam() == EntityInfo::Blue)) {
            blueCastle = build->getOnMapPosition();
        }
        mBuildings.push_back(build.get());
        build->getTeam() == EntityInfo::Red ? mSimData.nbRedBuildingBegin++ : mSimData.nbBlueBuildingBegin++;
        mSceneLayers[Back]->attachChild(std::move(build));
    }
    mSimData.nbRedBuildingEnd = mSimData.nbRedBuildingBegin;
    mSimData.nbBlueBuildingEnd = mSimData.nbBlueBuildingBegin;

    // Adding soldiers to the scene
    auto e = mMap.getEntitiesIt();
    int idr = 0;
    int idb = 0;
    int indice;
    sf::Vector2i objectif;

    for (; e.first != e.second ; e.first++) {
        if(e.first->getTeam() == EntityInfo::Blue) {
            indice = idb;
            idb++;
            objectif = blueCastle;
        }
        else {
            indice = idr;
            idr++;
            objectif = blueCastle;
        }

        std::unique_ptr<Soldier> soldier = std::make_unique<Soldier>(indice,e.first->getID() ,e.first->getTeam(), objectif, mTextures, mFonts, mAstar, mCommandQueue);
        soldier->setPosition(e.first->getPosition()*mMap.getBlockSize());

        if(e.first->getTeam() ==  EntityInfo::Team::Red)
            mRedTeam.push_back(soldier.get());
        else
            mBlueTeam.push_back(soldier.get());

        mSoldiers.push_back(soldier.get());
        mSceneLayers[Front]->attachChild(std::move(soldier));

    }

    mSimData.nbRedSoldierBegin = mSimData.nbRedSoldierEnd = idr;
    mSimData.nbBlueSoldierBegin = mSimData.nbBlueSoldierEnd = idb;

}

void World::recBarrier(sf::Vector2i position) {
    sf::Vector2i dir[4]={position+sf::Vector2i(0,1),
                         position+sf::Vector2i(1,0),
                         position+sf::Vector2i(0,-1),
                         position+sf::Vector2i(-1,0)};
    for (auto & i : dir) {
        if (i.x < 0 or i.y < 0 or i.x >= 64 or i.y >= 36 )
            continue;
        if(mMap.getTile(i).getTop().x == 2 and mMap.getTile(i).getTop().y >= 36 and mMap.getTile(i).getTop().y <= 39)
            for(auto &b :mBuildings) {
                if (b->getOnMapPosition() == i and not b->isDestroyed() and b->getType() == EntityInfo::Barrier) {
                    b->destroy();
                    recBarrier(i);
                    break;
                }
            }
    }
}

bool World::inMap(sf::Vector2f dpl) {
    return (dpl.x > 0 and dpl.x < 1280 and dpl.y > 0 and dpl.y < 720);
}

void World::switchDisplayDebug() {
    for (auto e :mSoldiers)
        e->switchDebugDisplay();
}

void World::trackNext() {
    mTracked++;
    if(mTracked > mSoldiers.size() - 1)
        mTracked = 0;
}

void World::trackPrev() {
    mTracked--;
    if(mTracked < 0)
        mTracked = static_cast<int>(mSoldiers.size()) - 1;
}

void World::untrack() {
    mTracked = -1;
}

sf::Vector2f World::trackedPos() {
    return mSoldiers[mTracked]->getPosition();
}

World::SimulationData &World::getSimData() {
    return mSimData;
}

World::SimulationData::SimulationData()
: mBlueVictory(false)
, mRedVictory(false)
, nbBlueBuildingBegin(0)
, nbBlueBuildingEnd(0)
, nbBlueSoldierBegin(0)
, nbBlueSoldierEnd(0)
, nbRedBuildingBegin(0)
, nbRedBuildingEnd(0)
, nbRedSoldierBegin(0)
, nbRedSoldierEnd(0)
{}

bool World::SimulationData::isEnded() const {
    return mBlueVictory or mRedVictory;
}
