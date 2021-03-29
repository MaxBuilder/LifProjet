//
// Created by Marc on 23/02/2021.
//

#include "AstarAlgo.hpp"

void AstarAlgo::afficherGraph() {
    for(int i = 0; i< width; i++) {
        for(int j = 0 ; j < length; j++){
            std::cout << Astar_grid[i][j].dist << "";
        }
        std::cout << std::endl;
    }
}


AstarAlgo::AstarAlgo() {
}


void AstarAlgo::setMap(TilesMap &map){
    mMap = std::make_shared<TilesMap>(map);
    length = 64;
    width = 32;
    for (unsigned int x(0); x < length; x++) {
        for (unsigned int y(0); y < width; y++) {
            //il faut copier la map
            Astar_grid[x][y].setCoordinate(float(x), float(y));
        }
    }
}

AstarAlgo::~AstarAlgo(){

}

void AstarAlgo::resetGraph() {
    for (unsigned int x(0); x < length; x++) {
        for (unsigned int y(0); y < width; y++) {
            Astar_grid[x][y].cout = 0;
            Astar_grid[x][y].color = 'w';
            Astar_grid[x][y].dist = 0;
            Astar_grid[x][y].parent = nullptr;

        }
    }
    //pour reset les case a parcourir
    knots.clear();
}

void AstarAlgo::addObjectif(int x,int y){
    objectif.x = x;
    objectif.y = y;
}

sf::Vector2i AstarAlgo::getObjectif() {
    return objectif;
}



void AstarAlgo::setPoids(int p) {
    poids = p;
}




//self les coordonnées de l'entité et target les coordonnées de la target
void AstarAlgo::initVoronoi(sf::Vector2f self, sf::Vector2f target){
    int lSelf = self.x;
    int wSelf = self.y;
    Astar_grid[lSelf][wSelf].color = 'g';
    Astar_grid[lSelf][wSelf].dist = 0;
    Astar_grid[lSelf][wSelf].coordNoeud.x = lSelf;
    Astar_grid[lSelf][wSelf].coordNoeud.y = wSelf;
    //pour commencer depuis self
    knots.push_back(&Astar_grid[lSelf][wSelf]);

    addObjectif(target.x,target.y);

}



bool AstarAlgo::Voronoi(){
    int min;
    //pour chaque noeud a regarder
    if(knots.size()!=0){
        min = minimum(knots);
        if (Astar_grid[objectif.x][objectif.y].color == 'b'){
            return false;
        }
        knots[min]->color = 'b';
        setNeighbour(knots[min]);
        knots.erase(knots.begin()+min);
        return true;
    }
    else{
        return false;
    }
}



void AstarAlgo::getPath(sf::Vector2f self, sf::Vector2f target, std::vector<sf::Vector2f> &path, int poid) {
    setPoids(poid);
    self = self/mMap->getBlockSize();
    //potencielement utile si on fait un algo pour chercher sur la map où est l'objectif
    //target = target/map->getBlockSize();
    initVoronoi(self, target);
    //partie qui run l'algo Astar
    while(Voronoi());
    AstarTile* tmp = &Astar_grid[objectif.x][objectif.y];
    while (tmp != nullptr){
        path.push_back(sf::Vector2f(tmp->coordNoeud.x,tmp->coordNoeud.y)*mMap->getBlockSize()+sf::Vector2f(1,1)*mMap->getBlockSize()/2.f);
        tmp = tmp->parent;
    }
    resetGraph();
}

/*
std::vector<sf::Vector2f> AstarAlgo::getPath(std::shared_ptr<TilesMap> &map,sf::Vector2f self, sf::Vector2f target, int poid) {
    setPoids(poid);
    self = self/map->getBlockSize();
    //potencielement utile si on fait un algo pour chercher
    //target = target/map->getBlockSize();
    initVoronoi(self, target);
    //partie qui run l'algo Astar
    while(Voronoi());
    std::vector<sf::Vector2f> path;
    AstarTile* tmp = &Astar_grid[objectif.x][objectif.y];
    while (tmp != nullptr){
        path.push_back(sf::Vector2f(tmp->coordNoeud.x,tmp->coordNoeud.y)*map->getBlockSize()+sf::Vector2f(1,1)*map->getBlockSize()/2.f);
        tmp = tmp->parent;
    }
    resetGraph();
    return path;
}
*/


int AstarAlgo::minimum(std::vector<AstarTile*> knots)const{
    int min = 1000000000;
    int index = 0;
    for(long unsigned int i = 0;i<knots.size();i++){
        if (knots[i]->dist < min){
            min = knots[i]->dist;
            index = i;
        }
    }
    return index;
}



double AstarAlgo::distance(sf::Vector2i ind)const {
    //heuristique Euclidean distance
    //double num = std::sqrt(std::pow(ind.y-objectif.y,double(2))+ std::pow(ind.x-objectif.x,double(2)));
    //heuristique chebyshev distance
    double num = std::max(std::pow(ind.y-objectif.y,double(2)),std::pow(ind.x-objectif.x,double(2)));
    return num;
}




void AstarAlgo::setNeighbour(AstarTile* knot) {
    cardinal c[8] = {South, North, Est, West, NorthEst, NorthWest, SouthEst, SouthWest};
    sf::Vector2i neiIndex;
    double dist;
    for(auto card : c){
        neiIndex = getNeighbourIndex(knot->coordNoeud.x,knot->coordNoeud.y,card);
        if (!indIsValid(neiIndex) || (Astar_grid[neiIndex.x][neiIndex.y].color == 'b') || !mMap->getTile(neiIndex.x,neiIndex.y).isCrossable()
            || crossCorner(neiIndex,card)) continue;
        //pour le calcule du cout d'une case
        float moveSpeed = mMap->getTile(knot->coordNoeud).getMoveSpeed();
        dist = (distance(neiIndex)*poids+static_cast<float>(knot->cout))/(moveSpeed*1000);
        if (Astar_grid[neiIndex.x][neiIndex.y].color == 'g'){
            if (dist < Astar_grid[neiIndex.x][neiIndex.y].dist ){
                Astar_grid[neiIndex.x][neiIndex.y].dist = dist;
                Astar_grid[neiIndex.x][neiIndex.y].parent = knot;
                Astar_grid[neiIndex.x][neiIndex.y].cout = knot->cout + 1;
            }
        }
        else { // if color == 'w'
            Astar_grid[neiIndex.x][neiIndex.y].dist = dist;
            Astar_grid[neiIndex.x][neiIndex.y].color = 'g';
            Astar_grid[neiIndex.x][neiIndex.y].coordNoeud = neiIndex;
            Astar_grid[neiIndex.x][neiIndex.y].parent = knot;
            Astar_grid[neiIndex.x][neiIndex.y].cout = knot->cout + 1;
            knots.push_back(&Astar_grid[neiIndex.x][neiIndex.y]);
        }
    }
}

sf::Vector2i AstarAlgo::getNeighbourIndex(int l,int w,cardinal c)const {
    sf::Vector2i coordNei;

    switch (c) {
        case cardinal::South : l++; if(l > length){ l = -1; w = -1;} break;
        case cardinal::North : l--; if(l < 0 ) {l = -1; w = -1;} break;
        case cardinal::Est   : w++; if ( w > width){ l = -1; w = -1;} break;
        case cardinal::West  : w--; if (w < 0 ) {l = -1; w = -1;} break;
        case cardinal::NorthEst :
            if (l-1 < 0 || w+1 > width){l = -1; w = -1;}
            else {l--; w++;}
            break;
        case cardinal::NorthWest :
            if (l-1 < 0 || w-1 < 0) {l = -1; w = -1;}
            else {l--; w--;}
            break;
        case cardinal::SouthEst   :
            if (l+1 > length || w+1 > width) {l = -1; w = -1;}
            else {l++; w++;}
            break;
        case cardinal::SouthWest  :
            if ( l+1 > length || w-1 < 0 ) {l = -1; w = -1;}
            else {l++; w--;}
            break;
        default :break;
    }
    coordNei.x = l; coordNei.y = w;
    return coordNei;
}



bool AstarAlgo::crossCorner(sf::Vector2i ind, cardinal card) {
    sf::Vector2i coord, coord2;
    switch (card) {
        case NorthEst :
            coord = getNeighbourIndex(ind.x,ind.y,South);
            coord2 = getNeighbourIndex(ind.x,ind.y,West);
            break;
        case NorthWest :
            coord = getNeighbourIndex(ind.x,ind.y,South);
            coord2 = getNeighbourIndex(ind.x,ind.y,Est);
            break;
        case SouthEst :
            coord = getNeighbourIndex(ind.x,ind.y,North);
            coord2 = getNeighbourIndex(ind.x,ind.y,West);
            break;
        case SouthWest :
            coord = getNeighbourIndex(ind.x,ind.y,North);
            coord2 = getNeighbourIndex(ind.x,ind.y,Est);
            break;
        default : // pour les cas Nort Sud Est West retourne false pour ne pas skip
            return false;
    }
    if(indIsValid(coord) && indIsValid(coord2)) {
        if (!mMap->getTile(coord.x,coord.y).isCrossable() || !mMap->getTile(coord2.x,coord2.y).isCrossable()) {
            return true;
        }
    }


    else return false;
}

bool AstarAlgo::indIsValid(sf::Vector2i ind) {
    return ( ind.x >= 0 && ind.y >= 0);
}









