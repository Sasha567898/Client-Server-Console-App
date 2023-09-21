
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS := -L\C:\MinGW\lib -lws2_32  # Link against ws2_32 library

# Executable name
TARGET := server

# Source files
SRCS := server.cpp main.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
