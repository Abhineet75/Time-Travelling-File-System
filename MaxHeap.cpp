#include "MaxHeap.h"
#include <utility>

int MaxHeap::parent(int i) 
{ 
    return (i - 1) / 2;
}

int MaxHeap::left_child(int i)
{ 
    return 2 * i + 1;
}


int MaxHeap::right_child(int i)
{ 
    return 2 * i + 2; 
}

void MaxHeap::heapifyUp(int i){

    while (i > 0 && heap[parent(i)].first < heap[i].first){

        swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}
 

// recursive method of doing it.
void MaxHeap::heapifyDown(int i){
    int largest = i;

    int l = left_child(i), r = right_child(i);

    if (l < (int)heap.size() && heap[l].first > heap[largest].first)
        largest = l;
    
    if (r < (int)heap.size() && heap[r].first > heap[largest].first)
        largest = r;

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}


MaxHeap::MaxHeap() {
    // empty
}

bool MaxHeap::empty() const
{ 
    return heap.empty(); 
}

int MaxHeap::size() const 
{ 
    return (int)heap.size(); 
}

void MaxHeap::push(pair<int,string> val) {

    heap.push_back(val);
    heapifyUp(heap.size() - 1);
}

pair<int,string> MaxHeap::top() const {
    if (heap.empty()) 
        return {0, ""};
    return heap[0];
}

void MaxHeap::pop() {
    if (heap.empty()) 
        return;
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty())
        heapifyDown(0);
}

void MaxHeap::clear()
{ 
    heap.clear(); 
}