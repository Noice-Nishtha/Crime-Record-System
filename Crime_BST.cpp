#include <iostream>
#include <string>

using namespace std;

/**
 * Node Structure for Crime Record
 * Represents a single entity (case) in the Binary Search Tree.
 */
struct Node {
    int CaseID;             // Unique primary key used for BST ordering
    string SuspectName;
    string Location;
    string Evidence;
    
    Node* left;             // Pointer to the left child
    Node* right;            // Pointer to the right child

    // Constructor to initialize a new node
    Node(int id, string name, string loc, string evidence) {
        CaseID = id;
        SuspectName = name;
        Location = loc;
        Evidence = evidence;
        left = nullptr;
        right = nullptr;
    }
};

/**
 * CrimeBST Class
 * Follows OOP principles by encapsulating the root and logic.
 * Exposes clean public wrapper functions while hiding recursive logic in private helpers.
 */
class CrimeBST {
private:
    Node* root;

    /**
     * Recursive Insert Helper
     * Time Complexity: O(log N) average, O(N) worst case (skewed tree)
     */
    Node* insertNode(Node* node, int id, const string& name, const string& loc, const string& evidence) {
        if (node == nullptr) {
            return new Node(id, name, loc, evidence);
        }

        if (id < node->CaseID) {
            node->left = insertNode(node->left, id, name, loc, evidence);
        } 
        else if (id > node->CaseID) {
            node->right = insertNode(node->right, id, name, loc, evidence);
        } 
        else {
            // Handles duplicate CaseID
            cout << "\n[Error] Case ID " << id << " already exists in the system!\n";
        }
        return node;
    }

    /**
     * Recursive Search Helper (by ID)
     * Time Complexity: O(log N) average, O(N) worst case
     */
    Node* searchByID(Node* node, int id) {
        // Base cases: root is null or key is present at the current node
        if (node == nullptr || node->CaseID == id) {
            return node;
        }

        // Maintain BST property
        if (node->CaseID < id) {
            return searchByID(node->right, id);
        }
        return searchByID(node->left, id);
    }

    /**
     * Recursive Inorder Traversal Helper (Left -> Root -> Right)
     * Time Complexity: O(N) as it visits every node precisely once
     */
    void inorderTraversal(Node* node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            
            cout << "Case ID: " << node->CaseID 
                 << " | Suspect: " << node->SuspectName 
                 << " | Location: " << node->Location 
                 << " | Evidence: " << node->Evidence << "\n";
                 
            inorderTraversal(node->right);
        }
    }

    /**
     * Recursive Location Search Helper
     * Time Complexity: O(N) as it must traverse the entire tree to find all matches
     */
    void searchByLocation(Node* node, const string& loc, bool& found) {
        if (node != nullptr) {
            searchByLocation(node->left, loc, found);
            
            // Check for exact match or substring match
            if (node->Location.find(loc) != string::npos) {
                cout << "Case ID: " << node->CaseID 
                     << " | Suspect: " << node->SuspectName 
                     << " | Location: " << node->Location 
                     << " | Evidence: " << node->Evidence << "\n";
                found = true;
            }
            
            searchByLocation(node->right, loc, found);
        }
    }

    /**
     * Helper Function: Find Minimum Node
     * Used exclusively to find the inorder successor during deletion.
     * Time Complexity: O(log N) average
     */
    Node* findMin(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    /**
     * Recursive Delete Helper
     * Time Complexity: O(log N) average, O(N) worst-case
     */
    Node* deleteNode(Node* node, int id) {
        // Base case: Element not found
        if (node == nullptr) {
            return node;
        }

        if (id < node->CaseID) {
            node->left = deleteNode(node->left, id);
        } 
        else if (id > node->CaseID) {
            node->right = deleteNode(node->right, id);
        } 
        else {
            // Node found. 

            // Case A: Leaf node (No children) OR Case B: One child
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node; // Deallocate memory
                return temp;
            } 
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Case C: Node has two children
            // Find the inorder successor (smallest element in the right subtree)
            Node* temp = findMin(node->right);

            // Inherit the successor's data
            node->CaseID = temp->CaseID;
            node->SuspectName = temp->SuspectName;
            node->Location = temp->Location;
            node->Evidence = temp->Evidence;

            // Delete the duplicate successor node from the right subtree
            node->right = deleteNode(node->right, temp->CaseID);
        }
        return node;
    }

    /**
     * Recursive Tree Destructor Helper
     * Post-order traversal to delete memory safely from leaves to root.
     */
    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    // Constructor
    CrimeBST() {
        root = nullptr;
    }
    
    // Destructor to prevent memory leaks
    ~CrimeBST() {
        destroyTree(root);
    }

    // --- Public Wrapper Functions ---

    void insertCase(int id, const string& name, const string& loc, const string& evidence) {
        root = insertNode(root, id, name, loc, evidence);
    }

    void searchCase(int id) {
        Node* result = searchByID(root, id);
        if (result != nullptr) {
            cout << "\n--- Case Found ---\n";
            cout << "Case ID: " << result->CaseID << "\n";
            cout << "Suspect Name: " << result->SuspectName << "\n";
            cout << "Location: " << result->Location << "\n";
            cout << "Evidence: " << result->Evidence << "\n";
            cout << "------------------\n";
        } else {
            cout << "\n[Notice] Case ID " << id << " not found in the system.\n";
        }
    }

    void displayAllCases() {
        if (root == nullptr) {
            cout << "\n[Notice] No cases currently recorded in the system.\n";
            return;
        }
        cout << "\n--- All Crime Records (Sorted ASC by Case ID) ---\n";
        inorderTraversal(root);
        cout << "--------------------------------------------------\n";
    }

    void searchLocation(const string& loc) {
        bool found = false;
        cout << "\n--- Cases matching Location: " << loc << " ---\n";
        searchByLocation(root, loc, found);
        if (!found) {
            cout << "[Notice] No cases found in '" << loc << "'.\n";
        }
        cout << "------------------------------------\n";
    }

    void deleteCase(int id) {
        if (searchByID(root, id) == nullptr) {
            cout << "\n[Error] Case ID " << id << " does not exist. Cannot delete.\n";
            return;
        }
        root = deleteNode(root, id);
        cout << "\n[Success] Case ID " << id << " successfully deleted.\n";
    }
};

// --- Main Menu Interface ---
int main() {
    CrimeBST system;
    int choice;
    
    do {
        cout << "\n=============================================\n";
        cout << "    CRIME RECORD MANAGEMENT SYSTEM (BST)     \n";
        cout << "=============================================\n";
        cout << "1. Insert a Case\n";
        cout << "2. Search Case by ID\n";
        cout << "3. Display All Cases\n";
        cout << "4. Search by Location\n";
        cout << "5. Delete Case\n";
        cout << "6. Exit\n";
        cout << "---------------------------------------------\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        
        // Input validation for non-integer inputs
        if (cin.fail()) {
            cin.clear();
            while (cin.get() != '\n'); 
            cout << "[Error] Invalid input type. Entering default exit.\n";
            choice = 6;
        }

        switch (choice) {
            case 1: {
                int id;
                string name, loc, evidence;
                cout << "Enter Case ID (unique integer): ";
                cin >> id;
                
                // Clear the newline character from the buffer before using getline
                while (cin.get() != '\n');
                
                cout << "Enter Suspect Name: ";
                getline(cin, name);
                cout << "Enter Location: ";
                getline(cin, loc);
                cout << "Enter Evidence: ";
                getline(cin, evidence);
                
                system.insertCase(id, name, loc, evidence);
                cout << "Case dispatched to insertion queue.\n";
                break;
            }
            case 2: {
                int id;
                cout << "Enter Case ID to search: ";
                cin >> id;
                system.searchCase(id);
                break;
            }
            case 3: {
                system.displayAllCases();
                break;
            }
            case 4: {
                string loc;
                while (cin.get() != '\n'); // Clear buffer
                
                cout << "Enter Location to search: ";
                getline(cin, loc);
                system.searchLocation(loc);
                break;
            }
            case 5: {
                int id;
                cout << "Enter Case ID to delete: ";
                cin >> id;
                system.deleteCase(id);
                break;
            }
            case 6:
                cout << "\nExiting Crime Record Management System. Memory cleared. Goodbye!\n\n";
                break;
            default:
                cout << "[Error] Invalid choice! Please select between 1 and 6.\n";
        }
    } while (choice != 6);

    return 0;
}