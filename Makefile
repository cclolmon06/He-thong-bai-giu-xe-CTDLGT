CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Liệt kê tất cả các file .cpp của cả 5 thành viên tại đây
SRCS = main.cpp MenuUI.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = parking_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp MenuUI.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe