#include <catch2/catch.hpp>
#include <type_traits>
#include <utility>
#include "../INIReader.h"

static_assert(std::is_copy_constructible<INIReader>::value, "Reader must remain copyable");
static_assert(std::is_copy_assignable<INIReader>::value, "Reader must remain copy assignable");
static_assert(std::is_move_constructible<INIReader>::value, "Reader must support moving");
static_assert(std::is_move_assignable<INIReader>::value, "Reader must support move assignment");

static void CheckReader(const INIReader& reader)
{
    REQUIRE(reader.ParseError() == 0);
    const auto sections = reader.GetSections();
    REQUIRE(sections == (decltype(sections){"protocol", "user"}));
    const auto fields = reader.GetFields("USER");
    REQUIRE(fields == (decltype(fields){"active", "email", "name", "pi"}));
    REQUIRE(reader.Get("USER", "NAME", "missing") == "Bob Smith");
    REQUIRE(reader.GetInteger("protocol", "version", -1) == 6);
}

static INIReader MoveFromLocal()
{
    INIReader source("fixtures/reader.ini");
    INIReader moved(std::move(source));
    CheckReader(moved);

    // Reuse the source without assuming its containers are empty after a move.
    source = INIReader("fixtures/replacement.ini");
    REQUIRE(source.ParseError() == 0);
    CheckReader(moved);
    return moved;
}

TEST_CASE("Moves preserve values and independent indexes")
{
    INIReader constructed = MoveFromLocal();
    CheckReader(constructed);

    INIReader assigned("fixtures/replacement.ini");
    {
        INIReader source("fixtures/reader.ini");
        assigned = std::move(source);
        CheckReader(assigned);
        source = INIReader("fixtures/replacement.ini");
        REQUIRE(source.ParseError() == 0);
    }
    CheckReader(assigned);

    // Moving does not remove the ability to make independent copies.
    INIReader copied(assigned);
    constructed = assigned;
    assigned = INIReader("fixtures/replacement.ini");
    CheckReader(copied);
    CheckReader(constructed);

    INIReader failed("fixtures/nonexistent.ini");
    REQUIRE(failed.ParseError() == -1);
    INIReader movedFailure(std::move(failed));
    REQUIRE(movedFailure.ParseError() == -1);
    REQUIRE(movedFailure.GetSections().empty());
}
