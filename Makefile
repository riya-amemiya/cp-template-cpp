CXX = g++-15
CXXFLAGS = -Wall -g -std=c++23

TARGET = main.out
TEST_DIR = ./tests

.PHONY: build
build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

.PHONY: test
test:
	oj t -c "./$(TARGET)" -d $(TEST_DIR)