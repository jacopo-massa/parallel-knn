#include <iostream>
#include <string>
#include "utils/util.h"
#include "utils/heap.h"
#include "utils/utimer.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <K> <input_file>" << endl;
        cerr << "K \t hyperparameter for KNN" << endl;
        cerr << "input_file, \t name of a file in the subdirectory 'input'" << endl;
        exit(-1);
    }

    int k = atoi(argv[1]);
    string filename = "input/" + string(argv[2]);

    vector <Point> points;
    string toWrite = "";

    {
        utimer t1("READ");
        points = readPoints(filename);
    }

    int size = points.size();

    {
        utimer t2("KNN");

        Heap mh(k);

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (i == j)
                    continue;
                float ed = points[i].squaredEuclideanDistance(points[j]);
                mh.push(make_pair(j, ed));
            }

            toWrite.append(writeNeighbours(make_pair(i, mh.finalize())));

            // clean for next step
            mh.clear();

            cout << "\r" << "Done " << i + 1 << "/" << size << flush;
        }
        cout << endl;
    }

    {
        utimer t3("WRITE");
        string filename = "seq_res.txt";
        ofstream out(filename);
        if (!out.is_open())
        {
            cerr << "Can't open file " << filename << endl;
            exit(-1);
        }

        out << toWrite << endl;
        out.close();
    }
    return 0;
}
