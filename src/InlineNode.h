#pragma once

#include "Types.h"
#include "TreePrinter.h"

#include <string>
#include <vector>

class InlineNode
{
public:
    virtual ~InlineNode() = default;
    virtual InlineNode* clone() const = 0;
    virtual std::string toHTML() const = 0;
    virtual void printTree(int depth, TreePrinter& p) const = 0;
    virtual void collectStats(DocumentStats& stats) const = 0;
};

class InlineSequence
{
private:
    std::vector<InlineNode*> parts;

    void clear();
    void copyFrom(const InlineSequence& other);

public:
    InlineSequence() = default;
    ~InlineSequence();
    InlineSequence(const InlineSequence& other);
    InlineSequence(InlineSequence&& other) noexcept;
    InlineSequence& operator=(InlineSequence other);

    void add(InlineNode* node);
    std::string toHTML() const;
    void printTree(int depth, TreePrinter& p) const;
    void collectStats(DocumentStats& stats) const;
};

class TextNode : public InlineNode
{
private:
    std::string text;

public:
    explicit TextNode(const std::string& t);
    InlineNode* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class BoldNode : public InlineNode
{
private:
    InlineSequence content;

public:
    explicit BoldNode(const InlineSequence& seq);
    InlineNode* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class ItalicNode : public InlineNode
{
private:
    InlineSequence content;

public:
    explicit ItalicNode(const InlineSequence& seq);
    InlineNode* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class CodeNode : public InlineNode
{
private:
    InlineSequence content;

public:
    explicit CodeNode(const InlineSequence& seq);
    InlineNode* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};

class StrikeNode : public InlineNode
{
private:
    InlineSequence content;

public:
    explicit StrikeNode(const InlineSequence& seq);
    InlineNode* clone() const override;
    std::string toHTML() const override;
    void printTree(int depth, TreePrinter& p) const override;
    void collectStats(DocumentStats& stats) const override;
};
