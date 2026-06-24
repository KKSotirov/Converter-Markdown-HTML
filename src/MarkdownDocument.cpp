#include "MarkdownDocument.h"

#include "InlineParser.h"
#include "TreePrinter.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <utility>

void MarkdownDocument::initializeRules()
{
    try
    {
        blockRules.push_back(new HRRule());
        blockRules.push_back(new HeadingRule());
        blockRules.push_back(new ListItemRule());
    }
    catch (...)
    {
        for (auto* r : blockRules) delete r;
        blockRules.clear();
        throw;
    }
}

void MarkdownDocument::clearRules()
{
    for (auto* r : blockRules) delete r;
    blockRules.clear();
}

void MarkdownDocument::clear()
{
    for (auto* b : blocks) delete b;
    blocks.clear();
    errors.clear();
    total_lines_read = 0;
}

void MarkdownDocument::copyFrom(const MarkdownDocument& other)
{
    blocks.reserve(other.blocks.size());
    for (auto* b : other.blocks)
        blocks.push_back(b ? b->clone() : nullptr);
    errors = other.errors;
    total_lines_read = other.total_lines_read;
}

MarkdownDocument::MarkdownDocument()
{
    initializeRules();
}

MarkdownDocument::~MarkdownDocument()
{
    clear();
    clearRules();
}

MarkdownDocument::MarkdownDocument(const MarkdownDocument& other)
{
    initializeRules();
    copyFrom(other);
}

MarkdownDocument::MarkdownDocument(MarkdownDocument&& other) noexcept
    : blocks(other.blocks),
      errors(std::move(other.errors)),
      total_lines_read(other.total_lines_read)
{
    other.blocks.clear();
    other.total_lines_read = 0;
    initializeRules();
}

MarkdownDocument& MarkdownDocument::operator=(MarkdownDocument other)
{
    blocks.swap(other.blocks);
    errors.swap(other.errors);
    int tmp = total_lines_read;
    total_lines_read = other.total_lines_read;
    other.total_lines_read = tmp;
    return *this;
}

bool MarkdownDocument::parseFromFile(const std::string& filename)
{
    clear();

    if (!isTextFile(filename))
    {
        std::cout << "Грешка: Файлът '" << filename << "' не съществува или не е текстов.\n";
        return false;
    }

    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "Грешка: Не може да се отвори файлът '" << filename << "'.\n";
        return false;
    }

    std::string line;
    ListBlock* current_list = nullptr;

    try
    {
        while (std::getline(in, line))
        {
            if (in.bad())
            {
                std::cout << "Грешка: Проблем при четене на файла (I/O грешка).\n";
                if (current_list) { delete current_list; current_list = nullptr; }
                return false;
            }

            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);

            ++total_lines_read;

            bool is_empty = true;
            for (char c : line)
            {
                if (!isSpace(c)) { is_empty = false; break; }
            }
            if (is_empty)
            {
                if (current_list)
                {
                    blocks.push_back(current_list);
                    current_list = nullptr;
                }
                continue;
            }

            std::string lead = trim(line);
            bool matched = false;
            for (auto* rule : blockRules)
            {
                if (rule->tryParse(lead, total_lines_read, blocks, errors, current_list))
                {
                    matched = true;
                    break;
                }
            }

            if (!matched)
            {
                if (current_list)
                {
                    blocks.push_back(current_list);
                    current_list = nullptr;
                }
                InlineSequence seq = InlineParser::parse(lead, total_lines_read, errors);
                blocks.push_back(new ParagraphBlock(seq));
            }
        }

        if (in.bad())
        {
            std::cout << "Грешка: Проблем при четене на файла (I/O грешка).\n";
            if (current_list) { delete current_list; current_list = nullptr; }
            return false;
        }
    }
    catch (...)
    {
        if (current_list) { delete current_list; current_list = nullptr; }
        throw;
    }

    if (current_list)
    {
        blocks.push_back(current_list);
        current_list = nullptr;
    }
    return true;
}

void MarkdownDocument::printErrors() const
{
    if (errors.empty())
    {
        std::cout << "Документът е напълно валиден съгласно спецификацията!\n";
        return;
    }
    std::cout << "Списък на грешките по редове:\n";
    for (const auto& err : errors)
        std::cout << "  Ред " << err.line_number << ": " << err.message << "\n";
}

void MarkdownDocument::printStats() const
{
    DocumentStats stats;
    stats.total_lines = total_lines_read;
    for (auto* b : blocks)
        if (b) b->collectStats(stats);

    std::cout << "Статистика на файла:\n";
    std::cout << "  Брой редове: " << stats.total_lines << "\n";
    std::cout << "  Разпределение по тип блокове:\n";
    std::cout << "    h1: " << stats.headings[1] << ", h2: " << stats.headings[2]
              << ", h3: " << stats.headings[3] << ", h4: " << stats.headings[4]
              << ", h5: " << stats.headings[5] << ", h6: " << stats.headings[6] << "\n";
    std::cout << "    p: " << stats.paragraphs << ", hr: " << stats.hr
              << ", li: " << stats.list_items << "\n";
    std::cout << "  Брой форматиращи елементи:\n";
    std::cout << "    <i>: " << stats.italic << ", <b>: " << stats.bold
              << ", <code>: " << stats.code << ", <s>: " << stats.strike << "\n";
}

void MarkdownDocument::printTreeStructure() const
{
    TreePrinter p;
    p.print("--- СИНТАКТИЧНО ДЪРВО ---");
    for (auto* b : blocks)
        if (b) b->printTree(0, p);
}

void MarkdownDocument::translateToHTML(const std::string& outfile) const
{
    std::ifstream check(outfile);
    if (check.good())
    {
        check.close();
        std::cout << "Внимание: Файлът '" << outfile << "' вече съществува. Презаписване? (y/n): ";

        if (!std::cin.good())
        {
            std::cout << "\nГрешка: Входът е в лошо състояние. Преводът е прекратен.\n";
            std::cin.clear();
            return;
        }

        char ans = 'n';
        std::cin >> ans;
        std::cin.ignore(10000, '\n');

        if (ans != 'y' && ans != 'Y')
        {
            std::cout << "Преводът е прекратен от потребителя.\n";
            return;
        }
    }

    std::ofstream out(outfile);
    if (!out.is_open())
    {
        std::cout << "Грешка: Не може да се създаде изходният файл.\n";
        return;
    }

    out << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n";
    out << "<title>Преведен Документ</title>\n</head>\n<body>\n";
    for (auto* b : blocks)
        if (b) out << b->toHTML();
    out << "</body>\n</html>\n";

    out.close();
    if (out.fail())
    {
        std::cout << "Грешка: Проблем при запис на изходния файл (диск пълен?).\n";
        return;
    }

    std::cout << "Успешно генериран HTML: " << outfile << "\n";
}
