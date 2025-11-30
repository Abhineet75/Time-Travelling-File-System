#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

using namespace std;

class TreeNode {
public:
    static int total_nodes;   // declaration of static variable

    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;

    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(int vid, string con, TreeNode* p);
    TreeNode();
    ~TreeNode();

    string read() const;
    string give_info() const;
};