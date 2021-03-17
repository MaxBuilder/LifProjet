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


AstarAlgo::AstarAlgo(TilesMap &map) {
    length = 64;
    width = 32;
    for (unsigned int x(0); x < length; x++) {
        for (unsigned int y(0); y < width; y++) {
            //il faut copier la map
            Astar_grid[x][y].moveSpeed = map.getTile(x, y).getMoveSpeed();
            Astar_grid[x][y].isCrossable = map.getTile(x, y).isCrossable();
            Astar_grid[x][y].setCoordinate(x, y);
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

coordinate AstarAlgo::getObjectif() {
    return objectif;
}



void AstarAlgo::setPoids(int p) {
    poids = p;
}




//l et w les coordonnées de self et lt et wt les coordonnées de la target
void AstarAlgo::initVoronoi(int lSelf,int wSelf, int lTarget, int wTarget){
    Astar_grid[lSelf][wSelf].color = 'g';
    Astar_grid[lSelf][wSelf].dist = 0;
    Astar_grid[lSelf][wSelf].coordNoeud.x = lSelf;
    Astar_grid[lSelf][wSelf].coordNoeud.y = wSelf;
    //pour commencer depuis self
    knots.push_back(&Astar_grid[lSelf][wSelf]);

    addObjectif(lTarget,wTarget);

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
        /*
        for (long unsigned int k(0);k<knots.size();k++) map.drawColor(knots[k]->coordNoeud);
        map.drawColor(knots[min]->coordNoeud);*/
        knots.erase(knots.begin()+min);
        return true;
    }
    else{
        return false;
    }
}



void AstarAlgo::getPath(std::shared_ptr<TilesMap> &map,int lSelf,int wSelf, int ltarget, int wTarget, std::vector<sf::Vector2f> &path, int poid) {
    setPoids(poid);
    initVoronoi(lSelf, wSelf, ltarget, wTarget);
    //partie qui run l'algo Astar
    while(Voronoi());

    AstarTile* tmp = &Astar_grid[objectif.x][objectif.y];
    while (tmp != nullptr){
        path.push_back(sf::Vector2f(tmp->coordNoeud.x,tmp->coordNoeud.y)*map->getBlockSize()+sf::Vector2f(1,1)*map->getBlockSize()/2.f);
        tmp = tmp->parent;
    }
    resetGraph();
}



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



double AstarAlgo::distance(coordinate ind)const {
    //heuristique Euclidean distance
    //double num = std::sqrt(std::pow(ind.y-objectif.y,double(2))+ std::pow(ind.x-objectif.x,double(2)));
    //heuristique chebyshev distance
    double num = std::max(std::pow(ind.y-objectif.y,double(2)),std::pow(ind.x-objectif.x,double(2)));
    return num;
}




void AstarAlgo::setNeighbour(AstarTile* knot) {
    cardinal c[8] = {South, North, Est, West, NorthEst, NorthWest, SouthEst, SouthWest};
    coordinate neiIndex;
    double dist;
    for(auto card : c){
        neiIndex = getNeighbourIndex(knot->coordNoeud.x,knot->coordNoeud.y,card);
        if (!indIsValid(neiIndex) || (Astar_grid[neiIndex.x][neiIndex.y].color == 'b') || !Astar_grid[neiIndex.x][neiIndex.y].isCrossable
            || crossCorner(neiIndex,card)) continue;
        //pour le calcule du cout d'un
        dist = (distance(neiIndex)/knot->getMoveSpeed())*poids+knot->cout;
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

coordinate AstarAlgo::getNeighbourIndex(int l,int w,cardinal c)const {
    coordinate coordNei;

    switch (c) {
        case cardinal::South : l++; if((unsigned int)l >= length){ l = -1; w = -1;} break;
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



bool AstarAlgo::crossCorner(coordinate ind, cardinal card) {
    coordinate coord, coord2;
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

    if (!Astar_grid[coord.x][coord.y].isCrossable || !Astar_grid[coord2.x][coord2.y].isCrossable)
        return true;
    else return false;
}

bool AstarAlgo::indIsValid(coordinate ind) {
    return ( ind.x >= 0 && ind.y >= 0);
}









