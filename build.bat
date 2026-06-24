@echo off

g++ -std=c++17 -Wall -Wextra -Isrc -o markdown_converter.exe src\main.cpp src\Utils.cpp src\TreePrinter.cpp src\InlineNode.cpp src\InlineParser.cpp src\Block.cpp src\BlockParser.cpp src\MarkdownDocument.cpp src\Command.cpp src\CommandLine.cpp

if %ERRORLEVEL% EQU 0 (
    echo Build OK
) else (
    echo Build FAILED
)