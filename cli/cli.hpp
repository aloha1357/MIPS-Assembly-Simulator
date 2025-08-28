#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace cli
{

// Exit codes as defined in the development guide
constexpr int EXIT_OK            = 0;
constexpr int EXIT_ARG_PARSE     = 2;
constexpr int EXIT_IO_ERROR      = 3;
constexpr int EXIT_RUNTIME_ERROR = 4;
constexpr int EXIT_TEST_FAILURE  = 5;

// Command types
enum class Command
{
    Help,
    Version,
    Run,
    Assemble,
    Disasm,
    Repl,
    Dump,
    Unknown
};

// Global options
struct GlobalOptions
{
    bool verbose = false;
};

// Command-specific configurations
struct RunConfig
{
    std::string program;
    long long   limit   = -1;  // -1 means no limit
    long long   timeout = -1;  // -1 means no timeout (in seconds)
    std::string trace;         // "regs", "mem", "all", or empty
};

struct AssembleConfig
{
    std::string input;
    std::string output;
    std::string map;  // symbol map file
};

struct DisasmConfig
{
    std::string input;
    uint64_t    start = 0;
    int         count = -1;  // -1 means all
};

struct ReplConfig
{
    std::string program;
};

struct DumpConfig
{
    bool        regs   = false;
    bool        mem    = false;
    std::string format = "text";  // "text" or "json"
};

// Parse result
struct ParseResult
{
    Command       cmd = Command::Unknown;
    GlobalOptions global;
    std::variant<RunConfig, AssembleConfig, DisasmConfig, ReplConfig, DumpConfig> config;
    int         error_code = EXIT_OK;
    std::string error_message;
};

// Main CLI functions
ParseResult parse_argv(const std::vector<std::string>& args);
int         dispatch(const ParseResult& result);
int         cli_main(const std::vector<std::string>& args);

// Helper functions for error handling
std::string format_help();
std::string format_version();

}  // namespace cli
