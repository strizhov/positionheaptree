/****************************
 * mylist.cpp:  trivial implementation of a list data type
 * **************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"

using std::cout;

//  Implements a stack that only supports push.  The implementation is an
//  array.  When the array fills up, it is reallocated to be twice is large.
//  See Cormen, Leiserson, Rivest, Stein, Introduction to Algorithms, Chapter
//  17 (Amortized Analysis) to see why this takes O(1) amortized time per
//  push, even though the reallocation step, which may require copying
//  all elements to a new location, takes Theta(n) time.
//
//  The idea is that the time for a reallocation takes time proportional
//  to the number of pushes since the last time it was reallocated.  The
//  O(n) time to reallocate can be charged at O(1) apiece to those pushes, 
//  which are never subsequently charge.

const int initSize = 4;
mylist::mylist()
{
    arrayPtr = new int[initSize];
    arraySize = initSize;
    currentIndex = -1;
}

mylist::~mylist()
{
    delete[] arrayPtr;
}

void mylist::add (int element)
{
    currentIndex++;
    if (currentIndex == arraySize)
	    memReAlloc();
    arrayPtr[currentIndex] = element;	
}

void mylist::memReAlloc ()
{
    int *newPtr = new int[2*arraySize];
    if (newPtr == NULL) { cout<<"Error (re)allocating memory in mylist"; exit(1); }
    int *p1 = arrayPtr;
    int *p2 = newPtr;
    for (int i = 0; i < arraySize; i++)
        *p2++ = *p1++;
    delete [] arrayPtr;
    arrayPtr = newPtr;
    arraySize *= 2;
}

/* Get rid of extra buffer space at end of the array  */
void mylist::compact()
{
    arraySize = currentIndex+1;
    int *newPtr = new int[arraySize];
    if (newPtr == NULL) { cout<<"Error (re)allocating memory in mylist"; exit(1); }
    int *p1 = arrayPtr;
    int *p2 = newPtr;
    for (int i = 0; i < arraySize; i++)
        *p2++ = *p1++;
    delete [] arrayPtr;
    arrayPtr = newPtr;
}

void mylist::print ()
{
   for (int i=0; i <= currentIndex; i++)
       cout << arrayPtr[i] << ' ';
   cout << '\n';
}
