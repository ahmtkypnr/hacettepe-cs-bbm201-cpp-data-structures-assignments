#include "ImageSharpening.h"
#include <iostream>

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = 3;
    kernel_width = 3;
    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double[kernel_width];
        for (int j = 0; j < kernel_width; ++j) {
            blurring_kernel[i][j] = 1.0/9;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    for (int i = 0; i < kernel_height; ++i) {
        delete[] this->blurring_kernel[i];
    }
    delete[] this->blurring_kernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution convolution(blurring_kernel, kernel_height, kernel_width, 1, true);
    ImageMatrix blurredImage = convolution.convolve(input_image);
    ImageMatrix sharpImg = (input_image - blurredImage) * k + input_image;
    for (int i = 0; i < sharpImg.get_height(); ++i) {
        for (int j = 0; j < sharpImg.get_width(); ++j) {
            if (sharpImg.get_data(i, j) > 255) {
                sharpImg.setData(255, i, j);
            } else if (sharpImg.get_data(i, j) < 0) {
                sharpImg.setData(0, i, j);
            }
        }
    }
    return sharpImg;
}
