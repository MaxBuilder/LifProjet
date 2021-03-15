//
// Created by thibaut on 01/02/2021.
//

#include "TilesMap.hpp"
#include <cassert>

TilesMap::TilesMap(const sf::Texture &texture, float blocSize)
: texture(texture)
{
    mDrawBuildings = false;
    mBlockSize = blocSize;

    mSprite.setTexture(texture);
    mBounds = 16.f;
    mSprite.setScale(float(mBlockSize) / mBounds, float(mBlockSize) / mBounds);
    mSprite.setOrigin(mBounds / 2.f, mBounds / 2.f);

    //clear();
}

void TilesMap::clear() {
    for(auto & x : grid_id) {
        for (auto & t : x) {
            t.paint(sf::Vector2i (-1,-1),0);
        }
    }
    mBuildings.clear();
}

Tile& TilesMap::getTile(int x, int y){
    assert(x >= 0 and y >= 0 and x < 64 and y < 36 );
    return grid_id[x][y];
}

float TilesMap::getBlockSize() const{
    return mBlockSize;
}

void TilesMap::setDrawBuildings(bool draw) {
    mDrawBuildings = draw;
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

    int left, top, width, height;
    Buildings::ID buildID;

    std::size_t size = mBuildings.size();
    wf.write((char *) &(size), sizeof(std::size_t));

    for (const auto &b : mBuildings){
        left = b.getPosition().left;
        top = b.getPosition().top;
        width = b.getPosition().width;
        height = b.getPosition().height;
        buildID = b.getID();

        wf.write((char *) &(buildID), sizeof(Buildings::ID));
        wf.write((char *) &(left), sizeof(int));
        wf.write((char *) &(top), sizeof(int));
        wf.write((char *) &(width), sizeof(int));
        wf.write((char *) &(height), sizeof(int));
    }

    wf.close();

}

void TilesMap::load(const std::string &file) {

    mBuildings.clear();
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
    int left, top, width, height;
    Buildings::ID buildID = Buildings::None;

    rf.read((char *) &(nb_buildings), sizeof(std::size_t));
    for (std::size_t i(0); i<nb_buildings; i++){

        rf.read((char *) &(buildID), sizeof(Buildings::ID));
        rf.read((char *) &(left), sizeof(int));
        rf.read((char *) &(top), sizeof(int));
        rf.read((char *) &(width), sizeof(int));
        rf.read((char *) &(height), sizeof(int));

        sf::IntRect rect1 ={left, top, width, height};

        addBuildings(BuildInfo(buildID, rect1));

    }

    rf.close();

}

std::pair<std::vector<BuildInfo>::iterator,
        std::vector<BuildInfo>::iterator> TilesMap::getBuildingsIt(){
    return std::make_pair(mBuildings.begin(),mBuildings.end());
}

void TilesMap::addBuildings(BuildInfo build){
    mBuildings.push_back(build);
}

void TilesMap::supBuildings(std::vector<BuildInfo>::iterator it){
    mBuildings.erase(it);
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
}