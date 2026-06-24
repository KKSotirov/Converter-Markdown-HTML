#pragma once

#include "Block.h"
#include "BlockParser.h"
#include "Types.h"

#include <string>
#include <vector>

class MarkdownDocument
{
private:
    std::vector<Block*> blocks;
    std::vector<ParseError> errors;
    int total_lines_read = 0;
    std::vector<BlockParserRule*> blockRules;

    void initializeRules();
    void clearRules();
    void clear();
    void copyFrom(const MarkdownDocument& other);

public:
    MarkdownDocument();
    ~MarkdownDocument();
    MarkdownDocument(const MarkdownDocument& other);
    MarkdownDocument(MarkdownDocument&& other) noexcept;
    MarkdownDocument& operator=(MarkdownDocument other);

    bool parseFromFile(const std::string& filename);
    void printErrors() const;
    void printStats() const;
    void printTreeStructure() const;
    void translateToHTML(const std::string& outfile) const;
};
