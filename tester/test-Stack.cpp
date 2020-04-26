#include "catch.hpp"
#include <chrono>
using namespace std::chrono;

#include <stack> // test linked list
#include "../implementations/Stack.cpp"

TEST_CASE("test stack", "[Stack]") {

    SECTION("stack works") {
        Stack<char> lst;
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
        for (char c = 'a'; c <= 'g'; c++) {
            REQUIRE(lst.size() == (unsigned int)(c-'a'));
            lst.push(c);
            REQUIRE(!lst.empty());
            REQUIRE(lst.top() == c);
        }
        for (char c = 'g'; c >= 'a'; c--) {
            REQUIRE(lst.top() == c);
            REQUIRE(!lst.empty());
            lst.pop();
            REQUIRE(lst.size() == (unsigned int)(c-'a'));
        };
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
    }

    SECTION("stack performs optimally") {
        // will always necessarily run a little slower than the second profiling
        auto ref_start = high_resolution_clock::now();
        std::stack<int> ref_stack;
        for (int i = 0; i <= 1000000; i++) ref_stack.push(i);
        for (int i = 0; i <= 1000000; i++) ref_stack.pop();
        auto ref_end = high_resolution_clock::now();
        auto ref_time = duration_cast<milliseconds>(ref_end-ref_start).count();

        auto user_start = high_resolution_clock::now();
        Stack<int> user_stack;
        for (int i = 0; i <= 1000000; i++) user_stack.push(i);
        for (int i = 0; i <= 1000000; i++) user_stack.pop();
        auto user_end = high_resolution_clock::now();
        auto user_time = duration_cast<milliseconds>(user_end-user_start).count();

        CHECK(user_time < 5*ref_time); // same order of magnitude
    }
}
