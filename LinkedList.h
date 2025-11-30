#pragma once
#include "Node.h"
#include "TreeNode.h"

/*
    The chaining element that I'm going to use for my hashmap
*/

class LinkedList {
public:
    Node* start;
    int list_size;

    LinkedList();
    ~LinkedList();

    void push(Node* a);
    TreeNode* search(int ver_id) const;
};