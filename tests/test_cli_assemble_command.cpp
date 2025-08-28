#include "../cli/assemble_executor.hpp"
#include "../cli/cli.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

/**
 * @brief BDD-style test suite for CLI assemble command execution
 *
 * Following Linus Torvalds development philosophy:
 * - Test behavior, not implementation details
 * - One feature per test
 * - Clear Given-When-Then structure
 */
class CLIAssembleCommandBDD : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create temporary test directory
        temp_dir = std::filesystem::temp_directory_path() / "mipsim_assemble_test";
        std::filesystem::create_directories(temp_dir);
    }

    void TearDown() override
    {
        // Clean up temporary files
        if (std::filesystem::exists(temp_dir))
        {
            std::filesystem::remove_all(temp_dir);
        }
    }

    // Helper to create a test assembly file
    void given_assembly_file(const std::string& filename, const std::string& content)
    {
        auto          filepath = temp_dir / filename;
        std::ofstream file(filepath);
        file << content;
        file.close();
        m_test_file = filepath.string();
    }

    // Helper to execute assemble command
    void when_executing_assemble_command(const cli::AssembleConfig& config)
    {
        m_result = cli::execute_assemble_command(config);
    }

    // Helper to execute assemble command with basic config
    void when_executing_assemble_command(const std::string& input, const std::string& output = "")
    {
        cli::AssembleConfig config;
        config.input  = input;
        config.output = output.empty() ? (input + ".bin") : output;
        m_result      = cli::execute_assemble_command(config);
    }

    void then_exit_code_should_be(int expected)
    {
        EXPECT_EQ(m_result, expected);
    }

    void then_output_file_should_exist(const std::string& filename)
    {
        EXPECT_TRUE(std::filesystem::exists(filename))
            << "Expected output file " << filename << " to exist";
    }

    void then_output_file_should_not_be_empty(const std::string& filename)
    {
        EXPECT_GT(std::filesystem::file_size(filename), 0)
            << "Expected output file " << filename << " to not be empty";
    }

  protected:
    std::filesystem::path temp_dir;
    std::string           m_test_file;
    int                   m_result = 0;
};

// ========================================
// BDD Scenarios
// ========================================

// Scenario: Successfully assemble a simple program
TEST_F(CLIAssembleCommandBDD, AssemblesSimpleProgramSuccessfully)
{
    // Given a simple assembly file
    given_assembly_file("simple.asm", "addi $t0, $zero, 42\n"
                                      "li $v0, 1\n"
                                      "move $a0, $t0\n"
                                      "syscall\n"
                                      "li $v0, 10\n"
                                      "syscall\n");

    // When I execute the assemble command
    std::string output_file = (temp_dir / "simple.bin").string();
    when_executing_assemble_command(m_test_file, output_file);

    // Then the exit code should be success
    then_exit_code_should_be(cli::EXIT_OK);
    // And the output file should exist
    then_output_file_should_exist(output_file);
    // And the output file should not be empty
    then_output_file_should_not_be_empty(output_file);
}

// Scenario: Handle missing input file
TEST_F(CLIAssembleCommandBDD, ReturnsIOErrorForMissingInputFile)
{
    // When I try to assemble a non-existent file
    when_executing_assemble_command("nonexistent.asm");

    // Then the exit code should be I/O error
    then_exit_code_should_be(cli::EXIT_IO_ERROR);
}

// Scenario: Handle invalid assembly syntax
TEST_F(CLIAssembleCommandBDD, ReturnsRuntimeErrorForInvalidAssembly)
{
    // Given an invalid assembly file
    given_assembly_file("invalid.asm", "this is not valid assembly code!");

    // When I execute the assemble command
    std::string output_file = (temp_dir / "invalid.bin").string();
    when_executing_assemble_command(m_test_file, output_file);

    // Then the exit code should be runtime error
    then_exit_code_should_be(cli::EXIT_RUNTIME_ERROR);
}

// Scenario: Handle empty input file
TEST_F(CLIAssembleCommandBDD, ReturnsRuntimeErrorForEmptyFile)
{
    // Given an empty assembly file
    given_assembly_file("empty.asm", "");

    // When I execute the assemble command
    std::string output_file = (temp_dir / "empty.bin").string();
    when_executing_assemble_command(m_test_file, output_file);

    // Then the exit code should be runtime error
    then_exit_code_should_be(cli::EXIT_RUNTIME_ERROR);
}

// Scenario: Generate symbol map file when requested
TEST_F(CLIAssembleCommandBDD, GeneratesSymbolMapWhenRequested)
{
    // Given an assembly file with labels
    given_assembly_file("with_labels.asm", "main:\n"
                                           "    addi $t0, $zero, 42\n"
                                           "    beq $t0, $zero, end\n"
                                           "    nop\n"
                                           "end:\n"
                                           "    li $v0, 10\n"
                                           "    syscall\n");

    // When I execute the assemble command with symbol map
    cli::AssembleConfig config;
    config.input  = m_test_file;
    config.output = (temp_dir / "with_labels.bin").string();
    config.map    = (temp_dir / "with_labels.map").string();
    when_executing_assemble_command(config);

    // Then the exit code should be success
    then_exit_code_should_be(cli::EXIT_OK);
    // And the output file should exist
    then_output_file_should_exist(config.output);
    // And the symbol map file should exist
    then_output_file_should_exist(config.map);
    // And the symbol map file should not be empty
    then_output_file_should_not_be_empty(config.map);
}

// Scenario: Default output filename when not specified
TEST_F(CLIAssembleCommandBDD, UsesDefaultOutputFilename)
{
    // Given a simple assembly file
    given_assembly_file("test.asm", "li $v0, 10\n"
                                    "syscall\n");

    // When I execute the assemble command without specifying output
    when_executing_assemble_command(m_test_file);

    // Then the exit code should be success
    then_exit_code_should_be(cli::EXIT_OK);
    // And the default output file should exist (input.bin)
    std::string default_output = m_test_file + ".bin";
    then_output_file_should_exist(default_output);
}

/**
 * @brief BDD-style tests for CLI assemble argument parsing
 */
class CLIAssembleArgumentParsingBDD : public ::testing::Test
{
  protected:
    void when_parsing_args(const std::vector<std::string>& args)
    {
        result = cli::parse_argv(args);
    }

    void then_command_should_be(cli::Command expected)
    {
        EXPECT_EQ(result.cmd, expected);
    }

    void then_error_code_should_be(int expected)
    {
        EXPECT_EQ(result.error_code, expected) << "Error message: " << result.error_message;
    }

    void then_error_message_should_contain(const std::string& expected)
    {
        EXPECT_TRUE(result.error_message.find(expected) != std::string::npos)
            << "Expected error message to contain '" << expected << "' but was: '"
            << result.error_message << "'";
    }

    void then_assemble_config_should_have(const std::string& input, const std::string& output = "",
                                          const std::string& map = "")
    {
        ASSERT_EQ(result.cmd, cli::Command::Assemble);
        auto& cfg = std::get<cli::AssembleConfig>(result.config);
        EXPECT_EQ(cfg.input, input);
        if (!output.empty())
        {
            EXPECT_EQ(cfg.output, output);
        }
        if (!map.empty())
        {
            EXPECT_EQ(cfg.map, map);
        }
    }

    cli::ParseResult result;
};

// Test: Basic assemble command parsing
TEST_F(CLIAssembleArgumentParsingBDD, ParsesAssembleCommandWithInputFile)
{
    // When I parse "mipsim assemble program.asm"
    when_parsing_args({"mipsim", "assemble", "program.asm"});

    // Then the command should be Assemble
    then_command_should_be(cli::Command::Assemble);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the assemble config should have input="program.asm"
    then_assemble_config_should_have("program.asm");
}

// Test: Assemble command with output file
TEST_F(CLIAssembleArgumentParsingBDD, ParsesAssembleCommandWithOutputFile)
{
    // When I parse "mipsim assemble program.asm -o output.bin"
    when_parsing_args({"mipsim", "assemble", "program.asm", "-o", "output.bin"});

    // Then the command should be Assemble
    then_command_should_be(cli::Command::Assemble);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the assemble config should have input="program.asm" and output="output.bin"
    then_assemble_config_should_have("program.asm", "output.bin");
}

// Test: Assemble command with symbol map
TEST_F(CLIAssembleArgumentParsingBDD, ParsesAssembleCommandWithSymbolMap)
{
    // When I parse "mipsim assemble program.asm --map symbols.map"
    when_parsing_args({"mipsim", "assemble", "program.asm", "--map", "symbols.map"});

    // Then the command should be Assemble
    then_command_should_be(cli::Command::Assemble);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the assemble config should have input="program.asm" and map="symbols.map"
    then_assemble_config_should_have("program.asm", "", "symbols.map");
}

// Test: Assemble command with all options
TEST_F(CLIAssembleArgumentParsingBDD, ParsesAssembleCommandWithAllOptions)
{
    // When I parse "mipsim assemble program.asm -o output.bin --map symbols.map"
    when_parsing_args(
        {"mipsim", "assemble", "program.asm", "-o", "output.bin", "--map", "symbols.map"});

    // Then the command should be Assemble
    then_command_should_be(cli::Command::Assemble);
    // And the error code should be 0
    then_error_code_should_be(cli::EXIT_OK);
    // And the assemble config should have all options set
    then_assemble_config_should_have("program.asm", "output.bin", "symbols.map");
}

// Test: Missing input file
TEST_F(CLIAssembleArgumentParsingBDD, RejectsMissingInputFile)
{
    // When I parse "mipsim assemble"
    when_parsing_args({"mipsim", "assemble"});

    // Then the error code should be 2
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "missing input file"
    then_error_message_should_contain("missing input file");
}

// Test: Missing value for output option
TEST_F(CLIAssembleArgumentParsingBDD, RejectsMissingOutputValue)
{
    // When I parse "mipsim assemble program.asm -o"
    when_parsing_args({"mipsim", "assemble", "program.asm", "-o"});

    // Then the error code should be 2
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "missing value for -o"
    then_error_message_should_contain("missing value for -o");
}

// Test: Unknown option
TEST_F(CLIAssembleArgumentParsingBDD, RejectsUnknownOption)
{
    // When I parse "mipsim assemble program.asm --unknown"
    when_parsing_args({"mipsim", "assemble", "program.asm", "--unknown"});

    // Then the error code should be 2
    then_error_code_should_be(cli::EXIT_ARG_PARSE);
    // And the error message should contain "unknown option"
    then_error_message_should_contain("unknown option");
}
