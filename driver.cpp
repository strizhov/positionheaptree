/****************************
 * driver.cpp:  menu for experimenting with the algorithms in heap.cpp
 * ***************************/
#include <iostream>
#include <time.h>
#include <string.h>
using namespace std;
#include "mylist.h"
#include "heap.h"
#include "file.h"
#include "generic.h"

int main ()
{
   char filename [256];
   char typeInput[256];
   int choice = 1;
   clock_t time1, time2;

   heap *H = NULL;
   while (choice != 0) {
      cout<<"\n----------------------------------------------\n";
      cout<<"0. Quit\n";
      cout<<"1. Create Position Heap with typed text\n";
      cout<<"2. Import a text from a file\n";
      cout<<"3. Find positions of a pattern string, indexed from right to left\n";
      cout<<"4. Print shape of heap in indented preorder\n";
      cout<<"----------------------------------------------\n";
      cout<<"Select : ";

      cin >> choice;
      if (choice == 1)
      {
	  cout << " Type your text : ";
	  cin>>typeInput;
          int inputLength = strlen(typeInput);
   	  cout << "\n\nBuilding position heap ...\n\n";
          if (H) delete H;
	  H = new heap (typeInput);
          if (!H) {cout << "Memory allocation failure on heap H\n"; exit(1);}
      }

      else if (choice == 2)
      {
	  cout << "Enter the name of the input file : ";
	  cin >> filename;
	  cout << "\n\nReading input file ...\n";
	  char *text = fileRead (filename);
   	  cout << "\nBuilding position heap ...\n\n";
          if (H) delete H;
	  H = new heap(text);
          if (!H) {cout << "Memory allocation failure on heap H\n"; exit(1);}
      }
      else if (choice == 3)
      {
          char pattern[256];
	  cout<<"Enter the pattern string : ";
	  cin>>pattern;
          int patternLength = strlen(pattern);
          mylist *Occurrences = H->search(pattern, patternLength);
          cout << "\npositions: "; Occurrences->print();
          delete  Occurrences;  // this is allocated inside search, so it
                                // must be deallocated when no longer needed
      }
      else if (choice == 4) 
      {
	  H->preorderPrint();
      }
   }
   return 0;
}
