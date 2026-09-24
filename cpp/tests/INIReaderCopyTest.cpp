// Regression tests for copying and assigning INIReader.

#include <catch2/catch.hpp>
#include "../INIReader.h"

static void CheckReader(INIReader& reader)
{
    REQUIRE(reader.ParseError() == 0);
    REQUIRE(reader.GetSections().size() == 2);
    REQUIRE(reader.GetSections().count("user") == 1);
    REQUIRE(reader.GetFields("USER").size() == 4);
    REQUIRE(reader.GetFields("USER").count("name") == 1);
    REQUIRE(reader.GetFields("missing").empty());
    REQUIRE(reader.Get("USER", "NAME", "missing") == "Bob Smith");
    REQUIRE(reader.GetInteger("protocol", "version", -1) == 6);
}

TEST_CASE("Copies own their section and field indexes")
{
    INIReader assigned("fixtures/reader.ini");
    {
        const INIReader source("fixtures/reader.ini");
        INIReader copied(source);
        CheckReader(copied);

        // Replace a populated destination, then destroy the source.
        assigned = source;
        CheckReader(assigned);

        // Replacing one copy must not invalidate another copy's fields.
        copied = INIReader("fixtures/replacement.ini");
        REQUIRE(copied.ParseError() == 0);
        CheckReader(assigned);
    }
    CheckReader(assigned);

    INIReader* alias = &assigned;
    assigned = *alias;
    CheckReader(assigned);

    // Destroy a copy before its source and check that the source survives.
    {
        INIReader copied(assigned);
        CheckReader(copied);
    }
    CheckReader(assigned);
}
