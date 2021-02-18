//
// Created by thibaut on 01/02/2021.
//

#include "TilesMap.hpp"
#include <cassert>

TilesMap::TilesMap(const sf::Texture &texture, float blocSize, sf::Vector2i origin)
: texture(texture)
{
    mDrawBuildings = false;
    mOrigin = origin;
    mBlockSize = blocSize;

    for(int y = 0; y < 36; y++) {
        for (int x = 0; x < 64; x++) {
            Tile *tile = &grid_id[x][y];

            tile->getSprite().setTexture(texture);
            tile->getSprite().setScale(float(mBlockSize) / 15.f, float(mBlockSize) / 15.f);
            tile->getSprite().setOrigin(15.f / 2.f, 15.f / 2.f);
            tile->getSprite().setPosition(mOrigin.x + float(x) * mBlockSize + mBlockSize / 2,
                                          mOrigin.y + float(y) * mBlockSize + mBlockSize / 2);
            Paint::paintSprite(0 , Textures::Ground::None, *tile);
        }
    }
    //clear();
}

void TilesMap::clear() {
    for(int y = 0; y < 36; y++) {
        for (int x = 0; x < 64; x++) {
            Paint::paintSprite(0 , Textures::Ground::None, grid_id[x][y]);
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

sf::Vector2i TilesMap::getOrigins() const{
    return mOrigin;
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

    for(const auto & x : grid_id) {
        for (const auto & y : x) {

            wf.write((char *) &(y.ground), sizeof(Textures::Ground::ID));
            wf.write((char *) &(y.rotate), sizeof(float));
        }
    }

    int left, top;
    Textures::Building::ID buildID;
    std::size_t size = mBuildings.size();
    wf.write((char *) &(size), sizeof(std::size_t));

    for (const auto &b : mBuildings){
        left = b.getPosition().left;
        top = b.getPosition().top;
        buildID = b.getID();

        wf.write((char *) &(buildID), sizeof(Textures::Building::ID));
        wf.write((char *) &(left), sizeof(int));
        wf.write((char *) &(top), sizeof(int));
        wf.write((char *) &(b.mRotation), sizeof(float));
    }

    wf.close();

}

void TilesMap::load(const std::string &file) {

    mBuildings.clear();
    std::ifstream rf(file, std::ios::out | std::ios::binary);
    if(!rf) {
        std::cout << "Cannot open file!" << std::endl;
        return ;
    }

    for(auto & x : grid_id) {
        for (auto & tile : x) {

            rf.read((char *) &(tile.ground), sizeof(Textures::Ground::ID));
            rf.read((char *) &(tile.rotate), sizeof(float));

            Paint::paintSprite(tile.rotate, tile.ground, tile);
        }
    }

    std::size_t nb_buildings = 0;
    int left, top;
    float rot;
    Textures::Building::ID buildID = Textures::Building::None;

    rf.read((char *) &(nb_buildings), sizeof(std::size_t));
    for (std::size_t i(0); i<nb_buildings; i++){

        rf.read((char *) &(buildID), sizeof(Textures::Building::ID));
        rf.read((char *) &(left), sizeof(int));
        rf.read((char *) &(top), sizeof(int));
        rf.read((char *) &(rot), sizeof(float));

        sf::IntRect rect1 ={left, top, 0, 0};

        BuildMap build(buildID, rect1,rot);
        addBuildings(build);

    }

    rf.close();


}

std::pair<std::vector<BuildMap>::iterator,
        std::vector<BuildMap>::iterator> TilesMap::getBuildingsIt(){
    return std::make_pair(mBuildings.begin(),mBuildings.end());
}

void TilesMap::addBuildings(BuildMap build){
    mBuildings.push_back(std::move(build));
}

void TilesMap::supBuildings(std::vector<BuildMap>::iterator it){
    mBuildings.erase(it);
}

void TilesMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    states.transform *= getTransform();

    for(int y = 0; y < 36; y++) {
        for (const auto & x : grid_id) {

            target.draw(x[y].getConstSprite(), states);

        }
    }
    if (mDrawBuildings)
        for( const auto &build : mBuildings)
            target.draw(build.getConstSprite(),states);

}