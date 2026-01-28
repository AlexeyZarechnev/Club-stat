#include "time.hpp"
#include <sys/types.h>

namespace club {

std::istream &operator>>(std::istream &is, Time &time) noexcept {
  uint16_t hours;
  uint16_t minutes;
  auto input = [&](uint16_t &dst) {
    char ch;
    is >> ch;
    dst = static_cast<uint16_t>(ch - '0') * 10;
    is >> ch;
    dst += static_cast<uint16_t>(ch - '0');
  };
  input(hours);
  is.ignore(); // skip ':'
  input(minutes);
  time = Time(hours, minutes);
  return is;
}

std::ostream &operator<<(std::ostream &os, const Time &time) noexcept {
  os << std::format("{:02}:{:02}", time.hours(), time.minutes());
  return os;
}

} // namespace club
