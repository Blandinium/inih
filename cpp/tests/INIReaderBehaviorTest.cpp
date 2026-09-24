#include <cassert>
#include <string>
#include <vector>
#include "../../ini.h"
#include "../INIReader.h"

// Returns the names in iteration order, so tests can check the ordering.
template <typename Names>
static std::vector<std::string> InOrder(const Names& names)
{
    return std::vector<std::string>(names.begin(), names.end());
}

int main()
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
    assert(reader.ParseError() == 0);
    assert(reader.Get("server", "host", "") == "localhost\nbackup");
    assert(reader.Get("server", "blank", "") == "filled");
    assert(reader.GetBoolean("server", "flag", false));
    assert(reader.GetInteger("server", "port", -1) == 8080);
    assert(reader.GetInteger64("server", "trillion", -1) == 1000000000000LL);
    assert(reader.GetUnsigned64("server", "trillion", 0) == 1000000000000ULL);
    assert(reader.Get("server", "missing", "fallback") == "fallback");
    assert(reader.GetFields("missing").empty());

    // Names are sorted ignoring case, and case variants keep the first spelling.
    const auto sections = reader.GetSections();
#if INI_CALL_HANDLER_ON_NEW_SECTION
    assert(InOrder(sections) == (std::vector<std::string>{"alpha", "empty", "Server"}));
#else
    assert(InOrder(sections) == (std::vector<std::string>{"alpha", "Server"}));
#endif
    assert(sections.count("SERVER") == 1);
    assert(*sections.find("SERVER") == "Server");
    assert(InOrder(reader.GetFields("sErVeR")) ==
           (std::vector<std::string>{"Blank", "Flag", "Host", "Port", "Trillion"}));
    assert(InOrder(reader.GetFields("alpha")) == std::vector<std::string>{"x"});
    assert(reader.GetFields("empty").empty());

    // Returned sets are independent of the stored indexes.
    auto fields = reader.GetFields("server");
    fields.clear();
    assert(reader.GetFields("server").size() == 5);
    return 0;
}
