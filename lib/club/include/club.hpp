#ifndef CLUB
#define CLUB

#include <string>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>
#include <queue>

class Club {
public:
    Club(int tables_count, time_t open_time, time_t close_time, int price) noexcept;

    Club(const Club &other) noexcept;
    Club(Club &&other) noexcept;

    Club &operator=(const Club &other) noexcept;
    Club &operator=(Club &&other) noexcept;

    void swap(Club &other) noexcept;

    // Incoming
    void new_client_1(time_t time, const std::string &name, std::ostream &out) noexcept;
    void take_table_2(time_t time, const std::string &name, int table_num, std::ostream &out) noexcept;
    void wait_3(time_t time, const std::string &name, std::ostream &out) noexcept;
    void client_gone_4(time_t time, const std::string &name, std::ostream &out) noexcept;

    void close(std::ostream &out);

    ~Club() noexcept = default;

private:
    // Outcoming
    void client_gone_11(time_t time, const std::string &name, std::ostream &out) noexcept;
    void take_table_12(time_t time, const std::string &name, int table_num, std::ostream &out) noexcept;
    void error_13(time_t time, const char *text, std::ostream &out) noexcept;

    // Counting
    void free_table(time_t time, int table_num);

    int _price;
    std::vector<int> _durations;
    std::vector<int> _moneys;
    std::map<int, time_t> _taken_tables;
    std::map<std::string, int> _clients;
    std::queue<std::string> _queue;
    time_t _open_time;
    time_t _close_time;
};

#endif