#pragma once

#include "InlineNode.h"
#include "Types.h"

#include <string>
#include <vector>

class InlineRule
{
public:
    virtual ~InlineRule() = default;
    virtual std::string getDelimiter() const = 0;
    virtual std::string getError() const = 0;
    virtual InlineNode* createNode(const InlineSequence& inner) const = 0;
    virtual bool isLeaf() const { return false; }
};

class BoldItalicRule : public InlineRule
{
public:
    std::string getDelimiter() const override;
    std::string getError() const override;
    InlineNode* createNode(const InlineSequence& inner) const override;
};

class ItalicRule : public InlineRule
{
public:
    std::string getDelimiter() const override;
    std::string getError() const override;
    InlineNode* createNode(const InlineSequence& inner) const override;
};

class BoldRule : public InlineRule
{
public:
    std::string getDelimiter() const override;
    std::string getError() const override;
    InlineNode* createNode(const InlineSequence& inner) const override;
};

class CodeRule : public InlineRule
{
public:
    std::string getDelimiter() const override;
    std::string getError() const override;
    InlineNode* createNode(const InlineSequence& inner) const override;
    bool isLeaf() const override;
};

class StrikeRule : public InlineRule
{
public:
    std::string getDelimiter() const override;
    std::string getError() const override;
    InlineNode* createNode(const InlineSequence& inner) const override;
};

class InlineParser
{
private:
    std::vector<InlineRule*> rules;

    InlineParser();
    ~InlineParser();

public:
    InlineParser(const InlineParser&) = delete;
    InlineParser& operator=(const InlineParser&) = delete;

    static InlineSequence parse(const std::string& text, int line_num, std::vector<ParseError>& errors);
};
