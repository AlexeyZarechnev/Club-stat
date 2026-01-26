#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <format>

#include "club.hpp"

constexpr static const char *USAGE = "club-stat <input_file>";

int main(int argc, char **argv) 
{
    if (argc != 2) {
        std::cerr << USAGE;
        return EXIT_FAILURE;
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "file not found: " << argv[1];
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}