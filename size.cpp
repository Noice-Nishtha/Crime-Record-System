
// #include <iostream>
// #include <string>
// #include <queue>
// using namespace std;

// class Node
// {
// public:
//     string word, meaning;
//     Node *left, *right;
//     Node(string w, string m)
//     {
//         word = w;
//         meaning = m;
//         left = right = nullptr;
//     }
// };

// class Dictionary
// {
// public:
//     Node *root;
//     Dictionary() { root = nullptr; }

//     // 1. Insert
//     Node *insert(Node *node, string w, string m)
//     {
//         if (node == nullptr)
//             return new Node(w, m);
//         if (w < node->word)
//             node->left = insert(node->left, w, m);
//         else if (w > node->word)
//             node->right = insert(node->right, w, m);
//         else
//             node->meaning = m; // Duplicate: Update meaning
//         return node;
//     }

//     // 3. Search
//     void search(Node *node, string w)
//     {
//         if (node == nullptr)
//         {
//             cout << "Word not found!\n";
//             return;
//         }
//         if (w == node->word)
//             cout << "Meaning: " << node->meaning << endl;
//         else if (w < node->word)
//             search(node->left, w);
//         else
//             search(node->right, w);
//     }

//     // 4. In-order Traversal
//     void display(Node *node)
//     {
//         if (node == nullptr)
//             return;
//         display(node->left);
//         cout << node->word << " : " << node->meaning << endl;
//         display(node->right);
//     }

//     // 5. Mirror Image
//     void mirror(Node *node)
//     {
//         if (node == nullptr)
//             return;
//         swap(node->left, node->right);
//         mirror(node->left);
//         mirror(node->right);
//     }

//     // 7. Level-wise Display
//     void displayLevelWise(Node *root)
//     {
//         if (!root)
//             return;
//         queue<Node *> q;
//         q.push(root);
//         while (!q.empty())
//         {
//             Node *current = q.front();
//             q.pop();
//             cout << current->word << " ";
//             if (current->left)
//                 q.push(current->left);
//             if (current->right)
//                 q.push(current->right);
//         }
//         cout << endl;
//     }
// };


#include<iostream>
using namespace std;

int main(){
    cout<<"Hello World";
    return 0;
}