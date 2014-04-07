#include "edge.h"
using namespace std;

void Edge::printID () {
}

/*
 * Using Contor pairing function which is basically a bijective mapping from N*N
 * -> N.
 */
long Edge::returnHashKey(int nodeID, int c) {
    // long contor = (1/2)*(nodeID + c)*(nodeID + c + 1) + c;
    // return contor;
    return (long)(nodeID + 
                  (((long)c) << 59));
}

