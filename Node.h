#pragma once
#include "TreeNode.h"

/*
    Node is just a object I'm using to chain 
*/

class Node {
public:
    TreeNode* tn;
    Node* next;

    Node(TreeNode* t);
    Node();
    ~Node(); 
};