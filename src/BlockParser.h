#pragma once

#include "Block.h"
#include "InlineParser.h"
#include "Types.h"
#include "Utils.h"

#include <string>
#include <vector>

class BlockParserRule
{
public:
    virtual ~BlockParserRule() = default;
    virtual bool tryParse(const std::string& line, int line_num,
                          std::vector<Block*>& blocks,
                          std::vector<ParseError>& errors,
                          ListBlock*& current_list) = 0;
};

class HRRule : public BlockParserRule
{
public:
    bool tryParse(const std::string& line, int line_num,
                  std::vector<Block*>& blocks,
                  std::vector<ParseError>& errors,
                  ListBlock*& current_list) override;
};

class HeadingRule : public BlockParserRule
{
public:
    bool tryParse(const std::string& line, int line_num,
                  std::vector<Block*>& blocks,
                  std::vector<ParseError>& errors,
                  ListBlock*& current_list) override;
};

class ListItemRule : public BlockParserRule
{
public:
    bool tryParse(const std::string& line, int line_num,
                  std::vector<Block*>& blocks,
                  std::vector<ParseError>& errors,
                  ListBlock*& current_list) override;
};
