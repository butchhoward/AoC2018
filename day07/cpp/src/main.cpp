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

typedef struct DataType
{
    unsigned char step, next;
} DataType;
typedef std::vector<DataType> InputDataType;

std::ostream & operator <<(std::ostream &os, const DataType& p)
{
    os << "(" << p.step << "->" << p.next << ")";
    return os;
}

std::ostream & operator <<(std::ostream &os, const InputDataType& d)
{
    for ( auto p : d)
    {
        os << p << std::endl;
    }
    return os;
}

DataType parse_data(const std::string& line)
{
    DataType p;

    std::string STEP("Step ");
    std::string NEXT("Step X must be finished before step ");

    p.step = line.substr(STEP.length(),1)[0];
    p.next = line.substr(NEXT.length(),1)[0];

    return p;

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

    std::string line;
    while (std::getline(datafile, line))
    {
        data.push_back(parse_data(line));
    }

    return data;
}

void solve_part1(const InputDataType& data)
{
    std::cout << data << std::endl;

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
        std::cout << "No data in file." << std::ends;
        std::exit(1);
    }

    solve_part1(data);
    solve_part2(data);

    return 0;
}
