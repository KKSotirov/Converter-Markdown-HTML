#pragma once

#include "InlineNode.h"
#include "Types.h"
#include "TreePrinter.h"

#include <string>
#include <vector>

class Block
{
public:
    virtual ~Block() = default;
    virtual Block* clone() const = 0;
    virtual std::string toHTML() const = 0;
    virtual void printTree(int depth, TreePrinter& p) const = 0;
    virtual void collectStats(DocumentStats& stats) const = 0;
};

class HeadingBlock : public Block
{
private:
    int level;
    InlineSequence content;

public:
    HeadingBlock(int lvl, const InlineSequence& seq);
    Block* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class ParagraphBlock : public Block
{
private:
    InlineSequence content;

public:
    explicit ParagraphBlock(const InlineSequence& seq);
    Block* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class HRBlock : public Block
{
public:
    Block* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class ListBlock : public Block
{
private:
    bool ordered;
    std::vector<InlineSequence> items;

public:
    explicit ListBlock(bool ord);
    Block* clone() const override;
    bool isOrdered() const;
    void addItem(const InlineSequence& seq);
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};
