#pragma once

#include <cstdint>
#include <iostream>

namespace club {

class Time {
private:
  constexpr static std::uint16_t MINUTES_IN_HOUR = 60;
  constexpr static std::uint16_t MINUTES_IN_DAY = 1440;

public:
  constexpr Time() noexcept = default;
  constexpr Time(int hours, int minutes) noexcept
      : _minutes_since_midnight(static_cast<uint16_t>(hours * 60 + minutes)) {}

  constexpr Time(const Time &other) noexcept = default;
  constexpr Time(Time &&other) noexcept = default;

  constexpr Time &operator=(const Time &other) noexcept = default;
  constexpr Time &operator=(Time &&other) noexcept = default;

  constexpr std::size_t hours() const noexcept {
    return (_minutes_since_midnight % MINUTES_IN_DAY) / MINUTES_IN_HOUR;
  }
  constexpr std::size_t minutes() const noexcept {
    return _minutes_since_midnight % MINUTES_IN_HOUR;
  }
  constexpr std::size_t ceil() const noexcept {
    return hours() + (minutes() > 0 ? 1 : 0);
  }

  constexpr Time operator+(Time other) const noexcept {
    Time tmp = *this;
    tmp += other;
    return tmp;
  }
  constexpr Time &operator+=(Time other) noexcept {
    _minutes_since_midnight =
        _minutes_since_midnight + other._minutes_since_midnight;
    return *this;
  }
  constexpr Time operator-(Time other) const {
    Time tmp = *this;
    tmp -= other;
    return tmp;
  }
  constexpr Time &operator-=(Time other) {
    if (other > *this) {
      throw std::underflow_error("Time underflow!");
    }
    _minutes_since_midnight =
        _minutes_since_midnight - other._minutes_since_midnight;
    return *this;
  }

  constexpr ~Time() noexcept = default;

  friend std::istream &operator>>(std::istream &is, Time &time) noexcept;
  friend std::ostream &operator<<(std::ostream &os, const Time &time) noexcept;

  friend constexpr bool operator<(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight < rhs._minutes_since_midnight;
  }
  friend constexpr bool operator<=(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight <= rhs._minutes_since_midnight;
  }
  friend constexpr bool operator>(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight > rhs._minutes_since_midnight;
  }
  friend constexpr bool operator>=(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight >= rhs._minutes_since_midnight;
  }
  friend constexpr bool operator==(const Time &lhs, const Time &rhs) noexcept {
    return lhs._minutes_since_midnight == rhs._minutes_since_midnight;
  }
  friend constexpr bool operator!=(const Time &lhs,
                                   const Time &rhs) noexcept = default;

private:
  std::uint16_t _minutes_since_midnight;
};

} // namespace club
