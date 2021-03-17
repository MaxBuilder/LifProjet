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

        //vetor qui contien les noeuds a check
        std::vector<AstarTile*>knots;

        //fonction à intégret dans le projet
        int poids;
        //a rename probablement ou supprimer
        coordinate objectif;

        double distance(coordinate ind)const;
        //fonction pas obligatoir et qui peut etre supprimer
        bool indIsValid(coordinate ind);

        int minimum(std::vector<AstarTile*> knots)const;
        void setNeighbour(AstarTile* knot);
        coordinate getNeighbourIndex(int x,int y,cardinal c)const;
        bool crossCorner(coordinate ind, cardinal card);

    public :
        void afficherGraph();
        AstarAlgo();
        AstarAlgo(TilesMap &map);
        ~AstarAlgo();
        void addObjectif(int x,int y);
        coordinate getObjectif();

        /** Execute la recherche et retourne le chemin complet de l'entité à la target
         *
         * @param lSelf et wSelf les coordonnées de l'entitée
         * @param ltarget et wTarget les coordonnées de la cible
         * @param path le Vector<sf::Vector2f> contenant le chemin jusqu'a la cible
         */
        void getPath(std::shared_ptr<TilesMap> &map,int lSelf,int wSelf, int ltarget, int wTarget, std::vector<sf::Vector2f> &path,int poid = 2);
        void resetGraph();



        void setPoids(int p);
        //l et w les coordonnées de self et lt et wt les coordonnées de la target
        void initVoronoi(int l, int w, int lt, int wt);
        //2eme version pas implémenté
        void initVoronoi(coordinate self, coordinate target);
        bool Voronoi();

};


#endif //ASTAR_PROJET_ASTARALGO_H
