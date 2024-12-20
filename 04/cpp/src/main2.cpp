#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>

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
    const std::vector<std::string> textRows((std::istream_iterator<std::string>(inputFile)),
                                            std::istream_iterator<std::string>());

    constexpr std::string_view searchWord1 = "MAS";
    constexpr std::string_view searchWord2 = "SAM";
    constexpr std::array wordsToCheck = {searchWord1, searchWord2};

    const auto findDiagonal = [&](int row, int col, int direction)
    {
        return std::ranges::any_of(wordsToCheck,
                                   [&](const auto &word)
                                   {
                                       for (int i = -1; i < static_cast<int>(word.size()) - 1; ++i)
                                       {
                                           if (row + i >= textRows.size() ||                       //
                                               row + i < 0 ||                                      //
                                               col + i * direction >= textRows[row + i].size() ||  //
                                               col + i * direction < 0)
                                           {
                                               return false;
                                           }
                                           if (textRows[row + i][col + i * direction] != word[i + 1])
                                           {
                                               return false;
                                           }
                                       }
                                       return true;
                                   });
    };

    int numberOfMatches = 0;
    for (int row = 1; row < textRows.size(); ++row)
    {
        for (int column = 1; column < textRows[row].size(); ++column)
        {
            numberOfMatches += findDiagonal(row, column, 1) && findDiagonal(row, column, -1);
        }
    }

    std::cout << "Number of matches: " << numberOfMatches << "\n";
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