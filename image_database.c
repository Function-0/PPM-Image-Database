/**
 *  Created by Sotirios-Efstathios Maneas on 2017-01-02.
 *  Modified by Ibrahim Jomaa on 2017-06-28.
 **/

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"

// Symbol for INSERT OPERATION: Add new image to database
#define INSERT 5
// Symbol for QUERY OPERATION: Search for image in database
#define QUERY 4
// Symbol for PRINT OPERATION: Output all images in database
#define PRINT 1
// Error message
#define ERROR_MSG "Invalid command.\n"

/**
 *  Based on user input, either: Insert an image into the database (INSERT);
 *  Output all image filenames matching specified attributes (QUERY); 
 *  Output all image filenames with their respective attributes found in the
 *  database (PRINT). Program ends when EOF (Ctrl-D) is entered.
 * 
 *  ===========================================================================
 *  NOTE THE FOLLOWING: 
 *  ===========================================================================
 *  INPUT SYNTAX:
 *  INSERT: i <ATTRIBUTE 1> <ATTRIBUTE 2> <ATTRIBUTE 3> <FILENAME>
 *  QUERY: q <ATTRIBUTE 1> <ATTRIBUTE 2> <ATTRIBUTE 3>
 *  PRINT: p
 *  ===========================================================================
 *  OUTPUT SYNTAX:
 *  QUERY: <FILENAME 1> <FILENAME 2> ... <FILENAME n>, where n is the number of
 *  image filenames that match the specified attributes.
 *  PRINT: LINE 1: <ATTRIBUTE 1> <ATTRIBUTE 2> <ATTRIBUTE 3> <FILENAME 1>
 *         LINE 2: <ATTRIBUTE 1> <ATTRIBUTE 2> <ATTRIBUTE 3> <FILENAME 2>
 *         LINE n: <ATTRIBUTE 1> <ATTRIBUTE 2> <ATTRIBUTE 3> <FILENAME n>
 *  ===========================================================================
 *  > If user input is invalid, outputs "Invalid command."
 *  > If there are no images in the database that match the specified
 *  attributes, QUERY outputs (NULL)
 *  > If there are no images in the database, PRINT outputs (NULL)
 *  > PRINT outputs filename info in alphabetical order
 **/
int main(void) {
        // char array to hold a line of input
	char buf[BUFFER_SIZE] = {'\0'};
        // char* array to hold the pointers to tokens
	char *args[INPUT_ARG_MAX_NUM];
        // the root of the tree
        struct TreeNode root;
        root.value = "";
        root.child = NULL;
        root.sibling = NULL;
	// Reference to the root of the tree
	struct TreeNode *root_ptr = &root;
        // Holds the number of tokens from valid user input
	int num_tokens;
	// Obtain 1st user input
	char *is_NULL = fgets(buf, BUFFER_SIZE, stdin);  
	// Keep receiving user input until EOF (Ctrl-D) is met
	while (is_NULL != NULL) {
	  // Parse the input
	  num_tokens = tokenize(buf, args);
	  // If we have an INSERT OPERATION
	  if (num_tokens == INSERT) {
	    // Call the insert function
	    tree_insert(root_ptr, args);
	  }
	  // Else, if we have a QUERY OPERATION
	  else if (num_tokens == QUERY) {
	    // Call the query function
	    tree_search(root_ptr, args);
	  }
	  // Else, if we have a PRINT OPERATION
	  else if (num_tokens == PRINT) {
	    // Call the print function
	    tree_print(root_ptr);
	  }
	  // Else, the input must be invalid
	  else {
	    // Output an error message
	    fprintf(stderr, ERROR_MSG);
	  }
	  // Receive the next user input
	  is_NULL = fgets(buf, BUFFER_SIZE, stdin);
        }
	return 0;
}
