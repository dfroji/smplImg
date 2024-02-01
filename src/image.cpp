#include "image.h"

#include <iostream>

#include "lodepng.h"

bool Image::decode_image(const char* filename) {

    unsigned int width, height;
    lodepng::State state;
    std::vector<unsigned char> data;
    std::vector<unsigned char> buffer;

    // Load given file
    lodepng::load_file(buffer, filename);

    // Try to decode given file
    unsigned error = lodepng::decode(data, width, height, state, buffer);
 
    // If decoding returned an error
    if(error) { 
        std::cout 
            << "Decoder error " << error << ": " 
            << lodepng_error_text(error) 
            << std::endl;
        return 1;
    }


    width_ = width;
    height_ = height;
    bitdepth_ = state.info_png.color.bitdepth;
    channels_ = lodepng_get_channels(&state.info_png.color);

    return 0;
}

bool Image::encode_image(const char* filename) {

    unsigned int width, height;
    std::vector<unsigned char> data = encode_data_();

    // Try to encode data to given file path
    unsigned error = lodepng::encode(filename, data, width_, height_);
    
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

//// PRIVATE ////

void Image::decode_data_(std::vector<unsigned char> data) {
    
}

std::vector<unsigned char> Image::encode_data_() {
    std::vector<unsigned char> data;
    return data;
}
