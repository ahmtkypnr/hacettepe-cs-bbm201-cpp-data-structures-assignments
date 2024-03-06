#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening imageSharpening;
    EdgeDetector edgeDetector;
    DecodeMessage decodeMessage;
    ImageMatrix sharpened = imageSharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpened);
    return decodeMessage.decodeFromImage(sharpened, edges);
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening imageSharpening;
    EdgeDetector edgeDetector;
    EncodeMessage encodeMessage;
    ImageMatrix sharpened = imageSharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> positions = edgeDetector.detectEdges(sharpened);
    return encodeMessage.encodeMessageToImage(img, message, positions);
}
