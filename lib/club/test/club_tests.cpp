#include <gtest/gtest.h>
#include <sstream>

#include "club.hpp"
#include "time.hpp"

TEST(ClubTests, EventTest) {
  club::Time time(10, 30);
  club::Event event1(time, 1, "TestClient");
  std::ostringstream os1;
  os1 << event1;
  EXPECT_EQ(os1.str(), "10:30 1 TestClient");

  club::Event event2(time, 2, "TestClient", 5);
  std::ostringstream os2;
  os2 << event2;
  EXPECT_EQ(os2.str(), "10:30 2 TestClient 5");
}

TEST(ClubTests, ArriveTest) {
  club::Club club(2, club::Time(9, 0), club::Time(22, 0), 100);
  club::Time arrive_time(10, 0);
  club::Club::Client client("Alice");

  club.arrive(arrive_time, client);
  std::vector<club::Event> events = club.close();

  ASSERT_EQ(events.size(), 2);
  EXPECT_EQ(events[0].id, 1); // Arrive event
  EXPECT_EQ(events[0].name, "Alice");
  EXPECT_EQ(events[0].time, arrive_time);
  EXPECT_EQ(events[1].id, 11); // Leave event
  EXPECT_EQ(events[1].name, "Alice");
  EXPECT_EQ(events[1].time, club::Time(22, 0));
}

TEST(ClubTests, LeaveTest) {
  club::Club club(2, club::Time(9, 0), club::Time(22, 0), 100);
  club::Time arrive_time(10, 0);
  club::Club::Client client("Alice");

  club.arrive(arrive_time, client);
  club.leave(club::Time(11, 0), client);
  std::vector<club::Event> events = club.close();

  ASSERT_EQ(events.size(), 2);
  EXPECT_EQ(events[0].id, 1); // Arrive event
  EXPECT_EQ(events[0].name, "Alice");
  EXPECT_EQ(events[0].time, arrive_time);
  EXPECT_EQ(events[1].id, 4); // Leave event
  EXPECT_EQ(events[1].name, "Alice");
  EXPECT_EQ(events[1].time, club::Time(11, 0));
}

TEST(ClubTests, TakeTableTest) {
  club::Club club(2, club::Time(9, 0), club::Time(22, 0), 100);
  club::Time arrive_time(10, 0);
  club::Club::Client client("Alice");

  club.arrive(arrive_time, client);
  club.take_table(club::Time(10, 15), client, 1);
  std::vector<club::Event> events = club.close();

  ASSERT_EQ(events.size(), 3);
  EXPECT_EQ(events[0].id, 1); // Arrive event
  EXPECT_EQ(events[0].name, "Alice");
  EXPECT_EQ(events[0].time, arrive_time);
  EXPECT_EQ(events[1].id, 2); // Take table event
  EXPECT_EQ(events[1].name, "Alice");
  EXPECT_EQ(events[1].time, club::Time(10, 15));
  EXPECT_EQ(events[1].table_id, 1);

  auto &stats = club.stats();
  EXPECT_EQ(stats[1], std::make_pair(club::Time(11, 45), 1200));
}

TEST(ClubTests, QueueTest) {
  club::Club club(0, club::Time(9, 0), club::Time(22, 0),
                  10); // 0 tables - all clients leave after queue
  club::Time arrive_time(10, 0);
  club::Club::Client client("Bob");
  int count = 10;

  for (int i = 0; i < count; ++i) {
    club.arrive(arrive_time, client);
    club.queue(arrive_time, client);
  }

  std::vector<club::Event> &events = club.close();
  ASSERT_EQ(events.size(), 3 * count);

  for (int i = 0; i < 3 * count; i += 3) {
    EXPECT_EQ(events[i], club::Event(arrive_time, 1, client));
    EXPECT_EQ(events[i + 1], club::Event(arrive_time, 3, client));
    EXPECT_EQ(events[i + 2], club::Event(arrive_time, 11, client));
  }
}

TEST(ClubTest, StatsTest) {
  club::Club club(1, club::Time(9, 0), club::Time(12, 0), 10);
  club::Club::Client client = "Bob";

  for (int i = 0; i < 60; ++i) {
    club.arrive(club::Time(10, i), client);
    club.take_table(club::Time(10, i), client, 1);
    club.leave(club::Time(10, i + 1), client);
  }

  club.close();

  auto &stats = club.stats();
  ASSERT_EQ(stats.size(), 2); // tables_count + 1
  EXPECT_EQ(stats[1].first, club::Time(1, 0));
  EXPECT_EQ(stats[1].second, 600);
}