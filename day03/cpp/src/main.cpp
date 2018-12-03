#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

std::vector<std::string> read_file(const std::string& filename)
{
    std::vector<std::string> DataArray;
    
    std::ifstream datafile(filename);
    if(!datafile)
    {
        std::cout << "Error opening output file" << std::endl;
        return DataArray;
    }

    std::string line;
    while (std::getline(datafile, line))
    {
        DataArray.push_back(line);
    }

    return DataArray;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Need a file name" << std::endl;
        std::exit(1);
    }

    auto data = read_file(argv[1]);

    for (auto v : data)
    {
        std::cout << v << std::endl;
    }

    return 0;
}
