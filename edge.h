
#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include "node.h"
using namespace std;

class Edge {
public:
    // Edges are hash-searched on the basis of startNode.
    // startNode = -1 means that this edge is not valid yet.
    int startNode;
    int endNode;
    int startLabelIndex;
    int endLabelIndex;
    void printID ();
    void insert ();
    void remove ();
    // node is the starting node and c is the ASCII input char.
    // Static because I want to call it without using an instantiated object.
    static long returnHashKey(int node, int c);
    static Edge findEdge(int node, int c);
    
    // Constructors.
    Edge () : startNode(-1) {};
    // everytime a new edge is created, a new node is also created and thus the
    // endNode is declared as below.
    Edge (int start, int first, int last) :
        startNode (start),
        endNode (Node::noOfNodes++),
        startLabelIndex (first),
        endLabelIndex (last) {};
    // Destructor
    ~Edge() {
      //  cout << "destroying edge " << startNode << " " << endNode << endl;
    }        

};        

#endif                    
