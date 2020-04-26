#include "catch.hpp"
#include <chrono>
using namespace std::chrono;

#include <queue> // test heap
#include "../implementations/PriorityQueue.cpp"

TEST_CASE("test priority queue", "[PriorityQueue]") {

    SECTION("priority queue works") {
        // note: elements are guaranteed to be unique
        PriorityQueue<char> pq;
        CHECK(pq.empty());
        CHECK(pq.size() == 0);
        for (char c = 'a'; c < 'g'; c++) {
            CHECK(pq.size() == (unsigned int)(c-'a'));
            pq.push(c);
            CHECK(!pq.empty());
            CHECK(pq.top() == c);
        }
        for (char c = 'z'; c >= 'g'; c--) {
            CHECK(pq.size() == (unsigned int)('z'-c+'g'-'a'));
            pq.push(c);
            CHECK(!pq.empty());
            CHECK(pq.top() == 'z');
        }
        for (char c = 'z'; c >= 'a'; c--) {
            CHECK(pq.top() == c);
            CHECK(!pq.empty());
            pq.pop();
            CHECK(pq.size() == (unsigned int)(c-'a'));
        };
        CHECK(pq.empty());
        CHECK(pq.size() == 0);
    }

    SECTION("priority queue performs optimally") {
        auto ref_start = high_resolution_clock::now();
        std::priority_queue<int> ref_pq;
        for (int i = 0; i <= 50000; i++) ref_pq.push(i);
        for (int i = 0; i <= 50000; i++) ref_pq.pop();
        auto ref_end = high_resolution_clock::now();
        auto ref_time = duration_cast<milliseconds>(ref_end-ref_start).count();

        auto user_start = high_resolution_clock::now();
        PriorityQueue<int> user_pq;
        for (int i = 0; i <= 50000; i++) user_pq.push(i);
        for (int i = 0; i <= 50000; i++) user_pq.pop();
        auto user_end = high_resolution_clock::now();
        auto user_time = duration_cast<milliseconds>(user_end-user_start).count();

        CHECK(user_time < 5*ref_time); // same order of magnitude
    }
}
