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

std::string diff_positions( const std::string& v1, const std::string& v2)
{
    std::string diffs;
    int limit =  std::min( v1.length(), v2.length());

    for (auto i = 0; i < limit; i++)
    {
        if (v1[i] != v2[i] )
        {
            diffs += v1[i];
        }
    }

    return diffs;
}

std::string common_positions( const std::string& v1, const std::string& v2)
{
    std::string commons;
    int limit =  std::min( v1.length(), v2.length());
    
    for (auto i = 0; i < limit; i++)
    {
        if (v1[i] == v2[i] )
        {
            commons += v1[i];
        }
    }

    return commons;
}

std::tuple<std::string,std::string> find_targets(const std::vector<std::string>& data)
{
    for(auto v1 : data) {
        for(auto v2 : data) {
            if (diff_positions(v1, v2).length() == 1 )
            {
                return std::make_tuple( v1, v2 );
            }
        }
    }

    return std::make_tuple( std::string(), std::string() );
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Need a file name" << std::endl;
        std::exit(1);
    }

    auto data = read_file(argv[1]);
    auto t = find_targets(data);

    std::cout << common_positions(std::get<0>(t), std::get<1>(t) )  << std::endl;

    return 0;
}
