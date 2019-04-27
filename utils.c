/**
 *  Created by Sotirios-Efstathios Maneas on 2017-01-02.
 *  Modified by Ibrahim Jomaa on 2017-06-28.
 **/

#include <stdio.h>
#include <string.h>

#include "utils.h"

// Symbol for INSERT OPERATION: Add new image to database
#define INSERT "i"
// Symbol for QUERY OPERATION: Search for image in database
#define QUERY "q"
// Symbol for PRINT OPERATION: Output all images in database
#define PRINT "p"

/**
 *  Tokenize the string stored in cmd based on DELIMITERS as separators.
 * 
 *  @param cmd The user input to analyze.
 *  @param cmd_argv Holds the valid parsed user input.
 *  @return The number of tokens that were parsed. Return -1 instead if user
 *  input was invalid.
 **/
int tokenize(char *cmd, char **cmd_argv) {
  // Holds the number of parsed tokens in a valid command. Assume we are given
  // an invalid command
  int num_parsed = -1;
  // Parse the 1st token
  char *some_token = strtok(cmd, DELIMITERS);
  // If parsing did not fail
  if (some_token != NULL) {
    // If the token represents a valid operation [Either: "i"; "q"; "p"]
    if (((some_token[0] == INSERT[0]) || (some_token[0] == QUERY[0]) || 
	(some_token[0] == PRINT[0])) && ((some_token[1] == '\0') ||
					 (some_token[1] == '\n'))) {
      // Holds the number of tokens the operation must have
      int num_tokens = 0;
      // If the token is an INSERT OPERATION
      if (some_token[0] == INSERT[0]) {
	// The command should have only 5 tokens
	num_tokens = 5;
      }
      // Else, if the token is a QUERY OPERATION
      else if (some_token[0] == QUERY[0]) {
	// The command should have only 4 tokens
	num_tokens = 4;
      }
      // Else, the token is a PRINT OPERATION
      else {
	// The command should have only 1 token
	num_tokens = 1;
      }
      // Reset the number of tokens parsed
      num_parsed = 0;
      // Keep parsing tokens until the required amount has been met [OR] until 
      // there are no more tokens left to be parsed from the command
      for ( ; (num_parsed < num_tokens) && (some_token != NULL); num_parsed++)
	{
	  // Add current token to our valid tokens
	  cmd_argv[num_parsed] = some_token;
	  // Parse the next token
	  some_token = strtok(NULL, DELIMITERS);
	}
      // If the number of tokens parsed does not equal the expected amount of
      // tokens to be parsed
      if (num_parsed != num_tokens) {
	// Void the number of tokens parsed
	num_parsed = -1;
      }
      // Else, if the last parsed token exists
      else if (some_token != NULL) {
	  // Void the number of tokens parsed
	  num_parsed = -1;
      }
    }
  }
  // Returns the number of valid tokens parsed from command
  // Returns -1 if command was invalid
  return num_parsed;
}
