#include <algorithm>
#include <cmath>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct Report
{
    std::vector<int> records;
};

std::istream &operator>>(std::istream &is, Report &report)
{
    std::string reportEntry;
    std::getline(is, reportEntry);
    std::istringstream reportEntryStream(reportEntry);

    report.records.clear();
    std::copy(std::istream_iterator<int>(reportEntryStream), std::istream_iterator<int>(),
              std::back_inserter(report.records));

    return is;
}

bool isReportSafe(const Report &report)
{
    const auto &levels = report.records;

    auto adjacentLevelsDifferenceView =
        std::views::zip(levels | std::views::take(levels.size() - 1), levels | std::views::drop(1)) |
        std::views::transform(
            [](const auto &levels)
            {
                const auto &[level1, level2] = levels;
                return level2 - level1;
            });

    const auto areAdjacentLevelsOk = [&]()
    {
        return std::ranges::all_of(adjacentLevelsDifferenceView,
                                   [](const auto &difference)
                                   {
                                       const auto absDifference = std::abs(difference);
                                       return absDifference >= 1 && absDifference <= 3;
                                   });
    };

    const auto areLevelsMonotonic = [&]()
    {
        return std::ranges::all_of(adjacentLevelsDifferenceView,
                                   std::bind(std::greater<int>(), std::placeholders::_1, 0)) ||
               std::ranges::all_of(adjacentLevelsDifferenceView, std::bind(std::less<int>(), std::placeholders::_1, 0));
    };

    return areAdjacentLevelsOk() && areLevelsMonotonic();
}

bool isReportSafeWithProblemDampener(const Report &report)
{
    const auto &levels = report.records;

    if (levels.size() < 3)
    {
        return true;
    }

    for (auto itToSkip = levels.begin(); itToSkip != levels.end(); ++itToSkip)
    {
        const auto getIt = [&](const auto it) { return it == itToSkip ? std::next(it) : it; };

        auto it1 = getIt(levels.begin());
        auto it2 = getIt(std::next(it1));
        const auto signbit = std::signbit(*it2 - *it1);

        const auto checkSafe = [&](const auto &it1, const auto &it2)
        {
            const auto diff = *it2 - *it1;
            const auto absDiff = std::abs(diff);
            return absDiff <= 3 && absDiff >= 1 && (std::signbit(diff) == signbit);
        };

        bool isSafe = true;
        while (it1 != levels.end() && it2 != levels.end())
        {
            if (!checkSafe(it1, it2))
            {
                isSafe = false;
                break;
            }
            it1 = it2;
            it2 = getIt(std::next(it1));
        }

        if (isSafe)
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[])
try
{
    if (argc < 2)
    {
        std::cerr << "Input data is missing\n"
                  << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    const std::vector<Report> reports((std::istream_iterator<Report>(inputFile)), std::istream_iterator<Report>());

    const auto numberOfSafeRecords = std::ranges::count_if(reports, isReportSafe);
    std::cout << "There is " << numberOfSafeRecords << " safe reports\n";

    const auto numberOfSafeRecordsWithProblemDampener = std::ranges::count_if(reports, isReportSafeWithProblemDampener);
    std::cout << "There is " << numberOfSafeRecordsWithProblemDampener << " safe reports with problem dampener\n";

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
