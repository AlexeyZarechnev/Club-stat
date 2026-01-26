#include "time.hpp"
#include <sys/types.h>

namespace club {

static constexpr uint16_t MINUTES_IN_HOUR = 60;
static constexpr uint16_t HOURS_IN_DAY = 24;
static constexpr uint16_t MINUTES_IN_DAY = HOURS_IN_DAY * MINUTES_IN_HOUR;

Time::Time(int hours, int minutes) noexcept
    : _minutes_since_midnight(static_cast<uint16_t>(hours * 60 + minutes)) {}

int Time::hours() const noexcept {
    return _minutes_since_midnight % MINUTES_IN_DAY / MINUTES_IN_HOUR;
}

int Time::minutes() const noexcept {
    return _minutes_since_midnight % MINUTES_IN_HOUR;
}

Time Time::operator+ (Time other) const noexcept {
    Time tmp = *this;
    tmp += other;
    return tmp;
}

Time& Time::operator+= (Time other) noexcept {
    _minutes_since_midnight += other._minutes_since_midnight;
    return *this;
}

bool operator<(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight < rhs._minutes_since_midnight;
}

bool operator<=(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight <= rhs._minutes_since_midnight;
}

bool operator>(const Time &lhs, const Time &rhs) noexcept {
    return !(lhs <= rhs);
}

bool operator>=(const Time &lhs, const Time &rhs) noexcept {
    return !(lhs < rhs);
}

bool operator==(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight == rhs._minutes_since_midnight;
}

std::istream &operator>>(std::istream &is, Time &time) {
    uint16_t hours;
    uint16_t minutes;
    auto input = [&](uint16_t& dst) {
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

std::ostream &operator<<(std::ostream &os, const Time &time) {
    os << std::format("{:02}:{:02}", time.hours(), time.minutes());
    return os;
}

} // namespace club
