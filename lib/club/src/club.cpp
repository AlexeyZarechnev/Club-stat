#include "club.hpp"
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
      _free_tables_count(tables_count), _revenue(0), _price(price),
      _tables(tables_count + 1, std::nullopt) {}

std::pair<Event, std::optional<Event>>
Club::arrive(Time time, const Client &client) noexcept {
  Event base = {time, 1, client};
  if (time < _open_time || time >= _close_time) {
    return {base, Event(time, 13, "NotOpenYet")};
  }
  if (_clients.find(client) != _clients.end()) {
    return {base, Event(time, 13, "YouShallNotPass")};
  }
  _clients.insert({client, std::nullopt});
  return {base, std::nullopt};
}

std::pair<Event, std::optional<Event>>
Club::take_table(Time time, const Client &client, std::size_t table) {
  Event base = {time, 2, client, table};
  if (!_clients.contains(client)) {
    return {base, Event(time, 13, "ClientUnknown")};
  }
  if (_tables[table]) {
    return {base, Event(time, 13, "PlaceIsBusy")};
  }
  free_table_(time, client);
  take_table_(time, client, table);
  return {base, std::nullopt};
}

std::pair<Event, std::optional<Event>>
Club::queue(Time time, const Client &client) noexcept {
  Event base = {time, 3, client};
  if (!_clients.contains(client)) {
    return {base, Event(time, 13, "ClientUnknown")};
  }
  if (_clients[client]) {
    return {base, Event(time, 13, "YouAlreadyHaveTable")};
  }
  if (_free_tables_count > 0) {
    return {base, Event(time, 13, "ICanWaitNoLonger")};
  }
  if (_queue.size() >= _tables.size()) {
    return {base, Event(time, 11, client)};
  }
  _queue.push(&_clients.find(client)->first);
  return {base, std::nullopt};
}

std::pair<Event, std::optional<Event>>
Club::leave(Time time, const Client &client) noexcept {
  Event base = {time, 4, client};
  if (!_clients.contains(client)) {
    return {base, Event(time, 13, "ClientUnknown")};
  }
  auto event = free_table_forever_(time, client);
  _clients.erase(client);
  return {base, event};
}

std::vector<Event> Club::close() noexcept {
  std::vector<Event> closed_events;
  for (auto &[client, table_opt] : _clients) {
    free_table_(_close_time, client);
    closed_events.emplace_back(_close_time, 4, client);
  }
  return closed_events;
}

std::size_t Club::revenue() const noexcept { return _revenue; }

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
    _revenue += elapsed.ceil() * _price;
    _tables[table] = std::nullopt;
    it->second = std::nullopt;
    _free_tables_count++;
    return table;
  }
  return std::nullopt;
}

std::optional<Event> Club::free_table_forever_(Time time,
                                               const Client &client) noexcept {
  std::size_t table = *free_table_(time, client);
  if (!_queue.empty()) {
    const Client *next_client = _queue.front();
    _queue.pop();
    take_table_(time, *next_client, table);
    return Event(time, 12, *next_client, table);
  }
  return std::nullopt;
}

} // namespace club
