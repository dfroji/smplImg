#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#include "image.h"

// Parse option from arguments
char* get_option(char** begin, char** end, const std::string& option) {
    char** iter = std::find(begin, end, option);
    if (iter != end && (iter + 1) != end) {
        return *(iter + 1);
    }
    return nullptr;
}

// Parse filter options from arguments
std::vector<char*> get_filter_option(char** begin, 
                                            char** end, 
                                            const std::string& option
                                            ) {
    char** iter = std::find(begin, end, option);
    if (iter != end && iter + 2 != end) {
        return {*(iter + 1), *(iter + 2)};
    } else if (iter != end && iter + 1 != end) {
        return {*(iter + 1), nullptr};
    }
    return {nullptr, nullptr};

}

bool invalid_filter(){
    std::cout << "Error: invalid filter option" << std::endl;
    return 1;
}

int main(int argc, char* argv[]) {

    // Parse paths to input and output
    // Exit if one is not found
    const char* input = get_option(argv, argv+argc, "-i");
    const char* output = get_option(argv, argv+argc, "-o");
    if (input == nullptr || output == nullptr) {
        std::cout << "Error: input or output path not found" << std::endl;
        return 1;
    }

    // Parse filter options
    // Exit if invalid filter or filter option
    std::vector<char*> filter_arg = get_filter_option(argv, argv+argc, "-h");
    const char* filter = filter_arg[0];
    const char* filter_option = filter_arg[1];
    if (filter == nullptr) {
        return invalid_filter();
    } else {
        // Exit if not a filter (placeholder solution until more filters are added)
        if (strcmp(filter, "median") && 
            strcmp(filter, "highboost") && 
            strcmp(filter, "dhighboost")
            ) {
            return invalid_filter();
        }
        // Exit if invalid option for valid filter
        if (!strcmp(filter, "median") && (filter_option == nullptr 
                                          || (atoi(filter_option)) < 3 
                                          || atoi(filter_option) % 3 != 0
                                         )) {
                
            std::cout 
            << "Error: option for median filter should be a multiple of 3" 
            << std::endl;
            return 1;
        } else if ((!strcmp(filter, "highboost") || !(strcmp(filter, "dhighboost"))) 
                    && (filter_option == nullptr)) {
            std::cout 
            << "Error: highboost filter should have an integer coefficient option"
            << std::endl;
            return 1;
        }
    }

    Image image = Image();

    // Decode, filter and encode image
    // Exit if decoding or encoding is unsuccessful
    if (image.decode_image(input)) return 1;
    image.filter(filter, filter_option);
    if (image.encode_image(output)) return 1;

    return 0;
}

