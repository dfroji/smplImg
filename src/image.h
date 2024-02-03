#pragma once

#include <vector>
#include <map>

struct Pixel {
    int red;
    int green;
    int blue;
    int alpha;
};

struct Coordinate {
    int x;
    int y;
};
inline bool operator<(Coordinate a, Coordinate b) {
    if (a.x < b.x) {return true;}
    else if (a.x > b.x) {return false;}
    else {return a.y < b.y;}
}

class Image {
    public:
        Image();
        ~Image();

        // Decode given file.
        // Return 0 if successful.
        bool decode_image(const char* filename);

        // Encode image to given file.
        // Return 0 if successful.
        bool encode_image(const char* filename);

        // Filter the image with the given option.
        void filter(const char* filter, const char* filter_option);

    private:
        std::map<Coordinate, Pixel> original_data_;
        std::map<Coordinate, Pixel*> filtered_data_;
        std::vector<Pixel*> pixels_;

        unsigned int width_;
        unsigned int height_;
        unsigned int bitdepth_;
        unsigned int channels_;

        // Generate data from the decoded image
        void decode_data_(std::vector<unsigned char> data);

        // Prepare data for encoding
        std::vector<unsigned char> encode_data_();

        // Return pixel at coordinate
        // If invalid Coordinate, return pixel with values given by fill
        Pixel get_pixel_at_(Coordinate xy, const int& fill = 0);

        // Get pixels under n*n-size mask
        std::vector<Pixel> get_under_mask_(Coordinate mid, const int& size);

        // Filter data with a median filter
        void median_filter_(const int& size);

        // Limit the given color value to correspond with bitdepth
        void limit_color_value_(int& color_value);

        // Filter data with a highboost filter
        void highboost_filter_(const int& k, bool is_diagonal);

};
