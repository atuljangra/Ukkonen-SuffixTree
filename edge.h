
#ifndef EDGE_H
#define EDGE_H

#include <iostream>

using namespace std;

class edge {
private:
    // Edges are hash-searched on the basis of startNode.
    // startNode = -1 means that this edge is not valid yet.
    int startNode;
    int endNode;
    int startLabelIndex;
    int endLabelIndex;
public:
    void printID ();
    void insert ();
    void remove ();
    // node is the starting node and c is the ASCII input char.
    // Static because I want to call it without using an instantiated object.
    static int returnHashKey(int node, int c);
    static edge findEdge(int node, int c);
    
    // Constructors.
    edge () : startNode(-1) {};
    edge (int start, int end, int first, int last) :
        startNode (start),
        endNode (end),
        startLabelIndex (first),
        endLabelIndex (last) {};
    // Destructor
    ~edge() {
        cout << "destroying edge " << startNode << " " << endNode << endl;
    }        

};        

#endif                    
