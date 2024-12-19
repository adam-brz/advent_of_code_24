#include <algorithm>
#include <exception>
#include <fstream>
#include <functional>
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
    std::regex doRegex(R"(do\(\))");
    std::regex doNotRegex(R"(don't\(\))");

    const std::string fileData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    const auto doPositions =
        std::ranges::subrange(std::sregex_iterator(fileData.begin(), fileData.end(), doRegex), std::sregex_iterator()) |
        std::views::transform([](const std::smatch &match) { return match.position(); }) |
        std::ranges::to<std::vector<size_t>>();

    const auto doNotPositions =
        std::ranges::subrange(std::sregex_iterator(fileData.begin(), fileData.end(), doNotRegex),
                              std::sregex_iterator()) |
        std::views::transform([](const std::smatch &match) { return match.position(); }) |
        std::ranges::to<std::vector<size_t>>();

    const auto mulResults =
        std::ranges::subrange(std::sregex_iterator(fileData.begin(), fileData.end(), mulRegex),
                              std::sregex_iterator()) |
        std::views::filter(
            [&](const std::smatch &match)
            {
                const auto pos = match.position();
                const auto lastDoBlockIt =
                    std::lower_bound(doPositions.crbegin(), doPositions.crend(), pos, std::greater{});
                const auto lastDoBlockPos = (lastDoBlockIt == doPositions.crend()) ? 0 : *lastDoBlockIt;
                const auto lastDoNotBlockIt =
                    std::lower_bound(doNotPositions.crbegin(), doNotPositions.crend(), pos, std::greater{});
                const auto lastDoNotBlockPos = (lastDoNotBlockIt == doNotPositions.crend()) ? 0 : *lastDoNotBlockIt;
                return lastDoBlockPos >= lastDoNotBlockPos;
            }) |
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