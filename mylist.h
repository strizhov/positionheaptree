/******************
/* mylist.h:  see mylist.cpp
/*****************/
class mylist
{
    public:
	mylist ();
        mylist (int initialSize);
	~mylist ();
	void add (int element);
        int size();              // number of elements in array
	void print();
        void compact();
    private:
	int* arrayPtr;     // array for storing elements of array
	int arraySize;     // currently allocated size of array
	int currentIndex;  // index of most recently added element
	void memReAlloc ();  // reallocates array to be twice as large
};
