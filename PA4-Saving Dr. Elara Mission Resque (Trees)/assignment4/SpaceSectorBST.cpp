#include "SpaceSectorBST.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

void destruct(Sector* node) {
    if (node != nullptr) {
        destruct(node->left);
        destruct(node->right);
        delete node;
    }
}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    destruct(root);
}

bool compare(Sector* first, Sector* second) {
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

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    Sector* sector = new Sector(x, y, z);
    if (root == nullptr) {
        root = sector;
    }
    else {
        Sector* current = root;
        while (true) {
            if (compare(sector, current)) {
                if (current->right == nullptr) {
                    sector->parent = current;
                    current->right = sector;
                    break;
                }
                current = current->right;
            }
            else {
                if (current->left == nullptr) {
                    sector->parent = current;
                    current->left = sector;
                    break;
                }
                current = current->left;
            }
        }
    }
}

void processLeftmost(Sector* &nodePtr, Sector& treeItem){
    if (nodePtr->left == nullptr) {
        treeItem = *nodePtr;
        Sector *delPtr = nodePtr;
        if (nodePtr->right != nullptr) {
            nodePtr->right->parent = nodePtr->parent;
        }
        nodePtr = nodePtr->right;
        delPtr->right = nullptr;
        delete delPtr;
    }
    else
        processLeftmost(nodePtr->left, treeItem);
}

void deleteNodeItem(Sector* &node) {
    Sector* temp;

    if (node->left == nullptr && node->right == nullptr) {
        delete node;
        node = nullptr;
    }
    else if (node->left == nullptr) {
        temp = node;
        node = node->right;
        node->parent = temp->parent;
        temp->right = nullptr;
        delete temp;
    }
    else if (node->right == nullptr) {
        temp = node;
        node = node->left;
        node->parent = temp->parent;
        temp->left = nullptr;
        delete temp;
    }
    else {
        Sector replacementItem(0, 0, 0);
        processLeftmost(node->right, replacementItem);
        *node = replacementItem;
    }
}

void traverse(Sector* &treePtr, const std::string& sector_code) {
    if (treePtr != nullptr) {
        if (sector_code == treePtr->sector_code) {
            deleteNodeItem(treePtr);
        }
        else {
            traverse(treePtr->left, sector_code);
            traverse(treePtr->right, sector_code);
        }
    }
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    traverse(root, sector_code);
}

void preorder(Sector* treePtr) {
    if (treePtr != nullptr) {
        cout << treePtr->sector_code << endl;
        preorder(treePtr->left);
        preorder(treePtr->right);
    }
}

void inorder(Sector* treePtr) {
    if (treePtr != nullptr) {
        inorder(treePtr->left);
        cout << treePtr->sector_code << endl;
        inorder(treePtr->right);
    }
}

void postorder(Sector* treePtr) {
    if (treePtr != nullptr) {
        postorder(treePtr->left);
        postorder(treePtr->right);
        cout << treePtr->sector_code << endl;
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorder(root);
    cout << endl;
}

void find(Sector* treePtr, const std::string& sector_code, Sector* &result) {
    if (treePtr != nullptr) {
        if (sector_code == treePtr->sector_code) {
            result = treePtr;
        }
        else {
            find(treePtr->left, sector_code, result);
            find(treePtr->right, sector_code, result);
        }
    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* sector = nullptr;
    find(root, sector_code, sector);
    if (sector != nullptr) {
        while (sector != nullptr) {
            path.push_back(sector);
            sector = sector->parent;
        }
    }
    reverse(path.begin(), path.end());
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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