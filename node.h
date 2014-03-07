#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

class node {
private:
    int id;
public:
    int suffixNode;
    node () : 
        id(0),
        suffixNode(-1) {};
    node (int nid, int suffixLink):
        id(nid),
        suffixNode(suffixLink) {};
    int getID ();
    ~node() {
        cout << "destroying node " << id << endl;

    }
};
#endif
