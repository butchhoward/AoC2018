#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <iostream>
#include <iomanip>
#include <sstream>

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

void parse_input(const std::string& value)
{
    //#1 @ 1,3: 4x4
    std::istringstream stream(value);
    int n;
    double f;
    bool b;
 
    stream >> n >> f >> std::boolalpha >> b;
    std::cout << "n = " << n << '\n'
              << "f = " << f << '\n'
              << "b = " << std::boolalpha << b << '\n';
 
    // extract the rest using the streambuf overload
    stream >> std::cout.rdbuf();
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
