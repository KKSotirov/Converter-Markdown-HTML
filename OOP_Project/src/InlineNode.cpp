#include "InlineNode.h"

void InlineSequence::clear()
{
    for (auto* node : parts) delete node;
    parts.clear();
}

void InlineSequence::copyFrom(const InlineSequence& other)
{
    parts.reserve(other.parts.size());
    for (auto* node : other.parts)
        parts.push_back(node ? node->clone() : nullptr);
}

InlineSequence::~InlineSequence()
{
    clear();
}

InlineSequence::InlineSequence(const InlineSequence& other)
{
    copyFrom(other);
}

InlineSequence::InlineSequence(InlineSequence&& other) noexcept
    : parts(other.parts)
{
    other.parts.clear();
}

InlineSequence& InlineSequence::operator=(InlineSequence other)
{
    parts.swap(other.parts);
    return *this;
}

void InlineSequence::add(InlineNode* node)
{
    if (node) parts.push_back(node);
}

std::string InlineSequence::toHTML() const
{
    std::string res;
    for (auto* node : parts)
        if (node) res += node->toHTML();
    return res;
}

void InlineSequence::printTree(int depth, TreePrinter& p) const
{
    for (auto* node : parts)
        if (node) node->printTree(depth, p);
}

void InlineSequence::collectStats(DocumentStats& stats) const
{
    for (auto* node : parts)
        if (node) node->collectStats(stats);
}

TextNode::TextNode(const std::string& t) : text(t) {}

InlineNode* TextNode::clone() const
{
    return new TextNode(text);
}

std::string TextNode::toHTML() const
{
    return text;
}

void TextNode::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Text: \"" + text + "\"");
}

void TextNode::collectStats(DocumentStats&) const {}

BoldNode::BoldNode(const InlineSequence& seq) : content(seq) {}

InlineNode* BoldNode::clone() const
{
    return new BoldNode(content);
}

std::string BoldNode::toHTML() const
{
    return "<b>" + content.toHTML() + "</b>";
}

void BoldNode::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Bold (*):");
    content.printTree(depth + 2, p);
}

void BoldNode::collectStats(DocumentStats& stats) const
{
    stats.bold++;
    content.collectStats(stats);
}

ItalicNode::ItalicNode(const InlineSequence& seq) : content(seq) {}

InlineNode* ItalicNode::clone() const
{
    return new ItalicNode(content);
}

std::string ItalicNode::toHTML() const
{
    return "<i>" + content.toHTML() + "</i>";
}

void ItalicNode::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Italic (**):");
    content.printTree(depth + 2, p);
}

void ItalicNode::collectStats(DocumentStats& stats) const
{
    stats.italic++;
    content.collectStats(stats);
}

CodeNode::CodeNode(const InlineSequence& seq) : content(seq) {}

InlineNode* CodeNode::clone() const
{
    return new CodeNode(content);
}

std::string CodeNode::toHTML() const
{
    return "<code> " + content.toHTML() + " </code>";
}

void CodeNode::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Code span (`):");
    content.printTree(depth + 2, p);
}

void CodeNode::collectStats(DocumentStats& stats) const
{
    stats.code++;
    content.collectStats(stats);
}

StrikeNode::StrikeNode(const InlineSequence& seq) : content(seq) {}

InlineNode* StrikeNode::clone() const
{
    return new StrikeNode(content);
}

std::string StrikeNode::toHTML() const
{
    return "<s>" + content.toHTML() + "</s>";
}

void StrikeNode::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Strike (~):");
    content.printTree(depth + 2, p);
}

void StrikeNode::collectStats(DocumentStats& stats) const
{
    stats.strike++;
    content.collectStats(stats);
}
