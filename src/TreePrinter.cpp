#include "TreePrinter.h"

#include <iostream>

void TreePrinter::print(const std::string& text)
{
    std::cout << text << '\n';
    ++lineCount;
    if (lineCount >= MAX_LINES)
    {
        std::cout << "[ Натиснете Enter за следваща страница... ]";
        std::string dummy;
        std::getline(std::cin, dummy);
        lineCount = 0;
    }
}
