#include "club.hpp"
#include <algorithm>
#include <optional>
#include <utility>

namespace club {

Event::Event(Time time, std::size_t id, const char *name) noexcept
    : time(time), id(id), name(name), table_id(std::nullopt) {}

Event::Event(Time time, std::size_t id, const std::string &name) noexcept
    : time(time), id(id), name(name), table_id(std::nullopt) {}

Event::Event(Time time, std::size_t id, std::string &&name) noexcept
    : time(time), id(id), name(std::move(name)), table_id(std::nullopt) {}

Event::Event(Time time, std::size_t id, const std::string &name,
             std::size_t table_id) noexcept
    : time(time), id(id), name(name), table_id(table_id) {}

bool operator==(const Event &lhs, const Event &rhs) noexcept {
  return lhs.id == rhs.id && lhs.name == rhs.name && lhs.time == rhs.time &&
         lhs.table_id == rhs.table_id;
}

std::ostream &operator<<(std::ostream &os, const Event &event) noexcept {
  os << event.time << " " << event.id << " " << event.name;
  if (event.table_id) {
    os << " " << *event.table_id;
  }
  return os;
}

Club::Club(size_t tables_count, Time open_time, Time close_time,
           std::size_t price)
    : _open_time(open_time), _close_time(close_time),
      _free_tables_count(tables_count), _price(price),
      _tables(tables_count + 1, std::nullopt),
      _stats(tables_count + 1, {Time(0, 0), 0}) {}

void Club::arrive(Time time, const Client &client) noexcept {
  _events.emplace_back(time, 1, client);
  if (time < _open_time || time >= _close_time) {
    _events.emplace_back(time, 13, "NotOpenYet");
    return;
  }
  if (_clients.contains(client)) {
    _events.emplace_back(time, 13, "YouShallNotPass");
    return;
  }
  _clients.insert({client, std::nullopt});
}

void Club::take_table(Time time, const Client &client, std::size_t table) {
  _events.emplace_back(time, 2, client, table);
  if (!_clients.contains(client)) {
    _events.emplace_back(time, 13, "ClientUnknown");
    return;
  }
  if (_tables[table]) {
    _events.emplace_back(time, 13, "PlaceIsBusy");
    return;
  }
  free_table_(time, client);
  take_table_(time, client, table);
}

void Club::queue(Time time, const Client &client) noexcept {
  _events.emplace_back(time, 3, client);
  if (!_clients.contains(client)) {
    _events.emplace_back(time, 13, "ClientUnknown");
    return;
  }
  if (_clients[client]) {
    _events.emplace_back(time, 13, "YouAlreadyHaveTable");
    return;
  }
  if (_free_tables_count > 0) {
    _events.emplace_back(time, 13, "ICanWaitNoLonger!");
    return;
  }
  if (_queue.size() >=
      _tables.size() -
          1) { // tables.size() - 1 == tables_count + 1 - 1 == tables_count
    _events.emplace_back(time, 11, client);
    _clients.erase(client);
    return;
  }
  _queue.push(&_clients.find(client)->first);
}

void Club::leave(Time time, const Client &client) noexcept {
  _events.emplace_back(time, 4, client);
  if (!_clients.contains(client)) {
    _events.emplace_back(time, 13, "ClientUnknown");
    return;
  }
  free_table_forever_(time, client);
  _clients.erase(client);
}

std::vector<Event> &Club::close() noexcept {
  std::vector<Event> last;
  for (auto &[client, table_opt] : _clients) {
    free_table_(_close_time, client);
    last.emplace_back(_close_time, 11, client);
  }
  std::sort(last.begin(), last.end(),
            [](auto &lhs, auto &rhs) { return lhs.name < rhs.name; });
  for (int i = 0; i < last.size(); ++i) {
    _events.emplace_back(std::move(last[i]));
  }
  return _events;
}

const std::vector<std::pair<Time, std::size_t>> &Club::stats() const noexcept {
  return _stats;
}

void Club::take_table_(Time time, const Client &client,
                       std::size_t table) noexcept {
  _tables[table] = time;
  _clients[client] = table;
  _free_tables_count--;
}

std::optional<std::size_t> Club::free_table_(Time time,
                                             const Client &client) noexcept {
  auto it = _clients.find(client);
  if (it != _clients.end() && it->second) {
    std::size_t table = *it->second;
    Time elapsed = time - *_tables[table];
    _stats[table].first += elapsed;
    _stats[table].second += elapsed.ceil() * _price;
    _tables[table] = std::nullopt;
    it->second = std::nullopt;
    _free_tables_count++;
    return table;
  }
  return std::nullopt;
}

void Club::free_table_forever_(Time time, const Client &client) noexcept {
  auto table = free_table_(time, client);
  if (table && !_queue.empty()) {
    const Client *next_client = _queue.front();
    _queue.pop();
    take_table_(time, *next_client, *table);
    _events.emplace_back(time, 12, *next_client, *table);
  }
}

} // namespace club
