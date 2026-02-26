CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread -Iinclude

SRC = \
	src/main.cpp \
	src/server_facade.cpp \
	src/server_core.cpp \
	src/strategy/echo_strategy.cpp \
	src/strategy/time_strategy.cpp \
	src/strategy/upper_strategy.cpp

OUT = server

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)