#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <list>
#include <map>
#include <memory>

#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>

#include <experimental/optional>
#include <experimental/set>
#include <numeric>
#include <iterator>

typedef unsigned char Step;

typedef struct DataType
{
    Step step, next;
} DataType;

typedef std::vector<DataType> InputDataType;

std::ostream & operator <<(std::ostream &os, const DataType& p)
{
    os << "(" << p.step << "->" << p.next << ")";
    return os;
}

std::ostream & operator <<(std::ostream &os, const InputDataType& data)
{
    for ( auto d : data)
    {
        os << d << std::endl;
    }
    return os;
}

typedef class StepTree StepTree;
class StepTree
{
public:
    StepTree() 
        : step(0)
    {}
    StepTree(const Step& s)
        : step(s)
    {}
    ~StepTree()
    {
    }

    void add(const DataType& d)
    {
        if (step == 0)
        {
            step = d.step;
            trees.push_back( std::make_unique<StepTree>(d.next) );
        }
        else
        {
            //find a spot
            //add the new tree in that spot
            trees.push_back( std::make_unique<StepTree>(d) );
        }
    }

    StepTree(const DataType& d)
    {
        add(d);
    }

    Step step;
    std::list< std::unique_ptr<StepTree> > trees;

    friend std::ostream & operator <<(std::ostream &os, const StepTree& t);
};

std::ostream & operator <<(std::ostream &os, const StepTree& t)
{
    os << t.step;
    for ( auto& ts : t.trees )
    {
        os << *ts;
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

    StepTree tree;

    for (auto d : data)
    {
        tree.add(d);
    }

    std::cout << "Tree:" << std::endl << tree << std::endl;

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
