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

typedef struct Point 
{
    int x,y;
    bool operator==(const Point& p) const
        { return p.x == this->x && p.y == this->y;}
    bool operator!=(const Point& p) const
        { return !(p == *this);}
} Point;

std::ostream & operator <<(std::ostream &os, const Point& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

typedef struct PointDistance
{
    Point p;
    int d;
} PointDistance;

std::ostream & operator <<(std::ostream &os, const PointDistance& d)
{
    os << d.p << "=" << d.d;
    return os;
}

typedef std::vector<PointDistance> PointDistances;

std::ostream & operator <<(std::ostream &os, const PointDistances& ds)
{
    for ( auto d : ds)
    {
        os << d << "; ";
    }
    return os;
}

typedef struct DataType
{
    Point p;
    PointDistances pds;
} DataType;

typedef std::vector<DataType> InputDataType;

std::ostream & operator <<(std::ostream &os, const InputDataType& data)
{
    for ( auto d : data)
    {
        os << d.p << "-->[" << d.pds << "]" << std::endl;
    }
    os << std::endl;
    return os;
}


DataType parse_data(const std::string line)
{
    Point p;

    std::istringstream stream(line);
    char c;

    stream 
           >> p.x
           >> std::setw(1) >> c             //comma
           >> p.y
           ;

    DataType d;
    d.p = p;

    return d;
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

int manhattan_distance( const Point& p1, const Point& p2)
{
    //For example, in the plane, the taxicab distance between (p1,p2)and (q1,q2) is |p1-q1|+|p2-q2|.
    return abs(p1.x-p2.x) + abs(p1.y-p2.y);
}

InputDataType point_distances(const InputDataType& a,const InputDataType& b)
{
    InputDataType ds;

    for (auto p1 : a )
    {
        DataType d;
        d.p = p1.p;

        PointDistances pds;
        for (auto p2 : b )
        {
            PointDistance pd = {p2.p, manhattan_distance(p1.p,p2.p)};
            d.pds.push_back(pd);
        }

        
        ds.push_back(d);
    }

    return ds;
}

int max_distance(const InputDataType& ds)
{
    std::vector<int> mxs;
    for (auto d : ds)
    {
        auto mxit = std::max_element(d.pds.begin(), d.pds.end(), 
            [](const PointDistance& p1, const PointDistance& p2){return p1.d < p2.d;});
        mxs.push_back(mxit->d);
    }
    return *std::max_element(mxs.begin(), mxs.end());
}


std::pair<Point, Point> city_bounds(const InputDataType& data)
{

    Point upper_left = {INT_MAX,INT_MAX}, lower_right = {INT_MIN, INT_MIN};
    for ( auto d : data )
    {
        upper_left.x = std::min(d.p.x, upper_left.x);
        upper_left.y = std::min(d.p.y, upper_left.y);
        lower_right.x = std::max(d.p.x, lower_right.x);
        lower_right.y = std::max(d.p.y, lower_right.y);
    }

    return std::make_pair(upper_left, lower_right);
}

std::pair<Point, Point> adjust_bounds(const std::pair<Point,Point>& b, int max_distance)
{
    Point upper_left(b.first), lower_right(b.second);

    upper_left.x -= max_distance;
    upper_left.y -= max_distance;
    lower_right.x += max_distance;
    lower_right.y += max_distance;

    return std::make_pair(upper_left, lower_right);
}

typedef struct Spot
{
    Point p;
    PointDistance nn;
} Spot;

typedef std::vector<Spot> Grid;

std::ostream & operator <<(std::ostream &os, const Spot& s)
{
    os << s.p << "->" << s.nn;
    return os;
}

std::ostream & operator <<(std::ostream &os, const Grid& g)
{
    //assumes they are sorted by x,y
    if (g.size())
    {
        int x = g[0].p.x;
        for (auto s : g)
        {
            if (s.p.x != x)
            {
                x = s.p.x;
                os << std::endl;
            }
            os << "[" << s << "] ";
        }
    }
    return os;
}

//build a grid with only the viable points plotted
Grid build_grid(const InputDataType& data, std::pair<Point, Point> bounds)
{
    Point upper_left(bounds.first), lower_right(bounds.second);

    InputDataType grid_points;
    for (int x = upper_left.x; x < lower_right.x; x++)
    {
        for (int y = upper_left.y; y < lower_right.y; y++)
        {
            DataType d;
            d.p = {x, y};
            grid_points.push_back( d );
        }
    }

    // auto ds = point_distances(data, grid_points);   
    // std::cout << "Point Distances" << std::endl << ds << std::endl;

    Grid grid;
    for (auto g : grid_points)
    {
        //find the nearest data point for each grid point
        PointDistances nearlist;
        PointDistance nearest = {{upper_left.x-1, upper_left.y-1}, INT_MAX};
        for (auto d : data)
        {
            auto distance = manhattan_distance(g.p, d.p);
            
            if (distance < nearest.d)
            {
                nearest = {d.p, distance};
                nearlist.clear();
                nearlist.push_back(nearest);
            }
            else if (distance == nearest.d && g.p != d.p)
            {
                //remove any grid points that are equi-distant from a data point
                nearlist.push_back({d.p, distance});
            }
        }
        if (nearlist.size() == 1)
        {
            Spot s = {g.p, nearest};
            grid.push_back(s);
        }
    }

    return grid;
}

//point+number, in this case area isstead of distance.
PointDistances get_areas(const InputDataType& data, const Grid& grid, std::pair<Point, Point> bounds)
{
    PointDistances areas;
    Point upper_left(bounds.first), lower_right(bounds.second);


    for (auto d : data)
    {
        if ( d.p.x <= upper_left.x || d.p.y <= upper_left.y ||
                d.p.x >= lower_right.x || d.p.y >= lower_right.y )
        {
            //ignore points with infinite area
            continue;
        }

        int area = std::count_if( grid.begin(), grid.end(),
            [d](const Spot& s){return s.nn.p == d.p;}
            );
        areas.push_back( {d.p, area} );
    }

    return areas;
}

void solve_part1(const InputDataType& data)
{
    auto bounds = city_bounds(data);
    // auto max_bounds = adjust_bounds(bounds, mxd);
    std::cout << "Bounds: " << bounds.first << bounds.second << std::endl;

    auto grid = build_grid(data, bounds);
    // std::cout << "Grid:" << std::endl << grid << std::endl << std::endl;
    std::cout << "Grid:" << grid.size() << std::endl;

    auto areas = get_areas(data, grid, bounds);
    // std::cout << "areas:" << areas << std::endl;

    auto answer = std::max_element(areas.begin(), areas.end(),
            [](const PointDistance& pd1, const PointDistance& pd2){return pd1.d < pd2.d;}
        );

    std::cout << "Answer: " << *answer << std::endl;

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
