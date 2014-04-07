#include "suffixTree.h"
#include <iostream>
#include <unordered_map> // Required c++11 support.
#include <string>
#include <cassert>
#include <ctime>
#include <chrono>
using namespace std;

// TODO Add proper debug messages using macros.

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
unordered_map <long, Edge> edgeHash;

/*
 * Insert an edge into the hash table
 */
void Edge::insert() {
    long key = returnHashKey(startNode, Input[startLabelIndex]);
   // edgeHash.insert(make_pair<int, Node>(key, this));
    edgeHash[key] = *this;
  //  cout << "Inserted " << startNode << " " << endNode << " " << startLabelIndex  
 //       << " " << endLabelIndex <<  " " << key <<  endl; 
}
/*
 * Remove an edge from the hash table.
 */
void Edge::remove() {
     long key = returnHashKey(startNode, Input[startLabelIndex]);
     edgeHash.erase(key);
   //  cout << "Removed " << key << " " << startNode << " " << Input[startLabelIndex] << endl;
}

/*
 * Find an edge in the hash table corresponding to NODE & ASCIICHAR
 */
Edge Edge::findEdge(int node, int asciiChar) {
    long key = returnHashKey(node, asciiChar);
    unordered_map<long, Edge>::const_iterator search = edgeHash.find(key);
    if (search != edgeHash.end()) {
    //    cout << "Entry found for " << node << " " << asciiChar << endl;
        return edgeHash.at(key);
    }
    
  //  cout << "Entry NOT found for " << node << " " << asciiChar << endl;
    // Return an invalid edge if the entry is not found.
    return Edge();
}
 // rootNode should be equal to the closest node to the end of the tree so
 // tht this can be used in the next iteration.
void suffixTree::migrateToClosestParent() {
    // If the current suffix tree is ending on a node, this condition is already
    // met.
    if (endReal()) {
   //     cout << "Nothing needs to be done for migrating" << endl;
    }
    else {
        Edge e = Edge::findEdge(rootNode, Input[startIndex]);
        // Above will always return a valid edge as we call this method after
        // adding above.
        if(e.startNode == -1) {
            cout <<  rootNode << " " << startIndex << " " << Input[startIndex] << endl;
        }
        assert(e.startNode != -1);
        int labelLength = e.endLabelIndex - e.startLabelIndex;

        // Go down
        while (labelLength <= (endIndex - startIndex)) {
            startIndex += labelLength + 1;
            rootNode = e.endNode;
            if (startIndex <= endIndex) {
                e = Edge::findEdge(e.endNode, Input[startIndex]);
          if(e.startNode == -1) {
            cout <<  rootNode << " " << startIndex << " " << Input[startIndex] << endl;
        }
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
    return newEdge->endNode;
}
/*
 * Main function which will carry out all the different phases of the Ukkonen's
 * algorithm. Through suffixTree we'll maintain the current position in the tree
 * and then add the prefix 0 -> lastIndex in the tree created in the previous
 * iteration.
 */
void carryPhase(suffixTree &tree, int lastIndex) {
   // cout << "Phase " << lastIndex << " Adding " << Input.substr(0, lastIndex + 1) << endl;
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
            int diff = tree.endIndex - tree.startIndex;
            if (Input[e.startLabelIndex + diff + 1] == Input[lastIndex])
                // We have a match
                break;
            //If match was not found this way, then we need to break this edge
            // and add a node and insert the string.
      //      cout << " breaking edge " << endl; 
            parentNode = breakEdge(tree, e);
        }

        // We have not matchng edge at this point, so we need to create a new
        // one, add it to the tree at parentNode position and then insert it
        // into the hash table.
        //
        // We are creating a new node here, which means we also need to update
        // the suffix link here. Suffix link from the last visited node to the
        // newly created node.
      //  cout << "adding new edge" << endl;
        Edge *newEdge = new Edge(parentNode, lastIndex, inputLength);
        newEdge -> insert();
        if (previousParentNode > 0)
            nodeArray[previousParentNode].suffixNode = parentNode;
        previousParentNode = parentNode;

        // Move to next suffix, i.e. next extension.
        if (tree.rootNode == 0)
            tree.startIndex++;
        else {
            tree.rootNode = nodeArray[tree.rootNode].suffixNode;
      //      printf("using suffix link while adding %d %d\n",tree.rootNode, nodeArray[tree.rootNode].suffixNode);
        }
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
    // index until where we have matched on edge.
    int iter = 0;
    int i = -1;
    if (e.startNode != -1) {
        while(i < len) {
            cout << "Search:\tEdge: " << e.startNode << " " << e.endNode << " : " 
                << Input[e.startLabelIndex]  << " " << Input[e.endLabelIndex] << " I: " << i << endl;
            // Match the pattern on this edge.
            iter = 0;
            // Match this edge as much as possible.
            while (e.endLabelIndex >= e.startLabelIndex + iter)   
                    {
                        cout << "Search:\tmatching " << Input[e.startLabelIndex + iter] << " " 
                             << pattern[i + iter + 1]
                            << " at index: " << e.startLabelIndex + iter << endl;
                        // If character matches we increase the iterator
                        // otherwise we are done. No match.
                        if (Input[e.startLabelIndex + iter] == pattern[i + iter + 1]) { 
                            iter++;
                            // If we have a match in the middle then we are done.
                            if (i + iter  + 1 >= len) {
                                cout << "Search:\tWe have a match ending at " 
                                     << e.startLabelIndex + iter  - 1 << endl;
                                return true;
                            }
                        }
                        else {
                            cout << "Search:\tMatch not found, matched only upto index:" << i+iter << endl;
                            return false;     
                        }
                   }
            // We have done all possible matches on this edge. We can proceed
            // only if the entire label matches.
            assert(iter = (e.endLabelIndex - e.startLabelIndex + 1));

            // Now we need to find another edge to match.
            e = Edge::findEdge(e.endNode, pattern[i + iter + 1]);
            if (e.startNode == -1) {
                cout << "Search:\tMatch not found, matched only upto " << i + iter  
                     << " " << pattern[i + iter + 1]<< endl;
                return false;    
                }
            i+=(iter);
        }
    }
    cout << "Search:\tMatched :D " << iter << " " << pattern << endl;
    return true;
}

/*
 * This function prints all the edges in the suffix tree.
 */
void printAllEdges() {
    int count = 0;
    cout << "StartNode\tEndNode\tSuffixLink\tFirstIndex\tlastIndex\tString" << endl;
    // For auto : C++11 FTW :)
    for (auto it = edgeHash.begin(); it != edgeHash.end(); it++) {
        cout << it -> second.startNode << "\t\t" << it -> second.endNode 
            << "\t\t" << nodeArray[it -> second.endNode].suffixNode
            << "\t\t" << it -> second.startLabelIndex 
            << "\t\t" << it -> second.endLabelIndex
            << "\t\t";
        count++;
        int head;
        if (inputLength > it -> second.endLabelIndex)
            head = it -> second.endLabelIndex;
        else 
            head = inputLength;
        for (int i = it -> second.startLabelIndex; i < head + 1; i++)
            cout << Input[i];
        cout << endl;
    }
    cout << "Total edges: " << count << endl;
}


int main () {
  cout << "Enter String" << endl;
  getline(cin, Input);
  // For aligning indices
  inputLength = Input.length() - 1;

  // Allocating memory to the array of nodes.
  nodeArray = (Node *)malloc(2*inputLength*(sizeof (Node)));
//  cout << "you entered " << Input  << " length " << inputLength << endl;

  // Start timer.
  // Creating initial suffixTree.
  auto start = std::chrono::high_resolution_clock::now();
  suffixTree tree (0, 0, -1);
  // Carry out different phases.
  for (int i = 0; i <= inputLength; i++)
      carryPhase(tree, i);

  auto end = std::chrono::high_resolution_clock::now();
  printAllEdges();
  cout << "Total time taken to build suffix tree of length " << inputLength <<" : " 
       << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
       << "ms"<< endl;

  cout << "Enter patterns or enter \"exit\" to exit." << endl;
  string pattern;
  getline(cin, pattern);
  while (pattern.compare("exit")) {
    search(pattern);
    cout << "----------------------------------------------------" << endl;
    cout << "Enter pattern or enter \"exit\" to exit." << endl;
    getline(cin, pattern);
  }
  cout << "Wait for some more time to see the tree." << endl;
  cout << "Seeds are being imported right now." << endl;
  cout << "Adios!" << endl;
  return 0;
}
