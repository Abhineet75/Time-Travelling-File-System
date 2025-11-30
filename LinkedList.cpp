#include "LinkedList.h"

LinkedList::LinkedList() {
    start = nullptr;
    list_size = 0;
}


LinkedList::~LinkedList() {

    Node* cur = start;
    while (cur)
    {
        Node* nxt = cur->next;
        delete cur;        // delete Node wrapper only; tn is not owned here
        cur = nxt;
    }
    start = nullptr;
    list_size = 0;
}

// pushing this node at the end of the chain.
void LinkedList::push(Node* a) {

    a->next = start;
    start = a;
    list_size++;
}


// searching for that node with a treenode which has that particular version id
TreeNode* LinkedList::search(int ver_id) const {
    
    Node* cur = start;
    
    while (cur != nullptr){
        if (cur->tn && cur->tn->version_id == ver_id)
            return cur->tn;
        cur = cur->next;
    }
    return nullptr;
}