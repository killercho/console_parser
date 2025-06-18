#include <gtest/gtest.h>
#include "../ConsoleParser.h"

TEST(ConsoleParserTests, RegisterArguments) {
    ConsoleParser parser;
    parser.RegisterArgument("abc", false, false, "");

    EXPECT_TRUE(parser.GetArgumentsRegistered().size() == 1) << "Argument not registered!";
    EXPECT_TRUE(parser.GetArgumentsRegistered().at(0) == "--abc")
        << "Argument not registered with correct name!";
}

TEST(ConsoleParserTests, HasArguments) {
    ConsoleParser parser;
    char**        argv = new char*[4];
    argv[0]            = new char[strlen("executable_name")];
    argv[1]            = new char[strlen("--abd")];
    argv[2]            = new char[strlen("--asd")];
    argv[3]            = new char[strlen("-dddd")];

    memcpy(argv[0], "executable_name", strlen("executable_name"));
    memcpy(argv[1], "--abd", strlen("--abd"));
    memcpy(argv[2], "--asd", strlen("--asd"));
    memcpy(argv[3], "-dddd", strlen("-dddd"));

    parser.ParseArguments(4, argv);

    EXPECT_TRUE(parser.HasArguments()) << "Parser shows no arguments when arguments were given!";
    parser.Reset();
    EXPECT_FALSE(parser.HasArguments()) << "Parser shows arguments when arguments reset!";

    for (int i = 0; i < 4; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST(ConsoleParserTests, RecognizingArguments) {
    ConsoleParser parser;
    char**        argv = new char*[4];
    argv[0]            = new char[strlen("executable_name") + 1];
    argv[1]            = new char[strlen("--abd") + 1];
    argv[2]            = new char[strlen("--asd") + 1];
    argv[3]            = new char[strlen("-dddd") + 1];

    memcpy(argv[0], "executable_name", strlen("executable_name") + 1);
    memcpy(argv[1], "--abd", strlen("--abd") + 1);
    memcpy(argv[2], "--asd", strlen("--asd") + 1);
    memcpy(argv[3], "-dddd", strlen("-dddd") + 1);

    parser.ParseArguments(4, argv);
    parser.RegisterArgument("abd", false, false, "");
    parser.RegisterArgument("dddd", true, false, "");

    EXPECT_TRUE(parser.IsArgumentPassed("abd"))
        << "Parser cannot find the argument that was asked for!";
    EXPECT_THROW(parser.IsArgumentPassed("ceg"), std::runtime_error)
        << "Parser finds the argument even though it was not defined!";
    EXPECT_TRUE(parser.IsArgumentPassed("dddd"))
        << "Parser cannot find the argument that was asked for!";

    for (int i = 0; i < 4; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST(ConsoleParserTests, UsingArguments) {
    ConsoleParser parser;
    char**        argv = new char*[5];
    argv[0]            = new char[strlen("executable_name") + 1];
    argv[1]            = new char[strlen("--abd") + 1];
    argv[2]            = new char[strlen("--asd") + 1];
    argv[3]            = new char[strlen("-dddd") + 1];
    argv[4]            = new char[strlen("additional") + 1];

    memcpy(argv[0], "executable_name", strlen("executable_name") + 1);
    memcpy(argv[1], "--abd", strlen("--abd") + 1);
    memcpy(argv[2], "--asd", strlen("--asd") + 1);
    memcpy(argv[3], "-dddd", strlen("-dddd") + 1);
    memcpy(argv[4], "additional", strlen("additional") + 1);

    parser.ParseArguments(5, argv);
    parser.RegisterArgument("abd", false, false, "");
    parser.RegisterArgument("dddd", true, true, "");

    EXPECT_TRUE(parser.IsArgumentPassed("abd"))
        << "Parser cannot find the argument that was asked for!";
    EXPECT_THROW(parser.GetArgumentAdditionalValue("abd"), std::runtime_error)
        << "Parser finds additional value when there should not be one!";

    EXPECT_THROW(parser.IsArgumentPassed("hgfsdg"), std::runtime_error)
        << "Parser finds argument that was not registered!";
    EXPECT_THROW(parser.GetArgumentAdditionalValue("hssg"), std::runtime_error)
        << "Parser finds additional value on arguments that were not defined!";

    EXPECT_TRUE(parser.GetArgumentAdditionalValue("dddd") == "additional")
        << "Argument additional value not parsed correctly!";

    for (int i = 0; i < 4; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}
