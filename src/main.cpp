#include <iostream>
#include <vector>
#include <algorithm>

#include "image.h"

// Parse option from arguments
char* get_option(char** begin, char** end, const std::string& option) {
    char** iter = std::find(begin, end, option);
    if (iter != end && (iter + 1) != end) {
        return *(iter + 1);
    }
    return 0;
}

int main(int argc, char* argv[]) {

    // Parse paths to input and output
    // Exit if one is not found
    const char* input = get_option(argv, argv+argc, "-i");
    const char* output = get_option(argv, argv+argc, "-o");
    if (input == 0 || output == 0) {
        std::cout << "Error: input or output path not found." << std::endl;
        return 1;
    }

    return 0;
}

