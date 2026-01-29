#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

std::string exec_with_tmp_file(const std::string &cmd,
                               std::function<bool(int)> &checker) {
  std::filesystem::path output_file =
      std::filesystem::temp_directory_path() / "test_output.txt";

  std::string full_cmd = cmd + " > " + output_file.string() + " 2>&1";

  int result = std::system(full_cmd.c_str());

  std::string output;
  if (std::filesystem::exists(output_file)) {
    std::ifstream in(output_file);
    if (in.is_open()) {
      output = std::string((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>());
      in.close();
    }
    std::filesystem::remove(output_file);
  }

  EXPECT_TRUE(checker(result));
  return output;
}

void test(const std::string &input, const std::string &expected_output,
          std::function<bool(int)> checker) {
  std::filesystem::path input_file =
      std::filesystem::temp_directory_path() / "test_input.txt";
  std::ofstream file(input_file);
  file << input;
  file.close();
  if constexpr () {
  
  }
  #if defined(_WIN32) || defined(_WIN64)
  std::string output =
      exec_with_tmp_file("club-stat.exe " + input_file.string(), checker);
  #else
  std::string output =
      exec_with_tmp_file("./club-stat " + input_file.string(), checker);
  #endif
  EXPECT_EQ(output, expected_output);
}

void test_good(std::string input, std::string expected_output) {
  test(input, expected_output, [](int result) { return result == 0; });
}

void test_bad(std::string input, std::string expected_output) {
  test(input, expected_output, [](int result) { return result != 0; });
}

TEST(ValidTests, BasicTest) {
  test_good(
      R"(3
09:00 19:00
10
09:41 1 client1
09:48 1 client2
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
12:33 4 client1
12:43 4 client2
15:52 4 client3)",
      R"(09:00
09:41 1 client1
09:48 1 client2
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
12:33 4 client1
12:43 4 client2
15:52 4 client3
19:00
1 30 02:39
2 30 02:18
3 50 04:53
)");
}

TEST(ValidTests, BoundaryTest) {
  test_good(
      R"(1
00:00 23:59
5
00:00 1 client1
00:01 2 client1 1
12:00 1 client2
12:01 3 client2
23:58 4 client1)",
      R"(00:00
00:00 1 client1
00:01 2 client1 1
12:00 1 client2
12:01 3 client2
23:58 4 client1
23:58 12 client2 1
23:59 11 client2
23:59
1 125 23:58
)");
}

TEST(ValidTests, FromStatementTest) {
  test_good(
      R"(3
09:00 19:00
10
08:48 1 client1
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:45 3 client4
12:33 4 client1
12:43 4 client2
15:52 4 client4)",
      R"(09:00
08:48 1 client1
08:48 13 NotOpenYet
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:52 13 ICanWaitNoLonger!
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:35 13 PlaceIsBusy
11:45 3 client4
12:33 4 client1
12:33 12 client4 1
12:43 4 client2
15:52 4 client4
19:00 11 client3
19:00
1 70 05:58
2 30 02:18
3 90 08:01
)");
}

TEST(ValidTests, ErrorsTest) {
  test_good(
      R"(2
09:00 18:00
20
09:01 1 client1
09:02 1 client1
09:03 2 client2 1
09:04 2 client1 1
09:05 1 client2
09:06 2 client1 2
09:07 4 client3
09:08 3 client1
09:09 1 client3
09:10 2 client3 1)",
      R"(09:00
09:01 1 client1
09:02 1 client1
09:02 13 YouShallNotPass
09:03 2 client2 1
09:03 13 ClientUnknown
09:04 2 client1 1
09:05 1 client2
09:06 2 client1 2
09:07 4 client3
09:07 13 ClientUnknown
09:08 3 client1
09:08 13 YouAlreadyHaveTable
09:09 1 client3
09:10 2 client3 1
18:00 11 client1
18:00 11 client2
18:00 11 client3
18:00
1 200 08:52
2 180 08:54
)");
}

TEST(InvalidTests, BadCount) {
  test_bad("12bv\n", "12bv");
  test_bad("12 \n", "12 ");
  test_bad("12", "12");
}

TEST(InvalidTests, BadTimes) {
  test_bad("1\n15", "15");
  test_bad("1\n15:28", "15:28");
  test_bad("1\n15;28", "15;28");
  test_bad("1\n15:228", "15:228");
  test_bad("1\n9:28", "9:28");
  test_bad("1\n09:28 12:", "09:28 12:");
  test_bad("1\n09:28 12a", "09:28 12a");
  test_bad("1\n09:28 12:43 r\n123\n", "09:28 12:43 r");
  test_bad("1\n09:28 12:43 \n123\n", "09:28 12:43 ");
}

TEST(InvalidTests, BadPrice) {
  test_bad("1\n09:28 10:29\n1a2", "1a2");
  test_bad("1\n09:28 10:29\nff", "ff");
  test_bad("1\n09:28 10:29\n4534 6", "4534 6");
  test_bad("1\n09:28 10:29\naaa", "aaa");
  test_bad("1\n09:28 10:29\n", "");
}

TEST(InvalidTests, BadEvent) {
  test_bad("1\n09:28 10:29\n12\n12", "12");
  test_bad("1\n09:28 10:29\n12\n12:344 1 bob", "12:344 1 bob");
  test_bad("1\n09:28 10:29\n12\n12:34 ff bob", "12:34 ff bob");
  test_bad("1\n09:28 10:29\n12\n12:34 1 Bob", "12:34 1 Bob");
  test_bad("1\n09:28 10:29\n12\n12:34 1 bob+", "12:34 1 bob+");
  test_bad("1\n09:28 10:29\n12\n12:34 1 bob_ ", "12:34 1 bob_ ");
}

TEST(InvalidTests, BadOrder) {
  test_bad("1\n09:28 10:29\n12\n13:34 1 bob\n12:34 1 bob", "12:34 1 bob");
}
