SHELL := /bin/bash

TARGET = program
TEST_TARGET = test_program

CXX = g++

SOURCES = src/main.cpp \
		  src/UniqueWordCounter.cpp

TEST_SOURCES = test/UniqueWordCounterTest.cpp \
			   test/IntegrationTest.cpp 

HEADERS = include/UniqueWordCounter.h

CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pthread -I include

GTEST_LIBS = -lgtest -lgtest_main -pthread

OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)

runTest: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS) $(GTEST_LIBS)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJECTS) $(TEST_OBJECTS)

.PHONY: all clean test
