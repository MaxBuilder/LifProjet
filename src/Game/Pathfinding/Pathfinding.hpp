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
#include "PathfindingTile.hpp"


/**
 * @enum cardinal
 * @brief used to check the surounding of a tile in a 2d table
 */
enum cardinal {
    South, North, Est, West, NorthEst, NorthWest, SouthEst, SouthWest
};


class Pathfinding {

private :
    unsigned int length{};
    unsigned int width{};
    PathfindingTile Astar_grid[64][36];
    std::shared_ptr<TilesMap> mMap;

    //vetor qui contien les noeuds a check
    std::vector<PathfindingTile *> knots;

    int poids{};
    sf::Vector2i objectif;

    /**
     * @brief heuristic used in the pathfinding. (heuristic Euclidean or chebyshev)
     * @return the value of the heuristic function used
     */
    double distance(sf::Vector2i ind) const;

    /**
     * @brief function that check if ind.x and ind.y are greater than 0
     * @return true if ind.x > 0 and ind.y > 0
     */
    bool indIsValid(sf::Vector2i ind);

    /**
     * @brief get the index of the tile with the smallest distance from knots
     * @param knots vector<PathfindingTile *> that contains the knot that didn't check
     * @return the index of the PathfindingTile with the smallest dist
     */
    int minimum(std::vector<PathfindingTile *> knots) const;

    /**
     * @brief set the value of the neighbour of knot
     * @param knot the tile which we want to define the value of its neighbor
     */
    void setNeighbour(PathfindingTile *knot);

    /**
     * @brief get the neighbour who is at the cardinality c of the tile[x][y]
     * @param x the coordinate x of the tile we want to check
     * @param y the coordinate y of the tile we want to check
     * @param c the cardinality of the neighbour we want to get
     * @return the coordinate of the neighbour we want
     */
    sf::Vector2i getNeighbourIndex(int x, int y, cardinal c) const;
    /**
     * @brief function that check if we are moving diagonally and if that is the case check if we cross a corner
     * @param ind the coordinate of the tile we need to check
     * @param card the cardinality we want to check
     * @return true if we cross a corner
     */
    bool crossCorner(sf::Vector2i ind, cardinal card);

public :

    /**
     * @brief funciton used for debug.
     */
    void afficherGraph();

    /**
     * @brief Constructor of Pathfinding
     */
    Pathfinding();

    /**
     * @brief destructor of Pathfinding
     */
    ~Pathfinding();

    /**
     * @brief set the coordinate x,y of the target
     * @param x coordinate x of the target
     * @param y coordinate y of the target
     */
    void addObjectif(int x, int y);

    /**
     * @brief initialize the Astar_grid based on the TilesMap
     * @param map the map from which we set the Astar_grid
     */
    void setMap(TilesMap &map);

    /**
     * @brief function to get the target
     * @return the coordinate of the target
     */
    sf::Vector2i getObjectif();

    /**
     * @brief Main function of the class that put the path from entity to target in path
     * @param self the coordinate of the entity
     * @param target the coordinate of the target
     * @param path Vector<sf::Vector2f> that will contain the path to the target.
     * @param poid the weigh of the heuristique, default value is 2
     */
    void getPath(sf::Vector2f self, sf::Vector2f target, std::vector<sf::Vector2f> &path, int poid = 2);

    /**
     * @brief reinitialize the Astar_grid and clear knot so it can be used by other entity
     */
    void resetGraph();

    /**
     * @brief set the weigh of the heuristic
     * @param p the value of the weigh
     */
    void setPoids(int p);

    /**
     * @brief set the value of the entity and its target
     * @param self the coordinate of the entity
     * @param target the coordinate of the target
     */
    void initVoronoi(sf::Vector2f self, sf::Vector2f target);

    /**
     * @brief function used in loop to calculate the path. return false when the path is found or not reachable
     * @return true if we still need to check some tile to get the path and false if the path is found or not reachable
     */
    bool Voronoi();

};


#endif //ASTAR_PROJET_ASTARALGO_H
