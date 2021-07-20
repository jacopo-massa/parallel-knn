#ifndef KNN_HEAP_H
#define KNN_HEAP_H

#include <queue>
#include <iostream>

typedef std::pair<int, float> PIF;

// Custom comparator, used in priority_queue constructor
class KNNComparator
{
public:
    int operator()(const PIF &p1, const PIF &p2);
};

// redefine priority_queue for readability
typedef std::priority_queue <PIF, std::vector<PIF>, KNNComparator> PQ;

// Heap is a priority_queue as base constructor, with some redefined methods
class Heap
{
public:
    Heap(int size);

    void push(PIF p);

    std::vector<int> finalize();

    void clear();

private:
    int size;
    PQ heap;
};

#endif //KNN_HEAP_H
