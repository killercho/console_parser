#include <iostream>
#include <console_parser/ConsoleParser.h>

int main(const int argc, char** argv) {
    // Example usage for the console parser
    // First create the parser object
    // It has a default constructor only, so no need to pass any arguments
    ConsoleParser parser;

    // Now that the object is created the arguments need to be registered
    // It is done with the RegisterArgument function.
    // This line registeres an argument with the name "long_arg_name" that is expected in its long
    // form and it is expected for it to have an additional value right after it. The last argument
    // is the help message of the argument.
    parser.RegisterArgument("long_arg_name", false, false, "Help message for the argument.");
    // From now on when the parser sees "--long_arg_name" in the argv it will connect it with this
    // variable that was defined before.

    // Lets say that this application will have two more arguments - one in a short form
    // and another one with an additional value.
    // The short form argument would be defined with:
    parser.RegisterArgument("short_arg", true, false,
                            "This is the help of the short form argument!");

    // The argument with additional value would be defined with:
    parser.RegisterArgument("additional_val_arg", false, true,
                            "Help for the argument that takes an additional value.");

    // When registering the argument a help message can be passed. This help message will be printed
    // when the help is needed with the GetConsoleHelp() function. If the argument help is not
    // enough to describe the purposes, additional help can be added with the AddAdditionalHelp()
    // function.
    parser.AddAdditionalHelp("This is an additional help to the arguments!");

    // At this point the parser can recognize the arguments:
    //  - "--long_arg_name" with no additional value;
    //  - "-short_arg" with not additional value;
    //  - "--additional_val_arg" with an additional value.

    // If at any point the same parser needs to be used but it needs to be registered with different
    // arguments or different args need to be parser, it  can be reset with the Reset() function.

    // Now that the parser knows what to search for, the application arguments can be passed.
    // This is done with the ParseArguments function.
    parser.ParseArguments(argc, argv);
    // Normally the first argument in argv is the name of the application. This argument
    // is ignored since it is not considered important for the purpose of the parser.

    // From here on the main idea of the parser is to make it easy to check what options were
    // passed and get any additional values that they might have.
    // First there needs to be a way to know if there were any arguments that were passed at all.
    // This is done with the HasArguments() function
    if (parser.HasArguments()) {
        // The parser has arguments that were passed to it!
        // Now it's time to check their values. This is done with the IsArgumentPassed() function.
        if (parser.IsArgumentPassed("short_arg")) {
            // The parser automatically converts the argument name to what it expects
            // which for this specific argument would be "-short_arg".
        } else {
            // The parser did not see the "-short_arg" argument being passed.
        }

        // If for some reason you want to check if the parser sees an argument that was not
        // registered then the IsArgumentPassed() function will throw an error.

        // Some of the arguments have values, to get it you can use GetArgumentAdditionalValue().

    } else {
        // The parser does not have any arguments... Go through other logic branch.
    }

    // Get all the help messages built in a single string:
    std::cout << parser.GetConsoleHelp();
    // The output from the command is in the README.md under the section
    // "Example help message defined in the parser"
}
