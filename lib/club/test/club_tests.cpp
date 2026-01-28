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
  club::Club club(2, club::Time(10, 0), club::Time(22, 0), 100);

  auto [event1, error1] = club.arrive(club::Time(9, 0), "Alice");
  EXPECT_EQ(event1.id, 1);
  EXPECT_TRUE(error1.has_value());
  EXPECT_EQ(error1->name, "NotOpenYet");

  auto [event2, error2] = club.arrive(club::Time(10, 30), "Alice");
  EXPECT_EQ(event2.id, 1);
  EXPECT_FALSE(error2.has_value());

  auto [event3, error3] = club.arrive(club::Time(11, 0), "Alice");
  EXPECT_EQ(event3.id, 1);
  EXPECT_TRUE(error3.has_value());
  EXPECT_EQ(error3->name, "YouShallNotPass");

  auto [event4, error4] = club.arrive(club::Time(23, 0), "Alice");
  EXPECT_EQ(event4.id, 1);
  EXPECT_TRUE(error4.has_value());
  EXPECT_EQ(error4->name, "NotOpenYet");
}

TEST(ClubTests, TakeTableTest) {
  club::Club club(2, club::Time(10, 0), club::Time(22, 0), 100);

  club.arrive(club::Time(10, 30), "Bob");

  auto [event1, error1] = club.take_table(club::Time(10, 45), "Bob", 1);
  EXPECT_EQ(event1.id, 2);
  EXPECT_EQ(*event1.table_id, 1);
  EXPECT_FALSE(error1.has_value());

  auto [event2, error2] = club.take_table(club::Time(11, 0), "Bob", 1);
  EXPECT_EQ(event2.id, 2);
  EXPECT_TRUE(error2.has_value());
  EXPECT_EQ(error2->name, "PlaceIsBusy");

  auto [event3, error3] = club.take_table(club::Time(11, 15), "Charlie", 1);
  EXPECT_EQ(event3.id, 2);
  EXPECT_TRUE(error3.has_value());
  EXPECT_EQ(error3->name, "ClientUnknown");
}

TEST(ClubTests, QueueTest) {
  club::Club club(1, club::Time(10, 0), club::Time(22, 0), 100);

  club.arrive(club::Time(11, 25), "Frank");
  auto [event1, error1] = club.queue(club::Time(11, 30), "Frank");
  EXPECT_EQ(event1.id, 3);
  EXPECT_TRUE(error1.has_value());
  EXPECT_EQ(error1->name, "ICanWaitNoLonger");

  club.arrive(club::Time(10, 30), "Dave");
  club.take_table(club::Time(10, 45), "Dave", 1);

  club.arrive(club::Time(11, 0), "Eve");

  auto [event2, error2] = club.queue(club::Time(11, 15), "Eve");
  EXPECT_EQ(event2.id, 3);
  EXPECT_FALSE(error2.has_value());

  auto [event3, error3] = club.queue(club::Time(11, 20), "Dave");
  EXPECT_EQ(event3.id, 3);
  EXPECT_TRUE(error3.has_value());
  EXPECT_EQ(error3->name, "YouAlreadyHaveTable");

  auto [event4, error4] = club.queue(club::Time(11, 25), "Grace");
  EXPECT_EQ(event4.id, 3);
  EXPECT_TRUE(error4.has_value());
  EXPECT_EQ(error4->name, "ClientUnknown");
}

TEST(ClubTests, LeaveTest) {
  club::Club club(1, club::Time(10, 0), club::Time(22, 0), 100);

  club.arrive(club::Time(10, 30), "Heidi");
  club.take_table(club::Time(10, 45), "Heidi", 1);

  auto [event1, error1] = club.leave(club::Time(12, 0), "Heidi");
  EXPECT_EQ(event1.id, 4);
  EXPECT_FALSE(error1.has_value());
  EXPECT_EQ(club.revenue(), 200);

  auto [event2, error2] = club.leave(club::Time(12, 15), "Heidi");
  EXPECT_EQ(event2.id, 4);
  EXPECT_TRUE(error2.has_value());
  EXPECT_EQ(error2->name, "ClientUnknown");
}

TEST(ClubTests, CloseTest) {
  club::Club club(1, club::Time(10, 0), club::Time(22, 0), 100);

  club.arrive(club::Time(10, 30), "Ivan");
  club.take_table(club::Time(11, 0), "Ivan", 1);

  club.arrive(club::Time(11, 12), "Judy");
  club.arrive(club::Time(11, 15), "Alex");

  auto events = club.close();
  EXPECT_EQ(events.size(), 3);
  EXPECT_EQ(events[0].time, club::Time(22, 0));
  EXPECT_EQ(events[1].time, club::Time(22, 0));
  EXPECT_EQ(events[2].time, club::Time(22, 0));
  EXPECT_EQ(club.revenue(), 1100);
}
