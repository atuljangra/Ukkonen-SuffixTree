#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

class node {
private:
    int id;
public:
    node() {
        this -> id = 0;
    }
    node (int id) {
        this -> id = id;
    }
    ~node() {
        cout << "destroying node " << id << endl;

    }
    void printID ();
};
#endif
