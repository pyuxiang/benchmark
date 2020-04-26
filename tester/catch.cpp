// To consider adding a timeout for TLE, and check space consumption.

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

// https://github.com/catchorg/Catch2/blob/master/docs/own-main.md#let-catch-take-full-control-of-args-and-config
int main(int argc, char* argv[]) {
    Catch::Session().run(argc, argv);
    return 0;
}
