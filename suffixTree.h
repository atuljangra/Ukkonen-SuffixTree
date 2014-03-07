#include "node.h"
#include "edge.h"
#include <iostream>

class suffixTree {
    public:
        int rootNode;   // Origin of the suffix tree
        int startIndex; // Starting index of the string represented.
        int endIndex;   // End index of the string represented.

        // Constructor
        suffixTree() :
            rootNode(0), 
            startIndex(-1),
            endIndex(-1){};
        suffixTree(int root, int start, int end) :
            rootNode(root),
            startIndex(start),
            endIndex(end) {};
};

                                                                             
