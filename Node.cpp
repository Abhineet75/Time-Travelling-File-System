#include "Node.h"

Node::Node(TreeNode* t) {
    tn = t;
    next = nullptr;
}

Node::Node() {
    tn = nullptr;   // do NOT allocate a TreeNode here (avoid leaks)
    next = nullptr;
}

// default destructor: do not delete tn (TreeNode is owned by File/tree)
Node::~Node() {}



