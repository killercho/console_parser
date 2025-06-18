#include "ConsoleParser.h"

#include <algorithm>
#include <sstream>
#include <utility>
#include <stdexcept>

void ConsoleParser::ParseArguments(const int argc, const char* const* argv) {
    m_currentArguments.clear();
    // Start from 1 because the first argument is the name of the app
    for (int i = 1; i < argc; ++i) {
        m_currentArguments.emplace_back(argv[i]);
    }
}

void ConsoleParser::Reset() {
    m_currentArguments.clear();
    m_argumentsRegistered.clear();
}

void ConsoleParser::RegisterArgument(const std::string& argumentName, bool shortForm,
                                     bool additionalValue, const std::string& helpMessage) {
    if (m_argumentsRegistered.count(argumentName) != 0) {
        throw std::runtime_error(
            ("Argument '" + argumentName + "' was already registered!").c_str());
    }

    m_argumentsRegistered.insert(std::make_pair(
        argumentName, std::make_pair(std::make_pair(shortForm, additionalValue), helpMessage)));
}

std::vector<std::string> ConsoleParser::GetArgumentsRegistered() const {
    std::vector<std::string> result;
    for (const auto& arg : m_argumentsRegistered) {
        bool              shortForm   = arg.second.first.first;
        const std::string correctName = (shortForm ? "-" : "--") + arg.first;
        result.push_back(correctName);
    }
    return result;
}

bool ConsoleParser::IsArgumentPassed(const std::string& argumentName) {
    if (m_argumentsRegistered.count(argumentName) == 0) {
        throw std::runtime_error(
            ("Argument '" + argumentName + "' has not been registered!").c_str());
        return false;
    }

    const bool        shortForm   = m_argumentsRegistered.at(argumentName).first.first;
    const std::string correctName = (shortForm ? "-" : "--") + argumentName;

    m_lastSearchIterator =
        std::find(m_currentArguments.begin(), m_currentArguments.end(), correctName);
    m_lastSearchName = argumentName;

    return (m_lastSearchIterator != m_currentArguments.end());
}

const std::string&
ConsoleParser::GetArgumentAdditionalValue(const std::string& argumentName) const {
    if (m_argumentsRegistered.count(argumentName) == 0) {
        throw std::runtime_error(
            ("Argument '" + argumentName + "' has not been registered!").c_str());
    }

    const bool shortForm        = m_argumentsRegistered.at(argumentName).first.first;
    const bool bAdditionalValue = m_argumentsRegistered.at(argumentName).first.second;

    if (!bAdditionalValue) {
        throw std::runtime_error(
            ("Argument '" + argumentName + "' has not been registered with additional value!")
                .c_str());
    }

    const std::string correctName = (shortForm ? "-" : "--") + argumentName;

    if (m_lastSearchName != argumentName) {
        const auto searchResult =
            std::find(m_currentArguments.begin(), m_currentArguments.end(), correctName);
        return *(std::next(searchResult));
    }

    return *(std::next(m_lastSearchIterator));
}

bool ConsoleParser::HasArguments() const { return (m_currentArguments.size() != 0); }

void ConsoleParser::AddAdditionalHelp(const std::string& help) { m_additionalHelp += help; }

std::string ConsoleParser::GetConsoleHelp() const {
    std::stringstream help;
    help.str("");

    help << m_additionalHelp << "\n";
    help << "Arguments that the app takes:\n";
    for (const auto& arg : m_argumentsRegistered) {
        const auto& name        = arg.first;
        const auto& helpMessage = arg.second.second;
        const bool  shortForm   = arg.second.first.first;
        help << "\t" << (shortForm ? "-" : "--") << name << ",\t\t\t" << helpMessage << "\n";
    }

    return help.str();
}
