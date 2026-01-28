#include "time.hpp"
#include <cctype>
#include <sys/types.h>

namespace club {

std::istream &operator>>(std::istream &is, Time &time) noexcept {
  std::string input;
  is >> input;

  if (input.size() != 5 || input[2] != ':') {
    is.setstate(std::ios::failbit);
    return is;
  }
  int hours = (input[0] - '0') * 10 + (input[1] - '0');
  int minutes = (input[3] - '0') * 10 + (input[4] - '0');
  time = Time(hours, minutes);
  return is;
}

std::ostream &operator<<(std::ostream &os, const Time &time) noexcept {
  os << std::format("{:02}:{:02}", time.hours(), time.minutes());
  return os;
}

} // namespace club
