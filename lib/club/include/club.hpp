#pragma once

#include "time.hpp"

namespace club {

class Club {
public:
    Club(int tables_count, Time open_time, Time close_time, int price) noexcept;

    Club(const Club &other) noexcept;
    Club(Club &&other) noexcept;

    Club &operator=(const Club &other) noexcept;
    Club &operator=(Club &&other) noexcept;

private:
    
};
} // namespace club
