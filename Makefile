CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = game.exe

SRCS = main.cpp Player.cpp Enemy.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
