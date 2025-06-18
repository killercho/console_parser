#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

// Responsible for parsing the input
// from the main app and traslating it
// to something easier to work with.
class ConsoleParser {
 private:
    // Holds the arguments that were last parsed.
    // Keeps the relative positioning of the arguments so that
    // arguments that expect some other value as entry can be used.
    std::vector<std::string> m_currentArguments;

    // Holds the registered arguments with whether
    // they are in short form and have additional value.
    // The last string holds the help message for each of the arguments.
    std::unordered_map<std::string, std::pair<std::pair<bool, bool>, std::string>>
        m_argumentsRegistered;

    // Holds the latest search that was made through the IsArgumentPassed() function
    // used for optimization of the search in the arguments.
    std::string                        m_lastSearchName;
    std::vector<std::string>::iterator m_lastSearchIterator;

    // Additional help message that will be placed on top of the help
    // messages for the arguments.
    std::string m_additionalHelp;

 public:
    // clang-format off
    // The clang format does not like the "=" formatting 
    // this is why it is turned off for this section ¯\_(ツ)_/¯
    ConsoleParser()                                = default;
    ConsoleParser(const ConsoleParser&)            = default;
    ConsoleParser(ConsoleParser&&)                 = delete;
    ConsoleParser& operator=(const ConsoleParser&) = default;
    ConsoleParser& operator=(ConsoleParser&&)      = delete;
    ~ConsoleParser()                               = default;
    // clang-format on

    // Parses the arguments given into the memory of the class.
    // Automatically removes the first argument
    // that corresponds to the name of the application that was started.
    void ParseArguments(const int argc, const char* const* argv);

    // Registers an argument with the argument name so that information about it
    // can later be extracted from the arguments being passed.
    // If the short form is used then the argument is expected to have only one '-' instead of '--'
    // If there is an additional value then the argument is expected to get a value that will be
    // returned as a string.
    //
    // Example: ./a.out --some_argument 50
    // will be registered with the line RegisterArgument("some_argument", false, true);
    //
    // Example: ./a.out -c
    // will be registered with the line RegisterArgument("c", true, false);
    void RegisterArgument(const std::string& argumentName, bool shortForm,
                          bool additionalValue = false, const std::string& helpMessage = "");

    // Clears the arguments that were parsed and the arguments that were registered.
    void Reset();

    // Returns all of the arguments that were registered
    // in either the form that they were passed.
    std::vector<std::string> GetArgumentsRegistered() const;

    // Returns true if the argumentName is in the arguments that were parsed.
    // Throws an error if the argument searched was not registered.
    // Automatically converted to what the parser expects.
    bool IsArgumentPassed(const std::string& argumentName);

    // Returns the additional value of the argument that is searched.
    // If the argument was not registered or it does not have additionalValue
    // it will throw an error.
    // Throws an error also if the argument that has that
    // additionalValue was not even passed to the app.
    // Recommended usage includes checking with IsArgumentPassed()
    // first before trying to get the value.
    const std::string& GetArgumentAdditionalValue(const std::string& argumentName) const;

    // Returns true if there are any arguments that are passed,
    // false otherwise.
    bool HasArguments() const;

    // Adds the help given to the back of the
    // current help message.
    void AddAdditionalHelp(const std::string& help);

    // Collects all of the help messages of the variables
    // and the additional helps if there were any
    // and creates a single help
    // message that will be returned.
    std::string GetConsoleHelp() const;
};
