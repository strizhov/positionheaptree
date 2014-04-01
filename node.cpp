/***************************
node.cpp:  implements a node in a downwardly-directed rooted tree, that is,
a tree where each node has a list of its children.  Each node has a pointer
to its leftmost child and a pointer to its sibling to the right.  The
children of a node are accessed by taking the left-child pointer and
then following right-sibling pointers.
**************************/
#include <iostream>
#include "node.h"
using std::cout;


node::node()
{
   child = -1;
   sibling = -1;
}

int node::getChild ()
{
   return child;
}

int node::getSibling ()
{
   return sibling;
}

void node::setChild (int c)
{
   child = c;
}

void node::setSibling (int s)
{
   sibling = s;
}

