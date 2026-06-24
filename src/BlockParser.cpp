#include "BlockParser.h"

bool HRRule::tryParse(const std::string& line, int, std::vector<Block*>& blocks,
                      std::vector<ParseError>&, ListBlock*& current_list)
{
    if (line != "- - -") return false;
    if (current_list)
    {
        blocks.push_back(current_list);
        current_list = nullptr;
    }
    blocks.push_back(new HRBlock());
    return true;
}

bool HeadingRule::tryParse(const std::string& line, int line_num, std::vector<Block*>& blocks,
                           std::vector<ParseError>& errors, ListBlock*& current_list)
{
    if (line.empty() || line[0] != '#') return false;

    size_t h = 0;
    while (h < line.size() && line[h] == '#') ++h;

    if (h > 6)
    {
        errors.push_back({line_num, "Невалидно ниво на заглавие (повече от 6 нива на #)"});
        return false;
    }

    if (h < line.size() && line[h] == ' ')
    {
        if (current_list)
        {
            blocks.push_back(current_list);
            current_list = nullptr;
        }
        std::string txt = trim(line.substr(h));
        InlineSequence seq = InlineParser::parse(txt, line_num, errors);
        blocks.push_back(new HeadingBlock(static_cast<int>(h), seq));
        return true;
    }
    return false;
}

bool ListItemRule::tryParse(const std::string& line, int line_num, std::vector<Block*>& blocks,
                            std::vector<ParseError>& errors, ListBlock*& current_list)
{
    bool is_unordered = (line.size() >= 2 && (line[0] == '-' || line[0] == '*') && line[1] == ' ');
    bool is_ordered = false;
    size_t dot_pos = 0;

    if (!line.empty() && isDigit(line[0]))
    {
        while (dot_pos < line.size() && isDigit(line[dot_pos])) ++dot_pos;
        if (dot_pos < line.size() && line[dot_pos] == '.'
            && dot_pos + 1 < line.size() && line[dot_pos + 1] == ' ')
            is_ordered = true;
    }

    if (is_ordered && line[0] == '0')
        is_ordered = false;

    if (!is_unordered && !is_ordered) return false;

    size_t skip = is_unordered ? 2 : (dot_pos + 2);
    std::string txt = trim(line.substr(skip));
    InlineSequence seq = InlineParser::parse(txt, line_num, errors);

    if (current_list && current_list->isOrdered() != is_ordered)
    {
        blocks.push_back(current_list);
        current_list = nullptr;
    }
    if (!current_list)
        current_list = new ListBlock(is_ordered);
    current_list->addItem(seq);
    return true;
}
