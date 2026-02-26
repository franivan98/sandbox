CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -pthread
SRC = src/main.cpp src/server_core.cpp src/server_facade.cpp 
OUT = server

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)