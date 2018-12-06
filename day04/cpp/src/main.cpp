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

typedef struct Event 
{
    int id;
    std::string yyyy,mm,dd,hh,mn,note;
} Event;

void dump(const Event& event)
{
    std::cout 
        << event.id << ", " 
        << event.yyyy << ", " 
        << event.mm << ", " 
        << event.dd << ", " 
        << event.hh << ", " 
        << event.mn << ", " 
        << event.note;
}
void dump(const std::vector<Event>& events)
{
    for ( auto e : events)
    {
        dump(e);
        std::cout << std::endl;
    }
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
    std::string yyyy,mm,dd,hh,mn,note;
 
    stream >> c             //[
           >> std::setw(4) >> item.yyyy
           >> c             //-
           >> std::setw(2) >> item.dd
           >> c             //-
           >> std::setw(2) >> item.mm
           >> std::ws
           >> std::setw(2) >> item.hh
           >> c             //:
           >> std::setw(2) >> item.mn
           >> std::setw(1) >> c             //]
           ;
           std::getline(stream, item.note);
    return item;
 }

int parse_guardid( const std::string note, int default_id)
{
    int id(default_id);
    // [1518-04-09 00:01] Guard #3407 begins shift

    auto octothorpe = note.find('#');
    std::cout << note << " octo: " 
        << octothorpe 
        << " initial id:" << id;
    if (octothorpe != std::string::npos)
    {
        std::istringstream s(note);
        s.seekg(octothorpe+1);
        s >> id;
        std::cout << " parsed id=" << id;
    }

    std::cout << " final id=" << id << std::endl;
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

    dump(events);
    std::cout << std::endl;



    return 0;
}
