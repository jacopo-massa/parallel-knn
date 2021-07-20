#include "heap.h"

using namespace std;

int KNNComparator::operator()(const PIF &p1, const PIF &p2)
{
    return p1.second < p2.second;
}

Heap::Heap(int size) : size(size) {}

void Heap::push(PIF p)
{
    if (heap.size() < size)
        heap.push(move(p));
    else
    {
        // to have a fixed-size heap, push only if a point better than "top" is found, popping "top" in case.
        if (p.second < heap.top().second)
        {
            heap.pop();
            heap.push(move(p));
        }
    }
}

// method that retrieves heap elements in reverse order
vector<int> Heap::finalize()
{
    vector<int> result(size);
    while (heap.size())
    {
        result[heap.size() - 1] = heap.top().first;
        heap.pop();
    }
    return result;
}

// clear internal priority_queue by re-defining it
void Heap::clear()
{
    heap = PQ();
}
