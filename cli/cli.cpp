#include "cli.hpp"
#include "assemble_executor.hpp"
#include "run_executor.hpp"
#include <iostream>
#include <sstream>

namespace cli
{

ParseResult parse_argv(const std::vector<std::string>& args)
{
    ParseResult result;

    if (args.empty())
    {
        result.error_code    = EXIT_ARG_PARSE;
        result.error_message = "no command provided";
        return result;
    }

    // Skip program name if present
    size_t start_idx = (args[0].find("mipsim") != std::string::npos) ? 1 : 0;

    if (start_idx >= args.size())
    {
        result.error_code    = EXIT_ARG_PARSE;
        result.error_message = "no command provided";
        return result;
    }

    const std::string& cmd = args[start_idx];

    // Handle global flags first
    if (cmd == "--help" || cmd == "-h")
    {
        result.cmd = Command::Help;
        return result;
    }

    if (cmd == "--version" || cmd == "-v")
    {
        result.cmd = Command::Version;
        return result;
    }

    // Handle subcommands
    if (cmd == "run")
    {
        result.cmd = Command::Run;
        RunConfig run_cfg;

        // Need at least the program file
        if (start_idx + 1 >= args.size())
        {
            result.error_code    = EXIT_ARG_PARSE;
            result.error_message = "missing program file";
            return result;
        }

        run_cfg.program = args[start_idx + 1];

        // Parse additional flags
        for (size_t i = start_idx + 2; i < args.size(); i++)
        {
            const std::string& arg = args[i];

            if (arg == "--limit")
            {
                if (i + 1 >= args.size())
                {
                    result.error_code    = EXIT_ARG_PARSE;
                    result.error_message = "missing value for --limit";
                    return result;
                }
                try
                {
                    run_cfg.limit = std::stoll(args[i + 1]);
                    i++;  // skip the value
                }
                catch (const std::exception&)
                {
                    result.error_code    = EXIT_ARG_PARSE;
                    result.error_message = "invalid value for --limit";
                    return result;
                }
            }
            else if (arg == "--timeout")
            {
                if (i + 1 >= args.size())
                {
                    result.error_code    = EXIT_ARG_PARSE;
                    result.error_message = "missing value for --timeout";
                    return result;
                }
                try
                {
                    run_cfg.timeout = std::stoll(args[i + 1]);
                    i++;  // skip the value
                }
                catch (const std::exception&)
                {
                    result.error_code    = EXIT_ARG_PARSE;
                    result.error_message = "invalid value for --timeout";
                    return result;
                }
            }
            else if (arg.substr(0, 2) == "--")
            {
                result.error_code    = EXIT_ARG_PARSE;
                result.error_message = "unknown option " + arg + " (see 'mipsim run --help')";
                return result;
            }
            else
            {
                result.error_code    = EXIT_ARG_PARSE;
                result.error_message = "unexpected argument: " + arg;
                return result;
            }
        }

        result.config = run_cfg;
        return result;
    }

    if (cmd == "assemble")
    {
        result.cmd = Command::Assemble;
        AssembleConfig assemble_cfg;

        // Need at least the input file
        if (start_idx + 1 >= args.size())
        {
            result.error_code    = EXIT_ARG_PARSE;
            result.error_message = "missing input file";
            return result;
        }

        assemble_cfg.input = args[start_idx + 1];

        // Parse additional flags
        for (size_t i = start_idx + 2; i < args.size(); i++)
        {
            const std::string& arg = args[i];

            if (arg == "-o" || arg == "--output")
            {
                if (i + 1 >= args.size())
                {
                    result.error_code    = EXIT_ARG_PARSE;
                    result.error_message = "missing value for -o";
                    return result;
                }
                assemble_cfg.output = args[i + 1];
                i++;  // skip the value
            }
            else if (arg == "--map")
            {
                if (i + 1 >= args.size())
                {
                    result.error_code    = EXIT_ARG_PARSE;
                    result.error_message = "missing value for --map";
                    return result;
                }
                assemble_cfg.map = args[i + 1];
                i++;  // skip the value
            }
            else if (arg.substr(0, 2) == "--")
            {
                result.error_code    = EXIT_ARG_PARSE;
                result.error_message = "unknown option " + arg + " (see 'mipsim assemble --help')";
                return result;
            }
            else
            {
                result.error_code    = EXIT_ARG_PARSE;
                result.error_message = "unexpected argument: " + arg;
                return result;
            }
        }

        result.config = assemble_cfg;
        return result;
    }

    // Unknown command
    result.cmd           = Command::Unknown;
    result.error_code    = EXIT_ARG_PARSE;
    result.error_message = "unknown command '" + cmd + "' (see 'mipsim --help')";
    return result;
}

int dispatch(const ParseResult& result)
{
    if (result.error_code != EXIT_OK)
    {
        std::cerr << "mipsim: " << result.error_message << std::endl;
        return result.error_code;
    }

    switch (result.cmd)
    {
    case Command::Help:
        std::cout << format_help() << std::endl;
        return EXIT_OK;

    case Command::Version:
        std::cout << format_version() << std::endl;
        return EXIT_OK;

    case Command::Run:
    {
        auto& run_cfg = std::get<RunConfig>(result.config);
        return execute_run_command(run_cfg);
    }

    case Command::Assemble:
    {
        auto& assemble_cfg = std::get<AssembleConfig>(result.config);
        return execute_assemble_command(assemble_cfg);
    }

    default:
        std::cerr << "mipsim: internal error - unhandled command" << std::endl;
        return EXIT_RUNTIME_ERROR;
    }
}

int cli_main(const std::vector<std::string>& args)
{
    ParseResult result = parse_argv(args);
    return dispatch(result);
}

std::string format_help()
{
    std::ostringstream oss;
    oss << "mipsim <command> [options]\n"
        << "\n"
        << "Commands:\n"
        << "  run         Execute a program (.asm or .bin)\n"
        << "  assemble    Assemble .asm → .bin\n"
        << "  disasm      Disassemble .bin → text\n"
        << "  repl        Interactive shell (step/regs/mem/break)\n"
        << "  dump        Print state (regs/pc/mem), scriptable output\n"
        << "  help        Per-command help\n"
        << "  version     Print version\n"
        << "\n"
        << "Global Options:\n"
        << "  -h, --help     Show this help message\n"
        << "  -v, --version  Show version information\n"
        << "      --verbose  Enable verbose output\n"
        << "\n"
        << "Examples:\n"
        << "  mipsim run prog.asm --limit 1000 --trace regs\n"
        << "  mipsim run prog.asm --timeout 30\n"
        << "  mipsim assemble src.asm -o out.bin --map symbols.map\n"
        << "  mipsim disasm out.bin --start 0x00400000 --count 10\n"
        << "\n"
        << "Run Command Options:\n"
        << "  --limit N      Stop execution after N cycles\n"
        << "  --timeout N    Stop execution after N seconds\n"
        << "  --trace TYPE   Enable tracing (regs|mem|all)\n";
    return oss.str();
}

std::string format_version()
{
    return "mipsim version 1.0.0 (MIPS Assembly Simulator)";
}

}  // namespace cli
