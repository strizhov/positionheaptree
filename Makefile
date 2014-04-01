.SUFFIXES:
.SUFFIXES:                      .o .cpp

.cpp.o:
	g++ -c $*.cpp

driver: driver.o node.o heap.o file.o mylist.o generic.o
	g++ driver.o node.o heap.o file.o mylist.o generic.o -o driver
