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
    int x,y;
} DataType;
typedef std::vector<DataType> InputDataType;

std::ostream & operator <<(std::ostream &os, const DataType& p)
{
    os << "(" << p.x << ", " << p.y << ")";
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

DataType parse_data(const std::string line)
{
    DataType p;

    std::istringstream stream(line);
    char c;

    stream 
           >> p.x
           >> std::setw(1) >> c             //comma
           >> p.y
           ;

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
    // std::cout << data << std::endl;

    return data;
}

int manhattan_distance( const DataType& p1, const DataType& p2)
{
    //For example, in the plane, the taxicab distance between (p1,p2)and (q1,q2) is |p1-q1|+|p2-q2|.
    return abs(p1.x-p2.x) + abs(p1.y-p2.y);
}


typedef struct PointDistanceFrom
{
    DataType p;
    int d;
} PointDistanceFrom;

std::ostream & operator <<(std::ostream &os, const PointDistanceFrom& d)
{
    os << d.p << "=>" << d.d;
    return os;
}

typedef struct PointDistancesFrom
{
    DataType from;
    std::vector< PointDistanceFrom > ps;
} PointDistancesFrom;

std::ostream & operator <<(std::ostream &os, const PointDistancesFrom& ds)
{
    os << ds.from << "-->";
    for ( auto d : ds.ps)
    {
        os << d << "; ";
    }
    os << std::endl;
    return os;
}


typedef std::vector<PointDistancesFrom> PointDistances;

std::ostream & operator <<(std::ostream &os, const PointDistances& pds)
{
    for ( auto pd : pds)
    {
        os << pd << std::endl;
    }
    return os;
}

PointDistances point_distances(const InputDataType& a,const InputDataType& b)
{
    PointDistances pds;

    for (auto p1 : a )
    {
        PointDistancesFrom pd1;
        pd1.from = p1;

        for (auto p2 : b )
        {
            PointDistanceFrom pd = {p2, manhattan_distance(p1,p2)};
            pd1.ps.push_back(pd);
        }

        pds.push_back(pd1);
    }

    return pds;
}

void solve_part1(const InputDataType& data)
{
    auto pds = point_distances(data,data);
    std::cout << pds;
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
