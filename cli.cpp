#include "cli/cli.hpp"
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    return cli::cli_main(args);
}