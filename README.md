Implementation of Ukkonen's algorithm in C++.
This is very special algorithm. You may have a look at the original paper here:
http://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf.


Do |make| to compile and |make run| to execute.

Output would be all the edges in the suffix tree with the following details:
StartNode Endnode SuffixlinkOfEndNode StartLabelIndex EndLabelIndex String

where:
StartLabelIndex is the starting index of the substring represented by this edge.
EndLabelIndex is the ending index of the substring represented by this edge.
String is the full label represented by this edge.

Alphabet set is unlimited. 

To get the explicit suffix tree you have to end the input string with a unique
character.

You can do a search in the suffix tree for any substring.

This is a linear time algorithm.

Only significant data structure that I've used is a hash table. This
implementaion of hash table has an constant average case complexity.

