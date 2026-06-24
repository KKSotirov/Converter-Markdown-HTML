#pragma once

#include <string>
#include <vector>

bool isSpace(char c);
bool isDigit(char c);
std::string trim(const std::string& s);
std::vector<std::string> splitSpaces(const std::string& str);
bool isTextFile(const std::string& filename);
