#include "Command.h"

#include <iostream>

void DocumentCommand::execute(MarkdownDocument& doc, const std::vector<std::string>& args)
{
    if (args.size() < requiredArgs())
    {
        std::cout << "Грешка: Липсват аргументи. Използвайте: " << getUsage() << "\n";
        return;
    }
    if (!doc.parseFromFile(args[1]))
    {
        std::cout << "Грешка при зареждане на файла: " << args[1] << "\n";
        return;
    }
    executeWithFile(doc, args);
}

std::string PrintCommand::getName() const { return "print"; }
std::string PrintCommand::getUsage() const { return "print <input>"; }
void PrintCommand::executeWithFile(MarkdownDocument& doc, const std::vector<std::string>&)
{
    doc.printTreeStructure();
}

std::string ValidateCommand::getName() const { return "validate"; }
std::string ValidateCommand::getUsage() const { return "validate <input>"; }
void ValidateCommand::executeWithFile(MarkdownDocument& doc, const std::vector<std::string>&)
{
    doc.printErrors();
}

std::string InfoCommand::getName() const { return "info"; }
std::string InfoCommand::getUsage() const { return "info <input>"; }
void InfoCommand::executeWithFile(MarkdownDocument& doc, const std::vector<std::string>&)
{
    doc.printStats();
}

std::string TranslateCommand::getName() const { return "translate"; }
size_t TranslateCommand::requiredArgs() const { return 3; }
std::string TranslateCommand::getUsage() const { return "translate <input> <output>"; }
void TranslateCommand::executeWithFile(MarkdownDocument& doc, const std::vector<std::string>& args)
{
    doc.translateToHTML(args[2]);
}

ExitCommand::ExitCommand(bool& running) : m_running(running) {}
std::string ExitCommand::getName() const { return "exit"; }
void ExitCommand::execute(MarkdownDocument&, const std::vector<std::string>&)
{
    m_running = false;
}
