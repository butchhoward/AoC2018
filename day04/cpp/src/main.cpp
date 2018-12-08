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


typedef enum {
    Sleep,
    Wake,
    Other
} EventType;

typedef struct Event 
{
    int id;
    std::string date;
    int hh,mn;
    EventType etype;
    std::string note;
} Event;

std::ostream & operator <<(std::ostream &os, const Event& event)
{
    os 
        << event.id << ", " 
        << event.date << ", " 
        << event.hh << ", " 
        << event.mn << ", " 
        << (event.etype == EventType::Sleep ? "Sleep" : (event.etype == EventType::Wake?"Wake":"Other"))
        << event.note;
    return os;
}

std::ostream & operator <<(std::ostream &os, std::vector<Event>& events)
{
    for ( auto e : events)
    {
        os << e << std::endl;
    }
    return os;
}


Event parse_input(const std::string& value)
{
// [1518-07-31 00:54] wakes up
// [1518-04-09 00:01] Guard #3407 begins shift
// [1518-04-03 00:36] wakes up
// [1518-10-24 00:03] Guard #1049 begins shift
// [1518-03-15 00:11] falls asleep

    std::istringstream stream(value);
    Event item;
    char c;

    stream >> c             //[
           >> std::setw(10) >> item.date
           >> std::ws
           >> std::setw(2) >> item.hh
           >> c             //:
           >> std::setw(2) >> item.mn
           >> std::setw(1) >> c             //]
           ;
           std::getline(stream, item.note);

           if (item.note.find("falls asleep") != std::string::npos)
           {
               item.etype = EventType::Sleep;
           }
           else if (item.note.find("wakes up") != std::string::npos)
           {
               item.etype = EventType::Wake;
           }
           else
           {
               item.etype = EventType::Other;
           }
           
    return item;
 }

int parse_guardid( const std::string note, int default_id)
{
    int id(default_id);
    // [1518-04-09 00:01] Guard #3407 begins shift

    auto octothorpe = note.find('#');
    if (octothorpe != std::string::npos)
    {
        std::istringstream s(note);
        s.seekg(octothorpe+1);
        s >> id;
    }

    return id;
}

std::vector<Event> read_file(const std::string& filename)
{
    std::vector<Event> events;
    
    std::ifstream datafile(filename);
    if(!datafile)
    {
        std::cout << "Error opening output file" << std::endl;
        return events;
    }

    std::vector<std::string> raw;
    std::string line;
    while (std::getline(datafile, line))
    {
        raw.push_back(line);
    }
    std::sort(raw.begin(), raw.end());

    int id(0);
    for ( auto line : raw)
    {
        auto event = parse_input(line);
        event.id = id = parse_guardid(event.note, id);

        events.push_back(event);
    }

    return events;
}

typedef struct Sleeping
{
    std::string date;
    int minute;
    int guard;
} Sleeping;

struct CompareSleepingPoint
{
    //less()
    bool operator()(const Sleeping& a, const Sleeping& b) const
    {
        if (a.date < b.date) return true;
        if (a.date > b.date) return false;
        if (a.minute < b.minute) return true;
        return false;
    }
};  

std::ostream & operator <<(std::ostream &os, const Sleeping& s)
{
    os << "Sleeping: "<< s.guard << " " << s.date << " " << s.minute;
    return os;
}

typedef std::set<Sleeping, CompareSleepingPoint> Schedule;

typedef struct Guard
{
    int id;
    Schedule sleeps;
} Guard;

typedef std::map<int, Guard> Guards;

std::ostream & operator <<(std::ostream &os, const Guard& g)
{
    os << "Guard: " << g.id << std::endl;
    for (auto s : g.sleeps)
    {
        os << s << std::endl;
    }
    os << "====" << std::endl;
    return os;
}

 Guards get_guard_schedule(const std::vector<Event>& events)
{
    Guards gs;
    const int MIDNITE_HOUR=0;

    bool sleeping(false);
    int fellasleep(0);

    for ( auto e : events)
    {
        if (e.hh == MIDNITE_HOUR)
        {
            Guard g;
            auto git = gs.find(e.id);
            if (git != gs.end())
            {
                sleeping = false;
                g = git->second;
            }
            g.id = e.id;

            if (e.etype == EventType::Sleep)
            {
                fellasleep = e.mn;
            }
            else if (e.etype == EventType::Wake)
            {
                for ( int f = fellasleep; f < e.mn; f++)
                {
                    Sleeping s = {e.date, f, e.id};
                    g.sleeps.insert(s);
                }
                sleeping = false;
            }
            gs[e.id] = g;
        }
    }

    return gs;
}

std::ostream & operator <<(std::ostream &os, const Guards& gs)
{
    os << "Guards:" << std::endl;
    for (auto g : gs)
    {
        os << g.second;
    }
    os << "================" << std::endl;
    return os;
}

Guard sleepingest_guard(const Guards& gs)
{
    std::map<int, int> gsum;

    for( auto g : gs)
    {
        int gid = g.first;
        int hs = g.second.sleeps.size();
        gsum[gid] = hs;
    }

    using pair_type = decltype(gsum)::value_type;
    auto idit = std::max_element(gsum.begin(), gsum.end(),
        [](const pair_type& a, const pair_type& b) {return a.second < b.second;}
        );

    auto git = gs.find(idit->first);
    return git->second;
}

//Guard, Minute pair for sleepingest and which minute
std::pair<int, int> sleepingest_guard_by_minute(const Guards& gs)
{

    typedef std::pair<int, int> Hourmax;
    std::map<int, Hourmax> ghsum;
    for (auto gp : gs)
    {
        auto g = gp.second;
        std::map<int, int> hcount;
        for( auto h=0; h<60; h++)
        {
            auto hs = std::count_if(g.sleeps.begin(), g.sleeps.end(), [h](const Sleeping& s){return s.minute == h;});
            hcount[h] = hs;
        }

        using pair_type = decltype(hcount)::value_type;
        auto hit = std::max_element(hcount.begin(), hcount.end(),
            [](const pair_type& a, const pair_type& b) {return a.second < b.second;}
            );

        ghsum[g.id] = Hourmax(hit->first, hit->second);
    }

    using pair_type = decltype(ghsum)::value_type;
    auto gsumit = std::max_element(ghsum.begin(), ghsum.end(),
        [](const pair_type& a, const pair_type& b) {return a.second.second < b.second.second;}
        );

    auto r = std::pair<int,int>(gsumit->first, gsumit->second.first);
    return r;
}


int minute_guard_sleep_most(const Guard& g)
{
    std::map<int, int> hcount;

    for( auto h=0; h<60; h++)
    {
        auto hs = std::count_if(g.sleeps.begin(), g.sleeps.end(), [h](const Sleeping& s){return s.minute == h;});
        hcount[h] = hs;
    }

    using pair_type = decltype(hcount)::value_type;
    auto mxit = std::max_element(hcount.begin(), hcount.end(),
        [](const pair_type& a, const pair_type& b) {return a.second < b.second;}
        );

    return mxit->first;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Need a file name" << std::endl;
        std::exit(1);
    }

    auto events = read_file(argv[1]);
    if (events.size()==0)
    {
        std::cout << "No events in file." << std::ends;
        std::exit(1);
    }

    // std::cout << events << std::endl;

    auto gs = get_guard_schedule(events);
    // std::cout << gs << std::endl;

    auto g = sleepingest_guard(gs);
    auto h = minute_guard_sleep_most(g);
    auto part1 = g.id * h;
    std::cout << "part1 sleeper: " << g.id << " hour=" << h << " answer=" << part1 << std::endl;

    auto g2 = sleepingest_guard_by_minute(gs);
    std::cout << "part2 sleeper: " << g2.first << " hour=" << g2.second << " answer=" << g2.first * g2.second << std::endl;
    return 0;
}
