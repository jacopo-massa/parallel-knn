#include <atomic>
#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include "utils/util.h"
#include "utils/heap.h"
#include "utils/utimer.cpp"
#include "utils/queue.cpp"

using namespace std;

PIV END(-1, NULL);
atomic<int> notAssignedIdx;

void knnTask(vector <Point> &points, int k, vector<myqueue<PIV> *> results, int id, int size, int start, int stop)
{
    myqueue<PIV> *outqueue = results[id];
    Heap mh(k);
    for (int i = start; i < stop; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j) continue;
            // calculate distances with all other points in the dataset
            float ed = points[i].squaredEuclideanDistance(points[j]);
            // adding them to the heap
            mh.push(make_pair(j, ed));
        }

        // send point result to worker's queue
        outqueue->push(move(make_pair(i, mh.finalize())));
        // clean for next task
        mh.clear();
    }

    int nap = notAssignedIdx++;
    // check if there's a left-out point to compute
    if (nap < size)
        knnTask(points, k, results, id, size, nap, nap + 1);

    // send "End-Of-Stream" at the end of all computations.
    outqueue->push(END);

}

void writerTask(vector<myqueue<PIV> *> results, int nw)
{
    string filename = "stl_res.txt";
    string toWrite = "";
    ofstream out(filename);
    int rr = 0;
    if (!out.is_open())
    {
        cerr << "Can't open file " << filename << endl;
        exit(-1);
    }
    while (nw)
    {
        PIV point = results[rr]->pop();
        // end for this queue
        if (point == END)
        {
            results.erase(results.begin() + rr);
            nw--;
            if (rr == nw)
                rr = 0;
        } else
            // append point result to final string
            toWrite.append(writeNeighbours(point));

        // round robin logic
        rr = (rr == nw) ? 0 : (rr + 1) % nw;
    }

    out << toWrite << endl;
    out.close();

}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <K> <W> <input_file>" << endl;
        cerr << "K \t hyperparameter for KNN" << endl;
        cerr << "W \t number of workers" << endl;
        cerr << "input_file, \t name of a file in the subdirectory 'input'" << endl;
        exit(-1);
    }

    int k = atoi(argv[1]);
    int nworkers = atoi(argv[2]);
    string filename = "input/" + string(argv[3]);

    vector <Point> points;
    vector <thread> threads;
    vector < myqueue<PIV> * > results(nworkers);

    {
        utimer t1("READ");
        points = readPoints(filename);
    }

    // define chunk_size
    int size = points.size();
    int perThread = size / nworkers;
    int start = 0;
    int stop;

    // save #left-out points, as index of the first one.
    notAssignedIdx = size - (size % nworkers);
    {
        utimer t2("KNN");
        for (int i = 0; i < nworkers; i++)
        {
            results[i] = new myqueue<PIV>();
            stop = start + perThread;
            // setup workers with #tasks = chunk_size
            threads.push_back(move(thread([&points, k, &results, i, size, start, stop]() {
                knnTask(points, k, results, i, size, start, stop);
            })));
            // update for next chunk
            start += perThread;
        }

        // collector thread
        threads.push_back(move(thread([&results, nworkers]() {
            writerTask(results, nworkers);
        })));

        for (auto &t: threads) { t.join(); }
    }
    return 0;
}
