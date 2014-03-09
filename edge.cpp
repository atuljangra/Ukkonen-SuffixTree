#include "edge.h"
using namespace std;

void Edge::printID () {
}

/*
 * We need to differentiate the role of c and nodeID,
 * thus multiplying a prime number.
 */
int Edge::returnHashKey(int nodeID, int c) {
    return (nodeID + 
            (c << 23));
}

