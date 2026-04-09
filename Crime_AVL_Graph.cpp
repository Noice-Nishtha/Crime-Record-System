#include <iostream>
#include <string>

using namespace std;

// ==========================================
// UTILITY FUNCTIONS (No STL allowed)
// ==========================================
int getMax(int a, int b) {
    return (a > b) ? a : b;
}

// ==========================================
// UNIT 1, 2 & 4: AVL TREE FOR INDEXING 
// ==========================================

// Structure for AVL Node
struct AVLNode {
    int CaseID;             // Key for indexing
    string SuspectName;
    string Location;
    string Evidence;
    
    int height;             // Height for balancing
    AVLNode* left;
    AVLNode* right;

    AVLNode(int id, string name, string loc, string evidence) {
        CaseID = id; SuspectName = name; Location = loc; Evidence = evidence;
        height = 1;         // New node is initially added at leaf
        left = right = nullptr;
    }
};

// ==========================================
// UNIT 3: GRAPH FOR NETWORK ANALYSIS
// ==========================================

// Structure for Adjacency List Node
struct AdjNode {
    int dest;
    AdjNode* next;
    AdjNode(int d) : dest(d), next(nullptr) {}
};

class CrimeGraph {
private:
    string nodes[200];      // Array to store distinct node names
    AdjNode* adjList[200];  // Array of pointers for adjacency list
    int numNodes;

    // Helper: Map a string to an integer ID for graph indexing
    // Time Complexity: O(V) where V is current number of vertices
    int getIndex(const string& name) {
        for(int i = 0; i < numNodes; i++) {
            if(nodes[i] == name) return i;
        }
        if(numNodes < 200) {
            nodes[numNodes] = name;
            return numNodes++;
        }
        return -1; // Graph full safeguard
    }

public:
    CrimeGraph() { 
        numNodes = 0; 
        for(int i=0; i<200; i++) adjList[i] = nullptr;
    }

    // Time Complexity: O(V + E)
    void clearGraph() {
        for(int i = 0; i < numNodes; i++) {
            AdjNode* curr = adjList[i];
            while(curr) {
                AdjNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
            adjList[i] = nullptr;
        }
        numNodes = 0;
    }

    // Add an undirected edge between two entities
    // Time Complexity: O(V) due to getIndex search
    void addEdge(const string& uName, const string& vName) {
        int u = getIndex(uName);
        int v = getIndex(vName);
        if(u == -1 || v == -1) return;

        // u -> v
        AdjNode* edge1 = new AdjNode(v);
        edge1->next = adjList[u];
        adjList[u] = edge1;

        // v -> u
        AdjNode* edge2 = new AdjNode(u);
        edge2->next = adjList[v];
        adjList[v] = edge2;
    }

    // BFS Search (Find shortest connection path)
    // Time Complexity: O(V + E)
    void BFS(const string& startName, const string& endName) {
        int start = getIndex(startName);
        int end = getIndex(endName);

        if(start == -1 || end == -1) {
             cout << "[Error] One or both entities not found in the graph network.\n";
             return;
        }

        bool visited[200] = {false};
        int parent[200];
        for(int i=0; i<200; i++) parent[i] = -1;

        // Custom Queue using Array
        int q[200];
        int front = 0, rear = 0;

        q[rear++] = start;
        visited[start] = true;

        bool found = false;
        while(front < rear) {
            int curr = q[front++];
            if(curr == end) {
                found = true;
                break;
            }

            AdjNode* temp = adjList[curr];
            while(temp) {
                if(!visited[temp->dest]) {
                    visited[temp->dest] = true;
                    parent[temp->dest] = curr;
                    q[rear++] = temp->dest;
                }
                temp = temp->next;
            }
        }

        if(found) {
            cout << "\n[Shortest Connection Found]:\n";
            int path[200];
            int pathLen = 0;
            int curr = end;
            while(curr != -1) {
                path[pathLen++] = curr;
                curr = parent[curr];
            }
            // Print path in reverse (from start to end)
            for(int i = pathLen - 1; i >= 0; i--) {
                cout << nodes[path[i]];
                if(i > 0) cout << " -> ";
            }
            cout << "\n";
        } else {
            cout << "\nNo connection found between " << startName << " and " << endName << ".\n";
        }
    }

    // DFS Recursive Utility
    void DFSUtil(int v, bool visited[]) {
        visited[v] = true;
        cout << nodes[v] << " ";
        
        AdjNode* temp = adjList[v];
        while(temp) {
            if(!visited[temp->dest]) {
                cout << "-> ";
                DFSUtil(temp->dest, visited);
            }
            temp = temp->next;
        }
    }

    // DFS Search (Explore full network of a suspect)
    // Time Complexity: O(V + E)
    void DFS(const string& startName) {
        int start = getIndex(startName);
        if(start == -1) {
             cout << "[Error] Entity not found in graph.\n";
             return;
        }

        bool visited[200] = {false};
        cout << "\n[Full Network map from '" << startName << "']:\n";
        DFSUtil(start, visited);
        cout << "\n";
    }
};

// ==========================================
// SYSTEM CONTROLLER (Combines Tree & Graph)
// ==========================================

class CrimeManagementSystem {
private:
    AVLNode* root;
    CrimeGraph graph;

    // --- AVL Helper Functions ---

    int height(AVLNode* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int getBalance(AVLNode* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    // Right Rotation (LL Case)
    // Time Complexity: O(1)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = getMax(height(y->left), height(y->right)) + 1;
        x->height = getMax(height(x->left), height(x->right)) + 1;

        return x; // New root
    }

    // Left Rotation (RR Case)
    // Time Complexity: O(1)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = getMax(height(x->left), height(x->right)) + 1;
        y->height = getMax(height(y->left), height(y->right)) + 1;

        return y; // New root
    }

    // Insert AVL Node
    // Time Complexity: O(log N) due to balanced property
    AVLNode* insertAVL(AVLNode* node, int id, const string& name, const string& loc, const string& evidence) {
        // 1. Standard BST insertion
        if (node == nullptr)
            return new AVLNode(id, name, loc, evidence);

        if (id < node->CaseID)
            node->left = insertAVL(node->left, id, name, loc, evidence);
        else if (id > node->CaseID)
            node->right = insertAVL(node->right, id, name, loc, evidence);
        else {
            cout << "\n[Error] Case ID " << id << " already exists!\n";
            return node; // Duplicates not allowed
        }

        // 2. Update height of this ancestor node
        node->height = 1 + getMax(height(node->left), height(node->right));

        // 3. Get the balance factor
        int balance = getBalance(node);

        // 4. Balance the tree
        // Left Left Case (LL)
        if (balance > 1 && id < node->left->CaseID)
            return rightRotate(node);

        // Right Right Case (RR)
        if (balance < -1 && id > node->right->CaseID)
            return leftRotate(node);

        // Left Right Case (LR)
        if (balance > 1 && id > node->left->CaseID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case (RL)
        if (balance < -1 && id < node->right->CaseID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Search AVL
    // Time Complexity: Guaranteed O(log N)
    AVLNode* searchAVL(AVLNode* node, int id) {
        if (node == nullptr || node->CaseID == id) return node;
        if (node->CaseID < id) return searchAVL(node->right, id);
        return searchAVL(node->left, id);
    }

    // Display Inorder
    // Time Complexity: O(N)
    void inorderAVL(AVLNode* node) {
        if (node != nullptr) {
            inorderAVL(node->left);
            cout << "ID: " << node->CaseID 
                 << " | Suspect: " << node->SuspectName 
                 << " | Loc: " << node->Location 
                 << " | Evd: " << node->Evidence 
                 << " | [h=" << node->height << "]\n";
            inorderAVL(node->right);
        }
    }

    // Traverse AVL to build relations in the graph
    // Time Complexity: O(N * V)
    void populateGraphHelper(AVLNode* node) {
        if(node != nullptr) {
            // Relate Suspect to Location
            graph.addEdge(node->SuspectName, node->Location);
            // Relate Suspect to Evidence
            graph.addEdge(node->SuspectName, node->Evidence);
            // Relate Location to Evidence
            graph.addEdge(node->Location, node->Evidence);
            
            populateGraphHelper(node->left);
            populateGraphHelper(node->right);
        }
    }

public:
    CrimeManagementSystem() { root = nullptr; }

    void insertCase(int id, const string& name, const string& loc, const string& evidence) {
        root = insertAVL(root, id, name, loc, evidence);
    }

    void searchCase(int id) {
        AVLNode* result = searchAVL(root, id);
        if (result != nullptr) {
            cout << "\n--- Case Found ---\n";
            cout << "Case ID:     " << result->CaseID << "\n";
            cout << "Suspect:     " << result->SuspectName << "\n";
            cout << "Location:    " << result->Location << "\n";
            cout << "Evidence:    " << result->Evidence << "\n";
            cout << "Tree Height: " << result->height << "\n";
            cout << "------------------\n";
        } else {
            cout << "\n[Notice] Case ID " << id << " not found.\n";
        }
    }

    void displayCases() {
        if (root == nullptr) {
            cout << "\n[Notice] Database is empty.\n";
            return;
        }
        cout << "\n--- All Records (Indexed by CaseID) ---\n";
        inorderAVL(root);
        cout << "---------------------------------------\n";
    }

    void buildGraph() {
        graph.clearGraph();            // Reset the old graph
        if(root == nullptr) {
            cout << "\n[Error] Empty database. Add cases before building graph.\n";
            return;
        }
        populateGraphHelper(root);     // Convert AVL data to graph edges
        cout << "\n[Success] Network Graph generated dynamically from AVL Index.\n";
    }

    void findConnection(const string& entityA, const string& entityB) {
        graph.BFS(entityA, entityB);
    }

    void exploreNetwork(const string& entity) {
        graph.DFS(entity);
    }
};

// ==========================================
// MAIN FUNCTION (MENU DRIVEN)
// ==========================================

int main() {
    CrimeManagementSystem system;
    int choice;
    
    // Test data automatically inserted for quick testing during viva
    system.insertCase(102, "Alice", "Downtown Alley", "Footprints");
    system.insertCase(104, "Bob", "Riverside", "Abandoned Car");
    system.insertCase(101, "Charlie", "Downtown Alley", "Bloody Knife");
    system.insertCase(105, "Alice", "Storage Unit", "Stolen Cash");
    system.insertCase(103, "Dave", "Storage Unit", "Ski Mask");

    do {
        cout << "\n=============================================\n";
        cout << "  CRIME RECORD PLATFORM: AVL + GRAPH ENGINE  \n";
        cout << "=============================================\n";
        cout << "1. Insert Case (AVL)\n";
        cout << "2. Search Case (O(log N))\n";
        cout << "3. Display Cases\n";
        cout << "4. Compile Graph Network\n";
        cout << "5. BFS: Find Shortest Link\n";
        cout << "6. DFS: Explore Suspect Network\n";
        cout << "7. Exit\n";
        cout << "---------------------------------------------\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            while (cin.get() != '\n'); 
            choice = 7;
        }

        switch (choice) {
            case 1: {
                int id;
                string name, loc, ev;
                cout << "Case ID (int): "; cin >> id;
                while (cin.get() != '\n');
                
                cout << "Suspect Name: "; getline(cin, name);
                cout << "Location: "; getline(cin, loc);
                cout << "Evidence: "; getline(cin, ev);
                
                system.insertCase(id, name, loc, ev);
                cout << "[Inserted via AVL Rotation Engine]\n";
                break;
            }
            case 2: {
                int id;
                cout << "Case ID to search: "; cin >> id;
                system.searchCase(id);
                break;
            }
            case 3:
                system.displayCases();
                break;
            case 4:
                system.buildGraph();
                break;
            case 5: {
                string A, B;
                while (cin.get() != '\n');
                cout << "Enter Target A: "; getline(cin, A);
                cout << "Enter Target B: "; getline(cin, B);
                system.findConnection(A, B);
                break;
            }
            case 6: {
                string target;
                while (cin.get() != '\n');
                cout << "Enter entity to explore (Suspect/Loc): "; getline(cin, target);
                system.exploreNetwork(target);
                break;
            }
            case 7:
                cout << "\nTerminating system...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 7);

    return 0;
}
