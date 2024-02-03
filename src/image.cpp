#include "image.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>

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

void Image::filter(const char* filter, const char* filter_option) {
    if (!strcmp(filter, "median")) median_filter_(atoi(filter_option));
    if (!strcmp(filter, "highboost")) highboost_filter_(atoi(filter_option), false);
    if (!strcmp(filter, "dhighboost")) highboost_filter_(atoi(filter_option), true);
}

//// PRIVATE ////

void Image::decode_data_(std::vector<unsigned char> data) {
    int x = 0;
    int y = 0;
    for (int i = 0; i < data.size(); i += 4) {
        Pixel* pixel = new Pixel{0, 0, 0, static_cast<int>(pow(2, bitdepth_))};

        pixel->red = data[i];
        pixel->green = data[i+1];
        pixel->blue = data[i+2];
        pixel->alpha = data[i+3];
        original_data_.insert({Coordinate{x,y}, *pixel});
        filtered_data_.insert({Coordinate{x,y}, pixel});
        pixels_.push_back(pixel);

        if (x + 1 == width_) {
            x = 0;
            y++;
        } else {
            x++;
        }
    }
}

std::vector<unsigned char> Image::encode_data_() {
    std::vector<unsigned char> data;
    data.reserve(pixels_.size() * 4);
    for (Pixel* pixel : pixels_) {
        data.push_back(pixel->red);
        data.push_back(pixel->green);
        data.push_back(pixel->blue);
        data.push_back(pixel->alpha);
        
    }
    return data;
}

Pixel Image::get_pixel_at_(Coordinate xy, const int& fill) {

    // If the pixel is out of bounds, return a filler pixel.
    if (xy.x < 0 || xy.x >= width_ || xy.y < 0 || xy.y >= height_) {
        return Pixel({fill, fill, fill, fill});
    }

    return original_data_.at(xy);
}

std::vector<Pixel> Image::get_under_mask_(Coordinate middle, const int& size) {
    std::vector<Pixel> pixels;

    // Find the pixels under the mask, 
    // the filtered pixel being in the middle
    int min = -(size - 1) / 2;
    int max = (size - 1) / 2;
    for (int i = min; i < max + 1; i++) {
        for (int j = min; j <= max; j++) {
            pixels.push_back(get_pixel_at_({middle.x + i, middle.y + j}));
        }
    }

    return pixels;
}

void Image::median_filter_(const int& size) {

    // Process every pixel of the image
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {

            // Get the pixels neighbouring pixels
            std::vector<Pixel> pixels = get_under_mask_({x, y}, size);

            // Get each pixel's color values and get the median of each color
            // The new pixel's color values will be the medians

            std::vector<int> red_values;
            std::vector<int> green_values;
            std::vector<int> blue_values;
            
            for (const Pixel& pixel : pixels) {
                red_values.push_back(pixel.red);
                green_values.push_back(pixel.green);
                blue_values.push_back(pixel.blue);
            }

            std::sort(red_values.begin(), red_values.end());
            std::sort(green_values.begin(), green_values.end());
            std::sort(blue_values.begin(), blue_values.end());

            Pixel* filtered_pixel = filtered_data_.at({x,y});
            int median = (pixels.size()) / 2;
            filtered_pixel->red = red_values[median];
            filtered_pixel->green = green_values[median];
            filtered_pixel->blue = blue_values[median];

        }
    }
}

void Image::limit_color_value_(int& color_value) {
    if (color_value < 0) color_value = 0;
    else if (color_value > pow(2, bitdepth_) - 1) color_value = pow(2, bitdepth_);
}

void Image::highboost_filter_(const int& k, bool is_diagonal) {
    
    // Process every pixel
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {

            std::vector<Pixel> pixels = get_under_mask_({x,y}, 3);

            // Get the current pixel's color values as references
            // for performing the filtering
            Pixel* filtered_pixel = filtered_data_.at({x,y});
            int& red = filtered_pixel->red;
            int& green = filtered_pixel->green;
            int& blue = filtered_pixel->blue;

            red = 0;
            green = 0;
            blue = 0;

            if (is_diagonal) {
                for (int i = 0; i < pixels.size(); i++) {
                    Pixel p = pixels[i];

                    // Middle of the mask
                    if (i == pixels.size() / 2) {
                        red += p.red * (8*k+1);
                        green += p.green * (8*k+1);
                        blue += p.blue * (8*k+1);
                        
                    // Every neighbour
                    } else {
                        red -= p.red * k;
                        green -= p.green * k;
                        blue -= p.blue * k;
                    }
                }
            } else {
                for (int i = 0; i < pixels.size(); i++) {
                    Pixel p = pixels[i];

                    // Middle of the mask i.e. current pixel
                    if (i == pixels.size() / 2) {
                        red += p.red * (4*k+1);
                        green += p.green * (4*k+1);
                        blue += p.blue * (4*k+1);
                       
                    // Ignore corners of the mask
                    } else if (i != 0 && i != 2 && i != 6 && i != 8) {
                        red -= p.red * k;
                        green -= p.green * k;
                        blue -= p.blue * k;
                    }
                }
            }

            // Limit the color values to be within the bitdepth
            limit_color_value_(red);
            limit_color_value_(green);
            limit_color_value_(blue);

        }
    }
}
