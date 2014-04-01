/*****************************
 * file.cpp:  reads a file containing a text into an array
 * ****************************/
#include <fstream>
#include <iostream>
using namespace std;
#include <stdlib.h>

// Reads the contents of a file into a null-terminated character array,
//   which the function allocates.  The user is responsible for deallocating
//   the array ...
char *fileRead(char *filename)
{
    ifstream inStream;

    // LOOK UP HOW TO QUERY A FILE ABOUT ITS LENGTH ...
    //  (In the meantime, here's a hack to do it.)
    inStream.open (filename);
    if (inStream.fail())
    {
        cout << "Attempt to open " << filename << " failed.\n";
        exit(1);
    }
    int length;
    char chr;
    for (length = 0; !inStream.eof(); length++)
    {
        inStream.get(chr);
	if (chr == '\n')
	   length--;
    }

    //length -= 2;
    cout << "That file has length: " << length << endl;
    inStream.close();

    inStream.open (filename);
    if (inStream.fail())
    {
        cout << "Attempt to open " << filename << " failed.\n";
        exit(1);
    }
    char *text = new char [length+1];
    if (!text)
    {
        cout << "Memory allocation failure:  character array to hold data\n";
        cout << "from input file.\n";
        exit (1);
    }
    for (int i = 0; i < length; i++)
    {
        inStream.get(chr);	
        if (chr!='\n')
	{
	    text[i] = chr;
	}
	else 
	    i--;

    }
    //text[length] = '\0';

    return text;
}
