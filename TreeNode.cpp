#include "TreeNode.h"

// To give the unique version ID to every tree node. 
int TreeNode::total_nodes = 0;

// defined by id, content, parent
TreeNode::TreeNode(int vid, string con, TreeNode* p) {
    total_nodes++;
    version_id = vid;

    content = con;
    message = "";
    time(&created_timestamp);
    snapshot_timestamp = 0;
    parent = p;
}

TreeNode::TreeNode() {
    version_id = -1;
    content = "";
    message = "";
    time(&created_timestamp);
    snapshot_timestamp = 0;
    parent = nullptr;
}

TreeNode::~TreeNode() {
    for (TreeNode* child : children) {
        delete child;
    }
    children.clear();
}

string TreeNode::read() const 
{
    return content;
}


// give the information contained in this TreeNode
string TreeNode::give_info() const 
{
    string m = content;

    string created = "\nCreated on ";
    created += ctime(&created_timestamp);
    m += created;

    string snapshoted = "Snapshotted on ";
    if (snapshot_timestamp != 0)
        snapshoted += ctime(&snapshot_timestamp);
    else
        snapshoted += "N/A\n";

    m += snapshoted;
    m += ("Version id: ") + to_string(version_id);

    return m;
}