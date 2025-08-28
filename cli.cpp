#include "cli.hpp"
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    return cli::cli_main(args);
}