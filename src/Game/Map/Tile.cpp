//
// Created by thibaut on 01/02/2021.
//

#include "Tile.hpp"
#include <cassert>

Tile::Tile(){
    ground = Textures::Ground::None;
    rotate = 0;
    moveSpeed = 1;
    crossable = false;
}
void Tile::paint(const Textures::Ground::ID &id,const float &rotation ){
    ground = id;

    if (rotation >= 0) rotate = rotation;
    else rotate = static_cast<float>((std::rand()%3)*90);

    switch (id) {

        case Textures::Ground::None :
            moveSpeed = 1;
            crossable = false;
            break;

        case Textures::Ground::Grass :
            moveSpeed = 1;
            crossable = true;
            break;

        case Textures::Ground::Sand :
            moveSpeed = 0.6f;
            crossable = true;
            break;

        case Textures::Ground::Wood :
            moveSpeed = 1.2f;
            crossable = true;
            break;

        case Textures::Ground::Water :
            moveSpeed = 0.3f;
            crossable = false;
            break;

        case Textures::Ground::Wall :
            moveSpeed = 1;
            crossable = false;
            break;
    }
}


Textures::Ground::ID Tile::getGround() const{
    return ground;
}

float Tile::getMoveSpeed() const{
    return moveSpeed;
}

bool Tile::isCrossable() const{
    return crossable;
}

void Tile::setGround(const Textures::Ground::ID &id){
    ground = id;
}

void Tile::setMoveSpeed(const float &speed){
    assert(speed >= 0);
    moveSpeed = speed;
}

void Tile::setcrossable(const bool &cross){
    crossable = cross;

}

float Tile::getRotation() const{
    return rotate;
}

void Tile::setRotation(const float &rotation){
    assert(rotation == 0 or rotation == 90 or rotation == 180 or rotation == 270);
    rotate = rotation;
}