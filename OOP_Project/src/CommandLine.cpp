#include "CommandLine.h"

#include "Command.h"
#include "MarkdownDocument.h"
#include "Utils.h"

#include <iostream>
#include <vector>

void CommandLine::run()
{
    MarkdownDocument doc;
    bool running = true;
    std::vector<Command*> registry;

    try
    {
        registry.push_back(new PrintCommand());
        registry.push_back(new ValidateCommand());
        registry.push_back(new InfoCommand());
        registry.push_back(new TranslateCommand());
        registry.push_back(new ExitCommand(running));
    }
    catch (...)
    {
        for (auto* cmd : registry) delete cmd;
        throw;
    }

    std::cout << "=== Markdown -> HTML Редактор (ФМИ ООП Проект) ===\n";
    std::cout << "Налични команди: print, validate, info, translate, exit\n";

    std::string inputLine;
    while (running && std::cout << "> " && std::getline(std::cin, inputLine))
    {
        std::string cleaned = trim(inputLine);
        if (cleaned.empty()) continue;

        std::vector<std::string> args = splitSpaces(cleaned);
        if (args.empty()) continue;

        bool found = false;
        for (auto* cmd : registry)
        {
            if (cmd->getName() == args[0])
            {
                cmd->execute(doc, args);
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cout << "Невалидна команда. Налични: print, validate, info, translate, exit\n";
        }
    }

    for (auto* cmd : registry) delete cmd;
}
