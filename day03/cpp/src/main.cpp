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


typedef struct Rect 
{
    int x;
    int y;
    int h;
    int w;
} Rect;

typedef struct Claim 
{
    int id;
    Rect rect;
} Claim;

void dump(const Rect& rect)
{
    std::cout 
    << " x=" << rect.x
    << " y=" << rect.y
    << " h=" << rect.h
    << " w=" << rect.w
    ;
}

void dump(const Claim& claim)
{
    std::cout << "id=" << claim.id;
    dump(claim.rect);
}

void dump(const std::vector<Claim>& claims)
{
    std::cout << "Claims:" << std::endl;
    for ( auto claim : claims)
    {
        dump(claim);
        std::cout << std::endl;
    }
}

Claim parse_input(const std::string& value)
{
    //0123456789012
    //#1 @ 1,3: 4x4
    std::istringstream stream(value);
    Claim
 item;
    char c;
 
    stream >> c             //#
           >> std::ws 
           >> item.id
           >> std::ws
           >> c             //@
           >> std::ws
           >> item.rect.x
           >> c             //comma
           >> item.rect.y
           >> c             //colon
           >> std::ws
           >> item.rect.w
           >> c             //comma
           >> item.rect.h
           ;
    return item;
 }

std::vector<Claim> read_file(const std::string& filename)
{
    std::vector<Claim> claims;
    
    std::ifstream datafile(filename);
    if(!datafile)
    {
        std::cout << "Error opening output file" << std::endl;
        return claims;
    }

    std::string line;
    while (std::getline(datafile, line))
    {
        auto claim = parse_input(line);
        if ( claim.rect.x < 0 ||
             claim.rect.y < 0 ||
             claim.rect.h <=0 ||
             claim.rect.w <= 0
             )
        {
            std::cout << "invalid claim";
            dump(claim);
            std::cout << std::endl;
            continue;
        }
        claims.push_back(claim);
    }

    return claims;
}

std::experimental::optional<Rect> get_overlap(const Rect& rect1, const Rect& rect2)
{
    int leftx = std::max( rect1.x, rect2.x);
    int topy = std::max(rect1.y, rect2.y);
    int rightx = std::min( rect1.x + rect1.w, rect2.x + rect2.w);
    int bottomy = std::min(rect1.y + rect1.h, rect2.y + rect2.h);

    // std::cout << " rect1: ";
    // dump(rect1);

    // std::cout << " rect2: ";
    // dump(rect2);

    // std::cout << " checks: " << leftx << " " << topy << " " << rightx << " " << bottomy;

    if (leftx < rightx && topy < bottomy )
    {
        Rect r = {leftx, topy, rightx-leftx, bottomy-topy};
        // std::cout << " r: ";
        // dump(r);
        return r;
    }

    return {};
}


std::vector<Rect> get_overlaps(const std::vector<Rect>& rects)
{
    std::vector<Rect> overlaps;

    for (size_t i=0; i< rects.size(); i++)
    {
        for (size_t j=i+1; j<rects.size(); j++)
        {
            // std::cout << "checking " << i << " " << j;
            auto overlap = get_overlap(rects[i], rects[j]);
            if (overlap)
            {
                // std::cout << " overlap: " << overlap->h * overlap->w;
                overlaps.push_back(*overlap);
            }
            // std::cout << std::endl;
        }
    }

    return overlaps;
}

std::vector<Rect> get_claim_overlaps(const std::vector<Claim>& items)
{
    std::vector<Rect> rects;

    for (auto item : items)
    {
        rects.push_back(item.rect);
    }

    return get_overlaps(rects);
}

Rect bounds(const std::vector<Claim>& claims)
{
    return 
    std::accumulate(
        std::next(claims.begin()), 
        claims.end(),
        claims[0].rect,
        [](const Rect& a, const Claim& c){
            auto leftx = std::min( a.x, c.rect.x);
            auto topy = std::min(a.y, c.rect.y);
            auto rightx = std::max( a.x + a.w, c.rect.x + c.rect.w);
            auto bottomy = std::max(a.y + a.h, c.rect.y + c.rect.h);
            Rect b = {leftx, topy, rightx-leftx, bottomy-topy};
            return b;
        }
    );
}

int clever(const std::vector<Claim>& claims)
{
    auto total_claim_area = std::accumulate(claims.begin(), claims.end(),
                                    0,
                                    [](int a, const Claim& item) 
                                        {return a + (item.rect.h * item.rect.w);}
                                    );
    std::cout << "total claims area=" << total_claim_area << std::endl;

    auto claim_overlaps = get_claim_overlaps(claims);
    std::cout << "overlap count=" << claim_overlaps.size() << std::endl;

    auto accumulate_area = [](int a, const Rect& r) 
        {
            return a + (r.h * r.w);
        };


    auto total_overlap_area = std::accumulate(claim_overlaps.begin(), claim_overlaps.end(),
                                    0,
                                    accumulate_area
                                    );
    std::cout << "overlap area=" << total_overlap_area << std::endl;

    auto redundant_overlaps = get_overlaps(claim_overlaps);
    std::cout << "redundant_overlaps count=" << redundant_overlaps.size() << std::endl;


    auto net_overlap_area = total_overlap_area;
    if (redundant_overlaps.size() > 0 )
    {
        auto redundant_overlap_area = std::accumulate(redundant_overlaps.begin(), redundant_overlaps.end(),
                                        0,
                                        accumulate_area
                                        );
        std::cout << "redundant area=" << redundant_overlap_area << std::endl;
        net_overlap_area -= redundant_overlap_area;
    }

    std::cout << "net overlap area=" << net_overlap_area << std::endl;

    return net_overlap_area;
}

typedef struct Point 
{
    int x;
    int y;
} Point;

struct ComparePoint
{
    //less()
    bool operator()(const Point& a, const Point& b) const
    {
        if (a.x < b.x) return true;
        if (a.x > b.x) return false;
        if (a.y < b.y) return true;
        return false;
    }
};  


int brute_force(const std::vector<Claim>& claims)
{
    std::set<Point, ComparePoint> claimed;
    std::set<Point, ComparePoint> lapland;
    for (auto c : claims)
    {
        for (int x = c.rect.x; x < c.rect.x + c.rect.w; x++)
        {
            for (int y = c.rect.y; y < c.rect.y + c.rect.h; y++)
            {
                Point p = {x,y};
                if ( !claimed.insert(p).second )
                {
                    lapland.insert(p);
                }
            }
        }
    }
    return lapland.size();
}


std::set<int> brute_force_only_once(const std::vector<Claim>& claims)
{
    std::set<int> perfect;

    std::set<Point, ComparePoint> claimed;
    std::set<Point, ComparePoint> lapland;
    for (auto c : claims)
    {
        for (int x = c.rect.x; x < c.rect.x + c.rect.w; x++)
        {
            for (int y = c.rect.y; y < c.rect.y + c.rect.h; y++)
            {
                Point p = {x,y};
                if ( !claimed.insert(p).second )
                {
                    lapland.insert(p);
                }
            }
        }
    }

    for (auto c : claims)
    {
        bool lapped=false;
        for (int x = c.rect.x; x < c.rect.x + c.rect.w; x++)
        {
            for (int y = c.rect.y; y < c.rect.y + c.rect.h; y++)
            {
                Point p = {x,y};
                if ( lapland.count(p) != 0 )
                {
                    lapped = true;
                    break;
                }
            }
            if (lapped) {
                break;
            }
        }
        if (!lapped)
        {
            perfect.insert(c.id);
        }
    }

    return perfect;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Need a file name" << std::endl;
        std::exit(1);
    }

    auto claims = read_file(argv[1]);
    if (claims.size()==0)
    {
        std::cout << "No claims in file." << std::ends;
        std::exit(1);
    }
    // dump(claims);
    // std::cout << std::endl;

    //just for information
    std::cout << "bounds:";
    dump(bounds(claims));
    std::cout << std::endl;

    std::cout << "brute overclaimed area:" << brute_force(claims) << std::endl;

    auto perfect = brute_force_only_once(claims);
    std::cout << "brute not overclaimed:" << std::endl;
    for (auto c : perfect)
    {
        std::cout << c << std::endl;
    }
    std::cout << "===" << std::endl;

    // auto clever_overclaimed = clever(claims);
    // std::cout << "clever overclaimed area:" << clever_overclaimed << std::endl;

    return 0;
}
