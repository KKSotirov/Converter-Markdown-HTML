#include "Utils.h"

#include <fstream>

bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

std::string trim(const std::string& s)
{
    size_t a = 0;
    while (a < s.size() && isSpace(s[a])) ++a;
    size_t b = s.size();
    while (b > a && isSpace(s[b - 1])) --b;
    return s.substr(a, b - a);
}

std::vector<std::string> splitSpaces(const std::string& str)
{
    std::vector<std::string> tokens;
    std::string current;
    for (char c : str)
    {
        if (isSpace(c))
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

bool isTextFile(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) return false;

    char buffer[1024];
    in.read(buffer, sizeof(buffer));
    std::streamsize n = in.gcount();
    in.close();

    for (std::streamsize i = 0; i < n; ++i)
        if (buffer[i] == '\0') return false;
    return true;
}
