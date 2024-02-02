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

    private:
        std::map<Coordinate, Pixel*> data_;
        std::vector<Pixel*> pixels_;

        unsigned int width_;
        unsigned int height_;
        unsigned int bitdepth_;
        unsigned int channels_;

        // Generate data_ from the decoded image
        void decode_data_(std::vector<unsigned char> data);

        // Prepare data_ for encoding
        std::vector<unsigned char> encode_data_();

};
