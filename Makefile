# コンパイラとフラグ
CXX = g++
CXXFLAGS = -Wall -g -std=c++20

# ターゲットとソースファイル
TARGET = main.out

.PHONY: build
build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp