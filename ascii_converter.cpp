#include "ascii_converter.hpp"

// This gradient maps pixel intensity (0=darkest, 255=lightest) to chars.
static const std::string GRADIENT = "@%#*+=-:. ";

std::vector<std::string> convertToASCII(const std::vector<std::vector<int>> &image) {
    std::vector<std::string> asciiArt;
    if (image.empty() || image[0].empty()) {
        return asciiArt; // empty image
    }

    int height = (int)image.size();
    int width = (int)image[0].size();
    asciiArt.reserve(height);

    // Account for characters being taller than they are wide (divide vertical res by 3)
    for (int y = 0; y < height; y += 3) {
        std::string line;
        line.reserve(width);
        for (int x = 0; x < width; ++x) {
            int pixel = image[y][x]; 
            // Pixel assumed 0 to 255
            // int index = (pixel * (int)GRADIENT.size()) / 256;

            // Invert the grayscale before indexing:
            int invertedPixel = 255 - pixel;
            int index = (invertedPixel * (int)GRADIENT.size()) / 256;
            
            if (index >= (int)GRADIENT.size()) index = (int)GRADIENT.size() - 1;
            line.push_back(GRADIENT[index]);
        }
        asciiArt.push_back(line);
    }

    return asciiArt;
}
