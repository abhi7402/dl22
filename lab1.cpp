/**
 * LAB 1: 
 *      Design and implement Parallel Breadth First Search and Depth First Search based on existing algorithms using OpenMP. 
 *      Use a Tree or an undirected graph for BFS and DFS.
 * 
 * TO EXECUTE CODE:
 *      >> g++ -fopenmp lab1.cpp
 *      >> ./a
 */

#include <omp.h>
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

class Node
{
public:
    int value;
    Node *left;
    Node *right;

    Node()
    {
        // Required empty constructor.
    }

    Node(int value)
    {
        this->value = value;
        this->left = NULL;
        this->right = NULL;
    }
};

Node *generateTree(int values[], int length)
{
    if (length == 0)
        return NULL;

    Node *treeNodes[length];
    for (int i = 0; i < length; i++)
    {
        if (values[i] != -1)
            treeNodes[i] = new Node(values[i]);
        else
            treeNodes[i] = NULL;
    }

    int parent = 0;
    int child = 1;
    while (child < length)
    {
        if (treeNodes[parent] != NULL)
        {
            treeNodes[parent]->left = treeNodes[child++];
            if (child < length)
                treeNodes[parent]->right = treeNodes[child++];
        }
        parent++;
    }

    Node *root = treeNodes[0];

    return root;
}

void bfs(Node *root)
{
    if (root == NULL)
        return;

    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();

#pragma omp parallel for
        for (int i = 0; i < size; i++)
        {
            Node *node = q.front();
            q.pop();

            if (node != NULL)
            {
                cout << node->value << " -> ";
                if (node->left != NULL)
                    q.push(node->left);
                if (node->right != NULL)
                    q.push(node->right);
            }
        }
    }
}

void dfs(Node *root)
{
    if (root == NULL)
        return;

#pragma omp critical
    cout << root->value << " -> ";

#pragma omp parallel sections num_threads(2)
    {
#pragma omp section
        dfs(root->left);
#pragma omp section
        dfs(root->right);
    }
}

int main()
{
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int length = sizeof(values) / sizeof(int); // Calculate the length of array.
    Node *root = generateTree(values, length); // Generate the binary tree.

    cout << "BFS: ";
    bfs(root);
    cout << "x" << endl;

    cout << "DFS: ";
    dfs(root);
    cout << "x" << endl;

    return 0;
}