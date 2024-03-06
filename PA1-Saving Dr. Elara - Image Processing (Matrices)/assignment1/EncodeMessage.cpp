#include "EncodeMessage.h"
#include <cmath>
#include <iostream>
#include <bits/stdc++.h>



// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {

}

bool EncodeMessage::isPrime(int number) {
    if (number < 2) {
        return false;
    }
    for (int i = 2; i < number; ++i) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

int EncodeMessage::fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else{
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    std::string copy_message = message;
    for (int i = 0; i < message.size(); ++i) {
        if (isPrime(i)) {
            int asci = int(message[i]) + fibonacci(i);
            if (asci <= 32) {
                asci += 33;
            } else if (asci >= 127) {
                asci = 126;
            }
            copy_message[i] = char(asci);
        }
    }

    int half;
    if (copy_message.size() % 2 == 0) {
        half = copy_message.size() / 2;
    } else {
        half = copy_message.size() / 2 + 1;
    }

    std::string head = copy_message.substr(0, half);
    std::string tail = copy_message.substr(half);
    std::string manipulated = tail + head;

    std::string binary_code = "";
    for (int i = 0; i < manipulated.size(); ++i) {
        int asci2 = manipulated[i];
        std::bitset<7> code(asci2);
        binary_code += code.to_string();
    }

    ImageMatrix result_img = img;

    for (int i = 0; i < positions.size(); ++i) {
        if (i >= binary_code.size()) {
            break;
        }
        int row = positions[i].first;
        int column = positions[i].second;

        std::bitset<8> binary((int) img.get_data(row, column));

        std::bitset<1> bitcode(binary_code[i]);

        binary.set(0, bitcode[0]);

        result_img.setData((double) binary.to_ulong(), row, column);
    }
    return result_img;
}
