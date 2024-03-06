// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {

}

// Destructor
EdgeDetector::~EdgeDetector() {

}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    double* gx[3] = {new double[3]{-1, 0, 1}, new double[3]{-2, 0, 2}, new double[3]{-1, 0, 1}};
    double* gy[3] = {new double[3]{-1, -2, -1}, new double[3]{0, 0, 0}, new double[3]{1, 2, 1}};

    Convolution convolutionGx(gx, 3, 3, 1, true);
    Convolution convolutionGy(gy, 3, 3, 1, true);

    for (int i = 0; i < 3; ++i) {
        delete[] gx[i];
        delete[] gy[i];
    }

    ImageMatrix ix = convolutionGx.convolve(input_image);
    ImageMatrix iy = convolutionGy.convolve(input_image);

    double magnitude = 0;
    for (int i = 0; i < ix.get_height(); ++i) {
        for (int j = 0; j < ix.get_width(); ++j) {
            magnitude += sqrt(pow(ix.get_data(i, j), 2) + pow(iy.get_data(i, j), 2));
        }
    }

    double threshold = magnitude / (ix.get_height() * ix.get_width());

    std::vector<std::pair<int, int>> edgePixels;

    for (int i = 0; i < ix.get_height(); ++i) {
        for (int j = 0; j < ix.get_width(); ++j) {
            magnitude = sqrt(pow(ix.get_data(i, j), 2) + pow(iy.get_data(i, j), 2));
            if (magnitude > threshold) {
                edgePixels.emplace_back(i, j);
            }
        }
    }
    return edgePixels;
}

