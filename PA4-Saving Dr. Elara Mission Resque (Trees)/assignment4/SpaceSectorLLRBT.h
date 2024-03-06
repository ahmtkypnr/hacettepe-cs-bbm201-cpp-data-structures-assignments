#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector *rotateLeft(Sector *current_sector);

    Sector *rotateRight(Sector *current_sector);

    void flipColors(Sector *current_sector);

    //Sector *insertSectorToTree(Sector *current_sector, Sector *sector_to_add);

    Sector *insertSectorToTree(Sector *current_sector, Sector *sector_to_add, Sector *parent);
};

#endif // SPACESECTORLLRBT_H
