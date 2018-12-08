#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>

#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>

#include <experimental/optional>
#include <experimental/set>
#include <numeric>
#include <iterator>

typedef unsigned char DataType;
typedef std::vector<DataType> InputDataType;

std::ostream & operator <<(std::ostream &os, const InputDataType& d)
{
    for ( auto c : d)
    {
        os << c;
    }
    return os;
}


InputDataType read_file(const std::string& filename)
{
    InputDataType data;
    
    std::ifstream datafile(filename);
    if(!datafile)
    {
        std::cout << "Error opening output file" << std::endl;
        return data;
    }

    std::vector<std::string> raw;
    std::string line;
    while (std::getline(datafile, line))
    {
        raw.push_back(line);
    }

    for ( auto line : raw)
    {
        for (auto c : line)
        {
            data.push_back(c);
        }
    }

    return data;
}

typedef std::map<DataType,DataType> Polar;
Polar polar = 
    {
        {'A', 'a'}, {'a', 'A'},
        {'B', 'b'}, {'b', 'B'},
        {'C', 'c'}, {'c', 'C'},
        {'D', 'd'}, {'d', 'D'},
        {'E', 'e'}, {'e', 'E'},
        {'F', 'f'}, {'f', 'F'},
        {'G', 'g'}, {'g', 'G'},
        {'H', 'h'}, {'h', 'H'},
        {'I', 'i'}, {'i', 'I'},
        {'J', 'j'}, {'j', 'J'},
        {'K', 'k'}, {'k', 'K'},
        {'L', 'l'}, {'l', 'L'},
        {'M', 'm'}, {'m', 'M'},
        {'N', 'n'}, {'n', 'N'},
        {'O', 'o'}, {'o', 'O'},
        {'P', 'p'}, {'p', 'P'},
        {'Q', 'q'}, {'q', 'Q'},
        {'R', 'r'}, {'r', 'R'},
        {'S', 's'}, {'s', 'S'},
        {'T', 't'}, {'t', 'T'},
        {'U', 'u'}, {'u', 'U'},
        {'V', 'v'}, {'v', 'V'},
        {'W', 'w'}, {'w', 'W'},
        {'X', 'x'}, {'x', 'X'},
        {'Y', 'y'}, {'y', 'Y'},
        {'Z', 'z'}, {'z', 'Z'},
    };

InputDataType collapse_units(const InputDataType& data)
{
    InputDataType uncollapsed;
    for (size_t i=0; i < data.size();)
    {
        auto a(data[i]);
        auto b(data[i+1]);

        if ( a == polar[b])
        {
            i+=2;
        }
        else
        {
            i++;
            uncollapsed.push_back(a);
        }
    }

    if (uncollapsed.size() == data.size())
    {
        return data;
    }

    return collapse_units(uncollapsed);
}

void solve_part1(const InputDataType& data)
{
    auto collapsed = collapse_units(data);
    std::cout << "data: " << data.size() << " collapsed: " << collapsed.size() << std::endl;
}

void solve_part2(const InputDataType& /* data */)
{

}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Need a file name" << std::endl;
        std::exit(1);
    }

    auto data = read_file(argv[1]);
    if (data.size()==0)
    {
        std::cout << "No events in file." << std::ends;
        std::exit(1);
    }


    solve_part1(data);
    solve_part2(data);

    return 0;
}
