#CXXFLAGS =	-O2 -g -Wall -fmessage-length=0   -I /Users/wing/Sdk/eigen
CXXFLAGS =	-I /Users/wing/Sdk/eigen 

OBJS =		MyLdpc.o 

TEST_OBJS = MyTest.o MyLdpc.o 

LIBS = 

TARGET =	MyLdpc

TEST =  MyTest

all:	$(TEST)

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

$(TEST): $(TEST_OBJS)
	$(CXX)  -o $(TEST)  $(TEST_OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET) $(TEST) $(TEST_OBJS)
