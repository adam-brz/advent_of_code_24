#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

struct IdPair : public std::pair<int, int>
{
    using std::pair<int, int>::pair;
};

std::istream &operator>>(std::istream &is, IdPair &idPair)
{
    return is >> idPair.first >> idPair.second;
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
    std::vector<int> firstList, secondList;

    for (const auto &[idFromFirst, idFromSecond] :
         std::ranges::subrange(std::istream_iterator<IdPair>(inputFile), std::istream_iterator<IdPair>()))
    {
        firstList.push_back(idFromFirst);
        secondList.push_back(idFromSecond);
    }

    std::ranges::sort(firstList);
    std::ranges::sort(secondList);

    auto distancesView = std::views::zip(firstList, secondList) |
                         std::views::transform([](const auto &pair) { return std::abs(pair.first - pair.second); });

    const auto distance = std::reduce(distancesView.begin(), distancesView.end(), 0);
    std::cout << "Total distance is: " << distance << "\n";

    auto similarityView = std::views::transform(
        firstList, [&secondList](const int id) { return id * static_cast<int>(std::ranges::count(secondList, id)); });

    const auto similarity = std::reduce(similarityView.begin(), similarityView.end(), 0);
    std::cout << "Similarity value is: " << similarity << "\n";

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