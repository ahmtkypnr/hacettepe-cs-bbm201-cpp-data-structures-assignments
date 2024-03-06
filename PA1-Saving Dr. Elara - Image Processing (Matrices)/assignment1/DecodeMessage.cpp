// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>
#include <bitset>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string lsb = "";
    for (int i = 0; i < edgePixels.size(); ++i) {
        int row = edgePixels[i].first;
        int column = edgePixels[i].second;

        std::bitset<7> binary((int) image.get_data(row, column));
        lsb += std::to_string(binary[0]);
    }

    if (lsb.size() % 7 != 0) {
        int pad = 7 - (lsb.size() % 7);
        for (int i = 0; i < pad; ++i) {
            lsb = "0" + lsb;
        }
    }

    std::string part;
    std::string result = "";
    for (int i = 0; i < lsb.size(); i += 7) {
        part = lsb.substr(i, 7);
        std::bitset<7> code(part);

        int number = (int) code.to_ulong();

        if (number <= 32) {
            number += 33;
        } else if (number >= 127) {
            number = 126;
        }

        char letter = char(number);
        result += letter;
    }
    return result;
}

