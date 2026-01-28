#include <cstddef>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#include "club.hpp"

constexpr static const char *USAGE = "club-stat <input_file>";

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << USAGE;
    return EXIT_FAILURE;
  }
  std::ifstream input(argv[1]);
  if (!input.is_open()) {
    std::cerr << argv[1];
    return EXIT_FAILURE;
  }
  std::stringstream sstream;
  std::string ev;
  std::size_t tables_count;
  std::getline(input, ev);
  sstream = std::stringstream(ev);
  if (!(sstream >> tables_count)) {
    std::cerr << ev;
    return EXIT_FAILURE;
  }
  club::Time open_time, close_time;
  std::getline(input, ev);
  sstream = std::stringstream(ev);
  if (!(sstream >> open_time >> close_time)) {
    std::cerr << ev;
    return EXIT_FAILURE;
  }
  std::size_t price;
  std::getline(input, ev);
  sstream = std::stringstream(ev);
  if (!(sstream >> price)) {
    std::cerr << ev;
    return EXIT_FAILURE;
  }
  club::Club club(tables_count, open_time, close_time, price);
  club::Time ev_time;
  std::size_t ev_id;
  std::string ev_name;
  std::size_t ev_table_id;
  while (std::getline(input, ev)) {
    sstream = std::stringstream(ev);
    if (!(sstream >> ev_time >> ev_id >> ev_name)) {
      std::cerr << ev;
      return EXIT_FAILURE;
    }
    switch (ev_id) {
    case 1: {
      club.arrive(ev_time, ev_name);
      break;
    }
    case 2: {
      if (!(sstream >> ev_table_id)) {
        std::cerr << ev;
        return EXIT_FAILURE;
      }
      club.take_table(ev_time, ev_name, ev_table_id);
      break;
    }
    case 3: {
      club.queue(ev_time, ev_name);
      break;
    }
    case 4: {
      club.leave(ev_time, ev_name);
      break;
    }
    default:
      std::cerr << ev;
      return EXIT_FAILURE;
    }
  }
  std::cout << open_time << '\n';
  auto &events = club.close();
  for (const auto &event : events) {
    std::cout << event << "\n";
  }
  std::cout << close_time << '\n';
  auto &stats = club.stats();
  for (std::size_t i = 1; i < stats.size(); ++i) {
    std::cout << i << " " << stats[i].second << " " << stats[i].first << "\n";
  }
  return EXIT_SUCCESS;
}