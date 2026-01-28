#pragma once

#include "time.hpp"
#include <ostream>
#include <queue>
#include <unordered_map>

namespace club {

struct Event {
  Event(Time time, std::size_t id, const char *name) noexcept;

  Event(Time time, std::size_t id, const std::string &name) noexcept;

  Event(Time time, std::size_t id, std::string &&name) noexcept;

  Event(Time time, std::size_t id, const std::string &name,
        std::size_t table_id) noexcept;

  Time time;
  std::size_t id;
  std::string name;
  std::optional<std::size_t> table_id;
};

std::ostream &operator<<(std::ostream &os, const Event &event) noexcept;

class Club {
public:
  using Client = std::string;
  Club(size_t tables_count, Time open_time, Time close_time, std::size_t price);

  Club(const Club &other) noexcept = default;
  Club(Club &&other) noexcept = default;

  Club &operator=(const Club &other) noexcept = default;
  Club &operator=(Club &&other) noexcept = default;

  ~Club() noexcept = default;

  std::pair<Event, std::optional<Event>> arrive(Time time,
                                                const Client &client) noexcept;

  std::pair<Event, std::optional<Event>>
  take_table(Time time, const Client &client, std::size_t table);

  std::pair<Event, std::optional<Event>> queue(Time time,
                                               const Client &client) noexcept;

  std::pair<Event, std::optional<Event>> leave(Time time,
                                               const Client &client) noexcept;

  std::vector<Event> close() noexcept;

  std::size_t revenue() const noexcept;

private:
  Time _open_time;
  Time _close_time;
  std::size_t _free_tables_count;
  std::size_t _price;
  std::size_t _revenue;
  std::vector<std::optional<Time>> _tables;
  std::queue<const Client *> _queue;
  std::unordered_map<Client, std::optional<std::size_t>> _clients;

  void take_table_(Time time, const Client &client, std::size_t table) noexcept;
  std::optional<std::size_t> free_table_(Time time,
                                         const Client &client) noexcept;
  std::optional<Event> free_table_forever_(Time time,
                                           const Client &client) noexcept;
};
} // namespace club
