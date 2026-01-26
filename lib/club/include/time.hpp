#pragma once

#include <cstdint>
#include <iostream>

namespace club {

class Time {
public:

    Time() noexcept = default;
    Time(int hours, int minutes) noexcept;

    Time(const Time &other) noexcept = default;
    Time(Time &&other) noexcept = default;

    Time &operator=(const Time &other) noexcept = default;
    Time &operator=(Time &&other) noexcept = default;

    int hours() const noexcept;
    int minutes() const noexcept;

    Time operator+ (Time other) const noexcept;
    Time& operator+= (Time other) noexcept;

    ~Time() noexcept = default;

    friend std::istream &operator>>(std::istream &is, Time &time);
    friend std::ostream &operator<<(std::ostream &os, const Time &time);

    friend bool operator<(const Time &lhs, const Time &rhs) noexcept;
    friend bool operator<=(const Time &lhs, const Time &rhs) noexcept;
    friend bool operator>(const Time &lhs, const Time &rhs) noexcept;
    friend bool operator>=(const Time &lhs, const Time &rhs) noexcept;
    friend bool operator==(const Time &lhs, const Time &rhs) noexcept;
    friend bool operator!=(const Time &lhs, const Time &rhs) noexcept = default;
private:
    uint16_t _minutes_since_midnight;
};

} // namespace club
