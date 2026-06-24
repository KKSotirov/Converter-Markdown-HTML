#include "InlineParser.h"

std::string BoldItalicRule::getDelimiter() const { return "***"; }
std::string BoldItalicRule::getError() const { return "Незатворен маркер за болд+италик (***)"; }

InlineNode* BoldItalicRule::createNode(const InlineSequence& inner) const
{
    InlineNode* boldNode = new BoldNode(inner);
    InlineSequence wrap;
    wrap.add(boldNode);
    return new ItalicNode(wrap);
}

std::string ItalicRule::getDelimiter() const { return "**"; }
std::string ItalicRule::getError() const { return "Незатворен маркер за италик (**)"; }
InlineNode* ItalicRule::createNode(const InlineSequence& inner) const { return new ItalicNode(inner); }

std::string BoldRule::getDelimiter() const { return "*"; }
std::string BoldRule::getError() const { return "Незатворен маркер за болд (*)"; }
InlineNode* BoldRule::createNode(const InlineSequence& inner) const { return new BoldNode(inner); }

std::string CodeRule::getDelimiter() const { return "`"; }
std::string CodeRule::getError() const { return "Незатворен маркер за код (`)"; }
InlineNode* CodeRule::createNode(const InlineSequence& inner) const { return new CodeNode(inner); }
bool CodeRule::isLeaf() const { return true; }

std::string StrikeRule::getDelimiter() const { return "~"; }
std::string StrikeRule::getError() const { return "Незатворен маркер за зачеркнат текст (~)"; }
InlineNode* StrikeRule::createNode(const InlineSequence& inner) const { return new StrikeNode(inner); }

InlineParser::InlineParser()
{
    try
    {
        rules.push_back(new BoldItalicRule());
        rules.push_back(new ItalicRule());
        rules.push_back(new BoldRule());
        rules.push_back(new CodeRule());
        rules.push_back(new StrikeRule());
    }
    catch (...)
    {
        for (auto* r : rules) delete r;
        rules.clear();
        throw;
    }
}

InlineParser::~InlineParser()
{
    for (auto* r : rules) delete r;
}

InlineSequence InlineParser::parse(const std::string& text, int line_num, std::vector<ParseError>& errors)
{
    static InlineParser instance;

    InlineSequence seq;
    size_t i = 0;
    size_t n = text.size();
    std::string current_text;

    while (i < n)
    {
        InlineRule* matched = nullptr;
        for (auto* rule : instance.rules)
        {
            const std::string& delim = rule->getDelimiter();
            if (i + delim.size() <= n && text.compare(i, delim.size(), delim) == 0)
            {
                matched = rule;
                break;
            }
        }

        if (matched)
        {
            if (!current_text.empty())
            {
                seq.add(new TextNode(current_text));
                current_text.clear();
            }

            const std::string& delim = matched->getDelimiter();
            size_t close = text.find(delim, i + delim.size());

            if (close != std::string::npos)
            {
                std::string inner = text.substr(i + delim.size(), close - (i + delim.size()));
                if (matched->isLeaf())
                {
                    InlineSequence leaf;
                    leaf.add(new TextNode(inner));
                    seq.add(matched->createNode(leaf));
                }
                else
                {
                    seq.add(matched->createNode(parse(inner, line_num, errors)));
                }
                i = close + delim.size();
            }
            else
            {
                errors.push_back({line_num, matched->getError()});
                current_text += delim;
                i += delim.size();
            }
        }
        else
        {
            current_text += text[i];
            ++i;
        }
    }

    if (!current_text.empty())
        seq.add(new TextNode(current_text));

    return seq;
}
