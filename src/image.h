#pragma once

#include <vector>

// Decode file with given filename to image.
// Return 0 if successful.
bool decode_image(std::vector<unsigned char>& image, 
                  unsigned int& width, 
                  unsigned int& height, 
                  const char* filename);


// Encode data from image to file with given filename.
// Return 0 if successful.
bool encode_image(const char* filename,
                  std::vector<unsigned char>& image,
                  unsigned int& width, 
                  unsigned int& height);
