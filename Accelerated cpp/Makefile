CXXFLAGS=-g -std=c++14

OBJS = main.o map.o Node.o


TARGET = astar.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TESTTARGET) $(MAIN_GAME_OBJ) $(TESTOBJS) $(TARGET)
