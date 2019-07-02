#################################################################
#  Makefile                                                     #
#                                                               #
#  Author: Abhinav Dutt                                         #
#                                                               #
#  make [clean]                                                 #
#                                                               #
#  Important: Run make from the directory containing this file. #
#                                                               #
#################################################################

output: ThreadPool.o main.o
	g++ ThreadPool.o main.o -o output
	
main.o: main.cpp
	g++ -c main.cpp
	
ThreadPool.o: ThreadPool.cpp ThreadPool.hpp
	g++ -c ThreadPool.cpp
	
clean:
	rm -rf *.o output




