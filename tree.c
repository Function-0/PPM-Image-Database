/**
 *  Created by Sotirios-Efstathios Maneas on 2017-01-02.
 *  Modified by Ibrahim Jomaa on 2017-06-28.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/**
 *  A helper function that allocates a new tree node.
 *
 *  @param value The entry's value. It represents either an attribute or a 
 *  filename.
 *  @return A pointer to the newly allocated node.
 **/
struct TreeNode *allocate_node(const char *value) {
  // Allocate memory on Heap for a new node
  struct TreeNode *result = malloc(sizeof(struct TreeNode));
  // Initialize node values:
  // Allocate memory on Heap for new cargo
  result->value = malloc((sizeof(char) * strlen(value)) + 1);
  // Insert the new cargo
  strcpy(result->value, value);
  // Points to next node in same depth level
  result->sibling = NULL;
  // Points to next node in next depth level
  result->child = NULL;
  // Returns a reference to the new node
  return result;
}

/**
 *  A helper function that inserts new node branches into the database.
 *
 *  @param root The root node of the database.
 *  @param values The cargo to be placed in the new nodes. May hold:
 *  Attribute 1 (A1); Attribute 2 (A2); Attribute 3 (A3); Filename.
 *  @param num_children The number of new nodes to be inserted.
 **/
void helper_tree_insert_children(struct TreeNode *root, char **values, 
				 int num_children) {
  // If 4 children are required 
  // (i.e., root node is main root node of database)
  if (num_children == 4) {
    // Create filename node
    struct TreeNode *filename = allocate_node(values[4]);
    // Create Attribute 3 (A3) node
    struct TreeNode *attribute_3 = allocate_node(values[3]);
    // Connect A3 to filename on next depth level
    attribute_3->child = filename;
    // Create Attribute 2 (A2) node
    struct TreeNode *attribute_2 = allocate_node(values[2]);
    // Connect A2 to A3 on next depth level
    attribute_2->child = attribute_3;
    // Create Attribute 1 (A1) node
    struct TreeNode *attribute_1 = allocate_node(values[1]);
    // Connect A1 to A2 on next depth level
    attribute_1->child = attribute_2;
    // Connect Root to A1 on next depth level
    root->child = attribute_1;
  }
  // Else, if 3 children are required 
  // (i.e., root node is Attribute 1 (A1) node)
  else if (num_children == 3) {
    // Create filename node
    struct TreeNode *filename = allocate_node(values[4]);
    // Create Attribute 3 (A3) node
    struct TreeNode *attribute_3 = allocate_node(values[3]);
    // Connect A3 to filename on next depth level
    attribute_3->child = filename;
    // Create Attribute 2 (A2) node
    struct TreeNode *attribute_2 = allocate_node(values[2]);
    // Connect A2 to A3 on next depth level
    attribute_2->child = attribute_3;
    // Connect A1 to A2 on next depth level
    root->child = attribute_2;
  }
  // Else, if 2 children are required 
  // (i.e., root node is Attribute 2 (A2) node)
  else if (num_children == 2) {
    // Create filename node
    struct TreeNode *filename = allocate_node(values[4]);
    // Create Attribute 3 (A3) node
    struct TreeNode *attribute_3 = allocate_node(values[3]);
    // Connect A3 to filename on next depth level
    attribute_3->child = filename;
    // Connect A2 to A3 on next depth level
    root->child = attribute_3;
  }
  // Else, 1 child is required (i.e., root node is Attribute 3 (A3) node)
  else {
    // Create filename node
    struct TreeNode *filename = allocate_node(values[4]);
    // Connect A3 to filename on next depth level
    root->child = filename;
  }
}

/**
 *  A helper function that inserts a new sibling node into the database.
 *
 *  @param root The root node of the database.
 *  @param value The cargo to be placed in the new node. May hold:
 *  Attribute 1 (A1); Attribute 2 (A2); Attribute 3 (A3); Filename.
 *  @param is_new_sibling Used to determine if a new sibling node was 
 *  inserted.
 *  @return A pointer to the new sibling node.
 **/
struct TreeNode *helper_tree_insert_sibling(struct TreeNode *root,
					    char *value, 
					    int *is_new_sibling) {
  // Holds the node to be returned
  struct TreeNode *result;
  // If this node matches the desired cargo
  if (strcmp(root->value, value) == 0) {
    // Our result is the given node. This indicates that a new sibling node
    // was not required to be inserted
    result = root;
  }
  // Else, move on to the next sibling node
  else {
    struct TreeNode *next_node = root->sibling;
    // If this node exists
    if (next_node != NULL) {
      // Recursive call: Attempt to insert a new sibling node at current node.
      // The new root node is the current sibling node
      result = helper_tree_insert_sibling(next_node, value, is_new_sibling);
    }
    // Else, create a new node with our specified cargo
    else {
      struct TreeNode *new_node = allocate_node(value);
      // Connect the previous node with our new sibling node
      root->sibling = new_node;
      // Our result is the new sibling node
      result = new_node;
      // Indicates that a new sibling node was inserted
      *is_new_sibling = 1;
    }
  }
  // Returns the new sibling node
  return result;
}

/**
 *  A helper function that sorts sibling nodes at of a certain depth level.
 *
 *  @param root The starting sibling node at of a certain depth level.
 *  @param is_swap_occured Used to determine if any nodes were swapped.
 *  @param main_root Node reference that points to next depth level [based on
 *  current sibling node depth level], where said next depth level has 
 *  multiple sibling nodes.
 *  @param prev_node Node reference to the previous node that connects to 
 *  aforementioned parameter root node.
 *  @return A pointer to the next sibling node at same depth level.
 **/
struct TreeNode *helper_sibling_node_sort(struct TreeNode *root,
					  int *is_swap_occured,
					  struct TreeNode *main_root,
					  struct TreeNode *prev_node) {
  // Holds the node to be returned. Default return node is the initial root
  // node
  struct TreeNode *result = root;
  // If sibling node exists
  if (root->sibling != NULL) {
    // Obtain sibling node
    struct TreeNode *main_sibling = root->sibling;
    // If root node's value is larger than sibling node's value
    if (strcmp(root->value, main_sibling->value) > 0) {
      // Swap the node values (i.e., swap the node references):
      // Saves the next sibling node's reference
      struct TreeNode *temp_node = main_sibling->sibling;
      // Have the current sibling node point to current root node
      main_sibling->sibling = root;
      // Have the current root node point to the next sibling node
      root->sibling = temp_node;
      // If the either of the nodes that were swapped was connected by the 
      // main node that oversees the rest of the current sibling nodes 
      // (i.e., the main node connects to the set of current sibling nodes)
      if (prev_node == NULL) {
	// Change the main node's connection to the swapped node
	main_root->child = main_sibling;
      }
      // Else, have the previous node's sibling (of the current root node) 
      // connect to the swapped node
      else {
	prev_node->sibling = main_sibling;
      }
      // Indicates that a node swap has occured
      *is_swap_occured = 1;
    }
    // If the next sibling node exists
    if (main_sibling->sibling != NULL) {
      // If our previous node was void
      if (prev_node == NULL) {
	// Have our previous node point to the current root node
	prev_node = root;
      }
      // Else, have our previous node point to the current sibling node
      else {
	prev_node = prev_node->sibling;
      }
      // Recursive call: Attempt to sort the next set of sibling nodes:
      // The new current root node is the current sibling node
      result = helper_sibling_node_sort(main_sibling, is_swap_occured,
					main_root, prev_node);
    }
  }
  // Returns the next node to be sorted
  return result;
}

/**
 *  Sorts sibling nodes at of a certain depth level. Follows the bubble sort
 *  method.
 *
 *  @param root The starting sibling node at of a certain depth level.
 *  @param main_root Node reference that points to next depth level [based on
 *  current sibling node depth level], where said next depth level has 
 *  multiple sibling nodes.
 **/
void sibling_node_sort(struct TreeNode *root,
		       struct TreeNode *main_root) {
  // Indicates if a swap has occured. Assume no swap occured
  int is_swap_occured = 0;
  // Attempt to sort the sibling nodes:
  // Our previous node is void
  helper_sibling_node_sort(root, &is_swap_occured, main_root, NULL);
  // Keep sorting the sibling nodes until no swap has occured. No swap 
  // occuring indicates the sibling nodes have been sorted
  while (is_swap_occured) {
    // Reset the swap detector
    is_swap_occured = 0;
    // Attempt to sort the sibling nodes:
    // Our previous node is void
    helper_sibling_node_sort(root, &is_swap_occured, main_root, NULL);
  }
}

/**
 *  Sorts all nodes in the database.
 *
 *  @param root The starting sibling node at the 1st depth level 
 *  [Attribute 1 (A1)].
 *  @param main_root The root node of the database. 
 *  @return A pointer to the next node set to be sorted.
 **/
struct TreeNode *database_sort(struct TreeNode *root, 
			       struct TreeNode *main_root) {
  // Holds the node to be returned. Default return node is the initial root
  // node
  struct TreeNode *result = root;
  // If sibling node exists
  if (root->sibling != NULL) {
    // Sort the sibling nodes
    sibling_node_sort(root, main_root);
    // If sibling node still exists [Which it may not after a node swap]
    if (root->sibling != NULL) {
      // Recursive call: Sort the next set of sibling nodes:
      // The new current root node is the current sibling node
      result = database_sort(root->sibling, main_root);
    }
  }
  // If child node exists
  if (root->child != NULL) {
    // The new main root is the current root
    main_root = root;
    // Recursive call: Sort the next set of sibling nodes:
    // The new current root node is the root node's child
    result = database_sort(root->child, main_root);
  }
  // Returns the next node set to be sorted.
  return result;
}

/**
 *  Insert a new image to a tree
 *
 *  @param tree A pointer to the root of the tree.
 *  @param values An array, whose first three members are the attribute 
 *  values for the image and the last one is the filename
 **/
void tree_insert(struct TreeNode *root, char **values) {
  // If root node is empty
  if (root->child == NULL) {
    // We must add 4 new nodes into the database [Attributes 1-3; filename]
    helper_tree_insert_children(root, values, 4);
  }
  // Else, move on to next node depth level
  else {
    // Indicates if a new sibling was inserted. Assume no sibling was inserted
    int is_new_sibling = 0;
    // Holds the node used for transfers to the next node depth level
    struct TreeNode *main_transfer_node = root->child;
    // Insert Attribute 1 (A1) sibling node
    struct TreeNode *attribute_1;
    attribute_1 = helper_tree_insert_sibling(main_transfer_node,
					     values[1], &is_new_sibling);
    // If a new sibling was inserted
    if (is_new_sibling == 1) {
      // We require 3 new children in the database [Attributes 2-3; filename]
      helper_tree_insert_children(attribute_1, values, 3);
    }
    // Else, move on to the next node depth level 
    // (Dupicate A1 was present in database)
    else {
      main_transfer_node = attribute_1->child;
      // Insert Attribute 2 (A2) sibling node
      struct TreeNode *attribute_2;
      attribute_2 = helper_tree_insert_sibling(main_transfer_node,
					       values[2], &is_new_sibling);
      // If a new sibling was inserted
      if (is_new_sibling == 1) {
	// We require 2 new children in the database [Attribute 3; filename]
	helper_tree_insert_children(attribute_2, values, 2);
      }
      // Else, move on to the next node depth level 
      // (Dupicate A2 was present in database)
      else {
	main_transfer_node = attribute_2->child;
	// Insert Attribute 3 (A3) sibling node
	struct TreeNode *attribute_3;
	attribute_3 = helper_tree_insert_sibling(main_transfer_node,
						 values[3], &is_new_sibling);
	// If a new sibling was inserted
	if (is_new_sibling == 1) {
	  // We require 1 new child in the database [filename]
	  helper_tree_insert_children(attribute_3, values, 1);
	}
	// Else, move on to the next node depth level
	// (Dupicate A3 was present in database)
	else {
	  main_transfer_node = attribute_3->child;
	  // Insert filename sibling node
	  helper_tree_insert_sibling(main_transfer_node, values[4], 
				     &is_new_sibling);
	  // If no new sibling was inserted, duplicate filename was present
	  // in database
	}
      }
    }
    // Sort the modified database:
    // Starting root is the main database root's child [Attribute 1 (A1)]
    database_sort(root->child, root);
  }
}

/**
 *  Searches a tree to print all files with matching attribute values.
 *
 *  @param tree A pointer to the root of the tree.
 *  @param values An array of attribute values
 **/
void tree_search(const struct TreeNode *root, char **values) {
  // Node detector. Assume that we have not yet found our required value
  int is_node_found = 0;
  // If database is not empty
  if (root->child != NULL) {
    // Move on to depth level 1 nodes (Attribute 1) A1 
    root = root->child;
    // Keep going through sibling nodes until we find the required A1 [OR] 
    // until we hit NULL
    while ((root != NULL) && (!is_node_found)) {
      // If this is the node we are looking for
      if (strcmp(root->value, values[1]) == 0) {
	// Node search is complete
	is_node_found = 1;
      }
      // If we have not yet found our node
      if (!is_node_found) {
	// Move on to the next sibling node
      root = root->sibling;
      }
    }
    // If we found the required node
    if (is_node_found) {
      // Reset our node detector
      is_node_found = 0;
      // Move on to depth level 2 nodes (Attribute 2) A2
      root = root->child;
      // Keep going through sibling nodes until we find the required A2 [OR]
      // until we hit NULL
      while ((root != NULL) && (!is_node_found)) {
	// If this is the node we are looking for
	if (strcmp(root->value, values[2]) == 0) {
	  // Node search is complete
	  is_node_found = 1;
	}
	// If we have not yet found our node
	if (!is_node_found) {
	  // Move on to the next sibling node
	  root = root->sibling;
	}
      }
      // If we found the required node
      if (is_node_found) {
	// Reset our node detector
	is_node_found = 0;
	// Move on to depth level 3 nodes (Attribute 3) A3
	root = root->child;
	// Keep going through sibling nodes until we find the required A3 [OR]
	// until we hit NULL
	while ((root != NULL) && (!is_node_found)) {
	  // If this is the node we are looking for
	  if (strcmp(root->value, values[3]) == 0) {
	    // Node search is complete
	    is_node_found = 1;
	  }
	  // If we have not yet found our node
	  if (!is_node_found) {
	    // Move on to the next sibling node
	    root = root->sibling;
	  }
	}
	// If we found the required node
	if (is_node_found) {
	  // Move on to depth level 4 nodes (filename)
	  root = root->child;
	  // Keep going through sibling nodes until we hit NULL
	  while (root != NULL) {
	    // If next sibling node does not exists
	    if (root->sibling == NULL) {
	      // Output current filename without trailing space
	      printf("%s", root->value);
	    }
	    // Else, output current filename with trailing space
	    else {
	      printf("%s ", root->value);
	    }
	    // Move on to the next sibling node
	    root = root->sibling;
	  }
	  // Add newline character at end of output
	  printf("\n");
	}
      }
    }
  }
  // If the node detector failed (i.e., no such info exists in database)
  if (!is_node_found) {
    // Output NULL
    printf("(NULL)\n");
  }
}

/**
 *  Helper function to print all info in database, in sorted order.
 *
 *  @param tree The root node of the database.
 *  @param values Holds the required info to output. May hold:
 *  Attribute 1 (A1); Attribute 2 (A2); Attribute 3 (A3); Filename.
 *  @param depth_level Indicates the current depth level in the database tree.
 *  @return The next node to be checked for cargo.
 **/
struct TreeNode *helper_tree_print(struct TreeNode *tree, char **values, 
				   int depth_level) {
  // Holds the node to be returned
  struct TreeNode *result;
  // Store current node cargo based on current depth level
  values[depth_level] = tree->value;
  // If a child node exists
  if (tree->child != NULL) {
    // Recursive call: Obtain the next node cargo:
    // We move on to the next child node and increment the depth level 
    depth_level++;
    result = helper_tree_print(tree->child, values, depth_level);
  }
  // If a child node does not exist
  if (tree->child == NULL) {
    // Output all stored cargo values
    printf("%s %s %s %s\n", values[0], values[1], values[2], values[3]);
  }
  // If a sibling node exists
  if (tree->sibling != NULL) {
    // If a child node exists
    if (tree->child != NULL) {
      // Revert the previous depth level change
      depth_level--;
    }
    // Recursive call: Obtain the next node cargo:
    // We move on to the next sibling node and keep the current depth level
    result = helper_tree_print(tree->sibling, values, depth_level);
  }
  // Returns the next node to be checked for cargo
  return result;
}


/**
 *  Prints a complete tree to the standard output.
 *
 *  @param tree A pointer to the root of the tree.
 **/
void tree_print(const struct TreeNode *tree) {
  // If database is empty
  if (tree->child == NULL) {
    // Output NULL
    printf("(NULL)\n");
  }
  // Else, output all info found in the database
  else {
    // Holds node cargo to be used for printing the output
    char *values[4];
    helper_tree_print(tree->child, values, 0);
  }
}
