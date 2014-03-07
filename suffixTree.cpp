#include "suffixTree.h"
#include <iostream>

// Maximum input length
#define MAXLENGTH 1024
// More than twice of the length of input
#define HASHLENGTH 2549

// Buffer for input string.
char Input[MAXLENGTH];

// TODO Use unorderedMap as a hash function.

/*
 * Edges are being stored in a hash table for better access time.
 */

using namespace std;
int main () {
  cout << "Enter String" << endl;
  cin.getline(Input, MAXLENGTH - 1);
  cout << "you entered " << Input << endl;
  cout << "Wait for some more time to see the tree." << endl;
  cout << "Seeds are being imported right now." << endl;
  cout << "Adios!" << endl;
  return 0;
}
