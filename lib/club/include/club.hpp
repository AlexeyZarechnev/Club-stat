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

  void arrive(Time time, const Client &client) noexcept;

  void take_table(Time time, const Client &client, std::size_t table);
  void queue(Time time, const Client &client) noexcept;

  void leave(Time time, const Client &client) noexcept;

  std::vector<Event>& close() noexcept;

  const std::vector<std::pair<Time, std::size_t>>& stats() const noexcept;

private:
  Time _open_time;
  Time _close_time;
  std::size_t _free_tables_count;
  std::size_t _price;
  std::vector<std::optional<Time>> _tables;
  std::vector<std::pair<Time, std::size_t>> _stats;
  std::vector<Event> _events;
  std::queue<const Client *> _queue;
  std::unordered_map<Client, std::optional<std::size_t>> _clients;

  void take_table_(Time time, const Client &client, std::size_t table) noexcept;
  std::optional<std::size_t> free_table_(Time time, const Client &client) noexcept;
  void free_table_forever_(Time time, const Client &client) noexcept;
};
} // namespace club
