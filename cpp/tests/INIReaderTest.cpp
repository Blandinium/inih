#include <catch2/catch.hpp>
#include "../INIReader.h"

TEST_CASE("Reader loads the example configuration")
{
    INIReader reader("../../examples/test.ini");
    REQUIRE(reader.ParseError() == 0);
    REQUIRE(reader.GetInteger("protocol", "version", -1) == 6);
    REQUIRE(reader.Get("user", "name", "UNKNOWN") == "Bob Smith");
    REQUIRE(reader.Get("user", "email", "UNKNOWN") == "bob@smith.com");
    REQUIRE(reader.GetReal("user", "pi", -1) == Approx(3.14159));
    REQUIRE(reader.GetBoolean("user", "active", false));
    REQUIRE(reader.GetSections().size() == 2);
    REQUIRE(reader.GetFields("user").size() == 5);
}
