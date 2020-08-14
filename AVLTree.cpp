#include "AVLTree.h"

// constructor, sets root PTR to NULL
AVL::AVL(){
    root_ptr_ = NULL;
}

// deconstructor
AVL::~AVL(){    
}


shared_ptr<avl_node> AVL::CreateNode(int data){
    shared_ptr<avl_node> node(new avl_node);
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->is_leaf = true;
    node->height = 1;
    node->balanced = 0;
    return node;
}

shared_ptr<avl_node> AVL::Insert(int data){
    
    // checks to see if there is a root. If not, insert the root and return it
    shared_ptr<avl_node> top = GetRoot();
    if (top == NULL){
        top = CreateNode(data);
        SetRoot(top);
        return top;
    }
    else {
        Insert(top, data);
    }
}

shared_ptr<avl_node> AVL::Insert(shared_ptr<avl_node> node, int data){
    
    // recursion base case - if the node doesn't exist, create it
    if (node == NULL){
        return CreateNode(data);
    }

    // recursive insert function start - go left if the data is less, go right if its greater
    if (node->data > data){
        node->left = Insert(node->left, data);
    }
    else {
        node->right = Insert(node->right, data);
    }

    // update the node's height and balance 
    node->height = max(GetHeight(node->left), GetHeight(node->right)) +1;
    node->balanced = GetBalance(node);

    // run through balancing the node if it caused the tree to become unbalanced
    if(node->balanced < -1 && data > node->right->data){
        return LeftRotation(node);
    }

    if (node->balanced > 1 && data < node->left->data){
        return RightRotation(node);
    }

    if (node->balanced > 1 && data > node->left->data){
        return Left_RightRotation(node);
    }

    if (node->balanced < -1 && data < node->right->data){
        return Right_LeftRotation(node);
    }

    // change the is_leaf variable if it's no longer a leaf
    if (node->left != NULL || node->right != NULL){
        node->is_leaf = false;
    }

    return node;
}

void AVL::Remove(int data){
    if (root_ptr_ == NULL){
        return;
    }

    shared_ptr<avl_node> cursor = GetRoot();
    shared_ptr<avl_node> parent;

    // search through the tree to find the data. Exit the loop once we've reached the end
    // or we found the node with the data
    while (cursor != NULL && cursor->data != data){
       parent = cursor;
       if (data < cursor->data){
           cursor = cursor->left;
       }
       else if (data > cursor -> data){
           cursor = cursor->right;
       }
    }

    // if the node is a leaf, just remove it
    if (cursor->is_leaf == true){
        if (parent->left == cursor){
            parent->left = NULL;
        }
        else {
            parent->right = NULL;
        }
        return;
    }

    //if the node is a parent with one child
    if (cursor->left == NULL && cursor->right != NULL){
        cursor->right->is_leaf = false;
        if (parent->left == cursor){
            parent->left = cursor->right;
        }
        else {
            parent->right = cursor->right;
        }
        return;
    }
    else if (cursor->left != NULL && cursor->right == NULL){
        if (parent->left == cursor){
            parent->left = cursor->left;
        }
        else {
            parent->right = cursor->left;
        }
        return;
    }

    // if the node is a parent with two children
    shared_ptr<avl_node> successor = cursor->right;
    shared_ptr<avl_node> succ_parent = cursor;
    while(successor->left != NULL){
        succ_parent = successor;
        successor = successor->left;
    }

    // if we are removing the root node but it's not a leaf
    if (parent == NULL){
        cursor->data = successor->data;
    }
    // if the successor does not have a left node
    else if (succ_parent == cursor){
        cursor->data = successor->data;
        cursor->right = successor->right;
        return;
    }

    // if the successor has a left node
    else {
        if (parent->left = cursor){
            parent->left->data = successor->data;
        }
        else {
            parent->right->data = successor->data;
        }
    }
    
    succ_parent->left = NULL;
    successor->is_leaf = false;
}

shared_ptr<avl_node> AVL::LeftRotation(shared_ptr<avl_node> node){
    
    // Creates two temporary nodes to help with flipping the nodes
    shared_ptr<avl_node> midnode = node->right;
    shared_ptr<avl_node> temp_node = midnode->left;

    // Rotate the nodes
    midnode->left = node;
    node->right = temp_node;

    // Get the old root node (node)'s height, and then new root of the subtree (midnode) new height
    node->height = max(GetHeight(node->left), GetHeight(node->right)) +1;
    midnode->height = max(GetHeight(midnode->left), GetHeight(midnode->right)) +1;

    // If we change the root with this new rotation, set the new root node
    shared_ptr<avl_node> root_ptr = GetRoot();
    if(node == root_ptr){
        SetRoot(midnode);
    }

    // if the rotation causes a new node to be a leaf, update is_leaf
    if (node->left == NULL && node->right == NULL){
        node->is_leaf = true;
    }

    // returns the new top of the subtree
    return midnode;
}

shared_ptr<avl_node> AVL::RightRotation(shared_ptr<avl_node> node){
    
    // Creates two temporary nodes to help with flipping the nodes
    shared_ptr<avl_node> midnode = node->left;
    shared_ptr<avl_node> temp_node = midnode->right;

    // Rotate the nodes
    midnode->right = node;
    node->left = temp_node;

    // Get the old root node (node)'s height, and then new root of the subtree (midnode) new height
    node->height = max(GetHeight(node->left), GetHeight(node->right)) +1;
    midnode->height = max(GetHeight(midnode->left), GetHeight(midnode->right)) +1;

    // If we change the root with this new rotation, set the new root node
    shared_ptr<avl_node> root_ptr = GetRoot();
    if(node == root_ptr){
        SetRoot(midnode);
    }

    // if the rotation causes a new node to be a leaf, update is_leaf
    if (node->left == NULL && node->right == NULL){
        node->is_leaf = true;
    }

    return midnode;
}

shared_ptr<avl_node> AVL::Left_RightRotation(shared_ptr<avl_node> node){
    node->left = LeftRotation(node->left);
    return RightRotation(node);
}

shared_ptr<avl_node> AVL::Right_LeftRotation(shared_ptr<avl_node> node){
    node->right = RightRotation(node->right);
    return LeftRotation(node);
}

shared_ptr<avl_node> AVL::GetRoot(){
    return root_ptr_;
}

void AVL::SetRoot(shared_ptr<avl_node> root_ptr){
    root_ptr_ = root_ptr;
}

int AVL::GetBalance(shared_ptr<avl_node> node){
    if(node != NULL){
        return (GetHeight(node->left) - GetHeight(node->right));
    }
    else {
        return 0;
    }
}

int AVL::GetHeight(shared_ptr<avl_node> node){
    if (node == NULL){
        return 0;
    }
    else {
        return node->height;
    }
}
