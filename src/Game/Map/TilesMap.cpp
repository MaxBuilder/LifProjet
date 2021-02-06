//
// Created by thibaut on 01/02/2021.
//

#include "TilesMap.hpp"
#include <cassert>

TilesMap::TilesMap(map::mode type,const sf::Texture &text, sf::Vector2u Wsize){
    windowSize = Wsize;
    texture = text;
    rotate = 0;
    ground_selection = Textures::ground::Grass;
    lastGround = Textures::ground::None;
    lastTileUpdate = {-1,-1};

    switch (type) {
        case map::mode::mapEditor :
            size = static_cast<float>(windowSize.x*0.8/64);
            origin = sf::Vector2i(size/2,windowSize.y*0.1+size/2);
            break;
        case map::mode::game :
            size = static_cast<float>(windowSize.x/64);
            origin = sf::Vector2i(size/2,size/2);
            break;
        case map::preview : break;
    }

    for(int y = 0; y < 36; y++){
        for(int x = 0; x < 64; x++){
            Tile *tile = &grid_id[x][y];

            tile->getSprite().setTexture(texture);
            tile->getSprite().setScale(size/15.f,size/15.f);
            tile->getSprite().setOrigin(15.f/2.f,15.f/2.f);
            tile->getSprite().setPosition(origin.x+float(x)*size,origin.y+float(y)*size);

            Paint::paintSprite(0 ,grid_id[x][y].getGround(), *tile);

        }
    }
}

Tile& TilesMap::getTile(int x, int y){
    assert(x >= 0 and y >= 0 and x < 64 and y < 36 );
    return grid_id[x][y];
}

void TilesMap::handleEvent(const sf::Event& event, const sf::RenderWindow& window){
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    pos.y = pos.y - origin.y + static_cast<int>(size/2);
    pos.y = pos.y/size;
    pos.x = pos.x/size;

    action ac;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) or (sf::Mouse::isButtonPressed(sf::Mouse::Left))){

        if (lastTileUpdate == pos and lastGround == ground_selection)
            return;
        else{
            lastTileUpdate = pos;
            lastGround = ground_selection;
        }

        ac.coordinate = pos;
        ac.rotation =  grid_id[pos.x][pos.y].getRotation();
        ac.id = grid_id[pos.x][pos.y].getGround();
        ac.tool = map::tool::standard;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        Paint::paintSprite(rotate, Textures::ground::None, grid_id[pos.x][pos.y]);
        undo.push_front(ac);

    }

    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        switch ( tool ){

            case map::tool::standard : Paint::paintSprite(rotate, ground_selection, grid_id[pos.x][pos.y]); break;

            case map::tool::square3 : paintSquare3(rotate, ground_selection, pos); break;

            case map::tool::circle5 : paintCircle5(rotate, ground_selection, pos); break;

            case map::tool::fill : paintFill(rotate, ground_selection, pos); break;

            default : break;
        }
        ac.tool = tool;
        undo.push_front(ac);
    }

    if (undo.size() > 20 )
        undo.pop_back();

}

void TilesMap::Undo() {
    std::cout<<"Undo\n";
    action ac;
    if( !undo.empty() )  ac = undo.front();
    else return;

    undo.pop_front();

    switch ( ac.tool ){

        case map::tool::standard : Paint::paintSprite(ac.rotation, ac.id, grid_id[ac.coordinate.x][ac.coordinate.y]); break;

        case map::tool::square3 : paintSquare3(ac.rotation,  ac.id, ac.coordinate); break;

        case map::tool::circle5 : paintCircle5(ac.rotation,  ac.id, ac.coordinate); break;

        case map::tool::fill : paintFill(ac.rotation,  ac.id, ac.coordinate); break;

        default : break;
    }

}

void TilesMap::setGroundSelection(Textures::ground::ID id){
    ground_selection = id;
}

void TilesMap::setRotation(float rotation){
    assert(rotation == 0 or rotation == 90 or rotation == -1);
    rotate = rotation;
}

void TilesMap::setTool(map::tool tmpTool) {
    tool = tmpTool;
}

void TilesMap::paintSquare3(float rotation, Textures::ground::ID id, sf::Vector2i coordinate){
    Paint::paintSprite(rotation, id, grid_id[coordinate.x][coordinate.y]);

    if (coordinate.x > 0){
        Paint::paintSprite(rotation, id, grid_id[coordinate.x-1][coordinate.y]);
        if (coordinate.y > 0)
            Paint::paintSprite(rotation, id, grid_id[coordinate.x-1][coordinate.y-1]);
        if (coordinate.y < 35)
            Paint::paintSprite(rotation, id, grid_id[coordinate.x-1][coordinate.y+1]);
    }
    if(coordinate.x < 63){
        Paint::paintSprite(rotation, id, grid_id[coordinate.x+1][coordinate.y]);
        if (coordinate.y > 0)
            Paint::paintSprite(rotation, id, grid_id[coordinate.x+1][coordinate.y-1]);
        if (coordinate.y < 35)
            Paint::paintSprite(rotation, id, grid_id[coordinate.x+1][coordinate.y+1]);
    }
    if (coordinate.y > 0)
        Paint::paintSprite(rotation, id, grid_id[coordinate.x][coordinate.y-1]);
    if (coordinate.y < 35)
        Paint::paintSprite(rotation, id, grid_id[coordinate.x][coordinate.y+1]);

}

void TilesMap::paintCircle5(float rotation, Textures::ground::ID id, sf::Vector2i coordinate){
    Paint::paintSprite(rotation, id, grid_id[coordinate.x][coordinate.y]);
}

void TilesMap::paintFill(float rotation, Textures::ground::ID id, sf::Vector2i coordinate){
    bool isPaint[36*64] = {false};
    recPaintFill(rotation, id, coordinate, isPaint);
}

void TilesMap::recPaintFill(float rotation, Textures::ground::ID id, sf::Vector2i co, bool* isPaint){

    isPaint[co.x+64*co.y] = true;

    if ( co.x > 0 and !isPaint[co.x-1+64*co.y] and
        grid_id[co.x][co.y].getGround() == grid_id[co.x-1][co.y].getGround() )

        recPaintFill(rotation, id, sf::Vector2i (co.x - 1, co.y), isPaint);

    if ( co.x < 63 and !isPaint[co.x+1+64*co.y] and
        grid_id[co.x][co.y].getGround() == grid_id[co.x+1][co.y].getGround() )

        recPaintFill(rotation, id, sf::Vector2i (co.x + 1, co.y), isPaint);

    if ( co.y > 0 and !isPaint[co.x+64*(co.y-1)] and
        grid_id[co.x][co.y].getGround() == grid_id[co.x][co.y-1].getGround() )

        recPaintFill(rotation, id, sf::Vector2i (co.x, co.y - 1), isPaint);

    if ( co.y < 35 and !isPaint[co.x+64*(co.y+1)] and
        grid_id[co.x][co.y].getGround() == grid_id[co.x][co.y+1].getGround() )

        recPaintFill(rotation, id, sf::Vector2i (co.x, co.y + 1), isPaint);

    Paint::paintSprite(rotation, id, grid_id[co.x][co.y]);

}

void TilesMap::save(const std::string &file) const{

    std::ofstream wf(file, std::ios::out | std::ios::binary);
    if(!wf) {
        std::cout << "Cannot open file!" << std::endl;
        return;
    }

    for(const auto & x : grid_id) {
        for (const auto & y : x) {

            wf.write((char *) &(y.ground), sizeof(Textures::ground::ID));
            wf.write((char *) &(y.rotate), sizeof(float));
        }
    }
    wf.close();

}

void TilesMap::load(const std::string &file) {

    std::ifstream rf(file, std::ios::out | std::ios::binary);
    if(!rf) {
        std::cout << "Cannot open file!" << std::endl;
        return ;
    }

    for(auto & x : grid_id) {
        for (auto & tile : x) {

            rf.read((char *) &(tile.ground), sizeof(Textures::ground::ID));
            rf.read((char *) &(tile.rotate), sizeof(float));

            Paint::paintSprite(tile.rotate, tile.ground, tile);
        }
    }
    rf.close();


}

void TilesMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    states.transform *= getTransform();

    for(int y = 0; y < 36; y++) {
        for (const auto & x : grid_id) {

            target.draw(x[y].getConstSprite(), states);

        }
    }
}