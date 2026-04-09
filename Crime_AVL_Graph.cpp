#include <iostream>
#include <string>

using namespace std;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================
int getMax(int a, int b) {
    return (a > b) ? a : b;
}

// ==========================================
// UNIT 1, 2 & 4: AVL TREE FOR INDEXING 
// ==========================================
/*
 * VIVA THEORY (Unit 4 - Indexing Concept):
 * Why AVL Tree for Indexing?
 * The CaseID serves as a primary key. By storing CaseIDs in a self-balancing AVL Tree, 
 * we guarantee O(log N) height. This ensures that no matter how many cases are inserted, 
 * the lookup (Search), Insertion, and Deletion operations will consistently take O(log N) time,
 * effectively acting as a high-speed database index compared to an O(N) array or skewed BST.
 */

struct AVLNode {
    int CaseID;             
    string SuspectName;
    string Location;
    string Evidence;
    
    int height;             
    AVLNode* left;
    AVLNode* right;

    AVLNode(int id, string name, string loc, string evidence) {
        CaseID = id; SuspectName = name; Location = loc; Evidence = evidence;
        height = 1;         
        left = right = nullptr;
    }
};

// ==========================================
// UNIT 3: GRAPH FOR NETWORK ANALYSIS
// ==========================================
/*
 * VIVA THEORY (Unit 3 - Graphs):
 * The Graph acts as an investigative relationship mapping tool.
 * Suspects, Locations, and Evidences are extracted from the AVL Index and mapped 
 * as distinct vertices (Nodes). Undirected Edges are added bidirectionally 
 * (e.g. Suspect <-> Location) simulating how real-world entities are connected.
 * This allows us to use BFS to find the absolute shortest conceptual link 
 * between two entities, and DFS to recursively trace a suspect's full footprint.
 */

struct AdjNode {
    int dest;
    AdjNode* next;
    AdjNode(int d) : dest(d), next(nullptr) {}
};

class CrimeGraph {
private:
    string nodes[200];      
    AdjNode* adjList[200];  
    int numNodes;

    int getIndex(const string& name) {
        for(int i = 0; i < numNodes; i++) {
            if(nodes[i] == name) return i;
        }
        if(numNodes < 200) {
            nodes[numNodes] = name;
            return numNodes++;
        }
        return -1; 
    }

public:
    CrimeGraph() { 
        numNodes = 0; 
        for(int i=0; i<200; i++) adjList[i] = nullptr;
    }

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

    // Bidirectional Graph Edge Creation
    void addEdge(const string& uName, const string& vName) {
        if (uName == vName) return; // Prevent logical self-loops

        int u = getIndex(uName);
        int v = getIndex(vName);
        if(u == -1 || v == -1) return;

        // Ensure edges are not duplicated if multiple cases map the same association
        AdjNode* curr = adjList[u];
        while(curr) {
            if(curr->dest == v) return; 
            curr = curr->next;
        }

        // Add Direction 1: u -> v
        AdjNode* edge1 = new AdjNode(v);
        edge1->next = adjList[u];
        adjList[u] = edge1;

        // Add Direction 2: v -> u (Ensuring purely Bidirectional relationship)
        AdjNode* edge2 = new AdjNode(u);
        edge2->next = adjList[v];
        adjList[v] = edge2;
    }

    // BFS (Shortest Connection Path with Parent Mapping Reconstruction)
    void BFS(const string& startName, const string& endName) {
        if (numNodes == 0) {
            cout << "\n[Error] Graph is completely empty.\n";
            return;
        }

        int start = getIndex(startName);
        int end = getIndex(endName);

        if(start == -1 || end == -1) {
             cout << "\n[Error] One or both entities not found in the graph network.\n";
             return;
        }

        if(start == end) {
            cout << "\nEntities are the same!\n";
            return;
        }

        // Visited structure prevents infinite loops or revisiting mapped nodes
        bool visited[200] = {false};
        
        // Parent structure crucially reconstructs the exact shortest path traversal
        int parent[200];
        for(int i=0; i<200; i++) parent[i] = -1;

        int q[200];
        int front = 0, rear = 0;

        q[rear++] = start;
        visited[start] = true;

        bool found = false;
        while(front < rear) {
            int curr = q[front++];
            if(curr == end) {
                found = true;
                break; // Shortest path guarantees first arrival is optimal
            }

            AdjNode* temp = adjList[curr];
            while(temp) {
                if(!visited[temp->dest]) {
                    visited[temp->dest] = true;
                    parent[temp->dest] = curr; // Map the parent for backtracking
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
            
            // Reconstruct the path backwards using mapping
            while(curr != -1) {
                path[pathLen++] = curr;
                curr = parent[curr];
            }
            
            // Output path correctly formatted Source -> Intermediary -> Destination
            for(int i = pathLen - 1; i >= 0; i--) {
                cout << nodes[path[i]];
                if(i > 0) cout << " -> ";
            }
            cout << "\n";
        } else {
            cout << "\nNo conceptual connection bridges '" << startName << "' and '" << endName << "'.\n";
        }
    }

    // DFS Recursive Utility (Uses Visited structure to block infinite recursion)
    void DFSUtil(int v, bool visited[], int depth) {
        visited[v] = true;
        
        // Indentation visually models the conceptual backtracking tree of a DFS
        for(int i = 0; i < depth; i++) cout << "   ";
        cout << " |-- " << nodes[v] << "\n";
        
        AdjNode* temp = adjList[v];
        while(temp) {
            if(!visited[temp->dest]) {
                DFSUtil(temp->dest, visited, depth + 1);
            }
            temp = temp->next;
        }
    }

    // DFS Search (Explore a suspect's full footprints)
    void DFS(const string& startName) {
        if (numNodes == 0) {
            cout << "\n[Error] Graph is empty.\n";
            return;
        }

        int start = getIndex(startName);
        if(start == -1) {
             cout << "\n[Error] Entity not found in graph.\n";
             return;
        }

        bool visited[200] = {false};
        cout << "\n[Deep Footprint Network from '" << startName << "']:\n";
        DFSUtil(start, visited, 0);
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
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        // Heights must be updated strictly from bottom-up logic
        y->height = getMax(height(y->left), height(y->right)) + 1;
        x->height = getMax(height(x->left), height(x->right)) + 1;

        return x; 
    }

    // Left Rotation (RR Case)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = getMax(height(x->left), height(x->right)) + 1;
        y->height = getMax(height(y->left), height(y->right)) + 1;

        return y; 
    }

    // Recursive AVL Insert with 4-Case Balance Handling
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
            return node; 
        }

        // 2. Update height of ancestor
        node->height = 1 + getMax(height(node->left), height(node->right));

        // 3. Obtain Balance Factor
        int balance = getBalance(node);

        // 4. Handle all 4 distinct balancing scenarios perfectly
        // LL Case: Left subtree heavier, newest inserted left
        if (balance > 1 && id < node->left->CaseID)
            return rightRotate(node);

        // RR Case: Right subtree heavier, newest inserted right
        if (balance < -1 && id > node->right->CaseID)
            return leftRotate(node);

        // LR Case: Left subtree heavier, newest inserted right
        if (balance > 1 && id > node->left->CaseID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RL Case: Right subtree heavier, newest inserted left
        if (balance < -1 && id < node->right->CaseID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode* searchAVL(AVLNode* node, int id) {
        if (node == nullptr || node->CaseID == id) return node;
        if (node->CaseID < id) return searchAVL(node->right, id);
        return searchAVL(node->left, id);
    }

    void inorderAVL(AVLNode* node) {
        if (node != nullptr) {
            inorderAVL(node->left);
            cout << "Case [" << node->CaseID << "]  " 
                 << "Suspect: " << node->SuspectName 
                 << "  |  Loc: " << node->Location 
                 << "  |  Evd: " << node->Evidence 
                 << "  [Bal-Height: " << node->height << "]\n";
            inorderAVL(node->right);
        }
    }

    void populateGraphHelper(AVLNode* node) {
        if(node != nullptr) {
            graph.addEdge(node->SuspectName, node->Location);
            graph.addEdge(node->SuspectName, node->Evidence);
            graph.addEdge(node->Location, node->Evidence);
            
            populateGraphHelper(node->left);
            populateGraphHelper(node->right);
        }
    }

    // Rebuild Graph automatically completely preventing stale data
    void buildGraph() {
        graph.clearGraph();            
        if(root != nullptr) {
            populateGraphHelper(root);     
        }
    }

public:
    CrimeManagementSystem() { root = nullptr; }

    void insertCase(int id, const string& name, const string& loc, const string& evidence) {
        root = insertAVL(root, id, name, loc, evidence);
        
        // AUTO-REBUILD: Automatically propagate changes pushing new AVL data into Graph
        buildGraph();
    }

    void searchCase(int id) {
        if (root == nullptr) {
            cout << "\n[Notice] System DB is currently empty.\n";
            return;
        }

        AVLNode* result = searchAVL(root, id);
        if (result != nullptr) {
            cout << "\n--- Case Found ---\n";
            cout << "Case ID:     " << result->CaseID << "\n";
            cout << "Suspect:     " << result->SuspectName << "\n";
            cout << "Location:    " << result->Location << "\n";
            cout << "Evidence:    " << result->Evidence << "\n";
            cout << "Tree Height: " << result->height << " (O(log n) efficiency mapped)\n";
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
    
    // Seed test cases to immediately trigger AVL rotations and Graph Network 
    system.insertCase(102, "Alice", "Downtown Alley", "Footprints");
    system.insertCase(104, "Bob", "Riverside", "Abandoned Car");
    system.insertCase(101, "Charlie", "Downtown Alley", "Bloody Knife");
    system.insertCase(105, "Alice", "Storage Unit", "Stolen Cash");
    system.insertCase(103, "Dave", "Storage Unit", "Ski Mask");

    do {
        cout << "\n=============================================\n";
        cout << "  CRIME RECORD PLATFORM: AVL + GRAPH ENGINE  \n";
        cout << "=============================================\n";
        cout << "1. Insert Case (Auto-balances & Updates Graph)\n";
        cout << "2. Search Case (O(log N))\n";
        cout << "3. Display Cases\n";
        cout << "4. BFS: Find Shortest Link (e.g., Alice -> Dave)\n";
        cout << "5. DFS: Explore Suspect Footprint (e.g., Alice)\n";
        cout << "6. Exit\n";
        cout << "---------------------------------------------\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            while (cin.get() != '\n'); 
            choice = 6;
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
                cout << "[Inserted via AVL Rotation Engine / Graph Synced]\n";
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
            case 4: {
                string A, B;
                while (cin.get() != '\n');
                cout << "Enter Target A: "; getline(cin, A);
                cout << "Enter Target B: "; getline(cin, B);
                system.findConnection(A, B);
                break;
            }
            case 5: {
                string target;
                while (cin.get() != '\n');
                cout << "Enter entity to explore (Suspect/Loc): "; getline(cin, target);
                system.exploreNetwork(target);
                break;
            }
            case 6:
                cout << "\nTerminating system...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
