#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <array>
#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include <iostream>
#include <simdjson.h>
using namespace simdjson;

TEST_SUITE ("Example derived tests.") {
    TEST_CASE ("No error in parsing the JSON fixture.") {
        /* Example snippet:
         *
         * ondemand::parser parser;
         * padded_string json = padded_string::load("examples/basic.json");
         * ondemand::document tweets = parser.iterate(json);
         * std::cout << uint64_t(tweets["search_metadata"]["count"]) << " results." << std::endl;
         */
        ondemand::parser parser;
        padded_string json = padded_string::load("examples/basic.json");  // TODO(sthagen) - minimize platform and location dependency 
        ondemand::document tweets = parser.iterate(json);
        std::ostringstream os;
        os << uint64_t(tweets["search_metadata"]["count"]) << " results.";
        SUBCASE("The result report is not empty.") {
            REQUIRE(!os.str().empty());
        }
        SUBCASE("The result is correct in report.") {
            REQUIRE(os.str().find("42 results.") != std::string::npos);
        }
    }
}
