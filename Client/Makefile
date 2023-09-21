
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

# For Windows, link against Ws2_32 library
ifeq ($(OS),Windows_NT)
	LDFLAGS := -lWs2_32
else
	LDFLAGS :=
endif

# Executable name
TARGET := client

# Source files
SRCS := client.cpp main.cpp process.cpp

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
