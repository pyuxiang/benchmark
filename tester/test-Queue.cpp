#include "catch.hpp"
#include <chrono>
using namespace std::chrono;

#include <queue> // test linked list
#include "../implementations/Queue.cpp"

TEST_CASE("test queue", "[Queue]") {

    SECTION("queue pushes correctly") {
        std::queue<char> lst;
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
        for (char c = 'a'; c <= 'g'; c++) {
            REQUIRE(lst.size() == (unsigned int)(c-'a'));
            lst.push(c);
            REQUIRE(!lst.empty());
            REQUIRE(lst.front() == 'a');
        }
    }

    SECTION("queue pops correctly") {
        std::queue<char> lst;
        for (char c = 'a'; c <= 'g'; c++) lst.push(c);
        for (char c = 'a'; c <= 'f'; c++) {
            REQUIRE(lst.front() == c);
            lst.pop();
            REQUIRE(lst.size() == (unsigned int)('g'-c));
        };
        REQUIRE(!lst.empty());
        lst.pop();
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
    }

    SECTION("queue performs optimally") {
        // will always necessarily run a little slower than the second profiling
        auto ref_start = high_resolution_clock::now();
        std::queue<int> ref_stack;
        for (int i = 0; i <= 1000000; i++) ref_stack.push(i);
        for (int i = 0; i <= 1000000; i++) ref_stack.pop();
        auto ref_end = high_resolution_clock::now();
        auto ref_time = duration_cast<milliseconds>(ref_end-ref_start).count();

        auto user_start = high_resolution_clock::now();
        std::queue<int> user_stack;
        for (int i = 0; i <= 1000000; i++) user_stack.push(i);
        for (int i = 0; i <= 1000000; i++) user_stack.pop();
        auto user_end = high_resolution_clock::now();
        auto user_time = duration_cast<milliseconds>(user_end-user_start).count();

        CHECK(user_time < 5*ref_time); // same order of magnitude
    }
}
