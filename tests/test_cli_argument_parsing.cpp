#include "../cli/cli.hpp"
#include <gtest/gtest.h>

/**
 * @brief BDD-style tests for CLI argument parsing
 * 
 * Following Linus-style development guide:// Test 8: Unknown flag handling
// Scenario: Unknown flag error
TES// Test 11: Version execution returns correct exit code
TEST_F(CLIExecutionBDD, VersionCommandReturnsSuccess) {
    // When I execute "mipsim --version"
    when_executing_cli({"mipsim", "--version"});
    
    // Then the exit code should be 0
    then_exit_code_should_be(cli::EXIT_OK);
}

// Test 12: Unknown command execution returns error codeumentParsingBDD, RejectsUnknownFlagWithHint) {
    // When I parse "mipsim run prog.asm --frobnicate"
    when_parsing_args({"mipsim", "run", "prog.asm", "--frobnicate"});
    
    // Then the error code should be 2
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "unknown option"
    then_error_message_should_contain("unknown option");
    // And the error message should contain "--frobnicate"
    then_error_message_should_contain("--frobnicate");
}

// Test 9: Missing required argumentsvior, not internals
 * - One behavior per test
 * - Clear error messages
 */
class CLIArgumentParsingBDD : public ::testing::Test {
protected:
    void SetUp() override {
        // Given a fresh CLI environment
    }

    // BDD-style helper methods
    void when_parsing_args(const std::vector<std::string>& args) {
        result = cli::parse_argv(args);
    }

    void then_command_should_be(cli::Command expected) {
        EXPECT_EQ(result.cmd, expected);
    }

    void then_error_code_should_be(int expected) {
        EXPECT_EQ(result.error_code, expected) 
            << "Error message: " << result.error_message;
    }

    void then_error_message_should_contain(const std::string& expected) {
        EXPECT_TRUE(result.error_message.find(expected) != std::string::npos)
            << "Expected error message to contain '" << expected 
            << "' but was: '" << result.error_message << "'";
    }

    void then_run_config_should_have(const std::string& program, long long limit = -1, long long timeout = -1) {
        ASSERT_EQ(result.cmd, cli::Command::Run);
        auto& run_cfg = std::get<cli::RunConfig>(result.config);
        EXPECT_EQ(run_cfg.program, program);
        EXPECT_EQ(run_cfg.limit, limit);
        EXPECT_EQ(run_cfg.timeout, timeout);
    }

    cli::ParseResult result;
};

// Test 1: Basic help functionality
// Scenario: Show help
TEST_F(CLIArgumentParsingBDD, ShowsHelpWhenRequestedWithLongFlag) {
    // When I parse "mipsim --help"
    when_parsing_args({"mipsim", "--help"});
    
    // Then the command should be Help
    then_command_should_be(cli::Command::Help);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
}

TEST_F(CLIArgumentParsingBDD, ShowsHelpWhenRequestedWithShortFlag) {
    // When I parse "mipsim -h"
    when_parsing_args({"mipsim", "-h"});
    
    // Then the command should be Help
    then_command_should_be(cli::Command::Help);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
}

// Test 2: Version functionality
// Scenario: Show version
TEST_F(CLIArgumentParsingBDD, ShowsVersionWhenRequestedWithLongFlag) {
    // When I parse "mipsim --version"
    when_parsing_args({"mipsim", "--version"});
    
    // Then the command should be Version
    then_command_should_be(cli::Command::Version);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
}

TEST_F(CLIArgumentParsingBDD, ShowsVersionWhenRequestedWithShortFlag) {
    // When I parse "mipsim -v"
    when_parsing_args({"mipsim", "-v"});
    
    // Then the command should be Version
    then_command_should_be(cli::Command::Version);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
}

// Test 3: Unknown command handling
// Scenario: Unknown command
TEST_F(CLIArgumentParsingBDD, RejectsUnknownCommandWithError) {
    // When I parse "mipsim frobnicate"
    when_parsing_args({"mipsim", "frobnicate"});
    
    // Then the command should be Unknown
    then_command_should_be(cli::Command::Unknown);
    // And the error code should be 2 (argument parse error)
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "unknown command"
    then_error_message_should_contain("unknown command");
}

// Test 4: Run command parsing
// Scenario: Run with program file
TEST_F(CLIArgumentParsingBDD, ParsesRunCommandWithProgramFile) {
    // When I parse "mipsim run program.asm"
    when_parsing_args({"mipsim", "run", "program.asm"});
    
    // Then the command should be Run
    then_command_should_be(cli::Command::Run);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the run config should have program="program.asm" and no limit
    then_run_config_should_have("program.asm", -1);
}

// Test 5: Run command with limit
// Scenario: Run with step limit
TEST_F(CLIArgumentParsingBDD, ParsesRunCommandWithStepLimit) {
    // When I parse "mipsim run program.asm --limit 1000"
    when_parsing_args({"mipsim", "run", "program.asm", "--limit", "1000"});
    
    // Then the command should be Run
    then_command_should_be(cli::Command::Run);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the run config should have program="program.asm" and limit=1000
    then_run_config_should_have("program.asm", 1000);
}

// Test 6: Run command with timeout
// Scenario: Run with timeout limit
TEST_F(CLIArgumentParsingBDD, ParsesRunCommandWithTimeout) {
    // When I parse "mipsim run program.asm --timeout 30"
    when_parsing_args({"mipsim", "run", "program.asm", "--timeout", "30"});
    
    // Then the command should be Run
    then_command_should_be(cli::Command::Run);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the run config should have program="program.asm" and timeout=30
    then_run_config_should_have("program.asm", -1, 30);
}

// Test 7: Run command with both limit and timeout
TEST_F(CLIArgumentParsingBDD, ParsesRunCommandWithLimitAndTimeout) {
    // When I parse "mipsim run program.asm --limit 1000 --timeout 30"
    when_parsing_args({"mipsim", "run", "program.asm", "--limit", "1000", "--timeout", "30"});
    
    // Then the command should be Run
    then_command_should_be(cli::Command::Run);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the run config should have program="program.asm", limit=1000, and timeout=30
    then_run_config_should_have("program.asm", 1000, 30);
}

// Test 8: Unknown flag handling
// Scenario: Unknown flag error
TEST_F(CLIArgumentParsingBDD, RejectsUnknownFlagWithHint) {
    // When I parse "mipsim run prog.asm --frobnicate"
    when_parsing_args({"mipsim", "run", "prog.asm", "--frobnicate"});
    
    // Then the error code should be 2
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "unknown option"
    then_error_message_should_contain("unknown option");
    // And the error message should contain "--frobnicate"
    then_error_message_should_contain("--frobnicate");
}

// Test 9: Missing required arguments
TEST_F(CLIArgumentParsingBDD, RejectsMissingProgramFile) {
    // When I parse "mipsim run"
    when_parsing_args({"mipsim", "run"});
    
    // Then the error code should be 2
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "missing program file"
    then_error_message_should_contain("missing program file");
}


/**
 * @brief BDD-style tests for CLI execution and dispatch
 */
class CLIExecutionBDD : public ::testing::Test {
protected:
    void SetUp() override {
        // Given a fresh CLI environment
    }

    // BDD-style helper methods
    void when_executing_cli(const std::vector<std::string>& args) {
        exit_code = cli::cli_main(args);
    }

    void then_exit_code_should_be(int expected) {
        EXPECT_EQ(exit_code, expected);
    }

    int exit_code = -1;
};

// Test 10: Help execution returns correct exit code
TEST_F(CLIExecutionBDD, HelpCommandReturnsSuccess) {
    // When I execute "mipsim --help"
    when_executing_cli({"mipsim", "--help"});
    
    // Then the exit code should be 0
    then_exit_code_should_be(cli::EXIT_OK);
}

// Test 11: Version execution returns correct exit code
TEST_F(CLIExecutionBDD, VersionCommandReturnsSuccess) {
    // When I execute "mipsim --version"
    when_executing_cli({"mipsim", "--version"});
    
    // Then the exit code should be 0
    then_exit_code_should_be(cli::EXIT_OK);
}

// Test 12: Unknown command execution returns error code
TEST_F(CLIExecutionBDD, UnknownCommandReturnsArgumentError) {
    // When I execute "mipsim frobnicate"
    when_executing_cli({"mipsim", "frobnicate"});
    
    // Then the exit code should be 2
    then_exit_code_should_be(cli::EXIT_ARG_PARSE);
}


/**
 * @brief BDD-style tests for help and version formatting
 */
class CLIOutputFormattingBDD : public ::testing::Test {
protected:
    void then_help_should_contain(const std::string& expected) {
        std::string help = cli::format_help();
        EXPECT_TRUE(help.find(expected) != std::string::npos)
            << "Expected help to contain '" << expected << "' but was:\n" << help;
    }

    void then_version_should_contain(const std::string& expected) {
        std::string version = cli::format_version();
        EXPECT_TRUE(version.find(expected) != std::string::npos)
            << "Expected version to contain '" << expected << "' but was:\n" << version;
    }
};

// Test 13: Help format contains required information
TEST_F(CLIOutputFormattingBDD, HelpContainsUsageInformation) {
    // Then help should contain "mipsim <command> [options]"
    then_help_should_contain("mipsim <command> [options]");
    // And help should contain "Commands"
    then_help_should_contain("Commands");
    // And help should contain "run"
    then_help_should_contain("run");
    // And help should contain "assemble"
    then_help_should_contain("assemble");
}

// Test 14: Version format contains version string
TEST_F(CLIOutputFormattingBDD, VersionContainsVersionString) {
    // Then version should contain "mipsim version"
    then_version_should_contain("mipsim version");
}
