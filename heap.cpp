/**********************************************/
// naiveBuildNaiveSearch directory
//   heap.cpp:  The heap class gives the simplest implementation of the
//      position heap, but not the fastest
/**********************************************/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "heap.h"     
#include "node.h"     // implements the nodes of the heap
#include "mylist.h"   // for returning a list of positions
#include "generic.h"  // various trivial operations

using std::cout;
using std::cin;
using std::endl;

/*
A trie is a rooted tree where each edge is labeled with a letter.  No
two edges with the same parent are labeled with the same letter.  

"Indexing into a trie on a string" consists of starting at the root and
finding the longest descending path whose edge labels spell out a prefix
of the string.  That is, at the first step, you descend on the edge
that is labeled with the first letter of the string, reaching a child
of the root.  At the second step, you descend from this child on an edge
labeled with the second letter of the string, reaching a grandchild.
At the third step, you repeat the operation from the grandchild, using
the third letter of the string, etc.

There are two ways that this sequence of operations can end.
   1.  At the i^{th} step, there is no edge to a child that is labeled
   with the i^{th} letter of the string.  Let's call this "falling off
   the tree."  The indexing path consists of the sequence of nodes up
   this point, and the edges on it spell a proper prefix of the string.

   2.  You reach the end of the string without falling off the tree.
   In this case, the indexing path spells the entire string.

Because no two edges to the children of a node have the same letter, the
indexing path is uniquely defined.

The position heap is a trie that has one node for each position in the text.

It is defined by induction on the length of a text; we can build it up by 
considering larger and larger suffixes of the text.  It is convenient to
think of the positions of the text as being numbered in descending order
from left to right, so that position 0 is the rightmost.

The position heap for the suffix of length 1 is a single root node corresponding
to position 0 in the text.  At step i, we have the position heap for the 
suffix T[i-1..0].  We modify it to be the position heap for the suffix
T[i..0].  We do this by indexing into it on T[i..0].  The height of the 
tree for T[i-1..0] is at most i-1, so we will fall off the tree 
when indexing into it on T[i..0].  

Suppose the indexing path has length k.  Then we have used up the first 
k letters of T[i..0] in indexing into the tree.  We got stuck on the (k+1)^{th}
letter.  So we extend the indexing path by one, by giving the last node
of the indexing path a new child on an edge labeled with the (k+1)^{th}
letter of T[i..0]  This node corresponds to position i of the text.

Since there is one node for each position in the text, we adopt the
following data structure, which avoids the need to have nodes point to their 
corresponding positions in the text.  We use an array of position heap nodes,
where the node in position i implicitly points to position i of the
text.  We also don't have to label the edges explicitly with letters from 
the alphabet.  If the child refers to position p in the text and has 
depth d, then it is reachable from the parent on the character d-1 
positions to the right of p in the text.

Therefore, the trie requires space for two integers per node, for a total
space requirement of 2n integers.

Once this is constructed, how is the structure useful for searching
for the places where a pattern string occurs as a substring of the text?
Index into the position heap using the pattern.  Each position of the text 
corresponding to a node on the path is a potential match.  Which are occurrences
is determined by just matching the pattern to the text at these
positions.  If we fall off the tree, these are the only occurrences.  
Otherwise, the positions corresponding to all descendants of the last node 
on the path are also occurrences of the pattern, and there are no other
positions where the pattern occurs.    
*/


/****************************************
position heap constructor.  Builds the position heap
for the text pointed to by 'str' ...
****************************************/
heap::heap(char *str)
{
    textLength = strlen (str);    // length of text
    nodeArray = new node[textLength]; // private array of heap nodes; there's
                                      // one for each position in the text
    //
    // Private version of text.  To keep storage cost down to two integers
    //  per character of text, you should use the text pointed to by *str,
    //  rather than keeping a private copy of the text.  For clarity, we keep
    //  a private copy.
    text = new char[textLength];   
    if (! nodeArray || !text) 
         {cout << "Memory allocation failure in heap constructor\n"; exit(1);}
    
    char *p1 = str;  char *p2 = text + textLength - 1;

    // Reverse indexing order to descend from left to right, with index 0
    //   being the rightmost, rather than the leftmost position.
    while (*p1 != '\0') 
        *p2-- = *p1++; 
    build();                       
}

// position heap destructor ...
heap::~heap()
{
    delete []nodeArray; 
    delete []text;
}

/*******************************************
build:  Build the position heap.  Assume text has been reversed in its 
array so that the indices are in ascending order from right to left.
*******************************************/
void heap::build ()
{

    for (int arrayIndex = 1; arrayIndex < textLength; arrayIndex++)
    {
        if ((arrayIndex % 100000) == 0)  // for monitoring progress, large texts
               cout << "Text position: " << arrayIndex << '\n';
        int pathNode, child;  // a parent and its child on the indexing path 
        int depth;            // depth of pathNode
        char *textptr = text + arrayIndex; // Next character on indexing path
        
        /* index into the heap on text[arrayIndex...0] ... */
        depth = child = 0;
        do
        {
            pathNode = child;
            child = childOnLetter(pathNode, depth++, *textptr--);
        } while (child != NOCHILD);  // (while we haven't fallen off the tree)

        //  pathNode is where we fell off the tree.  Add a new node for position
        //   arrayIndex
        insertChild(arrayIndex, pathNode);  
    }
}

/**************************************
insertChild:  insert 'child' as a child of 'parent' 
**************************************/
void heap::insertChild(int child, int parent)
{
    nodeArray[child].setSibling(nodeArray[parent].getChild());
    nodeArray[parent].setChild(child);
}

/*************************************/
//  search:  Find all occurrences of 'pattern' in the text from which the
//  position heap was constructed.  Return an array containing the indices
//  of the positions (numbered right to left) and store the number of 
//  positions in 'numOccurrences.  This array must be deallocated when
//  when it is no longer used to avoid a memory leak.
/*************************************/
mylist *heap::search(char *pattern, int patternLength)
{
   int lastNode;  // NOCHILD if we fall of the tree while indexing on pattern.
                  // Otherwise, it's the last node of indexing path.

   //  observe convention of making indices descend from left to right
   reverse (pattern, patternLength); 

   // Find all positions on indexing path that are occurrences of pattern ...
   mylist *Occurrences = pathOccurrences(pattern, patternLength, lastNode);

   // If pattern was exhausted during indexing, append positions in subtree
   //  rooted at lastNode ...
   if (lastNode != NOCHILD) appendSubtreeOccurrences(lastNode, Occurrences);

   // un-reverse the user's pattern string to leave it in its original state
   reverse (pattern, patternLength); 
   
   // Allocate an integer array and copy the positions to it ...
   Occurrences->compact();
   return Occurrences;
}

/**********************
// childOnLetter:  Find the child reachable from 'node' on character c; 
//  'nodeDepth' is the depth of 'node'
***********************/
int heap::childOnLetter(int node, int nodeDepth, char c)
{
   int child = nodeArray[node].getChild();
   while (child != NOCHILD && text[child - nodeDepth] != c)
      child = nodeArray[child].getSibling();
   return child;
}

/***********************
//  Index into heap on pattern, returning a list of all positions corresponding
//  to nodes on the path where the pattern occurs.
//
//  The method allocates the list of positions; this must be deleted
//  when it is no longer needed.
*************************/
mylist *heap::pathOccurrences(char *pattern, int patternLength, int &lastNode)
{
    int pathNode, child;  // parent and child on indexing path
    int depth;            // depth of pathNode
    // Indexing path has at most 'patternLength' positions on it
    mylist *Occurrences = new mylist(); 
    if (! Occurrences) {cout << "Memory allocation failure in pathOccurrences\n"; exit(1);}
    
    child = depth = 0;
    char *patPtr = pattern + patternLength - 1;  // start at left end of pattern
    do
    {
        pathNode = child;
        // the first depth characters of the pattern are known to match at
        // this node's position, because these are spelled out by the edges to
        // the node.  Check the last patternLength - depth characters to see 
        // if they also match.  If so, report the position as a match.
        if (isOccurrence(pattern, 
                         patternLength - depth, 
                         pathNode - patternLength+1))
            Occurrences->add(pathNode);
        child = childOnLetter(pathNode, depth++, *patPtr--);
    } while (child != NOCHILD && depth < patternLength);
    lastNode = child;
    return Occurrences;
}

/***********************
isOccurrence:  Tell whether reverse of a pattern string matches at 
*ending position* 'pos', given a pointer to the rightmost character 
(the one with index 0) in the pattern string.  That is, tell whether
the pattern and the text match when the pattern is lined up with its
rightmost position at 'pos'.   
************************/
bool heap::isOccurrence(char *pattern, int patternLength, int pos)
{
   if (pos < 0) return false;
   char *textptr = text + pos;
   
   for (int i = 0; i < patternLength; i++)
      if (*pattern++ != *textptr++) 
          return false;
   return true;
}

/***************************
appendSubtreeOccurrences:  If you didn't fall off the tree while indexing 
in on the pattern string, then all positions corresponding to descendants of 
the last node on the indexing path are also occurrences of the pattern.
Append them to the list of places where the pattern string occurs.
****************************/
void heap::appendSubtreeOccurrences(int node, mylist *Occurrences)
{
    Occurrences->add(node);   // append root of subtree
    for (int child = nodeArray[node].getChild();  //recursively append
            child != NOCHILD;                          //  descendants
            child = nodeArray[child].getSibling())
       appendSubtreeOccurrences (child, Occurrences);
}


/*****************************
preorderPrint:  Display the shape of the heap tree using indented preorder ...
*******************************/
void heap::preorderPrint()
{
    preorderAux(0,0);
}

void heap::preorderAux (int index, int depth)
{
    if (index == -1) return;
    else
    {
       for (int i = 0; i < depth; i++)
          cout << ' ';
       cout << "Node " << index << "  Depth " << depth;
       cout << "  Children: ";
       for (int child = nodeArray[index].getChild(); 
                child != NOCHILD; 
                child = nodeArray[child].getSibling())
             cout << '(' << text[child-depth] << ',' << child << ')';
       cout << '\n';
       for (int child = nodeArray[index].getChild(); 
                child != NOCHILD; 
                child = nodeArray[child].getSibling())
            preorderAux(child, depth+1);
    }
}

