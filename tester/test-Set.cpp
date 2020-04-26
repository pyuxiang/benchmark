#include "catch.hpp"
#include <chrono>
#include <random>
using namespace std::chrono;

#include <list> // container for set range
#include <set> // test AVL tree
#include "../implementations/Set.cpp"

// Methods can be defined in subclass using inheritance
template<typename T>
class StdSet: public std::set<T> {
public:
    bool contains(T item) {
        return this->find(item) != this->end();
    }
    // obtain list of all items start_item <= x < end_item
    std::list<T> range(T start_item, T end_item) {
        std::list<T> container;
        auto start = this->lower_bound(start_item);
        auto end = this->upper_bound(end_item);
        for (auto it = start;; it++) {
            if (it == end) break;
            container.push_back(*it);
        }
        return container;
    }
    // obtain in-order traversal
    std::list<T> full_range() {
        std::list<T> container;
        for (auto item: *this) {
            container.push_back(item);
        }
        return container;
    }
};
// alternative solution using template specialization
// template<class T> bool contains(std::set<T> tree, T item) { return tree.find(item) != tree.end(); }
// template<class C, class T> bool contains(C tree, T item) { return tree.contains(); }

TEST_CASE("test avl tree", "[avl]") {

    SECTION("avl tree works") {
        Set<long long> tree;
        REQUIRE(tree.empty());
        REQUIRE(tree.size() == 0);
        for (long long ll = 0; ll < 0x200000000; ll += 0xdca6c00) { // 38
            REQUIRE(tree.size() == (unsigned int)(ll/0xdca6c00));
            tree.insert(ll);
            REQUIRE(!tree.empty());
        }
        int curr_size = tree.size();
        for (long long i = 0; i < 12; i++) {
            long long entry = i*0xdca6c00 + 3;
            tree.insert(entry);
            REQUIRE(tree.contains(entry));
        }
        REQUIRE(tree.size() == (unsigned int)(curr_size+12));
        for (long long ll = 0; ll < 0x200000000; ll += 0xdca6c00) {
            REQUIRE(tree.contains(ll));
            tree.erase(ll);
        }
        for (long long i = 20; i >= 12; i--) {
            long long entry = i*0xdca6c00 + 3;
            tree.insert(entry);
            REQUIRE(tree.contains(entry));
        }
        REQUIRE(!tree.empty());
        REQUIRE(tree.size() == 21);

        // In-order traversal (the whole point to ordered binary search trees)
        long long result[4] = \
            {1156848643, 1388218371, 1619588099, 1850957827 };
        long long full_result[21] = \
            {         3,  231369731,  462739459,  694109187,  925478915,
             1156848643, 1388218371, 1619588099, 1850957827, 2082327555,
             2313697283, 2545067011, 2776436739, 3007806467, 3239176195,
             3470545923, 3701915651, 3933285379, 4164655107, 4396024835,
             4627394563 };

        unsigned int i = 0;
        for (auto item: tree.full_range()) {
            REQUIRE(item == full_result[i]); // full in-order traversal
            i++;
        }

        i = 0;
        for (auto item: tree.range(1000000000, 2000000000)) {
            REQUIRE(item == result[i]); // subset of in-order traversal
            i++;
        }
    }

    SECTION("avl tree performs optimally: insertion") {

        std::default_random_engine ref_engine(0.5);
        std::default_random_engine user_engine(0.5);
        std::uniform_int_distribution<int> dist(-2147483648, 2147483647);

        auto ref_start = high_resolution_clock::now();
        StdSet<int> ref_set;
        for (int i = 0; i <= 50000; i++) {
            int num = dist(ref_engine);
            ref_set.insert(num);
        }
        auto ref_end = high_resolution_clock::now();
        auto ref_time = duration_cast<milliseconds>(ref_end-ref_start).count();

        auto user_start = high_resolution_clock::now();
        Set<int> user_set;
        for (int i = 0; i <= 50000; i++) {
            int num = dist(user_engine);
            user_set.insert(num);
        }
        auto user_end = high_resolution_clock::now();
        auto user_time = duration_cast<milliseconds>(user_end-user_start).count();

        CHECK(user_time < 5*ref_time); // same order of magnitude
    }

    SECTION("avl tree performs optimally: traversal") {

        std::default_random_engine ref_engine(0.5);
        std::uniform_int_distribution<int> dist(-2147483648, 2147483647);

        StdSet<int> ref_set;  std::list<int> ref_result;
        Set<int> user_set; std::list<int> user_result;
        for (int i = 0; i <= 50000; i++) {
            int num = dist(ref_engine);
            ref_set.insert(num);
            user_set.insert(num);
        }

        auto ref_start = high_resolution_clock::now();
        for (int i = 0; i < 10; i++) {
            ref_result = ref_set.range(-1000000000, 1000000000);
        }
        auto ref_end = high_resolution_clock::now();
        auto ref_time = duration_cast<milliseconds>(ref_end-ref_start).count();

        auto user_start = high_resolution_clock::now();
        for (int i = 0; i < 10; i++) {
            user_result = user_set.range(-1000000000, 1000000000);
        }
        auto user_end = high_resolution_clock::now();
        auto user_time = duration_cast<milliseconds>(user_end-user_start).count();

        REQUIRE(ref_result.front() == user_result.front());
        REQUIRE(ref_result.back() == user_result.back());
        CHECK(user_time < 5*ref_time); // same order of magnitude
    }
}
