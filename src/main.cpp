#include "CommandLine.h"
#include <exception>
#include <iostream>
#include <new>
int main()
{
    try
    {
        CommandLine cli;
        cli.run();
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "Грешка: Недостатъчна памет (" << e.what() << "). Програмата приключва.\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Грешка: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Неочаквана грешка. Програмата приключва.\n";
        return 1;
    }
    return 0;
}


// Изкуствен интелект (AI) е 
//  * използван единствено като спомагателен инструмент за езиково оформяне на 
//  * техническата документация (README.md) i за генериране на примерно текстово 
//  * съдържание в тестовите .md файлове.