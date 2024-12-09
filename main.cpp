#include <iostream>
#include <string>
#include "image_loader.hpp"
#include "ascii_converter.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_bmp_file>\n";
        return 1;
    }

    std::string filepath = argv[1];

    // Load the image from BMP
    std::vector<std::vector<int>> image;
    if (!loadImage(filepath, image)) {
        std::cerr << "Failed to load image: " << filepath << "\n";
        return 1;
    }

    // Convert to ASCII
    std::vector<std::string> asciiArt = convertToASCII(image);

    // Print ASCII art
    for (const auto &line : asciiArt) {
        std::cout << line << "\n";
    }

    return 0;
}
