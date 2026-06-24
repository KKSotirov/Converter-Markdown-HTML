#pragma once

#include <string>

struct ParseError
{
    int line_number = 0;
    std::string message;
};

struct DocumentStats
{
    int total_lines = 0;
    int headings[7] = {0};
    int paragraphs = 0;
    int hr = 0;
    int list_items = 0;
    int bold = 0;
    int italic = 0;
    int code = 0;
    int strike = 0;
};

class TreePrinter;
