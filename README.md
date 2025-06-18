# Console parser class written in C++ inspired by the way python does arguments

This is a console parser written in C++, that works as the python console parser.
It is optimized for speed and memory, but it is still basic and it is easy to use in any environment (hell i wrote it at my workplace for professional use :D).
Also it is made on Linux, but it compiles under Windows as well, and i'm sure that with a little changes to the cmake it can compile under Mac, since no OS specific functionalities were used in the making.


Feel free to modify and use it however you like.

## Including the parser to your project

Including the parser to any cmake managed project is easy. All you need to do is add the `console_parser` directory to your project's desired location and the
library as a subdirectory and link it to the target that needs to do the parsing.
For more information and a specific example the main `CMakeLists.txt` in the repository can be checked out. There the parser is added as a library
exactly how it is supposed to be used.

## Using the parser

Using the parser is almost as easy as including it to a cmake project. Everything starts with creating the parser object.
Then registering the arguments that the parser will need and loading the arguments as they are given from the `int main()` function.
From there the parser can be asked:
 - whether there even are any arguments that need to be looked at;
 - whether an argument was defined or not;
 - what is the additional value that was parsed with a given argument.

The documentation of each of the functions is in the header itself with small examples on what function is used where and how.
For more references and indepth guide you can checkout the `main.cpp` where the parser is used for explanatory purposes.

## Example help message defined with the parser

```

This is an additional help to the arguments!
Arguments that the app takes:
	--additional_val_arg,			Help for the argument that takes an additional value.
	-short_arg,			This is the help of the short form argument!
	--long_arg_name,			Help message for the argument.

```

## Running the parser tests that are included

The parser contains tests in the `tests/` folder that are created using the *gtest* library.
To run them first cmake them using the option **-DBUILD_WITH_CONSOLE_PARSER_TESTS=On**. The whole command is as follows:

```bash

cmake .. -DBUILD_WITH_CONSOLE_PARSER_TESTS=On

```

This will pull the google testing library and setup the tests as a target that will be built.
If you want to build only the tests, the target name is *console_parser_tests*.

Once the tests are built the binary would be located in the `bin/tests/console_parser/` folder.
You can run only that binary from there and check the output, or you can use the `ctest` command,
which will also run any other google tests that were declared.
The `ctest` command needs to be run from the `console_parser/` folder in the build directory.
