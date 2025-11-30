#include "header_files.h"
#include "MaxHeap.h"
#include "TreeNode.h"
#include "Node.h"
#include "LinkedList.h"


using namespace std;


class File {
public:

    string FILENAME;

    TreeNode* root;
    TreeNode* active_version;
    
    /*
        My hashmap is a vector of LinkedLists.
        For every tree, I hash its version id
        and store the address to that in its respective bucket.
    */

    vector<LinkedList> hashmap;

    /*
        Number of buckets has been chosen as 101 (prime)
    */

    static const int BUCKETS = 101;

    int total_versions;

    File(const string& filename) {

        FILENAME = filename;
        root = nullptr;
        active_version = nullptr;
        hashmap.clear();
        create();
        //total_versions = 1;
    }

    ~File() {
        // Delete the version tree (root owns the whole subtree)
        if (root) delete root;
        root = nullptr;

        // LinkedList destructors (in hashmap) will free Nodes automatically
        // because hashmap is a member and will be destroyed automatically.
    }

    int hash(int a) const {
        if (a < 0) a = -a;
        return a % BUCKETS;
    }

    TreeNode* lookup(int vid) const {
        int h = hash(vid);
        if (h < 0 || h >= (int)hashmap.size()) return nullptr;

        // searching in that particular linkedList
        return hashmap[h].search(vid);
    }

    void create() {
        // The first one is snapshotted.
        root = new TreeNode(0, "", nullptr); // the parent of root is nullptr
        
        root->message = "This is the default text";
        time(&root->snapshot_timestamp);
        active_version = root;
        total_versions = 1;


        // push the address of this newly created file into the hashmap
        hashmap.resize(BUCKETS);
        
        int h = hash(0);
        
        Node* f = new Node(root);
        hashmap[h].push(f);
    }

    string read() const {
        return active_version?active_version->content : string();
    }

    void insert(const string& content_add) {
        if (!active_version) 
            return;

        if (active_version->message.empty()) {
            // just add
            active_version->content += content_add;
        } else {
            // create a new node (child)
            string previous_data = active_version->content;
            TreeNode* latest = new TreeNode(total_versions, previous_data + content_add, active_version);
            ++total_versions;

            // hash the new node as well
            Node* nl = new Node(latest);
            hashmap[hash(latest->version_id)].push(nl);


            // set the active version to the latest
            active_version->children.push_back(latest);
            active_version = latest;
        }
    }

    void update(const string& content_new) {  // similar to insert
        if (!active_version)
            return;
        if (active_version->message == "") {
            active_version->content = content_new;
        } else {
            ++total_versions;
            TreeNode* latest = new TreeNode(total_versions - 1, content_new, active_version);

            Node* nl = new Node(latest);
            hashmap[hash(latest->version_id)].push(nl);

            active_version->children.push_back(latest);
            active_version = latest;
        }
    }

    void snapshot(const string& msg) {
        if (!active_version)
            return;
        if (!active_version->message.empty())
            return;
        active_version->message = msg;
        time(&(active_version->snapshot_timestamp));
    }

    void rollback(int vid = 0) {
        TreeNode* s = hashmap[hash(vid)].search(vid);
        
        if (s){
            active_version =  s;
        }else{
            cout << "This version id was not found" << endl;
        }
    }

    void history() const {
        TreeNode* trav = active_version;
        while (trav != nullptr) {
            cout << "---------------" << endl;
            cout << trav->give_info() << endl;
            trav = trav->parent;
        }
    }
};


class SystemStat {
public:


    vector<string> recent_log;

    // max-heap: (size, filename). Largest size comes first.
    MaxHeap biggest_heap;

    // store latest known size of each file -> to help in lazy deletion
    map<string, int> tree_size;

    int file_count;
    unordered_set<string> filenames;
    map<string, File*> file_ptrs; // SystemStat owns these File*

    SystemStat(){ 
        file_count = 0;
    }

    ~SystemStat() {
        for (auto &p : file_ptrs) {
            if (p.second) delete p.second;
        }
        file_ptrs.clear();
        filenames.clear();
        tree_size.clear();
        recent_log.clear();
        while (!biggest_heap.empty()) biggest_heap.pop();
    }

    void addFile(File* f) {
        if (!f) return;
        const string& name = f->FILENAME;
        
        if (filenames.find(name) != filenames.end())
            return;

        file_ptrs[name] = f;
        filenames.insert(name);

        tree_size[name] = f->total_versions;
        biggest_heap.push({tree_size[name], name});

        time_t now; time(&now);
        recent_log.push_back(name);

        ++file_count;
    }

    void updateFile(const string& filename) {
        auto it = file_ptrs.find(filename);
        if (it == file_ptrs.end()) {
            cout << "updateFile: file not found: " << filename << endl;
            return;
        }

        File* current_file = it->second;
        tree_size[filename] = current_file->total_versions;

        // lazy push: don’t remove old ones
        biggest_heap.push({tree_size[filename], filename});

        time_t now; time(&now);
        recent_log.push_back(filename);
    }

    // get the single biggest file (unique)

    /*
        The idea is to keep cleaning the top of the heap
        until you find a node that is indeed the most recently
        updated biggest tree.
    */

    string biggestTree() {

        while (!biggest_heap.empty()){

            auto [sz, fname] = biggest_heap.top();
            
            if (tree_size[fname] == sz)
                return fname;
            
            biggest_heap.pop();
        }

        return "No files in storage";
    }

    // get last num biggest unique files
    vector<string> biggestUnique(int num) {
        
        vector<string> result;
        
        unordered_set<string> seen;

        // temp vector to hold popped entries (to push back later)
        vector<pair<int, string>> buffer;

        while (!biggest_heap.empty() && (int)result.size() < num) {
            auto [sz, fname] = biggest_heap.top();
            biggest_heap.pop();

            if (tree_size[fname] != sz) {
                // outdated
                continue;
            }
            // insert.second returns true if that element was inserted
            // if that already exists, then it's not inserted.
            if (seen.insert(fname).second){
                result.push_back(fname);
            }
            buffer.push_back({sz, fname});
        }

        // restore the popped valid entries
        for (auto &p : buffer) {
            biggest_heap.push(p);
        }

        return result;
    }

    // returns last 'num' unique filenames (most recent first)
    
    /*
        Continuing the same idea of keeping a log of the unique
        filenames already in the last num entries.
    */

    vector<string> mostRecentUnique(int num) {
        
        vector<string> res;
        unordered_set<string> seen;
        
        for(int i = (int)recent_log.size() - 1; i >= 0 && (int)res.size() < num; --i) 
        {
            
            const string &fn = recent_log[i];
            
            if (seen.insert(fn).second)
                res.push_back(fn);
        }
        return res;
    }
};



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SystemStat stat;

    while (true) {
        //cout << "--> ";
        string inp;
        if (!getline(cin, inp)) break;
        if (inp.empty()) continue;

        stringstream tokens(inp);
        string com;
        tokens >> com;

        if (com == "CREATE") {
            string filename;
            // check for atleast one.
            if (!(tokens >> filename)) { 
                cout << "CREATE needs a filename\n"; 
                continue; 
            }

            File* nf = new File(filename);
            stat.addFile(nf);

        } else if (com == "READ") {
            string filename;
            if (!(tokens >> filename)) 
            { 
                cout << "READ needs filename\n";
                continue; 
            }
            auto it = stat.file_ptrs.find(filename);
            
            if (it == stat.file_ptrs.end())
            { 
                cout << "File not found\n";
                continue;
            }
            
            File* cur = it->second;

            cout << cur->read() << "\n";
            stat.updateFile(filename);

        } else if (com == "INSERT") {
            string filename;
            if (!(tokens >> filename))
            { 
                cout << "INSERT needs filename\n";
                continue;
            }
            auto it = stat.file_ptrs.find(filename);
            
            if (it == stat.file_ptrs.end())
            { 
                cout << "File not found\n"; 
                continue;
            }
            File* cur = it->second;
            string rest;
            
            getline(tokens, rest);
            
            if (!rest.empty() && rest.front() == ' ') rest.erase(0, 1);
            
            cur->insert(rest);
            stat.updateFile(filename);

        } else if (com == "UPDATE") {
            string filename;
            if (!(tokens >> filename))
            { 
                cout << "UPDATE needs filename\n";
                continue; 
            }
            auto it = stat.file_ptrs.find(filename);
            if (it == stat.file_ptrs.end())
            { 
                cout << "File not found\n";
                continue;
            }
            
            File* cur = it->second;
            string rest;

            getline(tokens, rest);
            
            if (!rest.empty() && rest.front() == ' ')
                rest.erase(0, 1);
            
            cur->update(rest);
            stat.updateFile(filename);

        } else if (com == "ROLLBACK") {
            string filename; 
            int ver_id;
            if (!(tokens >> filename)) 
            { 
                cout << "ROLLBACK needs filename\n";
                continue;
            }
            
            string numstr;
            if (!(tokens >> numstr))
                ver_id = -1; // it'll go to parent 
            else
                ver_id = stoi(numstr);

            auto it = stat.file_ptrs.find(filename);
            if (it == stat.file_ptrs.end()) 
            { 
                cout << "File not found\n";
                continue; 
            }
            
            it->second->rollback(ver_id);
            stat.updateFile(filename);

        } else if (com == "HISTORY") {
            string filename;
            if (!(tokens >> filename)) 
            { 
                cout << "HISTORY needs filename\n"; 
                continue; 
            }
            auto it = stat.file_ptrs.find(filename);
            if (it == stat.file_ptrs.end())
            { 
                cout << "File not found\n";
                continue; 
            }
            
            it->second->history();
            stat.updateFile(filename);

        } else if (com == "SNAPSHOT") {
            string filename;
            if (!(tokens >> filename)) 
            { cout << "SNAPSHOT needs filename\n"; continue; }
            auto it = stat.file_ptrs.find(filename);
            if (it == stat.file_ptrs.end()) { cout << "File not found\n"; continue; }
            string msg;
            getline(tokens, msg);
            
            if (msg.empty())
            {
                cout << "Please enter the message for this snapshot" << endl;
                continue;
            }
            if (!msg.empty() && msg.front() == ' ') msg.erase(0, 1);
            it->second->snapshot(msg);
            stat.updateFile(filename);
        }
        else if (com == "VERSION"){
            string filename;
            if (!(tokens >> filename)) { cout << "VERSION needs filename\n"; continue; }

            string numstr;
            tokens >> numstr;
            int vid = stoi(numstr);

            auto it = stat.file_ptrs.find(filename);
            if (it == stat.file_ptrs.end()) {
                cout << "File not found\n";
                continue;
            }
            File* curfile = it->second;  // extract File* from iterator
            TreeNode* that_version = curfile->lookup(vid);

            if (that_version == nullptr)
            {
                cout << "This version id was not found" << endl;
            }else
            {
                cout << that_version->give_info() << endl;
            }


            // now I need to check if this version exists.

        } 
        else if (com == "BT") {
            cout << stat.biggestTree() << "\n";

        }else if (com == "BIGGEST_TREES") {
            
            string numstr;
            if(!(tokens >> numstr))
            {
                cout << "Error. Enter the number of files you want to see" << endl;
                continue;
            }
            int k = stoi(numstr);
            auto vec = stat.biggestUnique(k);
            if (vec.empty()){
                cout << "No files in storage\n";
            }else{
                for (const auto &s : vec) cout << s << "\n";
            }
        }
        
         else if (com == "T_VER") {
            string filename;
            if (!(tokens >> filename)) 
            { 
                cout << "T_VER needs filename\n";
                continue; 
            }
            auto it = stat.file_ptrs.find(filename);
            if (it == stat.file_ptrs.end())
            { 
                cout << "File not found\n"; 
                continue; 
            }
            cout << it->second->total_versions << "\n";

        } else if (com == "C_VER") {
            string filename;
            if (!(tokens >> filename)) 
            { 
                cout << "C_VER needs filename\n";
                continue;
            }
            auto it = stat.file_ptrs.find(filename);
           
            if (it == stat.file_ptrs.end())
            { 
                cout << "File not found\n"; 
                continue; 
            }
            cout << it->second->active_version->version_id << "\n";

        } else if (com == "FILENAMES") {
            if (stat.filenames.empty()) cout << "No files yet\n";
            else{
                for (const string &n : stat.filenames) cout << n << " ";
                cout << "\n";
            }

        } else if (com == "RECENT_FILES") {
            int k;
            if (!(tokens >> k)) 
            { 
                cout << "recent_unique needs a number\n";
                continue;
            }
            auto vec = stat.mostRecentUnique(k);
            for (const auto &s : vec) cout << s << "\n";

        } else if (com == "END") {
            break;

        } else {
            cout << "Invalid input. Try again.\n";
        }
    }

    return 0;
}


/*

CREATE fileA
INSERT fileA "Initial content"
SNAPSHOT fileA "First version"
READ fileA
INSERT fileA " More content added"
SNAPSHOT fileA "Second version"
ROLLBACK fileA 0
INSERT fileA " Branching content"
SNAPSHOT fileA "Branch from root"
CREATE fileB
INSERT fileB "Some text"
SNAPSHOT fileB "File B init"
BIGGEST_TREES 1
RECENT_FILES 2

1

*/