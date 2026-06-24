#pragma once

#include <string>

class TreePrinter
{
private:
    int lineCount = 0;
    static const int MAX_LINES = 22;

public:
    void print(const std::string& text);
};
