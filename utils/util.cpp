#include "util.h"

using namespace std;

vector <Point> readPoints(string filename)
{
    ifstream file(filename);
    vector <Point> points;

    if (file.is_open())
    {
        int idx = 0;
        Point p;
        while (file >> p)
        {
            // assign an incremental ID.
            p.id = ++idx;
            points.push_back(p);
        }

        file.close();
        return points;
    } else
    {
        cerr << "Can't open file " << filename << endl;
        exit(-1);
    }
}


string writeNeighbours(PIV p)
{
    string res = to_string(p.first) + ": ";
    vector<int> ids = p.second;
    stringstream neighbours;
    copy(ids.begin(), ids.end(), ostream_iterator<int>(neighbours, ","));
    string concatIds = neighbours.str();
    // remove last comma
    concatIds = concatIds.substr(0, concatIds.size() - 1);
    return res += concatIds + "\n";
}