#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

class Node {
public:
    int id;
    static int noOfNodes;
    int suffixNode;
    
    Node () : 
        id(0),
        suffixNode(-1) {};
    
    int getID ();
    
    ~Node() {
       //  cout << "destroying node " << id << endl;

    }
};
#endif
