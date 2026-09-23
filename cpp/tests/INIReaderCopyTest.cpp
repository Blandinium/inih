// Regression tests for copying and assigning INIReader. Run from cpp/tests/.

#include <cassert>
#include "../INIReader.h"

static void CheckReader(INIReader& reader)
{
    assert(reader.ParseError() == 0);
    assert(reader.GetSections().size() == 2);
    assert(reader.GetSections().count("user") == 1);
    assert(reader.GetFields("USER").size() == 4);
    assert(reader.GetFields("USER").count("name") == 1);
    assert(reader.GetFields("missing").empty());
    assert(reader.Get("USER", "NAME", "missing") == "Bob Smith");
    assert(reader.GetInteger("protocol", "version", -1) == 6);
}

int main()
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
        assert(copied.ParseError() == 0);
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
    return 0;
}
