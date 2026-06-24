#include "Block.h"

HeadingBlock::HeadingBlock(int lvl, const InlineSequence& seq) : level(lvl), content(seq) {}

Block* HeadingBlock::clone() const
{
    return new HeadingBlock(*this);
}

std::string HeadingBlock::toHTML() const
{
    return "<h" + std::to_string(level) + "> " + content.toHTML()
         + " </h" + std::to_string(level) + ">\n";
}

void HeadingBlock::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Heading (H" + std::to_string(level) + "):");
    content.printTree(depth + 2, p);
}

void HeadingBlock::collectStats(DocumentStats& stats) const
{
    stats.headings[level]++;
    content.collectStats(stats);
}

ParagraphBlock::ParagraphBlock(const InlineSequence& seq) : content(seq) {}

Block* ParagraphBlock::clone() const
{
    return new ParagraphBlock(*this);
}

std::string ParagraphBlock::toHTML() const
{
    return "<p> " + content.toHTML() + " </p>\n";
}

void ParagraphBlock::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Paragraph:");
    content.printTree(depth + 2, p);
}

void ParagraphBlock::collectStats(DocumentStats& stats) const
{
    stats.paragraphs++;
    content.collectStats(stats);
}

Block* HRBlock::clone() const
{
    return new HRBlock(*this);
}

std::string HRBlock::toHTML() const
{
    return "<hr></hr>\n";
}

void HRBlock::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + "Horizontal Rule (HR)");
}

void HRBlock::collectStats(DocumentStats& stats) const
{
    stats.hr++;
}

ListBlock::ListBlock(bool ord) : ordered(ord) {}

Block* ListBlock::clone() const
{
    return new ListBlock(*this);
}

bool ListBlock::isOrdered() const
{
    return ordered;
}

void ListBlock::addItem(const InlineSequence& seq)
{
    items.push_back(seq);
}

std::string ListBlock::toHTML() const
{
    std::string tag = ordered ? "ol" : "ul";
    std::string res = "<" + tag + ">\n";
    for (const auto& item : items)
        res += "  <li> " + item.toHTML() + " </li>\n";
    res += "</" + tag + ">\n";
    return res;
}

void ListBlock::printTree(int depth, TreePrinter& p) const
{
    p.print(std::string(depth, ' ') + (ordered ? "Ordered List (<ol>):" : "Unordered List (<ul>):"));
    for (const auto& item : items)
    {
        p.print(std::string(depth + 2, ' ') + "List Item (<li>):");
        item.printTree(depth + 4, p);
    }
}

void ListBlock::collectStats(DocumentStats& stats) const
{
    stats.list_items += static_cast<int>(items.size());
    for (const auto& item : items)
        item.collectStats(stats);
}
