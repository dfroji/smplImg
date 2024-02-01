#include "image.h"

#include <iostream>

#include "lodepng.h"

bool decode_image(std::vector<unsigned char>& image, 
                  unsigned int& width, 
                  unsigned int& height, 
                  const char* filename) {

    // Try to decode an image with the given filename
    unsigned int error = lodepng::decode(image, width, height, filename);
 
    // If decoding returned an error
    if(error) { 
        std::cout 
            << "Decoder error " << error << ": " 
            << lodepng_error_text(error) 
            << std::endl;
        return 1;
    }

    return 0;
}

bool encode_image(const char* filename,
                  std::vector<unsigned char>& image,
                  unsigned int& width, 
                  unsigned int& height) {

    // Try to encode data to given file path
    unsigned int error = lodepng::encode(filename, image, width, height);
    
    // If encoding returned an error
    if (error) {
        std::cout
            << "Encoder error: " << error << ": "
            << lodepng_error_text(error)
            << std::endl;
        return 1;
    }

    return 0;
}
