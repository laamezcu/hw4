#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool pathCheck(Node* root, int height, int& leafHeight) {
    if (!root) return true;

    if (!root->left && !root->right) {
        if (leafHeight == -1) {
            leafHeight = height;
        }
        return height == leafHeight;
    }

    return pathCheck(root->left, height + 1, leafHeight) &&
            pathCheck(root->right, height + 1, leafHeight);
}


bool equalPaths(Node * root)
{
    // Add your code below
    int leafHeight = -1;
    return pathCheck(root, 0, leafHeight);
}

