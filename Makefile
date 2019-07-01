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

# Executable name and location
BINDIR = output
TARGET = threadpool

# Compiler
CXX = g++

CXXFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -std=c++11 -lstdc++ -L/usr/local/lib -I/usr/include/boost
LDFLAGS = -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread 

OBJECTDIR = obj

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)

OBJECTS = $(OBJECTDIR)/ThreadPool.o

$(TARGET): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINDIR)/$(TARGET)