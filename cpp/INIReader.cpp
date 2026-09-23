// Read an INI file into easy-to-access name/value pairs.

// SPDX-License-Identifier: BSD-3-Clause

// Copyright (C) 2009-2020, Ben Hoyt

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "../ini.h"
#include "INIReader.h"

using std::string;

namespace {
string LowerCase(string text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return text;
}
}

INIReader::INIReader(const string& filename)
{
    _error = ini_parse(filename.c_str(), ValueHandler, this);
}

INIReader::INIReader(const char *buffer, size_t buffer_size)
{
  string content(buffer, buffer_size);
  _error = ini_parse_string(content.c_str(), ValueHandler, this);
}

int INIReader::ParseError() const
{
    return _error;
}

string INIReader::Get(const string& section, const string& name, const string& default_value) const
{
    const auto value = _values.find(MakeKey(section, name));
    return value != _values.end() ? value->second : default_value;
}

string INIReader::GetString(const string& section, const string& name, const string& default_value) const
{
    const string str = Get(section, name, "");
    return str.empty() ? default_value : str;
}

long INIReader::GetInteger(const string& section, const string& name, long default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

INI_API std::int64_t INIReader::GetInteger64(const std::string& section, const std::string& name, std::int64_t default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    std::int64_t n = strtoll(value, &end, 0);
    return end > value ? n : default_value;
}

unsigned long INIReader::GetUnsigned(const string& section, const string& name, unsigned long default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    unsigned long n = strtoul(value, &end, 0);
    return end > value ? n : default_value;
}

INI_API std::uint64_t INIReader::GetUnsigned64(const std::string& section, const std::string& name, std::uint64_t default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    std::uint64_t n = strtoull(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::GetReal(const string& section, const string& name, double default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::GetBoolean(const string& section, const string& name, bool default_value) const
{
    const string valstr = LowerCase(Get(section, name, ""));
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

std::vector<string> INIReader::Sections() const
{
    std::set<string> sectionSet;
    for (std::map<string, string>::const_iterator it = _values.begin(); it != _values.end(); ++it) {
        size_t pos = it->first.find('=');
        if (pos != string::npos) {
            sectionSet.insert(it->first.substr(0, pos));
        }
    }
    return std::vector<string>(sectionSet.begin(), sectionSet.end());
}

std::vector<string> INIReader::Keys(const string& section) const
{
    std::vector<string> keys;
    string keyPrefix = MakeKey(section, "");
    for (std::map<string, string>::const_iterator it = _values.begin(); it != _values.end(); ++it) {
        if (it->first.compare(0, keyPrefix.length(), keyPrefix) == 0) {
            keys.push_back(it->first.substr(keyPrefix.length()));
        }
    }
    return keys;
}

bool INIReader::HasSection(const string& section) const
{
    const string key = MakeKey(section, "");
    const auto pos = _values.lower_bound(key);
    if (pos == _values.end())
        return false;
    // Does the key at the lower_bound pos start with "section"?
    return pos->first.compare(0, key.length(), key) == 0;
}

bool INIReader::HasValue(const string& section, const string& name) const
{
    string key = MakeKey(section, name);
    return _values.count(key);
}

std::set<std::string> INIReader::GetSections() const
{
    return _sections;
}

std::set<std::string> INIReader::GetFields(const std::string& section) const
{
    const auto fieldSetIt = _fields.find(LowerCase(section));
    if (fieldSetIt == _fields.end())
        return {};
    return fieldSetIt->second;
}

string INIReader::MakeKey(const string& section, const string& name)
{
    return LowerCase(section + "=" + name);
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    if (!name)  // Happens when INI_CALL_HANDLER_ON_NEW_SECTION enabled
        return 1;
    auto* reader = static_cast<INIReader*>(user);
    const auto entry = reader->_values.emplace(MakeKey(section, name), value ? value : "");
    if (!entry.second) {
        auto& stored = entry.first->second;
        if (!stored.empty())
            stored += "\n";
        stored += value ? value : "";
    }

    // Preserve the original spelling while keeping section lookups case-insensitive.
    reader->_sections.emplace(section);
    reader->_fields[LowerCase(section)].emplace(name);

    return 1;
}
