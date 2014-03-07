#include "suffixTree.h"
#include <iostream>
#include <unordered_map> // Required c++11 support.
#include <string>
// Maximum input length
#define MAXLENGTH 1024

using namespace std;

// Buffer for input string.
string Input;
int inputLength;

// The initial tree contains one node i.e. root node so count = 1;
int Node::noOfNodes = 1;

// We need to maintain an array of nodes.
Node * nodeArray;
// Hash key for the edges would be the node from where edge is arising and the
// first character. 
// Note: This would always be unique due to a property of suffix tree which
// states that no two edges emerging from a node can have same character as it's
// label
struct Key {
    int nodeID;
    int asciiChar;
};

/*
 * Edges are being stored in a hash table for better access time.
 * I was planning to use map instead of unorderedMap, but since map keeps the
 * elements in ordered fashion, it's average case time complexity is logn.
 * On the other hand, unorderedmap has a average case constant time complexity.
 * Key would be an integer which would be a function of nodeID and asciiChar
 */
unordered_map <int, Edge> edgeHash;

/*
 * Insert an edge into the hash table
 */
void Edge::insert() {
    int key = returnHashKey(startNode, Input[startLabelIndex]);
   // edgeHash.insert(make_pair<int, Node>(key, this));
    edgeHash[key] = *this;
    cout << "inserted " << key <<" " << startNode << " " << Input[startLabelIndex] << endl; 
}
/*
 * Remove an edge from the hash table.
 */
void Edge::remove() {
     int key = returnHashKey(startNode, Input[startLabelIndex]);
     edgeHash.erase(key);
     cout << "Removed " << key << " " << startNode << " " << Input[startLabelIndex] << endl;
}

/*
 * Find an edge in the hash table corresponding to NODE & ASCIICHAR
 */
Edge Edge::findEdge(int node, int asciiChar) {
    int key = returnHashKey(node, asciiChar);
    unordered_map<int, Edge>::const_iterator search = edgeHash.find(key);
    if (search != edgeHash.end()) {
        cout << "Entry found for " << node << " " << asciiChar << endl;
        return edgeHash.at(key);
    }
    
    cout << "Entry NOT found for " << node << " " << asciiChar << endl;
    // Return an invalid edge if the entry is not found.
    return Edge();
}

/*
 * Main function which will carry out all the different phases of the Ukkonen's
 * algorithm. Through suffixTree we'll maintain the current position in the tree
 * and then add the prefix 0 -> lastIndex in the tree created in the previous
 * iteration.
 */
void carryPhase(suffixTree &tree, int lastIndex) {
    cout << "Phase " << lastIndex << " Adding " << Input.substr(0, lastIndex + 1) << endl;
}

int main () {
  cout << "Enter String" << endl;
  getline(cin, Input);
  inputLength = Input.length();

  // Allocating memory to the array of nodes.
  nodeArray = (Node *)malloc(inputLength*(sizeof (Node)));
  
  cout << "you entered " << Input  << " length " << inputLength << endl;
  
  // Creating initial suffixTree.
  suffixTree tree (0, 0, -1);
  // Carry out different phases.
  for (int i = 0; i < inputLength; i++)
      carryPhase(tree, i);
  cout << "Wait for some more time to see the tree." << endl;
  cout << "Seeds are being imported right now." << endl;
  cout << "Adios!" << endl;
  return 0;
}
