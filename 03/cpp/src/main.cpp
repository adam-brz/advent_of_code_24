#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <regex>
#include <string>

int main(int argc, char **argv)
try
{
    if (argc < 2)
    {
        std::cerr << "Input data is missing\n"
                  << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::regex mulRegex(R"(mul\((\d+),(\d+)\))");

    const auto mulResults =
        std::ranges::subrange(std::istream_iterator<std::string>(inputFile), std::istream_iterator<std::string>()) |
        std::views::transform(
            [&mulRegex](const std::string &chunk) {
                return std::ranges::subrange(std::sregex_iterator(chunk.begin(), chunk.end(), mulRegex),
                                             std::sregex_iterator());
            }) |
        std::views::join |
        std::views::transform([](const std::smatch &match)
                              { return std::stoi(match.str(1)) * std::stoi(match.str(2)); }) |
        std::ranges::to<std::vector<int>>();

    const auto sum = std::reduce(mulResults.begin(), mulResults.end(), 0);
    std::cout << "Sum of all multiplications: " << sum << "\n";

    return 0;
}
catch (const std::exception &e)
{
    std::cerr << "Exception: " << e.what() << "\n";
    return 1;
}
catch (...)
{
    std::cerr << "Unknown exception\n";
    return 1;
}