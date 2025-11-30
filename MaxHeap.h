#pragma once
#include <vector>
#include <string>
#include <utility>
using namespace std;



/*
    The max heap stores two pieces of information:
    the name of the file
    the size of the tree of the file

    The key is the size of the tree
*/

class MaxHeap 
{
private:
    vector<pair<int,string>> heap; 
    int parent(int i);
    int left_child(int i);
    int right_child(int i);
    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    MaxHeap();

    bool empty() const;
    int size() const;

    void push(pair<int,string> val);
    pair<int,string> top() const;
    void pop();
    void clear();
};