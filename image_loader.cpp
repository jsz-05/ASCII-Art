#include "image_loader.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

// We will parse a 24-bit BMP file manually.

// BMP File format (simplified):
// [BITMAPFILEHEADER] (14 bytes)
// [BITMAPINFOHEADER] (40 bytes)
// [Pixel Data]

// BITMAPFILEHEADER (14 bytes total):
//  2 bytes: signature 'BM'
//  4 bytes: file size (uint32_t)
//  2 bytes: reserved
//  2 bytes: reserved
//  4 bytes: offset to pixel data (uint32_t)

// BITMAPINFOHEADER (40 bytes):
//  4 bytes: header size (40)
//  4 bytes: width (int32_t)
//  4 bytes: height (int32_t)
//  2 bytes: planes (must be 1)
//  2 bytes: bits per pixel (we want 24)
//  4 bytes: compression (0 = BI_RGB)
//  4 bytes: image size (may be 0 for BI_RGB)
//  4 bytes: horizontal resolution
//  4 bytes: vertical resolution
//  4 bytes: colors in color table
//  4 bytes: important color count

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType;      // 'BM'
    uint32_t bfSize;      // File size
    uint16_t bfReserved1; 
    uint16_t bfReserved2; 
    uint32_t bfOffBits;   // Offset to pixel data
};

struct BMPInfoHeader {
    uint32_t biSize;          // 40
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;        // must be 1
    uint16_t biBitCount;      // we want 24
    uint32_t biCompression;   // must be 0 (BI_RGB)
    uint32_t biSizeImage;     
    int32_t  biXPelsPerMeter; 
    int32_t  biYPelsPerMeter; 
    uint32_t biClrUsed;       
    uint32_t biClrImportant;  
};
#pragma pack(pop)

bool loadImage(const std::string &filepath, std::vector<std::vector<int>> &image) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filepath << "\n";
        return false;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // Read file header
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (!file.good()) {
        std::cerr << "Failed to read BMP file header\n";
        return false;
    }

    // Check signature
    if (fileHeader.bfType != 0x4D42) { // 'BM' in little-endian
        std::cerr << "Not a BMP file.\n";
        return false;
    }

    // Read info header
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    if (!file.good()) {
        std::cerr << "Failed to read BMP info header\n";
        return false;
    }

    // Basic validation
    if (infoHeader.biPlanes != 1) {
        std::cerr << "Invalid BMP: planes not 1.\n";
        return false;
    }
    if (infoHeader.biBitCount != 24) {
        std::cerr << "This loader only supports 24-bit BMP.\n";
        return false;
    }
    if (infoHeader.biCompression != 0) {
        std::cerr << "This loader only supports uncompressed BMP (BI_RGB).\n";
        return false;
    }

    // Move to pixel data offset
    file.seekg(fileHeader.bfOffBits, std::ios::beg);
    if (!file.good()) {
        std::cerr << "Failed to seek to pixel data.\n";
        return false;
    }

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    bool flipVertical = true;

    // According to BMP spec, if height > 0, the image is bottom-up.
    // We'll read it and then invert it to have top-down indexing in our vector.
    if (height < 0) {
        // if height is negative, BMP data is top-down already
        height = -height;
        flipVertical = false;
    }

    // Each row is padded to a multiple of 4 bytes
    int rowSize = ((width * 3 + 3) / 4) * 4; 
    std::vector<unsigned char> rowData(rowSize);

    // Initialize image
    image.assign(height, std::vector<int>(width, 0));

    // Read pixel data
    // BMP stores in BGR format
    // If bottom-up, the first row in file is the bottom of the image.
    for (int y = 0; y < height; ++y) {
        file.read(reinterpret_cast<char*>(rowData.data()), rowSize);
        if (!file.good()) {
            std::cerr << "Failed to read pixel data.\n";
            return false;
        }

        for (int x = 0; x < width; ++x) {
            unsigned char B = rowData[x * 3 + 0];
            unsigned char G = rowData[x * 3 + 1];
            unsigned char R = rowData[x * 3 + 2];

            int gray = (R + G + B) / 3; 

            // If we are bottom-up, line y in file is line (height - 1 - y) in our image
            int targetY = flipVertical ? (height - 1 - y) : y;
            image[targetY][x] = gray;
        }
    }

    return true;
}
