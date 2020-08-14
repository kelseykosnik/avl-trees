// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/AVLTree.h"

#include <iostream>
#include <string>

using namespace std;

class test_AVL : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
	}
    
	// this function runs before every TEST_F function
	void SetUp() override {
    }

	// this function runs after every TEST_F function
	void TearDown() override {
	}
};

shared_ptr<avl_node> HandBuildNode( int data )
{
  shared_ptr<avl_node> top(new avl_node);
  top->data = data;
  top->left = NULL;
  top->right = NULL;
  top->is_leaf = false;
  return top;
}


shared_ptr<avl_node> HandBuildTree ( )
{
	/* Hand build an unbalanced tree with 0, 1, 2, 3, 4, 5 in it. Looks
	   like this:
	  
	      1
	     / \
	    0   4
	       /  \
	      3    5
	     /
	    2
	*/
	shared_ptr<avl_node> top = HandBuildNode(1);
	top->left = HandBuildNode(0);
	top->left->is_leaf = true;
	top->right = HandBuildNode(4);
	top->right->left = HandBuildNode(3);
	top->right->left->left = HandBuildNode(2);
	top->right->left->left->is_leaf = true;
	top->right->right = HandBuildNode(5);
	top->right->right->is_leaf = true;
	
	return top;
}

TEST_F(test_AVL, TestConstructor){
	AVL avltree;
	// expect root to be NULL when BST object constructed
	ASSERT_FALSE(avltree.GetRoot());
}

TEST_F(test_AVL, TestCreateNode){
	AVL avltree;
	shared_ptr<avl_node> nodeptr = avltree.CreateNode(4);  
	ASSERT_TRUE(nodeptr);
	ASSERT_EQ(4,nodeptr->data);
	ASSERT_FALSE(nodeptr->left);
	ASSERT_FALSE(nodeptr->right);
}

TEST_F(test_AVL, TestInsert){

	// insert them
	AVL avltree;
	shared_ptr<avl_node> root = NULL;

	root = avltree.Insert(2);
	root = avltree.Insert(1);
	root = avltree.Insert(3);
	root = avltree.Insert(4);

	//root shouldn't be null
	ASSERT_TRUE(avltree.GetRoot());
 
	// check for values in the nodes
	ASSERT_EQ(2,avltree.GetRoot()->data);
	ASSERT_EQ(1,avltree.GetRoot()->left->data);
	ASSERT_EQ(3,avltree.GetRoot()->right->data);
	ASSERT_EQ(4,avltree.GetRoot()->right->right->data);

	// check for NULLs in the leafs
	ASSERT_FALSE(avltree.GetRoot()->left->left);
	ASSERT_FALSE(avltree.GetRoot()->left->right);
	ASSERT_FALSE(avltree.GetRoot()->right->left);
	ASSERT_FALSE(avltree.GetRoot()->right->right->left);
	ASSERT_FALSE(avltree.GetRoot()->right->right->right);
}

TEST_F(test_AVL, TestRemove1child){
	// Create separate scopes for this test.
  {
		// Hand build a node with 0, 1, 2, 3, 4, 5 in it.
		shared_ptr<avl_node> top = HandBuildTree();
		
		AVL avltree;
		avltree.SetRoot(top);

		// remove the leaf node 5
		// try removing a leaf

		avltree.Remove(5);
		ASSERT_FALSE(top->right->right);
	}	
	{
		// Tests removing a branch with one child.
		// Hand build a node with 0, 1, 2, 3, 4, 5 in it.
		shared_ptr<avl_node> top = HandBuildTree();
		
		AVL avltree;
		avltree.SetRoot(top);

		avltree.Remove(3);
		ASSERT_FALSE(top->right->left->left);
		ASSERT_EQ(2,top->right->left->data);
	}
}

TEST_F(test_AVL, TestRemovefork){
	{
		shared_ptr<avl_node> top = HandBuildTree();
		AVL avltree;
		avltree.SetRoot(top);
		// Remove a node with two children (root)
		avltree.Remove(1);
		ASSERT_EQ(2,top->data);
		ASSERT_FALSE(top->right->left->left);
	}
	{
		shared_ptr<avl_node> top = HandBuildTree();
		AVL avltree;
		avltree.SetRoot(top);
		// Remove a node with two children (non-root)
		avltree.Remove(4);
		ASSERT_EQ(5,top->right->data);
		ASSERT_EQ(1,top->data);
		ASSERT_FALSE(top->right->right);
	}
}

TEST_F(test_AVL, TestLeftRotation){
	/* Build an tree with 0, 1, 3, 4, 5 in it. Looks
	   like this:
	  
	      1
	     / \
	    0   4
	       /  \
	      3    5
	*/     
	{
		AVL avltree;
		avltree.Insert(1);
		avltree.Insert(0);
		avltree.Insert(4);
		avltree.Insert(3);
		avltree.Insert(5);

		// root shouldn't be null
		ASSERT_TRUE(avltree.GetRoot());

		// Create a new node and insert it, making the tree unbalanced
		// shared_ptr<avl_node> six(new avl_node);
		// six->data = 6;
		// six->left = NULL;
		// six->right = NULL;

		avltree.Insert(6);

		// check to make sure the left rotation worked
		ASSERT_EQ(4,avltree.GetRoot()->data);
		ASSERT_EQ(1,avltree.GetRoot()->left->data);
		ASSERT_EQ(5,avltree.GetRoot()->right->data);
		ASSERT_EQ(0,avltree.GetRoot()->left->left->data);
		ASSERT_EQ(3,avltree.GetRoot()->left->right->data);
		ASSERT_EQ(6,avltree.GetRoot()->right->right->data);

		// Check for proper height for all nodes
		ASSERT_EQ(3,avltree.GetRoot()->height);
		ASSERT_EQ(2,avltree.GetRoot()->left->height);
		ASSERT_EQ(2,avltree.GetRoot()->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->right->height);
	}
	/* Build an tree with 0, 1, 3, 4, 5 in it. Looks
	   like this:
	  
	      1
	     / \
	    0   4
	         \
	    	   5
	*/    
	
	{
		// Use Insert to Build the tree
		AVL avltree;
		avltree.Insert(1);
		avltree.Insert(0);
		avltree.Insert(4);
		avltree.Insert(5);

		// Root shouldn't be null
		ASSERT_TRUE(avltree.GetRoot());

		// Add in the 6 to make the tree unbalanced
		avltree.Insert(6);
		
		// check to make sure the left rotation worked
		ASSERT_EQ(1,avltree.GetRoot()->data);
		ASSERT_EQ(0,avltree.GetRoot()->left->data);
		ASSERT_EQ(5,avltree.GetRoot()->right->data);
		ASSERT_EQ(4,avltree.GetRoot()->right->left->data);
		ASSERT_EQ(6,avltree.GetRoot()->right->right->data);

		// Check for proper height for all nodes
		ASSERT_EQ(3,avltree.GetRoot()->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->height);
		ASSERT_EQ(2,avltree.GetRoot()->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->right->height);
	}
}

TEST_F(test_AVL, TestRightRotation){
	/* Build an tree with 2, 3, 4, 5, and 6 in it. Looks
	   like this:
	  
	      5
	     / \
	    3   6
	   / \
	  2   4

	*/     
	{
		AVL avltree;
		avltree.Insert(5);
		avltree.Insert(6);
		avltree.Insert(3);
		avltree.Insert(4);
		avltree.Insert(2);

		// root shouldn't be null
		ASSERT_TRUE(avltree.GetRoot());

		// insert 1 to make the tree unbalanced
		avltree.Insert(1);

		// check to make sure the left rotation worked. Tree should be:
	/*    3
	     / \
	    2   5
	   /   /  \
	  1   4    6

	*/
		ASSERT_EQ(3,avltree.GetRoot()->data);
		ASSERT_EQ(2,avltree.GetRoot()->left->data);
		ASSERT_EQ(5,avltree.GetRoot()->right->data);
		ASSERT_EQ(1,avltree.GetRoot()->left->left->data);
		ASSERT_EQ(4,avltree.GetRoot()->right->left->data);
		ASSERT_EQ(6,avltree.GetRoot()->right->right->data);

		// Check for proper height for all nodes
		ASSERT_EQ(3,avltree.GetRoot()->height);
		ASSERT_EQ(2,avltree.GetRoot()->left->height);
		ASSERT_EQ(2,avltree.GetRoot()->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->right->height);
	}
	/* Build an tree with 2, 3, 4, 5, 6, 7 in it. Looks
	   like this:
	  
	      4
	     / \
	    3   6
	   /   /  \
	  2   5	   7
	*/    
	
	{
		// Use Insert to Build the tree
		AVL avltree;
		avltree.Insert(4);
		avltree.Insert(3);
		avltree.Insert(6);
		avltree.Insert(2);
		avltree.Insert(5);
		avltree.Insert(7);

		// Root shouldn't be null
		ASSERT_TRUE(avltree.GetRoot());

		// Add in the 1 to make the tree unbalanced
		avltree.Insert(1);
		
		// check to make sure the right rotation worked. Tree should be:
	/*
	       4
	     /   \
	    2      6
	   / \    /  \
	  1   3  5	  7
	*/
		ASSERT_EQ(4,avltree.GetRoot()->data);
		ASSERT_EQ(2,avltree.GetRoot()->left->data);
		ASSERT_EQ(6,avltree.GetRoot()->right->data);
		ASSERT_EQ(1,avltree.GetRoot()->left->left->data);
		ASSERT_EQ(3,avltree.GetRoot()->left->right->data);
		ASSERT_EQ(5,avltree.GetRoot()->right->left->data);
		ASSERT_EQ(7,avltree.GetRoot()->right->right->data);

		// Check for proper height for all nodes
		ASSERT_EQ(3,avltree.GetRoot()->height);
		ASSERT_EQ(2,avltree.GetRoot()->left->height);
		ASSERT_EQ(2,avltree.GetRoot()->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->right->height);
	}
}

TEST_F(test_AVL, TestLeft_RightRotation){
	/* Build an tree with 0, 1, 3, and 5 in it. Looks
	   like this:
	  
	      1
	     / \
	    0   5
	  	   /
		  3	  	

	*/     
	{
		AVL avltree;
		avltree.Insert(1);
		avltree.Insert(0);
		avltree.Insert(5);
		avltree.Insert(3);

		// root shouldn't be null
		ASSERT_TRUE(avltree.GetRoot());

		// insert 4 to make the tree unbalanced
		avltree.Insert(4);

		// check to make sure the left rotation worked. Tree should be:
	/*    1
	     / \
	    0   4
	  	   / \
		  3	  5
	*/
		ASSERT_EQ(1,avltree.GetRoot()->data);
		ASSERT_EQ(0,avltree.GetRoot()->left->data);
		ASSERT_EQ(4,avltree.GetRoot()->right->data);
		ASSERT_EQ(3,avltree.GetRoot()->right->left->data);
		ASSERT_EQ(5,avltree.GetRoot()->right->right->data);

		// Check for proper height for all nodes
		ASSERT_EQ(3,avltree.GetRoot()->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->height);
		ASSERT_EQ(2,avltree.GetRoot()->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->right->height);
	}
}

TEST_F(test_AVL, TestRight_LeftRotation){
	/* Build an tree with 2, 4, 5, and 6 in it. Looks
	   like this:
	  
	      5
	     / \
	    2   6
	 	 \
          4
	*/     
	{
		AVL avltree;
		avltree.Insert(5);
		avltree.Insert(2);
		avltree.Insert(6);
		avltree.Insert(4);

		// root shouldn't be null
		ASSERT_TRUE(avltree.GetRoot());

		// insert 3 to make the tree unbalanced
		avltree.Insert(3);

		// check to make sure the left rotation worked. Tree should be:
	/*    5
	     / \
	    3   6
	   / \
      2   4
	*/
		ASSERT_EQ(5,avltree.GetRoot()->data);
		ASSERT_EQ(3,avltree.GetRoot()->left->data);
		ASSERT_EQ(6,avltree.GetRoot()->right->data);
		ASSERT_EQ(2,avltree.GetRoot()->left->left->data);
		ASSERT_EQ(4,avltree.GetRoot()->left->right->data);

		// Check for proper height for all nodes
		ASSERT_EQ(3,avltree.GetRoot()->height);
		ASSERT_EQ(2,avltree.GetRoot()->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->right->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->left->height);
		ASSERT_EQ(1,avltree.GetRoot()->left->right->height);
	}
}