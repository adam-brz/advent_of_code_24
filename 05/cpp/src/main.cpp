#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <span>
#include <string>
#include <vector>

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
    const std::vector<std::string> dataRows((std::istream_iterator<std::string>(inputFile)),
                                            std::istream_iterator<std::string>());

    const auto [orderingData, updateData] = [&]()
    {
        const auto firstUpdateRowIt =
            std::find_if(dataRows.cbegin(), dataRows.cend(),
                         [](const std::string &row) { return row.find(',') != std::string::npos; });

        const std::span orderingData(dataRows.cbegin(), firstUpdateRowIt);
        const std::span updateData(firstUpdateRowIt, dataRows.cend());
        return std::pair(orderingData, updateData);
    }();

    std::map<int, std::set<int>> directedGraph;
    for (const auto &row : orderingData)
    {
        const auto separatorPos = row.find('|');
        const int before = std::stoi(row.substr(0, separatorPos));
        const int after = std::stoi(row.substr(separatorPos + 1));
        directedGraph[before].insert(after);
    }

    const auto getInvalidUpdateItemIt = [&directedGraph](const auto &update)
    {
        for (auto it = std::next(update.cbegin()); it != update.cend(); ++it)
        {
            const auto &current = *it;
            if (std::any_of(update.cbegin(), it,
                            [&](const auto &before)
                            { return directedGraph.contains(current) && directedGraph.at(current).contains(before); }))
            {
                return it;
            }
        }
        return update.end();
    };

    const auto isUpdateValid = [&](const auto &update) { return getInvalidUpdateItemIt(update) == update.end(); };

    const auto updates =
        std::views::all(updateData) |
        std::views::transform(
            [](const auto &row)
            {
                return std::views::split(row, ',') |
                       std::views::transform([](const auto &n) { return std::stoi(std::string(n.begin(), n.end())); }) |
                       std::ranges::to<std::vector<int>>();
            }) |
        std::ranges::to<std::vector<std::vector<int>>>();

    {
        auto validMiddleNumbersFromOrdersView =
            std::views::all(updates) |           //
            std::views::filter(isUpdateValid) |  //
            std::views::transform([](const auto &order) { return order.at(order.size() / 2); });

        const auto sum =
            std::reduce(validMiddleNumbersFromOrdersView.begin(), validMiddleNumbersFromOrdersView.end(), 0);
        std::cout << "Sum of middle numbers: " << sum << "\n";
    }
    {
        int sum = 0;
        for (const auto &row : updates)
        {
            std::list<int> numbers(row.cbegin(), row.cend());

            if (isUpdateValid(numbers))
            {
                continue;
            }

            while (!isUpdateValid(numbers))
            {
                const auto invalidItemIt = getInvalidUpdateItemIt(numbers);
                const int invalidNumber = *invalidItemIt;
                numbers.insert(std::prev(invalidItemIt), invalidNumber);
                numbers.erase(invalidItemIt);
            }

            sum += *std::next(numbers.begin(), static_cast<int>(numbers.size() / 2));
        }
        std::cout << "Sum of middle numbers after corrections: " << sum << "\n";
    }

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