#include <catch2/catch.hpp>
#include <string>
#include <vector>
#include "../INIReader.h"

// Returns the names in iteration order, so tests can check the ordering.
template <typename Names>
static std::vector<std::string> InOrder(const Names& names)
{
    return std::vector<std::string>(names.begin(), names.end());
}

TEST_CASE("Reader preserves values and case-insensitive names")
{
    const char input[] =
        "[Server]\n"
        "Host=localhost\n"
        "Host=backup\n"
        "Flag=TrUe\n"
        "Blank=\n"
        "Blank=filled\n"
        "Trillion=1000000000000\n"
        "[SERVER]\n"
        "Port=8080\n"
        "[empty]\n"
        "[alpha]\n"
        "x=1\n"
        "X=2\n";
    const INIReader reader(input, sizeof(input) - 1);
    REQUIRE(reader.ParseError() == 0);
    REQUIRE(reader.Get("server", "host", "") == "localhost\nbackup");
    REQUIRE(reader.Get("server", "blank", "") == "filled");
    REQUIRE(reader.GetBoolean("server", "flag", false));
    REQUIRE(reader.GetInteger("server", "port", -1) == 8080);
    REQUIRE(reader.GetInteger64("server", "trillion", -1) == 1000000000000LL);
    REQUIRE(reader.GetUnsigned64("server", "trillion", 0) == 1000000000000ULL);
    REQUIRE(reader.Get("server", "missing", "fallback") == "fallback");
    REQUIRE(reader.GetFields("missing").empty());

    // Names are sorted ignoring case, and case variants keep the first spelling.
    const auto sections = reader.GetSections();
#if INI_CALL_HANDLER_ON_NEW_SECTION
    REQUIRE(InOrder(sections) == (std::vector<std::string>{"alpha", "empty", "Server"}));
#else
    REQUIRE(InOrder(sections) == (std::vector<std::string>{"alpha", "Server"}));
#endif
    REQUIRE(sections.count("SERVER") == 1);
    REQUIRE(*sections.find("SERVER") == "Server");
    REQUIRE(InOrder(reader.GetFields("sErVeR")) ==
           (std::vector<std::string>{"Blank", "Flag", "Host", "Port", "Trillion"}));
    REQUIRE(InOrder(reader.GetFields("alpha")) == std::vector<std::string>{"x"});
    REQUIRE(reader.GetFields("empty").empty());

    // Returned sets are independent of the stored indexes.
    auto fields = reader.GetFields("server");
    fields.clear();
    REQUIRE(reader.GetFields("server").size() == 5);
}
