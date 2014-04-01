/****************************
 * generic.cpp:  miscellaneous trivial operations
 * ***************************/

#include "generic.h"

//  Reverse the string so it's indexed from the end instead of from
//   the beginning; there must be some library method available to reverse
//   a string.
void reverse (char *string, int stringLength)
{
    char temp;
    for (int i = 0; i < stringLength/2; i++)
    {
       temp = string[i];
       string[i] = string[stringLength-i-1];
       string[stringLength-i-1] = temp;
    }
    string[stringLength] = '\0';
}

