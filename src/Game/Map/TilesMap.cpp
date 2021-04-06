//
// Created by thibaut on 01/02/2021.
//

#include "TilesMap.hpp"
#include <cassert>

TilesMap::TilesMap(const sf::Texture &texture, float blocSize)
: texture(texture)
{
    mDrawEntity = false;
    mBlockSize = blocSize;

    mSprite.setTexture(texture);
    mBounds = 16.f;
    mSprite.setScale(float(mBlockSize) / mBounds, float(mBlockSize) / mBounds);
    mSprite.setOrigin(mBounds / 2.f, mBounds / 2.f);

}

TilesMap::TilesMap(const sf::Texture &mapTexture, const sf::Texture &entitiesTexture, float blocSize){
    mDrawEntity = true;
    mBlockSize = blocSize;

    mSprite.setTexture(mapTexture);
    mEntitySprite.setTexture(entitiesTexture);

    mBounds = 16.f;

    mSprite.setScale(float(mBlockSize) / mBounds, float(mBlockSize) / mBounds);
    mSprite.setOrigin(mBounds / 2.f, mBounds / 2.f);

    mEntitySprite.setScale(mBlockSize/32, mBlockSize/32);
    mEntitySprite.setOrigin(mBounds / 2.f, mBounds / 2.f);

}

void TilesMap::clear() {
    for(auto & x : grid_id) {
        for (auto & t : x) {
            t.paint(sf::Vector2i (-1,-1),0);
        }
    }
    mBuildings.clear();
    mEntities.clear();
}

Tile& TilesMap::getTile(int x, int y){
    assert(x >= 0 and y >= 0 and x < 64 and y < 36 );
    return grid_id[x][y];
}
Tile& TilesMap::getTile(sf::Vector2f position){
    if(not((position.x >= 0 and position.y >= 0 and position.x < 64 and position.y < 36 )))
        std::cout<<position.x<<" "<<position.y<<std::endl;
    assert(position.x >= 0 and position.y >= 0 and position.x < 64 and position.y < 36 );
    auto pos = static_cast<sf::Vector2i>(position);
    return grid_id[pos.x][pos.y];
}

Tile& TilesMap::getTile(sf::Vector2i position){
    if(not((position.x >= 0 and position.y >= 0 and position.x < 64 and position.y < 36 )))
        std::cout<<position.x<<" "<<position.y<<std::endl;
    assert(position.x >= 0 and position.y >= 0 and position.x < 64 and position.y < 36 );
    return grid_id[position.x][position.y];
}

float TilesMap::getBlockSize() const{
    return mBlockSize;
}

void TilesMap::save(const std::string &file) const{

    std::ofstream wf(file, std::ios::out | std::ios::binary);
    if(!wf) {
        std::cout << "Cannot open file!" << std::endl;
        return;
    }

    int tmpGroundx = 0,tmpGroundy = 0,tmpTopx = 0,tmpTopy = 0;
    bool havetop = false;
    float tmpRotate = 0;

    for(const auto & x : grid_id) {
        for (const auto & t : x) {

            havetop = t.haveTop();
            tmpGroundx = t.getGround().x;
            tmpGroundy = t.getGround().y;
            tmpTopx = t.getTop().x;
            tmpTopy = t.getTop().y;
            tmpRotate = t.getRotation();

            wf.write((char *) &(havetop), sizeof(int));
            wf.write((char *) &(tmpGroundx), sizeof(int));
            wf.write((char *) &(tmpGroundy), sizeof(int));
            wf.write((char *) &(tmpTopx), sizeof(int));
            wf.write((char *) &(tmpTopy), sizeof(int));
            wf.write((char *) &(tmpRotate), sizeof(float));
        }
    }

    float x,y;
    EntityInfo::ID buildID;
    EntityInfo::Team buildTeam;

    std::size_t size = mBuildings.size();
    wf.write((char *) &(size), sizeof(std::size_t));

    for (const auto &b : mBuildings){
        x = b.getPosition().x;
        y = b.getPosition().y;
        buildID = b.getID();
        buildTeam = b.getTeam();

        wf.write((char *) &(buildID), sizeof(EntityInfo::ID));
        wf.write((char *) &(buildTeam), sizeof(EntityInfo::Team));
        wf.write((char *) &(x), sizeof(float));
        wf.write((char *) &(y), sizeof(float));

    }

    EntityInfo::ID id;
    EntityInfo::Team team;
    size = mEntities.size();

    wf.write((char *) &(size), sizeof(std::size_t));
    for (const auto &e : mEntities){
        x = e.getPosition().x;
        y = e.getPosition().y;
        id = e.getID();
        team = e.getTeam();

        wf.write((char *) &(x),sizeof(float));
        wf.write((char *) &(y),sizeof(float));
        wf.write((char *) &(id),sizeof(EntityInfo::ID));
        wf.write((char *) &(team),sizeof(EntityInfo::Team));
    }

    wf.close();

}

void TilesMap::load(const std::string &file) {
    clear();
    std::ifstream rf(file, std::ios::out | std::ios::binary);
    if(!rf) {
        std::cout << "Cannot open file!" << std::endl;
        return ;
    }

    int tmpGroundx = 0,tmpGroundy = 0,tmpTopx = 0,tmpTopy = 0;
    bool havetop = false;
    float tmpRotate = 0;

    for(auto & x : grid_id) {
        for (auto & t : x) {

            rf.read((char *) &(havetop), sizeof(int));
            rf.read((char *) &(tmpGroundx), sizeof(int));
            rf.read((char *) &(tmpGroundy), sizeof(int));
            rf.read((char *) &(tmpTopx), sizeof(int));
            rf.read((char *) &(tmpTopy), sizeof(int));
            rf.read((char *) &(tmpRotate), sizeof(float));

            t.paint(sf::Vector2i(tmpGroundx,tmpGroundy),tmpRotate);
            if(havetop){
                t.paint(sf::Vector2i(tmpTopx,tmpTopy),tmpRotate);
            }
        }
    }

    std::size_t nb_buildings = 0;
    float x,y;
    EntityInfo::ID buildID = EntityInfo::ID::None;
    EntityInfo::Team buildTeam;

    rf.read((char *) &(nb_buildings), sizeof(std::size_t));
    if(nb_buildings > 100 ) return;
    for (std::size_t i(0); i<nb_buildings; i++){

        rf.read((char *) &(buildID), sizeof(EntityInfo::ID));
        rf.read((char *) &(buildTeam), sizeof(EntityInfo::Team));
        rf.read((char *) &(x), sizeof(float));
        rf.read((char *) &(y), sizeof(float));

        mBuildings.emplace_back(EntityInfo(sf::Vector2f(x,y),buildID,buildTeam,EntityInfo::Building));

    }

    std::size_t nb_entities = 0;
    EntityInfo::ID id;
    EntityInfo::Team team;

    rf.read((char *) &(nb_entities), sizeof(std::size_t));
    if(nb_entities > 100 ) return;
    for (std::size_t i(0); i<nb_entities; i++){

        rf.read((char *) &(x), sizeof(float));
        rf.read((char *) &(y), sizeof(float));
        rf.read((char *) &(id),sizeof(EntityInfo::ID));
        rf.read((char *) &(team),sizeof(EntityInfo::Team));

        sf::Vector2f pos(x,y);
        mEntities.emplace_back(EntityInfo(pos,id,team,EntityInfo::Type::Soldier));

    }
    rf.close();

}

std::pair<std::vector<EntityInfo>::iterator,
        std::vector<EntityInfo>::iterator> TilesMap::getBuildingsIt(){
    return std::make_pair(mBuildings.begin(),mBuildings.end());
}

std::pair<std::vector<EntityInfo>::iterator,
        std::vector<EntityInfo>::iterator> TilesMap::getEntitiesIt(){
    return std::make_pair(mEntities.begin(),mEntities.end());
}

void TilesMap::addBuildings(EntityInfo build){
    mBuildings.push_back(build);
}

void TilesMap::supBuildings(std::vector<EntityInfo>::iterator it){
    mBuildings.erase(it);
}

void TilesMap::addEntity(EntityInfo entity){
    mEntities.push_back(entity);
}

void TilesMap::supEntity(std::vector<EntityInfo>::iterator it){
    mEntities.erase(it);
}

void TilesMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    sf::Sprite sprite = mSprite;

    states.transform *= getTransform();

    for(int y = 0; y < 36; y++) {
        for (int x = 0; x < 64; x++) {
            const Tile t = grid_id[x][y];

            sprite.setTextureRect(sf::IntRect(mBounds*t.getGround().x, mBounds*t.getGround().y, mBounds, mBounds));
            sprite.setPosition( float(x) * mBlockSize + mBlockSize / 2,
                                float(y) * mBlockSize + mBlockSize / 2);
            sprite.setRotation(t.getRotation());
            target.draw(sprite, states);

            if (t.haveTop()){
                sprite.setTextureRect(sf::IntRect(mBounds*t.getTop().x, mBounds*t.getTop().y, mBounds, mBounds));
                target.draw(sprite, states);
            }
        }
    }

    if(mDrawEntity){
        sprite = mEntitySprite;

        for( const auto &Entity : mEntities){
            sf::IntRect rect = {0,0,32,32};
            if (Entity.getTeam() == EntityInfo::Team::Red) rect.top = 32;

            switch (Entity.getID()){
                case EntityInfo::Knight :
                    rect.left = 0;
                    break;
                case EntityInfo::Archer :
                    rect.left = 32;
                    break;
                case EntityInfo::Tank :
                    rect.left = 64;
                    break;
                default:
                    continue;
            }

            sprite.setTextureRect(rect);
            sprite.setPosition(Entity.getPosition()*mBlockSize+sf::Vector2f(5,5));
            target.draw(sprite, states);
        }
    }
}