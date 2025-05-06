#include <ctime>
#include <format>
#include <string>

#include "club.hpp"

Club::Club(int tables_count, time_t open_time, time_t close_time, int price) noexcept
    : _price(price)
    , _durations(tables_count + 1, 0)
    , _moneys(tables_count + 1, 0)
    , _open_time(open_time)
    , _close_time(close_time)
{
}

Club::Club(const Club &other) noexcept
    : _price(other._price)
    , _durations(other._durations)
    , _moneys(other._moneys)
    , _taken_tables(other._taken_tables)
    , _clients(other._clients)
    , _queue(other._queue)
    , _open_time(other._open_time)
    , _close_time(other._close_time)
{
}

Club::Club(Club &&other) noexcept
    : _price(std::move(other._price))
    , _durations(std::move(other._durations))
    , _moneys(std::move(other._moneys))
    , _taken_tables(std::move(other._taken_tables))
    , _clients(std::move(other._clients))
    , _queue(std::move(other._queue))
    , _open_time(std::move(other._open_time))
    , _close_time(std::move(other._close_time))
{
}

Club &Club::operator=(const Club &other) noexcept
{
    Club tmp(other);
    swap(tmp);
    return *this;
}

Club &Club::operator=(Club &&other) noexcept
{
    Club tmp(std::move(other));
    swap(tmp);
    return *this;
}

void Club::swap(Club &other) noexcept
{
    std::swap(_price, other._price);
    _durations.swap(other._durations);
    _moneys.swap(other._moneys);
    _taken_tables.swap(other._taken_tables);
    _clients.swap(other._clients);
    _queue.swap(other._queue);
    std::swap(_open_time, other._open_time);
    std::swap(_close_time, other._close_time);
}

static void print_time(time_t time, std::ostream &out) {
    struct tm *tm = std::gmtime(&time);
    out << std::format("{:02}:{:02}", tm->tm_hour, tm->tm_min);
}

void Club::new_client_1(time_t time, const std::string &name, std::ostream &out) noexcept
{
    print_time(time, out);
    out << " 1 " << name << '\n'; 

    if (_clients.contains(name)) {
        error_13(time, "YouShallNotPass", out);
    }

    if (std::difftime(_open_time, time) > 0 || std::difftime(time, _close_time) > 0) {
        error_13(time, "NotOpenYet", out);
        return;
    }

    _clients[name] = -1;
}

void Club::take_table_2(time_t time, const std::string &name, int table_num, std::ostream &out) noexcept
{
    print_time(time, out);
    out << " 2 " << name << " " << table_num << '\n';
    if (_taken_tables.contains(table_num)) {
        error_13(time, "PlaceIsBusy", out);
        return;
    }

    if (!_clients.contains(name)) {
        error_13(time, "ClientUnknown", out);
        return;
    }

    free_table(time, _clients[name]);
    
    _clients[name] = table_num;
    _taken_tables[table_num] = time;
}

void Club::wait_3(time_t time, const std::string &name, std::ostream &out) noexcept
{
    print_time(time, out);
    out << " 3 " << name << '\n';
    if (_taken_tables.size() < _durations.size() - 1) {
        error_13(time, "ICanWaitNoLonger!", out);
        return;
    }
    if (_queue.size() >= _durations.size()) {
        client_gone_11(time, name, out);
    }

    _queue.push(name);
}

void Club::client_gone_4(time_t time, const std::string &name, std::ostream &out) noexcept
{
    print_time(time, out);
    out << " 4 " << name << '\n'; 
    if (!_clients.contains(name)) {
        error_13(time, "ClientUnknown", out);
    }

    int table_num = _clients[name];
    _clients.erase(name);

    free_table(time, table_num);

    if (table_num != -1 && !_queue.empty()) {
        take_table_12(time, _queue.front(), table_num, out);
        _queue.pop();
    }
}

void Club::close(std::ostream &out)
{
    for (auto &client : _clients) {
        client_gone_11(_close_time, client.first, out);
    }
    print_time(_close_time, out);
    out << '\n';
    for (int i = 1; i < _moneys.size(); ++i) {
        out << i << " " << _moneys[i] << " ";
        print_time(_durations[i], out);
        out << '\n';
    }
}

void Club::client_gone_11(time_t time, const std::string &name, std::ostream &out) noexcept 
{
    print_time(time, out);
    out << " 11 " << name << '\n';

    free_table(time, _clients[name]);
}

void Club::take_table_12(time_t time, const std::string &name, int table_num, std::ostream &out) noexcept
{
    print_time(time, out);
    out << " 12 " << name << " " << table_num << '\n';

    _clients[name] = table_num;
    _taken_tables[table_num] = time;
}

void Club::error_13(time_t time, const char *text, std::ostream &out) noexcept
{
    print_time(time, out);
    out << " 13 " << text << '\n';
}

void Club::free_table(time_t time, int table_num)
{
    if (table_num == -1)
        return;
    
    time_t dur = std::difftime(time, _taken_tables[table_num]);
    _durations[table_num] += dur;
    _moneys[table_num] += _price * std::ceil((double)dur / 3600);
    _taken_tables.erase(table_num);
}
