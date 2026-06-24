#pragma once

#include "MarkdownDocument.h"

#include <string>
#include <vector>

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute(MarkdownDocument& doc, const std::vector<std::string>& args) = 0;
    virtual std::string getName() const = 0;
};

class DocumentCommand : public Command
{
protected:
    virtual void executeWithFile(MarkdownDocument& doc, const std::vector<std::string>& args) = 0;
    virtual size_t requiredArgs() const { return 2; }
    virtual std::string getUsage() const = 0;

public:
    void execute(MarkdownDocument& doc, const std::vector<std::string>& args) override;
};

class PrintCommand : public DocumentCommand
{
public:
    std::string getName() const override;

protected:
    std::string getUsage() const override;
    void executeWithFile(MarkdownDocument& doc, const std::vector<std::string>& args) override;
};

class ValidateCommand : public DocumentCommand
{
public:
    std::string getName() const override;

protected:
    std::string getUsage() const override;
    void executeWithFile(MarkdownDocument& doc, const std::vector<std::string>& args) override;
};

class InfoCommand : public DocumentCommand
{
public:
    std::string getName() const override;

protected:
    std::string getUsage() const override;
    void executeWithFile(MarkdownDocument& doc, const std::vector<std::string>& args) override;
};

class TranslateCommand : public DocumentCommand
{
public:
    std::string getName() const override;

protected:
    size_t requiredArgs() const override;
    std::string getUsage() const override;
    void executeWithFile(MarkdownDocument& doc, const std::vector<std::string>& args) override;
};

class ExitCommand : public Command
{
private:
    bool& m_running;

public:
    explicit ExitCommand(bool& running);
    std::string getName() const override;
    void execute(MarkdownDocument& doc, const std::vector<std::string>& args) override;
};
