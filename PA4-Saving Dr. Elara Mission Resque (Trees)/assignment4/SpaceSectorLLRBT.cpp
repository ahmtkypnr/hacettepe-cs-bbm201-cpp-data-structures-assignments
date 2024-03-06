#include <algorithm>
#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    string x, y, z;

    while (std::getline(file, line)) {
        stringstream ss(line);

        std::getline(ss, x, ',');
        std::getline(ss, y, ',');
        std::getline(ss, z, ',');

        insertSectorByCoordinates(stoi(x), stoi(y), stoi(z));
    }
}

void destruct2(Sector* node) {
    if (node != nullptr) {
        destruct2(node->left);
        destruct2(node->right);
        delete node;
    }
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    destruct2(root);
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector* current_sector) {
    Sector* right = current_sector->right;
    current_sector->right = right->left;
    right->left = current_sector;
    if (current_sector->right != nullptr) {
        current_sector->right->parent = current_sector;
    }
    right->parent = current_sector->parent;
    current_sector->parent = right;
    right->color = current_sector->color;
    current_sector->color = RED;
    return right;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector* current_sector) {
    Sector* right = current_sector->left;
    current_sector->left = right->right;
    right->right = current_sector;
    if (current_sector->left != nullptr) {
        current_sector->left->parent = current_sector;
    }
    right->parent = current_sector->parent;
    current_sector->parent = right;
    right->color = current_sector->color;
    current_sector->color = RED;
    return right;
}

void SpaceSectorLLRBT::flipColors(Sector* current_sector) {
    current_sector->color = RED;
    current_sector->left->color = BLACK;
    current_sector->right->color = BLACK;
}

bool compare_nodes(Sector* first, Sector* second) {
    if (first->x == second->x) {
        if (first->y == second->y) {
            if (first->z == second->z) {
                return true;
            }
            else if (first->z > second->z) {
                return true;
            }
        }
        else if (first->y > second->y) {
            return true;
        }
    }
    else if (first->x > second->x) {
        return true;
    }
    return false;
}

Sector* SpaceSectorLLRBT::insertSectorToTree(Sector* current_sector,Sector* sector_to_add, Sector* parent){

    if(current_sector == nullptr){
        sector_to_add->parent = parent;
        return sector_to_add;
    }

    if(!compare_nodes(sector_to_add, current_sector)){
        current_sector->left = insertSectorToTree(current_sector->left,sector_to_add, current_sector);
    }

    if(compare_nodes(sector_to_add, current_sector)){
        current_sector->right = insertSectorToTree(current_sector->right,sector_to_add, current_sector);
    }

    if (current_sector->right && current_sector->right->color == RED) {
        current_sector = rotateLeft(current_sector);
    }
    if (current_sector->left && current_sector->left->color == RED && current_sector->left->left && current_sector->left->left->color == RED) {
        current_sector = rotateRight(current_sector);
    }
    if (current_sector->left && current_sector->left->color == RED && current_sector->right && current_sector->right->color == RED) {
        flipColors(current_sector);
    }

    return current_sector;

}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    Sector* sector = new Sector(x,y,z);

    root = insertSectorToTree(root,sector, nullptr);
    root->color = BLACK; // Ensure root is always black
}

void preorder_a(Sector* treePtr) {
    if (treePtr != nullptr) {
        string color;
        if (treePtr->color) {
            color = "RED";
        }
        else {
            color = "BLACK";
        }
        cout << color << " sector: " << treePtr->sector_code << endl;
        preorder_a(treePtr->left);
        preorder_a(treePtr->right);
    }
}

void inorder_a(Sector* treePtr) {
    if (treePtr != nullptr) {
        inorder_a(treePtr->left);
        string color;
        if (treePtr->color) {
            color = "RED";
        }
        else {
            color = "BLACK";
        }
        cout << color << " sector: " << treePtr->sector_code << endl;
        inorder_a(treePtr->right);
    }
}

void postorder_a(Sector* treePtr) {
    if (treePtr != nullptr) {
        postorder_a(treePtr->left);
        postorder_a(treePtr->right);
        string color;
        if (treePtr->color) {
            color = "RED";
        }
        else {
            color = "BLACK";
        }
        cout << color << " sector: " << treePtr->sector_code << endl;
    }
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorder_a(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorder_a(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorder_a(root);
    cout << endl;
}

void find_node(Sector* treePtr, const std::string& sector_code, Sector* &result) {
    if (treePtr != nullptr) {
        if (sector_code == treePtr->sector_code) {
            result = treePtr;
        }
        else {
            find_node(treePtr->left, sector_code, result);
            find_node(treePtr->right, sector_code, result);
        }
    }
}



std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* sector = nullptr;
    find_node(root, sector_code, sector);

    if (sector == nullptr) {
        return path;
    }

    Sector* temp = sector;
    std::vector<Sector*> parents;
    while (temp != nullptr) {
        parents.push_back(temp);
        temp = temp->parent;
    }

    Sector* earth = nullptr;
    find_node(root, "0SSS", earth);

    Sector* temp2 = earth;
    std::vector<Sector*> parents2;
    while (temp2 != nullptr) {
        parents2.push_back(temp2);
        temp2 = temp2->parent;
    }

    bool loop = false;
    for (int i = 0; i < parents.size(); ++i) {
        if (loop) {
            break;
        }
        for (int j = 0; j < parents2.size(); ++j) {
            if (parents[i] == parents2[j]) {
                loop = true;
                int x = parents.size();
                int y = parents2.size();
                for (int k = i; k < x; ++k) {
                    parents.pop_back();
                }
                for (int k = j + 1; k < y; ++k) {
                    parents2.pop_back();
                }
                break;
            }
        }
    }

    reverse(parents.begin(), parents.end());

    for (int i = 0; i < parents2.size(); ++i) {
        path.push_back(parents2[i]);
    }
    for (int i = 0; i < parents.size(); ++i) {
        path.push_back(parents[i]);
    }

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if (path.empty()) {
        cout << "A path to Dr. Elara could not be found.";
    }
    else {
        cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size(); ++i) {
            cout << path[i]->sector_code;
            if (i < path.size() - 1) {
                cout << "->";
            }
        }
    }
    cout << endl;
}