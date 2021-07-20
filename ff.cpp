#include <iostream>
#include <string>
#include "utils/util.h"
#include "utils/heap.h"
#include "utils/utimer.cpp"

#include <ff/parallel_for.hpp>

using namespace std;
using namespace ff;

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <K> <W> <input_file>" << endl;
        cerr << "K \t hyperparameter for KNN" << endl;
        cerr << "W \t number of workers" << endl;
        cerr << "input_file \t name of a file in the subdirectory 'input'" << endl;
        exit(-1);
    }

    int k = atoi(argv[1]);
    int nworkers = atoi(argv[2]);
    string filename = "input/" + string(argv[3]);

    vector <Point> points;

    {
        utimer t1("READ");
        points = readPoints(filename);
    }

    int size = points.size();
    ParallelForReduce<string> pfr(nworkers);
    string toWrite = "";

    {
        utimer t2("KNN");

        pfr.parallel_reduce(toWrite, "", 0, size, 1, 0,
        [&](const long pointIdx, string &pointRes)
        {
            Heap mh(k);

            for (int i = 0; i < size; i++)
            {
                if (i == pointIdx) continue;
                // calculate distances with all other points in the dataset
                float ed = points[pointIdx].squaredEuclideanDistance(points[i]);
                // adding them to the heap
                mh.push(make_pair(i, ed));
            }

            PIV p = make_pair(pointIdx, mh.finalize());
            pointRes.append(writeNeighbours(p));
        },
        [](string &outString, const string pointRes)
        {
           outString.append(pointRes);
        });

        string filename = "ff_res.txt";
        ofstream out(filename);
        if (!out.is_open())
        {
            cerr << "Can't open file " << filename << endl;
            exit(-1);
        }

        out << toWrite;
        out.close();
    }

    return 0;
}
