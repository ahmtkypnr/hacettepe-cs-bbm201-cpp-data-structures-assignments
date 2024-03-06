#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() {
    height = 0;
    width = 0;
    stride = 1;
    padding = false;
    kernel = nullptr;
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    height = kh;
    width = kw;
    stride = stride_val;
    padding = pad;

    kernel = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernel[i] = new double[width];
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if (kernel != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] this->kernel[i];
        }
        delete[] this->kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other) : Convolution(other.getKernel(), other.getHeight(), other.getWidth(), other.getStride(), other.isPadding()){

}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this;
    }
    this->height = other.getHeight();
    this->width = other.getWidth();
    this->stride = other.getStride();
    this->padding = other.isPadding();

    if (kernel != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] this->kernel[i];
        }
        delete[] this->kernel;
    }

    kernel = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernel[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            kernel[i][j] = other.getKernel(i, j);
        }
    }

    return *this;
}

ImageMatrix Convolution::addPadding(const ImageMatrix& input, int padding) {
    int height = input.get_height();
    int width = input.get_width();

    int padded_height = height + 2 * padding;
    int padded_width = width + 2 * padding;

    ImageMatrix padded_matrix(padded_height, padded_width);

    for (int i = padding; i < height + padding; ++i) {
        for (int j = padding; j < width + padding; ++j) {
            padded_matrix.setData(input.get_data(i - padding, j - padding), i, j);
        }
    }

    return padded_matrix;
}

// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    ImageMatrix input;

    if (padding) {
        input = addPadding(input_image, 1);
    } else {
        input = input_image;
    }


    int output_height = (input_image.get_height() - height + 2 * padding) / stride + 1;
    int output_width = (input_image.get_width() - width + 2 * padding) / stride + 1;

    ImageMatrix result(output_height, output_width);


    for (int i = 0; i < output_height; ++i) {
        for (int j = 0; j < output_width; ++j) {
            double sum = 0;
            for (int k = 0; k < height; ++k) {
                for (int l = 0; l < width; ++l) {
                    int input_row = i * stride + k;
                    int input_col = j * stride + l;
                    if (input_row >= 0 && input_col >= 0) {
                        if (input_row < input.get_height() && input_col < input.get_width()) {
                            sum = sum + input.get_data(input_row, input_col) * kernel[k][l];
                        }
                    }
                }
            }
            result.setData(sum, i, j);
        }
    }
    return result;
}

double **Convolution::getKernel() const {
    return kernel;
}

double Convolution::getKernel(int i, int j) const {
    return kernel[i][j];
}

int Convolution::getHeight() const {
    return height;
}

int Convolution::getWidth() const {
    return width;
}

int Convolution::getStride() const {
    return stride;
}

bool Convolution::isPadding() const {
    return padding;
}
