/**********************************************
 * heap.h

There are three private variables:  text, textLength, and nodeArr-ay.
See comments in heap.cpp for an explanation of the problem solved by each
method.
 **********************************************/
#include <math.h>
// Objects to represent the nodes of the position heap's tree.
class node;
class mylist;
const int NOCHILD = -1;  // flag indicating a node has no child
class heap
{
    public:
        heap (char *str);
        ~heap();
        void build();
        mylist *search(char *pattern, int patternLength);
        void preorderPrint();
    private:
	node *nodeArray;   // array of position heap nodes
	char *text;        // private copy of text
	int textLength;    // length of text
        int childOnLetter(int node, int depth, char c);
        void insertChild(int parent, int child);
        mylist *pathOccurrences(char *pattern, int patternLength, int &lastNode);
        void appendSubtreeOccurrences(int node, mylist *Occurrences);
        bool isOccurrence(char *pattern, int patLength, int pos);
        void preorderAux (int index, int depth);
};
