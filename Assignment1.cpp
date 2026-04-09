// Consider the Dictionary Implementations which allow for efficient storage and retrieval of key-value pairs using binary search trees. Each node in the tree stores a (key, value) pair. The dictionary should support the following operations efficiently:
// Insert word (Handle insertion of duplicate entry)
// Delete word
// Search specific word
// Display dictionary (Traversal)
// Mirror image of dictionary
// Create a copy of dictionary
// Display dictionary level-wise

#include <iostream>
#include <string>
#include <stack>
#include <queue>

using namespace std;

struct node
{
    string word;
    string meaning;
    node *left, *right;

    node(string w, string m)
    {
        word = w;
        meaning = m;
        left = right = NULL;
    }
};

class Dictionary
{
public:
    node *root;

    Dictionary()
    {
        root = NULL;
    }

    // Insertion
    void insert(string word, string meaning)
    {
        node *N = new node(word, meaning);
        if (root == NULL)
        {
            root = N;
            return;
        }

        node *curr = root;
        node *parent = NULL;

        while (curr != NULL)
        {
            parent = curr;
            if (word < curr->word)
            {
                curr = curr->left;
            }
            else if (word > curr->word)
            {
                curr = curr->right;
            }
            else
            {
                cout << "Duplicate words not allowed!" << endl;
                delete N;
                return;
            }
        }

        if (word < parent->word)
        {
            parent->left = N;
        }
        else
        {
            parent->right = N;
        }
    }

    // Node Deletion
    void deleteNode(string key)
    {
        node *curr = root;
        node *parent = NULL;

        // Locate the target node and its parent
        while (curr != NULL && curr->word != key)
        {
            parent = curr;
            if (key < curr->word)
                curr = curr->left;
            else
                curr = curr->right;
        }

        if (curr == NULL)
        {
            cout << "Word not found!" << endl;
            return;
        }

        // Case 1 & 2: Leaf or One Child
        if (curr->left == NULL || curr->right == NULL)
        {
            node *newCurr;
            if (curr->left == NULL)
                newCurr = curr->right;
            else
                newCurr = curr->left;

            if (parent == NULL)
            {
                root = newCurr;
            }
            else if (curr == parent->left)
            {
                parent->left = newCurr;
            }
            else
            {
                parent->right = newCurr;
            }
            delete curr;
        }
        // Case 3: Two Children
        else
        {
            node *p_succ = NULL;
            node *successor = curr->right;
            while (successor->left != NULL)
            {
                p_succ = successor;
                successor = successor->left;
            }

            curr->word = successor->word;
            curr->meaning = successor->meaning;

            if (p_succ != NULL)
                p_succ->left = successor->right;
            else
                curr->right = successor->right;

            delete successor;
        }
    }

    // Searching
    void search(string target_word)
    {
        if (root == NULL)
        {
            cout << "Dictionary is empty" << endl;
            return;
        }
        node *curr = root;
        while (curr != NULL)
        {
            if (target_word == curr->word)
            {
                cout << "Found -> " << curr->word << ": " << curr->meaning << endl;
                return;
            }
            else if (target_word < curr->word)
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }
        }
        cout << "Word not found" << endl;
    }

    // Displaying Dictionary

    void displayInorder()
    {
        if (root == NULL)
        {
            cout << "Dictionary is empty" << endl;
            return;
        }
        stack<node *> S;
        node *curr = root;
        while (curr != NULL || !S.empty())
        {
            while (curr != NULL)
            {
                S.push(curr);
                curr = curr->left;
            }
            curr = S.top();
            S.pop();
            cout << curr->word << ": " << curr->meaning << endl;
            curr = curr->right;
        }
    }

    // Mirror Tree

    void mirror()
    {
        if (root == NULL)
            return;
        stack<node *> S;
        S.push(root);
        while (!S.empty())
        {
            node *curr = S.top();
            S.pop();

            swap(curr->left, curr->right);

            if (curr->left)
                S.push(curr->left);
            if (curr->right)
                S.push(curr->right);
        }
        cout << "Dictionary mirrored successfully." << endl;
    }

    // Create a Copy of Dictionary
    node *copyDictionary(node *original_root)
    {
        if (original_root == NULL)
            return NULL;

        node *copy_root = new node(original_root->word, original_root->meaning);
        queue<node *> q_orig, q_copy;

        q_orig.push(original_root);
        q_copy.push(copy_root);

        while (!q_orig.empty())
        {
            node *curr_orig = q_orig.front();
            q_orig.pop();
            node *curr_copy = q_copy.front();
            q_copy.pop();

            if (curr_orig->left)
            {
                node *L = new node(curr_orig->left->word, curr_orig->left->meaning);
                curr_copy->left = L;
                q_orig.push(curr_orig->left);
                q_copy.push(L);
            }
            if (curr_orig->right)
            {
                node *R = new node(curr_orig->right->word, curr_orig->right->meaning);
                curr_copy->right = R;
                q_orig.push(curr_orig->right);
                q_copy.push(R);
            }
        }
        return copy_root;
    }

    // Display Dictionary Level-wise

    void displayLevelWise()
    {
        if (root == NULL)
        {
            cout << "Dictionary is empty" << endl;
            return;
        }
        queue<node *> Q;
        Q.push(root);
        while (!Q.empty())
        {
            node *temp = Q.front();
            Q.pop();
            cout << temp->word << " ";
            if (temp->left != NULL)
                Q.push(temp->left);
            if (temp->right != NULL)
                Q.push(temp->right);
        }
        cout << endl;
    }
};


int main()
{
    Dictionary dict;
    int choice;
    string w, m;

    do
    {
        cout << "\n--- Dictionary Menu ---\n";
        cout << "1. Insert\n2. Delete\n3. Search\n4. Display (Inorder)\n5. Mirror\n6. Level-wise Display\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter word: ";
            cin >> w;
            cout << "Enter meaning: ";
            cin.ignore();
            getline(cin, m);
            dict.insert(w, m);
            break;
        case 2:
            cout << "Enter word to delete: ";
            cin >> w;
            dict.deleteNode(w);
            break;
        case 3:
            cout << "Enter word to search: ";
            cin >> w;
            dict.search(w);
            break;
        case 4:
            dict.displayInorder();
            break;
        case 5:
            dict.mirror();
            break;
        case 6:
            dict.displayLevelWise();
            break;
        }
    } while (choice != 7);

    return 0;
}
