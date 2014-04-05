#include "edge.h"
using namespace std;

void Edge::printID () {
}

/*
 * We need to differentiate the role of c and nodeID,
 * thus multiplying a prime number.
 */
long Edge::returnHashKey(int nodeID, int c) {
    return (long)(nodeID + 
            (((long)c) << 59));
}

