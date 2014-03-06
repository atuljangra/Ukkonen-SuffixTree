
#ifndef EDGE_H
#define EDGE_H

#include <iostream>

using namespace std;
class edge {
private:
    int id;
public:
    void printID ();
    edge() {
        this -> id = 0;
    }
    edge (int id) {
        this -> id = id;
    }
    ~edge() {
        cout << "destroying edge " << id << endl;
    }        
};        

#endif                    
