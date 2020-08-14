#ifndef AVLTREE_H__
#define AVLTREE_H__

#include <string>
#include <memory>
#include <vector>
#include <stdlib.h> 
#include <algorithm>

using namespace std;

  // avl_node is our AVL tree node structure
  struct avl_node {
    int data;
    shared_ptr<avl_node> left;
    shared_ptr<avl_node> right;
    bool is_leaf;
    int height; // distance from tree or subtree to a leaf
    int balanced; // keep track of subtree's balance (left - right subtree height)
  };


class AVL{
public:
  
  // constructor
  // Initially set root_ptr_ to a null shared_ptr of node
  AVL();
  
  // deconstructor
  ~AVL();

  // creates a new AVL node with the data given and two NULL children pointers
  // and returns a pointer
  shared_ptr<avl_node> CreateNode(int data);

  // inserts a node in the proper position of the AVL tree. Includes balances if the
  // insertion causes an unbalanced tree
  shared_ptr<avl_node> Insert(shared_ptr<avl_node> node, int data);

  // Insert function that creates root node. This function runs first - if there is already
  // a root, this function calls the above Insert function to insert the node properly
  shared_ptr<avl_node> Insert(int data);

  // removes a node from the tree that matches the input data. If there is no data that matches the 
  // input, it is ignored
  void Remove(int data);

  // Helper function to rotate a node left by taking in the node and rotating the node down to the left to
  // rebalance the tree
  shared_ptr<avl_node> LeftRotation(shared_ptr<avl_node> node);

  // Helper function to rotate a node right by taking i the node and rotating the node down to the right to
  // rebalance the tree
  shared_ptr<avl_node> RightRotation(shared_ptr<avl_node> node);

  // Double rotation function that sets up a single rotation by first rotating the node into a straight line
  // then calls right rotation to finish
  shared_ptr<avl_node> Left_RightRotation(shared_ptr<avl_node> node);

  // Double rotation function that sets up a single rotation by first rotating the node into a straight line
  // then calls left rotation to finish
  shared_ptr<avl_node> Right_LeftRotation(shared_ptr<avl_node> node);

  // Gets the root pointer and returns it
  shared_ptr<avl_node> GetRoot();

  // Set the top root
  void SetRoot(shared_ptr<avl_node> root_ptr); 

  // Get the balance factor for the subtrees left and right of the node passed through. Used to help
  // determine if a rebalance is needed
  int GetBalance(shared_ptr<avl_node> node);

  // Gets the height of the node from the top of the subtree through the longest path to a leaf. Returns 1 if
  // the node doesn't have any children
  int GetHeight(shared_ptr<avl_node> node);
    
private:
  shared_ptr<avl_node> root_ptr_;
};

#endif // AVLTREE_H__