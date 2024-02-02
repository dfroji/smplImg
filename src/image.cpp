#include "image.h"

#include <iostream>

#include "lodepng.h"

Image::Image() {

}

Image::~Image() {
    for (Pixel* pixel : pixels_) {
        delete pixel;
    }
}

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

    decode_data_(data);

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
    int x = 0;
    int y = 0;
    for (int i = 0; i < data.size(); i++) {
        Pixel* pixel = new Pixel{0, 0, 0, 0};
        switch (channels_) {
            case 1:
                pixel->red = data[i];
                data_.insert({Coordinate{x,y}, pixel});
                pixels_.push_back(pixel);
                break;
            case 2:
                pixel->red = data[i];
                pixel->alpha = data[i+1];
                data_.insert({Coordinate{x,y}, pixel});
                pixels_.push_back(pixel);
                i += 1;
                break;
            case 3:
                pixel->red = data[i];
                pixel->green = data[i+1];
                pixel->blue = data[i+2];
                data_.insert({Coordinate{x,y}, pixel});
                pixels_.push_back(pixel);
                i += 2;
                break;
            case 4:
                pixel->red = data[i];
                pixel->green = data[i+1];
                pixel->blue = data[i+2];
                pixel->alpha = data[i+3];
                data_.insert({Coordinate{x,y}, pixel});
                pixels_.push_back(pixel);
                i += 3;
                break;
        }

        if (x == width_ - 1) {
            x = 0;
            y++;
        } else {
            x++;
        }

    }
}

std::vector<unsigned char> Image::encode_data_() {
    std::vector<unsigned char> data;
    data.reserve(pixels_.size() * channels_);
    for (Pixel* pixel : pixels_) {
        switch (channels_) {
            case 1:
                data.push_back(pixel->red);
                break;
            case 2:
                data.push_back(pixel->red);
                data.push_back(pixel->alpha);
                break;
            case 3:
                data.push_back(pixel->red);
                data.push_back(pixel->green);
                data.push_back(pixel->blue);
                break;
            case 4:
                data.push_back(pixel->red);
                data.push_back(pixel->green);
                data.push_back(pixel->blue);
                data.push_back(pixel->alpha);
                break;
        }
    }
    return data;
}
