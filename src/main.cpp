#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <format>

#include "club.hpp"

constexpr static const char *USAGE = "club-stat <input_file>";

static time_t read_time(std::istream &input) 
{
    int hours = 0, minutes = 0;
    char dec, unit, sep;
    input >> dec >> unit;
    hours = 10 * (dec - '0') + (unit - '0');
    input >> sep;
    input >> dec >> unit;
    minutes = 10 * (dec - '0') + (unit - '0');
    return 3600 * hours + 60 * minutes;
}

static void proccess(std::istream &input) 
{
    int tables_count, price;
    input >> tables_count;
    std::time_t open_time = read_time(input);
    std::time_t close_time = read_time(input);
    input >> price;
    Club club(tables_count, open_time, close_time, price);
    std::string line;
    std::cout << std::format("{:02}:{:02}", open_time / 3600, (open_time % 3600) / 60) << '\n';
    while (std::getline(input, line)) {
        std::stringstream event(line);
        std::time_t ev_time = read_time(event);
        int id, table_num;
        std::string name;
        event >> id >> name;
        switch (id)
        {
        case 1:
            club.new_client_1(ev_time, name, std::cout);
            break;
        case 2:
            event >> table_num;
            club.take_table_2(ev_time, name, table_num, std::cout);
            break;
        case 3:
            club.wait_3(ev_time, name, std::cout);
            break;
        case 4:
            club.client_gone_4(ev_time, name, std::cout);
            break;
        }
    }
    club.close(std::cout);
}

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
    proccess(input);

    return EXIT_SUCCESS;
}