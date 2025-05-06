#include <gtest/gtest.h>
#include <sstream>

#include "club.hpp"

static time_t time(int hours, int minutes) {
    return 3600 * hours + 60 * minutes;
}

constexpr static const std::string client = "client_";

class ClubTests : public testing::Test {
protected:
    Club club;
    std::stringstream output;
    std::vector<std::string> clients;
    ClubTests() 
        : club(9, time(9, 1), time(19, 43), 35)
    {
        for (int i = 0; i <= 9; ++i) {
            std::string name(client);
            name.push_back(i + '0');
            clients.push_back(name);
        }
    }
};

TEST_F(ClubTests, event1Test) {
    club.new_client_1(time(9, 2), clients[0], output);
    EXPECT_EQ(output.str(), "09:02 1 client_0\n");
}

TEST_F(ClubTests, event1EarlyTest) {
    club.new_client_1(time(8, 59), clients[0], output);
    EXPECT_EQ(output.str(), "08:59 1 client_0\n08:59 13 NotOpenYet\n");
}

TEST_F(ClubTests, event1DoubleComeTest) {
    club.new_client_1(time(10, 20), clients[0], output);
    output.str("");
    club.new_client_1(time(10, 22), clients[0], output);
    EXPECT_EQ(output.str(), "10:22 1 client_0\n10:22 13 YouShallNotPass\n");
}

TEST_F(ClubTests, event2Test) {
    club.new_client_1(time(9, 1), clients[0], output);
    output.str("");
    club.take_table_2(time(9, 12), clients[0], 3, output);
    EXPECT_EQ(output.str(), "09:12 2 client_0 3\n");
}

TEST_F(ClubTests, event2UnknownClientTest) {
    club.take_table_2(time(13, 1), clients[1], 2, output);
    EXPECT_EQ(output.str(), "13:01 2 client_1 2\n13:01 13 ClientUnknown\n");
}

TEST_F(ClubTests, event2BusyTest) {
    club.new_client_1(time(9, 25), clients[0], output);
    club.take_table_2(time(9, 30), clients[0], 2, output);
    output.str("");
    club.take_table_2(time(10,0), clients[0], 2, output);
    EXPECT_EQ(output.str(), "10:00 2 client_0 2\n10:00 13 PlaceIsBusy\n");
}

TEST_F(ClubTests, event3Test) {
    for (int i = 0; i < 9; ++i) {
        club.new_client_1(time(9, i + 1), clients[i], output);
        club.take_table_2(time(9, i + 1), clients[i], i + 1, output);
    }
    output.str("");
    club.wait_3(time(10, 0), clients[9], output);
    EXPECT_EQ(output.str(), "10:00 3 client_9\n");
}

TEST_F(ClubTests, event3HaveFreeTest) {
    club.wait_3(time(14, 15), clients[0], output);
    EXPECT_EQ(output.str(), "14:15 3 client_0\n14:15 13 ICanWaitNoLonger!\n");
}

TEST_F(ClubTests, event4Test) {
    club.new_client_1(time(12, 16), clients[0], output);
    output.str("");
    club.client_gone_4(time(13, 10), clients[0], output);
    EXPECT_EQ(output.str(), "13:10 4 client_0\n");
}

TEST_F(ClubTests, event4UnknownTest) {
    club.client_gone_4(time(13, 10), clients[0], output);
    EXPECT_EQ(output.str(), "13:10 4 client_0\n13:10 13 ClientUnknown\n");
}

TEST_F(ClubTests, closeTest) {
    club.new_client_1(time(18, 41), clients[0], output);
    club.take_table_2(time(18, 41), clients[0], 2, output);
    output.str("");
    club.close(output);
    EXPECT_EQ(output.str(), "19:43 11 client_0\n\
19:43\n\
1 0 00:00\n\
2 70 01:02\n\
3 0 00:00\n\
4 0 00:00\n\
5 0 00:00\n\
6 0 00:00\n\
7 0 00:00\n\
8 0 00:00\n\
9 0 00:00\n");
}

TEST_F(ClubTests, completeDayTest) {
    club.new_client_1(time(9, 1), clients[0], output);
    club.take_table_2(time(9, 15), clients[0], 1, output);
    club.new_client_1(time(10, 0), clients[1], output);
    club.take_table_2(time(10, 5), clients[1], 2, output);
    club.new_client_1(time(11, 30), clients[2], output);
    club.wait_3(time(11, 35), clients[2], output);
    club.new_client_1(time(12, 0), clients[3], output);
    club.take_table_2(time(12, 10), clients[3], 3, output);
    club.new_client_1(time(13, 15), clients[4], output);
    club.wait_3(time(13, 20), clients[4], output);
    club.client_gone_4(time(14, 0), clients[0], output);
    club.take_table_2(time(14, 5), clients[2], 1, output);
    club.new_client_1(time(15, 30), clients[5], output);
    club.take_table_2(time(15, 35), clients[5], 4, output);
    club.new_client_1(time(16, 0), clients[6], output);
    club.wait_3(time(16, 5), clients[6], output);
    club.client_gone_4(time(17, 0), clients[1], output);
    club.take_table_2(time(17, 5), clients[4], 2, output);
    club.new_client_1(time(17, 30), clients[7], output);
    club.wait_3(time(17, 35), clients[7], output);
    club.new_client_1(time(18, 0), clients[8], output);
    club.take_table_2(time(18, 5), clients[8], 5, output);
    club.new_client_1(time(18, 30), clients[9], output);
    club.client_gone_4(time(18, 37), clients[9], output);
    club.close(output);
    EXPECT_EQ(output.str(), \
        "09:01 1 client_0\n\
09:15 2 client_0 1\n\
10:00 1 client_1\n\
10:05 2 client_1 2\n\
11:30 1 client_2\n\
11:35 3 client_2\n\
11:35 13 ICanWaitNoLonger!\n\
12:00 1 client_3\n\
12:10 2 client_3 3\n\
13:15 1 client_4\n\
13:20 3 client_4\n\
13:20 13 ICanWaitNoLonger!\n\
14:00 4 client_0\n\
14:05 2 client_2 1\n\
15:30 1 client_5\n\
15:35 2 client_5 4\n\
16:00 1 client_6\n\
16:05 3 client_6\n\
16:05 13 ICanWaitNoLonger!\n\
17:00 4 client_1\n\
17:05 2 client_4 2\n\
17:30 1 client_7\n\
17:35 3 client_7\n\
17:35 13 ICanWaitNoLonger!\n\
18:00 1 client_8\n\
18:05 2 client_8 5\n\
18:30 1 client_9\n\
18:37 4 client_9\n\
19:43 11 client_2\n\
19:43 11 client_3\n\
19:43 11 client_4\n\
19:43 11 client_5\n\
19:43 11 client_6\n\
19:43 11 client_7\n\
19:43 11 client_8\n\
19:43\n\
1 385 10:23\n\
2 350 09:33\n\
3 280 07:33\n\
4 175 04:08\n\
5 70 01:38\n\
6 0 00:00\n\
7 0 00:00\n\
8 0 00:00\n\
9 0 00:00\n" );
}