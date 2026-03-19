#include <iostream>
#include <string>

using namespace std;

// Structure for a Crime Record Node
struct Node {
    int CaseID;
    string SuspectName;
    string Location;
    string Evidence;
    Node* left;
    Node* right;

    // Constructor to initialize a new node
    Node(int id, const string& name, const string& loc, const string& evidence) {
        CaseID = id;
        SuspectName = name;
        Location = loc;
        Evidence = evidence;
        left = nullptr;
        right = nullptr;
    }
};

// Class for the Binary Search Tree managing crime records
class CrimeBST {
private:
    Node* root;

    // Recursive helper functions
    Node* insertNode(Node* node, int id, const string& name, const string& loc, const string& evidence);
    Node* searchByID(Node* node, int id);
    void inorderTraversal(Node* node);
    void searchByLocation(Node* node, const string& loc, bool& found);
    void searchByEvidence(Node* node, const string& evidence, bool& found);
    void searchByName(Node* node, const string& name, bool& found);
    Node* deleteNode(Node* node, int id);
    Node* findMin(Node* node);

public:
    CrimeBST() {
        root = nullptr;
    }

    void insertCase(int id, const string& name, const string& loc, const string& evidence);
    void searchCase(int id);
    void displayAllCases();
    void searchLocation(const string& loc);
    void searchEvidence(const string& evidence);
    void searchName(const string& name);
    void deleteCase(int id);
};

// 1. Insert a case
Node* CrimeBST::insertNode(Node* node, int id, const string& name, const string& loc, const string& evidence) {
    // If tree is empty, return a new node
    if (node == nullptr) {
        return new Node(id, name, loc, evidence);
    }

    // Otherwise, recur down the tree
    if (id < node->CaseID) {
        node->left = insertNode(node->left, id, name, loc, evidence);
    } else if (id > node->CaseID) {
        node->right = insertNode(node->right, id, name, loc, evidence);
    } else {
        // CaseID already exists
        cout << "Case ID " << id << " already exists in the system!\n";
    }

    // return the (unchanged) node pointer
    return node;
}

void CrimeBST::insertCase(int id, const string& name, const string& loc, const string& evidence) {
    root = insertNode(root, id, name, loc, evidence);
}

// 2. Search by CaseID
Node* CrimeBST::searchByID(Node* node, int id) {
    // Base Cases: root is null or key is present at root
    if (node == nullptr || node->CaseID == id) {
        return node;
    }

    // Key is greater than root's key
    if (node->CaseID < id) {
        return searchByID(node->right, id);
    }

    // Key is smaller than root's key
    return searchByID(node->left, id);
}

void CrimeBST::searchCase(int id) {
    Node* result = searchByID(root, id);
    if (result != nullptr) {
        cout << "\n--- Case Found ---\n";
        cout << "Case ID: " << result->CaseID << "\n";
        cout << "Suspect Name: " << result->SuspectName << "\n";
        cout << "Location: " << result->Location << "\n";
        cout << "Evidence: " << result->Evidence << "\n";
        cout << "------------------\n";
    } else {
        cout << "Case ID " << id << " not found in the system.\n";
    }
}

// 3. Display all cases using inorder traversal
void CrimeBST::inorderTraversal(Node* node) {
    if (node != nullptr) {
        // Traverse left subtree
        inorderTraversal(node->left);
        
        // Visit node
        cout << "Case ID: " << node->CaseID 
             << " | Suspect: " << node->SuspectName 
             << " | Location: " << node->Location 
             << " | Evidence: " << node->Evidence << "\n";
             
        // Traverse right subtree
        inorderTraversal(node->right);
    }
}

void CrimeBST::displayAllCases() {
    if (root == nullptr) {
        cout << "No cases currently recorded in the system.\n";
        return;
    }
    cout << "\n--- All Crime Records (Ordered by Case ID) ---\n";
    inorderTraversal(root);
    cout << "----------------------------------------------\n";
}

// 4. Search cases by Location
void CrimeBST::searchByLocation(Node* node, const string& loc, bool& found) {
    if (node != nullptr) {
        // Search in left subtree
        searchByLocation(node->left, loc, found);
        
        // If location matches (using string::find for partial match)
        if (node->Location.find(loc) != string::npos) {
            cout << "Case ID: " << node->CaseID 
                 << " | Suspect: " << node->SuspectName 
                 << " | Evidence: " << node->Evidence << "\n";
            found = true;
        }
        
        // Search in right subtree
        searchByLocation(node->right, loc, found);
    }
}

void CrimeBST::searchLocation(const string& loc) {
    bool found = false;
    cout << "\n--- Cases matching Location: " << loc << " ---\n";
    searchByLocation(root, loc, found);
    if (!found) {
        cout << "No cases found at this location.\n";
    }
    cout << "------------------------------------\n";
}

// 4a. Search cases by Evidence
void CrimeBST::searchByEvidence(Node* node, const string& evidence, bool& found) {
    if (node != nullptr) {
        searchByEvidence(node->left, evidence, found);
        
        if (node->Evidence.find(evidence) != string::npos) {
            cout << "Case ID: " << node->CaseID 
                 << " | Suspect: " << node->SuspectName 
                 << " | Location: " << node->Location 
                 << " | Evidence: " << node->Evidence << "\n";
            found = true;
        }
        
        searchByEvidence(node->right, evidence, found);
    }
}

void CrimeBST::searchEvidence(const string& evidence) {
    bool found = false;
    cout << "\n--- Cases matching Evidence: " << evidence << " ---\n";
    searchByEvidence(root, evidence, found);
    if (!found) {
        cout << "No cases found with this evidence.\n";
    }
    cout << "------------------------------------\n";
}

// 4b. Search cases by Suspect Name
void CrimeBST::searchByName(Node* node, const string& name, bool& found) {
    if (node != nullptr) {
        searchByName(node->left, name, found);
        
        if (node->SuspectName.find(name) != string::npos) {
            cout << "Case ID: " << node->CaseID 
                 << " | Suspect: " << node->SuspectName 
                 << " | Location: " << node->Location 
                 << " | Evidence: " << node->Evidence << "\n";
            found = true;
        }
        
        searchByName(node->right, name, found);
    }
}

void CrimeBST::searchName(const string& name) {
    bool found = false;
    cout << "\n--- Cases matching Suspect Name: " << name << " ---\n";
    searchByName(root, name, found);
    if (!found) {
        cout << "No cases found with this suspect name.\n";
    }
    cout << "------------------------------------\n";
}

// Helper for deletion to find minimum node in a subtree
Node* CrimeBST::findMin(Node* node) {
    Node* current = node;
    // Loop down to find the leftmost leaf
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// 5. Delete a case (handle leaf, one child, two children)
Node* CrimeBST::deleteNode(Node* node, int id) {
    // Base case
    if (node == nullptr) {
        return node;
    }

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if (id < node->CaseID) {
        node->left = deleteNode(node->left, id);
    }
    // If the key to be deleted is greater than the root's key,
    // then it lies in right subtree
    else if (id > node->CaseID) {
        node->right = deleteNode(node->right, id);
    }
    // if key is same as root's key, then this is the node to be deleted
    else {
        // Case 1 & 2: Node with only one child or no child (leaf node)
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Node with two children
        // Get the inorder successor (smallest in the right subtree)
        Node* temp = findMin(node->right);

        // Copy the inorder successor's content to this node
        node->CaseID = temp->CaseID;
        node->SuspectName = temp->SuspectName;
        node->Location = temp->Location;
        node->Evidence = temp->Evidence;

        // Delete the inorder successor
        node->right = deleteNode(node->right, temp->CaseID);
    }
    return node;
}

void CrimeBST::deleteCase(int id) {
    // Check if the case exists before proceeding
    if (searchByID(root, id) == nullptr) {
        cout << "Case ID " << id << " not found. Cannot delete.\n";
        return;
    }
    root = deleteNode(root, id);
    cout << "Case ID " << id << " successfully deleted.\n";
}

// Menu-driven program
int main() {
    CrimeBST system;
    int choice;
    
    do {
        cout << "\n=============================================\n";
        cout << "    CRIME RECORD MANAGEMENT SYSTEM (BST)     \n";
        cout << "=============================================\n";
        cout << "1. Insert a New Case\n";
        cout << "2. Search Case by ID\n";
        cout << "3. Display All Cases (Inorder Traversal)\n";
        cout << "4. Search Cases by Location\n";
        cout << "5. Search Cases by Suspect Name\n";
        cout << "6. Search Cases by Evidence\n";
        cout << "7. Delete a Case\n";
        cout << "0. Exit\n";
        cout << "---------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Handle input clear if an invalid type is entered
        if (cin.fail()) {
            cin.clear();
            // discard characters until newline or up to 1000 characters
            for (int i = 0; i < 1000; ++i) {
                if (cin.get() == '\n') break;
            }
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                string name, loc, evidence;
                cout << "Enter Case ID (unique integer): ";
                cin >> id;
                
                // Clear the newline character from the buffer before using getline
                char c;
                while ((c = cin.get()) != '\n' && c != EOF);
                
                cout << "Enter Suspect Name: ";
                getline(cin, name);
                
                cout << "Enter Location: ";
                getline(cin, loc);
                
                cout << "Enter Evidence: ";
                getline(cin, evidence);
                
                system.insertCase(id, name, loc, evidence);
                cout << "Case processed for insertion.\n";
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
                // Clear the buffer
                char c;
                while ((c = cin.get()) != '\n' && c != EOF);
                
                cout << "Enter Location to search: ";
                getline(cin, loc);
                system.searchLocation(loc);
                break;
            }
            case 5: {
                string name;
                char c;
                while ((c = cin.get()) != '\n' && c != EOF);
                
                cout << "Enter Suspect Name to search: ";
                getline(cin, name);
                system.searchName(name);
                break;
            }
            case 6: {
                string evidence;
                char c;
                while ((c = cin.get()) != '\n' && c != EOF);
                
                cout << "Enter Evidence to search: ";
                getline(cin, evidence);
                system.searchEvidence(evidence);
                break;
            }
            case 7: {
                int id;
                cout << "Enter Case ID to delete: ";
                cin >> id;
                system.deleteCase(id);
                break;
            }
            case 0:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}