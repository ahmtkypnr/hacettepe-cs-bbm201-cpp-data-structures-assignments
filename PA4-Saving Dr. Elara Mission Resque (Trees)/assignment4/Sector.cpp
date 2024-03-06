#include "Sector.h"
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    // TODO: Calculate the distance to the Earth, and generate the sector code
    this->x = x;
    this->y = y;
    this->z = z;
    this->distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    std::string code = std::to_string((int)floor(distance_from_earth));
    if (this->x == 0) {
        code.push_back('S');
    }
    else if (this->x > 0) {
        code.push_back('R');
    }
    else {
        code.push_back('L');
    }
    if (this->y == 0) {
        code.push_back('S');
    }
    else if (this->y > 0) {
        code.push_back('U');
    }
    else {
        code.push_back('D');
    }
    if (this->z == 0) {
        code.push_back('S');
    }
    else if (this->z > 0) {
        code.push_back('F');
    }
    else {
        code.push_back('B');
    }
    this->sector_code = code;
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    if (this == &other) {

    }
    else {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->distance_from_earth = other.distance_from_earth;
        this->sector_code = other.sector_code;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}