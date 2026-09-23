#include <cassert>
#include "../INIReader.h"

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
        "Port=8080\n";
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
    assert(reader.GetSections() == (std::set<std::string>{"Server", "SERVER"}));
    assert(reader.GetFields("sErVeR") ==
           (std::set<std::string>{"Host", "Flag", "Blank", "Trillion", "Port"}));

    // Returned sets are independent of the stored indexes.
    auto fields = reader.GetFields("server");
    fields.clear();
    assert(reader.GetFields("server").size() == 5);
    return 0;
}
