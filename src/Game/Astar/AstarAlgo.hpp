//
// Created by Marc on 23/02/2021.
//

#ifndef ASTAR_PROJET_ASTARALGO_H
#define ASTAR_PROJET_ASTARALGO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <SFML/Graphics.hpp>
#include "AstarTile.hpp"


enum cardinal{South, North, Est, West, NorthEst, NorthWest, SouthEst, SouthWest};


class AstarAlgo : public sf::Transformable{

    private :
        unsigned int length;
        unsigned int width;
        AstarTile Astar_grid[64][36];
        std::shared_ptr<TilesMap> mMap;

        //vetor qui contien les noeuds a check
        std::vector<AstarTile*>knots;

        //fonction à intégret dans le projet
        int poids;
        //a rename probablement ou supprimer
        sf::Vector2i objectif;

        double distance(sf::Vector2i ind)const;
        //fonction pas obligatoir et qui peut etre supprimer
        bool indIsValid(sf::Vector2i ind);

        int minimum(std::vector<AstarTile*> knots)const;
        void setNeighbour(AstarTile* knot);
        sf::Vector2i getNeighbourIndex(int x,int y,cardinal c)const;
        bool crossCorner(sf::Vector2i ind, cardinal card);

    public :
        void afficherGraph();
        AstarAlgo();
        ~AstarAlgo();
        void addObjectif(int x,int y);
        void setMap(TilesMap& map);
        sf::Vector2i getObjectif();

        /** Execute la recherche et retourne le chemin complet de l'entité à la target
         *
         * @param lSelf et wSelf les coordonnées de l'entitée
         * @param ltarget et wTarget les coordonnées de la cible
         * @param path le Vector<sf::Vector2f> contenant le chemin jusqu'a la cible
         */
        void getPath(sf::Vector2f self, sf::Vector2f target, std::vector<sf::Vector2f> &path,int poid = 2);
        // std::vector<sf::Vector2f> getPath(std::shared_ptr<TilesMap> &map,sf::Vector2f self, sf::Vector2f target,int poid = 2);
        void resetGraph();



        void setPoids(int p);
        //l et w les coordonnées de self et lt et wt les coordonnées de la target
        void initVoronoi(sf::Vector2f self, sf::Vector2f target);
        bool Voronoi();

};


#endif //ASTAR_PROJET_ASTARALGO_H
