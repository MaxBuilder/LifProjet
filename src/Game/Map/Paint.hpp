//
// Created by thibaut on 02/02/2021.
//

#ifndef LIFPROJET_PAINT_HPP
#define LIFPROJET_PAINT_HPP

#include "Tile.hpp"

#include "../../Game/Resources/ResourceIdentifiers.hpp"


class Paint {

public :

    static void paintSprite(float rotation, Textures::Ground::ID id, Tile& tile){
        tile.setGround(id);

        if (rotation >= 0) tile.setRotation(rotation);

        else tile.setRotation((static_cast<float>(std::rand()%3)*90));


        switch (id) {

            case Textures::Ground::None :
                tile.getSprite().setTextureRect(sf::IntRect(0, 0, 15, 15));
                tile.setMoveSpeed(1);
                tile.setcrossable(false);
                break;

            case Textures::Ground::Grass :
                tile.getSprite().setTextureRect(sf::IntRect(15, 0, 15, 15));
                tile.setMoveSpeed(1);
                tile.setcrossable(true);
                break;

            case Textures::Ground::Sand :
                tile.getSprite().setTextureRect(sf::IntRect(30, 0, 15, 15));
                tile.setMoveSpeed(0.6f);
                tile.setcrossable(true);
                break;

            case Textures::Ground::Wood :
                tile.getSprite().setTextureRect(sf::IntRect(45, 0, 15, 15));
                tile.setMoveSpeed(1.2);
                tile.setcrossable(true);
                break;

            case Textures::Ground::Water :
                tile.getSprite().setTextureRect(sf::IntRect(60, 0, 15, 15));
                tile.setMoveSpeed(0.3);
                tile.setcrossable(false);
                break;

            case Textures::Ground::Wall :
                tile.getSprite().setTextureRect(sf::IntRect(75, 0, 15, 15));
                tile.setMoveSpeed(1);
                tile.setcrossable(false);
                break;
        }
    }

};

#endif //LIFPROJET_PAINT_HPP
