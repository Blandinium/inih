// Example that shows simple usage of the INIReader class

#include <iostream>
#include "../INIReader.h"

int main()
{
    INIReader reader("../../examples/test.ini");

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'test.ini'\n";
        return 1;
    }
    std::cout << "Config loaded from 'test.ini': version="
              << reader.GetInteger("protocol", "version", -1) << ", name="
              << reader.Get("user", "name", "UNKNOWN") << ", email="
              << reader.Get("user", "email", "UNKNOWN") << ", pi="
              << reader.GetReal("user", "pi", -1) << ", active="
              << reader.GetBoolean("user", "active", true) << "\n";

    std::cout<<"Found sections and fields:"<<std::endl;
    for (const auto& section : reader.GetSections())
    {
        std::cout << "  [" << section << "]: ";
        const char* separator = "";
        for (const auto& field : reader.GetFields(section))
        {
            std::cout << separator << field;
            separator = ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
