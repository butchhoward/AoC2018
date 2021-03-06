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
typedef std::unique_ptr<StepTree> StepTreePtr;
class StepTree
{
public:
    StepTree() 
        : depth(0), step(0)
    {}
    StepTree(int dent, const Step& s)
        : depth(dent), step(s)
    {}
    ~StepTree()
    {
    }

    bool add(const DataType& d)
    {
        if (step == 0)
        {
            step = d.step;
            push_back( std::make_unique<StepTree>(depth+1, d.next) );
            return true;
        }
        else if ( step == d.step)
        {
            push_back( std::make_unique<StepTree>(depth+1, d.next) );
            return true;
        }
        else
        {
            for ( auto& subtree : trees )
            {
                if ( subtree->step == d.next )
                {
                    return true;
                }
                else
                {
                    return subtree->add(d);
                }
            }
            return true;
        }
    }

protected:
    int depth;
    Step step;
    std::list< StepTreePtr > trees;

    void push_back(StepTreePtr p)
    {
        p->depth = depth+1;
        trees.push_back(std::move(p));
    }

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
