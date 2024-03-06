#include <algorithm>
#include "KD_Tree.h"

// Default constructor implementation
KD_Tree::KD_Tree() : root(nullptr), split_threshold(0.1) {
}

// Parameterized constructor implementation
KD_Tree::KD_Tree(double threshold) : root(nullptr), split_threshold(threshold) {
}

// Destructor implementation
KD_Tree::~KD_Tree() {
    // Implementation for safely deleting the KD_Tree and its nodes
}

/*KDTreeNode* buildTree(std::vector<Point*>& points, int depth) {
    if (points.empty()) {
        return nullptr;
    }

    int k = points[0]->features.size();
    int splitDimension = depth % k;

    // Sort points along the chosen dimension
    std::sort(points.begin(), points.end(),
              [splitDimension](const Point* a, const Point* b) {
                  return a->features[splitDimension] < b->features[splitDimension];
              });

    // Find the median
    int medianIndex = points.size() / 2;
    Point* medianPoint = points[medianIndex];

    // Create internal node
    KDTreeNode* node = new KDTreeNode(medianPoint);
    node->splitDimension = splitDimension;
    node->splitValue = medianPoint->features[splitDimension];

    // Recursively build subtrees
    std::vector<Point*> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<Point*> rightPoints(points.begin() + medianIndex + 1, points.end());

    node->left = buildTree(leftPoints, depth + 1);
    node->right = buildTree(rightPoints, depth + 1);

    return node;
}*/

void KD_Tree::build(Dataset& data) {
    // Implementation for building the KD_Tree using the provided dataset
}

KDTreeNode* KD_Tree::getRoot() {
    // Implementation to return the root of the KD_Tree
    return nullptr;
}

// Add any additional function as you wish
