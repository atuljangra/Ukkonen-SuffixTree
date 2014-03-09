#include "suffixTree.h"
#include <iostream>
#include <unordered_map> // Required c++11 support.
#include <string>
#include <cassert>

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
 // rootNode should be equal to the closest node to the end of the tree so
 // tht this can be used in the next iteration.
void suffixTree::migrateToClosestParent() {
    // If the current suffix tree is ending on a node, this condition is already
    // met.
    if (endReal()) {
        cout << "Nothing needs to be done for migrating" << endl;
    }
    else {
        Edge e = Edge::findEdge(rootNode, Input[startIndex]);
        // Above will always return a valid edge as we call this method after
        // adding above.
        assert(e.startNode != -1);
        int labelLength = e.endLabelIndex - e.startLabelIndex;

        // Go down
        while (labelLength <= (endIndex - startIndex)) {
            startIndex += labelLength + 1;
            rootNode = e.endNode;
            if (startIndex <= endIndex) {
                e = Edge::findEdge(e.endNode, Input[startIndex]);
                assert(e.startNode != -1);
                labelLength = e.endLabelIndex - e.startLabelIndex;
            }
        }
        
    }
}

/*
 * Break an edge so as to add new string at a specific point.
 */
int breakEdge(suffixTree &s, Edge &e) {
    // Remove the edge 
    e.remove();

    Edge *newEdge = new Edge(s.rootNode, e.startLabelIndex, 
                             e.startLabelIndex + s.endIndex - s.startIndex);
    newEdge -> insert();
    // Add the suffix link for the new node.
    nodeArray[newEdge -> endNode].suffixNode = s.rootNode;
    e.startLabelIndex += s.endIndex - s.startIndex + 1;
    e.startNode = newEdge -> endNode;
    e.insert();
    return -1;
}
/*
 * Main function which will carry out all the different phases of the Ukkonen's
 * algorithm. Through suffixTree we'll maintain the current position in the tree
 * and then add the prefix 0 -> lastIndex in the tree created in the previous
 * iteration.
 */
void carryPhase(suffixTree &tree, int lastIndex) {
    cout << "Phase " << lastIndex << " Adding " << Input.substr(0, lastIndex + 1) << endl;
    int parentNode;
    // to keep track of the last encountered node.
    // Used for creating the suffix link.
    int previousParentNode = -1;
    while (true) {
        // First we try to match an edge for this, if there is one edge and all
        // other subsequent suffixs would already be there.
        Edge e;
        parentNode = tree.rootNode;

        if (tree.endReal() ) {
            e = Edge::findEdge(tree.rootNode, Input[lastIndex]);
            if (e.startNode != -1)
                break;
        }
        // If previoustree ends in between an edge, then we need to find that
        // edge and match after that. 
        else {
            e = Edge::findEdge(tree.rootNode, Input[tree.startIndex]);
            int length = tree.endIndex - tree.startIndex;
            if (Input[e.startLabelIndex + length + 1] == Input[lastIndex])
                // We have a match
                break;
            //If match was not found this way, then we need to break this edge
            // and add a node and insert the string.
            parentNode = breakEdge(tree, e);
        }

        // We have not matchng edge at this point, so we need to create a new
        // one, add it to the tree at parentNode position and then insert it
        // into the hash table.
        //
        // We are creating a new node here, which means we also need to update
        // the suffix link here. Suffix link from the last visited node to the
        // newly created node.
        Edge *newEdge = new Edge(parentNode, lastIndex, inputLength);
        newEdge -> insert();
        if (previousParentNode > 0)
            nodeArray[previousParentNode].suffixNode = parentNode;
        previousParentNode = parentNode;

        // Move to next suffix, i.e. next extension.
        if (tree.rootNode == 0)
            tree.startIndex++;
        else
            tree.rootNode = nodeArray[tree.rootNode].suffixNode;
        tree.migrateToClosestParent();
    }

    if (previousParentNode > 0)
        nodeArray[previousParentNode].suffixNode = parentNode;
    tree.endIndex++;
    tree.migrateToClosestParent();
}

bool search(string pattern) {
    int len = pattern.length();
    // Starting from 0 we start searching the pattern.
    Edge e = Edge::findEdge(0, pattern[0]);
    int iter = 0;
    int i = 0;
    if (e.startNode != -1) {
        while(i < len) {
            cout << "Edge: " << e.startNode << " " << e.endNode << " : " 
                << Input[e.startLabelIndex]  << " " << Input[e.endLabelIndex] << " I: " << i << endl;
        // Match the pattern on this edge.
        iter = 0;
        while ((Input[e.startLabelIndex + iter] == pattern[i + iter]) && 
               (e.endLabelIndex >= e.startLabelIndex + iter))   
                {
            cout << "matching " << Input[e.startLabelIndex + iter] << " " << pattern[iter]  << 
                " at index: " << e.startLabelIndex + iter << endl;
            iter++;
            if (i + iter >= len) {
                cout << "We have a match ending at " << e.startLabelIndex + iter  - 1 << endl;
                break;
            }
        }
        // Now we need to find another edge to match.
        e = Edge::findEdge(e.endNode, pattern[iter]);
        if (e.startNode == -1) {
            cout << "No more matches " << iter << endl;
            break;    
        }
            i+=iter;
        }

    }
    cout << "Match " << iter << " " << pattern << endl;
    return true;
}
void printAllEdges() {
    cout << "StartNode\tEndNode\tSuffixLink\tFirstIndex\tlastIndex\tString" << endl;
    // For auto : C++11 FTW :)
    for (auto it = edgeHash.begin(); it != edgeHash.end(); ++it) {
        cout << it -> second.startNode << "\t\t" << it -> second.endNode 
            << "\t\t" << nodeArray[it -> second.endNode].suffixNode
            << "\t\t" << it -> second.startLabelIndex 
            << "\t\t" << it -> second.endLabelIndex
            << "\t\t";

        int head;
        if (inputLength > it -> second.endLabelIndex)
            head = it -> second.endLabelIndex;
        else 
            head = inputLength;
        for (int i = it -> second.startLabelIndex; i < head + 1; i++)
            cout << Input[i];
        cout << endl;
    }
}
int main () {
  cout << "Enter String" << endl;
  getline(cin, Input);
  // For aligning indices
  inputLength = Input.length() - 1;

  // Allocating memory to the array of nodes.
  nodeArray = (Node *)malloc(inputLength*(sizeof (Node)));
  
  cout << "you entered " << Input  << " length " << inputLength << endl;
  
  // Creating initial suffixTree.
  suffixTree tree (0, 0, -1);
  // Carry out different phases.
  for (int i = 0; i <= inputLength; i++)
      carryPhase(tree, i);

  printAllEdges();

  string pattern;
  cout << "Enter pattern" << endl;
  getline(cin, pattern);
  search(pattern);
  cout << "Wait for some more time to see the tree." << endl;
  cout << "Seeds are being imported right now." << endl;
  cout << "Adios!" << endl;
  return 0;
}
